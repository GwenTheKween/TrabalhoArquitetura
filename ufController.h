#ifndef UF_CONTROLLER_H
#define UF_CONTROLLER_H

#include <vector>
#include <string>
#include "instructionStruct.h"

typedef struct{
	std::string ufName, opName, fi, fj, qj, next_qj, qk, next_qk;
	int rj, next_rj, rk, next_rk, instructionId;
	bool busy, next_busy;
} ufLine;

class UfController{

private:
	//inicialmente quantidade fixa de uf's de tipos determinados
	//pensar em qual estrutura usar
	//sugestao: usar struct como a de cima. Variaveis com next evitam conflito!!
	std::vector<ufLine> ufs;
	//talvez manter um vetor pras livres e um pras ocupadas? 
public:
	UfController();
	
	//checks if a compatible fu is available
	bool hasUfAvailable(bool needsFloatingPointUf);

	//returns ufName - the name of the chosen fu
	std::string populateUf(instruction dispatchedInstruction);
	
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
