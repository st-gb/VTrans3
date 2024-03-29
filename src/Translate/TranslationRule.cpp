/** TranslationRule.cpp
 *  Created on: 03.04.2010
 *      Author: Stefan */

#include "TranslationRule.hpp" //header file for this TranslationRule class
#include <Parse/GrammarPart.hpp> //class GrammarPart
#include <Parse/ParseByRise.hpp> //class ParseByRise for DEBUG_COUTN(...)

#include <exception> //for class std::exception
#include <limits>///std::numeric_limits
/** SUPPRESS_UNUSED_VARIABLE_WARNING(...) */
#include <compiler/GCC/suppress_unused_variable.h>

#ifndef MAXWORD
  #define MAXWORD 65535
#endif

//TODO change data type to fastestUnsignedDataType?
WORD TranslationRule::GetConsecutiveID(
  const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath ) const
{
  const GrammarPart * p_grammarpart = GetGrammarPartWithConsecutiveID(
    r_stdvec_p_grammarpartPath ) ;
  if( p_grammarpart )
  {
#ifdef _DEBUG
        WORD wConsecutiveID = p_grammarpart->m_wConsecutiveID ;
        SUPPRESS_UNUSED_VARIABLE_WARNING(wConsecutiveID)
        return p_grammarpart->m_wConsecutiveID ;
#else
        return p_grammarpart->m_wConsecutiveID ;
#endif
  }
  return MAXWORD;
}

//pointer address to >>const<<ant content
//const GrammarPart *
GrammarPart * TranslationRule::GetGrammarPartWithConsecutiveID(
  const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath ) const
{
//  GrammarPart * const p_grammarpart = NULL ;
  if( r_stdvec_p_grammarpartPath.size() >=
      m_syntaxtreepathConsecutiveID.m_wNumberOfElements
      //&& m_syntaxtreepathConsecutiveID.m_wNumberOfElements > 0
      )
  {
    //TODO if "the little sheep" is translated as an (object) enumumeration
    //element,2 possibilities must be offered: "die kleinen Schafe",
    // "das kleine Schaf"
    DEBUGN("TranslationRule::GetGrammarPartWithConsecutiveID"
      "m_syntaxtreepathConsecutiveID:"
      << m_syntaxtreepathConsecutiveID.GetAs_std_string() )
    if( m_syntaxtreepathConsecutiveID.m_wNumberOfElements > 0 )
    {
//      const GrammarPart * p_grammarpartContainingConsecutiveID ;
      GrammarPart * p_grammarpartContainingConsecutiveID = NULL;
      //Avoid g++ warning "'p_grammarpartContainingConsecutiveID' might be
      // used uninitialized in this function"
      SUPPRESS_UNUSED_VARIABLE_WARNING(p_grammarpartContainingConsecutiveID)
      bool bIdentical = true ;
      //Compare from end to begin.
      indexDataType index;
      std::vector<GrammarPart *>::const_reverse_iterator
        c_rev_iter_stdvec_p_grammarpart = r_stdvec_p_grammarpartPath.rbegin() ;
      for( index = //p_tr->m_wNumberOfElements - 1 ;
          m_syntaxtreepathConsecutiveID.m_wNumberOfElements - 1 ;
          index != std::numeric_limits<indexDataType>::max() &&
          c_rev_iter_stdvec_p_grammarpart != r_stdvec_p_grammarpartPath.rend()
         ; -- index
         )
      {
//        DEBUG_COUTN
        DEBUGN("comparing " << //p_tr->m_ar_wElements [ wIndex ] <<
            //p_translationrule->
            m_syntaxtreepathConsecutiveID.
            m_ar_wElements [ index ] <<
          mp_bottomUpParser->GetGrammarPartName( //p_tr->m_ar_wElements [ wIndex ]
            //p_translationrule->
            m_syntaxtreepathConsecutiveID.
              m_ar_wElements [ index ]
          )
          << " and "
          << (*c_rev_iter_stdvec_p_grammarpart)->m_wGrammarPartID
          << mp_bottomUpParser->GetGrammarPartName(
            (*c_rev_iter_stdvec_p_grammarpart)->m_wGrammarPartID )
          )
        if( //p_tr->m_ar_wElements [ wIndex ] !=
            m_syntaxtreepathConsecutiveID.m_ar_wElements [ index ] !=
            (*c_rev_iter_stdvec_p_grammarpart)->m_wGrammarPartID
          )
        {
          bIdentical = false ;
//          DEBUG_COUT("TranslationRuleApplies--not identical\n")
          break ;
        }
        p_grammarpartContainingConsecutiveID = *c_rev_iter_stdvec_p_grammarpart ;
        ++ c_rev_iter_stdvec_p_grammarpart ;
      }
      if( bIdentical )
      {
//        GrammarPart * const p_grammarpart = r_stdvec_p_grammarpartPath.front() ;
//        return p_grammarpart ;
        LOGN("GetGrammarPartWithConsecutiveID(...) "
          "p_grammarpartContainingConsecutiveID:" <<
          p_grammarpartContainingConsecutiveID )
        return p_grammarpartContainingConsecutiveID ;
      }
    }
  }
  return NULL ;
}

