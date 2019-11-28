#ifndef DISPATCH_CONTROLLER_H
#define DISPATCH_CONTROLLER_H

#include "screenManager.h"
#include "ufController.h"
#include "pipelineController.h"
#include "regResultController.h"
#include <stdio.h>
#include <queue>
#include <string>

class DispatchController{
	
private:
	std::queue <std::string> instructions;
	int intructionCounter;
	UfController ufCon;		
	PipelineController pipe; 	
	RegResController regCon;	
public:
	DispatchController();
	
	//will read instructions from file
	void loadInstructions();
	
	//will return -1 if dispatch fails or the instructionId
	int tryToDispatchNext();
};

#endif
