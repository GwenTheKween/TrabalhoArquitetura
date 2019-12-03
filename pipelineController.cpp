#include "pipelineController.h"

using namespace std;

#define DISPATCH 0
#define READ_OP 1
#define EXECUTION 2
#define WRITE_BACK 3

PipelineController::PipelineController(tableManager<std::string>& tm):
	gui(tm){}

//adds new line of intruction
void PipelineController::dispatchInstruction(int instructionId, std::string opName, int clockCycle){
	
	//copying values to new entry
	pipeLine newInstruction;
	newInstruction.instructionId = instructionId;
	newInstruction.opName = opName;
	newInstruction.currStage = DISPATCH;
	newInstruction.stage[DISPATCH] = clockCycle;
	newInstruction.finishedExec = false;

	this->instructions.push_back(newInstruction);
}

//calls ufController method to read operands. Updates pipeline and window?
void PipelineController::tryToReadOperands(UfController& ufCon, pipeLine &line, int clockCycle){
	//will ask the FU to read the operands and then will update the pipeline
	if (ufCon.readOperands(line.instructionId, line.opName)){
		line.stage[READ_OP] = clockCycle;
		line.currStage = READ_OP;
	}
}

//calls ufController method to run execution. Updates pipeline and window
void PipelineController::runExecution(UfController& ufCon, pipeLine &line, int clockCycle){
	
	//first time running execution
	if( line.currStage == READ_OP ){
		line.stage[EXECUTION] = clockCycle;
		line.currStage = EXECUTION;
	}
	
	//will perform 1 cycle of execution	
	if (ufCon.runExecution(line.instructionId, line.opName)){
		//then execution finished
		line.finishedExec = true;
		line.execEnd = clockCycle;
	}
}

//calls ufController method to check if write is available and if it is
//will update pipeline and call register result method to update register 
void PipelineController::tryToWriteResult(RegResController& regCon, UfController& ufCon, pipeLine &line, int clockCycle){
	
	if(ufCon.isWriteAvailable(line.instructionId, line.opName)){
		//find register name
		string regName = ufCon.getDestReg(line.instructionId, line.opName);
		//clear FU and register status
		ufCon.clearAndUpdateUf(line.instructionId, line.opName, regName);
		regCon.clearReg(regName);
		
		//terminating instruction
		line.stage[WRITE_BACK] = clockCycle;
		line.currStage = WRITE_BACK;	//used to check if instruction has ended
	}
}

//will try to perform next stage for every instruction in the pipeline
void PipelineController::performClockCycle(UfController& ufCon, RegResController& regCon, int clockCycle){
	
	for (int i=0; i<this->instructions.size(); i++){
		
		switch(instructions[i].currStage){
			case DISPATCH:	//then will try to read
				tryToReadOperands(ufCon, instructions[i], clockCycle);
				break;

			case READ_OP:  //then will star execution
				runExecution(ufCon, instructions[i], clockCycle);
				break;

			case EXECUTION:  //then will continue execution or try to write
				if(!instructions[i].finishedExec){
					runExecution(ufCon, instructions[i], clockCycle);
				}
				else
					tryToWriteResult(regCon, ufCon, instructions[i], clockCycle); 
				break;
			case WRITE_BACK:
				//finished instruction TODO: remove from vector? remove from screen?
				break;
		}
	}
}
