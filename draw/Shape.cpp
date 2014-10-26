#include "stdafx.h"
#include "Shape.h"


Shape::Shape()
	: m_StartPoint(0, 0)
	, m_EndPoint(0, 0)
{
}


Shape::~Shape()
{
}

void Shape::setPen(std::shared_ptr<Gdiplus::Pen>& pen) {
	this->m_Pen = pen;
}
void Shape::setStartPoint(Gdiplus::Point& point) {
	this->m_StartPoint = point;
}
void Shape::setEndPoint(Gdiplus::Point& point) {
	this->m_EndPoint = point;
}