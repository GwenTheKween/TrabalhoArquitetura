#include "screenManager.h"
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

//The implementation of the methods MUST BE DONE in this file, because of template classes' restriction


//tableManager will keep very little information, just the panel that it'll print to, and the amount of padding needed for each column
//everything else must be stored by the class that is calling it
template<class T> class tableManager{
private:
	int panel;
	std::vector<int> columnWidths;
public:
	//constructor if all you have is column and line names
	tableManager(int posX, //x position of the panel
				 int posY, //y position of the panel
				 std::vector<std::string> columnNames, //column names. Assumed to be the largest string on the column
				 std::vector<std::string> lineNames, // line names. doesn't include the first line
				 const char* format, //the string used in printf to format the data correctly. Assumed to NOT INCLUDE padding nor %.
				 std::vector<std::vector<T> > d); //data to be printed and stored by the manager

	//the destructor
	~tableManager();
};

/*
 * This constructor is used when you only have the names of columns and lines, and the data. It takes care of constructing the table
*/
template<class T> tableManager<T>::tableManager(
		int posX, //x position
		int posY, //y position
		std::vector<std::string> columnNames, //names of the columns. Assumed to be the largest string of the column
		std::vector<std::string> lineNames, //names of the lines, to go on the first column. Doesn't include the name of the first line
		const char* format, // format used in printf for the data
		std::vector<std::vector<T> > d){ //data to be stored


	//the first thing to be done is creating the panel that will hold the table
	//we can calculate the height and width needed for the panel in the following way:
	int height = d.size() + 2; //amount of lines to be printed + 2 because of the panel border
	int width = 1; //we start at 1 to account for the border
	for(int i = 0; i < columnNames.size(); i++){
		width += columnNames[i].length() + 1; //the length of each column and the separator
	}
	//now it creates a panel
	panel = sm.register_new_panel(posX, posY, width, height);

	//now we set up the padding information, and format the header of the table
	std::string header;
	columnWidths.resize(columnNames.size());
	//first line of the table, just the header
	header = columnNames[0];
	columnWidths[0] = columnNames[0].length();
	for(int i = 1; i<columnNames.size(); i++){
		header += "|" + columnNames[i];
		columnWidths[i] = columnNames[i].length();
	}
	//with that done, we can already print it. We print at line 1 because line 0 has the border
	sm.mvprint_to_panel(panel, 1,1, header.c_str());

	//formats the line separator, which is just a bunch of "-------" with the same size as a regular line
	std::string s = header;
	for(int j = 0; j<s.size(); j++){
		if(s[j] != '|') s[j] = '-';
	}

	//Finally, we format the rest of the lines of the table, and print them as the are finished
	std::stringstream ss;
	for(int i=0; i<lineNames.size(); i++){
		//we print the line separator at the begining of the loop to ease the formatting
		sm.mvprint_to_panel(panel, 2*i + 1, 1, s.c_str());
		//now we start to format the actual data.
		ss.width(columnWidths[0]);
		ss << lineNames[i];
		//then calculate the amount of padding for all columns, excluding the first
		for(int j=1; j<columnNames.size(); j++){
			ss << '|';
			ss.width(columnWidths[j]);
			ss << d[i][j-1];
		}
		sm.mvprint_to_panel(panel, 2*(i+1), 1, ss.str().c_str());
	}

}

//all it needs to do is zero out all variables and remove the panel
template<class T> tableManager<T>::~tableManager(){
	sm.removePanel(panel);
	panel = 0;
}

/*
template<class T> tableManager<T>::print_line(int lineNum){
	if(
}
*/
