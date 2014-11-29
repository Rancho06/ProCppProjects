#include "node.h"
#include <sstream>

void NBlock::AddStatement(NStatement* statement)
{
	m_Statements.push_back(statement);
}
void NBlock::CodeGen(CodeContext& context)
{
	for (auto item : m_Statements) {
		item->CodeGen(context);
	}
	if (m_bMainBlock) {
		context.m_Ops.push_back("goto,1");
		context.m_Gotos.insert(std::make_pair(context.m_Ops.size(), 1));
	}
}


NIf::NIf(NBoolean* boolean, NBlock* block)
:m_boolean(boolean), m_block(block)
{}
void NIf::CodeGen(CodeContext& context) {
	m_boolean->CodeGen(context);
	int jneLoc = context.m_Ops.size();
	context.m_Ops.push_back("jne,1");
	m_block->CodeGen(context);
	context.m_Ops[jneLoc] = "jne" + std::to_string(context.m_Ops.size() + 1);
}


NIfElse::NIfElse(NBoolean* boolean, NBlock* ifblock, NBlock* elseblock)
:m_boolean(boolean), m_ifblock(ifblock), m_elseblock(elseblock)
{}
void NIfElse::CodeGen(CodeContext& context) {
	m_boolean->CodeGen(context);
	int jeLoc = context.m_Ops.size();
	context.m_Ops.push_back("je,1");
	m_elseblock->CodeGen(context);
	int gotoLoc = context.m_Ops.size();
	context.m_Ops.push_back("goto,1");
	m_ifblock->CodeGen(context);
	context.m_Ops[jeLoc] = "je," + std::to_string(gotoLoc + 2);
	context.m_Ops[gotoLoc] = "goto," + std::to_string(context.m_Ops.size() + 1);
	context.m_Gotos.insert(std::make_pair(gotoLoc + 1, context.m_Ops.size() + 1));
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


void NForward::CodeGen(CodeContext& context) {
	context.m_Ops.push_back("forward");
}


void NAttack::CodeGen(CodeContext& context) {
	context.m_Ops.push_back("attack");
}


void NRangedAttack::CodeGen(CodeContext& context) {
	context.m_Ops.push_back("ranged_attack");
}


void NSaveLoc::CodeGen(CodeContext& context) {
	context.m_Ops.push_back("save_loc");
}


NMemInc::NMemInc(NNumeric* dir)
	: m_Dir(dir)
{
}
void NMemInc::CodeGen(CodeContext& context) {
	context.m_Ops.push_back("mem," + std::to_string(m_Dir->m_value));
	context.m_Ops.push_back("inc");
}


NMemDec::NMemDec(NNumeric* dir)
	: m_Dir(dir)
{
}
void NMemDec::CodeGen(CodeContext& context) {
	context.m_Ops.push_back("mem," + std::to_string(m_Dir->m_value));
	context.m_Ops.push_back("dec");
}


NMemAssign::NMemAssign(NNumeric* dir1, NNumeric* dir2) 
	:m_loc(dir1), m_value(dir2)
{
}
void NMemAssign::CodeGen(CodeContext& context) {
	context.m_Ops.push_back("mem," + std::to_string(m_loc->m_value));
	context.m_Ops.push_back("set," + std::to_string(m_value->m_value));
}


NIsHuman::NIsHuman(NNumeric* dir)
	:distance(dir)
{
}
void NIsHuman::CodeGen(CodeContext& context) {
	context.m_Ops.push_back("test_human," + std::to_string(distance->m_value));
}


NIsZombie::NIsZombie(NNumeric* dir)
	:distance(dir)
{
}
void NIsZombie::CodeGen(CodeContext& context) {
	context.m_Ops.push_back("test_zombie," + std::to_string(distance->m_value));
}


void NIsRandom::CodeGen(CodeContext& context) {
	context.m_Ops.push_back("test_random");
}


void NIsPassable::CodeGen(CodeContext& context) {
	context.m_Ops.push_back("test_passable");
}

