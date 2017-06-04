
// AssemblyConfigDoc.cpp : реализация класса CAssemblyConfigDoc
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "AssemblyConfig.h"
#endif

#include "AssemblyConfigDoc.h"
#include "SetGearsDlg.h"
#include "SetShaft1Dlg.h"
#include "SetShaft2Dlg.h"
#include "SetAssemblyDlg.h"
#include <propkey.h>

#define pi 3.14159265
// Подключение инвентора
#pragma once
#include <atlbase.h>
#import <RxInventor.tlb> \
rename_namespace("InventorNative") \
named_guids raw_dispinterfaces \
high_method_prefix("Method") \
rename("DeleteFile", "APIDeleteFile") \
rename("CopyFile", "APICopyFile") \
rename("MoveFile", "APIMoveFile") \
rename("SetEnvironmentVariable", "APISetEnvironmentVariable") \
rename("GetObject", "APIGetObject") \
exclude("_FILETIME", "IStream", "ISequentialStream", \
"_LARGE_INTEGER", "_ULARGE_INTEGER", "tagSTATSTG", \
"IEnumUnknown", "IPersistFile", "IPersist", "IPictureDisp")

using namespace InventorNative;

CComPtr<Application> pInvApp;
//InventorNative::DocumentPtr pPartDoc;

InventorNative::PartDocumentPtr pPartDocB1;
InventorNative::PartDocumentPtr pPartDocB2;

InventorNative::PartDocumentPtr pPartDocV1;
InventorNative::PartDocumentPtr pPartDocV2;

InventorNative::PartDocumentPtr pPartDocG1;
InventorNative::PartDocumentPtr pPartDocG2;

InventorNative::PartComponentDefinition *pPartComDef;
InventorNative::TransientGeometry *pTransGeom;
InventorNative::Transaction *pTrans;

InventorNative::PartDocumentPtr pPartDoc1;
InventorNative::PartComponentDefinition *pPartComDef1;
InventorNative::TransientGeometry *pTransGeom1;
InventorNative::Transaction *pTrans1;

InventorNative::AssemblyDocumentPtr pAsmDoc;
InventorNative::AssemblyComponentDefinitionPtr pAsmComDef;
InventorNative::TransientGeometry *pTransGeomAsm;
InventorNative::Matrix *oPositionMatrix;

TransactionManagerPtr pTransManager;
Document *Doc;

PlanarSketches *sketches;
Sketches3D   *sketches3D;
WorkPlanes *wp;
PartFeatures *ft;
WorkAxes *wax;
WorkPoints *wpt;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAssemblyConfigDoc

IMPLEMENT_DYNCREATE(CAssemblyConfigDoc, CDocument)

BEGIN_MESSAGE_MAP(CAssemblyConfigDoc, CDocument)
	ON_COMMAND(ID_SETGEARS, &CAssemblyConfigDoc::OnSetgears)
	ON_COMMAND(ID_SETSHAFT1, &CAssemblyConfigDoc::OnSetshaft1)
	ON_COMMAND(ID_SETSHAFT2, &CAssemblyConfigDoc::OnSetshaft2)
	ON_COMMAND(ID_SETASSEMBLY, &CAssemblyConfigDoc::OnSetassembly)
END_MESSAGE_MAP()


// создание/уничтожение CAssemblyConfigDoc

CAssemblyConfigDoc::CAssemblyConfigDoc()
{
	// TODO: добавьте код для одноразового вызова конструктора
	m_dL1 = 40;
	m_dL2 = 40;
	m_dL3 = 40;
	m_dL4 = 40;

	m_V1d1 = 35;
	m_V1d2 = 40;
	m_V1d3 = 32;

	m_V1l1 = 160;
	m_V1l2 = 20;
	m_V1l3 = 80;
	m_V1l4 = 60;

	m_V2d1 = 35;
	m_V2d2 = 40;
	m_V2d3 = 32;

	m_V2l1 = 160;
	m_V2l2 = 20;
	m_V2l3 = 80;
	m_V2l4 = 60;

	m_Vr = 4;
	m_Vh = 2;
	m_Vl = 10;

	m_dShS = 40;
	m_dShM = 6;//модуль
	m_dShZ1 = 17;//количество зубьев 1-го колеса
	m_dShZ2 = 34;//количество зубьев 2-го колеса






	d = m_dShM*m_dShZ1;//нач диаметр
	p = pi*m_dShM / 2;//толшина зуба
	da = (d + 2 * m_dShM);//диаметр выступов
	df = (d - 2.5*m_dShM);//диаметр впадин
	a = m_dShM / 2;

	//расчёт зуба
	yd = sqrt(d*d / 4 - p*p / 4);
	ya = sqrt(da*da / 4 - p*p / 16);
	xf = 9 * p / 14;
	yf = sqrt(df*df / 4 - xf*xf);
	//расчёт параметров колеса
	Do = da - 8.5*m_dShM;
	Dc = 1.5*m_V1d2 + 1;


	d2 = m_dShM*m_dShZ2;
	da2 = (d2 + 2 * m_dShM);
	df2 = (d2 - 2.5*m_dShM);
	yd2 = sqrt(d2*d2 / 4 - p*p / 4);
	ya2 = sqrt(da2*da2 / 4 - p*p / 16);
	yf2 = sqrt(df2*df2 / 4 - xf*xf);
	Do2 = da2 - 8.5*m_dShM;


	m_P1h = ((m_V1d1 / 2) * 3 - m_V1d1 / 2) / 3;
	m_P1r = sqrt(m_V1l2 / 6 * m_V1l2 / 6 + m_P1h / 2 * m_P1h / 2) - 0.01;
	m_P1num = 5;
	while ((2 * m_P1r + 0.4) < ((2 * m_V1d1 / 2 + 1.5 * m_P1h) * sin(pi / m_P1num))) {
		m_P1num++;
	}

	m_P2h = ((m_V2d1 / 2) * 3 - m_V2d1 / 2) / 3;
	m_P2r = sqrt(m_V2l2 / 6 * m_V2l2 / 6 + m_P2h / 2 * m_P2h / 2) - 0.01;
	m_P2num = 5;
	while ((2 * m_P2r + 0.4) < ((2 * m_V2d1 / 2 + 1.5 * m_P2h) * sin(pi / m_P2num))) {
		m_P2num++;
	}

	VSpT1 = m_V1d3 / 8;
	VSpT2 = m_V2d3 / 8;
	VSpL = m_dShS - 10;
	VSpB1 = m_V1d3 / 4;
	VSpB2 = m_V2d3 / 4;

	aw = (m_dShZ1 + m_dShZ2)*m_dShM / 2;
	//CheckupPar();
	//FunDiv10();
}

CAssemblyConfigDoc::~CAssemblyConfigDoc()
{
}

BOOL CAssemblyConfigDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}




// сериализация CAssemblyConfigDoc

void CAssemblyConfigDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		// TODO: добавьте код загрузки
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CAssemblyConfigDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CAssemblyConfigDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задайте содержимое поиска из данных документа. 
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CAssemblyConfigDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// диагностика CAssemblyConfigDoc

#ifdef _DEBUG
void CAssemblyConfigDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAssemblyConfigDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// команды CAssemblyConfigDoc


