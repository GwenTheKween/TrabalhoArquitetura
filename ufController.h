#ifndef UF_CONTROLLER_H
#define UF_CONTROLLER_H

#include <vector>
#include <string>

typedef struct{
	std::string ufName, opName, fi, fj, qj, qk;
	int rj, rk, busy, instructionId;
	
} ufLine;

class UfController{

private:
	//inicialmente quantidade fixa de uf's de tipos determinados
	//pensar em qual estrutura usar
	//sugestao: usar struct como a de cima
	std::vector<ufLine> ufs;
	//talvez manter um vetor pras livres e um pras ocupadas? 
public:
	UfController();
	
	//returns false if no uf is available for the informed operation otherwise returns true
	bool populateUf(std::string opName);
	
	//returns false if operands not ready otherwise returns true
	bool readOperands(int instructionId, std::string opName); //alguma ideia melhor? precisa identificar a instrucao de alguma forma... talvez so id
	
	//returns false if UF not yet done otherwise returns true
	bool runExecution(int instructionId, std::string opName);

	//returns false if the informed register is still waiting to be read by a UF otherwise returns true
	bool isWriteAvailable(std::string regName);
};
#endif
