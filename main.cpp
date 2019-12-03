#include <stdio.h>
#include "screenManager.h"
#include "tableManager.h"
#include "dispatchController.h"

screenManager sm;

tableManager<std::string> create_pipeline_table(int* nextX, int* nextY){
	int posX = *nextX;//position for the table
	int posY = 0;
	std::vector<std::string> columns = {
		"Instruction ID",
		"Issue Clock",
		"Read Operands",
		"Exec Complete",
		"Write Result"
	}; //column names for the final table
	std::vector<std::string> lines = {"0","1","2","3","4"}; //line names for the final table
	std::vector<std::string> tmp(columns.size() - 1," ");//temporary vector, to make matrix creation more readable
	std::vector<std::vector<std::string> > data(lines.size(), tmp);//creates the matrix with the printed data

	//creates the table itself
	tableManager<std::string> tm(posX, posY, columns, lines, data);
	*nextY = 2*lines.size() + 3; //new value of X, for the next column
	*nextX += 1;
	for(auto s:columns){
		*nextX += s.length() + 1;
	}
	return tm;
}

tableManager<std::string> create_regsters_table(int* nextX, int* nextY){
	int posX = 0;
	int posY = *nextY;
	std::vector<std::string> columns = {"  "};
	for(auto regName:regs){ //add register names as column names, with the correct padding
		std::stringstream ss;
		ss.width(6);
		ss << regName; //adds padding to the name
		columns.push_back(ss.str()); //adds the name to the column
	}
	std::vector<std::string> lines = {"FU"};
	std::vector<std::string> tmp(regs.size()," ");
	std::vector<std::vector<std::string> > data(lines.size(),tmp);

	tableManager<std::string> tm(posX, posY, columns, lines, data);
	int height = 2*lines.size() + 3;
	if(height > *nextY) *nextY = height;
	*nextX += 1; //calculates next X position
	for(auto s:columns){
		*nextX += s.length() + 1;
	}

	return tm;
}

tableManager<std::string> create_clock_table(){
	int x,y;
	getmaxyx(stdscr, y, x); //gets the maximum height and width of the screen
	int posY = y - 5; //sticks to the bottom of the screen
	std::vector<std::string> columns = {"Clock Cycle"};
	int posX = x - columns[0].length() - 2;//sticks the table to the right of the screen
	std::vector<std::string> lines = {"0"};
	std::vector<std::vector<std::string> > data;

	tableManager<std::string> tm(posX, posY, columns, lines, data);
	return tm;
}

tableManager<std::string> create_uf_controller_table(int *nextX, int *nextY){
	int posY = *nextY;
	int posX = *nextX;

	std::vector<std::string> columns = {
		" Name ",
		"Busy",
		"op Name",
		" fi",
		" fj",
		" fk",
		"  Qj  ",
		"  Qk  ",
		"Rj",
		"Rk"
	};

	std::vector<std::string> lines={
		"Int1",
		"Int2",
		"Float1",
		"Float2"
	};
	std::vector<std::string> v(1,"0");
	while(v.size() < (columns.size() - 1)){
		v.push_back(" ");
	}
	std::vector<std::vector<std::string> > data(4,v);
	tableManager<std::string> tm(posX, posY, columns, lines, data);
	*nextY += 2*lines.size() + 3;
	*nextX = 1;
	for(auto s:columns){
		*nextX += s.length() + 1;
	}
	return tm;
}

int main(){
	//coordinates for next table creation
	int nextX = 0, nextY = 0;

	//creates fourth table, for uf controller
	tableManager<std::string> tm_uf = create_uf_controller_table(&nextX, &nextY);
	UfController uc(tm_uf);

	//creates first table, for the pipeline information
	tableManager<std::string> tm_pipe = create_pipeline_table(&nextX, &nextY);
	PipelineController pc(tm_pipe);

	//creates second table, for registers information
	tableManager<std::string> tm_reg = create_regsters_table(&nextX,&nextY);
	RegResController rrc(tm_reg);

	//creates third table, to show the clock cycle
	tableManager<std::string> clock = create_clock_table();


	DispatchController dc;

	int clockCycle = 0;
	while(true){
		//main cycle
		//the first thing done is to try and dispatch a new instruction
		dc.tryToDispatchNext(uc, pc, rrc, clockCycle);

		pc.performClockCycle(uc, rrc, clockCycle);

		//then we update the clock cycle
		clockCycle++;
		break;
	}
	return 0;
}
