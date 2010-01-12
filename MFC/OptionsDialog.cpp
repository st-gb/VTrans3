// OptionsDialog.cpp: Implementierungsdatei
//

#include "../stdafx.h"
#include "VTrans.h"
#include "OptionsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld COptionsDialog 


COptionsDialog::COptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDialog)
	m_bCaseSensitive = FALSE;
	m_bEnsureUserWantsDeletion = FALSE;
	m_bEnsureUserWantsModification = FALSE;
	m_bShowClauseProgress = FALSE;
	m_bShowSentenceProgress = FALSE;
	m_bShowTextProgress = FALSE;
	m_dwMaxCharsPerLine = 0;
	m_bShowPrognosticedRemainingTime = FALSE;
	m_bBufferOutput = FALSE;
	m_bShowGraphicClauseProgress = FALSE;
	m_bShowGraphicSentenceProgress = FALSE;
	m_bShowGraphicTextProgress = FALSE;
	m_bShowNumericTextProgress = FALSE;
	m_bShowNumericSentenceProgress = FALSE;
	m_bShowNumericClauseProgress = FALSE;
	m_nGraphicClauseProgressType = -1;
	m_nGraphicSentenceProgressType = -1;
	m_nGraphicTextProgressType = -1;
	m_nThreadPriority = 0;
	m_bAdoptGrammaticallyIncorrectSentences = FALSE;
	m_bAllowLowLetterAtTheBeginningOfSentence = FALSE;
	m_bAutomaticFormatting = FALSE;
	//}}AFX_DATA_INIT
}


