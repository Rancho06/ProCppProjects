// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "World.h"
#include "aboutdlg.h"
#include "zombieView.h"
#include "MainFrm.h"
//#include <afxdlgs.h>

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	return FALSE;
}

void createZombies(int count) {
	for (int i = 0; i < count; ++i) {
		MachineState* newZombie = new MachineState();
		int facing = World::get().getRandom() % 4;
		switch (facing) {
		case 0:
			newZombie->m_Facing = MachineState::UP;
			break;
		case 1:
			newZombie->m_Facing = MachineState::DOWN;
			break;
		case 2:
			newZombie->m_Facing = MachineState::LEFT;
			break;
		case 3:
			newZombie->m_Facing = MachineState::RIGHT;
			break;
		default:
			newZombie->m_Facing = MachineState::UP;
			break;
		}
		do {
			newZombie->m_XPos = World::get().getRandom() % 20;
			newZombie->m_YPos = World::get().getRandom() % 20;
		} while (World::get().array[newZombie->m_XPos][newZombie->m_YPos] != 0);
		World::get().array[newZombie->m_XPos][newZombie->m_YPos] = 1;
		World::get().zombieStateLists.push_back(*newZombie);
		World::get().zombieMachine.BindState(World::get().zombieStateLists.back());
	}	
}

void createHumans(int count) {
	for (int i = 0; i < count; ++i) {
		MachineState* newHuman = new MachineState();
		do {
			newHuman->m_XPos = World::get().getRandom() % 20;
			newHuman->m_YPos = World::get().getRandom() % 20;
		} while (World::get().array[newHuman->m_XPos][newHuman->m_YPos] != 0);
		World::get().array[newHuman->m_XPos][newHuman->m_YPos] = 1;
		World::get().humanStateLists.push_back(*newHuman);
		World::get().humanMachine.BindState(World::get().humanStateLists.back());
	}	
}


LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	CreateSimpleStatusBar();

	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	// TEMP CODE: Initialize zombie test machine
	
	
	// END TEMP CODE
	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	KillTimer(1);
	World::get().isRunning = false;
	for (int i = 0; i < World::get().SIZE; ++i) {
		for (int j = 0; j < World::get().SIZE; ++j) {
			World::get().array[i][j] = 0;
		}
	}
	World::get().zombieFileName = "";
	World::get().humanFileName = "";
	World::get().zombieMachine.reset();
	World::get().humanMachine.reset();
	World::get().zombieStateLists.clear();
	World::get().humanStateLists.clear();
	World::get().resetTurnCount();
	EnableMenuItem(GetMenu(), ID_SIMULATION_RANDOMIZE, MF_DISABLED | MF_GRAYED);
	m_view.RedrawWindow();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Check if this is the turn timer
	if (wParam == 1)
	{
		if ((World::get().zombieStateLists.size() == 0) || (World::get().humanStateLists.size() == 0)) {
			KillTimer(1);
			World::get().isRunning = false;
			if (World::get().zombieStateLists.size() == 0) {
				MessageBox("Game Over: Humans Won!", "Result", MB_OK);
			}
			else {
				MessageBox("Game Over: Zombies won!", "Result", MB_OK);
			}
			
			return 0;
		}
		for (auto it = World::get().zombieStateLists.begin(); it != World::get().zombieStateLists.end(); ++it) {
			World::get().zombieMachine.TakeTurn(*it);
		}
		for (auto it = World::get().humanStateLists.begin(); it != World::get().humanStateLists.end(); ++it) {
			World::get().humanMachine.TakeTurn(*it);
		}
		World::get().incrementTurn();
		m_view.RedrawWindow();
	}
	return 0;
}

LRESULT CMainFrame::OnSimStart(WORD , WORD , HWND , BOOL& )
{
	// Add timer to run once per second
	if ((World::get().zombieStateLists.size() != 0) && (World::get().humanStateLists.size() != 0)) {
		if (World::get().isRunning) {
			KillTimer(1);
			World::get().isRunning = false;
		}
		else {
			SetTimer(1, 400);
			World::get().isRunning = true;
		}
	}
	return 0;
}

LRESULT CMainFrame::OnLoadZombie(WORD, WORD, HWND, BOOL&)
{
	CFileDialog fileDlg(true, _T("zom"), NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T("Zombie Program\0*.zom\0"));
	if (IDOK == fileDlg.DoModal()) {
		World::get().zombieFileName = fileDlg.m_szFileName;
		std::cout << World::get().zombieFileName;
		World::get().zombieMachine.LoadMachine(World::get().zombieFileName);
		int pos = World::get().zombieFileName.rfind("\\");
		World::get().zombieFileName = World::get().zombieFileName.substr(pos + 1);
	}
	if ((World::get().zombieFileName != "") && (World::get().humanFileName != "")) {
		EnableMenuItem(GetMenu(), ID_SIMULATION_RANDOMIZE, MF_ENABLED);
	}
	m_view.RedrawWindow();
	return 0;
}

LRESULT CMainFrame::OnLoadSurvivor(WORD, WORD, HWND, BOOL&)
{
	CFileDialog fileDlg(true, _T("zom"), NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T("Human Program\0*.zom\0"));
	if (IDOK == fileDlg.DoModal()) {
		World::get().humanFileName = fileDlg.m_szFileName;
		World::get().humanMachine.LoadMachine(World::get().humanFileName);
		int pos = World::get().humanFileName.rfind("\\");
		World::get().humanFileName = World::get().humanFileName.substr(pos + 1);
	}
	if ((World::get().zombieFileName != "") && (World::get().humanFileName != "")) {
		EnableMenuItem(GetMenu(), ID_SIMULATION_RANDOMIZE, MF_ENABLED);
	}
	m_view.RedrawWindow();
	return 0;
}

LRESULT CMainFrame::OnClear(WORD, WORD, HWND, BOOL&)
{
	KillTimer(1);
	World::get().isRunning = false;
	for (int i = 0; i < World::get().SIZE; ++i) {
		for (int j = 0; j < World::get().SIZE; ++j) {
			World::get().array[i][j] = 0;
		}
	}
	World::get().resetTurnCount();
	World::get().zombieStateLists.clear();
	World::get().humanStateLists.clear();	
	m_view.RedrawWindow();
	return 0;
}

LRESULT CMainFrame::OnRandomize(WORD, WORD, HWND, BOOL&)
{
	
	createZombies(20);
	createHumans(10);
	m_view.RedrawWindow();
	return 0;
}