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

bool RegResController::isRegAvailable(string regName) const{
    string r = registers.at(regName);
    if(r.compare("NULL") == 0) return 1;
    else return 0;    
}

string RegResController::getRegister(string regName) const{
    return registers.at(regName);
}

void RegResController::populateReg(string regName, string fuName){
    registers[regName] = fuName;
    return;
}

void RegResController::clearReg(string regName){
    registers[regName] = "NULL";
    return;
}
