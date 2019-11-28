#include <stdio.h>
#include "screenManager.h"
#include "tableManager.h"

screenManager sm;

int main(){
	/*
	std::vector<std::vector<int> > d;
	std::string tmp;
	std::vector<std::string> s;
	tmp = "Regs|busy";
	s.push_back(tmp);
	tmp = "----|----";
	s.push_back(tmp);
	tmp = " R0 | no ";
	s.push_back(tmp);
	tableManager<int> tm(0,0,s,d);
	*/
	std::vector<std::vector<int> >d(1);
	std::string n = "REGS";
	std::vector<std::string> col;
	col.push_back(n);
	n = "busy";
	col.push_back(n);
	std::vector<std::string> lin;
	n = "R0";
	lin.push_back(n);
	n = "R1";
	lin.push_back(n);
	tableManager<int> tm(0,0,col,lin,"d",d);
	getch();
	return 0;
}
