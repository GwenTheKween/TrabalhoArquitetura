#include <iostream>
#include <exception>
#include "ufController.h"

using namespace std;


UfController::UfController(){
	ufLine int1;
	int1.opName = "Int1";
	int1.busy = false;
	ufsInt.push_back(int1);

	ufLine int2;
	int1.opName = "Int2";
	int1.busy = false;
	ufsInt.push_back(int2);

	ufLine float1;
	int1.opName = "Float1";
	int1.busy = false;
	ufsFloat.push_back(float1);

	ufLine float2;
	int1.opName = "Float2";
	int1.busy = false;
	ufsFloat.push_back(float2);
}

//checks if a compatible fu is available and return it
ufLine UfController::hasUfAvailable(bool needsFloatingPointUf){
	std::vector<ufLine>& ufs = needsFloatingPointUf ? ufsFloat : ufsInt;
	for(auto uf : ufs)
	{
		if(!uf.busy)
			return uf;
	}
	throw logic_error("Nenhuma unidade funcional disponivel");
}

void UfController::populateUf(ufLine& uf, instruction& dispatchedInstruction, RegResController& regRes){
	uf.instructionId = dispatchedInstruction.id;
	if(uf.opName.substr(0, 3) == "Int")
		uf.execCyclesLeft = nCyclesFloating[dispatchedInstruction.opName];
	else
		uf.execCyclesLeft = 1;
	uf.next_busy = true;
	uf.opName = dispatchedInstruction.opName;
	if(dispatchedInstruction.isRtype)
	{
		uf.fi = dispatchedInstruction.rd;
		uf.fj = dispatchedInstruction.rs;
		uf.fk = dispatchedInstruction.rt;
	}
	else
	{
		if(dispatchedInstruction.opName != "SW")
		{
			uf.fi = dispatchedInstruction.rt;
			uf.fj = dispatchedInstruction.rs;
		}
		else
		{
			uf.fi = "-";
			uf.fj = dispatchedInstruction.rs;
			uf.fk = dispatchedInstruction.rt;
		}
	}
	uf.qj = regRes.isRegAvailable(uf.fj) ? "0" : regRes.getRegister(uf.fj);
	uf.qk = regRes.isRegAvailable(uf.fk) ? "0" : regRes.getRegister(uf.fk);

	uf.rj = uf.qj == "0" ? 1 : 0;
	uf.rk = uf.qk == "0" ? 1 : 0;
}

//returns false if operands not ready otherwise returns true
bool UfController::readOperands(int instructionId, string opName){ //alguma ideia melhor? precisa identificar a instrucao de alguma forma... talvez so id
	for(auto uf : ufsInt)
	{
		if(uf.instructionId == instructionId)
		{
			if(uf.rj && uf.rk)
			{
				uf.rj = uf.rk = 0;
				uf.qj = uf.qk = "0";
				return true;
			}
			else
				return false;
		}
	}
	for(auto uf : ufsFloat)
	{
		if(uf.instructionId == instructionId)
		{
			if(uf.rj && uf.rk)
			{
				uf.rj = uf.rk = 0;
				uf.qj = uf.qk = "0";
				return true;
			}
			else
				return false;
		}
	}
	fprintf(stderr, "Erro em readOperands: instrucao nao encontrada na UF");
	return false;
}

//returns false if UF not yet done otherwise returns true
bool UfController::runExecution(int instructionId, string opName){
	for(auto uf : ufsInt)
	{
		if(uf.instructionId == instructionId)
		{
			uf.execCyclesLeft--;
			return true;
		}
	}
	for(auto uf : ufsFloat)
	{
		if(uf.instructionId == instructionId)
		{
			uf.execCyclesLeft--;
			if(uf.execCyclesLeft == 0)
				return true;
			break;
		}
	}
	return false;
}

//returns false if the informed register is still waiting to be read by a UF otherwise returns true
bool UfController::isWriteAvailable(int instructionId, string opName){
	return true;
}

//returns the name of the destination register
string UfController::getDestReg(int instructionId, string opName){

	return "NULL";
}

//will change UF's status and update UFs waiting to read register
void UfController::clearAndUpdateUf(int instructionId, string opName, string regName){
	for(auto uf : ufsInt)
	{
		if(uf.instructionId == instructionId)
		{
			uf.busy = false;
			uf.fi = "";
			uf.fj = "";
			uf.fk = "";
			uf.instructionId = -1;
			uf.next_busy = false;
			uf.next_qj = "";
			uf.next_qk = "";
			uf.next_rj = -1;
			uf.next_rk = -1;
			uf.opName = "";
			uf.qj = "";
			uf.qk = "";
			uf.rj = -1;
			uf.rk = -1;
		}
	}
	for(auto uf : ufsFloat)
	{
		if(uf.instructionId == instructionId)
		{
			uf.busy = false;
			uf.fi = "";
			uf.fj = "";
			uf.fk = "";
			uf.instructionId = -1;
			uf.next_busy = false;
			uf.next_qj = "";
			uf.next_qk = "";
			uf.next_rj = -1;
			uf.next_rk = -1;
			uf.opName = "";
			uf.qj = "";
			uf.qk = "";
			uf.rj = -1;
			uf.rk = -1;
		}
	}
	fprintf(stderr, "Erro em clearAndUpdateUf: instrucao nao encontrada na UF");
}

//will update attributes with the values modified in the last clock cicle
void UfController::performClockTick(){
	for(auto uf : ufsInt){
		uf.qj = uf.next_qj;
		uf.qk = uf.next_rk;
		uf.rj = uf.next_rj;
		uf.rk = uf.next_rk;
	
	}
	for(auto uf : ufsFloat){
		uf.qj = uf.next_qj;
		uf.qk = uf.next_rk;
		uf.rj = uf.next_rj;
		uf.rk = uf.next_rk;
	}
}
