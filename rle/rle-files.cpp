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
	// Find the first file in the directory.
	WIN32_FIND_DATA ffd;
	std::string path = source + "\\*.*";
	HANDLE hFind = FindFirstFile(path.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		// No files found, display error message and return!
		std::cout << "Error! No files found!" << std::endl << std::endl;
		return;
	}
	// Add all files in this directory to a list of strings
	std::list<std::string> files;
	do
	{
		std::string temp = ffd.cFileName; if (temp != "." && temp != ".." && !(ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		{
			files.push_back(ffd.cFileName);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	m_Header.sig[0] = 'R';
	m_Header.sig[1] = 'L';
	m_Header.sig[2] = 'E';
	m_Header.sig[3] = 0x02;
	m_Header.dirNameLength = source.length();
	m_Header.dirName = source;

	for (auto i = files.begin(); i != files.end(); i++) {
		std::string fileName = *i; // get each filename
		file_info * myFile = new file_info(); // create a new file_info struct to store single file information

		// Requires <fstream>
		std::ifstream::pos_type size;
		char* memblock;

		// Set up the file read mode 
		std::ifstream file(source + "/" + fileName, std::ios::in | std::ios::binary | std::ios::ate);

		// If file name exists
		if (file.is_open())
		{
			size = file.tellg(); // grab the whole contents
			memblock = new char[static_cast<unsigned int>(size)];
			file.seekg(0, std::ios::beg);
			file.read(memblock, size); // File data has now been loaded into memblock array
			file.close();

			myFile->data.Compress(memblock, (int)size);
			myFile->fileSize = (int)size;
			myFile->compressedSize = myFile->data.m_Size;
			myFile->fileNameLength = fileName.size();
			myFile->fileName = fileName;

			m_Files.push_back(myFile);
			delete[] memblock;
			
		}
		else {
			std::cout << "The file "<< fileName <<" cannot be opened." << std::endl << std::endl;
		}
	}

	std::ofstream arc(m_Header.dirName + ".rl2", std::ios::out | std::ios::binary | std::ios::trunc); // Set up file write mode
	if (arc.is_open())
	{
		// Write to the file
		arc.write(m_Header.sig, 4);
		arc.write(reinterpret_cast<char*>(&(m_Header.dirNameLength)), 1);
		arc.write(m_Header.dirName.c_str(), (int)m_Header.dirNameLength);

		for (auto i = m_Files.begin(); i != m_Files.end(); i++) {
			arc.write(reinterpret_cast<char*>(&((*i)->fileSize)),4);
			arc.write(reinterpret_cast<char*>(&((*i)->compressedSize)), 4);
			arc.write(reinterpret_cast<char*>(&((*i)->fileNameLength)), 1);
			arc.write((*i)->fileName.c_str(), (int)(*i)->fileNameLength);
			arc.write((*i)->data.m_Data,(*i)->data.m_Size);
		}	
		arc.close();
	}

}

void RLE_v2::ExtractArchive(const std::string& source)
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
		file.seekg(0, std::ios::beg);
		file.read(memblock, size); // File data has now been loaded into memblock array
		file.close();

		// Load header info to m_Header
		for (int i = 0; i < 4; i++) {
			m_Header.sig[i] = memblock[i];
		}
		// Check if the file signature is correct
		if (m_Header.sig[0] == 'R' && m_Header.sig[1] == 'L' && m_Header.sig[2] == 'E' && m_Header.sig[3] == 0x02) {

			m_Header.dirNameLength = memblock[4];
			std::string dirName(&memblock[5], (int)m_Header.dirNameLength);
			m_Header.dirName = dirName;

			int index = (int)m_Header.dirNameLength + 5;

			while (index < (int)size) {
				file_info* myFile = new file_info();
				// file size
				myFile->fileSize = *(reinterpret_cast<int*>(&memblock[index]));
				index += 4;

				// compressed size
				myFile->compressedSize = *(reinterpret_cast<int*>(&memblock[index]));
				index += 4;

				// file name length
				myFile->fileNameLength = memblock[index];
				index++;

				// file name
				std::string fileName(&(memblock[index]), (int)myFile->fileNameLength);
				myFile->fileName = fileName;
				index += (int)myFile->fileNameLength;

				// decompress data
				myFile->data.Decompress(&(memblock[index]), myFile->compressedSize, myFile->fileSize);
				index += myFile->compressedSize;
				m_Files.push_back(myFile);
			}

			CreateDirectory(m_Header.dirName.c_str(), NULL);

			for (auto myFile = m_Files.begin(); myFile != m_Files.end(); myFile++) {
				std::ofstream arc(m_Header.dirName + "/" + (*myFile)->fileName, std::ios::out | std::ios::binary | std::ios::trunc); // Set up file write mode
				if (arc.is_open())
				{
					// Write data to the file
					arc.write((*myFile)->data.m_Data, (*myFile)->data.m_Size);
					arc.close();
				}
				else {
					std::cout << "Failed to open the file and write." << std::endl << std::endl;
				}
			}
		}
		else {
			std::cout << "Invalid File Type for Directory Extraction." << std::endl << std::endl;
		}
			

		// Release memblock
		delete[] memblock;
	}
	else {
		std::cout << "Sorry, file name not found!" << std::endl << std::endl;
	}
}

RLE_v2::~RLE_v2()
{
	//delete file_info structs
	for (auto i = m_Files.begin(); i != m_Files.end(); i++) {
		delete *i;
	}
}
