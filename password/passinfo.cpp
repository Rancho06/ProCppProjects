#include <iostream>

struct pass_info {
	std::string origin;
	unsigned char hashed[20];
	char hex[41];
};

struct decrypt_info {
	long long entryNumber;
	std::string hexString;
	std::string origin;
};