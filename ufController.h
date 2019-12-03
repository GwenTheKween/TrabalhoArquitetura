#ifndef UF_CONTROLLER_H
#define UF_CONTROLLER_H

#include <vector>
#include <string>
#include <exception>
#include "instructionStruct.h"
#include "regResultController.h"
#include "tableManager.h"

struct ufLine{
	std::string ufName, opName, fi, fj, fk, qj, next_qj, qk, next_qk;
	int rj, next_rj, rk, next_rk, instructionId, execCyclesLeft;
	bool busy, next_busy;
};

/*
inteiro 1
soma/subtracao float 2
mult float 10
div float 40
*/

class UfController{

private:
	std::vector<ufLine> ufsInt;
	std::vector<ufLine> ufsFloat;
	std::unordered_map<std::string, int> nCyclesFloating = {{"Add", 2}, {"Sub", 2}, {"Mul", 10}, {"Div", 40}};
	tableManager<std::string> gui;
	//talvez manter um vetor pras livres e um pras ocupadas? 
public:
	UfController(tableManager<std::string> tm);
	
	//checks if a compatible fu is available
	ufLine hasUfAvailable(bool needsFloatingPointUf);

	//returns ufName - the name of the chosen fu
	void populateUf(ufLine& uf, instruction& dispatchedInstruction, RegResController& regRes);
	
	//returns false if operands not ready otherwise returns true
	bool readOperands(int instructionId, std::string opName); //alguma ideia melhor? precisa identificar a instrucao de alguma forma... talvez so id
	
	//returns false if UF not yet done otherwise returns true
	bool runExecution(int instructionId, std::string opName);

	//returns false if the informed register is still waiting to be read by a UF otherwise returns true
	bool isWriteAvailable(int instructionId, std::string opName);
	
	//returns the name of the destination register	
	std::string getDestReg(int instructionId, std::string opName);

	//will change UF's status and update UFs waiting to read register
	void clearAndUpdateUf(int instructionId, std::string opName, std::string regName);
	
	//will update attributes with the values modified in the last clock cicle
	void performClockTick();
};
#endif
