#
// mybar.cpp : implementation file
//

#include "../stdafx.h"
#include "UnknownWordsBar.h"

// eigener Code, Anfang
#include "../Resource.h" //Use "./" because else e.g. cygwin's Resource.h is included.
//extern VocNode * m_first;
//extern VocNode * pCurrentVocNode;
// eigener Code, Ende

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyBar


void CUnknownWordsBar::DoDataExchange(CDataExchange* pDX)
{
	CSizingControlBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StatusDialog)
	DDX_Text(pDX, IDC_TEXT_STATIC, m_strText);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNAMIC(CUnknownWordsBar,CSizingControlBar)

BEGIN_MESSAGE_MAP(CUnknownWordsBar, baseCMyBar)
    //{{AFX_MSG_MAP(CSearchFormBar)
    ON_WM_CREATE()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
		//ON_CBN_SELCHANGE(IDC_ENGLISH_WORDKIND_COMBO, OnSelChangeEnglishWordkind)

END_MESSAGE_MAP()

CUnknownWordsBar::CUnknownWordsBar()
{
	//m_pMainFrame=NULL;
	//{{AFX_DATA_INIT(CUnknownWordsBar)
	m_strText = _T("");
	//}}AFX_DATA_INIT
}

CUnknownWordsBar::~CUnknownWordsBar()
{
}


/////////////////////////////////////////////////////////////////////////////
// CMyBar message handlers

int CUnknownWordsBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(baseCMyBar::OnCreate(lpCreateStruct)==-1)
		return -1;
	m_editText.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL|
		ES_MULTILINE|ES_WANTRETURN,CRect(0,0,100,100),this,IDC_TEXT_STATIC);
	return 0;
}

void CUnknownWordsBar::OnSize(UINT nType, int cx, int cy) 
{
	m_editText.MoveWindow(0,0,cx,cy,TRUE);
}