bool TranslationRule::Initialize(
  const std::string & c_r_std_strSyntaxTreePath//,
//  const std::string & c_r_std_strSyntaxTreePathForInsertionForTranslation
  )
{
  m_ui32ConcatenationID = 65535;
  m_bySideWhereToInsertChildNode = uninited;
  m_bySideWhereToInsertParentNode = uninited;
  std::string std_strUnknownGrammarPartID;
  return m_syntaxtreepathCompareWithCurrentPath.CreateGrammarPartIDArray(
    c_r_std_strSyntaxTreePath
    , mp_bottomUpParser
    , std_strUnknownGrammarPartID
    )
    ;
}

void TranslationRule::Insert(
  const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath,
  const std::string & r_stdstrTranslation
  )
{
//  std::vector<GrammarPart *>::const_iterator ci = r_stdvec_p_grammarpartPath;
//  for( ; ci != r_stdvec_p_grammarpartPath.en(); ++ ci )
//  {
//    if( *ci->m_wGrammarPartID == m_syntaxtreepathInsertionForTranslation.
//        m_ar_wElements[0] )
//    {
//
//    }
//  }
 GrammarPart * p_grammarpartLeaf = m_syntaxtreepathInsertionForTranslation.
   GetLeaf(r_stdvec_p_grammarpartPath);
 if( p_grammarpartLeaf )
 {
   std::string std_strGrammarPartName = mp_bottomUpParser->GetGrammarPartName(
     p_grammarpartLeaf->m_wGrammarPartID);
   if( m_bySideWhereToInsertParentNode )
   {
     GrammarPart * p_gpChild = NULL;
     BYTE bySideWhereToInsertPreviousChildNode = 0;
     switch(m_bySideWhereToInsertChildNode)
     {
       case left:
         bySideWhereToInsertPreviousChildNode = right;
         break;
       case right:
         bySideWhereToInsertPreviousChildNode = left;
         break;
     }
     switch(m_bySideWhereToInsertParentNode)
     {
     case left:
       p_gpChild = p_grammarpartLeaf->InsertLeftChild(
           m_uiParentNodeGrammarPartID,
           bySideWhereToInsertPreviousChildNode);
       break;
     case right:
       p_gpChild = p_grammarpartLeaf->InsertRightChild(
           m_uiParentNodeGrammarPartID,
           bySideWhereToInsertPreviousChildNode);
       break;
     }
     if( p_gpChild)
     {
       GrammarPart * * p_p_grammarpartLeafNode = NULL;
       switch(m_bySideWhereToInsertChildNode)
       {
         case left:
           p_p_grammarpartLeafNode = & p_gpChild->mp_grammarpartLeftChild;
           break;
         case right:
           p_p_grammarpartLeafNode = & p_gpChild->mp_grammarpartRightChild;
           break;
       }
       if( p_p_grammarpartLeafNode )
       {
         * p_p_grammarpartLeafNode = new GrammarPart(0,0,
           m_uiChildNodeGrammarPartID);
         (* p_p_grammarpartLeafNode)->m_stdstrTranslation = r_stdstrTranslation;
//         (* p_p_grammarpartLeafNode)->m_wGrammarPartID =
//             m_uiChildNodeGrammarPartID;
       }
     }
   }
 }
}

