#include "screenManager.h"

screenManager::screenManager():
	curr_id(0){
	initscr();
	cbreak();
	raw();
	noecho();
	keypad(stdscr, true);
	getmaxyx(stdscr, height, width);

	//creating stdscr panel, to ease logic on printing functions
	panelStruct std;
	std.win = stdscr;
	std.pan = NULL;
	std.width = width;
	std.height = height;
	std.posX = 0;
	std.posY = 0;

	panels[-1] = std;

	ids.push_back(-1);
}

screenManager::~screenManager(){
	clear();
	endwin();
}

int screenManager::register_new_panel(int posX, int posY,int wid, int hei){
	WINDOW* new_window;
	panelStruct pan;

	//create new ncurses window
	new_window = newwin(hei, wid, posY, posX);
	keypad(new_window, true);
	box(new_window, 0, 0);

	//set struct data
	pan.win = new_window;
	pan.pan = new_panel(new_window);
	pan.width = wid;
	pan.height = hei;
	pan.posX = posX;
	pan.posY = posY;

	//save data to unordered map
	panels[curr_id] = pan;
	//update the screen
	doupdate();

	//add current id as a used ID
	ids.push_back(curr_id);
	//return panel id
	return curr_id++;
}

void screenManager::print_to_panel(int id, const char* fmt, ...){
	va_list arg;
	va_start(arg,fmt);
	//if not, check if ID is valid
	if(panels[id].win){
		vwprintw(panels[id].win, fmt, arg);
	}

	update_panels();
	doupdate();
}

void screenManager::mvprint_to_panel(int id, int y, int x, const char* fmt, ...){
	va_list arg;
	va_start(arg, fmt);
	if(panels[id].win){
		wmove(panels[id].win,y,x);
		vwprintw(panels[id].win, fmt, arg);
	}

	update_panels();
	doupdate();
}

void screenManager::removePanel(int id){
	//delete panel
	del_panel(panels[id].pan);

	//delete window border
	wborder(panels[id].win,' ',' ',' ',' ',' ',' ',' ',' ');
	update_panels();
	doupdate();
	//delete window itself
	delwin(panels[id].win);

	//set all the info to 0
	memset(&panels[id],0,sizeof(panelStruct));

	for(std::vector<int>::iterator it=ids.begin();it!=ids.end(); it++){
		if(*it == id){
			ids.erase(it);
			break;
		}
	}
	panels.erase(id);
}

void screenManager::clear(){
	for(size_t i=0; i < ids.size(); i++){
		removePanel(ids[i]);
	}
}
