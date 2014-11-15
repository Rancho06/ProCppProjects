#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "op.h"
#include "exceptions.h"
#include <algorithm>
#include <assert.h>

// Defines state data accessible by the machine and ops
struct MachineState
{
	// Friend Machine so BindState works
	template <typename MachineTraits>
	friend class Machine;
	
	enum Facing { UP, RIGHT, DOWN, LEFT};
	MachineState()
		: m_ProgramCounter(1)
		, m_ActionsTaken(0)
		, m_Facing(UP)
		, m_Test(false)
		, m_Memory(nullptr)
		, m_XPos(0)
		, m_YPos(0)
		, m_currentLoc(0)
	{ 
	}

	// Active line number in behavior program
	int m_ProgramCounter;
	// Number of actions taken this turn
	int m_ActionsTaken;
	// Current facing of this character
	Facing m_Facing;
	// Test flag for branches
	bool m_Test;
	// add additional location info
	int m_XPos;
	int m_YPos;
	int m_label;
	int m_currentLoc;

	int GetActionsPerTurn() const throw() { return m_ActionsPerTurn; }
	int GetMaxMemory() const throw() {return m_MaxMemory; }
	bool GetInfect() const throw() { return m_InfectOnAttack; }
	int GetMemory(int location)
	{
		// FIXME: Throw exception if out of bounds or memory is not allocated
		return m_Memory[location];
	}
	void SetMemory(int location, int value)
	{
		m_Memory[location] = value;
	}
private:
	// Data which is set by the traits
	int m_ActionsPerTurn;
	int m_MaxMemory;
	bool m_InfectOnAttack;
	int* m_Memory;

};

// Describes the machine which processes ops.
// Different policies dictate behavior possible for machine.
template <typename MachineTraits>
class Machine
{
public:
	// Load in all the ops for this machine from the specified file
	void LoadMachine(std::string& filename);

	// Given the state, binds the trait parameters to it
	void BindState(MachineState& state);

	// Take a turn using this logic for the passed in state
	void TakeTurn(MachineState& state);
private:
	std::vector<Op*> m_Ops;
};

template <typename MachineTraits>
void Machine<MachineTraits>::LoadMachine(std::string& filename)
{
	// TEMP CODE: Add your parsing code here!
	m_Ops.clear();
	/*
	m_Ops.push_back(new OpRotate(0));
	m_Ops.push_back(new OpRotate(0));
	m_Ops.push_back(new OpRotate(1));
	m_Ops.push_back(new OpGoto(1));
	*/
	
	std::ifstream inputHandler(filename);
	if (inputHandler.is_open()) {
		while (!inputHandler.eof()) {
			std::string opString;
			std::getline(inputHandler, opString);
			int semiIndex = opString.find(';');
			if (semiIndex != std::string::npos) {
				opString = opString.substr(0, semiIndex);
			}
			std::string firstOpString = "", secondOpString = "";
			int commaIndex = opString.find(',');
			if (commaIndex != std::string::npos) {
				firstOpString = opString.substr(0, commaIndex);
				firstOpString.erase(remove_if(firstOpString.begin(), firstOpString.end(), isspace), firstOpString.end());
				secondOpString = opString.substr(commaIndex + 1);
				secondOpString.erase(remove_if(secondOpString.begin(), secondOpString.end(), isspace), secondOpString.end());
			}
			else {
				firstOpString = opString;
				firstOpString.erase(remove_if(firstOpString.begin(), firstOpString.end(), isspace), firstOpString.end());
			}

			if (firstOpString == "goto") {
				m_Ops.push_back(new OpGoto(atoi(secondOpString.c_str())));
			}
			else if (firstOpString == "rotate") {
				m_Ops.push_back(new OpRotate(atoi(secondOpString.c_str())));
			}
			else if (firstOpString == "endturn") {
				m_Ops.push_back(new OpEndturn());
			}
			else if (firstOpString == "attack") {
				m_Ops.push_back(new OpAttack());
			}
			else if (firstOpString == "je") {
				m_Ops.push_back(new OpJe(atoi(secondOpString.c_str())));
			}
			else if (firstOpString == "jne") {
				m_Ops.push_back(new OpJne(atoi(secondOpString.c_str())));
			}
			else if (firstOpString == "forward") {
				m_Ops.push_back(new OpForward());
			}
			else if (firstOpString == "test_zombie") {
				m_Ops.push_back(new OpTestZombie(atoi(secondOpString.c_str())));
			}
			else if (firstOpString == "test_passable") {
				m_Ops.push_back(new OpTestPassable());
			}
			else if (firstOpString == "test_random") {
				m_Ops.push_back(new OpTestRandom());
			}
			else if (firstOpString == "test_human") {
				m_Ops.push_back(new OpTestHuman(atoi(secondOpString.c_str())));
			}
			else if (firstOpString == "test_wall") {
				m_Ops.push_back(new OpTestWall());
			}
			else if (firstOpString == "save_loc") {
				m_Ops.push_back(new OpSaveLoc());
			}
			else if (firstOpString == "ranged_attack") {
				m_Ops.push_back(new OpRangedAttack());
			}
			else if (firstOpString == "mem") {
				m_Ops.push_back(new OpMem(atoi(secondOpString.c_str())));
			}
			else if (firstOpString == "set") {
				m_Ops.push_back(new OpSet(atoi(secondOpString.c_str())));
			}
			else if (firstOpString == "inc") {
				m_Ops.push_back(new OpInc());
			}
			else if (firstOpString == "dec") {
				m_Ops.push_back(new OpDec());
			}
			else if (firstOpString == "test_mem") {
				m_Ops.push_back(new OpTestMem(atoi(secondOpString.c_str())));
			}

		}
	}
	else {
		std::cout << "File Not Found" << std::endl;
	}
	for (unsigned int i = 0; i < m_Ops.size(); ++i) {
		std::cout << m_Ops[i]->m_OpName << " " << m_Ops[i]->m_Param << std::endl;
	}
	// END TEMP CODE
}

template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.m_ActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.m_MaxMemory = MachineTraits::MEMORY_LOCATIONS;
	state.m_InfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
	if (state.m_InfectOnAttack) {
		state.m_label = 1;
	}
	else {
		state.m_label = 2;
	}

	// Not sure if this is right
	state.m_ProgramCounter = 1;

	delete state.m_Memory;
	state.m_Memory = new int[state.m_MaxMemory];
	for (int i = 0; i < state.m_MaxMemory; i++)
	{
		state.m_Memory[i] = 0;
	}
}

template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
	//std::cout << "TAKING TURN" << std::endl;
	state.m_ActionsTaken = 0;
	while (state.m_ActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
		m_Ops.at(state.m_ProgramCounter - 1)->Execute(state);
	}
}
