// Defines all the classes for different operations
// which can be performed.
#pragma once

struct MachineState;

// Abstract Base Class
class Op
{
public:
	Op(const char* OpName, int parameter)
		: m_OpName(OpName)
		, m_Param(parameter)
	{ }

	void DebugOutput(MachineState& state);

	virtual void Execute(MachineState& state) = 0;
protected:
	int m_Param;
	const char* m_OpName;
};

// Defines the rotate operation
// Param of 0 means rotate clockwise
// Param of 1 means rotate counter-clockwise
struct OpRotate : Op
{
	OpRotate(int parameter)
		: Op("OpRotate", parameter)
	{ }

	virtual void Execute(MachineState& state) throw();
};

// Defines the goto operation
// Parameter determines the line number
struct OpGoto : Op
{
	OpGoto(int parameter)
		: Op("OpGoto", parameter)
	{ }

	virtual void Execute(MachineState& state);
};

struct OpAttack : Op
{
	OpAttack()
	: Op("OpAttack", NULL)
	{ }

	virtual void Execute(MachineState& state) throw();
};

struct OpRangedAttack : Op
{
	OpRangedAttack()
	: Op("OpRangedAttack", NULL)
	{ }

	virtual void Execute(MachineState& state);
};

struct OpForward : Op
{
	OpForward()
	: Op("OpForward", NULL)
	{ }

	virtual void Execute(MachineState& state) throw();
};

struct OpEndturn : Op
{
	OpEndturn()
	: Op("OpGoto", NULL)
	{ }

	virtual void Execute(MachineState& state) throw();
};

struct OpTestHuman : Op
{
	OpTestHuman(int parameter)
	: Op("OpTestHuman", parameter)
	{ }

	virtual void Execute(MachineState& state);
};

struct OpTestZombie : Op
{
	OpTestZombie(int parameter)
	: Op("OpTestZombie", parameter)
	{ }

	virtual void Execute(MachineState& state);
};

struct OpTestWall : Op
{
	OpTestWall()
	: Op("OpTestWall", NULL)
	{ }

	virtual void Execute(MachineState& state) throw();
};

struct OpTestRandom : Op
{
	OpTestRandom()
	: Op("OpTestRandom", NULL)
	{ }

	virtual void Execute(MachineState& state) throw();
};

struct OpTestPassable : Op
{
	OpTestPassable()
	: Op("OpTestPassable", NULL)
	{ }

	virtual void Execute(MachineState& state) throw();
};

struct OpJe : Op
{
	OpJe(int parameter)
	: Op("OpJe", parameter)
	{ }

	virtual void Execute(MachineState& state);
};

struct OpJne : Op
{
	OpJne(int parameter)
	: Op("OpJne", parameter)
	{ }

	virtual void Execute(MachineState& state);
};

struct OpTestMem : Op
{
	OpTestMem(int parameter)
	: Op("OpTestMem", parameter)
	{ }

	virtual void Execute(MachineState& state);
};

struct OpMem : Op
{
	OpMem(int parameter)
	: Op("OpMem", parameter)
	{ }

	virtual void Execute(MachineState& state);
};

struct OpSet : Op
{
	OpSet(int parameter)
	: Op("OpSet", parameter)
	{ }

	virtual void Execute(MachineState& state);
};

struct OpInc : Op
{
	OpInc()
	: Op("OpInc", NULL)
	{ }

	virtual void Execute(MachineState& state);
};

struct OpDec : Op
{
	OpDec()
	: Op("OpDec", 0)
	{ }

	virtual void Execute(MachineState& state);
};

struct OpSaveLoc : Op
{
	OpSaveLoc()
	: Op("OpSaveLoc", 0)
	{ }

	virtual void Execute(MachineState& state);
};
