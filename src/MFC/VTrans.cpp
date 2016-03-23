// VTrans.cpp : Legt das Klassenverhalten f�r die Anwendung fest.
//

#include "../stdafx.h"
#include "VTrans.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "VTransDoc.h"
// eigener Code, Anfang
#include "EnglishView.h" // vorher: VTransView.h
#include "LoadStatusDialog.h"
#include "OptionsDialog.h" // vorher: VTransView.h
#include <fstream> //for ofstream
#include <iostream> //for cout
#include <ios>//fo ios_base
#include "../IO.h" //for ::UTF8toASCII(...), ::writeToOutputStream(...) etc.
#include "../1LinePerWordPair.h" //for OneLinePerWordPair::LoadWords(...)
#include "../xmlwriter.h"
#include <sstream> //for ostringstream
#include "VocabularyInMainMem/DoublyLinkedList/WordNode.hpp"
#include "VocabularyInMainMem/DoublyLinkedList/WordList.hpp"
//#include <AtlBase.h>
#include <atlconv.h>//for A2W
// eigener Code, Ende

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern LetterTree g_lettertree ;
extern WordList wordList;

#define _LOG
//#define _DEBUG
//#define STEFAN_DEBUG
//FILE * logfile=NULL;

#ifdef _LOG
  #define LOG_FILE_PATH "release_log.txt"
  //#include <stdio.h>//for fflush()
  ////ofstream ofstreamLogFile;
  ////FILE * pfileLog;//=NULL;
  //#define LOG(str) fprintf(pfileLog,_T(str)); \
  //  /*fflush is important to ensure the last output was REALLY written to medium*/\
  //  fflush(pfileLog);
  //#define LOG1(str,var) fprintf(pfileLog,_T(str),var); \
  //  /*fflush is important to ensure the last output was REALLY written to medium*/\
  //  fflush(pfileLog);
  //#define LOG2(str,var1,var2) fprintf(pfileLog,_T(str),var1,var2); \
  //  /*fflush is important to ensure the last output was REALLY written to medium*/\
  //  fflush(pfileLog);
  #include <iostream>//for fflush()
  #include <fstream>//for ofstream
  std::ofstream ofstreamLogFile;//(LOG_FILE_PATH);
  //#define LOG(var) ofstreamLogFile<< var; \
  //  /*fflush is important to ensure the last output was REALLY written to medium*/\
  //  ofstreamLogFile.flush();
#else
/*If "_LOG" is not defined, replace any "LOG","LOG1","LOG2" by "" 
(->so NO overhead/ressources are claimed).*/
  #define LOG
  #define LOG1
  #define LOG2
#endif//ifdef _LOG

volatile bool g_bContinueLoading=true;

//#define _DEBUG
//#define LOG

//#define LOG(str) static Log(char *s, ...)\
//	{\
//	if(LOG_FILE_POINTER==NULL)
//    std::ofstream ofstreamEnglishTextToTransate;
//ofstreamEnglishTextToTransate.
//	fprintf(LOG_FILE_POINTER,str);\
//	vprintf(s,argptr);\
//	va_end(argptr);\
//	printf("\n");\
//	}

//FILE * logfile=NULL;
// eigener Code, ANFANG
//#include "Vocable.h"
//VocNode * m_first=NULL;
// eigener Code, ENDE
//volatile BYTE g_bContinueLoading=TRUE;
volatile DWORD g_dwFileLength=0;
volatile DWORD g_dwRun=0;

//#ifdef LOG
//#define LOG() int log(const char*, ...);
//#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CVTransApp

BEGIN_MESSAGE_MAP(CVTransApp, CWinApp)
	//{{AFX_MSG_MAP(CVTransApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingef�gt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VER�NDERN!
	//}}AFX_MSG_MAP
	// Dateibasierte Standard-Dokumentbefehle
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard-Druckbefehl "Seite einrichten"
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVTransApp Konstruktion

CVTransApp::CVTransApp()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einf�gen
	// Alle wichtigen Initialisierungen in InitInstance platzieren
#ifdef _DEBUG
	freopen("log.txt","w",stdout); // printf wird nun nach log.txt umgeleitet
#endif
#ifdef STEFAN_DEBUG
	freopen("log.txt","w",stdout); // printf wird nun nach log.txt umgeleitet
#endif
#ifdef _DEBUG
	printf("CVTransApp::CVTransApp() ANFANG\n");
	fflush(stdout);
#endif
#ifdef LOG

#endif
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CVTransApp-Objekt

CVTransApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CVTransApp Initialisierung

