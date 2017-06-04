// AssemblyTreeView.cpp: ���� ����������
//

#include "stdafx.h"
#include "AssemblyConfig.h"
#include "AssemblyTreeView.h"
#include "AssemblyConfigView.h"
#include "MainFrm.h"


// CAssemblyTreeView

IMPLEMENT_DYNCREATE(CAssemblyTreeView, CTreeView)

CAssemblyTreeView::CAssemblyTreeView()
{
	m_ComponentNames.RemoveAll();
	//m_ComponentNames.Add(L"������");
	m_ComponentNames.Add(L"�������� ��������");
	m_ComponentNames.Add(L"��� ��� 1");
	m_ComponentNames.Add(L"��� ��� 2");
}

CAssemblyTreeView::~CAssemblyTreeView()
{
}

BEGIN_MESSAGE_MAP(CAssemblyTreeView, CTreeView)
	
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// ����������� CAssemblyTreeView

#ifdef _DEBUG
void CAssemblyTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CAssemblyTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// ����������� ��������� CAssemblyTreeView


void CAssemblyTreeView::FillTree()
{
	CTreeCtrl& ctlTree = (CTreeCtrl&)GetTreeCtrl();

	ctlTree.DeleteAllItems();

	m_hRoot = ctlTree.InsertItem(L"������", -1, -1, NULL, TVI_FIRST);//�������� ������ -1 -1 ����� � ������� ����
	ctlTree.SetItemData(m_hRoot,-1);//������������� ����� ��������
	if (m_iSelected ==-1)
		ctlTree.SetItemState(m_hRoot, TVIS_BOLD, TVIS_BOLD);//bold - ������

	HTREEITEM item;

	for (int i = 0; i < m_ComponentNames.GetSize(); i++)
	{
		item = ctlTree.InsertItem(m_ComponentNames[i], -1, -1, m_hRoot, TVI_LAST);// tvi last ������� � �����
		ctlTree.SetItemData(item, i);
		if (i == m_iSelected)
			ctlTree.SetItemState(item, TVIS_BOLD, TVIS_BOLD);
		else
			ctlTree.SetItemState(item, 0, TVIS_BOLD);

	}

	ctlTree.Expand(m_hRoot, TVE_EXPAND);

	//ctlTree.Expand(m_hRoot, TVE_EXPAND);

	/*m_hRoot = ctlTree.InsertItem(L"������", -1, -1, NULL, TVI_FIRST);
	ctlTree.SetItemData(m_hRoot, 0);

	m_hComponent1 = ctlTree.InsertItem(L"���������1", m_hRoot, TVI_LAST);
	ctlTree.SetItemData(m_hComponent1, 1);

	m_hComponent2 = ctlTree.InsertItem(L"���������2", m_hRoot, TVI_LAST);
	ctlTree.SetItemData(m_hComponent2, 2);

	m_hComponent3 = ctlTree.InsertItem(L"���������3", m_hRoot, TVI_LAST);
	ctlTree.SetItemData(m_hComponent3, 3);*/

	

		

}

int CAssemblyTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	
	lpCreateStruct->style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �������� ������������������ ��� ��������
	
	CTreeCtrl& ctlTree = (CTreeCtrl&)GetTreeCtrl();//��������� ���������� ���� �� ������

	FillTree();
	



	return 0;
}


void CAssemblyTreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTreeView::OnLButtonDown(nFlags, point);
	// TODO: �������� ���� ��� ����������� ��������� ��� ����� ������������
	CTreeCtrl& ctlTree = (CTreeCtrl&)GetTreeCtrl();

	HTREEITEM hItem = ctlTree.GetSelectedItem();

	CAssemblyConfigView *pView;

	pView = ((CMainFrame*)AfxGetMainWnd())->m_pMainView;

	if (hItem)
	{
         m_iSelected = ctlTree.GetItemData(hItem);
		
		 switch (m_iSelected)
		 {

		 case 1: pView->Tree(2); break;
		 }
		FillTree();
		
        //ctlTree.SetItemState(hItem, TVIS_BOLD, TVIS_BOLD);

		
	}
	
	

	
}
