#include "stdafx.h"
#include "op.h"
#include "machine.h"
#include <iostream>
#include "World.h"

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
	std::cout << state.m_ProgramCounter << ":" << m_OpName << "," << m_Param << std::endl;
}

void OpRotate::Execute(MachineState& state) throw()
{
	//DebugOutput(state);
	switch (state.m_Facing)
	{
	case (MachineState::UP):
		if (m_Param == 0)
		{
			state.m_Facing = MachineState::RIGHT;
		}
		else
		{
			state.m_Facing = MachineState::LEFT;
		}
		break;
	case (MachineState::RIGHT):
		if (m_Param == 0)
		{
			state.m_Facing = MachineState::DOWN;
		}
		else
		{
			state.m_Facing = MachineState::UP;
		}
		break;
	case (MachineState::DOWN):
		if (m_Param == 0)
		{
			state.m_Facing = MachineState::LEFT;
		}
		else
		{
			state.m_Facing = MachineState::RIGHT;
		}
		break;
	default:
	case (MachineState::LEFT):
		if (m_Param == 0)
		{
			state.m_Facing = MachineState::UP;
		}
		else
		{
			state.m_Facing = MachineState::DOWN;
		}
		break;
	}

	++state.m_ProgramCounter;
	++state.m_ActionsTaken;
}

void OpGoto::Execute(MachineState& state)
{
	//DebugOutput(state);
	switch (state.m_label) {
	case 1:
		if (m_Param > World::get().zombieMachine.getOpSize()) {
			World::get().array[state.m_XPos][state.m_YPos] = 0;
			World::get().zombieStateLists.remove(state);
			throw InvalidLineException();
		}
		break;
	case 2:
		if (m_Param > World::get().humanMachine.getOpSize()) {
			World::get().array[state.m_XPos][state.m_YPos] = 0;
			World::get().humanStateLists.remove(state);
			throw InvalidLineException();
		}
		break;
	default:
		break;
	}
	state.m_ProgramCounter = m_Param;
	
}

void humanAttack(int x, int y) {
	if ((x >= 0) && (x <= 19) && (y >= 0) && (y <= 19)) {
		if (World::get().array[x][y] != 0) {
			World::get().array[x][y] = 0;
			for (auto it = World::get().zombieStateLists.begin(); it != World::get().zombieStateLists.end(); ++it) {
				if (((*it).m_XPos == x) && ((*it).m_YPos == y)) {
					World::get().zombieStateLists.erase(it);
					return;
				}
			}
			for (auto it = World::get().humanStateLists.begin(); it != World::get().humanStateLists.end(); ++it) {
				if (((*it).m_XPos == x) && ((*it).m_YPos == y)) {
					World::get().humanStateLists.erase(it);
					return;
				}
			}
		}
	}
}

void zombieAttack(int x, int y) {
	if ((x >= 0) && (x <= 19) && (y >= 0) && (y <= 19)) {
		if (World::get().array[x][y] == 2) {
			World::get().array[x][y] = 1;
			for (auto it = World::get().humanStateLists.begin(); it != World::get().humanStateLists.end(); ++it) {
				if (((*it).m_XPos == x) && ((*it).m_YPos == y)) {
					World::get().zombieMachine.BindState(*it);
					World::get().zombieStateLists.push_back(*it);
					World::get().humanStateLists.erase(it);				
					return;
				}
			}
		}
	}
}

void OpAttack::Execute(MachineState& state) throw()
{
	//DebugOutput(state);
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		if (state.m_label == 2) {
			humanAttack(state.m_XPos, state.m_YPos - 1);
		}
		else {
			zombieAttack(state.m_XPos, state.m_YPos - 1);
		}
		break;
	case (MachineState::RIGHT) :
		if (state.m_label == 2) {
			humanAttack(state.m_XPos + 1, state.m_YPos);
		}
		else {
			zombieAttack(state.m_XPos + 1, state.m_YPos);
		}
		break;
	case (MachineState::DOWN) :
		if (state.m_label == 2) {
			humanAttack(state.m_XPos, state.m_YPos + 1);
		}
		else {
			zombieAttack(state.m_XPos, state.m_YPos + 1);
		}
		
		break;
	default:
	case (MachineState::LEFT) :
		if (state.m_label == 2) {
			humanAttack(state.m_XPos - 1, state.m_YPos);
		}
		else {
			zombieAttack(state.m_XPos - 1, state.m_YPos);
		}	
		break;
	}
	++state.m_ProgramCounter;
	++state.m_ActionsTaken;
}

