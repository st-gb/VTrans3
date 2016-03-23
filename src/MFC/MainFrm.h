// MainFrm.h : Schnittstelle der Klasse CMainFrame
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__FB6F08E6_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
#define AFX_MAINFRM_H__FB6F08E6_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// eigener Code, Anfang
#include "UnknownWordsBar.h"
#include "WordEditorBar.h"
#include "SearchFormBar.h"
#include "EnglishView.h"
#include "VTrans.h"
#include "../Text.h"
#include "StatusDialog.h"
#include "../Translate.h" //für class Range
#include "../VocabularyInMainMem/WordSearch.hpp"

#define WM_TRANSLATE_AUTOMATICALLY_FINISHES WM_USER+1
//extern VocNode * m_first;
// eigener Code, Ende
extern volatile BYTE g_bContinue;

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame(WordSearchType * p_wordsearch);

// Attribute
public:

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // Eingebundene Elemente der Steuerleiste
	CVocableEditorBar m_wndVocableEditorBar;
	CSearchFormBar m_wndSearchFormBar; 
public:
	BYTE m_bShiftDown;
	BOOL m_bAdoptGrammaticallyIncorrectSentences;
	BOOL m_bAllowLowLetterAtTheBeginningOfSentence;
	BOOL m_bBufferOutput;
	BOOL m_bCaseSensitive;
	BOOL m_bEnablePreviousVocableButton;
	BOOL m_bEnableNextVocableButton;
	BOOL m_bEnsureUserWantsDeletion;
	BOOL m_bEnsureUserWantsModification;
	BOOL m_bShowClauseProgress;
	BOOL m_bShowPrognosticedRemainingTime;
	BOOL m_bShowSentenceProgress;
	BOOL m_bShowTextProgress;
	BOOL m_bShowGraphicClauseProgress;
	BOOL m_bShowNumericClauseProgress;
	BOOL m_bShowGraphicSentenceProgress;
	BOOL m_bShowNumericSentenceProgress;
	BOOL m_bShowGraphicTextProgress;
	BOOL m_bShowNumericTextProgress;
	BOOL m_bTimerIntalled;
	CUnknownWordsBar m_wndUnknownWordsBar;
	CStatic m_cStaticClauseProgress;
	CStatic m_cStaticRemainingTime;
	CStatic m_cStaticSentenceProgress;
	CStatic m_cStaticTextProgress;
	CProgressCtrl m_Progress;
	CProgressCtrl m_SentenceProgress;
	CProgressCtrl m_TextProgress;
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CWinThread * m_pTranslationThread;
	DWORD m_dwMaxCharsPerLine;
	int m_nThreadPriority;
	int m_nGraphicTextProgressType;
	int m_nGraphicSentenceProgressType;
	int m_nGraphicClauseProgressType;
	volatile BOOL m_bCreated;
	volatile BOOL m_bMayStartThread;
	LONG OnTranslateAutomaticallyFinishes(UINT wParam, LONG lParam);
// Generierte Message-Map-Funktionen
protected:
	
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditPaste();
	afx_msg void OnTranslateButton();
	afx_msg void OnExtrasCountWords();
	afx_msg void OnVocableEditorButton();
	afx_msg void OnViewSearchForm();
	afx_msg void OnDestroy();
	afx_msg void OnAddVocableButton();
	afx_msg void OnDestroyClipboard();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnOverwriteVocableButton();
	afx_msg void OnDeleteVocableButton();
	afx_msg void OnComboboxButton();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateNextVocableButton(CCmdUI* pCmdUI);
	afx_msg void OnNextVocableButton();
	afx_msg void OnUpdatePreviousVocableButton(CCmdUI* pCmdUI);
	afx_msg void OnPreviousVocableButton();
	afx_msg void OnUpdateTranslateAutomaticallyIfTextmodificationButton(CCmdUI* pCmdUI);
	afx_msg void OnTranslateAutomaticallyIfTextmodificationButton();
	afx_msg void OnExtrasOptionen();
	afx_msg void OnUpdateDeleteVocableButton(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEditCut();
	afx_msg void OnEditMarkAll();
	afx_msg void OnUpdateComboboxButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTranslateButton(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class TranslateAutomaticallyParameters
{
public:
	BYTE m_bAdoptGrammaticallyIncorrectSentences;
	BYTE m_bAllowLowLetterAtTheBeginningOfSentence;
	BYTE m_bCaseSensitive;
	DWORD m_dwMaxCharsPerLine;
	HWND m_postHWnd;
	HWND m_hWnd;
	HWND m_hWndProgress;
	HWND m_hWndSentenceProgress;
	HWND m_hWndTextProgress;
	HWND m_hWndStatusBar;
	Text * m_pText;
	std::vector<Range> m_vecRange;
	EnglishView * m_pEV;
	TranslateAutomaticallyParameters(Text * pText,std::vector<Range> & vecRange,EnglishView * pEV,HWND postHWnd,HWND hWnd,HWND hWndProgress,HWND hWndSentenceProgress,HWND hWndTextProgress)
	{
		m_pText=pText;
		m_vecRange=vecRange;
		m_pEV=pEV;
		m_postHWnd=postHWnd;
		m_hWnd=hWnd;
		m_hWndProgress=hWndProgress;
		m_hWndSentenceProgress=hWndSentenceProgress;
		m_hWndTextProgress=hWndTextProgress;
	}
};

class WaitForEndOfTranslateAutomaticallyParameters
{
public:
	HANDLE m_hThread;
};

class TranslateParameters
{
public:
	DWORD m_dwMaxCharsPerLine;
	EnglishView * m_pEV;
	EnglishView * m_pEnglishViewGerman;
	Text * m_pText;
	std::vector<Range> m_vecRange;
	std::vector<CStringVector> * m_pVecVecStr;
	StatusDialog * m_statusDlg;
	//HWND * m_pHWnd;
  HWND m_hwnd;
	TranslateParameters(Text * pText,std::vector<Range> & vecRange,
		std::vector<CStringVector> * pVecVecStr,StatusDialog * statusDlg,
		//HWND * pHWnd
    HWND hwnd)
	{
		m_pEV=NULL;
		m_pEnglishViewGerman=NULL;
		m_pText=pText;
		m_vecRange=vecRange;
		m_pVecVecStr=pVecVecStr;
		m_statusDlg=statusDlg;
		//m_pHWnd=pHWnd;
		m_hwnd=hwnd;
	}
};

UINT Translate(LPVOID pParam);
UINT TranslateAutomatically(LPVOID pParam);
UINT WaitForEndOfTranslateAutomatically(LPVOID pParam);



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_MAINFRM_H__FB6F08E6_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
