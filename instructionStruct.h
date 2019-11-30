#ifndef INSTRUCTION_STRUCT_H
#define INSTRUCTION_STRUCT_H
struct instruction{
	std::string opName, rs, rt, rd;
	int immed, id;
	bool useFp, isRtype;
};

#endif
