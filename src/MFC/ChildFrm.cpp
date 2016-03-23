// ChildFrm.cpp : Implementierung der Klasse CChildFrame
//

#include "../stdafx.h"
#include "VTrans.h"

#include "ChildFrm.h"

// eigener Code, Anfang
//#include "EnglishView.h"
#include "VTransDoc.h"
#include "VTransView.h"
#include "MainFrm.h"
#include "../Text.h" //für die globale Variable g_pText
#include "VocabularyInMainMem/DoublyLinkedList/WordList.hpp"
// eigener Code, Ende

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

volatile BOOL g_bExecutedOnTranslateAutomaticallyFinishes=FALSE;
//volatile BYTE g_bContinue=FALSE;//TRUE;
volatile BYTE g_bTranslateAutomaticallyBeforeEnd=1;

//volatile DWORD g_dwStatus=0;
//volatile DWORD g_dwSentenceStatus=0;
//volatile DWORD g_dwTotal=0;
//volatile DWORD g_dwTranslated=0;
//volatile DWORD g_dwTokensInSentence=0;
volatile unsigned long g_ulChar=-1;
volatile DWORD * g_pdwCursorPosition;
Text * g_pText;
CMainFrame * g_pMainFrame;

CTime tAtTheBeginningOfTheTranslation;
//WordNode * m_first=NULL;
WordNode * g_pWordNodeLast=NULL;
WordNode * pWordNodeCurrent=NULL;
extern WordList wordList;

BYTE bFlag=FALSE;
/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)
//IMPLEMENT_DYNCREATE(CChildFrame, CSplitterWnd)


/////////////////////////////////////////////////////////////////////////////
// CChildFrame Konstruktion/Destruktion

