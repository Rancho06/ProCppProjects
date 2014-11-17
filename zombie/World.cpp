#include "stdafx.h"
#include "World.h"

World::World()
{
	array = new int*[SIZE]();
	isRunning = false;
	turnCount = 0;
	for (int i = 0; i < SIZE; ++i) {
		array[i] = new int[SIZE];
		for (int j = 0; j < SIZE; ++j) {
			array[i][j] = 0;
		}
	}
	srand((unsigned int)time(NULL));
	zombieFileName = "";
	humanFileName = "";
	
}


World::~World()
{
	for (int i = 0; i < SIZE; ++i) {
		delete[] array[i];
	}
	delete[] array;
}

int World::getRandom() {
	return rand();
}

int World::getTurnCount() {
	return turnCount;
}

void World::incrementTurn() {
	++turnCount;
}

void World::resetTurnCount() {
	turnCount = 0;
}