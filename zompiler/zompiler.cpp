// zompiler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "node.h"
#include "parser.hpp"
#include <fstream>
#include <conio.h>

extern int g_LineNumber;
extern NBlock* g_MainBlock;

int _tmain(int argc, _TCHAR* argv[])
{
	// generate AST
	zompilerparse();

	// generate unoptimized code
	CodeContext myContext;
	if (g_MainBlock != nullptr) {
		g_MainBlock->CodeGen(myContext);
	}

	// optimize "goto" operation
	for (auto pair : myContext.m_Gotos) {
		int key = pair.first;
		int temp = pair.second;
		int value = 0;
		while (myContext.m_Gotos.find(temp) != myContext.m_Gotos.end()) {
			value = myContext.m_Gotos[temp];
			temp = value;
		}
		if (value != 0) {
			myContext.m_Ops[key - 1] = "goto," + std::to_string(value);
		}
	}

	// write all the code into a file named "out.zom"
	std::ofstream of("out.zom");
	if (of.is_open()) {
		for (unsigned int i = 0; i < myContext.m_Ops.size(); i++) {
			if (i == 0) {
				of << myContext.m_Ops[0];
			}
			else {
				of << std::endl << myContext.m_Ops[i];
			}
		}
	}
	of.close();

	_getch();
	return 0;
}

void zompilererror(const char* s)
{
	std::cout << s << " on line " << g_LineNumber << std::endl;
}
