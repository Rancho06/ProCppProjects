#include "PassDecoder.h"


PassDecoder::PassDecoder()
{
}


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

void doHash(const std::string& password, pass_info* info) {
	info->origin = password;
	sha1::calc(password.c_str(), password.length(), info->hashed);
	sha1::toHexString(info->hashed, info->hex);
}


void getPasswords(std::unordered_map<std::string, pass_info*>& map, int size, int initial, int end, int* array) {

	for (int count = initial; count < end; count++) {
		for (int i = 0; i < size; i++) {
			int quotient = (int)(std::pow(CHAR_TYPE_COUNT, i));
			array[i] = (count / quotient) % CHAR_TYPE_COUNT;
		}
		std::string password = convertToString(array, size);
		pass_info* passObject = new pass_info();
		doHash(password, passObject);
		map.insert(std::make_pair(passObject->hex, passObject));
	}
}


void getALLPasswords(std::unordered_map<std::string, pass_info*>& map) {

	for (int max = 1; max <= MAX_DIGIT; max++) {
		int* array = new int[max];
		for (int i = 0; i < max; i++) {
			array[i] = 0;
		}
		int totalCount = (int)std::pow(CHAR_TYPE_COUNT, max);
		Concurrency::parallel_invoke(
			[totalCount, max, &map, array]{getPasswords(map, max, 0, totalCount / 6, array); },
			[totalCount, max, &map, array]{getPasswords(map, max, totalCount / 6, totalCount / 3, array); },
			[totalCount, max, &map, array]{getPasswords(map, max, totalCount / 3, totalCount / 2, array); },
			[totalCount, max, &map, array]{getPasswords(map, max, totalCount / 2, (totalCount * 2) / 3, array); },
			[totalCount, max, &map, array]{getPasswords(map, max, (totalCount * 2) / 3, (totalCount * 5) / 6, array); },
			[totalCount, max, &map, array]{getPasswords(map, max, (totalCount * 5) / 6, totalCount, array); }
		);

		delete[] array;
	}

}


void bruteForce(std::list<decrypt_info*>& list) {

	std::unordered_map<std::string, pass_info*> passMap;
	passMap.rehash(2000000);
	LARGE_INTEGER freq, before, after;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&before);

	getALLPasswords(passMap);
	Concurrency::parallel_for_each(list.begin(), list.end(), [&passMap](decrypt_info* info){
		if (passMap.find(info->hexString) != passMap.end()) {
			info->origin = (passMap.at(info->hexString))->origin;
		}
		else {
			info->origin = "????";
		}
	});

	QueryPerformanceCounter(&after);
	float fElapsed = static_cast<float>(after.QuadPart - before.QuadPart) / freq.QuadPart;

	std::cout << "Time elapsed: " << fElapsed << std::endl << std::endl;
}


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

PassDecoder::~PassDecoder()
{
}

void PassDecoder::decrypt(const std::string& fileName, std::unordered_map< std::string, pass_info*>& map) {
	std::string content;
	std::ifstream fileInput;
	int entryNumber = 0;
	std::map<int, decrypt_info*> solvedMap;
	std::list<decrypt_info*> unsolvedList;
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
				unsolvedList.push_back(info);
			}
			solvedMap.insert(std::make_pair(entryNumber, info));

			entryNumber++;

		}
		fileInput.close();
		bruteForce(unsolvedList);
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