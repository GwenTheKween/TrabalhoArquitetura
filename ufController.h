#ifndef UF_CONTROLLER_H
#define UF_CONTROLLER_H

#include <vector>
#include <string>
#include <exception>
#include "instructionStruct.h"
#include "regResultController.h"
#include "tableManager.h"

struct ufLine{
	std::string ufName, opName, fi, fj, fk, qj, qk, qj_next, qk_next;
	int rj, rk, rj_next, rk_next, instructionId, execCyclesLeft;
	bool busy;
};

class UfController{

private:
	// Integer UFs
	std::vector<ufLine> ufsInt;
	// Floating point UFs
	std::vector<ufLine> ufsFloat;
	// Number of cycles for each floating point instruction
	std::unordered_map<std::string, int> nCyclesFloating;
	// Name of cleared fu's to update again while performing clock cycle
	std::vector<std::string> ufsCleared;
	tableManager<std::string> gui;

public:
	UfController(tableManager<std::string> tm);

	~UfController();
	
	//checks if a compatible fu is available
	ufLine* hasUfAvailable(bool needsFloatingPointUf);

	//returns ufName - the name of the chosen fu
	void populateUf(ufLine* uf,const instruction& dispatchedInstruction,RegResController* regRes);
	
	//returns false if operands not ready otherwise returns true
	bool readOperands(ufLine* uf); //alguma ideia melhor? precisa identificar a instrucao de alguma forma... talvez so id
	
	//returns false if UF not yet done otherwise returns true
	bool runExecution(ufLine* uf);

	//returns false if the informed register is still waiting to be read by a UF otherwise returns true
	bool isWriteAvailable(ufLine* FU);
	
	//returns the name of the destination register	
	std::string getDestReg(int instructionId);

	//will change UF's status and update UFs waiting to read register
	void clearAndUpdateUf(ufLine* uf);
	
	//will update attributes with the values modified in the last clock cicle
	void performClockTick();
};
#endif
