#pragma once
#include <iostream>
class CommandManager
{
public:
	CommandManager();
	~CommandManager();
	static void printMainMenu();
	static void printModeTwo();
	static void printModeThree();
	static void printModeFour();
	static void printMainDefault();
	static void printModeDefault();
	static void printNegativeError();
	static void printReturnMain();
	static void printFilePrompt();
};

