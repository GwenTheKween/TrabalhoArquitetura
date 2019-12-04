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
	std::vector<std::string> lines; //line names for the final table
	for(int i=0;i<MAX_PIPELINE_TABLE_SIZE;i++){
		lines.push_back(" "); //starts with empty lines, to be filled later
	}
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

bool run_code(){
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
	std::vector<std::string> empty;


	DispatchController dc;

	int clockCycle = 0;
	char input='n';
	int x,y;
	getmaxyx(stdscr,y,x);
	sm.mvprint_to_panel(-1,y-1,0,"press q to exit");
	while((instructionsLeft || instructionsExecuting) //while there are instructions to execute
			&& (input != 'q')){ //and the user doesn't wish to exit

		//run main loop; start by waiting input for the first step
		input = getch();

		//then try to dispatch the next instruction
		dc.tryToDispatchNext(&uc, &pc, &rrc, clockCycle);
		
		//moves everything in the pipeline one cycle forward
		pc.performClockCycle(uc, rrc, clockCycle);

		//commits all changes
		uc.performClockTick();
		//then we update the clock cycle
		clockCycle++;
		//and the table it is in
		std::stringstream ss;
		ss << clockCycle;
		clock.update_line(0,ss.str(),empty);
	}

	return (instructionsLeft == false) && (instructionsExecuting == false);
}

int main(){
	if(run_code()){ //if the program executed untill the end
		int x,y;
		getmaxyx(stdscr,y,x);
		std::string msg = "execution over, press q to exit";
		sm.clear();
		sm.mvprint_to_panel(-1,y/2,(x-msg.length())/2, "%s",msg.c_str()); //prints exit message
		char input;
		do{
			input = getch();
		}while(input != 'q');
	}
	return 0;
}
