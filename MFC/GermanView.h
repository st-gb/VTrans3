#if !defined(GERMANVIEW_H__FB6F08EG_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
#define GERMANVIEW_H__FB6F08EG_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector> // für m_Text;
#include "../Translate.h" // für m_Text;

class GermanView : public CView
{
	std::vector<CString> m_Text;
	//CObjectVector m_ExtendedText;
	BOOL bExtendedText;
	//int Func();
//protected: // Nur aus Serialisierung erzeugen
	//GermanView();
//	DECLARE_DYNCREATE(CVTransView)

// Attribute

// Operationen
public:

	GermanView();
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
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~GermanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif // !defined(GERMANVIEW_H__FB6F08EC_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)