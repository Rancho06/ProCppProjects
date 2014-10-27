#include "stdafx.h"
#include "MyEllipse.h"


MyEllipse::MyEllipse()
{
}


MyEllipse::~MyEllipse()
{
}


void MyEllipse::draw(Gdiplus::Graphics& graphics) {
	int xTopLeft, yTopLeft, width, height;
	int xStart = m_StartPoint.X;
	int yStart = m_StartPoint.Y;
	int xEnd = m_EndPoint.X;
	int yEnd = m_EndPoint.Y;
	if (xStart < xEnd) {
		xTopLeft = xStart;
		width = xEnd - xStart;
	}
	else {
		xTopLeft = xEnd;
		width = xStart - xEnd;
	}
	if (yStart < yEnd) {
		yTopLeft = yStart;
		height = yEnd - yStart;
	}
	else {
		yTopLeft = yEnd;
		height = yStart - yEnd;
	}
	graphics.DrawEllipse(m_Pen.get(), xTopLeft, yTopLeft, width, height);
}