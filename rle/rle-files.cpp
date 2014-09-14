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
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg);
		file.read(memblock, size);
		file.close();
		// File data has now been loaded into memblock array

		m_Data.Compress(memblock, (int)size);

		std::cout << "Compression: " << (double)((int)size - m_Data.m_Size) * 100 / ((int)size) << "%." << std::endl << std::endl;

		m_Header.sig[0] = 'R';
		m_Header.sig[1] = 'L';
		m_Header.sig[2] = 'E';
		m_Header.sig[3] = 0x01;
		m_Header.fileSize = static_cast<int>(size);
		m_Header.fileNameLength = source.length();
		m_Header.fileName = source;
		
		std::ofstream arc(source+".rl1", std::ios::out | std::ios::binary | std::ios::trunc);
		if (arc.is_open())
		{
			// Use arc.write function to write data here
			arc.write(m_Header.sig,4);
			arc.write(reinterpret_cast<char*>(&(m_Header.fileSize)), 4);
			arc.write(reinterpret_cast<char*>(&(m_Header.fileNameLength)), 1);
			arc.write(m_Header.fileName.c_str(),m_Header.fileNameLength);
			arc.write(m_Data.m_Data, m_Data.m_Size);
			arc.close();
		}
		// Make sure to clean up!
		delete[] memblock;
	}
	else {
		std::cout << "Sorry, file name not found!" << std::endl << std::endl;
	}
}

void RLE_v1::ExtractArchive(const std::string& source)
{
	
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
