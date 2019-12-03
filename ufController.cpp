#include <iostream>
#include <exception>
#include "ufController.h"

using namespace std;

//=================================================================================================
//auxiliary function
vector<string> table_data(ufLine& uf){
	vector<string> v;
	stringstream ss;

	//first column, if the FU is busy
	ss << uf.busy;
	v.push_back(ss.str());
	ss.str("");

	//next, what operation is being performed
	v.push_back(uf.opName);

	//then, the registers
	v.push_back(uf.fi);
	v.push_back(uf.fj);
	v.push_back(uf.fk);

	//then dependencies
	v.push_back(uf.qj);
	v.push_back(uf.qk);

	//finally, if the registers can be read
	ss << uf.rj;
	v.push_back(ss.str());
	ss.str("");
	ss << uf.rk;
	v.push_back(ss.str());

	return v;
}
//=================================================================================================

UfController::UfController(tableManager<std::string> tm):
	gui(tm){
	ufLine uf;
	uf.ufName = "Int1";
	uf.busy = false;
	uf.opName = uf.fi = uf.fj = uf.fk = uf.qj = uf.qk = "";
	uf.rj = uf.rk = uf.instructionId = uf.execCyclesLeft = 0;

	ufsInt.push_back(uf);

	uf.ufName = "Int2";
	ufsInt.push_back(uf);

	uf.ufName = "Float1";
	ufsFloat.push_back(uf);

	uf.ufName = "Float2";
	ufsFloat.push_back(uf);

	nCyclesFloating = {{"Add", 2}, {"Sub", 2}, {"Mul", 10}, {"Div", 40}};
}

//checks if a compatible fu is available and return it
ufLine* UfController::hasUfAvailable(bool needsFloatingPointUf){
	if(needsFloatingPointUf){
		for(size_t i = 0; i<ufsFloat.size(); i++){
			if(!ufsFloat[i].busy)
				return ufsFloat.data()+i;
		}
	}else{
		for(size_t i = 0; i < ufsInt.size(); i++){
			if(!ufsInt[i].busy){
				return ufsInt.data() + i;
			}
		}
	}
	throw logic_error("Nenhuma unidade funcional disponivel");
}

void UfController::populateUf(ufLine* uf,const instruction& dispatchedInstruction,RegResController* regRes){
	uf->instructionId = dispatchedInstruction.id;
	if(uf->ufName.substr(0, 3) == "Int")
		uf->execCyclesLeft = 1;
	else
		uf->execCyclesLeft = nCyclesFloating[dispatchedInstruction.opName];
	uf->busy = true;
	uf->opName = dispatchedInstruction.opName;
	if(dispatchedInstruction.isRtype)
	{
		uf->fi = dispatchedInstruction.rd;
		uf->fj = dispatchedInstruction.rs;
		uf->fk = dispatchedInstruction.rt;

		uf->qk = regRes->isRegAvailable(uf->fk) ? "0" : regRes->getRegister(uf->fk);
		uf->rk = (uf->qk == "0" ? 1 : 0);
	}
	else
	{
	if(dispatchedInstruction.opName != "SW")
		{
			uf->fi = dispatchedInstruction.rt;
			uf->fj = dispatchedInstruction.rs;

			uf->qk = "0";
			uf->rk = 1;
		}
		else
		{
			uf->fi = "-";
			uf->fj = dispatchedInstruction.rs;
			uf->fk = dispatchedInstruction.rt;

			uf->qk = regRes->isRegAvailable(uf->fk) ? "0" : regRes->getRegister(uf->fk);
			uf->rk = (uf->qk == "0" ? 1 : 0);
		}
	}
	uf->qj = regRes->isRegAvailable(uf->fj) ? "0" : regRes->getRegister(uf->fj);
	uf->rj = (uf->qj == "0" ? 1 : 0);

}

//returns false if operands not ready otherwise returns true
bool UfController::readOperands(ufLine* uf){ 
	if(uf->rj && uf->rk)
	{
		uf->rj = uf->rk = 0;
		uf->qj = uf->qk = "0";
		return true;
	}
	else
		return false;
}

//returns false if UF not yet done otherwise returns true
bool UfController::runExecution(ufLine* uf){
	uf->execCyclesLeft--;
	return uf->execCyclesLeft <= 0;
}

//returns false if the informed register is still waiting to be read by a UF otherwise returns true
bool UfController::isWriteAvailable(ufLine* FU){
	for(auto f:ufsInt){
		if(!(((f.fj != FU->fi) || f.rj == 0) && ((f.fk != FU->fi) || f.rk == 0))){
			return false;
		}
	}
	for(auto f:ufsFloat){
		if(!(((f.fj != FU->fi) || f.rj == 0) && ((f.fk != FU->fi) || f.rk == 0))){
			return false;
		}
	}
	return true;
}

//returns the name of the destination register
string UfController::getDestReg(int instructionId){
	for(auto uf : ufsInt)
	{
		if(uf.instructionId == instructionId)
		{
			return uf.fi;
		}
	}
	for(auto uf : ufsFloat)
	{
		if(uf.instructionId == instructionId)
		{
			return uf.fi;
		}
	}
	return "NULL";
}

//will change UF's status and update UFs waiting to read register
void UfController::clearAndUpdateUf(ufLine* uf){
	uf->busy = false;
	uf->opName = uf->fi = uf->fj = uf->fk = uf->qj = uf->qk = "";
	uf->rj = uf->rk = uf->instructionId = uf->execCyclesLeft = 0;

}

//will update attributes with the values modified in the last clock cicle
void UfController::performClockTick(){
	int line = 0;
	for(auto uf : ufsInt){
		gui.update_line(line++, uf.ufName, table_data(uf));
	}
	for(auto uf : ufsFloat){
		gui.update_line(line++, uf.ufName, table_data(uf));
	}
}
