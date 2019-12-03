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
} pipeLine; 	//line of the pipeline :)

class PipelineController{

private:
	//uma quantidade desconhecida de instrucoes, mas cada entrada tem despacho, leitura, exec e escrita
	//pensei em armazenar uma struct para cada linha, alguma ideia mais pratica?
	std::vector <pipeLine> instructions;

	tableManager<std::string> gui;

public:
	PipelineController(tableManager<std::string>& tm);

	//adds new line of intruction
	void dispatchInstruction(int instructionId, std::string opName, int clockCycle);

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
