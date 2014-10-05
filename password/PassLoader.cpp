#include "PassLoader.h"


PassLoader::PassLoader()
{
	this->map = new std::unordered_map<std::string, pass_info*>();
}

std::unordered_map<std::string, pass_info*>& PassLoader::getMap() {
	return *(this->map);
}


void hashProcess(const std::string& password, pass_info* info) {
	info->origin = password;
	sha1::calc(password.c_str(), password.length(), info->hashed);
	sha1::toHexString(info->hashed, info->hex);
}


void PassLoader::basicHash() {
	std::string password;
	pass_info* info = new pass_info();
	do {
		std::cout << "Please enter a password you want to hash: ";
		std::getline(std::cin, password);
		if (password.empty()) {
			std::cout << "Password cannot be empty.";
		}
	} while (password.empty());

	hashProcess(password, info);

	std::cout << "Hashed: " << info->hex << std::endl << std::endl;
	delete info;
}


void PassLoader::loadDictionary(const std::string& fileName) {

	LARGE_INTEGER freq, before, after;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&before);

	std::string content;
	std::ifstream fileInput;
	this->map->clear();
	this->map->rehash(100000);
	fileInput.open(fileName);
	if (fileInput.is_open()) {
		while (!fileInput.eof()) {
			std::getline(fileInput, content);
			pass_info* info = new pass_info();
			hashProcess(content, info);
			this->map->insert(std::make_pair((std::string)info->hex, info));
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




PassLoader::~PassLoader()
{
	delete this->map;
}

