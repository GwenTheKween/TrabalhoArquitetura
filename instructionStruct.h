#ifndef INSTRUCTION_STRUCT_H
#define INSTRUCTION_STRUCT_H

#include <string>

struct instruction{
	std::string opName, rs, rt, rd;
	int immed, id;
	bool useFp, isRtype;
};

#endif
