
// AssemblyConfigView.cpp : реализация класса CAssemblyConfigView
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "AssemblyConfig.h"
#endif

#include "AssemblyConfigDoc.h"
#include "AssemblyConfigView.h"
#include "AssemblyTreeView.h"
//#include "MainFrm.h"
/*
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
InventorNative::PartDocumentPtr pPartDoc;
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

*/
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAssemblyConfigView

IMPLEMENT_DYNCREATE(CAssemblyConfigView, CView)

BEGIN_MESSAGE_MAP(CAssemblyConfigView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_COMMAND(ID_LOAD, &CAssemblyConfigView::OnLoad)
	ON_COMMAND(ID_HIDE, &CAssemblyConfigView::OnHide)
END_MESSAGE_MAP()

// создание/уничтожение CAssemblyConfigView

CAssemblyConfigView::CAssemblyConfigView()
{
	// TODO: добавьте код создания

}

CAssemblyConfigView::~CAssemblyConfigView()
{
}

BOOL CAssemblyConfigView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// рисование CAssemblyConfigView

void CAssemblyConfigView::OnDraw(CDC* pDC)
{
	CAssemblyConfigDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	
	// TODO: добавьте здесь код отрисовки для собственных данных
	CRect rect;
	GetClientRect(&rect);

	m_ctrlModelView.SetWindowPos(this, 0, 0, rect.Width(), rect.Height(), NULL);//растягиваем окошко

	m_ctrlModelView.RedrawWindow();//перерисовка
	
	/*
	CFolderPickerDialog Dialog1;
	if (Dialog1.DoModal() == IDOK) {
		//m_Direct = Dialog1.GetPathName();
	}

	*/
	//m_ctrlModelView.PutSourcePath(L"D:\\_Details\\Shaft 1.dwg");
	//m_ctrlModelView.Zoom_Extents();
	//m_ctrlModelView.VisualStyle_Realistic();
	
				/*				   // TODO: добавьте свой код обработчика уведомлений
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

	pPartDoc = pInvApp->Documents->MethodAdd(kPartDocumentObject, pInvApp->FileManager->MethodGetTemplateFile(kPartDocumentObject, kMetricSystemOfMeasure, kGOST_DraftingStandard), true);

	pPartDoc->DisplayName = _T("Деталь 1");

	pPartDoc->get_ComponentDefinition(&pPartComDef);

	pInvApp->get_TransientGeometry(&pTransGeom);

	pTransManager = pInvApp->GetTransactionManager();

	Doc = CComQIPtr <Document>(pPartDoc);

	pTransManager->raw_StartTransaction(Doc, _T(""), &pTrans);

	pPartComDef->get_Sketches(&sketches);
	pPartComDef->get_Sketches3D(&sketches3D);

	pPartComDef->get_WorkPlanes(&wp);
	pPartComDef->get_Features(&ft);
	pPartComDef->get_WorkAxes(&wax);
	pPartComDef->get_WorkPoints(&wpt);

	PlanarSketch *pSketch;
	sketches->raw_Add(wp->GetItem(3), false, &pSketch);

	SketchPointPtr point[4];
	SketchLinePtr lines[4];


	SketchPoints *skPoints;
	SketchLines *skLines;
	Profiles *skProfiles;


	pSketch->get_SketchPoints(&skPoints);
	pSketch->get_SketchLines(&skLines);
	pSketch->get_Profiles(&skProfiles);*/
	
	/*point[0] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(5, 5), false);
	point[1] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(-5, 5), false);
	point[2] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(-5, -5), false);
	point[3] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(5, -5), false);

	lines[0] = skLines->MethodAddByTwoPoints(point[0], point[1]);
	lines[1] = skLines->MethodAddByTwoPoints(point[1], point[2]);
	lines[2] = skLines->MethodAddByTwoPoints(point[2], point[3]);
	lines[3] = skLines->MethodAddByTwoPoints(point[3], point[0]);*/
	
	/*

	SketchCirclePtr circlae1[1];


	SketchCircles *skCircles1;


	pSketch->get_SketchCircles(&skCircles1);

	pSketch->get_Profiles(&skProfiles);

	circlae1[0] = skCircles1->MethodAddByCenterRadius(pTransGeom->MethodCreatePoint2d(80, 50), 20);

	Profile *pProfile;
	skProfiles->raw__AddForSolid(&pProfile);

	ExtrudeFeatures *ftExtrude;
	ft->get_ExtrudeFeatures(&ftExtrude);

	ExtrudeFeaturePtr oExtrudeDef = ftExtrude->MethodAddByDistanceExtent(pProfile, 20, kPositiveExtentDirection, kJoinOperation);

	pTrans->MethodEnd();

	pPartDoc->MethodSaveAs("D:\\projects\\inventor\\модель1.ipt", true);


	pPartDoc1 = pInvApp->Documents->MethodAdd(kPartDocumentObject, pInvApp->FileManager->MethodGetTemplateFile(kPartDocumentObject, kMetricSystemOfMeasure, kGOST_DraftingStandard), true);
	pPartDoc1->DisplayName = _T("Деталь 2");
	pPartDoc1->get_ComponentDefinition(&pPartComDef1);
	pInvApp->get_TransientGeometry(&pTransGeom1);
	pTransManager = pInvApp->GetTransactionManager();
	Doc = CComQIPtr <Document>(pPartDoc1);
	pTransManager->raw_StartTransaction(Doc, _T(""), &pTrans1);
	pPartComDef1->get_Sketches(&sketches);
	pPartComDef1->get_Sketches3D(&sketches3D);
	pPartComDef1->get_WorkPlanes(&wp);
	pPartComDef1->get_Features(&ft);
	pPartComDef1->get_WorkAxes(&wax);
	pPartComDef1->get_WorkPoints(&wpt);
	PlanarSketch *pSketch1;
	sketches->raw_Add(wp->GetItem(3), false, &pSketch1);

	SketchCirclePtr circlae[1];


	SketchCircles *skCircles;
	Profiles *skProfiles1;


	pSketch1->get_SketchCircles(&skCircles);

	pSketch1->get_Profiles(&skProfiles1);

	circlae[0] = skCircles->MethodAddByCenterRadius(pTransGeom1->MethodCreatePoint2d(0, 5), 20);

	Profile *pProfile1;
	skProfiles1->raw__AddForSolid(&pProfile1);

	ExtrudeFeatures *ftExtrude1;
	ft->get_ExtrudeFeatures(&ftExtrude1);

	ExtrudeFeaturePtr oExtrudeDef1 = ftExtrude1->MethodAddByDistanceExtent(pProfile1, 120, kPositiveExtentDirection, kJoinOperation);

	pTrans1->MethodEnd();

	pPartDoc1->MethodSaveAs("D:\\projects\\inventor\\модель2.ipt", false);

	pAsmDoc = pInvApp->Documents->MethodAdd(kAssemblyDocumentObject, pInvApp->FileManager->MethodGetTemplateFile(kAssemblyDocumentObject, kMetricSystemOfMeasure, kGOST_DraftingStandard), true);
	pAsmDoc->DisplayName = _T("Сборка 1");
	pAsmDoc->get_ComponentDefinition(&pAsmComDef);
	pInvApp->get_TransientGeometry(&pTransGeomAsm);
	pTransGeomAsm->raw_CreateMatrix(&oPositionMatrix);

	ComponentOccurrencePtr Model1, Model2;
	Model1 = pAsmDoc->ComponentDefinition->Occurrences->MethodAdd("D:\\projects\\inventor\\модель1.ipt", oPositionMatrix);
	Model2 = pAsmDoc->ComponentDefinition->Occurrences->MethodAdd("D:\\projects\\inventor\\модель2.ipt", oPositionMatrix);




	//зависимость вставка

	SurfaceBody *SurfBody1, *SurfBody2;
	SurfaceBodies *SurfBodies1, *SurfBodies2;

	Model1->get_SurfaceBodies(&SurfBodies1);
	SurfBodies1->get_Item(1, &SurfBody1);

	Model2->get_SurfaceBodies(&SurfBodies2);
	SurfBodies2->get_Item(1, &SurfBody2);

	Edges *oEdges1, *oEdges2;
	EdgePtr oEdge1, oEdge2;


	SurfBody1->get_Edges(&oEdges1);
	oEdges1->get_Item(1, &oEdge1);

	SurfBody2->get_Edges(&oEdges2);
	oEdges2->get_Item(1, &oEdge2);

	MateConstraintPtr sPlane1;

	sPlane1 = pAsmComDef->Constraints->MethodAddInsertConstraint(oEdge1, oEdge2, true, 0);//совмещение (встречно, с выравниванием), смещение

	*/
	
}


// печать CAssemblyConfigView

BOOL CAssemblyConfigView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CAssemblyConfigView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CAssemblyConfigView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// диагностика CAssemblyConfigView

#ifdef _DEBUG
void CAssemblyConfigView::AssertValid() const
{
	CView::AssertValid();
}

void CAssemblyConfigView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAssemblyConfigDoc* CAssemblyConfigView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAssemblyConfigDoc)));
	return (CAssemblyConfigDoc*)m_pDocument;
}
#endif //_DEBUG


// обработчики сообщений CAssemblyConfigView


int CAssemblyConfigView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Добавьте специализированный код создания

	m_ctrlModelView.Create(NULL, WS_VISIBLE, CRect(0, 0, 1000, 800) , this, 0);

	return 0;
}





void CAssemblyConfigView::OnLoad()
{
	// TODO: добавьте свой код обработчика команд
	//long res = 1000;
	//m_ctrlModelView.PutSourcePath(L"D:\\_Details\\Shaft 1.dwg");
}

bool bShow = 0;
void CAssemblyConfigView::OnHide()
{
	// TODO: добавьте свой код обработчика команд
	m_ctrlModelView.ShowWindow(bShow);
	bShow = !bShow;

}

void CAssemblyConfigView::Tree(int i)
{
	CString Buf;
	Buf = L"D:\\_Details\\Shaft 1.dwg";
	m_ctrlModelView.PutSourcePath(Buf);
	m_ctrlModelView.PutSourcePath(L"D:\\_Details\\Shaft 1.dwg");
	//switch (i) {
	//case 2:m_ctrlModelView.PutSourcePath(Buf); break;
	
	//}
}
