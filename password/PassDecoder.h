#pragma once
#include <map>
#include "pass_struct.h"
#include <fstream>
#include <string>
#include <list>
#include <unordered_map>
#include <windows.h>
#include <ppl.h>
#include <algorithm>
#include "sha1.h"

const int MAX_DIGIT = 4;
const int CHAR_TYPE_COUNT = 36;
class PassDecoder
{
public:
	PassDecoder();
	~PassDecoder();

	void decrypt(const std::string&, std::unordered_map< std::string, pass_info*> &);
};

