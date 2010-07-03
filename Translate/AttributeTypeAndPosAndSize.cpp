/*
 * AttributeTypeAndPosAndSize.cpp
 *
 *  Created on: Jun 7, 2010
 *      Author: Stefan
 */

#include "AttributeTypeAndPosAndSize.hpp"
#include <windef.h> //BYTE

AttributeTypeAndPosAndSize::AttributeTypeAndPosAndSize(
  BYTE byType,
  WORD wIndex,
  WORD wLenght,
  BYTE byWordClass ,
  BYTE byLanguage // Language that is referred by the other parameters.
  )
  :
  m_byAttrDataType ( byType ) ,
  m_byLanguage ( byLanguage ) ,
  m_byWordClass ( byWordClass ) ,
  m_wIndex ( wIndex ) ,
  m_wLenght ( wLenght )
{
//  m_byLanguage = byLanguage ;
//  m_byWordClass = byWordClass ;
//  m_byAttrDataType = byType ;
//  m_wIndex = wIndex ;
//  m_wLenght = wLenght ;
}

//bool AttributeTypeAndPosAndSize::ConditionMatches(
//  const GrammarPart * const p_grammarpartLeaf ) const
//{
//  switch( m_byAttrDataType )
//  {
//    case AttributeTypeAndPosAndSize::string :
//    {
//      if( m_byLanguage ==
//        AttributeTypeAndPosAndSize::English )
//      {
//        std::string & r_stdstrAttrVal = p_grammarpartLeaf->
//          m_pvocabularyandtranslation->m_arstrEnglishWord[
//          r_atapas.m_wIndex ] ;
//        //std::string & r_stdstrTextTokens =
//        std::string stdstrTextTokens ;
//          sp_parsebyrise->GetTokensAsSpaceSeparatedString(
//          p_grammarpartLeaf->m_dwLeftmostIndex,
//          p_grammarpartLeaf->m_dwRightmostIndex ,
//          stdstrTextTokens
//          ) ;
//        if( stdstrTextTokens != ""  && stdstrTextTokens ==
//          r_stdstrAttrVal )
//        {
//          //bIdentical = true ;
//        }
//        else
//        {
//          bAllConditionsMatch = false ;
//          //break ;
//          return false ;
//        }
//      }
//    }
//    break ;
//    case AttributeTypeAndPosAndSize::bit :
//      if( m_byLanguage ==
//        AttributeTypeAndPosAndSize::German )
//      {
//        BYTE by = p_grammarpartLeaf->
//          m_pvocabularyandtranslation->m_arbyAttribute[
//          r_atapas.m_wIndex ] ;
//        if( cr_condition.m_byAttributeValue != by )
//          //bAllConditionsMatch = false ;
//          return false ;
//      }
//  }//switch
//}

