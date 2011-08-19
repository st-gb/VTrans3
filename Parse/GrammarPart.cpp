/*
 * GrammarPart.cpp
 *
 *  Created on: May 28, 2010
 *      Author: Stefan
 */
#include "GrammarPart.hpp"
#include <Attributes/EnglishWord.hpp> //class EnglishWord's enum
#include <Translate/TranslationRule.hpp> //class TranslationRule

  void GrammarPart::SetLeftChild(GrammarPart & r_grammarpart)
  {
    mp_grammarpartLeftChild = & r_grammarpart ;
#ifdef COMPILE_WITH_POINTER_TO_PARENT
    //This is important for traversing from the leaves directing root.
    //This should be done for getting the "person index" (for finite verbforms):
    //"the car    and    I  like you"
    //  \   /            |
    // 3rd person sing  1st person sing
    //      \           /
    //  1st person sing ("we") -> "m�gen"
    mp_grammarpartLeftChild->mp_grammarpartParent = this ;
#endif
  }

  void GrammarPart::SetRightChild(GrammarPart & r_grammarpart)
  {
    mp_grammarpartRightChild = & r_grammarpart ;
#ifdef COMPILE_WITH_POINTER_TO_PARENT
    //This is important for traversing from the leaves directing root.
    //This should be done for getting the "person index" (for finite verbforms):
    //"the car    and    I  like you"
    //  \   /            |
    // 3rd person sing  1st person sing
    //      \           /
    //  1st person sing ("we") -> "m�gen"
    mp_grammarpartRightChild->mp_grammarpartParent = this ;
#endif
  }

GrammarPart::GrammarPart(DWORD dwTokenIndexLeftMost, DWORD dwTokenIndexRightMost )
{
  Init() ;
  m_dwLeftmostIndex = dwTokenIndexLeftMost ;
  m_dwRightmostIndex = dwTokenIndexRightMost ;
}

GrammarPart::GrammarPart(
  DWORD dwTokenIndexLeftMost,
  DWORD dwTokenIndexRightMost ,
  WORD wGrammarPartID )
{
  Init() ;
  m_dwLeftmostIndex = dwTokenIndexLeftMost ;
  m_dwRightmostIndex = dwTokenIndexRightMost ;
  m_wGrammarPartID = wGrammarPartID ;
}

void GrammarPart::Init()
{
#ifdef COMPILE_WITH_POINTER_TO_PARENT
  mp_grammarpartParent = NULL ;
#endif
  m_bAssignedAsChild = false;
  m_wConsecutiveID = MAXWORD ;
  m_byPersonIndex = 0;
  m_ui32ConcatenationID = //0
    unconnected;
  mp_grammarpartLeftChild = NULL ;
  mp_grammarpartRightChild = NULL ;
}

GrammarPart * GrammarPart::InsertChild(
  //left or right child member var.
  GrammarPart * & r_p_grammarpartChild,
  unsigned uiGrammarPartID,
  BYTE bySideWhereToInsertChildNode
  )
{
  GrammarPart * p_gpToInsert = new GrammarPart(0,0, uiGrammarPartID);
  if( p_gpToInsert)
    if( r_p_grammarpartChild )
    {
      // ... ...
      //   l  r
      //    \/
      //    gp
      GrammarPart * p_grammarpartChild = r_p_grammarpartChild;

      // inserting new left child ("*")
      //    ...
      // *  r
      //  \/
      //  gp
      r_p_grammarpartChild = p_gpToInsert;
      // l
      // \/ ...
      // *  r
      //  \/
      //  gp
      switch(bySideWhereToInsertChildNode)
      {
      case TranslationRule::left:
        p_gpToInsert->mp_grammarpartLeftChild = p_grammarpartChild;
        break;
      case TranslationRule::right:
        p_gpToInsert->mp_grammarpartRightChild = p_grammarpartChild;
        break;
      }
    }
    else
      r_p_grammarpartChild = p_gpToInsert;
  return p_gpToInsert;
}

