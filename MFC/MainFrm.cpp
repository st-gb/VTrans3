// MainFrm.cpp : Implementierung der Klasse CMainFrame
//

#include "../stdafx.h"
#include "VTrans.h"

//#define USE_BISON_PARSER
#ifdef USE_BISON_PARSER
  #include "../simple_clause/simple_clause_c_pp.tab.hpp"
#endif

#include "MainFrm.h"
// eigener Code, Anfang
#include <afxmt.h>//für CEvent zum Synchronisieren von Threads
#include "ChildFrm.h"
#include "../Translate.h"
//#include "EnglishView.h"
#include "typeinfo.h" // Einbinden, um typeid verwenden zu können
#include "OptionsDialog.h"
#include "../VocabularyInMainMem/LetterTree/LetterTree.hpp"
#include "VocabularyInMainMem/DoublyLinkedList/WordList.hpp"
//#include "StatusDialog.h"
#include <math.h> // für ceil()
#ifdef USE_LETTER_TREE
#include "../Parse/sequential.h"
#endif
// eigener Code, Ende
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// eigener Code, Anfang
//#include"Vocable.h"
//extern VocNode * m_first;
extern volatile BOOL g_bExecutedOnTranslateAutomaticallyFinishes;
extern volatile BYTE g_bTranslateAutomaticallyBeforeEnd;

CEvent g_eventEndThread(FALSE,TRUE);
extern WordNode * pWordNodeCurrent;
extern WordList wordList;
extern CTime tAtTheBeginningOfTheTranslation;

//die nachfolgenden 3 Variablen werden für den Datanaustausch mit 
//CMainFrame::OnTranslateAutomaticallyFinishes genutzt
extern volatile unsigned long g_ulChar;
extern volatile DWORD * g_pdwCursorPosition;
extern Text * g_pText;
extern CMainFrame * g_pMainFrame;
extern LetterTree g_lettertree ;
// eigener Code, Ende

extern volatile BYTE g_bContinueLoading;
//WordNode * m_first=NULL;
extern volatile BYTE g_bContinue;
extern volatile DWORD g_dwStatus;
extern volatile DWORD g_dwSentenceStatus;
extern volatile DWORD g_dwTotal;
extern volatile DWORD g_dwTranslated;
extern volatile DWORD g_dwTokensInSentence;


extern std::ofstream ofstreamLogFile;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_TRANSLATE_BUTTON, OnTranslateButton)
	ON_COMMAND(ID_EXTRAS_COUNT_WORDS, OnExtrasCountWords)
	ON_COMMAND(ID_VIEW_VOCABLEEDITOR, OnVocableEditorButton)
	ON_COMMAND(ID_VIEW_SEARCH_FORM, OnViewSearchForm)
	ON_WM_DESTROY()
	ON_COMMAND(ID_ADD_VOCABLE_BUTTON, OnAddVocableButton)
	ON_WM_DESTROYCLIPBOARD()//,OnDestroyClipboard)
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_OVERWRITE_VOCABLE_BUTTON, OnOverwriteVocableButton)
	ON_COMMAND(ID_DELETE_VOCABLE_BUTTON, OnDeleteVocableButton)
	ON_COMMAND(ID_COMBOBOX_BUTTON, OnComboboxButton)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_NEXT_VOCABLE_BUTTON, OnUpdateNextVocableButton)
	ON_COMMAND(ID_NEXT_VOCABLE_BUTTON, OnNextVocableButton)
	ON_UPDATE_COMMAND_UI(ID_PREVIOUS_VOCABLE_BUTTON, OnUpdatePreviousVocableButton)
	ON_COMMAND(ID_PREVIOUS_VOCABLE_BUTTON, OnPreviousVocableButton)
	ON_UPDATE_COMMAND_UI(ID_TRANSLATE_AUTOMATICALLY_IF_TEXTMODIFICATION_BUTTON, OnUpdateTranslateAutomaticallyIfTextmodificationButton)
	ON_COMMAND(ID_TRANSLATE_AUTOMATICALLY_IF_TEXTMODIFICATION_BUTTON, OnTranslateAutomaticallyIfTextmodificationButton)
	ON_COMMAND(ID_EXTRAS_OPTIONEN, OnExtrasOptionen)
	ON_UPDATE_COMMAND_UI(ID_DELETE_VOCABLE_BUTTON, OnUpdateDeleteVocableButton)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_MARK_ALL, OnEditMarkAll)
	ON_UPDATE_COMMAND_UI(ID_COMBOBOX_BUTTON, OnUpdateComboboxButton)
	ON_UPDATE_COMMAND_UI(ID_TRANSLATE_BUTTON, OnUpdateTranslateButton)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
	// Globale Hilfebefehle
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
	ON_MESSAGE(WM_TRANSLATE_AUTOMATICALLY_FINISHES,OnTranslateAutomaticallyFinishes)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // Statusleistenanzeige
	//ID_INDICATOR_PROGRESS_PANE, // Fortschrittsleiste
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


UINT Translate(LPVOID pParam)
{
	BYTE bComboBoxes=0;
	DWORD dwFlags=0;
#ifdef _DEBUG
	printf("UINT Translate(LPVOID pParam )\n");
#endif
	TranslateParameters * pTranslateParameters=(TranslateParameters*)pParam;
	//CMyObject* pObject = (CMyObject*)pParam;
  if (pTranslateParameters == NULL)
    return 1;   // falls pTranslateParameters ungültig ist

	//if(::IsWindow(pTranslateParameters->m_statusDlg->m_hWnd))
	// solange warten, 
	// bis das Fenster-Handle gültig ist (dauert nur wenige Millisekunden)
	// (Das Fenster-Handle ist sonst in manchen Aufrufen von Translate()
	// ungültig und es kann dann kein Timer installiert werden;
	// SetTimer() funktioniert nur, wenn das Fenster ein gültiges Handle hat
	while(!::IsWindow(//*pTranslateParameters->m_pHWnd
    //m_hwnd
    pTranslateParameters->m_statusDlg->GetSafeHwnd()));
	if(pTranslateParameters->m_statusDlg->SetTimer(1,1000,NULL)==0)
		TRACE("Fehler bei Settimer\n");
	tAtTheBeginningOfTheTranslation=(CTime)CTime::GetCurrentTime();
	dwFlags&=FALSE;//pFrame->m_bCaseSensitive;
  LOG2N("23.05.2008 10.37.44","before translating text");

	*pTranslateParameters->m_pVecVecStr=TranslateText(
		*pTranslateParameters->m_pText,pTranslateParameters->m_vecRange,
		dwFlags,//pTranslateParameters->m_pEV
    pTranslateParameters->m_pEV->m_vecIntPair);
  LOG2N("23.5.08 10.38.15 j","after translating text");
	for(DWORD dwRangeIndex=0;g_bContinue && dwRangeIndex<pTranslateParameters->
		m_vecRange.size();dwRangeIndex++)
	{
		Range range=pTranslateParameters->m_vecRange.at(dwRangeIndex);
		for(DWORD dwCharacterIndexWithinEnglishText=range.m_dwStart;
      dwCharacterIndexWithinEnglishText<range.m_dwEnd;
      dwCharacterIndexWithinEnglishText++)
			pTranslateParameters->m_pText->SetColor(dwCharacterIndexWithinEnglishText,2);
	}
	for(DWORD dwRun=0;g_bContinue && dwRun<pTranslateParameters->
		m_pEnglishViewGerman->m_cstrvv.size();dwRun++)
	{
		CStringVector sv=pTranslateParameters->m_pEnglishViewGerman->m_cstrvv.
			at(dwRun);
		if(sv.size()>1)
		{
			//pView->m_bDrawVector=FALSE;
			bComboBoxes=1;
			break;
		}
	}
  LOGN("25.5.08 15.14.17 bla"<<(int)bComboBoxes);
	if(!bComboBoxes)
	{
		DWORD dwCharsPerLine=0;
		DWORD dwPos=0;
		//wichtig!: zuerst alle Elemente (Zeichen) von m_pText löschen
		pTranslateParameters->m_pEnglishViewGerman->m_pText->Clear();
		pTranslateParameters->m_pEnglishViewGerman->m_CursorPos=0;
		pTranslateParameters->m_pEnglishViewGerman->m_bDrawVector=0;
		//pTranslateParameters->m_dwMaxCharsPerLine=1000;
		for(DWORD dwRun=0;g_bContinue && dwRun<pTranslateParameters->
			m_pEnglishViewGerman->m_cstrvv.size();dwRun++)
		{
      LOGN("25.5.08 20.23.09 "<<pTranslateParameters->m_pEnglishViewGerman->
				m_cstrvv.size());
			//dwPos+=pEV->m_cstrvv.at(dwRun).GetLength();
			CStringVector cstrv=pTranslateParameters->m_pEnglishViewGerman->
				m_cstrvv.at(dwRun);
			CString str=cstrv.at(0);
			//wenn kein Komma und kein Punkt und pTranslateParameters->
			//m_pEnglishViewGerman->m_pText schon mindestens 1 Token hinzugefügt
			//wurde (zwischen der Zeichenkette vor einem Komma oder einem Punkt 
			//kommt kein Leerzeichen)
			if(str!=","  && str!="." && dwRun>0)
			{
				//wenn bis zum Erreichen der maximalen Anzahl der Zeichen pro Zeile
				//noch mindestens ein Zeichen (Leerzeichen) passt, ...
				if(dwCharsPerLine+1<=pTranslateParameters->
					m_dwMaxCharsPerLine)
				{
					//if(!bFirstNode)
					//	pEV->m_pText->Insert(dwPos,"\r\n",0);
					//dwPos+=2;
					//pEV->m_pText->Insert(dwPos,str.GetBuffer(str.GetLength()),0);
					//dwCharsPerLine=0;
					//...dann ein Leerzeichen einfügen
					pTranslateParameters->m_pEnglishViewGerman->m_pText->
						Insert(dwPos,' ',0);
					dwPos++;
					++dwCharsPerLine;
				}
			}
			//wenn die aktuelle Anzahl der Zeichen pro Zeile plus der Länge der
			//aktuellen Zeichenkette größer ist als die maximale Anzahl der 
			//Zeichen pro Zeile,...
			if(dwCharsPerLine+str.GetLength()>pTranslateParameters->
				m_dwMaxCharsPerLine)
			{
				//...dann eine neue Zeile einleiten mit den 2 Bytes für eine neue 
				//Zeile in einer DOS-/Windows-Textdatei: "13" "10" ("\r" "\n")
				pTranslateParameters->m_pEnglishViewGerman->m_pText->
					Insert(dwPos,"\r\n",0);
				//die 2 Bytes für eine neue Zeile in einer DOS-/Windows-Textdatei:
				//"13" "10" ("\r" "\n") gelten als 1 Position
				++dwPos;
				//pEV->m_pText->Insert(dwPos,str.GetBuffer(str.GetLength()),0);
				dwCharsPerLine=0;
			}
			//else
			pTranslateParameters->m_pEnglishViewGerman->m_pText->Insert(dwPos,
				str.GetBuffer(str.GetLength()),0);
			//pTranslateParameters->m_pEnglishViewGerman->m_pText->GetLength();
			dwPos+=str.GetLength();
			dwCharsPerLine+=str.GetLength();
		}
	}
  LOGN("25.5.08 20.18.25");
	if(::IsWindow(pTranslateParameters->m_statusDlg->m_hWnd))
	{
    //The HWND to the status dialog could be copied to pTranslateParameters
    //because it is NULL of it is not shown. So do it like this:
    CDialog * pdialog=(CDialog*)CDialog::FromHandle(pTranslateParameters->
      m_statusDlg->GetSafeHwnd());
		pTranslateParameters->m_statusDlg->KillTimer(1);
		//pTranslateParameters->m_statusDlg->EndDialog(IDOK);
    pdialog->EndDialog(IDOK);
	}
  LOGN("25.5.08 20.18.37");
	return 0;
}

