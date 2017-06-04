#pragma once


// ���������� ���� CSetShaft2Dlg

class CSetShaft2Dlg : public CDialog
{
	DECLARE_DYNAMIC(CSetShaft2Dlg)

public:
	CSetShaft2Dlg(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CSetShaft2Dlg();
	CAssemblyConfigDoc* m_pDoc;
// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	double m_V2d1;
	double m_V2d2;
	double m_V2d3;
	double m_V2l1;
	double m_V2l2;
	double m_V2l3;
	double m_V2l4;

	double m_dShM;
	double m_dShZ1;
	double m_dShS;
	double m_dShZ2;


	double m_dL1, m_dL2, m_dL3, m_dL4;//��������� ������

									  //double m_dShM, m_dShZ1, m_dShS, m_dShZ2;//�������� ������

	double m_V1d1, m_V1d2, m_V1d3;//�������� ���� 1

	//double m_V2d1, m_V2d2, m_V2d3;//�������� ���� 2

	double m_V1l1, m_V1l2, m_V1l3, m_V1l4;//����� ���� 1

	//double m_V2l1, m_V2l2, m_V2l3, m_V2l4;//����� ���� 2

	double m_Vh;//��������� ��� 2




	double d, da, df, p, yd, ya, yf, xf, a;//��������� ���� 1
	double d2, da2, df2, yd2, ya2, yf2;//��������� ���� 2
	double Do, Dc;//��������� ������ 1
	double Do2;//��������� ������ 2
			   //��� 1
	double m_P1h;//������� ������
	double m_P1r;//������ ���� �������
	int m_P1num;//����� ��� �������
				//��� 2
	double m_P2h;//������� ������
	double m_P2r;//������ ���� �������
	int m_P2num;//����� ��� �������

	double VSpB1, VSpL, VSpT1, VSpB2, VSpT2;//��������� ������
	afx_msg void OnBnClickedButton1();
	double m_Vr;
	double m_Vl;
};
