// VTransDoc.cpp : Implementierung der Klasse CVTransDoc
//

#include "../stdafx.h"
#include "VTrans.h"

#include "VTransDoc.h"
// eigener Code, Anfang
#include "MainFrm.h"
#include "ChildFrm.h"
#include "EnglishView.h"
#include "LoadStatusDialog.h" // für den Datei-Lade-Status
// eigener Code, Ende

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern volatile BYTE g_bContinueLoading;
/////////////////////////////////////////////////////////////////////////////
// CVTransDoc

//IMPLEMENT_DYNCREATE(CVTransDoc, CDocument)
IMPLEMENT_SERIAL(CVTransDoc, CDocument, 0 /* schema number*/ )

BEGIN_MESSAGE_MAP(CVTransDoc, CDocument)
	//{{AFX_MSG_MAP(CVTransDoc)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVTransDoc Konstruktion/Destruktion

CVTransDoc::CVTransDoc()
{
	// ZU ERLEDIGEN: Hier Code für One-Time-Konstruktion einfügen

	//m_EnglishText=NULL;
	//m_GermanText=NULL;
	bEnglishText=FALSE;
}

CVTransDoc::~CVTransDoc()
{
}

BOOL CVTransDoc::OnNewDocument()
{
	TRACE("BOOL CVTransDoc::OnNewDocument()\n");
	if (!CDocument::OnNewDocument())
		return FALSE;

	// ZU ERLEDIGEN: Hier Code zur Reinitialisierung einfügen
	// (SDI-Dokumente verwenden dieses Dokument)

	m_dwEnglishLines=0;
	CMainFrame * pFrame=(CMainFrame *) AfxGetApp()->m_pMainWnd;
	if(pFrame)
	{
		TRACE("pFrame\n");
/*		CChildFrame * pChild=(CChildFrame *)pFrame->GetActiveFrame();
		if(pChild)
		{
			EnglishView * pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(0,0);
			if(pEV)
				pEV->m_pText=&m_EnglishText;
			pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
			if(pEV)
				pEV->m_pText=&m_GermanText;
		}*/
		CChildFrame *pChild=(CChildFrame *)pFrame->GetActiveWindow();
#ifdef _DEBUG
		if(pChild)
		{
			TRACE("pChild\n");
			/*EnglishView * pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(0,0);
			if(pEV)
				pEV->m_pText=&m_EnglishText;
			pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
			if(pEV)
				pEV->m_pText=&m_GermanText;*/
		}
		else
			TRACE("!pChild\n");
#endif
		/*CChildFrame **/pChild = (CChildFrame *)pFrame->MDIGetActive();
		/*if(pChild)
		{
			TRACE("pChild\n");
			EnglishView * pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(0,0);
			if(pEV)
				pEV->m_pText=&m_EnglishText;
			pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
			if(pEV)
				pEV->m_pText=&m_GermanText;
		}
		else
			TRACE("!pChild\n");*/
		//GetActiveWindow
	}
	/*POSITION pos = GetFirstViewPosition();
	CView* pFirstView = GetNextView(pos);
	CFrameWnd* pFrameWnd=pFirstView->GetParentFrame();
	if(pFrameWnd)
		TRACE("pFrameWnd\n");
	else
		TRACE("!pFrameWnd\n");

	CChildFrame * pChild=(CChildFrame *)pFirstView->GetParentFrame();
	if(pChild)
	{
		TRACE("pChild\n");
		EnglishView * pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(0,0);
		if(pEV)
			pEV->m_pText=&m_EnglishText;
		pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
		if(pEV)
			pEV->m_pText=&m_GermanText;
	}
	else
		TRACE("!pChild\n");
	TRACE("BOOL CVTransDoc::OnNewDocument() - ENDE\n");*/
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CVTransDoc Serialisierung

void CVTransDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// ZU ERLEDIGEN: Hier Code zum Speichern einfügen
	}
	else
	{
		// ZU ERLEDIGEN: Hier Code zum Laden einfügen
	}
}

/////////////////////////////////////////////////////////////////////////////
// CVTransDoc Diagnose

#ifdef _DEBUG
void CVTransDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVTransDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVTransDoc Befehle

BOOL CVTransDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
#ifdef _DEBUG
	printf("BOOL CVTransDoc::OnOpenDocument(LPCTSTR lpszPathName) ANFANG\n");