GrammarPart * GrammarPart::InsertLeftChild(unsigned uiGrammarPartID,
  BYTE bySideWhereToInsertChildNode)
{
  return InsertChild(mp_grammarpartLeftChild, uiGrammarPartID,
      bySideWhereToInsertChildNode);
}
GrammarPart * GrammarPart::InsertRightChild(unsigned uiGrammarPartID,
    BYTE bySideWhereToInsertChildNode)
{
  return InsertChild(mp_grammarpartRightChild, uiGrammarPartID,
      bySideWhereToInsertChildNode);
}

//inline
void GrammarPart::SetGrammarPartID(WORD wGrammarPartID )
{
  switch( wGrammarPartID )
  {
    //the person index is alos important : e.g. if noun has the same string
  // for plural and singular then the person index of the verb gives info
  // about how its finite verbform should be in German:
  // The sheep run>>s<<. -> Das Schaf l�uft.
  // The sheep run. -> Die Schaf>>e<< laufen.
  case EnglishWord::mainVerbAllows0object3rdPersonSingularPresent :
    m_byPersonIndex = 3 ;
    break ;
  case EnglishWord::singular :
    m_byPersonIndex = 3 ;
    break ;
    //indefinite article ("a man") is singular per definition
  case EnglishWord::English_indefinite_article :
    m_byPersonIndex = 3 ;
    break ;
  }

  m_wGrammarPartID = wGrammarPartID ;
}

//Serves for the purpose that an applied grammar rule should not be added more
//than once.
//std::set::find(grammarpartToFind) return valid iterator
// if grammarpartInSet < grammarpartToFind == false
// and grammarpartToFind < grammarpartInSet == false
bool GrammarPart::operator < (const GrammarPart & cr_gpCompare ) const
{
  bool bLess = false ;
//  if( m_wGrammarPartID < cr_gpCompare.m_wGrammarPartID )
//    bLess = true ;
//  else
//  {
//    if( m_wGrammarPartID == cr_gpCompare.m_wGrammarPartID )
//      if( m_dwLeftmostIndex < cr_gpCompare.m_dwLeftmostIndex )
//        bLess = true ;
//      else
//      {
//        if( m_dwLeftmostIndex == cr_gpCompare.m_dwLeftmostIndex
////          && m_dwRightmostIndex < cr_gpCompare.m_dwRightmostIndex )
////            bLess = true ;
//          )
//          if( m_dwRightmostIndex < cr_gpCompare.m_dwRightmostIndex )
//            bLess = true ;
//          else
//          {
//            if( m_dwRightmostIndex == cr_gpCompare.m_dwRightmostIndex )
//              if( mp_grammarpartLeftChild->m_wGrammarPartID <
//                  cr_gpCompare.mp_grammarpartLeftChild->m_wGrammarPartID )
//                bLess = true ;
//              else
//              {
//                if( mp_grammarpartLeftChild->m_wGrammarPartID ==
//                  cr_gpCompare.mp_grammarpartLeftChild->m_wGrammarPartID )
//    //          GrammarPart * mp_grammarpartParent ;
//                  if ( //may be NULL!
//                      mp_grammarpartRightChild &&
//                      cr_gpCompare.mp_grammarpartRightChild &&
//                      mp_grammarpartRightChild->m_wGrammarPartID <
//                      cr_gpCompare.mp_grammarpartRightChild->m_wGrammarPartID )
//                    bLess = true ;
//              }
//          }
//      }
//  }

  //This if for finding out whether a rules was applied yet during resolving
  //of all rules: this is the case if the left (if only 1 child) or both children
  //
  if( mp_grammarpartLeftChild < cr_gpCompare.mp_grammarpartLeftChild )
    bLess = true ;
  else
    if( mp_grammarpartLeftChild == cr_gpCompare.mp_grammarpartLeftChild )
//          GrammarPart * mp_grammarpartParent ;
      if ( //may be NULL!
        mp_grammarpartRightChild &&
        cr_gpCompare.mp_grammarpartRightChild &&
        mp_grammarpartRightChild <
        cr_gpCompare.mp_grammarpartRightChild )
      bLess = true ;

//  if( m_wGrammarPartID < cr_gpCompare.m_wGrammarPartID &&
//      m_dwLeftmostIndex < cr_gpCompare.m_dwLeftmostIndex &&
//      m_dwRightmostIndex < cr_gpCompare.m_dwRightmostIndex
//      )
//    bLess = true ;

  return bLess ;
}
