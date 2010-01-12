// ChildFrm.h : Schnittstelle der Klasse CChildFrame
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__FB6F08E8_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
#define AFX_CHILDFRM_H__FB6F08E8_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define STEFAN_DEBUG

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attribute
public:
	CSplitterWnd m_SplitterWnd;

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementierung
public:
	int m_iHeight;
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_CHILDFRM_H__FB6F08E8_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
