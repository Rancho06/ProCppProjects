#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "op.h"
#include "exceptions.h"

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
	{ }

	// Active line number in behavior program
	int m_ProgramCounter;
	// Number of actions taken this turn
	int m_ActionsTaken;
	// Current facing of this character
	Facing m_Facing;
	// Test flag for branches
	bool m_Test;

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
	m_Ops.push_back(new OpRotate(0));
	m_Ops.push_back(new OpRotate(0));
	m_Ops.push_back(new OpRotate(1));
	m_Ops.push_back(new OpGoto(1));
	// END TEMP CODE
}

template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.m_ActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.m_MaxMemory = MachineTraits::MEMORY_LOCATIONS;
	state.m_InfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
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
	std::cout << "TAKING TURN" << std::endl;
	state.m_ActionsTaken = 0;
	while (state.m_ActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
		m_Ops.at(state.m_ProgramCounter - 1)->Execute(state);
	}
}
