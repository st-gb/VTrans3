#if !defined(__SEARCHFORMBAR_H__)
     #define __SEARCHFORMBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// mybar.h : header file
//

#include "sizecbar.h"
#include "WordEditorBar.h"
#include "../Word.hpp"
#include "VTrans.h"
#include "../VocabularyInMainMem/WordSearch.hpp"
//#include "MainFrm.h"
//#include "MainFrm.h"
//extern VocNode * m_first;
/////////////////////////////////////////////////////////////////////////////
// CMyBar window

#ifndef ID_USER
  //der Wert wird empfohlen in MSDN Library Visual Studio 6.0,
	//Artikel: "TN022: Standard Commands Implementation"
	//Überschrift: "ID Ranges"
	//"Your application should define its commands in the range 0x8000 to 0xDFFF."
	//weil bei einem Wert von 0x8000 unter Windows XP einige statische Texte
	//(CStatic) deaktiviert, also  grau waren und sich die IDs nicht mit denen von
	//WordEditorBar und UnknownWordsBar überschneiden sollten, habe ich 0x9200 
	//gewählt
	#define ID_USER 0x9200
#endif

#ifndef baseCMyBar
#define baseCMyBar CSizingControlBar
#endif

class CSearchFormBar : public baseCMyBar//, public CFrameWnd
{
	DECLARE_DYNAMIC(CSearchFormBar)
// Construction
public:
  CSearchFormBar(WordSearchType * p_wordsearch);
  //CSearchFormBar(CMyBar &);

// Attributes
public:
	//CMainFrame * m_pMainFrame;
  //I_WordSearch * m_wordsearch ;
  WordSearchType * mp_wordsearch ;
  void Attach(CVocableEditorBar &);

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSearchFormBar)
    //}}AFX_VIRTUAL

// Implementation
public:
	//CMainFrame * m_pMainFrame;
	//void OnSelChangeEnglishWordkind();
	virtual ~CSearchFormBar();
		//void OnIDUser101();

protected:
		CButton m_englishCheck[10];
    CButton m_reverseButton;
		CButton m_forwardButton;
		CComboBox m_englishWordKind;
		CEdit m_searchEdit;
		CEdit m_germanEdit[14];
    CFont   m_font;
		CVocableEditorBar * m_pVocableEditorBar;
		CStatic m_english;
		CStatic m_wordkind;
		CStatic m_englishStatic[10];

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyBar)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnChangeWordEdit();
		afx_msg void OnIDUser101();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__MYBAR_H__)
