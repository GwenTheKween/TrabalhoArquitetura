#include "regResultController.h"

std::vector<std::string> regs = {"F0", "F2", "F4", "F6", "F8", "F10", "F12"};

using namespace std;
RegResController::RegResController(tableManager<string>& tm):
	gui(tm){
    for(size_t i = 0; i < regs.size(); i++){
        registers[regs[i]]= "NULL"; // registers available have a NULL string
    }
    return;
}

bool RegResController::isRegAvailable(string regName) {
    string r = registers[regName];
    if(r.compare("NULL") == 0) return 1;
    else return 0;    
}

string RegResController::getRegister(string regName){
    return registers[regName];
}

void RegResController::populateReg(string regName, string fuName){
	registers[regName] = fuName;
	update_table();
	return;
}

void RegResController::clearReg(string regName){
	update_table();
    registers[regName] = "NULL";
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
