// password.cpp : Defines the entry point for the console application.
//
#include <string>
#include <windows.h>
#include <fstream>
#include "sha1.h"
#include "passinfo.cpp"
#include <unordered_map>

bool isNumber(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}


void hashProcess(const std::string& password, pass_info* info) {
	info->origin = password;
	sha1::calc(password.c_str(), password.length(), info->hashed);
	sha1::toHexString(info->hashed, info->hex);
}


void basicHash() {
	std::string password;
	//unsigned char hash[20];
	pass_info* info = new pass_info();
	do {
		std::cout << "Please enter a password you want to hash: ";
		std::getline(std::cin, password);
		if (password.empty()) {
			std::cout << "Password cannot be empty.";
		}
	} while (password.empty());

	hashProcess(password, info);
	
	std::cout << "Hashed: " << info->hex << std::endl <<std::endl;
	delete info;
}

void loadDictionary(const std::string& fileName, std::unordered_map<std::string, pass_info*>& map) {

	LARGE_INTEGER freq, before, after;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&before);
	
	std::string content;
	std::ifstream fileInput;
	map.clear();
	map.rehash(100000);
	fileInput.open(fileName);
	if (fileInput.is_open()) {
		while (!fileInput.eof()) {
			std::getline(fileInput, content);
			//std::cout << content << std::endl;
			pass_info* info = new pass_info();
			hashProcess(content, info);
			map.insert(std::make_pair((std::string)info->hex, info));

		}

		QueryPerformanceCounter(&after);
		float fElapsed = static_cast<float>(after.QuadPart - before.QuadPart) / freq.QuadPart;

		std::cout << "Time elapsed: " << fElapsed << std::endl << std::endl;
		fileInput.close();
	}
	else {
		std::cout << "Failed to open the file: " << fileName << std::endl << std::endl;
	}
}


void decrypt(const std::string& fileName, std::unordered_map<std::string, pass_info*>& map) {
	std::string content;
	std::ifstream fileInput;
	
	fileInput.open(fileName);
	
	if (fileInput.is_open()) {
		std::ofstream fileOutput("pass_solved.txt", std::ofstream::out | std::ofstream::trunc);
		while (!fileInput.eof()) {
			std::getline(fileInput, content);
			fileOutput << content << ", ";
			auto it = map.find(content);
			if (it != map.end()) {
				fileOutput << map.at(content)->origin;
			}
			else {
				fileOutput << "????";
			}
			fileOutput << std::endl;
		}
		fileOutput.close();
		fileInput.close();
	}
	else {
		std::cout << "Failed to open the file: " << fileName << std::endl << std::endl;
	}

}


int main(int argc, char* argv[])
{
	std::string inputString;
	std::string defaultDicName = "d8.txt";
	std::string defaultPassName = "pass.txt";
	int choice;

	std::unordered_map<std::string, pass_info*> map;
	bool isChoiceTwo = false, isChoiceThree = false;

	do {
		inputString = "";
		choice = 0;

		std::cout << "Select an opotion:" << std::endl;
		std::cout << "1. Basic Hashing" << std::endl;
		std::cout << "2. Load Dictionary" << std::endl;
		std::cout << "3. Decrypt" << std::endl;
		std::cout << "4. Exit" << std::endl;
		std::cout << "> ";
		std::getline(std::cin, inputString);
		std::cout << std::endl;

		if (isNumber(inputString)) {
			choice = std::stoi(inputString);
			switch (choice) {
			case 1:
				basicHash();
				break;
			case 2:
				isChoiceTwo = true;
				
				do {
					
					std::cout << "You are in Mode 2 (Load Dictionary)." << std::endl;
					std::cout << "Please choose from the following options:" << std::endl;
					std::cout << "1. Use default dictionary file \"d8.txt\"" << std::endl;
					std::cout << "2. Use my own dictionary file" << std::endl;
					std::cout << "3. Return back to the main menu" << std::endl;
					std::cout << "> ";
					std::getline(std::cin, inputString);
					std::cout << std::endl;
					
					if (isNumber(inputString)) {
						choice = std::stoi(inputString);
						switch (choice) {
						case 1:
							loadDictionary(defaultDicName, map);
							break;
						case 2:
							std::cout << "Enter your dictionary file name: " << std::endl;
							std::cout << "> ";
							std::getline(std::cin, inputString);
							std::cout << std::endl;
							loadDictionary(inputString, map);
							break;
						case 3:
							isChoiceTwo = false;
							std::cout << "Returning to the Main Menu........." << std::endl << std::endl;
							break;
						default:
							std::cout << "Sorry, you should only enter 1, 2 or 3 to continue." << std::endl << std::endl;
							break;
						}
					}
					else {
						std::cout << "Sorry, please enter a positive integer number." << std::endl << std::endl;
					}
				} while (isChoiceTwo);
				break;
			case 3:
				isChoiceThree = true;
				do {	
					std::cout << "You are in Mode 3 (Decrypt)." << std::endl;
					std::cout << "Please choose from the following options:" << std::endl;
					std::cout << "1. Use default password file \"pass.txt\"" << std::endl;
					std::cout << "2. Use my own password file" << std::endl;
					std::cout << "3. Return back to the main menu" << std::endl;
					std::cout << "> ";
					std::getline(std::cin, inputString);
					std::cout << std::endl;
					
					if (isNumber(inputString)) {
						choice = std::stoi(inputString);
						switch (choice) {
						case 1:
							decrypt(defaultPassName, map);
							break;
						case 2:
							std::cout << "Enter your password file name: " << std::endl;
							std::cout << "> ";
							std::getline(std::cin, inputString);
							std::cout << std::endl;
							decrypt(inputString, map);
							break;
						case 3:
							isChoiceThree = false;
							std::cout << "Returning to the Main Menu........." << std::endl << std::endl;
							break;
						default:
							std::cout << "Sorry, you should only enter 1, 2 or 3 to continue." << std::endl << std::endl;
							break;
						}
					}
					else {
						std::cout << "Sorry, please enter a positive integer number." << std::endl << std::endl;
					}
				} while (isChoiceThree);
				break;
			case 4:
				std::cout << "Exiting the program ..............." << std::endl << std::endl;
				return 0;
			default:
				std::cout << "Sorry, you should only enter 1, 2, 3 or 4 to continue." << std::endl << std::endl;

			}
		}
		else {
			std::cout << "Sorry, please enter a positive integer number." << std::endl << std::endl;
		}
	} while (true);

	return 0;
}

