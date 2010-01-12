#if !defined (TOKEN_H_INCLUDED)
	    #define TOKEN_H_INCLUDED

  #include <vector>
  #include <string>
  #include "rest.h" //for CStringVector etc.
  //#ifndef _WINDOWS
  //	#include <typedefs.h> //for DWORD etc.
  //	#include <StdString.h> //for class compatible to MFC's "CString"  
  //#endif//#ifndef _WINDOWS

  class PositionCString
  {
  public:
	  BYTE m_color;
	  DWORD m_dwLength;
	  DWORD m_dwStart;
	  CString m_Str;
	  PositionCString(CString str,DWORD dwStart,DWORD dwLength)
	  {
		  m_Str=str;
		  m_dwStart=dwStart;
		  m_dwLength=dwLength;
	  };
  };

  //Use std::string to be more (Linux) compiler compatible.
  class Positionstdstring
  {
  public:
	  BYTE m_color;
	  DWORD m_dwLength;
	  DWORD m_dwStart;
    //std::string m_str;
    //Use the same attribute name as "PositionCString": so it's easier to
    //replace "PositionCString" by "Positionstdstring" when no 
    //"rename refactoring" is available.
    std::string m_Str;
	  Positionstdstring(std::string & str,DWORD dwStart,DWORD dwLength)
	  {
		  m_Str=str;
		  m_dwStart=dwStart;
		  m_dwLength=dwLength;
	  };
  }; // end class class Positionstdstring

  typedef	std::vector<PositionCString> PositionCStringVector;
  //Use std::string to be more (Linux) compiler compatible.
  typedef	std::vector<Positionstdstring> PositionstdstringVector;

  void												Append(CStringVector &,const CStringVector &);
  void Append(
    std::vector<CStringVector> & destinationCStringVector,
    const std::vector<CStringVector> & append) ;
  BYTE												Compare(const CStringVector &,const CStringVector &);
  CString											FormatGermanSentence(CStringVector germanSentence);
  CString											GetBetween(int first,int last);
  CString											GetBetween(CString,int first,int last);
  CStringVector								GetBetween(CStringVector sv,int first,int last);
  PositionCStringVector				GetBetween(const PositionCStringVector & sv,
															  int first,int last);
  CStringVector								GetBetweenAsCStringVector(const 
															  PositionCStringVector & pcstrv,int first,int last);
  CStringVector								GetCStringVector(CString str);
  CStringVector								GetCStringVector2(const CString & str);
  std::vector<CStringVector>	GetCStringVectorVector(const CString & str);
  PositionCStringVector GetPositionCStringVector(CString & str) ;
  PositionCStringVector       GetPositionCStringVector(CString & str,DWORD dwAdd) ;
  BOOL										    IsFirstLetterUpper(CString & str);
  BOOL												IsFirstUpper(CString str); // ersetzt das erste Zeichen einer Zeichenkette durch einen Groﬂbuchstaben
  BOOL												IsLetter(BYTE ch);
  void												MakeFirstLetterUpper(CString &);
  void                        MakeFirstLetterUpper(std::string & str);
  void												MakeFirstLetterLower(CString & str);
  CString											MakeFirstUpper(CString str); // ersetzt das erste Zeichen einer Zeichenkette durch einen Groﬂbuchstaben
  CString											MakeFirstLower(CString str); // ersetzt das erste Zeichen einer Zeichenkette durch einen Groﬂbuchstaben
  PositionstdstringVector     Tokenize(const CString & str) ;
#endif //!defined (TOKEN_H_INCLUDED)
