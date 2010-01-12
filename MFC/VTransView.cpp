// VTransView.cpp : Implementierung der Klasse CVTransView
//

#include "../stdafx.h"
#include "VTrans.h"

#include "VTransDoc.h"
#include "VTransView.h"

// eigener Code, Anfang
#include "ChildFrm.h"
#include "EnglishView.h"
#include "MainFrm.h"
// eigener Code, Ende

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVTransView

IMPLEMENT_DYNCREATE(CVTransView, CView)

BEGIN_MESSAGE_MAP(CVTransView, CView)
	//{{AFX_MSG_MAP(CVTransView)
	//}}AFX_MSG_MAP
	// Standard-Druckbefehle
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVTransView Konstruktion/Destruktion

CVTransView::CVTransView()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen,

	TRACE("CVTransView::CVTransView()\n");
	CVTransDoc* pDoc = GetDocument();
	CMainFrame * pFrame=(CMainFrame *) AfxGetApp()->m_pMainWnd;
	if(pFrame)
	{
		CChildFrame *pChild=(CChildFrame *)pFrame->GetActiveWindow();
		if(pChild)
		{
			TRACE("pChild\n");
			EnglishView * pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(0,0);
			if(pEV)
				pEV->m_pText=&pDoc->m_EnglishText;
			pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
			if(pEV)
				pEV->m_pText=&pDoc->m_GermanText;
		}
	}
	TRACE("CVTransView::CVTransView() - ENDE\n");
}

CVTransView::~CVTransView()
{
}

BOOL CVTransView::PreCreateWindow(CREATESTRUCT& cs)
{
	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	TRACE("BOOL CVTransView::PreCreateWindow(CREATESTRUCT& cs)\n");
	CVTransDoc* pDoc = GetDocument();
	CMainFrame * pFrame=(CMainFrame *) AfxGetApp()->m_pMainWnd;
	if(pFrame)
	{
		CChildFrame *pChild=(CChildFrame *)pFrame->GetActiveWindow();
		if(pChild)
		{
			TRACE("pChild\n");
			EnglishView * pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(0,0);
			if(pEV)
				pEV->m_pText=&pDoc->m_EnglishText;
			pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
			if(pEV)
				pEV->m_pText=&pDoc->m_GermanText;
		}
	}
	TRACE("BOOL CVTransView::PreCreateWindow(CREATESTRUCT& cs) - ENDE\n");
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVTransView Zeichnen

void CVTransView::OnDraw(CDC* pDC)
{
	CVTransDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// ZU ERLEDIGEN: Hier Code zum Zeichnen der ursprünglichen Daten hinzufügen
	TRACE("BOOL CVTransView::PreCreateWindow(CREATESTRUCT& cs)\n");
	CMainFrame * pFrame=(CMainFrame *) AfxGetApp()->m_pMainWnd;
	if(pFrame)
	{
		CChildFrame *pChild=(CChildFrame *)pFrame->GetActiveWindow();
		if(pChild)
		{
			TRACE("pChild\n");
			EnglishView * pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(0,0);
			if(pEV)
				pEV->m_pText=&pDoc->m_EnglishText;
			pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
			if(pEV)
				pEV->m_pText=&pDoc->m_GermanText;
		}
	}
	TRACE("BOOL CVTransView::PreCreateWindow(CREATESTRUCT& cs) - ENDE\n");
}

/////////////////////////////////////////////////////////////////////////////
// CVTransView Drucken

BOOL CVTransView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Standardvorbereitung
	return DoPreparePrinting(pInfo);
}

void CVTransView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Zusätzliche Initialisierung vor dem Drucken hier einfügen
}

void CVTransView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Hier Bereinigungsarbeiten nach dem Drucken einfügen
}

/////////////////////////////////////////////////////////////////////////////
// CVTransView Diagnose

#ifdef _DEBUG
void CVTransView::AssertValid() const
{
	CView::AssertValid();
}

void CVTransView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVTransDoc* CVTransView::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVTransDoc)));
	return (CVTransDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVTransView Nachrichten-Handler


void CVTransView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
