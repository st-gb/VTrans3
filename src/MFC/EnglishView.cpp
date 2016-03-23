#include "../StdAfx.h" //für vorkompilierte Header
#include "EnglishView.h"
#include "VTransDoc.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "MemDc.h" // für das Puffern der Bildschirmausgabe
//#include"Text.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CHARACTER 1

extern volatile BOOL g_bExecutedOnTranslateAutomaticallyFinishes;
extern volatile BYTE g_bTranslateAutomaticallyBeforeEnd;
extern volatile BYTE g_bContinue;
extern CTime tAtTheBeginningOfTheTranslation;

//die nachfolgenden 3 Variablen werden für den Datanaustausch mit 
//CMainFrame::OnTranslateAutomaticallyFinishes genutzt
extern volatile unsigned long g_ulChar;
extern volatile DWORD * g_pdwCursorPosition;
extern Text * g_pText;
extern CMainFrame * g_pMainFrame;

void ComboBoxList::Clear()
{
	TRACE("void ComboBoxList::Clear() - ANFANG\n");
	ComboBoxListElement * pCurrent=NULL;
	pCurrent=m_pFirst;
	while(pCurrent)
	{
		ComboBoxListElement * pNext=pCurrent->m_pNext;
		delete pCurrent;
		pCurrent=pNext;
	}
	m_pFirst=m_pLast=NULL;
	TRACE("void ComboBoxList::Clear() - ENDE\n");
}

IMPLEMENT_DYNCREATE(EnglishView, CView)

BEGIN_MESSAGE_MAP(EnglishView, CView)
	//{{AFX_MSG_MAP(EnglishView)
	ON_WM_CHAR()
	ON_WM_ERASEBKGND() // Deklarieren, um die Methode zu überschreiben, denn
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_ACTIVATE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	// Standard-Druckbefehle
END_MESSAGE_MAP()

/*int EnglishView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//if(!m_wndVert.Create(SBS_VERT|WS_VISIBLE,CRect(0,0,10,10),this,33500))
	//	return -1;
	//edit.Create(WS_CHILD|WS_VISIBLE,CRect(20,20,40,40),this,33100);
	//edit.Create(WS_CHILD|ES_AUTOHSCROLL,CRect(20,20,40,40),this,33100);
	return 0;
}*/

/////////////////////////////////////////////////////////////////////////////
// CVTransView Konstruktion/Destruktion

EnglishView::EnglishView()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen,

	TRACE("EnglishView::EnglishView() - ANFANG\n");
	ColorText ct;
	ct.color=0;
	ct.str="I and u";
	m_CursorPos=0;
	//m_first=NULL;
	//m_Text.push_back(ct);
	m_bDrawVector=FALSE;
	m_pDC.m_hDC=NULL;
	m_bDCAttached=FALSE;
	m_bCalculateCursorPos=FALSE;
	//m_wndVert.Create(/*WS_CHILD|*/SBS_VERT,CRect(0,0,10,10),this,33000);

	m_bFirstTimeDisplayed=TRUE;
	m_pText=NULL;
	m_dwMarkingStart=0;
	m_dwMarkingEnd=0;
	m_dwStartLine=0;
	m_bMark=FALSE;
	m_bDynamicTextFlow=TRUE;
	m_bDrawCursor=TRUE;
	m_dwNumberOfLines=0;
	m_bInitiallyDisplayed=FALSE;
	m_dwHighestID=ID_USER+100;
	m_bHasFocus=0;
	TRACE("EnglishView::EnglishView() - ENDE\n");
}

EnglishView::~EnglishView()
{
}

/*BOOL EnglishView::PreCreateWindow(CREATESTRUCT& cs)
{
	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	// eigener Code, Anfang
	//m_wndVert.Create(SBS_VERT,CRect(0,0,10,10),this,33000);
	// eigener Code, Ende
	return CView::PreCreateWindow(cs);
}*/

void EnglishView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	//MessageBeep((WORD)-1);
	AfxMessageBox("kk",MB_OK,NULL);
	if(nState==WA_ACTIVE || nState==WA_CLICKACTIVE)
	{
			AfxMessageBox("kann keinen Timer installieren! Das heisst, das es\
keinen blinkenden Cursor in einem oder 2 Textfeld(ern) des aktiven Kindfensters geben wird",MB_OK,NULL);
		int iInstallResult=SetTimer(1,500,NULL);
		if(!iInstallResult)
			AfxMessageBox("kann keinen Timer installieren! Das heisst, das es\
keinen blinkenden Cursor in einem oder 2 Textfeld(ern) des aktiven Kindfensters geben wird",MB_OK,NULL);
	}
	if(nState==WA_INACTIVE)
		KillTimer(1);
}

void EnglishView::OnActivateFrame( UINT nState, CFrameWnd* pFrameWnd )
{
	//MessageBeep((WORD)-1);
	//AfxMessageBox("kk",MB_OK,NULL);
}

