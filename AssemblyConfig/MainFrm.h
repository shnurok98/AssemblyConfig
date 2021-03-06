
// MainFrm.h : ��������� ������ CMainFrame
//
#include "CAcCtrl.h"
#include "AssemblyConfigView.h"
#include "AssemblyTreeView.h"
#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // ������� ������ �� ������������
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ��������
public:
	CAssemblyConfigView* m_pMainView;
	CAssemblyTreeView* m_pTreeView;
// ��������
public:

// ���������������
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����������
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ���������� ����� ������ ��������� ����������
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	CSplitterWnd      m_wndSplitter;
	
// ��������� ������� ����� ���������
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


