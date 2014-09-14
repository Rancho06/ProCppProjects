// rle.cpp : Defines the entry point for the console application.
//

#include "rletest.h"
#include "rle-algo.hpp"
#include <iostream>
#include "rle-files.h"
//#include <sstream>

void Part1Tests()
{
	std::cout << "Testing compression..." << std::endl;
	RLE<char> r;
	r.Compress(CHAR_1, strlen(CHAR_1));
	std::cout << r << std::endl;
	r.Compress(CHAR_2, strlen(CHAR_2));
	std::cout << r << std::endl;
	r.Compress(CHAR_3, strlen(CHAR_3));
	std::cout << r << std::endl;
	r.Compress(CHAR_4, strlen(CHAR_4));
	std::cout << r << std::endl << std::endl;
	std::cout << "Testing decompression..." << std::endl;
	r.Decompress(RLE_CHAR_1, strlen(RLE_CHAR_1), 40);
	std::cout << r << std::endl;
	r.Decompress(RLE_CHAR_2, strlen(RLE_CHAR_2), 12);
	std::cout << r << std::endl;
	r.Decompress(RLE_CHAR_3, strlen(RLE_CHAR_3), 14);
	std::cout << r << std::endl << std::endl;
}

bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

void singleFileCreate(const std::string& fileName) {
	RLE_v1* singleFileController = new RLE_v1();
	singleFileController->CreateArchive(fileName);
	delete singleFileController;
}

void singleFileExtract(const std::string& fileName) {
	RLE_v1* singleFileController = new RLE_v1();
	singleFileController->ExtractArchive(fileName);
	delete singleFileController;
}

int main(int argc, char* argv[])
{
	// Main entry point!
	std::string inputString;

	int choice;
	bool isPartTwo = false, isPartThree = false;

	do {
		inputString = "";
		choice = 0;

		std::cout << "Please choose an opotion:" << std::endl;
		std::cout << "1. Part 1 (Tests)" << std::endl;
		std::cout << "2. Part 2 (Single File)" << std::endl;
		std::cout << "3. Part 3 (Directories)" << std::endl;
		std::cout << "4. Exit" << std::endl;
		std::cout << "> ";
		std::getline(std::cin, inputString);
		std::cout << std::endl;

		if (is_number(inputString)) {
			choice = std::stoi(inputString);
			switch (choice) {
			case 1:
				Part1Tests();
				break;
			case 2:
				isPartTwo = true;
				do {
					std::cout << "You are in Part 2 (Single File)." << std::endl;
					std::cout << "Please choose from the following options:" << std::endl;
					std::cout << "1. Create an archive" << std::endl;
					std::cout << "2. Extract an archive" << std::endl;
					std::cout << "3. Return back to the main menu" << std::endl;
					std::cout << "> ";
					std::getline(std::cin, inputString);
					std::cout << std::endl;

					if (is_number(inputString)) {
						choice = std::stoi(inputString);
						switch (choice) {
							case 1:
								std::cout << "Please enter the name of the file you wish to compress: " << std::endl;
								std::cout << "> ";
								std::getline(std::cin, inputString);
								std::cout << std::endl;
								singleFileCreate(inputString);
								break;
							case 2:
								std::cout << "Please enter the name of the file you wish to extract: " << std::endl;
								std::cout << "> ";
								std::getline(std::cin, inputString);
								std::cout << std::endl;
								singleFileExtract(inputString);
								break;
							case 3:
								isPartTwo = false;
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
				} while (isPartTwo);
				break;
			case 3:
				isPartThree = true;
				do {
					std::cout << "You are in Part 3 (Directories)." << std::endl;
					std::cout << "Please choose from the following options:" << std::endl;
					std::cout << "1. Create an archive" << std::endl;
					std::cout << "2. Extract an archive" << std::endl;
					std::cout << "3. Return back to the main menu" << std::endl;
					std::cout << "> ";
					std::getline(std::cin, inputString);
					std::cout << std::endl;

					if (is_number(inputString)) {
						choice = std::stoi(inputString);
						switch (choice) {
						case 1:
							break;
						case 2:
							break;
						case 3:
							isPartThree = false;
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
				} while (isPartThree);
				break;
			case 4:
				std::cout << "Exiting the program ..............." << std::endl <<std::endl;
				return 0;
			default:
				std::cout << "Sorry, you should only enter 1, 2, 3 or 4 to continue." << std::endl << std::endl;

			}
		}
		else {
			std::cout << "Sorry, please enter a positive integer number." << std::endl <<std::endl;
		}
	} while (true);

	return 0;
}

