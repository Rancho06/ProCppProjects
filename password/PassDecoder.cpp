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
void compareHex(decrypt_info* info, const std::string& hex, pass_info& object) {
	if (info->hexString == hex) {
		info->origin = object.origin;
	}
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

// convert integers into corresponding strings using given algorithm
void computeHex(int size, int start, int end, std::vector<decrypt_info*>& list) {
	int* array = new int[size];
	for (int MSB = start; MSB < end; MSB++) {
		array[size - 1] = MSB;
		for (int i = size - 2; i >= 0; i--) {
			array[i] = 0;
		}
		bool isMax;
		do {
			isMax = true;
			for (int i = 0; i < size - 1; i++) {
				if (array[i] == 36) {
					array[i] = 0;
					array[i + 1] += 1;
				}
				isMax = isMax && (array[i] == 35);
			}
			std::string password = convertToString(array, size);
			pass_info passObject;
			doHash(password, &passObject);
			std::string hex(passObject.hex, 40);
			Concurrency::parallel_for_each(list.begin(), list.end(), [hex, &passObject](decrypt_info* info){
				compareHex(info, hex, passObject);
			});
			++(array[0]);
		} while (!isMax);
	}
	delete[] array;
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
			Concurrency::parallel_invoke(
				[max, &unsolvedList]{computeHex(max, 0, 6, unsolvedList); },
				[max, &unsolvedList]{computeHex(max, 6, 12, unsolvedList); },
				[max, &unsolvedList]{computeHex(max, 12, 18, unsolvedList); },
				[max, &unsolvedList]{computeHex(max, 18, 24, unsolvedList); },
				[max, &unsolvedList]{computeHex(max, 24, 30, unsolvedList); },
				[max, &unsolvedList]{computeHex(max, 30, 36, unsolvedList); }
			);
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