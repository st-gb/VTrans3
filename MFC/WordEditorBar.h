#if !defined(__VOCABLEEDITORBAR_H__)
#define __VOCABLEEDITORBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// mybar.h : header file
//

#include "sizecbar.h"
#include "../Word.hpp"
/////////////////////////////////////////////////////////////////////////////
// CMyBar window

#ifndef MINIMAL
	#define MINIMAL 0
#endif


#ifndef baseCMyBar
#define baseCMyBar CSizingControlBar
#endif

#define WORDKIND_INDEX_FOR_NOUN 0
#define WORDKIND_INDEX_FOR_VERB 1
#define WORDKIND_INDEX_FOR_ADJECTIVE 2
#define WORDKIND_INDEX_FOR_ADVERB 3
#define WORDKIND_INDEX_FOR_PREPOSITION 4
#define WORDKIND_INDEX_FOR_PRONOUN 5
#define WORDKIND_INDEX_FOR_CONJUNCTION 6

//Many controls are shared among wordkinds. So they have the same ID:
#define ENGLISH_SINGULAR_EDIT ID_USER+12
#define ENGLISH_INFINITIVE_EDIT ID_USER+12
#define ENGLISH_POSITIVE_EDIT ID_USER+12
#define ENGLISH_ADVERB_EDIT ID_USER+12
#define ENGLISH_PREPOSITION_EDIT ID_USER+12
#define ENGLISH_PRONOUN_EDIT ID_USER+12
#define ENGLISH_CONJUNCTION_EDIT ID_USER+12

#define ENGLISH_PLURAL_EDIT ID_USER+13

class CVocableEditorBar : public CSizingControlBar
{
// Construction
public:
    CVocableEditorBar();

// Attributes
public:

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Operations
public:
	void AdjectiveControls();
	void AdverbControls();
	void AuxiliaryVerbControls();
	void ConjunctionControls();
  void EnglishAdjectiveToGUI(Word & cr_wordEnglish) ;
	Word * GUIToEnglishWord();
	Word * GUIToGermanWord();
	void NounControls();
	void PrepositionControls();
	void PronounControls();
	void VerbControls(BYTE mode);
	void WordToGUI(//WordNode *
    //const is not possible because of "dynamic_cast<...>(...)"
    //const 
    Word & cr_wordEnglish
    , 
    //const is not possible because of "dynamic_cast<...>(...)"
    //const 
    Word & cr_wordGerman
    );

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyBar)
    //}}AFX_VIRTUAL

// Implementation
public:
	BYTE m_bSetFocusOnEnglishWordKind;
	void OnChangeIDUser10();
	void OnChangeIDUser20();
	void OnChangeIDUser40();
	void OnChangeIDUser49();
	void OnIDUser2();
	void OnIDUser4();
	void OnIDUser5();
	void OnIDUser8();
	void OnIDUser9();
	void OnIDUser16();
	void OnIDUser17();
	void OnIDUser18();
	void OnIDUser22(); // Kontrollkästchen "~"
	void OnIDUser23(); // Kontrollkästchen "~e"
	void OnIDUser24(); // Kontrollkästchen "~en/n"
	void OnIDUser25(); // Kontrollkästchen "kein Fall"
	void OnIDUser26(); // Kontrollkästchen "3. Fall"
	void OnIDUser27(); // Kontrollkästchen "4. Fall"
	void OnIDUser30(); // Kontrollkästchen "ge~-2t"
	void OnIDUser72(); // Kontrollkästchen "more/most"
	void OnSelChangeEnglishWordkind();

	virtual ~CVocableEditorBar();

//protected:
		BYTE m_bAr[80];
		BYTE m_bPreviousWordKind;
    CButton m_wndChild;
		CButton m_englishButton[19];
		CComboBox m_englishWordKind;
		CComboBox m_englishComboBox;
		CComboBox m_germanComboBox;
		CListBox m_germanListBox[3];
		CEdit m_englishEdit[20];
		CEdit m_germanEdit[20];
		CButton m_germanButton[20];
		CStatic m_germanStatic[20];
    CFont   m_font;
		CStatic m_english;
		CStatic m_wordkind;
		CStatic m_englishStatic[10];
		CString m_strAr[45];
		int m_nAr[5];

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

#endif // !defined(__MYBAR_H__)
