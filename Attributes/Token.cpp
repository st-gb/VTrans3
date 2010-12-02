//Ohne das folgende include: fatal error C1010: Unerwartetes Dateiende w�hrend der Suche nach dem vorkompilierten Header.
#include "StdAfx.h"
#include "rest.h" //for CStringVector etc.
#include "Token.h"
#include <vector>
//SUPPRESS_UNUSED_VARIABLE_WARNING(...)
#include <preprocessor_macros/suppress_unused_variable.h>

extern std::ofstream ofstreamLogFile; //for "LOGN" macro

//void Append(VTrans_string_typeVector & destinationCStringVector,const VTrans_string_typeVector & append)
//{
//	TRACE("Append ANFANG\n");
//	for(unsigned int i=0;i<append.size();i++)
//	{
//		destinationCStringVector.push_back(append.at(i));
//	}
//	TRACE("Append ENDE\n");
//}

/*void Append(CObjectVector & destinationCStringVector,const CObjectVector & append)
{
	TRACE("Append ANFANG\n");
	for(int i=0;i<append.size();i++)
	{
		destinationCStringVector.push_back(append.at(i));
	}
	TRACE("Append ENDE\n");
}*/

//void Append(std::vector<VTrans_string_typeVector> & destinationCStringVector,
//  const std::vector<VTrans_string_typeVector> & append)
//{
//	TRACE("Append ANFANG\n");
//	for(DWORD i=0;i<append.size();i++)
//	{
//		destinationCStringVector.push_back(append.at(i));
//	}
//	TRACE("Append ENDE\n");
//}

//BYTE Compare(const VTrans_string_typeVector & cstrvec1,const VTrans_string_typeVector & cstrvec2)
//{
//	if(cstrvec1.size()==cstrvec2.size())
//	{
//		//TRACE("Die Gr��en sind gleich.\n");
//		BOOL bEqual=TRUE;
//		for(DWORD i=0;i<cstrvec1.size();i++)
//		{
//			//TRACE("cstrvec1.at(i): %s cstrvec2.at(i): %s\n",cstrvec1.at(i),cstrvec2.at(i));
//			if(cstrvec1.at(i)!=cstrvec2.at(i))
//			{
//				bEqual=FALSE;
//				break;
//			}
//		}
//		if(bEqual)
//		{
//			TRACE("Compare - ENDE -return TRUE\n");
//			return TRUE;
//		}
//	}
//	return FALSE;
//}

//int CountTokens(CString text)
//{
//	int count =0;
//	BOOL flag=FALSE;
//	char ch;
//	int i=0;
//	for(;i<text.GetLength();i++)
//	{
//		ch=text.GetAt(i);
//		if(ch!=' ' && ch!='!' && ch!='.' && ch!='\n')
//		{
//			if(!flag)
//			{
//				flag=TRUE;
//				count++;
//			}
//		}
//		else
//		{
//			flag=FALSE;
//		}
//	}
//	return count;
//}

//CString FormatGermanSentence(VTrans_string_typeVector germanSentence)
//{
//	CString retStr;
//	for(DWORD i=0;i<germanSentence.size();i++)
//	{
//		retStr+=germanSentence.at(i);
//		if(i+1<germanSentence.size())
//		{
//			CString str=germanSentence.at(i+1);
//			if(str!="," && str!=".")
//				retStr+=" ";
//		}
//	}
//	return retStr;
//}

//CString GetTokenAt(CString str,int index)
//{
//	BOOL flag=FALSE;
//	int count=0;
//	int start=0;
//	int i=0;
//	for(;i<str.GetLength();i++)
//	{
//		if(str[i]==' ' || str[i]=='.')
//		{
//			if(count++==index)
//				return str.Mid(start,i-start);
//			start=i+1;
//		}
//	}
//	return CString("");
//}

