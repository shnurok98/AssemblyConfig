// SetGearsDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "AssemblyConfig.h"
#include "AssemblyConfigDoc.h"
#include "SetGearsDlg.h"
#include "afxdialogex.h"

// диалоговое окно CSetGearsDlg

IMPLEMENT_DYNAMIC(CSetGearsDlg, CDialog)

CSetGearsDlg::CSetGearsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_dShM(0)
	, m_dShZ1(0)
	, m_dShS(0)
	, m_dShZ2(0)
{

}

CSetGearsDlg::~CSetGearsDlg()
{
}

void CSetGearsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dShM);
	DDX_Text(pDX, IDC_EDIT3, m_dShZ1);
	DDX_Text(pDX, IDC_EDIT4, m_dShS);
	DDX_Text(pDX, IDC_EDIT2, m_dShZ2);
}


BEGIN_MESSAGE_MAP(CSetGearsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetGearsDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// обработчики сообщений CSetGearsDlg


void CSetGearsDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);// данные из элементов попадают в переменные
	
	//„≈ ѕј–јћ
	//‘”Ќ ÷»я ѕ≈–≈—„≈“ј
	FunDiv10();
	m_pDoc->OpenInventor();
	m_pDoc->Gear(1, VSpB1, VSpL, VSpT1, df, m_dShS, p, ya, xf, yf, yd, d, m_dShZ1, a, Do, Dc, m_V1d2);
	m_pDoc->Gear(2, VSpB2, VSpL, VSpT2, df2, m_dShS, p, ya2, xf, yf2, yd2, d2, m_dShZ2, a, Do2, Dc, m_V1d2);
	FunDove10();
}

void CSetGearsDlg::FunDiv10()
{
	m_dL1 /= 10;
	m_dL2 /= 10;
	m_dL3 /= 10;
	m_dL4 /= 10;

	m_dShS /= 10;

	m_V1d1 /= 10;
	m_V1d2 /= 10;
	m_V1d3 /= 10;

	m_V2d3 /= 10;
	m_V2d2 /= 10;
	m_V2d1 /= 10;

	m_V1l1 /= 10;
	m_V1l2 /= 10;
	m_V1l3 /= 10;
	m_V1l4 /= 10;

	m_V2l1 /= 10;
	m_V2l2 /= 10;
	m_V2l3 /= 10;
	m_V2l4 /= 10;

	m_Vr /= 10;
	m_Vh /= 10;
	m_Vl /= 10;

	d /= 10;
	da /= 10;
	df /= 10;
	p /= 10;
	yd /= 10;
	ya /= 10;
	yf /= 10;
	xf /= 10;
	a /= 10;
	d2 /= 10;
	da2 /= 10;
	df2 /= 10;
	yd2 /= 10;
	ya2 /= 10;
	yf2 /= 10;
	Do /= 10;
	Dc /= 10;
	Do2 /= 10;

	m_P1h /= 10;
	m_P1r /= 10;
	m_P2h /= 10;
	m_P2r /= 10;

	VSpT1 /= 10;
	VSpT2 /= 10;
	VSpL /= 10;
	VSpB1 /= 10;
	VSpB2 /= 10;
}

void CSetGearsDlg::FunDove10()
{
	m_dL1 *= 10;
	m_dL2 *= 10;
	m_dL3 *= 10;
	m_dL4 *= 10;

	m_dShS *= 10;

	m_V1d1 *= 10;
	m_V1d2 *= 10;
	m_V1d3 *= 10;

	m_V2d3 *= 10;
	m_V2d2 *= 10;
	m_V2d1 *= 10;

	m_V1l1 *= 10;
	m_V1l2 *= 10;
	m_V1l3 *= 10;
	m_V1l4 *= 10;

	m_V2l1 *= 10;
	m_V2l2 *= 10;
	m_V2l3 *= 10;
	m_V2l4 *= 10;

	m_Vr *= 10;
	m_Vh *= 10;
	m_Vl *= 10;

	d *= 10;
	da *= 10;
	df *= 10;
	p *= 10;
	yd *= 10;
	ya *= 10;
	yf *= 10;
	xf *= 10;
	a *= 10;
	d2 *= 10;
	da2 *= 10;
	df2 *= 10;
	yd2 *= 10;
	ya2 *= 10;
	yf2 *= 10;
	Do *= 10;
	Dc *= 10;
	Do2 *= 10;

	m_P1h *= 10;
	m_P1r *= 10;
	m_P2h *= 10;
	m_P2r *= 10;

	VSpT1 *= 10;
	VSpT2 *= 10;
	VSpL *= 10;
	VSpB1 *= 10;
	VSpB2 *= 10;
}
