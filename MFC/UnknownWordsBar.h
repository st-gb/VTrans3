#if !defined(UNKNOWNWORDSBAR_H__FB6F08E2_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
     #define UNKNOWNWORDSBAR_H__FB6F08E2_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// mybar.h : header file
//

#include "sizecbar.h"

/////////////////////////////////////////////////////////////////////////////
// CMyBar window

#ifndef ID_USER
  //der Wert wird empfohlen in MSDN Library Visual Studio 6.0,
	//Artikel: "TN022: Standard Commands Implementation"
	//Überschrift: "ID Ranges"
	//"Your application should define its commands in the range 0x8000 to 0xDFFF."
	//weil bei einem Wert von 0x8000 unter Windows XP einige statische Texte
	//(CStatic) deaktiviert, also  grau waren und sich die IDs nicht mit denen von
	//WordEditorBar überschneiden sollten, habe ich 0x9100 gewählt
	#define ID_USER 0x9100
#endif

#ifndef baseCMyBar
#define baseCMyBar CSizingControlBar
#endif

class CUnknownWordsBar : public baseCMyBar//, public CFrameWnd
{
	DECLARE_DYNAMIC(CUnknownWordsBar)
// Construction
public:
    CUnknownWordsBar();
    //CSearchFormBar(CMyBar &);


// Attributes
public:
	//CMainFrame * m_pMainFrame;
	CEdit m_editText;
// Dialogfelddaten
	//{{AFX_DATA(CUnknownWordsBar)
	CString	m_strText;
	//}}AFX_DATA

// Overridables
//    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Operations
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CUnknownWordsBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSearchFormBar)
    //}}AFX_VIRTUAL

// Implementation
public:
	//CMainFrame * m_pMainFrame;
	//void OnSelChangeEnglishWordkind();
	virtual ~CUnknownWordsBar();
		//void OnIDUser101();

protected:

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyBar)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(UNKNOWNWORDSBAR_H__FB6F08E2_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