//CString GetBetween(CString str,int first,int last)
//{
//	BOOL flag=FALSE;
//	int count=0;
//	int start=0;
//	int i=0;
//	CString ret_str;
//	for(;i<str.GetLength();i++)
//	{
//		if(str[i]!=' ' && str[i]!='.')
//		{
//			if(!flag)
//				start=i;
//			flag=TRUE;
//		}
//		else
//			if(flag)
//			{
//				if(count>=first && count<=last)
//				{
//					ret_str+=str.Mid(start,i-start)+" ";
//				}
//				count++;
//				if(count==last+1)
//					return ret_str;
//				flag=FALSE;
//			}
//	}
//	return CString("");
//}

VTrans_string_typeVector GetBetween(
  const VTrans_string_typeVector & cstrv,
  int first,
  int last
  )
{
//	TRACE("GetBetween ANFANG\n");
	VTrans_string_typeVector retCStrVec;
	//In MFC/Windows the TRACE macro has a variable # of args--I do
	//not know how to enable variable numbers of args for the same macro name.
	//So I exclude it from non-Windows.
#ifdef _WINDOWS
	TRACE("first: %d\t last: %d\n",first,last);
	TRACE("last-first: %d\n",last-first);
#endif
	for(DWORD i=first;i<cstrv.size() && i<=(DWORD)last;)
	{
		//In MFC/Windows the TRACE macro has a variable # of args--I do
		//not know how to enable variable numbers of args for the same macro name.
		//So I exclude it from non-Windows.
#ifdef _WINDOWS
		TRACE("cstrv.at(i): %s\n",cstrv.at(i));
#endif
		retCStrVec.push_back(cstrv.at(i++));
	}

//	TRACE("GetBetween ENDE\n");
	return retCStrVec;
}

PositionStringVector GetBetween(
  const PositionStringVector & pcstrv,
  int first,
  int last
  )
{
//  DEBUG_COUT("GetBetween ANFANG\n");
#ifdef _DEBUG
	int size = pcstrv.size() ;
	SUPPRESS_UNUSED_VARIABLE_WARNING(size)
#endif
	PositionStringVector retPCStrVec;
	#ifdef _DEBUG
//	printf("first: %d\t last: %d\n",first,last);
//	printf("last-first: %d\n",last-first);
#endif
//	DEBUG_COUT("pcstrv.size():" << pcstrv.size() << "\n")
	for(DWORD i=first;i<pcstrv.size() && i<=(DWORD)last;)
	{
//		#ifdef _DEBUG
//		printf("pcstrv.at(i): %s\n",pcstrv.at(i).m_Str.c_str() );
//    #endif
		PositionString pcstr=pcstrv.at(i++);
		retPCStrVec.push_back(pcstr);
	}
//	DEBUG_COUT("PositionCStringVector GetBetween(const PositionCStringVector & ")
//    "pcstrv,int first,int last) ENDE\n");
	return retPCStrVec;
}

//VTrans_string_typeVector GetBetweenAsCStringVector(
//  const PositionCStringVector & pcstrv,
//  int first,
//  int last
//  )
//{
//	#ifdef _DEBUG
//	printf("CStringVector GetBetweenAsCStringVector(const PositionCStringVector & pcstrv,int first,int last) ANFANG\n");
//#endif
//	VTrans_string_typeVector retCStrVec;
//	//In MFC/Windows the TRACE macro has a variable # of args--I do
//	//not know how to enable variable numbers of args for the same macro name.
//	//So I exclude it from non-Windows.
//#ifdef _WINDOWS
//	TRACE("first: %d\t last: %d\n",first,last);
//	TRACE("last-first: %d\n",last-first);
//#endif
//	for(DWORD i=first;i<pcstrv.size() && i<=(DWORD)last;)
//	{
//#ifdef _DEBUG
//		printf("pcstrv.at(i): %s\n",pcstrv.at(i).m_Str);
//#endif
//		PositionCString pcstr=pcstrv.at(i++);
//		retCStrVec.push_back(pcstr.m_Str);
//	}
//	#ifdef _DEBUG
//	printf("CStringVector GetBetweenAsCStringVector(const PositionCStringVector & pcstrv,int first,int last) ENDE\n");
//	#endif
//	return retCStrVec;
//}

