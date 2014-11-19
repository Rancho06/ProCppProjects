#pragma once

#include "singleton.h"
#include "machine.h"
#include "traits.h"
#include <list>
#include <ctime>

class World :
	public Singleton<World>
{
public:
	World();
	int getRandom();
	void incrementTurn();
	void resetTurnCount();
	int getTurnCount();
	~World();

	const int SIZE = 20;
	int** array;
	std::list<MachineState> zombieStateLists;
	std::list<MachineState> humanStateLists;
	Machine<ZombieTraits> zombieMachine;
	Machine<HumanTraits> humanMachine;
	bool isRunning;
	std::string zombieFileName;
	std::string humanFileName;

private:
	int turnCount;
};

