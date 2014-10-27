#include "stdafx.h"
#include "MyLine.h"


MyLine::MyLine()
{
}


MyLine::~MyLine()
{
}

// overload the pure virtual draw() from base class
void MyLine::draw(Gdiplus::Graphics& graphics) {
	graphics.DrawLine(m_Pen.get(), m_StartPoint.X, m_StartPoint.Y, m_EndPoint.X, m_EndPoint.Y);
}