#ifndef PIPELINE_CONTROLLER_H
#define PIPELINE_CONTROLLER_H

#define MAX_PIPELINE_TABLE_SIZE 5

#include <vector>
#include <string>
#include "ufController.h"
#include "regResultController.h"
#include "tableManager.h"
#include "instructionStruct.h"

extern bool instructionsExecuting;

typedef struct{
	int instructionId;
	std::string opName;
	int stage[4];	//clock cicle for each stage(dispatch, readOp, exec, writeBack)
	int currStage, execEnd;
	bool finishedExec;
	bool floating; //if the instruction uses floating point FUs
	ufLine *UF; //which UF is executing the current instruction
} pipeLine; 	//line of the pipeline :)

class PipelineController{

private:
	std::vector <pipeLine> instructions;

	tableManager<std::string> gui;
	//this vector stores the fact that the instruction whose ID is "instructionLine[index]" 
	//is printed in the line "index" of the table
	std::vector<instruction> instructionLine;

public:
	PipelineController(tableManager<std::string>& tm);

	//adds new line of intruction
	void dispatchInstruction(const instruction& inst, ufLine* uf, int clockCycle);

	//calls ufController method to read operands. Updates pipeline and window?
	bool tryToReadOperands(UfController& ufCon, pipeLine &line, int clockCycle);

	//calls ufController method to run execution. Updates pipeline and window
	bool runExecution(UfController& ufCon, pipeLine &line, int clockCycle);
	
	//calls ufController method to check if write is available and if it is
	//will update pipeline and call register result method to update register 
	bool tryToWriteResult(RegResController& regCon, UfController& ufCon, pipeLine &line, int clockCycle);

	//will try to perform next stage for every instruction in the pipeline
	void performClockCycle(UfController& ufCon, RegResController& regCon, int clockCycle);

	//finds the index of an instruction, given it's ID
	int findInstByID(int instructionId);

	//update the table, either a single line, or the whole table
	void updateTable(int instructionId);

	//adds or removes a line on the table
	void removeTableEntry(int instructionId);
	void addTableEntry(const instruction& inst);

	//finds which line the instruction is being printed
	int getInstrLine(int instructionId);
};


#endif
