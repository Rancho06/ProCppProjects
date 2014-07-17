// rle.cpp : Defines the entry point for the console application.
//

#include "rletest.h"
#include "rle-algo.hpp"
#include <iostream>
#include "rle-files.h"

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

int main(int argc, char* argv[])
{
	// Main entry point!
	return 0;
}

