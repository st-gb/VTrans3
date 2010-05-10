/*
 * SyntaxTreePath.cpp
 *
 *  Created on: 03.04.2010
 *      Author: Stefan
 */

#include "SyntaxTreePath.hpp"
#include <Parse/ParseByRise.hpp>
#include <string>
#include <rest.h> //DEBUG_COUT
#include <windef.h>

SyntaxTreePath::SyntaxTreePath(
    std::string & r_stdstrSyntaxTreePath
    , ParseByRise * p_parsebyrise
    )
{
  mp_parsebyrise = p_parsebyrise ;
  CreateGrammarPartIDArray(r_stdstrSyntaxTreePath
      , p_parsebyrise ) ;
}

//The copy contructor needs an own implementation because an array is created
//on the heap: in order to NOT release memory (then the original would also try
//to release it again->points to other data or code->program crash)
//for this array in the destructor
//we simply set a flag that we are referring this array.
SyntaxTreePath::SyntaxTreePath( const SyntaxTreePath & stpToCopyFrom )
{
  //for checking for correct release of memory:
//  DEBUG_COUT("SyntaxTreePath copy ctor\n")
  //m_byOtherReferencesToGrammarPartIDArray ++ ;
//  m_bReferringOthersGrammarPartIDArray = true ;
//  stpToCopyFrom.m_bReferredOthersGrammarPartIDArray = true ;
  m_wNumberOfElements = stpToCopyFrom.m_wNumberOfElements ;
//  m_ar_wElements = stpToCopyFrom.m_ar_wElements ;
  m_ar_wElements = new WORD[ m_wNumberOfElements ] ;
  if( m_ar_wElements )
  {
    memcpy( m_ar_wElements , //void * destination,
      stpToCopyFrom.m_ar_wElements , //const void * source
      sizeof(WORD) * m_wNumberOfElements //Number of bytes to copy.
      ) ;
  }
  mp_parsebyrise = stpToCopyFrom.mp_parsebyrise ;
  //m_byOtherReferencesToGrammarPartIDArray = 0 ;
}

SyntaxTreePath::~SyntaxTreePath()
{
  //for checking for correct release of memory:
//  DEBUG_COUT("~SyntaxTreePath (destructor) (obj at address " << this << "\n")
//  if(
//    m_ar_wElements && //m_byOtherReferencesToGrammarPartIDArray == 0
//    //! m_bReferringOthersGrammarPartIDArray
//    ! m_bReferredByOthersGrammarPartIDArray
//    )
  {
    //for checking for correct release of memory:
//    DEBUG_COUT("~SyntaxTreePath (destructor)--release mem. at " <<
//      m_ar_wElements << "\n")
    //Release mem.
    delete [] m_ar_wElements ;
  }
}

void SyntaxTreePath::CreateGrammarPartIDArray(
    const std::string & r_stdstrSyntaxTreePath
    ,     ParseByRise * p_parsebyrise )
{
  bool bNewEle = false ;
  std::string stdstrCurrentElement ;
  std::vector<WORD> vec_wElements ;
  WORD wStringStart = 0 ;
  WORD wGrammarPartID ;
  WORD wNumberOfElements = 1 ;
  mp_parsebyrise = p_parsebyrise ;
  for( WORD i = 0 ; i < r_stdstrSyntaxTreePath.length() ; ++ i )
  {
    if( i == r_stdstrSyntaxTreePath.length() - 1 )
    {
      stdstrCurrentElement = r_stdstrSyntaxTreePath.substr(
          wStringStart, ( i + 1 ) - wStringStart ) ;
      bNewEle = true ;
    }
    if( r_stdstrSyntaxTreePath[i] == '.' )
    {
      stdstrCurrentElement = r_stdstrSyntaxTreePath.substr(
          wStringStart, i - wStringStart ) ;
      bNewEle = true ;
    }
    if( bNewEle )
    {
      ++ wNumberOfElements ;
      wStringStart = i + 1 ;
      if( mp_parsebyrise->GetGrammarPartID( stdstrCurrentElement ,
          wGrammarPartID )
        )
      {
        vec_wElements.push_back(wGrammarPartID) ;
      }
      bNewEle = false ;
    }
  }
  DEBUG_COUT("SyntaxTreePath::CreateGrammarPartIDArray size:" <<
      vec_wElements.size() + "\n" )
  WORD * ar_wElements = new WORD [ vec_wElements.size() ] ;
  if( ar_wElements )
  {
    WORD wIndex = 0 ;
    for( std::vector<WORD>::const_iterator iter = vec_wElements.begin() ;
        iter != vec_wElements.end() ; ++ iter )
    {
      //DEBUG("curr") ;
      ar_wElements[ wIndex ++ ] = *iter ;
    }
    m_ar_wElements = ar_wElements ;
    m_wNumberOfElements = vec_wElements.size() ;
#ifdef _DEBUG
    std::string str = GetAs_std_string() ;
    DEBUG_COUTN("SyntaxTreePath(" << this << ")::CreateGrammarPartIDArray()"
      "--array as string:"
      << str )
#endif
  }
}

