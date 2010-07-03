/*
 * TranslationRule.cpp
 *
 *  Created on: 03.04.2010
 *      Author: Stefan
 */

#include "TranslationRule.hpp"
#include <Parse/GrammarPart.hpp>
#include <Parse/ParseByRise.hpp>
#include <exception> //for std::exception
#include <supress_unused_variable.h>

WORD TranslationRule::GetConsecutiveID(
  const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath ) const
{
  const GrammarPart * p_grammarpart = GetGrammarPartWithConsecutiveID(
    r_stdvec_p_grammarpartPath ) ;
  if( p_grammarpart )
  {
#ifdef _DEBUG
        WORD wConsecutiveID = p_grammarpart->m_wConsecutiveID ;
        SUPRESS_UNUSED_VARIABLE_WARNING(wConsecutiveID)
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
//  GrammarPart * const p_grammarpart = NULL ;
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
  bool bCurrentlyKleeneStarOperator = false ;
  if( r_stdvec_wCurrentGrammarPartPath.size() >=
      m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements
      && m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements
    )
  {
    bIdentical = true ;
//    WORD wLenghtDiff = r_stdvec_wCurrentGrammarPartPath.size() -
//        m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements ;
    std::vector<WORD>::const_reverse_iterator rev_iter =
        r_stdvec_wCurrentGrammarPartPath.rbegin() ;
    //Compare from end to begin.
    WORD wIndex ;
    WORD wGrammarPartIDforTranslationRule ;
    for( wIndex = //p_tr->m_wNumberOfElements - 1 ;
        //p_translationrule->
        m_syntaxtreepathCompareWithCurrentPath.
        m_wNumberOfElements - 1 ;
        wIndex != MAXWORD
        && rev_iter != r_stdvec_wCurrentGrammarPartPath.rend()
            ; //-- wIndex
//        ++ rev_iter
        )
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
//        << r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
        << *rev_iter
        << mp_parsebyrise->GetGrammarPartName(
            //r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
          *rev_iter
          )
        << "\n"
        )
//      if( //p_tr->m_ar_wElements [ wIndex ] !=
//          //p_translationrule->
//          m_syntaxtreepathCompareWithCurrentPath.
//          m_ar_wElements [ wIndex ] !=
//        r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
//        )
//        {
//          bIdentical = false ;
//          DEBUG_COUT("TranslationRuleApplies--not identical\n")
//          break ;
//        }
      if( bCurrentlyKleeneStarOperator )
      {
        if( wGrammarPartIDforTranslationRule == *rev_iter )
        {
          bCurrentlyKleeneStarOperator = false ;
          -- wIndex ;
        }
        ++ rev_iter ;
      }
      else
      {
        wGrammarPartIDforTranslationRule =
            m_syntaxtreepathCompareWithCurrentPath.m_ar_wElements [ wIndex ] ;
        if( wGrammarPartIDforTranslationRule == KLEENE_STAR_OPERATOR )
        {
          if( wIndex - 1 >= 0 )
            -- wIndex ;
          else
            return false ;
          bCurrentlyKleeneStarOperator = true ;
          wGrammarPartIDforTranslationRule =
              m_syntaxtreepathCompareWithCurrentPath.m_ar_wElements [ wIndex ] ;
        }
        else
        {
          if( wGrammarPartIDforTranslationRule !=
              //r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
              *rev_iter
            )
          {
            bIdentical = false ;
            DEBUG_COUT("TranslationRuleApplies--not identical\n")
            break ;
          }
          -- wIndex ;
          //Only advance if the current grammar part ID is not Kleene star operator
          //because the ID after Kleene star *must* match the (indirect) ID of
          //the vector.
          ++ rev_iter ;
        }
      }
    }
  }
  //If currently there is a Kleene star operand, the rule is NOT identical
  //because a next (indirect) vector's grammar part ID must match the
  //translation rule's grammar part ID:
  //e.g. for translation grammar part path
  //"obj.*.def_article_plural.def_article"
  //the vector's grammar part path must contain an "obj" grammar part after
  // a def_article_plural gramar part.
  return !bCurrentlyKleeneStarOperator && bIdentical ;
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

//http://www.parashift.com/c++-faq-lite/exceptions.html#faq-17.2:
//"The best way to signal constructor failure is therefore to throw an exception."
//Throws a string that is an _unknown_ token of the syntax tree path as exception.
TranslationRule::TranslationRule(
    const std::string & r_stdstrSyntaxTreePath
    , ParseByRise * p_parsebyrise )
  : mp_parsebyrise(p_parsebyrise)
{
  if( !
      m_syntaxtreepathCompareWithCurrentPath.CreateGrammarPartIDArray(
      r_stdstrSyntaxTreePath
      , p_parsebyrise )
//      ;
    )
    throw std::exception() ;
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
