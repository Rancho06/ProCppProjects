#include <iostream>

struct pass_info {
	std::string origin;
	unsigned char hashed[20];
	char hex[41];
};