#ifndef REG_RES_CONTROLLER_H
#define REG_RES_CONTROLLER_H

#include <map>
#include <string>
#include <vector>
#include "tableManager.h"

extern std::vector<std::string> regs;

class RegResController{

private:
	//will map register to fu using it
	std::map <std::string, std::string> registers, registers_next;	

	tableManager<std::string> gui;
	//the dispatch will have to check availability first so no conflict happens because of the writeback
public:
	RegResController(tableManager<std::string>& tm);

	//checks if a register is being used by a fu
	bool isRegAvailable(std::string regName) ;
	
	//obtains the functional unit that is using this register
	std::string getRegister(std::string regName);

	//specifies the functional unit using a register
	void populateReg(std::string regName, std::string fuName);

	//used when a register is no longer being used
	void clearReg(std::string regName);

	//used when there might be new information to be displayed
	void update_table();
	
	void performClockTick();
};

#endif // REG_RES_CONTROLLER