void CAssemblyConfigDoc::OnSetgears()
{
	// TODO: добавьте свой код обработчика команд
	
	CSetGearsDlg dlg;
	dlg.m_pDoc = this;

	dlg.m_dShM = m_dShM;
	dlg.m_dShZ1 = m_dShZ1;
	dlg.m_dShZ2 = m_dShZ2;
	dlg.m_dShS = m_dShS;

	dlg.m_dL1 = m_dL1;
	dlg.m_dL2 = m_dL2;
	dlg.m_dL3 = m_dL3;
	dlg.m_dL4 = m_dL4;

	dlg.m_V1d1 = m_V1d1;
	dlg.m_V1d2 = m_V1d2;
	dlg.m_V1d3 = m_V1d3;

	dlg.m_V2d1 = m_V2d1;
	dlg.m_V2d2 = m_V2d2;
	dlg.m_V2d3 = m_V2d3;

	dlg.m_V1l1 = m_V1l1;
	dlg.m_V1l2 = m_V1l2;
	dlg.m_V1l3 = m_V1l3;
	dlg.m_V1l4 = m_V1l4;

	dlg.m_V2l1 = m_V2l1;
	dlg.m_V2l2 = m_V2l2;
	dlg.m_V2l3 = m_V2l3;
	dlg.m_V2l4 = m_V2l4;

	dlg.m_Vr = m_Vr;
	dlg.m_Vh = m_Vh;
	dlg.m_Vl = m_Vl;

	dlg.d = d;
	dlg.da = da;
	dlg.df = df;
	dlg.p = p;
	dlg.yd = yd;
	dlg.ya = ya;
	dlg.yf = yf;
	dlg.xf = xf;
	dlg.a = a;

	dlg.d2 = d2;
	dlg.da2 = da2;
	dlg.df2 = df2;
	dlg.yd2 = yd2;
	dlg.ya2 = ya2;
	dlg.yf2 = yf2;

	dlg.Do = Do;
	dlg.Dc = Dc;
	dlg.Do2 = Do2;

	dlg.m_P1h = m_P1h;
	dlg.m_P1r = m_P1r;
	dlg.m_P1num = m_P1num;

	dlg.m_P2h = m_P2h;
	dlg.m_P2r = m_P2r;
	dlg.m_P2num = m_P2num;

	dlg.VSpB1 = VSpB1;
	dlg.VSpL = VSpL;
	dlg.VSpT1 = VSpT1;
	dlg.VSpB2 = VSpB2;
	dlg.VSpT2 = VSpT2;
	if (dlg.DoModal() == IDOK) {
		m_dShM = dlg.m_dShM;
		m_dShZ1 = dlg.m_dShZ1;
		m_dShZ2 = dlg.m_dShZ2;
		m_dShS = dlg.m_dShS;

		m_dL1 = dlg.m_dL1;
		m_dL2 = dlg.m_dL2;
		m_dL3 = dlg.m_dL3;
		m_dL4 = dlg.m_dL4;

		m_V1d1 = dlg.m_V1d1;
		m_V1d2 = dlg.m_V1d2;
		m_V1d3 = dlg.m_V1d3;

		m_V2d1 = dlg.m_V2d1;
		m_V2d2 = dlg.m_V2d2;
		m_V2d3 = dlg.m_V2d3;

		m_V1l1 = dlg.m_V1l1;
		m_V1l2 = dlg.m_V1l2;
		m_V1l3 = dlg.m_V1l3;
		m_V1l4 = dlg.m_V1l4;

		m_V2l1 = dlg.m_V2l1;
		m_V2l2 = dlg.m_V2l2;
		m_V2l3 = dlg.m_V2l3;
		m_V2l4 = dlg.m_V2l4;

		m_Vr = dlg.m_Vr;
		m_Vh = dlg.m_Vh;
		m_Vl = dlg.m_Vl;

		d = dlg.d;
		da = dlg.da;
		df = dlg.df;
		p = dlg.p;
		yd = dlg.yd;
		ya = dlg.ya;
		yf = dlg.yf;
		xf = dlg.xf;
		a = dlg.a;

		d2 = dlg.d2;
		da2 = dlg.da2;
		df2 = dlg.df2;
		yd2 = dlg.yd2;
		ya2 = dlg.ya2;
		yf2 = dlg.yf2;

		Do = dlg.Do;
		Dc = dlg.Dc;
		Do2 = dlg.Do2;

		m_P1h = dlg.m_P1h;
		m_P1r = dlg.m_P1r;
		m_P1num = dlg.m_P1num;

		m_P2h = dlg.m_P2h;
		m_P2r = dlg.m_P2r;
		m_P2num = dlg.m_P2num;

		VSpB1 = dlg.VSpB1;
		VSpL = dlg.VSpL;
		VSpT1 = dlg.VSpT1;
		VSpB2 = dlg.VSpB2;
		VSpT2 = dlg.VSpT2;

		ParamsMaker();
		FunDiv10();
	}

}


void CAssemblyConfigDoc::OnSetshaft1()
{
	// TODO: добавьте свой код обработчика команд

	CSetShaft1 dlg;
	dlg.m_pDoc = this;

	dlg.m_dShM = m_dShM;
	dlg.m_dShZ1 = m_dShZ1;
	dlg.m_dShZ2 = m_dShZ2;
	dlg.m_dShS = m_dShS;

	dlg.m_dL1 = m_dL1;
	dlg.m_dL2 = m_dL2;
	dlg.m_dL3 = m_dL3;
	dlg.m_dL4 = m_dL4;

	dlg.m_V1d1 = m_V1d1;
	dlg.m_V1d2 = m_V1d2;
	dlg.m_V1d3 = m_V1d3;

	dlg.m_V2d1 = m_V2d1;
	dlg.m_V2d2 = m_V2d2;
	dlg.m_V2d3 = m_V2d3;

	dlg.m_V1l1 = m_V1l1;
	dlg.m_V1l2 = m_V1l2;
	dlg.m_V1l3 = m_V1l3;
	dlg.m_V1l4 = m_V1l4;

	dlg.m_V2l1 = m_V2l1;
	dlg.m_V2l2 = m_V2l2;
	dlg.m_V2l3 = m_V2l3;
	dlg.m_V2l4 = m_V2l4;

	dlg.m_Vr = m_Vr;
	dlg.m_Vh = m_Vh;
	dlg.m_Vl = m_Vl;

	dlg.d = d;
	dlg.da = da;
	dlg.df = df;
	dlg.p = p;
	dlg.yd = yd;
	dlg.ya = ya;
	dlg.yf = yf;
	dlg.xf = xf;
	dlg.a = a;

	dlg.d2 = d2;
	dlg.da2 = da2;
	dlg.df2 = df2;
	dlg.yd2 = yd2;
	dlg.ya2 = ya2;
	dlg.yf2 = yf2;

	dlg.Do = Do;
	dlg.Dc = Dc;
	dlg.Do2 = Do2;

	dlg.m_P1h = m_P1h;
	dlg.m_P1r = m_P1r;
	dlg.m_P1num = m_P1num;

	dlg.m_P2h = m_P2h;
	dlg.m_P2r = m_P2r;
	dlg.m_P2num = m_P2num;

	dlg.VSpB1 = VSpB1;
	dlg.VSpL = VSpL;
	dlg.VSpT1 = VSpT1;
	dlg.VSpB2 = VSpB2;
	dlg.VSpT2 = VSpT2;
	if (dlg.DoModal() == IDOK) {
		m_dShM = dlg.m_dShM;
		m_dShZ1 = dlg.m_dShZ1;
		m_dShZ2 = dlg.m_dShZ2;
		m_dShS = dlg.m_dShS;

		m_dL1 = dlg.m_dL1;
		m_dL2 = dlg.m_dL2;
		m_dL3 = dlg.m_dL3;
		m_dL4 = dlg.m_dL4;

		m_V1d1 = dlg.m_V1d1;
		m_V1d2 = dlg.m_V1d2;
		m_V1d3 = dlg.m_V1d3;

		m_V2d1 = dlg.m_V2d1;
		m_V2d2 = dlg.m_V2d2;
		m_V2d3 = dlg.m_V2d3;

		m_V1l1 = dlg.m_V1l1;
		m_V1l2 = dlg.m_V1l2;
		m_V1l3 = dlg.m_V1l3;
		m_V1l4 = dlg.m_V1l4;

		m_V2l1 = dlg.m_V2l1;
		m_V2l2 = dlg.m_V2l2;
		m_V2l3 = dlg.m_V2l3;
		m_V2l4 = dlg.m_V2l4;

		m_Vr = dlg.m_Vr;
		m_Vh = dlg.m_Vh;
		m_Vl = dlg.m_Vl;

		d = dlg.d;
		da = dlg.da;
		df = dlg.df;
		p = dlg.p;
		yd = dlg.yd;
		ya = dlg.ya;
		yf = dlg.yf;
		xf = dlg.xf;
		a = dlg.a;

		d2 = dlg.d2;
		da2 = dlg.da2;
		df2 = dlg.df2;
		yd2 = dlg.yd2;
		ya2 = dlg.ya2;
		yf2 = dlg.yf2;

		Do = dlg.Do;
		Dc = dlg.Dc;
		Do2 = dlg.Do2;

		m_P1h = dlg.m_P1h;
		m_P1r = dlg.m_P1r;
		m_P1num = dlg.m_P1num;

		m_P2h = dlg.m_P2h;
		m_P2r = dlg.m_P2r;
		m_P2num = dlg.m_P2num;

		VSpB1 = dlg.VSpB1;
		VSpL = dlg.VSpL;
		VSpT1 = dlg.VSpT1;
		VSpB2 = dlg.VSpB2;
		VSpT2 = dlg.VSpT2;

		ParamsMaker();
	}

}


