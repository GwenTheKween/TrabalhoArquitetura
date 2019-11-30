#include <iostream>
#include "ufController.h"

using namespace std;


UfController::UfController(){


}

//checks if a compatible fu is available
bool UfController::hasUfAvailable(bool needsFloatingPointUf){

	return true;
}

//returns ufName - the name of the chosen fu
string UfController::populateUf(instruction dispatchedInstruction){
	return "populateUf";
}

//returns false if operands not ready otherwise returns true
bool UfController::readOperands(int instructionId, string opName){ //alguma ideia melhor? precisa identificar a instrucao de alguma forma... talvez so id
	return true;

}
//returns false if UF not yet done otherwise returns true
bool UfController::runExecution(int instructionId, string opName){
	return true;
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
	
}

//will update attributes with the values modified in the last clock cicle
void UfController::performClockTick(){
}