BOOL CVTransApp::InitInstance()
{
  g_lettertree.SetUserInterface(this) ;
#ifdef _LOG
  ofstreamLogFile.open(LOG_FILE_PATH,ios_base::out|ios_base::trunc);
  //pfileLog=fopen(LOG_FILE_PATH,"w");
#endif
	TRACE("BOOL CVTransApp::InitInstance() ANFANG\n");
	AfxEnableControlContainer();

	// Standardinitialisierung
	// Wenn Sie diese Funktionen nicht nutzen und die Gr��e Ihrer fertigen 
	//  ausf�hrbaren Datei reduzieren wollen, sollten Sie die nachfolgenden
	//  spezifischen Initialisierungsroutinen, die Sie nicht ben�tigen, entfernen.

#ifdef _AFXDLL
	Enable3dControls();			// Diese Funktion bei Verwendung von MFC in gemeinsam genutzten DLLs aufrufen
#else
	Enable3dControlsStatic();	// Diese Funktion bei statischen MFC-Anbindungen aufrufen
#endif

	// �ndern des Registrierungsschl�ssels, unter dem unsere Einstellungen gespeichert sind.
	// ZU ERLEDIGEN: Sie sollten dieser Zeichenfolge einen geeigneten Inhalt geben
	// wie z.B. den Namen Ihrer Firma oder Organisation.
	//SetRegistryKey(_T("Probably Alienmade Software"));
	//free((void*)m_pszRegistryKey);
	//m_pszRegistryKey=_tcsdup(_T("HKEY_CURRENT_USER\\Software\\Probably Alienmade Software\\"));
	//m_pszRegistryKey=_tcsdup(_T("Probably Alienmade Software"));

	LoadStdProfileSettings(16);  // Standard INI-Dateioptionen laden (einschlie�lich MRU)

	// Dokumentvorlagen der Anwendung registrieren. Dokumentvorlagen
	//  dienen als Verbindung zwischen Dokumenten, Rahmenfenstern und Ansichten.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_VTRANSTYPE,
		RUNTIME_CLASS(CVTransDoc),
		RUNTIME_CLASS(CChildFrame), // Benutzerspezifischer MDI-Child-Rahmen
		RUNTIME_CLASS(EnglishView)); // war vorher: CVTransView
	AddDocTemplate(pDocTemplate);

  //int nNumArgs;
  //LPTSTR lptstr=GetCommandLine();
  //LPTSTR * plptstr=CommandLineToArgvA(lptstr,&nNumArgs);
  //Text text;
  ////If the commandline only contains the program name.
  //if(nNumArgs==1)
  //{
  //  const char * pch;
  //  std::string str;
  //  std::ifstream ifstreamGermanText(
  //    //Output filename.
  //    "german.txt",ios_base::in);
  //  ifstreamGermanText>>str;
  //  pch=str.c_str();
  //  //while(pch++!='\0')
  //  //  text.Insert(text.GetLength(),
  //  //    //Current character of text to translate.
  //  //    pch);
  //  text.Insert(text.GetLength(),(LPSTR)pch,0);
  //}//If the commandline only contains the program name.
  //else
  //{
  //  //while(plptstr[1]++!='\0')
  //  //  text.Insert(text.GetLength(),
  //  //    //Current character of sentence to translate.
  //  //    plptstr[1]);
  //  text.Insert(text.GetLength(),
  //      //First commmand line argument.
  //      plptstr[1],0);
  //}
  //  std::vector<Range> vecRange;
  //  std::vector<CStringVector> vecstrvec=TranslateText(text,vecRange,0,NULL);
  //  if(vecstrvec.size()>0 && nNumArgs>2)
  //  {
  //    std::ofstream ofstreamTranslToGerman(
  //      //Output filename.
  //      plptstr[2],ios_base::out);
  //    if(ofstreamTranslToGerman)
  //    {
  //      CStringVector cstrvec=vecstrvec.at(0);
  //      for(WORD wIndex=0;wIndex<cstrvec.size();++wIndex)
  //        ofstreamTranslToGerman<<cstrvec.at(wIndex).GetBuffer();
  //    }
  //    ofstreamTranslToGerman.close();
  //  }
  ////}
  //   
  //// Free memory allocated for CommandLineToArgvW arguments.
  //LocalFree(plptstr);
  LOG("23.05.2008 11.21.27\n");

//#ifdef _DEBUG
//  bool bRunNonInteractive=false;
//#else
  bool bRunNonInteractive=true;
  LOG("23.05.2008 11.21.40\n");
  int nNumArgs;
  LPWSTR lptstr=GetCommandLineW();
  LPWSTR //* plptstr,
    * arlptstr=CommandLineToArgvW(lptstr,&nNumArgs);
  Text text;
  LOG("23.05.2008 11.21.55 " << nNumArgs << "\n");
  std::string stdstrDictionaryFile = "words.txt" ;
  if( nNumArgs //== 
    >= 2 && 
    //Don't simply use == for comparing, because this would compare pointer 
    //(pointer addresses).
    strcmp(CW2A(arlptstr[1]),"GUI") == STRINGS_ARE_IDENTICAL )
  {
    LOG(CW2A(arlptstr[1]) << strcmp(CW2A(arlptstr[1]),"GUI") << "\n") ;
    //CW2A(arlptstr[1])
    bRunNonInteractive = false;
    LOG("23.05.2008 11.22.08\n");
    if( nNumArgs >= 3 )
      stdstrDictionaryFile = std::string( CW2A(arlptstr[2] ) );
  }