void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDialog)
	DDX_Check(pDX, IDC_CASE_SENSITIVE_CHECK, m_bCaseSensitive);
	DDX_Check(pDX, IDC_ENSURE_USER_WANTS_DELETION_CHECK, m_bEnsureUserWantsDeletion);
	DDX_Check(pDX, IDC_ENSURE_USER_WANTS_MODIFICATION_CHECK, m_bEnsureUserWantsModification);
	DDX_Text(pDX, IDC_MAX_CHARS_PER_LINE_EDIT, m_dwMaxCharsPerLine);
	DDX_Check(pDX, IDC_PROGNOSTICED_REMAINING_TIME_CHECK, m_bShowPrognosticedRemainingTime);
	DDX_Check(pDX, IDC_BUFFER_OUTPUT_CHECK, m_bBufferOutput);
	DDX_Check(pDX, IDC_CLAUSE_PROGRESS_MODE_GRAPHIC_CHECK, m_bShowGraphicClauseProgress);
	DDX_Check(pDX, IDC_SENTENCE_PROGRESS_MODE_GRAPHIC_CHECK, m_bShowGraphicSentenceProgress);
	DDX_Check(pDX, IDC_TEXT_PROGRESS_MODE_GRAPHIC_CHECK, m_bShowGraphicTextProgress);
	DDX_Check(pDX, IDC_TEXT_PROGRESS_MODE_NUMERIC_CHECK, m_bShowNumericTextProgress);
	DDX_Check(pDX, IDC_SENTENCE_PROGRESS_MODE_NUMERIC_CHECK, m_bShowNumericSentenceProgress);
	DDX_Check(pDX, IDC_CLAUSE_PROGRESS_MODE_NUMERIC_CHECK, m_bShowNumericClauseProgress);
	DDX_Radio(pDX, IDC_ROUGH_GRAPHIC_CLAUSE_PROGRESS_RADIO, m_nGraphicClauseProgressType);
	DDX_Radio(pDX, IDC_ROUGH_GRAPHIC_SENTENCE_PROGRESS_RADIO, m_nGraphicSentenceProgressType);
	DDX_Radio(pDX, IDC_ROUGH_GRAPHIC_TEXT_PROGRESS_RADIO, m_nGraphicTextProgressType);
	DDX_Slider(pDX, IDC_THREAD_PRIORITY_SLIDER, m_nThreadPriority);
	DDX_Check(pDX, IDC_ADOPT_GRAMMATICALLY_INCORRECT_SENTENCES_CHECK, m_bAdoptGrammaticallyIncorrectSentences);
	DDX_Check(pDX, IDC_ALLOW_LOW_LETTER_AT_THE_BEGINNING_OF_SENTENCE_CHECK, m_bAllowLowLetterAtTheBeginningOfSentence);
	DDX_Check(pDX, IDC_AUTOMATIC_FORMATTING_CHECK, m_bAutomaticFormatting);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDialog, CDialog)
	//{{AFX_MSG_MAP(COptionsDialog)
	ON_BN_CLICKED(IDC_CLAUSE_PROGRESS_MODE_GRAPHIC_CHECK, OnClauseProgressModeGraphicCheck)
	ON_BN_CLICKED(IDC_SENTENCE_PROGRESS_MODE_GRAPHIC_CHECK, OnSentenceProgressModeGraphicCheck)
	ON_BN_CLICKED(IDC_TEXT_PROGRESS_MODE_GRAPHIC_CHECK, OnTextProgressModeGraphicCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten COptionsDialog 

BOOL COptionsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Zusätzliche Initialisierung hier einfügen
	
	CSliderCtrl * pSlider=(CSliderCtrl *)GetDlgItem(IDC_THREAD_PRIORITY_SLIDER);
	if(pSlider)
	{
		pSlider->SetRange(0,5,TRUE);//Bereich (Positionen) zwischen 0 und 5
		pSlider->SetPageSize(2);//so viele Positionen sollen sich verändern,
		//wenn die Bild-Auf- oder Bild-Ab-Taste oder die Maus auf den 
		//Schieberegler geklickt wird
		pSlider->SetPos(m_nThreadPriority);//Position setzen
	}

	if(!m_bShowGraphicClauseProgress)
	{
		GetDlgItem(IDC_ROUGH_GRAPHIC_CLAUSE_PROGRESS_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SMOOTH_GRAPHIC_CLAUSE_PROGRESS_RADIO)->EnableWindow(FALSE);
	}
	if(!m_bShowGraphicSentenceProgress)
	{
		GetDlgItem(IDC_ROUGH_GRAPHIC_SENTENCE_PROGRESS_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SMOOTH_GRAPHIC_SENTENCE_PROGRESS_RADIO)->EnableWindow(FALSE);
	}
	if(!m_bShowGraphicTextProgress)
	{
		GetDlgItem(IDC_ROUGH_GRAPHIC_TEXT_PROGRESS_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SMOOTH_GRAPHIC_TEXT_PROGRESS_RADIO)->EnableWindow(FALSE);
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void COptionsDialog::OnClauseProgressModeGraphicCheck() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	UpdateData(TRUE);
	if(m_bShowGraphicClauseProgress)
	{
		GetDlgItem(IDC_ROUGH_GRAPHIC_CLAUSE_PROGRESS_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_SMOOTH_GRAPHIC_CLAUSE_PROGRESS_RADIO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ROUGH_GRAPHIC_CLAUSE_PROGRESS_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SMOOTH_GRAPHIC_CLAUSE_PROGRESS_RADIO)->EnableWindow(FALSE);
	}
}

void COptionsDialog::OnSentenceProgressModeGraphicCheck() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	UpdateData(TRUE);
	if(m_bShowGraphicSentenceProgress)
	{
		GetDlgItem(IDC_ROUGH_GRAPHIC_SENTENCE_PROGRESS_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_SMOOTH_GRAPHIC_SENTENCE_PROGRESS_RADIO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ROUGH_GRAPHIC_SENTENCE_PROGRESS_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SMOOTH_GRAPHIC_SENTENCE_PROGRESS_RADIO)->EnableWindow(FALSE);
	}
}

void COptionsDialog::OnTextProgressModeGraphicCheck() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	UpdateData(TRUE);
	if(m_bShowGraphicTextProgress)
	{
		GetDlgItem(IDC_ROUGH_GRAPHIC_TEXT_PROGRESS_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_SMOOTH_GRAPHIC_TEXT_PROGRESS_RADIO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ROUGH_GRAPHIC_TEXT_PROGRESS_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SMOOTH_GRAPHIC_TEXT_PROGRESS_RADIO)->EnableWindow(FALSE);
	}	
}
