/*
 * GrammarPart.cpp
 *
 *  Created on: May 28, 2010
 *      Author: Stefan
 */
#include "GrammarPart.hpp"

//Serves for the purpose that an applied grammar rule should not be added more
//than once.
//std::set::find(grammarpartToFind) return valid iterator
// if grammarpartInSet < grammarpartToFind == false
// and grammarpartToFind < grammarpartInSet == false
bool GrammarPart::operator < (const GrammarPart & cr_gpCompare ) const
{
  bool bLess = false ;
  if( m_wGrammarPartID < cr_gpCompare.m_wGrammarPartID )
    bLess = true ;
  else
  {
    if( m_wGrammarPartID == cr_gpCompare.m_wGrammarPartID )
      if( m_dwLeftmostIndex < cr_gpCompare.m_dwLeftmostIndex )
        bLess = true ;
      else
      {
        if( m_dwLeftmostIndex == cr_gpCompare.m_dwLeftmostIndex
//          && m_dwRightmostIndex < cr_gpCompare.m_dwRightmostIndex )
//            bLess = true ;
          )
          if( m_dwRightmostIndex < cr_gpCompare.m_dwRightmostIndex )
            bLess = true ;
          else
          {
            if( m_dwRightmostIndex == cr_gpCompare.m_dwRightmostIndex )
              if( mp_grammarpartLeftChild->m_wGrammarPartID <
                  cr_gpCompare.mp_grammarpartLeftChild->m_wGrammarPartID )
                bLess = true ;
              else
              {
                if( mp_grammarpartLeftChild->m_wGrammarPartID ==
                  cr_gpCompare.mp_grammarpartLeftChild->m_wGrammarPartID )
    //          GrammarPart * mp_grammarpartParent ;
                  if ( //may be NULL!
                      mp_grammarpartRightChild &&
                      cr_gpCompare.mp_grammarpartRightChild &&
                      mp_grammarpartRightChild->m_wGrammarPartID <
                      cr_gpCompare.mp_grammarpartRightChild->m_wGrammarPartID )
                    bLess = true ;
              }
          }
      }
  }

//  if( m_wGrammarPartID < cr_gpCompare.m_wGrammarPartID &&
//      m_dwLeftmostIndex < cr_gpCompare.m_dwLeftmostIndex &&
//      m_dwRightmostIndex < cr_gpCompare.m_dwRightmostIndex
//      )
//    bLess = true ;

  return bLess ;
}