UINT TranslateAutomatically(LPVOID pParam)
{
	DWORD dwFlags=0;
	CStatusBar wndStatusBar;
#ifdef _DEBUG
	printf("UINT TranslateAutomatically(LPVOID pParam) ANFANG\n");
#endif
		g_bExecutedOnTranslateAutomaticallyFinishes=FALSE;
#ifdef _DEBUG
		printf("nach: g_bExecutedOnTranslateAutomaticallyFinishes=FALSE;\n");
#endif
	TranslateAutomaticallyParameters * pTranslateAutomaticallyParameters=
		(TranslateAutomaticallyParameters*)pParam;
	//CMyObject* pObject = (CMyObject*)pParam;
  if(pTranslateAutomaticallyParameters == NULL)
	{
#ifdef _DEBUG
		printf("UINT TranslateAutomatically(LPVOID pParam) - return 1 - \
pTranslateAutomaticallyParameters == NULL\n");
#endif
    return 1;   // falls pTranslateParameters ungültig ist
	}
	//if(::IsWindow(pTranslateParameters->m_statusDlg->m_hWnd))
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	//CMainFrame * pFrame=new CMainFrame;
	//CMainFrame * pFrame=(CMainFrame *)
		//CMainFrame::FromHandle(pTranslateAutomaticallyParameters->m_postHWnd);
#ifdef _DEBUG
	printf("vor pFrame->Attach\n");
#endif
	//pFrame->Attach(pTranslateAutomaticallyParameters->m_postHWnd);

	//Ein BOOL-Wert ist vom Datentyp int (belegt also 2 oder 4 Byte).
	//Ein BOOL hat entweder den Wert TRUE (1) oder FALSE (0).
	//die folgenden Ja-Nein-Werte werden in dwFlags gespeichert.
	//dwFlags ist vom Datentyp "unsigned long", aus dem durch
	//die Anweisung "typedef DWORD unsigned long" der Datentyp "DWORD"
	//wurde. Da der Datentyp "unsigned long" 4 Byte belegt, lassen
	//sich 32 verschiedene Ja-Nein-Werte darin speichern (1 Byte=8 Bit;
	//4*8=32). 
  //Um die BOOL-Werte in einer einzigen "unsigned long"-Variable
	//zu speichern, muss die "unsigned long"-Variable bitweise-Oder
	//mit dem BOOL-Wert verknüpft werden. Das heisst, dass wenn ein
	//mindestens 1 Bit an der Position gesetzt ist, erhält das
	//Ergebnis den Wert '1'.
	//Jeder weitere BOOL-Wert-der in dwFlags gespeichert werden soll,
	//muss eine Position weiter links gespeichert werden. Das erreicht
	//man, indem man den BOOL-Wert mit "2 hoch 'Index des BOOL-Wertes,
	//der gespeichert werden soll'" multipliziert. Will man also den
	//zweiten BOOL-Wert in dwFlags speichern, rechnet man "'zweiter BOOL-
	//Wert'*2". Ist der BOOL-Wert TRUE (1) entsteht dadurch: 1*"2hoch1"=2.
	//Das ist binär: 00000000000000000000000000000010. Wenn dieser BOOL-
	//Wert FALSE (0) wäre, ergäbe das eine 0 (binär:
	//00000000000000000000000000000000).
	//Will man den 3. BOOL-Wert speichern, rechnet man BOOL-Wert*"2hoch2".
	//(=BOOL-Wert*4).

	//dwFlags:                                 00000000000000000000000000000000
	//wenn pFrame->m_bCaseSensitive=TRUE (1):  00000000000000000000000000000001
	// dann ist das Ergebnis:                  00000000000000000000000000000001
	//wenn pFrame->m_bCaseSensitive=FALSE (0): 00000000000000000000000000000001
	// dann ist das Ergebnis:                  00000000000000000000000000000000
	dwFlags|=pTranslateAutomaticallyParameters->m_bCaseSensitive;
#ifdef _DEBUG
	printf("pTranslateAutomaticallyParameters->m_bCaseSensitive: %u dwFlags: \
%u\n",pTranslateAutomaticallyParameters->m_bCaseSensitive,dwFlags);
#endif
	//wenn pFrame->m_bAdoptGrammaticallyIncorrectSentences=TRUE, dann ergibt 
	//das: 00000000000000000000000000000010
	dwFlags|=pTranslateAutomaticallyParameters->
		m_bAdoptGrammaticallyIncorrectSentences*2;
#ifdef _DEBUG
	printf("pFrame->m_bAdoptGrammaticallyIncorrectSentences: %u dwFlags: %u\n",
		pTranslateAutomaticallyParameters->
		m_bAdoptGrammaticallyIncorrectSentences,dwFlags);
#endif
	//wenn pFrame->m_bAllowLowLetterAtTheBeginningOfSentence=TRUE, dann ergibt 
	//das: 00000000000000000000000000000100
	dwFlags|=pTranslateAutomaticallyParameters->
		m_bAllowLowLetterAtTheBeginningOfSentence*4;
#ifdef _DEBUG
	printf("pFrame->m_bAllowLowLetterAtTheBeginningOfSentence: %u dwFlags: %u\n",
		pTranslateAutomaticallyParameters->
		m_bAllowLowLetterAtTheBeginningOfSentence,dwFlags);
#endif
	pTranslateAutomaticallyParameters->m_pEV->m_cstrvv=
		TranslateText(*pTranslateAutomaticallyParameters->m_pText,
		pTranslateAutomaticallyParameters->m_vecRange,dwFlags,
		//pTranslateAutomaticallyParameters->m_pEV
    pTranslateAutomaticallyParameters->m_pEV->m_vecIntPair);
	//if(g_bContinue)
	//{
		for(DWORD dwRun=0;dwRun<pTranslateAutomaticallyParameters->m_vecRange.size();dwRun++)
		{
			Range range=pTranslateAutomaticallyParameters->m_vecRange.at(dwRun);
			for(DWORD dwRun2=range.m_dwStart;dwRun2<range.m_dwEnd;dwRun2++)
				pTranslateAutomaticallyParameters->m_pText->SetColor(dwRun2,2);
		}
		EnglishView View,view2;
		EnglishView * pView;
		pView=(EnglishView *)EnglishView::FromHandle(pTranslateAutomaticallyParameters->m_hWnd);
		//pView=(EnglishView *)EnglishView::FromHandle(pTranslateAutomaticallyParameters->m_hWnd);
		//View.Attach(pTranslateAutomaticallyParameters->m_hWnd);
		view2.Attach(pTranslateAutomaticallyParameters->m_hWnd);
		//pTranslateAutomaticallyParameters->m_hWndKillTimer(1)==FALSE)
		//pFrame->KillTimer(1);
		{
#ifdef _DEBUG
			printf("ProgressControl.Attach(pTranslateAutomaticallyParameters->\
m_hWndProgress)\n");
#endif
			//CProgressCtrl * ProgressControl=(CProgressCtrl *)
			CProgressCtrl ProgressControl;
			//CProgressCtrl::FromHandle(pTranslateAutomaticallyParameters->m_hWndProgress);
			//ProgressControl.Attach(pTranslateAutomaticallyParameters->m_hWndProgress);
			//ProgressControl.DestroyWindow();
			//ProgressControl.ShowWindow(SW_HIDE);
			//ProgressControl.Detach();
			//ProgressControl->DestroyWindow();
			//::DestroyWindow(pTranslateAutomaticallyParameters->m_hWndProgress);
		}
		{
#ifdef _DEBUG
			printf("ProgressControl.Attach(pTranslateAutomaticallyParameters->\
m_hWndSentenceProgress)\n");
#endif
			CProgressCtrl ProgressControl;
			//ProgressControl.Attach(pTranslateAutomaticallyParameters->m_hWndSentenceProgress);
			//ProgressControl.DestroyWindow();
			//::DestroyWindow(pTranslateAutomaticallyParameters->m_hWndSentenceProgress);
		//DestroyWindow(pTranslateAutomaticallyParameters->m_hWndProgress);
		}
		{
			CProgressCtrl ProgressControl;
			//ProgressControl.Attach(pTranslateAutomaticallyParameters->m_hWndTextProgress);
			//::DestroyWindow(pTranslateAutomaticallyParameters->m_hWndTextProgress);
		}
		/*pFrame->m_bCreated=FALSE;
		UINT IDArray[]={ID_SEPARATOR,ID_INDICATOR_CAPS,ID_INDICATOR_NUM,ID_INDICATOR_SCRL};
		wndStatusBar.Attach(pTranslateAutomaticallyParameters->m_hWndStatusBar);
		wndStatusBar.SetIndicators(IDArray,sizeof(IDArray)/sizeof(UINT));
		pFrame->SetWindowText("jsjpppfj");
		pFrame->m_bMayStartThread=TRUE;
		//toolBarCtrl.HideButton(ID_TRANSLATE_BUTTON,FALSE);
		char buffer[20];
		CTime tCurrent=CTime::GetCurrentTime();
		CTimeSpan ts(tCurrent.GetTime());
		ts-=CTimeSpan(tAtTheBeginningOfTheTranslation.GetTime());
		_ultoa(ts.GetTotalSeconds(),buffer,10);
		CString strAmount(buffer);
		wndStatusBar.SetWindowText(_T("Übersetzung in "+strAmount+" Sekunde(n)"));
		wndStatusBar.Detach();
		pFrame->Detach();*/
		//delete pFrame;
		//DestroyWindow(pTranslateAutomaticallyParameters->m_hWndSentenceProgress);
		//DestroyWindow(pTranslateAutomaticallyParameters->m_hWndTextProgress);
		//View.m_ComboBoxList.Clear();
		//View.CreateComboBoxes();
		//View.Detach();
		//pTranslateAutomaticallyParameters->m_pEV->m_ComboBoxList.Clear(); // zuerst alle ComboBoxen 
			// (Kombinationfelder) entfernen
		//pTranslateAutomaticallyParameters->m_pEV->CreateComboBoxes(); // Kombinationsfelder erzeugen
		//pTranslateAutomaticallyParameters->m_pEV->Invalidate();
	//}
		//CMainFrame mainFrame;
		//mainFrame.Attach(pTranslateAutomaticallyParameters->m_postHWnd);
		//mainFrame.KillTimer(1);
		//mainFrame.m_Progress.DestroyWindow();
		//mainFrame.m_SentenceProgress.DestroyWindow();
		//mainFrame.m_TextProgress.DestroyWindow();
		//mainFrame.Detach();
		//g_bExecutedOnTranslateAutomaticallyFinishes=FALSE;
		//CProgressCtrl progressCtrl, * pCtrl;
		//progressCtrl.Attach(pTranslateAutomaticallyParameters->m_hWndProgress);
		//pFrame->m_Progress.DestroyWindow();
		//pCtrl=(CProgressCtrl*)progressCtrl.FromHandle(pTranslateAutomaticallyParameters->m_hWndProgress);
		//if(!::DestroyWindow(pTranslateAutomaticallyParameters->m_hWndProgress))
		//{
		//	printf("::DestroyWindow fehlgeschlagen\n");
		//}
		/*if(!progressCtrl.DestroyWindow())
		{
			printf("DestroyWindow fehlgeschlagen\n");
		}
		if(!pCtrl->DestroyWindow())
		{
			printf("DestroyWindow fehlgeschlagen\n");
		}*
		//progressCtrl.Detach();
		/*progressCtrl.Attach(pTranslateAutomaticallyParameters->m_hWndSentenceProgress);
		progressCtrl.DestroyWindow();
		progressCtrl.Detach();
		progressCtrl.Attach(pTranslateAutomaticallyParameters->m_hWndTextProgress);
		progressCtrl.DestroyWindow();
		progressCtrl.Detach();*/

		//pFrame->m_Progress.DestroyWindow();
		//pFrame->m_SentenceProgress.DestroyWindow();
		//pFrame->m_TextProgress.DestroyWindow();
		//pFrame->m_bCreated=FALSE;
		//UINT IDArray[]={ID_SEPARATOR,ID_INDICATOR_CAPS,ID_INDICATOR_NUM,ID_INDICATOR_SCRL};
		//pFrame->m_wndStatusBar.SetIndicators(IDArray,4);
		pFrame->m_bMayStartThread=TRUE;
		BYTE bComboBoxes=0;
	for(dwRun=0;dwRun<pTranslateAutomaticallyParameters->
		m_pEV->m_cstrvv.size();dwRun++)
	{
		CStringVector sv=pTranslateAutomaticallyParameters->m_pEV->m_cstrvv.
			at(dwRun);
		if(sv.size()>1)
		{
			//pView->m_bDrawVector=FALSE;
			bComboBoxes=1;
			break;
		}
	}
	if(!bComboBoxes)
	{
		DWORD dwCharsPerLine=0;
		DWORD dwPos=0;
		//wichtig!: zuerst alle Elemente (Zeichen) von m_pText löschen
		pTranslateAutomaticallyParameters->m_pEV->m_pText->Clear();
		pTranslateAutomaticallyParameters->m_pEV->m_CursorPos=0;
#ifdef _DEBUG
		printf("pTranslateAutomaticallyParameters->m_dwMaxCharsPerLine: %u\n",
			pTranslateAutomaticallyParameters->m_dwMaxCharsPerLine);
#endif
			for(DWORD dwRun=0;dwRun<pTranslateAutomaticallyParameters->m_pEV->
				m_cstrvv.size();dwRun++)
			{
				//dwPos+=pEV->m_cstrvv.at(dwRun).GetLength();
				CStringVector cstrv=pTranslateAutomaticallyParameters->m_pEV->
					m_cstrvv.at(dwRun);
				CString str=cstrv.at(0);
				if(str!=","  && str!="." && dwRun>0)
				{
					if(dwCharsPerLine+1<=pTranslateAutomaticallyParameters->
						m_dwMaxCharsPerLine)
					{
						//if(!bFirstNode)
						//	pEV->m_pText->Insert(dwPos,"\r\n",0);
						//dwPos+=2;
						//pEV->m_pText->Insert(dwPos,str.GetBuffer(str.GetLength()),0);
						//dwCharsPerLine=0;
						pTranslateAutomaticallyParameters->m_pEV->m_pText->
							Insert(dwPos,' ',0);
						dwPos++;
						++dwCharsPerLine;
					}
				}
				//wenn die aktuelle Anzahl der Zeichen pro Zeile plus der Länge der
				//aktuellen Zeichenkette größer ist als die maximale Anzahl der 
				//Zeichen pro Zeile,...
				if(dwCharsPerLine+str.GetLength()>pTranslateAutomaticallyParameters->
					m_dwMaxCharsPerLine)
				{
					//...dann eine neue Zeile einleiten mit den 2 Bytes für eine neue 
					//Zeile in einer DOS-/Windows-Textdatei: "13" "10" ("\r" "\n")
					pTranslateAutomaticallyParameters->m_pEV->m_pText->Insert(dwPos,
						"\r\n",0);
					//die 2 Bytes für eine neue Zeile in einer DOS-/Windows-Textdatei:
					//"13" "10" ("\r" "\n") gelten als 1 Position
					++dwPos;
					//pEV->m_pText->Insert(dwPos,str.GetBuffer(str.GetLength()),0);
					dwCharsPerLine=0;
				}
				//else
				pTranslateAutomaticallyParameters->m_pEV->m_pText->Insert(dwPos,str.
					GetBuffer(str.GetLength()),0);
				dwPos+=str.GetLength();
				dwCharsPerLine+=str.GetLength();
			}
			//pTranslateAutomaticallyParameters->m_pEV->m_pText->GetLength();
	}
	view2.m_bDrawVector=TRUE;
		//if(g_bContinue)
		//Nach der Übersetzung soll die Statusleiste wiederhergestellt werden.
		//Dazu müssen aber die Fortschrittsleisten zerstört werden, was mir
		//von dieser Funktion aus (TranslateAutomatically) aber nicht gelang
		//(Thread-Probleme: ich glaube, dass man von einem anderen Thread
		//aus kein MFC-Objekt (zum Beispiel CProgressCtrl) als der Thread,
		//der das MFC-Objekt erstellte, zerstören kann).
		//Deshalb wird CMainFrame::OnTranslateAutomaticallyFinishes()
		//ausgeführt, wo das Zerstören einwandfrei funktioniert, weil die
		//Fortschrittsleisten im selben Thread erstellt wurden
	//if(g_bContinue)
		::PostMessage(pTranslateAutomaticallyParameters->m_postHWnd,
			WM_TRANSLATE_AUTOMATICALLY_FINISHES,bComboBoxes,0);
		//AfxMessageBox("WaitForSingleObject",MB_OK,NULL);
		//Sleep(2000);
		//::WaitForSingleObject(g_eventEndThread,INFINITE);
		//AfxMessageBox("WaitForSingleObject",MB_OK,NULL);
		//Sleep(2000);
		//g_eventEndThread.ResetEvent();
		//while(!g_bExecutedOnTranslateAutomaticallyFinishes);
	// beim direkten Aufruf von m_ComboBoxList.Clear() und/oder 
	// m_pEV->CreateComboBoxes() entstand folgender Fehler (nur bei der Debug-
	// Konfiguration) :
	// Debug Assertion Failed!
	// File: afxwin2.inl
	// Line: 761
	// Das liegt wohl an folgender Funktion:
	// _AFXWIN_INLINE int CComboBox::GetItemHeight(int nIndex) const
	// Da ich vermute, dass das mit dem Multithreading zusammenhängt, habe ich
	// versucht, nach kurz vor der Beendung dieses Threads (TranslateAutomatically)
	// mittels ::PostMessage eine Nachricht zu schicken, die von einer Funktion der 
	// Klasse CMainFrame bearbeitet wird

	//pTranslateAutomaticallyParameters->m_pEV->m_ComboBoxList.m_pFirst=new ComboBoxListElement;;
	//view2.m_ComboBoxList.m_pFirst=new ComboBoxListElement;

		
	/*	CDC * pDC=view2.GetDC();
	CDC dc;
	dc.Attach(pDC->GetSafeHdc());
		{
				View.m_ComboBoxList.m_pFirst=new ComboBoxListElement;
				CComboBox box,box2;
				box.FromHandle(view2.m_ComboBoxList.m_pFirst->ComboBox.GetSafeHwnd());
				box2.Attach(View.m_ComboBoxList.m_pFirst->ComboBox.GetSafeHwnd());
				box.Create(WS_VISIBLE|WS_CHILD|CBS_DROPDOWNLIST,
				CRect(0,0,0,0),&view2,36000);
				box.MoveWindow(0,0,80,50);
				//CFont * pFont=box.GetFont();
				//CFont * pOldFont=dc.SelectObject(pFont);
				//DWORD dwWidth=0;
				for(DWORD dwRun2=0;dwRun2<3;dwRun2++)
				{
					box.AddString("hhhh");
					CSize size=dc.GetOutputTextExtent("hhhh");
				}
				box.SetCurSel(0);
				View.m_ComboBoxList.m_pFirst->m_pNext=NULL;
				View.m_ComboBoxList.m_pLast=View.m_ComboBoxList.m_pFirst;
				//View.m_ComboBoxList.m_pLast->m_pNext=NULL;
				box2.Detach();
				//dc.SelectObject(pOldFont);
		}*/

/*WORD dwHorizontalStart=0;
	CComboBox firstBox;
	for(dwRun=0;dwRun<View.m_cstrvv.size();dwRun++)
	{
		CStringVector cstrv=View.m_cstrvv.at(dwRun);
		TRACE("CStringVector cstrv=m_cstrvv.at(dwRun)\n");
		if(cstrv.size()>1) // Vektor hat mehr als 1 Element (CString)
		{
			if(View.m_ComboBoxList.m_pFirst)
			{
				View.m_ComboBoxList.m_pLast->m_pNext=new ComboBoxListElement;
				View.m_ComboBoxList.m_pLast=View.m_ComboBoxList.m_pLast->m_pNext;
				CComboBox box;
				box.FromHandle(View.m_ComboBoxList.m_pLast->ComboBox.GetSafeHwnd());
				box.Create(WS_VISIBLE|WS_CHILD|CBS_DROPDOWNLIST,
				CRect(0,0,0,0),&View,36000);
				CFont * pFont=box.GetFont();
				CFont * pOldFont=dc.SelectObject(pFont);
				DWORD dwWidth=0;
				for(DWORD dwRun2=0;dwRun2<cstrv.size();dwRun2++)
				{
					box.AddString(cstrv.at(dwRun2));
					CSize size=dc.GetOutputTextExtent(cstrv.at(dwRun2));
					if(size.cx>dwWidth)
						 dwWidth=size.cx;
				}
				box.MoveWindow(dwHorizontalStart,0,
					dwWidth+30,firstBox.GetItemHeight(-1)+
					(cstrv.size()+1)*firstBox.GetItemHeight(0));
				dwHorizontalStart+=dwWidth+30;
				box.SetCurSel(0);
				View.m_ComboBoxList.m_pLast->m_pNext=NULL;
				dc.SelectObject(pOldFont);
			}
			else
			{
				View.m_ComboBoxList.m_pFirst=new ComboBoxListElement;
				firstBox.FromHandle(View.m_ComboBoxList.m_pFirst->ComboBox.GetSafeHwnd());
				firstBox.Create(WS_VISIBLE|WS_CHILD|CBS_DROPDOWNLIST,
				CRect(0,0,0,0),&View,36000);
				CFont * pFont=firstBox.GetFont();
				CFont * pOldFont=dc.SelectObject(pFont);
				DWORD dwWidth=0;
				for(DWORD dwRun2=0;dwRun2<cstrv.size();dwRun2++)
				{
					firstBox.AddString(cstrv.at(dwRun2));
					CSize size=dc.GetOutputTextExtent(cstrv.at(dwRun2));
					if(size.cx>dwWidth)
						 dwWidth=size.cx;
				}
				//char buffer[30];
				//ultoa(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1),buffer,10);
				//ultoa(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(0),buffer,10);
				//ultoa(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+(cstrv.size()+1)*m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(0),buffer,10);
				firstBox.MoveWindow(dwHorizontalStart,0,dwWidth+30,firstBox.GetItemHeight(-1)+(cstrv.size()+1)*firstBox.GetItemHeight(0));
				dwHorizontalStart+=dwWidth+30;
				firstBox.SetCurSel(0);
				View.m_ComboBoxList.m_pFirst->m_pNext=NULL;
				View.m_ComboBoxList.m_pLast=View.m_ComboBoxList.m_pFirst;
				View.m_ComboBoxList.m_pLast->m_pNext=NULL;
				dc.SelectObject(pOldFont);
			}
		}
		else
		{
			CSize size=dc.GetOutputTextExtent(cstrv.at(0));
			dwHorizontalStart+=size.cx;
		}
			if(dwRun<View.m_cstrvv.size()-1)
			{
				cstrv=View.m_cstrvv.at(dwRun+1);
				CString str=cstrv.at(0);
				if(str!=","  && str!=".")
				{
					CSize size=dc.GetOutputTextExtent(" ");
					dwHorizontalStart+=size.cx;
				}
			}
	}

	CRect clientRect;
	View.GetClientRect(clientRect);
	dwHorizontalStart=0;
	ComboBoxListElement * pCurrent=NULL;
	if(View.m_ComboBoxList.m_pFirst)
	{
		pCurrent=View.m_ComboBoxList.m_pFirst;
	}
	DWORD dwLine=0;
	for(dwRun=0;dwRun<View.m_cstrvv.size();dwRun++)
	{
		CStringVector cstrv=View.m_cstrvv.at(dwRun);
		TRACE("CStringVector cstrv=m_cstrvv.at(dwRun)\n");
		if(cstrv.size()>1) // Vektor hat mehr als 1 Element (CString)
		{
			if(pCurrent)
			{
				CComboBox box;
				box.FromHandle(pCurrent->ComboBox.GetSafeHwnd());
				CFont * pFont=box.GetFont();
				CFont * pOldFont=pDC->SelectObject(pFont);
				DWORD dwWidth=0;
				for(DWORD dwRun2=0;dwRun2<cstrv.size();dwRun2++)
				{
					//m_ComboBoxList.m_pFirst->ComboBox.SetDroppedWidth(500);
					CSize size=pDC->GetOutputTextExtent(cstrv.at(dwRun2));
					if(size.cx>dwWidth) // Länge der längsten Zeichenkettem in dwWidth speichern
						 dwWidth=size.cx;
				}
				if(dwHorizontalStart+dwWidth+30>clientRect.right-15 && dwWidth+30<=clientRect.right-15)
				{ // wenn das Kombinationsfeld nicht mehr in die aktuelle Zeile passt und
					// die Kombinationsfeldbreite kleiner als die Kindfensterbreite ist
					//char buffer[30];
					//ultoa(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+
					//	(cstrv.size()+1)*m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(0),buffer,10);
					//AfxMessageBox("zweit"+CString(buffer),MB_OK,NULL);
					//ultoa((dwLine+1)*(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+5),buffer,10);
					//AfxMessageBox("erst"+CString(buffer),MB_OK,NULL);
					box.MoveWindow(0,++dwLine*
						(box.GetItemHeight(-1)+5),
						dwWidth+30,box.GetItemHeight(-1)+
						(cstrv.size()+1)*box.GetItemHeight(0));
					dwHorizontalStart=0;
				}
				else
				{
					///char buffer[30];
					//ultoa(dwLine*(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+5),buffer,10);
					//AfxMessageBox("erst"+CString(buffer),MB_OK,NULL);
					//ultoa(m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(-1)+
					//	(cstrv.size()+1)*m_ComboBoxList.m_pFirst->ComboBox.GetItemHeight(0),buffer,10);
					//AfxMessageBox("zweit"+CString(buffer),MB_OK,NULL);
					box.MoveWindow(dwHorizontalStart,dwLine*
						(box.GetItemHeight(-1)+5),
						dwWidth+30,
						box.GetItemHeight(-1)+
						(cstrv.size()+1)*box.GetItemHeight(0));
				}
				dwHorizontalStart+=dwWidth+30;
				pDC->SelectObject(pOldFont);
				pCurrent=pCurrent->m_pNext;
			}
		}
		else
		{
			CSize size=dc.GetOutputTextExtent(cstrv.at(0));
			if(dwHorizontalStart+size.cx>clientRect.right-15 && size.cx<=clientRect.right-15)
			{
				dwHorizontalStart=size.cx;
				dwLine++;
			}
			else
				dwHorizontalStart+=size.cx;
		}
		if(dwRun<View.m_cstrvv.size()-1)
		{
			cstrv=View.m_cstrvv.at(dwRun+1);
			CString str=cstrv.at(0);
			if(str!=","  && str!=".")
			{
				CSize size=pDC->GetOutputTextExtent(" ");
				if(dwHorizontalStart+size.cx>clientRect.right-15 && size.cx<=clientRect.right-15)
				{
					dwHorizontalStart=size.cx;
					dwLine++;
				}
				else
					dwHorizontalStart+=size.cx;
			}
		}
	}*/

	//dc.Detach();
	//view2.Invalidate();
	//view2.Detach();
	pView->Invalidate();
	view2.Detach();
	//if(
	//pView->m_bDrawVector=TRUE;
	//if(g_bContinue)
	//	View.Invalidate();
	//MoveComboBoxes();

	//ohne ExitThread beendet sich der Thread nicht immer und die Funktion
	//"GetExitCodeThread() liefert "STILL_ACTIVE" zurück
#ifdef _DEBUG
	printf("vor ExitThread\n");
#endif

	//ExitThread(0);
	g_bTranslateAutomaticallyBeforeEnd=1;
	g_bContinue=FALSE;
#ifdef _DEBUG
	printf("UINT TranslateAutomatically(LPVOID pParam) - return 0\n");
#endif
	return 0;
}

