/*
@copyright: Shu Zhou, Fall 2014
*/

#include "PassDecoder.h"

PassDecoder::PassDecoder(){}

// convert an int array to a string using 36-based. a-z:0-25, 0-9:26-35.
std::string convertToString(int* array, const int size) {
	char* password = new char[size];
	for (int i = 0; i < size; i++) {
		if (array[i] < 26) {
			password[i] = array[i] + 97;
		}
		else {
			password[i] = array[i] + 22;
		}
	}
	std::string passString(password, size);
	delete password;
	return passString;
}

// takes a string as a password and stores its hash & hex information into object
void doHash(const std::string& password, pass_info* info) {
	info->origin = password;
	sha1::calc(password.c_str(), password.length(), info->hashed);
	sha1::toHexString(info->hashed, info->hex);
}

// compare given hex string with the hex value inside unsolvedList
void compareHex(std::vector<decrypt_info*>& list, const std::string& hex, int start, int end, pass_info& object) {
	if (start + 1 >= end) {
		if (list.at(start)->hexString == hex) {
			list.at(start)->origin = object.origin;
		}
		return;
	}
	Concurrency::parallel_invoke(
		[&list, &hex, start, end, &object]{compareHex(list, hex, start, (start + end) / 2, object); },
		[&list, &hex, start, end, &object]{compareHex(list, hex, (start + end) / 2, end, object); }
	);
}

// write all the hex and password into a file
void outputToFile(std::map<int, decrypt_info*>& map) {
	std::ofstream fileOutput;
	fileOutput.open("pass_solved.txt");
	for (auto it = map.begin(); it != map.end(); it++) {
		fileOutput << (it->second)->hexString;
		fileOutput << ", ";
		fileOutput << (it->second)->origin;
		fileOutput << std::endl;
	}
	fileOutput.close();
}

PassDecoder::~PassDecoder(){}

// convert "start" into a string and check that string
void computeHex(int max, int start, int end, std::vector<decrypt_info*>& unsolvedList) {
	if (start + 1 >= end) {
		int* array = new int[max];
		for (int i = 0; i < max; i++) {
			int quotient = (int)(std::pow(CHAR_TYPE_COUNT, i));
			array[i] = (start / quotient) % CHAR_TYPE_COUNT;
		}
		std::string password = convertToString(array, max);
		pass_info passObject;
		doHash(password, &passObject);
		std::string hex(passObject.hex, 40);
		compareHex(unsolvedList, hex, 0, unsolvedList.size(), passObject);
		delete[] array;
		return;
	}
	int step = (end - start) / 6;
	Concurrency::parallel_invoke(
		[start, step, max, &unsolvedList]{computeHex(max, start, start + step, unsolvedList); },
		[start, step, max, &unsolvedList]{computeHex(max, start + step, start + step * 2, unsolvedList); },
		[start, step, max, &unsolvedList]{computeHex(max, start + step * 2, start + step * 3, unsolvedList); },
		[start, step, max, &unsolvedList]{computeHex(max, start + step * 3, start + step * 4, unsolvedList); },
		[start, step, max, &unsolvedList]{computeHex(max, start + step * 4, start + step * 5, unsolvedList); },
		[start, step, max, &unsolvedList]{computeHex(max, start + step * 5, start + step * 6, unsolvedList); }
	);
}

// handle the whole process of password decryption
void PassDecoder::decrypt(const std::string& fileName, const std::unordered_map< std::string, pass_info*>& map) {
	std::string content;
	std::ifstream fileInput;
	int entryNumber = 0;
	std::map<int, decrypt_info*> solvedMap;
	std::vector<decrypt_info*> unsolvedList;
	fileInput.open(fileName);
	if (fileInput.is_open()) {
		entryNumber = 0;
		while (!fileInput.eof()) {
			std::getline(fileInput, content);
			auto it = map.find(content);
			decrypt_info* info = new decrypt_info();
			info->entryNumber = entryNumber;
			info->hexString = content;
			if (it != map.end()) {
				info->origin = map.at(content)->origin;
			}
			else {
				info->origin = "????";
				unsolvedList.push_back(info);
			}
			solvedMap.insert(std::make_pair(entryNumber, info));
			entryNumber++;
		}
		fileInput.close();
		// start timing
		LARGE_INTEGER freq, before, after;
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&before);

		for (int max = 1; max <= MAX_DIGIT; max++) {
			int totalCount = (int)std::pow(CHAR_TYPE_COUNT, max);
			computeHex(max, 0, totalCount, unsolvedList);
		}

		QueryPerformanceCounter(&after);
		float fElapsed = static_cast<float>(after.QuadPart - before.QuadPart) / freq.QuadPart;
		// end timing
		std::cout << "Time elapsed: " << fElapsed << std::endl << std::endl;
		outputToFile(solvedMap);
	}
	else {
		std::cout << "Failed to open the file: " << fileName << std::endl << std::endl;
	}
	// release heap memory
	for (auto it = solvedMap.begin(); it != solvedMap.end(); it++) {
		delete it->second;
	}
}