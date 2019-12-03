#ifndef PIPELINE_CONTROLLER_H
#define PIPELINE_CONTROLLER_H

#include <vector>
#include <string>
#include "ufController.h"
#include "regResultController.h"
#include "tableManager.h"

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

public:
	PipelineController(tableManager<std::string>& tm);

	//adds new line of intruction
	void dispatchInstruction(int instructionId, std::string opName, bool floating, ufLine* uf, int clockCycle);

	//calls ufController method to read operands. Updates pipeline and window?
	bool tryToReadOperands(UfController& ufCon, pipeLine &line, int clockCycle);

	//calls ufController method to run execution. Updates pipeline and window
	bool runExecution(UfController& ufCon, pipeLine &line, int clockCycle);
	
	//calls ufController method to check if write is available and if it is
	//will update pipeline and call register result method to update register 
	bool tryToWriteResult(RegResController& regCon, UfController& ufCon, pipeLine &line, int clockCycle);

	//will try to perform next stage for every instruction in the pipeline
	void performClockCycle(UfController& ufCon, RegResController& regCon, int clockCycle);
};


#endif
