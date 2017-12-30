/** SyntaxTreePath.cpp
 *  Created on: 03.04.2010
 *      Author: Stefan   */

#include "SyntaxTreePath.hpp"
//class VTrans::UnknownGrammarPartNameException
#include <IO/UnknownGrammarPartNameException.hpp>
#include <Parse/ParseByRise.hpp>//class ParseByRise
/** SUPPRESS_UNUSED_VARIABLE_WARNING(...) */
#include <compiler/GCC/suppress_unused_variable.h>
#include <preprocessor_macros/logging_preprocessor_macros.h>//DEBUG_COUTN(...)
//for sp_userinterface->Message(const std::string & )
#include <UserInterface/I_UserInterface.hpp>
#include <string>//class std::string
//#include <rest.h> //DEBUG_COUT
#include <windef.h>//for WORD
#define __STDC_LIMIT_MACROS
#include <stdint.h> //__UINT16_MAX__
#include <limits.h> //UINT16_MAX
#include <deque> //class std::deque

#ifndef MAXWORD
  #define MAXWORD 65535
#endif

I_UserInterface * SyntaxTreePath::sp_userinterface;

SyntaxTreePath::SyntaxTreePath(
  const std::string & cr_stdstrSyntaxTreePath
  , VTrans3::BottomUpParser * p_bottomUpParser
  )
{
  LOGN_DEBUG(//"SyntaxTreePath(" <<
    cr_stdstrSyntaxTreePath << "," << p_bottomUpParser << ")" )
  mp_bottomUpParser = p_bottomUpParser ;
  std::string std_strUnknownGrammarPartID;

  if( CreateGrammarPartIDArray(
      cr_stdstrSyntaxTreePath ,
      p_bottomUpParser ,
      std_strUnknownGrammarPartID
      ) == SyntaxTreePath::unknown_grammar_part_name
    )
    //Cannot return an error code for constructors, so throw an exception.
    throw GetGrammarPartIDexception(std_strUnknownGrammarPartID);
}

//The copy contructor needs an own implementation because an array is created
//on the heap: in order to NOT release memory (then the original would also try
//to release it again->points to other data or code->program crash)
//for this array in the destructor
//we simply set a flag that we are referring this array.
SyntaxTreePath::SyntaxTreePath(
    const SyntaxTreePath & c_r_syntaxtreepathToCopyFrom )
{
  copy(c_r_syntaxtreepathToCopyFrom);
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

void SyntaxTreePath::copy(const SyntaxTreePath & c_r_syntaxtreepathToCopyFrom)
{
  //for checking for correct release of memory:
  //  DEBUG_COUT("SyntaxTreePath copy ctor\n")
  //m_byOtherReferencesToGrammarPartIDArray ++ ;
  //  m_bReferringOthersGrammarPartIDArray = true ;
  //  stpToCopyFrom.m_bReferredOthersGrammarPartIDArray = true ;
  m_wNumberOfElements = c_r_syntaxtreepathToCopyFrom.m_wNumberOfElements ;
  //  m_ar_wElements = stpToCopyFrom.m_ar_wElements ;
  m_ar_wElements = new WORD[ m_wNumberOfElements ] ;
  if( m_ar_wElements )
  {
    memcpy( m_ar_wElements , //void * destination,
      c_r_syntaxtreepathToCopyFrom.m_ar_wElements , //const void * source
      sizeof(WORD) * m_wNumberOfElements //Number of bytes to copy.
      ) ;
  }
  mp_bottomUpParser = c_r_syntaxtreepathToCopyFrom.mp_bottomUpParser ;
  //m_byOtherReferencesToGrammarPartIDArray = 0 ;
}

//0: failure
BYTE SyntaxTreePath::CreateGrammarPartIDArray(
    const std::string & r_stdstrSyntaxTreePath
    , VTrans3::BottomUpParser * p_bottomUpParser
    , std::string & r_std_strUnknownGrammarPartID
    )
{
  LOGN_DEBUG(//"SyntaxTreePath::CreateGrammarPartIDArray(" <<
    r_stdstrSyntaxTreePath << "," << p_bottomUpParser << ") begin")
  bool bNewEle = false ;
  std::string stdstrCurrentElement ;
  std::vector<WORD> vec_wElements ;
  WORD wStringStart = 0 ;
  WORD wGrammarPartID ;
  WORD wNumberOfElements = 1 ;
  mp_bottomUpParser = p_bottomUpParser ;
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
        if( mp_bottomUpParser->GetGrammarPartID( stdstrCurrentElement ,
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
//          sp_userinterface->Message( "unknown grammar part name:" +
//            stdstrCurrentElement ) ;
          r_std_strUnknownGrammarPartID = stdstrCurrentElement;
          LOGN_ERROR(//"SyntaxTreePath::CreateGrammarPartIDArray(...)--"
            "unknown grammar part name:" << /*r_stdstrSyntaxTreePath*/
            stdstrCurrentElement )
          throw VTrans::UnknownGrammarPartNameException(stdstrCurrentElement);
          return unknown_grammar_part_name ;
        }
      bNewEle = false ;
    }
  } //"for"-loop
//  DEBUG_COUT("SyntaxTreePath::CreateGrammarPartIDArray size:" <<
//      vec_wElements.size() + "\n" )
  WORD * ar_wElements = new WORD [ vec_wElements.size() ] ;
  if( ar_wElements ) //<> 0
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
    //DEBUG_COUTN("SyntaxTreePath(" <<
    LOGN_DEBUG(
      this << ")::CreateGrammarPartIDArray()"
      "--array as string:"
      << str )
#endif
    return all_grammar_parts_resolved ;
  }
  return no_grammar_part_resolved ;
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

