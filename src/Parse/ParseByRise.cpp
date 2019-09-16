/** File:   ParseByRise.cpp
 * Author: Stefan
 * Created on 13. Februar 2010, 21:05  */

#include <algorithm>
#include <set> //for std::set

//class VocabularyAndTranslation 

//TranslationControllerBase::s_lettertree
//#include <Controller/TranslationControllerBase.hpp>
#include <Controller/TranslationProcess.hpp> //class TranslationProcess

/** SUPPRESS_UNUSED_VARIABLE_WARNING(...) */
#include <compiler/GCC/suppress_unused_variable.h>

#include "ParseByRise.hpp" //class BottumUpParser declararation
#include <Attributes/EnglishWord.hpp> //for class EnglishWord's English_word_class enum
#include <Attributes/Token.h> //class PositionStringVector
#include <preprocessor_macros/logging_preprocessor_macros.h> //DEBUG_COUT(...)
#include <UserInterface/I_UserInterface.hpp>//class I_UserInterface
//#include <VocabularyInMainMem/LetterTree/LetterNode.hpp>//class LetterNode
//#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>//class LetterTree
//class VocabularyAndTranslation
#include <VocabularyInMainMem/VocabularyAndTranslation.hpp>
#include <IO/UnknownGrammarPartNameException.hpp>
//#include <Xerces/ReadViaSAX2.hpp>//ReadViaSAX2InitAndTermXerces(...)
//#include <Xerces/SAX2GrammarRuleHandler.hpp>//class SAX2GrammarRuleHandler

//extern LetterTree g_lettertree ;

