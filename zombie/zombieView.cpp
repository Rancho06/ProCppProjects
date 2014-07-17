// zombieView.cpp : implementation of the CZombieView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "zombieView.h"
using namespace Gdiplus;

CZombieView::CZombieView()
	: m_BitmapImage(1024, 768)
	, m_GraphicsImage(&m_BitmapImage)
{

}

BOOL CZombieView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CZombieView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	DrawGrid();

	Graphics graphics(dc);
	graphics.DrawImage(&m_BitmapImage, 0, 0);

	return 0;
}

void CZombieView::DrawGrid()
{
	m_GraphicsImage.Clear(Color(255, 255, 255));
		Pen pen(Color(0, 0, 0));
	m_GraphicsImage.DrawRectangle(&pen, 10, 10, 600, 600);
}
