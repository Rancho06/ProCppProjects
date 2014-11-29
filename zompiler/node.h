#pragma once

#include <list>
#include <vector>
#include <string>
#include <map>

struct CodeContext
{
	std::vector<std::string> m_Ops;
};

class Node
{
public:
	virtual void CodeGen(CodeContext& context) = 0;
};

class NNumeric : public Node
{
public:
	NNumeric(std::string& value);
	int m_value;
	virtual void CodeGen(CodeContext& context) { }
};

class NStatement : public Node
{
public:
	virtual void CodeGen(CodeContext& context) = 0;
};

class NRotate : public NStatement
{
public:
	NRotate(NNumeric* dir);
	virtual void CodeGen(CodeContext& context);
private:
	NNumeric* m_Dir;
};

class NForward : public NStatement
{
public:
	virtual void CodeGen(CodeContext& context);
};

class NAttack : public NStatement
{
public:
	virtual void CodeGen(CodeContext& context);
};

class NRangedAttack : public NStatement
{
public:
	virtual void CodeGen(CodeContext& context);
};

class NMemInc : public NStatement
{
public:
	NMemInc(NNumeric* dir);
	virtual void CodeGen(CodeContext& context);
private:
	NNumeric* m_Dir;
};

class NMemDec : public NStatement
{
public:
	NMemDec(NNumeric* dir);
	virtual void CodeGen(CodeContext& context);
private:
	NNumeric* m_Dir;
};

class NMemAssign : public NStatement
{
public:
	NMemAssign(NNumeric* dir1, NNumeric* dir2);
	virtual void CodeGen(CodeContext& context);
private:
	NNumeric* m_loc;
	NNumeric* m_value;
};

class NSaveLoc : public NStatement
{
public:
	virtual void CodeGen(CodeContext& context);
};




class NBoolean : public Node
{
public:
	virtual void CodeGen(CodeContext& context) = 0;
};

class NIsHuman : public NBoolean
{
public:
	NIsHuman(NNumeric*);
	virtual void CodeGen(CodeContext& context);
private:
	NNumeric* distance;
};

class NIsZombie : public NBoolean
{
public:
	NIsZombie(NNumeric*);
	virtual void CodeGen(CodeContext& context);
private:
	NNumeric* distance;
};

class NIsRandom : public NBoolean
{
public:
	virtual void CodeGen(CodeContext& context);
};

class NIsPassable : public NBoolean
{
public:
	virtual void CodeGen(CodeContext& context);
};

class NBlock : public Node
{
public:
	NBlock() { m_bMainBlock = false; }
	virtual void CodeGen(CodeContext& context);
	void AddStatement(NStatement* statement);
	void SetMainBlock() { m_bMainBlock = true; }
private:
	std::list<NStatement*> m_Statements;
	bool m_bMainBlock;
};

class NIf : public NBlock
{
public:
	NIf(NBoolean*, NBlock*);
	virtual void CodeGen(CodeContext& context);
private:
	NBoolean* m_boolean;
	NBlock* m_block;
};

class NIfElse : public NBlock
{
public:
	NIfElse(NBoolean*, NBlock*, NBlock*);
	virtual void CodeGen(CodeContext& context);
private:
	NBoolean* m_boolean;
	NBlock* m_ifblock;
	NBlock* m_elseblock;
};