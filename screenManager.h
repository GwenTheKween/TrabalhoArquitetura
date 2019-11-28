#ifndef SCREEN_MANAGER_API
#define SCREEN_MANAGER_API

#include <stdio.h>
#include <ncurses.h>
#include <panel.h>
#include <unordered_map>
#include <stdarg.h>
#include <vector>

#include <string.h> //used for memset


typedef struct{
	WINDOW* win;
	PANEL* pan;
	int width, height, posX, posY;
} panelStruct;

class screenManager{
private:
	std::unordered_map<int, panelStruct> panels;
	std::vector<int> ids;

	int width, height, curr_id;
public:
	screenManager();
	~screenManager();

	int register_new_panel(int posX, int posY, int wid = 20, int hei=10);
	//print_to_panel works the same way as printf functions
	void print_to_panel(int id, const char* fmt, ...) __attribute__((format(printf, 3,4)));
	void mvprint_to_panel(int id, int y, int x, const char* fmt, ...) __attribute__((format(printf, 5,6)));

	//remove a single panel
	void removePanel(int id);
	//clear all panels and windows
	void clear();
};

extern screenManager sm;

#endif //SCREEN_MANAGER_API
