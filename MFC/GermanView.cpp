#include "../stdafx.h"
#include "GermanView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*int GermanView::Func()// public CView
{
	return 1;
}*/

/////////////////////////////////////////////////////////////////////////////
// CVTransView Konstruktion/Destruktion

GermanView::GermanView()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einf�gen,
	BOOL bExtendedText=FALSE;
}

GermanView::~GermanView()
{
}

BOOL GermanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// ZU ERLEDIGEN: �ndern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVTransView Zeichnen

void GermanView::OnDraw(CDC* pDC)
{
//	CVTransDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	// ZU ERLEDIGEN: Hier Code zum Zeichnen der urspr�nglichen Daten hinzuf�gen

	//for();
}

/////////////////////////////////////////////////////////////////////////////
// CVTransView Drucken

BOOL GermanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Standardvorbereitung
	return DoPreparePrinting(pInfo);
}

void GermanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Zus�tzliche Initialisierung vor dem Drucken hier einf�gen
}

void GermanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Hier Bereinigungsarbeiten nach dem Drucken einf�gen
}

/////////////////////////////////////////////////////////////////////////////
// CVTransView Diagnose

#ifdef _DEBUG
void GermanView::AssertValid() const
{
	CView::AssertValid();
}

void GermanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

/*CVTransDoc* CVTransView::GetDocument() // Die endg�ltige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVTransDoc)));
	return (CVTransDoc*)m_pDocument;
}*/
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVTransView Nachrichten-Handler
