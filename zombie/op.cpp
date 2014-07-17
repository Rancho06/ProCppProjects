#include "stdafx.h"
#include "op.h"
#include "machine.h"
#include <iostream>

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
	std::cout << state.m_ProgramCounter << ":" << m_OpName << "," << m_Param << std::endl;
}

void OpRotate::Execute(MachineState& state)
{
	DebugOutput(state);
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

	/*std::cout << "Now facing ";
	switch (state.m_Facing)
	{
	case (MachineState::UP):
		std::cout << "UP";
		break;
	case (MachineState::RIGHT):
		std::cout << "RIGHT";
		break;
	case (MachineState::DOWN):
		std::cout << "DOWN";
		break;
	case (MachineState::LEFT):
		std::cout << "LEFT";
		break;
	}
	std::cout << std::endl;*/
	state.m_ProgramCounter++;
	state.m_ActionsTaken++;
}

void OpGoto::Execute(MachineState& state)
{
	DebugOutput(state);
	state.m_ProgramCounter = m_Param;
}
