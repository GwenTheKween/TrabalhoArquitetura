#include <stdio.h>
#include "screenManager.h"

screenManager sm;

int main(){
	int i = sm.register_new_panel(0,0);
	sm.print_to_panel(i,"number %d and %f",3, 42.1);
	getch();
	return 0;
}
