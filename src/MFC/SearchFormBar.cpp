// mybar.cpp : implementation file
//

#include "../stdafx.h"
#include "SearchFormBar.h"

// eigener Code, Anfang
#include <typeinfo.h> // für RTTI (RunTime Type Information)
#include "../Resource.h"
#include "MainFrm.h"
#include "VTrans.h"
#include "../Translate.h" // für CStringVector
#include "../VocabularyInMainMem/LetterTree/LetterTree.hpp"
#include "VocabularyInMainMem/DoublyLinkedList/WordNode.hpp"
#include "VocabularyInMainMem/DoublyLinkedList/WordList.hpp"

//extern VocNode * m_first;
extern WordNode * pWordNodeCurrent;
extern LetterTree g_lettertree ;
extern WordList wordList;

// eigener Code, Ende

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyBar

IMPLEMENT_DYNAMIC(CSearchFormBar,CSizingControlBar)

BEGIN_MESSAGE_MAP(CSearchFormBar, baseCMyBar)
    //{{AFX_MSG_MAP(CSearchFormBar)
    ON_WM_CREATE()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
		//ON_CBN_SELCHANGE(IDC_ENGLISH_WORDKIND_COMBO, OnSelChangeEnglishWordkind)
		ON_EN_CHANGE(IDC_WORD_EDIT, OnChangeWordEdit)
		ON_BN_CLICKED(IDC_VOCABLE_FORWARD,OnIDUser101)

END_MESSAGE_MAP()

CSearchFormBar::CSearchFormBar(WordSearchType * p_wordsearch)
{
	//m_pMainFrame=NULL;
  mp_wordsearch = p_wordsearch ;
}

/*CSearchFormBar::CSearchFormBar(CMyBar & mybar)
{
	m_ptrMyBar=mybar;
}*/

CSearchFormBar::~CSearchFormBar()
{
}

void CSearchFormBar::Attach(CVocableEditorBar & vebar)
{
	m_pVocableEditorBar=&vebar;
}


/////////////////////////////////////////////////////////////////////////////
// CMyBar message handlers

int CSearchFormBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(baseCMyBar::OnCreate(lpCreateStruct)==-1)
		return -1;

    /*if (!m_wndChild.Create(_T("Paste"),
        WS_CHILD|WS_VISIBLE,
        CRect(0,0,0,0), this, ID_EDIT_PASTE))
        return -1;*/

		/*if (!m_english.Create(_T("Englisch"),WS_CHILD|WS_VISIBLE,
        CRect(10,20,10,20), this,ID_USER+1))
				return -1;*/

	if(!m_reverseButton.Create(_T("<"),WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
		CRect(10,20,10,20), this,ID_USER+100))
		return -1;
	if(!m_forwardButton.Create(_T(">"),WS_CHILD|WS_VISIBLE,
		CRect(30,20,50,40),this,IDC_VOCABLE_FORWARD))
		return -1;
	if(!m_searchEdit.Create(WS_CHILD|WS_VISIBLE,
		CRect(10,20,10,20), this,IDC_WORD_EDIT))
		return -1;

    //UpdateDialogControls(this, TRUE);
			//UpdateData(FALSE);
			//Invalidate(TRUE);
			//OnPaint();
		
    // older versions of Windows* (NT 3.51 for instance)
    // fail with DEFAULT_GUI_FONT
	if (!m_font.CreateStockObject(DEFAULT_GUI_FONT))
		if (!m_font.CreatePointFont(80,"MS Sans Serif"))
			return -1;

    //m_english.SetFont(&m_font);
		//m_wordkind.SetFont(&m_font);
	m_searchEdit.SetFont(&m_font);
	//m_forwardButton.SetFont(&m_font);
	m_reverseButton.SetFont(&m_font);
	

		//m_englishWordKind.SetHorizontalExtent(200);
		//m_englishWordKind.SetDroppedWidth(200);
//		m_englishWordKind.AddString(_T("Substantiv"));
	//	m_englishWordKind.AddString(_T("Verb"));
//		m_englishWordKind.AddString(_T("Adjektiv"));
	//	m_englishWordKind.AddString(_T("Adverb"));
//		m_englishWordKind.SetCurSel(0);
    return 0;

		//if(m_englishWordKind.GetCurSel()==0)
		//{
//		m_englishCheck[0].MoveWindow(CRect(100,50,140,140));
//		m_englishStatic[0].MoveWindow(CRect(0,20,40,40));
//		m_englishStatic[1].MoveWindow(CRect(50,0,100,15));
		//m_englishStatic[2].MoveWindow(CRect(140,0,200,15));
//		m_englishStatic[3].MoveWindow(CRect(230,0,300,15));
		//m_english.MoveWindow(CRect(0,20,40,40));
	//	m_englishEdit[0].MoveWindow(CRect(140,20,220,40));
//		m_englishEdit[1].MoveWindow(CRect(230,20,300,40));
	//	m_englishWordKind.MoveWindow(CRect(50,20,130,100));
    //virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
    //virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
			//RecalcLayout();

//	UpdateData(FALSE);
//	Invalidate(FALSE);
	//OnPaint();
		//}
}

void CSearchFormBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	//TRACE("void CSearchFormBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)\n");
    baseCMyBar::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);

    UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

void CSearchFormBar::OnSize(UINT nType, int cx, int cy) 
{
	baseCMyBar::OnSize(nType, cx, cy);

  // TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);

	m_searchEdit.MoveWindow(CRect(0,0,100,15));
	m_reverseButton.MoveWindow(CRect(0,20,20,40));
	//m_forwardButton.MoveWindow(CRect(30,20,50,40));
    //m_wndChild.MoveWindow(rc);
}

