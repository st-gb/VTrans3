/*
 * SyntaxTreePath.cpp
 *
 *  Created on: 03.04.2010
 *      Author: Stefan
 */

#include "SyntaxTreePath.hpp"
#include <Parse/ParseByRise.hpp>
#include <UserInterface/I_UserInterface.hpp>
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

//0: failure
BYTE SyntaxTreePath::CreateGrammarPartIDArray(
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
      //If e.g. definite_article_plural.definite_article it is important if
      // a direct or indirect parent node is "object" so that it is translated.
      //The syntax tree path may vary because between
      //  definite_article_plural.definite_article and object can be other nodes
      // that e.g. are used for conjunctions.
      // So the "Kleene star operator" is useful for these situations.
      if( stdstrCurrentElement == "*" )
      {
        vec_wElements.push_back(KLEENE_STAR_OPERATOR) ;
      }
      else
        if( mp_parsebyrise->GetGrammarPartID( stdstrCurrentElement ,
            wGrammarPartID )
          )
        {
          vec_wElements.push_back(wGrammarPartID) ;
        }
        else //no such string in the map->return failure.
          //return 0 ;
          //According to http://www.cplusplus.com/doc/tutorial/exceptions/
          //exceptions can also be data types like strings
//          throw //stdstrCurrentElement ;
//           GetGrammarPartIDexception(stdstrCurrentElement ) ;
        {
          sp_userinterface->Message( "unknown grammar part name:" +
            stdstrCurrentElement ) ;
          return 0 ;
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
    return 1 ;
  }
  return 0 ;
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
//So for instance if we want to get the leaf of "def_article_noun.noun" , i.e.
// get the node "noun" ausgehend from "def_article_noun.definite_aricle"
// The 1st common node from the back is "def_article_noun", so advance from
// this node to "def_article_noun.noun"

//1.Iterates from the end of the vector heading its begin
//  -this vector is usually sorted that it has the grammar part leaf node at
//   its end. So it walks heading root at first:  here from "car" to "def_noun"
//   the  car
//    \   /
//   def_noun  <-  "forking": from here it walks heading leaf
//2.If a node with a grammar part ID that equals the 1st grammar part ID of
//   the own grammar part ID array is found, then this is the "forking":
//   the  car
//    \  /
//   def_noun  <-  "forking": from here it walks heading leaf
//3. walks heading leaf following the grammar part node with the same ID as in
//   the cintained grammar part ID array:
//   e.g. if array is [def_noun;"the"]: it walks from "def_noun" to "the"
//   the  car
//    \  /
//   def_noun  <-  "forking": from here it walks heading leaf
GrammarPart * SyntaxTreePath::GetLeaf(
  //The parse tree path, first element is the root or closer to the root than
  // the last element.
  //  e.g. for  the car
  //              \ /
  //           def_noun
  // the vector is  [def_noun;article]
  const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath) const
{
  GrammarPart * p_grammarpartRet = NULL ;
#ifdef _DEBUG
  std::string strPathFromGrammarPartPointerVector =
      mp_parsebyrise->GetPathAs_std_string(
    r_stdvec_p_grammarpartPath ) ;
  std::string strWORDarray = GetAs_std_string( ) ;
  DEBUG_COUTN("SyntaxTreePath(" << this << ")::GetLeaf()"
    "--array as string:"
    << strWORDarray << "  "
    << strPathFromGrammarPartPointerVector )
#endif
  if( m_wNumberOfElements > 0 )
  {
    GrammarPart * p_grammarpart = NULL ;
    //e.g. find "def_article_noun.noun" in
    // "subject.def_article_noun.definite_article"
    //
    for( std::vector<GrammarPart *>::const_reverse_iterator
        c_reverse_iter_stdvec_p_grammarpart =
      //Start from the end (leaf, at least closer to the leaf than the begin)
      //e.g. for the parse tree path "definite_article_noun.noun"
      //  e.g. for  the car
      //              \ /
      //       definite_article_noun
      //start from "the" ("noun")
      //of the current parse tree path.
      r_stdvec_p_grammarpartPath.rbegin() ;
      c_reverse_iter_stdvec_p_grammarpart != r_stdvec_p_grammarpartPath.rend() ;
      ++ c_reverse_iter_stdvec_p_grammarpart
      )
    {
      //for( WORD wArrayIndex = m_wNumberOfElements )
//      DEBUG_COUTN("* r_iter: " << *c_reverse_iter_stdvec_p_grammarpart )
      DEBUG_COUTN("SyntaxTreePath--current grammar part: "
        << *c_reverse_iter_stdvec_p_grammarpart
        << " ID:" << (*c_reverse_iter_stdvec_p_grammarpart)->m_wGrammarPartID
        << " as string:" << mp_parsebyrise->GetGrammarPartName(
          (*c_reverse_iter_stdvec_p_grammarpart)->m_wGrammarPartID )
        )
#ifdef _DEBUG
      if( *c_reverse_iter_stdvec_p_grammarpart )
      {
//        DEBUG_COUTN("* r_iter: " << r_iter->GetG )
      }
    #endif

      // e.g. "def_article_noun" from the parse tree path
      //  "def_article_noun.noun" found in the grammar part path
      //    "subject.def_article_noun.definite_article" .
      if( (*c_reverse_iter_stdvec_p_grammarpart)->m_wGrammarPartID ==
          m_ar_wElements[0] )
      {
        p_grammarpart = *c_reverse_iter_stdvec_p_grammarpart ;
        break ;
      }
    }
    //e.g. now we have "def_article_noun" from syntax tree path 
    // "def_article_noun.noun".
    if( p_grammarpart )
    {
      //bool
      GrammarPart * p_grammarpartChild = NULL ;
      //"Walk" directing the leaf in the
      //e.g. beginning from "def_article_noun" go further the way that the rule
      //IDs define.
      //  e.g. the tree is
      //    the            car
      // definite_article  noun
      //         \         /
      //         def_article_noun  <-starting here. the grammar part ID defines
      // the way directing the leaf as "def_article_noun->definite_article",
      // So take the child node that has the grammar part ID of "definite_article"
      for( WORD wIndex = 1 ; wIndex < m_wNumberOfElements ; ++ wIndex )
      {
        DEBUG_COUTN("the next child should have grammar part ID: "
          << m_ar_wElements[ wIndex ]
          << " as string: "
          << mp_parsebyrise->GetGrammarPartName( m_ar_wElements[ wIndex ] )
          )
        p_grammarpartChild = p_grammarpart->mp_grammarpartLeftChild ;
        if( p_grammarpartChild &&
          p_grammarpartChild->m_wGrammarPartID == m_ar_wElements[ wIndex ] )
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
          else //neither right nor left child match the syntax tree path.
            return NULL ;
        }
//        if( ! p_grammarpartChild )
//          break ;
      }
      if( p_grammarpartChild )
        p_grammarpartRet = p_grammarpartChild ;
      else
        p_grammarpartRet = p_grammarpart ;
    }
  }
  return p_grammarpartRet ;
}

//e.g. test if "def_article_noun.noun" is a part of
// "subject.def_article_noun.definite aricle" (true)
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