void CAssemblyConfigDoc::OnSetshaft2()
{
	// TODO: добавьте свой код обработчика команд

	CSetShaft2Dlg dlg;
	dlg.m_pDoc = this;

	dlg.m_dShM = m_dShM;
	dlg.m_dShZ1 = m_dShZ1;
	dlg.m_dShZ2 = m_dShZ2;
	dlg.m_dShS = m_dShS;

	dlg.m_dL1 = m_dL1;
	dlg.m_dL2 = m_dL2;
	dlg.m_dL3 = m_dL3;
	dlg.m_dL4 = m_dL4;

	dlg.m_V1d1 = m_V1d1;
	dlg.m_V1d2 = m_V1d2;
	dlg.m_V1d3 = m_V1d3;

	dlg.m_V2d1 = m_V2d1;
	dlg.m_V2d2 = m_V2d2;
	dlg.m_V2d3 = m_V2d3;

	dlg.m_V1l1 = m_V1l1;
	dlg.m_V1l2 = m_V1l2;
	dlg.m_V1l3 = m_V1l3;
	dlg.m_V1l4 = m_V1l4;

	dlg.m_V2l1 = m_V2l1;
	dlg.m_V2l2 = m_V2l2;
	dlg.m_V2l3 = m_V2l3;
	dlg.m_V2l4 = m_V2l4;

	dlg.m_Vr = m_Vr;
	dlg.m_Vh = m_Vh;
	dlg.m_Vl = m_Vl;

	dlg.d = d;
	dlg.da = da;
	dlg.df = df;
	dlg.p = p;
	dlg.yd = yd;
	dlg.ya = ya;
	dlg.yf = yf;
	dlg.xf = xf;
	dlg.a = a;

	dlg.d2 = d2;
	dlg.da2 = da2;
	dlg.df2 = df2;
	dlg.yd2 = yd2;
	dlg.ya2 = ya2;
	dlg.yf2 = yf2;

	dlg.Do = Do;
	dlg.Dc = Dc;
	dlg.Do2 = Do2;

	dlg.m_P1h = m_P1h;
	dlg.m_P1r = m_P1r;
	dlg.m_P1num = m_P1num;

	dlg.m_P2h = m_P2h;
	dlg.m_P2r = m_P2r;
	dlg.m_P2num = m_P2num;

	dlg.VSpB1 = VSpB1;
	dlg.VSpL = VSpL;
	dlg.VSpT1 = VSpT1;
	dlg.VSpB2 = VSpB2;
	dlg.VSpT2 = VSpT2;
	if (dlg.DoModal() == IDOK) {
		m_dShM = dlg.m_dShM;
		m_dShZ1 = dlg.m_dShZ1;
		m_dShZ2 = dlg.m_dShZ2;
		m_dShS = dlg.m_dShS;

		m_dL1 = dlg.m_dL1;
		m_dL2 = dlg.m_dL2;
		m_dL3 = dlg.m_dL3;
		m_dL4 = dlg.m_dL4;

		m_V1d1 = dlg.m_V1d1;
		m_V1d2 = dlg.m_V1d2;
		m_V1d3 = dlg.m_V1d3;

		m_V2d1 = dlg.m_V2d1;
		m_V2d2 = dlg.m_V2d2;
		m_V2d3 = dlg.m_V2d3;

		m_V1l1 = dlg.m_V1l1;
		m_V1l2 = dlg.m_V1l2;
		m_V1l3 = dlg.m_V1l3;
		m_V1l4 = dlg.m_V1l4;

		m_V2l1 = dlg.m_V2l1;
		m_V2l2 = dlg.m_V2l2;
		m_V2l3 = dlg.m_V2l3;
		m_V2l4 = dlg.m_V2l4;

		m_Vr = dlg.m_Vr;
		m_Vh = dlg.m_Vh;
		m_Vl = dlg.m_Vl;

		d = dlg.d;
		da = dlg.da;
		df = dlg.df;
		p = dlg.p;
		yd = dlg.yd;
		ya = dlg.ya;
		yf = dlg.yf;
		xf = dlg.xf;
		a = dlg.a;

		d2 = dlg.d2;
		da2 = dlg.da2;
		df2 = dlg.df2;
		yd2 = dlg.yd2;
		ya2 = dlg.ya2;
		yf2 = dlg.yf2;

		Do = dlg.Do;
		Dc = dlg.Dc;
		Do2 = dlg.Do2;

		m_P1h = dlg.m_P1h;
		m_P1r = dlg.m_P1r;
		m_P1num = dlg.m_P1num;

		m_P2h = dlg.m_P2h;
		m_P2r = dlg.m_P2r;
		m_P2num = dlg.m_P2num;

		VSpB1 = dlg.VSpB1;
		VSpL = dlg.VSpL;
		VSpT1 = dlg.VSpT1;
		VSpB2 = dlg.VSpB2;
		VSpT2 = dlg.VSpT2;

		ParamsMaker();
	}

}


void CAssemblyConfigDoc::OnSetassembly()
{
	// TODO: добавьте свой код обработчика команд

	CSetAssemblyDlg dlg;
	dlg.m_pDoc = this;

	dlg.m_dShM = m_dShM;
	dlg.m_dShZ1 = m_dShZ1;
	dlg.m_dShZ2 = m_dShZ2;
	dlg.m_dShS = m_dShS;

	dlg.m_dL1 = m_dL1;
	dlg.m_dL2 = m_dL2;
	dlg.m_dL3 = m_dL3;
	dlg.m_dL4 = m_dL4;

	dlg.m_V1d1 = m_V1d1;
	dlg.m_V1d2 = m_V1d2;
	dlg.m_V1d3 = m_V1d3;

	dlg.m_V2d1 = m_V2d1;
	dlg.m_V2d2 = m_V2d2;
	dlg.m_V2d3 = m_V2d3;

	dlg.m_V1l1 = m_V1l1;
	dlg.m_V1l2 = m_V1l2;
	dlg.m_V1l3 = m_V1l3;
	dlg.m_V1l4 = m_V1l4;

	dlg.m_V2l1 = m_V2l1;
	dlg.m_V2l2 = m_V2l2;
	dlg.m_V2l3 = m_V2l3;
	dlg.m_V2l4 = m_V2l4;

	dlg.m_Vr = m_Vr;
	dlg.m_Vh = m_Vh;
	dlg.m_Vl = m_Vl;

	dlg.d = d;
	dlg.da = da;
	dlg.df = df;
	dlg.p = p;
	dlg.yd = yd;
	dlg.ya = ya;
	dlg.yf = yf;
	dlg.xf = xf;
	dlg.a = a;

	dlg.d2 = d2;
	dlg.da2 = da2;
	dlg.df2 = df2;
	dlg.yd2 = yd2;
	dlg.ya2 = ya2;
	dlg.yf2 = yf2;

	dlg.Do = Do;
	dlg.Dc = Dc;
	dlg.Do2 = Do2;

	dlg.m_P1h = m_P1h;
	dlg.m_P1r = m_P1r;
	dlg.m_P1num = m_P1num;

	dlg.m_P2h = m_P2h;
	dlg.m_P2r = m_P2r;
	dlg.m_P2num = m_P2num;

	dlg.VSpB1 = VSpB1;
	dlg.VSpL = VSpL;
	dlg.VSpT1 = VSpT1;
	dlg.VSpB2 = VSpB2;
	dlg.VSpT2 = VSpT2;

	dlg.aw = aw;

	if (dlg.DoModal() == IDOK) {
		m_dShM = dlg.m_dShM;
		m_dShZ1 = dlg.m_dShZ1;
		m_dShZ2 = dlg.m_dShZ2;
		m_dShS = dlg.m_dShS;

		m_dL1 = dlg.m_dL1;
		m_dL2 = dlg.m_dL2;
		m_dL3 = dlg.m_dL3;
		m_dL4 = dlg.m_dL4;

		m_V1d1 = dlg.m_V1d1;
		m_V1d2 = dlg.m_V1d2;
		m_V1d3 = dlg.m_V1d3;

		m_V2d1 = dlg.m_V2d1;
		m_V2d2 = dlg.m_V2d2;
		m_V2d3 = dlg.m_V2d3;

		m_V1l1 = dlg.m_V1l1;
		m_V1l2 = dlg.m_V1l2;
		m_V1l3 = dlg.m_V1l3;
		m_V1l4 = dlg.m_V1l4;

		m_V2l1 = dlg.m_V2l1;
		m_V2l2 = dlg.m_V2l2;
		m_V2l3 = dlg.m_V2l3;
		m_V2l4 = dlg.m_V2l4;

		m_Vr = dlg.m_Vr;
		m_Vh = dlg.m_Vh;
		m_Vl = dlg.m_Vl;

		d = dlg.d;
		da = dlg.da;
		df = dlg.df;
		p = dlg.p;
		yd = dlg.yd;
		ya = dlg.ya;
		yf = dlg.yf;
		xf = dlg.xf;
		a = dlg.a;

		d2 = dlg.d2;
		da2 = dlg.da2;
		df2 = dlg.df2;
		yd2 = dlg.yd2;
		ya2 = dlg.ya2;
		yf2 = dlg.yf2;

		Do = dlg.Do;
		Dc = dlg.Dc;
		Do2 = dlg.Do2;

		m_P1h = dlg.m_P1h;
		m_P1r = dlg.m_P1r;
		m_P1num = dlg.m_P1num;

		m_P2h = dlg.m_P2h;
		m_P2r = dlg.m_P2r;
		m_P2num = dlg.m_P2num;

		VSpB1 = dlg.VSpB1;
		VSpL = dlg.VSpL;
		VSpT1 = dlg.VSpT1;
		VSpB2 = dlg.VSpB2;
		VSpT2 = dlg.VSpT2;

		aw = dlg.aw;

		ParamsMaker();

	}


}