UINT WaitForEndOfTranslateAutomatically(LPVOID pParam)
{
	LPDWORD lpExitCode=new DWORD;
	WaitForEndOfTranslateAutomaticallyParameters * 
		pWaitForEndOfTranslateAutomaticallyParameters=
		(WaitForEndOfTranslateAutomaticallyParameters*)pParam;
#ifdef _DEBUG
	printf("UINT WaitForEndOfTranslateAutomatically(LPVOID pParam) ANFANG\n");
	fflush(stdout);
#endif
	//CMyObject* pObject = (CMyObject*)pParam;
  if (pWaitForEndOfTranslateAutomaticallyParameters == NULL)
    return 1;   // falls pTranslateParameters ungültig ist
	*lpExitCode=STILL_ACTIVE;
			while(*lpExitCode==STILL_ACTIVE)//wenn der Übersetzungsthread
			{//noch läuft, dann warten, bis der Übersetzungsthread beendet ist
				//MessageBeep((WORD)-1);
				if(!::GetExitCodeThread(
					pWaitForEndOfTranslateAutomaticallyParameters->m_hThread,lpExitCode))
					// wenn die Funktion fehlschlägt, liefert ::GetExitCodeThread 
					// FALSE zurück. Wenn man dann nicht aus der Schleife 
					// ausbricht, wird die while-Scheife endlos ausgeführt
				{
#ifdef _DEBUG
					printf("!::GetExitCodeThread(pFrame->m_pTranslationThread->m_hThread,lpExitCode)\n");
#endif
					break;
				}
#ifdef _DEBUG
				printf("*lpExitCode: %u\n",*lpExitCode);
#endif
			}
#ifdef _DEBUG
	printf("UINT WaitForEndOfTranslateAutomatically(LPVOID pParam) ENDE\n");
	fflush(stdout);
#endif
  return 0 ;
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame Konstruktion/Zerstörung

CMainFrame::CMainFrame(WordSearchType * p_wordsearch)
  : m_wndSearchFormBar(p_wordsearch)
{
	// ZU ERLEDIGEN: Hier Code zur Member-Initialisierung einfügen

#ifdef _DEBUG
	printf("CMainFrame::CMainFrame()");
#endif
	TRACE("CMainFrame::CMainFrame()");
	m_bAdoptGrammaticallyIncorrectSentences=TRUE;
	m_bAllowLowLetterAtTheBeginningOfSentence=TRUE;
	m_bCaseSensitive=TRUE;
	m_bEnableNextVocableButton=FALSE;
	m_bEnablePreviousVocableButton=FALSE;
	m_bEnsureUserWantsModification=TRUE;
	m_bEnsureUserWantsDeletion=TRUE;
	m_bMayStartThread=TRUE;
	m_bTimerIntalled=FALSE;
	m_pTranslationThread=NULL;
	m_bShowClauseProgress=TRUE;
	m_bShowGraphicClauseProgress=TRUE;
	m_bShowNumericClauseProgress=FALSE;//TRUE;
	m_bShowGraphicSentenceProgress=TRUE;
	m_bShowNumericSentenceProgress=FALSE;//TRUE;
	m_bShowGraphicTextProgress=TRUE;
	m_bShowNumericTextProgress=FALSE;//TRUE;
	m_bShowPrognosticedRemainingTime=TRUE;
	m_bShowSentenceProgress=TRUE;
	m_bShowTextProgress=TRUE;
	m_dwMaxCharsPerLine=80;
	m_nGraphicClauseProgressType=1;
	m_bShiftDown=FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_bCreated = FALSE;
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Symbolleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}

	int iTBCtlID = m_wndToolBar.CommandToIndex(ID_TRANSLATE_AUTOMATICALLY_IF_TEXTMODIFICATION_BUTTON);
	m_wndToolBar.SetButtonStyle(iTBCtlID,TBSTYLE_CHECK);
	iTBCtlID = m_wndToolBar.CommandToIndex(ID_VIEW_VOCABLEEDITOR);
	m_wndToolBar.SetButtonStyle(iTBCtlID,TBSTYLE_CHECK);
	iTBCtlID = m_wndToolBar.CommandToIndex(ID_COMBOBOX_BUTTON);
	m_wndToolBar.SetButtonStyle(iTBCtlID,TBSTYLE_CHECK);
	//CToolBarCtrl & toolBarCtrl=m_wndToolBar.GetToolBarCtrl();
	//toolBarCtrl.Indeterminate(ID_COMBOBOX_BUTTON,TRUE);

	if (!m_wndStatusBar.CreateEx(this,SBT_TOOLTIPS,WS_CHILD|WS_VISIBLE|
		CBRS_BOTTOM|0,AFX_IDW_STATUS_BAR) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Statusleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}

  if (!m_wndVocableEditorBar.Create(_T("Vokabeleditor"),this, CSize(80, 80),
    TRUE, 123))//AFX_IDW_CONTROLBAR_FIRST + 33))
  {
    TRACE0("Failed to create mybar\n");
    return -1;      // fail to create
	}
  if (!m_wndSearchFormBar.Create(_T("Suchformular"),this, CSize(80, 80),
        TRUE, 124))//AFX_IDW_CONTROLBAR_FIRST + 33))
  {
    TRACE0("Failed to create searchformbar\n");
    return -1;      // fail to create
	}
  if (!m_wndUnknownWordsBar.Create(_T("Liste der unbekannten Wörter"),this,
		CSize(80, 80),TRUE,125))//AFX_IDW_CONTROLBAR_FIRST + 33))
  {
	  TRACE0("Failed to create mybar\n");
		return -1;      // fail to create
	}

	m_wndSearchFormBar.Attach(m_wndVocableEditorBar);
	//m_wndSearchFormBar.m_pMainFrame=this;

	m_wndVocableEditorBar.SetBarStyle(m_wndVocableEditorBar.GetBarStyle()|
       CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC|CBRS_FLOAT_MULTI );
	m_wndSearchFormBar.SetBarStyle(m_wndSearchFormBar.GetBarStyle()|
       CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndUnknownWordsBar.SetBarStyle(m_wndUnknownWordsBar.GetBarStyle()|
       CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
  m_wndUnknownWordsBar.EnableDocking(CBRS_ALIGN_ANY);

	ShowControlBar(&m_wndUnknownWordsBar,FALSE,FALSE);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable

	// ZU ERLEDIGEN: Löschen Sie diese drei Zeilen, wenn Sie nicht wollen, dass die Symbolleiste
	//  andockbar ist.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  //m_wndVocableEditorBar.EnableDocking(CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM);
  m_wndSearchFormBar.EnableDocking(CBRS_ALIGN_ANY);
	UINT ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Show Search Form"),
		(UINT)1);
	if(ui!=1)
		ShowControlBar(&m_wndSearchFormBar,FALSE,FALSE);

	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_wndVocableEditorBar.LoadState(_T("BarState1"));
	m_wndVocableEditorBar.EnableDocking(CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM);
	DockControlBar(&m_wndVocableEditorBar);//,AFX_IDW_DOCKBAR_TOP);
	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Show Vocable Editor"),
		(UINT)1);
	TRACE("ui: %u\n",ui);
	if(ui==1)
	{
		CToolBarCtrl & toolBarCtrl=m_wndToolBar.GetToolBarCtrl();
		toolBarCtrl.CheckButton(ID_VIEW_VOCABLEEDITOR,TRUE );
	}
	else
	{
		ShowControlBar(&m_wndVocableEditorBar,FALSE,FALSE);
		//m_wndToolBar.SetCheck(.SetButtonStyle(iTBCtlID,TBSTYLE_CHECK);
		//CheckMenuItem(NULL,ID_VIEW_VOCABLEEDITOR,MF_BYCOMMAND|MF_UNCHECKED);//MF_GRAYED);
	}

  DockControlBar(&m_wndSearchFormBar);//,AFX_IDW_DOCKBAR_TOP);

	//m_wndSearchFormBar.m_pMainFrame=this;
	
	LoadBarState(_T("BarState1"));

	//m_wndVocableEditorBar.m_szHorz.cy=100;
	//RecalcLayout();
	//m_first=new VocNode();
	//LoadWords(m_first);

	/*if(wordList.m_pWordNodeFirst)
	{
		WordNode * pWordNodecurrent=wordList.m_pWordNodeFirst;
		while(pWordNodecurrent)
		{
			if(typeid(*pWordNodecurrent->m_pWord)==typeid(EnglishPreposition))
			{
				EnglishPreposition * ep=dynamic_cast<EnglishPreposition*>(
					pWordNodecurrent->m_pWord);
				m_wndVocableEditorBar.m_englishComboBox.AddString(ep->m_strWord);
			}
			if(typeid(*pWordNodecurrent->m_pWord)==typeid(GermanPreposition))
			{
				GermanPreposition * gp=dynamic_cast<GermanPreposition*>(
					pWordNodecurrent->m_pWord);
				m_wndVocableEditorBar.m_germanComboBox.AddString(gp->m_strWord);
			}
			pWordNodecurrent=pWordNodecurrent->m_pWordNodeNext;
		}
	}*/

	CMenu * menu=GetMenu();
	if(menu)
		menu->EnableMenuItem(ID_EDIT_PASTE,MF_DISABLED);
	else
		MessageBox("!menu",NULL,MB_OK);
	RecalcLayout(FALSE);

	//auslesen der in der Systemregistrierung gespeicherten
	//Optionsdaten. die Zahl beim letzen Parameter der GetProfileInt()-
	//Funktion ist die Zahl, die zurückgeliefert werden soll, falls
	//noch keine Registrierungseinträge existieren, zum Beispiel beim
	//erstmaligen Programmstart. Diese Zahl stellt sozusagen eine Vor-
	//Konfiguration dar
	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Case Sensitive Parsing"),(UINT)0);
	if(ui==1)
		m_bCaseSensitive=TRUE;
	else
		m_bCaseSensitive=FALSE;

	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Ensure User Wants Deletion"),(UINT)1);
	if(ui==1)
		m_bEnsureUserWantsDeletion=TRUE;
	else
		m_bEnsureUserWantsDeletion=FALSE;

	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Ensure User Wants Modification"),(UINT)1);
	if(ui==1)
		m_bEnsureUserWantsModification=TRUE;
	else
		m_bEnsureUserWantsModification=FALSE;

	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Show Graphic Clause Progress"),(UINT)1);
	if(ui==1)
		m_bShowGraphicClauseProgress=TRUE;
	else
		m_bShowGraphicClauseProgress=FALSE;

	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Show Graphic Sentence Progress"),(UINT)1);
	if(ui==1)
		m_bShowGraphicSentenceProgress=TRUE;
	else
		m_bShowGraphicSentenceProgress=FALSE;

	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Show Graphic Text Progress"),(UINT)1);
	if(ui==1)
		m_bShowGraphicTextProgress=TRUE;
	else
		m_bShowGraphicTextProgress=FALSE;

	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Show Numeric Clause Progress"),(UINT)1);
	if(ui==1)
		m_bShowNumericClauseProgress=TRUE;
	else
		m_bShowNumericClauseProgress=FALSE;

	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Show Numeric Sentence Progress"),(UINT)1);
	if(ui==1)
		m_bShowNumericSentenceProgress=TRUE;
	else
		m_bShowNumericSentenceProgress=FALSE;

	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Show Numeric Text Progress"),(UINT)1);
	if(ui==1)
		m_bShowNumericTextProgress=TRUE;
	else
		m_bShowNumericTextProgress=FALSE;

	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Show Prognosticed Remaining Time"),(UINT)1);
	if(ui==1)
		m_bShowPrognosticedRemainingTime=TRUE;
	else
		m_bShowPrognosticedRemainingTime=FALSE;
	// soll die Ausgabe gepuffert werden? (vermeidet lästiges Flackern)
	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Buffer Output"),(UINT)1);
	if(ui==1)
		m_bBufferOutput=TRUE;
	else
		m_bBufferOutput=FALSE;
	//Anzahl der Zeichen, nach der der übersetzte Text umgebrochen werden soll.
	//Der Zeilenumbruch erfolgt nur nach einer Übersetzung.
	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Maximum Number Of Characters Per Line"),(UINT)80);
	m_dwMaxCharsPerLine=ui;
	//Anzeige-Typ des grafischen Fortschritts: grob (0) oder fein (1)
	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Graphic Clause Progress Type"),(UINT)1);
	if(ui==0 || ui==1)
		m_nGraphicClauseProgressType=ui;
	else
		m_nGraphicClauseProgressType=1;
	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Graphic Sentence Progress Type"),(UINT)1);
	if(ui==0 || ui==1)
		m_nGraphicSentenceProgressType=ui;
	else
		m_nGraphicSentenceProgressType=1;
	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Graphic Text Progress Type"),(UINT)1);
	if(ui==0 || ui==1)
		m_nGraphicTextProgressType=ui;
	else
		m_nGraphicTextProgressType=1;

	CToolBarCtrl & toolBarCtrl=m_wndToolBar.GetToolBarCtrl();
	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Translate Automatically On Text Modification"),(UINT)1);
	if(ui==1) // Schaltfläche "bei Textänderung automatisch neu übersetzen"
		toolBarCtrl.CheckButton(ID_TRANSLATE_AUTOMATICALLY_IF_TEXTMODIFICATION_BUTTON,TRUE);
	else
		toolBarCtrl.CheckButton(ID_TRANSLATE_AUTOMATICALLY_IF_TEXTMODIFICATION_BUTTON,FALSE);
	ui=AfxGetApp()->GetProfileInt(_T("Settings"),_T("Thread Priority"),(UINT)2);
	if(ui>=0 && ui<6)
		m_nThreadPriority=ui;
	else
		m_nThreadPriority=2;
#ifdef _DEBUG
	printf("CMainFrame::OnCreate()- ENDE\n");
	fflush(stdout);
#endif
	TRACE("CMainFrame::OnCreate() - ENDE\n");
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Diagnose

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Nachrichten-Handler

#define TEST_LETTERTREE

void CMainFrame::OnTranslateButton() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	//CMDIChildWnd *pChild = MDIGetActive();
	CToolBarCtrl & toolBarCtrl=m_wndToolBar.GetToolBarCtrl();
#ifdef _DEBUG
	printf("void CMainFrame::OnTranslateButton() ANFANG\n");
#endif
  LOG2N("23.05.2008 10.39.05","trans btn");
	if(toolBarCtrl.IsButtonEnabled(ID_TRANSLATE_BUTTON))
	{
	CChildFrame * pChild=(CChildFrame*)MDIGetActive();
	if(pChild)
	{
		//CEditView * pEV=(CEditView*)pChild->m_SplitterWnd.GetPane(1,0);
		//CEditView * pEV=(CEditView*)pChild->m_SplitterWnd.GetPane(1,0);
		//CString strText;
		//pEV->GetSelectedText(strText);
		EnglishView * pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(0,0);
		CString strText;
		CString strWord;
		m_wndUnknownWordsBar.m_strText="";
		if(pEV->m_pText->m_first)
		{
			BYTE bUnknownWord=FALSE;
			TextNode * current=pEV->m_pText->m_first;
			while(1)
			{
				if(current->ch==' ' || current->ch==13 || current->ch=='.' ||
					current->ch==',' || current->ch=='?' || current->ch=='!')
				{
					strWord.MakeLower();
					if(!IsEnglishWord(strWord))
					{
#ifdef _DEBUG
						printf("!IsEnglishWord(strWord)\n");
#endif
						//wenn schon mindestens 1 Token in der Liste der unbekannten Wörter 
						//steht
						if(bUnknownWord)
							//dann eine neue Zeile einleiten
							m_wndUnknownWordsBar.m_strText+=_T("\r\n");//+strWord;
						//sonst lediglich das Token anhängen
						//else
							m_wndUnknownWordsBar.m_strText+=strWord;
						bUnknownWord=TRUE;
					}
					strWord="";
				}
				else
					strWord+=current->ch;
				if(current->m_next)
					current=current->m_next;
				else
					break;
			}
			if(bUnknownWord)
			{
#ifdef _DEBUG
				printf("bUnknownWord\n");
				fflush(stdout);
#endif
				m_wndUnknownWordsBar.UpdateData(FALSE);
				DockControlBar(&m_wndUnknownWordsBar);//,AFX_IDW_DOCKBAR_TOP);
				ShowControlBar(&m_wndUnknownWordsBar,TRUE,FALSE);
			}
			current=pEV->m_pText->m_first;
			while(current)
			{
				if(current->ch==13) // Neuzeile durch
					strText+=" "; // Leerzeichen ersetzen
				else
					strText+=current->ch;
				current=current->m_next;
			}
#ifdef _DEBUG
			printf("nach: while(current)\n");
#endif
			CString str,german_text;
			BOOL	bValidSentence=FALSE; // gültiger Satz ?
			DWORD i=0, start=0;
	//pEV->m_VV=TranlateText
			EnglishView * pEV2=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
			pEV2->m_bDrawVector=TRUE;
#ifdef _DEBUG
			printf("vor TranslateText\n");
#endif
			std::vector<Range> vecRange;
			g_bContinue=TRUE;
			CWinThread * translationThread;
			StatusDialog statusDlg;
			g_dwStatus=0;
			g_dwSentenceStatus=0;
			TranslateParameters * pTranslateParameters=new TranslateParameters(
				pEV->m_pText,vecRange,&pEV2->m_cstrvv,&statusDlg,//&statusDlg.m_hWnd
        statusDlg.GetSafeHwnd());
			pTranslateParameters->m_pEV=pEV;
			pTranslateParameters->m_pEnglishViewGerman=pEV2;
			pTranslateParameters->m_dwMaxCharsPerLine=m_dwMaxCharsPerLine;
      LOG2N("23.05.2008 10.39.40","begin trans");
#ifdef TEST_LETTERTREE
	    //DWORD dwTextLength=pEV->m_pText->GetLength();
     // Text & text = * pEV->m_pText ;
	    //TRACE("dwSize: %u\n",dwSize);
	    ////g_dwTotal=text.GetLength();
     // vector<CStringVector> vecvecstr ;
	    //for(int i=0;i< dwTextLength; i++)
	    //{
     //   LOG("23.05.2008 11.19.33 "<<i<<"\n");
		   // //wenn Punkt (mögliches Satzendezeichen für einen Aussagesatz)
		   // if(current->ch=='.')
		   // {
			  //  TRACE("if(current->ch=='.')\n");
     //     LOG("23.05.2008 11.19.43\n");
			  //  CString strSentence;
			  //  if(text.GetCString(strSentence,dwStart,i))
			  //  {
				 //   TRACE("strSentence: %s\n",strSentence);
				 //   strSentence.Replace(13,' '); // Carriage Returns durch Leerzeichen ersetzen
				 //   strSentence.Replace(10,' '); // Newlines durch Leerzeichen ersetzen
				 //   CString strTransformed=Transform(strSentence);
				 //   CStringVector svSentence=GetCStringVector(strTransformed);
				 //   PositionCStringVector psvOriginalSentence=GetPositionCStringVector(
					//    strSentence,dwStart);
				 //   PositionCStringVector psvSentence=psvOriginalSentence;
     //       Sentence sentence = ::Parse(psvSentence) ;
     //       for(sentence.
     //       EngNounsLetterTree engnounlt(
     //       ::Append(vecvecstr,TranslateENR(ee.m_pEnr,0,//FALSE,
     //         false,(bool)bTranslateAsObject,personIndex,
	    //	      bFirstTokenInSentence,bArticle,nStartID,engnounlt));
     //     }
     //   }
     // }
      EnglishNounRecord enr ;
      //enr
      EngNounsLetterTree engnounlt(* g_lettertree.search("car" ) );
      BYTE by1 = 1 , by0 = 0 ;
      int n0 = 0 ;
      ::TranslateENR(
        &enr,
        0,//FALSE,
        false,
        false,
        by1,
	      by0,
        by0,
        n0,
        engnounlt) ;
      CString strSentence ; //= "cars like cars." ;
      if(pEV->m_pText->GetCString(strSentence,0,pEV->m_pText->GetLength()-1) )
      {
        //pEV->m_pText->GetCString(strSentence,0,pEV->m_pText->GetLength()-1) ;
    	  PositionCStringVector psvSentence=GetPositionCStringVector(
				  strSentence,0);
        PositionstdstringVector posstdstrvecSentence = 
          //Positionstdstring::Tokenize(strSentence);
          ::Tokenize(strSentence);
#ifdef USE_BISON_PARSER
        b4_namespace::BisonEnglishParser bison_parser ;
        bison_parser.parse() ;
#else
        SequentialParse sp ;
        //Sentence sentence = sp.Parse(psvSentence) ;
        Sentence sentence = sp.Parse(posstdstrvecSentence) ;
        std::vector<std::vector<std::string>> vecvecstr ;
        NSLetterTree::Translate(sentence,vecvecstr) ;
        by0 = by0 ; //senseless instruction.Just for being able to set a breakpoint.
#endif 
      }
#endif //#ifdef TEST_LETTERTREE
			translationThread=AfxBeginThread(Translate,pTranslateParameters);//,THREAD_PRIORITY_NORMAL,0,0,NULL);
			//MessageBox msgBox("hh","jjj",MB_OKCANCEL);
			if(statusDlg.DoModal()==IDCANCEL)
			{
				g_bContinue=FALSE;
				//bUserCanceled=TRUE;
			}
			//pEV2->m_cstrvv=TranslateText(*pEV->m_pText,vecRange);
			for(i=0;i<pEV2->m_cstrvv.size();i++)
			{
#ifdef _DEBUG
				printf("pEV2->m_cstrvv.at(i).at(0): %s\n",pEV2->m_cstrvv.at(i).at(0));
#endif
			}
#ifdef _DEBUG
				printf("vecRange.size(): %u\n",vecRange.size());
#endif
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
			{
#ifdef _DEBUG
				printf("dwRun: %u\n",dwRun);
#endif
				Range range=vecRange.at(dwRun);
				for(DWORD dwRun2=range.m_dwStart;dwRun2<range.m_dwEnd;dwRun2++)
				{
#ifdef _DEBUG
					printf("dwRun2: %u range.m_dwStart: %u range.m_dwEnd: %u\n",
						dwRun2,range.m_dwStart,range.m_dwEnd);
#endif
					pEV->m_pText->SetColor(dwRun2,2);
				}
			}
			//for(dwRun=0; dwRun<7;dwRun++)
			//	pEV->m_pText->SetColor(dwRun,1);
			pEV->Invalidate();
#ifdef _DEBUG
			printf("vor TRACE\n");
#endif
			TRACE("pEV2->m_cstrvv.size(): %u\n",pEV->m_cstrvv.size());
	//pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
	//pEV2->m_pText->Clear();
#ifdef _DEBUG
			printf("vor for\n");
#endif
			/*for(i=0;i<german_text.GetLength();i++)
			{
				pEV2->m_pText->Insert(i,german_text[i]);
			}*/
#ifdef _DEBUG
			printf("nach for\n");
#endif
			TRACE("nach for\n");
			//pEV->Invalidate();
			pEV2->m_ComboBoxList.Clear(); // zuerst alle ComboBoxen 
			// (Kombinationfelder) entfernen
			pEV2->CreateComboBoxes(); // Kombinationfelder erzeugen
			pEV2->Invalidate();
	//m_wndPositionBar.SetDlgItemText(IDC_POSITION_EDIT,buffer);german_text
	//const char * s=pEV->LockBuffer();
	//s=german_text;
			//pEV2->Invalidate();
			TRACE("german_text: %s\n",german_text);
      LOG2N("23.05.2008 10.39.56:"<<german_text,
          "german:"<<german_text);
		}
	//CString * pStr=pEV->LockBuffer();
	//pStr=german_text;
	}
	}
#ifdef _DEBUG
	printf("void CMainFrame::OnTranslateButton() ENDE\n");
#endif
  LOG2N("23.05.2008 10.40.10","trans btn end");
}

void CMainFrame::OnExtrasCountWords() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen

	if(wordList.m_pWordNodeFirst!=NULL)
	{
		unsigned long count=0;
		WordNode * pWordNodeCurrent=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(pWordNodeCurrent==NULL)//englisch
				break;
			if(pWordNodeCurrent->m_pWordNodeNext==NULL)//deutsch
				break;
			pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
			count++;
			if(pWordNodeCurrent->m_pWordNodeNext==NULL)
				break;
			pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
		}
		char buffer[35];
		_ltoa(count,buffer,10);
		MessageBox("Anzahl der Vokabeln: "+CString(buffer),"Information",MB_OK);
	}
	else
		MessageBox("Anzahl der Vokabeln: 0","Information",MB_OK);

}

void CMainFrame::OnVocableEditorButton() // Vokabeleditor ein- oder ausblenden
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	ShowControlBar(&m_wndVocableEditorBar,!m_wndVocableEditorBar.IsVisible(),FALSE);

/*if(m_wndVocableEditorBar.IsWindowVisible())
		m_wndVocableEditorBar.ShowWindow(SW_HIDE);
	else
		m_wndVocableEditorBar.ShowWindow(SW_SHOW);
	RecalcLayout(); // Nach dem Aufruf von ShowWindow ist ein Aufruf von Recalaclayout nötig,
	// damit die Kontrollleiste auch wirklich verschwindet
	//Invalidate();*/
}

void CMainFrame::OnViewSearchForm() // Vokabeleditor ein- oder ausblenden
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	//CMainFrame * pFrame=(CMainFrame *)AfxGetMainWnd();
	//pFrame->
	CWnd * pWnd=GetDlgItem(ID_ADD_VOCABLE_BUTTON);
	if(pWnd)
		pWnd->EnableWindow(FALSE);

	if(m_wndSearchFormBar.IsWindowVisible())
		m_wndSearchFormBar.ShowWindow(SW_HIDE);
	else
		m_wndSearchFormBar.ShowWindow(SW_SHOW);
	RecalcLayout(); // Nach dem Aufruf von ShowWindow ist ein Aufruf von Recalaclayout nötig,
	// damit die Kontrollleiste auch wirklich verschwindet
	//Invalidate();
}

void CMainFrame::OnDestroy() 
{
	CMDIFrameWnd::OnDestroy();
	
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen
	//SaveBarState(_T("BarState1"));
}

BOOL CMainFrame::DestroyWindow() 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	LPDWORD lpExitCode=new DWORD;
	*lpExitCode=STILL_ACTIVE;
	g_bContinue=FALSE;
	while(*lpExitCode==STILL_ACTIVE)
	{
		if(m_pTranslationThread)
		{
			if(!::GetExitCodeThread(m_pTranslationThread->m_hThread,lpExitCode))
			// wenn die Funktion fehlschlägt, liefert ::GetExitCodeThread 
			// FALSE zurück. Wenn man dann nicht aus der Schleife 
			// ausbricht, wird die while-Scheife endlos ausgeführt
				break;
		}
		else
			break;
	}
	m_wndVocableEditorBar.SaveState(_T("BarState1"));

	if(m_bCaseSensitive)
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Case Sensitive Parsing"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Case Sensitive Parsing"),0);

	if(m_bEnsureUserWantsDeletion)
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Ensure User Wants Deletion"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Ensure User Wants Deletion"),0);
	if(m_bEnsureUserWantsModification)
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Ensure User Wants Modification"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Ensure User Wants Modification"),0);
	//grafische Anzeige des Fortschritts bei der Übersetzung/
	//Grammatikprüfung des aktuellen Haupt- oder Nebensatz
	if(m_bShowGraphicClauseProgress)
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Graphic Clause Progress"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Graphic Clause Progress"),0);
	//grafische Anzeige des Fortschritts bei der Übersetzung/
	//Grammatikprüfung des Satzes
	if(m_bShowGraphicSentenceProgress)
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Graphic Sentence Progress"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Graphic Sentence Progress"),0);
	//grafische Anzeige des Fortschritts bei der Übersetzung/
	//Grammatikprüfung des ganzen Textes
	if(m_bShowGraphicTextProgress)
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Graphic Text Progress"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Graphic Text Progress"),0);
	//numerische Anzeige (als Zahl) des Fortschritts bei der Übersetzung/
	//Grammatikprüfung des aktuellen Haupt- oder Nebensatz
	if(m_bShowNumericClauseProgress)
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Numeric Clause Progress"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Numeric Clause Progress"),0);
	//numerische Anzeige (als Zahl) des Fortschritts bei der Übersetzung/
	//Grammatikprüfung des Satzes
	if(m_bShowNumericSentenceProgress)
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Numeric Sentence Progress"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Numeric Sentence Progress"),0);
	//numerische Anzeige (als Zahl) des Fortschritts bei der Übersetzung/
	//Grammatikprüfung des ganzen Textes
	if(m_bShowNumericTextProgress)
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Numeric Text Progress"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Numeric Text Progress"),0);
	//numerische Anzeige (als Zahl) der prognostizierten verbeleibenden 
	//Zeit für die Übersetzung/Grammatikprüfung des ganzen Textes
	if(m_bShowPrognosticedRemainingTime)
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Prognosticed Remaining Time"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Prognosticed Remaining Time"),0);

	if(m_bBufferOutput)
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Buffer Output"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Buffer Output"),0);

	AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Maximum Number Of Characters Per Line"),
		(UINT)m_dwMaxCharsPerLine);

	if(m_wndVocableEditorBar.IsVisible())
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Vocable Editor"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Vocable Editor"),0);

	//Suchformular beim nächsten Mal anzeigen/verbergen
	if(m_wndSearchFormBar.IsVisible())
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Search Form"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("Show Search Form"),0);

	//Anzeige-Typ des grafischen Fortschritts: grob (0) oder fein (1)
	AfxGetApp()->WriteProfileInt(_T("Settings"),
		_T("Graphic Clause Progress Type"),(UINT)m_nGraphicClauseProgressType);
	AfxGetApp()->WriteProfileInt(_T("Settings"),
		_T("Graphic Sentence Progress Type"),(UINT)m_nGraphicSentenceProgressType);
	AfxGetApp()->WriteProfileInt(_T("Settings"),
		_T("Graphic Text Progress Type"),(UINT)m_nGraphicTextProgressType);

	CToolBarCtrl & toolBarCtrl=m_wndToolBar.GetToolBarCtrl();
	if(toolBarCtrl.IsButtonChecked(
		ID_TRANSLATE_AUTOMATICALLY_IF_TEXTMODIFICATION_BUTTON))
		AfxGetApp()->WriteProfileInt(_T("Settings"),
			_T("Translate Automatically On Text Modification"),1);
	else
		AfxGetApp()->WriteProfileInt(_T("Settings"),
			_T("Translate Automatically On Text Modification"),0);
	
	////Arbeitsspeicher, der von Vokabeln belegt war, an das Betriebssystem 
	////freigeben
	//if(wordList.m_pWordNodeFirst!=NULL)
	//{
	//	WordNode * pWordNodeCurrent=wordList.m_pWordNodeFirst, * pWordNodePrevious=NULL;
	//	while(1)
	//	{
	//		if(pWordNodeCurrent->m_pWordNodeNext)
	//		{
	//			pWordNodePrevious=pWordNodeCurrent;
	//			pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	//			delete pWordNodePrevious;
	//		}
	//		else
	//			break;
	//	}
	//	delete pWordNodeCurrent;
	//}
  //~WordList() releases the mem now.
	
	return CMDIFrameWnd::DestroyWindow();
}