bool TranslationRule::Matches(const SyntaxTreePath::PTN_IDpathContainer_type &
  cr_currParseTreeNodePath) const
{
//  bool bIdentical = false ;
//  bool bCurrentlyKleeneStarOperator = false ;
//  if( r_stdvec_wCurrentGrammarPartPath.size() >=
//      m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements
//      && m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements
//    )
//  {
//    bIdentical = true ;
////    WORD wLenghtDiff = r_stdvec_wCurrentGrammarPartPath.size() -
////        m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements ;
//    std::vector<WORD>::const_reverse_iterator rev_iter =
//        r_stdvec_wCurrentGrammarPartPath.rbegin() ;
//    //Compare from end to begin.
//    WORD wIndex ;
//    WORD wGrammarPartIDforTranslationRule ;
//    //Avoid g++ warning "'wGrammarPartIDforTranslationRule' might be used
//    // uninitialized in this function"
//    SUPPRESS_UNUSED_VARIABLE_WARNING(wGrammarPartIDforTranslationRule)
//    for( wIndex = //p_tr->m_wNumberOfElements - 1 ;
//        //p_translationrule->
//        m_syntaxtreepathCompareWithCurrentPath.
//        m_wNumberOfElements - 1 ;
//        wIndex != MAXWORD
//        && rev_iter != r_stdvec_wCurrentGrammarPartPath.rend()
//            ; //-- wIndex
////        ++ rev_iter
//        )
//    {
////      DEBUG_COUTN
//      DEBUGN
//      ( "comparing " << //p_tr->m_ar_wElements [ wIndex ] <<
//          //p_translationrule->
//        m_syntaxtreepathCompareWithCurrentPath.
//          m_ar_wElements [ wIndex ] <<
//        mp_parsebyrise->GetGrammarPartName( //p_tr->m_ar_wElements [ wIndex ]
//            //p_translationrule->
//          m_syntaxtreepathCompareWithCurrentPath.
//            m_ar_wElements [ wIndex ]
//        )
//        << " and "
////        << r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
//        << *rev_iter
//        << mp_parsebyrise->GetGrammarPartName(
//            //r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
//          *rev_iter
//          )
//        )
////      if( //p_tr->m_ar_wElements [ wIndex ] !=
////          //p_translationrule->
////          m_syntaxtreepathCompareWithCurrentPath.
////          m_ar_wElements [ wIndex ] !=
////        r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
////        )
////        {
////          bIdentical = false ;
////          DEBUG_COUT("TranslationRuleApplies--not identical\n")
////          break ;
////        }
//      if( bCurrentlyKleeneStarOperator )
//      {
//        if( wGrammarPartIDforTranslationRule == *rev_iter )
//        {
//          bCurrentlyKleeneStarOperator = false ;
//          -- wIndex ;
//        }
//        ++ rev_iter ;
//      }
//      else
//      {
//        wGrammarPartIDforTranslationRule =
//            m_syntaxtreepathCompareWithCurrentPath.m_ar_wElements [ wIndex ] ;
//        if( wGrammarPartIDforTranslationRule == KLEENE_STAR_OPERATOR )
//        {
//          if( wIndex - 1 >= 0 )
//            -- wIndex ;
//          else
//            return false ;
//          bCurrentlyKleeneStarOperator = true ;
//          wGrammarPartIDforTranslationRule =
//              m_syntaxtreepathCompareWithCurrentPath.m_ar_wElements [ wIndex ] ;
//        }
//        else
//        {
//          if( wGrammarPartIDforTranslationRule !=
//              //r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
//              *rev_iter
//            )
//          {
//            bIdentical = false ;
//            DEBUG_COUT("TranslationRuleApplies--not identical\n")
//            break ;
//          }
//          -- wIndex ;
//          //Only advance if the current grammar part ID is not Kleene star operator
//          //because the ID after Kleene star *must* match the (indirect) ID of
//          //the vector.
//          ++ rev_iter ;
//        }
//      }
//    }
//  }
//  //If currently there is a Kleene star operand, the rule is NOT identical
//  //because a next (indirect) vector's grammar part ID must match the
//  //translation rule's grammar part ID:
//  //e.g. for translation grammar part path
//  //"obj.*.def_article_plural.def_article"
//  //the vector's grammar part path must contain an "obj" grammar part after
//  // a def_article_plural gramar part.
//  return !bCurrentlyKleeneStarOperator && bIdentical ;
  return m_syntaxtreepathCompareWithCurrentPath.Matches(
    cr_currParseTreeNodePath ) ;
}

void TranslationRule::SetConsecutiveIDSyntaxTreePath(
  const std::string & r_stdstrSyntaxTreePath
//  , ParseByRise * p_parsebyrise
  )
{
  std::string std_strUnknownGrammarPartID;
  if( m_syntaxtreepathConsecutiveID.CreateGrammarPartIDArray(
        r_stdstrSyntaxTreePath
        , mp_bottomUpParser
        , std_strUnknownGrammarPartID
      ) == SyntaxTreePath::unknown_grammar_part_name
    )
  {
//    r_stdstrSyntaxTreePath += "";
    //Statement is for ability to set breakpoints only.
//    int i = 8;
  }
}

//http://www.parashift.com/c++-faq-lite/exceptions.html#faq-17.2:
//"The best way to signal constructor failure is therefore to throw an exception."
//Throws a string that is an _unknown_ token of the syntax tree path as exception.
TranslationRule::TranslationRule(
    const std::string & r_stdstrSyntaxTreePath
    , VTrans3::BottomUpParser * p_bottomUpParser )
  : mp_bottomUpParser(p_bottomUpParser)
//  , m_ui32ConcatenationID(0)
{
  if( ! Initialize(r_stdstrSyntaxTreePath)
//      ;
    )
    throw std::exception() ;
}

TranslationRule::TranslationRule( //const std::string & r_stdstrSyntaxTreePath
    const char * p_ch_SyntaxTreePath
    , VTrans3::BottomUpParser * p_bottomUpParser
    )
  : mp_bottomUpParser(p_bottomUpParser)
//  , m_ui32ConcatenationID(0)
{
  std::string stdstrSyntaxTreePath(p_ch_SyntaxTreePath) ;
  DEBUG_COUT("TranslationRule::TranslationRule begin\n")
  //TODO uncomment
  Initialize(stdstrSyntaxTreePath);
  DEBUG_COUT("TranslationRule::TranslationRule end\n")
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
  return m_syntaxtreepathCompareWithCurrentPath <
      r.m_syntaxtreepathCompareWithCurrentPath ;
}

//std::ostream & operator << (
//  std::ostream & std_ostream
//  , const TranslationRule & translationrule
//  ) //const
//{
////    std::ostream std_ostream;
//  std_ostream << translationrule.m_syntaxtreepathCompareWithCurrentPath.
//    GetAs_std_string();
//  if( translationrule.)
//  return std_ostream;
//}
