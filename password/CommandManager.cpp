#include "CommandManager.h"


CommandManager::CommandManager()
{
}


CommandManager::~CommandManager()
{
}

void CommandManager::printMainMenu() {
	std::cout << "Select an opotion:" << std::endl;
	std::cout << "1. Basic Hashing" << std::endl;
	std::cout << "2. Load Dictionary" << std::endl;
	std::cout << "3. Decrypt" << std::endl;
	std::cout << "4. Exit" << std::endl;
	std::cout << "> ";
}

void CommandManager::printModeTwo() {
	std::cout << "You are in Mode 2 (Load Dictionary)." << std::endl;
	std::cout << "Please choose from the following options:" << std::endl;
	std::cout << "1. Use default dictionary file \"d8.txt\"" << std::endl;
	std::cout << "2. Use my own dictionary file" << std::endl;
	std::cout << "3. Return back to the main menu" << std::endl;
	std::cout << "> ";
}

void CommandManager::printModeThree() {
	std::cout << "You are in Mode 3 (Decrypt)." << std::endl;
	std::cout << "Please choose from the following options:" << std::endl;
	std::cout << "1. Use default password file \"pass.txt\"" << std::endl;
	std::cout << "2. Use my own password file" << std::endl;
	std::cout << "3. Return back to the main menu" << std::endl;
	std::cout << "> ";
}

void CommandManager::printModeFour() {
	std::cout << "Exiting the program ..............." << std::endl << std::endl;
}

void CommandManager::printMainDefault() {
	std::cout << "Sorry, you should only enter 1, 2, 3 or 4 to continue." << std::endl << std::endl;
}

void CommandManager::printModeDefault() {
	std::cout << "Sorry, you should only enter 1, 2 or 3 to continue." << std::endl << std::endl;
}

void CommandManager::printNegativeError() {
	std::cout << "Sorry, please enter a positive integer number." << std::endl << std::endl;
}

void CommandManager::printReturnMain() {
	std::cout << "Returning to the Main Menu........." << std::endl << std::endl;
}

void CommandManager::printFilePrompt() {
	std::cout << "Enter your file name: " << std::endl;
	std::cout << "> ";
}