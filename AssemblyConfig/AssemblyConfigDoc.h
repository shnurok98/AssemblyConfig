
// AssemblyConfigDoc.h : ��������� ������ CAssemblyConfigDoc
//


#pragma once


class CAssemblyConfigDoc : public CDocument
{
protected: // ������� ������ �� ������������
	CAssemblyConfigDoc();
	DECLARE_DYNCREATE(CAssemblyConfigDoc)

// ��������
public:

// ��������
public:
	double m_dL1, m_dL2, m_dL3, m_dL4;//��������� ������

	double m_dShM, m_dShZ1, m_dShS, m_dShZ2;//�������� ������

	double m_V1d1, m_V1d2, m_V1d3;//�������� ���� 1

	double m_V2d1, m_V2d2, m_V2d3;//�������� ���� 2

	double m_V1l1, m_V1l2, m_V1l3, m_V1l4;//����� ���� 1

	double m_V2l1, m_V2l2, m_V2l3, m_V2l4;//����� ���� 2

	double m_Vh, m_Vr, m_Vl;//��������� ��� 2




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
	double aw;
// ���������������
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//virtual void TestBox();
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ����������
public:
	virtual ~CAssemblyConfigDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ��������������� �������, �������� ���������� ������ ��� ����������� ������
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnSetgears();
	afx_msg void OnSetshaft1();
	afx_msg void OnSetshaft2();
	afx_msg void OnSetassembly();
	void Gear(int Gi, double VSpB, double VSpL, double VSpT, double df, double GS, double p, double ya, double xf, double yf, double yd, double d, double GZ, double a, double Do, double Dc, double Dv);
	void Bearing(int Gi, double m_Vl2, double m_Vd1, double m_Ph, double m_Pr, double m_Pnum);
	void Shaft(int Gi, double VSpB, double VSpL, double VSpT, double L, double m_Vd3, double m_Vl4, double m_Vd1, double m_Vl2, double m_Vd2, double m_Vl1, double m_Vh, double m_Vl3, double m_Vl, double m_Vr);
	void Assembly(double aw, double m_V1d2, double m_V2d2, double VSpT1, double VSpT2, double m_dL3, double m_V2l2, double VSpB1, double VSpL);
	void OpenInventor();
	void FunDiv10();
	void CheckupPar();
	void ParamsMaker();
};
