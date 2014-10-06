/*
@copyright: Shu Zhou, Fall 2014
*/

#pragma once
#include <unordered_map>
#include "pass_struct.h"
#include <string>
#include "sha1.h"
#include <windows.h>
#include <fstream>

// PassLoader class is used to load words in dictionary and store their hash information

class PassLoader
{
	std::unordered_map<std::string, pass_info*>* map;
public:
	PassLoader();
	~PassLoader();

	void basicHash();
	void loadDictionary(const std::string&);
	std::unordered_map<std::string, pass_info*>& getMap();
};