//#endif
  InitDictionary(stdstrDictionaryFile);
  if(bRunNonInteractive)
    RunNonInteractive();
  //}
  else
  //if(nNumArgs<0)
  {
    LOG("23.05.2008 11.22.16\n");
	  // Haupt-MDI-Rahmenfenster erzeugen
	  CMainFrame* pMainFrame = new CMainFrame(//&m_wordsearch
      &g_lettertree );
	  if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
    {
      LOG("23.05.2008 11.22.32\n");
		  return FALSE;
    }
	  m_pMainWnd = pMainFrame;

	  // Befehlszeile parsen, um zu pr�fen auf Standard-Umgebungsbefehle DDE, Datei offen
	  CCommandLineInfo cmdInfo;
	  ParseCommandLine(cmdInfo);

    LOG("23.05.2008 11.22.42\n");

    if(bRunNonInteractive)
	  // Verteilung der in der Befehlszeile angegebenen Befehle
	  if (!ProcessShellCommand(cmdInfo))
  #ifdef _LOG
    {
      LOG("23.05.2008 11.22.52\n");
  #endif
		  return FALSE;
  #ifdef _LOG
    }
  #endif

    LOG("23.05.2008 11.23.03\n");
	  // Das Hauptfenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	  pMainFrame->ShowWindow(m_nCmdShow);
	  pMainFrame->UpdateWindow();
  }
	//#ifdef LOG
		//logfile=freopen("log.txt","w",stdout);
	//#endif
#ifdef _DEBUG
	printf("BOOL CVTransApp::InitInstance() - return TRUE");
#endif
	return TRUE;
}

void CVTransApp::InitDictionary(
  const std::string & cr_stdstrDictionaryFile 
  )
{
	WordNode * pWordNodeCurrent=NULL;
	WordNode * pWordNodePrevious=NULL;
	Word * pWord=NULL;
#ifdef _DEBUG
	printf("void LoadWords(WordNode * pWordNode) ANFANG\n");
#endif
	//zuerst die integralen Vokabeln der verketteten Liste hinzuf�gen, Anfang
	wordList.m_pWordNodeFirst=new WordNode();
	wordList.m_pWordNodeFirst->m_pWord=new EnglishAuxiliaryVerb(
		"be\nam\nare\nis\nare\nare\nare\nwas\nwere\n"
		"was\nwere\nwere\nwere\nbeen\n",false);

	//current=voclist.m_first;
	wordList.m_pWordNodeFirst->m_pWordNodePrevious=NULL;
	pWordNodeCurrent=wordList.m_pWordNodeFirst;
	//delete voc;
	//voc=new GermanVerb("sein\nbin\nbist\nist\nsind\nseid\nsind\nwar\nwarst\nwar\nwaren\nward\nwaren\ngewesen\n");
	//voclist.Append(voc);
	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	GermanVerb gv=GermanVerb("sein\nbin\nbist\nist\nsind\nseid\nsind\nwar\nwarst\nwar\nwaren\nward\nwaren\ngewesen\n");
	gv.m_bCase=4;
	gv.m_bMove=0;
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new GermanVerb(gv);
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;
	//delete voc;
	//voc=new EnglishAuxiliaryVerb("have\nhave\nhave\nhas\nhave\nhave\nhave\nhad\nhad\nhad\nhad\nhad\nhad\nhad\n");
	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new 
		EnglishAuxiliaryVerb("have\nhave\nhave\nhas\nhave\nhave\n"
		"have\nhad\nhad\nhad\nhad\nhad\nhad\nhad\n",false);
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;

	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new GermanVerb("haben\nhabe\nhast\nhat\nhaben\nhabt\nhaben\nhatte\nhattest\nhatte\nhatten\nhattet\nhatten\ngehabt\n4");
	//pWordNodeCurrent->m_pWordNodeNext->m_pWord->m_bMove=0;
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;

	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new 
		EnglishAuxiliaryVerb("will\nwill\nwill\nwill\nwill\nwill\n"
		"will\nwill\nwill\nwill\nwill\nwill\nwill\nwill\n",false);
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;

	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new GermanVerb(
		"werden\nwerde\nwirst\nwird\nwerden\nwerdet\nwerden\nwurde\n"
		"wurdest\nwurde\nwurden\nwurdet\nwurden\ngeworden\n");
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;

	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new 
		EnglishAuxiliaryVerb("do\ndo\ndo\ndoes\ndo\ndo\ndo\ndid\n"
		"did\ndid\ndid\ndid\ndid\ndone\n",false);
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;

	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new 
		GermanVerb("machen\nmache\nmachst\nmacht\nmachen\nmacht\n"
		"machen\nmachte\nmachtest\nmachte\nmachten\nmachtet\n"
		"machten\ngemacht\n");
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;

	/*previous=current;
	current->next=new VocNode();
	EnglishConjunction ec;
	ec.m_strWord=_T("but");
	current->next->voc=new EnglishConjunction(ec); // StandardKopierKonstruktor
	current=current->next;
	current->m_Previous=previous;

	previous=current;
	current->next=new VocNode();
	GermanConjunction gc;
	gc.m_strWord=_T("aber");
	current->next->voc=new GermanConjunction(gc); // StandardKopierKonstruktor
	current=current->next;
	current->m_Previous=previous;

	{
	previous=current;
	current->next=new VocNode();
	EnglishConjunction ec;
	ec.m_strWord=_T("if");
	current->next->voc=new EnglishConjunction(ec); // StandardKopierKonstruktor
	current=current->next;
	current->m_Previous=previous;

	previous=current;
	current->next=new VocNode();
	GermanConjunction gc;
	gc.m_strWord=_T("falls");
	gc.m_Syntax=1; // Satzbau ist Subjekt Objekt Pr�dikat 
	current->next->voc=new GermanConjunction(gc); // StandardKopierKonstruktor
	current=current->next;
	current->m_Previous=previous;
	}

	{
	previous=current;
	current->next=new VocNode();
	EnglishConjunction ec;
	ec.m_strWord=_T("when");
	current->next->voc=new EnglishConjunction(ec); // StandardKopierKonstruktor
	current=current->next;
	current->m_Previous=previous;

	previous=current;
	current->next=new VocNode();
	GermanConjunction gc;
	gc.m_strWord=_T("wenn");
	gc.m_Syntax=1; // Satzbau ist Subjekt Objekt Pr�dikat 
	current->next->voc=new GermanConjunction(gc); // StandardKopierKonstruktor
	current=current->next;
	current->m_Previous=previous;
	}

	{
	previous=current;
	current->next=new VocNode();
	EnglishConjunction ec;
	ec.m_strWord=_T("that");
	current->next->voc=new EnglishConjunction(ec); // StandardKopierKonstruktor
	current=current->next;
	current->m_Previous=previous;

	previous=current;
	current->next=new VocNode();
	GermanConjunction gc;
	gc.m_strWord=_T("dass");
	gc.m_Syntax=1; // Satzbau ist Subjekt Objekt Pr�dikat 
	current->next->voc=new GermanConjunction(gc); // StandardKopierKonstruktor
	current=current->next;
	current->m_Previous=previous;
	}*/
  OneLinePerWordPair::LoadWords( cr_stdstrDictionaryFile, 
    pWordNodeCurrent) ;
}

