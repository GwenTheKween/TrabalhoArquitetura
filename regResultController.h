#ifndef REG_RES_CONTROLLER_H
#define REG_RES_CONTROLLER_H

#include <unordered_map>
#include <string>
#include <vector>

class RegResController{

private:
	//will map register to fu using it
	std::unordered_map <std::string, std::string> registers;	
	//the dispatch will have to check availability first so no conflict happens because of the writeback
public:
	RegResController();

	//checks if a register is being used by a fu
	bool isRegAvailable(std::string regName);
	
	//obtains the functional unit that is using this register
	std::string getRegister(std::string regName);

	//specifies the functional unit using a register
	void populateReg(std::string regName, std::string fuName);

	//used when a register is no longer being used
	void clearReg(std::string regName);
};

#endif // REG_RES_CONTROLLER