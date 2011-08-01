/*
 * EnglishWord.cpp
 *
 *  Created on: 31.07.2011
 *      Author: Stefan
 */
#include "EnglishWord.hpp"
#include "Word.hpp" //IsVowel(...), IsConsonant(...)

EnglishAuxiliaryVerb::EnglishAuxiliaryVerb(
  //const CString & str,
  const VTrans::string_type & str,
  bool bModalAuxiliary
  )
{
  WORD i=0;
  int start=0;
  WORD wIndex=0;
  m_bIntegral=TRUE;
  while( i < str.length() )
  {
    if( str[i] == '\n' )
    {
      m_strWords[wIndex++] = //str.Mid(start,i-start);
        str.substr( start, i - start ) ;
      start = i + 1 ;
    }
    i++ ;
  }
  //ein modales Hilfsverb wird NICHT f�r die �berstzung
  //unbedingt ben�tigt f�r einige Zeitformen (im Gegensatz
  //zu "to be" f�r fortschreitenden Pr�sens ("I am working.")
  m_bIntegral = ! bModalAuxiliary ;
}

//void EnglishAuxiliaryVerb::InitGetNextString( )
//{
//  m_byIndex = 0 ;
//}

bool EnglishAuxiliaryVerb::GetNextString( std::string & r_stdstr )
{
  bool bSucc = false ;
  if( m_byIndex < NUMBER_OF_ENGLISH_AUX_STRINGS )
  {
    r_stdstr = m_strWords [ m_byIndex ++ ] ;
    bSucc = true ;
  }
  return bSucc ;
}

BYTE EnglishAuxiliaryVerb::GetWordClass()
{
  return auxiliary_verb ;
}

//inline
void EnglishVerb::Get3rdPersonForm(
  VTrans::string_type & r_vtransstr )
{
  WORD wStringSize = r_vtransstr.size() ;
  if ( wStringSize > 2 )
  {
    VTrans::string_type vtransstrEnding = r_vtransstr.substr(
      wStringSize - 2 ) ;
   if( vtransstrEnding == //"wish->wishes"
       "sh" || vtransstrEnding ==
           //"switch->switches"
       "ch"
     )
   {
     r_vtransstr += "es" ;
     return ;
   }
  }
  else
    if ( wStringSize > 1 )
    {
      VTrans::string_type vtransstrEnding = r_vtransstr.substr(
        wStringSize - 1 ) ;
      if( vtransstrEnding ==
          //"go->goes"
          "o"
        )
      {
        r_vtransstr += "es" ;
        return ;

      }
    }
  r_vtransstr += "s" ;
}

void EnglishVerb::GetProgressiveForm(
  VTrans::string_type & r_vtransstr )
{
  WORD wStringSize = r_vtransstr.size() ;
  if ( wStringSize > 2 )
  {
    VTrans::string_type vtransstrEnding = r_vtransstr.substr(
      wStringSize - 2 ) ;
    if( vtransstrEnding[1] ==
       //e.g. "prepare->preparing"
       'e'
     )
    {
     r_vtransstr.erase( r_vtransstr.length() , 1 ) ;
    }
    if( //E.g. "refer->refer>>r<<ing", "put->put>>t<<ing"
       //but not e.g. "talk->talk>>r<<ing"
       IsVowel(vtransstrEnding[0] ) &&
       IsConsonant(vtransstrEnding[1] )
     )
    {
     //Double the consonant
     r_vtransstr += vtransstrEnding[1] ;

    }
    r_vtransstr += "ing" ;
  }
//  else
//    if ( wStringSize > 1 )
//    {
//      VTrans::string_type vtransstrEnding = r_vtransstr.substr(
//        wStringSize - 1 ) ;
//      if( vtransstrEnding ==
//          //"go->goes"
//          "o"
//        )
//      {
//        r_vtransstr += "es" ;
//        return ;
//
//      }
//    }
//  r_vtransstr += "s" ;
}