void OpRangedAttack::Execute(MachineState& state)
{
	//DebugOutput(state);
	if (state.m_label == 1) {
		throw ZombieRangedAttackException();
	}
	if (state.m_label == 2) {
		switch (state.m_Facing)
		{
		case (MachineState::UP) :
			humanAttack(state.m_XPos, state.m_YPos - 2);
			break;
		case (MachineState::RIGHT) :
			humanAttack(state.m_XPos + 2, state.m_YPos);
			break;
		case (MachineState::DOWN) :
			humanAttack(state.m_XPos, state.m_YPos + 2);
			break;
		default:
		case (MachineState::LEFT) :
			humanAttack(state.m_XPos - 2, state.m_YPos);
			break;
		}
	}
	++state.m_ProgramCounter;
	++state.m_ActionsTaken;
}

void OpForward::Execute(MachineState& state) throw()
{
	//DebugOutput(state);
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		if ((state.m_YPos > 0) && (World::get().array[state.m_XPos][state.m_YPos - 1] == 0)) {
			World::get().array[state.m_XPos][state.m_YPos] = 0;
			--state.m_YPos;
			World::get().array[state.m_XPos][state.m_YPos] = state.m_label;
		}
		break;
	case (MachineState::RIGHT) :
		if ((state.m_XPos < 19) && (World::get().array[state.m_XPos + 1][state.m_YPos] == 0)) {
			World::get().array[state.m_XPos][state.m_YPos] = 0;
			++state.m_XPos;
			World::get().array[state.m_XPos][state.m_YPos] = state.m_label;
		}
		break;
	case (MachineState::DOWN) :
		if ((state.m_YPos < 19) && (World::get().array[state.m_XPos][state.m_YPos + 1] == 0)) {
			World::get().array[state.m_XPos][state.m_YPos] = 0;
			++state.m_YPos;
			World::get().array[state.m_XPos][state.m_YPos] = state.m_label;
		}
		break;
	default:
	case (MachineState::LEFT) :
		if ((state.m_XPos > 0) && (World::get().array[state.m_XPos - 1][state.m_YPos] == 0)) {
			World::get().array[state.m_XPos][state.m_YPos] = 0;
			--state.m_XPos;
			World::get().array[state.m_XPos][state.m_YPos] = state.m_label;
		}
		break;
	}
	++state.m_ProgramCounter;
	++state.m_ActionsTaken;
}

void OpEndturn::Execute(MachineState& state) throw()
{
	//DebugOutput(state);
	++state.m_ProgramCounter;
	state.m_ActionsTaken = state.GetActionsPerTurn();
}

void OpTestHuman::Execute(MachineState& state)
{
	//DebugOutput(state);
	if ((m_Param != 1) && (m_Param != 2)) {
		throw TestDistanceException();
	}
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		state.m_Test = ((state.m_YPos > m_Param - 1) && (World::get().array[state.m_XPos][state.m_YPos - m_Param] == 2));
		break;
	case (MachineState::RIGHT) :
		state.m_Test = ((state.m_XPos < 20 - m_Param) && (World::get().array[state.m_XPos + m_Param][state.m_YPos] == 2));
		break;
	case (MachineState::DOWN) :
		state.m_Test = ((state.m_YPos < 20 - m_Param) && (World::get().array[state.m_XPos][state.m_YPos + m_Param] == 2));
		break;
	default:
	case (MachineState::LEFT) :
		state.m_Test = ((state.m_XPos > m_Param - 1) && (World::get().array[state.m_XPos - m_Param][state.m_YPos] == 2));
		break;
	}
	++state.m_ProgramCounter;
}

void OpTestZombie::Execute(MachineState& state)
{
	//DebugOutput(state);
	if ((m_Param != 1) && (m_Param != 2)) {
		throw TestDistanceException();
	}
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		state.m_Test = ((state.m_YPos > m_Param - 1) && (World::get().array[state.m_XPos][state.m_YPos - m_Param] == 1));
		break;
	case (MachineState::RIGHT) :
		state.m_Test = ((state.m_XPos < 20 - m_Param) && (World::get().array[state.m_XPos + m_Param][state.m_YPos] == 1));
		break;
	case (MachineState::DOWN) :
		state.m_Test = ((state.m_YPos < 20 - m_Param) && (World::get().array[state.m_XPos][state.m_YPos + m_Param] == 1));
		break;
	default:
	case (MachineState::LEFT) :
		state.m_Test = ((state.m_XPos > m_Param - 1) && (World::get().array[state.m_XPos - m_Param][state.m_YPos] == 1));
		break;
	}
	++state.m_ProgramCounter;
}

void OpTestWall::Execute(MachineState& state) throw()
{
	//DebugOutput(state);
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		state.m_Test = (state.m_YPos == 0);
		break;
	case (MachineState::RIGHT) :
		state.m_Test = (state.m_XPos == 19);
		break;
	case (MachineState::DOWN) :
		state.m_Test = (state.m_YPos == 19);
		break;
	default:
	case (MachineState::LEFT) :
		state.m_Test = (state.m_XPos == 0);
		break;
	}
	++state.m_ProgramCounter;
}

