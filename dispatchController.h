#ifndef DISPATCH_CONTROLLER_H
#define DISPATCH_CONTROLLER_H

#include <stdio.h>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

struct instruction{
	std::string opName, rs, rt, rd;
	int immed, id;
	bool useFp, isRtype;
};

#include "screenManager.h"
#include "regResultController.h"
#include "pipelineController.h"
#include "ufController.h"


class DispatchController{
	
private:
	std::queue <instruction> instructionQueue;
	UfController ufCon;		
	PipelineController pipe; 	
	RegResController regCon;
	std::vector< std::unordered_map<std::string, std::string> > createInstructionsMaps();
public:
	DispatchController();
	
	//will read instructions from file
	void loadInstructions();
	
	//will return -1 if dispatch fails or the instructionId
	int tryToDispatchNext(UfController ufCon, PipelineController pipe, RegResController regCon, int clockCycle);
};

#endif
