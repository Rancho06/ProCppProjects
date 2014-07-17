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

class NBoolean : public Node
{
public:
	virtual void CodeGen(CodeContext& context) = 0;
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