void CSearchFormBar::OnChangeWordEdit()
{ // wird aufgerufen, wenn ein Eintrag im Kombinationsfeld 'Einheit'
	// ausgewählt wird
#ifdef _DEBUG
	printf("void CSearchFormBar::OnChangeWordEdit()BEGINN\n");
	fflush(stdout);
#endif
	CMainFrame * pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	//pFrame->m_wndToolBar.GetDlgItem(ID_NEXT_VOCABLE_BUTTON)->EnableWindow(TRUE);
	//GetDlgItem(IDC_WORD_EDIT)->EnableWindow(FALSE);
	//GetDlgItem(IDC_VOCABLE_FORWARD)->EnableWindow(TRUE);
	char * buffer="ffg";
	pFrame->m_wndToolBar.SetDlgItemText(ID_NEXT_VOCABLE_BUTTON,buffer);
	//SetDlgItemText(IDC_VOCABLE_FORWARD,buffer);
		///return -1;
	CString str;
	GetDlgItemText(IDC_WORD_EDIT,str);
	TRACE("CSearchFormBar::OnChangeWordEdit--Zeichenkette: %s\n",str);
  //std::set<VocabularyAndTranslation> * psetvocabularyandtranslation = 
  std::set<VocabularyAndTranslation *> * psetpvocabularyandtranslation = 
    g_lettertree.search(str.GetBuffer(), 0, str.GetLength()) ;
  //TRACE("Word exits in LetterTree: %u\n", psetvocabularyandtranslation ) ;
  TRACE("Word exits in LetterTree: %u\n", psetpvocabularyandtranslation ) ;
  //if(psetvocabularyandtranslation)
  if(psetpvocabularyandtranslation)
  {
    TRACE("psetvocabularyandtranslation->begin exists\n") ;
    //VocabularyAndTranslation & vocabularyandtranslation = 
    //  *psetvocabularyandtranslation->begin() ;
    VocabularyAndTranslation * pvocabularyandtranslation = 
      *psetpvocabularyandtranslation->begin() ;
    //if( vocabularyandtranslation.m_byType == WORD_TYPE_NOUN )
    if( pvocabularyandtranslation->m_byType == WORD_TYPE_NOUN )
    {
      //TRACE("%s", vocabularyandtranslation.m_arstrEnglishWord[0].c_str() ) ;
      //TRACE("%s", vocabularyandtranslation.m_arstrEnglishWord[1].c_str() ) ;
      //TRACE("%s %s", vocabularyandtranslation.m_arstrGermanWord[0].c_str(),
      //  vocabularyandtranslation.m_arstrGermanWord[1].c_str() ) ;
      TRACE("%s", pvocabularyandtranslation->m_arstrEnglishWord[0].c_str() ) ;
      TRACE("%s", pvocabularyandtranslation->m_arstrEnglishWord[1].c_str() ) ;
      TRACE("%s %s", pvocabularyandtranslation->m_arstrGermanWord[0].c_str(),
        pvocabularyandtranslation->m_arstrGermanWord[1].c_str() ) ;
    }
  }
	/*if(str.GetLength()<2)
	{
		m_forwardButton.Create(_T(">"),WS_CHILD|WS_VISIBLE,
		CRect(30,20,50,40),this,1011);//IDC_VOCABLE_FORWARD))
		m_forwardButton.ShowWindow(TRUE);
	}*/

	//GetDlgItem(ID_USER+101)->EnableWindow(TRUE);
	UpdateDialogControls(this,TRUE);
	UpdateData(FALSE);
	//		Invalidate(TRUE);
	//		OnPaint();
	pWordNodeCurrent = NULL;
	if(wordList.m_pWordNodeFirst)
	{
		if( str == _T("*") )
		{
			pWordNodeCurrent = wordList.m_pWordNodeFirst;
			if(wordList.m_pWordNodeFirst->m_pWordNodeNext)
			{
        m_pVocableEditorBar->WordToGUI(
          *wordList.m_pWordNodeFirst->m_pWord,  
          *wordList.m_pWordNodeFirst->m_pWordNodeNext->m_pWord );
				pWordNodeCurrent = pWordNodeCurrent->m_pWordNodeNext;
				if(pWordNodeCurrent->m_pWordNodeNext)
					pFrame->m_bEnableNextVocableButton = TRUE;
			}					
		}
		else
		{
		  //TRACE("m_first ist ungleich NULL\n");

		}
	}
  //Word * p_wordEnglish ;
  //Word * p_wordGerman ;
  Word wordEnglish ;
  Word wordGerman ;
  AutomDelWord automdelwordEng ;
  AutomDelWord automdelwordGer ;
  //wordList.GetFirstOccurance( 
  //  std::string(str.GetBuffer() ) 
  //  , & p_wordEnglish
  //  , & p_wordGerman
  //  ) ;
	m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind = 0;
  if( 
    mp_wordsearch->GetFirstOccurance( 
      std::string(str.GetBuffer() ) 
      //, p_wordEnglish
      //, p_wordGerman
      //, wordEnglish
      //, wordGerman
      , automdelwordEng
      , automdelwordGer
      )
    )
  //if( p_wordEnglish )
	  m_pVocableEditorBar->WordToGUI( //current
      //* p_wordEnglish
      //, *p_wordGerman
      //wordEnglish
      //, wordGerman
      *automdelwordEng.GetWord()
      ,*automdelwordEng.GetWord()
      );
	if(pWordNodeCurrent)
		((CVTransApp*)AfxGetApp())->m_strSearch=str;
	UpdateDialogControls(this,TRUE);
	UpdateData(FALSE);
}


//OnSelChangeEnglishWordkind

/*void CMyBar::OnSelChangeEnglishWordkind()
{

}*/
void CSearchFormBar::OnIDUser101()
{

}