void CAssemblyConfigDoc::Gear(int Gi, double VSpB, double VSpL, double VSpT, double df, double GS, double p, double ya, double xf, double yf, double yd, double d, double GZ, double a, double Do, double Dc, double Dv)
{
	if (Gi == 1)
	{
		pPartDocG1 = pInvApp->Documents->MethodAdd(kPartDocumentObject, pInvApp->FileManager->MethodGetTemplateFile(kPartDocumentObject, kMetricSystemOfMeasure, kGOST_DraftingStandard), true);

		pPartDocG1->DisplayName = _T("Gear 1");

		pPartDocG1->get_ComponentDefinition(&pPartComDef);

		pInvApp->get_TransientGeometry(&pTransGeom);

		pTransManager = pInvApp->GetTransactionManager();

		Doc = CComQIPtr <Document>(pPartDocG1);
	}
	else
	{
		pPartDocG2 = pInvApp->Documents->MethodAdd(kPartDocumentObject, pInvApp->FileManager->MethodGetTemplateFile(kPartDocumentObject, kMetricSystemOfMeasure, kGOST_DraftingStandard), true);

		pPartDocG2->DisplayName = _T("Gear 2");

		pPartDocG2->get_ComponentDefinition(&pPartComDef);

		pInvApp->get_TransientGeometry(&pTransGeom);

		pTransManager = pInvApp->GetTransactionManager();

		Doc = CComQIPtr <Document>(pPartDocG2);
	}


	pTransManager->raw_StartTransaction(Doc, _T(""), &pTrans);

	pPartComDef->get_Sketches(&sketches);
	pPartComDef->get_Sketches3D(&sketches3D);

	pPartComDef->get_WorkPlanes(&wp);
	pPartComDef->get_Features(&ft);
	pPartComDef->get_WorkAxes(&wax);
	pPartComDef->get_WorkPoints(&wpt);




	PlanarSketch *pSketch;
	sketches->raw_Add(wp->GetItem(3), false, &pSketch);

	SketchCirclesPtr Circ[1];

	SketchCircles *skCirc;
	Profiles *skProfiles;

	pSketch->get_SketchCircles(&skCirc);
	pSketch->get_Profiles(&skProfiles);


	Circ[0] = skCirc->MethodAddByCenterRadius(pTransGeom->MethodCreatePoint2d(0, 0), df / 2);//окружность впадин шестерни



	Profile *pProfile;
	skProfiles->raw__AddForSolid(&pProfile);

	ExtrudeFeatures *ftExtrude;
	ft->get_ExtrudeFeatures(&ftExtrude);
	ExtrudeFeaturePtr oExtrudeDef = ftExtrude->MethodAddByDistanceExtent(pProfile, GS, kSymmetricExtentDirection, kJoinOperation);
	//2 эскиз
	PlanarSketch *pSketch1;
	sketches->raw_Add(wp->GetItem(3), false, &pSketch1);

	SketchPointPtr point1[4];
	SketchLinePtr lines1[2];
	SketchArcsPtr Arcs1[2];
	SketchPoints *skPoints1;
	SketchLines *skLines1;
	Profiles *skProfiles1;
	SketchArcs *skArcs1;


	pSketch1->get_SketchPoints(&skPoints1);
	pSketch1->get_SketchLines(&skLines1);
	pSketch1->get_Profiles(&skProfiles1);
	pSketch1->get_SketchArcs(&skArcs1);

	//точки для дуги
	point1[0] = skPoints1->MethodAdd(pTransGeom->MethodCreatePoint2d(-p / 4, ya), false);
	point1[1] = skPoints1->MethodAdd(pTransGeom->MethodCreatePoint2d(-xf, yf), false);
	point1[2] = skPoints1->MethodAdd(pTransGeom->MethodCreatePoint2d(p / 4, ya), false);
	point1[3] = skPoints1->MethodAdd(pTransGeom->MethodCreatePoint2d(xf, yf), false);

	//дуги зубца
	Arcs1[0] = skArcs1->MethodAddByThreePoints(point1[0], pTransGeom->MethodCreatePoint2d(-p / 2, yd), point1[1]);
	Arcs1[1] = skArcs1->MethodAddByThreePoints(point1[2], pTransGeom->MethodCreatePoint2d(p / 2, yd), point1[3]);
	//объединение дуг
	lines1[0] = skLines1->MethodAddByTwoPoints(point1[0], point1[2]);
	lines1[1] = skLines1->MethodAddByTwoPoints(point1[1], point1[3]);



	Profile *pProfile1;
	skProfiles1->raw__AddForSolid(&pProfile1);

	ExtrudeFeatures *ftExtrude1;
	ft->get_ExtrudeFeatures(&ftExtrude1);
	ExtrudeFeaturePtr oExtrudeDef1 = ftExtrude1->MethodAddByDistanceExtent(pProfile1, GS, kSymmetricExtentDirection, kJoinOperation);
	//скругление основания зуба

	FilletFeatures *pFilletFt;
	ft->get_FilletFeatures(&pFilletFt);

	EdgeCollection *edgeColl;

	pInvApp->TransientObjects->raw_CreateEdgeCollection(vtMissing, &edgeColl);

	SurfaceBody *SurfBody;
	SurfaceBodies *SurfBodies;

	pPartComDef->get_SurfaceBodies(&SurfBodies);

	SurfBodies->get_Item(1, &SurfBody);

	Edges *edges;

	SurfBody->get_Edges(&edges);

	Edge *ed1, *ed2;
	edges->get_Item(8, &ed1);
	edges->get_Item(6, &ed2);


	edgeColl->MethodAdd(ed1);
	edgeColl->MethodAdd(ed2);


	FilletFeaturePtr fillFeat = pFilletFt->MethodAddSimple(edgeColl, 2 * d / (10 * GZ), false, false, false, false, false, false); // скругление

	ChamferFeatures *chamFeatures;
	ft->get_ChamferFeatures(&chamFeatures);

	EdgeCollection *edgeColl1;

	pInvApp->TransientObjects->raw_CreateEdgeCollection(vtMissing, &edgeColl1);

	SurfaceBody *SurfBody1;
	SurfaceBodies *SurfBodies1;

	pPartComDef->get_SurfaceBodies(&SurfBodies1);

	SurfBodies1->get_Item(1, &SurfBody1);

	Edges *edges1;

	SurfBody1->get_Edges(&edges1);

	Edge *ed4, *ed3;
	edges1->get_Item(15, &ed3);
	edges1->get_Item(16, &ed4);


	edgeColl1->MethodAdd(ed3);
	edgeColl1->MethodAdd(ed4);

	ChamferFeaturePtr chamFeature = chamFeatures->MethodAddUsingDistance(edgeColl1, a, false, false, false);  //фаска


																											  //массив зубьев
	ObjectCollection *pCollection;

	pInvApp->TransientObjects->raw_CreateObjectCollection(vtMissing, &pCollection);

	pCollection->MethodAdd(oExtrudeDef1);
	pCollection->MethodAdd(chamFeature);
	pCollection->MethodAdd(fillFeat);

	CircularPatternFeatures *pCircPatFeat;


	ft->get_CircularPatternFeatures(&pCircPatFeat);


	int n = pCircPatFeat->GetCount();


	n = pCollection->GetCount();


	CircularPatternFeature *circFeat = pCircPatFeat->MethodAdd(pCollection, wax->GetItem(3), true, GZ, pi * 2, true, kIdenticalCompute);

	PlanarSketch *pSketch2;
	sketches->raw_Add(wp->GetItem(3), false, &pSketch2);

	SketchCirclesPtr Circ2[1];

	SketchCircles *skCirc2;
	Profiles *skProfiles2;

	pSketch2->get_SketchCircles(&skCirc2);
	pSketch2->get_Profiles(&skProfiles2);


	Circ2[0] = skCirc2->MethodAddByCenterRadius(pTransGeom->MethodCreatePoint2d(0, 0), Dv / 2);

	Profile *pProfile2;
	skProfiles2->raw__AddForSolid(&pProfile2);

	ExtrudeFeatures *ftExtrude2;
	ft->get_ExtrudeFeatures(&ftExtrude2);
	ExtrudeFeaturePtr oExtrudeDef2 = ftExtrude2->MethodAddByDistanceExtent(pProfile2, GS, kSymmetricExtentDirection, kCutOperation);

	if (Do > Dc)
	{
		PlanarSketch *pSketchE;
		sketches->raw_Add(wp->GetItem(3), false, &pSketchE);

		SketchCirclesPtr CircE[2];

		SketchCircles *skCircE;
		Profiles *skProfilesE;

		pSketchE->get_SketchCircles(&skCircE);
		pSketchE->get_Profiles(&skProfilesE);


		CircE[0] = skCircE->MethodAddByCenterRadius(pTransGeom->MethodCreatePoint2d(0, 0), Do / 2);
		CircE[0] = skCircE->MethodAddByCenterRadius(pTransGeom->MethodCreatePoint2d(0, 0), Dc / 2);

		Profile *pProfileE;
		skProfilesE->raw__AddForSolid(&pProfileE);

		ExtrudeFeatures *ftExtrudeE;
		ft->get_ExtrudeFeatures(&ftExtrudeE);
		ExtrudeFeaturePtr oExtrudeDefE = ftExtrudeE->MethodAddByDistanceExtent(pProfileE, GS, kSymmetricExtentDirection, kCutOperation);

		oExtrudeDefE = ftExtrudeE->MethodAddByDistanceExtent(pProfileE, GS / 2, kSymmetricExtentDirection, kJoinOperation);

	}
	//паз под шпонку
	PlanarSketch *pSketch3;
	sketches->raw_Add(wp->GetItem(3), false, &pSketch3);

	SketchPointPtr point3[4];
	SketchLinePtr lines3[4];
	SketchPoints *skPoints3;
	SketchLines *skLines3;
	Profiles *skProfiles3;



	pSketch3->get_SketchPoints(&skPoints3);
	pSketch3->get_SketchLines(&skLines3);
	pSketch3->get_Profiles(&skProfiles3);



	point3[0] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(VSpB / 2, 0), false);
	point3[1] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(VSpB / 2, Dv / 2 + VSpT / 2), false);
	point3[2] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(-VSpB / 2, Dv / 2 + VSpT / 2), false);
	point3[3] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(-VSpB / 2, 0), false);


	lines3[0] = skLines3->MethodAddByTwoPoints(point3[0], point3[1]);
	lines3[1] = skLines3->MethodAddByTwoPoints(point3[1], point3[2]);
	lines3[1] = skLines3->MethodAddByTwoPoints(point3[2], point3[3]);
	lines3[1] = skLines3->MethodAddByTwoPoints(point3[3], point3[0]);



	Profile *pProfile3;
	skProfiles3->raw__AddForSolid(&pProfile3);

	ExtrudeFeatures *ftExtrude3;
	ft->get_ExtrudeFeatures(&ftExtrude3);
	ExtrudeFeaturePtr oExtrudeDef3 = ftExtrude3->MethodAddByDistanceExtent(pProfile3, VSpL, kSymmetricExtentDirection, kCutOperation);

	pTrans->MethodEnd();
	
	if (Gi==1)
	pPartDocG1->MethodSaveAs("D:\\_Details\\Gear 1.ipt", true);
	else
	pPartDocG2->MethodSaveAs("D:\\_Details\\Gear 2.ipt", false);
}

