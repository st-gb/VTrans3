// VTransView.h : Schnittstelle der Klasse CVTransView
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VTRANSVIEW_H__FB6F08EC_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
#define AFX_VTRANSVIEW_H__FB6F08EC_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CVTransView : public CView
{
protected: // Nur aus Serialisierung erzeugen
	CVTransView();
	DECLARE_DYNCREATE(CVTransView)

// Attribute
public:
	CVTransDoc* GetDocument();

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CVTransView)
	public:
	virtual void OnDraw(CDC* pDC);  // überladen zum Zeichnen dieser Ansicht
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CVTransView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CVTransView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Testversion in VTransView.cpp
inline CVTransDoc* CVTransView::GetDocument()
   { return (CVTransDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_VTRANSVIEW_H__FB6F08EC_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
