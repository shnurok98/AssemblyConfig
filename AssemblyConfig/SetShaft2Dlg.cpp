// SetShaft2Dlg.cpp: файл реализации
//

#include "stdafx.h"
#include "AssemblyConfig.h"
#include "AssemblyConfigDoc.h"
#include "SetShaft2Dlg.h"
#include "afxdialogex.h"


// диалоговое окно CSetShaft2Dlg

IMPLEMENT_DYNAMIC(CSetShaft2Dlg, CDialog)

CSetShaft2Dlg::CSetShaft2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG3, pParent)
	, m_V2d1(0)
	, m_V2d2(0)
	, m_V2d3(0)
	, m_V2l1(0)
	, m_V2l2(0)
	, m_V2l3(0)
	, m_V2l4(0)
	, m_Vr(0)
	, m_Vl(0)
{

}

CSetShaft2Dlg::~CSetShaft2Dlg()
{
}

void CSetShaft2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_V2d1);
	DDX_Text(pDX, IDC_EDIT2, m_V2d2);
	DDX_Text(pDX, IDC_EDIT3, m_V2d3);
	DDX_Text(pDX, IDC_EDIT4, m_V2l1);
	DDX_Text(pDX, IDC_EDIT5, m_V2l2);
	DDX_Text(pDX, IDC_EDIT6, m_V2l3);
	DDX_Text(pDX, IDC_EDIT7, m_V2l4);
	DDX_Text(pDX, IDC_EDIT8, m_Vr);
	DDX_Text(pDX, IDC_EDIT9, m_Vl);
}


BEGIN_MESSAGE_MAP(CSetShaft2Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetShaft2Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// обработчики сообщений CSetShaft2Dlg


void CSetShaft2Dlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	m_pDoc->OpenInventor();
	m_pDoc->Shaft(2, VSpB2, VSpL, VSpT2, m_dL3, m_V2d3, m_V2l4, m_V2d1, m_V2l2, m_V2d2, m_V2l1, m_Vh, m_V2l3, m_Vl, m_Vr);
}
