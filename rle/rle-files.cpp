#include "rle-files.h"
#include "rle-algo.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <windows.h>

void RLE_v1::CreateArchive(const std::string& source)
{
	// Requires <fstream>
	std::ifstream::pos_type size;
	char* memblock;

	// Set up the file read mode 
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);

	// If file name exists
	if (file.is_open())
	{
		size = file.tellg(); // grab the whole contents
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg);
		file.read(memblock, size); // File data has now been loaded into memblock array
		file.close();
		
		m_Data.Compress(memblock, (int)size); // Compress the file and load to m_Data

		// Message compression ratio
		std::cout << "Compression: " << (double)((int)size - m_Data.m_Size) * 100 / ((int)size) << "%." << std::endl << std::endl;
		
		// Load header info to m_Header
		m_Header.sig[0] = 'R';
		m_Header.sig[1] = 'L';
		m_Header.sig[2] = 'E';
		m_Header.sig[3] = 0x01;
		m_Header.fileSize = static_cast<int>(size);
		m_Header.fileNameLength = source.length();
		m_Header.fileName = source;
		
		std::ofstream arc(m_Header.fileName + ".rl1", std::ios::out | std::ios::binary | std::ios::trunc); // Set up file write mode
		if (arc.is_open())
		{
			// Write to the file
			arc.write(m_Header.sig,4);
			arc.write(reinterpret_cast<char*>(&(m_Header.fileSize)), 4);
			arc.write(reinterpret_cast<char*>(&(m_Header.fileNameLength)), 1);
			arc.write(m_Header.fileName.c_str(),m_Header.fileNameLength);
			arc.write(m_Data.m_Data, m_Data.m_Size);
			arc.close();
		}
		// Release memblock
		delete[] memblock;
	}
	else {
		std::cout << "Sorry, file name not found!" << std::endl << std::endl;
	}
}

void RLE_v1::ExtractArchive(const std::string& source)
{
	std::ifstream::pos_type size;
	char* memblock;

	// Set up the file read mode 
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);

	// If file name exists
	if (file.is_open())
	{
		size = file.tellg(); // grab the whole contents
		memblock = new char[static_cast<unsigned int>(size)];
		std::cout << size;
		file.seekg(0, std::ios::beg);
		file.read(memblock, size); // File data has now been loaded into memblock array
		file.close();

		// Load header info to m_Header
		for (int i = 0; i < 4; i++) {
			m_Header.sig[i] = memblock[i];
		}
		// Check if the file signature is correct
		if (m_Header.sig[0] == 'R' && m_Header.sig[1] == 'L' && m_Header.sig[2] == 'E' && m_Header.sig[3] == 0x01) {

			m_Header.fileSize = *(reinterpret_cast<int*>(&memblock[4]));
			m_Header.fileNameLength = memblock[8];

			std::string str(&memblock[9], (int)m_Header.fileNameLength);
			m_Header.fileName = str; // file name

			m_Data.Decompress(&(memblock[9 + (int)m_Header.fileNameLength]), (int)size - (int)m_Header.fileNameLength - 9, m_Header.fileSize);
			
			std::ofstream arc(m_Header.fileName, std::ios::out | std::ios::binary | std::ios::trunc); // Set up file write mode
			if (arc.is_open())
			{
				// Write to the file
				arc.write(m_Data.m_Data, m_Data.m_Size);
				arc.close();
			}
			else {
				std::cout << "Failed to open the file and write." << std::endl << std::endl;
			}
		}
		else {
			std::cout << "Invalid File Type for Extraction." << std::endl << std::endl;
		}
		
		// Release memblock
		delete[] memblock;
	}
	else {
		std::cout << "Sorry, file name not found!" << std::endl << std::endl;
	}
}

void RLE_v2::CreateArchive(const std::string& source)
{
	
}

void RLE_v2::ExtractArchive(const std::string& source)
{
	
}

RLE_v2::~RLE_v2()
{
	
}
