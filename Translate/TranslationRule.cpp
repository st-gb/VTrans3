/*
 * TranslationRule.cpp
 *
 *  Created on: 03.04.2010
 *      Author: Stefan
 */

  #include "TranslationRule.hpp"

  TranslationRule::TranslationRule(
      const std::string & r_stdstrSyntaxTreePath
      , ParseByRise * p_parsebyrise )
  {
    m_SyntaxTreePath.CreateGrammarPartIDArray(
        r_stdstrSyntaxTreePath
        , p_parsebyrise ) ;
  }

  TranslationRule::TranslationRule( //const std::string & r_stdstrSyntaxTreePath
      const char * p_ch_SyntaxTreePath
      , ParseByRise * p_parsebyrise )
  {
    std::string stdstrSyntaxTreePath(p_ch_SyntaxTreePath) ;
    DEBUG_COUT("TranslationRule::TranslationRule\n")
    //TODO uncomment
    m_SyntaxTreePath.CreateGrammarPartIDArray(
        stdstrSyntaxTreePath
        , p_parsebyrise ) ;
    DEBUG_COUT("TranslationRule::TranslationRule\n")
  }

  bool TranslationRule::operator < ( const TranslationRule & r) const
  {
//    if( m_wNumberOfElements < r.m_wNumberOfElements )
//      return true ;
//    else if( m_wNumberOfElements > r.m_wNumberOfElements )
//      return false ;
//    else //same number of elements
//    {
//      for( WORD wIndex = 0 ; wIndex < m_wNumberOfElements ; ++ wIndex )
//      {
//        if( m_ar_wElements[ wIndex ] < r.m_ar_wElements[ wIndex ] )
//          return true ;
//        else if ( m_ar_wElements[ wIndex ] > r.m_ar_wElements[ wIndex ] )
//          return false ;
//      }
//      //here: Contents are identical.
//      return false ;
//    }
    return m_SyntaxTreePath < r.m_SyntaxTreePath ;
  }
