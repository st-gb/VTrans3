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
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen,
	BOOL bExtendedText=FALSE;
}

GermanView::~GermanView()
{
}

BOOL GermanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVTransView Zeichnen

void GermanView::OnDraw(CDC* pDC)
{
//	CVTransDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	// ZU ERLEDIGEN: Hier Code zum Zeichnen der ursprünglichen Daten hinzufügen

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
	// ZU ERLEDIGEN: Zusätzliche Initialisierung vor dem Drucken hier einfügen
}

void GermanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Hier Bereinigungsarbeiten nach dem Drucken einfügen
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

/*CVTransDoc* CVTransView::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVTransDoc)));
	return (CVTransDoc*)m_pDocument;
}*/
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVTransView Nachrichten-Handler