void CAssemblyConfigDoc::Bearing(int Gi, double m_Vl2, double m_Vd1, double m_Ph, double m_Pr, double m_Pnum)
{
	if (Gi == 1)
	{
		pPartDocB1 = pInvApp->Documents->MethodAdd(kPartDocumentObject, pInvApp->FileManager->MethodGetTemplateFile(kPartDocumentObject, kMetricSystemOfMeasure, kGOST_DraftingStandard), true);

		pPartDocB1->DisplayName = _T("Bearing 1");

		pPartDocB1->get_ComponentDefinition(&pPartComDef);

		pInvApp->get_TransientGeometry(&pTransGeom);

		pTransManager = pInvApp->GetTransactionManager();

		Doc = CComQIPtr <Document>(pPartDocB1);
	}
	else
	{
		pPartDocB2 = pInvApp->Documents->MethodAdd(kPartDocumentObject, pInvApp->FileManager->MethodGetTemplateFile(kPartDocumentObject, kMetricSystemOfMeasure, kGOST_DraftingStandard), true);

		pPartDocB2->DisplayName = _T("Bearing 2");

		pPartDocB2->get_ComponentDefinition(&pPartComDef);

		pInvApp->get_TransientGeometry(&pTransGeom);

		pTransManager = pInvApp->GetTransactionManager();

		Doc = CComQIPtr <Document>(pPartDocB2);
	}

	pTransManager->raw_StartTransaction(Doc, _T(""), &pTrans);

	pPartComDef->get_Sketches(&sketches);
	pPartComDef->get_Sketches3D(&sketches3D);

	pPartComDef->get_WorkPlanes(&wp);
	pPartComDef->get_Features(&ft);
	pPartComDef->get_WorkAxes(&wax);
	pPartComDef->get_WorkPoints(&wpt);
	// строится стенки подшипника

	PlanarSketch *pSketch3;
	sketches->raw_Add(wp->GetItem(1), false, &pSketch3);

	SketchPointPtr point3[13];
	SketchLinePtr lines3[10];
	SketchArcsPtr Arcs3[2];


	SketchPoints *skPoints3;
	SketchLines *skLines3;
	SketchArcs *skArcs3;
	Profiles *skProfiles3;


	pSketch3->get_SketchPoints(&skPoints3);
	pSketch3->get_SketchLines(&skLines3);
	pSketch3->get_SketchArcs(&skArcs3);
	pSketch3->get_Profiles(&skProfiles3);

	//1 half down
	point3[0] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(-m_Vl2 / 2, m_Vd1 / 2), false);
	point3[1] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(-m_Vl2 / 2, m_Vd1 / 2 + m_Ph), false);

	point3[2] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(m_Vl2 / 3 - m_Vl2 / 2, m_Vd1 / 2 + m_Ph), false);
	point3[3] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(0, (m_Vd1 / 2 + 1.5 * m_Ph)), false);
	point3[4] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d((m_Vl2 / 3) * 2 - m_Vl2 / 2, m_Vd1 / 2 + m_Ph), false);

	Arcs3[0] = skArcs3->MethodAddByCenterStartEndPoint(point3[3], point3[4], point3[2], false);

	point3[5] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(m_Vl2 / 2, m_Vd1 / 2 + m_Ph), false);
	point3[6] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(m_Vl2 / 2, m_Vd1 / 2), false);

	lines3[0] = skLines3->MethodAddByTwoPoints(point3[0], point3[1]);
	lines3[1] = skLines3->MethodAddByTwoPoints(point3[1], point3[2]);
	lines3[2] = skLines3->MethodAddByTwoPoints(point3[4], point3[5]);
	lines3[3] = skLines3->MethodAddByTwoPoints(point3[5], point3[6]);
	lines3[4] = skLines3->MethodAddByTwoPoints(point3[6], point3[0]);

	//2 half up

	point3[7] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(-m_Vl2 / 2, (m_Vd1 / 2 + 2 * m_Ph)), false);
	point3[8] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(-m_Vl2 / 2, m_Vd1 / 2 * 3), false);
	point3[9] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(m_Vl2 / 2, m_Vd1 / 2 * 3), false);
	point3[10] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(m_Vl2 / 2, (m_Vd1 / 2 + 2 * m_Ph)), false);

	point3[11] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d(m_Vl2 / 3 - m_Vl2 / 2, (m_Vd1 / 2 + 2 * m_Ph)), false);

	point3[12] = skPoints3->MethodAdd(pTransGeom->MethodCreatePoint2d((m_Vl2 / 3) * 2 - m_Vl2 / 2, (m_Vd1 / 2 + 2 * m_Ph)), false);

	Arcs3[1] = skArcs3->MethodAddByCenterStartEndPoint(point3[3], point3[11], point3[12], false);

	lines3[5] = skLines3->MethodAddByTwoPoints(point3[7], point3[8]);
	lines3[6] = skLines3->MethodAddByTwoPoints(point3[8], point3[9]);
	lines3[7] = skLines3->MethodAddByTwoPoints(point3[9], point3[10]);
	lines3[8] = skLines3->MethodAddByTwoPoints(point3[10], point3[12]);
	lines3[9] = skLines3->MethodAddByTwoPoints(point3[11], point3[7]);

	Profile *pProfile3;
	skProfiles3->raw__AddForSolid(&pProfile3);

	RevolveFeatures *ftrevolve1;
	ft->get_RevolveFeatures(&ftrevolve1);

	RevolveFeature *Revolve1 = ftrevolve1->MethodAddFull(pProfile3, wax->GetItem(2), kJoinOperation);


	//строятся тела качения


	PlanarSketch *pSketch4;
	sketches->raw_Add(wp->GetItem(1), false, &pSketch4);

	SketchPointPtr point4[3];
	SketchLinePtr lines4[1];
	SketchArcsPtr Arcs4[1];


	SketchPoints *skPoints4;
	SketchLines *skLines4;
	SketchArcs *skArcs4;
	Profiles *skProfiles4;


	pSketch4->get_SketchPoints(&skPoints4);
	pSketch4->get_SketchLines(&skLines4);
	pSketch4->get_SketchArcs(&skArcs4);
	pSketch4->get_Profiles(&skProfiles4);

	point4[0] = skPoints4->MethodAdd(pTransGeom->MethodCreatePoint2d(0, (m_Vd1 / 2 + 1.5 * m_Ph + m_Pr)), false);
	point4[1] = skPoints4->MethodAdd(pTransGeom->MethodCreatePoint2d(0, (m_Vd1 / 2 + 1.5 * m_Ph - m_Pr)), false);
	point4[2] = skPoints4->MethodAdd(pTransGeom->MethodCreatePoint2d(0, (m_Vd1 / 2 + 1.5 * m_Ph)), false);

	Arcs4[0] = skArcs4->MethodAddByCenterStartEndPoint(point4[2], point4[0], point4[1], false);

	lines4[5] = skLines4->MethodAddByTwoPoints(point4[1], point4[0]);


	Profile *pProfile4;
	skProfiles4->raw__AddForSolid(&pProfile4);

	RevolveFeaturesPtr ftrevolve2;
	ft->get_RevolveFeatures(&ftrevolve2);

	RevolveFeaturePtr Revolve2 = ftrevolve2->MethodAddFull(pProfile4, wax->GetItem(3), kJoinOperation);


	ObjectCollection *pCollection;

	pInvApp->TransientObjects->raw_CreateObjectCollection(vtMissing, &pCollection);

	pCollection->MethodAdd(Revolve2);

	CircularPatternFeatures *pCircPatFeat;


	ft->get_CircularPatternFeatures(&pCircPatFeat);


	int n = pCircPatFeat->GetCount();


	n = pCollection->GetCount();

	//массив тел качения
	CircularPatternFeature *circFeat = pCircPatFeat->MethodAdd(pCollection, wax->GetItem(2), true, m_Pnum, (pi * 2 * (m_Pnum - 1)) / m_Pnum, true, kIdenticalCompute);
	pTrans->MethodEnd();

	if (Gi == 1)
		pPartDocB1->MethodSaveAs("D:\\_Details\\Bearing 1.ipt", true);
	else
		pPartDocB2->MethodSaveAs("D:\\_Details\\Bearing 2.ipt", false);
}