void CMainFrame::OnEditPaste()
{
	//CMDIFrameWnd::OnDestroy();
	CChildFrame * pChild=(CChildFrame*)MDIGetActive();
	CWnd* pWnd = GetFocus();
	//wenn die zurückgelieferten Zeiger auf ein Kindfenster und auf ein
	//CWnd (Steuerelement/Fenster) gültig (also ungleich NULL sind)
	if(pChild && pWnd)
	{
		EnglishView * pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(0,0);
		//wenn sich die Windows-Zwischenablage öffnen lässt und der Fokus
		//während der Benutzer-Aktion "aus der Windows-Zwischenablage einfügen"
		//auf dem englischen Teil eines Kindfensters lag
		if(OpenClipboard() && pWnd==pEV)
		{
			//wenn das Format der Zwischenablage "Text" ist
			if(IsClipboardFormatAvailable(CF_TEXT)
				|| IsClipboardFormatAvailable(CF_OEMTEXT))
			{
				TRACE("CF_TEXT oder CF_OEMTEXT\n");
				/* get text from the clipboard */
				HANDLE hClipData;
				LPSTR lpClipData;
				HANDLE hText = NULL;
				LPSTR lpszText;
				if(!(hClipData = GetClipboardData(CF_TEXT)))
				{
					CloseClipboard();
													//break;
				}
				if(hText != NULL)
				{
					GlobalFree(hText);
				}
				if(!(hText = GlobalAlloc(GMEM_MOVEABLE,GlobalSize(hClipData))))
				{
                            //OutOfMemory();
					CloseClipboard();
                            //break;
				}
				if(!(lpClipData = (char *)GlobalLock(hClipData)))
				{
                            //OutOfMemory();
					CloseClipboard();
                            //break;
				}
				if (!(lpszText = (char *)GlobalLock(hText)))
				{
                            //OutOfMemory();
					CloseClipboard();
                            //break;
				}
//Um die Warnungen über veraltete C-Bibliotheken zu deaktivieren
#pragma warning disable 4996
				strcpy(lpszText, lpClipData);
				GlobalUnlock(hClipData);
				CloseClipboard();
				GlobalUnlock(hText);
				TRACE("lpClipData: %s\n",lpClipData);
				BOOL bPreviousCharacterWasCarriageReturn=FALSE;
				DWORD dwStringLength=strlen(lpClipData);
				DWORD dwCount=0;
				if(m_pTranslationThread)
				{
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
					WaitForSingleObject(m_pTranslationThread->m_hThread,INFINITE);
				}
				if(pEV->m_bMark) // wenn Text markiert wurde
				{
					DWORD dwMarkingStart=pEV->m_dwMarkingStart<pEV->m_dwMarkingEnd?pEV->m_dwMarkingStart:pEV->m_dwMarkingEnd;
					DWORD dwMarkingEnd=pEV->m_dwMarkingStart>pEV->m_dwMarkingEnd?pEV->m_dwMarkingStart:pEV->m_dwMarkingEnd;
					pEV->m_pText->Delete(dwMarkingStart,dwMarkingEnd-dwMarkingStart);
					pEV->m_bMark=FALSE;
				}
				pEV->m_pText->Insert(pEV->m_CursorPos,lpClipData,0);
				for(DWORD dwRun=0;dwRun<dwStringLength;dwRun++)
				{
					if(lpClipData[dwRun]==10)
					{
						if(!bPreviousCharacterWasCarriageReturn)
							dwCount++;
						else
							bPreviousCharacterWasCarriageReturn=FALSE;
					}
					else
					{
						if(lpClipData[dwRun]==13)
							bPreviousCharacterWasCarriageReturn=TRUE;
						else
							bPreviousCharacterWasCarriageReturn=FALSE;
						dwCount++;
					}
				}
				pEV->m_CursorPos+=dwCount;
				pEV->Invalidate();
				//die Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
				//wird nicht direkt nach der Funktion TranslateAutomatically()
				//ausgeführt, wenn die Funktion EnglishView::OnChar() vor 
				//der Beendigung der Funktion TranslateAutomatically() ausgeführt
				//wird.
				//Wenn die Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
				//nach der Übersetzung noch nicht ausgeführt wurde, wird folgender
				//Code ausgeführt:
				if(!g_bExecutedOnTranslateAutomaticallyFinishes && m_bCreated)
				{
					KillTimer(1);
					if(m_bShowGraphicClauseProgress)
						m_Progress.DestroyWindow();
					if(m_bShowNumericClauseProgress)
						m_cStaticClauseProgress.DestroyWindow();
					if(m_bShowGraphicSentenceProgress)
						m_SentenceProgress.DestroyWindow();
					if(m_bShowNumericSentenceProgress)
						m_cStaticSentenceProgress.DestroyWindow();
					if(m_bShowGraphicTextProgress)
						m_TextProgress.DestroyWindow();
					if(m_bShowNumericTextProgress)
						m_cStaticTextProgress.DestroyWindow();
					if(m_bShowPrognosticedRemainingTime)
						m_cStaticRemainingTime.DestroyWindow();
					m_bCreated=FALSE;
					m_bMayStartThread=TRUE;
					//damit das Zerstören der Fortschrittsbalken (CProgressCtrl)
					//nicht in der Funktion 
					//CMainFrame::OnTranslateAutomaticallyFinishes() ausgeführt 
					//wird (sie wurden ja bereits über diesem Kommentar mit
					//DestroyWindow() zerstört und sollen nicht doppelt zerstört 
					//werden), wird g_bExecutedOnTranslateAutomaticallyFinishes 
					//auf TRUE gesetzt. Wwenn die Funktion 
					//CMainFrame::OnTranslateAutomaticallyFinishes() ausgeführt 
					//wird, wird g_bExecutedOnTranslateAutomaticallyFinishes 
					//ausgewertet.
					g_bExecutedOnTranslateAutomaticallyFinishes=TRUE;
				}
				CToolBarCtrl & toolBarCtrl=m_wndToolBar.GetToolBarCtrl();
				if(toolBarCtrl.IsButtonChecked(ID_TRANSLATE_AUTOMATICALLY_IF_TEXTMODIFICATION_BUTTON))
				{
					//Text * pText=new Text(*pEV->m_pText); // Kopie anlegen
					std::vector<Range> vecRange;
					EnglishView * pEV2=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
					if(pEV2)
					{
						if(m_bCreated == FALSE)
						{
							BYTE bIDCount=0;//3;
							std::vector<UINT> vecUINT;
							if(m_bShowClauseProgress)
							{
								if(m_bShowNumericClauseProgress)
								{
									bIDCount++;
									vecUINT.push_back(ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE);
								}
								if(m_bShowGraphicClauseProgress)
								{
									bIDCount++;
									vecUINT.push_back(ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
								}
								//bIDCount++;
								//vecUINT.push_back(ID_INDICATOR_CLAUSE_PROGRESS_PANE);
							}
							if(m_bShowSentenceProgress)
							{
								if(m_bShowNumericSentenceProgress)
								{
									bIDCount++;
									vecUINT.push_back(ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE);
								}
								if(m_bShowGraphicSentenceProgress)
								{
									bIDCount++;
									vecUINT.push_back(ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
								}
								//bIDCount++;
								//vecUINT.push_back(ID_INDICATOR_SENTENCE_PROGRESS_PANE);
							}
							if(m_bShowTextProgress)
							{
								if(m_bShowNumericTextProgress)
								{
									bIDCount++;
									vecUINT.push_back(ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE);
								}
								if(m_bShowGraphicTextProgress)
								{
									bIDCount++;
									vecUINT.push_back(ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
								}
								//bIDCount++;
								//vecUINT.push_back(ID_INDICATOR_TEXT_PROGRESS_PANE);
							}
							if(m_bShowPrognosticedRemainingTime)
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
								m_wndStatusBar.SetIndicators(lpIDArray,sizeof(lpIDArray)/sizeof(UINT));
							}
							CStatusBarCtrl & statusbarCtrl=m_wndStatusBar.GetStatusBarCtrl();
							//int nHorz=0,nVert=0,nSpacing=0;
							//statusbarCtrl.GetBorders(nHorz,nVert,nSpacing);
							CRect rect,rect2;
							//pFrame->m_wndStatusBar.GetWindowRect(rect);
							m_wndStatusBar.GetClientRect(rect2);
#ifdef _DEBUG
							printf("lpRect->bottom: %d lpRect->left %d lpRect->right %d lpRect->top %d\n",rect.bottom,rect.left,rect.right,rect.top);
							printf("lpRect->bottom: %d lpRect->left %d lpRect->right %d lpRect->top %d\n",rect2.bottom,rect2.left,rect2.right,rect2.top);
							//printf("nHorz: %d nVert: %d nSpacing: %d\n",nHorz,nVert,nSpacing);
#endif
							if(bIDCount>0)
							{
								if(m_bShowPrognosticedRemainingTime)
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
							if(m_bShowNumericClauseProgress)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								m_cStaticClauseProgress.Create("",WS_VISIBLE|WS_CHILD,
									MyRect,&m_wndStatusBar,ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE);
							}
							if(m_bShowGraphicClauseProgress)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								if(m_nGraphicClauseProgressType==0)
									m_Progress.Create(WS_VISIBLE|WS_CHILD,MyRect,
										&m_wndStatusBar,ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
								if(m_nGraphicClauseProgressType==1)
									m_Progress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
										&m_wndStatusBar,ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
								m_Progress.SetRange(0,100); //Set the range to between 0 and 100
								m_Progress.SetStep(1); // Set the step amount
							}
#ifdef _DEBUG
							printf("pFrame->m_bShowSentenceProgress\n");
#endif
							if(m_bShowNumericSentenceProgress)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								m_cStaticSentenceProgress.Create("",WS_VISIBLE|WS_CHILD,
									MyRect,&m_wndStatusBar,ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE);
							}
							if(m_bShowGraphicSentenceProgress)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								if(m_nGraphicSentenceProgressType==0)
									m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD,MyRect,
										&m_wndStatusBar,ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
								if(m_nGraphicSentenceProgressType==1)
									m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
										&m_wndStatusBar,ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
								m_SentenceProgress.SetRange(0,100); //Set the range to between 0 and 100
								m_SentenceProgress.SetStep(1); // Set the step amount
							}
							if(m_bShowNumericTextProgress)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								m_cStaticTextProgress.Create("",WS_VISIBLE|WS_CHILD,
									MyRect,&m_wndStatusBar,ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE);
							}
							if(m_bShowGraphicTextProgress)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								if(m_nGraphicTextProgressType==0)
									m_TextProgress.Create(WS_VISIBLE|WS_CHILD,MyRect,
										&m_wndStatusBar,ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
								if(m_nGraphicTextProgressType==1)
									m_TextProgress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
										&m_wndStatusBar,ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
								m_TextProgress.SetRange(0,100); //Set the range to between 0 and 100
								m_TextProgress.SetStep(1); // Set the step amount
							}
							if(m_bShowPrognosticedRemainingTime)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								m_cStaticRemainingTime.Create("",WS_VISIBLE|WS_CHILD,MyRect,
									&m_wndStatusBar,ID_INDICATOR_REMAINING_TIME);
								tAtTheBeginningOfTheTranslation=(CTime)CTime::GetCurrentTime();
							}
							LPCTSTR str="Fortschritt des Haupt- oder Nebensatzes";
							//statusbarCtrl.SetTipText(0,str);
							//statusbarCtrl.SetTipText(1,_T("Fortschritt des aktuellen Satzes"));
							//statusbarCtrl.SetTipText(2,_T("Fortschritt des ganzen Textes"));
							m_bCreated = TRUE;
							if(SetTimer(1,1000,NULL)==0)
							{
#ifdef _DEBUG
								printf("SetTimer(1,1000,NULL)==0\n");
#endif
							}
							else
							{
#ifdef _DEBUG
								printf("SetTimer(1,1000,NULL)!=0\n");
#endif
							}
							m_bTimerIntalled=TRUE;
						}
						TranslateAutomaticallyParameters * pTranslateAutomaticallyParameters=
							new TranslateAutomaticallyParameters(pEV->m_pText,vecRange,pEV2,
							m_hWnd,pEV2->GetSafeHwnd(),m_Progress.GetSafeHwnd(),
							m_SentenceProgress.GetSafeHwnd(),m_TextProgress.GetSafeHwnd());
						pTranslateAutomaticallyParameters->m_bCaseSensitive=m_bCaseSensitive;
						pTranslateAutomaticallyParameters->m_dwMaxCharsPerLine=m_dwMaxCharsPerLine;
						g_bContinue=TRUE;
						int nPriority=0;
						switch(m_nThreadPriority)
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
						m_pTranslationThread=AfxBeginThread(TranslateAutomatically,
							pTranslateAutomaticallyParameters,nPriority,0,0,NULL);
					}
				}
			}
			CloseClipboard();
		}
	}
}

