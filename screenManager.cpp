#include "screenManager.h"


screenManager::screenManager():
	curr_id(0){
	initscr();
	cbreak();
	raw();
	keypad(stdscr, true);
	getmaxyx(stdscr, height, width);
}

screenManager::~screenManager(){
	clear();
	endwin();
}

int screenManager::register_new_panel(int posX, int posY,int wid, int hei){
	WINDOW* new_window;
	new_window = newwin(hei, wid, posY, posX);
	keypad(new_window, true);
	windows[curr_id] = new_window;
	box(new_window, 0, 0);
	panels[curr_id] = new_panel(new_window);
	doupdate();

	return curr_id++;
}

void screenManager::print_to_panel(int id, const char* fmt, ...){
	va_list arg;
	//first check if id is stdscr
	if(id < 0){
		wprintw(stdscr, fmt, arg);
	}else{
		//if not, check if ID is valid
		if(windows[id]){
			mvwprintw(windows[id], 1, 1, fmt, arg);
		}
	}

	update_panels();
	doupdate();
}

void screenManager::removePanel(int id){
	//delete panel
	del_panel(panels[id]);

	//delete window border
	wborder(windows[id],' ',' ',' ',' ',' ',' ',' ',' ');
	update_panels();
	doupdate();
	//delete window itself
	delwin(windows[id]);

	panels.erase(id);
	windows.erase(id);
}

void screenManager::clear(){
	while(!panels.empty()){
		removePanel(panels.size() - 1);
	}
}
