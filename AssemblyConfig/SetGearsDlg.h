#pragma once


// диалоговое окно CSetGearsDlg

class CSetGearsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetGearsDlg)

public:
	CSetGearsDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CSetGearsDlg();
	CAssemblyConfigDoc* m_pDoc;
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	void FunDiv10();
	void FunDove10();
	double m_dShM;
	double m_dShZ1;
	double m_dShS;
	double m_dShZ2;

	
	double m_dL1, m_dL2, m_dL3, m_dL4;//параметры сборки

	//double m_dShM, m_dShZ1, m_dShS, m_dShZ2;//Зубчатое колесо

	double m_V1d1, m_V1d2, m_V1d3;//диаметры вала 1

	double m_V2d1, m_V2d2, m_V2d3;//диаметры вала 2

	double m_V1l1, m_V1l2, m_V1l3, m_V1l4;//длины вала 1

	double m_V2l1, m_V2l2, m_V2l3, m_V2l4;//длины вала 2

	double m_Vr, m_Vh, m_Vl;//отверстие вал 2




	double d, da, df, p, yd, ya, yf, xf, a;//параметры зуба 1
	double d2, da2, df2, yd2, ya2, yf2;//параметры зуба 2
	double Do, Dc;//параметры колеса 1
	double Do2;//параметры колеса 2
			   //вал 1
	double m_P1h;//толшина кольца
	double m_P1r;//радиус тела качения
	int m_P1num;//число тел качения
				//вал 2
	double m_P2h;//толшина кольца
	double m_P2r;//радиус тела качения
	int m_P2num;//число тел качения

	double VSpB1, VSpL, VSpT1, VSpB2, VSpT2;//параметры шпонки
	//double m_dShZ2;
	
};
