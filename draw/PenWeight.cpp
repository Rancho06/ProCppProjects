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

// if an identical pen is set up already, find and use it
// if no such pen is set up, create one and save it for future use
std::shared_ptr<Gdiplus::Pen> PenWeight::GetPen(Gdiplus::Color color, float width) 
{
	std::stringstream newStream;
	newStream << "(" << color.GetRed() << ",";
	newStream << color.GetGreen() << ",";
	newStream << color.GetBlue() << ");";
	newStream << std::setprecision(2) << width;
	std::string newString = newStream.str();
	// when an identical pen is found
	if (this->pens.find(newString) != this->pens.end()) {
		return pens.at(newString);
	}
	// no identical pens are found
	else {
		std::shared_ptr<Gdiplus::Pen> newPen = std::make_shared<Gdiplus::Pen>(color, width);
		pens.insert(std::pair<std::string, std::shared_ptr<Gdiplus::Pen>>(newString, newPen));
		return newPen;
	}
}