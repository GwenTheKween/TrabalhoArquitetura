#include <stdio.h>
#include "screenManager.h"
#include "tableManager.h"
#include "dispatchController.h"

screenManager sm;

tableManager<std::string> create_pipeline_table(int* nextX, int* nextY){
	int posX = 0;//position for the table
	int posY = 0;
	std::vector<std::string> columns = {
		"Instruction ID",
		"Issue Clock",
		"Read Operands",
		"Exec Complete",
		"Write Result"
	}; //column names for the final table
	std::vector<std::string> lines = {"1","2","3","4","5"}; //line names for the final table
	std::vector<std::string> tmp(columns.size() - 1," ");//temporary vector, to make matrix creation more readable
	std::vector<std::vector<std::string> > data(lines.size(), tmp);//creates the matrix with the printed data

	//creates the table itself
	tableManager<std::string> tm(posX, posY, columns, lines, data);
	*nextY = 2*lines.size() + 3; //new value of X, for the next column
	*nextX = 1;
	for(auto s:columns){
		*nextX += s.length() + 1;
	}
	return tm;
}

tableManager<std::string> create_regsters_table(int* nextX, int* nextY){
	int posX = *nextX;
	int posY = 0;
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

tableManager<std::string> create_clock_table(int *nextX, int *nextY){
	int x,y;
	getmaxyx(stdscr, y, x); //gets the maximum height and width of the screen
	int posY = 0;
	std::vector<std::string> columns = {"Clock Cycle"};
	int posX = x - columns[0].length() - 2;//sticks the table to the right of the screen
	std::vector<std::string> lines = {"0"};
	std::vector<std::vector<std::string> > data;

	tableManager<std::string> tm(posX, posY, columns, lines, data);
	*nextX = 0;
	return tm;
}

int main(){
	//coordinates for next table creation
	int nextX, nextY;

	//creates first table, for the pipeline information
	tableManager<std::string> tm1 = create_pipeline_table(&nextX, &nextY);
	PipelineController pc(tm1);
	sm.mvprint_to_panel(-1, nextX,nextY, "%d %d", nextX, nextY);
	getch();

	//creates second table, for registers information
	tableManager<std::string> tm2 = create_regsters_table(&nextX,&nextY);
	RegResController rrc(tm2);
	getch();

	//creates third table, to show the clock cycle
	tableManager<std::string> clock = create_clock_table(&nextX, &nextY);
	getch();
	return 0;
}
