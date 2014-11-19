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

class ZombieMemoryException : public std::exception
{
	const char* what() const throw() { return "Zombies cannot use memory"; }
};

class ZombieRangedAttackException : public std::exception
{
	const char* what() const throw() { return "Zombies cannot do a ranged attack"; }
};

class TestDistanceException : public std::exception
{
	const char* what() const throw() { return "test_human or test_zombie can only be within the distance of 1 or 2"; }
};

class InvalidLineException : public std::exception
{
	const char* what() const throw() { return "Tried to access an invalid line number in your source file"; }
};

class SlotOutOfBoundsException : public std::exception
{
	const char* what() const throw() { return "Memory Slot is invalid because out of bounds"; }
};

class InvalidFileException : public std::exception
{
	const char* what() const throw() { return "Invalid .zom source file."; }
};