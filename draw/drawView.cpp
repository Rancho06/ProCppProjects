// drawView.cpp : implementation of the CDrawView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "drawView.h"
using namespace Gdiplus;

CDrawView::CDrawView()
	: m_BitmapImage(1024, 768)
	, m_GraphicsImage(&m_BitmapImage)
	, m_Pen(Color(0, 0, 0), 1.0f)
{
	//Pen pen(Color(0, 0, 255));
	//m_GraphicsImage.DrawLine(&pen, 0, 0, 200, 100);
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
	return 0;
}

LRESULT CDrawView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	m_StartPoint.X = xPos;
	m_StartPoint.Y = yPos;
	return 0;
}

LRESULT CDrawView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	m_EndPoint.X = xPos;
	m_EndPoint.Y = yPos;
	m_GraphicsImage.DrawLine(&m_Pen, m_StartPoint.X, m_StartPoint.Y, m_EndPoint.X, m_EndPoint.Y);
	RedrawWindow();
	return 0;
}

LRESULT CDrawView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
