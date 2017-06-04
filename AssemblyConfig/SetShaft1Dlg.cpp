// SetShaft1Dlg.cpp: файл реализации
//

#include "stdafx.h"
#include "AssemblyConfig.h"
#include "AssemblyConfigDoc.h"
#include "SetShaft1Dlg.h"
#include "afxdialogex.h"


// диалоговое окно CSetShaft1

IMPLEMENT_DYNAMIC(CSetShaft1, CDialog)

CSetShaft1::CSetShaft1(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG2, pParent)
	, m_V1d1(0)
	, m_V1d2(0)
	, m_V1d3(0)
	, m_V1l1(0)
	, m_V1l2(0)
	, m_V1l3(0)
	, m_V1l4(0)
{

}

CSetShaft1::~CSetShaft1()
{
}

void CSetShaft1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_V1d1);
	DDX_Text(pDX, IDC_EDIT2, m_V1d2);
	DDX_Text(pDX, IDC_EDIT3, m_V1d3);
	DDX_Text(pDX, IDC_EDIT4, m_V1l1);
	DDX_Text(pDX, IDC_EDIT5, m_V1l2);
	DDX_Text(pDX, IDC_EDIT6, m_V1l3);
	DDX_Text(pDX, IDC_EDIT7, m_V1l4);
}


BEGIN_MESSAGE_MAP(CSetShaft1, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetShaft1::OnBnClickedButton1)
END_MESSAGE_MAP()


// обработчики сообщений CSetShaft1


void CSetShaft1::OnBnClickedButton1()
{
	UpdateData(TRUE);
	m_pDoc->OpenInventor();
	m_pDoc->Shaft(1, VSpB1, VSpL, VSpT1, m_dL2, m_V1d3, m_V1l4, m_V1d1, m_V1l2, m_V1d2, m_V1l1, m_Vh, m_V1l3, m_Vl, m_Vr);
}
