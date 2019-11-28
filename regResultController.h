#ifndef REG_RES_CONTROLLER_H
#define REG_RES_CONTROLLER_H

#include <unordered_map>
#include <string>

class RegResController{

private:
	//will map register to uf using it
	std::unordered_map <std::string, std::string> registers;	

public:
	//precisa definir um valor que permita reconhecer que esta livre
	
	bool isRegAvailable(std::string regName);
	
	void populateReg(std::string regName, std::string ufName);

	void clearReg(std::string regName);
};
#endif
