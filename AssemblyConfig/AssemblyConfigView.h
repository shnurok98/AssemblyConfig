
// AssemblyConfigView.h : ��������� ������ CAssemblyConfigView
//
#include "CAcCtrl.h"
#include "AssemblyConfigDoc.h"
#include "AssemblyTreeView.h"
#pragma once


class CAssemblyConfigView : public CView
{
protected: // ������� ������ �� ������������
	CAssemblyConfigView();
	DECLARE_DYNCREATE(CAssemblyConfigView)

// ��������
public:
	CAssemblyConfigDoc* GetDocument() const;
	CModelViewCtrl    m_ctrlModelView;//����� active x

// ��������
public:

// ���������������
public:
	virtual void OnDraw(CDC* pDC);  // �������������� ��� ��������� ����� �������������
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����������
public:
	virtual ~CAssemblyConfigView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void On32771();
	afx_msg void OnLoad();
	afx_msg void OnHide();
	void Tree(int i);
};

#ifndef _DEBUG  // ���������� ������ � AssemblyConfigView.cpp
inline CAssemblyConfigDoc* CAssemblyConfigView::GetDocument() const
   { return reinterpret_cast<CAssemblyConfigDoc*>(m_pDocument); }
#endif

