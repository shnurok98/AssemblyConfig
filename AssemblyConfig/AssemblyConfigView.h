
// AssemblyConfigView.h : интерфейс класса CAssemblyConfigView
//
#include "CAcCtrl.h"
#include "AssemblyConfigDoc.h"
#include "AssemblyTreeView.h"
#pragma once


class CAssemblyConfigView : public CView
{
protected: // создать только из сериализации
	CAssemblyConfigView();
	DECLARE_DYNCREATE(CAssemblyConfigView)

// Атрибуты
public:
	CAssemblyConfigDoc* GetDocument() const;
	CModelViewCtrl    m_ctrlModelView;//класс active x

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	virtual ~CAssemblyConfigView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void On32771();
	afx_msg void OnLoad();
	afx_msg void OnHide();
	void Tree(int i);
};

#ifndef _DEBUG  // отладочная версия в AssemblyConfigView.cpp
inline CAssemblyConfigDoc* CAssemblyConfigView::GetDocument() const
   { return reinterpret_cast<CAssemblyConfigDoc*>(m_pDocument); }
#endif

