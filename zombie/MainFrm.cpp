// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "World.h"
#include "aboutdlg.h"
#include "zombieView.h"
#include "MainFrm.h"

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
	// TODO: add code to initialize document

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
		for (auto it = World::get().zombieStateLists.begin(); it != World::get().zombieStateLists.end(); ++it) {
			World::get().zombieMachine.TakeTurn(*it);
		}
		for (auto it = World::get().humanStateLists.begin(); it != World::get().humanStateLists.end(); ++it) {
			World::get().humanMachine.TakeTurn(*it);
		}
		m_view.RedrawWindow();
	}
	return 0;
}

LRESULT CMainFrame::OnSimStart(WORD , WORD , HWND , BOOL& )
{
	// Add timer to run once per second
	SetTimer(1, 1500);
	return 0;
}

LRESULT CMainFrame::OnLoadZombie(WORD, WORD, HWND, BOOL&)
{
	World::get().zombieMachine.LoadMachine(std::string("search.zom"));
	createZombies(1);
	return 0;
}

LRESULT CMainFrame::OnLoadSurvivor(WORD, WORD, HWND, BOOL&)
{
	World::get().humanMachine.LoadMachine(std::string("the_governor.zom"));
	createHumans(10);
	return 0;
}

LRESULT CMainFrame::OnClear(WORD, WORD, HWND, BOOL&)
{
	return 0;
}

LRESULT CMainFrame::OnRandomize(WORD, WORD, HWND, BOOL&)
{
	return 0;
}