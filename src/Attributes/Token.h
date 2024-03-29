#if !defined (TOKEN_H_INCLUDED)
	    #define TOKEN_H_INCLUDED

  #include <vector> //class std::vector
  #include <string>
//  #include "rest.h" //for VTrans_string_typeVector etc.
  #include <Controller/string_type.hpp> //typedef VTrans::string_type
  //#ifndef _WINDOWS
  //	#include <typedefs.h> //for DWORD etc.
  //	#include <StdString.h> //for class compatible to MFC's "CString"  
  //#endif//#ifndef _WINDOWS

  #include <Attributes/PositionString.hpp>

  typedef std::vector<VTrans::string_type> VTrans_string_typeVector;

  //Use std::string to be more (Linux) compiler compatible.
  class Positionstdstring
  {
  public:
	  BYTE m_color;
	  DWORD m_dwLength;
	  DWORD m_dwStart;
    //std::string m_str;
    //Use the same attribute name as "PositionString": so it's easier to
    //replace "PositionString" by "Positionstdstring" when no 
    //"rename refactoring" is available.
    std::string m_Str;
	  Positionstdstring(std::string & str,DWORD dwStart,DWORD dwLength)
	  {
		  m_Str=str;
		  m_dwStart=dwStart;
		  m_dwLength=dwLength;
	  };
  }; // end class class Positionstdstring

  //Use std::string to be more (Linux) compiler compatible.
  typedef	std::vector<Positionstdstring> PositionstdstringVector;

  void												Append(
    VTrans_string_typeVector &
    ,const VTrans_string_typeVector & );
  void Append(
    std::vector<VTrans_string_typeVector> & destinationCStringVector,
    const std::vector<VTrans_string_typeVector> & append ) ;
  BYTE												Compare(
    const VTrans_string_typeVector &
    , const VTrans_string_typeVector & );
  VTrans::string_type					FormatGermanSentence( 
    VTrans_string_typeVector germanSentence );
  VTrans::string_type					GetBetween( 
    int first
    , int last);
  VTrans::string_type					GetBetween(
    VTrans::string_type
    , int first
    , int last );
  VTrans_string_typeVector		GetBetween(
    const VTrans_string_typeVector & sv
    , int first
    , int last
    );
  VTrans_string_typeVector		GetBetweenAsCStringVector(
    const PositionStringVector & pcstrv
    ,int first
    ,int last );
//  std::string GetBetweenAsStdString(
//    const PositionStringVector & rc_pstrv
//    ,int first
//    ,int last ) ;
  VTrans_string_typeVector		GetCStringVector( VTrans::string_type str);
  VTrans_string_typeVector		GetCStringVector2( const VTrans::string_type & str);
  std::vector<VTrans_string_typeVector>	GetCStringVectorVector(
    const VTrans::string_type & str);
  PositionStringVector GetPositionCStringVector(VTrans::string_type & str) ;
  PositionStringVector       GetPositionCStringVector(
    VTrans::string_type & str
    , DWORD dwAdd ) ;
  BOOL										    IsFirstLetterUpper(VTrans::string_type & str);
  BOOL												IsFirstUpper(VTrans::string_type str); // ersetzt das erste Zeichen einer Zeichenkette durch einen Gro�buchstaben
  BOOL												IsLetter(BYTE ch);
  void												MakeFirstLetterUpper(VTrans::string_type &);
  void                        MakeFirstLetterUpper(std::string & str);
  void												MakeFirstLetterLower(VTrans::string_type & str);
  VTrans::string_type					MakeFirstUpper(VTrans::string_type str); // ersetzt das erste Zeichen einer Zeichenkette durch einen Gro�buchstaben
  VTrans::string_type					MakeFirstLower(VTrans::string_type str); // ersetzt das erste Zeichen einer Zeichenkette durch einen Gro�buchstaben
  PositionstdstringVector     Tokenize(const VTrans::string_type & str) ;
#endif //!defined (TOKEN_H_INCLUDED)
