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
	zompilerparse();

	// TODO: CodeGen from g_MainBlock
	CodeContext myContext;
	if (g_MainBlock != nullptr) {
		g_MainBlock->CodeGen(myContext);
	}
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