CString concatenate(Word * pWord)
{
	CString tmp;
	if(typeid(*pWord)==typeid(EnglishAdjective)) // Wortart ist Adjektiv
	{
		EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(pWord);
		if(ea)
		{
			if(ea->m_bMoreMost)
				tmp='3'+CString((TCHAR)(ea->m_bMoreMost+ASCII_CODE_FOR_DIGIT_1))+
          CString((TCHAR)(ea->m_bAllowedPlace+ASCII_CODE_FOR_DIGIT_1))+
					ea->m_strPositiv+"9"+ea->m_strAdverb+"90";
			else
				tmp='3'+CString((TCHAR)(ea->m_bMoreMost+ASCII_CODE_FOR_DIGIT_1))+
        CString((TCHAR)(ea->m_bAllowedPlace+ASCII_CODE_FOR_DIGIT_1))+
					ea->m_strPositiv+"9"+ea->m_strComperativ+"9"+ea->
					m_strSuperlativ+"9"+ea->m_strAdverb+"90";
			//tmp+=ea->wordkind+ea->positiv+"9"+ea->comperativ+"9"+ea->superlativ+"9"+ea->adverb+"90";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(EnglishAdverb))
	{
		EnglishAdverb * ea=dynamic_cast<EnglishAdverb*>(pWord);
		if(ea)
		{
			tmp='4'+ea->m_strWord+"9"+(char)(ea->m_bType+49)+"0";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(EnglishAuxiliaryVerb))
	{
		EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>(pWord);
		if(eav)
		{
			tmp='7';
			int i=0;
			for(;i<14;i++)
			{
				tmp+=eav->m_strWords[i]+"9";
			}
			tmp+="0";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(EnglishConjunction))
	{
		EnglishConjunction * ec=dynamic_cast<EnglishConjunction*>(pWord);
		if(ec)
		{
			tmp='8';
			tmp+=ec->m_strWord+"9";
			tmp+="0";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(EnglishNoun))
	{
		EnglishNoun * en=dynamic_cast<EnglishNoun*>(pWord);
		if(en!=NULL)
		{
			tmp='1'+en->m_strSingular+"9"+en->m_strPlural+"9"+
				CString( (TCHAR)(en->m_bType+ASCII_CODE_FOR_DIGIT_1) )+
        CString( (TCHAR)(en->m_bTranslationType+ASCII_CODE_FOR_DIGIT_1) )+
        CString(
				  //wenn ein Substantiv zählbar ist, soll in der Vokabeldatei eine 
				  //"1" stehen, wenn nicht, soll da eine "2" stehen
				  //in der Datenstruktur im Arbeitsspeicher steht aber eine 1,
				  //wenn ein Substantiv zählbar ist, deswegen invertieren mit "!"
				  (TCHAR)((!en->m_bCountable)+ASCII_CODE_FOR_DIGIT_1) 
          )+"0";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(EnglishPreposition))
	{
		EnglishPreposition * ep=dynamic_cast<EnglishPreposition*>(pWord);
		if(ep)
		{
			tmp='5'+ep->m_strWord+"90";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(EnglishPronoun))
	{
		EnglishPronoun * ep=dynamic_cast<EnglishPronoun*>(pWord);
		if(ep)
		{
			tmp='6'+ep->m_strSingular+'9'+ep->m_strPlural+"90";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(EnglishVerb))
	{
		EnglishVerb * ev=dynamic_cast<EnglishVerb*>(pWord);
		if(ev)
		{
			tmp='2'+ev->m_strInfinitive+"9"+ev->m_strPastTense+"9"+
				ev->m_strPastParticiple+"9"+ev->m_strPreposition+"9";
			// wenn Hilsverb gleich 'sein'
			if(ev->m_bAuxiliaryType)
				tmp+='1';
			else       // sonst ist Hilsverb 'haben'
				tmp+='2';
			//wenn nach der gebeugten Verbform ein To + Infinitiv stehen darf
			if(ev->m_bAllowsToPlusInfinitive)
			{
				//wenn nach der gebeugten Verbform eine ing-Form stehen darf
				if(ev->m_bAllowsIngForm)
					tmp+='1';
				else
					tmp+='2';
			}
			else
				//wenn nach der gebeugten Verbform eine ing-Form stehen darf
				if(ev->m_bAllowsIngForm)
					tmp+='3';
				else 
					tmp+='4';
			tmp+="0";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(GermanAdjective)) // wenn deutsch
	{
		GermanAdjective * ga=dynamic_cast<GermanAdjective*>(pWord);
		if(ga)
		{
			tmp='3'+ga->m_strPositiv+"9"+ga->m_strComperativ+"9"+
				ga->m_strSuperlativ+"9"+ga->m_strWortstamm+"9"+"0";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(GermanAdverb)) // wenn deutsch
	{
		GermanAdverb * ga=dynamic_cast<GermanAdverb*>(pWord);
		if(ga)
		{
			tmp='4'+ga->m_strWord+"9"+"0";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(GermanConjunction))
	{
		GermanConjunction * gc=dynamic_cast<GermanConjunction*>(pWord);
		if(gc)
		{
			tmp='8';
			tmp+=gc->m_strWord+"9";
			if(gc->m_bWordOrder==0)
				tmp+="1";
			if(gc->m_bWordOrder==1)
				tmp+="2";
			tmp+="0";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(GermanNoun))
	{
		GermanNoun * gn=dynamic_cast<GermanNoun*>(pWord);
		if(gn)
		{
			tmp='1'+gn->m_strSingular+"9"+gn->m_strPlural+"9"+CString((TCHAR)gn->m_bArticle)+"0";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(GermanPreposition))
	{
		GermanPreposition * gp=dynamic_cast<GermanPreposition*>(pWord);
		if(gp)
		{
			tmp='5'+gp->m_strWord+"90";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(GermanPronoun))
	{
		GermanPronoun * gp=dynamic_cast<GermanPronoun*>(pWord);
		if(gp)
		{
			tmp='6'+gp->m_strWord+"90";
			return tmp;
		}
	}
	if(typeid(*pWord)==typeid(GermanVerb))
	{
		GermanVerb * gv=dynamic_cast<GermanVerb*>(pWord);
		if(gv)
		{
			if(gv->m_bAuxiliaryVerb)
				tmp="7";
			else
				tmp="2";
			int i=0;
			for(;i<16;i++)
			{
				tmp+=gv->m_strWords[i]+"9";
			}
			if(!gv->m_bAuxiliaryVerb) // wenn kein Hilfsverb
			{
				tmp+=gv->m_strPreposition+"9";
				if(gv->m_bMove) // wenn das Verb ein Verb der Bewegung
				{            //  in eine bestimmte Richtung ist
					tmp+="2";
				}
				else
				{
					tmp+="1";
				}
			}
			if(gv->m_bBe) // Hilfsverb: sein
			{
				tmp+='2';
			}
			else // Hilfsverb: haben
			{
				tmp+='1';
			}
			if(!gv->m_bAuxiliaryVerb)
			{
				if(gv->m_bCase==0) // kein Fall
					tmp+='1';
				if(gv->m_bCase==1) // 3. Fall
					tmp+='2';
				if(gv->m_bCase==2) // 4. Fall
					tmp+='3';
				if(gv->m_bCase==3) // 3. und 4. Fall
					tmp+='4';
				if(!gv->m_bReflexive) // kein Fall
					tmp+='1';
				if(gv->m_bReflexive) // kein Fall
					tmp+='2';
			}
			tmp+="0";
			return tmp;
		}
	}
	return tmp;
}

void CMainFrame::OnAddVocableButton() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
#ifdef _DEBUG
	printf("void CMainFrame::OnAddVocableButton() - ANFANG\n");
#endif
	//m_wndMyBar GUIToWord();
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * pWordNodeCurrent=wordList.m_pWordNodeFirst;
		Word * pWordEnglish,*pWordGerman;
		pWordEnglish=m_wndVocableEditorBar.GUIToEnglishWord();
		pWordGerman=m_wndVocableEditorBar.GUIToGermanWord();
		while(1)
		{
			//++*voc;
			if(*pWordNodeCurrent->m_pWord==pWordEnglish) // wenn Vokabel der verketteten Liste mit der
			{ //  Vokabel, die aus der Benutzeroberfläche entstanden ist, identisch
				if(pWordNodeCurrent->m_pWordNodeNext!=NULL) //  ist, dann zusätzlich die deutsche 
				{ // Übersetzung dieser Vokabel mit der neu entstandenen deutschen
					pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext; // Vokabel aus der Benutzeroberfläche
					if(*pWordNodeCurrent->m_pWord==pWordGerman) //  auf Gleichheit überprüfen
					{
						MessageBox(_T("Vokabel existiert bereits und wird nicht hinzugefügt"),
							_T("Hinweis"),MB_ICONINFORMATION);
						return; // Vokabel existiert schon, Funktion beenden
					}
				}
			}
			if(pWordNodeCurrent->m_pWordNodeNext)
				pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
			else
				break;
		}
	}
	WordNode * pWordNodeCurrent=NULL;
	WordNode * pWordNodePrevious=NULL;
	CString str;
	wordList.m_pWordNodeLast->m_pWordNodeNext=new WordNode();
	wordList.m_pWordNodeLast->m_pWordNodeNext->m_pWord=
		m_wndVocableEditorBar.GUIToEnglishWord();
	if(typeid(*wordList.m_pWordNodeLast->m_pWordNodeNext->m_pWord)==
		typeid(EnglishPreposition))
	{
		EnglishPreposition * ep=dynamic_cast<EnglishPreposition*>(
			wordList.m_pWordNodeLast->m_pWordNodeNext->m_pWord);
		m_wndVocableEditorBar.m_englishComboBox.AddString(ep->m_strWord);
	}
	pWordNodePrevious=wordList.m_pWordNodeLast->m_pWordNodeNext;
	pWordNodeCurrent=wordList.m_pWordNodeLast->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=wordList.m_pWordNodeLast;
	str=concatenate(pWordNodeCurrent->m_pWord);
	//TRACE("concatenate(voc): %s\n",concatenate(voc));
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=m_wndVocableEditorBar.GUIToGermanWord();
	if(typeid(*pWordNodeCurrent->m_pWordNodeNext->m_pWord)==typeid(GermanPreposition))
	{
		GermanPreposition * gp=dynamic_cast<GermanPreposition*>(
			pWordNodeCurrent->m_pWordNodeNext->m_pWord);
		m_wndVocableEditorBar.m_germanComboBox.AddString(gp->m_strWord);
	}
	//voc=m_wndVocableEditorBar.GUIToGermanWord();
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodeNext=NULL;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;
	str+=concatenate(pWordNodeCurrent->m_pWord);
	wordList.m_pWordNodeLast=pWordNodeCurrent;
	//voclist.Append(voc);
	CFile file;
	if(file.Open("words.txt",CFile::modeWrite|CFile::shareDenyRead,NULL))
	{
		file.SeekToEnd();
		file.Write(str.GetBuffer(str.GetLength()),str.GetLength());
		file.Close();
	}
}

void CMainFrame::OnOverwriteVocableButton() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	//wenn pCurrentVocNode auf eine Vokabel zeigt
	if(pWordNodeCurrent)
	{
		//wenn sich vor pCurrentVocNode mindestens ein Listenelement befindet
		if(pWordNodeCurrent->m_pWordNodePrevious)
		{
			WordNode * pWordNodePrevious=pWordNodeCurrent->m_pWordNodePrevious;
			//wenn sich hinter pCurrentVocNode mindestens ein Listenelement befindet
			if(pWordNodeCurrent->m_pWordNodeNext)
			{
				WordNode * pWordNodeNext=pWordNodeCurrent->m_pWordNodeNext;
				if(pWordNodeNext->m_pWordNodeNext)
				{
					pWordNodeNext=pWordNodeNext->m_pWordNodeNext;
					pWordNodePrevious->m_pWordNodeNext=pWordNodeNext;
					delete pWordNodeNext->m_pWordNodePrevious;
					pWordNodeNext->m_pWordNodePrevious=pWordNodePrevious;
					delete pWordNodeCurrent;
					pWordNodeCurrent=NULL;
				}
				else
				{
					delete pWordNodeNext;
					delete pWordNodeCurrent;
					pWordNodePrevious->m_pWordNodeNext=NULL;
					wordList.m_pWordNodeLast=pWordNodePrevious;
				}
			}
			//hinter pCurrentVocNode befindet sich kein Listenelement, pCurrentVocNode
			//ist das Ende der Liste
			else
			{
				if(pWordNodePrevious->m_pWordNodePrevious)
				{
					pWordNodePrevious=pWordNodePrevious->m_pWordNodePrevious;
					wordList.m_pWordNodeLast=pWordNodePrevious;
					delete pWordNodePrevious->m_pWordNodeNext;
					pWordNodePrevious->m_pWordNodeNext=NULL;
					delete pWordNodeCurrent;
					pWordNodeCurrent=NULL;
				}
			}
		}
		//vor pCurrentVocNode befindet sich kein Listenelement, pCurrentVocNode
		//ist der Anfang der Liste
		else 
		{
			if(pWordNodeCurrent->m_pWordNodeNext)
			{
				WordNode * pWordNodeNext=pWordNodeCurrent->m_pWordNodeNext;
				if(pWordNodeNext->m_pWordNodeNext)
					pWordNodeNext=pWordNodeNext->m_pWordNodeNext;
				//VocNode * previous=
				pWordNodeNext->m_pWordNodePrevious=NULL;
				wordList.m_pWordNodeFirst=pWordNodeNext;
				delete pWordNodeNext->m_pWordNodePrevious;
				delete pWordNodeCurrent;
				pWordNodeCurrent=NULL;
			}
			else // Liste hat nur 1 Element
			{
				wordList.m_pWordNodeFirst=NULL;
				wordList.m_pWordNodeLast=NULL;
				delete pWordNodeCurrent;
				pWordNodeCurrent=NULL;
			}
		}
	}
	WordNode * current=NULL;
	WordNode * previous=NULL;
	CString str;
	wordList.m_pWordNodeLast->m_pWordNodeNext=new WordNode();
	wordList.m_pWordNodeLast->m_pWordNodeNext->m_pWord=
		m_wndVocableEditorBar.GUIToEnglishWord();
	pWordNodeCurrent=wordList.m_pWordNodeLast->m_pWordNodeNext;
	previous=wordList.m_pWordNodeLast->m_pWordNodeNext;
	current=wordList.m_pWordNodeLast->m_pWordNodeNext;
	current->m_pWordNodePrevious=wordList.m_pWordNodeLast;
	str=concatenate(current->m_pWord);
	current->m_pWordNodeNext=new WordNode();
	current->m_pWordNodeNext->m_pWord=m_wndVocableEditorBar.GUIToGermanWord();
	current=current->m_pWordNodeNext;
	current->m_pWordNodeNext=NULL;
	current->m_pWordNodePrevious=previous;
	str+=concatenate(current->m_pWord);
	wordList.m_pWordNodeLast=current;

	CFile file;
	if(wordList.m_pWordNodeFirst && file.Open("words.txt",CFile::modeWrite|
		CFile::shareDenyRead,NULL))
	{
		file.SetLength(0L);
		BOOL bEnglish=TRUE;
		str="";
		current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(!current->m_pWord->m_bIntegral)
				str=concatenate(current->m_pWord);
			file.Write(str.GetBuffer(str.GetLength()),str.GetLength());
			if(current->m_pWordNodeNext)
				current=current->m_pWordNodeNext;
			else
				break;
		}
		file.Close();
	}
}

void CMainFrame::OnDeleteVocableButton() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	
	BOOL bDelete=TRUE;
	if(m_bEnsureUserWantsDeletion)
	{
		bDelete=FALSE;
		if(MessageBox("Sind Sie sicher, dass Sie die Vokabel löschen möchten?",
			"Nachfrage",MB_YESNO|MB_ICONQUESTION)==IDYES)
			bDelete=TRUE;
	}
	if(!bDelete)
		return;
  wordList.Delete() ;

	CFile file;
	if(wordList.m_pWordNodeFirst && file.Open("words.txt",CFile::modeWrite|
		CFile::shareDenyRead,NULL))
	{
		file.SetLength(0L);
		//Vocable * voc=NULL;
		//file.SeekToEnd();
		BOOL bEnglish=TRUE;
		CString str;
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
//			if(bEnglish)
	//			voc=m_wndVocableEditorBar.GUIToEnglishWord();
		//	else
			//	voc=m_wndVocableEditorBar.GUIToGermanWord();
			//bEnglish=!bEnglish;
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
			}
			else if(typeid(*current->m_pWord)==typeid(GermanVerb))
			{
				GermanVerb * ga=dynamic_cast<GermanVerb*>(current->m_pWord);
				if(!ga->m_bAuxiliaryVerb)
				{
					if(!current->m_pWord->m_bIntegral)
					{
						str=concatenate(current->m_pWord);
						file.Write(str.GetBuffer(str.GetLength()),str.GetLength());
					}
				}
			}
			else
			{
				if(!current->m_pWord->m_bIntegral)
				{
					str=concatenate(current->m_pWord);
				//str+=concatenate(voc);
					file.Write(str.GetBuffer(str.GetLength()),str.GetLength());
				}
			}
			if(current->m_pWordNodeNext)
				current=current->m_pWordNodeNext;
			else
				break;
		}
		file.Close();
	}
	//if(
}

void CMainFrame::OnDestroyClipboard()
{
	//MessageBox("uzzz",NULL,MB_OK);
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	CMenu * menu=GetMenu();
	if(menu)
		menu->EnableMenuItem(ID_EDIT_PASTE,MF_DISABLED);
	//else
		MessageBox("!menu",NULL,MB_OK);
	
	CMDIFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	TRACE("void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)\n");
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen

	BOOL bClipBoardContainsText=FALSE;
	if(OpenClipboard())
	{
		if(IsClipboardFormatAvailable(CF_TEXT)
			|| IsClipboardFormatAvailable(CF_OEMTEXT))
		{
			CToolBarCtrl & toolBarCtrl=m_wndToolBar.GetToolBarCtrl();
			toolBarCtrl.EnableButton(ID_EDIT_PASTE,TRUE);
			//toolBarCtrl.CheckButton(ID_EDIT_PASTE,TRUE);
			toolBarCtrl.Indeterminate(ID_EDIT_PASTE,FALSE);
			bClipBoardContainsText=TRUE;
		}
		CloseClipboard();
	}
	if(!bClipBoardContainsText)
	{
		CToolBarCtrl & toolBarCtrl=m_wndToolBar.GetToolBarCtrl();
		toolBarCtrl.EnableButton(ID_EDIT_PASTE,FALSE);
		//toolBarCtrl.CheckButton(ID_EDIT_PASTE,FALSE);
		toolBarCtrl.Indeterminate(ID_EDIT_PASTE,TRUE);
	}
	CMenu * menu=GetMenu();
	if(menu)
	{
		menu->EnableMenuItem(ID_EDIT_PASTE,MF_BYCOMMAND|MF_GRAYED);//MF_GRAYED);
		CString str;
		menu->GetMenuString(ID_EDIT_PASTE,str,MF_BYCOMMAND);
		TRACE("str: %s\n",str);
		DrawMenuBar();
	}
	//EnableMenuItem(GetMenu(),ID_EDIT_PASTE,MF_BYCOMMAND|MF_GRAYED);
	//GetDlgItem(ID_EDIT_PASTE)->EnableWindow(TRUE);
//	else
	//	MessageBox("!menu",NULL,MB_OK);
	  //ModifyMenu
	//UpdateWindow(menu->m_hMenu);
	//menu->UpdateWindow();
	UpdateDialogControls(this,TRUE);
}

void CMainFrame::OnComboboxButton() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	TRACE("void CMainFrame::OnComboboxButton() - ANFANG\n");
	CChildFrame * pChild=(CChildFrame*)MDIGetActive();
	if(pChild)
	{
		EnglishView * pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
		if(pEV)
		{
			pEV->m_bDrawVector=!pEV->m_bDrawVector;
			if(pEV->m_pText->m_first)
				pEV->m_pText->Clear();
			//wenn Kombinationsfelder angezeigt werden sollen
			if(pEV->m_bDrawVector)
			{
				CToolBarCtrl & toolBarCtrl=m_wndToolBar.GetToolBarCtrl();
				toolBarCtrl.CheckButton(ID_COMBOBOX_BUTTON,TRUE);
				//toolBarCtrl.(ID_COMBOBOX_BUTTON,TRUE);
				pEV->m_bDrawCursor=FALSE;
				pEV->KillTimer(1);
				ComboBoxListElement * pCurrent=NULL;
				pCurrent=pEV->m_ComboBoxList.m_pFirst;
				while(pCurrent)
				{
					TRACE("pCurrent\n");
					pCurrent->ComboBox.ShowWindow(SW_SHOW);
					if(pCurrent->m_pNext)
						pCurrent=pCurrent->m_pNext;
					else
						break;
				}
			}
			//wenn KEINE Kombinationsfelder angezeigt werden sollen
			if(!pEV->m_bDrawVector)
			{
				pEV->m_bDrawCursor=TRUE;
				CToolBarCtrl & toolBarCtrl=m_wndToolBar.GetToolBarCtrl();
				toolBarCtrl.CheckButton(ID_COMBOBOX_BUTTON,FALSE);
				//int iInstallResult=pEV->SetTimer(1,500,NULL);
				//if(!iInstallResult)
				//	AfxMessageBox("kann keinen Timer installieren! Das heisst, das es\
//keinen blinkenden Cursor in einem oder 2 Textfeld(ern) des aktiven Kindfensters geben wird",MB_OK,NULL);
				ComboBoxListElement * pCurrent=NULL;
				pCurrent=pEV->m_ComboBoxList.m_pFirst;
				DWORD dwPos=0;
				DWORD dwCharsPerLine=0;
				BOOL bFirstNode=TRUE;
				for(DWORD dwRun=0;dwRun<pEV->m_cstrvv.size();dwRun++)
				{
					//dwPos+=pEV->m_cstrvv.at(dwRun).GetLength();
					CStringVector cstrv=pEV->m_cstrvv.at(dwRun);
					CString str=cstrv.at(0);
					if(str!=","  && str!="." && dwRun>0)
					{
						if(dwCharsPerLine+1<=m_dwMaxCharsPerLine)
						{
							//if(!bFirstNode)
							//	pEV->m_pText->Insert(dwPos,"\r\n",0);
							//dwPos+=2;
							//pEV->m_pText->Insert(dwPos,str.GetBuffer(str.GetLength()),0);
							//dwCharsPerLine=0;
							pEV->m_pText->Insert(dwPos,' ',0);
							dwPos++;
							++dwCharsPerLine;
						}
					}
					if(cstrv.size()>1)
					{
						if(pCurrent)
						{
							pCurrent->ComboBox.GetLBText(pCurrent->ComboBox.GetCurSel(),str);
							if(dwCharsPerLine+str.GetLength()>m_dwMaxCharsPerLine)
							{
								if(!bFirstNode)
									pEV->m_pText->Insert(dwPos,"\r\n",0);
								//dwPos+=2;
								++dwPos;
								//pEV->m_pText->Insert(dwPos,str.GetBuffer(str.GetLength()),0);
								dwCharsPerLine=0;
							}
							//else
							pEV->m_pText->Insert(dwPos,str.GetBuffer(str.GetLength()),0);
							pCurrent->ComboBox.ShowWindow(SW_HIDE);
							pCurrent=pCurrent->m_pNext;
						}
					}
					else
					{
						if(dwCharsPerLine+str.GetLength()>m_dwMaxCharsPerLine)
						{
							if(!bFirstNode)
								pEV->m_pText->Insert(dwPos,"\r\n",0);
							++dwPos;
							//pEV->m_pText->Insert(dwPos,str.GetBuffer(str.GetLength()),0);
							dwCharsPerLine=0;
						}
					//else
						pEV->m_pText->Insert(dwPos,str.GetBuffer(str.GetLength()),0);
					}
					dwPos+=str.GetLength();
					dwCharsPerLine+=str.GetLength();
					bFirstNode=FALSE;
				}
				/*while(pCurrent)
				{
					pCurrent->ComboBox.ShowWindow(SW_HIDE);
					pCurrent=pCurrent->m_pNext;
				}*/
			}
			pEV->Invalidate();
		}
	}	
	TRACE("void CMainFrame::OnComboboxButton() - ENDE\n");
}

void CMainFrame::OnEditCopy() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	
	TRACE("void CMainFrame::OnEditCopy() - Anfang\n");
	//HANDLE hText=NULL;
	HANDLE hText = NULL;
	CChildFrame * pChild=(CChildFrame*)MDIGetActive();
	CWnd* pWnd = GetFocus();
	if(pChild && pWnd)
	{
		EnglishView * pEV=(EnglishView*)pChild->GetActiveView();
		//EnglishView * pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(0,0);
		if(pEV)// && hText!=NULL)
		{
			if(pEV->m_bMark && OpenClipboard())
			{
				DWORD dwMarkingStart=pEV->m_dwMarkingStart<pEV->m_dwMarkingEnd?pEV->m_dwMarkingStart:pEV->m_dwMarkingEnd;
				DWORD dwMarkingEnd=pEV->m_dwMarkingStart>pEV->m_dwMarkingEnd?pEV->m_dwMarkingStart:pEV->m_dwMarkingEnd;
				CString str;
				pEV->m_pText->GetCString(str,dwMarkingStart,dwMarkingEnd-1);

				/* Allocate memory and copy the string to it */
				//str="dgfhg";
				HGLOBAL clipbuffer;
        char * buffer;
        EmptyClipboard();
        clipbuffer = GlobalAlloc(GMEM_DDESHARE, str.GetLength()+1);
        buffer = (char*)GlobalLock(clipbuffer);
        strcpy(buffer, LPCSTR(str));
        GlobalUnlock(clipbuffer);
        SetClipboardData(CF_TEXT,clipbuffer);
				CloseClipboard();
				GlobalFree(clipbuffer);
				/*TRACE("OpenClipboard()\n");
				if(!(hData=GlobalAlloc(GMEM_DDESHARE, GlobalSize (str.GetBuffer(str.GetLength())))))
				{
					return;
				}
				TRACE("OpenClipboard()\n");
				if(!(lpData = (char *)GlobalLock(hData)))
				{
					return;
				}
				TRACE("vor: if(!(lpszText = (char *)GlobalLock (str.GetBuffer(str.GetLength()))))\n");
				if(!(lpszText = (char *)GlobalLock ( (LPCTSTR)str ))) //.GetBuffer(str.GetLength()) )))
				{
					return;
				}*/
				//TRACE("OpenClipboard()\n");
				//strcpy(lpData, lpszText);
				//GlobalUnlock(hData);
				//GlobalUnlock (hText);
				/* Clear the current contents of the clipboard, and set
                         * the data handle to the new string.
                         */
				/*TRACE("vor: OpenClipboard()\n");
				if(OpenClipboard())
				{
					TRACE("OpenClipboard()\n");
					EmptyClipboard();
					SetClipboardData(CF_TEXT, hData);
					CloseClipboard();
				}
				hData = NULL;*/
			}
		}
/*		if(LOWORD(wParam)==IDM_CUT)
		{
			GlobalFree (hText);
			hText = NULL;
			//EnableMenuItem(GetMenu (hWnd), IDM_CUT, MF_GRAYED);
			//EnableMenuItem(GetMenu(hWnd), IDM_COPY, MF_GRAYED);
			//InvalidateRect (hWnd, NULL, TRUE);
			//UpdateWindow (hWnd);
		}*/
	}
}

void CMainFrame::OnUpdateNextVocableButton(CCmdUI* pCmdUI) 
{
	// TODO: Code für die Befehlsbehandlungsroutine zum Aktualisieren der Benutzeroberfläche hier einfügen
	if(m_bEnableNextVocableButton)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnNextVocableButton() 
{
//	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
//	if(pWordNodeCurrent)
//	{
//		TRACE("m_first ist ungleich NULL\n");
//		WordNode * current=pWordNodeCurrent;
//		BYTE Hits=0;
//		CString str=((CVTransApp*)AfxGetApp())->m_strSearch;
//		BOOL bFound=FALSE;
//		m_bEnableNextVocableButton=FALSE;
//		m_bEnablePreviousVocableButton=FALSE;
//		if(str=="*")
//		{
//			if(pWordNodeCurrent->m_pWordNodeNext)
//			{
//				WordNode * pWordNodeNext=pWordNodeCurrent->m_pWordNodeNext;
//				if(pWordNodeNext->m_pWordNodeNext)
//				{
//					pWordNodeCurrent=pWordNodeNext->m_pWordNodeNext;
//					m_wndVocableEditorBar.WordToGUI(pWordNodeCurrent);
//					m_bEnablePreviousVocableButton=TRUE;
//					if(pWordNodeCurrent->m_pWordNodeNext)
//					{
//						pWordNodeNext=pWordNodeCurrent->m_pWordNodeNext;
//						if(pWordNodeNext->m_pWordNodeNext)
//							m_bEnableNextVocableButton=TRUE;
//					}
//				}
//			}
//			return;
//		}
//		while(1)
//		{
//			if(Hits==1)
//				m_bEnablePreviousVocableButton=TRUE;
//			if(Hits==2)
//			{
//				m_bEnableNextVocableButton=TRUE;
//				break;
//			}
//
//			if(current->m_pWordNodeNext)
//				current=current->m_pWordNodeNext;
//			else
//			{
//				break;
//			}
//			if(typeid(*current->m_pWord)==typeid(EnglishAdjective)) // wenn englisches Substantiv
//			{
//				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(current->m_pWord);
//				if(ea!=NULL)
//				{
//					if(ea->m_strPositiv==str)
//					{
//						Hits++;
//						//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
//						if(!bFound)
//						{
//							pWordNodeCurrent=current;
//							TRACE("m_ptrMyBar->WordToGUI(current);\n");
//							m_wndVocableEditorBar.WordToGUI(current);
//						}
//						//else
//							//GetDlgItem(ID_USER+101)->EnableWindow(TRUE);
//						//m_ptrMyBar->MoveWindow(0,0,100,100,TRUE);
//						//MessageBox("jjllll",NULL,MB_OK);
//						bFound=TRUE;
//					}
//				}
//			}
//			if(typeid(*current->m_pWord)==typeid(EnglishAdverb)) // wenn englisches Substantiv
//			{
//				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>(current->m_pWord);
//				if(eav!=NULL)
//				{
//					if(eav->m_strWord==str)
//					{
//						Hits++;
//						if(!bFound)
//						{
//							pWordNodeCurrent=current;
//							//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
//							TRACE("m_ptrMyBar->WordToGUI(current);\n");
//							m_wndVocableEditorBar.WordToGUI(current);
//						}
//						//else
//							//GetDlgItem(ID_USER+101)->EnableWindow(TRUE);
//						//m_ptrMyBar->MoveWindow(0,0,100,100,TRUE);
//						//MessageBox("jjllll",NULL,MB_OK);
//						bFound=TRUE;
//					}
//				}
//			}
//			if(typeid(*current->m_pWord)==typeid(EnglishConjunction))
//			{
//				EnglishConjunction * ec=dynamic_cast<EnglishConjunction*>(current->m_pWord);
//				if(ec!=NULL)
//				{
//					if(ec->m_strWord==str)
//					{
//						Hits++;
//						if(!bFound)
//						{
//							pWordNodeCurrent=current;
//							//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
//							TRACE("m_ptrMyBar->WordToGUI(current);\n");
//							m_wndVocableEditorBar.WordToGUI(current);
//						}
//						bFound=TRUE;
//					}
//				}
//			}
//			if(typeid(*current->m_pWord)==typeid(EnglishNoun)) // wenn englisches Substantiv
//			{
//				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
//				if(en!=NULL)
//				{
//					TRACE("en!=NULL\n");
//					if(en->m_strSingular==str || en->m_strPlural==str)
//					{
//						Hits++;
//#ifdef _DEBUG
//						printf("en->m_strSingular: %s\n",en->m_strSingular);
//#endif
//						if(!bFound)
//						{
//							pWordNodeCurrent=current;
//							TRACE("m_ptrMyBar->WordToGUI(current);\n");
//							m_wndVocableEditorBar.WordToGUI(current);
//						}
//						else
//						{
//						}
//						bFound=TRUE;
//					}
//				}
//			}
//			if(typeid(*current->m_pWord)==typeid(EnglishPreposition)) // wenn englische Präposition
//			{
//				EnglishPreposition * ep=dynamic_cast<EnglishPreposition*>(current->m_pWord);
//				if(ep!=NULL)
//				{
//					if(ep->m_strWord==str)
//					{
//						Hits++;
//						if(!bFound)
//						{
//							pWordNodeCurrent=current;
//							//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
//							TRACE("m_ptrMyBar->WordToGUI(current);\n");
//							m_wndVocableEditorBar.WordToGUI(current);
//						}
//						bFound=TRUE;
//					}
//				}
//			}
//			if(typeid(*current->m_pWord)==typeid(EnglishPronoun)) // wenn englisches Pronomen
//			{
//				EnglishPronoun * ep=dynamic_cast<EnglishPronoun*>(current->m_pWord);
//				if(ep!=NULL)
//				{
//					if(ep->m_strSingular==str || ep->m_strPlural==str)
//					{
//						Hits++;
//						if(!bFound)
//						{
//							pWordNodeCurrent=current;
//							//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
//							TRACE("m_ptrMyBar->WordToGUI(current);\n");
//							m_wndVocableEditorBar.WordToGUI(current);
//						}
//						bFound=TRUE;
//					}
//				}
//			}
//			if(typeid(*current->m_pWord)==typeid(EnglishVerb)) // wenn englisches Substantiv
//			{
//				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
//				if(ev!=NULL)
//				{
//					if(ev->m_strInfinitive==str)
//					{
//						Hits++;
//						if(!bFound)
//						{
//							pWordNodeCurrent=current;
//							//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
//							TRACE("m_ptrMyBar->WordToGUI(current);\n");
//							m_wndVocableEditorBar.WordToGUI(current);
//						}
//						bFound=TRUE;
//					}
//				}
//			}
//			if(typeid(*current->m_pWord)==typeid(GermanAdjective)) // wenn deutsches Adjektiv
//			{
//				GermanAdjective * ga=dynamic_cast<GermanAdjective*>(current->m_pWord);
//				if(ga!=NULL)
//				{
//					if(ga->m_strPositiv==str || ga->m_strComperativ==str || 
//						ga->m_strSuperlativ==str ||	ga->m_strWortstamm==str || 
//						ga->m_strWortstamm+_T("er")==str ||	
//						ga->m_strWortstamm+_T("e")==str || 
//						ga->m_strWortstamm+_T("es")==str ||
//						ga->m_strWortstamm+_T("en")==str)
//					{
//						Hits++;
//						if(!bFound)
//						{
//							pWordNodeCurrent=current->m_pWordNodePrevious;
//							m_wndVocableEditorBar.WordToGUI(current);
//						}
//						bFound=TRUE;
//					}
//				}
//			}
//			if(typeid(*current->m_pWord)==typeid(GermanAdverb)) // wenn deutsches Adverb
//			{
//				GermanAdverb * gav=dynamic_cast<GermanAdverb*>(current->m_pWord);
//				if(gav!=NULL)
//				{
//					if(gav->m_strWord==str)
//					{
//						Hits++;
//						if(!bFound)
//						{
//							pWordNodeCurrent=current->m_pWordNodePrevious;
//							TRACE("m_ptrMyBar->WordToGUI(current);\n");
//							m_wndVocableEditorBar.WordToGUI(current);
//						}
//						bFound=TRUE;
//					}
//				}
//			}
//			//wenn deutsche Konjunktion
//			if(typeid(*current->m_pWord)==typeid(GermanConjunction))
//			{
//				GermanConjunction * gc=dynamic_cast<GermanConjunction*>(current->m_pWord);
//				if(gc!=NULL)
//				{
//					if(gc->m_strWord==str)
//					{
//						Hits++;
//						if(!bFound)
//						{
//							pWordNodeCurrent=current;
//							//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
//							TRACE("m_ptrMyBar->WordToGUI(current);\n");
//							m_wndVocableEditorBar.WordToGUI(current);
//						}
//						bFound=TRUE;
//					}
//				}
//			}
//			if(typeid(*current->m_pWord)==typeid(GermanPreposition)) // wenn deutsche Präposition
//			{
//				GermanPreposition * gp=dynamic_cast<GermanPreposition*>(current->m_pWord);
//				if(gp!=NULL)
//				{
//					if(gp->m_strWord==str)
//					{
//						Hits++;
//						if(!bFound)
//						{
//							pWordNodeCurrent=current->m_pWordNodePrevious;
//							TRACE("m_ptrMyBar->WordToGUI(current);\n");
//							m_wndVocableEditorBar.WordToGUI(current);
//						}
//						bFound=TRUE;
//					}
//				}
//			}
//			if(typeid(*current->m_pWord)==typeid(GermanPronoun)) // wenn deutsches Pronomen
//			{
//				GermanPronoun * gp=dynamic_cast<GermanPronoun*>(current->m_pWord);
//				if(gp!=NULL)
//				{
//					if(gp->m_strWord==str)
//					{
//						Hits++;
//						if(!bFound)
//						{
//							pWordNodeCurrent=current->m_pWordNodePrevious;
//							TRACE("m_ptrMyBar->WordToGUI(current);\n");
//							m_wndVocableEditorBar.WordToGUI(current);
//						}
//						bFound=TRUE;
//					}
//				}
//			}
//			if(typeid(*current->m_pWord)==typeid(GermanNoun)) // wenn englisches Substantiv
//			{
//				GermanNoun * gn=dynamic_cast<GermanNoun*>(current->m_pWord);
//				if(gn!=NULL)
//				{
//					BOOL bHit=FALSE;
//					CStringVector sv=GetCStringVector2(gn->m_strSingular);
//					CStringVector sv2=GetCStringVector2(gn->m_strPlural);
//					if(gn->m_strSingular==str || gn->m_strPlural==str)
//					{
//						bHit=TRUE;
//						Hits++;
//#ifdef _DEBUG
//						printf("gn->m_strSingular: %s\n",gn->m_strSingular);
//#endif
//						if(!bFound)
//						{
//							//m_pVocableEditorBar->m_szHorz.cy=100;
//							//m_pVocableEditorBar->CalcDynamicLayout(100,LM_LENGTHY);
//							pWordNodeCurrent=current->m_pWordNodePrevious;
//							m_wndVocableEditorBar.WordToGUI(current);
//						}
//						else
//						{
//							GetDlgItem(IDC_VOCABLE_FORWARD)->EnableWindow(TRUE);
//						}
//						bFound=TRUE;
//					}
//					if(sv.size()>0 && !bHit)
//					{
//						if(str==sv.at(0))
//						{
//							bHit=TRUE;
//							Hits++;
//#ifdef _DEBUG
//							printf("gn->m_strSingular: %s\n",gn->m_strSingular);
//#endif
//							if(!bFound)
//							{
//								//m_pVocableEditorBar->m_szHorz.cy=100;
//								//m_pVocableEditorBar->CalcDynamicLayout(100,LM_LENGTHY);
//								pWordNodeCurrent=current->m_pWordNodePrevious;
//								m_wndVocableEditorBar.WordToGUI(current);
//							}
//							else
//							{
//								GetDlgItem(IDC_VOCABLE_FORWARD)->EnableWindow(TRUE);
//							}
//							bFound=TRUE;
//						}
//					}
//					if(sv2.size()>0 && !bHit)
//					{
//						if(str==sv2.at(0))
//						{
//							Hits++;
//#ifdef _DEBUG
//							//printf("en->singular: %s\n",en->singular);
//#endif
//							if(!bFound)
//							{
//								//m_pVocableEditorBar->m_szHorz.cy=100;
//								//m_pVocableEditorBar->CalcDynamicLayout(100,LM_LENGTHY);
//								pWordNodeCurrent=current->m_pWordNodePrevious;
//								m_wndVocableEditorBar.WordToGUI(current);
//							}
//							else
//							{
//								GetDlgItem(IDC_VOCABLE_FORWARD)->EnableWindow(TRUE);
//							}
//							bFound=TRUE;
//						}
//					}
//				}
//			}
//			if(typeid(*current->m_pWord)==typeid(GermanVerb)) // wenn deutsches Substantiv
//			{
//				GermanVerb * gv=dynamic_cast<GermanVerb*>(current->m_pWord);
//				if(gv!=NULL)
//				{
//					for(BYTE bRun=0;bRun<16;bRun++)
//						if(gv->m_strWords[bRun]==str)
//						{
//							Hits++;
//							if(!bFound)
//							{
//								pWordNodeCurrent=current->m_pWordNodePrevious;
//								m_wndVocableEditorBar.WordToGUI(current);
//							}
//							bFound=TRUE;
//							break; // bei zwei gleichen gebeugten Verbformen [betrifft 3. 
//							// Person Singular (er/sie/es) und 2. Person Plural (ihr) ]
//							// die Variable "Hits" NICHT erhöhen
//							// Beispiel: "er geht" , "ihr geht"
//						}
//				}
//			}
//		}
//	}
  //wordList.GetNextWord() ;
}

void CMainFrame::OnUpdatePreviousVocableButton(CCmdUI* pCmdUI) 
{
	// TODO: Code für die Befehlsbehandlungsroutine zum Aktualisieren der Benutzeroberfläche hier einfügen
	if(m_bEnablePreviousVocableButton)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnPreviousVocableButton() 
{

}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	
    /*CWnd* pWnd = GetFocus();
    if (pWnd != NULL)
		{
			CMDIChildWnd *pChild = MDIGetActive();
			if(pChild)
			{
				EnglishView *pView = (EnglishView *) pChild->GetActiveView();
				CVTransDoc *pDoc = (CVTransDoc *) pChild->GetActiveDocument();
				if(pWnd!=pView)
				{
					//AfxMessageBox("jdsfjs",MB_OK,NULL);
					pView->KillTimer(1);
					pView->m_bDrawCursor=TRUE;
					//pView->Invalidate();
					//pDoc->UpdateAllViews(NULL,0L,NULL);
					//pView->
				}
			}
		}*/
  CWnd * pWnd = GetFocus();
	if(pWnd!=NULL)
	{
		CMDIChildWnd *pChild = MDIGetActive();
		if(pChild)
		{
			EnglishView *pView = (EnglishView *) pChild->GetActiveView();
			if(pView)
			{
				if(pWnd!=pView && typeid(*pWnd)==typeid(CComboBox))
				{
#ifdef _DEBUG
					printf("pWnd!=pView && typeid(*pWnd)==typeid(CComboBox)\n");
					printf("pWnd->GetDlgCtrlID(): %u\n",pWnd->GetDlgCtrlID());
#endif
					for(DWORD dwRun=0;dwRun<pView->m_vecIntPair.size();++dwRun)
					{
#ifdef _DEBUG
						printf("dwRun<pView->m_vecIntPair.size()\n");
						printf("pView->m_vecIntPair.at(dwRun).m_n1: %u\n",pView->
							m_vecIntPair.at(dwRun).m_n1);
#endif
						if(pWnd->GetDlgCtrlID()==pView->m_vecIntPair.at(dwRun).m_n1)
						{
							CComboBox *pComboBox1=(CComboBox *)pWnd;
							ComboBoxListElement * pComboBoxListElementCurrent=pView->
								m_ComboBoxList.m_pFirst;
#ifdef _DEBUG
							printf("pWnd->GetDlgCtrlID()==pView->m_vecIntPair.at(dwRun).m_n1\n");
#endif
							while(pComboBoxListElementCurrent!=NULL)
							{
								if(pComboBoxListElementCurrent->ComboBox.GetDlgCtrlID()==
									pView->m_vecIntPair.at(dwRun).m_n2)
									pComboBoxListElementCurrent->ComboBox.SetCurSel(pComboBox1->
										GetCurSel());
								pComboBoxListElementCurrent=pComboBoxListElementCurrent->
									m_pNext;
							}
						}
						if(pWnd->GetDlgCtrlID()==pView->m_vecIntPair.at(dwRun).m_n2)
						{
							CComboBox *pComboBox1=(CComboBox *)pWnd;
							ComboBoxListElement * pComboBoxListElementCurrent=pView->
								m_ComboBoxList.m_pFirst;
							while(pComboBoxListElementCurrent!=NULL)
							{
								if(pComboBoxListElementCurrent->ComboBox.GetDlgCtrlID()==
									pView->m_vecIntPair.at(dwRun).m_n1)
									pComboBoxListElementCurrent->ComboBox.SetCurSel(pComboBox1->
										GetCurSel());
								pComboBoxListElementCurrent=pComboBoxListElementCurrent->
									m_pNext;
							}
						}
					}
				}
			}
		}
	}
#ifdef _DEBUG
#endif
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}


void CMainFrame::OnUpdateTranslateAutomaticallyIfTextmodificationButton(CCmdUI* pCmdUI) 
{
	// TODO: Code für die Befehlsbehandlungsroutine zum Aktualisieren der Benutzeroberfläche hier einfügen

	//pCmdUI->SetCheck(1);
}

void CMainFrame::OnTranslateAutomaticallyIfTextmodificationButton() 
{ // Funktion muss existieren, damit die Schaltfläche überhaupt aktiviert wird
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen

	//CCmdUI * pCmdUI=AfxGetMenuItem(
	//pCmdUI->SetCheck(!pCmdUI->GetCheck());
	CToolBarCtrl & toolBarCtrl=m_wndToolBar.GetToolBarCtrl();
	toolBarCtrl.EnableButton(ID_TRANSLATE_BUTTON,FALSE);
	//TBBUTTON
	//toolBarCtrl.SetState(ID_TRANSLATE_BUTTON,TBSTATE_INDETERMINATE);
	//toolBarCtrl.CheckButton(ID_EDIT_PASTE,FALSE);
	//toolBarCtrl.Indeterminate(ID_TRANSLATE_BUTTON,TRUE);

}

void CMainFrame::OnExtrasOptionen() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	
	COptionsDialog dlg;
	dlg.m_bAdoptGrammaticallyIncorrectSentences=m_bAdoptGrammaticallyIncorrectSentences;
	dlg.m_bAllowLowLetterAtTheBeginningOfSentence=m_bAllowLowLetterAtTheBeginningOfSentence;
	dlg.m_bBufferOutput=m_bBufferOutput;
	dlg.m_bCaseSensitive=m_bCaseSensitive;
	dlg.m_bEnsureUserWantsDeletion=m_bEnsureUserWantsDeletion;
	dlg.m_bEnsureUserWantsModification=m_bEnsureUserWantsModification;
	dlg.m_bShowClauseProgress=m_bShowClauseProgress;
	dlg.m_bShowSentenceProgress=m_bShowSentenceProgress;
	dlg.m_bShowTextProgress=m_bShowTextProgress;
	dlg.m_bShowPrognosticedRemainingTime=m_bShowPrognosticedRemainingTime;
	dlg.m_bShowGraphicClauseProgress=m_bShowGraphicClauseProgress;
	dlg.m_bShowGraphicSentenceProgress=m_bShowGraphicSentenceProgress;
	dlg.m_bShowGraphicTextProgress=m_bShowGraphicTextProgress;
	dlg.m_bShowNumericClauseProgress=m_bShowNumericClauseProgress;
	dlg.m_bShowNumericSentenceProgress=m_bShowNumericSentenceProgress;
	dlg.m_bShowNumericTextProgress=m_bShowNumericTextProgress;
	dlg.m_dwMaxCharsPerLine=m_dwMaxCharsPerLine;
	dlg.m_nGraphicClauseProgressType=m_nGraphicClauseProgressType;
	dlg.m_nGraphicSentenceProgressType=m_nGraphicSentenceProgressType;
	dlg.m_nGraphicTextProgressType=m_nGraphicTextProgressType;
	dlg.m_nThreadPriority=m_nThreadPriority;
	//dlg.UpdateData(FALSE);
	dlg.DoModal();
	m_bAdoptGrammaticallyIncorrectSentences=dlg.m_bAdoptGrammaticallyIncorrectSentences;
	m_bAllowLowLetterAtTheBeginningOfSentence=dlg.m_bAllowLowLetterAtTheBeginningOfSentence;
/*#ifdef _DEBUG
	CString str;
	str.Format("m_bAdoptGrammaticallyIncorrectSentences: %d",
		m_bAdoptGrammaticallyIncorrectSentences);
	AfxMessageBox(str,MB_OK,NULL);
#endif*/
	m_bBufferOutput=dlg.m_bBufferOutput;
	m_bCaseSensitive=dlg.m_bCaseSensitive;
	m_bEnsureUserWantsDeletion=dlg.m_bEnsureUserWantsDeletion;
	m_bEnsureUserWantsModification=dlg.m_bEnsureUserWantsModification;
	m_bShowClauseProgress=dlg.m_bShowClauseProgress;
	m_bShowSentenceProgress=dlg.m_bShowSentenceProgress;
	m_bShowTextProgress=dlg.m_bShowTextProgress;
	m_bShowPrognosticedRemainingTime=dlg.m_bShowPrognosticedRemainingTime;
	m_bShowGraphicClauseProgress=dlg.m_bShowGraphicClauseProgress;
	m_bShowNumericClauseProgress=dlg.m_bShowNumericClauseProgress;
	m_bShowGraphicSentenceProgress=dlg.m_bShowGraphicSentenceProgress;
	m_bShowNumericSentenceProgress=dlg.m_bShowNumericSentenceProgress;
	m_bShowGraphicTextProgress=dlg.m_bShowGraphicTextProgress;
	m_bShowNumericTextProgress=dlg.m_bShowNumericTextProgress;
	m_dwMaxCharsPerLine=dlg.m_dwMaxCharsPerLine;
	m_nGraphicClauseProgressType=dlg.m_nGraphicClauseProgressType;
	m_nGraphicSentenceProgressType=dlg.m_nGraphicSentenceProgressType;
	m_nGraphicTextProgressType=dlg.m_nGraphicTextProgressType;
	m_nThreadPriority=dlg.m_nThreadPriority;
}

LONG CMainFrame::OnTranslateAutomaticallyFinishes(UINT wParam, LONG lParam)
{
#ifdef _DEBUG
	printf("LONG CMainFrame::OnTranslateAutomaticallyFinishes(UINT wParam, LONG lParam) ANFANG\n");
#endif

	//if(g_ulChar!=-1)
	//{
			LPDWORD lpExitCode=new DWORD;
			*lpExitCode=STILL_ACTIVE;
			/*while(*lpExitCode==STILL_ACTIVE)//wenn der Übersetzungsthread
			{//noch läuft, dann warten, bis der Übersetzungsthread beendet ist
				//MessageBeep((WORD)-1);
				if(g_pMainFrame)
					if(g_pMainFrame->m_pTranslationThread)
					{
						if(!::GetExitCodeThread(g_pMainFrame->m_pTranslationThread->m_hThread,lpExitCode))
							// wenn die Funktion fehlschlägt, liefert ::GetExitCodeThread 
							// FALSE zurück. Wenn man dann nicht aus der Schleife 
							// ausbricht, wird die while-Scheife endlos ausgeführt
						{
#ifdef _DEBUG
							printf("!::GetExitCodeThread(pFrame->m_pTranslationThread->m_hThread,lpExitCode)\n");
#endif
							break;
						}
					}
					else
					{
#ifdef _DEBUG
						printf("!pFrame->m_pTranslationThread\n");
#endif
						break;
					}
#ifdef _DEBUG
				printf("*lpExitCode: %u\n",*lpExitCode);
#endif
			}*/
			/*WaitForSingleObject(g_pMainFrame->m_pTranslationThread->m_hThread,INFINITE);
		g_pText->Insert(*g_pdwCursorPosition++,(char)g_ulChar,0); // Zeichen einfügen mit
		CChildFrame * pChild=(CChildFrame*)g_pMainFrame->MDIGetActive();
		if(pChild)
		{
			EnglishView * pEVGerman=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
			if(pEVGerman)
			{
				std::vector<Range> vecRange;
					TranslateAutomaticallyParameters * pTranslateAutomaticallyParameters=
						new TranslateAutomaticallyParameters(g_pText,vecRange,pEVGerman,
						g_pMainFrame->GetSafeHwnd(),pEVGerman->GetSafeHwnd(),
						g_pMainFrame->m_Progress.m_hWnd,//GetSafeHwnd(),
						g_pMainFrame->m_SentenceProgress.GetSafeHwnd(),
						g_pMainFrame->m_TextProgress.GetSafeHwnd());
					pTranslateAutomaticallyParameters->m_hWndStatusBar=
						g_pMainFrame->m_wndStatusBar.GetSafeHwnd();
					pTranslateAutomaticallyParameters->
						m_bAdoptGrammaticallyIncorrectSentences=g_pMainFrame->
						m_bAdoptGrammaticallyIncorrectSentences;
					pTranslateAutomaticallyParameters->
						m_bAllowLowLetterAtTheBeginningOfSentence=g_pMainFrame->
						m_bAllowLowLetterAtTheBeginningOfSentence;
					pTranslateAutomaticallyParameters->m_bCaseSensitive=
						g_pMainFrame->m_bCaseSensitive;
					pTranslateAutomaticallyParameters->m_dwMaxCharsPerLine=
						g_pMainFrame->m_dwMaxCharsPerLine;
					//TranslateAutomaticallyParameters translateAutomaticallyParameters;
					//translateAutomaticallyParameters.m_hWndProgress=pFrame->m_Progress.m_hWnd;
					g_bContinue=TRUE;
					//pFrame->m_pTranslationThread=AfxBeginThread(TranslateAutomatically,
					//	pTranslateAutomaticallyParameters);
					int nPriority=0;
					switch(g_pMainFrame->m_nThreadPriority)
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
		g_ulChar=-1;
		g_pMainFrame->m_pTranslationThread=AfxBeginThread(TranslateAutomatically,
		pTranslateAutomaticallyParameters,nPriority,0,0,NULL);
			}
		}
	}
	else
	{*/

	//!g_bExecutedOnTranslateAutomaticallyFinishes wird auf TRUE gesetzt, wenn
	//nach der Beendigung des Threades zum Beispiel die Funktion OnChar
	//aufgerufen wurde
	if(!g_bExecutedOnTranslateAutomaticallyFinishes)
	{
		BYTE bComboBoxes=(BYTE)wParam;
#ifdef _DEBUG
		printf("g_bExecutedOnTranslateAutomaticallyFinishes==FALSE\n");
#endif

	m_bMayStartThread=FALSE;
	//AfxMessageBox("ndbfg",MB_OK,NULL);
	CToolBarCtrl & toolBarCtrl=m_wndToolBar.GetToolBarCtrl();
	CChildFrame * pChild=(CChildFrame*)MDIGetActive();
	if(pChild)
	{
		//EnglishView * pView=(EnglishView *) pChild->GetActiveView();
		EnglishView * pEVGerman=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
		if(pEVGerman)
		{
			if(bComboBoxes)
				pEVGerman->m_bDrawVector=TRUE;
			else
			{
/*		DWORD dwCharsPerLine=0;
		DWORD dwPos=0;
			for(DWORD dwRun=0;dwRun<pEVGerman->m_cstrvv.size();dwRun++)
			{
				//dwPos+=pEV->m_cstrvv.at(dwRun).GetLength();
				CStringVector cstrv=pEVGerman->m_cstrvv.at(dwRun);
				CString str=cstrv.at(0);
				if(str!=","  && str!="." && dwRun>0)
				{
					if(dwCharsPerLine+1<=m_dwMaxCharsPerLine)
					{
						//if(!bFirstNode)
						//	pEV->m_pText->Insert(dwPos,"\r\n",0);
						//dwPos+=2;
						//pEV->m_pText->Insert(dwPos,str.GetBuffer(str.GetLength()),0);
						//dwCharsPerLine=0;
						pEVGerman->m_pText->Insert(dwPos,' ',0);
						dwPos++;
					}
				}
				if(dwCharsPerLine+str.GetLength()>m_dwMaxCharsPerLine)
				{
					pEVGerman->m_pText->Insert(dwPos,"\r\n",0);
					dwPos+=2;
					//pEV->m_pText->Insert(dwPos,str.GetBuffer(str.GetLength()),0);
					dwCharsPerLine=0;
				}
				//else
					pEVGerman->m_pText->Insert(dwPos,str.GetBuffer(str.GetLength()),0);
				dwPos+=str.GetLength();
				dwCharsPerLine+=str.GetLength();
			}*/
				pEVGerman->m_bDrawVector=FALSE;
			}
			//LPDWORD lpExitCode=new DWORD;
			//*lpExitCode=STILL_ACTIVE;
			/*while(*lpExitCode==STILL_ACTIVE)
			{
				//MessageBeep((WORD)-1);
				if(m_pTranslationThread)
				{
					if(!::GetExitCodeThread(m_pTranslationThread->m_hThread,lpExitCode))
						// wenn die Funktion fehlschlägt, liefert ::GetExitCodeThread 
						// FALSE zurück. Wenn man dann nicht aus der Schleife 
						// ausbricht, wird die while-Scheife endlos ausgeführt
						break;
					MessageBeep((WORD)-1);
				}
				else
					break;
			}*/
			//AfxMessageBox("vor clear",MB_OK,NULL);
			pEVGerman->m_ComboBoxList.Clear(); // zuerst alle ComboBoxen 
			// (Kombinationfelder) entfernen
			pEVGerman->CreateComboBoxes(); // Kombinationsfelder erzeugen
			if(pEVGerman->m_ComboBoxList.m_pFirst)
			{
				toolBarCtrl.CheckButton(ID_COMBOBOX_BUTTON,TRUE);
			}
			pEVGerman->Invalidate();
		}
	}
	if(KillTimer(1)==FALSE)
	{
#ifdef _DEBUG
		printf("KillTimer(1)==FALSE\n");
#endif
	}
	//if(m_bShowClauseProgress)
	//{
		if(m_bShowGraphicClauseProgress)
			m_Progress.DestroyWindow();
		if(m_bShowGraphicClauseProgress)
			m_cStaticClauseProgress.DestroyWindow();
	//}
	//if(m_bShowSentenceProgress)
	//{
		if(m_bShowGraphicSentenceProgress)
			m_SentenceProgress.DestroyWindow();
		if(m_bShowNumericSentenceProgress)
			m_cStaticSentenceProgress.DestroyWindow();
	//}
	//if(m_bShowTextProgress)
	//{
		if(m_bShowGraphicTextProgress)
			m_TextProgress.DestroyWindow();
		if(m_bShowNumericTextProgress)
			m_cStaticTextProgress.DestroyWindow();
	//}
	if(m_bShowPrognosticedRemainingTime)
		m_cStaticRemainingTime.DestroyWindow();
	m_bCreated=FALSE;
	UINT IDArray[]={ID_SEPARATOR,ID_INDICATOR_CAPS,ID_INDICATOR_NUM,ID_INDICATOR_SCRL};
	m_wndStatusBar.SetIndicators(IDArray,sizeof(IDArray)/sizeof(UINT));
	m_bMayStartThread=TRUE;
	g_bExecutedOnTranslateAutomaticallyFinishes=TRUE;
	toolBarCtrl.HideButton(ID_TRANSLATE_BUTTON,FALSE);
	char buffer[20];
	CTime tCurrent=CTime::GetCurrentTime();
	CTimeSpan ts(tCurrent.GetTime());
	ts-=CTimeSpan(tAtTheBeginningOfTheTranslation.GetTime());
//Um die Warnungen über veraltete C-Bibliotheken zu deaktivieren
#pragma warning disable 4996
	_ultoa(ts.GetTotalSeconds(),buffer,10);
	CString strAmount(buffer);
	m_wndStatusBar.SetWindowText(_T("Übersetzung in "+strAmount+" Sekunde(n)"));
	//g_eventEndThread.SetEvent();
	}
#ifdef _DEBUG
	else
		printf("g_bExecutedOnTranslateAutomaticallyFinishes==TRUE\n");
#endif
	//}
#ifdef _DEBUG
	printf("LONG CMainFrame::OnTranslateAutomaticallyFinishes(UINT wParam, LONG lParam) ENDE\n");
#endif
	return 0;
}

void CMainFrame::OnUpdateDeleteVocableButton(CCmdUI* pCmdUI) 
{
	// TODO: Code für die Befehlsbehandlungsroutine zum Aktualisieren der Benutzeroberfläche hier einfügen

	if(pWordNodeCurrent)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen
	
	//RECT rc;
	//m_wndStatusBar.GetItemRect(1, &rc);
	// Reposition the progress control correctly!
	//m_Progress.SetWindowPos(&wndTop, rc.left, rc.top, rc.right - rc.left,
	//	rc.bottom - rc.top, 0);
	if(m_bCreated)
	{
		RECT MyRect;
		BYTE bIDCount=0;
		std::vector<UINT> vecUINT;
		//Reposition
		if(m_bShowGraphicClauseProgress)
		// vor dem Erneuten Erstellen zuerst zerstören
			m_Progress.DestroyWindow();
		if(m_bShowNumericClauseProgress)
		// vor dem Erneuten Erstellen zuerst zerstören
			m_cStaticClauseProgress.DestroyWindow();
		if(m_bShowGraphicSentenceProgress)
		// vor dem Erneuten Erstellen zuerst zerstören
			m_SentenceProgress.DestroyWindow();
		if(m_bShowNumericSentenceProgress)
		// vor dem Erneuten Erstellen zuerst zerstören
			m_cStaticSentenceProgress.DestroyWindow();
		if(m_bShowGraphicTextProgress)
		// vor dem Erneuten Erstellen zuerst zerstören
			m_TextProgress.DestroyWindow();
		if(m_bShowNumericTextProgress)
		// vor dem Erneuten Erstellen zuerst zerstören
			m_cStaticTextProgress.DestroyWindow();
		if(m_bShowPrognosticedRemainingTime)
		// vor dem Erneuten Erstellen zuerst zerstören
			m_cStaticRemainingTime.DestroyWindow();
		//if(m_bShowClauseProgress)
		//{
			if(m_bShowNumericClauseProgress)
			{
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE);
			}
			if(m_bShowGraphicClauseProgress)
			{
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
			}
		//}
		//if(m_bShowSentenceProgress)
		//{
			if(m_bShowNumericSentenceProgress)
			{
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE);
			}
			if(m_bShowGraphicSentenceProgress)
			{
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
			}
		//}
		//if(m_bShowTextProgress)
		//{
			if(m_bShowNumericTextProgress)
			{
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE);
			}
			if(m_bShowGraphicTextProgress)
			{
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
			}
		//}
		if(m_bShowPrognosticedRemainingTime)
		{
			bIDCount++;
			vecUINT.push_back(ID_INDICATOR_REMAINING_TIME);
		}
		if(vecUINT.size()>0)
		{
			UINT * lpIDArray=new UINT[bIDCount];
			for(BYTE bRun=0;bRun<vecUINT.size();bRun++)
				lpIDArray[bRun]=vecUINT.at(bRun);
			m_wndStatusBar.SetIndicators(lpIDArray,sizeof(lpIDArray)/sizeof(UINT));
		}
		CStatusBarCtrl & statusbarCtrl=m_wndStatusBar.GetStatusBarCtrl();
		CRect rectStatusBarClientArea;
		m_wndStatusBar.GetClientRect(rectStatusBarClientArea);
		if(bIDCount>0)
		{
			if(m_bShowPrognosticedRemainingTime)
			{
				int * Widths=new int[bIDCount];
				for(BYTE bRun=0;bRun<bIDCount;bRun++)
					Widths[bRun]=rectStatusBarClientArea.right*(bRun+1)/bIDCount;
				statusbarCtrl.SetParts(bIDCount,Widths);
			}
			else
			{
				int * Widths=new int[bIDCount];
				for(BYTE bRun=0;bRun<bIDCount;bRun++)
					Widths[bRun]=rectStatusBarClientArea.right*(bRun+1)/bIDCount;
				statusbarCtrl.SetParts(bIDCount,Widths);
			}
		}
		bIDCount=0;
		if(m_bShowNumericClauseProgress)
		{
			m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
			m_cStaticClauseProgress.Create("",WS_VISIBLE|WS_CHILD,
				MyRect,&m_wndStatusBar,ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE);
		}
		if(m_bShowGraphicClauseProgress)
		{
			m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
			if(m_nGraphicClauseProgressType==0)
				m_Progress.Create(WS_VISIBLE|WS_CHILD,MyRect,
					&m_wndStatusBar,ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
			if(m_nGraphicClauseProgressType==1)
				m_Progress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
					&m_wndStatusBar,ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
			m_Progress.SetRange(0,100); //Set the range to between 0 and 100
			m_Progress.SetStep(1); // Set the step amount
		}
#ifdef _DEBUG
		printf("m_bShowSentenceProgress\n");
#endif
		if(m_bShowNumericSentenceProgress)
		{
			m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
			m_cStaticSentenceProgress.Create("",WS_VISIBLE|WS_CHILD,
				MyRect,&m_wndStatusBar,ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE);
		}
		if(m_bShowGraphicSentenceProgress)
		{
			m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
			if(m_nGraphicSentenceProgressType==0)
				m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD,MyRect,
					&m_wndStatusBar,ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
			if(m_nGraphicSentenceProgressType==1)
				m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
					&m_wndStatusBar,ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
			m_SentenceProgress.SetRange(0,100); //Set the range to between 0 and 100
			m_SentenceProgress.SetStep(1); // Set the step amount
		}
		if(m_bShowNumericTextProgress)
		{
			m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
			m_cStaticTextProgress.Create("",WS_VISIBLE|WS_CHILD,
			MyRect,&m_wndStatusBar,ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE);
		}
		if(m_bShowGraphicTextProgress)
		{
			m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
			if(m_nGraphicTextProgressType==0)
				m_TextProgress.Create(WS_VISIBLE|WS_CHILD,MyRect,
					&m_wndStatusBar,ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
			if(m_nGraphicTextProgressType==1)
				m_TextProgress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
					&m_wndStatusBar,ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
			m_TextProgress.SetRange(0,100); //Set the range to between 0 and 100
			m_TextProgress.SetStep(1); // Set the step amount
		}
		if(m_bShowPrognosticedRemainingTime)
		{
			m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
			m_cStaticRemainingTime.Create("",WS_VISIBLE|WS_CHILD,MyRect,
				&m_wndStatusBar,ID_INDICATOR_REMAINING_TIME);
			tAtTheBeginningOfTheTranslation=(CTime)CTime::GetCurrentTime();
		}
	}
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen und/oder Standard aufrufen
	
#ifdef _DEBUG
	printf("void CMainFrame::OnTimer(UINT nIDEvent) ANFANG\n");
#endif

	//MessageBeep((WORD)-1);
	//MessageBox("hshd","hhh",MB_OK);
	if(m_bShowPrognosticedRemainingTime)
	{
		char buffer[20]="";
		CTime tCurrent=CTime::GetCurrentTime();
		CTimeSpan ts(tCurrent.GetTime());
		ts-=CTimeSpan(tAtTheBeginningOfTheTranslation.GetTime());
		CString * strAmount;
		RECT MyRect;
		if(m_bCreated)
		{
			//if(m_bShowClauseProgress)
			//	m_Progress.DestroyWindow();
			//if(m_bShowSentenceProgress)
			//	m_SentenceProgress.DestroyWindow();
			//if(m_bShowTextProgress)
			//	m_TextProgress.DestroyWindow();
			//m_cStaticRemainingTime.DestroyWindow();
			m_bCreated=FALSE;
		}
		if(m_bCreated == FALSE)
		{
			/*BYTE bIDCount=0;//3;
			std::vector<UINT> vecUINT;
			if(m_bShowClauseProgress)
			{
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_CLAUSE_PROGRESS_PANE);
			}
			if(m_bShowSentenceProgress)
			{
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_SENTENCE_PROGRESS_PANE);
			}
			if(m_bShowTextProgress)
			{
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_TEXT_PROGRESS_PANE);
			}
			if(m_bShowPrognosticedRemainingTime)
			{
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
				m_wndStatusBar.SetIndicators(lpIDArray,sizeof(lpIDArray)/sizeof(UINT));
			}
			CStatusBarCtrl & statusbarCtrl=m_wndStatusBar.GetStatusBarCtrl();
			LPRECT lpRect;
			CRect rect2;
			m_wndStatusBar.GetClientRect(rect2);
			if(bIDCount>0)
			{
				int * Widths=new int[bIDCount];
				//Widths[bIDCount-1]=50;//m_strAmount
				for(BYTE bRun=0;bRun<bIDCount;bRun++)
					Widths[bRun]=(rect2.right)*(bRun+1)/bIDCount;
				statusbarCtrl.SetParts(bIDCount,Widths);
			}
			if(m_bShowClauseProgress)
			{
				m_wndStatusBar.GetItemRect(0, &MyRect);
				m_Progress.Create(WS_VISIBLE|WS_CHILD, MyRect, &m_wndStatusBar,IDC_INDICATOR_CLAUSE_PROGRESS_PANE);
				m_Progress.SetRange(0,100); //Set the range to between 0 and 100
				m_Progress.SetStep(1); // Set the step amount
			}
			if(m_bShowSentenceProgress)
			{
				m_wndStatusBar.GetItemRect(1, &MyRect);
				m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD, MyRect, &m_wndStatusBar,ID_INDICATOR_SENTENCE_PROGRESS_PANE);
				m_SentenceProgress.SetRange(0,100); //Set the range to between 0 and 100
				m_SentenceProgress.SetStep(1); // Set the step amount
			}
			if(m_bShowTextProgress)
			{
				m_wndStatusBar.GetItemRect(2, &MyRect);
				m_TextProgress.Create(WS_VISIBLE|WS_CHILD, MyRect, &m_wndStatusBar,ID_INDICATOR_TEXT_PROGRESS_PANE);
				m_TextProgress.SetRange(0,100); //Set the range to between 0 and 100
				m_TextProgress.SetStep(1); // Set the step amount
			}
			//m_cStaticRemainingTime.UpdateData(TRUE);
			LPCTSTR str="Fortschritt des Haupt- oder Nebensatzes";
			//statusbarCtrl.SetTipText(0,str);
			//statusbarCtrl.SetTipText(1,_T("Fortschritt des aktuellen Satzes"));
			//statusbarCtrl.SetTipText(2,_T("Fortschritt des ganzen Textes"));*/
			m_bCreated = TRUE;
		}
		if((double)g_dwTokensInSentence>0) // Division durch Null vermeiden
		{ // (ergibt sonst einen Laufzeitfehler)
			if((double)((double)((double)g_dwTranslated+(double)g_dwStatus)/
				(double)g_dwTokensInSentence)>0.0) // Division durch Null vermeiden
			{ // (ergibt sonst einen Laufzeitfehler)
				//MessageBeep((WORD)-1);
				//_ultoa((double)ts.GetTotalSeconds()/(double)((double)(g_dwTran slated+g_dwStatus)/(double)g_dwTotal)-ts.GetTotalSeconds(),buffer,10);
				//_ultoa(,buffer,10);
				//_gcvt((double)ts.GetTotalSeconds()/(double)1.0-((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal)-ts.GetTotalSeconds(),3,buffer);
				//_gcvt((double)((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal),7,buffer);
				if(m_bShowClauseProgress)
					if(m_bShowGraphicClauseProgress)
					{
						CProgressCtrl *pPC=(CProgressCtrl*) m_wndStatusBar.GetDlgItem(ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);//,&m_wndStatusBar.m_hWnd);
#ifdef _DEBUG
						printf("vor: pPC->SetPos((double)((double)g_dwStatus/(double)g_dwTotal)*100.0);\n");
#endif
						pPC->SetPos((double)((double)g_dwStatus/(double)g_dwTotal)*100.0);
#ifdef _DEBUG
						printf("nach: pPC->SetPos((double)((double)g_dwStatus/(double)g_dwTotal)*100.0);\n");
#endif
					}
				//_gcvt((double)(g_dwStatus),7,buffer);
				//_ultoa(g_dwStatus,buffer,10);
				//printf("ts.GetTotalSeconds(): %u\n",ts.GetTotalSeconds());
				//printf("
				//printf("((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal): %u\n",((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal));
				//UpdateData(FALSE);
			}
			if(m_bShowSentenceProgress)
				if(m_bShowGraphicSentenceProgress)
				{
					CProgressCtrl *pPC=(CProgressCtrl*) m_wndStatusBar.GetDlgItem(ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
					pPC->SetPos((double)g_dwSentenceStatus/(double)g_dwTokensInSentence*100.0);
					//pPC->SetPos((double)((double)g_dwStatus/(double)g_dwTotal)*100.0);
				}
		}
#ifdef _DEBUG
		printf("g_dwTotal: %u\n",g_dwTotal);
#endif
		//MessageBeep((WORD)-1);
		if(g_dwTotal>0)
		{
			//MessageBeep((WORD)-1);
			if((double)((double)((double)g_dwTranslated+(double)g_dwStatus)/
				(double)g_dwTotal)>0.0)
			{
				//_gcvt((double)ts.GetTotalSeconds()/(double)((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal)-(double)ts.GetTotalSeconds(),0,buffer);
				if(m_bShowTextProgress)
					if(m_bShowGraphicTextProgress)
					{
						CProgressCtrl * pPC=(CProgressCtrl*)m_wndStatusBar.
							GetDlgItem(ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
						pPC->SetPos((double)((double)(g_dwTranslated+g_dwSentenceStatus)/
							(double)g_dwTotal)*100.0);
					}
				double dTotalSeconds=(double)ts.GetTotalSeconds();
				double dRatio=(double)((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal);
				//_ultoa(ceil((double)(ts.GetTotalSeconds()/(double)((g_dwTranslated+g_dwStatus)/g_dwTotal)))-ts.GetTotalSeconds(),buffer,10);
				_ultoa(ceil(dTotalSeconds/dRatio)-dTotalSeconds,buffer,10);
				//_ultoa(33,buffer,10);
				strAmount=new CString(buffer);
				//UpdateData(FALSE);
			}
		}
		//const LPCTSTR lpsz="dd";//=new LPCTSTR("ksdchfgjs");//_T("jjjk");
		//lpszNewText="ksdchfgjs";
		if(m_bCreated==TRUE)
		{
			//char buffer[256];
			//m_wndStatusBar.SetPaneText(3,buffer);
			/*CStatusBar* pStatus = (CStatusBar*) 
			GetDescendantWindow(AFX_IDW_STATUS_BAR);
			if (pStatus != NULL)
			{
				pStatus->SetPaneText(3, buffer);
				pStatus->UpdateWindow();
			}*/

			//m_wndStatusBar.SetPaneText(m_wndStatusBar.
			//	CommandToIndex(ID_INDICATOR_REMAINING_TIME),lpsz,TRUE);//*strAmount,TRUE);
			//CStatusBarCtrl & cStatusBarCtrl=m_wndStatusBar.GetStatusBarCtrl();
			//m_wndStatusBar.GetItemRect(3, &MyRect);
			//m_cStaticRemainingTime.Create(*strAmount,WS_VISIBLE|WS_CHILD,MyRect,&m_wndStatusBar,ID_INDICATOR_REMAINING_TIME);
			m_wndStatusBar.SetDlgItemText(ID_INDICATOR_REMAINING_TIME,*strAmount);
			//const CString str="dflgk";
			//cStatusBarCtrl.SetText(lpsz,2,0);
		}
		CDC * pDC=GetDC();
		CFont * pFont=m_cStaticRemainingTime.GetFont();
		CFont * pOldFont=pDC->SelectObject(pFont);
		CString str;
		CSize sizeClauseProgress;
		CSize sizeSentenceProgress;
		CSize sizeTextProgress;
		CSize sizeRemainingTime;

		//_ultoa(33,buffer,10);
		//strAmount=new CString(buffer);
		BYTE bIDCount=0;//3;
		BYTE bMainParts=0;
		std::vector<UINT> vecUINT;
		// Teilsatzfortschritt
		//if(m_bShowClauseProgress)
		//{
			if(m_bShowNumericClauseProgress)
			{
				_ultoa(g_dwStatus,buffer,10);
				str+=CString(buffer)+"/";
				_ultoa(g_dwTokensInSentence,buffer,10);
				str+=buffer;
				sizeClauseProgress=pDC->GetTextExtent(str);
				m_wndStatusBar.SetDlgItemText(ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE,str);
				str="";
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE);
				//pWidths[
			}
			if(m_bShowGraphicClauseProgress)
			{
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
			}
			if(m_bShowNumericClauseProgress || m_bShowGraphicClauseProgress)
				bMainParts++;
		//}
		// Satzfortschritt
		//if(m_bShowSentenceProgress)
		//{
			if(m_bShowNumericSentenceProgress)
			{
				_ultoa(g_dwSentenceStatus,buffer,10);
				str+=CString(buffer)+"/";
				_ultoa(g_dwTokensInSentence,buffer,10);
				str+=CString(buffer);
				sizeSentenceProgress=pDC->GetTextExtent(str);
				m_wndStatusBar.SetDlgItemText(ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE,str);
				str="";
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE);
			}
			if(m_bShowGraphicSentenceProgress)
			{
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
			}
			if(m_bShowNumericSentenceProgress || m_bShowGraphicSentenceProgress)
				bMainParts++;
		//}
		// Textfortschritt
		//if(m_bShowTextProgress)
		//{
			if(m_bShowNumericTextProgress)
			{
				_ultoa(g_dwTranslated+g_dwStatus,buffer,10);
				str+=CString(buffer)+"/";
				_ultoa(g_dwTotal,buffer,10);
				str+=CString(buffer);
				sizeTextProgress=pDC->GetTextExtent(str);
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE);
				m_wndStatusBar.SetDlgItemText(ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE,str);
			}
			if(m_bShowGraphicTextProgress)
			{
				bIDCount++;
				vecUINT.push_back(ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
			}
			if(m_bShowNumericTextProgress || m_bShowGraphicTextProgress)
				bMainParts++;
		//}
		// prognostizierte Zeit in Sekunden
		if(m_bShowPrognosticedRemainingTime)
		{
			bIDCount++;
			vecUINT.push_back(ID_INDICATOR_REMAINING_TIME);
			sizeRemainingTime=pDC->GetTextExtent(*strAmount);
		}
		int * pWidths=new int[bIDCount];
		pDC->SelectObject(pOldFont);
		CRect rectClient;
		m_wndStatusBar.GetClientRect(rectClient);
		sizeRemainingTime.cx+=20;//+20 Pixel für den "Griff" auf der rechten Seite
		//der Statuszeile, mit dem man die Größe des Fensters verändern kann
		BYTE bRun=0;
		BYTE bMainPartIndex=0;
		//if(m_bShowClauseProgress)
		//{
			//wenn beide Fortschrittsmodi (numersich oder grafisch)
			//angezeigt werden sollen
			if(m_bShowGraphicClauseProgress && m_bShowNumericClauseProgress)
			{
				pWidths[bRun]=(rectClient.right-sizeRemainingTime.cx)*bMainPartIndex/(bMainParts)+
					sizeClauseProgress.cx;
#ifdef _DEBUG
		printf("pWidths[%u]: %u\n",bRun,pWidths[bRun]);
#endif
				bRun++;
				pWidths[bRun]=(rectClient.right-sizeRemainingTime.cx)*(bMainPartIndex+1)/(bMainParts);
#ifdef _DEBUG
		printf("pWidths[%u]: %u\n",bRun,pWidths[bRun]);
#endif
				bRun++;
				bMainPartIndex++;
			}
			else
			{
				//wenn wenigstens ein Fortschrittsmodus (numersich oder grafisch)
				//angezeigt werden soll
				if(m_bShowGraphicClauseProgress || m_bShowNumericClauseProgress)
				{
					pWidths[bRun]=(rectClient.right-sizeRemainingTime.cx)*
						(bMainPartIndex+1)/(bMainParts);
					CString str;
					str.Format("pWidths[bRun]: %u\n",pWidths[bRun]);
					//AfxMessageBox(str,MB_OK,NULL);
					bRun++;
					bMainPartIndex++;

				}
			}
		//}
		//if(m_bShowSentenceProgress)
		//{
			//wenn beide Fortschrittsmodi (numersich oder grafisch)
			//angezeigt werden sollen
			if(m_bShowGraphicSentenceProgress && m_bShowNumericSentenceProgress)
			{
				pWidths[bRun]=(rectClient.right-sizeRemainingTime.cx)*
					bMainPartIndex/(bMainParts)+sizeSentenceProgress.cx;
#ifdef _DEBUG
		printf("pWidths[%u]: %u\n",bRun,pWidths[bRun]);
#endif
				bRun++;
				pWidths[bRun]=(rectClient.right-sizeRemainingTime.cx)*
					(bMainPartIndex+1)/(bMainParts);
#ifdef _DEBUG
		printf("pWidths[%u]: %u\n",bRun,pWidths[bRun]);
#endif
				bRun++;
				bMainPartIndex++;
			}
			else
			{
				//wenn wenigstens ein Fortschrittsmodus (numersich oder grafisch)
				//angezeigt werden soll
				if(m_bShowGraphicSentenceProgress || m_bShowNumericSentenceProgress)
				{
					pWidths[bRun]=(rectClient.right-sizeRemainingTime.cx)*
						(bMainPartIndex+1)/(bMainParts);
					bRun++;
					bMainPartIndex++;
				}
			}
		//}
		//if(m_bShowTextProgress)
		//{
			//wenn beide Fortschrittsmodi (numersich oder grafisch)
			//angezeigt werden sollen
			if(m_bShowGraphicTextProgress && m_bShowNumericTextProgress)
			{
				//Breite des Clientbereichs der Statusleiste minus der Breite
				//für die verbleibende Zeit mal den Index des Fortschritts 
				//(Haupt-/Nebensatz,Satz,ganzer Text) durch die Anzahl der
				//Fortschritte(Haupt-/Nebensatz,Satz,ganzer Text) plus der
				//Breite des numerischen Textfortschritts
				//Beispiel: Breite der Statuszeile: 500 Pixel (rectClient.right)
				//  Breite der verbeleibenden Zeit: 20 Pixel (sizeRemainingTime.cx)
				//  Index des Fortschritts: 2 (bMainPartIndex)
				//  Anzahl der Fortschritte: 3 (bMainParts)
				//  Breite des numereischen Textfortschritts: 20 (sizeTextProgress.cx)
				// Ergebnis: (500-20)*2/3+20==340 (pWidths[bRun])
				pWidths[bRun]=(rectClient.right-sizeRemainingTime.cx)*bMainPartIndex/
					(bMainParts)+sizeTextProgress.cx;
#ifdef _DEBUG
		printf("pWidths[%u]: %u\n",bRun,pWidths[bRun]);
#endif
				bRun++;
				//Beispiel: Breite der Statuszeile: 500 Pixel (rectClient.right)
				//  Breite der verbeleibenden Zeit: 20 Pixel (sizeRemainingTime.cx)
				//  Index des Fortschritts: 2 (bMainPartIndex)
				//  Anzahl der Fortschritte: 3 (bMainParts)
				// Ergebnis: (500-20)*3/3==480 (pWidths[bRun])
				pWidths[bRun]=(rectClient.right-sizeRemainingTime.cx)*(bMainPartIndex+1)/
					(bMainParts);
#ifdef _DEBUG
		printf("pWidths[%u]: %u\n",bRun,pWidths[bRun]);
#endif
				bRun++;
				bMainPartIndex++;
			}
			else
			{
				//wenn wenigstens ein Fortschrittsmodus (numersich oder grafisch)
				//angezeigt werden soll
				if(m_bShowGraphicTextProgress || m_bShowNumericTextProgress)
				{
					pWidths[bRun]=(rectClient.right-sizeRemainingTime.cx)*(bMainPartIndex+1)/
						(bMainParts);
#ifdef _DEBUG
					printf("pWidths[%u]: %u\n",bRun,pWidths[bRun]);
#endif
					bRun++;
					bMainPartIndex++;
				}
			}
		//}
		/*for(BYTE bRun=0;bRun<bIDCount-1;bRun++)
			Widths[bRun]=(rectClient.right-size.cx)*(bRun+1)/(bIDCount-1);*/
		pWidths[bIDCount-1]=rectClient.right;
		//Widths[0]=50;
		//Widths[1]=100;
		//Widths[2]=150;
		//Widths[3]=-1;
			//int Widths []={rect2.right/3,rect2.right*2/3,rect2.right};
		CStatusBarCtrl & statusbarCtrl=m_wndStatusBar.GetStatusBarCtrl();
		statusbarCtrl.SetParts(bIDCount,pWidths);
		//RECT MyRect;
#ifdef _DEBUG
		printf("bIDCount: %u\n",bIDCount);
#endif
		bIDCount=0;
		//if(m_bShowClauseProgress)
		//{
			if(m_bShowGraphicClauseProgress)
			{
				m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
				CString str;
				str.Format("MyRect.left: %u MyRect.right: %u\n",MyRect.left,MyRect.right);
				//AfxMessageBox(str,MB_OK,NULL);
				//MyRect.right=100;
				m_cStaticClauseProgress.MoveWindow(&MyRect,FALSE);
			}
			if(m_bShowNumericClauseProgress)
			{
				m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
				m_Progress.MoveWindow(&MyRect,FALSE);
			}
		//}
		//if(m_bShowSentenceProgress)
		//{
			if(m_bShowGraphicSentenceProgress)
			{
				m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
				m_cStaticSentenceProgress.MoveWindow(&MyRect,FALSE);
			}
			if(m_bShowNumericSentenceProgress)
			{
				m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
				m_SentenceProgress.MoveWindow(&MyRect,FALSE);
			}
		//}
		//if(m_bShowTextProgress)
		//{
			if(m_bShowGraphicTextProgress)
			{
				m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
				m_cStaticTextProgress.MoveWindow(&MyRect,FALSE);
			}
			if(m_bShowNumericTextProgress)
			{
				m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
				m_TextProgress.MoveWindow(&MyRect,FALSE);
			}
		//}
		if(m_bShowPrognosticedRemainingTime)
		{
			m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
			m_cStaticRemainingTime.MoveWindow(&MyRect,FALSE);
		}

		//m_wndStatusBar.RepositionBars(ID_INDICATOR_CLAUSE_PROGRESS_PANE,
		//	ID_INDICATOR_REMAINING_TIME,NULL,CWnd::reposDefault,NULL,NULL,FALSE);
	}
	else
	{
		if((double)g_dwTokensInSentence>0)
		{
			if((double)((double)((double)g_dwTranslated+(double)g_dwStatus)/(double)g_dwTokensInSentence)>0.0)
			{
				//MessageBeep((WORD)-1);
				//_ultoa((double)ts.GetTotalSeconds()/(double)((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal)-ts.GetTotalSeconds(),buffer,10);
				//_ultoa(,buffer,10);
				//_gcvt((double)ts.GetTotalSeconds()/(double)1.0-((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal)-ts.GetTotalSeconds(),3,buffer);
				//_gcvt((double)((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal),7,buffer);
				if(m_bShowClauseProgress)
				{
					CProgressCtrl *pPC=(CProgressCtrl*) m_wndStatusBar.GetDlgItem(IDC_INDICATOR_CLAUSE_PROGRESS_PANE);//,&m_wndStatusBar.m_hWnd);
#ifdef _DEBUG
					printf("vor: pPC->SetPos((double)((double)g_dwStatus/(double)g_dwTotal)*100.0);\n");
#endif
					pPC->SetPos((double)((double)g_dwStatus/(double)g_dwTotal)*100.0);
#ifdef _DEBUG
					printf("nach: pPC->SetPos((double)((double)g_dwStatus/(double)g_dwTotal)*100.0);\n");
#endif
				}
				//_gcvt((double)(g_dwStatus),7,buffer);
				//_ultoa(g_dwStatus,buffer,10);
				//printf("ts.GetTotalSeconds(): %u\n",ts.GetTotalSeconds());
				//printf("
				//printf("((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal): %u\n",((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal));
				//UpdateData(FALSE);
			}
			if(m_bShowSentenceProgress)
			{
				CProgressCtrl *pPC=(CProgressCtrl*) m_wndStatusBar.GetDlgItem(ID_INDICATOR_SENTENCE_PROGRESS_PANE);
				pPC->SetPos((double)g_dwSentenceStatus/(double)g_dwTokensInSentence*100.0);
			//pPC->SetPos((double)((double)g_dwStatus/(double)g_dwTotal)*100.0);
			}
		}
#ifdef _DEBUG
		printf("g_dwTotal: %u\n",g_dwTotal);
#endif
		//MessageBeep((WORD)-1);
		if(g_dwTotal>0)
		{
			//MessageBeep((WORD)-1);
			if((double)((double)((double)g_dwTranslated+(double)g_dwStatus)/(double)g_dwTotal)>0.0)
			{
				//_gcvt((double)ts.GetTotalSeconds()/(double)((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal)-(double)ts.GetTotalSeconds(),0,buffer);
				if(m_bShowTextProgress)
				{
					CProgressCtrl * pPC=(CProgressCtrl*)m_wndStatusBar.GetDlgItem(ID_INDICATOR_TEXT_PROGRESS_PANE);
					pPC->SetPos((double)((double)(g_dwTranslated+g_dwSentenceStatus)/(double)g_dwTotal)*100.0);
				}
				//UpdateData(FALSE);
			}
		}
	}
	CMDIFrameWnd::OnTimer(nIDEvent);
#ifdef _DEBUG
	printf("void CMainFrame::OnTimer(UINT nIDEvent) ENDE\n");
#endif
}

void CMainFrame::OnEditCut() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen

	HANDLE hText = NULL;
	CChildFrame * pChild=(CChildFrame*)MDIGetActive();
	CWnd* pWnd = GetFocus();
	if(pChild && pWnd)
	{
		EnglishView * pEV=(EnglishView*)pChild->GetActiveView();
		if(pEV)
		{
			if(pEV->m_bMark && OpenClipboard())
			{
				DWORD dwMarkingStart=pEV->m_dwMarkingStart<pEV->m_dwMarkingEnd?pEV->m_dwMarkingStart:pEV->m_dwMarkingEnd;
				DWORD dwMarkingEnd=pEV->m_dwMarkingStart>pEV->m_dwMarkingEnd?pEV->m_dwMarkingStart:pEV->m_dwMarkingEnd;
				CString str;

				/* Allocate memory and copy the string to it */
				HGLOBAL clipbuffer;
        char * buffer;
				if(m_pTranslationThread)
				{
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
					WaitForSingleObject(m_pTranslationThread->m_hThread,INFINITE);
				}
				pEV->m_pText->GetCString(str,dwMarkingStart,dwMarkingEnd-1);
        EmptyClipboard();
        clipbuffer = GlobalAlloc(GMEM_DDESHARE, str.GetLength()+1);
        buffer = (char*)GlobalLock(clipbuffer);
        strcpy(buffer, LPCSTR(str));
        GlobalUnlock(clipbuffer);
        SetClipboardData(CF_TEXT,clipbuffer);
				CloseClipboard();
				GlobalFree(clipbuffer);
				//dwMarkingStart=pEV->m_dwMarkingStart<pEV->m_dwMarkingEnd?pEV->m_dwMarkingStart:pEV->m_dwMarkingEnd;
				//dwMarkingEnd=pEV->m_dwMarkingStart>pEV->m_dwMarkingEnd?pEV->m_dwMarkingStart:pEV->m_dwMarkingEnd;
#ifdef _DEBUG
				printf("dwMarkingStart: %u  dwMarkingEnd-dwMarkingStart:%u\n",dwMarkingStart,dwMarkingEnd-dwMarkingStart);
#endif
				pEV->m_pText->Delete(dwMarkingStart,dwMarkingEnd-dwMarkingStart);
				pEV->m_bMark=FALSE;
				pEV->m_CursorPos=dwMarkingStart;
				//die Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
				//wird nicht direkt nach der Funktion TranslateAutomatically()
				//ausgeführt, wenn die Funktion EnglishView::OnChar() vor 
				//der Beendigung der Funktion TranslateAutomatically() ausgeführt
				//wird.
				//Wenn die Funktion CMainFrame::OnTranslateAutomaticallyFinishes()
				//nach der Übersetzung noch nicht ausgeführt wurde, wird folgender
				//Code ausgeführt:
				if(!g_bExecutedOnTranslateAutomaticallyFinishes && m_bCreated)
				{
					KillTimer(1);
					if(m_bShowGraphicClauseProgress)
						m_Progress.DestroyWindow();
					if(m_bShowNumericClauseProgress)
						m_cStaticClauseProgress.DestroyWindow();
					if(m_bShowGraphicSentenceProgress)
						m_SentenceProgress.DestroyWindow();
					if(m_bShowNumericSentenceProgress)
						m_cStaticSentenceProgress.DestroyWindow();
					if(m_bShowGraphicTextProgress)
						m_TextProgress.DestroyWindow();
					if(m_bShowNumericTextProgress)
						m_cStaticTextProgress.DestroyWindow();
					if(m_bShowPrognosticedRemainingTime)
						m_cStaticRemainingTime.DestroyWindow();
					m_bCreated=FALSE;
					m_bMayStartThread=TRUE;
					//damit das Zerstören der Fortschrittsbalken (CProgressCtrl)
					//nicht in der Funktion 
					//CMainFrame::OnTranslateAutomaticallyFinishes() ausgeführt 
					//wird (sie wurden ja bereits über diesem Kommentar mit
					//DestroyWindow() zerstört und sollen nicht doppelt zerstört 
					//werden), wird g_bExecutedOnTranslateAutomaticallyFinishes 
					//auf TRUE gesetzt. Wwenn die Funktion 
					//CMainFrame::OnTranslateAutomaticallyFinishes() ausgeführt 
					//wird, wird g_bExecutedOnTranslateAutomaticallyFinishes 
					//ausgewertet.
					g_bExecutedOnTranslateAutomaticallyFinishes=TRUE;
				}
				CToolBarCtrl & toolBarCtrl=m_wndToolBar.GetToolBarCtrl();
				if(toolBarCtrl.IsButtonChecked(ID_TRANSLATE_AUTOMATICALLY_IF_TEXTMODIFICATION_BUTTON))
				{
					//toolBarCtrl.HideButton(ID_TRANSLATE_BUTTON,TRUE);
					Text * pText=new Text(*pEV->m_pText); // Kopie anlegen
					std::vector<Range> vecRange;
					EnglishView * pEV2=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
					if(pEV2)
					{
						pEV2->m_bDrawVector=TRUE;
						if(m_bCreated == FALSE)
						{
							BYTE bIDCount=0;//3;
							std::vector<UINT> vecUINT;
							if(m_bShowClauseProgress)
							{
								if(m_bShowNumericClauseProgress)
								{
									bIDCount++;
									vecUINT.push_back(ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE);
								}
								if(m_bShowGraphicClauseProgress)
								{
									bIDCount++;
									vecUINT.push_back(ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
								}
								//bIDCount++;
								//vecUINT.push_back(ID_INDICATOR_CLAUSE_PROGRESS_PANE);
							}
							if(m_bShowSentenceProgress)
							{
								if(m_bShowNumericSentenceProgress)
								{
									bIDCount++;
									vecUINT.push_back(ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE);
								}
								if(m_bShowGraphicSentenceProgress)
								{
									bIDCount++;
									vecUINT.push_back(ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
								}
								//bIDCount++;
								//vecUINT.push_back(ID_INDICATOR_SENTENCE_PROGRESS_PANE);
							}
							if(m_bShowTextProgress)
							{
								if(m_bShowNumericTextProgress)
								{
									bIDCount++;
									vecUINT.push_back(ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE);
								}
								if(m_bShowGraphicTextProgress)
								{
									bIDCount++;
									vecUINT.push_back(ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
								}
								//bIDCount++;
								//vecUINT.push_back(ID_INDICATOR_TEXT_PROGRESS_PANE);
							}
							if(m_bShowPrognosticedRemainingTime)
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
								m_wndStatusBar.SetIndicators(lpIDArray,sizeof(lpIDArray)/sizeof(UINT));
							}
							CStatusBarCtrl & statusbarCtrl=m_wndStatusBar.GetStatusBarCtrl();
							//int nHorz=0,nVert=0,nSpacing=0;
							//statusbarCtrl.GetBorders(nHorz,nVert,nSpacing);
							CRect rect,rect2;
							//pFrame->m_wndStatusBar.GetWindowRect(rect);
							m_wndStatusBar.GetClientRect(rect2);
#ifdef _DEBUG
							printf("lpRect->bottom: %d lpRect->left %d lpRect->right %d lpRect->top %d\n",rect.bottom,rect.left,rect.right,rect.top);
							printf("lpRect->bottom: %d lpRect->left %d lpRect->right %d lpRect->top %d\n",rect2.bottom,rect2.left,rect2.right,rect2.top);
							//printf("nHorz: %d nVert: %d nSpacing: %d\n",nHorz,nVert,nSpacing);
#endif
							if(bIDCount>0)
							{
								if(m_bShowPrognosticedRemainingTime)
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
							if(m_bShowNumericClauseProgress)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								m_cStaticClauseProgress.Create("",WS_VISIBLE|WS_CHILD,
									MyRect,&m_wndStatusBar,ID_INDICATOR_NUMERIC_CLAUSE_PROGRESS_PANE);
							}
							if(m_bShowGraphicClauseProgress)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								if(m_nGraphicClauseProgressType==0)
									m_Progress.Create(WS_VISIBLE|WS_CHILD,MyRect,
										&m_wndStatusBar,ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
								if(m_nGraphicClauseProgressType==1)
									m_Progress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
										&m_wndStatusBar,ID_INDICATOR_GRAPHIC_CLAUSE_PROGRESS_PANE);
								m_Progress.SetRange(0,100); //Set the range to between 0 and 100
								m_Progress.SetStep(1); // Set the step amount
							}
#ifdef _DEBUG
							printf("pFrame->m_bShowSentenceProgress\n");
#endif
							if(m_bShowNumericSentenceProgress)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								m_cStaticSentenceProgress.Create("",WS_VISIBLE|WS_CHILD,
									MyRect,&m_wndStatusBar,ID_INDICATOR_NUMERIC_SENTENCE_PROGRESS_PANE);
							}
							if(m_bShowGraphicSentenceProgress)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								if(m_nGraphicSentenceProgressType==0)
									m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD,MyRect,
										&m_wndStatusBar,ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
								if(m_nGraphicSentenceProgressType==1)
									m_SentenceProgress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
										&m_wndStatusBar,ID_INDICATOR_GRAPHIC_SENTENCE_PROGRESS_PANE);
								m_SentenceProgress.SetRange(0,100); //Set the range to between 0 and 100
								m_SentenceProgress.SetStep(1); // Set the step amount
							}
							if(m_bShowNumericTextProgress)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								m_cStaticTextProgress.Create("",WS_VISIBLE|WS_CHILD,
									MyRect,&m_wndStatusBar,ID_INDICATOR_NUMERIC_TEXT_PROGRESS_PANE);
							}
							if(m_bShowGraphicTextProgress)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								if(m_nGraphicTextProgressType==0)
									m_TextProgress.Create(WS_VISIBLE|WS_CHILD,MyRect,
										&m_wndStatusBar,ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
								if(m_nGraphicTextProgressType==1)
									m_TextProgress.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,MyRect,
										&m_wndStatusBar,ID_INDICATOR_GRAPHIC_TEXT_PROGRESS_PANE);
								m_TextProgress.SetRange(0,100); //Set the range to between 0 and 100
								m_TextProgress.SetStep(1); // Set the step amount
							}
							if(m_bShowPrognosticedRemainingTime)
							{
								m_wndStatusBar.GetItemRect(bIDCount++, &MyRect);
								m_cStaticRemainingTime.Create("",WS_VISIBLE|WS_CHILD,MyRect,&m_wndStatusBar,ID_INDICATOR_REMAINING_TIME);
								tAtTheBeginningOfTheTranslation=(CTime)CTime::GetCurrentTime();
							}
							LPCTSTR str="Fortschritt des Haupt- oder Nebensatzes";
							//statusbarCtrl.SetTipText(0,str);
							//statusbarCtrl.SetTipText(1,_T("Fortschritt des aktuellen Satzes"));
							//statusbarCtrl.SetTipText(2,_T("Fortschritt des ganzen Textes"));
							m_bCreated = TRUE;
							if(SetTimer(1,1000,NULL)==0)
							{
#ifdef _DEBUG
								printf("SetTimer(1,1000,NULL)==0\n");
#endif
							}
							else
							{
#ifdef _DEBUG
								printf("SetTimer(1,1000,NULL)!=0\n");
#endif
							}
							m_bTimerIntalled=TRUE;
						}
						TranslateAutomaticallyParameters * pTranslateAutomaticallyParameters=
							new TranslateAutomaticallyParameters(pText,vecRange,pEV2,
							m_hWnd,pEV2->GetSafeHwnd(),m_Progress.GetSafeHwnd(),
							m_SentenceProgress.GetSafeHwnd(),m_TextProgress.GetSafeHwnd());
						pTranslateAutomaticallyParameters->m_bCaseSensitive=m_bCaseSensitive;
						pTranslateAutomaticallyParameters->m_dwMaxCharsPerLine=m_dwMaxCharsPerLine;
						g_bContinue=TRUE;
						int nPriority=0;
						switch(m_nThreadPriority)
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
						m_pTranslationThread=AfxBeginThread(TranslateAutomatically,
							pTranslateAutomaticallyParameters,nPriority,0,0,NULL);
					}
				}
			}
		}
	}
}

