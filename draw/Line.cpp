#include "stdafx.h"
#include "Line.h"


Line::Line()
{
}


Line::~Line()
{
}


void Line::draw(Gdiplus::Graphics& graphics) {
	graphics.DrawLine(m_Pen.get(), m_StartPoint.X, m_StartPoint.Y, m_EndPoint.X, m_EndPoint.Y);
}