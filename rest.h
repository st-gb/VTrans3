#if !defined(REST_H)
		 #define REST_H
  #include <fstream> //for ofstream

  #define USE_LETTER_TREE

  //#ifndef _WINDOWS
//  #ifndef _MSC_VER //MS Compiler
//	  #include <typedefs.h> //for DWORD etc.
//	  #include <StdString.h> //for class compatible to MFC's "CString"
//  #endif//#ifndef _WINDOWS

  //Diese Header-Datei ist daf�r da, um unendliche Rekursionen (die Compiler-Fehler verursachen) 
  //zu vermeiden und f�r Deklarationen, die sonst inhaltlich nicht in eine 
  //andere Header-Datei passen.

  //Unendliche Rekursion entsehen dadurch, dass eine 2. Datei, die von der 1. 
  //Datei per "#include" eingebunden wird, die 1. Datei per "#include" 
  //einbindet, weil bei "#include" der Pr�prozessor den gesamten Inhalt der 
  //angegebenen Datei an diese Stelle kopiert.

  //volatile BYTE g_bContinueLoading=TRUE;
  ////WordNode * m_first=NULL;
  //volatile BYTE g_bContinue=TRUE;
  //volatile DWORD g_dwStatus=0;
  //volatile DWORD g_dwSentenceStatus=0;
  //volatile DWORD g_dwTotal=0;
  //volatile DWORD g_dwTranslated=0;
  //volatile DWORD g_dwTokensInSentence=0;
  #include <vector> //for typedef	std::vector<CString> CStringVector;
  #ifdef _WINDOWS
	  #include <afxstr.h> //for class CString
  #endif

  //Says that an additional array of pointers with same size as number 
  //of strings should be allocated.
  #define COMPILE_WITH_REFERENCE_TO_LAST_LETTER_NODE

  //Flags/Schalter/Optionen f�r Ausgangstext(werden in Parse...(...) �bergeben), Anfang
  #define CASE_SENSITIVE 1
  #define ADOPT_GRAMMATICALLY_INCORRECT_SENTENCES 2
  #define ALLOW_LOW_LETTER_AT_THE_BEGINNING_OF_SENTENCE 4
  #define DO_NOT_PARSE_SUBJECT_FOR_REL_CLAUSES 2147483648//bin�r:10000000000000000000000000000000
  #define PREDICATE_VERB_ALLOWS_GERUND 1073741824
  #define LEAVE_OUT_1ST_OBJECT 536870912 
  #define ENUMERATION_ELEMENT_CONTAINS_REL_CLAUSE 128
  #define THIRD_PERSON_SINGULAR 3
  #define ENUMERATION_ELEMENT_CONTAINS_PROPER_NAME 128
  #define CONTAINS_FIRST_TOKEN_OF_SENTENCE 256//bin�r: 100000000
  //Flags, Ende

  //std::ofstream ofstreamLogFile;//(LOG_FILE_PATH);

  typedef	std::vector<CString> CStringVector;

  class IntPair
  {
  public:
	  int m_n1;
	  int m_n2;
  };

  class Range
  {
  public:
	  DWORD m_dwStart;
	  DWORD m_dwEnd;
	  Range(){m_dwStart=0; m_dwEnd=0;};
  };

  //"seems" because: A sentence may be grammatically correct but because of 
  //missing words VTrans regards it as incorrect.
  #define SENTENCE_SEEMS_TO_BE_GRAMMATICALLY_INCORRECT 0
  #define SENTENCE_IS_GRAMMATICALLY_CORRECT 1

  class SentenceAndValidityAndProperName
  {
  public:
    BYTE byReturnOfSentenceParsing;
    std::vector<CStringVector> vecvecstrSentenceTokens;
    std::vector<Range> vecrangeProperName;
    //This vector is therefore:# wenn das ausgew�hlte Element einer Drop-Down-Box, die die Elemente "du","Sie" beinhaltet, ge�ndert wird, sollte sich auch die Auswahl der damit zusammenh�ngenden anderen Drop-Down-Box (z.B. die finite Verbform ) �ndern
    std::vector<IntPair> m_vecintpairConntectedWords;

    SentenceAndValidityAndProperName(){
      byReturnOfSentenceParsing=SENTENCE_SEEMS_TO_BE_GRAMMATICALLY_INCORRECT;
    };
    SentenceAndValidityAndProperName(
      BYTE byReturnOfSentenceParsing,
      const std::vector<CStringVector> & vecvecstrSentenceTokens,
      const std::vector<Range> & vecrangeProperName,
      const std::vector<IntPair> & vecintpairConntectedWords)
    {
      this->byReturnOfSentenceParsing=byReturnOfSentenceParsing;
      this->vecvecstrSentenceTokens=vecvecstrSentenceTokens;
      this->vecrangeProperName=vecrangeProperName;
      m_vecintpairConntectedWords=vecintpairConntectedWords;
    };
  };


  #define FILE_PATH_FOR_INTERRUPTED_TRANSLATION "int_resp_crashed_trans_exe.txt"

  #define _LOG
  //#define _DEBUG
  //#define STEFAN_DEBUG
  //FILE * logfile=NULL;

  //#define FILE_PATH_FOR_INTERRUPTED_TRANSLATION "int_resp_crashed_trans_exe.txt"

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
    #include <fstream>
    //std::ofstream ofstreamLogFile(LOG_FILE_PATH);
    #define LOG(var) ofstreamLogFile<< var; \
      /*fflush is important to ensure the last output was REALLY written to medium*/\
      ofstreamLogFile.flush();
    #define LOGN(var) ofstreamLogFile<< var<<"\n"; \
      /*fflush is important to ensure the last output was REALLY written to medium*/\
      ofstreamLogFile.flush();
    //Achtung: der Compiler gibt keinen Fehler aus, wenn nur 1 Parameter �bergeben wird!
    //->mit Vorsicht einsetzen
    #define LOG2N(outstreamRelease,outstreamDebug) ofstreamLogFile<< outstreamRelease<<"\n"; \
      /*fflush is important to ensure the last output was REALLY written to medium*/\
      ofstreamLogFile.flush();

  #else
  /*If "_LOG" is not defined, replace any "LOG","LOG1","LOG2" by "" 
  (->so NO overhead/ressources are claimed).*/
    #define LOG
    #define LOG1
    #define LOG2
    //#ifdef _DEBUG
    //  #define LOG2N(outstreamRelease,outstreamDebug) ofstreamLogFile<< outstreamDebug<<"\n"; \
    //    /*fflush is important to ensure the last output was REALLY written to medium*/\
    //    ofstreamLogFile.flush();
    //#endif//#ifdef _DEBUG
  #endif//ifdef _LOG

  //#include <atlstr.h>//This include is needed for "CString" when compiling as DLL.

  //#define SUBJECT 0
  //#define PREDICATE 1
  //#define OBJECT 2
  //class CStringVector
  //{
  //  BYTE m_bySentenceElementType;
  //  std::vector<CString> m_vecstr;
  //  
  //  CStringVector(){m_bySentenceElementType=255;}
  //  
  //  void push_back(const CString & str){
  //    m_vecstr.push_back(str); 
  //  }
  //  void push_back(const CString & str,bySentenceElementType){
  //    m_vecstr.push_back(str); 
  //    m_bySentenceElementType=bySentenceElementType;
  //  }
  //  CString at(int nIndex){return m_vecstr.at(nIndex)};
  //};

  #define STRINGS_ARE_IDENTICAL 0

#endif //#if !defined(REST_H)
