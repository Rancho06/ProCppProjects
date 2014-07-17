#pragma once
#include <exception>
class InvalidOp : public std::exception
{
	const char* what() const throw() { return "Invalid OP code or parameters."; }
};

class AccessViolation : public std::exception
{
	const char* what() const throw() { return "Machine tried to access outside available memory."; }
};
