#if !defined(AFX_OPTIONSDIALOG_H__A0D85A81_5827_11D5_A7D8_0000B45A1FE5__INCLUDED_)
#define AFX_OPTIONSDIALOG_H__A0D85A81_5827_11D5_A7D8_0000B45A1FE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDialog.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld COptionsDialog 

class COptionsDialog : public CDialog
{
// Konstruktion
public:
	COptionsDialog(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(COptionsDialog)
	enum { IDD = IDD_OPTIONS_DIALOG };
	BOOL	m_bCaseSensitive;
	BOOL	m_bEnsureUserWantsDeletion;
	BOOL	m_bEnsureUserWantsModification;
	BOOL	m_bShowClauseProgress;
	BOOL	m_bShowSentenceProgress;
	BOOL	m_bShowTextProgress;
	DWORD	m_dwMaxCharsPerLine;
	BOOL	m_bShowPrognosticedRemainingTime;
	BOOL	m_bBufferOutput;
	BOOL	m_bShowGraphicClauseProgress;
	BOOL	m_bShowGraphicSentenceProgress;
	BOOL	m_bShowGraphicTextProgress;
	BOOL	m_bShowNumericTextProgress;
	BOOL	m_bShowNumericSentenceProgress;
	BOOL	m_bShowNumericClauseProgress;
	int		m_nGraphicClauseProgressType;
	int		m_nGraphicSentenceProgressType;
	int		m_nGraphicTextProgressType;
	int		m_nThreadPriority;
	BOOL	m_bAdoptGrammaticallyIncorrectSentences;
	BOOL	m_bAllowLowLetterAtTheBeginningOfSentence;
	BOOL	m_bAutomaticFormatting;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(COptionsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(COptionsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClauseProgressModeGraphicCheck();
	afx_msg void OnSentenceProgressModeGraphicCheck();
	afx_msg void OnTextProgressModeGraphicCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_OPTIONSDIALOG_H__A0D85A81_5827_11D5_A7D8_0000B45A1FE5__INCLUDED_
