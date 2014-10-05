// password.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include "CommandManager.h"
#include "PassDecoder.h"
#include "PassLoader.h"

const std::string defaultDicName = "d8.txt";
const std::string defaultPassName = "pass.txt";

bool isNumber(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

void handlePartTwo(PassLoader& loader) {
	std::string inputString;
	int choice;
	bool isChoiceTwo = true;
	do {
		CommandManager::printModeTwo();
		std::getline(std::cin, inputString);
		std::cout << std::endl;

		if (isNumber(inputString)) {
			choice = std::stoi(inputString);
			switch (choice) {
				case 1:
					loader.loadDictionary(defaultDicName);
					break;
				case 2:
					CommandManager::printFilePrompt();
					std::getline(std::cin, inputString);
					std::cout << std::endl;
					loader.loadDictionary(inputString);
					break;
				case 3:
					isChoiceTwo = false;
					CommandManager::printReturnMain();
					break;
				default:
					CommandManager::printModeDefault();
					break;
			}
		}
		else {
			CommandManager::printNegativeError();
		}
	} while (isChoiceTwo);
}

void handlePartThree(PassLoader& loader, PassDecoder& decoder) {
	std::string inputString;
	int choice;
	bool isChoiceThree = true;
	do {
		CommandManager::printModeThree();
		std::getline(std::cin, inputString);
		std::cout << std::endl;

		if (isNumber(inputString)) {
			choice = std::stoi(inputString);
			switch (choice) {
			case 1:
				decoder.decrypt(defaultPassName, loader.getMap());
				break;
			case 2:
				CommandManager::printFilePrompt();
				std::getline(std::cin, inputString);
				std::cout << std::endl;
				decoder.decrypt(inputString, loader.getMap());
				break;
			case 3:
				isChoiceThree = false;
				CommandManager::printReturnMain();
				break;
			default:
				CommandManager::printModeDefault();
				break;
			}
		}
		else {
			CommandManager::printNegativeError();
		}
	} while (isChoiceThree);
}

int main(int argc, char* argv[])
{
	std::string inputString;	
	int choice;
	PassLoader loader;
	PassDecoder decoder;
	bool isChoiceThree = false;

	do {
		inputString = "";
		choice = 0;
		CommandManager::printMainMenu();	
		std::getline(std::cin, inputString);
		std::cout << std::endl;

		if (isNumber(inputString)) {
			choice = std::stoi(inputString);
			switch (choice) {
				case 1:
					loader.basicHash();
					break;
				case 2:
					handlePartTwo(loader);
					break;
				case 3:
					handlePartThree(loader, decoder);
					break;
				case 4:
					CommandManager::printModeFour();
					return 0;
				default:
					CommandManager::printMainDefault();
					break;
			}
		}
		else {
			CommandManager::printNegativeError();
		}
	} while (true);

	return 0;
}