void CAssemblyConfigDoc::Shaft(int Gi, double VSpB, double VSpL, double VSpT, double L, double m_Vd3, double m_Vl4, double m_Vd1, double m_Vl2, double m_Vd2, double m_Vl1, double m_Vh, double m_Vl3, double m_Vl, double m_Vr)
{
	if (Gi == 1)
	{
		pPartDocV1 = pInvApp->Documents->MethodAdd(kPartDocumentObject, pInvApp->FileManager->MethodGetTemplateFile(kPartDocumentObject, kMetricSystemOfMeasure, kGOST_DraftingStandard), true);

		pPartDocV1->DisplayName = _T("Shaft 1");

		pPartDocV1->get_ComponentDefinition(&pPartComDef);

		pInvApp->get_TransientGeometry(&pTransGeom);

		pTransManager = pInvApp->GetTransactionManager();

		Doc = CComQIPtr <Document>(pPartDocV1);
	}
	else
	{
		pPartDocV2 = pInvApp->Documents->MethodAdd(kPartDocumentObject, pInvApp->FileManager->MethodGetTemplateFile(kPartDocumentObject, kMetricSystemOfMeasure, kGOST_DraftingStandard), true);

		pPartDocV2->DisplayName = _T("Shaft 2");

		pPartDocV2->get_ComponentDefinition(&pPartComDef);

		pInvApp->get_TransientGeometry(&pTransGeom);

		pTransManager = pInvApp->GetTransactionManager();

		Doc = CComQIPtr <Document>(pPartDocV2);
	}

	pTransManager->raw_StartTransaction(Doc, _T(""), &pTrans);

	pPartComDef->get_Sketches(&sketches);
	pPartComDef->get_Sketches3D(&sketches3D);

	pPartComDef->get_WorkPlanes(&wp);
	pPartComDef->get_Features(&ft);
	pPartComDef->get_WorkAxes(&wax);
	pPartComDef->get_WorkPoints(&wpt);


	//Создание эскиза вращения вала


	PlanarSketch *pSketch;
	sketches->raw_Add(wp->GetItem(3), false, &pSketch);

	SketchPointPtr point[11];
	SketchLinePtr lines[11];


	SketchPoints *skPoints;
	SketchLines *skLines;
	Profiles *skProfiles;


	pSketch->get_SketchPoints(&skPoints);
	pSketch->get_SketchLines(&skLines);
	pSketch->get_Profiles(&skProfiles);

	point[0] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(0, 0), false);
	point[1] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(0, (m_Vd3 / 2) - (0.0721*m_Vd3)), false);
	point[2] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(-(m_Vd3 / 8), m_Vd3 / 2), false);
	point[3] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(-m_Vl4, m_Vd3 / 2), false);
	point[4] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(-m_Vl4, m_Vd1 / 2), false);
	point[5] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(-(m_Vl4 + m_Vl2), m_Vd1 / 2), false);
	point[6] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(-(m_Vl4 + m_Vl2), m_Vd2 / 2), false);
	point[7] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(-(m_Vl1 - m_Vl2), m_Vd2 / 2), false);
	point[8] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(-(m_Vl1 - m_Vl2), m_Vd1 / 2), false);
	point[9] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(-m_Vl1, m_Vd1 / 2), false);
	point[10] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(-m_Vl1, 0), false);


	lines[0] = skLines->MethodAddByTwoPoints(point[0], point[1]);
	lines[1] = skLines->MethodAddByTwoPoints(point[1], point[2]);
	lines[2] = skLines->MethodAddByTwoPoints(point[2], point[3]);
	lines[3] = skLines->MethodAddByTwoPoints(point[3], point[4]);
	lines[4] = skLines->MethodAddByTwoPoints(point[4], point[5]);
	lines[5] = skLines->MethodAddByTwoPoints(point[5], point[6]);
	lines[6] = skLines->MethodAddByTwoPoints(point[6], point[7]);
	lines[7] = skLines->MethodAddByTwoPoints(point[7], point[8]);
	lines[8] = skLines->MethodAddByTwoPoints(point[8], point[9]);
	lines[9] = skLines->MethodAddByTwoPoints(point[9], point[10]);
	lines[10] = skLines->MethodAddByTwoPoints(point[10], point[0]);

	Profile *pProfile;
	skProfiles->raw__AddForSolid(&pProfile);

	RevolveFeatures *ftrevolve;
	ft->get_RevolveFeatures(&ftrevolve);

	RevolveFeature *Revolve = ftrevolve->MethodAddFull(pProfile, wax->GetItem(1), kJoinOperation);
	//отверстие под шпонки
	PlanarSketch *pSketch1;
	sketches->raw_Add(wp->GetItem(3), false, &pSketch1);

	SketchPointPtr point1[8];
	SketchLinePtr lines1[8];


	SketchPoints *skPoints1;
	SketchLines *skLines1;
	Profiles *skProfiles1;


	pSketch1->get_SketchPoints(&skPoints1);
	pSketch1->get_SketchLines(&skLines1);
	pSketch1->get_Profiles(&skProfiles1);

	point1[0] = skPoints1->MethodAdd(pTransGeom->MethodCreatePoint2d((-m_Vl4 / 2 + VSpL / 2), m_Vd3 / 2), false);
	point1[1] = skPoints1->MethodAdd(pTransGeom->MethodCreatePoint2d((-m_Vl4 / 2 + VSpL / 2), m_Vd3 / 2 - VSpT), false);
	point1[2] = skPoints1->MethodAdd(pTransGeom->MethodCreatePoint2d((-m_Vl4 / 2 - VSpL / 2), m_Vd3 / 2 - VSpT), false);
	point1[3] = skPoints1->MethodAdd(pTransGeom->MethodCreatePoint2d((-m_Vl4 / 2 - VSpL / 2), m_Vd3 / 2), false);

	lines1[0] = skLines1->MethodAddByTwoPoints(point1[0], point1[1]);
	lines1[1] = skLines1->MethodAddByTwoPoints(point1[1], point1[2]);
	lines1[2] = skLines1->MethodAddByTwoPoints(point1[2], point1[3]);
	lines1[3] = skLines1->MethodAddByTwoPoints(point1[3], point1[0]);

	point1[4] = skPoints1->MethodAdd(pTransGeom->MethodCreatePoint2d((-m_Vl1 + m_Vl2 / 2 + L + VSpL / 2), m_Vd2 / 2), false);
	point1[5] = skPoints1->MethodAdd(pTransGeom->MethodCreatePoint2d((-m_Vl1 + m_Vl2 / 2 + L + VSpL / 2), m_Vd2 / 2 - VSpT), false);
	point1[6] = skPoints1->MethodAdd(pTransGeom->MethodCreatePoint2d((-m_Vl1 + m_Vl2 / 2 + L - VSpL / 2), m_Vd2 / 2 - VSpT), false);
	point1[7] = skPoints1->MethodAdd(pTransGeom->MethodCreatePoint2d((-m_Vl1 + m_Vl2 / 2 + L - VSpL / 2), m_Vd2 / 2), false);

	lines1[4] = skLines1->MethodAddByTwoPoints(point1[4], point1[5]);
	lines1[5] = skLines1->MethodAddByTwoPoints(point1[5], point1[6]);
	lines1[6] = skLines1->MethodAddByTwoPoints(point1[6], point1[7]);
	lines1[7] = skLines1->MethodAddByTwoPoints(point1[7], point1[4]);

	Profile *pProfile1;
	skProfiles1->raw__AddForSolid(&pProfile1);

	ExtrudeFeatures *ftExtrude1;
	ft->get_ExtrudeFeatures(&ftExtrude1);
	ExtrudeFeaturePtr oExtrudeDef1 = ftExtrude1->MethodAddByDistanceExtent(pProfile1, VSpB, kSymmetricExtentDirection, kCutOperation);



	if (Gi == 1)
	{
		pTrans->MethodEnd();
		pPartDocV1->MethodSaveAs("D:\\_Details\\Shaft 1.ipt", true);
		pPartDocV1->MethodSaveAs("D:\\_Details\\Shaft 1.dwg", true);
	}
	else
	{
		PlanarSketch *pSketcheС;
		sketches->raw_Add(wp->GetItem(3), false, &pSketcheС);

		SketchCirclesPtr CirceС[1];

		SketchCircles *skCirceС;
		Profiles *skProfileseС;

		pSketcheС->get_SketchCircles(&skCirceС);
		pSketcheС->get_Profiles(&skProfileseС);


		CirceС[0] = skCirceС->MethodAddByCenterRadius(pTransGeom->MethodCreatePoint2d(- m_Vl, 0), m_Vr / 2);
		pSketcheС->get_Profiles(&skProfileseС);

		Profile *pProfileС;
		skProfileseС->raw__AddForSolid(&pProfileС);

		ExtrudeFeatures *ftExtrudeС;
		ft->get_ExtrudeFeatures(&ftExtrudeС);

		ExtrudeFeaturePtr oExtrudeDef1 = ftExtrude1->MethodAddByThroughAllExtent(pProfileС, kSymmetricExtentDirection, kCutOperation);

		pTrans->MethodEnd();
		pPartDocV2->MethodSaveAs("D:\\_Details\\Shaft 2.ipt", true);
	}
}

