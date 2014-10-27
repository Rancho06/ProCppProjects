#include "stdafx.h"
#include "PenWeight.h"
#include <sstream>
#include <iomanip>

PenWeight::PenWeight()
{
}


PenWeight::~PenWeight()
{
}


std::shared_ptr<Gdiplus::Pen> PenWeight::GetPen(Gdiplus::Color color, float width) 
{
	std::stringstream newStream;
	newStream << "(" << color.GetRed() << ",";
	newStream << color.GetGreen() << ",";
	newStream << color.GetBlue() << ");";
	newStream << std::setprecision(2) << width;
	std::string newString = newStream.str();
	if (this->pens.find(newString) != this->pens.end()) {
		return pens.at(newString);
	}
	else {
		std::shared_ptr<Gdiplus::Pen> newPen = std::make_shared<Gdiplus::Pen>(color, width);
		pens.insert(std::pair<std::string, std::shared_ptr<Gdiplus::Pen>>(newString, newPen));
		return newPen;
	}
}