//see http://www.cs.caltech.edu/courses/cs11/material/cpp/donnie/cpp-ops.html:

//_Must_ define an assignment operator, else the array is not being copied
//but the array pointer is just referring the origin's array. ->
//the array pointer refers to an invalid location if the original array was destroyed.
SyntaxTreePath & SyntaxTreePath::operator = ( const SyntaxTreePath & c_r_stpToCopyFrom)
{
  copy(c_r_stpToCopyFrom);
  return * this;
}

std::string SyntaxTreePath::GetAs_std_string( ) const
{
  std::string stdstr ;
//#ifdef _DEBUG
  WORD wGrammarPartID ;
//#endif
//  for(WORD  w=0; w < m_ar_wElements ; ++ w )
//    stdstr += mp_parsebyrise->GetGrammarPartName( m_ar_wElements[w]) ;
  if( m_wNumberOfElements )
  {
    wGrammarPartID = m_ar_wElements[ 0 ] ;
    stdstr += mp_bottomUpParser->GetGrammarPartName( wGrammarPartID );

    for( WORD wIndex = 1 ; wIndex < m_wNumberOfElements ; ++ wIndex )
    {
  #ifdef _DEBUG
      wGrammarPartID = m_ar_wElements[ wIndex ] ;
      stdstr += "." + mp_bottomUpParser->GetGrammarPartName( wGrammarPartID );
  #else
      stdstr += "." + mp_bottomUpParser->GetGrammarPartName(
        m_ar_wElements[ wIndex ] );
  #endif
    }
  }
  return stdstr ;
}

void SyntaxTreePath::GetAsGrammarPartIDvector(
  std::vector<GrammarPart *> & r_stdvector_p_grammarpartParseTreePath ,
  std::vector<WORD> & r_stdvec_w_grammarpartPath
  )
{
  std::vector<GrammarPart *>::const_iterator c_iter =
    r_stdvector_p_grammarpartParseTreePath.begin() ;
  WORD wIndex = 0 ;
  while( c_iter != r_stdvector_p_grammarpartParseTreePath.end() )
  {
#ifdef _DEBUG
    WORD wGrammarPartID = (*c_iter)->m_wGrammarPartID;
    r_stdvec_w_grammarpartPath.push_back( wGrammarPartID ) ;
#else
    r_stdvec_w_grammarpartPath.push_back( (*c_iter)->m_wGrammarPartID ) ;
#endif
    ++ wIndex ;
    ++ c_iter ;
  }
}

//TODO: make inline?
/** Gets the node GrammarPart pointer that is common between m_ar_wElements
 *  and r_stdvec_p_grammarpartPath */
GrammarPart * SyntaxTreePath::GetCommonNode(
  const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath) const
{
  GrammarPart * p_grammarpart = NULL ;
  //e.g. find "def_article_noun.noun" in
  // "subject.def_article_noun.definite_article"
  
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
  //      DEBUG_COUTN("SyntaxTreePath--"
    LOGN_DEBUG("current grammar part: "
      << *c_reverse_iter_stdvec_p_grammarpart
      << " ID:" << (*c_reverse_iter_stdvec_p_grammarpart)->m_wGrammarPartID
      << " as string:" << mp_bottomUpParser->GetGrammarPartName(
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
  #ifdef _DEBUG
      std::string std_strGrammarPartNameGrammarPartPath =
        mp_bottomUpParser->GetGrammarPartName( p_grammarpart->m_wGrammarPartID);
  #endif
      break ;
    }
  }
  return p_grammarpart;
}

