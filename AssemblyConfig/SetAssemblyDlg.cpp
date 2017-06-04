// SetAssemblyDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "AssemblyConfig.h"
#include "AssemblyConfigDoc.h"
#include "SetAssemblyDlg.h"
#include "afxdialogex.h"


// диалоговое окно CSetAssemblyDlg

IMPLEMENT_DYNAMIC(CSetAssemblyDlg, CDialog)

CSetAssemblyDlg::CSetAssemblyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG4, pParent)
	, m_dL1(0)
	, m_dL2(0)
	, m_dL3(0)
	, m_dL4(0)
{

}

CSetAssemblyDlg::~CSetAssemblyDlg()
{
}

void CSetAssemblyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dL1);
	DDX_Text(pDX, IDC_EDIT2, m_dL2);
	DDX_Text(pDX, IDC_EDIT3, m_dL3);
	DDX_Text(pDX, IDC_EDIT4, m_dL4);
}


BEGIN_MESSAGE_MAP(CSetAssemblyDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetAssemblyDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// обработчики сообщений CSetAssemblyDlg


void CSetAssemblyDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений

	UpdateData(TRUE);// данные из элементов попадают в переменные
	//10
	m_pDoc->OpenInventor();
	m_pDoc->Gear(1, VSpB1, VSpL, VSpT1, df, m_dShS, p, ya, xf, yf, yd, d, m_dShZ1, a, Do, Dc, m_V1d2);
	m_pDoc->Gear(2, VSpB2, VSpL, VSpT2, df2, m_dShS, p, ya2, xf, yf2, yd2, d2, m_dShZ2, a, Do2, Dc, m_V1d2);
	m_pDoc->Shaft(1, VSpB1, VSpL, VSpT1, m_dL2, m_V1d3, m_V1l4, m_V1d1, m_V1l2, m_V1d2, m_V1l1, m_Vh, m_V1l3, m_Vl, m_Vr);
	m_pDoc->Shaft(2, VSpB2, VSpL, VSpT2, m_dL3, m_V2d3, m_V2l4, m_V2d1, m_V2l2, m_V2d2, m_V2l1, m_Vh, m_V2l3, m_Vl, m_Vr);
	m_pDoc->Bearing(1, m_V1l2, m_V1d1, m_P1h, m_P1r, m_P1num);
	m_pDoc->Bearing(2, m_V2l2, m_V2d1, m_P2h, m_P2r, m_P2num);
	m_pDoc->Assembly(aw, m_V1d2, m_V2d2, VSpT1, VSpT2, m_dL3, m_V2l2, VSpB1, VSpL);
	//*10
}
