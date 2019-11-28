#include <stdio.h>
#include "screenManager.h"
#include "tableManager.h"

screenManager sm;

int main(){
	//gerando dados a serem impressos
	std::vector<std::vector<int> >d;
	std::vector<int> v;
	v.push_back(1);
	v.push_back(0);
	d.push_back(v);
	v[0]--;
	d.push_back(v);

	//gerando nomes das colunas
	std::string n = "REGS";
	std::vector<std::string> col;
	col.push_back(n);
	n = "busy";
	col.push_back(n);
	n = "val";
	col.push_back(n);

	//gerando nomes das linhas
	std::vector<std::string> lin;
	n = "R0";
	lin.push_back(n);
	n = "R1";
	lin.push_back(n);

	//tabela em si
	tableManager<int> tm(0,0,col,lin,"d",d);

	//aguarda digitar um caracter, para poder ver o q esta acontecendo
	getch();
	return 0;
}
