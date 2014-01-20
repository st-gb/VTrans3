/*
 * EnglishWord.cpp
 *
 *  Created on: 31.07.2011
 *      Author: Stefan
 */
#include "EnglishWord.hpp"
#include "Word.hpp" //IsVowel(...), IsConsonant(...)
#include <compiler/GCC/enable_disable_write_strings_warning.h> //GCC_DIAG_OFF(...)

EnglishWord::English_word_class EnglishWord::MapGrammarPartIDtoWordClass(
  English_word_class grammarPartID)
{
  English_word_class engWordClass = beyond_last_entry;
  /** Map grammar part IDs/ classes to word classes. */
  switch(grammarPartID)
  {
//  case EnglishWord::noun:
//  case EnglishWord::adverb:
////    m_pword = new EnglishNoun() ;
//    break;
  case EnglishWord::singular:
  case EnglishWord::plural_noun:
    engWordClass = EnglishWord::noun;
    break;
  case EnglishWord::adjective_positiveForm:
    engWordClass = EnglishWord::adjective;
    break;
  case EnglishWord::main_verb_allows_0object_infinitive:
  case EnglishWord::mainVerbAllows0object3rdPersonSingularPresent:
  case EnglishWord::mainVerbAllows1object3rdPersonSingularPresent:
  case EnglishWord::mainVerbAllows2objects3rdPersonSingularPresent:
  case EnglishWord::main_verb_allows_1object_infinitive:
  case EnglishWord::main_verb_allows_2objects_infinitive:
    engWordClass = EnglishWord::main_verb;
    break;
  default:
    engWordClass = grammarPartID;
    break;
  }
  return engWordClass;
}

fastestUnsignedDataType EnglishWord::GetGermanWordIndexFromGramarPartID(
  English_word_class grammarPartID)
{
  fastestUnsignedDataType germanWordIndex = 0;
  GCC_DIAG_OFF(switch)
  /** Map grammar part IDs/ classes to word classes. */
  switch(grammarPartID)
  {
//  case EnglishWord::noun:
//  case EnglishWord::adverb:
////    m_pword = new EnglishNoun() ;
//    break;
  case EnglishWord::plural_noun:
    germanWordIndex = 1;
    break;
  case EnglishWord::mainVerbAllows0object3rdPersonSingularPresent:
  case EnglishWord::mainVerbAllows1object3rdPersonSingularPresent:
  case EnglishWord::mainVerbAllows2objects3rdPersonSingularPresent:
    germanWordIndex = 3;
    break;
  }
  GCC_DIAG_ON(switch)
  return germanWordIndex;
}

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

/*BYTE*/ EnglishWord::English_word_class EnglishAuxiliaryVerb::GetWordClass()
{
  return auxiliary_verb ;
}

//inline
void EnglishVerb::Get3rdPersonSingularPresent(
  VTrans::string_type & r_vtransstr )
{
  VTrans::string_type::size_type stringSize = r_vtransstr.size() ;
  if ( stringSize > 2 )
  {
    VTrans::string_type vtransstrEnding = r_vtransstr.substr(
      stringSize - 2 ) ;
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
    if ( stringSize > 1 )
    {
      VTrans::string_type vtransstrEnding = r_vtransstr.substr(
        stringSize - 1 ) ;
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

VTrans::string_type EnglishVerb::Get3rdPersonSingularPresent(
  const VTrans::string_type & r_vtransstr )
{
  VTrans::string_type vtransstr(r_vtransstr);
  Get3rdPersonSingularPresent(vtransstr);
  return vtransstr;
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
