/*
 * TranslationRule.cpp
 *
 *  Created on: 03.04.2010
 *      Author: Stefan
 */

#include "TranslationRule.hpp"
#include <Parse/GrammarPart.hpp>
#include <Parse/ParseByRise.hpp>

WORD TranslationRule::GetConsecutiveID(
  const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath ) const
{
  const GrammarPart * p_grammarpart = GetGrammarPartWithConsecutiveID(
    r_stdvec_p_grammarpartPath ) ;
  if( p_grammarpart )
  {
#ifdef _DEBUG
        WORD wConsecutiveID = p_grammarpart->m_wConsecutiveID ;
        return p_grammarpart->m_wConsecutiveID ;
#else
        return p_grammarpart->m_wConsecutiveID ;
#endif
  }
  return MAXWORD ;
}

//pointer address to >>const<<ant content
const GrammarPart * TranslationRule::GetGrammarPartWithConsecutiveID(
  const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath ) const
{
  GrammarPart * const p_grammarpart = NULL ;
  if( r_stdvec_p_grammarpartPath.size() >=
      m_syntaxtreepathConsecutiveID.m_wNumberOfElements
      //&& m_syntaxtreepathConsecutiveID.m_wNumberOfElements > 0
      )
  {
    if( m_syntaxtreepathConsecutiveID.m_wNumberOfElements > 0 )
    {
      const GrammarPart * p_grammarpartContainingConsecutiveID ;
      bool bIdentical = true ;
      //Compare from end to begin.
      WORD wIndex ;
      std::vector<GrammarPart *>::const_reverse_iterator
        c_rev_iter_stdvec_p_grammarpart = r_stdvec_p_grammarpartPath.rbegin() ;
      for( wIndex = //p_tr->m_wNumberOfElements - 1 ;
          m_syntaxtreepathConsecutiveID.m_wNumberOfElements - 1 ;
          wIndex != MAXWORD &&
          c_rev_iter_stdvec_p_grammarpart != r_stdvec_p_grammarpartPath.rend()
         ; -- wIndex
         )
      {
        DEBUG_COUT("comparing " << //p_tr->m_ar_wElements [ wIndex ] <<
            //p_translationrule->
            m_syntaxtreepathConsecutiveID.
            m_ar_wElements [ wIndex ] <<
          mp_parsebyrise->GetGrammarPartName( //p_tr->m_ar_wElements [ wIndex ]
            //p_translationrule->
            m_syntaxtreepathConsecutiveID.
              m_ar_wElements [ wIndex ]
          )
          << " and "
          << (*c_rev_iter_stdvec_p_grammarpart)->m_wGrammarPartID
          << mp_parsebyrise->GetGrammarPartName(
            (*c_rev_iter_stdvec_p_grammarpart)->m_wGrammarPartID )
          << "\n"
          )
        if( //p_tr->m_ar_wElements [ wIndex ] !=
            m_syntaxtreepathConsecutiveID.m_ar_wElements [ wIndex ] !=
            (*c_rev_iter_stdvec_p_grammarpart)->m_wGrammarPartID
          )
        {
          bIdentical = false ;
          DEBUG_COUT("TranslationRuleApplies--not identical\n")
          break ;
        }
        p_grammarpartContainingConsecutiveID = *c_rev_iter_stdvec_p_grammarpart ;
        ++ c_rev_iter_stdvec_p_grammarpart ;
      }
      if( bIdentical )
      {
//        GrammarPart * const p_grammarpart = r_stdvec_p_grammarpartPath.front() ;
//        return p_grammarpart ;
        return p_grammarpartContainingConsecutiveID ;
      }
    }
  }
  return NULL ;
}

bool TranslationRule::Matches(
  const std::vector<WORD> & r_stdvec_wCurrentGrammarPartPath ) const
{
  bool bIdentical = false ;
  if( r_stdvec_wCurrentGrammarPartPath.size() >=
      m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements
      && m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements
    )
  {
    bIdentical = true ;
    WORD wLenghtDiff = r_stdvec_wCurrentGrammarPartPath.size() -
        m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements ;
    //Compare from end to begin.
    WORD wIndex ;
    for( wIndex = //p_tr->m_wNumberOfElements - 1 ;
        //p_translationrule->
        m_syntaxtreepathCompareWithCurrentPath.
        m_wNumberOfElements - 1 ;
        wIndex != MAXWORD  ; -- wIndex )
    {
      DEBUG_COUT("comparing " << //p_tr->m_ar_wElements [ wIndex ] <<
          //p_translationrule->
        m_syntaxtreepathCompareWithCurrentPath.
          m_ar_wElements [ wIndex ] <<
        mp_parsebyrise->GetGrammarPartName( //p_tr->m_ar_wElements [ wIndex ]
            //p_translationrule->
          m_syntaxtreepathCompareWithCurrentPath.
            m_ar_wElements [ wIndex ]
        )
        << " and "
        << r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
        << mp_parsebyrise->GetGrammarPartName(
            r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff ) )
        << "\n"
        )
      if( //p_tr->m_ar_wElements [ wIndex ] !=
          //p_translationrule->
          m_syntaxtreepathCompareWithCurrentPath.
          m_ar_wElements [ wIndex ] !=
        r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
        )
      {
        bIdentical = false ;
        DEBUG_COUT("TranslationRuleApplies--not identical\n")
        break ;
      }
    }
  }
  return bIdentical ;
}

void TranslationRule::SetConsecutiveIDSyntaxTreePath(
  const std::string & r_stdstrSyntaxTreePath
//  , ParseByRise * p_parsebyrise
  )
{
  m_syntaxtreepathConsecutiveID.CreateGrammarPartIDArray(
    r_stdstrSyntaxTreePath
    , mp_parsebyrise ) ;
}

TranslationRule::TranslationRule(
    const std::string & r_stdstrSyntaxTreePath
    , ParseByRise * p_parsebyrise )
  : mp_parsebyrise(p_parsebyrise)
{
  m_syntaxtreepathCompareWithCurrentPath.CreateGrammarPartIDArray(
      r_stdstrSyntaxTreePath
      , p_parsebyrise ) ;
}

TranslationRule::TranslationRule( //const std::string & r_stdstrSyntaxTreePath
    const char * p_ch_SyntaxTreePath
    , ParseByRise * p_parsebyrise )
  : mp_parsebyrise(p_parsebyrise)
{
  std::string stdstrSyntaxTreePath(p_ch_SyntaxTreePath) ;
  DEBUG_COUT("TranslationRule::TranslationRule\n")
  //TODO uncomment
  m_syntaxtreepathCompareWithCurrentPath.CreateGrammarPartIDArray(
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
  return m_syntaxtreepathCompareWithCurrentPath < r.m_syntaxtreepathCompareWithCurrentPath ;
}
