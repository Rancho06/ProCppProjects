/*
	This class implements the flyweight design patter for Pen class.
*/

#pragma once
#include "singleton.h"
#include <unordered_map>
#include <memory>

class PenWeight :
	public Singleton<PenWeight>
{
public:
	PenWeight();
	~PenWeight();
	std::shared_ptr<Gdiplus::Pen> GetPen(Gdiplus::Color, float);
private:
	DECLARE_SINGLETON(PenWeight);
	std::unordered_map<std::string, std::shared_ptr<Gdiplus::Pen>> pens;
};

