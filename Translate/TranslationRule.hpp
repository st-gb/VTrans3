/*
 * TranslationRule.hpp
 *
 *  Created on: 03.04.2010
 *      Author: Stefan
 */

#ifndef TRANSLATIONRULE_HPP_
#define TRANSLATIONRULE_HPP_

#include "SyntaxTreePath.hpp" //class SyntaxTreePath
//#include <rest.h> //DEBUG_COUT
#include <preprocessor_macros/logging_preprocessor_macros.h> //DEBUG_COUT(...)

//A translationrule desribes the path within a syntax tree.
//It is inserted into a <TranslationRule*,ConditionAndTranslation> map
//after creation.
//If this patch matches the current syntax tree path then the dazugehoerige
//ConditionAndTranslation is processed  to see if the conditions match.
class TranslationRule
{
public:
//  WORD * m_ar_wElements ;
//  WORD m_wNumberOfElements ;
  ParseByRise * mp_parsebyrise ;
  //This path is compared with the current parse tree path: if identical,
  //proceed with the ConditionAndTranslation value of the
  //TranslationRule->ConditionAndTranslation map
  SyntaxTreePath m_syntaxtreepathCompareWithCurrentPath ;
  SyntaxTreePath m_syntaxtreepathConsecutiveID ;
//  TranslationRule( WORD * ar_wElements , WORD wNumberOfElements)
//  {
//    m_ar_wElements = ar_wElements ;
//    m_wNumberOfElements = wNumberOfElements ;
//  }
  WORD GetConsecutiveID( const std::vector<GrammarPart *> &
    r_stdvec_p_grammarpartPath ) const ;
  //const
  GrammarPart * GetGrammarPartWithConsecutiveID(
    const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath ) const ;
  bool Matches(
    const std::vector<WORD> & cr_stdvec_wCurrentGrammarPartPath ) const ;
  void SetConsecutiveIDSyntaxTreePath(
    const std::string & r_stdstrSyntaxTreePath
//    , ParseByRise * p_parsebyrise
    ) ;
  TranslationRule( const std::string & r_stdstrSyntaxTreePath
      , ParseByRise * p_parsebyrise ) ;
  TranslationRule( const char * p_ch_SyntaxTreePath
      , ParseByRise * p_parsebyrise ) ;
  ~TranslationRule( )
  {
    DEBUG_COUT("~TranslationRule (destructor)\n")
//    //Release mem.
//    delete [] m_ar_wElements ;
  }
  bool operator < ( const TranslationRule & r) const ;
};

#endif /* TRANSLATIONRULE_HPP_ */
