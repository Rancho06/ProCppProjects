// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include <iostream>
#include "aboutdlg.h"
#include "drawView.h"
#include "MainFrm.h"
#include <atlstr.h>

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	CreateSimpleStatusBar();

	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, WS_EX_CLIENTEDGE);
	m_view.SetScrollSize(1,1);
	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

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
	m_view.m_GraphicsImage.Clear(Gdiplus::Color(255, 255, 255));
	m_view.RedrawWindow();
	m_view.fileIsSaved = false;
	m_view.currentFileName = "";
	m_view.undoLists.clear();
	m_view.redoLists.clear();
	return 0;
}


LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFileDialog fileDlg(true, _T("PNG"), NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T("PNG Format\0*.PNG\0"));

	if (IDOK == fileDlg.DoModal()) {
		m_view.currentFileName = fileDlg.m_szFileName;
		Gdiplus::Bitmap myFile(CA2W(m_view.currentFileName.c_str()));
		m_view.m_GraphicsImage.Clear(Gdiplus::Color(255, 255, 255));
		m_view.m_GraphicsImage.DrawImage(&myFile, 0, 0);
		m_view.RedrawWindow();
		m_view.undoLists.clear();
		m_view.redoLists.clear();
		m_view.fileIsSaved = true;
	}
	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
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

LRESULT CMainFrame::OnPenColor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	Gdiplus::Color currentColor;
	m_view.m_Pen.GetColor(&currentColor);
	COLORREF color = currentColor.ToCOLORREF();
	CColorDialog test(color);
	test.DoModal();
	color = test.GetColor();
	Gdiplus::Color newColor(GetRValue(color), GetGValue(color), GetBValue(color));
	m_view.m_Pen.SetColor(newColor);
	return 0;
}


LRESULT CMainFrame::OnWidthChange(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_0, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_1, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_2, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_3, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_4, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_5, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(wID, MF_CHECKED);
	switch (wID) {
		case ID_WIDTH_0:
			m_view.m_Pen.SetWidth(0.5f);
			break;
		case ID_WIDTH_1:
			m_view.m_Pen.SetWidth(1.0f);
			break;
		case ID_WIDTH_2:
			m_view.m_Pen.SetWidth(1.5f);
			break;
		case ID_WIDTH_3:
			m_view.m_Pen.SetWidth(2.0f);
			break;
		case ID_WIDTH_4:
			m_view.m_Pen.SetWidth(2.5f);
			break;
		case ID_WIDTH_5:
			m_view.m_Pen.SetWidth(5.0f);
			break;
		default:
			m_view.m_Pen.SetWidth(1.0f);
			break;
	}
	return 0;
}

LRESULT CMainFrame::OnShapeChange(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_LINE, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_ELLIPSE, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_RECTANGLE, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(wID, MF_CHECKED);
	switch (wID) {
		case ID_SHAPE_LINE:
			m_view.shapeType = CDrawView::LINE;
			break;
		case ID_SHAPE_ELLIPSE:
			m_view.shapeType = CDrawView::ELLIPSE;
			break;
		case ID_SHAPE_RECTANGLE:
			m_view.shapeType = CDrawView::RECTANGLE;
			break;
		default:
			m_view.shapeType = CDrawView::LINE;
			break;
	}
	return 0;
}

LRESULT CMainFrame::OnEditUndo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_view.undoLists.size() != 0) {		
		m_view.redoLists.push_back(m_view.undoLists.back());
		m_view.undoLists.pop_back();
		m_view.m_GraphicsImage.Clear(Gdiplus::Color(255, 255, 255));
		for (auto it = m_view.undoLists.begin(); it != m_view.undoLists.end(); it++) {
			it->get()->draw(m_view.m_GraphicsImage);
		}
		m_view.RedrawWindow();
	}	
	return 0;
}

LRESULT CMainFrame::OnEditRedo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_view.redoLists.size() != 0) {
		m_view.undoLists.push_back(m_view.redoLists.back());
		m_view.redoLists.back().get()->draw(m_view.m_GraphicsImage);
		m_view.redoLists.pop_back();
		m_view.RedrawWindow();
	}	
	return 0;
}

LRESULT CMainFrame::OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (!m_view.fileIsSaved) {
		CFileDialog fileDlg(false, _T("PNG"), _T("image.PNG"),
			NULL, _T("PNG Format\0*.PNG\0"));
		if (IDOK == fileDlg.DoModal()) {
			m_view.currentFileName = fileDlg.m_szFileName;
			CLSID pngClsid;
			CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &pngClsid);
			m_view.m_BitmapImage.Save(CA2W(m_view.currentFileName.c_str()), &pngClsid);
			m_view.fileIsSaved = true;
		}
	}
	else {
		CLSID pngClsid;
		CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &pngClsid);
		m_view.m_BitmapImage.Save(CA2W(m_view.currentFileName.c_str()), &pngClsid);
	}	
	return 0;
}

LRESULT CMainFrame::OnFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFileDialog fileDlg(false, _T("PNG"), _T("image.PNG"),
		NULL, _T("PNG Format\0*.PNG\0"));
	if (IDOK == fileDlg.DoModal()) {
		m_view.currentFileName = fileDlg.m_szFileName;
		CLSID pngClsid;
		CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &pngClsid); 
		m_view.m_BitmapImage.Save(CA2W(m_view.currentFileName.c_str()), &pngClsid);
		m_view.fileIsSaved = true;
	}
	return 0;
}