void CAssemblyConfigDoc::Assembly(double aw, double m_V1d2, double m_V2d2, double VSpT1, double VSpT2, double m_dL3, double m_V2l2, double VSpB1, double VSpL)
{
	pAsmDoc = pInvApp->Documents->MethodAdd(kAssemblyDocumentObject, pInvApp->FileManager->MethodGetTemplateFile(kAssemblyDocumentObject, kMetricSystemOfMeasure, kGOST_DraftingStandard), true);
	pAsmDoc->DisplayName = _T("Сборка");
	pAsmDoc->get_ComponentDefinition(&pAsmComDef);
	pInvApp->get_TransientGeometry(&pTransGeomAsm);
	pTransGeomAsm->raw_CreateMatrix(&oPositionMatrix);

	ComponentOccurrencePtr Model1, Model2, Model3, Model4, Model5, Model6, Model7, Model8;
	Model1 = pAsmDoc->ComponentDefinition->Occurrences->MethodAdd("D:\\_Details\\Shaft 1.ipt", oPositionMatrix);
	Model2 = pAsmDoc->ComponentDefinition->Occurrences->MethodAdd("D:\\_Details\\Shaft 2.ipt", oPositionMatrix);
	Model3 = pAsmDoc->ComponentDefinition->Occurrences->MethodAdd("D:\\_Details\\Gear 1.ipt", oPositionMatrix);
	Model4 = pAsmDoc->ComponentDefinition->Occurrences->MethodAdd("D:\\_Details\\Gear 2.ipt", oPositionMatrix);
	Model5 = pAsmDoc->ComponentDefinition->Occurrences->MethodAdd("D:\\_Details\\Bearing 1.ipt", oPositionMatrix);
	Model6 = pAsmDoc->ComponentDefinition->Occurrences->MethodAdd("D:\\_Details\\Bearing 2.ipt", oPositionMatrix);
	Model7 = pAsmDoc->ComponentDefinition->Occurrences->MethodAdd("D:\\_Details\\Bearing 1.ipt", oPositionMatrix);
	Model8 = pAsmDoc->ComponentDefinition->Occurrences->MethodAdd("D:\\_Details\\Bearing 2.ipt", oPositionMatrix);

	SurfaceBody *SurfBody1, *SurfBody2, *SurfBody3, *SurfBody4, *SurfBody5, *SurfBody6, *SurfBody7, *SurfBody8;
	SurfaceBodies *SurfBodies1, *SurfBodies2, *SurfBodies3, *SurfBodies4, *SurfBodies5, *SurfBodies6, *SurfBodies7, *SurfBodies8;

	Model1->get_SurfaceBodies(&SurfBodies1);
	SurfBodies1->get_Item(1, &SurfBody1);

	Model2->get_SurfaceBodies(&SurfBodies2);
	SurfBodies2->get_Item(1, &SurfBody2);

	Model3->get_SurfaceBodies(&SurfBodies3);
	SurfBodies3->get_Item(1, &SurfBody3);

	Model4->get_SurfaceBodies(&SurfBodies4);
	SurfBodies4->get_Item(1, &SurfBody4);

	Model5->get_SurfaceBodies(&SurfBodies5);
	SurfBodies5->get_Item(1, &SurfBody5);

	Model6->get_SurfaceBodies(&SurfBodies6);
	SurfBodies6->get_Item(1, &SurfBody6);

	Model7->get_SurfaceBodies(&SurfBodies7);
	SurfBodies7->get_Item(1, &SurfBody7);

	Model8->get_SurfaceBodies(&SurfBodies8);
	SurfBodies8->get_Item(1, &SurfBody8);

	Edges *oEdges1, *oEdges2, *oEdges3, *oEdges4, *oEdges5, *oEdges6, *oEdges7, *oEdges8;
	EdgePtr oEdge1, oEdge2, oEdge3, oEdge4, oEdge5, oEdge6, oEdge7, oEdge8;

	SurfBody1->get_Edges(&oEdges1);
	oEdges1->get_Item(29, &oEdge1);

	SurfBody1->get_Edges(&oEdges2);
	oEdges2->get_Item(26, &oEdge2);

	SurfBody2->get_Edges(&oEdges3);
	oEdges3->get_Item(30, &oEdge3);

	SurfBody2->get_Edges(&oEdges4);
	oEdges4->get_Item(29, &oEdge4);

	SurfBody5->get_Edges(&oEdges5);
	oEdges5->get_Item(5, &oEdge5);

	SurfBody6->get_Edges(&oEdges6);
	oEdges6->get_Item(5, &oEdge6);

	SurfBody7->get_Edges(&oEdges7);
	oEdges7->get_Item(5, &oEdge7);

	SurfBody8->get_Edges(&oEdges8);
	oEdges8->get_Item(5, &oEdge8);

	Faces *oFaces1, *oFaces2, *oFaces3, *oFaces4, *oFaces5, *oFaces6, *oFaces7, *oFaces8, *oFaces9, *oFaces10, *oFaces11, *oFaces12;
	FacePtr oFace1, oFace2, oFace3, oFace4, oFace5, oFace6, oFace7, oFace8, oFace9, oFace10, oFace11, oFace12;

	SurfBody1->get_Faces(&oFaces1);
	oFaces1->get_Item(9, &oFace1);

	SurfBody1->get_Faces(&oFaces3);
	oFaces3->get_Item(6, &oFace3);

	SurfBody1->get_Faces(&oFaces5);
	oFaces5->get_Item(7, &oFace5);

	SurfBody2->get_Faces(&oFaces4);
	oFaces4->get_Item(7, &oFace4);

	SurfBody2->get_Faces(&oFaces2);
	oFaces2->get_Item(10, &oFace2);

	SurfBody2->get_Faces(&oFaces6);
	oFaces6->get_Item(9, &oFace6);

	SurfBody3->get_Faces(&oFaces7);
	oFaces7->get_Item(5, &oFace7);

	SurfBody3->get_Faces(&oFaces8);
	oFaces8->get_Item(2, &oFace8);

	SurfBody3->get_Faces(&oFaces9);
	oFaces9->get_Item(3, &oFace9);

	SurfBody4->get_Faces(&oFaces10);
	oFaces10->get_Item(5, &oFace10);

	SurfBody4->get_Faces(&oFaces11);
	oFaces11->get_Item(4, &oFace11);

	SurfBody4->get_Faces(&oFaces12);
	oFaces12->get_Item(3, &oFace12);



	MateConstraintPtr sPlane1, sPlane2, sPlane3, sPlane4, sPlane5, sPlane6, sPlane7, sPlane8, sPlane9, sPlane10, sPlane11, sPlane12, sPlane13;

	//валы
	sPlane1 = pAsmComDef->Constraints->MethodAddMateConstraint(oFace2, oFace1, (aw - m_V1d2 / 2 - m_V2d2 / 2 + VSpT1 + VSpT2), kNoInference, kNoInference);
	sPlane2 = pAsmComDef->Constraints->MethodAddMateConstraint(oFace3, oFace4, m_dL3 - m_V2l2 / 2, kNoInference, kNoInference);
	sPlane3 = pAsmComDef->Constraints->MethodAddMateConstraint(oFace5, oFace6, 0, kNoInference, kNoInference);
	//подшипники
	sPlane4 = pAsmComDef->Constraints->MethodAddInsertConstraint(oEdge1, oEdge5, true, 0);
	sPlane5 = pAsmComDef->Constraints->MethodAddInsertConstraint(oEdge3, oEdge6, true, 0);
	sPlane6 = pAsmComDef->Constraints->MethodAddInsertConstraint(oEdge2, oEdge7, true, 0);
	sPlane7 = pAsmComDef->Constraints->MethodAddInsertConstraint(oEdge4, oEdge8, true, 0);

	//шестерни

	sPlane8 = pAsmComDef->Constraints->MethodAddMateConstraint(oFace1, oFace7, 3 * VSpT1 / 2, kNoInference, kNoInference);
	sPlane9 = pAsmComDef->Constraints->MethodAddMateConstraint(oFace5, oFace8, VSpB1, kNoInference, kNoInference);
	sPlane10 = pAsmComDef->Constraints->MethodAddMateConstraint(oFace3, oFace9, VSpL, kNoInference, kNoInference);

	sPlane11 = pAsmComDef->Constraints->MethodAddMateConstraint(oFace2, oFace10, 3 * VSpT1 / 2, kNoInference, kNoInference);
	sPlane12 = pAsmComDef->Constraints->MethodAddMateConstraint(oFace6, oFace11, VSpB1, kNoInference, kNoInference);
	sPlane13 = pAsmComDef->Constraints->MethodAddMateConstraint(oFace4, oFace12, VSpL, kNoInference, kNoInference);
}


