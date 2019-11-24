#ifndef SCREEN_MANAGER_API
#define SCREEN_MANAGER_API

#include <stdio.h>
#include <ncurses.h>
#include <panel.h>
#include <vector>
#include <stdarg.h>

class screenManager{
private:
	std::vector<WINDOW*> windows;
	std::vector<PANEL*> panels;

	int width, height;
public:
	screenManager();
	~screenManager();

	int register_new_panel();
	//print_to_panel works the same way as printf functions
	void print_to_panel(int id, const char* fmt, ...) __attribute__((format(printf, 3,4)));

	//remove a single panel
	void removePanel(int position);
	//clear all panels and windows
	void clear();
};

#endif //SCREEN_MANAGER_API
