/*
 * GermanWord.cpp
 *
 *  Created on: 31.07.2011
 *      Author: Stefan
 */
#include "GermanWord.hpp"

//static var definitions.
const char * const GermanVerb::presentPersonEndings [] = {
  "e" //gehE
  , "st" //gehST
  , "t" //er gehT
  , "en" //wir gehEN
  , "t" //ihr gehT
  , "en"//sie gehEN
  };
const char * const GermanVerb::presentPersonEndings2 [] = {
  "" //ich arbeite
  , "st" //du arbeiteST
  , "t" //er arbeiteT
  , "n" //wir arbeiteN
  , "t" //ihr arbeiteT
  , "en" //wir arbeiteN
  };
const char * const GermanVerb::pastPersonEndings [] = {
  "" //ging, arbeitete
  , "st" //gingST, arbeiteteST
  , "" //er ging, arbeitete
  , "en" //gingEN, arbeitetEN
  , "t" //ihr gingT, arbeiteteT
  , "en" //sie gingEN, arbeitetEN
  };

GermanAuxiliaryVerb::GermanAuxiliaryVerb(const VTrans::string_type & str)
{
  WORD i=0;
  int start=0;
  WORD wIndex=0;
  m_bIntegral=TRUE;
  while( i < str.length() )
  {
    if(str[i]=='\n')
    {
      m_strWords[wIndex++] = //str.Mid(start,i-start);
        str.substr( start ,i - start );
      start = i + 1;
    }
    i++;
  }
}

bool GermanAuxiliaryVerb::GetNextString( std::string & r_stdstr )
{
  bool bSucc = false ;
  if( m_byIndex < 15 )
  {
    r_stdstr = m_strWords [ m_byIndex ++ ] ;
    bSucc = true ;
  }
  return bSucc ;
}

void GermanVerb::CreateFromString(const VTrans::string_type & str)
{
  WORD i=0;
  int start=0;
  WORD wIndex=0;
  m_bIntegral=TRUE;
  while( i < str.length() )
  {
    if( wIndex == 14 )
      m_bCase = str[i] - 50 ;
    if( str[i] == '\n' )
    {
      m_strWords[wIndex++] = //str.Mid(start,i-start);
        str.substr(start,i-start);
      start=i+1;
    }
    i++;
  }
  m_bAuxiliaryVerb=TRUE;
  m_bReflexive=FALSE;
}

GermanVerb::GermanVerb(const VTrans::string_type & str)
{
  CreateFromString(str) ;
}

GermanVerb::GermanVerb(const VTrans::string_type & str , e_case cas)
{
  CreateFromString(str) ;
  m_bCase = cas ;
}