bool SyntaxTreePath::operator < ( const SyntaxTreePath & r) const
{
  if( m_wNumberOfElements < r.m_wNumberOfElements )
    return true ;
  else if( m_wNumberOfElements > r.m_wNumberOfElements )
    return false ;
  else //same number of elements
  {
    for( WORD wIndex = 0 ; wIndex < m_wNumberOfElements ; ++ wIndex )
    {
      if( m_ar_wElements[ wIndex ] < r.m_ar_wElements[ wIndex ] )
        return true ;
      else if ( m_ar_wElements[ wIndex ] > r.m_ar_wElements[ wIndex ] )
        return false ;
    }
    //here: Contents are identical.
    return false ;
  }
}

std::string SyntaxTreePath::GetAs_std_string( ) const
{
  std::string stdstr ;
#ifdef _DEBUG
  WORD wGrammarPartID ;
#endif
//  for(WORD  w=0; w < m_ar_wElements ; ++ w )
//    stdstr += mp_parsebyrise->GetGrammarPartName( m_ar_wElements[w]) ;
  for( WORD wIndex = 0 ; wIndex < m_wNumberOfElements ; ++ wIndex )
  {
#ifdef _DEBUG
    wGrammarPartID = m_ar_wElements[ wIndex ] ;
    stdstr += mp_parsebyrise->GetGrammarPartName( wGrammarPartID )
        + "." ;
#else
    stdstr += mp_parsebyrise->GetGrammarPartName( m_ar_wElements[ wIndex ] )
        + "." ;
#endif
  }
  return stdstr ;
}

//Gets the leaf node ausgehend from the the back of vector.
//So for instance if we want to get the leaf of "noun_construction.noun" , i.e.
// get the node "noun" ausgehend from "noun_construct.definite_aricle"
// The 1st common node from the back is "noun_construct", so advance from
// this node to "noun_construct.noun"
GrammarPart * SyntaxTreePath::GetLeaf(
  //The parse tree part, first element is the root or closer to the root than
  // the last element.
  //  e.g. for  the car
  //              \ /
  //           def_noun
  // the vector is  [def_noun;article]
  const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath) const
{
  GrammarPart * p_grammarpartRet = NULL ;
#ifdef _DEBUG
  std::string str = mp_parsebyrise->GetPathAs_std_string(
    r_stdvec_p_grammarpartPath ) ;
  std::string strWORDarray = GetAs_std_string( ) ;
  DEBUG_COUTN("SyntaxTreePath(" << this << ")::GetLeaf()"
    "--array as string:"
    << strWORDarray )
#endif
  if( m_wNumberOfElements > 0 )
  {
    GrammarPart * p_grammarpart = NULL ;
    //e.g. find "noun_construct.noun" in
    // "subject.noun_construct.definite_article"
    //
    for( std::vector<GrammarPart *>::const_reverse_iterator r_iter =

      r_stdvec_p_grammarpartPath.rbegin() ;
      r_iter != r_stdvec_p_grammarpartPath.rend() ; ++ r_iter )
    {
      //for( WORD wArrayIndex = m_wNumberOfElements )

      // e.g. "noun_construct" from the parse tree path
      //  "noun_construct.noun" found in
      //    "subject.noun_construct.definite_article" .
      if( (*r_iter)->m_wGrammarPartID == m_ar_wElements[0] )
      {
        p_grammarpart = *r_iter ;
        break ;
      }
    }
    //e.g. now we have "noun_construct" from syntax tree path 
    // "noun_construct.noun".
    if( p_grammarpart )
    {
      //bool
      GrammarPart * p_grammarpartChild ;
      //"Walk" directing the leaf in the
      //e.g. beginning from "noun_construct" go further the way that the rule
      //IDs define.
      //  e.g. the tree is
      //    the            car
      // definite_article  noun
      //         \         /
      //         noun_construct  <-starting here. the grammar part ID defines
      // the way directing the leaf as "noun_construct->definite_article",
      // So take the child node that has the grammar part ID of "definite_article"
      for( WORD wIndex = 1 ; wIndex < m_wNumberOfElements ; ++ wIndex )
      {
        p_grammarpartChild = p_grammarpart->mp_grammarpartLeftChild ;
        if( p_grammarpartChild &&
          p_grammarpart->m_wGrammarPartID == m_ar_wElements[ wIndex ] )
        {
          p_grammarpart = p_grammarpartChild ;
        }
        else
        {
          p_grammarpartChild = p_grammarpart->mp_grammarpartRightChild ;
          if( p_grammarpartChild &&
            p_grammarpartChild->m_wGrammarPartID == m_ar_wElements[ wIndex ] )
          {
            p_grammarpart = p_grammarpartChild ;
          }
        }
        if( ! p_grammarpartChild )
          break ;
      }
      if( p_grammarpartChild )
        p_grammarpartRet = p_grammarpartChild ;
    }
  }
  return p_grammarpartRet ;
}

//e.g. test if "noun_construct.noun" is a part of
// "subject.noun_construct.definite aricle" (true)
bool SyntaxTreePath::IsPartOf(std::vector<WORD> & r_stdvec_wGrammarPartPath )
{
  bool bIsPartOf = false ;
  for( std::vector<WORD>::const_iterator iter =
    r_stdvec_wGrammarPartPath.begin() ;
    iter != r_stdvec_wGrammarPartPath.end() ; ++ iter )
  {
    for( WORD wArrayIndex = 0 ; wArrayIndex < m_wNumberOfElements ;
       ++ wArrayIndex )
    {
      if( *iter == m_ar_wElements[ wArrayIndex ] )
      {
        bIsPartOf = true ;
        break ;
      }
    }
  }
  return bIsPartOf ;
}