void CVTransApp::writeToOutputStream(std::ostream & rofstreamTranslToGerman,
  //std::vector<Range> & vecRange,std::vector<CStringVector> & vecstrvec
  std::vector<SentenceAndValidityAndProperName> & vecsentenceandvalidityandpropername,
  xmlwriter & MyXml
  )
{
  if(rofstreamTranslToGerman)
  {
    CStringVector strvec;
    SentenceAndValidityAndProperName sentenceandvalidityandpropername;
    for(WORD wSentenceIndex=0;wSentenceIndex<
      vecsentenceandvalidityandpropername.size();++wSentenceIndex)
    {
      IntPair intpairConntectedWords;
      Range range;
      sentenceandvalidityandpropername=vecsentenceandvalidityandpropername.
        at(wSentenceIndex);
      rofstreamTranslToGerman<<sentenceandvalidityandpropername.
        byReturnOfSentenceParsing;
      //Only if the sentence is grammatically correct a translation exists
      //and thus it makes sense to write it and the proper names.
      if(sentenceandvalidityandpropername.byReturnOfSentenceParsing==1)
      {
        MyXml.Createtag("sentence");
        for(WORD wWordIndex=0;wWordIndex<//vecstrvec
          sentenceandvalidityandpropername.vecvecstrSentenceTokens.size();++wWordIndex)
        {
          LOGN("31.05.2008 22.30.20");
          strvec=sentenceandvalidityandpropername.vecvecstrSentenceTokens.
            at(wWordIndex);
          LOGN("31.05.2008 22.30.44");
          //rofstreamTranslToGerman<<//vecstrvec
          //  sentenceandvalidityandpropername.vecvecstrSentenceTokens.at(wWordIndex).
          //  at(0).GetBuffer()<<" ";
          LOGN("31.05.2008 22.30.55");
          //MyXml.CreateChild("word",sentenceandvalidityandpropername.
          //  vecvecstrSentenceTokens.at(wWordIndex).at(0).GetBuffer());
          MyXml.Createtag("word");
          LPWSTR wstrTranslation;
          int nReturnOfWideCharToMultiByte;
          LPSTR lpTranslationInUTF8=NULL;
          for(WORD wTranslationForEnglishWordIndex=0;
            wTranslationForEnglishWordIndex<strvec.size();
            ++wTranslationForEnglishWordIndex)
          {
            //Without this errors while building:
             USES_CONVERSION;
            wstrTranslation=A2W(strvec.at(wTranslationForEnglishWordIndex).
              GetBuffer()
              //"�������"
              );
            nReturnOfWideCharToMultiByte=WideCharToMultiByte(CP_UTF8, 0, //lpwStr
              wstrTranslation,
              //Specifies the number of wide characters in the string pointed to by the lpWideCharStr parameter. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically. The length will include the null-terminator. 
              -1,
              lpTranslationInUTF8,//Points to the buffer to receive the translated string.
                //If the function succeeds, and cbMultiByte is zero, the return value is the required size, in bytes, for a buffer that can receive the translated string. 
                0,//cbMultiByte
                NULL,//If this parameter is NULL, a system default value is used. 
                NULL//This parameter may be NULL. The function is faster when both lpDefaultChar and lpUsedDefaultChar are NULL. 
              );
            ////If a German umlaut, at least 1 more byte is needed.
            //nReturnOfWideCharToMultiByte*=200;
            LOG("23.05.2008 11.23.17 "<<nReturnOfWideCharToMultiByte<<"\n");
#ifdef _LOG
            BYTE byWideCharByte;
            for(DWORD i=0;i<wcslen(wstrTranslation);++i)
            {
              byWideCharByte=*(wstrTranslation+i);
              LOG(byWideCharByte<<"("<<(int)byWideCharByte<<")");
              byWideCharByte=*((BYTE *)(wstrTranslation+i)+1);
              LOG(byWideCharByte<<"("<<(int)byWideCharByte<<")");
            }
#endif
            if(nReturnOfWideCharToMultiByte)
            {
              //If the function succeeds, and cbMultiByte is zero, the return value is the required size, in bytes, for a buffer that can receive the translated string
              lpTranslationInUTF8=new CHAR[nReturnOfWideCharToMultiByte];
              if(WideCharToMultiByte(
                //When this is set, dwFlags must be zero and both lpDefaultChar and lpUsedDefaultChar must be NULL.
                CP_UTF8,//CodePage 
                0,//dwFlags  
                //lpwStr
                wstrTranslation,//Points to the wide-character string to be converted.
                -1,//cchWideChar: Specifies the number of wide characters in the string pointed to by the lpWideCharStr parameter. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically. The length will include the null-terminator.
                lpTranslationInUTF8,//Points to the buffer to receive the translated string. 
                  //If the function succeeds, and cbMultiByte is zero, the return value is the required size, in bytes, for a buffer that can receive the translated string. 
                  nReturnOfWideCharToMultiByte,//cbMultiByte
                  NULL,//LPCSTR lpDefaultChar, If this parameter is NULL, a system default value is used. 
                  NULL//LPBOOL lpUsedDefaultChar:This parameter may be NULL. The function is faster when both lpDefaultChar and lpUsedDefaultChar are NULL. 
                  )
                )
              {
                LOG("23.05.2008 11.24.05 "<<lpTranslationInUTF8<<"\n");
                MyXml.CreateChild("translation",//EncodeToUTF8(
                  lpTranslationInUTF8);
                LOGN("31.05.2008 22.27.11");
              }
              else
                LOG("23.05.2008 11.24.24 "<<::GetLastError()<<ERROR_INSUFFICIENT_BUFFER<<" "<<ERROR_INVALID_FLAGS<<" "<<ERROR_INVALID_PARAMETER<<"\n");
            }
          }//loop through possible German translations for an English word.
          MyXml.CloseLasttag();//"word" tag
        }//Loop through tokens.
        rofstreamTranslToGerman<<
          //carriage Return +Newline is used to indicate the end of the sentence
          "\r\nproper name Size:"<<
          //HTONL: Convert Host(Intel) TO Network(Big Endian) byte order Long value.
          //htonl(
          (unsigned long)//vecRange
          sentenceandvalidityandpropername.vecrangeProperName.size()//)
          <<" ";
	      for(DWORD dwRangeIndex=0;dwRangeIndex<//vecRange
          sentenceandvalidityandpropername.vecrangeProperName.size();dwRangeIndex++)
	      {
		      range=//vecRange
            sentenceandvalidityandpropername.vecrangeProperName.at(dwRangeIndex);
		      //for(DWORD dwCharacterIndexWithinEnglishText=range.m_dwStart;
        //    dwCharacterIndexWithinEnglishText<range.m_dwEnd;
            //dwCharacterIndexWithinEnglishText++)

          std::ostringstream ostrstream;
          //The start index ist wrongly stored into the field "m_dwEnd".
          //ostrstream<<vecsentenceandvalidityandpropername.at(0).vecrangeProperName.at(wIndex).m_dwEnd;
          //ostrstream<<range.at(wIndex).m_dwEnd;
          ostrstream<<range.m_dwStart;
          MyXml.AddAtributes("start",ostrstream.str());
          std::ostringstream ostrstream2;
          //The end index ist wrongly stored into the field "m_dwStart".
          //ostrstream2<<vecsentenceandvalidityandpropername.at(0).vecrangeProperName.at(wIndex).m_dwStart;
          //ostrstream2<<range.at(wIndex).m_dwStart;
          ostrstream2<<range.m_dwEnd;
          MyXml.AddAtributes("end",ostrstream2.str());
          MyXml.Createtag("range-of-proper-name");
          MyXml.CloseLasttag();//"range-of-proper-name" tag
		      rofstreamTranslToGerman<<
            //HTONL: Convert Host(Intel) TO Network(Big Endian) byte order Long value.
            //htonl(
            (unsigned long)range.m_dwStart//)
            <<//htonl(
            (unsigned long)range.m_dwEnd//)
            ;
	      }//Proper name range loop
        MyXml.CloseLasttag();//"sentence" tag
        for(DWORD dwRangeIndex=0;dwRangeIndex<//vecRange
          sentenceandvalidityandpropername.m_vecintpairConntectedWords.size();dwRangeIndex++)
	      {
		      intpairConntectedWords=
            sentenceandvalidityandpropername.m_vecintpairConntectedWords.at(dwRangeIndex);
          std::ostringstream ostrstream;
          ostrstream<<intpairConntectedWords.m_n1-(ID_USER+100);
          MyXml.AddAtributes("number1",ostrstream.str());
          std::ostringstream ostrstream2;
          ostrstream2<<intpairConntectedWords.m_n2-(ID_USER+100);
          MyXml.AddAtributes("number2",ostrstream2.str());
          MyXml.Createtag("connected-words");
          MyXml.CloseLasttag();//"connected-words" tag
        }
      }//if(sentenceandvalidityandpropername.byReturnOfSentenceParsing==1)
      //MyXml.CloseAlltags();
      //MyXml.CloseLasttag();//"sentence" tag
    }//sentences loop
    //rofstreamTranslToGerman.close();
  }//if(rofstreamTranslToGerman)
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg-Dialog f�r Info �ber Anwendung

class CAboutDlg : public CDialog
{
public:
	CButton m_TestButton;
	CAboutDlg();

// Dialogdaten
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CEdit	m_editHomepage;
	CString	m_homepage_edit;
	CString	m_strRegistryKey;
	CString	m_strEMail;
	CString	m_strConfiguration;
	//}}AFX_DATA

	// �berladungen f�r virtuelle Funktionen, die vom Anwendungs-Assistenten erzeugt wurden
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	//{{AFX_MSG(CAboutDlg)
		// Keine Nachrichten-Handler
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_homepage_edit = _T(""); //http://home.t-online.de/home/0309322035-0001/
	m_strRegistryKey = CString("HKEY_CURRENT_USER\\Software\\")+AfxGetApp()->
    m_pszRegistryKey//+CString("\\VTrans")
    ;
	//m_strEMail = _T("rasenmaehermann@web.de");
	#ifdef _DEBUG
		m_strConfiguration = _T("Debug");
	#else
		m_strConfiguration = _T("Release");
	#endif
	//}}AFX_DATA_INIT
	RECT rect;
	rect.top=0;//100;
	rect.left=0;//100;
	rect.right=200;
	rect.bottom=320;

	//m_editHomepage.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|ES_READONLY,rect,NULL,31000);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_HOMEPAGE_EDIT, m_homepage_edit);
	DDX_Text(pDX, IDC_REGISTRY_KEY_EDIT, m_strRegistryKey);
	DDX_Text(pDX, IDC_EMAIL_EDIT, m_strEMail);
	DDX_Text(pDX, IDC_CONFIGURATION_STATIC, m_strConfiguration);
	//}}AFX_DATA_MAP
	/*m_TestButton.Create(_T("Test"),WS_CHILD|WS_VISIBLE,CRect(0,0,200,200),this,1011);
	m_TestButton.MoveWindow(CRect(0,0,200,200));
	m_TestButton.ShowWindow(TRUE);*/
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// Keine Nachrichten-Handler
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Anwendungsbefehl zum Ausf�hren des Dialogfelds
void CVTransApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	//aboutDlg.m_TestButton.MoveWindow(CRect(0,0,400,400));
	//aboutDlg.m_TestButton.ShowWindow(TRUE);
	//COptionsDialog dlg;
	//dlg.DoModal();
	aboutDlg.DoModal();
	//aboutDlg.m_editHomepage.SetReadOnly(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CVTransApp-Nachrichtenbehandlungsroutinen


BOOL CVTransApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Speziellen Code hier einf�gen und/oder Basisklasse aufrufen
	
    /*CWnd* pWnd = GetFocus();
    if (pWnd != NULL)
		{
			CMDIChildWnd *pChild = MDIGetActive();
			if(pChild)
			{
				EnglishView *pView = (EnglishView *) pChild->GetActiveView();
				if(pWnd!=pView)
					AfxMessageBox("jdsfjs",MB_OK,NULL);
			}
		}*/
	return CWinApp::PreTranslateMessage(pMsg);
}

UINT OpenFile(LPVOID pParam)
{
	//m_dwEnglishLines=0;
	CFile file;
	OpenFileParameters * pOpenFileParameters=(OpenFileParameters*)pParam;
	//CLoadStatusDialog * pLoadStatusDialog=(CLoadStatusDialog *)CWnd::
	//	FromHandle(*pOpenFileParameters->m_phWndLoadStatusDialog);

#ifdef _DEBUG
	printf("UINT OpenFile(LPVOID pParam) ANFANG\n");
#endif
  if (pOpenFileParameters==NULL)
    return 0;   // falls pOpenFileParameters ung�ltig ist
	if(file.Open(pOpenFileParameters->m_lpszPathName,CFile::modeReadWrite|
		CFile::shareDenyRead,NULL))
	{
		BYTE * byteArray=new byte[file.GetLength()];
		g_dwFileLength=file.GetLength();
		if(!byteArray) //wenn byteArray gleich Null
		{
			file.Close();
			AfxMessageBox("Es existiert momentan nicht gen�gend Arbeitsspeicher zum \
Lesen der Datei.",MB_OK,NULL);
			return 0;
		}
		//TRACE("Datei %s erfolgreich ge�ffnet\n",lpszPathName);
		if(g_dwFileLength>0)
		{
			//DWORD dwInsertionPos=0;
			//TextNode * pTextNodeCurrent=m_EnglishText.m_first;
			//g_dwFileLength=file.GetLength();
			TextNode * pTextNodeCurrent=NULL;
			TextNode * pTextNodePrevious=NULL;
			file.Read(byteArray,g_dwFileLength);
#ifdef _DEBUG
			printf("vor: while(!::IsWindow(*pOpenFileParameters->\
m_phWndLoadStatusDialog));\n");
#endif
			// solange warten, 
			// bis das Fenster-Handle g�ltig ist (dauert nur wenige Millisekunden)
			// (Das Fenster-Handle ist sonst in manchen Aufrufen von Translate()
			// ung�ltig und es kann dann kein Timer installiert werden;
			// SetTimer() funktioniert nur, wenn das Fenster ein g�ltiges Handle hat
			//while(!::IsWindow(pOpenFileParameters->m_pLoadStatusDialog->m_hWnd));
			while(!::IsWindow(*pOpenFileParameters->m_phWndLoadStatusDialog));
#ifdef _DEBUG
			printf("nach: while(!::IsWindow(*pOpenFileParameters->\
m_phWndLoadStatusDialog));\n");
#endif
			//pLoadStatusDialog->SetTimer(1,1000,NULL);
			::SetDlgItemText(*pOpenFileParameters->m_phWndLoadStatusDialog,
				IDC_ACTION_STATIC,
				_T("Aktion: Einf�gen der Bytes in die interne Datenstruktur"));
			for(g_dwRun=0;g_dwRun<g_dwFileLength && g_bContinueLoading;g_dwRun++)
			{
				//if(byteArray[dwRun]!=10)
				if(pTextNodePrevious==NULL)
				{
					if((pOpenFileParameters->m_pEnglishText->m_first=
						new TextNode(byteArray[g_dwRun],0))==NULL)
					{
						AfxMessageBox(_T("Nicht gen�gend Arbeitsspeicher vorhanden um \
die Datei zu laden."),MB_OK,0);
						break;
					}
					pTextNodeCurrent=pOpenFileParameters->m_pEnglishText->m_first;
					pTextNodePrevious=pOpenFileParameters->m_pEnglishText->m_first;
				}
				else
				{
					if((pTextNodeCurrent->m_next=new TextNode(byteArray[g_dwRun],0))==NULL)
					{
						AfxMessageBox(_T("Nicht gen�gend Arbeitsspeicher vorhanden um \
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
	if(::IsWindow(*pOpenFileParameters->m_phWndLoadStatusDialog))
	{
#ifdef _DEBUG
		printf("::IsWindow(*pOpenFileParameters->m_phWndLoadStatusDialog)\n");
#endif
		//Nach dem Laden der Datei soll das Lade-Status-Dialogfeld geschlossen 
		//werden.
		//Dazu muss dieses aber zerst�rt werden, was mir
		//von dieser Funktion aus (OpenFile) aber nicht gelang
		//(Thread-Probleme: ich glaube, dass man von einem anderen Thread
		//aus kein MFC-Objekt (zum Beispiel CDialog) als der Thread,
		//der das MFC-Objekt erstellte, zerst�ren kann).
		//Deshalb wird CLoadStatusDialog::OnEndLoadStatusDialog()
		//ausgef�hrt, wo das Zerst�ren einwandfrei funktioniert, weil das
		//Dialogfeld im selben Thread erstellt wurde
		::PostMessage(*pOpenFileParameters->m_phWndLoadStatusDialog,
			WM_END_LOADSTATUSDIALOG,NULL,NULL);

		//CLoadStatusDialog loadStatusDialog;
		//CLoadStatusDialog * pLoadStatusDialog=(CLoadStatusDialog *)CWnd::
		//	FromHandle(pOpenFileParameters->m_hWndLoadStatusDialog);
		//loadStatusDialog.Attach(*pOpenFileParameters->m_phWndLoadStatusDialog);
		//pOpenFileParameters->m_pLoadStatusDialog->KillTimer(1);
		//pOpenFileParameters->m_pLoadStatusDialog->EndDialog(IDOK);
		//loadStatusDialog.KillTimer(1);
		//loadStatusDialog.EndDialog(IDOK);
		//loadStatusDialog.Detach();
	}

#ifdef _DEBUG
	printf("UINT OpenFile(LPVOID pParam) return 1\n");
#endif
	return 1;
}

void CVTransApp::RunNonInteractive()
{
  int nNumArgs;
  LPWSTR lptstr=GetCommandLineW();
  LPWSTR * plptstr,* arlptstr=CommandLineToArgvW(lptstr,&nNumArgs);
  Text text;
  std::cout<<"Trans--running non-interactive\n"<<endl;
  if(nNumArgs==1)
  {
    const char * pch;
    std::string str;
    std::ifstream ifstreamGermanText(
      //Output filename.
      "english.txt",ios_base::in);
    //wenn erfolgreich ge�ffnet.
    if(ifstreamGermanText)
    {
      char ch;
      //LPWSTR pwstrTranslation=NULL;
      while(true)
      {
        ch=ifstreamGermanText.get();
        if(ifstreamGermanText.eof())
          break;
        str+=ch;
      }
      LOGN("28.05.2008 20.59.56");
      //LPWSTR wstrTranslation((LPWSTR)str.c_str());
      LOGN("28.05.2008 20.58.44");

      //pch=pszA;
      pch=::UTF8toASCII(str.c_str());
      LOGN("12.06.2008 22.24.35 "<<(unsigned long)pch<<pch);
    }
    else
    {
      LOGN("28.05.2008 21.12.17");
      return;
    }
    //pch=str.c_str();
    //while(pch++!='\0')
    //  text.Insert(text.GetLength(),
    //    //Current character of text to translate.
    //    *pch);
    text.Insert(0,(LPSTR)pch,0);
  }
  else//nNumArgs>1
  {
    //while(plptstr[1]++!='\0')
    //  text.Insert(text.GetLength(),
    //    //Current character of sentence to translate.
    //    plptstr[1]);
    text.Insert(0,CW2A(arlptstr[1]),0);
  }
  std::vector<Range> vecRange;
  //while(plptstr[1]++!='\0')
  //  text.Insert(text.GetLength(),
  //  //*plptstr[1]: Next character of sentence to translate.
  //  *plptstr[1]);
  g_bContinue=TRUE;
  //LoadWords(m_first);
  //InitDictionary();
  //::TranslateAndWriteOutputFile();
  std::vector<SentenceAndValidityAndProperName> 
    vecsentenceandvalidityandpropername;
  std::vector<CStringVector> vecstrvec=TranslateText(text,//vecRange
    vecsentenceandvalidityandpropername,0//,NULL
    );
  //if(vecstrvec.size()>0)
  {
    xmlwriter MyXml("XML.txt");
    MyXml.Createtag("sentences");
    //CStringVector cstrvec=vecstrvec.at(0);
    if(nNumArgs>2)
    {
      plptstr=&arlptstr[2];
      std::ofstream ofstreamTranslToGerman(
        //Output filename.
        *plptstr,std::ios_base::out);
      //if(ofstreamTranslToGerman)
      //{
      //  for(WORD wIndex=0;wIndex<vecstrvec.size();++wIndex)
      //    ofstreamTranslToGerman<<vecstrvec.at(wIndex).at(0).GetBuffer()<<" ";
      //}
      //ofstreamTranslToGerman.close();
      
      ::writeToOutputStream(ofstreamTranslToGerman,//vecRange,vecstrvec
        vecsentenceandvalidityandpropername,MyXml
        );
    }//if(nNumArgs>2)
    else
    {
      std::ofstream ofstreamTranslToGerman(
        //Output filename.
        "trans.txt",std::ios_base::out);
      //if(ofstreamTranslToGerman)
      //{
      //  for(WORD wIndex=0;wIndex<vecstrvec.size();++wIndex)
      //    ofstreamTranslToGerman<<vecstrvec.at(wIndex).at(0).GetBuffer()<<" ";
      //  ofstreamTranslToGerman.close();
      //}
      ::writeToOutputStream(ofstreamTranslToGerman,//vecRange,vecstrvec
        //std::cout,
        vecsentenceandvalidityandpropername,
        MyXml
        );

      //Die Umleitung der Std-Ausgabe(um einfacher in die Log-Datei 
      //schreiben zu k�nnen) wieder r�ckg�ngig machen.
      //freopen(stdout,"a",stdout); 
      //fclose(m_fileLog);
      for(WORD wIndex=0;wIndex<vecstrvec.size();++wIndex)
      {
        cout<<vecstrvec.at(wIndex).at(0).GetBuffer()<<" ";
        TRACE(vecstrvec.at(wIndex).at(0).GetBuffer());
      }

#ifdef _LOG
      if(vecsentenceandvalidityandpropername.size()>0)
        LOG("23.05.2008 11.24.59 "<<
          vecsentenceandvalidityandpropername.at(0).vecrangeProperName.size()<<"\n");
#endif
      //for(WORD wIndex=0;wIndex<vecsentenceandvalidityandpropername.at(0).vecrangeProperName.size();++wIndex)
      //{
      //}
    }//else of "if(nNumArgs>2)"
    MyXml.CloseAlltags();//Includes "sentences"-tag
  }
}

void CVTransApp::ShowInvalidVocabularyFileFormatMessage(
  const VTrans::string_type & strWordFile,
  DWORD dwOffsetOfBeginOfEntry,
  DWORD dwOffset
  )
{
  //#ifdef _WINDOWS
//  if( ! pWordNodeCurrent->m_pWordNodeNext->m_pWord)
  char * pbCurrentDirectory = new char[10001];
  if( pbCurrentDirectory)
  {
    CString strMessage;
    GetCurrentDirectory(10001, pbCurrentDirectory);
    strcat(pbCurrentDirectory,"\\");
    strcat(pbCurrentDirectory, strWordFile.GetBuffer(strWordFile.
      GetLength()));
    strMessage.Format( _T("Die Datei %s \nenthält kein gültiges \
  Format oder sonstiger Fehler zwischen\nOffset (=Position in Byte ab \
  Dateibeginn) %u (dezimal)\nund Offset (=Position in Byte ab Dateibeginn) \
  %u (dezimal) .\nDas Laden der Vokabeln wird beendet. Versuchen Sie, den \
  Fehler in der Dateistruktur zu beheben."),
    pbCurrentDirectory, dwOffsetOfBeginOfEntry, dwOffset);
    AfxMessageBox(strMessage, MB_OK, 0);
    delete [] pbCurrentDirectory;//Speicher an das Betriebssystem freigeben
//    break;
  }
  //#endif //#ifdef _WINDOWS
}

void UI::ShowWordFileError(DWORD dwOffsetOfBeginOfEntry, DWORD dwOffsetOfEndOfEntry)
{
  char * pbCurrentDirectory=new char[10001];
  CString strMessage;
  GetCurrentDirectory(10001,pbCurrentDirectory);
  strMessage.Format(_T("Die Datei %s \nenth�lt kein g�ltiges "
    "Format oder sonstiger Fehler zwischen Offset %u und Offset %u.\n "
    "Das Laden der Vokabeln wird beendet. "
    "Versuchen Sie den Fehler in der Dateistruktur zu beheben"),
    pbCurrentDirectory,dwOffsetOfBeginOfEntry,dwOffsetOfEndOfEntry);
  //if(
  AfxMessageBox(strMessage,MB_OKCANCEL,0) //== MB_CANCEL)
    ;
  delete pbCurrentDirectory;//Speicher an das Betriebssystem freigeben
}