void CMainFrame::OnEditMarkAll() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen

	CChildFrame * pChild=(CChildFrame*)MDIGetActive();
	if(pChild)
	{
		EnglishView * pEV=(EnglishView*)pChild->GetActiveView();
		if(pEV)
		{
			pEV->m_dwMarkingStart=0;
			pEV->m_dwMarkingEnd=pEV->m_pText->GetTextLength();
			pEV->m_bMark=TRUE;
		}
	}
}

void CMainFrame::OnUpdateComboboxButton(CCmdUI* pCmdUI) 
{
	// TODO: Code für die Befehlsbehandlungsroutine zum Aktualisieren der Benutzeroberfläche hier einfügen

	BOOL bGermanViewContainsAtLeastOneComboBox=FALSE;
	CChildFrame * pChild=(CChildFrame*)MDIGetActive();
	if(pChild)
	{
		EnglishView * pEV=(EnglishView*)pChild->m_SplitterWnd.GetPane(1,0);
		if(pEV)
			if(pEV->m_ComboBoxList.m_pFirst)
			{
				bGermanViewContainsAtLeastOneComboBox=TRUE;
				pCmdUI->Enable(TRUE);
			}
	}
	if(!bGermanViewContainsAtLeastOneComboBox)
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateTranslateButton(CCmdUI* pCmdUI) 
{
	// TODO: Code für die Befehlsbehandlungsroutine zum Aktualisieren der Benutzeroberfläche hier einfügen

	//LPDWORD lpExitCode=new DWORD;
  DWORD dwExitCode ;
	if(m_pTranslationThread)
	{
		::GetExitCodeThread(m_pTranslationThread->m_hThread,      // handle to the thread
      // address to receive termination status
			//lpExitCode
      & dwExitCode
			);
		if(//*lpExitCode
      dwExitCode == STILL_ACTIVE)
		{
			pCmdUI->Enable(FALSE);
			return;
		}
	}
	pCmdUI->Enable(TRUE);
}

//diese Funktion dient zum Möglichmachen des Markierens von Text in den 
//Kindfenstern; eine Variable wie "m_bShiftDown" muss keine Member-Variable
//der Ansicht eines Kindfensters sein (verbarucht nur unnötig Speicher),
//sondern reicht als globale Variable für alle Kindfenster
void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen und/oder Standard aufrufen
#ifdef _DEBUG
	printf("void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)\
 ANFANG\n");
#endif
	// Umschalt-Taste (die Taste, mit der man Großbuchstaben erzeugt) wurde
	//gedrückt
	if(nChar==16)
	{
		m_bShiftDown=TRUE;
	}
	
	CMDIFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

//diese Funktion dient zum Möglichmachen des Markierens von Text in den 
//Kindfenstern; eine Variable wie "m_bShiftDown" muss keine Member-Variable
//der Ansicht eines Kindfensters sein (verbarucht nur unnötig Speicher),
//sondern reicht als globale Variable für alle Kindfenster
void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen und/oder Standard aufrufen
	// Umschalt-Taste (die Taste, mit der man Großbuchstaben erzeugt) wurde
	//losgelassen
	if(nChar==16)
	{
		m_bShiftDown=FALSE;
	}
	
	CMDIFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}
