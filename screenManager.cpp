#include "screenManager.h"


screenManager::screenManager(){
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

int screenManager::register_new_panel(){
	//FIRST TEST
	//I will probably not work correctly at first, many stuffs to fix
	WINDOW* new_window;
	new_window = newwin(height, width, 0,0);
	keypad(new_window, true);
	windows.push_back(new_window);
	box(new_window, 0, 0);
	panels.push_back(new_panel(new_window));
	doupdate();
	return windows.size()-1;
}

void screenManager::print_to_panel(int id, const char* fmt, ...){
	va_list arg;
	if(id >= panels.size()){
		return;
	}
	if(id < 0){
		wprintw(stdscr, fmt, arg);
	}else{
		wprintw(windows[id],fmt,arg);
	}

	update_panels();
	doupdate();
}

void screenManager::removePanel(int position){
	//delete panel
	del_panel(panels[position]);

	//delete window border
	wborder(windows[position],' ',' ',' ',' ',' ',' ',' ',' ');
	update_panels();
	doupdate();
	//delete window itself
	delwin(windows[position]);

	//possibly remove references?
	panels[position] = NULL;
	windows[position] = NULL;
}

void screenManager::clear(){
	while(!panels.empty()){
		removePanel(panels.size() - 1);
	}
}
