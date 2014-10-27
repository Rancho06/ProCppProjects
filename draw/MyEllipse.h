/*
	This class is used to draw a ellipse
*/

#pragma once
#include "Shape.h"
class MyEllipse :
	public Shape
{
public:
	MyEllipse();
	~MyEllipse();
	void draw(Gdiplus::Graphics&);
};