//ParseByRise::ParseByRise()
//  :
//  m_p_userinterface(NULL)
//{
////  DEBUGN("ParseByRise "
//  LOGN_DEBUG("begin")
//  Init();
////  DEBUGN("ParseByRise "
//  LOGN_DEBUG("end")
//}
//
//ParseByRise::ParseByRise(
//  I_UserInterface & r_userinterface )
//  :
//  m_p_userinterface(& r_userinterface)
//   //m_wNumberOfSuperordinateRules( 0 )
//  //,
//{
//  LOGN_DEBUG("begin")
//  Init();
////  DEBUG_COUTN("ParseByRise(I_UserInterface &) "
//  LOGN_DEBUG("end")
//}
namespace VTrans3
{
BottomUpParser::BottomUpParser( /*TranslationControllerBase & r_translationcontrollerbase*/ 
    I_UserInterface * p_userInterface,
    TranslationProcess & r_translationProcess
  )
  : m_p_userinterface(/*& r_translationcontrollerbase*/ p_userInterface)
  , //m_r_translationcontrollerbase(r_translationcontrollerbase)
    m_r_translationProcess(r_translationProcess)
  , m_pInputText(NULL)
{ //TODO "this" can't be ausgeklappt werden in NetBeans "Variables" view at this  point
  Init();
}

void BottomUpParser::Init()
{
  LOGN_DEBUG("begin")
  m_dwMapIndex = 0;
  m_wParseLevel = 0;
  InitGrammarRules() ;
  LOGN_DEBUG("end")
}

//ParseByRise::ParseByRise(const ParseByRise& orig) {
//}

BottomUpParser::~BottomUpParser()
{
  LOGN_DEBUG(//"~ParseByRise() "
    "begin")
  std::multimap<DWORD,GrammarPart *>::const_iterator c_iter =
      m_stdmultimap_dwLeftmostIndex2p_grammarpart.begin() ;
  for( ; c_iter != m_stdmultimap_dwLeftmostIndex2p_grammarpart.end() ;
    ++ c_iter )
  {

//    DEBUG_COUTN(
    LOGN_DEBUG(
      "deleting grammar part " << GetGrammarPartName(
      c_iter->second->m_wGrammarPartID ) )
    /** Release mem only for the GrammarPart for the leftmost index mmap and not twice:
    * leftmost index mmap has the address of the _same_ GrammarPart stored. */
    //FIXME SIGSEV here
    delete c_iter->second ;
  }
  /** Empty containers here to see which container may cause a runtime arror.*/
  m_stdmultimap_dwLeftmostIndex2p_grammarpart.clear();
  m_stdmultimap_dwRightmostIndex2p_grammarpart.clear();
  m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel.clear();
  m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel.clear();
//  mp_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate ;
//  mp_stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;
  LOGN_DEBUG(//"~ParseByRise() "
    "end")
}

void BottomUpParser::ClearAllGrammarStuff()
{
  LOGN_DEBUG(//"ParseByRise::ClearAllGrammarStuff() "
    "begin")
  m_stdmap_wRuleID2RuleName.clear() ;
  m_stdmap_RuleName2RuleID.clear() ;
  m_stdmultimap_wGrammarPartID2SuperordinateID.clear() ;
  //Used in InsertIfGrammarRuleAppliesTo(...) for getting all grammar rules
  // whose left child has the same grammarpart ID is the map's key value.
  m_stdmmap_wLeftChildGrammarPartID2SuperordinateGrammarRule.clear() ;

  m_stdmultimap_wGrammarPartID2wGrammarPartID.clear() ;
//  m_stdmap_RuleName2RuleID.clear() ;
  m_stdmap_wGrammarPartID2SuperordinateID.clear() ;
  LOGN_DEBUG(//"ParseByRise::ClearAllGrammarStuff() "
    "end")
}

/** Clears (empties) the previously generated parse tree.
* This should be done for a next parse tree generation . */
void BottomUpParser::ClearParseTree()
{
  const fastestUnsignedDataType numSuperOrdinateGrammarParts =
    //m_stdset_grammarpartAllSuperordinate.size();
    m_allSuperordinateGrammarParts2pointerToThem.size();
  LOGN_DEBUG("# of superordinate grammar parts: "
    << numSuperOrdinateGrammarParts )
  //m_stdset_grammarpartAllSuperordinate.clear() ;
  m_allSuperordinateGrammarParts2pointerToThem.clear();
  m_wBiggestNumberOfTokensForAppliedGrammarRule = 0 ;
//   m_stdmultimap_dwRightmostIndex2grammarpart.clear() ;
//   m_stdmultimap_dwLeftmostIndex2grammarpart.clear() ;
   m_stdmultimap_dwRightmostIndex2p_grammarpart.clear() ;

   std::multimap<DWORD, GrammarPart *>::const_iterator c_iter =
       m_stdmultimap_dwLeftmostIndex2p_grammarpart.begin();
   GrammarPart * p_gpCurrent;
   while( c_iter != m_stdmultimap_dwLeftmostIndex2p_grammarpart.end() )
   {
     /** Was allocated on the heap */
     p_gpCurrent = c_iter->second;
     delete p_gpCurrent;
     ++ c_iter;
   }
   m_stdmultimap_dwLeftmostIndex2p_grammarpart.clear() ;

//   m_stdset_p_grammarpartAllSuperordinate.clear() ;
 }

void BottomUpParser::DeleteFromOutMostTokenIndexContainer(
  GrammarPart * p_grammarpartRootNode,
  std::multimap<DWORD, GrammarPart *> & r_std_multimap_dw2p_grammarpart,
  DWORD dwTokenIndex
  )
{
  std::multimap<DWORD, GrammarPart *>::
    //iterator mustn't be "const" for multimap::erase(...)
    iterator iter =
      //m_stdmultimap_dwRightmostIndex2p_grammarpart.find(
      r_std_multimap_dw2p_grammarpart.find(
//        p_grammarpartRootNode->m_dwRightmostIndex
      dwTokenIndex
      );
  while( iter != r_std_multimap_dw2p_grammarpart.end() && iter->first ==
//        p_grammarpartRootNode->m_dwRightmostIndex
      dwTokenIndex
      )
  {
    if( iter->second == p_grammarpartRootNode)
    {
//          m_stdmultimap_dwRightmostIndex2p_grammarpart.de() ;
      r_std_multimap_dw2p_grammarpart.erase(iter);
      break;
    }
    ++ iter;
  }
}

std::string BottomUpParser::GetErrorMessage(
  const enum BottomUpParser::InsertGrammarRuleReturnCodes insertGrammarRuleReturnCode
  //,stdstrLeftChild
  )
{
  GCC_DIAG_OFF(switch)
  switch(insertGrammarRuleReturnCode)
  {
  case BottomUpParser::unknownLeftGrammarPart:
    return "unknown left child grammar part"; // \""
      /*GetStdWstring(stdstrLeftChild ) + L"\"";*/
//    break;
  case BottomUpParser::unknownRightGrammarPart:
    return "unknown right child grammar part";// \""
      /* + GetStdWstring(stdstrRightChild ) + L"\"";*/
//    break;
  case BottomUpParser::unknownLeftAndRightGrammarPart:
      return "unknown left "// \""
//      + GetStdWstring(stdstrLeftChild)
        "and right " //\"" + GetStdWstring(stdstrRightChild)
      /*+ L"\*/" child grammar part";
    break;
  }
  GCC_DIAG_ON(switch)
  return "";
}

//TODO what if more than 1 parse tree covers the same amount of tokens?
/* @return the root of the parse tree that covers the widest
 *  range of tokens. This is senseful because it is wanted to translate a
 * the largest contigious block of tokens.
 * E.g.: "the vacuum cleaner sucks." : get the largest covering tokens starting at
 *   pos "0".
   : "the" (pos 0-0) covers 1 token,
   : "the vacuum cleaner" (pos 0-2) covers 3 tokens
   : "the vacuum cleaner sucks." (pos 0-3) covers 4 tokens  */
GrammarPart * BottomUpParser::GetParseTreeCoveringMostTokens(
  DWORD dwLeftmostTokenIndex
  )
{
  DWORD dwNumberOfTokensCoveredMax = 0 ;
  DWORD dwNumberOfTokensCoveredCurrent ;
  GrammarPart * p_gp = NULL ;
  //std::multimap<DWORD, GrammarPart>::const_iterator iter ;

  //Get a list with all parse trees that have the leftmost token at index
  //"dwLeftmostTokenIndex".
//  std::pair<c_iter_mmap_dw2grammarpart,c_iter_mmap_dw2grammarpart>
//    stdpair_iter =
//    m_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
//    dwLeftmostTokenIndex) ;
  std::pair<c_iter_mmap_dw2p_grammarpart,c_iter_mmap_dw2p_grammarpart>
    stdpair_iter =
    m_stdmultimap_dwLeftmostIndex2p_grammarpart.equal_range(
    dwLeftmostTokenIndex) ;
#ifdef _DEBUG
  WORD wNumberOfGrammapartsStartingAtPos = 0 ;
#endif
  for( //c_iter_mmap_dw2grammarpart iterCurrent
      c_iter_mmap_dw2p_grammarpart iterCurrent = stdpair_iter.first ;
      iterCurrent != stdpair_iter.second ; ++ iterCurrent )
  {
#ifdef _DEBUG
    ++ wNumberOfGrammapartsStartingAtPos ;
#endif
    dwNumberOfTokensCoveredCurrent = //iterCurrent->second.m_dwRightmostIndex -
      iterCurrent->second->m_dwRightmostIndex -
      dwLeftmostTokenIndex
      //to include grammar parts with 1 token
      + 1 ;
    if( dwNumberOfTokensCoveredCurrent > dwNumberOfTokensCoveredMax )
    {
      p_gp = //(GrammarPart *) & iterCurrent->second ;
          iterCurrent->second ;
      dwNumberOfTokensCoveredMax = dwNumberOfTokensCoveredCurrent ;
    }
    //There may be more than a valid parse tree with the same number of tokens:
    //e.g. for "the sheep":
    //        "the sheep"
    //            \  /
    //     DefiniteArticlePlural  -> "die Schafe"
    //        "the sheep"
    //            \  /
    //     DefiniteArticleSingular -> "das Schaf"
    else if( dwNumberOfTokensCoveredCurrent == dwNumberOfTokensCoveredMax )
    {
#ifdef _DEBUG
      //This instruction is just for setting a breakpoint.
      //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//      #pragma GCC diagnostic ignored  "-Wunused"
      //Useless instruction just for breakpoint possibility.
      int i = 0 ;
      SUPPRESS_UNUSED_VARIABLE_WARNING(i)
#endif
    }
  }
#ifdef _DEBUG
  if( ! p_gp )
    {
    //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//    #pragma GCC diagnostic ignored  "-Wunused"
    //Useless instruction just for breakpoint possibility.
    int i = 0 ;
    SUPPRESS_UNUSED_VARIABLE_WARNING(i)
    }
#endif
  return p_gp ;
}

/** @param r_stdvec_p_grammarpartCoveringMostTokensATokentIndex :
 *   all parse trees beginning at token index "dwLeftMostTokenIndex".
*    Often these parse trees here have the same structure but different
*    words (->synonyms): "the man works."-> "der Mann|Mensch arbeitet" */
void BottomUpParser::GetGrammarPartCoveringMostTokens(
  DWORD dwLeftmostTokenIndex ,
  //Should be a vector because it may be more then 1 token groups may have
  //different translations like "the sheep" -> "das Schaf", "die Schafe"
  std::vector<GrammarPart *> &
    r_stdvec_p_grammarpartCoveringMostTokensATokentIndex
  ) const
{
  DWORD dwNumberOfTokensCoveredMax = 0 ;
  DWORD dwNumberOfTokensCoveredCurrent ;
  GrammarPart * p_grammarpart = NULL ;
  //May not be empty at the beginning.
  r_stdvec_p_grammarpartCoveringMostTokensATokentIndex.clear() ;
  //std::multimap<DWORD, GrammarPart>::const_iterator iter ;

  //Get a list with all parse trees that have the leftmost token at index
  //"dwLeftmostTokenIndex".
//  std::pair<c_iter_mmap_dw2grammarpart,c_iter_mmap_dw2grammarpart>
//    stdpair_iter =
//    m_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
  std::pair<c_iter_mmap_dw2p_grammarpart,c_iter_mmap_dw2p_grammarpart>
    stdpair_iter_multimap_dwTokenIndex2p_grammarpart_ElementsAtTokenIndex =
    m_stdmultimap_dwLeftmostIndex2p_grammarpart.equal_range(
    dwLeftmostTokenIndex) ;
#ifdef _DEBUG
  WORD wNumberOfGrammapartsStartingAtPos = 0 ;
#endif
  for( //c_iter_mmap_dw2grammarpart
      c_iter_mmap_dw2p_grammarpart c_iter_mmap_dw2p_grammarpartCurrent =
        stdpair_iter_multimap_dwTokenIndex2p_grammarpart_ElementsAtTokenIndex.
        first ;
      c_iter_mmap_dw2p_grammarpartCurrent !=
        stdpair_iter_multimap_dwTokenIndex2p_grammarpart_ElementsAtTokenIndex.
        second ;
      ++ c_iter_mmap_dw2p_grammarpartCurrent )
  {
#ifdef _DEBUG
    ++ wNumberOfGrammapartsStartingAtPos ;
#endif
    dwNumberOfTokensCoveredCurrent = //iterCurrent->second.m_dwRightmostIndex -
        c_iter_mmap_dw2p_grammarpartCurrent->second->m_dwRightmostIndex -
      dwLeftmostTokenIndex
      //to include grammar parts with 1 token
      + 1 ;
    if( dwNumberOfTokensCoveredCurrent > dwNumberOfTokensCoveredMax )
    {
      //drop the added items with less # of tokens
      r_stdvec_p_grammarpartCoveringMostTokensATokentIndex.clear() ;
//#ifdef _DEBUG
      p_grammarpart = //(GrammarPart * ) & iterCurrent->second ;
        c_iter_mmap_dw2p_grammarpartCurrent->second ;
      r_stdvec_p_grammarpartCoveringMostTokensATokentIndex.push_back(
        p_grammarpart ) ;
//#endif
      dwNumberOfTokensCoveredMax = dwNumberOfTokensCoveredCurrent ;
    }
    //There may be more than a valid parse tree with the same number of tokens:
    //e.g. for "the sheep":
    //        "the sheep"
    //            \  /
    //     DefiniteArticlePlural  -> "die Schafe"
    //        "the sheep"
    //            \  /
    //     DefiniteArticleSingular -> "das Schaf"
    else if( dwNumberOfTokensCoveredCurrent == dwNumberOfTokensCoveredMax )
    {
#ifdef _DEBUG
      //This instruction is just for setting a breakpoint.
      //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//      #pragma GCC diagnostic ignored  "-Wunused"
      //Useless instruction just for breakpoint possibility.
      int i = 0 ;
      SUPPRESS_UNUSED_VARIABLE_WARNING(i)
#endif
      p_grammarpart = //(GrammarPart * ) & iterCurrent->second ;
          c_iter_mmap_dw2p_grammarpartCurrent->second ;
      r_stdvec_p_grammarpartCoveringMostTokensATokentIndex.push_back(
        p_grammarpart ) ;
    }
  }
#ifdef _DEBUG
  if( ! p_grammarpart )
    {
    //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//    #pragma GCC diagnostic ignored  "-Wunused"
    //Useless instruction just for breakpoint possibility.
    int i = 0 ;
    SUPPRESS_UNUSED_VARIABLE_WARNING(i)
    }
#endif
//  return p_grammarpart ;
}

bool BottomUpParser::GetGrammarPartID(
  const std::string & cr_stdstrGrammarPartName, PTN_IDtype & r_parseTreeNodeID)
{
//  DEBUG_COUTN("ParseByRise::GetGrammarPartID(" <<
  LOGN_DEBUG(
    cr_stdstrGrammarPartName << ",...) begin")
  bool bSuccess = false ;
  PTNtypeName2PTN_IDtype::const_iterator iter =
    m_stdmap_RuleName2RuleID.find(cr_stdstrGrammarPartName) ;
  if( iter != m_stdmap_RuleName2RuleID.end() )
  {
    r_parseTreeNodeID = iter->second ;
    bSuccess = true ;
  }
//  DEBUG_COUTN("ParseByRise::GetGrammarPartID(" <<
  LOGN_DEBUG(
    cr_stdstrGrammarPartName << ",...) return " << bSuccess )
  return bSuccess ;
}

std::string BottomUpParser::GetGrammarPartName(PTN_IDtype ruleID) const
{
//  std::string stdstrRuleName ;
  PTN_ID2PTNtypeNameType::const_iterator iter =
    m_stdmap_wRuleID2RuleName.find( ruleID ) ;
  if( iter != m_stdmap_wRuleID2RuleName.end() )
    return iter->second ;
  return std::string(//"?gram pt"
      "*") ;
}

fastestUnsignedDataType BottomUpParser::GetNumberOfLargestParseTrees()
{
  fastestUnsignedDataType numberOfLargestParseTrees = 0;
  DWORD dwLeftMostTokenIndex = 0 ;
  std::vector<GrammarPart *> stdvec_p_grammarpartCoveringMostTokensAtTokenIndex;
  do
  {
    GetGrammarPartCoveringMostTokens(
      dwLeftMostTokenIndex ,
      stdvec_p_grammarpartCoveringMostTokensAtTokenIndex
      ) ;
    WORD wSize = stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.size();
    numberOfLargestParseTrees += wSize;
    if( stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.empty() )
      dwLeftMostTokenIndex = 0;
    else
      dwLeftMostTokenIndex =
        stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.at(0)->
        m_dwRightmostIndex + 1;
  }
  while( dwLeftMostTokenIndex );
  return numberOfLargestParseTrees;
}

void BottomUpParser::GetLargestParseTrees(
  std::vector<GrammarPart *> & r_stdvec_p_grammarpartLargestParseTrees)
{
  r_stdvec_p_grammarpartLargestParseTrees.clear();
  DWORD dwLeftMostTokenIndex = 0 ;
  std::vector<GrammarPart *> stdvec_p_grammarpartCoveringMostTokensAtTokenIndex;
  do
  {
    GetGrammarPartCoveringMostTokens(
      dwLeftMostTokenIndex ,
      stdvec_p_grammarpartCoveringMostTokensAtTokenIndex
      ) ;
    WORD wSize = stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.size();
    if( stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.empty() )
      dwLeftMostTokenIndex = 0;
    else
      dwLeftMostTokenIndex =
        stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.at(0)->
        m_dwRightmostIndex + 1;
    //TODO? : There is no information that the parse trees inside the vector to
    //  insert have the same length after inserting into the other vector.
    r_stdvec_p_grammarpartLargestParseTrees.insert(
      r_stdvec_p_grammarpartLargestParseTrees.end(),
      stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.begin(),
      stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.end());
    LOGN_DEBUG("dwLeftMostTokenIndex:" << dwLeftMostTokenIndex )
  }
  while( dwLeftMostTokenIndex );
}


std::string BottomUpParser::GetPathAs_std_string(
  const std::vector<WORD> & r_stdvec_wGrammarPartPath )
{
  std::string stdstr ;
  WORD wIndex = 0 ;
  //Gets "-1"->65535 as unsigned if vector size is "0", but then the loop body
  //is not executed.
  WORD wSizeMinus1 = r_stdvec_wGrammarPartPath.size() - 1 ;
//  for(WORD  w=0; w < m_ar_wElements ; ++ w )
//    stdstr += mp_parsebyrise->GetGrammarPartName( m_ar_wElements[w]) ;
  for( std::vector<WORD>::const_iterator iter =
    r_stdvec_wGrammarPartPath.begin() ;
    iter != r_stdvec_wGrammarPartPath.end() ; ++ iter )
  {
    stdstr += GetGrammarPartName( *iter ) ;
    //Not the last element (if size == 2: wSizeMinus1-> 1 ).
    if( wIndex < wSizeMinus1 )
      stdstr += "." ;
    ++ wIndex ;
  }
  return stdstr ;
}

std::string BottomUpParser::GetPathAs_std_string(
  const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath )
{
  std::string stdstr ;
  WORD wIndex = 0 ;
  //Gets "-1"->65535 as unsigned if vector size is "0", but then the loop body
  //is not executed.
  WORD wSizeMinus1 = r_stdvec_p_grammarpartPath.size() - 1 ;
//  for(WORD  w=0; w < m_ar_wElements ; ++ w )
//    stdstr += mp_parsebyrise->GetGrammarPartName( m_ar_wElements[w]) ;
  for( std::vector<GrammarPart *>::const_iterator iter =
      r_stdvec_p_grammarpartPath.begin() ;
    iter != r_stdvec_p_grammarpartPath.end() ; ++ iter )
  {
    stdstr += GetGrammarPartName( (*iter)->m_wGrammarPartID ) ;
    //Not the last element (if size == 2: wSizeMinus1-> 1 ).
    if( wIndex < wSizeMinus1 )
      stdstr += "." ;
    ++ wIndex ;
  }
  return stdstr ;
}

//return: bitfield: 1 bit for every single person Index (1st person singular...3rd
//person  plural)
//TODO: combine child node's person indices to the value of the
//parent node when going upwards (->root node) at parsing should be faster:
//
//                 /  
//  ee_comma_ee node:  |
//  3rd pers. plural   |
//            /  \     |
// ee_comma node: \   ee_conj_ee node:
// 3rd pers. sing. |  1st pers. pl. ("we")
//       /  \        /   |
//  +-----+   +-----+   / \ (if "\"= last char:g++ warning:"multi-line comment")
//  the car , the cat and I
BYTE BottomUpParser::GetSubjectPersonIndex( GrammarPart * p_grammarpart)
{
//  if( p_grammarpart )
//  {
//    DEBUG_COUT( "GetSubjectPersonIndex\n" );
//
//    m_stdvec_wGrammarPartPath.push_back( p_grammarpart->m_wGrammarPartID ) ;
//    m_stdvec_p_grammarpartPath.push_back( p_grammarpart ) ;
//
//    ++ m_wParseLevel ;
//    DEBUG_COUT( "GetSubjectPersonIndex--current syntax tree path:" <<
//      GetSyntaxTreePathAsName( m_stdvec_wGrammarPartPath ) << "\n" )
//    do
//    {
//      DEBUG_COUT ( "GetSubjectPersonIndex--current syntax tree path:" <<
//        GetSyntaxTreePathAsName( m_stdvec_wGrammarPartPath ) << "\n" );
//      if( p_grammarpart->mp_grammarpartRightChild )
//       {
//        p_grammarpartChild = p_grammarpart->mp_grammarpartRightChild ;
////           std::cout << "pushinG "
////             << mp_parsebyrise->GetGrammarPartName(
////               //p_grammarpart->mp_grammarpartRightChild->
////               p_grammarpartChild->m_wGrammarPartID )
////             << p_grammarpartChild
////             << " current size="
////             << m_stdvecNodesToProcess.size()
////             << "\n" ;
//         //Add to process possible children of the right nodes later.
//         m_stdvecNodesToProcess.push_back( //p_grammarpart->mp_grammarpartRightChild
//           p_grammarpartChild ) ;
//       }
//      if( p_grammarpart->mp_grammarpartLeftChild )
//      {
//        p_grammarpart = p_grammarpart->mp_grammarpartLeftChild ;
//
//        m_stdvec_wGrammarPartPath.push_back(
//            p_grammarpart->m_wGrammarPartID ) ;
//        m_stdvec_p_grammarpartPath.push_back( p_grammarpart) ;
//
////          std::cout << "current syntax tree path:" << GetSyntaxTreePathAsName(
////            r_parsebyrise ) << "\n" ;
//        ++ m_wParseLevel ;
//      }
//      else //No further child->Left _leave_ of parse tree found.
//      {
//        DEBUG_COUT( "GetSubjectPersonIndex(): current grammar part type:" <<
//            mp_parsebyrise->GetGrammarPartName(
//            p_grammarpart->m_wGrammarPartID ) << " current size=" <<
//                m_stdvecNodesToProcess.size() << "\n" ) ;
//        if( TranslationRuleApplies( stdstrTranslation ) )
//        {
//          DEBUG_COUT( "translation rule applies\n" ) ;
//          stdstrWholeTransl += stdstrTranslation + " " ;
//        }
//        switch( p_grammarpart->m_wGrammarPartID )
//        {
//        case EnglishWord::noun:
//          DEBUG_COUT(  "translation:" <<
//            p_grammarpart->m_psetpvocabularyandtranslation->
//            m_arstrGermanWord[0] <<
//            "\n" ) ;
//        }
//        m_stdvec_wGrammarPartPath.pop_back() ;
//        m_stdvec_p_grammarpartPath.pop_back() ;
//
//        //std::vector<GrammarPart *>::iterator iter =
//        //Assign previously stored right child.
//        p_grammarpart =
//          m_stdvecNodesToProcess.
//          //"Returns a read/write reference to the data at the last
//          //*  element of the %vector."
//          back() ;
////          std::cout << "current syntax tree path:" << GetSyntaxTreePathAsName(
////            r_parsebyrise ) << "\n" ;
//
//        m_stdvec_wGrammarPartPath.push_back(
//            p_grammarpart->m_wGrammarPartID ) ;
//        m_stdvec_p_grammarpartPath.push_back( p_grammarpart) ;
//
//        //p_grammarpart = *iter ;
//        DEBUG_COUT( "Translate(): popping " <<
//            mp_parsebyrise->GetGrammarPartName(
//            p_grammarpart->m_wGrammarPartID ) << " current size=" <<
//                m_stdvecNodesToProcess.size() << "\n" ) ;
////          DrawGrammarPartNameAndPossiblyToken( *iter , wxpaintdc) ;
//        m_stdvecNodesToProcess.pop_back() ;
//        //std::cout.flush() ;
//        //usleep(100000) ;
//        -- m_wParseLevel ;
//      }
//    }
//    while( ! m_stdvecNodesToProcess.empty() ) ;
//  }
//  ParseTreeTraverser::SummarizePersonIndex spi(
  return 0 ;
}

void BottomUpParser::GetTokensAsSpaceSeparatedString(
  DWORD dwLeftmostIndex,
  DWORD dwRightmostIndex ,
  std::string & r_stdstr
  )
{
  bool bAtLeast1Token = false ;
  //PositionstdstringVector::
//  std::vector<Positionstdstring>::const_iterator iter ;
  std::vector<PositionString>::const_iterator iter ;
  iter = m_psv.begin() ;
  WORD wIndex = 0 ;
  while( iter != m_psv.end() && wIndex <= dwRightmostIndex )
  {
    if( wIndex >= dwLeftmostIndex )
    {
      if( bAtLeast1Token )
        r_stdstr += " " ;
      r_stdstr += iter->m_Str ;
      bAtLeast1Token = true ;
    }
    ++ wIndex ;
    ++ iter ;
  }
//  m_psv.at(dwLeftmostIndex) ;
  //return stdstr ;
}

//std::string ParseByRise::GetGrammarPartName(WORD wRuleID )
//{
//  std::string stdstrRuleName ;
//  std::map<WORD,std::string>::const_iterator iter =
//    m_stdmap_wRuleID2RuleName.find( wRuleID ) ;
//  if( iter != m_stdmap_wRuleID2RuleName.end() )
//    return iter->second ;
//  return std::string("") ;
//}

void BottomUpParser::InitGrammar()
{
//  m_stdmultimap_wGrammarPartID2wGrammarPartID.insert()
  InsertGrammarRule(
    EnglishWord::English_definite_article
    , EnglishWord::noun
    , "def_article_noun"
    ) ;

}

//Test if the grammar part is a word class.
//All grammar rules are based on word classes. Word classes are the leaves of
//the parse trees.
bool BottomUpParser::GrammarPartIDIsWordClass( PTN_IDtype parseTreeNodeID )
{
  bool bGrammarPartIDIsWordClass = false ;
//  switch( wGrammarPartID )
//  {
//  case EnglishWord::noun :
//  case EnglishWord::main_verb :
//  case EnglishWord::English_definite_article :
//  case EnglishWord::conjunction :
//    bGrammarPartIDIsWordClass = true ;
//    break ;
//  }
  if( parseTreeNodeID >= EnglishWord::noun && parseTreeNodeID <
      EnglishWord::beyond_last_entry )
    bGrammarPartIDIsWordClass = true ;
  return bGrammarPartIDIsWordClass ;
}

void BottomUpParser::InitGrammarRules()
{
  LOGN_DEBUG(/*"ParseByRise::InitGrammarRules()"*/ "begin")
  m_wNumberOfSuperordinateRules = EnglishWord::beyond_last_entry ;
  InsertFundamentalRuleIDs() ;
//  DEBUG_COUTN("ParseByRise::InitGrammarRules() "
  LOGN_DEBUG("end")
}

void BottomUpParser::InsertGrammarRulesFor3rdPersonSingular()
{
  InsertSuperClassGrammarRule(
    "article_singular"
    , "3rdPersSingSubjOrObjEnumEle"
    ) ;
  InsertSuperClassGrammarRule( "he", "3rdPersSingSubjOrObjEnumEle" ) ;
  InsertSuperClassGrammarRule( "she", "3rdPersSingSubjOrObjEnumEle" ) ;
  InsertSuperClassGrammarRule( "it", "3rdPersSingSubjOrObjEnumEle" ) ;

  InsertGrammarRule(
    "3rdPersSingSubjOrObjEnumEle"
    , EnglishWord::conjunction_and
    , "3rdPersSingSubjOrObjEnumEle_AND"
    ) ;
  InsertGrammarRule(
    "3rdPersSingSubjOrObjEnumEle_AND"
    , "3rdPersSingSubjOrObjEnumEle"
    , "3rdPersPlurSubjOrObjEnumEle"
    ) ;
  InsertGrammarRule(
    "3rdPersSingSubjOrObjEnumEle_AND"
    , "3rdPersPlurSubjOrObjEnumEle"
    , "3rdPersPlurSubjOrObjEnumEle"
    ) ;
}

void BottomUpParser::InsertFundamentalRuleIDs()
{
  LOGN_DEBUG(//"InsertFundamentalRuleIDs "
    "begin")
//  for( BYTE by = 0 ; by < NUMBER_OF_ENGLISH_WORD_CLASSES ; ++ by )
//  {
//
//  }
  //InsertGrammarRule( EnglishWord::noun, "noun" ) ;
  InsertGrammarPartID2NameMappingForWordClasses() ;

//  InsertArticleGrammarRules
//  InsertGrammarRulesFor3rdPersonSingular() ;

//  //TODO
//  WORD wIDdef_article_noun_conj =
//    InsertGrammarRule(
////    "def_article_noun",
//    "SubjOrObjEnumEle" ,
//    EnglishWord::conjunction,
//    //"def_article_noun_conj"
//    "SubjOrObjEnumEle_AND_conj"
//    ) ;
//  InsertGrammarRule(
//    EnglishWord::conjunction,
//    "def_article_noun",
//    "conj_def_article_noun"
//    ) ;
//  InsertGrammarRule( // e.g. "the car and " + "the cat and the dog"
//    "def_article_noun_conj",
//    "noun_constr,conj,noun_constr",
//    "noun_constr,conj,noun_constr"
//    ) ;
//  InsertGrammarRule(
//    //e.g.
//    // "the car             and  the car              and"
//    //    \   /              /    \   /               /
//    // subj_or_obj_enum_ele /   subj_or_obj_enum_ele /
//    //                \    /                    \   /
//    // subj_or_obj_enum_ele_AND_conj  subj_or_obj_enum_ele_AND_conj
//    //                           \     /
//    //                subj_or_obj_enum_ele_AND_conj
////    "def_article_noun_conj",
//    "SubjOrObjEnumEle_AND_conj" ,
////    "def_article_noun_conj",
//    "SubjOrObjEnumEle_AND_conj" ,
//    //"def_article_noun_conj"
//    wIDdef_article_noun_conj
//    ) ;
//  InsertGrammarRule(
//    EnglishWord::conjunction
//    , "subj_or_obj_ele"
//    , "conj_and_subj_or_obj_ele" //z.B. "and I", "or the car"
//    ) ;
//  InsertGrammarRule(
//    EnglishWord::comma
//    , "subj_or_obj_ele"
//    , "comma_and_subj_or_obj_ele" //z.B. "and I", "or the car"
//    ) ;
//  InsertGrammarRule(
//    "subj_or_obj_ele"
//    , "comma_and_subj_or_obj_ele"
//    //z.B. "you , I", "I , the car"
//    , "subj_or_obj_ele_comma_and_subj_or_obj_ele"
//    ) ;

  LOGN_DEBUG(//"InsertFundamentalRuleIDs "
    "end")
}

//return: new rule ID
PTN_IDtype BottomUpParser::InsertGrammarRule(
  const char * cp_chLeftGrammarRuleName
  , PTN_IDtype grammarRuleIDRight
  , //std::string
  const char * cp_chSuperordinateGrammarRuleName
  )
{
  PTNtypeName2PTN_IDtype::const_iterator iter =
    m_stdmap_RuleName2RuleID.find(
    cp_chLeftGrammarRuleName ) ;
  if( iter != m_stdmap_RuleName2RuleID.end() )
  {
    PTN_IDtype grammarRuleIDLeft = iter->second ;
    LOGN_DEBUG("inserting grammar rule \"" << cp_chSuperordinateGrammarRuleName
      << "\" " << "(ID=" << m_wNumberOfSuperordinateRules << ")"
      << " for " << cp_chLeftGrammarRuleName << grammarRuleIDLeft
      << ","
      << grammarRuleIDRight
      << "\n")
    InsertGrammarRule(
      grammarRuleIDLeft
      , grammarRuleIDRight
      , cp_chSuperordinateGrammarRuleName ) ;
  }
  return m_wNumberOfSuperordinateRules - 1 ;
}

void BottomUpParser::InsertGrammarRule(
  PTN_IDtype grammarRuleIDLeft ,
  const char * cp_chRightGrammarRuleName
  , //std::string
  const char * cp_chSuperordinateGrammarRuleName
  )
{
  PTNtypeName2PTN_IDtype::const_iterator iter =
    m_stdmap_RuleName2RuleID.find(
    cp_chRightGrammarRuleName ) ;
  if( iter != m_stdmap_RuleName2RuleID.end() )
  {
    PTN_IDtype wGrammarRuleIDRight = iter->second ;
    InsertGrammarRule(
      grammarRuleIDLeft
      , wGrammarRuleIDRight
      , cp_chSuperordinateGrammarRuleName ) ;
  }
}

enum BottomUpParser::InsertGrammarRuleReturnCodes BottomUpParser::
  InsertGrammarRule(
  const char * cp_chLeftGrammarRuleName
  , const char * cp_chRightGrammarRuleName
  , //std::string
  const char * cp_chSuperordinateGrammarRuleName
  )
{
  enum BottomUpParser::InsertGrammarRuleReturnCodes byReturnValue =
    AllGrammarPartsAreKnown;
  std::string unknownGrammarPartNames;
  PTNtypeName2PTN_IDtype::const_iterator c_iterLeft =
    m_stdmap_RuleName2RuleID.find(cp_chLeftGrammarRuleName);
  PTNtypeName2PTN_IDtype::const_iterator c_iterRight =
    m_stdmap_RuleName2RuleID.find(
        cp_chRightGrammarRuleName ) ;
  PTNtypeName2PTN_IDtype::const_iterator
    c_iter_std_map_std_str2wRuleName2RuleIDend =
    m_stdmap_RuleName2RuleID.end();
  if( c_iterLeft == //m_stdmap_RuleName2RuleID.end()
      c_iter_std_map_std_str2wRuleName2RuleIDend
    )
  {
//    m_p_userinterface->Message("Unknown grammar part ");
    byReturnValue = unknownLeftGrammarPart;
    unknownGrammarPartNames = "\"" + std::string(cp_chLeftGrammarRuleName) + "\"";
  }
  if( //c_iterLeft
    c_iterRight == //m_stdmap_RuleName2RuleID.end()
    c_iter_std_map_std_str2wRuleName2RuleIDend
    )
  {
    if( byReturnValue == unknownLeftGrammarPart )
    {
      byReturnValue = unknownLeftAndRightGrammarPart;
      unknownGrammarPartNames += ",\"" + std::string(cp_chRightGrammarRuleName) + "\"";
    }
    else
    {
      byReturnValue = unknownRightGrammarPart;
      unknownGrammarPartNames = "\"" + std::string(cp_chRightGrammarRuleName) + "\"";
    }
  }
  if( byReturnValue == AllGrammarPartsAreKnown )
  {
    PTN_IDtype grammarRuleIDLeft = c_iterLeft->second ;
    PTN_IDtype grammarRuleIDRight = c_iterRight->second ;
    LOGN_DEBUG("inserting rule \"" << cp_chSuperordinateGrammarRuleName
      << "\" " << "(ID=" << m_wNumberOfSuperordinateRules << ")"
      << " for " << cp_chLeftGrammarRuleName << grammarRuleIDLeft
      << ","
      << cp_chRightGrammarRuleName << grammarRuleIDRight
      << "\n")
    InsertGrammarRule(
      grammarRuleIDLeft
      , grammarRuleIDRight
      , cp_chSuperordinateGrammarRuleName ) ;
    LOGN_DEBUG("After inserting rule \"" << cp_chSuperordinateGrammarRuleName
      << "\" " )
  }
  else //if( byReturnValue != AllGrammarPartsAreKnown )
  {
    LOGN_ERROR("at least 1 child has an unknown grammar part names:" <<
      unknownGrammarPartNames);
    VTrans::UnknownGrammarPartNameException ugpne(unknownGrammarPartNames);
    throw ugpne;
  }
  return byReturnValue;
}

void BottomUpParser::InsertGrammarRule(
  const char * cp_chLeftGrammarRuleName
  , const char * cp_chRightGrammarRuleName
  , //std::string
  //EXISTING rule / grammar part ID
  PTN_IDtype superordinateGrammarRuleID
  )
{
  PTNtypeName2PTN_IDtype::const_iterator c_iterLeft =
    m_stdmap_RuleName2RuleID.find(
    cp_chLeftGrammarRuleName ) ;
  PTNtypeName2PTN_IDtype::const_iterator c_iterRight =
    m_stdmap_RuleName2RuleID.find(
    cp_chLeftGrammarRuleName ) ;
  if( c_iterLeft != m_stdmap_RuleName2RuleID.end() &&
      //c_iterLeft
      c_iterRight != m_stdmap_RuleName2RuleID.end()
      )
  {
    PTN_IDtype grammarRuleIDLeft = c_iterLeft->second ;
    PTN_IDtype grammarRuleIDRight = c_iterRight->second ;
//    InsertGrammarRule(
//      wGrammarRuleIDLeft
//      , wGrammarRuleIDRight
//      , cp_chSuperordinateGrammarRuleName ) ;

    //When parsing, compare the neighboured grammar parts' IDs for a shared
    //superordinate grammar part ID.
    //e.g.    def_article_noun (superordinate grammar part)
    //        /          \ (if "\"= last char:g++ warning:"multi-line comment")
    //definite article  noun
    m_stdmultimap_wGrammarPartID2SuperordinateID.insert(
      std::pair<PTN_IDtype,PTN_IDtype> (grammarRuleIDLeft,
      //ID for added rule.
      superordinateGrammarRuleID)
      ) ;

    //Used in InsertIfGrammarRuleAppliesTo(...) for getting all grammar rules
    // whose left child has the same grammarpart ID is the map's key value.
    m_stdmmap_wLeftChildGrammarPartID2SuperordinateGrammarRule.insert(
      std::pair<WORD,GrammarRule> (
        grammarRuleIDLeft,
        GrammarRule(grammarRuleIDRight,superordinateGrammarRuleID)
        )
      ) ;

    m_stdmultimap_wGrammarPartID2wGrammarPartID.insert(
      std::pair<PTN_IDtype,PTN_IDtype> (grammarRuleIDLeft, grammarRuleIDRight)
      ) ;

  //  m_stdmap_wRuleID2RuleName.insert( std::pair<WORD,std::string>
  //    ( m_wNumberOfSuperordinateRules, std::string( cp_ch) )
  //    ) ;
  //  m_stdmap_RuleName2RuleID.insert( std::pair<std::string,WORD>
  //    ( std::string( cp_ch) , m_wNumberOfSuperordinateRules )
  //    ) ;
//    InsertRule_ID2NameAndName2IDmapping( m_wNumberOfSuperordinateRules
//      , cp_chGrammarPartName ) ;
  }
}

//return: new rule ID
// purpose: if a superordinate grammar rule for many different grammar parts
//   exists:  e.g. superordinate grammar rule "subj_or_obj_ee" exists for
//   -definite_article_noun ("a man" from "a man saw me.")
//   -indefinite_article_noun ("a man" from "a man saw me.")
//   -gerund ("working" from "working and sleeping are great.")
//So (less) grammar rule can be defined: "subj_or_obj_ee" + "conj"
//  instead of multiple rules
//   -"definite_article_noun" + "conj"
//   -"indefinite_article_noun" + "conj"
//   -"gerund" + "conj"
//  or if every subclass of "subj_or_obj_ee"  can be part of a grammar rule:
//   e.g. "definite_article_noun" + conj + "gerund"
//    subclass^2 rules would be needed?:
//  -"definite_article_noun" + conj + "definite_article_noun"
//  -"definite_article_noun" + conj + "indefinite_article_noun"
//  -"definite_article_noun" + conj + "gerund"
//  -"indefinite_article_noun" + conj + "definite_article_noun"
//  -"indefinite_article_noun" + conj + "indefinite_article_noun"
//  -"indefinite_article_noun" + conj + "gerund"
//  -"gerund" + conj + "definite_article_noun"
//  -"gerund" + conj + "indefinite_article_noun"
//  -"gerund" + conj + "gerund"

///Insert a rule with only 1 child (i.e. no left and right children)
PTN_IDtype BottomUpParser::InsertSuperClassGrammarRule(
  PTN_IDtype subclassGrammarRuleID
  , //std::string
  const char * cp_chSuperclassGrammarRuleName
  )
{
  PTNtypeName2PTN_IDtype::const_iterator iter =
    m_stdmap_RuleName2RuleID.find(
      cp_chSuperclassGrammarRuleName ) ;
  if( //superclass rule (name) does not exist yet.
      iter == m_stdmap_RuleName2RuleID.end() )
  {
    //superordinate grammar part ID.
    //e.g.   subj_or_obj_ee
    //         |
    //   definite_article_noun (superordinate grammar part)
    //         /          \ (if "\"= last char:g++ warning:"multi-line comment")
    // definite article  noun
    m_stdmap_wGrammarPartID2SuperordinateID.insert(
      std::pair<PTN_IDtype,PTN_IDtype> (
        subclassGrammarRuleID,
        //ID for added rule.
        m_wNumberOfSuperordinateRules
        )
      ) ;
//    //Used in InsertIfGrammarRuleAppliesTo(...) for getting all grammar rules
//    // whose left child has the same grammarpart ID is the map's key value.
//    m_stdmmap_wLeftChildGrammarPartID2SuperordinateGrammarRule.insert(
//      std::pair<WORD,GrammarRule> (wGrammarRuleIDLeft,
//        GrammarRule(wGrammarRuleIDRight,m_wNumberOfSuperordinateRules)
//        )
//      ) ;
//
//    m_stdmultimap_wGrammarPartID2wGrammarPartID.insert(
//      std::pair<WORD,WORD> (wGrammarRuleIDLeft, wGrammarRuleIDRight)
//      ) ;

  //  m_stdmap_wRuleID2RuleName.insert( std::pair<WORD,std::string>
  //    ( m_wNumberOfSuperordinateRules, std::string( cp_ch) )
  //    ) ;
  //  m_stdmap_RuleName2RuleID.insert( std::pair<std::string,WORD>
  //    ( std::string( cp_ch) , m_wNumberOfSuperordinateRules )
  //    ) ;
    InsertRule_ID2NameAndName2IDmapping( m_wNumberOfSuperordinateRules
      , cp_chSuperclassGrammarRuleName ) ;
    ++ m_wNumberOfSuperordinateRules ;
    return m_wNumberOfSuperordinateRules - 1 ;
  }
  else
  {
    LOGN_WARNING("existing rule is being replaced")
    m_stdmap_wGrammarPartID2SuperordinateID.insert(
      std::pair<PTN_IDtype,PTN_IDtype> (
        subclassGrammarRuleID,
        //ID for added rule.
        iter->second
        )
      ) ;
    return iter->second ;
  }
}

//WORD
enum BottomUpParser::InsertGrammarRuleReturnCodes BottomUpParser::InsertSuperClassGrammarRule(
  const char * cp_chSubclassGrammarRuleName
  , //std::string
  const char * cp_chSuperclassGrammarRuleName
  )
{
  enum BottomUpParser::InsertGrammarRuleReturnCodes returnValue = AllGrammarPartsAreKnown;
  //This condition prevented inserting a super class rule for grammar part
  // whose name was already in the map.
  PTNtypeName2PTN_IDtype::const_iterator iter =
    m_stdmap_RuleName2RuleID.find(
    cp_chSubclassGrammarRuleName ) ;
  if( //If the name exists within the map
      iter != m_stdmap_RuleName2RuleID.end() )
  {
    PTN_IDtype subclassPTN_ID = iter->second ;
    LOGN("inserting rule \"" << cp_chSuperclassGrammarRuleName << "\" "
      << "(ID=" << m_wNumberOfSuperordinateRules << ")"
      << " for " << cp_chSubclassGrammarRuleName << subclassPTN_ID
//      << ","
//      << wGrammarRuleIDRight
      //<< "\n"
      )
//    InsertGrammarRule(
//      wGrammarRuleIDLeft
//      , 65535
//      , cp_chSuperordinateGrammarRuleName ) ;
    InsertSuperClassGrammarRule(
      subclassPTN_ID
      , //std::string
      cp_chSuperclassGrammarRuleName
      ) ;
  }
  else
  {
//    LOGN_ERROR("subclass rule not in map")
    returnValue = unknownLeftGrammarPart;
    LOGN_ERROR("left child \"" + std::string(cp_chSubclassGrammarRuleName) +
      "\" has an unknown grammar part name");
    VTrans::UnknownGrammarPartNameException ugpne(cp_chSubclassGrammarRuleName);
    throw ugpne;
  }
//  return m_wNumberOfSuperordinateRules - 1 ;
  return returnValue;
}

void BottomUpParser::InsertGrammarRule(
  //A grammar part with this ID must exist when calling this function.
  PTN_IDtype grammarRuleIDLeft
  //A grammar part with this ID must exist when calling this function.
  , PTN_IDtype grammarRuleIDRight
  , //std::string
  //Name of the new rule to insert that consists of left and right existing
  //rules.
  const char * cp_chSuperordinateGrammarRuleName
  )
{
  PTNtypeName2PTN_IDtype::const_iterator c_ruleName2ruleIDiter =
    m_stdmap_RuleName2RuleID.find(
      cp_chSuperordinateGrammarRuleName );
  ///If parse tree node ID for superordinate name was not found.
  if( c_ruleName2ruleIDiter == m_stdmap_RuleName2RuleID.end() )
  {
    //When parsing, compare the neighboured grammar parts' IDs for a shared
    //superordinate grammar part ID.
    //e.g.    def_article_noun (superordinate grammar part)
    //        /          \ (if "\"= last char:g++ warning:"multi-line comment")
    //definite article  noun
    m_stdmultimap_wGrammarPartID2SuperordinateID.insert(
      std::pair<PTN_IDtype,PTN_IDtype> (grammarRuleIDLeft,
      //ID for added rule.
      m_wNumberOfSuperordinateRules)
      ) ;
    //Used in InsertIfGrammarRuleAppliesTo(...) for getting all grammar rules
    // whose left child has the same grammarpart ID is the map's key value.
    m_stdmmap_wLeftChildGrammarPartID2SuperordinateGrammarRule.insert(
      std::pair<PTN_IDtype,GrammarRule> (
        grammarRuleIDLeft,
        GrammarRule(grammarRuleIDRight,m_wNumberOfSuperordinateRules)
        )
      ) ;
  //  m_stdmap_wRuleID2RuleName.insert( std::pair<WORD,std::string>
  //    ( m_wNumberOfSuperordinateRules, std::string( cp_ch) )
  //    ) ;
  //  m_stdmap_RuleName2RuleID.insert( std::pair<std::string,WORD>
  //    ( std::string( cp_ch) , m_wNumberOfSuperordinateRules )
  //    ) ;
    InsertRule_ID2NameAndName2IDmapping( m_wNumberOfSuperordinateRules
      , cp_chSuperordinateGrammarRuleName ) ;
    ++ m_wNumberOfSuperordinateRules ;
  }
  else
  {
    PTN_IDtype superordinatePTN_ID = c_ruleName2ruleIDiter->second;
#ifdef _DEBUG //because I can't show iter's value with Cygwin gdb Debugger
    //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//    #pragma GCC diagnostic ignored  "-Wunused"
    SUPPRESS_UNUSED_VARIABLE_WARNING(superordinatePTN_ID)
    //Useless instruction just for breakpoint possibility.
    int i ;
    SUPPRESS_UNUSED_VARIABLE_WARNING(i)
#endif
    //When parsing, compare the neighboured grammar parts' IDs for a shared
    //superordinate grammar part ID.
    //e.g.    def_article_noun (superordinate grammar part)
    //        /          \ (if "\"= last char:g++ warning:"multi-line comment")
    //definite article  noun
    m_stdmultimap_wGrammarPartID2SuperordinateID.insert(
      std::pair<PTN_IDtype,PTN_IDtype> (grammarRuleIDLeft,
      //ID for added rule.
      superordinatePTN_ID )
      ) ;
    //Used in InsertIfGrammarRuleAppliesTo(...) for getting all grammar rules
    // whose left child has the same grammarpart ID is the map's key value.
    m_stdmmap_wLeftChildGrammarPartID2SuperordinateGrammarRule.insert(
      std::pair<PTN_IDtype,GrammarRule> (
        grammarRuleIDLeft,
        GrammarRule(grammarRuleIDRight,superordinatePTN_ID)
        )
      ) ;
  }
  m_stdmultimap_wGrammarPartID2wGrammarPartID.insert(
    std::pair<PTN_IDtype,PTN_IDtype> (grammarRuleIDLeft, grammarRuleIDRight)
    ) ;
}

void BottomUpParser::InsertGrammarRule(PTN_IDtype wGrammarRuleID
  , const char * cp_ch )
{
  //GrammarRule()
//  m_stdmap_wRuleID2RuleName.insert( std::pair<WORD,std::string>
//    ( wGrammarRuleID, std::string( cp_ch) )
//    ) ;
  InsertRule_ID2NameAndName2IDmapping( wGrammarRuleID , cp_ch ) ;
}

void BottomUpParser::InsertRule_ID2NameAndName2IDmapping(
  PTN_IDtype parseTreeNodeID, const char * cp_ch )
{
  m_stdmap_wRuleID2RuleName.insert( std::pair<PTN_IDtype,std::string>
    ( parseTreeNodeID, std::string( cp_ch) )
    ) ;
  m_stdmap_RuleName2RuleID.insert( std::pair<std::string,PTN_IDtype>
    ( std::string( cp_ch) , parseTreeNodeID )
    ) ;
}

//void ParseByRise::InsertIntoPossibilityTree(
////  std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator & iter
////  , std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator &
////    iterRightOf
//  std::multimap<DWORD, GrammarPart>::iterator iterLeftGrammarPart
//  , std::multimap<DWORD, GrammarPart>::iterator iterRightGrammarPart
//  , WORD wGrammarPartIDOfRule
//  )
//{
//  //stdmapRightmostTokenIndex2GrammarRule.insert(iterRightOf-> ) ;
//  GrammarPart grammarpart(iterLeftGrammarPart->first, wGrammarPartIDOfRule ) ;
//  //For the translation: add children
//  grammarpart.SetLeftChild( iterLeftGrammarPart->second ) ;
//  grammarpart.SetRightChild( iterRightGrammarPart->second ) ;
////  m_stdmultimap_.insert( iterLeftGrammarPart->first , )
//  m_stdmultimap_wLeftmostIndex2grammarpart.insert(
//    iterLeftGrammarPart->first
//    , grammarpart ) ;
//  m_stdmultimap_wLeftmostIndex2grammarpart.insert(
//    iterRightGrammarPart->first
//    , grammarpart ) ;
//}

void BottomUpParser::Message(const std::wstring & cr_stdwstr )
{
  if( m_p_userinterface )
    m_p_userinterface->Message(cr_stdwstr) ;
}

void BottomUpParser::RemoveParseTree(GrammarPart * p_grammarpartRootNode)
{
  //m_stdset_grammarpartAllSuperordinate.erase( * p_grammarpartRootNode);
  m_allSuperordinateGrammarParts2pointerToThem.erase (* p_grammarpartRootNode);
//   m_stdmultimap_dwRightmostIndex2grammarpart.clear() ;
//   m_stdmultimap_dwLeftmostIndex2grammarpart.clear() ;

  DeleteFromOutMostTokenIndexContainer(
    p_grammarpartRootNode, //GrammarPart * p_grammarpartRootNode,
//    std::multimap<DWORD, GrammarPart *> & r_std_multimap_dw2p_grammarpart,
    m_stdmultimap_dwRightmostIndex2p_grammarpart,
    p_grammarpartRootNode->m_dwRightmostIndex //DWORD dwTokenIndex
    );
 DeleteFromOutMostTokenIndexContainer(
   p_grammarpartRootNode, //GrammarPart * p_grammarpartRootNode,
//    std::multimap<DWORD, GrammarPart *> & r_std_multimap_dw2p_grammarpart,
   m_stdmultimap_dwLeftmostIndex2p_grammarpart,
   p_grammarpartRootNode->m_dwLeftmostIndex //DWORD dwTokenIndex
   );

 //Do not delete because may be child of other GrammarPart
// //Was allocated on the heap
// delete p_grammarpartRootNode;
}

void BottomUpParser::RemoveSuperordinateRulesFromRootNodes()
{
  GrammarPart * p_grammarpartChild = NULL;
  GrammarPart * p_grammarpartCurrent = NULL;
  std::vector<GrammarPart *> std_vec_p_grammarpart;
  std::multimap<DWORD, GrammarPart *>::const_iterator c_iterParseTreeRoots =
      m_stdmultimap_dwLeftmostIndex2p_grammarpart.begin();
  while( c_iterParseTreeRoots !=
      m_stdmultimap_dwLeftmostIndex2p_grammarpart.end()
      )
  {
    p_grammarpartCurrent = c_iterParseTreeRoots->second;
    p_grammarpartChild = p_grammarpartCurrent->mp_grammarpartLeftChild;

    if( p_grammarpartChild )
    {
      std::map<PTN_IDtype,PTN_IDtype>::const_iterator c_iter_std_map_w2w =
        m_stdmap_wGrammarPartID2SuperordinateID.find( p_grammarpartChild->
                m_wGrammarPartID);
      if( //grammar part ID found in map
        c_iter_std_map_w2w != m_stdmap_wGrammarPartID2SuperordinateID.end()
        && c_iter_std_map_w2w->second == p_grammarpartCurrent->m_wGrammarPartID
        ) //-> current grammar part is superordinate grammar part
      {
        //Do not delete immediately: else the iterator gets invalid->SIGSEG.
        std_vec_p_grammarpart.push_back(p_grammarpartCurrent);
      }
//      while( IsSuperordinateGrammarRule(p_grammarpart->m_wGrammarPartID ) )
    //{
    //  delete p_grammarpart;
//      p_grammarpartCurrent = p_grammarpartChild;
    }
    //if( IsDuplicateOfAnotherParseTree[AtSameLeftmostIndex]( p_grammarpartnsert)
    ++ c_iterParseTreeRoots;
  } //"while" loop

  std::vector<GrammarPart *>::iterator iter = std_vec_p_grammarpart.begin();
  while( iter != std_vec_p_grammarpart.end() )
  {
    RemoveParseTree( //p_grammarpartCurrent
      * iter );
    ++ iter;
  }
}
}
