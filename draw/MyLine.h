#pragma once
#include "Shape.h"
class MyLine :
	public Shape
{
public:
	void draw(Gdiplus::Graphics&);
	MyLine();
	~MyLine();
};

