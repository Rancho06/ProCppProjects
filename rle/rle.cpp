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

int main(int argc, char* argv[])
{
	// Main entry point!
	std::string inputString;

	int choice;

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

		if (is_number(inputString)) {
			choice = std::stoi(inputString);
			switch (choice) {
			case 1:
				Part1Tests();
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				std::cout << "Exiting the program ..............." << std::endl;
				return 0;
			default:
				std::cout << "Sorry, you should only enter 1, 2, 3 or 4 to continue." << std::endl;

			}
		}
		else {
			std::cout << "Sorry, please enter a positive integer number." << std::endl;
		}
	} while (true);

	return 0;
}

