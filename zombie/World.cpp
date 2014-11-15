#include "stdafx.h"
#include "World.h"


World::World()
{
	array = new int*[SIZE]();
	for (int i = 0; i < SIZE; ++i) {
		array[i] = new int[SIZE];
		for (int j = 0; j < SIZE; ++j) {
			array[i][j] = 0;
		}
	}
	srand((unsigned int)time(NULL));
	
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