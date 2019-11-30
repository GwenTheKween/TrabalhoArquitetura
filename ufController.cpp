#include <iostream>
#include "ufController.h"

using namespace std;


//UfController();

//checks if a compatible fu is available
bool hasUfAvailable(bool needsFloatingPointUf){
	cout << "hasUfAvailable" <<endl;

	return true;
}

//returns ufName - the name of the chosen fu
std::string populateUf(instruction dispatchedInstruction){
	cout << "populateUf" <<endl;
	return "populateUf";
}

//returns false if operands not ready otherwise returns true
bool readOperands(int instructionId, std::string opName){ //alguma ideia melhor? precisa identificar a instrucao de alguma forma... talvez so id
	cout << "readOperands" << endl;
	return true;

}
//returns false if UF not yet done otherwise returns true
bool runExecution(int instructionId, std::string opName){
	cout << "runExecution" <<endl;
	return true;
}

//returns false if the informed register is still waiting to be read by a UF otherwise returns true
bool isWriteAvailable(std::string regName){
	cout <<"isWriteAvailable" <<endl;
	return true;
}

//will change UF's status and update UFs waiting to read register
void clearAndUpdateUf(int instructionId, std::string opName, std::string regName){
	
	cout << "clearAndUpdateUf" <<endl;
}

//will update attributes with the values modified in the last clock cicle
void performClockTick(){
	cout << "performClockTick" <<endl;
}