void OpTestPassable::Execute(MachineState& state) throw()
{
	//DebugOutput(state);
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		state.m_Test = ((state.m_YPos > 0) && (World::get().array[state.m_XPos][state.m_YPos - 1] == 0));
		break;
	case (MachineState::RIGHT) :
		state.m_Test = ((state.m_XPos < 19) && (World::get().array[state.m_XPos + 1][state.m_YPos] == 0));
		break;
	case (MachineState::DOWN) :
		state.m_Test = ((state.m_YPos < 19) && (World::get().array[state.m_XPos][state.m_YPos + 1] == 0));
		break;
	default:
	case (MachineState::LEFT) :
		state.m_Test = ((state.m_XPos > 0) && (World::get().array[state.m_XPos - 1][state.m_YPos] == 0));
		break;
	}
	++state.m_ProgramCounter;
}

void OpTestRandom::Execute(MachineState& state) throw()
{
	//DebugOutput(state);
	if (World::get().getRandom() % 2 == 0) {
		state.m_Test = false;
	}
	else {
		state.m_Test = true;
	}
	++state.m_ProgramCounter;
}

void OpJe::Execute(MachineState& state)
{
	//DebugOutput(state);
	switch (state.m_label) {
	case 1:
		if (m_Param > World::get().zombieMachine.getOpSize()) {
			throw InvalidLineException();
		}
		break;
	case 2:
		if (m_Param > World::get().humanMachine.getOpSize()) {
			throw InvalidLineException();
		}
		break;
	default:
		break;
	}
	if (state.m_Test) {
		state.m_ProgramCounter = m_Param;
	}
	else {
		++state.m_ProgramCounter;
	}

}

void OpJne::Execute(MachineState& state)
{
	//DebugOutput(state);
	switch (state.m_label) {
	case 1:
		if (m_Param > World::get().zombieMachine.getOpSize()) {
			throw InvalidLineException();
		}
		break;
	case 2:
		if (m_Param > World::get().humanMachine.getOpSize()) {
			throw InvalidLineException();
		}
		break;
	default:
		break;
	}
	if (!state.m_Test) {
		state.m_ProgramCounter = m_Param;
	}
	else {
		++state.m_ProgramCounter;
	}
}

void OpMem::Execute(MachineState& state)
{
	//DebugOutput(state);
	if (state.m_label == 1) {
		throw ZombieMemoryException();
	}
	if ((m_Param != 0) && (m_Param != 1)) {
		throw SlotOutOfBoundsException();
	}
	if (m_Param < state.GetMaxMemory()) {
		state.m_currentLoc = m_Param;
	}
	++state.m_ProgramCounter;
}

void OpSet::Execute(MachineState& state)
{
	//DebugOutput(state);
	if (state.m_label == 1) {
		throw ZombieMemoryException();
	}
	if ((state.m_currentLoc != 0) && (state.m_currentLoc != 1)) {
		throw SlotOutOfBoundsException();
	}
	state.SetMemory(state.m_currentLoc, m_Param);
	++state.m_ProgramCounter;
}

void OpInc::Execute(MachineState& state)
{
	//DebugOutput(state);
	if (state.m_label == 1) {
		throw ZombieMemoryException();
	}
	if ((state.m_currentLoc != 0) && (state.m_currentLoc != 1)) {
		throw SlotOutOfBoundsException();
	}
	state.SetMemory(state.m_currentLoc, state.GetMemory(state.m_currentLoc) + 1);
	++state.m_ProgramCounter;
}

void OpDec::Execute(MachineState& state)
{
	//DebugOutput(state);
	if (state.m_label == 1) {
		throw ZombieMemoryException();
	}
	if ((state.m_currentLoc != 0) && (state.m_currentLoc != 1)) {
		throw SlotOutOfBoundsException();
	}
	state.SetMemory(state.m_currentLoc, state.GetMemory(state.m_currentLoc) - 1);
	++state.m_ProgramCounter;
}

void OpTestMem::Execute(MachineState& state)
{
	//DebugOutput(state);
	if (state.m_label == 1) {
		throw ZombieMemoryException();
	}
	if ((state.m_currentLoc != 0) && (state.m_currentLoc != 1)) {
		throw SlotOutOfBoundsException();
	}
	state.m_Test = (state.GetMemory(state.m_currentLoc) == m_Param);
	++state.m_ProgramCounter;
}

void OpSaveLoc::Execute(MachineState& state)
{
	//DebugOutput(state);
	if (state.m_label == 1) {
		throw ZombieMemoryException();
	}
	state.SetMemory(0, state.m_XPos);
	state.SetMemory(1, state.m_YPos);
	++state.m_ProgramCounter;
}