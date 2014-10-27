/*
	The base class for Line, Rectangle, and Ellipse
*/

#pragma once
#include <memory>

class Shape
{
public:
	Shape();
	~Shape();
	virtual void draw(Gdiplus::Graphics&) = 0;
	void setPen(std::shared_ptr<Gdiplus::Pen>&);
	void setStartPoint(Gdiplus::Point&);
	void setEndPoint(Gdiplus::Point&);

protected:
	Gdiplus::Point m_StartPoint;
	Gdiplus::Point m_EndPoint;
	std::shared_ptr<Gdiplus::Pen> m_Pen;
};

