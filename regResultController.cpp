#include <stdexcept>
#include "regResultController.h"

std::vector<std::string> regs = {"F0", "F1", "F2", "F3", "F4", "F5", "F6"};

using namespace std;
RegResController::RegResController(tableManager<string>& tm): gui(tm){
    for(size_t i = 0; i < regs.size(); i++){
        registers[regs[i]] = "NULL"; // registers available have a NULL string
    }
    return;
}

bool RegResController::isRegAvailable(string regName) {
	try
	{
		string r = registers.at(regName);
		if(r.compare("NULL") == 0) return true;
		else return false;
	}
	catch(std::out_of_range& e)
	{
		return true;
	}
}

string RegResController::getRegister(string regName){
    return registers[regName];
}

void RegResController::populateReg(string regName, string fuName){
	// update only if 'registers' already have 'regName'
	if(registers.find(regName) != registers.end())
	{
		registers[regName] = fuName;
		update_table();
	}
	return;
}

void RegResController::clearReg(string regName){
	// update only if 'registers' already have 'regName'
	if(registers.find(regName) != registers.end())
	{
		registers[regName] = "NULL";
		update_table();
	}
	return;
}

void RegResController::update_table(){
	vector<string> data;
	for(auto it = registers.begin(); it != registers.end(); it++){
		if(it->second != "NULL")
			data.push_back(it->second);
		else
			data.push_back(" ");
	}
	gui.update_line(0,"FU",data);
}
