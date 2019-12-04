#include "pipelineController.h"
#include <iostream>
using namespace std;

#define DISPATCH 0
#define READ_OP 1
#define EXECUTION 2
#define WRITE_BACK 3
#define DONE 4

//============================================================================
//auxiliary function
vector<string> stages_to_vector(int stages[4]){
	vector<string> v;
	stringstream ss;
	for(int i = 0; i < 4; i++){
		if(stages[i] >= 0){
			ss << stages[i];
		}
		v.push_back(ss.str());
		ss.str("");
	}
	return v;
}
//============================================================================

PipelineController::PipelineController(tableManager<std::string>& tm):
	gui(tm){
	}

//adds new line of intruction
void PipelineController::dispatchInstruction(int instructionId, std::string opName, bool floating, ufLine* uf, int clockCycle){
	
	//copying values to new entry
	pipeLine newInstruction;
	newInstruction.instructionId = instructionId;
	newInstruction.opName = opName;
	newInstruction.currStage = DISPATCH;
	newInstruction.stage[DISPATCH] = clockCycle;
	newInstruction.stage[READ_OP] = -1;
	newInstruction.stage[EXECUTION] = -1;
	newInstruction.stage[WRITE_BACK] = -1;
	newInstruction.finishedExec = false;
	newInstruction.floating = floating;
	newInstruction.UF = uf;

	this->instructions.push_back(newInstruction);

	//updates the screen
	addTableEntry(instructionId);
}

//calls ufController method to read operands. Updates pipeline and window?
bool PipelineController::tryToReadOperands(UfController& ufCon, pipeLine &line, int clockCycle){
	//will ask the FU to read the operands and then will update the pipeline
	if (ufCon.readOperands(line.UF)){
		line.stage[READ_OP] = clockCycle;
		line.currStage = READ_OP;
		return true;
	}
	return false;
}

//calls ufController method to run execution. Updates pipeline and window
bool PipelineController::runExecution(UfController& ufCon, pipeLine &line, int clockCycle){
	
	//first time running execution
	if( line.currStage == READ_OP ){
		line.stage[EXECUTION] = clockCycle;
		line.currStage = EXECUTION;
		if (ufCon.runExecution(line.UF)){
			//then execution finished in only 1 cycle
			line.finishedExec = true;
			line.execEnd = clockCycle;
		}
		return true;
	}
	
	//will perform 1 cycle of execution	
	if (ufCon.runExecution(line.UF)){
		//then execution finished
		line.finishedExec = true;
		line.execEnd = clockCycle;
	}
	return false;
}

//calls ufController method to check if write is available and if it is
//will update pipeline and call register result method to update register 
bool PipelineController::tryToWriteResult(RegResController& regCon, UfController& ufCon, pipeLine &line, int clockCycle){
	if(ufCon.isWriteAvailable(line.UF)){
		//find register name
		string regName = ufCon.getDestReg(line.instructionId);
		//clear FU and register status
		ufCon.clearAndUpdateUf(line.UF);
		regCon.clearReg(regName);
		
		//terminating instruction
		line.stage[WRITE_BACK] = clockCycle;
		line.currStage = WRITE_BACK;	//used to check if instruction has ended

		return true;
	}
	return false;
}

//will try to perform next stage for every instruction in the pipeline
void PipelineController::performClockCycle(UfController& ufCon, RegResController& regCon, int clockCycle){
	for (size_t i=0; i < instructions.size(); i++){
		bool changed = false;
		switch(instructions[i].currStage){
			case DISPATCH:	//then will try to read
				changed = tryToReadOperands(ufCon, instructions[i], clockCycle);
				break;

			case READ_OP:  //then will start execution
				changed = runExecution(ufCon, instructions[i], clockCycle);
				break;

			case EXECUTION:  //then will continue execution or try to write
				
				if(!instructions[i].finishedExec){
					changed = runExecution(ufCon, instructions[i], clockCycle);
				}
				else
					changed = tryToWriteResult(regCon, ufCon, instructions[i], clockCycle); 
				break;
			case WRITE_BACK:
				instructions[i].currStage = DONE;
				removeTableEntry(instructions[i].instructionId);
				break;
		}
		if(changed){
			updateTable(instructions[i].instructionId);
		}
	}
}

int PipelineController::findInstByID(int instructionId){
	for(size_t i = 0; i<instructions.size(); i++){
		if(instructions[i].instructionId == instructionId)
			return i;
	}
	return -1;
}

void PipelineController::updateTable(int instructionId){
	//only updates information on the given line
	int line = getInstrLine(instructionId);
	int index = findInstByID(instructionId);
	stringstream ss;
	ss << instructionLine[line];
	sm.mvprint_to_panel(-1,30,0,"updated line %d for instr with index %d",line, index);
	gui.update_line(line, ss.str(), stages_to_vector(instructions[index].stage));
}

void PipelineController::removeTableEntry(int instructionId){
	int line = getInstrLine(instructionId);
	instructionLine.erase(instructionLine.begin() + line);
	int mx = (instructionLine.size() < MAX_PIPELINE_TABLE_SIZE) ? 
			  instructionLine.size() : 
			  MAX_PIPELINE_TABLE_SIZE;
	for(;line < mx; line++){
		updateTable(instructionLine[line]);//updates the following lines to go up
	}
	if(line < MAX_PIPELINE_TABLE_SIZE){
		vector<string> v(4," ");
		gui.update_line(line, " ", v);
	}
}

void PipelineController::addTableEntry(int instructionId){
	instructionLine.push_back(instructionId);
	if(instructionLine.size() < MAX_PIPELINE_TABLE_SIZE){
		updateTable(instructionId);
	}
	return;
}

int PipelineController::getInstrLine(int instructionId){
	for(size_t index=0;index<instructionLine.size();index++){
		if(instructionLine[index] == instructionId)
			return index;
	}
	return -1;
}
