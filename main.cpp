#include <stdio.h>
#include "screenManager.h"

screenManager sm;

int main(){
	int i = sm.register_new_panel();
	sm.print_to_panel(i,"hello world");
	getch();
	return 0;
}