void EnglishView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	
	if(bActivate)
	{
			//AfxMessageBox("kann keinen Timer installieren! Das heisst, das es\
//keinen blinkenden Cursor in einem oder 2 Textfeld(ern) des aktiven Kindfensters geben wird",MB_OK,NULL);
		int iInstallResult=SetTimer(1,500,NULL);
		if(!iInstallResult)
			AfxMessageBox("Kann keinen Timer installieren! Das heisst, dass es \
keinen blinkenden Cursor in einem oder 2 Textfeld(ern) des aktiven Kindfensters \
geben wird. Das Beenden aktiver Programme kann Timer freigeben. Starten Sie \
dann diese Anwendung nochmals.",MB_OK,NULL);
	}
	else
	{
		KillTimer(1);
		m_bDrawCursor=TRUE;
		Invalidate(FALSE);
	}
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void EnglishView::OnInitialUpdate() // diese Funktion wird nach
{ // CDocument::OnOpenDocument aufgerufen und kann deshalb benutzt 
	//werden, um die Einstellungen für die Bildlaufleiste zu erledigen,
	//wie z.B. der Scrollbereich oder die Größe des ziehbaren Kästchens
	CRect rect;
	SCROLLINFO si;
	TRACE("void EnglishView::OnInitialUpdate()- ANFANG\n");
#ifdef _DEBUG
	printf("void EnglishView::OnInitialUpdate()- ANFANG\n");
#endif
	GetClientRect(rect);
	if(rect.right>=15)
		rect.left=rect.right-15;
	m_wndVert.DestroyWindow(); // vor dem erneuten Erstellen unbedingt zerstören
	m_bInitiallyDisplayed=TRUE;
	ZeroMemory(&si, sizeof(SCROLLINFO));
  si.cbSize = sizeof(SCROLLINFO);
	si.fMask=SIF_ALL; // ??
 	si.nMin=0; // minimum (int)

	si.nPos=0; // Bildlaufleistenposition auf 0 setzen
	si.nTrackPos=0; // ??
	//si.fMask = SIF_TRACKPOS|SIF_PAGE|SIF_RANGE;
	/* die 32-Bit Position der Bildlaufleiste wird nun in
	si.nTrackPos gespeichert, wenn die Bildlaufleiste das Ereignis SB_THUMBTRACK
	erhält (sonst steht im Parameter nPos der Ereignisfunktion 'OnVScroll' oder
	'OnHScroll' nur eine 16-Bit Position gespeichert, wenn SB_THUMBTRACK auftritt, 
	die nur Werte bis maximal 32667 erlaubt)*/
	/* SIF_PAGE muss angegeben werden, damit das Kästchen der Bildlaufleiste die
		richtige Größe hat*/
	CDC* pDC=GetDC();
	TEXTMETRIC textmetric;
	pDC->GetTextMetrics(&textmetric); // für die Zeilenhöhe
	si.nPage=rect.bottom/textmetric.tmHeight;//si.nMax;
	CVTransDoc* pDoc = GetDocument();
	if(pDoc)
	{
		//TRACE("pDoc!\n");
		//pDoc->m_EnglishText;
		TRACE("pDoc!!!\n");
		//wenn der deutsche Bereich
		if(pDoc->bEnglishText)
		{
			m_wndVert.Create(SBS_VERT,rect,this,IDC_GERMAN_SCROLLVERT); // Stil, Position und Größe, CDialog, ID
			bEnglishPart=FALSE;
			m_bHasFocus=0;
			m_pText=&pDoc->m_GermanText;
			TRACE("m_pText==&pDoc->m_GermanText: %d\n",m_pText==&pDoc->m_GermanText);
			//m_wndVert.Create(SBS_VERT|WS_VISIBLE,rect,this,IDC_GERMAN_SCROLLVERT); // Stil, Position und Größe, CDialog, ID
			pDoc->bEnglishText=FALSE;
		}
		//wenn der englische Bereich
		else
		{
			m_wndVert.Create(SBS_VERT,rect,this,IDC_ENGLISH_SCROLLVERT); // Stil, Position und Größe, CDialog, ID
			CVTransDoc * pDoc=GetDocument();
			bEnglishPart=TRUE;
			m_bHasFocus=1;
			m_pText=&pDoc->m_EnglishText;
			TRACE("m_pText==&pDoc->m_EnglishText: %d\n",m_pText==&pDoc->m_EnglishText);
			//m_wndVert.Create(SBS_VERT|WS_VISIBLE,rect,this,IDC_ENGLISH_SCROLLVERT); // Stil, Position und Größe, CDialog, ID
			pDoc->bEnglishText=TRUE;
			CalculateNumberOfLines();
			if(pDoc)
				si.nMax=m_dwNumberOfLines;
		}
		m_wndVert.SetScrollInfo(&si,TRUE); // Scrollposition
		m_wndVert.ShowScrollBar(TRUE); // Bildlaufleiste nun anzeigen
	}
	else
	{
		TRACE("!pDoc\n");
	}
	//m_wndVert.Create(SBS_VERT|WS_VISIBLE,rect,this,33500); // Stil, Position und Größe, CDialog, ID
	//CalculateNumberOfLines();
	TRACE("void EnglishView::OnInitialUpdate() - ENDE\n");
#ifdef _DEBUG
	printf("void EnglishView::OnInitialUpdate()- ENDE\n");
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CVTransView Zeichnen

void EnglishView::OnDraw(CDC* pDC)
{
	TRACE("void EnglishView::OnDraw(CDC* pDC) - ANFANG\n");
#ifdef _DEBUG
	printf("void EnglishView::OnDraw(CDC* pDC) - ANFANG\n");
	printf("m_cstrvv.size(): %u\n",m_cstrvv.size());
	if(m_pText)
	printf("Adresse von m_pText: %u\n",m_pText);
#endif
	CVTransDoc * pDoc=GetDocument();
	if(pDoc)
	{
		TRACE("pDoc!!!\n");
	}
	else
		TRACE("!pDoc\n");
	if(m_pText)
		TRACE("m_pText!=NULL\n");
	else
		TRACE("m_pText==NULL\n");
	// ZU ERLEDIGEN: Hier Code zum Zeichnen der ursprünglichen Daten hinzufügen

	if(m_bFirstTimeDisplayed)
	{
		m_bFirstTimeDisplayed=FALSE;
		CRect rect;
		GetClientRect(rect);
		if(rect.right>=15)
			rect.left=rect.right-15;
	}
	//CString strTextFace;
	//pDC->GetTextFace(strTextFace);
	//TRACE("strTextFace: %s\n",strTextFace);
	//CSize size=pDC->GetOutputTextExtent("Sports play a very big part in English schools. The boys play football and cricket");
	//TRACE("size.cx: %u\n",size.cx);
	CRect rect;
	GetClientRect(rect);
	if(rect.right>=15)
		rect.left=rect.right-15;
	SCROLLINFO si;
	ZeroMemory(&si, sizeof(SCROLLINFO));
  si.cbSize = sizeof(SCROLLINFO);
	si.fMask=SIF_ALL; // ??
 	si.nMin=0; // minimum (int)
	si.nMax=2;
	si.nPos=0; // Bildlaufleistenposition auf 0 setzen
	si.nTrackPos=0; // ??
	//si.fMask = SIF_TRACKPOS|SIF_PAGE|SIF_RANGE;
	/* die 32-Bit Position der Bildlaufleiste wird nun in
	si.nTrackPos gespeichert, wenn die Bildlaufleiste das Ereignis SB_THUMBTRACK
	erhält (sonst steht im Parameter nPos der Ereignisfunktion 'OnVScroll' oder
	'OnHScroll' nur eine 16-Bit Position gespeichert, wenn SB_THUMBTRACK auftritt, 
	die nur Werte bis maximal 32667 erlaubt)*/
	/* SIF_PAGE muss angegeben werden, damit das Kästchen der Bildlaufleiste die
		richtige Größe hat*/
	si.nPage=0;//si.nMax;
	if(!m_bDCAttached)
	{
		m_pDC.m_hDC=pDC->GetSafeHdc();
		m_pDC.m_hAttribDC=pDC->GetSafeHdc();
		m_bDCAttached=TRUE;
	}

//after:m_bCalculateCursorPos=FALSE;


	TEXTMETRIC textmetric;
	pDC->GetTextMetrics(&textmetric);

	TRACE("m_ColorText.size(): %d\n",m_ColorText.size());
	int line=0,xpos=0;
	//wenn Kombinationsfelder angezeigt werden sollen
	if(m_bDrawVector)
	{
#ifdef _DEBUG
		printf("m_bDrawVector\n");
#endif
		CRect clientRect;
		GetClientRect(clientRect);
		CVTransApp * pApp=(CVTransApp *)AfxGetApp();
		if(pApp)
		{
			CMainFrame * pFrame=(CMainFrame *)pApp->m_pMainWnd;
			if(pFrame)
			{
				if(pFrame->m_bBufferOutput) // die Ausgabe soll gepuffert erfolgen, um das
				{ // Flackern zu verhindern. Der Puffer beansprucht ungefähr
					// "Breite in Pixel"*"Höhe in Pixel"*"3 BYTE", also bei einer
					// Breite von 640 Pixeln und einer Höhe von 480 Pixeln 
					// wahrscheinlich 921600 Bytes; der Speicher, den der Puffer
					// belegt, wird aber noch vor dem Ende von dieser Funktion 
					// (OnDraw() ) freigegeben, genauer gesagt am Ende des Blockes,
					// in dem die Variable (Objekt) der CMemDC-Klasse deklariert wurde.
					CMemDC memDC(pDC,clientRect);
					int ypos=0;
					memDC->PatBlt(0,0,clientRect.right,clientRect.bottom,WHITENESS);
					ComboBoxListElement * pCurrent=NULL;
					pCurrent=m_ComboBoxList.m_pFirst;
					DWORD dwLineHeight=0;
					if(m_ComboBoxList.m_pFirst)
					{
		#ifdef _DEBUG
						printf("m_ComboBoxList.m_pFirst\n");
		#endif
						dwLineHeight=m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+5;
		#ifdef _DEBUG
						printf("dwLineHeight: %u\n",dwLineHeight);
		#endif
					}
					else
					{
		#ifdef _DEBUG
						printf("!m_ComboBoxList.m_pFirst\n");
		#endif
						dwLineHeight=textmetric.tmHeight;
					}
					for(DWORD dwRun=0;dwRun<m_cstrvv.size();++dwRun)
					{
						CStringVector cstrv=m_cstrvv.at(dwRun);
						TRACE("CStringVector cstrv=m_cstrvv.at(dwRun)\n");
						if(cstrv.size()>1) // Vektor hat mehr als 1 Element (vom Typ CString)
						{
							//memDC->SetTextColor(RGB(0,255,0)); // grün
							//CFont font;
							//if(!font.CreateStockObject(DEFAULT_GUI_FONT))
							//	font.CreatePointFont(80, "MS Sans Serif");
							CSize size=memDC->GetOutputTextExtent(cstrv.at(0));
							if(pCurrent)
							{
								TRACE("pCurrent\n");
								DWORD dwCount=10;//pCurrent->ComboBox.GetCount();
								DWORD dwWidth=ypos+size.cy*dwCount+5;
								CRect rect;
								pCurrent->ComboBox.GetWindowRect(rect);
								pCurrent=pCurrent->m_pNext;
								if(xpos+rect.right-rect.left>clientRect.right-15 && 
									rect.right-rect.left<=clientRect.right-15)
								{
									xpos=rect.right-rect.left;
									line++;
								}
								else
									xpos+=rect.right-rect.left;
								if(dwRun<m_cstrvv.size()-1)
								{
									cstrv=m_cstrvv.at(dwRun+1);
									CString str=cstrv.at(0);
									if(str!=","  && str!="." && str!="?")
									{
										size=memDC->GetOutputTextExtent(" ");
										xpos+=size.cx;
									}
								}
							}
							else
								TRACE("!pCurrent\n");
							ypos+=textmetric.tmHeight>size.cy?textmetric.tmHeight:size.cy;
						}
						else // Vektor hat weniger als 1 Element oder 1 Element (vom Typ CString)
						{
							memDC->SetTextColor(RGB(0,0,0)); // schwarz
							TRACE("CStringVector cstrv=m_cstrvv.at(dwRun)\n");
							CSize size=memDC->GetOutputTextExtent(cstrv.at(0));
							CRect clientRect;
							GetClientRect(clientRect);
							if(clientRect.right>=15)
								clientRect.right-=15;
							// wenn die Zeichenkette nicht mehr vollständig in das (Kind-)Fenster passt
							if(xpos+size.cx>clientRect.right && clientRect.right>=size.cx)
							{
								line++; // Zeilennummer um 1 erhöhen
								xpos=0; // horizontale Position auf 0 setzen
							}
							//wenn mindestens ein Kombinationsfeld existiert
							if(m_ComboBoxList.m_pFirst)
								memDC->TextOut(xpos,line*dwLineHeight+dwLineHeight-textmetric.
									tmHeight-3,cstrv.at(0));
							//wenn kein Kombinationsfeld existiert
							else
								memDC->TextOut(xpos,line*dwLineHeight,cstrv.at(0));
							xpos+=size.cx;//outputcharwidth[0];
							//wenn das aktuelle Element nicht das letzte Element im Vektor ist
							if(dwRun<m_cstrvv.size()-1)
							{
								cstrv=m_cstrvv.at(dwRun+1);
								CString str=cstrv.at(0);
								//wenn die Zeichenkette kein Komma, Punkt oder Fragezeichen ist,
								//...
								if(str!=_T(",")  && str!=_T(".") && str!=_T("?"))
								{
									//...dann die Breite eines Leerzeichen zwischen aktuellem und
									//nächstem Element
									size=memDC->GetOutputTextExtent(" ");
									xpos+=size.cx;
								}
							}
						}
		#ifdef _DEBUG
						printf("cstrv.at(0): %s\n",cstrv.at(0));
		#endif
					}
				}
				else // die Ausgabe soll NICHT gepuffert werden
				{
					pDC->PatBlt(0,0,clientRect.right,clientRect.bottom,WHITENESS);
					ComboBoxListElement * pCurrent=NULL;
					pCurrent=m_ComboBoxList.m_pFirst;
					DWORD dwLineHeight=0;
					if(m_ComboBoxList.m_pFirst)
					{
		#ifdef _DEBUG
						printf("m_ComboBoxList.m_pFirst\n");
		#endif
						dwLineHeight=m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+5;
		#ifdef _DEBUG
						printf("dwLineHeight: %u\n",dwLineHeight);
		#endif
					}
					else
					{
		#ifdef _DEBUG
						printf("!m_ComboBoxList.m_pFirst\n");
		#endif
						dwLineHeight=textmetric.tmHeight;
					}
					int ypos=0;
					for(DWORD dwRun=0;dwRun<m_cstrvv.size();dwRun++)
					{
						CStringVector cstrv=m_cstrvv.at(dwRun);
						TRACE("CStringVector cstrv=m_cstrvv.at(dwRun)\n");
						if(cstrv.size()>1) // Vektor hat mehr als 1 Element (vom Typ CString)
						{
							pDC->SetTextColor(RGB(0,255,0)); // grün
							CFont font;
							if(!font.CreateStockObject(DEFAULT_GUI_FONT))
								font.CreatePointFont(80, "MS Sans Serif");
							CSize size=pDC->GetOutputTextExtent(cstrv.at(0));
							if(pCurrent)
							{
								TRACE("pCurrent\n");
								DWORD dwCount=10;//pCurrent->ComboBox.GetCount();
								DWORD dwWidth=ypos+size.cy*dwCount+5;
								CRect rect;
								pCurrent->ComboBox.GetWindowRect(rect);
								pCurrent=pCurrent->m_pNext;
								if(xpos+rect.right-rect.left>clientRect.right-15 && 
									rect.right-rect.left<=clientRect.right-15)
								{
									xpos=rect.right-rect.left;
									line++;
								}
								else
									xpos+=rect.right-rect.left;
								if(dwRun<m_cstrvv.size()-1)
								{
									cstrv=m_cstrvv.at(dwRun+1);
									CString str=cstrv.at(0);
									if(str!=","  && str!="." && str!="?")
									{
										size=pDC->GetOutputTextExtent(" ");
										xpos+=size.cx;
									}
								}
							}
							else
								TRACE("!pCurrent\n");
							ypos+=textmetric.tmHeight>size.cy?textmetric.tmHeight:size.cy;
						}
						else // Vektor hat weniger als 1 Element oder 1 Element (vom Typ CString)
						{
							pDC->SetTextColor(RGB(0,0,0)); // schwarz
							TRACE("CStringVector cstrv=m_cstrvv.at(dwRun)\n");
							CSize size=pDC->GetOutputTextExtent(cstrv.at(0));
							CRect clientRect;
							GetClientRect(clientRect);
							if(clientRect.right>=15)
								clientRect.right-=15;
							if(xpos+size.cx>clientRect.right && clientRect.right>=size.cx)
							{ // wenn die Zeichenkette nicht mehr vollständig in das (Kind-)Fenster passt
								line++; // Zeilennummer um 1 erhöhen
								xpos=0; // horizontale Position auf 0 setzen
							}
							if(m_ComboBoxList.m_pFirst)
								pDC->TextOut(xpos,line*dwLineHeight+dwLineHeight-textmetric.tmHeight-3,cstrv.at(0));
							else
								pDC->TextOut(xpos,line*dwLineHeight,cstrv.at(0));
							xpos+=size.cx;//outputcharwidth[0];
							if(dwRun<m_cstrvv.size()-1)
							{
								cstrv=m_cstrvv.at(dwRun+1);
								CString str=cstrv.at(0);
								if(str!=","  && str!="." && str!="?")
								{
									size=pDC->GetOutputTextExtent(" ");
									xpos+=size.cx;
								}
							}
						}
		#ifdef _DEBUG
						printf("cstrv.at(0): %s\n",cstrv.at(0));
		#endif
					}
				}
			}
		}
	}
	//wenn KEINE Kombinationsfelder angezeigt werden sollen
	else
	{
		CVTransApp * pApp=(CVTransApp *)AfxGetApp();
		//Anzahl der sichtbaren Zeilen
		DWORD dwNumberOfVisibleLines=rect.bottom/textmetric.tmHeight;
		//Anzahl der Zeilen, die gezeichnet wurden
		DWORD dwLinesDrawn=0;
#ifdef _DEBUG
		printf("dwNumberOfVisibleLines: %u\n",dwNumberOfVisibleLines);
#endif
		if(pApp)
		{
			CMainFrame * pFrame=(CMainFrame *)pApp->m_pMainWnd;
			if(pFrame)
			{
#ifdef _DEBUG
				printf("!m_bDrawVector\n");
				{
					DWORD dwM_pTextLength=m_pText->GetLength();
					TextNode * pTnCurrent=NULL;
					if(m_pText->m_first)
						pTnCurrent=m_pText->m_first;
					for(DWORD dwRun=0;dwRun<dwM_pTextLength;++dwRun)
					{
						//printf("pTnCurrent->ch: %c\n",pTnCurrent->ch);
						pTnCurrent=pTnCurrent->m_next;
					}
				}
		#endif
				//pFrame->m_bBufferOutput=TRUE;
				// die Ausgabe soll gepuffert erfolgen, um das
				// Flackern zu verhindern. Der Puffer beansprucht ungefähr
				// "Breite in Pixel"*"Höhe in Pixel"*"3 BYTE", also bei einer
				// Breite von 640 Pixeln und einer Höhe von 480 Pixeln 
				// wahrscheinlich 921600 Bytes; der Speicher, den der Puffer
				// belegt, wird aber noch vor dem Ende von dieser Funktion 
				// (OnDraw() ) freigegeben, genauer gesagt am Ende des Blockes,
				// in dem die Variable (Objekt) der CMemDC-Klasse deklariert wurde.
				// die Zeichen-Operationen wirken auf den Arbeitsspeicher (memDC); 
				// erst zuletzt wenn der Destruktor von memDC aufgerufen wurde, 
				// wird der Arbeitsspeicher-Inhalt auf den Bildschirm 
				// übertragen
				if(pFrame->m_bBufferOutput)
				{
					CRect clientRect;
					GetClientRect(&clientRect);
					CMemDC memDC(pDC,clientRect);
					//memDC->FillSolidRect(0,0,clientRect.Width(),clientRect.Height(),RGB(255,255,255));
					memDC->PatBlt(0,0,clientRect.right,clientRect.bottom,WHITENESS);
					// PatBlt soll extrem schnell sein, man kann aber nur beim Füllen
					// mit schwarz oder weiss nehmen
					// als Breite muss die volle Breite des Clientbereiches angegeben
					// werden, damit während der Neuzeichnen-Phase kein Grafikmüll an
					// der Position der Bildlaufleiste angezeigt wird
					DWORD dwHorizontalOffset=0;
					DWORD dwLine=0;
					TRACE("nicht drawvector\n");
					DWORD pos=0;
					if(m_pText)
						if(m_pText->m_first)
						{
							//gibt an, ob eine neue Cursorposition zugewiesen wurde
							BYTE bNewCursorPositionAssigned=0;
							BYTE bNewLine;
							//der vorherige Durchlauf erzeugte eine neue Zeile
							BYTE bPreviousPassWasNewLine=0;
							CSize size;
							CString strCurrentToken;
							CString strLine;
							DWORD dwMarkingBegin=m_dwMarkingStart<=m_dwMarkingEnd?
								m_dwMarkingStart:m_dwMarkingEnd;
							DWORD dwMarkingEnd=m_dwMarkingStart>=m_dwMarkingEnd?
								m_dwMarkingStart:m_dwMarkingEnd;
							DWORD dwPosition=0;
							DWORD dwPositionOfBeginOfCurrentLine=0;
							DWORD dwLineNumber=0;
							//die erste Zeile, die sichtbar ist: wenn die Bildlaufleiste nach 
							//unten gescrollt wird, ist Zeilennummer der ersten sichtbare Zeile 
							//höher als die Zeilennummer der ersten Zeile
							DWORD dwFirstVisibleLine=m_dwStartLine;
							DWORD dwNewLine=0;
							TextNode * pTextNodeCurrent=m_pText->m_first;
							TextNode * pTextNodeBeginOfLine=m_pText->m_first;
							TextNode * pTextNodeBeginOfLastToken=m_pText->m_first;
							// Zeichen, die sich vor der ersten
							// sichtbaren Zeile befinden, zählen.
							//sowohl die Zeichenfolge "carriage return" "newline" ("13" "10") als
							//auch eine einzelne "13" leiten eine neue Zeile ein.
							//aber auch wenn ein Token nicht mehr in eine Zeile passt, aber in die
							//Nächste oder ein Token so lang ist, dass es noch nicht mal in eine
							//leere Zeile passt, wird eine neue Zeile eingeleitet
							while(dwLinesDrawn<dwNumberOfVisibleLines)
							{
								bNewLine=0;
								if(bPreviousPassWasNewLine)
								{
									strLine="";
									bPreviousPassWasNewLine=0;
								}
								//wenn carriage return...
								if(pTextNodeCurrent->ch==13)
								{
									bPreviousPassWasNewLine=1;
									//eine neue Zeile beginnt bei der horizontalen Position 
									//(Offset) 0
									dwHorizontalOffset=0;
									bNewLine=1;
									if(dwFirstVisibleLine)
										--dwFirstVisibleLine;
									else
									{
										CSize size;
										DWORD dwRunInsideOutputLoop=0;
										//alle Zeichen vom Anfang der Zeile bis ausschliesslich
										//dem aktuellen Zeichen (13/newline) ausgeben
										while(pTextNodeBeginOfLine!=pTextNodeCurrent)
										{
											//wenn sich das Zeichen innerhalb des markierten 
											//Bereichs liegt...
											if(m_bMark && dwPositionOfBeginOfCurrentLine+
												dwRunInsideOutputLoop>=dwMarkingBegin && 
												dwPositionOfBeginOfCurrentLine+
												dwRunInsideOutputLoop<dwMarkingEnd)
											{
												//...dann soll die Textfarbe weiss sein (also
												//invertiert)
												memDC->SetTextColor(RGB(255,255,255));
												memDC->SetBkColor(RGB(0,0,0)); // Hintergrund schwarz
	#ifdef _DEBUG
												//printf("Hintergrund schwarz\n");
	#endif
											}
											//wenn sich das Zeichen ausserhalb des markierten 
											//Bereichs liegt...
											else
											{
												memDC->SetBkColor(RGB(255,255,255)); // Hintergrund weiss
	#ifdef _DEBUG	
												//printf("Hintergrund weiss\n");
	#endif
												if(pTextNodeBeginOfLine->color==0)
												{
													//Textfarbe: schwarz
													memDC->SetTextColor(RGB(0,0,0));
	#ifdef _DEBUG	
													printf("Textfarbe schwarz\n");
	#endif
												}
												if(pTextNodeBeginOfLine->color==1)
												{
													//Textfarbe: rot
													memDC->SetTextColor(RGB(255,0,0));
	#ifdef _DEBUG	
													printf("Textfarbe rot\n");
	#endif
												}
												if(pTextNodeBeginOfLine->color==2)
												{
													//Textfarbe: blau
													memDC->SetTextColor(RGB(0,0,255));
	#ifdef _DEBUG	
													printf("Textfarbe rot\n");
	#endif
												}
											}
											//1 Zeichen ausgeben
											memDC->TextOut(
                        dwHorizontalOffset,dwLinesDrawn*
												textmetric.tmHeight,
                        //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                        //is faster than creating a CString(pTextNodeBeginOfLine->ch) in order to use 
                        //GetOutputTextExtent(const CString& str ) const; .
                        //&((TCHAR)pTextNodeBeginOfLine->ch),1
                        CString((TCHAR)pTextNodeBeginOfLine->ch));
											//wenn die aktuelle Ausgabeposition der Cursorposition
											//entspricht und der Cursor gezeichnet werden soll
											//Hinweis: die Prüfung "dwPositionOfBeginOfCurrentLine+
											//dwRunInsideOutputLoop++ == m_CursorPos" muss vor 
											//der Prüfung, ob m_bDrawCursor TRUE ist erfolgen,
											//da sonst schon nach der Prüfung, ob m_bDrawCursor 
											//TRUE ist, abgebrochen wird und der markierte Text 
											//nur angezeigt wird, wenn m_bDrawCursor TRUE ist.
											if(dwPositionOfBeginOfCurrentLine+
												dwRunInsideOutputLoop++==m_CursorPos && m_bDrawCursor)
											{
												CPen pen;
												memDC->MoveTo(dwHorizontalOffset,dwLinesDrawn*
													textmetric.tmHeight);//xpos,line*textmetric.tmHeight);
												if(m_bMark && m_dwMarkingEnd<m_dwMarkingStart  && 
													m_CursorPos==m_dwMarkingEnd)
												{
													pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
													memDC->SelectObject(pen);
												}
												//Text-Cursor zeichnen
												memDC->LineTo(dwHorizontalOffset,(dwLinesDrawn+1)*
													textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
												pen.DeleteObject();
												//Vordergrundfarbe auf schwarz setzen
												pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	#ifdef _DEBUG
												printf("1. LineTo\n");
	#endif
											}
											memDC->SetBkColor(RGB(0,0,0));
											size=pDC->GetOutputTextExtent(
                        //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                        //is faster than creating a CString(current->ch) in order to use 
                        //GetOutputTextExtent(const CString& str ) const; .
                        //&(CHAR)pTextNodeBeginOfLine->ch,1
                        CString((TCHAR)pTextNodeBeginOfLine->ch));
											dwHorizontalOffset+=size.cx;
											//if(pOutputCharacter->m_next)
											pTextNodeBeginOfLine=pTextNodeBeginOfLine->m_next;
											//else
											//	break;
											//dwPositionInsideToken++;
											//dwRunInsideOutputLoop++;
										}
										if(dwPositionOfBeginOfCurrentLine+
											dwRunInsideOutputLoop==m_CursorPos && m_bDrawCursor)
										{
											CPen pen;
											memDC->MoveTo(dwHorizontalOffset,dwLinesDrawn*
												textmetric.tmHeight);//xpos,line*textmetric.tmHeight);
											if(m_bMark && m_dwMarkingEnd<m_dwMarkingStart  && 
												m_CursorPos==m_dwMarkingEnd)
											{
												pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
												memDC->SelectObject(pen);
											}
											memDC->LineTo(dwHorizontalOffset,(dwLinesDrawn+1)*
												textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
											pen.DeleteObject();
											//Vordergrundfarbe auf schwarz setzen
											pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
#ifdef _DEBUG
											printf("1. LineTo\n");
#endif
										}
										//...dann Zeilenindex um 1 erhöhen
										++dwLineNumber;
										++dwLinesDrawn;
									}
									pTextNodeBeginOfLine=pTextNodeCurrent->m_next;
									if(pTextNodeCurrent->m_next)
										//...und das nächste Zeichen newline...
										if(pTextNodeCurrent->m_next->ch==10)
										{
											pTextNodeCurrent=pTextNodeCurrent->m_next;
											pTextNodeBeginOfLine=pTextNodeCurrent->m_next;
										}
									//auch wenn die Zeichen "13" "10" hintereinander stehen
									//zählt das nur als 1 Position
									dwPositionOfBeginOfCurrentLine=dwPosition+1;
								}
								//wenn Leerzeichen
								if(pTextNodeCurrent->ch==' ')
								{
					#ifdef _DEBUG
									printf("current->ch==' '\n");
					#endif
									//Zeichenkette leeren
									strCurrentToken="";
									//++dwLineNumber;
									//dwHorizontalOffset=0;
									strLine+=pTextNodeCurrent->ch;
									//dwPositionOfEndOfPreviousLine=dwNewCursorPosition;
									pTextNodeBeginOfLastToken=pTextNodeCurrent->m_next;
								}
								//wenn Zeichen ausser Leerzeichen
								else
								{
									//wenn nicht Zeichenfolge "13" "10" oder Zeichen "10"
									if(!bNewLine)
									{
										//Zeichen an die Zeichenfolge anhängen
										strCurrentToken+=pTextNodeCurrent->ch;
										//Zeichen an die Zeichenfolge anhängen
										strLine+=pTextNodeCurrent->ch;
									}
								}
								size=pDC->GetOutputTextExtent(strLine);
					#ifdef _DEBUG
								printf("size.cx: %u\n",size.cx);
					#endif
								//wenn ein automatischer Zeilenumbruch stattfindet, weil sonst
								//die Zeile über die vertikale Bildlaufleiste hinaus ginge
								//wenn die Breite in Pixel der Zeile größer ist als die Breite des
								//Clientbereiches minus der Breite in Pixel der Bildlaufleiste (15)
								//minus der Breite in Pixel für einen möglichen Cursor (1)
								if(size.cx>clientRect.right-16)
								{
									CSize sizeCurrentToken=pDC->GetOutputTextExtent(strCurrentToken);
									dwHorizontalOffset=0;
					#ifdef _DEBUG
									printf("size.cx>clientRect.right-16\n");
					#endif
									//wenn das letzte Token in eine Zeile passt
									//(wenn in der aktuellen Zeile KEIN(E) Leerzeichen ist/
									//sind, passt das aktuelle Token NICHT in eine Zeile)
									//wenn die Breite in Pixel der aktuellen Zeichenkette des 
									//aktuellen Token kleiner gleich ist als die Breite des
									//Clientbereiches minus der Breite in Pixel der 
									//Bildlaufleiste (15) minus der Breite in Pixel für einen 
									//möglichen Cursor (1)
									if(sizeCurrentToken.cx<=clientRect.right-16)
									{
										if(!dwFirstVisibleLine)
										{
											CSize size;
											DWORD dwRunInsideOutputLoop=0;
						#ifdef _DEBUG
											printf("sizeCurrentToken.cx<=clientRect.right-16\n");
						#endif
											strLine=strCurrentToken;
											while(pTextNodeBeginOfLine!=pTextNodeBeginOfLastToken)
											{
												//wenn sich das Zeichen innerhalb des markierten 
												//Bereichs liegt...
												if(m_bMark && dwPositionOfBeginOfCurrentLine+
													dwRunInsideOutputLoop>=dwMarkingBegin && 
													dwPositionOfBeginOfCurrentLine+
													dwRunInsideOutputLoop<dwMarkingEnd)
												{
													//...dann soll die Textfarbe weiss sein (also
													//invertiert)
													memDC->SetTextColor(RGB(255,255,255));
													memDC->SetBkColor(RGB(0,0,0)); // Hintergrund schwarz
		#ifdef _DEBUG
													printf("Hintergrund schwarz\n");
		#endif
												}
												//wenn sich das Zeichen ausserhalb des markierten 
												//Bereichs liegt...
												else
												{
													memDC->SetBkColor(RGB(255,255,255)); // Hintergrund weiss
		#ifdef _DEBUG
													//printf("Hintergrund weiss\n");
		#endif
													if(pTextNodeBeginOfLine->color==0)
													{
														//Textfarbe: schwarz
														memDC->SetTextColor(RGB(0,0,0));
		#ifdef _DEBUG
														//printf("Textfarbe schwarz\n");
		#endif
													}
													if(pTextNodeBeginOfLine->color==1)
													{
														//Textfarbe: rot
														memDC->SetTextColor(RGB(255,0,0));
		#ifdef _DEBUG
														printf("Textfarbe rot\n");
		#endif
													}
													if(pTextNodeBeginOfLine->color==2)
													{
														//Textfarbe: blau
														memDC->SetTextColor(RGB(0,0,255));
		#ifdef _DEBUG
														printf("Textfarbe rot\n");
		#endif					
													}
												}
												memDC->TextOut(dwHorizontalOffset,dwLinesDrawn*
													textmetric.tmHeight,
                          //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                          //is faster than creating a CString(current->ch) in order to use 
                          //GetOutputTextExtent(const CString& str ) const; .
                          //&(CHAR)pTextNodeBeginOfLine->ch,1
                          CString((TCHAR)pTextNodeBeginOfLine->ch));
												//wenn die aktuelle Ausgabeposition der Cursorposition
												//entspricht und der Cursor gezeichnet werden soll
												//Hinweis: die Prüfung "dwPositionAfterOutput+
												//dwRunInsideOutputLoop++ == m_CursorPos" muss vor 
												//der Prüfung, ob m_bDrawCursor TRUE ist erfolgen,
												//da sonst schon nach der Prüfung, ob m_bDrawCursor 
												//TRUE ist, abgebrochen wird und der markierte Text 
												//nur angezeigt wird, wenn m_bDrawCursor TRUE ist.
												if(dwPositionOfBeginOfCurrentLine+dwRunInsideOutputLoop++==
													m_CursorPos && m_bDrawCursor)
												{
													CPen pen;
													memDC->MoveTo(dwHorizontalOffset,dwLinesDrawn*
														textmetric.tmHeight);//xpos,line*textmetric.tmHeight);
													if(m_bMark && m_dwMarkingEnd<m_dwMarkingStart  && 
														m_CursorPos==m_dwMarkingEnd)
													{
														pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
														memDC->SelectObject(pen);
													}
													memDC->LineTo(dwHorizontalOffset,(dwLinesDrawn+1)*
														textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
													pen.DeleteObject();
													//Vordergrundfarbe auf schwarz setzen
													pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
		#ifdef _DEBUG
													printf("1. LineTo\n");
		#endif
												}
												memDC->SetBkColor(RGB(0,0,0));
												size=pDC->GetOutputTextExtent(
                          //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                          //is faster than creating a CString(current->ch) in order to use 
                          //GetOutputTextExtent(const CString& str ) const; .
                          //&(CHAR)pTextNodeBeginOfLine->ch,1
                          CString((TCHAR)pTextNodeBeginOfLine->ch));
												dwHorizontalOffset+=size.cx;
												//if(pOutputCharacter->m_next)
												pTextNodeBeginOfLine=pTextNodeBeginOfLine->m_next;
												//else
												//	break;
												//dwPositionInsideToken++;
												//dwRunInsideOutputLoop++;
											}
										}
										pTextNodeBeginOfLine=pTextNodeBeginOfLastToken;
										//dwPositionOfBeginOfCurrentLine=dwPosition-strCurrentToken.
										//	GetLength();
										dwPositionOfBeginOfCurrentLine=dwPosition-strCurrentToken.
											GetLength()+1;
										strLine=strCurrentToken;
									}
									//wenn das letzte Token NICHT in eine Zeile passt
									else
									{
										if(!dwFirstVisibleLine)
										{
											CSize size;
											DWORD dwRunInsideOutputLoop=0;
											while(pTextNodeBeginOfLine!=pTextNodeCurrent)
											{
												//wenn sich das Zeichen innerhalb des markierten 
												//Bereichs liegt...
												if(m_bMark && dwPositionOfBeginOfCurrentLine+
													dwRunInsideOutputLoop>=dwMarkingBegin && 
													dwPositionOfBeginOfCurrentLine+
													dwRunInsideOutputLoop<dwMarkingEnd)
												{
													//...dann soll die Textfarbe weiss sein (also
													//invertiert)
													memDC->SetTextColor(RGB(255,255,255));
													memDC->SetBkColor(RGB(0,0,0)); // Hintergrund schwarz
		#ifdef _DEBUG
													printf("Hintergrund schwarz\n");
		#endif
												}
												//wenn sich das Zeichen ausserhalb des markierten 
												//Bereichs liegt...
												else
												{
													memDC->SetBkColor(RGB(255,255,255)); // Hintergrund weiss
		#ifdef _DEBUG
													//printf("Hintergrund weiss\n");
		#endif
													if(pTextNodeBeginOfLine->color==0)
													{
														//Textfarbe: schwarz
														memDC->SetTextColor(RGB(0,0,0));
		#ifdef _DEBUG
														//printf("Textfarbe schwarz\n");
		#endif
													}
													if(pTextNodeBeginOfLine->color==1)
													{
														//Textfarbe: rot
														memDC->SetTextColor(RGB(255,0,0));
		#ifdef _DEBUG
														printf("Textfarbe rot\n");
		#endif
													}
													if(pTextNodeBeginOfLine->color==2)
													{
														//Textfarbe: blau
														memDC->SetTextColor(RGB(0,0,255));
		#ifdef _DEBUG
														printf("Textfarbe rot\n");
		#endif
													}
												}
												memDC->TextOut(dwHorizontalOffset,dwLinesDrawn*
													textmetric.tmHeight,
                          //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                          //is faster than creating a CString(current->ch) in order to use 
                          //GetOutputTextExtent(const CString& str ) const; .
                          //&(CHAR)pTextNodeBeginOfLine->ch,1
                          CString((TCHAR)pTextNodeBeginOfLine->ch));
												//wenn die aktuelle Ausgabeposition der Cursorposition
												//entspricht und der Cursor gezeichnet werden soll
												//Hinweis: die Prüfung "dwPositionAfterOutput+
												//dwRunInsideOutputLoop++ == m_CursorPos" muss vor 
												//der Prüfung, ob m_bDrawCursor TRUE ist erfolgen,
												//da sonst schon nach der Prüfung, ob m_bDrawCursor 
												//TRUE ist, abgebrochen wird und der markierte Text 
												//nur angezeigt wird, wenn m_bDrawCursor TRUE ist.
												if(dwPositionOfBeginOfCurrentLine+dwRunInsideOutputLoop++==
													m_CursorPos && m_bDrawCursor)
												{
													CPen pen;
													memDC->MoveTo(dwHorizontalOffset,dwLinesDrawn*
														textmetric.tmHeight);//xpos,line*textmetric.tmHeight);
													if(m_bMark && m_dwMarkingEnd<m_dwMarkingStart  && 
														m_CursorPos==m_dwMarkingEnd)
													{
														pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
														memDC->SelectObject(pen);
													}
													memDC->LineTo(dwHorizontalOffset,(dwLinesDrawn+1)*
														textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
													pen.DeleteObject();
													//Vordergrundfarbe auf schwarz setzen
													pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
		#ifdef _DEBUG
													printf("1. LineTo\n");
		#endif
												}
												memDC->SetBkColor(RGB(0,0,0));
												size=pDC->GetOutputTextExtent(
                          //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                          //is faster than creating a CString(current->ch) in order to use 
                          //GetOutputTextExtent(const CString& str ) const; .
                          //&(CHAR)pTextNodeBeginOfLine->ch,1
                          CString((TCHAR)pTextNodeBeginOfLine->ch));
												dwHorizontalOffset+=size.cx;
												//if(pOutputCharacter->m_next)
												pTextNodeBeginOfLine=pTextNodeBeginOfLine->m_next;
												//else
												//	break;
												//dwPositionInsideToken++;
												//dwRunInsideOutputLoop++;
											}
										}
										strLine=CString((TCHAR)pTextNodeCurrent->ch);
										strCurrentToken=CString((TCHAR)(pTextNodeCurrent->ch));
										pTextNodeBeginOfLine=pTextNodeCurrent;
										dwPositionOfBeginOfCurrentLine=dwPosition;
										//dwPositionOfBeginOfCurrentLine=dwPosition+1;
									}
									if(dwFirstVisibleLine)
										--dwFirstVisibleLine;
									else
									{
										++dwLineNumber;
										++dwLinesDrawn;
									}
									size=pDC->GetOutputTextExtent(strLine);
								}
								++dwPosition;
								if(pTextNodeCurrent->m_next)
									pTextNodeCurrent=pTextNodeCurrent->m_next;
								//wenn das Ende der verketteten Liste erreicht ist
								else
								{
									if(!dwFirstVisibleLine)
									{
										CSize size;
										DWORD dwRunInsideOutputLoop=0;
										dwHorizontalOffset=0;
										while(pTextNodeBeginOfLine!=NULL)
										{
											//wenn sich das Zeichen innerhalb des markierten 
											//Bereichs liegt...
											if(m_bMark && dwPositionOfBeginOfCurrentLine+
												dwRunInsideOutputLoop>=dwMarkingBegin && 
												dwPositionOfBeginOfCurrentLine+
												dwRunInsideOutputLoop<dwMarkingEnd)
											{
												//...dann soll die Textfarbe weiss sein (also
												//invertiert)
												memDC->SetTextColor(RGB(255,255,255));
												memDC->SetBkColor(RGB(0,0,0)); // Hintergrund schwarz
	#ifdef _DEBUG
												printf("Hintergrund schwarz\n");
	#endif
											}
											//wenn sich das Zeichen ausserhalb des markierten 
											//Bereichs liegt...
											else
											{
												memDC->SetBkColor(RGB(255,255,255)); // Hintergrund weiss
	#ifdef _DEBUG
												//printf("Hintergrund weiss\n");
	#endif
												if(pTextNodeBeginOfLine->color==0)
												{
													//Textfarbe: schwarz
													memDC->SetTextColor(RGB(0,0,0));
	#ifdef _DEBUG
													//printf("Textfarbe schwarz\n");
	#endif
												}
												if(pTextNodeBeginOfLine->color==1)
												{
													//Textfarbe: rot
													memDC->SetTextColor(RGB(255,0,0));
	#ifdef _DEBUG
													printf("Textfarbe rot\n");
	#endif
												}
												if(pTextNodeBeginOfLine->color==2)
												{
													//Textfarbe: blau
													memDC->SetTextColor(RGB(0,0,255));
	#ifdef _DEBUG
													printf("Textfarbe rot\n");
	#endif
												}
											}
											//1 Zeichen ausgeben
											memDC->TextOut(dwHorizontalOffset,dwLinesDrawn*
												textmetric.tmHeight,
                        //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                        //is faster than creating a CString(current->ch) in order to use 
                        //GetOutputTextExtent(const CString& str ) const; .
                        //&(CHAR)pTextNodeBeginOfLine->ch,1
                        CString((TCHAR)pTextNodeBeginOfLine->ch));
											//wenn die aktuelle Ausgabeposition der Cursorposition
											//entspricht und der Cursor gezeichnet werden soll
											//Hinweis: die Prüfung "dwPositionAfterOutput+
											//dwRunInsideOutputLoop++ == m_CursorPos" muss vor 
											//der Prüfung, ob m_bDrawCursor TRUE ist erfolgen,
											//da sonst schon nach der Prüfung, ob m_bDrawCursor 
											//TRUE ist, abgebrochen wird und der markierte Text 
											//nur angezeigt wird, wenn m_bDrawCursor TRUE ist.
											if(dwPositionOfBeginOfCurrentLine+dwRunInsideOutputLoop++==
												m_CursorPos && m_bDrawCursor)
											{
												CPen pen;
												memDC->MoveTo(dwHorizontalOffset,dwLinesDrawn*
													textmetric.tmHeight);//xpos,line*textmetric.tmHeight);
												if(m_bMark && m_dwMarkingEnd<m_dwMarkingStart  && 
													m_CursorPos==m_dwMarkingEnd)
												{
													pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
													memDC->SelectObject(pen);
												}
												memDC->LineTo(dwHorizontalOffset,(dwLinesDrawn+1)*
													textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
												pen.DeleteObject();
												//Vordergrundfarbe auf schwarz setzen
												pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	#ifdef _DEBUG
												printf("1. LineTo\n");
	#endif
											}
											memDC->SetBkColor(RGB(0,0,0));
											size=pDC->GetOutputTextExtent(
                        //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                        //is faster than creating a CString(current->ch) in order to use 
                        //GetOutputTextExtent(const CString& str ) const; .
                        //&(CHAR)pTextNodeBeginOfLine->ch,1
                        CString((TCHAR)pTextNodeBeginOfLine->ch));
											dwHorizontalOffset+=size.cx;
											//if(pOutputCharacter->m_next)
											pTextNodeBeginOfLine=pTextNodeBeginOfLine->m_next;
										//else
										//	break;
										//dwPositionInsideToken++;
										//dwRunInsideOutputLoop++;
										}
										//WENN SICH DER CURSOR AN DER LETZTEN POSITION BEFINDET UND
										//wenn die aktuelle Ausgabeposition der Cursorposition
										//entspricht und der Cursor gezeichnet werden soll
										//Hinweis: die Prüfung "dwPositionAfterOutput+
										//dwRunInsideOutputLoop++ == m_CursorPos" muss vor 
										//der Prüfung, ob m_bDrawCursor TRUE ist erfolgen,
										//da sonst schon nach der Prüfung, ob m_bDrawCursor 
										//TRUE ist, abgebrochen wird und der markierte Text 
										//nur angezeigt wird, wenn m_bDrawCursor TRUE ist.
										if(dwPositionOfBeginOfCurrentLine+dwRunInsideOutputLoop==
											m_CursorPos && m_bDrawCursor)
										{
											CPen pen;
											memDC->MoveTo(dwHorizontalOffset,dwLinesDrawn*
												textmetric.tmHeight);//xpos,line*textmetric.tmHeight);
											if(m_bMark && m_dwMarkingEnd<m_dwMarkingStart  && 
												m_CursorPos==m_dwMarkingEnd)
											{
												pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
												memDC->SelectObject(pen);
											}
											memDC->LineTo(dwHorizontalOffset,(dwLinesDrawn+1)*
												textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
											pen.DeleteObject();
											//Vordergrundfarbe auf schwarz setzen
											pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
#ifdef _DEBUG
											printf("1. LineTo\n");
#endif
										}
									}
									break;
								}
							}
						}// if(m_pText->m_first)
						//wenn keine Zeichen im Text sind
						else // m_pText->m_first==NULL
						{
							if(m_bDrawCursor)
							{
								memDC->MoveTo(0,0);//xpos,line*textmetric.tmHeight);
								memDC->LineTo(0,textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
			#ifdef _DEBUG
								printf("7. LineTo\n");
			#endif
								memDC->SetBkColor(RGB(0,0,0));
							}
						}
				}
				//dieser Teil wird ausgeführt, wenn die Ausgabe nicht
				//gepuffert werden soll.
				else
				{
					CRect clientRect;
					GetClientRect(&clientRect);
					// PatBlt soll extrem schnell sein, man kann aber nur beim Füllen
					// mit schwarz oder weiss nehmen
					// als Breite muss die volle Breite des Clientbereiches angegeben
					// werden, damit während der Neuzeichnen-Phase kein Grafikmüll an
					// der Position der Bildlaufleiste angezeigt wird
					pDC->PatBlt(0,0,clientRect.right,clientRect.bottom,WHITENESS);
					DWORD dwHorizontalOffset=0;
					DWORD dwLine=0;
					TRACE("nicht drawvector\n");
					DWORD pos=0;
					if(m_pText)
						if(m_pText->m_first)
						{
							//gibt an, ob eine neue Cursorposition zugewiesen wurde
							BYTE bNewCursorPositionAssigned=0;
							BYTE bNewLine;
							//der vorherige Durchlauf erzeugte eine neue Zeile
							BYTE bPreviousPassWasNewLine=0;
							CSize size;
							CString strCurrentToken;
							CString strLine;
							DWORD dwMarkingBegin=m_dwMarkingStart<=m_dwMarkingEnd?
								m_dwMarkingStart:m_dwMarkingEnd;
							DWORD dwMarkingEnd=m_dwMarkingStart>=m_dwMarkingEnd?
								m_dwMarkingStart:m_dwMarkingEnd;
							DWORD dwPosition=0;
							DWORD dwPositionOfBeginOfCurrentLine=0;
							DWORD dwLineNumber=0;
							//die erste Zeile, die sichtbar ist: wenn die Bildlaufleiste nach 
							//unten gescrollt wird, ist Zeilennummer der ersten sichtbare Zeile 
							//höher als die Zeilennummer der ersten Zeile
							DWORD dwFirstVisibleLine=m_dwStartLine;
							DWORD dwNewLine=0;
							TextNode * pTextNodeCurrent=m_pText->m_first;
							TextNode * pTextNodeBeginOfLine=m_pText->m_first;
							TextNode * pTextNodeBeginOfLastToken=m_pText->m_first;
							// Zeichen, die sich vor der ersten
							// sichtbaren Zeile befinden, zählen.
							//sowohl die Zeichenfolge "carriage return" "newline" ("13" "10") als
							//auch eine einzelne "13" leiten eine neue Zeile ein.
							//aber auch wenn ein Token nicht mehr in eine Zeile passt, aber in die
							//Nächste oder ein Token so lang ist, dass es noch nicht mal in eine
							//leere Zeile passt, wird eine neue Zeile eingeleitet
							while(dwLinesDrawn<dwNumberOfVisibleLines)
							{
								bNewLine=0;
								if(bPreviousPassWasNewLine)
								{
									strLine="";
									bPreviousPassWasNewLine=0;
								}
								//wenn carriage return...
								if(pTextNodeCurrent->ch==13)
								{
									bPreviousPassWasNewLine=1;
									//eine neue Zeile beginnt bei der horizontalen Position 
									//(Offset) 0
									dwHorizontalOffset=0;
									bNewLine=1;
									if(dwFirstVisibleLine)
										--dwFirstVisibleLine;
									else
									{
										CSize size;
										DWORD dwRunInsideOutputLoop=0;
										//alle Zeichen vom Anfang der Zeile bis ausschliesslich
										//dem aktuellen Zeichen (13/newline) ausgeben
										while(pTextNodeBeginOfLine!=pTextNodeCurrent)
										{
											//wenn sich das Zeichen innerhalb des markierten 
											//Bereichs liegt...
											if(m_bMark && dwPositionOfBeginOfCurrentLine+
												dwRunInsideOutputLoop>=dwMarkingBegin && 
												dwPositionOfBeginOfCurrentLine+
												dwRunInsideOutputLoop<dwMarkingEnd)
											{
												//...dann soll die Textfarbe weiss sein (also
												//invertiert)
												pDC->SetTextColor(RGB(255,255,255));
												pDC->SetBkColor(RGB(0,0,0)); // Hintergrund schwarz
	#ifdef _DEBUG
												//printf("Hintergrund schwarz\n");
	#endif
											}
											//wenn sich das Zeichen ausserhalb des markierten 
											//Bereichs liegt...
											else
											{
												pDC->SetBkColor(RGB(255,255,255)); // Hintergrund weiss
	#ifdef _DEBUG	
												//printf("Hintergrund weiss\n");
	#endif
												if(pTextNodeBeginOfLine->color==0)
												{
													//Textfarbe: schwarz
													pDC->SetTextColor(RGB(0,0,0));
	#ifdef _DEBUG	
													//printf("Textfarbe schwarz\n");
	#endif
												}
												if(pTextNodeBeginOfLine->color==1)
												{
													//Textfarbe: rot
													pDC->SetTextColor(RGB(255,0,0));
	#ifdef _DEBUG	
													printf("Textfarbe rot\n");
	#endif
												}
												if(pTextNodeBeginOfLine->color==2)
												{
													//Textfarbe: blau
													pDC->SetTextColor(RGB(0,0,255));
	#ifdef _DEBUG	
													printf("Textfarbe rot\n");
	#endif
												}
											}
											//1 Zeichen ausgeben
											pDC->TextOut(dwHorizontalOffset,dwLinesDrawn*
												textmetric.tmHeight,
                        //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                        //is faster than creating a CString(current->ch) in order to use 
                        //GetOutputTextExtent(const CString& str ) const; .
                        //&(CHAR)pTextNodeBeginOfLine->ch,1
                        CString((TCHAR)pTextNodeBeginOfLine->ch));
											//wenn die aktuelle Ausgabeposition der Cursorposition
											//entspricht und der Cursor gezeichnet werden soll
											//Hinweis: die Prüfung "dwPositionOfBeginOfCurrentLine+
											//dwRunInsideOutputLoop++ == m_CursorPos" muss vor 
											//der Prüfung, ob m_bDrawCursor TRUE ist erfolgen,
											//da sonst schon nach der Prüfung, ob m_bDrawCursor 
											//TRUE ist, abgebrochen wird und der markierte Text 
											//nur angezeigt wird, wenn m_bDrawCursor TRUE ist.
											if(dwPositionOfBeginOfCurrentLine+
												dwRunInsideOutputLoop++==m_CursorPos && m_bDrawCursor)
											{
												CPen pen;
												pDC->MoveTo(dwHorizontalOffset,dwLinesDrawn*
													textmetric.tmHeight);//xpos,line*textmetric.tmHeight);
												if(m_bMark && m_dwMarkingEnd<m_dwMarkingStart  && 
													m_CursorPos==m_dwMarkingEnd)
												{
													pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
													pDC->SelectObject(pen);
												}
												//Text-Cursor zeichnen
												pDC->LineTo(dwHorizontalOffset,(dwLinesDrawn+1)*
													textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
												pen.DeleteObject();
												//Vordergrundfarbe auf schwarz setzen
												pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	#ifdef _DEBUG
												printf("1. LineTo\n");
	#endif
											}
											pDC->SetBkColor(RGB(0,0,0));
											size=pDC->GetOutputTextExtent(
                        //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                        //is faster than creating a CString(current->ch) in order to use 
                        //GetOutputTextExtent(const CString& str ) const; .
                        //&(CHAR)pTextNodeBeginOfLine->ch,1
                        CString((TCHAR)pTextNodeBeginOfLine->ch));
											dwHorizontalOffset+=size.cx;
											//if(pOutputCharacter->m_next)
											pTextNodeBeginOfLine=pTextNodeBeginOfLine->m_next;
											//else
											//	break;
											//dwPositionInsideToken++;
											//dwRunInsideOutputLoop++;
										}
										if(dwPositionOfBeginOfCurrentLine+
											dwRunInsideOutputLoop==m_CursorPos && m_bDrawCursor)
										{
											CPen pen;
											pDC->MoveTo(dwHorizontalOffset,dwLinesDrawn*
												textmetric.tmHeight);//xpos,line*textmetric.tmHeight);
											if(m_bMark && m_dwMarkingEnd<m_dwMarkingStart  && 
												m_CursorPos==m_dwMarkingEnd)
											{
												pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
												pDC->SelectObject(pen);
											}
											pDC->LineTo(dwHorizontalOffset,(dwLinesDrawn+1)*
												textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
											pen.DeleteObject();
											//Vordergrundfarbe auf schwarz setzen
											pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
#ifdef _DEBUG
											printf("1. LineTo\n");
#endif
										}
										//...dann Zeilenindex um 1 erhöhen
										++dwLineNumber;
										++dwLinesDrawn;
									}
									pTextNodeBeginOfLine=pTextNodeCurrent->m_next;
									if(pTextNodeCurrent->m_next)
										//...und das nächste Zeichen newline...
										if(pTextNodeCurrent->m_next->ch==10)
										{
											pTextNodeCurrent=pTextNodeCurrent->m_next;
											pTextNodeBeginOfLine=pTextNodeCurrent->m_next;
										}
									//auch wenn die Zeichen "13" "10" hintereinander stehen
									//zählt das nur als 1 Position
									dwPositionOfBeginOfCurrentLine=dwPosition+1;
								}
								//wenn Leerzeichen
								if(pTextNodeCurrent->ch==' ')
								{
					#ifdef _DEBUG
									printf("current->ch==' '\n");
					#endif
									//Zeichenkette leeren
									strCurrentToken="";
									//++dwLineNumber;
									//dwHorizontalOffset=0;
									strLine+=pTextNodeCurrent->ch;
									//dwPositionOfEndOfPreviousLine=dwNewCursorPosition;
									pTextNodeBeginOfLastToken=pTextNodeCurrent->m_next;
								}
								//wenn Zeichen ausser Leerzeichen
								else
								{
									//wenn nicht Zeichenfolge "13" "10" oder Zeichen "10"
									if(!bNewLine)
									{
										//Zeichen an die Zeichenfolge anhängen
										strCurrentToken+=pTextNodeCurrent->ch;
										//Zeichen an die Zeichenfolge anhängen
										strLine+=pTextNodeCurrent->ch;
									}
								}
								size=pDC->GetOutputTextExtent(strLine);
					#ifdef _DEBUG
								printf("size.cx: %u\n",size.cx);
					#endif
								//wenn ein automatischer Zeilenumbruch stattfindet, weil sonst
								//die Zeile über die vertikale Bildlaufleiste hinaus ginge
								//wenn die Breite in Pixel der Zeile größer ist als die Breite des
								//Clientbereiches minus der Breite in Pixel der Bildlaufleiste (15)
								//minus der Breite in Pixel für einen möglichen Cursor (1)
								if(size.cx>clientRect.right-16)
								{
									CSize sizeCurrentToken=pDC->GetOutputTextExtent(strCurrentToken);
									dwHorizontalOffset=0;
					#ifdef _DEBUG
									printf("size.cx>clientRect.right-16\n");
					#endif
									//wenn das letzte Token in eine Zeile passt
									//(wenn in der aktuellen Zeile KEIN(E) Leerzeichen ist/
									//sind, passt das aktuelle Token NICHT in eine Zeile)
									//wenn die Breite in Pixel der aktuellen Zeichenkette des
									//aktuellen Token kleiner gleich ist als die Breite des
									//Clientbereiches minus der Breite in Pixel der 
									//Bildlaufleiste (15) minus der Breite in Pixel für einen 
									//möglichen Cursor (1)
									if(sizeCurrentToken.cx<=clientRect.right-16)
									{
										if(!dwFirstVisibleLine)
										{
											CSize size;
											DWORD dwRunInsideOutputLoop=0;
						#ifdef _DEBUG
											printf("sizeCurrentToken.cx<=clientRect.right-16\n");
						#endif
											strLine=strCurrentToken;
											while(pTextNodeBeginOfLine!=pTextNodeBeginOfLastToken)
											{
												//wenn sich das Zeichen innerhalb des markierten 
												//Bereichs liegt...
												if(m_bMark && dwPositionOfBeginOfCurrentLine+
													dwRunInsideOutputLoop>=dwMarkingBegin && 
													dwPositionOfBeginOfCurrentLine+
													dwRunInsideOutputLoop<dwMarkingEnd)
												{
													//...dann soll die Textfarbe weiss sein (also
													//invertiert)
													pDC->SetTextColor(RGB(255,255,255));
													pDC->SetBkColor(RGB(0,0,0)); // Hintergrund schwarz
		#ifdef _DEBUG
													printf("Hintergrund schwarz\n");
		#endif
												}
												//wenn sich das Zeichen ausserhalb des markierten 
												//Bereichs liegt...
												else
												{
													pDC->SetBkColor(RGB(255,255,255)); // Hintergrund weiss
		#ifdef _DEBUG
													//printf("Hintergrund weiss\n");
		#endif
													if(pTextNodeBeginOfLine->color==0)
													{
														//Textfarbe: schwarz
														pDC->SetTextColor(RGB(0,0,0));
		#ifdef _DEBUG
														//printf("Textfarbe schwarz\n");
		#endif
													}
													if(pTextNodeBeginOfLine->color==1)
													{
														//Textfarbe: rot
														pDC->SetTextColor(RGB(255,0,0));
		#ifdef _DEBUG
														printf("Textfarbe rot\n");
		#endif
													}
													if(pTextNodeBeginOfLine->color==2)
													{
														//Textfarbe: blau
														pDC->SetTextColor(RGB(0,0,255));
		#ifdef _DEBUG
														printf("Textfarbe rot\n");
		#endif					
													}
												}
												pDC->TextOut(dwHorizontalOffset,dwLinesDrawn*
													textmetric.tmHeight,
                          //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                          //is faster than creating a CString(current->ch) in order to use 
                          //GetOutputTextExtent(const CString& str ) const; .
                          //&(CHAR)pTextNodeBeginOfLine->ch,1
                          CString((TCHAR)pTextNodeBeginOfLine->ch));
												//wenn die aktuelle Ausgabeposition der Cursorposition
												//entspricht und der Cursor gezeichnet werden soll
												//Hinweis: die Prüfung "dwPositionAfterOutput+
												//dwRunInsideOutputLoop++ == m_CursorPos" muss vor 
												//der Prüfung, ob m_bDrawCursor TRUE ist erfolgen,
												//da sonst schon nach der Prüfung, ob m_bDrawCursor 
												//TRUE ist, abgebrochen wird und der markierte Text 
												//nur angezeigt wird, wenn m_bDrawCursor TRUE ist.
												if(dwPositionOfBeginOfCurrentLine+dwRunInsideOutputLoop++==
													m_CursorPos && m_bDrawCursor)
												{
													CPen pen;
													pDC->MoveTo(dwHorizontalOffset,dwLinesDrawn*
														textmetric.tmHeight);//xpos,line*textmetric.tmHeight);
													if(m_bMark && m_dwMarkingEnd<m_dwMarkingStart  && 
														m_CursorPos==m_dwMarkingEnd)
													{
														pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
														pDC->SelectObject(pen);
													}
													pDC->LineTo(dwHorizontalOffset,(dwLinesDrawn+1)*
														textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
													pen.DeleteObject();
													//Vordergrundfarbe auf schwarz setzen
													pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
		#ifdef _DEBUG
													printf("1. LineTo\n");
		#endif
												}
												pDC->SetBkColor(RGB(0,0,0));
												size=pDC->GetOutputTextExtent(
                          //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                          //is faster than creating a CString(current->ch) in order to use 
                          //GetOutputTextExtent(const CString& str ) const; .
                          //&(CHAR)pTextNodeBeginOfLine->ch,1
                          CString((TCHAR)pTextNodeBeginOfLine->ch));
												dwHorizontalOffset+=size.cx;
												//if(pOutputCharacter->m_next)
												pTextNodeBeginOfLine=pTextNodeBeginOfLine->m_next;
												//else
												//	break;
												//dwPositionInsideToken++;
												//dwRunInsideOutputLoop++;
											}
										}
										pTextNodeBeginOfLine=pTextNodeBeginOfLastToken;
										//dwPositionOfBeginOfCurrentLine=dwPosition-strCurrentToken.
										//	GetLength();
										dwPositionOfBeginOfCurrentLine=dwPosition-strCurrentToken.
											GetLength()+1;
										strLine=strCurrentToken;
									}
									//wenn das letzte Token NICHT in eine Zeile passt
									else
									{
										if(!dwFirstVisibleLine)
										{
											CSize size;
											DWORD dwRunInsideOutputLoop=0;
											while(pTextNodeBeginOfLine!=pTextNodeCurrent)
											{
												//wenn sich das Zeichen innerhalb des markierten 
												//Bereichs liegt...
												if(m_bMark && dwPositionOfBeginOfCurrentLine+
													dwRunInsideOutputLoop>=dwMarkingBegin && 
													dwPositionOfBeginOfCurrentLine+
													dwRunInsideOutputLoop<dwMarkingEnd)
												{
													//...dann soll die Textfarbe weiss sein (also
													//invertiert)
													pDC->SetTextColor(RGB(255,255,255));
													pDC->SetBkColor(RGB(0,0,0)); // Hintergrund schwarz
		#ifdef _DEBUG
													printf("Hintergrund schwarz\n");
		#endif
												}
												//wenn sich das Zeichen ausserhalb des markierten 
												//Bereichs liegt...
												else
												{
													pDC->SetBkColor(RGB(255,255,255)); // Hintergrund weiss
		#ifdef _DEBUG
													//printf("Hintergrund weiss\n");
		#endif
													if(pTextNodeBeginOfLine->color==0)
													{
														//Textfarbe: schwarz
														pDC->SetTextColor(RGB(0,0,0));
		#ifdef _DEBUG
														//printf("Textfarbe schwarz\n");
		#endif
													}
													if(pTextNodeBeginOfLine->color==1)
													{
														//Textfarbe: rot
														pDC->SetTextColor(RGB(255,0,0));
		#ifdef _DEBUG
														printf("Textfarbe rot\n");
		#endif
													}
													if(pTextNodeBeginOfLine->color==2)
													{
														//Textfarbe: blau
														pDC->SetTextColor(RGB(0,0,255));
		#ifdef _DEBUG
														printf("Textfarbe rot\n");
		#endif
													}
												}
												pDC->TextOut(dwHorizontalOffset,dwLinesDrawn*
													textmetric.tmHeight,
                          //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                          //is faster than creating a CString(current->ch) in order to use 
                          //GetOutputTextExtent(const CString& str ) const; .
                          //&(CHAR)pTextNodeBeginOfLine->ch,1
                          CString((TCHAR)pTextNodeBeginOfLine->ch));
												//wenn die aktuelle Ausgabeposition der Cursorposition
												//entspricht und der Cursor gezeichnet werden soll
												//Hinweis: die Prüfung "dwPositionAfterOutput+
												//dwRunInsideOutputLoop++ == m_CursorPos" muss vor 
												//der Prüfung, ob m_bDrawCursor TRUE ist erfolgen,
												//da sonst schon nach der Prüfung, ob m_bDrawCursor 
												//TRUE ist, abgebrochen wird und der markierte Text 
												//nur angezeigt wird, wenn m_bDrawCursor TRUE ist.
												if(dwPositionOfBeginOfCurrentLine+dwRunInsideOutputLoop++==
													m_CursorPos && m_bDrawCursor)
												{
													CPen pen;
													pDC->MoveTo(dwHorizontalOffset,dwLinesDrawn*
														textmetric.tmHeight);//xpos,line*textmetric.tmHeight);
													if(m_bMark && m_dwMarkingEnd<m_dwMarkingStart  && 
														m_CursorPos==m_dwMarkingEnd)
													{
														pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
														pDC->SelectObject(pen);
													}
													pDC->LineTo(dwHorizontalOffset,(dwLinesDrawn+1)*
														textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
													pen.DeleteObject();
													//Vordergrundfarbe auf schwarz setzen
													pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
		#ifdef _DEBUG
													printf("1. LineTo\n");
		#endif
												}
												pDC->SetBkColor(RGB(0,0,0));
												size=pDC->GetOutputTextExtent(
                          //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                          //is faster than creating a CString(current->ch) in order to use 
                          //GetOutputTextExtent(const CString& str ) const; .
                          //&(CHAR)pTextNodeBeginOfLine->ch,1
                          CString((TCHAR)pTextNodeBeginOfLine->ch));
												dwHorizontalOffset+=size.cx;
												//if(pOutputCharacter->m_next)
												pTextNodeBeginOfLine=pTextNodeBeginOfLine->m_next;
												//else
												//	break;
												//dwPositionInsideToken++;
												//dwRunInsideOutputLoop++;
											}
										}
										strLine=CString((TCHAR)(pTextNodeCurrent->ch));
										strCurrentToken=CString((TCHAR)(pTextNodeCurrent->ch));
										pTextNodeBeginOfLine=pTextNodeCurrent;
										dwPositionOfBeginOfCurrentLine=dwPosition;
										//dwPositionOfBeginOfCurrentLine=dwPosition+1;
									}
									if(dwFirstVisibleLine)
										--dwFirstVisibleLine;
									else
									{
										++dwLineNumber;
										++dwLinesDrawn;
									}
									size=pDC->GetOutputTextExtent(strLine);
								}
								++dwPosition;
								if(pTextNodeCurrent->m_next)
									pTextNodeCurrent=pTextNodeCurrent->m_next;
								//wenn das Ende der verketteten Liste erreicht ist
								else
								{
									if(!dwFirstVisibleLine)
									{
										CSize size;
										DWORD dwRunInsideOutputLoop=0;
										dwHorizontalOffset=0;
										while(pTextNodeBeginOfLine!=NULL)
										{
											//wenn sich das Zeichen innerhalb des markierten 
											//Bereichs liegt...
											if(m_bMark && dwPositionOfBeginOfCurrentLine+
												dwRunInsideOutputLoop>=dwMarkingBegin && 
												dwPositionOfBeginOfCurrentLine+
												dwRunInsideOutputLoop<dwMarkingEnd)
											{
												//...dann soll die Textfarbe weiss sein (also
												//invertiert)
												pDC->SetTextColor(RGB(255,255,255));
												pDC->SetBkColor(RGB(0,0,0)); // Hintergrund schwarz
	#ifdef _DEBUG
												printf("Hintergrund schwarz\n");
	#endif
											}
											//wenn sich das Zeichen ausserhalb des markierten 
											//Bereichs liegt...
											else
											{
												pDC->SetBkColor(RGB(255,255,255)); // Hintergrund weiss
	#ifdef _DEBUG
												//printf("Hintergrund weiss\n");
	#endif
												if(pTextNodeBeginOfLine->color==0)
												{
													//Textfarbe: schwarz
													pDC->SetTextColor(RGB(0,0,0));
	#ifdef _DEBUG
													//printf("Textfarbe schwarz\n");
	#endif
												}
												if(pTextNodeBeginOfLine->color==1)
												{
													//Textfarbe: rot
													pDC->SetTextColor(RGB(255,0,0));
	#ifdef _DEBUG
													printf("Textfarbe rot\n");
	#endif
												}
												if(pTextNodeBeginOfLine->color==2)
												{
													//Textfarbe: blau
													pDC->SetTextColor(RGB(0,0,255));
	#ifdef _DEBUG
													printf("Textfarbe rot\n");
	#endif
												}
											}
											//1 Zeichen ausgeben
											pDC->TextOut(dwHorizontalOffset,dwLinesDrawn*
												textmetric.tmHeight,
                        //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                        //is faster than creating a CString(current->ch) in order to use 
                        //GetOutputTextExtent(const CString& str ) const; .
                        //&(CHAR)pTextNodeBeginOfLine->ch,1
                        CString((TCHAR)pTextNodeBeginOfLine->ch));
											//wenn die aktuelle Ausgabeposition der Cursorposition
											//entspricht und der Cursor gezeichnet werden soll
											//Hinweis: die Prüfung "dwPositionAfterOutput+
											//dwRunInsideOutputLoop++ == m_CursorPos" muss vor 
											//der Prüfung, ob m_bDrawCursor TRUE ist erfolgen,
											//da sonst schon nach der Prüfung, ob m_bDrawCursor 
											//TRUE ist, abgebrochen wird und der markierte Text 
											//nur angezeigt wird, wenn m_bDrawCursor TRUE ist.
											if(dwPositionOfBeginOfCurrentLine+dwRunInsideOutputLoop++==
												m_CursorPos && m_bDrawCursor)
											{
												CPen pen;
												pDC->MoveTo(dwHorizontalOffset,dwLinesDrawn*
													textmetric.tmHeight);//xpos,line*textmetric.tmHeight);
												if(m_bMark && m_dwMarkingEnd<m_dwMarkingStart  && 
													m_CursorPos==m_dwMarkingEnd)
												{
													pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
													pDC->SelectObject(pen);
												}
												pDC->LineTo(dwHorizontalOffset,(dwLinesDrawn+1)*
													textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
												pen.DeleteObject();
												//Vordergrundfarbe auf schwarz setzen
												pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	#ifdef _DEBUG
												printf("1. LineTo\n");
	#endif
											}
											pDC->SetBkColor(RGB(0,0,0));
											size=pDC->GetOutputTextExtent(
                        //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                        //is faster than creating a CString(current->ch) in order to use 
                        //GetOutputTextExtent(const CString& str ) const; .
                        //&(CHAR)pTextNodeBeginOfLine->ch,1
                        CString((TCHAR)pTextNodeBeginOfLine->ch));
											dwHorizontalOffset+=size.cx;
											//if(pOutputCharacter->m_next)
											pTextNodeBeginOfLine=pTextNodeBeginOfLine->m_next;
										//else
										//	break;
										//dwPositionInsideToken++;
										//dwRunInsideOutputLoop++;
										}
										//WENN SICH DER CURSOR AN DER LETZTEN POSITION BEFINDET UND
										//wenn die aktuelle Ausgabeposition der Cursorposition
										//entspricht und der Cursor gezeichnet werden soll
										//Hinweis: die Prüfung "dwPositionAfterOutput+
										//dwRunInsideOutputLoop++ == m_CursorPos" muss vor 
										//der Prüfung, ob m_bDrawCursor TRUE ist erfolgen,
										//da sonst schon nach der Prüfung, ob m_bDrawCursor 
										//TRUE ist, abgebrochen wird und der markierte Text 
										//nur angezeigt wird, wenn m_bDrawCursor TRUE ist.
										if(dwPositionOfBeginOfCurrentLine+dwRunInsideOutputLoop==
											m_CursorPos && m_bDrawCursor)
										{
											CPen pen;
											pDC->MoveTo(dwHorizontalOffset,dwLinesDrawn*
												textmetric.tmHeight);//xpos,line*textmetric.tmHeight);
											if(m_bMark && m_dwMarkingEnd<m_dwMarkingStart  && 
												m_CursorPos==m_dwMarkingEnd)
											{
												pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
												pDC->SelectObject(pen);
											}
											pDC->LineTo(dwHorizontalOffset,(dwLinesDrawn+1)*
												textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
											pen.DeleteObject();
											//Vordergrundfarbe auf schwarz setzen
											pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
#ifdef _DEBUG
											printf("1. LineTo\n");
#endif
										}
									}
									break;
								}
							}
						}// if(m_pText->m_first)
						//wenn keine Zeichen im Text sind
						else // m_pText->m_first==NULL
						{
							if(m_bDrawCursor)
							{
								pDC->MoveTo(0,0);//xpos,line*textmetric.tmHeight);
								pDC->LineTo(0,textmetric.tmHeight);//xpos,(line+1)*textmetric.tmHeight);
			#ifdef _DEBUG
								printf("7. LineTo\n");
			#endif
								pDC->SetBkColor(RGB(0,0,0));
							}
						}
				}
			}
		}
	}
	if(m_bDCAttached)
	{
		m_wndVert.MoveWindow(rect,TRUE);
	}
	else
	{
		m_wndVert.ShowScrollBar(TRUE); // Bildlaufleiste nun anzeigen
	}
//EndOfOnDraw:;
	TRACE("void EnglishView::OnDraw(CDC* pDC) - ENDE\n");
#ifdef _DEBUG
	printf("void EnglishView::OnDraw(CDC* pDC) - ENDE\n");
#endif
	//CView::OnDraw(pDC);
}

BOOL EnglishView::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE; // Rückgabe von FALSE: kein Neuzeichnen des Hintergrundes
}

void EnglishView::OnSize(UINT nType, int cx, int cy) 
{
	CDC * pDC;
	CRect rect;
	SCROLLINFO si;
	TEXTMETRIC textmetric;
#ifdef _DEBUG
	printf("void EnglishView::OnSize(UINT nType, int cx, int cy) - ANFANG\n");
#endif
	TRACE("void EnglishView::OnSize(UINT nType, int cx, int cy) - ANFANG\n");
	pDC=GetDC();
	GetClientRect(rect);
	rect.left=rect.right-15;
	//Invalidate();
	//wenn diese Ansicht schon angezeigt wurde
	if(m_bInitiallyDisplayed)
	{
#ifdef _DEBUG
		printf("m_bInitiallyDisplayed\n");
#endif
		//m_wndVert.DestroyWindow(); // vor dem erneuten Erstellen unbedingt zerstören
		ZeroMemory(&si, sizeof(SCROLLINFO));
	  si.cbSize = sizeof(SCROLLINFO);
		si.fMask=SIF_ALL; // ??
 		si.nMin=0; // minimum (int)

		si.nPos=0; // Bildlaufleistenposition auf 0 setzen
		si.nTrackPos=0; // ??
		//si.fMask = SIF_TRACKPOS|SIF_PAGE|SIF_RANGE;
		/* die 32-Bit Position der Bildlaufleiste wird nun in
		si.nTrackPos gespeichert, wenn die Bildlaufleiste das Ereignis SB_THUMBTRACK
		erhält (sonst steht im Parameter nPos der Ereignisfunktion 'OnVScroll' oder
		'OnHScroll' nur eine 16-Bit Position gespeichert, wenn SB_THUMBTRACK auftritt, 
		die nur Werte bis maximal 32667 erlaubt)*/
		/* SIF_PAGE muss angegeben werden, damit das Kästchen der Bildlaufleiste die
			richtige Größe hat*/
		pDC->GetTextMetrics(&textmetric); // für die Zeilenhöhe
		si.nPage=rect.bottom/textmetric.tmHeight;//si.nMax;
		if(bEnglishPart)
		{
			CalculateNumberOfLines();
			//m_wndVert.Create(SBS_VERT,rect,this,IDC_ENGLISH_SCROLLVERT); // Stil, Position und Größe, CDialog, ID
			//if(m_wndVert.
			m_wndVert.MoveWindow(rect,FALSE);
			CVTransDoc * pDoc=GetDocument();
			if(pDoc)
				si.nMax=m_dwNumberOfLines;
		}
		else
			//m_wndVert.Create(SBS_VERT,rect,this,IDC_GERMAN_SCROLLVERT); // Stil, Position und Größe, CDialog, ID
			m_wndVert.MoveWindow(rect,FALSE);
		m_wndVert.SetScrollInfo(&si,TRUE); // Scrollposition
		//m_wndVert.ShowScrollBar(TRUE); // Bildlaufleiste nun anzeigen
		if(m_bHasFocus)
		{
			int iInstallResult;
			KillTimer(1);
			iInstallResult=SetTimer(1,500,NULL);
			if(!iInstallResult)
				AfxMessageBox("Kann keinen Timer installieren! Das heisst, dass es \
keinen blinkenden Cursor in einem oder 2 Textfeld(ern) des aktiven Kindfensters \
geben wird. Das Beenden aktiver Programme kann Timer freigeben. Starten Sie \
dann diese Anwendung nochmals.",MB_OK,NULL);
		}
	}

	TRACE("void EnglishView::OnSize(UINT nType, int cx, int cy) - ENDE\n");
#ifdef _DEBUG
	printf("void EnglishView::OnSize(UINT nType, int cx, int cy) - ENDE\n");
#endif
}
/////////////////////////////////////////////////////////////////////////////
// CVTransView Drucken

void EnglishView::OnTimer(UINT nIDEvent)
{
	Invalidate(TRUE);
	m_bDrawCursor=!m_bDrawCursor; // Flag invertieren
	CView::OnTimer(nIDEvent);
}

BOOL EnglishView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Standardvorbereitung
	return DoPreparePrinting(pInfo);
}

void EnglishView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Zusätzliche Initialisierung vor dem Drucken hier einfügen
}

void EnglishView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Hier Bereinigungsarbeiten nach dem Drucken einfügen
}

void EnglishView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	TRACE("void EnglishView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)\n");
#ifdef _DEBUG
	printf("void EnglishView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)-ANFANG\n");
	printf("nChar: %u %c\n",nChar,(char)nChar);
	fflush(stdout);
#endif
	if(nChar==13)
	{
		ColorText ct;
		DWORD dwNewLines=0L;
		ct.color=0;
		ct.str=CString((TCHAR)nChar);
		//m_ColorText.push_back(ct);
		//m_Text.Insert(0,nChar);
		//TRACE("\nnChar: %d\n",nChar);
		if(m_pText->m_first) // Text schwarz färben
		{
			TextNode * current=m_pText->m_first;
			while(1)
			{
				current->color=0;
				if(current->ch==13)
					dwNewLines++;
				if(current->m_next)
					current=current->m_next;
				else
					break;
			}
		}
		if(m_bMark) // wenn Text markiert wurde
		{
			DWORD dwMarkingStart=m_dwMarkingStart<m_dwMarkingEnd?m_dwMarkingStart:m_dwMarkingEnd;
			DWORD dwMarkingEnd=m_dwMarkingStart>m_dwMarkingEnd?m_dwMarkingStart:m_dwMarkingEnd;
			m_pText->Delete(dwMarkingStart,dwMarkingEnd-dwMarkingStart);
			m_bMark=FALSE;
			m_CursorPos=dwMarkingStart;
		}
		// Zeichen einfügen mit schwarzer Farbe (0)
		m_pText->Insert(m_CursorPos++,(char)nChar,0);
		//der Cursor soll nach dem Einfügen erscheinen (wenn m_bDrawCursor nicht 
		//auf TRUE gesetzt wird, ist der Cursor eventuell nicht sichtbar, weil
		//er beim Blinken in der Phase des Nicht-Anzeigens ist)
		m_bDrawCursor=TRUE;
		Invalidate();
		TEXTMETRIC textmetric;
		TRACE("vor: CDC* pDC=GetDC();\n");
		CDC* pDC=GetDC();
		TRACE("vor: pDC->GetTextMetrics(&textmetric);\n");
		pDC->GetTextMetrics(&textmetric); // für die Zeilenhöhe
		CRect rect;
		TRACE("vor: GetClientRect(lpRect);\n");
		GetClientRect(rect);
		TRACE("vor: if(textmetric.tmHeight*(dwNewLines+1)>rect.bottom)\n");
////DEaktiviert "warning C4018: '>': Konflikt zwischen 'signed' und 'unsigned'"
//#pragma warning(disable : 4018)
		if((LONG)(textmetric.tmHeight*(dwNewLines+1))>rect.bottom)
		{
			//CRect rect;
			//GetClientRect(&rect);
			if(rect.right>=15)
				rect.left=rect.right-15;
		}
////REaktiviert "warning C4018: '>': Konflikt zwischen 'signed' und 'unsigned'"
//#pragma warning(restore : 4018)
	}
	//damit bei Rückschritt nicht noch ein Zeichen (im else-Zweig) eingefügt 
	//wird, steht hier eine leere Verzweigung
	else if(nChar==8)
	{
	}
	else
	{
		if(bEnglishPart)
		{
			CVTransApp * pApp=(CVTransApp *)AfxGetApp();
			CMainFrame * pFrame=NULL;
			if(pApp)
			{
				pFrame=(CMainFrame *)pApp->m_pMainWnd;
				if(pFrame)
					if(pFrame->m_pTranslationThread)
					{
#ifdef _DEBUG
						printf("Übersetzungsthread läuft bereits\n");
#endif
						g_bContinue=FALSE;
						//Mit der Funktion "WaitForSingleObject" gelingt die Textveränderung
						//während der Übersetzung viel flüssiger als mit einer Schleife, in
						//der ständig mittels "::GetExitCodeThread"überprüft wird, ob der 
						//Thread schon beendet wurde. Die Funktion "WaitForSingleObject"
						//scheint also den Prozessor weniger zu belasten, was die MSDN 
						//Biblithek (also die Dokumentation/Hilfe zu Visual C++ belegt:
						//"The thread consumes very little processor time while waiting for
						//the object state to become signaled or the time-out interval to elapse."
						//warten, bis der Übersetzungsthread beendet ist
						WaitForSingleObject(pFrame->m_pTranslationThread->m_hThread,INFINITE);
#ifdef _DEBUG
						printf("Übersetzungsthread beendet\n");
#endif
					}
			}
#ifdef _DEBUG
			printf("Übersetzungsthread läuft nicht\n");
			fflush(stdout);
#endif

#ifdef _DEBUG
			if(g_bExecutedOnTranslateAutomaticallyFinishes)
				printf("g_bExecutedOnTranslateAutomaticallyFinishes==TRUE\n");
			else
				printf("g_bExecutedOnTranslateAutomaticallyFinishes==FALSE\n");
			if(pFrame->m_bCreated)
				printf("pFrame->m_bCreated\n");
			else
				printf("pFrame->m_bCreated==FALSE\n");
#endif
			//die Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
			//wird nicht direkt nach der Funktion TranslateAutomatically()
			//ausgeführt, wenn diese Funktion (EnglishView::OnChar()) vor 
			//der Beendigung der Funktion TranslateAutomatically() ausgeführt
			//wird.
			//wenn die Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
			//nach der Übersetzung noch nicht ausgeführt wurde, wird folgender
			//Code ausgeführt:
			if(!g_bExecutedOnTranslateAutomaticallyFinishes && pFrame->m_bCreated)
			{
				//AfxMessageBox("!g_bExecutedOnTranslateAutomaticallyFinishes && pFrame->m_bCreated",MB_OK,NULL);
				//MessageBeep((WORD)-1);
				pFrame->KillTimer(1);
				//if(pFrame->m_bShowClauseProgress)
				//{
					if(pFrame->m_bShowGraphicClauseProgress)
					{
						pFrame->m_Progress.DestroyWindow();
#ifdef _DEBUG
						printf("nach: pFrame->m_Progress.DestroyWindow()\n");
#endif
					}
					if(pFrame->m_bShowNumericClauseProgress)
						pFrame->m_cStaticClauseProgress.DestroyWindow();
				//}
				//if(pFrame->m_bShowSentenceProgress)
				//{
					if(pFrame->m_bShowGraphicSentenceProgress)
						pFrame->m_SentenceProgress.DestroyWindow();
					if(pFrame->m_bShowNumericSentenceProgress)
						pFrame->m_cStaticSentenceProgress.DestroyWindow();
				//}
				//if(pFrame->m_bShowTextProgress)
				//{
					if(pFrame->m_bShowGraphicTextProgress)
						pFrame->m_TextProgress.DestroyWindow();
					if(pFrame->m_bShowNumericTextProgress)
						pFrame->m_cStaticTextProgress.DestroyWindow();
				//}
				if(pFrame->m_bShowPrognosticedRemainingTime)
					pFrame->m_cStaticRemainingTime.DestroyWindow();
				pFrame->m_bCreated=FALSE;
				pFrame->m_bMayStartThread=TRUE;
				//damit das Zerstören der Fortschrittsbalken (CProgressCtrl)
				//nicht in der Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
				//ausgeführt wird (sie wurden ja bereits über diesem Kommentar mit
				//DestroyWindow() zerstört und sollen nicht doppelt zerstört werden), 
				//wird g_bExecutedOnTranslateAutomaticallyFinishes auf TRUE gesetzt
				//wenn die Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
				//ausgeführt wird, wird g_bExecutedOnTranslateAutomaticallyFinishes 
				//ausgewertet.
				g_bExecutedOnTranslateAutomaticallyFinishes=TRUE;
			}
			CToolBarCtrl & toolBarCtrl=pFrame->m_wndToolBar.GetToolBarCtrl();
			//wenn automatische Übersetzung aktiviert ist
			if(toolBarCtrl.IsButtonChecked(ID_TRANSLATE_AUTOMATICALLY_IF_TEXTMODIFICATION_BUTTON))
			{
				//MessageBeep((WORD)-1);
				//AfxMessageBox("autiomatisches Übersetzen",MB_OK,NULL);
				//CToolBarCtrl & toolBarCtrl=pFrame->m_wndToolBar.GetToolBarCtrl();
				//toolBarCtrl.HideButton(ID_TRANSLATE_BUTTON,TRUE);
				//Text * pText=new Text(*m_pText); // Kopie anlegen
				std::vector<Range> vecRange;
				CChildFrame * pChild=(CChildFrame*)pFrame->MDIGetActive();
				EnglishView * pEVGerman=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
				if(pEVGerman)
				{
					pEVGerman->m_bDrawVector=TRUE;
					//pFrame->m_bCreated=FALSE;
					if(pFrame->m_bCreated == FALSE)
					{
					//AfxMessageBox("pFrame->m_bCreated == FALSE",MB_OK,NULL);
						BYTE bIDCount=0;//3;
						BYTE bMainPartsCount=0;//Anzahl der grafischen Fortschritte
						std::vector<UINT> vecUINT;
						//if(pFrame->m_bShowClauseProgress)
							//{
						if(pFrame->m_bShowNumericClauseProgress)
						{
							bIDCount++;
							vecUINT.push_back(ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE);
						}
						if(pFrame->m_bShowGraphicClauseProgress)
						{
							bIDCount++;
							bMainPartsCount++;
							vecUINT.push_back(ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
						}
						//bIDCount++;
						//vecUINT.push_back(ID_INDICATOR_CLAUSE_PROGRESS_PANE);
					//}
							//if(pFrame->m_bShowSentenceProgress)
							//{
						if(pFrame->m_bShowNumericSentenceProgress)
						{
							bIDCount++;
							vecUINT.push_back(ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE);
						}
						if(pFrame->m_bShowGraphicSentenceProgress)
						{
							bIDCount++;
							bMainPartsCount++;
							vecUINT.push_back(ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
						}
								//bIDCount++;
								//vecUINT.push_back(ID_INDICATOR_SENTENCE_PROGRESS_PANE);
							//}
							//if(pFrame->m_bShowTextProgress)
							//{
						if(pFrame->m_bShowNumericTextProgress)
						{
							bIDCount++;
							vecUINT.push_back(ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE);
						}
						if(pFrame->m_bShowGraphicTextProgress)
						{
							bIDCount++;
							bMainPartsCount++;
							vecUINT.push_back(ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
						}
								//bIDCount++;
								//vecUINT.push_back(ID_INDICATOR_TEXT_PROGRESS_PANE);
							//}
						if(pFrame->m_bShowPrognosticedRemainingTime)
						{
								//MessageBeep((WORD)-1);
							bIDCount++;
							vecUINT.push_back(ID_INDICATOR_REMAINING_TIME);
						}
						if(vecUINT.size()>0)
						{
							UINT * lpIDArray=new UINT[bIDCount];
							for(BYTE bRun=0;bRun<vecUINT.size();bRun++)
								lpIDArray[bRun]=vecUINT.at(bRun);
								//lpIDArray[vecUINT.size()]=ID_INDICATOR_CAPS;
								//lpIDArray[vecUINT.size()+1]=ID_INDICATOR_NUM;
								//lpIDArray[vecUINT.size()+2]=ID_INDICATOR_SCRL;
							pFrame->m_wndStatusBar.SetIndicators(lpIDArray,
								sizeof(lpIDArray)/sizeof(UINT));
						}
						CStatusBarCtrl & statusbarCtrl=pFrame->m_wndStatusBar.GetStatusBarCtrl();
							//int nHorz=0,nVert=0,nSpacing=0;
							//statusbarCtrl.GetBorders(nHorz,nVert,nSpacing);
						CRect rect,rect2;
							//pFrame->m_wndStatusBar.GetWindowRect(rect);
						pFrame->m_wndStatusBar.GetClientRect(rect2);
#ifdef _DEBUG
						printf("lpRect->bottom: %d lpRect->left %d lpRect->right %d lpRect->top %d\n",rect.bottom,rect.left,rect.right,rect.top);
						printf("lpRect->bottom: %d lpRect->left %d lpRect->right %d lpRect->top %d\n",rect2.bottom,rect2.left,rect2.right,rect2.top);
							//printf("nHorz: %d nVert: %d nSpacing: %d\n",nHorz,nVert,nSpacing);
#endif
						if(bIDCount>0)
						{
							if(pFrame->m_bShowPrognosticedRemainingTime)
							{
								int * Widths=new int[bIDCount];
								for(BYTE bRun=0;bRun<bIDCount;bRun++)
									Widths[bRun]=rect2.right*(bRun+1)/bIDCount;
									//int Widths []={rect2.right/3,rect2.right*2/3,rect2.right};
								statusbarCtrl.SetParts(bIDCount,Widths);
							}
							else
							{
								int * Widths=new int[bIDCount];
								for(BYTE bRun=0;bRun<bIDCount;bRun++)
									Widths[bRun]=rect2.right*(bRun+1)/bIDCount;
								//int Widths []={rect2.right/3,rect2.right*2/3,rect2.right};
								statusbarCtrl.SetParts(bIDCount,Widths);
							}
						}
						//CString strOutput;
							//strOutput.Format("bIDCount: %u\n",bIDCount);
							//AfxMessageBox(strOutput,MB_OK,NULL);
						bIDCount=0;
						RECT MyRect;
							//if(pFrame->m_bShowClauseProgress)
							//{
								//pFrame->m_wndStatusBar.GetItemRect(0, &MyRect);
#ifdef _DEBUG
						printf("pFrame->m_bShowClauseProgress\n");
#endif
						if(pFrame->m_bShowNumericClauseProgress)
						{
							pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
							pFrame->m_cStaticClauseProgress.Create("",WS_VISIBLE|WS_CHILD,
								MyRect,&pFrame->m_wndStatusBar,ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE);
						}
						if(pFrame->m_bShowGraphicClauseProgress)
						{
							pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
					//AfxMessageBox("vor der Erstellung",MB_OK,NULL);
							if(pFrame->m_nGraphicClauseProgressType==0)
								pFrame->m_Progress.Create(WS_VISIBLE|WS_CHILD,MyRect,
								&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
							if(pFrame->m_nGraphicClauseProgressType==1)
								pFrame->m_Progress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
								&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
					//AfxMessageBox("nach der Erstellung",MB_OK,NULL);
							pFrame->m_Progress.SetRange(0,100); //Set the range to between 0 and 100
							pFrame->m_Progress.SetStep(1); // Set the step amount
						}
								//pFrame->m_Progress.Create(WS_VISIBLE|WS_CHILD, MyRect, &pFrame->m_wndStatusBar,IDC_INDICATOR_CLAUSE_PROGRESS_PANE);
								//pFrame->m_Progress.SetRange(0,100); //Set the range to between 0 and 100
								//pFrame->m_Progress.SetStep(1); // Set the step amount
								//pFrame->m_Progress.StepIt();
							//}
							//if(pFrame->m_bShowSentenceProgress)
							//{
								//pFrame->m_wndStatusBar.GetItemRect(1, &MyRect);
#ifdef _DEBUG
						printf("pFrame->m_bShowSentenceProgress\n");
#endif
						if(pFrame->m_bShowNumericSentenceProgress)
						{
							pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
							pFrame->m_cStaticSentenceProgress.Create("",WS_VISIBLE|WS_CHILD,
								MyRect,&pFrame->m_wndStatusBar,
								ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE);
						}
						if(pFrame->m_bShowGraphicSentenceProgress)
						{
							pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
							if(pFrame->m_nGraphicSentenceProgressType==0)
								pFrame->m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD,MyRect,
									&pFrame->m_wndStatusBar,
									ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
							if(pFrame->m_nGraphicSentenceProgressType==1)
								pFrame->m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD|
								PBS_SMOOTH,MyRect,&pFrame->m_wndStatusBar,
								ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
							pFrame->m_SentenceProgress.SetRange(0,100); //Set the range to between 0 and 100
							pFrame->m_SentenceProgress.SetStep(1); // Set the step amount
						}
								//pFrame->m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD, MyRect, &pFrame->m_wndStatusBar,ID_INDICATOR_SENTENCE_PROGRESS_PANE);
								//pFrame->m_SentenceProgress.SetRange(0,100); //Set the range to between 0 and 100
								//pFrame->m_SentenceProgress.SetStep(1); // Set the step amount
								//pFrame->m_SentenceProgress.StepIt();
							//}
							//if(pFrame->m_bShowTextProgress)
							//{
								//pFrame->m_wndStatusBar.GetItemRect(2, &MyRect);
						if(pFrame->m_bShowNumericTextProgress)
						{
							pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
							pFrame->m_cStaticTextProgress.Create("",WS_VISIBLE|WS_CHILD,
								MyRect,&pFrame->m_wndStatusBar,ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE);
						}
						if(pFrame->m_bShowGraphicTextProgress)
						{
							pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
							if(pFrame->m_nGraphicTextProgressType==0)
								pFrame->m_TextProgress.Create(WS_VISIBLE|WS_CHILD,MyRect,
									&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
							if(pFrame->m_nGraphicTextProgressType==1)
								pFrame->m_TextProgress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
									&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
							pFrame->m_TextProgress.SetRange(0,100); //Set the range to between 0 and 100
							pFrame->m_TextProgress.SetStep(1); // Set the step amount
						}
								//pFrame->m_TextProgress.Create(WS_VISIBLE|WS_CHILD, MyRect, &pFrame->m_wndStatusBar,ID_INDICATOR_TEXT_PROGRESS_PANE);
								//pFrame->m_TextProgress.SetRange(0,100); //Set the range to between 0 and 100
								//pFrame->m_TextProgress.SetStep(1); // Set the step amount
								//pFrame->m_TextProgress.StepIt();
							//}
						if(pFrame->m_bShowPrognosticedRemainingTime)
						{
							pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
					//AfxMessageBox("vor der Erstellung",MB_OK,NULL);
							pFrame->m_cStaticRemainingTime.Create("",WS_VISIBLE|WS_CHILD,
								MyRect,&pFrame->m_wndStatusBar,ID_INDICATOR_REMAINING_TIME);
					//AfxMessageBox("nach der Erstellung",MB_OK,NULL);
							tAtTheBeginningOfTheTranslation=(CTime)CTime::GetCurrentTime();
						}
						LPCTSTR str="Fortschritt des Haupt- oder Nebensatzes";
							//statusbarCtrl.SetTipText(0,str);
							//statusbarCtrl.SetTipText(1,_T("Fortschritt des aktuellen Satzes"));
							//statusbarCtrl.SetTipText(2,_T("Fortschritt des ganzen Textes"));
						pFrame->m_bCreated = TRUE;
					}
					if(pFrame->SetTimer(1,1000,NULL)==0)//Fehler beim 
					{//Installieren des Timers, möglicherweise gibt es
								//keinen Timer mehr, denn Timer sind begrenzt
								//AfxMessageBox("Timewr erfolgreich",MB_OK,NULL);
#ifdef _DEBUG
						printf("pFrame->SetTimer(1,1000,NULL)==0\n");
#endif
					}
					else//Timer erfolgreich installiert
					{
							//AfxMessageBox("Timer erfolgreich",MB_OK,NULL);
								//MessageBox("Timer erfolgreich",NULL,MB_OK);
#ifdef _DEBUG
						printf("pFrame->SetTimer(1,1000,NULL)!=0\n");
#endif
					}
					pFrame->m_bTimerIntalled=TRUE;
					//HWND hWnd;
					//TranslateAutomaticallyParameters * pTranslateAutomaticallyParameters=
					//	new TranslateAutomaticallyParameters(pText,vecRange,pEVGerman,
					//	pFrame->m_hWnd,this->m_hWnd,hWnd,hWnd,hWnd);
					//::DuplicateHandle
					TranslateAutomaticallyParameters * pTranslateAutomaticallyParameters=
						new TranslateAutomaticallyParameters(m_pText,vecRange,pEVGerman,
						pFrame->GetSafeHwnd(),pEVGerman->GetSafeHwnd(),pFrame->m_Progress.m_hWnd,//GetSafeHwnd(),
						pFrame->m_SentenceProgress.GetSafeHwnd(),pFrame->m_TextProgress.GetSafeHwnd());
					pTranslateAutomaticallyParameters->m_hWndStatusBar=
						pFrame->m_wndStatusBar.GetSafeHwnd();
					pTranslateAutomaticallyParameters->
						m_bAdoptGrammaticallyIncorrectSentences=pFrame->
						m_bAdoptGrammaticallyIncorrectSentences;
					pTranslateAutomaticallyParameters->
						m_bAllowLowLetterAtTheBeginningOfSentence=pFrame->
						m_bAllowLowLetterAtTheBeginningOfSentence;
					pTranslateAutomaticallyParameters->m_bCaseSensitive=pFrame->m_bCaseSensitive;
					pTranslateAutomaticallyParameters->m_dwMaxCharsPerLine=pFrame->m_dwMaxCharsPerLine;
					//TranslateAutomaticallyParameters translateAutomaticallyParameters;
					//translateAutomaticallyParameters.m_hWndProgress=pFrame->m_Progress.m_hWnd;
					//g_bContinue=TRUE;
					//pFrame->m_pTranslationThread=AfxBeginThread(TranslateAutomatically,
					//	pTranslateAutomaticallyParameters);
					int nPriority=0;
					switch(pFrame->m_nThreadPriority)
					{
						case 0: nPriority=THREAD_PRIORITY_IDLE;
							break;
						case 1: nPriority=THREAD_PRIORITY_LOWEST;
							break;
						case 2: nPriority=THREAD_PRIORITY_BELOW_NORMAL;
							break;
						case 3: nPriority=THREAD_PRIORITY_NORMAL;
							break;
						case 4: nPriority=THREAD_PRIORITY_ABOVE_NORMAL;
							break;
						case 5: nPriority=THREAD_PRIORITY_HIGHEST;
							break;
					}
					//pFrame->m_pTranslationThread->m_bAutoDelete=FALSE;
					g_bTranslateAutomaticallyBeforeEnd=0;
					ColorText ct;
					DWORD dwNewLines=0L;
					ct.color=0;
					ct.str=CString((TCHAR)nChar);
		//m_ColorText.push_back(ct);
		//m_Text.Insert(0,nChar);
		//TRACE("\nnChar: %d\n",nChar);
					if(m_pText->m_first) // Text schwarz färben
					{
						TextNode * current=m_pText->m_first;
						while(1)
						{
							current->color=0;
							if(current->ch==13)
								dwNewLines++;
							if(current->m_next)
								current=current->m_next;
							else
								break;
						}
					}
					m_pText->GetLength();
					if(m_bMark) // wenn Text markiert wurde
					{
						DWORD dwMarkingStart=m_dwMarkingStart<m_dwMarkingEnd?m_dwMarkingStart:m_dwMarkingEnd;
						DWORD dwMarkingEnd=m_dwMarkingStart>m_dwMarkingEnd?m_dwMarkingStart:m_dwMarkingEnd;
						m_pText->Delete(dwMarkingStart,dwMarkingEnd-dwMarkingStart);
						m_bMark=FALSE;
						m_CursorPos=dwMarkingStart;
					}
					m_pText->GetLength();
					// Zeichen einfügen mit schwarzer Farbe (0)
					m_pText->Insert(m_CursorPos++,(char)nChar,0);
					m_pText->GetLength();
					//der Cursor soll nach dem Einfügen erscheinen (wenn m_bDrawCursor nicht 
					//auf TRUE gesetzt wird, ist der Cursor eventuell nicht sichtbar, weil
					//er beim Blinken in der Phase des Nicht-Anzeigens ist)
					m_bDrawCursor=TRUE;
					// neuzeichnen
					Invalidate();
					//Invalidate();
					g_bContinue=TRUE;

					pFrame->m_pTranslationThread=AfxBeginThread(TranslateAutomatically,
						pTranslateAutomaticallyParameters,nPriority,0,0,NULL);
				}
			}
			//wenn die automatische Übersetzung nicht aktiviert ist
			else
			{
				if(m_bMark) // wenn Text markiert wurde
				{
					DWORD dwMarkingStart=m_dwMarkingStart<m_dwMarkingEnd?m_dwMarkingStart:m_dwMarkingEnd;
					DWORD dwMarkingEnd=m_dwMarkingStart>m_dwMarkingEnd?m_dwMarkingStart:m_dwMarkingEnd;
					m_pText->Delete(dwMarkingStart,dwMarkingEnd-dwMarkingStart);
					m_bMark=FALSE;
					m_CursorPos=dwMarkingStart;
				}
				// Zeichen einfügen mit schwarzer Farbe (0)
				m_pText->Insert(m_CursorPos++,(char)nChar,0);
				//der Cursor soll nach dem Einfügen erscheinen (wenn m_bDrawCursor nicht 
				//auf TRUE gesetzt wird, ist der Cursor eventuell nicht sichtbar, weil
				//er beim Blinken in der Phase des Nicht-Anzeigens ist)
				m_bDrawCursor=TRUE;
				// neuzeichnen
				Invalidate();
			}
		}
	}
	TRACE("m_CursorPos: %u\n",m_CursorPos);
#ifdef _DEBUG
	printf("void EnglishView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)-ENDE\n");
#endif
}

void EnglishView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
#ifdef _DEBUG
	printf("void EnglishView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)-ANFANG\n");
	printf("nChar %u\n",nChar);
	printf("m_CursorPos: %u\n",m_CursorPos);
#endif
	//::OnKeyDown(nChar, nRepCnt, nFlags);

	//wenn die Rückschritt-Taste (Backspace, <--) gedrückt wurde
	if(nChar==8)
	{
		CVTransApp * pApp=(CVTransApp *)AfxGetApp();
		CMainFrame * pFrame=NULL;
		if(pApp)
		{
			pFrame=(CMainFrame *)pApp->m_pMainWnd;
			if(pFrame)
			{
				//der Übersetzungsthread sollte vor dem Löschen eines oder mehrerer
				//Zeichen (wenn mehrere Zeichen markiert sind) beendet werden, damit
				//nicht während der Übersetzung der Textinhalt verändert wird, auf den
				//die Übersetzungsfunktion auch zugreift
				if(pFrame->m_pTranslationThread)
				{
					g_bContinue=FALSE;
					//Mit der Funktion "WaitForSingleObject" gelingt die Textveränderung
					//während der Übersetzung viel flüssiger als mit einer Schleife, in
					//der ständig mittels "::GetExitCodeThread"überprüft wird, ob der 
					//Thread schon beendet wurde. Die Funktion "WaitForSingleObject"
					//scheint also den Prozessor weniger zu belasten, was die MSDN 
					//Bibliothek (also die Dokumentation/Hilfe zu Visual C++) belegt:
					//"The thread consumes very little processor time while waiting for
					//the object state to become signaled or the time-out interval to elapse."
					//warten, bis der Übersetzungsthread beendet ist
					WaitForSingleObject(pFrame->m_pTranslationThread->m_hThread,INFINITE);
				}
				//wenn Text markiert ist
				if(m_bMark)
				{
#ifdef _DEBUG
					printf("m_bMark\n");
#endif
					DWORD dwMarkingStart=m_dwMarkingStart<m_dwMarkingEnd?
						m_dwMarkingStart:m_dwMarkingEnd;
					DWORD dwMarkingEnd=m_dwMarkingStart>m_dwMarkingEnd?
						m_dwMarkingStart:m_dwMarkingEnd;
					m_pText->Delete(dwMarkingStart,dwMarkingEnd-dwMarkingStart);
					m_bMark=FALSE;
					m_CursorPos=dwMarkingStart;
				}// Ende von: wenn markiert
				//wenn kein Text markiert ist
				else
				{
					if(m_CursorPos>0L)
						m_pText->Delete(--m_CursorPos);
				}
				//der Cursor soll nach dem Einfügen erscheinen (wenn m_bDrawCursor nicht 
				//auf TRUE gesetzt wird, ist der Cursor eventuell nicht sichtbar, weil
				//er beim Blinken in der Phase des Nicht-Anzeigens ist)
				m_bDrawCursor=TRUE;
				//neuzeichnen
				Invalidate();
				if(m_pText->m_first) // Text schwarz färben
				{
					TextNode * current=m_pText->m_first;
					while(1)
					{
						TRACE("in der Schleife\n");
						current->color=0;
						if(current->m_next)
							current=current->m_next;
						else
							break;
					}
				}
				//wenn dieses Textfeld das englische Textfeld ist
				if(bEnglishPart)
				{
					//die Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
					//wird nicht direkt nach der Funktion TranslateAutomatically()
					//ausgeführt, wenn diese Funktion (EnglishView::OnChar()) vor 
					//der Beendigung der Funktion TranslateAutomatically() ausgeführt
					//wird.
					//wenn die Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
					//nach der Übersetzung noch nicht ausgeführt wurde, wird folgender
					//Code ausgeführt:
					if(!g_bExecutedOnTranslateAutomaticallyFinishes && pFrame->m_bCreated)
					{
						pFrame->KillTimer(1);
						//if(pFrame->m_bShowClauseProgress)
						//{
							if(pFrame->m_bShowGraphicClauseProgress)
								pFrame->m_Progress.DestroyWindow();
							if(pFrame->m_bShowNumericClauseProgress)
								pFrame->m_cStaticClauseProgress.DestroyWindow();
						//}
						//if(pFrame->m_bShowSentenceProgress)
						//{
							if(pFrame->m_bShowGraphicSentenceProgress)
								pFrame->m_SentenceProgress.DestroyWindow();
							if(pFrame->m_bShowNumericSentenceProgress)
								pFrame->m_cStaticSentenceProgress.DestroyWindow();
						//}
						//if(pFrame->m_bShowTextProgress)
						//{
							if(pFrame->m_bShowGraphicTextProgress)
								pFrame->m_TextProgress.DestroyWindow();
							if(pFrame->m_bShowNumericTextProgress)
								pFrame->m_cStaticTextProgress.DestroyWindow();
						//}
						if(pFrame->m_bShowPrognosticedRemainingTime)
							pFrame->m_cStaticRemainingTime.DestroyWindow();
						pFrame->m_bCreated=FALSE;
						pFrame->m_bMayStartThread=TRUE;
						//damit das Zerstören der Fortschrittsbalken (CProgressCtrl)
						//nicht in der Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
						//ausgeführt wird (sie wurden ja bereits über diesem Kommentar mit
						//DestroyWindow() zerstört und sollen nicht doppelt zerstört werden), 
						//wird g_bExecutedOnTranslateAutomaticallyFinishes auf TRUE gesetzt
						//wenn die Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
						//ausgeführt wird, wird g_bExecutedOnTranslateAutomaticallyFinishes 
						//ausgewertet.
						g_bExecutedOnTranslateAutomaticallyFinishes=TRUE;
					}
					CToolBarCtrl & toolBarCtrl=pFrame->m_wndToolBar.GetToolBarCtrl();
					//wenn die automatische Übersetzung aktiviert ist
					if(toolBarCtrl.IsButtonChecked(ID_TRANSLATE_AUTOMATICALLY_IF_TEXTMODIFICATION_BUTTON))
					{
						//CToolBarCtrl & toolBarCtrl=pFrame->m_wndToolBar.GetToolBarCtrl();
						//toolBarCtrl.HideButton(ID_TRANSLATE_BUTTON,TRUE);
						//Text * pText=new Text(*m_pText); // Kopie anlegen
						std::vector<Range> vecRange;
						CChildFrame * pChild=(CChildFrame*)pFrame->MDIGetActive();
						EnglishView * pEVGerman=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
						if(pEVGerman)
						{
							pEVGerman->m_bDrawVector=TRUE;
							if(pFrame->m_bCreated == FALSE)
							{
								BYTE bIDCount=0;//3;
								std::vector<UINT> vecUINT;
								if(pFrame->m_bShowClauseProgress)
								{
									if(pFrame->m_bShowNumericClauseProgress)
									{
										bIDCount++;
										vecUINT.push_back(ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE);
									}
									if(pFrame->m_bShowGraphicClauseProgress)
									{
										bIDCount++;
										vecUINT.push_back(ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
									}
										//bIDCount++;
										//vecUINT.push_back(ID_INDICATOR_CLAUSE_PROGRESS_PANE);
								}
								if(pFrame->m_bShowSentenceProgress)
								{
									if(pFrame->m_bShowNumericSentenceProgress)
									{
										bIDCount++;
										vecUINT.push_back(ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE);
									}
									if(pFrame->m_bShowGraphicSentenceProgress)
									{
											bIDCount++;
											vecUINT.push_back(ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
									}
									//bIDCount++;
									//vecUINT.push_back(ID_INDICATOR_SENTENCE_PROGRESS_PANE);
								}
								if(pFrame->m_bShowTextProgress)
								{
									if(pFrame->m_bShowNumericTextProgress)
									{
										bIDCount++;
										vecUINT.push_back(ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE);
									}
									if(pFrame->m_bShowGraphicTextProgress)
									{
										bIDCount++;
										vecUINT.push_back(ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
									}
									//bIDCount++;
									//vecUINT.push_back(ID_INDICATOR_TEXT_PROGRESS_PANE);
								}
								if(pFrame->m_bShowPrognosticedRemainingTime)
								{
									//MessageBeep((WORD)-1);
									bIDCount++;
									vecUINT.push_back(ID_INDICATOR_REMAINING_TIME);
								}
								if(vecUINT.size()>0)
								{
									UINT * lpIDArray=new UINT[bIDCount];
									for(BYTE bRun=0;bRun<vecUINT.size();bRun++)
										lpIDArray[bRun]=vecUINT.at(bRun);
									//lpIDArray[vecUINT.size()]=ID_INDICATOR_CAPS;
									//lpIDArray[vecUINT.size()+1]=ID_INDICATOR_NUM;
									//lpIDArray[vecUINT.size()+2]=ID_INDICATOR_SCRL;
									pFrame->m_wndStatusBar.SetIndicators(lpIDArray,sizeof(lpIDArray)/sizeof(UINT));
								}
								CStatusBarCtrl & statusbarCtrl=pFrame->m_wndStatusBar.GetStatusBarCtrl();
								//int nHorz=0,nVert=0,nSpacing=0;
								//statusbarCtrl.GetBorders(nHorz,nVert,nSpacing);
								CRect rect,rect2;
								//pFrame->m_wndStatusBar.GetWindowRect(rect);
								pFrame->m_wndStatusBar.GetClientRect(rect2);
#ifdef _DEBUG
								printf("lpRect->bottom: %d lpRect->left %d lpRect->right %d lpRect->top %d\n",rect.bottom,rect.left,rect.right,rect.top);
								printf("lpRect->bottom: %d lpRect->left %d lpRect->right %d lpRect->top %d\n",rect2.bottom,rect2.left,rect2.right,rect2.top);
								//printf("nHorz: %d nVert: %d nSpacing: %d\n",nHorz,nVert,nSpacing);
#endif
								if(bIDCount>0)
								{
									if(pFrame->m_bShowPrognosticedRemainingTime)
									{
										int * Widths=new int[bIDCount];
										for(BYTE bRun=0;bRun<bIDCount;bRun++)
											Widths[bRun]=rect2.right*(bRun+1)/bIDCount;
										//int Widths []={rect2.right/3,rect2.right*2/3,rect2.right};
										statusbarCtrl.SetParts(bIDCount,Widths);
									}
									else
									{
										int * Widths=new int[bIDCount];
										for(BYTE bRun=0;bRun<bIDCount;bRun++)
											Widths[bRun]=rect2.right*(bRun+1)/bIDCount;
										//int Widths []={rect2.right/3,rect2.right*2/3,rect2.right};
										statusbarCtrl.SetParts(bIDCount,Widths);
									}
								}
								bIDCount=0;
								RECT MyRect;
#ifdef _DEBUG
								printf("pFrame->m_bShowClauseProgress\n");
#endif
								if(pFrame->m_bShowNumericClauseProgress)
								{
									pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
									pFrame->m_cStaticClauseProgress.Create("",WS_VISIBLE|WS_CHILD,
										MyRect,&pFrame->m_wndStatusBar,ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE);
								}
								if(pFrame->m_bShowGraphicClauseProgress)
								{
									pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
									if(pFrame->m_nGraphicClauseProgressType==0)
										pFrame->m_Progress.Create(WS_VISIBLE|WS_CHILD,MyRect,
											&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
									if(pFrame->m_nGraphicClauseProgressType==1)
										pFrame->m_Progress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
											&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
									pFrame->m_Progress.SetRange(0,100); //Set the range to between 0 and 100
									pFrame->m_Progress.SetStep(1); // Set the step amount
								}
#ifdef _DEBUG
								printf("pFrame->m_bShowSentenceProgress\n");
#endif
								if(pFrame->m_bShowNumericSentenceProgress)
								{
									pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
									pFrame->m_cStaticSentenceProgress.Create("",WS_VISIBLE|WS_CHILD,
										MyRect,&pFrame->m_wndStatusBar,ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE);
								}
								if(pFrame->m_bShowGraphicSentenceProgress)
								{
									pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
									if(pFrame->m_nGraphicSentenceProgressType==0)
										pFrame->m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD,MyRect,
											&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
									if(pFrame->m_nGraphicSentenceProgressType==1)
										pFrame->m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
											&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
									pFrame->m_SentenceProgress.SetRange(0,100); //Set the range to between 0 and 100
									pFrame->m_SentenceProgress.SetStep(1); // Set the step amount
								}
								if(pFrame->m_bShowNumericTextProgress)
								{
									pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
									pFrame->m_cStaticTextProgress.Create("",WS_VISIBLE|WS_CHILD,
										MyRect,&pFrame->m_wndStatusBar,ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE);
								}
								if(pFrame->m_bShowGraphicTextProgress)
								{
									pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
									if(pFrame->m_nGraphicTextProgressType==0)
										pFrame->m_TextProgress.Create(WS_VISIBLE|WS_CHILD,MyRect,
											&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
									if(pFrame->m_nGraphicTextProgressType==1)
										pFrame->m_TextProgress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
											&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
									pFrame->m_TextProgress.SetRange(0,100); //Set the range to between 0 and 100
									pFrame->m_TextProgress.SetStep(1); // Set the step amount
								}
								if(pFrame->m_bShowPrognosticedRemainingTime)
								{
									pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
									pFrame->m_cStaticRemainingTime.Create("",WS_VISIBLE|
										WS_CHILD,MyRect,&pFrame->m_wndStatusBar,
										ID_INDICATOR_REMAINING_TIME);
									tAtTheBeginningOfTheTranslation=(CTime)CTime::GetCurrentTime();
								}
								LPCTSTR str="Fortschritt des Haupt- oder Nebensatzes";
								//statusbarCtrl.SetTipText(0,str);
								//statusbarCtrl.SetTipText(1,_T("Fortschritt des aktuellen Satzes"));
								//statusbarCtrl.SetTipText(2,_T("Fortschritt des ganzen Textes"));
								pFrame->m_bCreated = TRUE;
								if(pFrame->SetTimer(1,1000,NULL)==0)
								{
#ifdef _DEBUG
									printf("pFrame->SetTimer(1,1000,NULL)==0\n");
#endif
								}
								else
								{
#ifdef _DEBUG
									printf("pFrame->SetTimer(1,1000,NULL)!=0\n");
#endif
								}
								pFrame->m_bTimerIntalled=TRUE;
							}
							//HWND hWnd;
							//TranslateAutomaticallyParameters * pTranslateAutomaticallyParameters=
							//	new TranslateAutomaticallyParameters(pText,vecRange,pEVGerman,
							//	pFrame->m_hWnd,this->m_hWnd,hWnd,hWnd,hWnd);
							TranslateAutomaticallyParameters * 
								pTranslateAutomaticallyParameters=
								new TranslateAutomaticallyParameters(m_pText,vecRange,
								pEVGerman,pFrame->m_hWnd,pEVGerman->GetSafeHwnd(),
								pFrame->m_Progress.GetSafeHwnd(),pFrame->m_SentenceProgress.
								GetSafeHwnd(),pFrame->m_TextProgress.GetSafeHwnd());
							pTranslateAutomaticallyParameters->m_bCaseSensitive=
								pFrame->m_bCaseSensitive;
							pTranslateAutomaticallyParameters->m_dwMaxCharsPerLine=
								pFrame->m_dwMaxCharsPerLine;
							g_bContinue=TRUE;
							int nPriority=0;
							switch(pFrame->m_nThreadPriority)
							{
								case 0: nPriority=THREAD_PRIORITY_IDLE;
									break;
								case 1: nPriority=THREAD_PRIORITY_LOWEST;
									break;
								case 2: nPriority=THREAD_PRIORITY_BELOW_NORMAL;
									break;
								case 3: nPriority=THREAD_PRIORITY_NORMAL;
									break;
								case 4: nPriority=THREAD_PRIORITY_ABOVE_NORMAL;
									break;
								case 5: nPriority=THREAD_PRIORITY_HIGHEST;
									break;
							}
							pFrame->m_pTranslationThread=AfxBeginThread(TranslateAutomatically,
								pTranslateAutomaticallyParameters,nPriority,0,0,NULL);
						}
					}
				}
			}
		}
		return; // Invalidate() unten in dieser Funktion überspringen
	}
	// wenn die Taste 'Ende' gedrückt würde
	else if(nChar==35)
	{
		CVTransApp * pApp=(CVTransApp *)AfxGetApp();
		CMainFrame * pFrame=NULL;
		if(pApp)
		{
			pFrame=(CMainFrame *)pApp->m_pMainWnd;
			if(pFrame)
			{
				if(pFrame->m_bShiftDown)
				{
#ifdef _DEBUG
					printf("pFrame->m_bShiftDown\n");
#endif
					if(!m_bMark)
					{
						m_dwMarkingStart=m_CursorPos;
						m_bMark=TRUE;
					}
				}
				else
				{
					m_bMark=FALSE;
					m_dwMarkingStart=0;
					m_dwMarkingEnd=0;
				}
			}
		}
		if(m_pText->m_first)
		{
			BYTE bAssignedLineBeforeCursor=0;
			//gibt an, ob eine neue Cursorposition zugewiesen wurde
			BYTE bNewCursorPositionAssigned=0;
			//gibt an, ob in dem aktuellen Schleifendurchlauf ein manueller
			//Zeilenumbruch (Zeichenfolge carriage return [dezimal 13] newline 
			//[dezimal 10] oder nur ein carriage return) statt fand
			BYTE bNewLine;
			//um Textmaße zu bekommen->Berechnung, in welche sichtbare Zeile 
			//geklickt wurde
			CDC * pDC;
			//Breite in Pixel der Zeichenkette, die die aktuelle Zeile 
			//bis zur aktuellen Position wiederspiegelt
			CSize sizeLine;
			CSize sizeWidthFromLeftClientAreaToCursor;
			CRect rectClient;
			//Zeichenkette, die das aktuelle Token bis zur aktuellen (horizontalen)
			//Position wiederspiegelt
			CString strCurrentToken;
			//Zeichenkette, die die aktuelle Zeile bis zur aktuellen (horizontalen)
			//Position wiederspiegelt
			CString strLine;
			//Zeilennumer der sichtbaren Zeile, in die geklickt wurde; Zeilennummer
			//1 ist die erste sichtbare Zeile
			CString strLineBeforeCursor;
			//DWORD dwBeginOfLineBeforeCursor;
			DWORD dwCurrentPosition=0;
			//aktuelle Zeilennumer der sichtbaren Zeile, Zeilennummer 1 ist die 
			//erste sichtbare Zeile; wird erst hochgezählt, nachdem 
			//dwFirstVisibleLine gleich null ist
			DWORD dwCurrentLineNumberFromFirstVisibleLine=0;
			//die erste Zeile, die sichtbar ist: wenn die Bildlaufleiste nach 
			//unten gescrollt wird, ist die Zeilennummer der ersten sichtbaren Zeile 
			//höher als die Zeilennummer der ersten Zeile
			//m_dwStartLine: Member-Variable der Klasse "EnglishView"
			//dwFirstVisibleLine wird im Folgenden bei jeder neuen Zeile um 1 
			//erniedrigt, wenn dwFirstVisibleLine größer als 0 ist
			DWORD dwFirstVisibleLine=m_dwStartLine;
			//um für den Fall, dass ein automatischer Zeilenumbruch stattfindet
			//und der Mausklick rechts neben dem Zeilenende war den Cursor am
			//Zeilenende zu positionieren
			DWORD dwPositionOfEndOfPreviousLine=0;
			DWORD dwTextLength;
			TextNode * pTextNodeCurrent=NULL;
			TextNode * pTextNodeBeginOfCurrentToken=NULL;
			pTextNodeCurrent=m_pText->m_first;
			pDC=GetDC();
			dwTextLength=m_pText->GetLength();
			//um die Breite des Client-Bereiches zu bekommen
			GetClientRect(rectClient);


			while(1)
			{
				bNewLine=0;
				//wenn carriage return...
				if(pTextNodeCurrent->ch==13)
				{
					//wenn das aktuelle Zeilenende an der selben Position liegt wie 
					//die aktuelle Cursorposition, braucht die Cursorposition NICHT 
					//neu zugewiesen werden
					if(dwCurrentPosition==m_CursorPos)
						break;
					//nur wenn das aktuelle Zeilenende NICHT an der selben Position 
					//liegt wie die aktuelle Cursorposition und wenn das aktuelle 
					//Zeilenende hinter der aktuellen Cursorposition liegt soll eine 
					//neue Cursorposition zugewiesen werden
					if(dwCurrentPosition>m_CursorPos)
					{
						m_CursorPos=dwCurrentPosition;
						break;
					}
					bNewLine=1;
					if(pTextNodeCurrent->m_next)
						//...und das nächste Zeichen newline...
						if(pTextNodeCurrent->m_next->ch==10)
						{
							pTextNodeCurrent=pTextNodeCurrent->m_next;
						}
					strCurrentToken="";
					strLine="";
				}
				//wenn Leerzeichen
				if(pTextNodeCurrent->ch==' ')
				{
	#ifdef _DEBUG
					printf("ptextNodeCurrent->ch==' '\n");
	#endif
					//Zeichenkette des aktuellen Token leeren
					strCurrentToken="";
					strLine+=pTextNodeCurrent->ch;
					//dwPositionOfEndOfPreviousLine=dwNewCursorPosition;
					pTextNodeBeginOfCurrentToken=pTextNodeCurrent->m_next;
				}
				//wenn Zeichen ausser Leerzeichen
				else
				{
					//wenn nicht Zeichenfolge "13" "10" oder Zeichen "13"
					if(!bNewLine)
					{
						//Zeichen an die Zeichenfolge für das aktuelle Token anhängen
						strCurrentToken+=pTextNodeCurrent->ch;
						//Zeichen an die Zeichenfolge der aktuellen Zeile anhängen
						strLine+=pTextNodeCurrent->ch;
					}
				}
				//Breite in Pixel der Zeile ermitteln
				sizeLine=pDC->GetOutputTextExtent(strLine);

				//Wenn ein automatischer Zeilenumbruch stattfindet, weil sonst
				//die Zeile über die vertikale Bildlaufleiste hinaus ginge.
				//Wenn die Breite in Pixel der Zeile größer ist als die Breite des
				//Clientbereiches minus der Breite in Pixel der Bildlaufleiste (15)
				//minus der Breite in Pixel für einen möglichen Cursor (1)
				if(sizeLine.cx>rectClient.right-16)
				{
#ifdef _DEBUG
					printf("sizeLine.cx>rectClient.right-16\n");
#endif
					//wenn die Zeile aus nur einem Token besteht (kein(e) Leerzeichen
					//zwischen den Zeichen)
					if(strLine==strCurrentToken)
					{
						//wenn das aktuelle Zeilenende an der selben Position liegt wie
						//die aktuelle Cursorposition, braucht die Cursorposition NICHT 
						//neu zugewiesen werden
						if(dwCurrentPosition-1==m_CursorPos)
							break;
						//nur wenn das aktuelle Zeilenende NICHT an der selben Position 
						//liegt wie die aktuelle Cursorposition und wenn das aktuelle 
						//Zeilenende hinter der aktuellen Cursorposition liegt soll eine 
						//neue Cursorposition zugewiesen werden
						if(dwCurrentPosition-1>m_CursorPos)
						{
							m_CursorPos=dwCurrentPosition-1;
							break;
						}
						strLine=strLine.Right(1);
						strCurrentToken=strLine;
					}
					//wenn die Zeile aus zwei oder mehreren Token besteht (Leerzeichen
					//zwischen den Zeichen)
					else
					{
						//wenn das aktuelle Zeilenende an der selben Position liegt wie
						//die aktuelle Cursorposition, braucht die Cursorposition NICHT 
						//neu zugewiesen werden
						if((float)dwCurrentPosition-strCurrentToken.GetLength()==
							m_CursorPos)
							break;
						//nur wenn das aktuelle Zeilenende NICHT an der selben Position 
						//liegt wie die aktuelle Cursorposition und wenn das aktuelle 
						//Zeilenende hinter der aktuellen Cursorposition liegt soll eine 
						//neue Cursorposition zugewiesen werden
						if((float)dwCurrentPosition-strCurrentToken.GetLength()>
							m_CursorPos)
						{
							m_CursorPos=dwCurrentPosition-strCurrentToken.GetLength();
							break;
						}
						strLine=strLine.Right(strCurrentToken.GetLength());
					}
				}
				++dwCurrentPosition;
				if(pTextNodeCurrent->m_next)
					pTextNodeCurrent=pTextNodeCurrent->m_next;
				//wenn das Listenende erreicht wurde
				else
				{
					//nur wenn das aktuelle Zeilenende NICHT an der selben Position 
					//liegt wie die aktuelle Cursorposition und wenn das aktuelle 
					//Zeilenende hinter der aktuellen Cursorposition liegt soll eine 
					//neue Cursorposition zugewiesen werden
					if(dwCurrentPosition>=m_CursorPos)
						m_CursorPos=dwCurrentPosition;
					break;
				}
			}
		}
		m_bDrawCursor=TRUE;
		//return; // Invalidate() unten in dieser Funktion überspringen
	}
	//wenn die Taste "Pos 1" gedrückt wurde
	else if(nChar==36)
	{
		CVTransApp * pApp=(CVTransApp *)AfxGetApp();
		CMainFrame * pFrame=NULL;
		if(pApp)
		{
			pFrame=(CMainFrame *)pApp->m_pMainWnd;
			if(pFrame)
			{
				if(pFrame->m_bShiftDown)
				{
					if(!m_bMark)
					{
						m_dwMarkingStart=m_CursorPos;
						m_bMark=TRUE;
					}
				}
				else
				{
					m_bMark=FALSE;
				}
			}
		}
		if(m_pText)
			if(m_pText->m_first)
			{
				DWORD dwHorizontalOffset=0;
				DWORD dwLine=0;
				TRACE("nicht drawvector\n");
				DWORD pos=0;
				TextNode * current=m_pText->m_first;
				DWORD dwNewLines=0;
				DWORD dwRun=pos;
				DWORD dwStart=dwRun;
				BOOL bPreviousCharacterWasCarriageReturn=FALSE;
				TextNode * pNewLine=current;
				BOOL bSeperator=TRUE;
				BOOL bSeperatorOccured=FALSE;
				BOOL bTokendoesntFitIntoLine=FALSE;
				BOOL bDoOutput=FALSE;
				BOOL bSpaceOccured=FALSE;
				CRect clientRect;
				GetClientRect(clientRect);
#ifdef _DEBUG
				printf("clientRect.right-15: %u\n",clientRect.right-15);
#endif
				TextNode * pBeginOfWord=current;
				DWORD dwTokenLength=0;
				DWORD dwTokenWidth=0;
				DWORD dwLineWidth=0;
				DWORD dwMarkingBegin=m_dwMarkingStart<=m_dwMarkingEnd?m_dwMarkingStart:m_dwMarkingEnd;
				DWORD dwMarkingEnd=m_dwMarkingStart>=m_dwMarkingEnd?m_dwMarkingStart:m_dwMarkingEnd;
				TextNode * pBeginOfToken=NULL;//current;
				TextNode * pBeginOfLine=current;
				TextNode * pLineStopChar=current;
				BYTE bType=0;
				BOOL bAssignpBeginOfToken=FALSE; // TRUE: der Zeiger pBeginOfToken soll
				// (current) zugewiesen werden
				BOOL bAssignpBeginOfLine=FALSE;
				DWORD dwPositionAfterOutput=0;
				DWORD dwCharactersTillLastNewLine=0;
				DWORD dwBeginOfLastNewline=0;
				DWORD dwRealPos=0;
				BOOL bAssignpNewLine=FALSE;
				BOOL bNewline=FALSE;
				CDC * pDC=GetDC();
				//DWORD dwCurrentPosition=0;
				while(1)
				{
#ifdef _DEBUG
					if(current)
						printf("current->ch: %u %c dwRun: %u dwRealPos: %u\n",current->ch,current->ch,dwRun,dwRealPos);
					else
						printf("!current dwRun: %u\n",dwRun);
					printf("dwTokenWitdh: %u\n",dwTokenWidth);
#endif
					//if(!current->m_next) // wenn das Listenende erreicht wurde
					//	bSeperator=FALSE;
					if(current)
					{
						if(current->ch==13 || (bPreviousCharacterWasCarriageReturn && current->ch==10) || current->ch==' ')
						{
							bPreviousCharacterWasCarriageReturn=FALSE;
							if(current->ch==' ')//wenn Leerzeichen
							{
								bType=0;
								if(bNewline)
								{
									if(dwRealPos>m_CursorPos)
										break;
									dwBeginOfLastNewline=dwRealPos;
									bNewline=FALSE;
								}
								if(bAssignpNewLine)
								{
									pNewLine=current;
									bAssignpNewLine=FALSE;
								}
								if(bAssignpBeginOfLine)
								{
									pBeginOfLine=current;
									bAssignpBeginOfLine=FALSE;
								}
								bSpaceOccured=TRUE;
								CSize size=pDC->GetOutputTextExtent(
                  //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                  //is faster than creating a CString(current->ch) in order to use 
                  //GetOutputTextExtent(const CString& str ) const; .
                  //&(CHAR)current->ch,1
                  CString((TCHAR)current->ch));
								//dwTokenWidth+=size.cx;
								dwLineWidth+=size.cx;
								//dwLineWidth+=
								if((LONG)(dwHorizontalOffset+dwLineWidth+dwTokenWidth)>clientRect.right-15)
								{ // wenn das Wort nicht mehr in die aktuelle Zeile passt
									bDoOutput=TRUE;
									if((LONG)(dwLineWidth+dwTokenWidth)<=clientRect.right-15) // wenn das Wort in eine Zeile passt
									{
										dwHorizontalOffset=0;
										dwLine++;
										pNewLine=pBeginOfToken;
									}
									else // Token passt nicht in eine Zeile, also alle Zeichen
									{ // bis zum rechten Rand ausgeben
#ifdef _DEBUG
										printf("bTokendoesntFitIntoLine=TRUE\n");
#endif
										bTokendoesntFitIntoLine=TRUE;
										pNewLine=current;
										if(dwRealPos>m_CursorPos)
											break;
										dwBeginOfLastNewline=dwRealPos;
									}
								}
							}
							if(current->ch==13)
							//else
							{
								dwTokenLength=0;
								if(bNewline)
								{
									if(dwRealPos>m_CursorPos)
										break;
									dwBeginOfLastNewline=dwRealPos;
								}
								bNewline=TRUE;
								if(bAssignpNewLine)
								{
									pNewLine=current;
									bAssignpNewLine=FALSE;
								}
								bType=3;
								bAssignpBeginOfToken=TRUE;
								bAssignpBeginOfLine=TRUE;
								if(dwNewLines<m_dwStartLine)
								{
									dwTokenWidth=0;
									dwLineWidth=0;
									dwNewLines++;
									dwHorizontalOffset=0;
								}
								else
								{
									bDoOutput=TRUE;
									pBeginOfToken=NULL;
								}
							}
							if(current->ch==10)
								bType=4;
							bAssignpNewLine=TRUE;
							//bSeperatorOccured=TRUE;
							//bDoOutput=TRUE;
						}
						else
						{
							if(bNewline)
							{
								if(dwRealPos>m_CursorPos)
									break;
								dwBeginOfLastNewline=dwRealPos;
								bNewline=FALSE;
							}
							if(bAssignpNewLine)
							{
								pNewLine=current;
								bAssignpNewLine=FALSE;
							}
							if(bAssignpBeginOfLine)
							{
								pBeginOfLine=current;
								bAssignpBeginOfLine=FALSE;
							}
							if(bSpaceOccured)
							{
#ifdef _DEBUG
								printf("bSpaceOccured\n");
#endif
								pLineStopChar=current;
								pBeginOfToken=current;
								dwLineWidth+=dwTokenWidth;
								dwTokenWidth=0;
								bSpaceOccured=FALSE;
								//dwHorizontalOffset=dwTokenWidth;
								dwTokenLength=1;
							}
							else
								dwTokenLength++;
							bType=CHARACTER;
							CSize size=pDC->GetOutputTextExtent(
                //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                //is faster than creating a CString(current->ch) in order to use 
                //GetOutputTextExtent(const CString& str ) const; .
                //&(CHAR)current->ch,1
                CString((TCHAR)current->ch));
#ifdef _DEBUG
							printf("dwTokenWidth: %u size.cx: %u\n",dwTokenWidth,size.cx);
							printf("dwHorizontalOffset: %u dwLineWidth: %u\n",dwHorizontalOffset,dwLineWidth);
#endif
							dwTokenWidth+=size.cx;
							if((LONG)(dwHorizontalOffset+dwLineWidth+dwTokenWidth)>clientRect.right-15)
							{ // wenn das Token nicht mehr in die aktuelle Zeile passt
								//bSeperator=FALSE;
#ifdef _DEBUG
								printf("das Token passt nicht mehr in die aktuelle Zeile\n");
#endif
								{
									if(dwNewLines==m_dwStartLine)
										bDoOutput=TRUE;
									if((LONG)dwTokenWidth<=clientRect.right-15) // wenn das Token 
									{ // in eine Zeile passt
										//dwHorizontalOffset=0;
										//dwLine++;
										if(dwNewLines<m_dwStartLine)
										{
											pBeginOfLine=pBeginOfToken;
											dwNewLines++;
											dwTokenWidth=0;
											dwLineWidth=0;
											dwHorizontalOffset=0;
										}
										else
										{
											pNewLine=pBeginOfToken;
											if(dwRealPos-dwTokenLength+1>m_CursorPos)//der Beginn der
												break;//neuen Zeile muss vor oder bei der aktuellen Cursor
											//position sein
											dwBeginOfLastNewline=dwRealPos-dwTokenLength+1;
										}
									}
									else // Token passt nicht in eine Zeile, also alle Zeichen
									{ // bis zum rechten Rand ausgeben
										if(dwNewLines<m_dwStartLine)
										{
											pBeginOfLine=current;
											dwNewLines++;
											dwTokenWidth=0;
											dwLineWidth=0;
										}
										else
										{
#ifdef _DEBUG
											printf("bTokendoesntFitIntoLine=TRUE\n");
											printf("dwTokenWidth: %u\n",dwTokenWidth);
#endif
											bTokendoesntFitIntoLine=TRUE;
											pNewLine=current;
											if(dwRealPos>m_CursorPos)//der Beginn der neuen
												break;//Zeile muss vor oder bei der aktuellen
											//Cursorposition sein
											dwBeginOfLastNewline=dwRealPos;
										}
									}
								}
							}
						}
					}
					else
					{
						//m_CursorPos=dwRun;
						m_CursorPos=dwRealPos;
						if(bNewline)
						{
							if(dwRealPos>m_CursorPos)
								break;
							dwBeginOfLastNewline=dwRealPos;
							bNewline=FALSE;
						}
						break;
						bType=2;
						//bFlag=TRUE;
					}
					if(bDoOutput)//!bSeperator)
					{
#ifdef _DEBUG
						printf("bDoOutput\n");
#endif
						if(pBeginOfLine)
						{
#ifdef _DEBUG
							printf("pBeginOfLine\n");
#endif
							CSize size;
							if(pBeginOfToken)
							{
#ifdef _DEBUG
								printf("pBeginOfToken\n");
#endif
								if((LONG)dwTokenWidth<=clientRect.right-15)
								{ // Token passt nicht in eine Zeile
#ifdef _DEBUG
									printf("dwTokenWidth<=clientRect.right-15\n");
#endif
									dwHorizontalOffset=0;
									dwLine++;
									//dwLineWidth=dwTokenWidth;
								}
								dwLineWidth=0;
#ifdef _DEBUG
								printf("dwTokenWidth: %u\n",dwTokenWidth);
#endif
								//}
								pBeginOfLine=pBeginOfToken;
								pBeginOfToken=NULL;
							}
							else
							{
#ifdef _DEBUG
								printf("!pBeginOfToken\n");
#endif					
								//dwLineWidth=0;
								if(bTokendoesntFitIntoLine)
								{
#ifdef _DEBUG
									printf("pBeginOfLine=current\n");
#endif
									pBeginOfLine=current;
								}
								if(bType==3)
								{
									dwTokenWidth=0;
									dwLineWidth=0;
									dwLine++;
									dwHorizontalOffset=0;
									pBeginOfLine=NULL;//FALSE;
								}
							}
							if(bTokendoesntFitIntoLine)
							{
#ifdef _DEBUG
								printf("2. bTokendoesntFitIntoLine\n");
#endif
								dwHorizontalOffset=0;
								dwLine++;
								dwLineWidth=0;
								dwTokenWidth=0;
								bTokendoesntFitIntoLine=FALSE;
								if(bType==CHARACTER)
								{
									CSize size=pDC->GetOutputTextExtent(
                    //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                    //is faster than creating a CString(current->ch) in order to use 
                    //GetOutputTextExtent(const CString& str ) const; .
                    //&(CHAR)current->ch,1
                    CString((TCHAR)current->ch));
									dwTokenWidth=size.cx;
#ifdef _DEBUG
									printf("dwTokenWidth: %u\n",dwTokenWidth);
#endif
								}
							}
							else
							{
								//dwTokenWidth=0;
							}
							bType=0;
							bDoOutput=FALSE;
						}
						else
						{
#ifdef _DEBUG
							printf("!pBeginOfLine\n");
#endif
							if(pBeginOfToken)
							{
#ifdef _DEBUG
								printf("pBeginOfToken\n");
#endif
								CSize size=pDC->GetTextExtent(
                  //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                  //is faster than creating a CString(current->ch) in order to use 
                  //GetOutputTextExtent(const CString& str ) const; .
                  //&(CHAR)current->ch,1
                  CString((TCHAR)current->ch));
								if(bTokendoesntFitIntoLine)
								{
									dwHorizontalOffset=0;
									dwLine++;
									bTokendoesntFitIntoLine=FALSE;
									if(bType==CHARACTER)
									{
										pBeginOfToken=current;
										dwTokenWidth=size.cx;
										//bSeperator=TRUE;
									}
								}
								else
								{
									dwTokenWidth=0;
								}
								bType=0;
								bDoOutput=FALSE;
							}
#ifdef _DEBUG
							else
								printf("!pBeginOfToken\n");
#endif								
						}
						//dwPositionAfterOutput=dwRun;
						dwPositionAfterOutput=dwRealPos;
						//pNewLine=current;
						//dwCharactersTillLastNewLine=dwRun;
						dwCharactersTillLastNewLine=dwRealPos;
					}
					if(!current)//Listenende
						break;
					if(current->ch==13)
						bPreviousCharacterWasCarriageReturn=TRUE;
					current=current->m_next;
					dwRun++;
					//if(dwRun>m_CursorPos)
					//if(dwRealPos>=m_CursorPos)
					//	break;
					if(bType==3 || bType==0 || bType==1)
						dwRealPos++;
				}
				if(m_pText)
				{
					dwPositionAfterOutput=dwRun;
				}
				//m_CursorPos=dwCharactersTillLastNewLine;
				m_CursorPos=dwBeginOfLastNewline;
#ifdef _DEBUG
				printf("neue Cursorposition: %lu\n",m_CursorPos);
#endif
			}
			if(pApp)
			{
				pFrame=(CMainFrame *)pApp->m_pMainWnd;
				if(pFrame)
				{
					if(pFrame->m_bShiftDown)
					{
						if(m_bMark)
						{
							m_dwMarkingEnd=m_CursorPos;
						}
					}
					else
					{
						m_bMark=FALSE;
					}
				}
			}
		m_bDrawCursor=TRUE;
#ifdef _DEBUG
		printf("void EnglishView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)-ENDE\n");
#endif
		//return; // Invalidate() unten in dieser Funktion überspringen
	}
	//wenn die Pfeil-nach-links-Taste gedrückt wurde
	else if(nChar==37)
	{
		CVTransApp * pApp=(CVTransApp *)AfxGetApp();
		CMainFrame * pFrame=NULL;
		if(pApp)
		{
			pFrame=(CMainFrame *)pApp->m_pMainWnd;
			if(pFrame)
			{
				if(pFrame->m_bShiftDown)
				{
					if(!m_bMark)
						m_dwMarkingStart=m_CursorPos;
					m_bMark=TRUE;
				}
				else
				{
					m_bMark=FALSE;
				}
			}
		}
		if(m_CursorPos>0L)
			m_CursorPos--;
		if(pApp)
		{
			pFrame=(CMainFrame *)pApp->m_pMainWnd;
			if(pFrame)
			{
				if(pFrame->m_bShiftDown)
				{
					if(m_bMark)
						m_dwMarkingEnd=m_CursorPos;
					else
						m_dwMarkingStart=m_CursorPos;
				}
				else
				{
					m_bMark=FALSE;
				}
			}
		}
		m_bDrawCursor=TRUE;
		//return; // Invalidate() unten in dieser Funktion überspringen
	}
	//wenn die Pfeil-nach-oben-Taste gedrückt wurde
	else if(nChar==38)
	{
		CMainFrame * pFrame=NULL;
		CVTransApp * pApp=(CVTransApp *)AfxGetApp();
		if(pApp)
		{
			pFrame=(CMainFrame *)pApp->m_pMainWnd;
			if(pFrame)
			{
				if(pFrame->m_bShiftDown)
				{
					if(!m_bMark)
					{
						m_dwMarkingEnd=m_CursorPos;
						m_bMark=TRUE;
					}
				}
				else
				{
					m_bMark=FALSE;
				}
			}
		}
		if(m_pText->m_first)
		{
			BYTE bAssignedLineBeforeCursor=0;
			//gibt an, ob eine neue Cursorposition zugewiesen wurde
			BYTE bNewCursorPositionAssigned=0;
			//gibt an, ob in dem aktuellen Schleifendurchlauf ein manueller
			//Zeilenumbruch (Zeichenfolge carriage return [dezimal 13] newline 
			//[dezimal 10] oder nur ein carriage return) statt fand
			BYTE bNewLine;
			//der vorherige Durchlauf erzeugte eine neue Zeile
			BYTE bPreviousPassWasNewLine=0;
			//um Textmaße zu bekommen->Berechnung, in welche sichtbare Zeile 
			//geklickt wurde
			CDC * pDC;
			//Breite in Pixel der Zeichenkette, die die aktuelle Zeile 
			//bis zur aktuellen Position wiederspiegelt
			CSize sizeLine;
			CSize sizeWidthFromLeftClientAreaToCursor;
			CRect rectClient;
			//Zeichenkette, die das aktuelle Token bis zur aktuellen (horizontalen)
			//Position wiederspiegelt
			CString strCurrentToken;
			//Zeichenkette, die die aktuelle Zeile bis zur aktuellen (horizontalen)
			//Position wiederspiegelt
			CString strLine;
			//Zeilennumer der sichtbaren Zeile, in die geklickt wurde; Zeilennummer
			//1 ist die erste sichtbare Zeile
			CString strLineBeforeCursor;
			DWORD dwBeginOfLineBeforeCursor;
			DWORD dwClickedLineFromFirstVisibleLine;
			DWORD dwCurrentPosition=0;
			//aktuelle Zeilennumer der sichtbaren Zeile, Zeilennummer 1 ist die 
			//erste sichtbare Zeile; wird erst hochgezählt, nachdem 
			//dwFirstVisibleLine gleich null ist
			DWORD dwCurrentLineNumberFromFirstVisibleLine=0;
			//die erste Zeile, die sichtbar ist: wenn die Bildlaufleiste nach 
			//unten gescrollt wird, ist die Zeilennummer der ersten sichtbaren Zeile 
			//höher als die Zeilennummer der ersten Zeile
			//m_dwStartLine: Member-Variable der Klasse "EnglishView"
			//dwFirstVisibleLine wird im Folgenden bei jeder neuen Zeile um 1 
			//erniedrigt, wenn dwFirstVisibleLine größer als 0 ist
			DWORD dwFirstVisibleLine=m_dwStartLine;
			DWORD dwHorizontalOffset=0;
			//um für den Fall, dass ein automatischer Zeilenumbruch stattfindet
			//und der Mausklick rechts neben dem Zeilenende war den Cursor am
			//Zeilenende zu positionieren
			DWORD dwPositionOfEndOfPreviousLine=0;
			DWORD dwTextLength;
			//für die Berechnung, in welche Zeile innerhalb des sichtbaren Bereichs
			//geklickt wurde
			TEXTMETRIC textmetric;
			TextNode * pTextNodeCurrent=NULL;
			TextNode * pTextNodeBeginOfCurrentToken=NULL;
			pTextNodeCurrent=m_pText->m_first;
			pDC=GetDC();
			dwTextLength=m_pText->GetLength();
			// für die Zeilenhöhe
			pDC->GetTextMetrics(&textmetric);
			//Berechnung, in welche Zeile innerhalb des sichtbaren Bereichs 
			//geklickt wurde
			dwClickedLineFromFirstVisibleLine=m_point.y/textmetric.tmHeight;
			//um die Breite des Client-Bereiches zu bekommen
			GetClientRect(rectClient);


			while(1)
			{
				bNewLine=0;
				//wenn carriage return...
				if(pTextNodeCurrent->ch==13)
				{
					bPreviousPassWasNewLine=1;
					//eine neue Zeile beginnt bei der horizontalen Position 
					//(Offset) 0
					dwHorizontalOffset=0;
					bNewLine=1;
					if(pTextNodeCurrent->m_next)
						//...und das nächste Zeichen newline...
						if(pTextNodeCurrent->m_next->ch==10)
						{
							pTextNodeCurrent=pTextNodeCurrent->m_next;
						}
					//nur wenn die aktuelle Position mit Zeilenumbruch vor der 
					//Cursorposition liegt, liegt diese Zeile vor der Zeile mit dem
					//Cursor
					if(dwCurrentPosition<m_CursorPos)
					{
						strLineBeforeCursor=strLine;
						dwBeginOfLineBeforeCursor=dwCurrentPosition-strLine.
							GetLength();
						//man kann nicht sagen, dass wenn strLineBeforeCursor leer ist,
						//strLineBeforeCursor nichts zugewiesen wurde, denn es kann
						//sein, dass der Inhalt einer leeren Zeile zugewiesen wurde.
						//Deswegen wird die folgende Hilfsvariable benutzt
						bAssignedLineBeforeCursor=1;
					}
					//wenn der Cursor in der aktuellen Zeile liegt
					else
					{
						//Berechnung der Breite in Pixel vom linken Rand des 
						//Clientbereiches bis zum Cursor
						//Beispiel: dwCurrentPosition=4, m_CursorPos=0, 
						//strLine.GetLength()=5: 
						//sizeWidthFromLeftClientAreaToCursor=pDC->GetOutputTextExtent(
						//	strLine.Left(dwCurrentPosition-m_CursorPos+1-
						//	strLine.GetLength()));
						sizeWidthFromLeftClientAreaToCursor=pDC->GetOutputTextExtent(
							strLine.Left(strLine.GetLength()-(dwCurrentPosition-
							m_CursorPos)));
#ifdef _DEBUG
						printf("sizeWidthFromLeftClientAreaToCursor.cx: %d\n",
							sizeWidthFromLeftClientAreaToCursor.cx);
#endif
						break;
					}
					strCurrentToken="";
					strLine="";
				}
				//wenn Leerzeichen
				if(pTextNodeCurrent->ch==' ')
				{
	#ifdef _DEBUG
					printf("ptextNodeCurrent->ch==' '\n");
	#endif
					//Zeichenkette des aktuellen Token leeren
					strCurrentToken="";
					strLine+=pTextNodeCurrent->ch;
					//dwPositionOfEndOfPreviousLine=dwNewCursorPosition;
					pTextNodeBeginOfCurrentToken=pTextNodeCurrent->m_next;
				}
				//wenn Zeichen ausser Leerzeichen
				else
				{
					//wenn nicht Zeichenfolge "13" "10" oder Zeichen "13"
					if(!bNewLine)
					{
						//Zeichen an die Zeichenfolge für das aktuelle Token anhängen
						strCurrentToken+=pTextNodeCurrent->ch;
						//Zeichen an die Zeichenfolge der aktuellen Zeile anhängen
						strLine+=pTextNodeCurrent->ch;
					}
				}
				//Breite in Pixel der Zeile ermitteln
				sizeLine=pDC->GetOutputTextExtent(strLine);

				//Wenn ein automatischer Zeilenumbruch stattfindet, weil sonst
				//die Zeile über die vertikale Bildlaufleiste hinaus ginge.
				//Wenn die Breite in Pixel der Zeile größer ist als die Breite des
				//Clientbereiches minus der Breite in Pixel der Bildlaufleiste (15)
				//minus der Breite in Pixel für einen möglichen Cursor (1)
				if(sizeLine.cx>rectClient.right-16)
				{
#ifdef _DEBUG
					printf("sizeLine.cx>rectClient.right-16\n");
#endif
					//wenn die Zeile aus nur einem Token besteht (kein(e) Leerzeichen
					//zwischen den Zeichen)
					if(strLine==strCurrentToken)
					{
						//wenn sich der Cursor erst auf der nächsten Zeile oder noch 
						//später befindet
						if(dwCurrentPosition-1<m_CursorPos)
						{
							strLineBeforeCursor=strLine.Left(strLine.GetLength()-1);
							dwBeginOfLineBeforeCursor=dwCurrentPosition+1-strLine.
								GetLength();
#ifdef _DEBUG
							printf("dwBeginOfLineBeforeCursor: %u\n",
								dwBeginOfLineBeforeCursor);
#endif
							//man kann nicht sagen, dass wenn strLineBeforeCursor leer ist,
							//strLineBeforeCursor nichts zugewiesen wurde, denn es kann
							//sein, dass der Inhalt einer leeren Zeile zugewiesen wurde.
							//Deswegen wird die folgende Hilfsvariable benutzt
							bAssignedLineBeforeCursor=1;
						}
						//wenn sich der Cursor auf der aktuellen Zeile befindet
						else
						{
							//Berechnung der Breite in Pixel vom linken Rand des 
							//Clientbereiches bis zum Cursor
							sizeWidthFromLeftClientAreaToCursor=pDC->GetOutputTextExtent(
								strCurrentToken.Left(strCurrentToken.GetLength()-(
								dwCurrentPosition-m_CursorPos)-1));
#ifdef _DEBUG
							printf("sizeWidthFromLeftClientAreaToCursor.cx: %u\n",
								sizeWidthFromLeftClientAreaToCursor.cx);
#endif
							break;
						}
						strLine=strLine.Right(1);
						strCurrentToken=strLine;
					}
					//wenn die Zeile aus zwei oder mehreren Token besteht (Leerzeichen
					//zwischen den Zeichen)
					else
					{
						//wenn sich der Cursor erst auf der nächsten Zeile oder noch 
						//später befindet
						//if(dwCurrentPosition-strLine.GetLength()-1-(strLine.GetLength()-
						//	strCurrentToken.GetLength())<=m_CursorPos)
						if((float)dwCurrentPosition-strCurrentToken.GetLength()<
							m_CursorPos)
						{
							////wenn die Zeile aus nur einem Token besteht (kein(e) Leerzeichen
							////zwischen den Zeichen)
							//if(strLine==strCurrentToken)
							//	strLineBeforeCursor=strLine.Left(strLine.GetLength()-1);
							//else
								//Länge der aktuellen Zeile 
								//minus Länge des aktuellen Token bis zur aktuellen 
								//(horizontalen) Position minus 1: erhält die Zeile, die auf 
								//dem Bildschirm sichtbar ist, wenn 1) nur ein Token in der 
								//aktuellen Zeile war und 2) wenn 2 oder mehrere Token in der
								//aktuellen Zeile waren
								strLineBeforeCursor=strLine.Left(strLine.GetLength()-
									strCurrentToken.GetLength()-1);
							//dwBeginOfLineBeforeCursor=dwCurrentPosition-strLine.
							dwBeginOfLineBeforeCursor=dwCurrentPosition+1-strLine.
								GetLength();
	#ifdef _DEBUG
							printf("strLine: %s\n",strLine);
							printf("strLine.GetLength(): %d dwCurrentPosition: %u\n",
								strLine.GetLength(),dwCurrentPosition);
							printf("strLineBeforeCursor: %s\n",strLineBeforeCursor);
	#endif

							//man kann nicht sagen, dass wenn strLineBeforeCursor leer ist,
							//strLineBeforeCursor nichts zugewiesen wurde, denn es kann
							//sein, dass der Inhalt einer leeren Zeile zugewiesen wurde.
							//Deswegen wird die folgende Hilfsvariable benutzt
							bAssignedLineBeforeCursor=1;
						}
						//wenn sich der Cursor auf der aktuellen Zeile befindet
						else
					//if((float)dwCurrentPosition-strCurrentToken.GetLength()>=
					//	m_CursorPos)
						{
							//strLine=strLine.Right(strToken.GetLength());
							//Berechnung der Breite in Pixel vom linken Rand des 
							//Clientbereiches bis zum Cursor
							sizeWidthFromLeftClientAreaToCursor=pDC->GetOutputTextExtent(
								strLine.Left(strLine.GetLength()-(
								dwCurrentPosition-m_CursorPos)-1));
#ifdef _DEBUG
							printf("sizeWidthFromLeftClientAreaToCursor.cx: %u\n",
								sizeWidthFromLeftClientAreaToCursor.cx);
#endif
							break;
						}
						strLine=strLine.Right(strCurrentToken.GetLength());
					}
					//else
					//wenn die Zeile aus nur einem Token besteht (kein(e) Leerzeichen
					//zwischen den Zeichen)
					//if(pDC->GetOutputTextExtent(strCurrentToken).cx>rectClient.right-16)
					//{
//#ifdef _DEBUG
					//	printf("pDC->GetOutputTextExtent(strCurrentToken).cx>rectClient.\
//right-16\n");
//#endif
					//}
					//else
						
				}
				++dwCurrentPosition;
				if(pTextNodeCurrent->m_next)
					pTextNodeCurrent=pTextNodeCurrent->m_next;
				//wenn das Listenende erreicht wurde
				else
				{
					//???wenn das aktuelle Token bis zur aktuellen Position nicht mehr???
					//???in die aktuelle Zeile gepasst hat, dann befindet sich der Cursor???
					//???im aktuellen Token???
					sizeWidthFromLeftClientAreaToCursor=pDC->GetOutputTextExtent(
						strLine.Left(strLine.GetLength()-(dwCurrentPosition-
						m_CursorPos)));
#ifdef _DEBUG
					printf("strLine.Left(strLine.GetLength()-(dwCurrentPosition-\
m_CursorPos)): %s\n",strLine.Left(strLine.GetLength()-(dwCurrentPosition-
						m_CursorPos)));
					printf("sizeWidthFromLeftClientAreaToCursor.cx: %d\n",
						sizeWidthFromLeftClientAreaToCursor.cx);
#endif
					break;
				}
			}
			//bAssignedLineBeforeCursor ist nur wahr, wenn der Cursor NICHT in
			//der ersten Zeile stand
			if(bAssignedLineBeforeCursor)
			{
				CSize sizeLineBeforeCursorFromLeft;
				WORD wLineLength=strLineBeforeCursor.GetLength();
#ifdef _DEBUG
				printf("wLineLength: %u\n",wLineLength);
#endif
				//die Länge der Zeile vor dem Cursor 0 ist
				if(wLineLength==0)
					m_CursorPos=dwBeginOfLineBeforeCursor;
				//die Länge der Zeile vor dem Cursor ist größer als 0
				else
				{
					//gibt an, ob eine neue Cursorposition zugewiesen wurde
					BYTE bAssignedNewCursorPosition=0;
					for(WORD wRun=0;wRun<wLineLength;++wRun)
					{
						//Breite in Pixel mit der Anzahl "dwRun" Zeichen von links 
						//der Zeile direkt vor der Zeile mit dem Cursor kriegen
						sizeLineBeforeCursorFromLeft=pDC->GetOutputTextExtent(
							strLineBeforeCursor.Left(wRun+1));
						//wenn die Breite in Pixel mit der Anzahl "dwRun" Zeichen von 
						//links der Zeile direkt vor der Zeile mit dem Cursor größer
						//ist als die Breite in Pixel vom linken Rand des 
						//Clientbereiches bis zum Cursor
						if(sizeLineBeforeCursorFromLeft.cx>
							sizeWidthFromLeftClientAreaToCursor.cx)
							//wenn die Breite in Pixel mit der Anzahl "dwRun+1" Zeichen von 
							//links der Zeile direkt vor der Zeile mit dem Cursor minus 
							//der Breite in Pixel vom linken Rand des Clientbereiches bis 
							//zum Cursor kleiner ist als die Breite in Pixel vom linken 
							//Rand des Clientbereiches bis zum Cursor minus der Breite in 
							//Pixel mit der Anzahl "dwRun" Zeichen von links der Zeile 
							//direkt vor der Zeile mit dem Cursor 
							//Wenn also der Cursor horizontal näher am Ende des Zeichens
							//direkt über dem Cursor ist als am Anfang des Zeichens direkt 
							//über dem Cursor.
							if(sizeLineBeforeCursorFromLeft.cx-
								sizeWidthFromLeftClientAreaToCursor.cx<
								sizeWidthFromLeftClientAreaToCursor.cx-pDC->
								GetOutputTextExtent(strLineBeforeCursor.Left(wRun)).cx)
							{
								bAssignedNewCursorPosition=1;
								m_CursorPos=dwBeginOfLineBeforeCursor+strLineBeforeCursor.
									Left(wRun+1).GetLength();
								break;
							}
							//Wenn der Cursor horizontal näher am Anfang des Zeichens
							//direkt über dem Cursor ist als am Ende des Zeichens direkt 
							//über dem Cursor oder der Cursor gleich nah am Anfang und am
							//Ende des Zeichens direkt über dem Cursor ist (der Cursor ist
							//exakt in der Mitte des Zeichens direkt über dem Cursor)
							else
							{
								bAssignedNewCursorPosition=1;
								m_CursorPos=dwBeginOfLineBeforeCursor+strLineBeforeCursor.
									Left(wRun).GetLength();
#ifdef _DEBUG
								printf("dwBeginOfLineBeforeCursor: %u m_CursorPos: %u\n",
									dwBeginOfLineBeforeCursor,m_CursorPos);
#endif
								break;
							}
					}
					//wenn keine neue Cursorposition zugewiesen wurde, ist die Breite 
					//in Pixel der Zeile über der Zeile mit dem Cursor geringer oder 
					//gleich breit wie die Breite vom linken Rand des Clientbereiches
					//bis zum Cursor
					if(!bAssignedNewCursorPosition)
						m_CursorPos=dwBeginOfLineBeforeCursor+wLineLength;
				}
			}
		}

		//return; // Invalidate() unten in dieser Funktion überspringen
	}
	//wenn die Pfeil-nach-rechts-Taste gedrückt wurde
	else if(nChar==39)
	{
		CVTransApp * pApp=(CVTransApp *)AfxGetApp();
		CMainFrame * pFrame=NULL;
		if(pApp)
		{
			pFrame=(CMainFrame *)pApp->m_pMainWnd;
			if(pFrame)
			{
				if(pFrame->m_bShiftDown)
				{
					if(!m_bMark)
						m_dwMarkingStart=m_CursorPos;
					m_bMark=TRUE;
				}
				else
				{
					m_bMark=FALSE;
				}
			}
		}
		if(m_pText->m_first)
		{
			BOOL bPreviousCharacterWasCarriageReturn=FALSE;
			DWORD dwCount=0;
			TextNode * pTextNodeCurrent=m_pText->m_first;
			while(pTextNodeCurrent)
			{
				if(pTextNodeCurrent->ch==10)
				{
					if(!bPreviousCharacterWasCarriageReturn)
						dwCount++;
					else
						bPreviousCharacterWasCarriageReturn=FALSE;
				}
				else
				{
					if(pTextNodeCurrent->ch==13)
						bPreviousCharacterWasCarriageReturn=TRUE;
					else
						bPreviousCharacterWasCarriageReturn=FALSE;
					dwCount++;
				}
				pTextNodeCurrent=pTextNodeCurrent->m_next;
			}
			if(m_CursorPos<dwCount)
				m_CursorPos++;
		}
		if(pApp)
		{
			pFrame=(CMainFrame *)pApp->m_pMainWnd;
			if(pFrame)
			{
				if(pFrame->m_bShiftDown)
				{
					if(m_bMark)
						m_dwMarkingEnd=m_CursorPos;
					else
						m_dwMarkingStart=m_CursorPos;
					m_bMark=TRUE;
				}
				else
				{
					m_bMark=FALSE;
				}
			}
		}
		m_bDrawCursor=TRUE;
		//return; // Invalidate() unten in dieser Funktion überspringen
	}
	//wenn die Pfeil-nach-unten-Taste gedrückt wurde
	//(wenn Cursor nach unten)
	else if(nChar==40)
	{
		BOOL bPreviousCharacterWasCarriageReturn=FALSE;
		BOOL bSeperator=TRUE;
		CDC * pDC=GetDC();
		CRect clientRect;
		TextNode * pTextNodeCurrent=NULL;
		CVTransApp * pApp=(CVTransApp *)AfxGetApp();
		CMainFrame * pFrame=NULL;
		if(pApp)
		{
			pFrame=(CMainFrame *)pApp->m_pMainWnd;
			if(pFrame)
			{
				if(pFrame->m_bShiftDown)
				{
					if(!m_bMark)
					{
						m_dwMarkingStart=m_CursorPos;
						m_bMark=TRUE;
					}
				}
				else
				{
					m_bMark=FALSE;
				}
			}
		}
		GetClientRect(clientRect);
		if(m_pText->m_first)
		{
			BOOL bPreviousCharacterWasCarriageReturn=FALSE;
			BOOL bBreakLoop=FALSE;
			BYTE bCursorAtTheBeginningOfLine=0;
			BYTE bFounddwPixelFromLeftClientEdgeToCursor=0;
			BYTE bNewLine;
			BYTE bNewLineAfterCursor=0;
			//der vorherige Durchlauf erzeugte eine neue Zeile
			BYTE bPreviousPassWasNewLine=0;
			CSize sizeLine;
			CString strCurrentToken;
			CString strLine;
			DWORD dwCurrentPosition=0;
			DWORD dwPositionOfEndOfPreviousLine=0;
			DWORD dwPixelFromLeftClientEdgeToCursor=0;
			TextNode * pTextNodeBeginOfCurrentToken=NULL;
			bSeperator=TRUE;
			pTextNodeCurrent=m_pText->m_first;
			//sowohl die Zeichenfolge "carriage return" "newline" ("13" "10") als
			//auch eine einzelne "10" leiten eine neue Zeile ein.
			//aber auch wenn ein Token nicht mehr in eine Zeile passt, aber in die
			//Nächste oder ein Token so lang ist, dass es noch nicht mal in eine
			//leere Zeile passt, wird eine neue Zeile eingeleitet
			//es soll versucht werden, die horizontale Position möglichst 
			//beizubehalten
			while(pTextNodeCurrent)
			{
				bNewLine=0;
				if(dwCurrentPosition==m_CursorPos)
					dwPixelFromLeftClientEdgeToCursor=sizeLine.cx;
				if(bPreviousPassWasNewLine)
				{
					strLine="";
					bPreviousPassWasNewLine=0;
				}
				//wenn der Cursor am Zeilenanfang steht,...
				if(dwCurrentPosition==m_CursorPos && strLine=="")
				{
					//...muss das vermerkt werden, da sonst nur geprüft wird, ob
					//die Breite in Pixel bis zum aktuellen Zeichen der aktuellen 
					//Zeile breiter ist als die Breite in Pixel zwischen dem linken 
					//Rand des Client-Bereiches und der horizontalen Position des 
					//Cursors (wenn der Cursor am Zeilenanfang steht, ist die Breite 
					//in Pixel bis zum aktuellen Zeichen der aktuellen Zeile "0")
					//um die neue Cursorpoition zuzuweisen
					bCursorAtTheBeginningOfLine=1;
				}
				//wenn die Cursorposition der Ansicht mit der Position in diesem
				//Algorithmus übereinstimmt...
				//if(dwCurrentPosition==m_CursorPos)
					//...dann den Abstand von der linken Seite des Clientbereiches 
					//bis zum Cursor in Pixeln speichern
				//	dwPixelFromLeftClientEdgeToCursor=pDC->
				//		GetOutputTextExtent(strLine).cx;
				//if(dwCurrentPosition>m_CursorPos && bCursorAtTheBeginningOfLine)
				//{
				//}
				//wenn carriage return...
				if(pTextNodeCurrent->ch==13)
				{
					//if(dwCurrentPosition>m_CursorPos)
					//{
					//	m_CursorPos=dwCurrentPosition;
					//	break;
					//}
					//wenn in der Zeile des Cursors ein Zeilenende ist und direkt 
					//danach auch
					if(bCursorAtTheBeginningOfLine)
					{
						m_CursorPos=dwCurrentPosition+1;
						break;
					}
					//wenn nach dem Cursor bereits ein Zeilenende war, liegt das 
					//aktuelle Zeilenende weiter links als die horizontale Position
					//des Text-Cursors, der sich eine Zeile weiter oben befindet
					if(bNewLineAfterCursor)
					{
						m_CursorPos=dwCurrentPosition;
						break;
					}
					//if(!bNewLineAfterCursor && bFounddwPixelFromLeftClientEdgeToCursor)
					if(dwCurrentPosition>=m_CursorPos)
					{
#ifdef _DEBUG
						printf("!bNewLineAfterCursor && bFounddwPixelFromLeftClient\
EdgeToCursor\n");
#endif
						bNewLineAfterCursor=1;
					}
					strLine="";
					bPreviousPassWasNewLine=1;
					/*if(!bFounddwPixelFromLeftClientEdgeToCursor)
						dwPixelFromLeftClientEdgeToCursor=0;*/
					bNewLine=1;
					if(pTextNodeCurrent->m_next)
						//...und das nächste Zeichen newline...
						if(pTextNodeCurrent->m_next->ch==10)
						{
							pTextNodeCurrent=pTextNodeCurrent->m_next;
						}
				}
				//wenn Leerzeichen
				if(pTextNodeCurrent->ch==' ')
				{
	#ifdef _DEBUG
					printf("pTextNodeCurrent->ch==' '\n");
	#endif
					//Zeichenkette des aktuellen Token leeren
					strCurrentToken="";
					//Zeichen der Zeichenkette, die die aktuelle Zeile repräsentiert,
					//hinzufügen
					strLine+=pTextNodeCurrent->ch;
					//dwPositionOfEndOfPreviousLine=dwCurrentPosition;
					//Adresse des Beginns des folgenden Tokens speichern, um sie
					//pTextNodeCurrent zuzuweisen, falls ein automatischer 
					//Zeilenumbruch stattfindet, wo die akuelle Zeichenkette des 
					//aktuellen Token in eine einzige Zeile passt
					pTextNodeBeginOfCurrentToken=pTextNodeCurrent->m_next;
				}
				//wenn Zeichen ausser Leerzeichen
				else
				{
					//wenn nicht Zeichenfolge "13" "10" oder Zeichen "10"
					if(!bNewLine)
					{
						//Zeichen an die Zeichenfolge anhängen
						strCurrentToken+=pTextNodeCurrent->ch;
						//Zeichen an die Zeichenfolge anhängen
						strLine+=pTextNodeCurrent->ch;
					}
				}
				//Breite in Pixel der aktuellen Zeichenkette der aktuellen Zeile 
				//ermitteln
				sizeLine=pDC->GetOutputTextExtent(strLine);
	#ifdef _DEBUG
				printf("sizeLine.cx: %u\n",sizeLine.cx);
	#endif
				//wenn nach dem Cursor bereits ein Zeilenende war und die Breite
				//in Pixel bis zum aktuellen Zeichen der aktuellen Zeile breiter 
				//ist als die Breite in Pixel zwischen dem linken Rand des 
				//Client-Bereiches und der horizontalen Position des Cursors
				if(bNewLineAfterCursor && sizeLine.cx>
					(LONG)dwPixelFromLeftClientEdgeToCursor)
				{
	#ifdef _DEBUG
					printf("bNewLineAfterCursor && sizeLine.cx>\
dwPixelFromLeftClientEdgeToCursor\n");
	#endif
					m_CursorPos=sizeLine.cx-dwPixelFromLeftClientEdgeToCursor<
						dwPixelFromLeftClientEdgeToCursor-(sizeLine.cx-pDC->
						GetOutputTextExtent(
              //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
              //is faster than creating a CString(current->ch) in order to use 
              //GetOutputTextExtent(const CString& str ) const; .
              //&(CHAR)pTextNodeCurrent->ch,1
              CString((TCHAR)pTextNodeCurrent->ch)).cx)?
						dwCurrentPosition+1:dwCurrentPosition;
	#ifdef _DEBUG
					printf("dwCurrentPosition: %d\n",dwCurrentPosition);
	#endif
					break;
				}
				//if(!bFounddwPixelFromLeftClientEdgeToCursor)
				//	dwPixelFromLeftClientEdgeToCursor=sizeLine.cx;

				//wenn ein automatischer Zeilenumbruch stattfindet, weil sonst
				//die Zeile über die vertikale Bildlaufleiste hinaus ginge
				//wenn die Breite in Pixel der Zeile größer ist als die Breite des
				//Clientbereiches minus der Breite in Pixel der Bildlaufleiste (15)
				//minus der Breite in Pixel für einen möglichen Cursor (1)
				if(sizeLine.cx>clientRect.right-16)
				{
					CSize sizeCurrentToken=pDC->GetOutputTextExtent(strCurrentToken);
	#ifdef _DEBUG
					printf("sizeLine.cx>clientRect.right-16\n");
	#endif
					//wenn nach dem Cursor KEINE neue Zeile eingeleitet wurde
					//if(!bNewLineAfterCursor && bFounddwPixelFromLeftClientEdgeToCursor)
					//	bNewLineAfterCursor=1;
					//wenn das aktuelle Token in eine Zeile passt
					//(wenn in der aktuellen Zeile KEIN(E) Leerzeichen ist/sind, passt
					//das aktuelle Token NICHT in eine einzige Zeile)
					//wenn die Breite in Pixel der aktuellen Zeichenkette des aktuellen 
					//Token kleiner gleich ist als die Breite des Clientbereiches 
					//minus der Breite in Pixel der Bildlaufleiste (15)
					//minus der Breite in Pixel für einen möglichen Cursor (1)
					if(sizeCurrentToken.cx<=clientRect.right-16 && strCurrentToken.
						GetLength()>0)
					{
	#ifdef _DEBUG
						printf("sizeCurrentToken.cx<=clientRect.right-16\n");
	#endif
						dwCurrentPosition-=strCurrentToken.GetLength();
						++dwCurrentPosition;
						if(bCursorAtTheBeginningOfLine)
						{
							m_CursorPos=dwCurrentPosition;
							break;
						}
						//auf den Beginn des aktuellen Tokens positionieren (also 
						//innerhalb der verketteten Liste Richtung Listenbeginn zurück-
						//springen)
						pTextNodeCurrent=pTextNodeBeginOfCurrentToken;
						strLine=strCurrentToken.Left(1);
						if(dwCurrentPosition==m_CursorPos)
							bCursorAtTheBeginningOfLine=1;
					}
					//wenn das aktuelle Token NICHT in eine Zeile passt, entspricht
					//das aktuelle Token exakt der aktuellen Zeile
					else
					{
						//wenn der Cursor am Anfang einer Zeile steht und die aktuelle
						//Position hinter der BildschirmCursorposition des Cursors liegt
						if(dwCurrentPosition>m_CursorPos && bCursorAtTheBeginningOfLine)
						{
							m_CursorPos=dwCurrentPosition;
							break;
						}
						strLine=strCurrentToken.Right(1);
						if(dwCurrentPosition==m_CursorPos)
							bCursorAtTheBeginningOfLine=1;
					}
					if(dwCurrentPosition>m_CursorPos)
						bNewLineAfterCursor=1;
					//strLine ist jetzt 1 Zeichen lang
					sizeLine=pDC->GetOutputTextExtent(strLine);
					//wenn nach dem Cursor bereits ein Zeilenende war und die Breite
					//in Pixel der aktuellen Zeile breiter ist als die Breite in Pixel
					//zwischen dem linken Rand des Client-Bereiches und der 
					//horizontalen Position des Cursors
					if(bNewLineAfterCursor && sizeLine.cx>
						(LONG)dwPixelFromLeftClientEdgeToCursor)
					{
						m_CursorPos=sizeLine.cx-dwPixelFromLeftClientEdgeToCursor<
							dwPixelFromLeftClientEdgeToCursor-(sizeLine.cx-pDC->
							GetOutputTextExtent(
                //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
                //is faster than creating a CString(current->ch) in order to use 
                //GetOutputTextExtent(const CString& str ) const; .
                //&(CHAR)pTextNodeCurrent->ch,1
                CString((TCHAR)pTextNodeCurrent->ch)).cx)?
							dwCurrentPosition:dwCurrentPosition-1;
						break;
					}
					//if(!bFounddwPixelFromLeftClientEdgeToCursor)
					//	dwPixelFromLeftClientEdgeToCursor=sizeLine.cx;
				}
				//dwCurrentPosition um 1 erhöhen: bei jedem Listenelement außer 
				//wenn current->ch "13" ist und direkt danach eine "10" folgt
				++dwCurrentPosition;
				//wenn das Listenende noch nicht erreicht ist
				//if(pTextNodeCurrent->m_next)
				pTextNodeCurrent=pTextNodeCurrent->m_next;
				//else
				//{
				//	break;
				//}
			}
			//wenn das Listenende erreicht wurde und nach dem Cursor eine neue 
			//Zeile eingeleitet wurde: das ist der Fall, wenn die Breite in
			//Pixel der Zeile direkt unter der Zeile mit dem Cursor kleiner oder
			//gleich breit in Pixel ist wie der horizontale Abstand vom Cursor 
			//zur linken Client-Seite
			if(!pTextNodeCurrent && bNewLineAfterCursor)
				m_CursorPos=dwCurrentPosition;
		}

		//return; // Invalidate() unten in dieser Funktion überspringen
	}
	//wenn die Entfernen-Taste (Delete, Entf) gedrückt wurde
	else if(nChar==46)
	{
		CVTransApp * pApp=(CVTransApp *)AfxGetApp();
		if(pApp)
		{
			CMainFrame * pFrame=NULL;
			pFrame=(CMainFrame *)pApp->m_pMainWnd;
			if(pFrame)
			{
				if(pFrame->m_pTranslationThread)
				{
					g_bContinue=FALSE;
					//Mit der Funktion "WaitForSingleObject" gelingt die Textveränderung
					//während der Übersetzung viel flüssiger als mit einer Schleife, in
					//der ständig mittels "::GetExitCodeThread"überprüft wird, ob der 
					//Thread schon beendet wurde. Die Funktion "WaitForSingleObject"
					//scheint also den Prozessor weniger zu belasten, was die MSDN 
					//Bibliothek (also die Dokumentation/Hilfe zu Visual C++) belegt:
					//"The thread consumes very little processor time while waiting for
					//the object state to become signaled or the time-out interval to elapse."
					//warten, bis der Übersetzungsthread beendet ist
					WaitForSingleObject(pFrame->m_pTranslationThread->m_hThread,INFINITE);
				}
				if(m_bMark)
				{
					DWORD dwMarkingStart=m_dwMarkingStart<m_dwMarkingEnd?
						m_dwMarkingStart:m_dwMarkingEnd;
					DWORD dwMarkingEnd=m_dwMarkingStart>m_dwMarkingEnd?
						m_dwMarkingStart:m_dwMarkingEnd;
#ifdef _DEBUG
					printf("dwMarkingStart: %u  dwMarkingEnd-dwMarkingStart:%u\n",
						dwMarkingStart,dwMarkingEnd-dwMarkingStart);
#endif
					m_pText->Delete(dwMarkingStart,dwMarkingEnd-dwMarkingStart);

					m_bMark=FALSE;
					m_CursorPos=dwMarkingStart;
				}
				//wenn keine Markierung existiert
				else
				{
					//if(m_CursorPos<m_pText->GetLength())
					//	m_pText->Delete(m_CursorPos);
					DWORD dwTextLength=m_pText->GetLength();
					if(m_pText->m_first && dwTextLength>=m_CursorPos) // wenn Liste nicht leer 
					{
						TextNode * current=m_pText->m_first;
						BOOL bPreviousCharacterWasCarriageReturn=FALSE;
						DWORD dwRun=0;
						while(dwRun<m_CursorPos)
						{
							if(current->ch==13)
								bPreviousCharacterWasCarriageReturn=TRUE;
							else
							{
								if(!(current->ch==10 && bPreviousCharacterWasCarriageReturn==
									TRUE))
									dwRun++;
								bPreviousCharacterWasCarriageReturn=FALSE;
							}
							if(current->m_next)
								current=current->m_next;
							else
								break;
						}
						if(dwRun==m_CursorPos)
						{
							TextNode * previous=NULL;
							TextNode * first=NULL;
							if(current->m_previous)
							{
								previous=current->m_previous;
								first=previous;
							}
							dwRun=0;
							TextNode * pBeforeDeletion=NULL;
							if(current->m_previous)
								pBeforeDeletion=current->m_previous;
							TextNode * next=current->m_next;
							if(current->ch==13)
							{
								if(current->m_next)
									if(current->m_next->ch==10)
									{
										delete current;
										current=next;
										if(next->m_next)
											next=next->m_next;
									}
							}
							delete current;
							current=next;
							if(current)//dwRun+1==dwLength)
							{
#ifdef _DEBUG
								printf("current\n");
#endif
								if(pBeforeDeletion)
								{
#ifdef _DEBUG
									printf("pBeforeDeletion\n");
#endif
									pBeforeDeletion->m_next=current;//->m_next;
									current->m_previous=pBeforeDeletion;
								}
								else
								{
									m_pText->m_first=current;
									m_pText->m_first->m_previous=NULL;
								}
							}
							else
							{
#ifdef _DEBUG
								printf("!current\n");
#endif
								if(pBeforeDeletion)
									pBeforeDeletion->m_next=NULL;
								else
									m_pText->m_first=NULL;
							}
						}
					}
				}
				if(m_pText->m_first) // Text schwarz färben
				{
					TextNode * current=m_pText->m_first;
#ifdef _DEBUG
					printf("m_pText->m_first\n");
#endif
					while(1)
					{
						current->color=0;
						if(current->m_next)
							current=current->m_next;
						else
							break;
					}
#ifdef _DEBUG
					printf("nach der Schleife\n");
#endif
				}
				if(bEnglishPart)
				{
					BOOL bFlag=FALSE;
#ifdef _DEBUG
					printf("bEnglishPart\n");
#endif
					if(bFlag)
					{
#ifdef _DEBUG
						printf("Thread beendet");
#endif
					}
					//die Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
					//wird nicht direkt nach der Funktion TranslateAutomatically()
					//ausgeführt, wenn diese Funktion (EnglishView::OnChar()) vor 
					//der Beendigung der Funktion TranslateAutomatically() ausgeführt
					//wird.
					//wenn die Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
					//nach der Übersetzung noch nicht ausgeführt wurde, wird folgender
					//Code ausgeführt:
					if(!g_bExecutedOnTranslateAutomaticallyFinishes && pFrame->m_bCreated)
					{// wenn der Thread die Funktion 
						//CMainFrame::OnTranslateAutomaticallyFinishes noch
						//nicht aufgerufen hat
						pFrame->KillTimer(1);
						//if(pFrame->m_bShowClauseProgress)
						//{
						if(pFrame->m_bShowGraphicClauseProgress)
							pFrame->m_Progress.DestroyWindow();
						if(pFrame->m_bShowNumericClauseProgress)
							pFrame->m_cStaticClauseProgress.DestroyWindow();
						//}
						//if(pFrame->m_bShowSentenceProgress)
						//{
						if(pFrame->m_bShowGraphicSentenceProgress)
							pFrame->m_SentenceProgress.DestroyWindow();
						if(pFrame->m_bShowNumericSentenceProgress)
							pFrame->m_cStaticSentenceProgress.DestroyWindow();
				//}
				//if(pFrame->m_bShowTextProgress)
				//{
						if(pFrame->m_bShowGraphicTextProgress)
							pFrame->m_TextProgress.DestroyWindow();
						if(pFrame->m_bShowNumericTextProgress)
							pFrame->m_cStaticTextProgress.DestroyWindow();
				//}
						if(pFrame->m_bShowPrognosticedRemainingTime)
							pFrame->m_cStaticRemainingTime.DestroyWindow();
						pFrame->m_bCreated=FALSE;
						//UINT IDArray[]={ID_SEPARATOR,ID_INDICATOR_CAPS,ID_INDICATOR_NUM,ID_INDICATOR_SCRL};
						//pFrame->m_wndStatusBar.SetIndicators(IDArray,4);
						pFrame->m_bMayStartThread=TRUE;
						//pFrame->OnTranslateAutomaticallyFinishes(0,0);
						//damit das Zerstören der Fortschrittsbalken (CProgressCtrl)
						//nicht in der Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
						//ausgeführt wird (sie wurden ja bereits über diesem Kommentar mit
						//DestroyWindow() zerstört und sollen nicht doppelt zerstört werden), 
						//wird g_bExecutedOnTranslateAutomaticallyFinishes auf TRUE gesetzt
						//wenn die Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
						//ausgeführt wird, wird g_bExecutedOnTranslateAutomaticallyFinishes 
						//ausgewertet.
						g_bExecutedOnTranslateAutomaticallyFinishes=TRUE;
					}
					CToolBarCtrl & toolBarCtrl=pFrame->m_wndToolBar.GetToolBarCtrl();
					//wenn die automatische Übersetzung, falls eine Veränderung des
					//Textes vorliegt, aktiviert ist
					if(toolBarCtrl.IsButtonChecked(
						ID_TRANSLATE_AUTOMATICALLY_IF_TEXTMODIFICATION_BUTTON))
					{
						//CToolBarCtrl & toolBarCtrl=pFrame->m_wndToolBar.GetToolBarCtrl();
						//toolBarCtrl.EnableButton(ID_TRANSLATE_BUTTON,TRUE);
						//toolBarCtrl.Indeterminate(ID_TRANSLATE_BUTTON,TRUE);
						//toolBarCtrl.HideButton(ID_TRANSLATE_BUTTON,TRUE);
						//CWnd * pWnd=pFrame->m_wndToolBar.GetDlgItem(ID_ADD_VOCABLE_BUTTON);
						//CButton * pbutton=(CButton *)pFrame->m_wndToolBar.GetDlgItem(ID_ADD_VOCABLE_BUTTON);
						//if(pWnd)
						//pWnd->EnableWindow(FALSE);
						//GetDlgItem(GetDlgItem(
						//pFrame->m_bEnableNextVocableButton=FALSE;
						//pFrame->m_wndToolBar.UpdateDialogControls(pFrame,FALSE);
						//Text * pText=new Text(*m_pText); // Kopie anlegen
						std::vector<Range> vecRange;
						CChildFrame * pChild=(CChildFrame*)pFrame->MDIGetActive();
						EnglishView * pEVGerman=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
						if(pEVGerman)
						{
							pEVGerman->m_bDrawVector=TRUE;
							pEVGerman->m_bDrawCursor=FALSE; // wenn auf die Ansicht, die das
							// editieren erlaubt (ohne Kombinationsfelder), in der unteren Sicht
							// (EnglishView) eingeschaltet wird,
							// darf der Cursor nicht blinken, weil die obere Sicht (EnglishView)
							// ebenfalls blinkt (der Anwedner weiss sonst nicht, wo sich nun der 
							// Eingabefokus befindet
							//(this->GetSafeHwnd());
							g_bContinue=TRUE;
							//pFrame->m_pTranslationThread=AfxBeginThread(TranslateAutomatically,pTranslateAutomaticallyParameters);//,THREAD_PRIORITY_NORMAL,0,0,NULL);
							if(pFrame->m_bMayStartThread)
							{
#ifdef _DEBUG
								printf("pFrame->m_bMayStartThread\n");
#endif
								//pFrame->m_ProgressBar("",100,100,FALSE,0);
								//pFrame->m_wndStatusBar.GetItemRect(0, &MyRect);
								if(pFrame->m_bCreated == FALSE)// && !pFrame->m_bShowPrognosticedRemainingTime)
								{
#ifdef _DEBUG
									printf("pFrame->m_bCreated == FALSE\n");
#endif
									BYTE bIDCount=0;//3;
									std::vector<UINT> vecUINT;
									if(pFrame->m_bShowClauseProgress)
									{
										if(pFrame->m_bShowNumericClauseProgress)
										{
											bIDCount++;
											vecUINT.push_back(ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE);
										}
										if(pFrame->m_bShowGraphicClauseProgress)
										{
											bIDCount++;
											vecUINT.push_back(ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
										}
										//bIDCount++;
										//vecUINT.push_back(ID_INDICATOR_CLAUSE_PROGRESS_PANE);
									}
									if(pFrame->m_bShowSentenceProgress)
									{
										if(pFrame->m_bShowNumericSentenceProgress)
										{
											bIDCount++;
											vecUINT.push_back(ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE);
										}
										if(pFrame->m_bShowGraphicSentenceProgress)
										{
											bIDCount++;
											vecUINT.push_back(ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
										}
										//bIDCount++;
										//vecUINT.push_back(ID_INDICATOR_SENTENCE_PROGRESS_PANE);
									}
									if(pFrame->m_bShowTextProgress)
									{
										if(pFrame->m_bShowNumericTextProgress)
										{
											bIDCount++;
											vecUINT.push_back(ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE);
										}
										if(pFrame->m_bShowGraphicTextProgress)
										{
											bIDCount++;
											vecUINT.push_back(ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
										}
										//bIDCount++;
										//vecUINT.push_back(ID_INDICATOR_TEXT_PROGRESS_PANE);
									}
									if(pFrame->m_bShowPrognosticedRemainingTime)
									{
										//MessageBeep((WORD)-1);
										bIDCount++;
										vecUINT.push_back(ID_INDICATOR_REMAINING_TIME);
									}
									if(vecUINT.size()>0)
									{
										UINT * lpIDArray=new UINT[bIDCount];
										for(BYTE bRun=0;bRun<vecUINT.size();bRun++)
											lpIDArray[bRun]=vecUINT.at(bRun);
										//lpIDArray[vecUINT.size()]=ID_INDICATOR_CAPS;
										//lpIDArray[vecUINT.size()+1]=ID_INDICATOR_NUM;
										//lpIDArray[vecUINT.size()+2]=ID_INDICATOR_SCRL;
										pFrame->m_wndStatusBar.SetIndicators(lpIDArray,
											sizeof(lpIDArray)/sizeof(UINT));
									}
									CStatusBarCtrl & statusbarCtrl=pFrame->m_wndStatusBar.GetStatusBarCtrl();
									//int nHorz=0,nVert=0,nSpacing=0;
									//statusbarCtrl.GetBorders(nHorz,nVert,nSpacing);
									CRect rect,rect2;
									//pFrame->m_wndStatusBar.GetWindowRect(rect);
									pFrame->m_wndStatusBar.GetClientRect(rect2);
#ifdef _DEBUG
									printf("lpRect->bottom: %d lpRect->left %d lpRect->right %d lpRect->top %d\n",rect.bottom,rect.left,rect.right,rect.top);
									printf("lpRect->bottom: %d lpRect->left %d lpRect->right %d lpRect->top %d\n",rect2.bottom,rect2.left,rect2.right,rect2.top);
									//printf("nHorz: %d nVert: %d nSpacing: %d\n",nHorz,nVert,nSpacing);
#endif
									if(bIDCount>0)
									{
										if(pFrame->m_bShowPrognosticedRemainingTime)
										{
											int * Widths=new int[bIDCount];
											for(BYTE bRun=0;bRun<bIDCount;bRun++)
												Widths[bRun]=rect2.right*(bRun+1)/bIDCount;
											//int Widths []={rect2.right/3,rect2.right*2/3,rect2.right};
											statusbarCtrl.SetParts(bIDCount,Widths);
										}
										else
										{
											int * Widths=new int[bIDCount];
											for(BYTE bRun=0;bRun<bIDCount;bRun++)
												Widths[bRun]=rect2.right*(bRun+1)/bIDCount;
											//int Widths []={rect2.right/3,rect2.right*2/3,rect2.right};
											statusbarCtrl.SetParts(bIDCount,Widths);
										}
									}
									bIDCount=0;
									RECT MyRect;
									if(pFrame->m_bShowNumericClauseProgress)
									{
										pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
										pFrame->m_cStaticClauseProgress.Create("",WS_VISIBLE|WS_CHILD,
											MyRect,&pFrame->m_wndStatusBar,
											ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE);
									}
									if(pFrame->m_bShowGraphicClauseProgress)
									{
										pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
										if(pFrame->m_nGraphicClauseProgressType==0)
											pFrame->m_Progress.Create(WS_VISIBLE|WS_CHILD,MyRect,
												&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
										if(pFrame->m_nGraphicClauseProgressType==1)
											pFrame->m_Progress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
												&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
										pFrame->m_Progress.SetRange(0,100); //Set the range to between 0 and 100
										pFrame->m_Progress.SetStep(1); // Set the step amount
									}
#ifdef _DEBUG
									printf("pFrame->m_bShowSentenceProgress\n");
#endif
									if(pFrame->m_bShowNumericSentenceProgress)
									{
										pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
										pFrame->m_cStaticSentenceProgress.Create("",WS_VISIBLE|WS_CHILD,
											MyRect,&pFrame->m_wndStatusBar,ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE);
									}
									if(pFrame->m_bShowGraphicSentenceProgress)
									{
										pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
										if(pFrame->m_nGraphicSentenceProgressType==0)
											pFrame->m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD,MyRect,
												&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
										if(pFrame->m_nGraphicSentenceProgressType==1)
											pFrame->m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
												&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
										pFrame->m_SentenceProgress.SetRange(0,100); //Set the range to between 0 and 100
										pFrame->m_SentenceProgress.SetStep(1); // Set the step amount
									}
									if(pFrame->m_bShowNumericTextProgress)
									{
										pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
										pFrame->m_cStaticTextProgress.Create("",WS_VISIBLE|WS_CHILD,
											MyRect,&pFrame->m_wndStatusBar,ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE);
									}
									if(pFrame->m_bShowGraphicTextProgress)
									{
										pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
										if(pFrame->m_nGraphicTextProgressType==0)
											pFrame->m_TextProgress.Create(WS_VISIBLE|WS_CHILD,MyRect,
												&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
										if(pFrame->m_nGraphicTextProgressType==1)
											pFrame->m_TextProgress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
												&pFrame->m_wndStatusBar,ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
										pFrame->m_TextProgress.SetRange(0,100); //Set the range to between 0 and 100
										pFrame->m_TextProgress.SetStep(1); // Set the step amount
									}
									if(pFrame->m_bShowPrognosticedRemainingTime)
									{
										pFrame->m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
										pFrame->m_cStaticRemainingTime.Create("",WS_VISIBLE|WS_CHILD,
											MyRect,&pFrame->m_wndStatusBar,ID_INDICATOR_REMAINING_TIME);
										tAtTheBeginningOfTheTranslation=(CTime)CTime::GetCurrentTime();
									}
									LPCTSTR str="Fortschritt des Haupt- oder Nebensatzes";
									//statusbarCtrl.SetTipText(0,str);
									//statusbarCtrl.SetTipText(1,_T("Fortschritt des aktuellen Satzes"));
									//statusbarCtrl.SetTipText(2,_T("Fortschritt des ganzen Textes"));
									pFrame->m_bCreated = TRUE;
									//Sleep(2000);
									//pFrame->m_Progress.DestroyWindow();
									//pFrame->m_SentenceProgress.DestroyWindow();
									//pFrame->m_TextProgress.DestroyWindow();
									//pFrame->m_bCreated = FALSE;
									//UINT * lpIDArray=new UINT[4];
									//lpIDArray[0]=ID_SEPARATOR;
									//lpIDArray[1]=ID_INDICATOR_CAPS;
									//lpIDArray[2]=ID_INDICATOR_NUM;
									//lpIDArray[3]=ID_INDICATOR_SCRL;
									//pFrame->m_wndStatusBar.SetIndicators(lpIDArray,4);
								}
								if(pFrame->SetTimer(1,1000,NULL)==0)
								{
#ifdef _DEBUG
									printf("pFrame->SetTimer(1,1000,NULL)==0\n");
#endif
								}
								else
								{
#ifdef _DEBUG
									printf("pFrame->SetTimer(1,1000,NULL)!=0\n");
#endif
								}
								pFrame->m_bTimerIntalled=TRUE;
								//Sleep(2000);
								//g_dwSentenceStatus=0;

								TranslateAutomaticallyParameters * 
									pTranslateAutomaticallyParameters=
									new TranslateAutomaticallyParameters(m_pText,vecRange,
									pEVGerman,pFrame->m_hWnd,pEVGerman->GetSafeHwnd(),
									pFrame->m_Progress.GetSafeHwnd(),pFrame->
									m_SentenceProgress.GetSafeHwnd(),pFrame->m_TextProgress.
									GetSafeHwnd());
								pTranslateAutomaticallyParameters->m_bCaseSensitive=pFrame->m_bCaseSensitive;
								pTranslateAutomaticallyParameters->m_dwMaxCharsPerLine=pFrame->m_dwMaxCharsPerLine;
								int nPriority=0;
								switch(pFrame->m_nThreadPriority)
								{
									case 0: nPriority=THREAD_PRIORITY_IDLE;
										break;
									case 1: nPriority=THREAD_PRIORITY_LOWEST;
										break;
									case 2: nPriority=THREAD_PRIORITY_BELOW_NORMAL;
										break;
									case 3: nPriority=THREAD_PRIORITY_NORMAL;
										break;
									case 4: nPriority=THREAD_PRIORITY_ABOVE_NORMAL;
										break;
									case 5: nPriority=THREAD_PRIORITY_HIGHEST;
										break;
								}
								pFrame->m_pTranslationThread=AfxBeginThread(
									TranslateAutomatically,pTranslateAutomaticallyParameters,
									nPriority,0,0,NULL);
								pFrame->m_pTranslationThread->m_bAutoDelete=TRUE;
#ifdef _DEBUG
								printf("AfxBeginThread()\n");
#endif
								//MessageBeep((WORD)-1);
								//MessageBeep((WORD)-1);
							}
						}
					}
				}
			}
		}
	}
	m_bDrawCursor=TRUE;
	Invalidate();
	TRACE("nChar %u\n",nChar);
	TRACE("m_CursorPos: %u\n",m_CursorPos);
#ifdef _DEBUG
	printf("void EnglishView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)-ENDE\n");
#endif
	//return CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void EnglishView::OnKillFocus( CWnd* pNewWnd )
{
#ifdef _DEBUG
	printf("void EnglishView::OnKillFocus( CWnd* pNewWnd ) ANFANG\n");
#endif
	CVTransApp * pApp=(CVTransApp *)AfxGetApp();
	if(pApp)
	{
		CMainFrame * pFrame=(CMainFrame *)pApp->m_pMainWnd;
		if(pFrame)
		{
			CChildFrame * pChild=(CChildFrame*)pFrame->MDIGetActive();
			if(!(pNewWnd==&pChild->m_SplitterWnd))
			{
				KillTimer(1);
				m_bDrawCursor=TRUE;
				m_bHasFocus=0;
				Invalidate(FALSE);
			}
		}
	}
}

void EnglishView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	TRACE("void EnglishView::OnLButtonDown(UINT nFlags, CPoint point) Anfang\n");
#ifdef _DEBUG
	printf("void EnglishView::OnLButtonDown(UINT nFlags, CPoint point) Anfang\n");
#endif
	//wenn keine CComboBoxen (Kombinationsfelder) angezeigt werden
	if(!m_bDrawVector)
	{
	//m_pDC.Attach(m_pDC.GetSafeHdc());
	TRACE("point.x: %d point.y: %d\n",point.x,point.y);
	//while(1)
	m_point=point;
	m_bCalculateCursorPos=TRUE;
	//CDC* pDC=GetDC();
	//OnDraw(pDC);
	Invalidate();
	CRect rect;
	TRACE("vor: GetClientRect(lpRect);\n");
	GetClientRect(rect);
	DWORD dwTextLength=m_pText->GetLength();
	DWORD dwStart=0;
	TEXTMETRIC textmetric;
	CDC * pDC=GetDC();
	pDC->GetTextMetrics(&textmetric); // für die Zeilenhöhe
	CString strTextFace;
	pDC->GetTextFace(strTextFace);
	TRACE("strTextFace: %s\n",strTextFace);
	//CSize size=pDC->GetOutputTextExtent("Sports play a very big part in English schools. The boys play football and cricket");
	//TRACE("size.cx: %u\n",size.cx);
	//Berechnung, in welche Zeile innerhalb des sichtbaren Bereichs geklickt wurde
	DWORD dwLine=m_point.y/textmetric.tmHeight;
	DWORD dwCharsBefore=0;
	DWORD dwRun=0;
	DWORD dwHorizontalOffset=0;
	BOOL bPreviousCharacterWasCarriageReturn=FALSE;
	BOOL bSeperator=TRUE;
	CRect clientRect;
	GetClientRect(clientRect);

	BYTE lastChar=0;
	//CSize size;
	CString str2;
	DWORD dwWordStart=0;
	TextNode * current=NULL;
	BOOL bGotoAfter=FALSE;
	if(m_pText->m_first)
	//if(dwLine>0 && m_pText->m_first)
	{
		//gibt an, ob eine neue Cursorposition zugewiesen wurde
		BYTE bNewCursorPositionAssigned=0;
		BYTE bNewLine;
		//gibt an, dass dwLineNumber um 1 erhöht werden soll
		BYTE bIncrement_dwLineNumber=0;
		//der vorherige Durchlauf erzeugte eine neue Zeile
		BYTE bPreviousPassWasNewLine=0;
		BOOL bPreviousCharacterWasCarriageReturn=FALSE;
		BOOL bBreakLoop=FALSE;
		CSize size;
		CString strCurrentToken;
		CString strLine;
		DWORD dwBeginOfNextWord=0;
		DWORD dwNewCursorPosition=0;
		DWORD dwNewLines=0;
		DWORD dwLineNumber=0;
		DWORD dwPositionOfEndOfPreviousLine=0;
		DWORD dwCharacters=0;
		//die erste Zeile, die sichtbar ist: wenn die Bildlaufleiste nach 
		//unten gescrollt wird, ist die Zeilennummer der ersten sichtbaren Zeile 
		//höher als die Zeilennummer der ersten Zeile
		//m_dwStartLine: Member-Variable der Klasse "EnglishView"
		//dwFirstVisibleLine wird im Folgenden bei jeder neuen Zeile um 1 
		//erniedrigt, wenn dwFirstVisibleLine größer als 0 ist
		DWORD dwFirstVisibleLine=m_dwStartLine;
		DWORD dwNewLine=0;
		DWORD dwWordStart=0;
		TextNode * pTextNodeBeginOfCurrentToken=NULL;
		dwRun=0;
		dwLineNumber=0;
		bSeperator=TRUE;
		current=m_pText->m_first;
		TRACE("vor: while(dwLineNumber<m_dwStartLine)\n");
		TRACE("vor: while(dwLineNumber<dwLine)\n");
		TRACE("dwLine>0 && m_pText->m_first\n");
		// Zeichen, die sich vor der ersten
		// sichtbaren Zeile befinden, zählen.
		/*while(dwLineNumber<m_dwStartLine)
		{
			if(current->ch==' ' || current->ch==13 || (bPreviousCharacterWasCarriageReturn && current->ch==10) || !current->m_next)
			{ // wenn Leerzeichen, Carriage Return, Newline mit direkt davor befindlichem
						// Carriage Return oder wenn das Ende der verketteten Liste erreicht wurde
				if(!bSeperator || dwRun==0)
				{
					CString str;
					//if(current->ch==' ')
					//	m_pText->GetCString(str,dwStart,dwRun);
					if(current->ch==13 || (bPreviousCharacterWasCarriageReturn && current->ch==10))
						m_pText->GetCString(str,dwStart,dwRun-1); // carriage return oder 
						//newline NICHT einbeziehen in die Zeichenkette 'str'
					else
						m_pText->GetCString(str,dwStart,dwRun);
					TRACE("str: %s\n",str);
					CSize size=pDC->GetOutputTextExtent(str);
					if(dwHorizontalOffset+size.cx>clientRect.right-15)
						if(size.cx<=clientRect.right-15) // wenn das Wort in eine Zeile passt
						{
							dwHorizontalOffset=0;
							dwLineNumber++;
						}
					for(DWORD dwRun2=0;dwRun2<str.GetLength();dwRun2++)
					{
						TRACE("current->ch: %u\n",current->ch);
						if(str.GetAt(dwRun2)!=13 && !(bPreviousCharacterWasCarriageReturn && str.GetAt(dwRun2)==10))
						{
							bPreviousCharacterWasCarriageReturn=FALSE;
							size=pDC->GetOutputTextExtent(str.GetAt(dwRun2));
							if(dwHorizontalOffset+size.cx>clientRect.right-15 && clientRect.right-15>=size.cx)
							{ // wenn die Zeichenkette nicht mehr vollständig in das Fenster passt
								dwLineNumber++;
								dwHorizontalOffset=0;
							}
							pDC->TextOut(dwHorizontalOffset,dwLine*textmetric.tmHeight,str.GetAt(dwRun2));
#ifdef _DEBUG
							printf("Gebe %c aus\n",str.GetAt(dwRun2));
#endif
						}
						if(str.GetAt(dwRun2)!=13 && !(bPreviousCharacterWasCarriageReturn && str.GetAt(dwRun2)==10))
							dwHorizontalOffset+=size.cx;
						//else if(bPreviousCharacterWasCarriageReturn && current->ch==10)
						if(str.GetAt(dwRun2)==13)
							bPreviousCharacterWasCarriageReturn=TRUE;
						else
							bPreviousCharacterWasCarriageReturn=FALSE;
					}
					dwStart=dwRun+1;
					bSeperator=TRUE;
					}
					else // wenn bSeperator==TRUE;
					{
						bSeperator=TRUE;
						dwStart=dwRun+1;
						if(current->ch==' ')
						{
							CSize size=pDC->GetOutputTextExtent(current->ch);
							if(dwHorizontalOffset+size.cx>clientRect.right-15 && clientRect.right-15>=size.cx)
							{ // wenn die Zeichenkette nicht mehr vollständig in das Fenster passt
								dwLineNumber++;
								dwHorizontalOffset=0;
							}
							dwHorizontalOffset+=size.cx;
							//pDC->TextOut(dwHorizontalOffset,dwLine*textmetric.tmHeight,str.GetAt(dwRun2));
						}
					}
				}
				else
					bSeperator=FALSE;
			if(current->m_next)
				current=current->m_next;
			else
				goto after;
			dwCharsBefore++;
			dwRun++;
		}*/
#ifdef _DEBUG
		printf("dwFirstVisibleLine: %u dwLineNumber: %u dwLine: %u\n",
			dwFirstVisibleLine,dwLineNumber,dwLine);
#endif
		//sowohl die Zeichenfolge "carriage return" "newline" ("13" "10") als
		//auch eine einzelne "13" leiten eine neue Zeile ein.
		//aber auch wenn ein Token nicht mehr in eine Zeile passt, aber in die
		//Nächste oder ein Token so lang ist, dass es noch nicht mal in eine
		//leere Zeile passt, wird eine neue Zeile eingeleitet
		
		//wenn die aktuelle Zeilennummer innerhalb des sichtbaren Bereichs (erste
		//sichtbare Zeile: dwLineNumber=1) kleiner ist als die Zeilennummer der 
		//Zeile, in die innerhalb des sichtbaren Bereichs (erste sichtbare 
		//Zeile: dwLine=1) geklickt wurde
		while(dwLineNumber<dwLine+1)
		{
			bNewLine=0;
			/*if(bIncrement_dwLineNumber)
			{
				++dwLineNumber;
				bIncrement_dwLineNumber=0;
			}*/
			//wenn im vorherigen Schleifendurchlauf eine neue Zeile war
			if(bPreviousPassWasNewLine)
			{
				//Zeichenkette, die die aktuelle Zeile bis zum aktuellen Zeichen
				//darstellt, auf Länge 0 setzen
				strLine="";
				bPreviousPassWasNewLine=0;
			}
			//wenn carriage return...
			if(current->ch==13)
			{
				bPreviousPassWasNewLine=1;
				//eine neue Zeile beginnt bei der horizontalen Position 
				//(Offset) 0
				dwHorizontalOffset=0;
				bNewLine=1;
				//wenn dwFirstVisibleLine nicht "0" ist, ...
				if(dwFirstVisibleLine)
					//... dann um 1 erniedrigen
					--dwFirstVisibleLine;
				else
				{
					/*//wenn die geklickte Zeile mit der aktuellen Zeile übereinstimmt 
					//und wenn das Ende der aktuellen Zeichenkette links neben der 
					//Position des Mausklicks liegt
					if(dwLineNumber==dwLine && point.x>size.cx)
					{
						m_CursorPos=dwNewCursorPosition;
						bNewCursorPositionAssigned=1;
						break;
					}*/
					//dann Zeilenindex um 1 erhöhen
					++dwLineNumber;
				}
				if(current->m_next)
					//...und das nächste Zeichen newline...
					if(current->m_next->ch==10)
					{
						current=current->m_next;
					}
			}
			/*//wenn newline...
			else if(current->ch==10)
			{
				if(dwFirstVisibleLine)
					--dwFirstVisibleLine;
				else
				{
					////wenn die geklickte Zeile mit der aktuellen Zeile übereinstimmt und
					////wenn das Ende der aktuellen Zeichenkette links neben der Position
					////des Mausklicks liegt
					//if(dwLineNumber==dwLine && point.x>size.cx)
					//{
					//	m_CursorPos=dwNewCursorPosition;
					//	bNewCursorPositionAssigned=1;
					//	break;
					//}
					//...dann Zeilenindex um 1 erhöhen
					++dwLineNumber;
				}
				//eine neue Zeile beginnt bei der horizontalen Position (Offset) 0
				dwHorizontalOffset=0;
				bNewLine=1;
				bPreviousPassWasNewLine=1;
			}*/
			//wenn Leerzeichen
			if(current->ch==' ')
			{
#ifdef _DEBUG
				printf("current->ch==' '\n");
#endif
				//Zeichenkette des aktuellen Token leeren
				strCurrentToken="";
				//++dwLineNumber;
				//dwHorizontalOffset=0;
				strLine+=current->ch;
				dwPositionOfEndOfPreviousLine=dwNewCursorPosition;
				pTextNodeBeginOfCurrentToken=current->m_next;
			}
			//wenn Zeichen ausser Leerzeichen
			else
			{
				//wenn nicht Zeichenfolge "13" "10" oder Zeichen "13"
				if(!bNewLine)
				{
					//Zeichen an die Zeichenfolge für das aktuelle Token anhängen
					strCurrentToken+=current->ch;
					//Zeichen an die Zeichenfolge der aktuellen Zeile anhängen
					strLine+=current->ch;
				}
			}
			//Breite in Pixel der Zeile ermitteln
			size=pDC->GetOutputTextExtent(strLine);
			//wenn das Ende der aktuellen Zeichenkette links neben der Position
			//des Mausklicks liegt und ein manueller Zeilenumbruch (carriage 
			//return + newline, carriage return) eingeleitet 
			//wurde und die aktuelle Zeile die Zeile ist, in die geklickt wurde
			if(point.x>size.cx && bNewLine && dwLineNumber-1==dwLine && 
				dwFirstVisibleLine==0)
			{
				//dann Cursor auf das Zeilenende setzen
				m_CursorPos=dwNewCursorPosition;
				bNewCursorPositionAssigned=1;
				break;
			}
#ifdef _DEBUG
			printf("size.cx: %u\n",size.cx);
#endif
			//Wenn ein automatischer Zeilenumbruch stattfindet, weil sonst
			//die Zeile über die vertikale Bildlaufleiste hinaus ginge.
			//Wenn die Breite in Pixel der Zeile größer ist als die Breite des
			//Clientbereiches minus der Breite in Pixel der Bildlaufleiste (15)
			//minus der Breite in Pixel für einen möglichen Cursor (1)
			if(size.cx>clientRect.right-16)
			{
				CSize sizeCurrentToken=pDC->GetOutputTextExtent(strCurrentToken);
#ifdef _DEBUG
				printf("size.cx>clientRect.right-16\n");
#endif
				//wenn das aktuelle Token in eine Zeile passt
				//(wenn in der aktuellen Zeile KEIN(E) Leerzeichen ist/sind, passt
				//das aktuelle Token NICHT in eine Zeile)
				//wenn die Breite in Pixel der aktuellen Zeichenkette des aktuellen
				//Token kleiner gleich ist als die Breite des Clientbereiches 
				//minus der Breite in Pixel der Bildlaufleiste (15) minus der 
				//Breite in Pixel für einen möglichen Cursor (1)
				if(sizeCurrentToken.cx<=clientRect.right-16 && strCurrentToken.
						GetLength()>0)
				{
//#ifdef _DEBUG
//					printf("sizeCurrentToken.cx<=clientRect.right-15\n");
//#endif
					//wenn die geklickte Zeile mit der aktuellen Zeile übereinstimmt und
					//wenn das Ende der aktuellen Zeichenkette links neben der Position
					//des Mausklicks liegt
					//(das wirkliche Ende der Zeile berechnet sich aus: 
					//"Breite der Zeile ohne Berücksichtigung des automatischen Zeilen-
					//umbruchs" minus "Breite der letzten Zeichenkette beginnend ab dem
					//letzten Leerzeichen bis zum aktuellen Zeichen" minus "Breite eines
					//Leerzeichens")...
					if(dwLineNumber==dwLine && dwFirstVisibleLine==0 && point.x>
						(size.cx-sizeCurrentToken.cx-pDC->GetOutputTextExtent(" ").cx))
					{
#ifdef _DEBUG
						printf("m_CursorPos=dwPositionOfEndOfPreviousLine(%u)\n",
							dwPositionOfEndOfPreviousLine);
#endif
						//...dann soll der Cursor am Ende der Zeile sein (und erscheinen)
						//(dwPositionOfEndOfPreviousLine ist die Position des 
						//vorherigen Leerzeichens)
						m_CursorPos=dwPositionOfEndOfPreviousLine;
						bNewCursorPositionAssigned=1;
						break;
					}
					//strCurrentToken=strLine;
					//wenn das letzte Token in eine Zeile passt
					//if(sizeCurrentToken.cx<=clientRect.right-15)
					//{
						//auf den Beginn des aktuellen Tokens positionieren (also 
						//innerhalb der verketteten Liste Richtung Listenbeginn zurück-
						//springen)
					current=pTextNodeBeginOfCurrentToken;
					dwNewCursorPosition-=strCurrentToken.GetLength()-1;
					strLine=strCurrentToken.Left(1);
				}
				//wenn das aktuelle Token nicht in eine Zeile passt
				else
				{
					if(dwLineNumber==dwLine && dwFirstVisibleLine==0 && point.x>
						size.cx-pDC->GetOutputTextExtent(
              //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
              //is faster than creating a CString(current->ch) in order to use 
              //GetOutputTextExtent(const CString& str ) const; .
              //&(CHAR)current->ch,1
              CString((TCHAR)current->ch)).cx)
					{
						m_CursorPos=dwNewCursorPosition-1;
						bNewCursorPositionAssigned=1;
						break;
					}
					strLine=strCurrentToken.Right(1);
				}
						//strLine=strCurrentToken;
				//}
				/*else
				{
					strLine=CString(current->ch);
					strCurrentToken=CString(current->ch);
					if(size.cx-pDC->GetOutputTextExtent(current->ch).cx<point.x)
					{
						m_CursorPos=dwNewCursorPosition;
						bNewCursorPositionAssigned=1;
					}
				}*/
				if(dwFirstVisibleLine)
					--dwFirstVisibleLine;
				else
				{
					++dwLineNumber;
					//wenn dwLineNumber sofort verändert würde, könnte es sein, dass
					//die Abbruchbedingung der Schleife beim nächsten 
					//Schleifendurchlauf erfüllt sein würde ohne dass das Ende 
					//der aktuellen Zeichenkette rechts neben der Position des 
					//Mausklicks liegt
					bIncrement_dwLineNumber=1;
				}
				size=pDC->GetOutputTextExtent(strLine);
			}
			//wenn die geklickte Zeile mit der aktuellen Zeile übereinstimmt und
			//wenn das Ende der aktuellen Zeichenkette rechts neben der Position
			//des Mausklicks liegt oder bei der Position des Mausklicks
			if(dwLineNumber==dwLine && dwFirstVisibleLine==0 && size.cx>=point.x)
			{
#ifdef _DEBUG
				printf("dwLineNumber==dwLine && size.cx>point.x\n");
				printf("strLine: %s\n",strLine);
				printf("dwNewCursorPosition: %u\n",dwNewCursorPosition);
#endif
				//wenn in diesem Schleifendurchlauf KEIN automatischer Zeilenumbruch
				//eingeleitet wurde
				if(!bNewLine)
				{
					//wenn noch keine neue Cursorposition zugewiesen wurde
					if(!bNewCursorPositionAssigned)
					{
						//an welcher Position ist der Mausklick mit der linken Taste
						//näher dran?
						//Beispiel: nur ein Buchstabe: "W", 9 Pixel breit
						// size ist also "9"
						// sizeCurrentCharacter ist auch "9"
						// der Mausklick (point.x) war bei Position 4 (point.x beginnt 
						// bei Position "0"), also ist die resultierende 
						// Cursorposition 0, da die Differenz zwischen 4 und 0 kleiner
						// ist als zwischen 9 und 4
						CSize size2;
						CSize size3;
#ifdef _DEBUG
						printf("!bNewCursorPositionAssigned\n");
#endif
						for(int dwRun=0;dwRun<strLine.GetLength();++dwRun)
						{
							size2=pDC->GetOutputTextExtent(strLine.Right(dwRun));
							size3=pDC->GetOutputTextExtent(strLine.Right(dwRun+1));
							//wenn die Differenz aus Pixelbreite der aktuellen 
							//Zeichenkette und der Pixelbreite einer Zeichenkette, die 
							//zunehmend dem rechten Teil der aktuellen Zeichenkette 
							//entspricht kleiner als die horizontale Pixelkoordinate
							//des Mausklicks ist und die Cursorposition neu zugewiesen
							//wurde
							if(size.cx-size2.cx<point.x && bNewCursorPositionAssigned)
								break;
#ifdef _DEBUG
								printf("size.cx: %d size2.cx: %d size3.cx: %d point.x: %d\n",
									size.cx,size2.cx,size3.cx,point.x);
#endif
							//wenn die x-Position des Mausklicks näher an dem Zeichen
							//ist, das näher am Zeilenanfang ist als das Zeichen, das 
							//näher am Zeilenende ist
							if(size.cx-(point.x-size2.cx)<point.x-(size.cx-size3.cx))
								m_CursorPos=++dwNewCursorPosition-dwRun;
							else
								m_CursorPos=dwNewCursorPosition-dwRun;
							bNewCursorPositionAssigned=1;
						}
					}
					/*if(!bNewCursorPositionAssigned)
					{
						if(point.x-(size.cx-sizeCurrentCharacter.cx)<size.cx-point.x)
							m_CursorPos=dwNewCursorPosition;
						else
							m_CursorPos=++dwNewCursorPosition;
						bNewCursorPositionAssigned=1;
					}*/
#ifdef _DEBUG
					printf("m_CursorPos: %u\n",m_CursorPos);
#endif
					//break;
				}
			}
			//dwNewCursorPositionum 1 erhöhen: bei jedem Listenelement außer 
			//wenn current->ch "13" ist und direkt danach eine "10" folgt
			++dwNewCursorPosition;
			//wenn das Listenende noch nicht erreicht ist
			if(current->m_next)
				current=current->m_next;
			else
			{
				//wenn die geklickte Zeile mit der aktuellen Zeile übereinstimmt und
				//wenn das Ende der aktuellen Zeichenkette links neben der Position
				//des Mausklicks liegt
				if(dwLineNumber==dwLine && dwFirstVisibleLine==0 && point.x>size.cx)
				{
					m_CursorPos=dwNewCursorPosition;
					bNewCursorPositionAssigned=1;
				}
				break;
			}
		}
		//wenn die Cursorposition noch nicht zugewiesen wurde, wurde unterhalb
		//des Textes geklickt
		if(!bNewCursorPositionAssigned)
			m_CursorPos=dwTextLength;
		//m_CursorPos=dwNewCursorPosition;
		//wenn die neue Cursorposition noch nicht zugewiesen wurde, liegt das
		//daran, dass die Position des Mausklicks rechts vom Ende der Zeile, 
		//in die geklickt wurde, liegt.
		/*if(m_CursorPos!=dwNewCursorPosition)
		{
			//wenn das Textende noch nicht erreicht wurde
			if(current->m_next)
				//Da für die letzte Neuzeile 
				//dwNewCursorPosition um 1 erhöht wurde, muss 1 abgezogen werden, 
				//damit der Cursor am Zeilenende und nicht am Zeilenanfang der 
				//nächsten Zeile liegt.
				m_CursorPos=dwNewCursorPosition-1;
			//wenn das Textende erreicht wurde
			else
				m_CursorPos=dwNewCursorPosition;
		}*/
		/*while(dwLineNumber<dwLine)
		{
			TRACE("current->ch: %c\n",current->ch);
			printf("current->ch: %c current->ch: %u\n",current->ch,current->ch);
			//wenn newline
			if(current->ch==10)
			{
				//dwLineNumber++;
				//dwHorizontalOffset=0;
				bSeperator=TRUE;
			}
			//wenn Leerzeichen
			if(current->ch==' ')
			{
				TRACE("current->ch==' '\n");
				CSize size=pDC->GetOutputTextExtent(" ");
				TRACE("dwHorizontalOffset: %u size.cx: %u clientRect.right: %u\n",dwHorizontalOffset,size.cx,clientRect.right);
				if(dwHorizontalOffset+size.cx>clientRect.right-15)
				{ // Leerzeichen passt nicht mehr in die aktuelle Zeile
					dwLineNumber++;
					dwHorizontalOffset=size.cx;
				}
				else
					dwHorizontalOffset+=size.cx;
				bSeperator=TRUE;
			}
			else
			{
				if(bSeperator)
				{
				//	pBeginOfNextWord=current;
					CString strWord;
					TextNode * pTextNodeRun=current;
					dwBeginOfNextWord=dwRun;
					dwWordStart=dwRun;
					TRACE("bSeperator\n");
					TRACE("current->ch: %c\n",current->ch);
					while(1)
					{
						if(pTextNodeRun->ch==' ' || pTextNodeRun->ch==13)
						{
							TRACE("pTextNodeRun->ch==' ' || pTextNodeRun->ch==13\n");
							printf("pTextNodeRun->ch==' ' || pTextNodeRun->ch==13\n");
							printf("pTextNodeRun->ch: %u\n",pTextNodeRun->ch);
							CString strLine;
							m_pText->GetCString(strLine,0,dwRun);
							TRACE("strLine: %s\n",strLine);
							CSize size=pDC->GetOutputTextExtent(strLine);
							TRACE("size.cx: %u\n",size.cx);
							size=pDC->GetOutputTextExtent(strWord);
							TRACE("strWord: %s\n",strWord);
							TRACE("dwHorizontalOffset: %u size.cx: %u clientRect.right: %u\n",dwHorizontalOffset,size.cx,clientRect.right);
							if(dwHorizontalOffset+size.cx>clientRect.right-15)
							{
								dwLineNumber++;
								dwHorizontalOffset=size.cx;
								dwBeginOfNextWord=dwRun;
							}
							else
								dwHorizontalOffset+=size.cx;
							break;
						}
						strWord+=pTextNodeRun->ch;
						if(pTextNodeRun->m_next)
							pTextNodeRun=pTextNodeRun->m_next;
						else
						{
							//dwBeginOfNextWord=dwRun;
							TRACE("Listenende erreicht\n");
							CString strLine;
							m_pText->GetCString(strLine,0,dwRun);
							TRACE("strLine: %s\n",strLine);
							CSize size=pDC->GetOutputTextExtent(strLine);
							TRACE("size.cx: %u\n",size.cx);
							size=pDC->GetOutputTextExtent(strWord);
							TRACE("strWord: %s\n",strWord);
							TRACE("dwHorizontalOffset: %u size.cx: %u clientRect.right: %u\n",dwHorizontalOffset,size.cx,clientRect.right);
							if(dwHorizontalOffset+size.cx>clientRect.right-15)
								dwLineNumber++;
							else
								dwHorizontalOffset+=size.cx;
							break;
						}
					}
					bSeperator=FALSE;
				}
			}
			if(dwLineNumber==dwLine)
				break;
			if(current->ch==13)
			{
				dwLineNumber++;
				dwHorizontalOffset=0;
				bSeperator=TRUE;
				if(dwLineNumber==dwLine)
					if(current->m_next)
						if(current->m_next->ch==10)
						{
							printf("current->m_next->ch==10\n");
							current=current->m_next;
							if(current->m_next)
								current=current->m_next;
							dwBeginOfNextWord=dwRun+2;
						}
						else
							printf("!current->m_next->ch==10\n");
			}
			if(dwLineNumber==dwLine)
				break;
			if(current->m_next)
				current=current->m_next;
			else
				break;
			dwRun++;
		}*/
		/*DWORD dwAbsolutePosition=dwBeginOfNextWord;
		if(dwLine>dwLineNumber)
		{
			TRACE("dwLine>dwLineNumber\n");
			m_CursorPos=dwTextLength;
			bGotoAfter=TRUE;
			//break;
			//goto after;
		}
setcursor:
		TRACE("Marke setcursor\n");
		printf("Marke setcursor\n");
		if(!bGotoAfter)
		{
	dwStart+=dwCharsBefore; // plus Anzahl der Zeichen, die sich vor der
			// ersten sichtbaren Zeile befinden
	TRACE("dwStart: %u\n",dwStart);
	TRACE("Schriftart: %s\n",str2);
	size.cx=0;
	dwRun=dwStart;//+1;
	//dwWordStart=dwStart;
	bSeperator=TRUE;
	dwHorizontalOffset=0;
	CString str;
	TextNode * pBeginOfCurrentWord=NULL;
	DWORD dwLineWidthUntilLastSpace=0;
	for(;dwRun<dwTextLength;dwRun++)
	{
		TRACE("current->ch: %c\n",current->ch);
		printf("current->ch: %c current->ch: %u\n",current->ch,current->ch);
		str+=current->ch;
		printf("str: %s\n",str);
		TRACE("str: %s\n",str);
		lastChar=current->ch;
		CSize size=pDC->GetOutputTextExtent(str);
		if(lastChar==' ')
		{ // wenn Leerzeichen, Carriage Return, Newline mit direkt davor befindlichem
					// Carriage Return oder wenn das Ende der verketteten Liste erreicht wurde
			dwLineWidthUntilLastSpace=size.cx;

			if(!bSeperator || dwRun==0)
			{
				bSeperator=TRUE;
				//dwWordStart=dwRun+1;
			}
			else // wenn bSeperator==TRUE;
			{
			}
		}
		else // wenn ein Zeichen, das kein Leerzeichen ist
		{
			if(bSeperator)
			{
				TRACE("bSeperator\n");
				dwWordStart=dwAbsolutePosition+dwRun;
				pBeginOfCurrentWord=current;
				TextNode * current2=current;
				//for(DWORD dwRun2=0;dwRun2<dwTextLength;dwRun2++)
				//{
				BOOL bFlag=FALSE;
				CString str="";
				BOOL bBreakLoop=FALSE;
				while(1)
				{
					if(current2->ch==' ')
					{
						TRACE("current2->ch==' '\n");
						printf("current2->ch==' '\n");
						bFlag=TRUE;
							TRACE("bFlag\n");
							CSize size=pDC->GetOutputTextExtent(str);
							TRACE("str: %s\n",str);
							printf("str: %s\n",str);
							TRACE("size.cx %d  dwHorizontalOffset: %u\n",size.cx,dwHorizontalOffset);
							printf("size.cx %d  dwLineWidthUntilLastSpace: %u clientRect.right-15: %u\n",size.cx,dwLineWidthUntilLastSpace,clientRect.right-15);
							if(dwLineWidthUntilLastSpace+size.cx>clientRect.right-15)
							{
								printf("dwLineWidthUntilLastSpace+size.cx>clientRect.right-15\n");
								TRACE("dwWordStart: %u\n",dwWordStart);
								m_CursorPos=dwWordStart;
								bBreakLoop=TRUE;
								break;
								//goto after;
							}
							dwHorizontalOffset+=size.cx;
							break;
					}
					else
					{
						str+=current2->ch;
						if(bFlag)
						{
						}
					}
					if(!current2->m_next)
					{
						TRACE("!current2->m_next\n");
							CSize size=pDC->GetOutputTextExtent(str);
							TRACE("str: %s\n",str);
							TRACE("size.cx %d  dwHorizontalOffset: %u\n",size.cx,dwHorizontalOffset);
							if(dwLineWidthUntilLastSpace+size.cx>clientRect.right-15 && dwHorizontalOffset!=0)
							{
								TRACE("dwWordStart: %u\n",dwWordStart);
								m_CursorPos=dwWordStart;
								bBreakLoop=TRUE;
								break;
								//goto after;
							}
							break;
					}
					if(current2->m_next)
						current2=current2->m_next;
					else
						break;
				}
				if(bBreakLoop)
					break;
			}
			bSeperator=FALSE;
		}
		if(lastChar==13 || !current->m_next)
		{
			printf("lastChar==13 || !current->m_next\n");
			if(dwHorizontalOffset<point.x)
			{
				printf("dwHorizontalOffset<point.x\n");
				m_CursorPos=dwAbsolutePosition+dwRun;
				break;
				//goto after;
			}
		}
		if(size.cx>point.x)
		{
			TRACE("size.cx>point.x\n");
#ifdef _DEBUG
			printf("size.cx>point.x\n");
			printf("sice.cx: %u point.x: %u\n",size.cx,point.x);
#endif
			TRACE("dwAbsolutePosition: %u dwWordStart: %u str.GetLength(): %u\n",dwAbsolutePosition,dwWordStart,str.GetLength());
#ifdef _DEBUG
			printf("dwAbsolutePosition: %u dwWordStart: %u str.GetLength(): %u\n",dwAbsolutePosition,dwWordStart,str.GetLength());
#endif
			m_CursorPos=dwAbsolutePosition+str.GetLength();
			break;
			//goto after;
		}
		if(current->m_next)
			current=current->m_next;
		else
			break;
	}
		}*/
	}
//after:;
	TRACE("void EnglishView::OnLButtonDown(UINT nFlags, CPoint point) ENDE\n");
#ifdef _DEBUG
	printf("m_CursorPos: %u\n",m_CursorPos);
	printf("void EnglishView::OnLButtonDown(UINT nFlags, CPoint point) ENDE\n");
#endif
	}
}

void EnglishView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(nSBCode==SB_LINEDOWN) // wenn auf den unteren Pfeil geklickt wird
	{
		m_dwStartLine++;
		m_wndVert.SetScrollPos(m_dwStartLine);
	}
	if(nSBCode==SB_LINEUP) // wenn auf den oberen Pfeil geklickt wird
	{
		if(m_dwStartLine>0)
			m_dwStartLine--;
		m_wndVert.SetScrollPos(m_dwStartLine);
	}
	if(nSBCode==SB_THUMBTRACK) // während man das Kästchen der Bildlaufleiste zieht
	{
		m_dwStartLine=nPos;
		m_wndVert.SetScrollPos(nPos);
	}
	Invalidate();
}
/*int EnglishView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// side-step CView's implementation since we don't want to activate
	//  this view
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}*/

/////////////////////////////////////////////////////////////////////////////
// CVTransView Diagnose


#ifdef _DEBUG
void EnglishView::AssertValid() const
{
	CView::AssertValid();
}

void EnglishView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

/*CVTransDoc* CVTransView::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVTransDoc)));
	return (CVTransDoc*)m_pDocument;
}*/
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVTransView Nachrichten-Handler

void EnglishView::CreateComboBoxes()
{
	TRACE("void EnglishView::CreateComboBoxes() - ANFANG\n");
#ifdef _DEBUG
	printf("void EnglishView::CreateComboBoxes() - ANFANG\n");
#endif
	CDC * pDC=GetDC();
	DWORD dwHorizontalStart=0;
	for(DWORD dwRun=0;dwRun<m_cstrvv.size();dwRun++)
	{
		CStringVector cstrv=m_cstrvv.at(dwRun);
		TRACE("CStringVector cstrv=m_cstrvv.at(dwRun)\n");
		if(cstrv.size()>1) // Vektor hat mehr als 1 Element (CString)
		{
			if(m_ComboBoxList.m_pFirst)
			{
				m_ComboBoxList.m_pLast->m_pNext=new ComboBoxListElement;
				m_ComboBoxList.m_pLast=m_ComboBoxList.m_pLast->m_pNext;
				m_ComboBoxList.m_pLast->ComboBox.Create(WS_VISIBLE|WS_CHILD|CBS_DROPDOWNLIST,
				CRect(0,0,0,0),this,m_dwHighestID++);
				//m_vecComboBoxes.push_back(combobox);
				//m_ComboBoxList.m_pLast->m_ComboBox.MoveWindow(CRect(xpos,ypos,xpos+size.cx+5,ypos+size.cy+5));
				CFont * pFont=m_ComboBoxList.m_pFirst->ComboBox.GetFont();
				CFont * pOldFont=pDC->SelectObject(pFont);
				DWORD dwWidth=0;
				for(DWORD dwRun2=0;dwRun2<cstrv.size();dwRun2++)
				{
					m_ComboBoxList.m_pLast->ComboBox.AddString(cstrv.at(dwRun2));
					//m_ComboBoxList.m_pFirst->ComboBox.SetDroppedWidth(500);
					CSize size=pDC->GetOutputTextExtent(cstrv.at(dwRun2));
					if(size.cx>(LONG)dwWidth)
						 dwWidth=size.cx;
				}
				
				m_ComboBoxList.m_pLast->ComboBox.MoveWindow(dwHorizontalStart,0,
					dwWidth+30,m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+
					(cstrv.size()+1)*m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(0));
				dwHorizontalStart+=dwWidth+30;
				m_ComboBoxList.m_pLast->ComboBox.SetCurSel(0);
				//m_ComboBoxList.m_pLast=m_ComboBoxList.m_pFirst;
				//m_ComboBoxList.m_pLast=m_ComboBoxList.m_pFirst->m_pNext;
				m_ComboBoxList.m_pLast->m_pNext=NULL;
				pDC->SelectObject(pOldFont);
				/*for(DWORD dwRun2=0;dwRun2<cstrv.size();dwRun2++)
				{
					m_ComboBoxList.m_pLast->ComboBox.AddString(cstrv.at(dwRun2));
				}
				m_ComboBoxList.m_pLast=m_ComboBoxList.m_pLast->m_pNext;
				m_ComboBoxList.m_pLast=NULL;*/
			}
			else
			{
				m_ComboBoxList.m_pFirst=new ComboBoxListElement;
				m_ComboBoxList.m_pFirst->ComboBox.Create(WS_VISIBLE|WS_CHILD|CBS_DROPDOWNLIST,
				CRect(0,0,0,0),this,m_dwHighestID++);
				// Höhe des Editcontrols+Produkt aus der Anzahl der Items und Höhe der Items
				//m_vecComboBoxes.push_back(combobox);
				//m_ComboBoxList.m_pLast->m_ComboBox.MoveWindow(CRect(xpos,ypos,xpos+size.cx+5,ypos+size.cy+5));
				CFont * pFont=m_ComboBoxList.m_pFirst->ComboBox.GetFont();
				CFont * pOldFont=pDC->SelectObject(pFont);
				DWORD dwWidth=0;
				for(DWORD dwRun2=0;dwRun2<cstrv.size();dwRun2++)
				{
					m_ComboBoxList.m_pFirst->ComboBox.AddString(cstrv.at(dwRun2));
					//m_ComboBoxList.m_pFirst->ComboBox.SetDroppedWidth(500);
					CSize size=pDC->GetOutputTextExtent(cstrv.at(dwRun2));
					if(size.cx>(LONG)dwWidth)
						 dwWidth=size.cx;
				}
				char buffer[30];
//Um die Warnungen über veraltete C-Bibliotheken zu deaktivieren
//#pragma warning disable 4996
#pragma warning(disable : 4996)
				ultoa(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1),buffer,10);
				//AfxMessageBox("itemheight(-1)"+CString(buffer),MB_OK,NULL);
				ultoa(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(0),buffer,10);
				//AfxMessageBox("itemheight(0)"+CString(buffer),MB_OK,NULL);
				ultoa(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+(cstrv.size()+1)*m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(0),buffer,10);
				//AfxMessageBox("itemhe(-1)*itemheight(0)"+CString(buffer),MB_OK,NULL);
				m_ComboBoxList.m_pFirst->ComboBox.MoveWindow(dwHorizontalStart,0,dwWidth+30,m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+(cstrv.size()+1)*m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(0));
				dwHorizontalStart+=dwWidth+30;
				m_ComboBoxList.m_pFirst->ComboBox.SetCurSel(0);
				//m_ComboBoxList.m_pLast=m_ComboBoxList.m_pFirst;
				m_ComboBoxList.m_pFirst->m_pNext=NULL;
				m_ComboBoxList.m_pLast=m_ComboBoxList.m_pFirst;
				m_ComboBoxList.m_pLast->m_pNext=NULL;
				pDC->SelectObject(pOldFont);
			}
			//combobox.ShowWindow(TRUE);
			//combobox.ShowWindow(SW_SHOW); // ShowWindow nur aßerhalb voon OnDraw aufrufen?
		}
		else
		{
			CSize size=pDC->GetOutputTextExtent(cstrv.at(0));
			dwHorizontalStart+=size.cx;
		}
			if(dwRun<m_cstrvv.size()-1)
			{
				cstrv=m_cstrvv.at(dwRun+1);
				CString str=cstrv.at(0);
				if(str!=","  && str!=".")
				{
					CSize size=pDC->GetOutputTextExtent(" ");
					dwHorizontalStart+=size.cx;
				}
			}
	}

	/*CComboBox combobox;
	combobox.Create(WS_CHILD|CBS_DROPDOWNLIST,
	CRect(xpos,ypos,xpos+size.cx+5,ypos+size.cy+5),this,
	36000);
	m_vecComboBoxes.push_back(combobox);
	combobox.MoveWindow(CRect(xpos,ypos,xpos+size.cx+5,ypos+size.cy+5));
	//combobox.ShowWindow(TRUE);
	combobox.ShowWindow(SW_SHOW); // ShowWindow nur aßerhalb voon OnDraw aufrufen?
	*/
	TRACE("void EnglishView::CreateComboBoxes() - ENDE\n");
	MoveComboBoxes();
}

void EnglishView::MoveComboBoxes()
{
	CDC * pDC=GetDC();
	CRect clientRect;
	GetClientRect(clientRect);
	DWORD dwHorizontalStart=0;
	ComboBoxListElement * pCurrent=NULL;
#ifdef _DEBUG
	printf("void EnglishView::MoveComboBoxes() - ANFANG\n");
#endif
	if(m_ComboBoxList.m_pFirst)
	{
		pCurrent=m_ComboBoxList.m_pFirst;
	}
	DWORD dwLine=0;
	for(DWORD dwRun=0;dwRun<m_cstrvv.size();dwRun++)
	{
		CStringVector cstrv=m_cstrvv.at(dwRun);
		TRACE("CStringVector cstrv=m_cstrvv.at(dwRun)\n");
		if(cstrv.size()>1) // Vektor hat mehr als 1 Element (CString)
		{
			if(pCurrent)
			{
				CFont * pFont=pCurrent->ComboBox.GetFont();
				CFont * pOldFont=pDC->SelectObject(pFont);
				DWORD dwWidth=0;
				for(DWORD dwRun2=0;dwRun2<cstrv.size();dwRun2++)
				{
					//m_ComboBoxList.m_pFirst->ComboBox.SetDroppedWidth(500);
					CSize size=pDC->GetOutputTextExtent(cstrv.at(dwRun2));
					if(size.cx>(LONG)dwWidth) // Länge der längsten Zeichenkettem in dwWidth speichern
						 dwWidth=size.cx;
				}
				if((LONG)(dwHorizontalStart+dwWidth+30)>clientRect.right-15 && 
          (LONG)(dwWidth+30)<=clientRect.right-15)
				{ // wenn das Kombinationsfeld nicht mehr in die aktuelle Zeile passt und
					// die Kombinationsfeldbreite kleiner als die Kindfensterbreite ist
					char buffer[30];
					ultoa(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+
						(cstrv.size()+1)*m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(0),buffer,10);
					//AfxMessageBox("zweit"+CString(buffer),MB_OK,NULL);
					ultoa((dwLine+1)*(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+5),buffer,10);
					//AfxMessageBox("erst"+CString(buffer),MB_OK,NULL);
					pCurrent->ComboBox.MoveWindow(0,++dwLine*
						(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+5),
						dwWidth+30,m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+
						(cstrv.size()+1)*m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(0));
					dwHorizontalStart=0;
				}
				else
				{
					char buffer[30];
					ultoa(dwLine*(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+5),buffer,10);
					//AfxMessageBox("erst"+CString(buffer),MB_OK,NULL);
					ultoa(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+
						(cstrv.size()+1)*m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(0),buffer,10);
//#pragma warning restore 4996
					//AfxMessageBox("zweit"+CString(buffer),MB_OK,NULL);
					pCurrent->ComboBox.MoveWindow(dwHorizontalStart,dwLine*
						(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+5),
						dwWidth+30,
						m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+
						(cstrv.size()+1)*m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(0));
				}
				dwHorizontalStart+=dwWidth+30;
				pDC->SelectObject(pOldFont);
				pCurrent=pCurrent->m_pNext;
			}
		}
		else
		{
			CSize size=pDC->GetOutputTextExtent(cstrv.at(0));
			if((LONG)dwHorizontalStart+size.cx>clientRect.right-15 && 
        size.cx<=clientRect.right-15)
			{
				dwHorizontalStart=size.cx;
				dwLine++;
			}
			else
				dwHorizontalStart+=size.cx;
		}
		if(dwRun<m_cstrvv.size()-1)
		{
			cstrv=m_cstrvv.at(dwRun+1);
			CString str=cstrv.at(0);
			if(str!=","  && str!=".")
			{
				CSize size=pDC->GetOutputTextExtent(" ");
				if((LONG)dwHorizontalStart+size.cx>clientRect.right-15 && 
          size.cx<=clientRect.right-15)
				{
					dwHorizontalStart=size.cx;
					dwLine++;
				}
				else
					dwHorizontalStart+=size.cx;
			}
		}
	}
}

int EnglishView::OnToolHitTest( CPoint point, TOOLINFO* pTI )
{
	return 0;
}

BOOL EnglishView::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
		strcpy(pTTT->szText,"hhjhh");
    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // idFrom ist der HWND des Tools
        nID = ::GetDlgCtrlID((HWND)nID);
        if(nID)
        {
            pTTT->lpszText = MAKEINTRESOURCE(nID);
            pTTT->hinst = AfxGetResourceHandle();
            return(TRUE);
        }
    }
    return(FALSE);
}


void EnglishView::CalculateNumberOfLines()
{
#ifdef _DEBUG
	printf("void EnglishView::CalculateNumberOfLines() - ANFANG\n");
	fflush(stdin);
#endif
	CRect clientRect;
	CVTransDoc * pDoc=NULL;
	DWORD dwNumberOfLines=0;
	DWORD dwHorizontalOffset=0;
	DWORD dwLine=0;
	DWORD pos=0;
	pDoc=GetDocument();
	GetClientRect(&clientRect);
	TRACE("nicht drawvector\n");
	if(m_pText)
		if(m_pText->m_first)
		{
			BOOL bAssignpBeginOfToken=FALSE; // TRUE: der Zeiger pBeginOfToken soll
			BOOL bAssignpBeginOfLine=TRUE;
			BOOL bPreviousCharacterWasCarriageReturn=FALSE;
			BOOL bSeperator=TRUE;
			BOOL bSeperatorOccured=FALSE;
			BOOL bTokendoesntFitIntoLine=FALSE;
			BOOL bDoOutput=FALSE;
			BOOL bSpaceOccured=FALSE;
			BYTE bType=0;
			CDC * pDC=GetDC();
			DWORD dwTokenWidth=0;
			DWORD dwLineWidth=0;
			DWORD dwMarkingBegin=m_dwMarkingStart<=m_dwMarkingEnd?m_dwMarkingStart:m_dwMarkingEnd;
			DWORD dwMarkingEnd=m_dwMarkingStart>=m_dwMarkingEnd?m_dwMarkingStart:m_dwMarkingEnd;
			DWORD dwNewLines=0;
			DWORD dwRun=pos;
			DWORD dwStart=dwRun;
			DWORD dwPositionAfterOutput=0;
			DWORD dwRealPos=0;//wirkliche Position, dezimal 13 und dezimal 13 und dezimal
			DWORD dwPositionUntilCurrentToken=0;
			TextNode * pBeginOfToken=NULL;//current;
			TextNode * pBeginOfLine=NULL;//current;
			TextNode * current=m_pText->m_first;
			TextNode * pLineStopChar=current;
			TextNode * pBeginOfWord=current;
#ifdef _DEBUG
			printf("clientRect.right-15: %u\n",clientRect.right-15);
#endif
			while(1)
			{
#ifdef _DEBUG
//				if(current)
//					printf("current->ch: %u %c dwRun: %u\n",current->ch,current->ch,dwRun);
//				else
//					printf("!current dwRun: %u\n",dwRun);
//				printf("dwTokenWidth: %u\n",dwTokenWidth);
#endif
				if(current)
				{
					if(current->ch==13)
					{
						dwNumberOfLines++;
						//dwRealPos++;
						//bAssignpBeginOfToken=TRUE;
						//bAssignpBeginOfLine=TRUE;
						dwLineWidth=0;
						dwTokenWidth=0;
						bPreviousCharacterWasCarriageReturn=TRUE;
					}
					else if(current->ch==' ')
					{
#ifdef _DEBUG
						printf("current->ch==' '\n");
#endif
						bPreviousCharacterWasCarriageReturn=FALSE;
						//bSpaceOccured=TRUE;
						CSize size=pDC->GetOutputTextExtent(
              //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
              //is faster than creating a CString(current->ch) in order to use 
              //GetOutputTextExtent(const CString& str ) const; .
              //&(CHAR)current->ch
              CString((TCHAR)current->ch),1);
						dwLineWidth+=dwTokenWidth+size.cx;
						dwTokenWidth=0;
						if((LONG)dwLineWidth>clientRect.right-15)
						{ // wenn das Leerzeichen nicht mehr in die aktuelle Zeile passt
							dwNumberOfLines++;
							// wenn das Leerzeichen in eine Zeile passt
							//if(size.cx>clientRect.right-15)
							//	dwLineWidth=0;
							//else // Token passt nicht in eine Zeile, also alle Zeichen
							//{ // bis zum rechten Rand ausgeben
#ifdef _DEBUG
							//	printf("bTokendoesntFitIntoLine=TRUE\n");
#endif
							//}
							dwLineWidth=0;
							dwTokenWidth=size.cx;
						}
					}
					else if(bPreviousCharacterWasCarriageReturn && current->ch==10)
						bPreviousCharacterWasCarriageReturn=FALSE;
					else // alle Zeichen ausser Leerzeichen, 13 und 10 mit vorigen 13
					{
						bPreviousCharacterWasCarriageReturn=FALSE;
						CSize size=pDC->GetOutputTextExtent(
              //Using GetOutputTextExtent(LPCTSTR lpszString,int nCount ) const;
              //is faster than creating a CString(current->ch) in order to use 
              //GetOutputTextExtent(const CString& str ) const; .
              //&(CHAR)current->ch
              CString((TCHAR)current->ch),1);
#ifdef _DEBUG
						//printf("dwTokenWidth: %u size.cx: %u\n",dwTokenWidth,size.cx);
						//printf("dwHorizontalOffset: %u dwLineWidth: %u\n",dwHorizontalOffset,dwLineWidth);
#endif
						dwTokenWidth+=size.cx;
						if((LONG)(dwLineWidth+dwTokenWidth)>clientRect.right-15)
						{ // wenn das Token nicht mehr in die aktuelle Zeile passt
#ifdef _DEBUG
							printf("dwLineWidth+dwTokenWidth>clientRect.right-15\n");
#endif
							dwNumberOfLines++;
							//if(dwTokenWidth<=clientRect.right-15) // wenn das Token 
							//{ // in eine Zeile passt

							//}
							if((LONG)dwTokenWidth>clientRect.right-15)//else // Token passt nicht in eine Zeile, also alle Zeichen
							{ // bis zum rechten Rand ausgeben
#ifdef _DEBUG
								printf("bTokendoesntFitIntoLine=TRUE\n");
								printf("dwTokenWidth: %u\n",dwTokenWidth);
#endif
								//bTokendoesntFitIntoLine=TRUE;
								dwTokenWidth=size.cx;
							}
							dwLineWidth=0;
						}
					}
				}
				else
					break;
				
					/*if(current->ch==13 || (bPreviousCharacterWasCarriageReturn && current->ch==10) || current->ch==' ')
					{
						bPreviousCharacterWasCarriageReturn=FALSE;
						if(current->ch==' ')
						{
							bType=1;
							dwRealPos++;
							if(bAssignpBeginOfLine)
							{
								pBeginOfLine=current;
								bAssignpBeginOfLine=FALSE;
							}
							bSpaceOccured=TRUE;
							CSize size=pDC->GetOutputTextExtent(current->ch);
							dwLineWidth+=size.cx;
							if(dwHorizontalOffset+dwLineWidth+dwTokenWidth>clientRect.right-15)
							{ // wenn das Wort nicht mehr in die aktuelle Zeile passt
								dwNumberOfLines++;
								bDoOutput=TRUE;
								if(dwLineWidth+dwTokenWidth<=clientRect.right-15) // wenn das Wort in eine Zeile passt
								{
									dwHorizontalOffset=0;
									dwLine++;
								}
								else // Token passt nicht in eine Zeile, also alle Zeichen
								{ // bis zum rechten Rand ausgeben
#ifdef _DEBUG
									printf("bTokendoesntFitIntoLine=TRUE\n");
#endif
									bTokendoesntFitIntoLine=TRUE;
								}
							}
							else
								dwPositionUntilCurrentToken=dwRealPos;
						}
						if(current->ch==13)
						//else
						{
							dwNumberOfLines++;
							dwRealPos++;
							bType=3;
							bAssignpBeginOfToken=TRUE;
							bAssignpBeginOfLine=TRUE;
							if(dwNewLines<m_dwStartLine)
							{
								dwTokenWidth=0;
								dwLineWidth=0;
								dwNewLines++;
								dwHorizontalOffset=0;
								pBeginOfLine=NULL;
								dwPositionAfterOutput=dwRealPos;
							}
							else
							{
								bDoOutput=TRUE;
								pBeginOfToken=NULL;
							}
						}
					}
					else
					{
						dwRealPos++;
						bType=0;
						if(bAssignpBeginOfLine)
						{
							pBeginOfLine=current;
							bAssignpBeginOfLine=FALSE;
						}
						if(bSpaceOccured)
						{
#ifdef _DEBUG
							printf("bSpaceOccured\n");
#endif
							pLineStopChar=current;
							pBeginOfToken=current;
							dwLineWidth+=dwTokenWidth;
							dwTokenWidth=0;
							bSpaceOccured=FALSE;
						}
						bType=CHARACTER;
						CSize size=pDC->GetOutputTextExtent(current->ch);
#ifdef _DEBUG
						printf("dwTokenWidth: %u size.cx: %u\n",dwTokenWidth,size.cx);
						printf("dwHorizontalOffset: %u dwLineWidth: %u\n",dwHorizontalOffset,dwLineWidth);
#endif
						dwTokenWidth+=size.cx;
						if(dwHorizontalOffset+dwLineWidth+dwTokenWidth>clientRect.right-15)
						{ // wenn das Token nicht mehr in die aktuelle Zeile passt
							dwNumberOfLines++;
							if(dwNewLines==m_dwStartLine)
								bDoOutput=TRUE;
							if(dwTokenWidth<=clientRect.right-15) // wenn das Token 
							{ // in eine Zeile passt
								if(dwNewLines<m_dwStartLine)
								{
									pBeginOfLine=pBeginOfToken;
									dwNewLines++;
									dwTokenWidth=0;
									dwLineWidth=0;
									dwHorizontalOffset=0;
								}
							}
							else // Token passt nicht in eine Zeile, also alle Zeichen
							{ // bis zum rechten Rand ausgeben
								if(dwNewLines<m_dwStartLine)
								{
									pBeginOfLine=current;
									dwNewLines++;
									dwTokenWidth=0;
									dwLineWidth=0;
								}
								else
								{
#ifdef _DEBUG
									printf("bTokendoesntFitIntoLine=TRUE\n");
									printf("dwTokenWidth: %u\n",dwTokenWidth);
#endif
									bTokendoesntFitIntoLine=TRUE;
								}
							}
						}
					}
				}
				else
				{
					if(pBeginOfLine)
						pBeginOfToken=NULL;
					bSeperator=FALSE;
					bDoOutput=TRUE;
					bType=2;
				}
				if(bDoOutput)//!bSeperator)
				{
#ifdef _DEBUG
					printf("bDoOutput  dwRealPos: %lu  dwPositionAfterOutput: %lu  m_CursorPos: %lu\n",dwRealPos,dwPositionAfterOutput,m_CursorPos);
#endif
					if(pBeginOfLine)
					{
#ifdef _DEBUG
						printf("pBeginOfLine\n");
#endif
						CSize size;
						if(pBeginOfToken) // ein ganzes Token passt in die Zeile
						{ //
#ifdef _DEBUG
							printf("pBeginOfToken\n");
#endif
							if(dwTokenWidth<=clientRect.right-15)
							{
#ifdef _DEBUG
								printf("dwTokenWidth<=clientRect.right-15\n");
#endif
								dwHorizontalOffset=0;
								dwLine++;
							}
							dwLineWidth=0;
#ifdef _DEBUG
							printf("dwTokenWidth: %u\n",dwTokenWidth);
#endif
						pBeginOfLine=pBeginOfToken;
						pBeginOfToken=NULL;
					}
					else
					{
#ifdef _DEBUG
						printf("!pBeginOfToken\n");
#endif
						if(bType==3)
						{
#ifdef _DEBUG
							printf("if(bType==3)\n");
#endif
							pBeginOfLine=NULL;
						}
						if(bType==2)// der Grund, warum eine Ausgabe erfolgte ist
						{// das Listenende
						}
						if(bTokendoesntFitIntoLine)
						{
#ifdef _DEBUG
							printf("pBeginOfLine=current\n");
#endif
							pBeginOfLine=current;
						}
						if(bType==3)
						{
							dwTokenWidth=0;
							dwLineWidth=0;
							dwLine++;
							dwHorizontalOffset=0;
						}
					}
					if(bTokendoesntFitIntoLine)
					{
#ifdef _DEBUG
						printf("2. bTokendoesntFitIntoLine\n");
#endif
						dwHorizontalOffset=0;
						dwLine++;
						dwLineWidth=0;
						dwTokenWidth=0;
						if(bType==CHARACTER)
						{
							CSize size=pDC->GetOutputTextExtent(current->ch);
							dwTokenWidth=size.cx;
#ifdef _DEBUG
							printf("dwTokenWidth: %u\n",dwTokenWidth);
#endif
						}
					}
					else
					{
					}
					bType=0;
					bDoOutput=FALSE;
				}
				else // pBeginOfLine==FALSE
				{
#ifdef _DEBUG
					printf("!pBeginOfLine\n");
#endif
					if(pBeginOfToken)
					{
#ifdef _DEBUG
						printf("pBeginOfToken\n");
#endif
						if(bTokendoesntFitIntoLine)
						{
							dwHorizontalOffset=0;
							dwLine++;
							if(bType==CHARACTER)
							{
								CSize size=pDC->GetOutputTextExtent(current->ch);
								pBeginOfToken=current;
								dwTokenWidth=size.cx;
							}
						}
						else
						{
							dwTokenWidth=0;
						}
						bType=0;
						bDoOutput=FALSE;
					}
					else// dieser Programmzweig wird ausgeführt, wenn in einer Zeile
					{// ausschliesslich ein newline (dezimal: 13) steht
#ifdef _DEBUG
						printf("!pBeginOfToken\n");
#endif
						dwLine++;
						bAssignpBeginOfLine=TRUE;
					}
				}
				if(bTokendoesntFitIntoLine)
				{
					dwPositionAfterOutput=dwPositionUntilCurrentToken;//++;dwRealPos-1;
					bTokendoesntFitIntoLine=FALSE;
				}
				else
					dwPositionAfterOutput=dwPositionUntilCurrentToken;
			}
			if(!current)//Listenende
				break;
			if(current->ch==13)
				bPreviousCharacterWasCarriageReturn=TRUE;*/
			current=current->m_next;
			//dwRun++;
		}
	}// if(m_pText->m_first)
	//if(pDoc)
	m_dwNumberOfLines=dwNumberOfLines;
#ifdef _DEBUG
	printf("m_dwNumberOfLines: %lu\n",m_dwNumberOfLines);
	printf("void EnglishView::CalculateNumberOfLines() - ENDE\n");
#endif
}

void EnglishView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen
	m_bHasFocus=1;
}
