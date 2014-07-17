#include "node.h"
#include <sstream>

void NBlock::AddStatement(NStatement* statement)
{
	m_Statements.push_back(statement);
}

void NBlock::CodeGen(CodeContext& context)
{
	// TODO: Loop through statements in list and code gen them
}

NNumeric::NNumeric(std::string& value)
{
	m_value = std::atoi(value.c_str());
}

NRotate::NRotate(NNumeric* dir)
	: m_Dir(dir)
{
}

void NRotate::CodeGen(CodeContext& context)
{
	if (m_Dir->m_value == 0)
	{
		context.m_Ops.push_back("rotate,0");
	}
	else if (m_Dir->m_value == 1)
	{
		context.m_Ops.push_back("rotate,1");
	}
}
