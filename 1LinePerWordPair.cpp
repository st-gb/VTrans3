#include "StdAfx.h" //wenn nicht includiert und unter Visual C++: Fehler "C1010"
#include "rest.h" //for LOGN() etc.
#include "IO.h"
#include "UI.h"
#include "1LinePerWordPair.h"
#include "VocabularyInMainMem/DoublyLinkedList/WordNode.hpp"
#include "VocabularyInMainMem/DoublyLinkedList/WordList.hpp"

extern std::ofstream ofstreamLogFile; //for LOGN(...)
extern WordList wordList;

void OneLinePerWordPair::LoadWords(
  const std::string & cr_stdstrDictionaryFile
  , WordNode * pWordNodeCurrent)
{
  FILE *f;
  int i;
  BOOL break_while=FALSE;
  BOOL flag=FALSE;
  CString concatenate;
  //CString strWordFile=_T(//"http://www.f4.fhtw-berlin.de/~s0518039/"
  //  "words.txt");
  WordNode * pWordNodePrevious=NULL;

  //zuerst die integralen Vokabeln der verketteten Liste hinzufügen, Ende
  LOGN("05.06.2008 22.22.17");
  if( ( f = fopen(//strWordFile
        cr_stdstrDictionaryFile.c_str() ,"rb") 
      ) != NULL 
    )
  {
    LOGN("05.06.2008 22.22.26");
	  concatenate="";
	  BYTE bEnglishWord=TRUE;
	  CString str;
	  DWORD dwOffset=0;
	  DWORD dwOffsetOfBeginOfEntry=0;//Offset (Position) des Anfanges eines 
	  //Vokabel-Eintrages
	  while( ( i = getc(f)) != -1 )
	  {
      switch( (char) i )
      {
        case '0':
		    {
				  int ret=0;
				  pWordNodePrevious=pWordNodeCurrent;
				  //pWordNodeCurrent->m_pWordNodeNext=new WordNode();
				  //pWordNodeCurrent->m_pWordNodeNext->m_pWord=
            extract(str,TRUE,ret);
          //Word * p_word = FileFormatFrom2001::extract(str,TRUE,ret);
          //m_vocabularyinmainmem.insert( p_word ) ;
				  //TODO also provide an error message for non-Windows binaries. 
//#ifdef _WINDOWS
//				  if(!pWordNodeCurrent->m_pWordNodeNext->m_pWord)
//				  {
////					  char * pbCurrentDirectory=new char[10001];
////					  CString strMessage;
////					  GetCurrentDirectory(10001,pbCurrentDirectory);
////					  strcat(pbCurrentDirectory,"\\");
////					  strcat(pbCurrentDirectory,strWordFile.GetBuffer(strWordFile.
////						  GetLength()));
////					  strMessage.Format(_T("Die Datei %s \nenthält kein gültiges \
////Format oder sonstiger Fehler zwischen\nOffset (=Position in Byte ab \
////Dateibeginn) %u (dezimal)\nund Offset (=Position in Byte ab Dateibeginn) \
////%u (dezimal) .\nDas Laden der Vokabeln wird beendet. Versuchen Sie, den \
////Fehler in der Dateistruktur zu beheben."),
////					  pbCurrentDirectory,dwOffsetOfBeginOfEntry,dwOffset);
////					  AfxMessageBox(strMessage,MB_OK,0);
//            UI::ShowWordFileError(dwOffsetOfBeginOfEntry, dwOffset) ;
//            //To fullfill the loop break criteria.
//            fseek(f,0,SEEK_END) ;
//					  //delete pbCurrentDirectory;//Speicher an das Betriebssystem freigeben
//					  break;
//				  }
//#endif //#ifdef _WINDOWS
				  //pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
				  //pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;
				  if(ret==-1)
				  {
					  break;
				  }
				  str="";
          dwOffsetOfBeginOfEntry=dwOffset+1;
			  }//case
        break ;
        case '\n': // deutsch
			  {
				  int ret=0;
				  pWordNodePrevious=pWordNodeCurrent;
				  pWordNodeCurrent->m_pWordNodeNext=new WordNode();
				  pWordNodeCurrent->m_pWordNodeNext->m_pWord=extract(str,FALSE,ret);
				  //TODO also provide an error message for non-Windows binaries. 
//#ifdef _WINDOWS
//				  if(!pWordNodeCurrent->m_pWordNodeNext->m_pWord)
//				  {
//            UI::ShowWordFileError(dwOffsetOfBeginOfEntry, dwOffset) ;
//            //To fullfill the loop break criteria.
//            fseek(f,0,SEEK_END) ;
//
//					  //delete pbCurrentDirectory;//Speicher an das Betriebssystem freigeben
//					  break;
//				  }
//#endif //#ifdef _WINDOWS
				  //pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
				  //pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;
				  if(ret!=0)
					  //MessageBox("Datei %s enthält kein gültiges Format oder sonstiger Fehler","Fehler beim Lesen der Datei vocs.txt",MB_OK);
					  break;
				  str="";
          dwOffsetOfBeginOfEntry=dwOffset+1;
			  }//case '\r'
        break ;
        default:
		    {
			    str+=(char)i;
		    }
		  }//switch
		  ++dwOffset;
	  }//End of while loop.
	  fclose(f);
	  if(pWordNodeCurrent!=NULL)
	  {
		  pWordNodeCurrent->m_pWordNodeNext=NULL;
	  }
  }
#ifdef _WINDOWS
  else
  {
	  char * pbCurrentDirectory=new char[10001];
	  GetCurrentDirectory(10001,pbCurrentDirectory);
	  CString strMessage = _T("Die Datei ") + CString(pbCurrentDirectory) +
      //strWordFile +
      cr_stdstrDictionaryFile.c_str() +
	  _T(//"\\words.txt
      "\nkonnte nicht geöffnet werden.\nDiese Datei enthält \
das Englisch-Deutsch-Wörterbuch.\nWenn diese Datei nicht geladen wird, \
gibt es anfänglich keine Vokabeln.\nMögliche Ursachen:\
 \n 1. sie existiert nicht\n 2. Fehler beim Öffnen der Datei");
	  AfxMessageBox(strMessage,MB_OK,0);
  }
#endif
  wordList.m_pWordNodeLast=pWordNodeCurrent;
  wordList.m_pWordNodeLast->m_pWordNodeNext=NULL;
#ifdef _DEBUG
  printf("void LoadWords(WordNode * pWordNode) ENDE\n");
#endif
}