std::string GetBetweenAsStdString(
  const PositionStringVector & rc_pstrv
  ,int first
  ,int last )
{
  std::string stdstr ;
  PositionStringVector pstrv = GetBetween(rc_pstrv,first,last) ;
//    last = pstrv.size() ;
  PositionStringVector::const_iterator c_iter = pstrv.begin() ;
  if( c_iter != pstrv.end() )
  {
    stdstr += c_iter->m_Str ;
    ++ c_iter ;
  }
  while( c_iter != pstrv.end() )
//    for( first = 0 ; first < last ; ++ first )
  {
    stdstr += " " + c_iter->m_Str ;
    ++ c_iter ;
  }
  return stdstr ;
}

//VTrans_string_typeVector GetCStringVector(CString str)
//{
//	//TRACE("GetCStringVector ANFANG\n");
//	//TRACE("str: %s\n",str);
//	VTrans_string_typeVector sv;
//	BOOL flag=FALSE;
//	int count=0;
//	int start=0;
//	int i=0;
//	for(;i<str.GetLength();i++)
//	{
////		if(str[i]==' ' || str[i]=='.')
//		if(str[i]==' ')// || i==str.GetLength()-1)
//		{
//			sv.push_back(str.Mid(start,i-start));
//			//if(count++==index)
////				return str.Mid(start,i-start);
//			start=i+1;
//		}
//		/*else if(i==str.GetLength()-1)
//		{
//			sv.push_back(str.Mid(start,str.GetLength()-start));
//		}*/
//	}
//	//TRACE("GetCStringVector ENDE\n");
//	return sv;
//}
//
//VTrans_string_typeVector GetCStringVector2(const CString & str)
//{
////	TRACE("GetCStringVector2 ANFANG\n");
//	VTrans_string_typeVector sv;
//	BOOL flag=FALSE;
//	int count=0;
//	int start=0;
//	int i=0;
//	for(;i<str.GetLength();i++)
//	{
//		if(str[i]==' ')
//		{
//			sv.push_back(str.Mid(start,i-start));
//			//if(count++==index)
////				return str.Mid(start,i-start);
//			start=i+1;
//		}
//		else if(i+1==str.GetLength())
//		{
//			sv.push_back(str.Mid(start,i-start+1));
//		}
//	}
//	//TRACE("GetCStringVector2 ENDE\n");
//	return sv;
//}
//
//std::vector<VTrans_string_typeVector> GetCStringVectorVector(const CString & str)
//{
//	std::vector<VTrans_string_typeVector> vecvecStr;
//	int nFirst=0;
//	for(int dwRun=0;dwRun<str.GetLength();dwRun++)
//	{
//		if(str.GetAt(dwRun)==' ' || dwRun==str.GetLength()-1)
//		{
//			VTrans_string_typeVector vecCString;
//			vecCString.push_back(str.Mid(nFirst,dwRun-nFirst));
//			vecvecStr.push_back(vecCString);
//			nFirst=dwRun+1;
//		}
//	}
//	return vecvecStr;
//}
//
//int GetNumToken(CString str)
//{
//	int i=0;
//	int count=0;
//	while(GetTokenAt(str,i++)!="")
//		count++;
//	return count;
//}
//
//PositionCStringVector GetPositionCStringVector(CString & str)
//{
//	TRACE("PositionCStringVector GetPositionCStringVector(CString & str) - ANFANG\n");
//	PositionCStringVector pv;
//	int start=0;
//	BYTE previousChar=' ';
//	BOOL bFirstChar=TRUE;
//	BOOL bLetterOccured=FALSE;
//	BOOL bDelimiter=FALSE;
//	BOOL bPunctuationMark=FALSE;
//	for(int i=0;i<str.GetLength();i++)
//	{
//		if(str[i]==',' || str[i]=='.')
//		{
//			//if(previousChar!=' ')
//				//str.Insert(i,' ');
//			bLetterOccured=FALSE;
//			//start=i+1;
//			bDelimiter=TRUE;
//			bPunctuationMark=TRUE;
//		}
//		else if(str[i]==' ')// || i==str.GetLength()-1)
//		{
//			bDelimiter=TRUE;
//			bLetterOccured=FALSE;
///*			if(previousChar==' ')
//			{
//				//str.Delete(--i,1);
//				TRACE("str:%s\n",str);
//				start=i+1;
//			}
//			else
//			{
//				pv.push_back(PositionCString(str.Mid(start,i-start),start,1));
//			}*/
//		}
//		else if(i==str.GetLength()-1)
//		{ // letzes Zeichen
//			bDelimiter=TRUE;
//		}
//		else
//		{
//			if(!bLetterOccured)
//			{
//				start=i;
//				bLetterOccured=TRUE;
//			}
//		}
//		if(bDelimiter)
//		{
//			if(i>0 && IsLetter(previousChar))
//				pv.push_back(PositionCString(str.Mid(start,i-start),start,1));
//			bDelimiter=FALSE;
//			if(bPunctuationMark)
//			{
//#pragma warning( disable : 2665 )//Fehler "unpassender Typ" deaktivieren
//				pv.push_back(PositionCString(//(CString)str[i]
//					CString(str[i]),(DWORD)start,(DWORD)1));
//#pragma warning(restore:C2665)
//        bPunctuationMark=FALSE;
//			}
//		}
//		previousChar=str[i];
//	}
//	for(DWORD j=0;j<pv.size();j++)
//	{
//		PositionCString pcstr=pv.at(j);
//#ifdef _DEBUG
//		printf("pcstr.m_Str :%s start: %u  length: %u\n",pcstr.m_Str,pcstr.m_dwStart,pcstr.m_dwLength);
//#endif
//	}
//	return pv;
//}
//
////diese Funktion liefert einen Vektor aus Token zur�ck.
////der Vektor wird aus einer Zeichenkette generiert.
////Ein Token beginnt entweder am Anfang der Zeichenkette oder nach dem letzten
////Leerzeichen, Punkt, Komma, Fragezeichen oder Ausrufezeichen
////Ein Token endet entweder am Ende der Zeichenkette oder wenn ein
////Leerzeichen, Punkt, Komma, Fragezeichen oder Ausrufezeichen auftritt
//PositionCStringVector GetPositionCStringVector(CString & str,DWORD dwAdd)
//{
//#ifdef _DEBUG
//	printf("PositionCStringVector GetPositionCStringVector(CString & str,DWORD dwAdd) - ANFANG\n");
//#endif
//	PositionCStringVector pv;
//	int start=0;
//	BYTE previousChar=' ';
//	BOOL bFirstChar=TRUE;
//	BOOL bLetterOccured=FALSE;
//	BOOL bDelimiter=FALSE;
//	BOOL bPunctuationMark=FALSE;
//	for(int i=0;i<str.GetLength();i++)
//	{
//		if(str[i]==',' || str[i]=='.' || str[i]=='?')
//		{
//			//if(previousChar!=' ')
//				//str.Insert(i,' ');
//			bLetterOccured=FALSE;
//			//start=i+1;
//			bDelimiter=TRUE;
//			bPunctuationMark=TRUE;
//		}
//		else if(str[i]==' ')// || i==str.GetLength()-1)
//		{
//			bDelimiter=TRUE;
//			bLetterOccured=FALSE;
//		}
//		else if(i==str.GetLength()-1)
//		{ // letzes Zeichen
//			bDelimiter=TRUE;
//		}
//		else // wenn ein Zeichen au�er '.' oder ',' oder '?' oder
//		{ // Leerzeichen und wenn nicht das Ende der Zeichenkette erreicht
//			if(!bLetterOccured) // wenn davor KEIN Buchstabe (Zeichen au�er '.'
//			{ //	oder ',' oder '?' ) vorkam
//				start=i;
//				bLetterOccured=TRUE;
//			}
//		}
//		if(bDelimiter)
//		{
//			//if(i>0 && IsLetter(previousChar))
//			if(i>0 && previousChar!=' ' && previousChar!='.' && previousChar!='!'
//				&& previousChar!='?' && previousChar!=',')
//				pv.push_back(PositionCString(str.Mid(start,i-start),dwAdd+start,1));
//			bDelimiter=FALSE;
//			if(bPunctuationMark)
//			{
//				pv.push_back(PositionCString((CString)str[i],dwAdd+start,1));
//				bPunctuationMark=FALSE;
//			}
//		}
//		previousChar=str[i];
//	}
//#ifdef _DEBUG
//	for(DWORD j=0;j<pv.size();j++)
//	{
//		PositionCString pcstr=pv.at(j);
//		printf("pcstr.m_Str :%s start: %u  length: %u\n",pcstr.m_Str,
//      pcstr.m_dwStart,pcstr.m_dwLength);
//	}
//#endif
//#ifdef _DEBUG
//	printf("PositionCStringVector GetPositionCStringVector(CString & str,DWORD dwAdd) - return pv\n");
//#endif
//	return pv;
//}
//
//BOOL IsFirstLetterUpper(CString & str)
//{
//	for(int i=0;i<str.GetLength();i++)
//	{
//		if(isalpha(str[i]))
//		{
//			CString tmp(str[i]);
//			tmp.MakeUpper();
//			LOGN("2008 08 14 22.52 :"<<str[i]<<";"<<tmp[0])
//			if(str[i]==tmp[0])
//				return TRUE;
//		}
//	}
//	return FALSE;
//}
//
//BOOL IsFirstUpper(CString str)
//{
//	CString tmp(str[0]);
//	CString tmp2(str[0]);
//	tmp.MakeUpper();
//	if(tmp2==tmp)
//	{
//		return TRUE;
//	}
//	return FALSE;
//}
//
//BOOL IsLetter(BYTE ch)
//{
//	if((ch>64 && ch <91) || (ch>96 && ch<123))
//		return TRUE;
//	return FALSE;
//}
//
//void MakeFirstLetterUpper(CString & str)
//{
//	for(int i=0;i<str.GetLength();i++)
//	{
//		if(isalpha(str[i]))
//		{
//			CString tmp(str[i]);
//			tmp.MakeUpper();
//			str.SetAt(i,tmp.GetAt(0));
//			break;
//		}
//	}
//}
//
//void MakeFirstLetterUpper(std::string & str)
//{
//  for(std::string::size_type i=0;i<str.size();i++)
//	{
//		if(isalpha(str[i]))
//		{
//			CString tmp(str[i]);
//			tmp.MakeUpper();
//			str[i]=tmp.GetAt(0);
//			break;
//		}
//	}
//}
//
//void MakeFirstLetterLower(CString & str)
//{
//	for(int i=0;i<str.GetLength();i++)
//	{
//		if(isalpha(str[i]))
//		{
//			CString tmp(str[i]);
//			tmp.MakeLower();
//			str.SetAt(i,tmp.GetAt(0));
//			break;
//		}
//	}
//}
//
//CString MakeFirstUpper(CString str)
//{
//	CString tmp(str[0]);
//	tmp.MakeUpper();
//	str.SetAt(0,tmp.GetAt(0));
//	return str;
//}
//
//CString Transform(CString str)
//{
//	TRACE("Transform Anfang\n");
//	BYTE previousChar=' ';
//	BOOL bFirstChar=TRUE;
//	BOOL bLetterOccured=FALSE;
//	for(int i=0;i<str.GetLength();i++)
//	{
//		if(!bFirstChar)
//		{
//			if(str[i]==',' || str[i]=='.' || str[i]=='?')
//			{
//				if(previousChar!=' ')
//					str.Insert(i,' ');
//			}
//			else if(str[i]==' ')
//			{
//				if(previousChar==' ')
//				{
//					str.Delete(--i,1);
//					//In MFC/Windows the TRACE macro has a variable # of args--I do
//					//not know how to enable variable numbers of args for the same macro name.
//					//So I exclude it from non-Windows.
//			#ifdef _WINDOWS
//					TRACE("str:%s\n",str);
//			#endif
//				}
//			}
//			else
//			{
//				if(previousChar==' ' && !bLetterOccured)
//					str.Delete(--i,1);
//				if(previousChar==',')
//					str.Insert(i,' ');
//				bLetterOccured=TRUE;
//			}
//		}
//		if(str[i]!='.' && str[i]!='?' && str[i]!=',' && str[i]!=' ')
//			bLetterOccured=TRUE;
//		bFirstChar=FALSE;
//		previousChar=str[i];
//	}
//	//In MFC/Windows the TRACE macro has a variable # of args--I do
//	//not know how to enable variable numbers of args for the same macro name.
//	//So I exclude it from non-Windows.
//#ifdef _WINDOWS
//	TRACE("str:%s\n",str);
//#endif
//	return str;
//}
//
//    PositionstdstringVector Tokenize(const CString & str)
//    {
//	    PositionstdstringVector posstdstrvec;
//	    int start=0;
//	    BYTE previousChar=' ';
//	    BOOL bFirstChar=TRUE;
//	    BOOL bLetterOccured=FALSE;
//	    BOOL bDelimiter=FALSE;
//	    BOOL bPunctuationMark=FALSE;
//	    for(int i=0;i<str.GetLength();i++)
//	    {
//        switch(str[i])
//        {
//        case ',' :
//        case '.' :
//        case '?' :
//			    bLetterOccured=FALSE;
//			    //start=i+1;
//			    bDelimiter=TRUE;
//			    bPunctuationMark=TRUE;
//          break ; //Leave switch() (else the foll. instr. are executed.
//        case ' ' :// || i==str.GetLength()-1)
//			    bDelimiter=TRUE;
//			    bLetterOccured=FALSE;
//          break ; //Leave switch() (else the foll. instr. are executed.
//        }
//		    if(!bDelimiter && i==str.GetLength()-1)
//		    { // letzes Zeichen
//			    bDelimiter=TRUE;
//		    }
//		    else // wenn ein Zeichen au�er '.' oder ',' oder '?' oder
//		    { // Leerzeichen und wenn nicht das Ende der Zeichenkette erreicht
//			    if(!bLetterOccured) // wenn davor KEIN Buchstabe (Zeichen au�er '.'
//			    { //	oder ',' oder '?' ) vorkam
//				    start=i;
//				    bLetterOccured=TRUE;
//			    }
//		    }
//		    if(bDelimiter)
//		    {
//			    //if(i>0 && IsLetter(previousChar))
//			    if( i > 0 )
//          {
//            switch(previousChar )
//            {
//            case ' ' :
//            case '.' :
//            case '!' :
//            case '?' :
//            case ',' :
//              break;
//            default:
//              posstdstrvec.push_back(
//                Positionstdstring( std::string( (LPCTSTR)
//                  str.Mid(start,i-start) ),
//                  //dwAdd + start,1
//                  start,1
//                  )
//                );
//            }
//          }
//			    bDelimiter = FALSE;
//			    if(bPunctuationMark)
//			    {
//				    posstdstrvec.push_back(
//              Positionstdstring( //std::string(str[i]),
//                //Because there is no std::string(char) constuctor
//                std::string(1,str[i]),
//                //dwAdd+start
//                start,1));
//				    bPunctuationMark=FALSE;
//			    }
//		    }
//		    previousChar=str[i];
//	    }//for-loop
//      return posstdstrvec ;
//    } //static Tokenize(const CString & str)