void CAssemblyConfigDoc::OpenInventor()
{

	BeginWaitCursor();


	// Get hold of the program id of Inventor.
	CLSID InvAppClsid;
	HRESULT hRes = CLSIDFromProgID(L"Inventor.Application", &InvAppClsid);
	if (FAILED(hRes)) {
		pInvApp = nullptr;
		return;// ReturnAndShowCOMError(hRes, L"ConnectToInventor,CLSIDFromProgID failed");
	}

	// See if Inventor is already running...
	CComPtr<IUnknown> pInvAppUnk = nullptr;
	hRes = ::GetActiveObject(InvAppClsid, NULL, &pInvAppUnk);
	if (FAILED(hRes)) {
		// Inventor is not already running, so try to start it...
		TRACE(L"Could not get hold of an active Inventor, will start a new session\n");
		hRes = CoCreateInstance(InvAppClsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IUnknown), (void **)&pInvAppUnk);
		if (FAILED(hRes)) {
			pInvApp = nullptr;
			return;// ReturnAndShowCOMError(hRes, L"ConnectToInventor,CoCreateInstance failed");
		}
	}

	// Get the pointer to the Inventor application...
	hRes = pInvAppUnk->QueryInterface(__uuidof(Application), (void **)&pInvApp);
	if (FAILED(hRes)) {
		return;// ReturnAndShowCOMError(hRes, L"ConnectToInventor,QueryInterface failed");
	}

	pInvApp->put_Visible(TRUE);

}

void CAssemblyConfigDoc::FunDiv10()
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

void CAssemblyConfigDoc::CheckupPar()
{
	//вал 2

	if (m_V2d1 < m_V2d3)
	{
	//изменить диаметр вала 2
	}
	if (m_V2d1 > m_V2d2)
	{
	//изменить диаметр вала 2
	}

	if (m_V2l1 != (m_V2l2*2+m_V2l3+m_V2l4))
	{
	//изменить длины вала 2 они не соответствуют
	}
	if (m_V2l4 < m_V2l2)
	{
	//Неверно указанная длина 4 у вала 2
	}
	m_Vr = 4;

	m_Vl = 28;

	if (m_V1d3 < m_Vr)
	{
	//изменить диаметр отвертия вала 2
	}
	if (m_V2l3/2 < m_Vl)
	{
	//изменить длину для отвертия вала 2
	}
	

	
	//зубчатое зацепление
	if (m_V1d2 > df)
	{
	//изменить модуль или количество зубьев или диаметр вала 1
	}

	if (m_V2d2 > df)
	{
	//изменить модуль или передаточное число или диаметр вала 2
	}

	
	//Сборка
	if ((m_V2l3 - m_V2l2) < m_dShS)
	{
	//изменить ширину шестерни
	}
	if ((m_V1l3 - m_V1l2) < m_dShS)
	{
	//изменить ширину шестерни
	}
	if ((m_dL1 + m_dL2) != m_V1l3)
	{
	//изменить длины вала 1 не соответствуют
	}
	if ((m_dL3 + m_dL4) != m_V2l3)
	{
	//изменить длины вала 1 не соответствуют
	}

}

void CAssemblyConfigDoc::ParamsMaker()
{
	d = m_dShM*m_dShZ1;//нач диаметр
	p = pi*m_dShM / 2;//толшина зуба
	da = (d + 2 * m_dShM);//диаметр выступов
	df = (d - 2.5*m_dShM);//диаметр впадин
	a = m_dShM / 2;

	//расчёт зуба
	yd = sqrt(d*d / 4 - p*p / 4);
	ya = sqrt(da*da / 4 - p*p / 16);
	xf = 9 * p / 14;
	yf = sqrt(df*df / 4 - xf*xf);
	//расчёт параметров колеса
	Do = da - 8.5*m_dShM;
	Dc = 1.5*m_V1d2 + 1;


	d2 = m_dShM*m_dShZ2;
	da2 = (d2 + 2 * m_dShM);
	df2 = (d2 - 2.5*m_dShM);
	yd2 = sqrt(d2*d2 / 4 - p*p / 4);
	ya2 = sqrt(da2*da2 / 4 - p*p / 16);
	yf2 = sqrt(df2*df2 / 4 - xf*xf);
	Do2 = da2 - 8.5*m_dShM;


	m_P1h = ((m_V1d1 / 2) * 3 - m_V1d1 / 2) / 3;
	m_P1r = sqrt(m_V1l2 / 6 * m_V1l2 / 6 + m_P1h / 2 * m_P1h / 2) - 0.01;
	m_P1num = 5;
	while ((2 * m_P1r + 0.4) < ((2 * m_V1d1 / 2 + 1.5 * m_P1h) * sin(pi / m_P1num))) {
		m_P1num++;
	}

	m_P2h = ((m_V2d1 / 2) * 3 - m_V2d1 / 2) / 3;
	m_P2r = sqrt(m_V2l2 / 6 * m_V2l2 / 6 + m_P2h / 2 * m_P2h / 2) - 0.01;
	m_P2num = 5;
	while ((2 * m_P2r + 0.4) < ((2 * m_V2d1 / 2 + 1.5 * m_P2h) * sin(pi / m_P2num))) {
		m_P2num++;
	}

	VSpT1 = m_V1d3 / 8;
	VSpT2 = m_V2d3 / 8;
	VSpL = m_dShS - 10;
	VSpB1 = m_V1d3 / 4;
	VSpB2 = m_V2d3 / 4;

	aw = (m_dShZ1 + m_dShZ2)*m_dShM / 2;
}




