// drawView.cpp : implementation of the CDrawView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "drawView.h"
using namespace Gdiplus;

class ShapeMaker {
public:
	static std::shared_ptr<Shape> createShape(CDrawView::SHAPETYPE shapeType) {
		switch (shapeType) {
		case CDrawView::LINE:
			return std::make_shared<MyLine>();
			break;
		case CDrawView::ELLIPSE:
			return std::make_shared<MyEllipse>();
			break;
		case CDrawView::RECTANGLE:
			return std::make_shared<MyRectangle>();
			break;
		default:
			return std::make_shared<MyLine>();
			break;
		}
	}
};


CDrawView::CDrawView()
	: m_BitmapImage(1024, 768)
	, m_GraphicsImage(&m_BitmapImage)
	, m_Pen(Color(0, 0, 0), 1.0f)
	, shapeType(LINE)
{
}

BOOL CDrawView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CDrawView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	//TODO: Add your drawing code here
	Graphics graphics(dc);
	graphics.DrawImage(&m_BitmapImage, 0, 0);
	if (m_Shape) {
		m_Shape.get()->draw(graphics);
	}
	return 0;
}

LRESULT CDrawView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	m_Shape = ShapeMaker::createShape(shapeType);
	std::shared_ptr<Pen> myPen(m_Pen.Clone());
	m_Shape.get()->setPen(myPen);
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	m_Shape.get()->setStartPoint(Point(xPos, yPos));
	return 0;
}

LRESULT CDrawView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	m_Shape.get()->setEndPoint(Point(xPos, yPos));
	m_Shape.get()->draw(m_GraphicsImage);
	RedrawWindow();
	undoLists.push_back(m_Shape);
	m_Shape.reset();
	return 0;
}

LRESULT CDrawView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_Shape) {
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		m_Shape.get()->setEndPoint(Point(xPos, yPos));
		RedrawWindow();
	}
	return 0;
}