#endif
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Speziellen Erstellungscode hier einfügen

	/*m_dwEnglishLines=0;
	CFile file;
	if(file.Open(lpszPathName,CFile::modeReadWrite|CFile::shareDenyRead,NULL))
	{
		TRACE("Datei %s erfolgreich geöffnet\n",lpszPathName);
		BYTE * byteArray=new byte[file.GetLength()];
		if(byteArray && file.GetLength()>0)
		{
			//DWORD dwInsertionPos=0;
			//TextNode * pTextNodeCurrent=m_EnglishText.m_first;
			DWORD dwFileLength=file.GetLength();
			TextNode * pTextNodeCurrent=NULL;
			TextNode * pTextNodePrevious=NULL;
			file.Read(byteArray,dwFileLength);
			for(DWORD dwRun=0;dwRun<dwFileLength;dwRun++)
			{
				//if(byteArray[dwRun]!=10)
				if(pTextNodePrevious==NULL)
				{
					if((m_EnglishText.m_first=new TextNode(byteArray[dwRun],0))==NULL)
					{
						AfxMessageBox(_T("Nicht genügend Arbeitsspeicher vorhanden um \
die Datei zu laden."),MB_OK,0);
						break;
					}
					pTextNodeCurrent=m_EnglishText.m_first;
					pTextNodePrevious=m_EnglishText.m_first;
				}
				else
				{
					if((pTextNodeCurrent->m_next=new TextNode(byteArray[dwRun],0))==NULL)
					{
						AfxMessageBox(_T("Nicht genügend Arbeitsspeicher vorhanden um \
die Datei zu laden."),MB_OK,0);
						break;
					}
					pTextNodeCurrent=pTextNodeCurrent->m_next;
					pTextNodeCurrent->m_previous=pTextNodePrevious;
					pTextNodePrevious=pTextNodeCurrent;
				}
				//if(pTextNodePrevious!=NULL)
				//	pTextNodeCurrent->m_previous=pTextNodePrevious;
				//else
				//	m_EnglishText.m_first=pTextNodeCurrent;
				//pTextNodePrevious=pTextNodeCurrent;
				//pTextNodeCurrent=pTextNodeCurrent->m_next;
				//m_EnglishText.Insert(dwInsertionPos++,byteArray[dwRun],0);
				//else
				//if(byteArray[dwRun]==13)
				//	m_dwEnglishLines++;
			}
			//pTextNodeCurrent=NULL;
			pTextNodeCurrent->m_next=NULL;
			// allozierten Speicher freigeben
			delete [] byteArray;
		}
		file.Close();
	}
	//CVTransApp * pApp=(CVTransApp)AfxGetApp();
	*/
	CWinThread * loadFileThread;
	CLoadStatusDialog loadStatusDlg;
	OpenFileParameters * pOpenFileParameters=new OpenFileParameters();
	pOpenFileParameters->m_lpszPathName=lpszPathName;
	pOpenFileParameters->m_pEnglishText=&m_EnglishText;
	pOpenFileParameters->m_phWndLoadStatusDialog=&loadStatusDlg.m_hWnd;
	pOpenFileParameters->m_pLoadStatusDialog=&loadStatusDlg;
	g_bContinueLoading=TRUE;
	loadFileThread=AfxBeginThread(OpenFile,pOpenFileParameters);//,THREAD_PRIORITY_NORMAL,0,0,NULL);
	//MessageBox msgBox("hh","jjj",MB_OKCANCEL);
#ifdef _DEBUG
	printf("vor: if(loadStatusDlg.DoModal()==IDCANCEL)\n");
#endif
	if(loadStatusDlg.DoModal()==IDCANCEL)
	{
		g_bContinueLoading=FALSE;
		
		//bUserCanceled=TRUE;
	}
#ifdef _DEBUG
	printf("BOOL CVTransDoc::OnOpenDocument(LPCTSTR lpszPathName) ENDE\n");
#endif
	return TRUE;
}

BOOL CVTransDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	
	CFile file;
	if(file.Open(lpszPathName,CFile::modeWrite|CFile::shareDenyRead,NULL))
	{
		TRACE("Datei %s erfolgreich geöffnet\n",lpszPathName);
		//BYTE * byteArray=new byte[file.GetLength()];
		//file.Read(byteArray,file.GetLength());
		CString str;
		m_EnglishText.GetCString(str,0,m_EnglishText.GetLength());
		file.Write(str.GetBuffer(str.GetLength()),str.GetLength());
		file.Close();
	}
	return TRUE;
	//CDocument::OnSaveDocument(lpszPathName);
}
