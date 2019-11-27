#include <stdio.h>
#include "screenManager.h"

screenManager sm;

int main(){
	int i = sm.register_new_panel(0,0, 40, 20);
	sm.mvprint_to_panel(i,1,1,"UF\t|ocupado");
	sm.mvprint_to_panel(i,2,1,"----------------------");
	sm.mvprint_to_panel(i,3,1,"ULA\t| X");
	getch();
	return 0;
}