/** Needed for advancing directing leaf node. */
GrammarPart * MatchesGrammarPartIDOfLeftOrRightChild(
  GrammarPart * p_grammarpart,
  const fastestUnsignedDataType uiCurrentGrammarPartIDfromThisSyntaxTreePath)
{
  GrammarPart * p_grammarpartChild = p_grammarpart->mp_grammarpartLeftChild ;
  if( p_grammarpartChild &&
    p_grammarpartChild->m_wGrammarPartID ==
     uiCurrentGrammarPartIDfromThisSyntaxTreePath )
  {
    p_grammarpart = p_grammarpartChild ;
  }
  else
  {
    p_grammarpartChild = p_grammarpart->mp_grammarpartRightChild ;
    if( p_grammarpartChild &&
      p_grammarpartChild->m_wGrammarPartID ==
       uiCurrentGrammarPartIDfromThisSyntaxTreePath )
    {
      p_grammarpart = p_grammarpartChild ;
    }
    else //neither right nor left child match the syntax tree path.
//              if(bKleeneStarOperator)
//              {
//              }
//              else
        return NULL ;
  }
  return p_grammarpart;
}

//TODO: make inline?
/** Depth-First Search algorithm for the Kleene star property:
    at first all left children are taken until a leaf node is reached, then the 
 *  right childen beginning with the first inserted one are used. */
