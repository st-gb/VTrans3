// VTrans.h : Haupt-Header-Datei für die Anwendung VTRANS
//

#if !defined(AFX_VTRANS_H__FB6F08E2_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
     #define AFX_VTRANS_H__FB6F08E2_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "../Resource.h"       // Hauptsymbole

// eigener Code, Anfang
//#include <fstream> //for ofstream
#include "LoadStatusDialog.h"
#include "../rest.h" //for CStringVector
#include "../Text.h"
#include "../xmlwriter.h"
#include <vector>
#include "../xmlwriter.h"
//#include "Translate.h"//for class Range
#include "../Word.hpp"
#include "../I_UserInterface.hpp"
#include "../VocabularyInMainMem/WordSearch.hpp"

extern WordNode * m_first;//=NULL;
//#include "Vocable.h"
//VocNode * m_first=NULL;
// eigener Code, Ende

//#define _LOG
////#define _DEBUG
////#define STEFAN_DEBUG
////FILE * logfile=NULL;
//
////#define FILE_PATH_FOR_INTERRUPTED_TRANSLATION "int_resp_crashed_trans_exe.txt"
//
//#ifdef _LOG
//  #define LOG_FILE_PATH "release_log.txt"
//  //#include <stdio.h>//for fflush()
//  ////ofstream ofstreamLogFile;
//  ////FILE * pfileLog;//=NULL;
//  //#define LOG(str) fprintf(pfileLog,_T(str)); \
//  //  /*fflush is important to ensure the last output was REALLY written to medium*/\
//  //  fflush(pfileLog);
//  //#define LOG1(str,var) fprintf(pfileLog,_T(str),var); \
//  //  /*fflush is important to ensure the last output was REALLY written to medium*/\
//  //  fflush(pfileLog);
//  //#define LOG2(str,var1,var2) fprintf(pfileLog,_T(str),var1,var2); \
//  //  /*fflush is important to ensure the last output was REALLY written to medium*/\
//  //  fflush(pfileLog);
//  #include <iostream>//for fflush()
//  #include <fstream>
//  //std::ofstream ofstreamLogFile(LOG_FILE_PATH);
//  #define LOG(var) ofstreamLogFile<< var; \
//    /*fflush is important to ensure the last output was REALLY written to medium*/\
//    ofstreamLogFile.flush();
//  #define LOGN(var) ofstreamLogFile<< var<<"\n"; \
//    /*fflush is important to ensure the last output was REALLY written to medium*/\
//    ofstreamLogFile.flush();
//  //Achtung: der Compiler gibt keinen Fehler aus, wenn nur 1 Parameter übergeben wird!
//  //->mit Vorsicht einsetzen
//  #define LOG2N(outstreamRelease,outstreamDebug) ofstreamLogFile<< outstreamRelease<<"\n"; \
//    /*fflush is important to ensure the last output was REALLY written to medium*/\
//    ofstreamLogFile.flush();
//
//#else
///*If "_LOG" is not defined, replace any "LOG","LOG1","LOG2" by "" 
//(->so NO overhead/ressources are claimed).*/
//  #define LOG
//  #define LOG1
//  #define LOG2
//  #ifdef _DEBUG
//    #define LOG2N(outstreamRelease,outstreamDebug) ofstreamLogFile<< outstreamDebug<<"\n"; \
//      /*fflush is important to ensure the last output was REALLY written to medium*/\
//      ofstreamLogFile.flush();
//  #endif//#ifdef _DEBUG
//#endif//ifdef _LOG

//Declaring a single class saves more ressources than to include a header file with this class decl.
class Range;

class SentenceAndValidityAndProperName;
/////////////////////////////////////////////////////////////////////////////
// CVTransApp:
// Siehe VTrans.cpp für die Implementierung dieser Klasse
//

class CVTransApp 
  : 
  public CWinApp
  , public I_UserInterface
{
public:
	CString m_strSearch;
	CVTransApp();
  WordSearchType m_wordsearch ;

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CVTransApp)
	public:
	virtual BOOL InitInstance();
  void InitDictionary( const std::string & cr_stdstrDictionaryFile );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
  void RunNonInteractive();
  //std::vector<CStringVector> TranslateText(
  //  Text & text,
  //  std::vector<SentenceAndValidityAndProperName> & rvecsentenceandvalidityandpropername,
  //  const DWORD & dwFlags  );
  void writeToOutputStream(std::ostream & rofstreamTranslToGerman,
    //std::vector<Range> & vecRange,std::vector<CStringVector> & vecstrvec
    std::vector<SentenceAndValidityAndProperName> & vecsentenceandvalidityandpropername,
    xmlwriter & MyXml);
	//}}AFX_VIRTUAL
  void Message(std::string stdstr )
  {
    ::AfxMessageBox( stdstr.c_str() ) ;
  }
// Implementierung
	//{{AFX_MSG(CVTransApp)
	afx_msg void OnAppAbout();
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

UINT OpenFile(LPVOID pParam);

class OpenFileParameters
{
public:
	CLoadStatusDialog * m_pLoadStatusDialog;
	HWND * m_phWndLoadStatusDialog;
	LPCTSTR m_lpszPathName;
	Text * m_pEnglishText;
};

namespace UI
{
  void ShowWordFileError(DWORD dwOffsetOfBeginOfEntry, DWORD dwOffsetOfEndOfEntry) ;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_VTRANS_H__FB6F08E2_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