CChildFrame::CChildFrame()
{
	// ZU ERLEDIGEN: Hier Code zur Member-Initialisierung einfügen
	TRACE("CChildFrame::CChildFrame() - ANFANG\n");
	bFlag=FALSE;
	TRACE("CChildFrame::CChildFrame() - ENDE\n");
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	if( !CMDIChildWnd::PreCreateWindow(cs) )
	//if( !CSplitterWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame Diagnose

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
	//CSplitterWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
	//CSplitterWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame Nachrichten-Handler

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	TRACE("BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) \n");

	//m_SplitterWnd.CreateStatic(this,2,1,WS_CHILD|WS_VISIBLE|WS_VSCROLL,100000);
	m_SplitterWnd.CreateStatic(this,2,1);//,WS_CHILD|WS_VISIBLE|WS_VSCROLL,100000);

	//m_SplitterWnd.SetColumnInfo(0,150,100);
	//m_SplitterWnd.SetColumnInfo(1,150,100);
	//m_SplitterWnd.SetRowInfo(0,200,120);
	//m_SplitterWnd.SetRowInfo(1,200,100);
	//m_SplitterWnd.SetRowInfo(2,150,100);

	//CEditView pView;//=new CEditView;
	//EnglishView pView;//=new CEditView;
	//pView.Create();

	CEditView pView2;//=new CEditView;

	SIZE size;
	size.cx=200;
	size.cy=200;
	//m_SplitterWnd.CreateView(0,0,pView.GetRuntimeClass(),CSize(200,150),pContext);
	//m_SplitterWnd.CreateView(0,0,RUNTIME_CLASS(CEditView),CSize(130,50),pContext);
	//m_SplitterWnd.CreateView(1,0,RUNTIME_CLASS(CEditView),CSize(0,0),pContext);
	//m_SplitterWnd.CreateView(1,0,pView.GetRuntimeClass(),CSize(200,150),pContext);
	//SetActiveView((CView*)m_SplitterWnd.GetPane(0,0));

	if (!m_SplitterWnd.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(130, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	//Zeiger auf den englischen Bereich erhalten
	EnglishView * pEV=(EnglishView*)m_SplitterWnd.GetPane(0,0);
	//pEV->OnToolTipNotify(
	pEV->EnableToolTips(TRUE);

	// add the second splitter pane - an input view in column 1
	if (!m_SplitterWnd.CreateView(1, 0,
		RUNTIME_CLASS(EnglishView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	/*if (!m_SplitterWnd.CreateView(1, 0,
		RUNTIME_CLASS(CEditView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}*/

	// activate the input view
	//SetActiveView((CView*)m_wndSplitter.GetPane(0,1));

	//return TRUE;
	CVTransDoc * pDoc=(CVTransDoc*)GetActiveDocument();
	if(pDoc)
	{
		EnglishView * pEV=(EnglishView*)m_SplitterWnd.GetPane(0,0);
		pEV->m_pText=&pDoc->m_EnglishText;
		pEV=(EnglishView*)m_SplitterWnd.GetPane(1,0);
		pEV->m_pText=&pDoc->m_GermanText;
	}
	else
		TRACE("!pDoc\n");
	/*if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(130, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CInputView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,1));
*/

	TRACE("BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) - ENDE\n");
	return TRUE;
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	//m_SplitterWnd.SetRowInfo(0,cy/2,cy/3);
	//m_SplitterWnd.SetRowInfo(1,cy/2,cy/3);
	TRACE("void CChildFrame::OnSize(UINT nType, int cx, int cy) \n");
#ifdef _DEBUG
	printf("void CChildFrame::OnSize(UINT nType, int cx, int cy) ANFANG\n");
#endif

	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen
#ifdef _DEBUG
	printf("cx: %d  cy: %d:  m_iHeight: %d\n",cx,cy,m_iHeight);
#endif
	if(bFlag)
	{
		//CVTransDoc * pDoc=(CVTransDoc*)GetActiveDocument();
		CVTransApp * pApp=(CVTransApp *)AfxGetApp();
		//CMainFrame * pFrame=(CMainFrame *) AfxGetApp()->m_pMainWnd;
		CMainFrame * pFrame=(CMainFrame *) pApp->m_pMainWnd;
		if(!pFrame)
			TRACE("!pFrame\n");
		else
		{
			TRACE("pFrame\n");
			CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
			CVTransDoc *pDoc = (CVTransDoc *) pChild->GetActiveDocument();
			if(!pDoc)
				TRACE("!pDoc\n");
		}
		//CVTransDoc * pDoc=(CVTransDoc *) pFrame->GetActiveDocument();
		/*CVTransDoc * pDoc=(CVTransDoc *)GetActiveDocument();
		if(pDoc)
		{
			TRACE("pDoc\n");
			EnglishView * pEV=(EnglishView*)m_SplitterWnd.GetPane(0,0);
			pEV->m_pText=&pDoc->m_EnglishText;
			//pEV=(EnglishView*)m_SplitterWnd.GetPane(1,0);
			pEV->m_pText=&pDoc->m_GermanText;
		}
		else
			TRACE("!pDoc\n");*/
		CVTransView * pView=(CVTransView *)GetActiveView();
		if(pView)
		{
			TRACE("pView\n");
			//pDoc=(CVTransDoc *)pView->GetDocument();//GetActiveWindow
			/*CVTransDoc * pDoc=pView->GetDocument();//GetActiveWindow
			if(pDoc)
			{
				TRACE("pDoc\n");
				EnglishView * pEV=(EnglishView*)m_SplitterWnd.GetPane(0,0);
				pEV->m_pText=&pDoc->m_EnglishText;
				pEV=(EnglishView*)m_SplitterWnd.GetPane(1,0);
				pEV->m_pText=&pDoc->m_GermanText;
			}
			else
				TRACE("!pDoc\n");*/
		}
		else
			TRACE("!pView\n");

		TRACE("cy/2: %d\n",cy/2);
		TRACE("cy/3: %d\n",cy/3);
		int cyCur=0,cyMin=0;
		m_SplitterWnd.GetRowInfo(0,cyCur,cyMin);
#ifdef _DEBUG
		printf("cyCur: %d,cyMin: %d:\n",cyCur,cyMin);
		//m_SplitterWnd.GetRowInfo(1,cyCur,cyMin);
		printf("cyCur: %d,cyMin: %d:\n",cyCur,cyMin);
#endif
		m_SplitterWnd.SetRowInfo(0,(int)((double)(cyCur*100)/(double)m_iHeight*(double)cy/100.0),0);//2,0);//cy/3);
		m_SplitterWnd.SetRowInfo(1,cy-(int)((double)(cyCur*100)/(double)m_iHeight*(double)cy/100.0),0);//cy/2,0);//cy/3);
		m_SplitterWnd.RecalcLayout();
#ifdef _DEBUG
		m_SplitterWnd.GetRowInfo(0,cyCur,cyMin);
		printf("cyCur: %d,cyMin: %d:\n",cyCur,cyMin);
		TRACE("cyCur: %d,cyMin: %d:\n",cyCur,cyMin);
		m_SplitterWnd.GetRowInfo(1,cyCur,cyMin);
		printf("cyCur: %d,cyMin: %d:\n",cyCur,cyMin);
		TRACE("cyCur: %d,cyMin: %d:\n",cyCur,cyMin);
#endif
		//LPRECT lpRect;
		//::GetClientRect(lpRect);
		//m_SplitterWnd.GetClientRect
		//TRACE("lpRect.bottom: %d\n",lpRect->bottom);
		//TRACE("lpRect.bottom: %d\n",lpRect->bottom);
		//EnglishView * pEV=(EnglishView*)m_SplitterWnd.GetPane(0,0);
		EnglishView * pEV=(EnglishView*)m_SplitterWnd.GetPane(1,0);
		pEV->MoveComboBoxes();
		pEV->CalculateNumberOfLines();
		ColorText ct;
		ct.str="tt";
		ct.color=2;
		//pEV->m_Text.push_back(ct);
		//pEV->Invalidate(TRUE);
	}
	else
		bFlag=TRUE;
	m_iHeight=cy;

#ifdef _DEBUG
	printf("void CChildFrame::OnSize(UINT nType, int cx, int cy) ENDE\n");
#endif
	TRACE("void CChildFrame::OnSize(UINT nType, int cx, int cy) - ENDE\n");
}

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
//BEGIN_MESSAGE_MAP(CChildFrame, CSplitterWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CChildFrame::OnClose() 
{
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen und/oder Standard aufrufen
//#ifdef _DEBUG
	CVTransApp * pApp=(CVTransApp *)AfxGetApp();
	if(pApp)
	{
		CMainFrame * pFrame=(CMainFrame *)pApp->m_pMainWnd;
		DWORD dwExitCode=0;
		if(pFrame)
		{
			if(pFrame->m_pTranslationThread)
			{
				GetExitCodeThread(pFrame->m_pTranslationThread->m_hThread,&dwExitCode);
				if(dwExitCode==STILL_ACTIVE)
				{
					if(MessageBox("Der Text dieses Kindfensters wird gerade überprüft\
/übersetzt. Wollen Sie diesen Prozess wirklich abbrechen?","Frage",MB_YESNO)==IDNO)
						//das Schliessen des Fensters ( CMDIChildWnd::OnClose() )überspringen
						return;
					//Flag setzen, damit die (möglicherweise verschachtelt 
					//aufgerufenen) Übersetzungs- /Grammatiküberprüfungsfunktionen
					//frühzeitig beendet werden
					g_bContinue=FALSE;
					//Ohne die Funktion "WaitForSingleObject" stürzte VTrans 
					//manchmal ab
					//WaitForSingleObject läuft viel flüssiger als eine Schleife, in
					//der ständig mittels "::GetExitCodeThread"überprüft wird, ob der 
					//Thread schon beendet wurde. Die Funktion "WaitForSingleObject"
					//scheint also den Prozessor weniger zu belasten, was die MSDN 
					//Biblithek (also die Dokumentation/Hilfe zu Visual C++ belegt:
					//"The thread consumes very little processor time while waiting for
					//the object state to become signaled or the time-out interval to elapse."
					//warten, bis der Übersetzungsthread beendet ist
					WaitForSingleObject(pFrame->m_pTranslationThread->m_hThread,INFINITE);
				}
			}
		}
	}
	CMDIChildWnd::OnClose();
}
