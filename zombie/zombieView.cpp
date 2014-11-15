// zombieView.cpp : implementation of the CZombieView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "World.h"
#include "zombieView.h"
using namespace Gdiplus;


int x = 0, y = 0;
std::string messages[4];
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

LRESULT CZombieView::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{

	x = (GET_X_LPARAM(lParam) - 10) / 30;
	y = (GET_Y_LPARAM(lParam) - 10) / 30;
	
	return 0;
}

void CZombieView::DrawGrid()
{
	m_GraphicsImage.Clear(Color(255, 255, 255));
	Pen pen(Color(0, 0, 0));
	SolidBrush brush(Color(0, 0, 0));
	//m_GraphicsImage.DrawRectangle(&pen, 10, 10, 600, 600);

	Font font(L"Times New Roman", 12);
	if ((x >= 0) && (x <= 19) && (y >= 0) && (y <= 19) && (World::get().array[x][y] == 2)) {
		messages[0] = "Human at (" + std::to_string(x) + "," + std::to_string(y) + ")";
		messages[1] = "Memory:";
		for (auto it = World::get().humanStateLists.begin(); it != World::get().humanStateLists.end(); ++it) {
			if (((*it).m_XPos == x) && ((*it).m_YPos == y)) {
				messages[2] = "0: " + std::to_string((*it).GetMemory(0));
				messages[3] = "1: " + std::to_string((*it).GetMemory(1));
				break;
			}
		}
	}
	m_GraphicsImage.DrawString(CA2W(messages[0].c_str()), -1, &font, PointF(620, 270), &brush);
	m_GraphicsImage.DrawString(CA2W(messages[1].c_str()), -1, &font, PointF(620, 290), &brush);
	m_GraphicsImage.DrawString(CA2W(messages[2].c_str()), -1, &font, PointF(620, 310), &brush);
	m_GraphicsImage.DrawString(CA2W(messages[3].c_str()), -1, &font, PointF(620, 330), &brush);
	
	m_GraphicsImage.DrawString(CA2W("Month: 14"), -1, &font, PointF(620, 210), &brush);
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 20; ++j) {
			m_GraphicsImage.DrawRectangle(&pen, 10 + 30 * i, 10 + 30 * j, 30, 30);
		}
	}
	brush.SetColor(Color(255, 0, 0));
	m_GraphicsImage.DrawString(CA2W("Zombies"), -1, &font, PointF(620, 10), &brush);
	m_GraphicsImage.DrawString(CA2W(("Program: search.zom")), -1, &font, PointF(620, 30), &brush);
	m_GraphicsImage.DrawString(CA2W("Alive: 0"), -1, &font, PointF(620, 50), &brush);
	for (auto it = World::get().zombieStateLists.begin(); it != World::get().zombieStateLists.end(); ++it) {
		
		Point points[3];
		int x = 30 * (*it).m_XPos + 10, y = 30 * (*it).m_YPos + 10;
		switch ((*it).m_Facing) {
		case (MachineState::UP) :
			points[0] = Point(x, y + 30);
			points[1] = Point(x + 30, y + 30);
			points[2] = Point(x + 15, y);
			break;
		case (MachineState::RIGHT) :
			points[0] = Point(x, y);
			points[1] = Point(x, y + 30);
			points[2] = Point(x + 30, y + 15);
			break;
		case (MachineState::DOWN) :
			points[0] = Point(x, y);
			points[1] = Point(x + 15, y + 30);
			points[2] = Point(x + 30, y);
			break;
		default:
		case (MachineState::LEFT) :
			points[0] = Point(x + 30, y);
			points[1] = Point(x, y + 15);
			points[2] = Point(x + 30, y + 30);
			break;
		}
		m_GraphicsImage.FillPolygon(&brush, points, 3);
	}

	brush.SetColor(Color(0, 255, 0));
	m_GraphicsImage.DrawString(CA2W("Humans"), -1, &font, PointF(620, 115), &brush);
	m_GraphicsImage.DrawString(CA2W(("Program: the_governor.zom")), -1, &font, PointF(620, 135), &brush);
	m_GraphicsImage.DrawString(CA2W("Alive: 0"), -1, &font, PointF(620, 155), &brush);

	for (auto it = World::get().humanStateLists.begin(); it != World::get().humanStateLists.end(); ++it) {
		Point points[3];
		int x = 30 * (*it).m_XPos + 10, y = 30 * (*it).m_YPos + 10;
		switch ((*it).m_Facing) {
		case (MachineState::UP) :
			points[0] = Point(x, y + 30);
			points[1] = Point(x + 30, y + 30);
			points[2] = Point(x + 15, y);
			break;
		case (MachineState::RIGHT) :
			points[0] = Point(x, y);
			points[1] = Point(x, y + 30);
			points[2] = Point(x + 30, y + 15);
			break;
		case (MachineState::DOWN) :
			points[0] = Point(x, y);
			points[1] = Point(x + 15, y + 30);
			points[2] = Point(x + 30, y);
			break;
		default:
		case (MachineState::LEFT) :
			points[0] = Point(x + 30, y);
			points[1] = Point(x, y + 15);
			points[2] = Point(x + 30, y + 30);
			break;
		}
		m_GraphicsImage.FillPolygon(&brush, points, 3);
	}

	
}
