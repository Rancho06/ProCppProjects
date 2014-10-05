#ifndef PASS_STRUCT_H
#define PASS_STRUCT_H


#include <iostream>

struct pass_info {
	std::string origin;
	unsigned char hashed[20];
	char hex[41];
};

struct decrypt_info {
	int entryNumber;
	std::string hexString;
	std::string origin;
};

#endif