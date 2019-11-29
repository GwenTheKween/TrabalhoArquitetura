#include "regResultController.h"

using namespace std;
RegResController::RegResController(){
    vector<string> regs = {"F0", "F2", "F4", "F6", "F8", "F10", "F12"}; //only a certain set of registers, may be expanded
    for(int i = 0; i < regs.size(); i++){
        registers.emplace(regs[i], "NULL"); // registers available have a NULL string
    }
    return;
}

bool RegResController::isRegAvailable(string regName){
    string r = registers.at(regName);
    if(r.compare("NULL") == 0) return 1;
    else return 0;    
}

string RegResController::getRegister(string regName){
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