GrammarPart * GetNodeDirectingLeaves(
  const fastestUnsignedDataType uiCurrentGrammarPartIDfromThisSyntaxTreePath,
  GrammarPart * p_grammarpart)
{
  std::deque<GrammarPart *> unvisitedGrammarPartVector;
  /** Strategy : visit left nodes first */
  do
  {
    if( p_grammarpart->mp_grammarpartLeftChild )
    {
      if( p_grammarpart->mp_grammarpartRightChild )
        unvisitedGrammarPartVector.push_back(p_grammarpart->mp_grammarpartRightChild);
      p_grammarpart = p_grammarpart->mp_grammarpartLeftChild;
    }
    else
    {
      /** No left and no right child */
      if( unvisitedGrammarPartVector.empty() )
        return NULL;
      else
      {
        //the elements the beginning are nearer the common node/ root
        p_grammarpart = unvisitedGrammarPartVector.front();
        unvisitedGrammarPartVector.pop_front();
      }
    }
//    if(  )
//      ;
    
//    p_grammarpartChild = MatchesGrammarPartIDOfLeftOrRightChild(p_grammarpart, uiCurrentGrammarPartIDfromThisSyntaxTreePath);
  }while( p_grammarpart->m_wGrammarPartID != 
        uiCurrentGrammarPartIDfromThisSyntaxTreePath );
  return p_grammarpart;
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
//   the contained grammar part ID array:
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
      mp_bottomUpParser->GetPathAs_std_string(
    r_stdvec_p_grammarpartPath ) ;
  std::string strWORDarray = GetAs_std_string( ) ;
//  DEBUG_COUTN("SyntaxTreePath(" <<
  LOGN_DEBUG(
    this //<< ")::GetLeaf()"
    //"--array as string: "
    << " this STP: " << strWORDarray
    << " other STP: " << strPathFromGrammarPartPointerVector )
#endif
  if( m_wNumberOfElements > 0 )
  {
    GrammarPart * p_grammarpart = GetCommonNode(r_stdvec_p_grammarpartPath);
    //e.g. now we have "def_article_noun" from syntax tree path 
    // "def_article_noun.noun".
    if( p_grammarpart )
    {
      //bool
      GrammarPart * p_grammarpartChild = NULL ;
      unsigned uiCurrentGrammarPartIDfromThisSyntaxTreePath;
      std::string std_strCurrentGrammarPartNameFromThisSyntaxTreePath;
#ifdef _DEBUG
      std::string std_strGrammarPartNameGrammarPartPath;
#endif
      bool bKleeneStarOperator = false;
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
        uiCurrentGrammarPartIDfromThisSyntaxTreePath = m_ar_wElements[ wIndex ];
#ifdef _DEBUG
        std_strCurrentGrammarPartNameFromThisSyntaxTreePath =
          mp_bottomUpParser->GetGrammarPartName(
          uiCurrentGrammarPartIDfromThisSyntaxTreePath );
        std_strGrammarPartNameGrammarPartPath =
          mp_bottomUpParser->GetGrammarPartName( p_grammarpart->m_wGrammarPartID);
#endif //#ifdef _DEBUG
        LOGN_DEBUG("the next child should have grammar part ID: "
          << m_ar_wElements[ wIndex ]
          << " as string: "
          << std_strCurrentGrammarPartNameFromThisSyntaxTreePath
          )
        
        /** Use Kleene star : walk nodes directing leaves. 
        * This minimizes translation rules for object e.g. :
          Just 1 translation rule :  "clauseWith1Obj.*.mainVerbInf1Obj"
          instead of many :
           -"clauseWith1Obj.1stPersSingSimplePresentClauseAllow1Obj.mainVerbInf1Obj" 
           -"clauseWith1Obj.2ndPersSingSimplePastClauseAllow1Obj.mainVerbInf1Obj" 
         For declination of "der" in "I like the man" -> "Ich mag DEN Mann" */
              
        if( uiCurrentGrammarPartIDfromThisSyntaxTreePath ==
            KLEENE_STAR_OPERATOR)
        {
          bKleeneStarOperator = true;
          continue;
        }
        if( bKleeneStarOperator)
        {
          p_grammarpart = GetNodeDirectingLeaves(
            uiCurrentGrammarPartIDfromThisSyntaxTreePath, p_grammarpart);
//          if( p_grammarpartChild )
//            p_grammarpart = p_grammarpartChild;
//          else
//            break;
          if( ! p_grammarpart )
            return NULL;
//          if( mp_parsebyrise->GetGrammarPart(
//              p_grammarpart,
//              uiCurrentGrammarPartIDfromThisSyntaxTreePath)
//              )
        }
        else
        {
          p_grammarpart = MatchesGrammarPartIDOfLeftOrRightChild(
            p_grammarpart, uiCurrentGrammarPartIDfromThisSyntaxTreePath);
          if( ! p_grammarpart )
            return NULL;
        }
//        if( ! p_grammarpartChild )
//          break ;
      }
//      if( p_grammarpartChild )
//        p_grammarpartRet = p_grammarpartChild ;
//      else
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

//bool SyntaxTreePath::Matches(
//  const std::vector<WORD> & cr_stdvec_wGrammarPartPath ,
//  const std::vector<GrammarPart *> & cr_stdvec_grammarpartPath
//  ) const
//{
//  mp_parsebyrise->Get
//}

bool SyntaxTreePath::Matches(
  const std::vector<WORD> & cr_stdvec_wGrammarPartPath
  ) const
{
  return Matches(
    m_ar_wElements,
    m_wNumberOfElements ,
    cr_stdvec_wGrammarPartPath//,
//    mp_parsebyrise
    ) ;
}

bool SyntaxTreePath::Matches(
  WORD * ar_wElements ,
  WORD wNumberOfElements ,
  const std::vector<WORD> & cr_stdvec_wGrammarPartPath
  ) //const
{
  bool bIdentical = false ;
  bool bCurrentlyKleeneStarOperator = false ;
  //e.g. compare "*.1stPersSingPresentProgressive.mainVerbInf0Obj" with
  //"1stPersSingPresentProgressive.mainVerbInf0Obj"
//  if( cr_stdvec_wGrammarPartPath.size() >= wNumberOfElements
//      //Not 0 elements <=> At least 1 element.
//      && wNumberOfElements
//    )
  {
    bIdentical = true ;
//    WORD wLenghtDiff = r_stdvec_wCurrentGrammarPartPath.size() -
//        m_wNumberOfElements ;
    std::vector<WORD>::const_reverse_iterator c_rev_iter_wGrammarPartPath =
        cr_stdvec_wGrammarPartPath.rbegin() ;
    //Compare from end to begin.
    WORD wIndex ;
    WORD wGrammarPartIDforTranslationRule ;
    WORD wCurrentGrammarPartIDforCurrentParseTreePath;
    //Avoid g++ warning "'wGrammarPartIDforTranslationRule' might be used
    // uninitialized in this function"
    SUPPRESS_UNUSED_VARIABLE_WARNING(wGrammarPartIDforTranslationRule)
    for( wIndex = //p_tr->m_wNumberOfElements - 1 ;
        wNumberOfElements - 1 ;
        wIndex != MAXWORD
          //UINT16_MAX
        && c_rev_iter_wGrammarPartPath != cr_stdvec_wGrammarPartPath.rend()
            ; //-- wIndex
//        ++ rev_iter
        )
    {
//      DEBUG_COUTN
//      DEBUGN
//      ( "comparing " << //p_tr->m_ar_wElements [ wIndex ] <<
//          ar_wElements [ wIndex ] <<
//        mp_parsebyrise->GetGrammarPartName( //p_tr->m_ar_wElements [ wIndex ]
//            ar_wElements [ wIndex ]
//        )
//        << " and "
////        << r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
//        << * c_rev_iter_wGrammarPartPath
//        << mp_parsebyrise->GetGrammarPartName(
//            //r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
//          * c_rev_iter_wGrammarPartPath
//          )
//        )
//      if( //p_tr->m_ar_wElements [ wIndex ] !=
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
        //Matches the grammar part ID following the Kleene star operator.
        if( wGrammarPartIDforTranslationRule == *c_rev_iter_wGrammarPartPath )
        {
          bCurrentlyKleeneStarOperator = false ;
          -- wIndex ;
        }
        ++ c_rev_iter_wGrammarPartPath ;
      }
      else
      {
        wGrammarPartIDforTranslationRule =
            ar_wElements [ wIndex ] ;
        if( wGrammarPartIDforTranslationRule == KLEENE_STAR_OPERATOR )
        {
          if( wIndex - 1 >= 0 )
            -- wIndex ;
          else
            return //false ;
              //<=> identical because "*" means: 0 or any amount of grammar
              //parts may follow.
              true;
          bCurrentlyKleeneStarOperator = true ;
          wGrammarPartIDforTranslationRule =
              ar_wElements [ wIndex ] ;
        }
        else
        {
          wCurrentGrammarPartIDforCurrentParseTreePath =
            * c_rev_iter_wGrammarPartPath;
          if( wGrammarPartIDforTranslationRule !=
              //r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
              wCurrentGrammarPartIDforCurrentParseTreePath
            )
          {
            bIdentical = false ;
//            std::string std_strCurrentGrammarPartNameforCurrentParseTreePath =
//              mp_parsebyrise->GetGrammarPartName(
//                wCurrentGrammarPartIDforCurrentParseTreePath);
//            std::string std_strCurrentGrammarPartNameforTranslationRule =
//              mp_parsebyrise->GetGrammarPartName(
//                wGrammarPartIDforTranslationRule);
//            DEBUGN( FULL_FUNC_NAME << "--not identical\n")
            break ;
          }
          -- wIndex ;
          //Only advance if the current grammar part ID is not Kleene star operator
          //because the ID after Kleene star *must* match the (indirect) ID of
          //the vector.
          ++ c_rev_iter_wGrammarPartPath ;
        }
      }
    }

//    //If kleene star and at least 1 grammar part follows (= the Kleene start
//    //is not the last grammar part).
//    if( bCurrentlyKleeneStarOperator &&
//        //grammar part iID following the kleene start oper is not a kleene star
//        wGrammarPartIDforTranslationRule != KLEENE_STAR_OPERATOR )

    //If 1 path is "definite_article_singular.definite_article" and the other
    //is "obj.definite_article_singular.definite_article" it is not identical.
    if( c_rev_iter_wGrammarPartPath == cr_stdvec_wGrammarPartPath.rend() )
    {
      //if e.g. "*.1stPersSingPresentProgressive.MainVerbProgressive0Obj" and
      // "MainVerbProgressive0Obj"
//      if( wIndex != MAXWORD && ar_wElements[ wIndex] != KLEENE_STAR_OPERATOR )

      //e.g. at end of "definite_article_singular.definite_article" and at
      // "definite_article_singular" for path
      // "*.obj.*.definite_article_singular.definite_article"
      for( ; wIndex != MAXWORD; -- wIndex)
      {
        //e.g. at "obj" of "*.obj.*.definite_article_singular.definite_article"
        if( ar_wElements[ wIndex] != KLEENE_STAR_OPERATOR )
        {
          bIdentical = false;
          break;
        }
      }
    }
    else
      bIdentical = false;
  }
  //If currently there is a Kleene star operand, the rule is NOT identical
  //because a next (indirect) vector's grammar part ID must match the
  //translation rule's grammar part ID:
  //e.g. for translation grammar part path
  //"obj.*.def_article_plural.def_article"
  //the vector's grammar part path must contain an "obj" grammar part after
  // a def_article_plural gramar part.
  return ! bCurrentlyKleeneStarOperator &&
      bIdentical ;
}
