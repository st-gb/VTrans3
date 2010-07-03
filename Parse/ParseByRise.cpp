/* 
 * File:   ParseByRise.cpp
 * Author: Stefan
 * 
 * Created on 13. Februar 2010, 21:05
 */

#include <algorithm>
#include <set> //for std::set

//class VocabularyAndTranslation 
#include <VocabularyInMainMem/LetterTree/VocabularyAndTranslation.hpp>
#include "ParseByRise.hpp"
#include <VocabularyInMainMem/LetterTree/LetterNode.hpp>
#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>
#include <Token.h> //for PositionstdstringVector
#include <Xerces/ReadViaSAX2.hpp>
#include <Xerces/SAX2GrammarRuleHandler.hpp>
#include <supress_unused_variable.h>

extern LetterTree g_lettertree ;

ParseByRise::ParseByRise()
  :
   //m_wNumberOfSuperordinateRules( 0 )
  //,
  m_wParseLevel(0)
  , m_dwMapIndex(0)
{
  InitGrammarRules() ;
}

ParseByRise::ParseByRise(const ParseByRise& orig) {
}

ParseByRise::~ParseByRise()
{
  std::multimap<DWORD,GrammarPart *>::const_iterator c_iter =
      m_stdmultimap_dwLeftmostIndex2p_grammarpart.begin() ;
  for( ; c_iter != m_stdmultimap_dwLeftmostIndex2p_grammarpart.end() ;
    ++ c_iter )
  {

    DEBUG_COUTN("deleting grammar part " << GetGrammarPartName(
      c_iter->second->m_wGrammarPartID ) )
    //Release mem only for the GrammarPart for the leftmost index mmap and not twice:
    //leftmost index mmap has the address of the _same_ GrammarPart stored.
    //FIXME SIGSEV here
    delete c_iter->second ;
  }
}

void BuildTokenVector(
  const std::string & stdstrText
//  , PositionstdstringVector & psv
  , PositionStringVector & psv
  )
{
  bool bDoLoop = true ;
  const char * pch ;
  //stdstrText.find()
  pch = stdstrText.c_str() ;
  std::string stdstrToken ;
  WORD wEndOfTokenIndex = MAXWORD ;
  WORD wBeginOfTokenIndex = //MAXWORD ;
    0 ;
  WORD wCharIndex = 0 ;
//  //while not "\0"
//  while( *pch )
  do
  {
    switch( *pch )
    {
      case '\t':
      case ' ':
      case '\0':
        //If unset.
        if( wEndOfTokenIndex == MAXWORD )
        {
          wEndOfTokenIndex = wCharIndex ;
          stdstrToken = stdstrText.substr(
              wBeginOfTokenIndex,wEndOfTokenIndex-wBeginOfTokenIndex) ;
          psv.push_back(
//            Positionstdstring( stdstrToken ,
            PositionString( stdstrToken ,
              wBeginOfTokenIndex,wBeginOfTokenIndex)
            ) ;
          //std::cout << ""
        }
        if( *pch == '\0' )
           bDoLoop = false ;
        //Always setting the value is ~ as fast to first compare
        //( if( "wBeginOfTokenIndex == MAXWORD" ) )
        wBeginOfTokenIndex = MAXWORD ;
        break;
      default:
//        if( bBeginOfNewToken )
//          bBeginOfNewToken = false
//        else
//          bBeginOfNewToken = true ;
        //If unset.
        if( wBeginOfTokenIndex == MAXWORD )
          wBeginOfTokenIndex = wCharIndex ;
        //Always setting the value is ~ as fast to first compare
        //( if( "wEndOfTokenIndex == MAXWORD" ) )
        wEndOfTokenIndex = MAXWORD ;
    }
    //if( bBeginOfNewToken )
    ++ pch ;
    ++ wCharIndex ;
  }
  while( bDoLoop ) ;
}

void ParseByRise::ClearAllGrammarStuff()
{
  m_stdmap_wRuleID2RuleName.clear() ;
  m_stdmap_RuleName2RuleID.clear() ;
  m_stdmultimap_wGrammarPartID2SuperordinateID.clear() ;
  //Used in InsertIfGrammarRuleAppliesTo(...) for getting all grammar rules
  // whose left child has the same grammarpart ID is the map's key value.
  m_stdmmap_wLeftChildGrammarPartID2SuperordinateGrammarRule.clear() ;

  m_stdmultimap_wGrammarPartID2wGrammarPartID.clear() ;
//  m_stdmap_RuleName2RuleID.clear() ;
  m_stdmap_wGrammarPartID2SuperordinateID.clear() ;
}

//Clears (empties) the previously generated parse tree.
//This should be done for a next parse tree generation .
void ParseByRise::ClearParseTree()
 {
  m_stdset_grammarpartAllSuperordinate.clear() ;
  m_wBiggestNumberOfTokensForAppliedGrammarRule = 0 ;
//   m_stdmultimap_dwRightmostIndex2grammarpart.clear() ;
//   m_stdmultimap_dwLeftmostIndex2grammarpart.clear() ;
   m_stdmultimap_dwRightmostIndex2p_grammarpart.clear() ;
   m_stdmultimap_dwLeftmostIndex2p_grammarpart.clear() ;
   m_stdset_p_grammarpartAllSuperordinate.clear() ;
 }

//Creates the leafs of possible parse trees ausgehend from the source text.
// Leaves are word classes, e.g. nouns.
// If a grammar rule applies later then these leaves can make up a parse trees:
// the vacuum cleaner sucks.
// +-+ +------------+ +---+    <- leaves
//   \ /              /
// def_article_noun    /           <- tree struct: parse tree
//              \   /
//              clause       <- tree struct: parse tree
void ParseByRise::CreateInitialGrammarParts (std::string & stdstrText )
{
  //PositionstdstringVector psv ;
  m_wParseLevel = 1 ;
  m_psv.clear() ;
//  std::map<DWORD, GrammarPart> stdmapwLeftmostIndex2grammarpart ;
//  std::map<DWORD, GrammarPart> stdmapwRightmostIndex2grammarpart ;

  BuildTokenVector(stdstrText,m_psv) ;
#ifdef _DEBUG
  DWORD dwSize ;
#endif
//  PositionstdstringVector::const_iterator iter = m_psv.begin() ;
  PositionStringVector::const_iterator iter = m_psv.begin() ;
  #ifdef _DEBUG
  DEBUG_COUT( "tokens:\n" );
  #endif
  DWORD wTokenIndex = 0 ;
  //ParseByRise parsebyrise ;
  while( iter != m_psv.end() )
  {
    #ifdef _DEBUG
    std::cout << "\"" << (*iter).m_Str << "\"\n" ;
    #endif
    //std::set<VocabularyAndTranslation *> setpvocabularyandtranslation ;

//   if( g_lettertree.IsSingular(
//     //searchIncludingInflections(
//     psv
//     , wTokenIndex ,
//     setpvocabularyandtranslation
//     ) )
//   {
//     outputGermanTranslation(setpvocabularyandtranslation) ;
//   }
    //outputWordTypeAndGermanTranslation(psv,wTokenIndex) ;
    //parsebyrise.StoreWordTypeAndGermanTranslation(
    StoreWordTypeAndGermanTranslation(
      m_psv //PositionstdstringVector & psv
      , wTokenIndex //DWORD dwTokenIndex
    //  , std::map <WORD, std::set<VocabularyAndTranslation *> *> &
    //    stdmap_wIndex2p_set_p_vocabularyandtranslation
    //  , std::vector<GrammarPart> & stdvector_grammarpart ;

      //Maintaining 2 maps with both leftnost and rightmost indexes should be faster
      //when searching for neighboured grammar parts:
      //  0    1       2      3      <-index
      //"the vacuum cleaner sucks."
      // +----------------+
      //   e.g. current grammar part: rightmost index: 2: find neighbour with
      //     leftmost index "3".
//      , stdmapwLeftmostIndex2grammarpart
//      , stdmapwRightmostIndex2grammarpart
//      , stdmultimap_wLeftmostIndex2grammarpart
//      , stdmultimap_wRightmostIndex2grammarpart
      ) ;
#ifdef _DEBUG
    dwSize = //parsebyrise.
//      m_stdmultimap_dwRightmostIndex2grammarpart.size() ;
      m_stdmultimap_dwRightmostIndex2p_grammarpart.size() ;
    dwSize = //parsebyrise.
//      m_stdmultimap_dwLeftmostIndex2grammarpart.size() ;
      m_stdmultimap_dwLeftmostIndex2p_grammarpart.size() ;
#endif
    ++ iter ;
    ++ wTokenIndex ;
  }
}

//Return the grammar part where all of its (sub-)children cover the widest
//range of tokens. This is senseful because it is wanted to translate a
//the largest contigious block of tokens.
//E.g.: "the vacuum cleaner sucks." : get the largest covering tokens starting at
//   pos "0".
//  : "the" (pos 0-0) covers 1 token,
//  : "the vacuum cleaner" (pos 0-2) covers 3 tokens
//  : "the vacuum cleaner sucks." (pos 0-3) covers 4 tokens
GrammarPart * ParseByRise::GetGrammarPartCoveringMostTokens(
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
      SUPRESS_UNUSED_VARIABLE_WARNING(i)
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
    SUPRESS_UNUSED_VARIABLE_WARNING(i)
    }
#endif
  return p_gp ;
}

void ParseByRise::GetGrammarPartCoveringMostTokens(
  DWORD dwLeftmostTokenIndex ,
  std::vector<GrammarPart *> & r_stdvec_p_grammarpart
  )
{
  DWORD dwNumberOfTokensCoveredMax = 0 ;
  DWORD dwNumberOfTokensCoveredCurrent ;
  GrammarPart * p_grammarpart = NULL ;
  //May not be empty at the beginning.
  r_stdvec_p_grammarpart.clear() ;
  //std::multimap<DWORD, GrammarPart>::const_iterator iter ;

  //Get a list with all parse trees that have the leftmost token at index
  //"dwLeftmostTokenIndex".
//  std::pair<c_iter_mmap_dw2grammarpart,c_iter_mmap_dw2grammarpart>
//    stdpair_iter =
//    m_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
  std::pair<c_iter_mmap_dw2p_grammarpart,c_iter_mmap_dw2p_grammarpart>
    stdpair_iter =
    m_stdmultimap_dwLeftmostIndex2p_grammarpart.equal_range(
    dwLeftmostTokenIndex) ;
#ifdef _DEBUG
  WORD wNumberOfGrammapartsStartingAtPos = 0 ;
#endif
  for( //c_iter_mmap_dw2grammarpart
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
      //drop the added items with less # of tokens
      r_stdvec_p_grammarpart.clear() ;
//#ifdef _DEBUG
      p_grammarpart = //(GrammarPart * ) & iterCurrent->second ;
          iterCurrent->second ;
      r_stdvec_p_grammarpart.push_back( p_grammarpart ) ;
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
      SUPRESS_UNUSED_VARIABLE_WARNING(i)
#endif
      p_grammarpart = //(GrammarPart * ) & iterCurrent->second ;
          iterCurrent->second ;
      r_stdvec_p_grammarpart.push_back( p_grammarpart ) ;
    }
  }
#ifdef _DEBUG
  if( ! p_grammarpart )
    {
    //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//    #pragma GCC diagnostic ignored  "-Wunused"
    //Useless instruction just for breakpoint possibility.
    int i = 0 ;
    SUPRESS_UNUSED_VARIABLE_WARNING(i)
    }
#endif
//  return p_grammarpart ;
}

bool ParseByRise::GetGrammarPartID( const std::string & r_str , WORD & wID )
{
  bool bSuccess = false ;
  std::map<std::string,WORD>::const_iterator iter =
    m_stdmap_RuleName2RuleID.find(r_str) ;
  if( iter != m_stdmap_RuleName2RuleID.end() )
  {
    wID = iter->second ;
    bSuccess = true ;
  }
  return bSuccess ;
}

std::string ParseByRise::GetGrammarPartName(WORD wRuleID )
{
  std::string stdstrRuleName ;
  std::map<WORD,std::string>::const_iterator iter =
    m_stdmap_wRuleID2RuleName.find( wRuleID ) ;
  if( iter != m_stdmap_wRuleID2RuleName.end() )
    return iter->second ;
  return std::string("?gram pt") ;
}

std::string ParseByRise::GetPathAs_std_string(
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

std::string ParseByRise::GetPathAs_std_string(
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
//persion  plural)
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
BYTE ParseByRise::GetSubjectPersonIndex( GrammarPart * p_grammarpart)
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

void ParseByRise::GetTokensAsSpaceSeparatedString(
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

void ParseByRise::InitGrammar()
{
//  m_stdmultimap_wGrammarPartID2wGrammarPartID.insert()
  InsertGrammarRule(
    EnglishWord::English_definite_article
    , EnglishWord::noun
    , "def_article_noun"
    ) ;

}

// \brief Insert a grammar part <-> grammar part name mapping for word classes.
void ParseByRise::InsertGrammarPartID2NameMappingForWordClasses()
{
  InsertGrammarRule( EnglishWord::noun, "noun" ) ;
  InsertGrammarRule( EnglishWord::singular, "singular_noun" ) ;
  InsertGrammarRule( EnglishWord::plural_noun, "plural_noun" ) ;
//  InsertGrammarRule( EnglishWord::main_verb, "mainVerbInfinitive" ) ;
  InsertGrammarRule( EnglishWord::main_verb_allows_0object_infinitive ,
    "mainVerbInf0Obj" ) ;
  InsertGrammarRule( EnglishWord::main_verb_allows_1object_infinitive ,
    "mainVerbInf1Obj" ) ;
  InsertGrammarRule( EnglishWord::main_verb_allows_2objects_infinitive ,
    "mainVerbInf2Obj" ) ;

  InsertGrammarRule( EnglishWord::mainVerbAllows0object3rdPersonSingularPresent,
    "MainVerb3rdPersSingPres0Obj" ) ;
  InsertGrammarRule( EnglishWord::mainVerbAllows1object3rdPersonSingularPresent,
    "MainVerb3rdPersSingPres1Obj" ) ;
  InsertGrammarRule( EnglishWord::mainVerbAllows2objects3rdPersonSingularPresent,
    "MainVerb3rdPersSingPres2Obj" ) ;
  InsertGrammarRule( EnglishWord::main_verb_allows_0object_progressive_form,
    "MainVerbProgressive0Obj" ) ;
  InsertGrammarRule( EnglishWord::main_verb_allows_1object_progressive_form,
    "MainVerbProgressive1Obj" ) ;
  InsertGrammarRule( EnglishWord::main_verb_allows_2objects_progressive_form,
    "MainVerbProgressive2Obj" ) ;

  InsertGrammarRule( EnglishWord::English_definite_article,
    "definite_article" ) ;
  InsertGrammarRule( EnglishWord::English_indefinite_article,
    "indefinite_article" ) ;

  InsertGrammarRule( EnglishWord::personal_pronoun_I , "I" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_you_sing ,"you_sing" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_he , "he") ;
  InsertGrammarRule(EnglishWord::personal_pronoun_she , "she") ;
  InsertGrammarRule(EnglishWord::personal_pronoun_it, "it" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_we, "we" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_you_plur, "you_plur" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_they, "they" ) ;

  InsertGrammarRule( EnglishWord::reflexive_pronoun_myself, "myself" ) ;
  InsertGrammarRule( EnglishWord::reflexive_pronoun_yourself, "yourself" ) ;
  InsertGrammarRule( EnglishWord::reflexive_pronoun_himself, "himself" ) ;
  InsertGrammarRule( EnglishWord::reflexive_pronoun_herself, "herself" ) ;
  InsertGrammarRule( EnglishWord::reflexive_pronoun_itself, "itself" ) ;
  InsertGrammarRule( EnglishWord::reflexive_pronoun_ourselves, "ourselves" ) ;
  InsertGrammarRule( EnglishWord::reflexive_pronoun_yourselves, "yourselves" );
  InsertGrammarRule( EnglishWord::reflexive_pronoun_themselves, "themselves") ;

  InsertGrammarRule( EnglishWord::conjunction_and ,"and" ) ;
  InsertGrammarRule( EnglishWord::conjunction, "conjunction" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun, "pers_pronoun" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_objective_form ,
    "pers_pron_obj" ) ;
}

//Test if the grammar part is a word class.
//All grammar rules are based on word classes. Word classes are the leaves of
//the parse trees.
bool ParseByRise::GrammarPartIDIsWordClass( WORD wGrammarPartID )
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
  if( wGrammarPartID >= EnglishWord::noun && wGrammarPartID <
      EnglishWord::beyond_last_entry )
    bGrammarPartIDIsWordClass = true ;
  return bGrammarPartIDIsWordClass ;
}

void ParseByRise::InitGrammarRules()
{
  m_wNumberOfSuperordinateRules = EnglishWord::beyond_last_entry ;
  InsertFundamentalRuleIDs() ;
}

void ParseByRise::InsertGrammarRulesFor3rdPersonSingular()
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

void ParseByRise::InsertFundamentalRuleIDs()
{
//  for( BYTE by = 0 ; by < NUMBER_OF_ENGLISH_WORD_CLASSES ; ++ by )
//  {
//
//  }
  //InsertGrammarRule( EnglishWord::noun, "noun" ) ;
  InsertGrammarPartID2NameMappingForWordClasses() ;
//  InsertGrammarRule(
//    EnglishWord::English_definite_article,
//    EnglishWord::noun,
//    "def_article_noun") ;
//  InsertGrammarRule(
//    "def_article_noun"
//    , EnglishWord::main_verb
//    , "clause"
//    ) ;

//  InsertGrammarRule(
//    EnglishWord::English_definite_article,
//    EnglishWord::singular ,
//    "definite_article_singular"
//    ) ;
//  InsertGrammarRule(
//    EnglishWord::English_definite_article,
//    EnglishWord::plural_noun ,
//    "definite_article_plural"
//    ) ;
//  InsertGrammarRule(
//    EnglishWord::English_indefinite_article,
//    EnglishWord::singular ,
//    "indefinite_article_singular"
//    ) ;
//  InsertSuperClassGrammarRule(
//    "indefinite_article_singular"
//    , "article_singular"
//    ) ;
//  InsertSuperClassGrammarRule(
//    "definite_article_singular"
//    , "article_singular"
//    ) ;

//  InsertGrammarRulesFor3rdPersonSingular() ;

//  InsertGrammarRule(
//    "3rdPersPlurSubjOrObjEnumEle"
//    , EnglishWord::conjunction_and
//    , "3rdPersPlurSubjOrObjEnumEle_AND"
//    ) ;
//  InsertGrammarRule(
//    "3rdPersPlurSubjOrObjEnumEle_AND"
//    , "3rdPersPlurSubjOrObjEnumEle"
//    , "3rdPersPlurSubjOrObjEnumEle"
//    ) ;
//  InsertGrammarRule(
//    "3rdPersPlurSubjOrObjEnumEle_AND"
//    , "3rdPersSingSubjOrObjEnumEle"
//    , "3rdPersPlurSubjOrObjEnumEle"
//    ) ;
//
//  InsertGrammarRule(
//    "3rdPersPlurSubjOrObjEnumEle_AND" //"the cars and"
//    , "I"  // "I"
//    , "1stPersPlurSubjOrObjEnumEle" // = 1st person plural index / "we"
//    ) ;
//  InsertGrammarRule(
//    "I" //"I"
//    , EnglishWord::conjunction_and  // "and"
//    , "I_and" // = 1st person plural index / "we"
//    ) ;
//  InsertGrammarRule(
//    "I" //"I"
////    , "mainVerbInfinitive"  // "and"
//    , "mainVerbInf0Obj"
//    , "1stPersSingSimplePresentClause" // = 1st person plural index / "we"
//    ) ;
//  InsertGrammarRule(
//    "I" //"I"
//    , "mainVerbInf1Obj"
//    , "1stPersSingSimplePresentSP1Obj" // = 1st person plural index / "we"
//    ) ;
  InsertSuperClassGrammarRule(
    EnglishWord::reflexive_pronoun_myself ,
    "myself_object"
    ) ;
  InsertGrammarRule(
    "1stPersSingSimplePresentSP1Obj"
    , "myself_object"
    , "1stPersSingSimplePresent1ObjClause"
    ) ;
  InsertGrammarRule(
    "you_sing" //"I"
    , "mainVerbInf0Obj"
    , "2ndPersSingSimplePresentClause" // = 1st person plural index / "we"
    ) ;
  InsertGrammarRule(
    "you_sing" //"I"
    , "mainVerbInf0Obj"
    , "2ndPersPlurSimplePresentClause" // = 1st person plural index / "we"
    ) ;

//  //This rule exists especially for cases where the singular and plural are the
//  //same for a noun (e.g. "1 sheep"; "2 sheep").
//  //So if only 1 enum ele as singular noun in the subject and the finite verb of the predicate
//  //matches the 3rd person singular we do not need the summarize function that
//  // examines all subject enum eles.
//  InsertGrammarRule(
//    //"article_singular"
//    "3rdPersSingSubjOrObjEnumEle"
//    , EnglishWord::mainVerbAllows0object3rdPersonSingularPresent
//    , //"singular_clause"
//      "3rdPersSingClause"
//    ) ;
//  InsertGrammarRule(
//    "3rdPersSingSubjOrObjEnumEle"
//    , EnglishWord::mainVerbAllows1object3rdPersonSingularPresent
//    , "3rdPersSingClause1Obj"
//    ) ;
//  //Insert superclass in order to treat all plural nouns possibilities
//  //(article or not, ...) the same way.
//  InsertSuperClassGrammarRule(
//    "plural_noun" ,
//    "plural_noun_superclass"
//    ) ;
//  InsertSuperClassGrammarRule(
//    "definite_article_plural" ,
//    "plural_noun_superclass"
//    ) ;
//  InsertSuperClassGrammarRule(
//    "plural_noun_superclass" ,
//    "3rdPersPlurSubjOrObjEnumEle"
//    ) ;
//
//  //For an object:
//  InsertSuperClassGrammarRule(
//    "3rdPersPlurSubjOrObjEnumEle" ,
//    "SubjOrObjEnumEle" ) ;
//  InsertSuperClassGrammarRule(
//    "3rdPersSingSubjOrObjEnumEle" ,
//    "SubjOrObjEnumEle" ) ;

//  //This rule exists especially for cases where the singular and plural are the
//  //same for a noun (e.g. "1 sheep"; "2 sheep").
//  //So if only 1 enum ele in the subject and the finite verb of the predicate
//  //matches the 3rd person plural we do not need the summarize function that
//  // examines all subject enum eles.
//  InsertGrammarRule(
////    "plural_noun_superclass"
//    "3rdPersPlurSubjOrObjEnumEle"
//    //means "infinitive"
////    , EnglishWord::main_verb
////    , "mainVerbInfinitive"
//    , "mainVerbInf0Obj"
//    , "3rdPersPluralClause"
//    ) ;
//  InsertGrammarRule(
//    "3rdPersPlurSubjOrObjEnumEle"
//    , "mainVerbInf1Obj"
//    , "3rdPersPluralClause1Obj"
//    ) ;
//  InsertSuperClassGrammarRule(
//    "plural_noun_superclass"
//    , "SubjOrObjEnumEle"
//    ) ;
//  InsertSuperClassGrammarRule(
//    "SubjOrObjEnumEle"
//    , "ObjEnumEle"
//    ) ;
//  InsertSuperClassGrammarRule(
//    "pers_pron_obj"
//    , "ObjEnumEle"
//    ) ;
//  InsertGrammarRule(
//    "ObjEnumEle"
//    , EnglishWord::conjunction_and
//    , "ObjEnumEle_and"
//    ) ;
//  InsertGrammarRule(
//    "ObjEnumEle_and"
//    , "ObjEnumEle_and"
//    , "ObjEnumEle_and"
//    ) ;
//  InsertGrammarRule(
//    "ObjEnumEle_and"
//    , "ObjEnumEle"
//    , "ObjEnumEle_and_ObjEnumEle"
//    ) ;
//  InsertSuperClassGrammarRule(
//    "ObjEnumEle_and_ObjEnumEle"
//    , "obj"
//    ) ;
//  InsertSuperClassGrammarRule(
//    "ObjEnumEle"
//    , "obj"
//    ) ;
  InsertGrammarRule(
    "3rdPersPluralClause1Obj"
    , "obj"
    , "3rdPersPluralClauseWith1Obj"
    ) ;
  InsertGrammarRule(
    "3rdPersPluralClause1Obj"
    , "3rdPersPluralReflexivePronoun"
    , "3rdPersPluralClauseWithReflexPronoun"
    ) ;
//  InsertSuperClassGrammarRule(
//    "def_article_noun"
//    , "SubjOrObjEnumEle"
//    ) ;
  InsertSuperClassGrammarRule(
    "pers_pronoun"
    , "subj_enum_ele"
    ) ;
  InsertGrammarRule(
    "subj_enum_ele"
    , EnglishWord::conjunction_and
    , "subj_enum_ele_and"
    ) ;
  //If 1 enum ele can only belong to a subject and the other sum ele can be part
  // of EITHER a subject or an object then the combination can be only a part
  //of a subject.
  InsertGrammarRule( //e.g. "I and the car"
    "subj_enum_ele_and" // "I and"
    , "SubjOrObjEnumEle" //"the car"
    , "subj_enum_ele" //then this can only be a subject (and not an object)
    ) ;
  InsertGrammarRule(
    EnglishWord::conjunction_and
    , "subj_enum_ele"
    , "and_subj_enum_ele"
    ) ;
  //If 1 enum ele can only belong to a subject and the other sum ele can be part
  // of EITHER a subject or an object then the combination can be only a part
  //of a subject.
  InsertGrammarRule( //e.g. "the car and I"
    "SubjOrObjEnumEle" //"the car"
    , "and subj_enum_ele" // "and I "
    , "subj_enum_ele" //then this can only be a subject (and not an object)
    ) ;
  InsertSuperClassGrammarRule(
    "subj_enum_ele"
    , "subject"
    ) ;

  InsertSuperClassGrammarRule(
    "def_article_noun"
    , "SubjOrObjEnumEle"
    ) ;
  InsertSuperClassGrammarRule(
    "SubjOrObjEnumEle"
    , "subject"
    ) ;
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
  InsertGrammarRule(
    //"def_article_noun_conj",
    "SubjOrObjEnumEle_AND_conj" ,
//    "def_article_noun",
    "SubjOrObjEnumEle" ,
    //"noun_constr,conj,noun_constr"
    "SubjOrObjEnumEle,conj,SubjOrObjEnumEle"
    ) ;
  InsertSuperClassGrammarRule(
    "SubjOrObjEnumEle,conj,SubjOrObjEnumEle"
    , "subject"
    ) ;
  InsertGrammarRule(
    //"subj_or_obj_ee"
    "subject"
//    , EnglishWord::main_verb
//    , "mainVerbInfinitive"
    , "mainVerbInf0Obj"
    , "clause"
    ) ;
  InsertGrammarRule(
    "clause"
    , "obj"
    , "clause_with_obj"
    ) ;
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

//  SAX2GrammarRuleHandler sax2grammarrulehandler(*this) ;
//  ReadViaSAX2("grammar_rules.xml", & sax2grammarrulehandler ) ;
  //Must create on heap, else the callback functions like "startElement" aren't
  //called?!
  SAX2GrammarRuleHandler * p_sax2grammarrulehandler = new
      SAX2GrammarRuleHandler(*this) ;
  ReadViaSAX2AndDeleteContentHandler("grammar_rules.xml",
    p_sax2grammarrulehandler ) ;
}

//return: new rule ID
WORD ParseByRise::InsertGrammarRule(
  const char * cp_chLeftGrammarRuleName
  , WORD wGrammarRuleIDRight
  , //std::string
  const char * cp_chSuperordinateGrammarRuleName
  )
{
  std::map<std::string,WORD>::const_iterator iter =
    m_stdmap_RuleName2RuleID.find(
    cp_chLeftGrammarRuleName ) ;
  if( iter != m_stdmap_RuleName2RuleID.end() )
  {
    WORD wGrammarRuleIDLeft = iter->second ;
    DEBUG_COUT("inserting grammar rule \"" << cp_chSuperordinateGrammarRuleName << "\" "
      << "(ID=" << m_wNumberOfSuperordinateRules << ")"
      << " for " << cp_chLeftGrammarRuleName << wGrammarRuleIDLeft
      << ","
      << wGrammarRuleIDRight
      << "\n")
    InsertGrammarRule(
      wGrammarRuleIDLeft
      , wGrammarRuleIDRight
      , cp_chSuperordinateGrammarRuleName ) ;
  }
  return m_wNumberOfSuperordinateRules - 1 ;
}

void ParseByRise::InsertGrammarRule(
  WORD wGrammarRuleIDLeft ,
  const char * cp_chRightGrammarRuleName
  , //std::string
  const char * cp_chSuperordinateGrammarRuleName
  )
{
  std::map<std::string,WORD>::const_iterator iter =
    m_stdmap_RuleName2RuleID.find(
    cp_chRightGrammarRuleName ) ;
  if( iter != m_stdmap_RuleName2RuleID.end() )
  {
    WORD wGrammarRuleIDRight = iter->second ;
    InsertGrammarRule(
      wGrammarRuleIDLeft
      , wGrammarRuleIDRight
      , cp_chSuperordinateGrammarRuleName ) ;
  }
}

void ParseByRise::InsertGrammarRule(
  const char * cp_chLeftGrammarRuleName
  , const char * cp_chRightGrammarRuleName
  , //std::string
  const char * cp_chSuperordinateGrammarRuleName
  )
{
  std::map<std::string,WORD>::const_iterator c_iterLeft =
    m_stdmap_RuleName2RuleID.find(
    cp_chLeftGrammarRuleName ) ;
  std::map<std::string,WORD>::const_iterator c_iterRight =
    m_stdmap_RuleName2RuleID.find(
        cp_chRightGrammarRuleName ) ;
  if( c_iterLeft != m_stdmap_RuleName2RuleID.end() &&
      //c_iterLeft
      c_iterRight != m_stdmap_RuleName2RuleID.end()
      )
  {
    WORD wGrammarRuleIDLeft = c_iterLeft->second ;
    WORD wGrammarRuleIDRight = c_iterRight->second ;
    DEBUG_COUT("inserting rule \"" << cp_chSuperordinateGrammarRuleName << "\" "
      << "(ID=" << m_wNumberOfSuperordinateRules << ")"
      << " for " << cp_chLeftGrammarRuleName << wGrammarRuleIDLeft
      << ","
      << cp_chRightGrammarRuleName << wGrammarRuleIDRight
      << "\n")
    InsertGrammarRule(
      wGrammarRuleIDLeft
      , wGrammarRuleIDRight
      , cp_chSuperordinateGrammarRuleName ) ;
  }
}

void ParseByRise::InsertGrammarRule(
  const char * cp_chLeftGrammarRuleName
  , const char * cp_chRightGrammarRuleName
  , //std::string
  //EXISTING rule / grammar part ID
  WORD wSuperordinateGrammarRuleID
  )
{
  std::map<std::string,WORD>::const_iterator c_iterLeft =
    m_stdmap_RuleName2RuleID.find(
    cp_chLeftGrammarRuleName ) ;
  std::map<std::string,WORD>::const_iterator c_iterRight =
    m_stdmap_RuleName2RuleID.find(
    cp_chLeftGrammarRuleName ) ;
  if( c_iterLeft != m_stdmap_RuleName2RuleID.end() &&
      //c_iterLeft
      c_iterRight != m_stdmap_RuleName2RuleID.end()
      )
  {
    WORD wGrammarRuleIDLeft = c_iterLeft->second ;
    WORD wGrammarRuleIDRight = c_iterRight->second ;
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
      std::pair<WORD,WORD> (wGrammarRuleIDLeft,
      //ID for added rule.
      wSuperordinateGrammarRuleID)
      ) ;

    //Used in InsertIfGrammarRuleAppliesTo(...) for getting all grammar rules
    // whose left child has the same grammarpart ID is the map's key value.
    m_stdmmap_wLeftChildGrammarPartID2SuperordinateGrammarRule.insert(
      std::pair<WORD,GrammarRule> (
        wGrammarRuleIDLeft,
        GrammarRule(wGrammarRuleIDRight,wSuperordinateGrammarRuleID)
        )
      ) ;

    m_stdmultimap_wGrammarPartID2wGrammarPartID.insert(
      std::pair<WORD,WORD> (wGrammarRuleIDLeft, wGrammarRuleIDRight)
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

WORD ParseByRise::InsertSuperClassGrammarRule(
  WORD wSubclassGrammarRuleID
  , //std::string
  const char * cp_chSuperclassGrammarRuleName
  )
{
  std::map<std::string,WORD>::const_iterator iter =
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
      std::pair<WORD,WORD> (
        wSubclassGrammarRuleID,
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
    DEBUG_COUTN("warning: existing rule is being replaced")
    m_stdmap_wGrammarPartID2SuperordinateID.insert(
      std::pair<WORD,WORD> (
        wSubclassGrammarRuleID,
        //ID for added rule.
        iter->second
        )
      ) ;
    return iter->second ;
  }
}

WORD ParseByRise::InsertSuperClassGrammarRule(
  const char * cp_chSubclassGrammarRuleName
  , //std::string
  const char * cp_chSuperclassGrammarRuleName
  )
{
  //This condition prevented inserting a super class rule for grammar part
  // whose name was already in the map.
  std::map<std::string,WORD>::const_iterator iter =
    m_stdmap_RuleName2RuleID.find(
    cp_chSubclassGrammarRuleName ) ;
  if( //If the name exists within the map
      iter != m_stdmap_RuleName2RuleID.end() )
  {
    WORD wSubclassGrammarRuleID = iter->second ;
    DEBUG_COUT("inserting rule \"" << cp_chSuperclassGrammarRuleName << "\" "
      << "(ID=" << m_wNumberOfSuperordinateRules << ")"
      << " for " << cp_chSubclassGrammarRuleName << wSubclassGrammarRuleID
//      << ","
//      << wGrammarRuleIDRight
      << "\n")
//    InsertGrammarRule(
//      wGrammarRuleIDLeft
//      , 65535
//      , cp_chSuperordinateGrammarRuleName ) ;
    InsertSuperClassGrammarRule(
      wSubclassGrammarRuleID
      , //std::string
      cp_chSuperclassGrammarRuleName
      ) ;
  }
  else
  {
    DEBUG_COUTN("InsertSuperClassGrammarRule Rule already in map")
  }
  return m_wNumberOfSuperordinateRules - 1 ;
}

void ParseByRise::InsertGrammarRule(
  //A grammar part with this ID must exist when calling this function.
  WORD wGrammarRuleIDLeft
  //A grammar part with this ID must exist when calling this function.
  , WORD wGrammarRuleIDRight
  , //std::string
  //Name of the new rule to insert that consists of left and right existing
  //rules.
  const char * cp_chSuperordinateGrammarRuleName
  )
{
  std::map<std::string,WORD>::const_iterator c_iter =
    m_stdmap_RuleName2RuleID.find(
      cp_chSuperordinateGrammarRuleName ) ;
  if( c_iter == m_stdmap_RuleName2RuleID.end() )
  {
    //When parsing, compare the neighboured grammar parts' IDs for a shared
    //superordinate grammar part ID.
    //e.g.    def_article_noun (superordinate grammar part)
    //        /          \ (if "\"= last char:g++ warning:"multi-line comment")
    //definite article  noun
    m_stdmultimap_wGrammarPartID2SuperordinateID.insert(
      std::pair<WORD,WORD> (wGrammarRuleIDLeft,
      //ID for added rule.
      m_wNumberOfSuperordinateRules)
      ) ;
    //Used in InsertIfGrammarRuleAppliesTo(...) for getting all grammar rules
    // whose left child has the same grammarpart ID is the map's key value.
    m_stdmmap_wLeftChildGrammarPartID2SuperordinateGrammarRule.insert(
      std::pair<WORD,GrammarRule> (
        wGrammarRuleIDLeft,
        GrammarRule(wGrammarRuleIDRight,m_wNumberOfSuperordinateRules)
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
#ifdef _DEBUG //because I can't show iter's value with Cygwin gdb Debugger
    //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//    #pragma GCC diagnostic ignored  "-Wunused"
    WORD wSuperordinateGrammarPartID = c_iter->second ;
    SUPRESS_UNUSED_VARIABLE_WARNING(wSuperordinateGrammarPartID)
    //Useless instruction just for breakpoint possibility.
    int i ;
    SUPRESS_UNUSED_VARIABLE_WARNING(i)
#endif
    //When parsing, compare the neighboured grammar parts' IDs for a shared
    //superordinate grammar part ID.
    //e.g.    def_article_noun (superordinate grammar part)
    //        /          \ (if "\"= last char:g++ warning:"multi-line comment")
    //definite article  noun
    m_stdmultimap_wGrammarPartID2SuperordinateID.insert(
      std::pair<WORD,WORD> (wGrammarRuleIDLeft,
      //ID for added rule.
      c_iter->second )
      ) ;
    //Used in InsertIfGrammarRuleAppliesTo(...) for getting all grammar rules
    // whose left child has the same grammarpart ID is the map's key value.
    m_stdmmap_wLeftChildGrammarPartID2SuperordinateGrammarRule.insert(
      std::pair<WORD,GrammarRule> (
        wGrammarRuleIDLeft,
        GrammarRule(wGrammarRuleIDRight,c_iter->second)
        )
      ) ;
  }
  m_stdmultimap_wGrammarPartID2wGrammarPartID.insert(
    std::pair<WORD,WORD> (wGrammarRuleIDLeft, wGrammarRuleIDRight)
    ) ;
}

void ParseByRise::InsertGrammarRule(WORD wGrammarRuleID
  , const char * cp_ch )
{
  //GrammarRule()
//  m_stdmap_wRuleID2RuleName.insert( std::pair<WORD,std::string>
//    ( wGrammarRuleID, std::string( cp_ch) )
//    ) ;
  InsertRule_ID2NameAndName2IDmapping( wGrammarRuleID , cp_ch ) ;
}

void ParseByRise::InsertRule_ID2NameAndName2IDmapping( WORD wGrammarRuleID
    , const char * cp_ch )
{
  m_stdmap_wRuleID2RuleName.insert( std::pair<WORD,std::string>
    ( wGrammarRuleID, std::string( cp_ch) )
    ) ;
  m_stdmap_RuleName2RuleID.insert( std::pair<std::string,WORD>
    ( std::string( cp_ch) , wGrammarRuleID )
    ) ;
}

bool ParseByRise:://GrammarRuleAppliesTo(
  InsertIfGrammarRuleAppliesTo(
  //Maintaining 2 maps with both leftmost and rightmost indexes should be faster
  //when searching for neighboured grammar parts:
  //  0    1       2      3      <-index
  //"the vacuum cleaner sucks."
  // +----------------+
  //   e.g. current grammar part: rightmost index: 2: find neighbour with
  //     leftmost index "3".
//  std::multimap<DWORD, GrammarPart>::iterator
//    iter_mm_rightmostidx2grammarptLeftGrammarPart
//  , std::multimap<DWORD, GrammarPart>::iterator
//    iter_wLeftmostIndex2grammarpartRightGrammarPart
  std::multimap<DWORD, GrammarPart *>::iterator
    iter_mm_rightmostidx2p_grammarptLeftGrammarPart
  , std::multimap<DWORD, GrammarPart *>::iterator
    iter_wLeftmostIndex2p_grammarpartRightGrammarPart
  )
{
  bool bRuleApplied = false ;
  DWORD dwLeftMostTokenIndexOfRule ;
  DWORD dwRightMostTokenIndexOfRule ;
#ifdef _DEBUG
  std::map<WORD, std::string>::const_iterator ci_stdmap_wRuleID2stdstrRuleNameSuperordinate ;
  std::map<WORD, std::string>::const_iterator ci_stdmap_wLeftGrammarPartID2stdstrRuleName ;
  std::map<WORD, std::string>::const_iterator ci_stdmap_wRightGrammarPartID2stdstrRuleName ;
#endif
  WORD wLeftGrammarPartIDForRule =
//    iter_mm_rightmostidx2grammarptLeftGrammarPart->second.m_wGrammarPartID ;
    iter_mm_rightmostidx2p_grammarptLeftGrammarPart->second->m_wGrammarPartID ;
  WORD wRightGrammarPartIDForRule =
//    iter_wLeftmostIndex2grammarpartRightGrammarPart->second.m_wGrammarPartID ;
    iter_wLeftmostIndex2p_grammarpartRightGrammarPart->second->m_wGrammarPartID ;
  //for( GrammarRule )
//  //Using a 2-dimensional array if a fast way. dimension 1 should equal
//  //dimension 2. But on the other hand adding rules afterwards is not easy
//  //if the array size was exactly as the number of rules before.
//  WORD wGrammarRule = m_ararwGrammarRuleID[ wLeftGrammarPartIDForRule ]
//    [ wLeftGrammarPartIDForRule ] ;
//    //If a rule exists.
//  if( wGrammarRule != 0 )

  //typedef std::multimap<WORD, WORD>::const_iterator c_iter_mm ;
//  std::pair< c_iter_mm,c_iter_mm> stdpair1stAndLastLeft2RightGrammarPartID =
//    m_stdmultimap_wGrammarPartID2wGrammarPartID.equal_range(
//    wLeftGrammarPartIDForRule ) ;
  typedef std::multimap<WORD, GrammarRule>::const_iterator c_iter_mm ;
  std::pair< c_iter_mm,c_iter_mm> stdpair1stAndLastLeft2RightGrammarPartID =
    //Get all rules that match to the left grammar part ID.
    //e.g. for params passed "the", "car":
    // -left grammar part ID: "definite_article"
    // -grammar rule may be "definite_article" + "noun" = "def_article_noun".
    // it gets all rules whose left grammar part ID is "definite_article"
    //(The grammar rules container isn't modified during parsing)
    m_stdmmap_wLeftChildGrammarPartID2SuperordinateGrammarRule.equal_range(
    wLeftGrammarPartIDForRule ) ;
  WORD wSuperordinateGrammarPartID ;

  //e.g. token 1: "the" -> article, token2: "man" -> noun
  // article + noun = def_article_noun
  //if( RulesAppliesTo( iter, iterLeftGrammarPart ) )
//  for( c_iter_mm iterLeft2RightGrammarPartIDCurrent =
//    stdpair1stAndLastLeft2RightGrammarPartID.first ;
//    iterLeft2RightGrammarPartIDCurrent !=
//    stdpair1stAndLastLeft2RightGrammarPartID.second ;
//    ++ iterLeft2RightGrammarPartIDCurrent
//    )

  //Traverse all rules whose left grammar part ID is the same as for the passed
  //left grammar part.
  //(The grammar rules container isn't modified during parsing)
  for( c_iter_mm iterLeft2RightGrammarPartIDCurrent =
    stdpair1stAndLastLeft2RightGrammarPartID.first ;
    iterLeft2RightGrammarPartIDCurrent !=
    stdpair1stAndLastLeft2RightGrammarPartID.second ;
    ++ iterLeft2RightGrammarPartIDCurrent
    )
  {
    //grammar rule whose left child grammar part ID is the same as the passed
    //ID.
    const GrammarRule & r_grammarrule = iterLeft2RightGrammarPartIDCurrent->
      second ;
    //Rule matches for 2 neighboured grammatical items.
    if( //iterLeft2RightGrammarPartIDCurrent->second ==

      //The 2 passed grammar parts belong to the same rule:
      //e.g. rule "definite_noun": superordinate ID = 2, right child grammar part ID:1
        //  "the": ID = 0   "car" ID = 1
      r_grammarrule.m_wRightChildGrammarPartID == wRightGrammarPartIDForRule
//#ifdef _DEBUG
//      )
//      if(
//      //And the left grammar part was has not been applied to a rule yet.
//      //(this serves as a break condition)
//#else
//      &&
//#endif
//      m_stdset_p_grammarpartAllSuperordinate.find(
//        & iter_mm_rightmostidx2grammarptLeftGrammarPart->second ) ==
//        m_stdset_p_grammarpartAllSuperordinate.end()
      )
    {
//      bRuleApplied = true ;
      wSuperordinateGrammarPartID = r_grammarrule.m_wSuperordinateGrammarRuleID ;
      dwLeftMostTokenIndexOfRule =
//        iter_mm_rightmostidx2grammarptLeftGrammarPart->second.
//          m_dwLeftmostIndex ;
        iter_mm_rightmostidx2p_grammarptLeftGrammarPart->second->
        m_dwLeftmostIndex ;
      dwRightMostTokenIndexOfRule =
//        iter_wLeftmostIndex2grammarpartRightGrammarPart->second.
//          m_dwRightmostIndex ;
        iter_wLeftmostIndex2p_grammarpartRightGrammarPart->second->
        m_dwRightmostIndex ;
      //TODO create on heap and only add pointer to it.
      //GrammarPart grammarpart( //iterLeftGrammarPart->first, //wGrammarPartIDOfRule
      GrammarPart * p_grammarpart = new GrammarPart(
        //wLeftGrammarPartIDForRule
        dwLeftMostTokenIndexOfRule
        , //wRightGrammarPartIDForRule
        dwRightMostTokenIndexOfRule
        , wSuperordinateGrammarPartID
        ) ;
      //e.g. for the grammar part "def_article_noun" add "the" (article) and
      //"car" (noun)
      //In order to translate: add children; also for GrammarPart::"<" operator
//      grammarpart.AddLeftChild(
      p_grammarpart->AddLeftChild(
//        iter_mm_rightmostidx2grammarptLeftGrammarPart->second ) ;
        * iter_mm_rightmostidx2p_grammarptLeftGrammarPart->second ) ;
//      grammarpart.AddRightChild(
      p_grammarpart->AddRightChild(
//        iter_wLeftmostIndex2grammarpartRightGrammarPart->second ) ;
        * iter_wLeftmostIndex2p_grammarpartRightGrammarPart->second ) ;
      //Do not store 1 and the same grammar part more than once in
      // -the container "leftmost token index -> grammar part"
      // -the container "rightmost token index -> grammar part"#
      ////Do not do this because: if "the sheep and the sheep":
      //// first: "the sheep and the sheep"
      ////           \  /          \   /
      ////       def_singular    def_singular
      ////               \          /
      ////               3rdPersPlural
      //// Then another 3rdPersPlural would not be possible:
      ////      "the sheep and the sheep"
      ////           \  /          \   /
      ////       def_plural    def_plural
      ////               \          /
      ////               3rdPersPlural
      //This must be checked as a break criteria! Else resolving grammar rules
      //was endless.
      if( m_stdset_grammarpartAllSuperordinate.find( //grammarpart
        * p_grammarpart ) ==
          m_stdset_grammarpartAllSuperordinate.end()
          )
      {
        GrammarPart & grammarpart = * p_grammarpart ;
        bRuleApplied = true ;
//        //e.g. for the grammar part "def_article_noun" add "the" (article) and
//        //"car" (noun)
//        //In order to translate: add children
//        grammarpart.AddLeftChild(
//          iter_mm_rightmostidx2grammarptLeftGrammarPart->second ) ;
//        grammarpart.AddRightChild(
//          iter_wLeftmostIndex2grammarpartRightGrammarPart->second ) ;
#ifdef COMPILE_WITH_LEFT2RIGHT_RELATIONSHIP
        //when getting the personindex this is a fast method:
        //the car and the cat suck.
        //  \ /    /   \  /
        //  nc(1) /   nc(2)
        //   \   /     /
        //   nc_conj  /
        //        \  /
        //     nc_conj_nc
        //        |
        //
        //insert left child to right child mappings for:
        // the -> car
        // nc(1) -> and
        // the  -> cat
        // nc_conj -> nc(2)
        // then when I want to know the person index:
        // the car -> singular -> 3rd pers sing; save "3rd person sing" in
        //    parent "nc(1)"
        //   -add "nc(1)" to a list for the next level,
        //  -continue with
        // the car + and : no change
        // the cat -> singular -> 3rd pers sing
        // "the cat and" + "the cat" : 3rd pers sing + 3rd pers sing = 3rd pers plur
        //
        m_stdmapgrammarpartLeftChild2grammarpartRightChild.insert(
          std::pair<GrammarPart *,GrammarPart *>(
          iter_mm_rightmostidx2grammarptLeftGrammarPart->second,
          iter_wLeftmostIndex2grammarpartRightGrammarPart->second
          )
          ) ;
#endif
        //Bad approach !? better simply check whether the number of tokens for
        //an applied rule got larger for each parse level?!
  //      //Memorize the grammar parts involved in applied rules as a break
  //      //condition for applying rules for all parse levels.
  //      m_stdset_p_grammarpartAllSuperordinate.insert(
  //        & iter_mm_rightmostidx2grammarptLeftGrammarPart->second ) ;
  //      m_stdset_p_grammarpartAllSuperordinate.insert(
  //        & iter_wLeftmostIndex2grammarpartRightGrammarPart->second ) ;

        //In order to not to add it again to the list of ALL grammar parts
#ifdef _DEBUG
        std::pair<std::set<GrammarPart>::iterator,bool> pair_ =
#endif
          m_stdset_grammarpartAllSuperordinate.insert(
          //grammarpart
          * p_grammarpart ) ;
#ifdef _DEBUG
        //"false if the set already contained an element whose key had an
        //equivalent value in the ordering"
        if( pair_.second == false )
        {
          //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//          #pragma GCC diagnostic ignored  "-Wunused"
          const GrammarPart & r_gp = *(pair_.first) ;
          SUPRESS_UNUSED_VARIABLE_WARNING(r_gp)
//          #pragma GCC diagnostic pop
          int i = 0 ;
          SUPRESS_UNUSED_VARIABLE_WARNING(i)
          DEBUG_COUT( "already contained")
        }
#endif

        m_wNumberOfTokensForAppliedGrammarRule = dwRightMostTokenIndexOfRule -
            dwLeftMostTokenIndexOfRule ;
        if( m_wNumberOfTokensForAppliedGrammarRule >
          m_wBiggestNumberOfTokensForAppliedGrammarRule )
          m_wBiggestNumberOfTokensForAppliedGrammarRule =
              m_wNumberOfTokensForAppliedGrammarRule ;

      //  m_stdmultimap_.insert( iterLeftGrammarPart->first , )

        //The map with the leftmost indexes can be used for translation:
        //  1. iterate over all GrammarPart beginning at index i
        //  2. use the GrammarPart that starts at index i and covers the most tokens,
        //    i.e. has the rightmost token index of all GrammarPart elements
        //    starting at index i
        //m_stdmultimap_dwLeftmostIndex2grammarpart.insert(
        //m_iter_stdmap_wParseLevelIndex2stdmultimap_dwLeftmostIndex2grammarpart->second.
//        mp_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate->
        m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel.
          insert(
//          std::pair<DWORD, GrammarPart> (
          std::pair<DWORD, GrammarPart *> (
          //iterLeftGrammarPart->first
          //wLeftGrammarPartIDForRule
          dwLeftMostTokenIndexOfRule
          , //grammarpart
          p_grammarpart
          )
        ) ;
        //m_stdmultimap_dwRightmostIndex2grammarpart.insert(
        //m_iter_stdmap_wParseLevelIndex2stdmultimap_dwLeftmostIndex2grammarpart->second.
//        mp_stdmultimap_dwRightmostIndex2grammarpartSuperordinate->
        m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel.
          insert(
//          std::pair<DWORD, GrammarPart> (
          std::pair<DWORD, GrammarPart *> (
          //iterRightGrammarPart->first
          //wRightGrammarPartIDForRule
          dwRightMostTokenIndexOfRule
          , //grammarpart
          p_grammarpart
          )
        ) ;

  //      //Now delete the 2 inner grammar parts as a break criteria to avoid
  //      //an endless loop. e.g. delete the token indices of
  //      // "the" and "vacuum cleaner" for ""the vacuum cleaner".
  //      //Delete the left of the 2 inner grammar parts.
  //      m_stdmultimap_dwRightmostIndex2grammarpart.erase(
  //        iter_mm_rightmostidx2grammarptLeftGrammarPart ) ;
  //      //Delete the right of the 2 inner grammar parts.
  //      m_stdmultimap_dwLeftmostIndex2grammarpart.erase(
  //        iter_wLeftmostIndex2grammarpartRightGrammarPart ) ;


        #ifdef _DEBUG
        std::string stdstrSuperordinate = "" ;
        std::string stdstrRightChild = "";
        std::string stdstrLeftChild = "" ;
        ci_stdmap_wRuleID2stdstrRuleNameSuperordinate =
          m_stdmap_wRuleID2RuleName.find( wSuperordinateGrammarPartID) ;
        ci_stdmap_wLeftGrammarPartID2stdstrRuleName =
            m_stdmap_wRuleID2RuleName.find(
//            iter_mm_rightmostidx2grammarptLeftGrammarPart->second.
            iter_mm_rightmostidx2p_grammarptLeftGrammarPart->second->
            m_wGrammarPartID ) ;
        ci_stdmap_wRightGrammarPartID2stdstrRuleName =
          m_stdmap_wRuleID2RuleName.find(
//          iter_wLeftmostIndex2grammarpartRightGrammarPart->second.
          iter_wLeftmostIndex2p_grammarpartRightGrammarPart->second->
          m_wGrammarPartID ) ;
        GrammarPart & r_grammarpartLeft =
//            iter_mm_rightmostidx2grammarptLeftGrammarPart->second ;
          * iter_mm_rightmostidx2p_grammarptLeftGrammarPart->second ;
        GrammarPart & r_grammarpartRight =
//            iter_wLeftmostIndex2grammarpartRightGrammarPart->second ;
          * iter_wLeftmostIndex2p_grammarpartRightGrammarPart->second ;
        if( ci_stdmap_wRuleID2stdstrRuleNameSuperordinate !=
            m_stdmap_wRuleID2RuleName.end()
          )
          stdstrSuperordinate = ci_stdmap_wRuleID2stdstrRuleNameSuperordinate->
          second ;
        if( ci_stdmap_wRightGrammarPartID2stdstrRuleName !=
              m_stdmap_wRuleID2RuleName.end()
          )
         stdstrRightChild = ci_stdmap_wRightGrammarPartID2stdstrRuleName->
           second ;
        if( ci_stdmap_wLeftGrammarPartID2stdstrRuleName !=
            m_stdmap_wRuleID2RuleName.end()
          )
          stdstrLeftChild = ci_stdmap_wLeftGrammarPartID2stdstrRuleName->
          second ;
        DEBUG_COUT( "children rule \"" << stdstrSuperordinate
          << "(" << & grammarpart << ",ID=" << wSuperordinateGrammarPartID << ")"
          <<  "\" found for "
          //"the token range"
          //<< dwLeftMostTokenIndexOfRule << "..."
          //<< dwRightMostTokenIndexOfRule
          << stdstrLeftChild
          << "(" << & r_grammarpartLeft << "ID=" << r_grammarpartLeft.m_wGrammarPartID << ")"
          << r_grammarpartLeft.m_dwLeftmostIndex
          << ","
          << r_grammarpartLeft.m_dwRightmostIndex
          << ";"
          << stdstrRightChild
          << "(" << & r_grammarpartRight << ",ID=" << r_grammarpartRight.m_wGrammarPartID << ")"
          << r_grammarpartRight.m_dwLeftmostIndex
          << ","
          << r_grammarpartRight.m_dwRightmostIndex
          //<< "]"
          << "\n"
          );
        //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//        #pragma GCC diagnostic ignored  "-Wunused"
        //Useless instruction just for breakpoint possibility.
        int i = 0 ;
        SUPRESS_UNUSED_VARIABLE_WARNING(i)
        #endif
      }
      else
      {
        //already applied a rule to these children-> delete.
        delete p_grammarpart ;
//        DEBUG_COUTN("grammar part " << GetGrammarPartName(
//          grammarpart.m_wGrammarPartID ) << grammarpart.m_dwLeftmostIndex
//          << "," <<
//          grammarpart.m_dwRightmostIndex << " already applied to a rule yet")
      }
      //IDEA (e.g. as break condition when to end parsing): 
      // -insert parse level
      // -insert number of tokens
      //Then the parse area that covers the most tokens is taken to translate.
      //Or maybe the highest parse level is taken for translation.

//      //possible break conditions:
//      //-if no more grammar rules can be applied, i.e. the size of the
//      //  grammar part container does not grow anymore.
//      m_stdmapGrammarPartSize2GrammarPart.insert(
//        std::pair<DWORD, GrammarPart>(
//        dwRightMostTokenIndexOfRule - dwLeftMostTokenIndexOfRule
//        )
//        ) ;

//      //for every rise level ( where a rule is supposed to be applied in order
//      //to get a larger area) a new map is created (and added to a container).
//      mp_stdmultimap_dwRightmostIndex2grammarpartCurrent->insert(
//        std::pair<DWORD, GrammarPart> (
//        //iterRightGrammarPart->first
//        wRightGrammarPartIDForRule
//        , grammarpart )
//      ) ;

//      //Erase in order to not apply the rule again (and to fullfill the break
//      //criteria when no rule could be applied.
//      m_stdmultimap_dwLeftmostIndex2grammarpart.erase(
//        iterLeftGrammarPart
//        , iterLeftGrammarPart ) ;
//      EraseEntry( m_stdmultimap_dwLeftmostIndex2grammarpart ,
//        iterLeftGrammarPart->first, iterLeftGrammarPart->second ) ;
//      m_stdmultimap_dwRightmostIndex2grammarpart.erase
    }
#ifdef _DEBUG
    else
    {
//      GrammarPart & r_gp = iter_mm_rightmostidx2grammarptLeftGrammarPart->
//          second ;
//      DEBUG_COUTN("grammar part " << GetGrammarPartName(
//        r_gp.m_wGrammarPartID ) << r_gp.m_dwLeftmostIndex << "," <<
//        r_gp.m_dwRightmostIndex << " already applied to a rule yet")
    }
#endif
  }
  return bRuleApplied ;
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
//  grammarpart.AddLeftChild( iterLeftGrammarPart->second ) ;
//  grammarpart.AddRightChild( iterRightGrammarPart->second ) ;
////  m_stdmultimap_.insert( iterLeftGrammarPart->first , )
//  m_stdmultimap_wLeftmostIndex2grammarpart.insert(
//    iterLeftGrammarPart->first
//    , grammarpart ) ;
//  m_stdmultimap_wLeftmostIndex2grammarpart.insert(
//    iterRightGrammarPart->first
//    , grammarpart ) ;
//}

//Minimizes, e.g. "article + noun" = "def_article_noun"
BYTE ParseByRise::ResolveGrammarRules(
  //Maintaining 2 maps with both leftmost and rightmost indexes should be faster
  //when searching for neighboured grammar parts:
  //  0    1       2      3      <-index
  //"the vacuum cleaner sucks."
  // +----------------+
  //   e.g. current grammar part: rightmost index: 2: find neighbour with
  //     leftmost index "3".

  //Use a multimap because at an index x more than 1 grammar part may exist.
  //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
  //"verb".
//  std::multimap<DWORD, GrammarPart> & r_stdmultimap_dwLeftmostIndex2grammarpart
//  , std::multimap<DWORD, GrammarPart> & r_stdmultimap_dwRightmostIndex2grammarpart
//  , std::multimap<DWORD, GrammarPart> &
//    r_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate
//  , std::multimap<DWORD, GrammarPart> &
//    r_stdmultimap_dwRightmostIndex2grammarpartSuperordinate
  )
{
  BYTE byGrammarRuleApplied = 0 ;
  m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel.clear() ;
  m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel.clear() ;
//  WORD wGrammarIDOfRule ;
//  m_iter_stdmap_wParseLevelIndex2stdmultimap_dwLeftmostIndex2grammarpart =
//    m_stdmap_wParseLevelIndex2stdmultimap_dwLeftmostIndex2grammarpart.insert(
//    m_dwMapIndex
//    ,
//    )
//  mp_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate =
//    & r_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate ;
//  mp_stdmultimap_dwRightmostIndex2grammarpartSuperordinate =
//    & r_stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;
//  std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator iter =
//    stdmap_wIndex2p_set_p_vocabularyandtranslation.begin() ;

  //The left grammar part is the one whos rightmost token index is the
  //neighbour of the right part's leftmost token index:
  // 1     2       3      4    <-index
  //the vacuum cleaner sucks.
  //    +-------------+
  //               ^
  //               |
  //      rightmost index
//  std::multimap<DWORD, GrammarPart>::iterator
//    iter_mm_rightmostidx2grammarptLeftGrammarPart =
//    r_stdmultimap_dwRightmostIndex2grammarpart.begin() ;
  std::multimap<DWORD, GrammarPart *>::iterator
    iter_mm_rightmostidx2p_grammarptLeftGrammarPart =
    m_stdmultimap_dwRightmostIndex2p_grammarpart.begin() ;
    //m_stdmultimap_dwRightmostIndex2grammarpart.begin() ;

//  if ( iter != stdmap_wIndex2p_set_p_vocabularyandtranslation.end()
//    )
  {
//    std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator
//      iterRightOf = iter ;
//    iterRightOf ++ ;
    while( //iter_mm_rightmostidx2grammarptLeftGrammarPart !=
//      r_stdmultimap_dwRightmostIndex2grammarpart.end()
      iter_mm_rightmostidx2p_grammarptLeftGrammarPart !=
      m_stdmultimap_dwRightmostIndex2p_grammarpart.end()
      )
    {
//      DEBUG_COUTN("current rightmost index:" <<
//          iter_mm_rightmostidx2grammarptLeftGrammarPart->first )
      //Find the direct right neighbuored grammar part.
//      iterRightGrammarPart = r_stdmapwLeftmostIndex2grammarpart.find(
//        iterLeftGrammarPart->first + 1 )
//      typedef std::multimap<DWORD, GrammarPart>::iterator
//        iter_mm_dword2grammarpart ;
//      std::pair< iter_mm_dword2grammarpart,iter_mm_dword2grammarpart>
//        //1st and last iterator having the same value within the multimap.
//        stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart =
//        r_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
//        //m_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
//        //Get all grammar parts that are direct neighbours of the left grammar
//        //part.
//        iter_mm_rightmostidx2grammarptLeftGrammarPart->first + 1 ) ;
//
//      //e.g. token 1: "the" -> article, token2: "man" -> noun
//      // article + noun = def_article_noun
//      //if( RulesAppliesTo( iter, iterLeftGrammarPart ) )
//      for( iter_mm_dword2grammarpart
//        //This iterator has all items that have the same leftmost token index
//        //that is _directly_ right of the rightmost token index of the
//        //current grammar part.
//        iter_wLeftmostIndex2grammarpartRightGrammarPart =
//        //Iterator with 1st key with searched value.
//        stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart.first ;
//        iter_wLeftmostIndex2grammarpartRightGrammarPart !=
//        //Iterator beyond last key with searched value.
//        stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart.second ;
//        ++ iter_wLeftmostIndex2grammarpartRightGrammarPart )
//      {
//        DEBUG_COUTN("grammar part with leftmost index starting at:" <<
//          iter_mm_rightmostidx2grammarptLeftGrammarPart->first + 1 << ":"
//          << GetGrammarPartName(
//          iter_mm_rightmostidx2grammarptLeftGrammarPart->second.m_wGrammarPartID )
//          )

      typedef std::multimap<DWORD, GrammarPart *>::iterator
        iter_mm_dword2p_grammarpart ;
      std::pair< iter_mm_dword2p_grammarpart,iter_mm_dword2p_grammarpart>
        //1st and last iterator having the same value within the multimap.
        stdpair1stAndLastRangeIterwLeftmostIndex2p_grammarpart =
        m_stdmultimap_dwLeftmostIndex2p_grammarpart.equal_range(
        //m_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
        //Get all grammar parts that are direct neighbours of the left grammar
        //part.
        iter_mm_rightmostidx2p_grammarptLeftGrammarPart->first + 1 ) ;

      //e.g. token 1: "the" -> article, token2: "man" -> noun
      // article + noun = def_article_noun
      //if( RulesAppliesTo( iter, iterLeftGrammarPart ) )
      for( iter_mm_dword2p_grammarpart
        //This iterator has all items that have the same leftmost token index
        //that is _directly_ right of the rightmost token index of the
        //current grammar part.
        iter_wLeftmostIndex2p_grammarpartRightGrammarPart =
        //Iterator with 1st key with searched value.
        stdpair1stAndLastRangeIterwLeftmostIndex2p_grammarpart.first ;
        iter_wLeftmostIndex2p_grammarpartRightGrammarPart !=
        //Iterator beyond last key with searched value.
        stdpair1stAndLastRangeIterwLeftmostIndex2p_grammarpart.second ;
        ++ iter_wLeftmostIndex2p_grammarpartRightGrammarPart )
      {
//        if( GrammarRuleAppliesTo(
//           iterLeftGrammarPart->second.m_wGrammarPartID
//           , iterRightGrammarPart->second.m_wGrammarPartID
//           , wGrammarIDOfRule
//            )
//          )
//        {
//
//          //E.g. insert "def_article_noun".
//          //There may be more than 1 possibility to translate a text/ sentence.
//          //E.g. "the glasses are broken" : broken = "kaputt" oder Participle perfect
//          //of "to break" -> "gebrochen".
//          //So insert every possibility.
//          InsertIntoPossibilityTree(//dwIndexOf1stToken, dwIndexOfLastToken,
//            //iter, iterLeftGrammarPart
//            iterLeftGrammarPart
//            , iterRightGrammarPart
//            //Use numbers because they can be compared faster than strings.
//            //E.g. ID for "def_article_noun"
//            , wIDofGrammaticalItem ) ;
//        }
//        InsertIfGrammarRuleAppliesTo(
//          iter_mm_rightmostidx2grammarptLeftGrammarPart
//          , iter_wLeftmostIndex2grammarpartRightGrammarPart ) ;
        if( InsertIfGrammarRuleAppliesTo(
          iter_mm_rightmostidx2p_grammarptLeftGrammarPart
          , iter_wLeftmostIndex2p_grammarpartRightGrammarPart ) )
          byGrammarRuleApplied = 1 ;
      }
//      ++ iter_mm_rightmostidx2grammarptLeftGrammarPart ;
      ++ iter_mm_rightmostidx2p_grammarptLeftGrammarPart ;
    } //while
  }
  if( byGrammarRuleApplied )
  {
    m_stdmultimap_dwRightmostIndex2p_grammarpart.insert(
      m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel.begin() ,
      m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel.end()
      ) ;
  //    //parsebyrise.
  //    m_stdmultimap_dwLeftmostIndex2grammarpart.insert(
  //      stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.begin( ),
  //      stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.end( )
  //      );
    m_stdmultimap_dwLeftmostIndex2p_grammarpart.insert(
      m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel.begin() ,
      m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel.end()
      ) ;
  }
  ++ m_wParseLevel ;
  return byGrammarRuleApplied ;
}

//Inserts for every word found in the dictionary its leftmost token index
//into a list and its rightmost token index (because a word may have more
//than 1 token, e.g. "vacuum cleaner") into another list.
//(words are grammar part at the leaves of the parse tree)
void ParseByRise::StoreWordTypeAndGermanTranslation(
//  PositionstdstringVector & psv
  PositionStringVector & psv
  , DWORD dwTokenIndex
//  , std::map <WORD, std::set<VocabularyAndTranslation *> *> &
//    stdmap_wIndex2p_set_p_vocabularyandtranslation
//  , std::vector<GrammarPart> & stdvector_grammarpart ;

  //Maintaining 2 maps with both leftnost and rightmost indexes should be faster
  //when searching for neighboured grammar parts:
  //  0    1       2      3      <-index
  //"the vacuum cleaner sucks."
  // +----------------+
  //   e.g. current grammar part: rightmost index: 2: find neighbour with
  //     leftmost index "3".
//  , std::map<DWORD, GrammarPart> & r_stdmapwLeftmostIndex2grammarpart
//  , std::map<DWORD, GrammarPart> & r_stdmapwRightmostIndex2grammarpart
  //Use a multimap because at an index x more than 1 grammar part may exist.
  //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
  //"verb".
//  , std::multimap<DWORD, GrammarPart> & r_stdmultimap_wLeftmostIndex2grammarpart
//  , std::multimap<DWORD, GrammarPart> & r_stdmultimap_wRightmostIndex2grammarpart
  )
{
  //When using references the container was empty after leaving this function.
//  std::multimap<DWORD, GrammarPart> & r_stdmultimap_wLeftmostIndex2grammarpart
//    = m_stdmultimap_dwLeftmostIndex2grammarpart ;
//  std::multimap<DWORD, GrammarPart> & r_stdmultimap_wRightmostIndex2grammarpart
//    = m_stdmultimap_dwRightmostIndex2grammarpart ;

//  std::multimap<DWORD, GrammarPart> * p_stdmultimap_wLeftmostIndex2grammarpart
//    = & m_stdmultimap_dwLeftmostIndex2grammarpart ;
//  std::multimap<DWORD, GrammarPart> * p_stdmultimap_wRightmostIndex2grammarpart
//    = & m_stdmultimap_dwRightmostIndex2grammarpart ;
  std::set<VocabularyAndTranslation *> setpvocabularyandtranslation ;
  DWORD dwTokenIndexRightMost = dwTokenIndex ;
  LetterNode * p_letternode = g_lettertree.searchAndReturnLetterNode( psv,
    //If "vacuum cleaner" and wTokenIndex is "0" before the call it gets "1".
    dwTokenIndexRightMost );
  //If the word was found.
  if( p_letternode )
  {
    DEBUG_COUT( "word found in dictionary: " << GetBetweenAsStdString( psv,
        dwTokenIndex, dwTokenIndexRightMost) << "\n" )
    std::set<VocabularyAndTranslation *> * psetpvocabularyandtranslation =
      NULL ;
    psetpvocabularyandtranslation = p_letternode->
      m_psetpvocabularyandtranslation ;
    if( psetpvocabularyandtranslation )
    {
//      stdmap_wIndex2p_set_p_vocabularyandtranslation.insert(
//        std::pair<WORD>, std::set<VocabularyAndTranslation *> *>
//        (wTokenIndex,psetpvocabularyandtranslation) ) ;

      //GrammarPart grammarPart(
//      //Via the indices the tokens can be got later.
//      dwTokenIndex, dwTokenIndexRightMost) ;

      //GetVocabulariesContainingWordClass(psetpvocabularyandtranslation,
      //  setpvocabularyandtranslation);

      //Loop for all vocabularies having at least 1 word that
      //equals the current token.
      //There may be more than 1 vocabulary for the same word:
      //e.g. "love" may be either a noun ("the love") or a verb ("to love").
      for(std::set<VocabularyAndTranslation *>::iterator iter =
        psetpvocabularyandtranslation->begin() ; iter !=
        psetpvocabularyandtranslation->end() ; iter ++ )
      {
        //Create a grammar part object for _every_ single
        //VocabularyAndTranslation object.
        GrammarPart * p_grammarPart = new GrammarPart(
          //Via the indices the tokens can be got later.
          dwTokenIndex, dwTokenIndexRightMost) ;
//        //word type
//        (*iter)->m_byType ;
//        grammarPart.m_wGrammarPartID = (*iter)->m_byType ;
//        grammarPart.SetGrammarPartID( (*iter)->m_byType ) ;
        p_grammarPart->SetGrammarPartID( (*iter)->m_byType ) ;
        //For accessing the vocabulary attributes later for translating.
//        grammarPart.m_pvocabularyandtranslation =
        p_grammarPart->m_pvocabularyandtranslation =
          (*iter) ;
        //r_stdmapwLeftmostIndex2grammarpart.insert(
        //r_stdmultimap_wLeftmostIndex2grammarpart.insert(
//        p_stdmultimap_wLeftmostIndex2grammarpart->insert(
//          std::pair<WORD, GrammarPart>
//            ( dwTokenIndex, grammarPart )
//          ) ;
        DEBUG_COUTN("inserting "
          << GetGrammarPartName( (*iter)->m_byType )
          << "(ID)=" << (WORD) (*iter)->m_byType
          << " address: " << p_grammarPart
          << " for token range " << dwTokenIndex << ":" << dwTokenIndexRightMost
          )
        m_stdmultimap_dwLeftmostIndex2p_grammarpart.insert(
          std::pair<WORD, GrammarPart *>
            ( dwTokenIndex, p_grammarPart )
          ) ;
        //r_stdmapwRightmostIndex2grammarpart.insert(
        //r_stdmultimap_wRightmostIndex2grammarpart.insert(
//        p_stdmultimap_wRightmostIndex2grammarpart->insert(
//          std::pair<WORD, GrammarPart>
//            ( dwTokenIndexRightMost , grammarPart )
//          ) ;
        m_stdmultimap_dwRightmostIndex2p_grammarpart.insert(
          std::pair<WORD, GrammarPart *>
            ( dwTokenIndexRightMost , p_grammarPart )
          ) ;
      }
    }
  }
//  DWORD dwSize = //parsebyrise.
//    m_stdmultimap_dwRightmostIndex2grammarpart.size() ;
//  dwSize = //parsebyrise.
//    m_stdmultimap_dwLeftmostIndex2grammarpart.size() ;
}

bool ParseByRise::InsertSuperordinateGrammarPart(
//  std::multimap<DWORD, GrammarPart> & multmap_token_index2grammarpt
  std::multimap<DWORD, GrammarPart *> & r_multmap_token_index2p_grammarpt
  , bool bMemorizeInsertion
  )
{
  bool bReplaced = false ;
  DWORD dwMapIndex = 0 ;
#ifdef _DEBUG
  std::map<WORD, std::string>::const_iterator ci_stdmap_wRuleID2stdstrRuleName ;
  std::map<WORD, std::string>::const_iterator ci_stdmap_wLeftGrammarPartID2stdstrRuleName ;
#endif
  std::map<WORD,WORD>::iterator iter_stdmap_wGrammarPartID2wSuperordinateID ;
//  std::multimap<DWORD, GrammarPart>::iterator iter_mm_idx2grammarpt
  std::multimap<DWORD, GrammarPart *>::iterator iter_mm_token_idx2p_grammarpt
//    = r_multmap_token_index2p_grammarpt.begin() ;
    = m_stdmultimap_dwLeftmostIndex2p_grammarpart.begin() ;
  WORD wGrammarPartID ;

  while( //iter_mm_idx2grammarpt !=
      //multmap_token_index2grammarpt.end()
      iter_mm_token_idx2p_grammarpt !=
            r_multmap_token_index2p_grammarpt.end()
    )
  {
    wGrammarPartID = //iter_mm_idx2grammarpt->second.
        iter_mm_token_idx2p_grammarpt->second->
        m_wGrammarPartID ;
//    m_stdmultimap_wGrammarPartID2SuperordinateID.find(wGrammarPartID)
//          std::pair< iter_mm_dword2grammarpart,iter_mm_dword2grammarpart>
//            //1st and last iterator having the same value within the multimap.
//            stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart =
//                m_stdmultimap_wGrammarPartID2SuperordinateID.equal_range(
//            //m_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
//            //Get all grammar parts that are direct neighbours of the left grammar
//            //part.
//            iter_mm_rightmostidx2grammarptLeftGrammarPart->first + 1 ) ;
//
//          //e.g. token 1: "the" -> article, token2: "man" -> noun
//          // article + noun = def_article_noun
//          //if( RulesAppliesTo( iter, iterLeftGrammarPart ) )
//          for( iter_mm_dword2grammarpart
//            //This iterator has all items that have the same leftmost token index
//            //that is _directly_ right of the rightmost token index of the
//            //current grammar part.
//            iter_wLeftmostIndex2grammarpartRightGrammarPart =
//            //Iterator with 1st key with searched value.
//            stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart.first ;
//            iter_wLeftmostIndex2grammarpartRightGrammarPart !=
//            //Iterator beyond last key with searched value.
//            stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart.second ;
//            ++ iter_wLeftmostIndex2grammarpartRightGrammarPart )


    //loop for replacing the grammar part ID by the superordinate grammar part
    //ID: e.g.    "the car"
    //              \  /
    //  definite_article_noun
    //       |
    //  subj_or_obj_enum_ele  <-- superordinate
    //       |
    //   subject    <--superordinate
    //
    //  -> replaced the ID 2 times
//    do
//    {
//      bReplaced = false ;
      iter_stdmap_wGrammarPartID2wSuperordinateID =
          m_stdmap_wGrammarPartID2SuperordinateID.find(
        wGrammarPartID) ;
      if( iter_stdmap_wGrammarPartID2wSuperordinateID !=
          m_stdmap_wGrammarPartID2SuperordinateID.end()
        )
      {
#ifdef _DEBUG
       std::string stdstrGrammarPart = GetGrammarPartName(wGrammarPartID ) ;
       //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//       #pragma GCC diagnostic ignored  "-Wunused-variable"
       GrammarPart & r_grammarpartSubclass = //iter_mm_idx2grammarpt->second ;
           * iter_mm_token_idx2p_grammarpt->second ;
       SUPRESS_UNUSED_VARIABLE_WARNING(r_grammarpartSubclass)
//       WORD wGrammarPartIDsubclass = iter_mm_idx2grammarpt->first ;
       WORD wGrammarPartIDsubclass = iter_mm_token_idx2p_grammarpt->first ;
       SUPRESS_UNUSED_VARIABLE_WARNING(wGrammarPartIDsubclass)
#endif
        //e.g. "the car"
        //     def_noun
        //        |
        // subj_or_obj_ee
        // -> the indices for "subj_or_obj_ee" stay the same. So copy.
//        GrammarPart grammarpartSuperordinate(
//          iter_mm_idx2grammarpt->second ) ;
        GrammarPart * p_grammarpartSuperordinate = new GrammarPart(
          * iter_mm_token_idx2p_grammarpt->second ) ;
//        GrammarPart & grammarpartSuperordinate = * p_grammarpartSuperordinate ;
  //      iter_mm_rightmostidx2grammarptLeftGrammarPart->second.m_wGrammarPartID =
  //          iter_stdmap_w2w->second ;
        //e.g. subj_or_obj_ee->def_noun
//        grammarpartSuperordinate.mp_grammarpartLeftChild =
        p_grammarpartSuperordinate->mp_grammarpartLeftChild =
          //& iter_mm_idx2grammarpt->second ;
          iter_mm_token_idx2p_grammarpt->second ;
//        grammarpartSuperordinate.mp_grammarpartRightChild = NULL ;
        p_grammarpartSuperordinate->mp_grammarpartRightChild = NULL ;
#ifdef _DEBUG
       WORD wGrammarPartIDSuperordinate =
         iter_stdmap_wGrammarPartID2wSuperordinateID->second ;
       std::string stdstrGrammarPartIDSuperordinate =
           GetGrammarPartName(wGrammarPartIDSuperordinate) ;
//       grammarpartSuperordinate.m_wGrammarPartID = wGrammarPartIDSuperordinate ;
       p_grammarpartSuperordinate->m_wGrammarPartID =
           wGrammarPartIDSuperordinate ;
#else
//        grammarpartSuperordinate.m_wGrammarPartID =
        p_grammarpartSuperordinate->m_wGrammarPartID =
          iter_stdmap_wGrammarPartID2wSuperordinateID->second ;
#endif
  //      iter_mm_rightmostidx2grammarptLeftGrammarPart->second =
  //          grammarpartSuperordinate ;
  #ifdef _DEBUG
//        WORD wIndex = iter_mm_idx2grammarpt->first ;
    //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//    #pragma GCC diagnostic ignored  "-Wunused"
        WORD wIndex = iter_mm_token_idx2p_grammarpt->first ;
        SUPRESS_UNUSED_VARIABLE_WARNING(wIndex)
//    #pragma GCC diagnostic pop
  #endif
        //Do not store 1 and the same grammar part more than once in
        // -the container "leftmost token index -> grammar part"
        // -the container "rightmost token index -> grammar part"
        if( m_stdset_grammarpartAllSuperordinate.find(
//          grammarpartSuperordinate
          * p_grammarpartSuperordinate) ==
            m_stdset_grammarpartAllSuperordinate.end()
            )
        {
//          std::pair<c_iter_mmap_dw2p_grammarpart,c_iter_mmap_dw2p_grammarpart>
//            stdpair_iter =
//          m_stdmultimap_dwRightmostIndex2p_grammarpart.equal_range(
//            p_grammarpartSuperordinate->mp_grammarpartLeftChild->
//            m_dwRightmostIndex ) ;
//          for( //c_iter_mmap_dw2grammarpart iterCurrent
//              c_iter_mmap_dw2p_grammarpart iterCurrent = stdpair_iter.first ;
//              iterCurrent != stdpair_iter.second ; ++ iterCurrent )
//          {
//            if( iterCurrent->second ==
//                p_grammarpartSuperordinate->mp_grammarpartLeftChild )
//          }

          //TODO runtime error in
    //      DirectingLeavesMultipleIterTraverser::
    //        GetNextRightGrammarPartNotProcessedYet()
          //when inserted into the map

//          multmap_token_index2grammarpt.insert(
//            std::pair< WORD, GrammarPart >
//            (
//    //          iter_stdmap_w2w->first ,
//              iter_mm_idx2grammarpt->first ,
//              grammarpartSuperordinate
//            )
//            ) ;
          //http://msdn.microsoft.com/en-us/library/1ka6hya8%28v=VS.100%29.aspx:
          //"Inserting elements invalidates no iterators."
//          r_multmap_token_index2p_grammarpt.insert(
//            std::pair< WORD, GrammarPart *>
//            (
//    //          iter_stdmap_w2w->first ,
//              iter_mm_token_idx2p_grammarpt->first ,
//              & grammarpartSuperordinate
//            )
//            ) ;
#ifdef _DEBUG
          std::string stdstrSuperordinate ;
          std::string stdstrSubclassName ;
          WORD wSuperordinateGrammarPartID =
            iter_stdmap_wGrammarPartID2wSuperordinateID->second ;
          ci_stdmap_wRuleID2stdstrRuleName = m_stdmap_wRuleID2RuleName.find(
            wSuperordinateGrammarPartID) ;
          ci_stdmap_wLeftGrammarPartID2stdstrRuleName =
              m_stdmap_wRuleID2RuleName.find(
                wGrammarPartID ) ;
          GrammarPart & r_grammarpartLeft =
//              iter_mm_idx2grammarpt->second ;
            * iter_mm_token_idx2p_grammarpt->second ;
          if( ci_stdmap_wRuleID2stdstrRuleName !=
              m_stdmap_wRuleID2RuleName.end()
            )
            stdstrSuperordinate = ci_stdmap_wRuleID2stdstrRuleName->second ;
          if( ci_stdmap_wLeftGrammarPartID2stdstrRuleName !=
                m_stdmap_wRuleID2RuleName.end()
            )
            stdstrSubclassName = ci_stdmap_wLeftGrammarPartID2stdstrRuleName->
              second ;
          {
            DEBUG_COUTN( "superclass rule \"" << stdstrSuperordinate
            << "("
            //<< & grammarpartSuperordinate
            << p_grammarpartSuperordinate
            << ",ID=" << wSuperordinateGrammarPartID << ")"
            <<  "\" found for "
            //"the token range"
            //<< dwLeftMostTokenIndexOfRule << "..."
            //<< dwRightMostTokenIndexOfRule
            << stdstrSubclassName
            << "(address:" << & r_grammarpartLeft
            << ",mmap index:" << dwMapIndex
            << "ID=" << r_grammarpartLeft.m_wGrammarPartID << ")"
            << r_grammarpartLeft.m_dwLeftmostIndex
            << ","
            << r_grammarpartLeft.m_dwRightmostIndex
            << ";"
            //<< "]"
            ) ;
          }
#endif
//          //Insert if at the second map, not when at the first map!
//          //(else it is NOT inserted into the 2nd map)
//          if( bMemorizeInsertion //->lefmost token index map
//              )
          {
            m_stdset_grammarpartAllSuperordinate.insert(
              //grammarpartSuperordinate
              * p_grammarpartSuperordinate
              ) ;
            m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel.
              insert(
              std::pair< WORD, GrammarPart *>
              (
                //Leftmost token index of the child grammar part.
                iter_mm_token_idx2p_grammarpt->first ,
                //& grammarpartSuperordinate
                p_grammarpartSuperordinate
              )
              ) ;
          }
//          else
          {
            m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel.
              insert(
              std::pair< WORD, GrammarPart *>
              (
//                iter_mm_token_idx2p_grammarpt->first ,
                p_grammarpartSuperordinate->mp_grammarpartLeftChild->
                            m_dwRightmostIndex ,
//                & grammarpartSuperordinate
                p_grammarpartSuperordinate
              )
              ) ;
          }
          bReplaced = true ;
        }
        else
          delete p_grammarpartSuperordinate ;
        //just replace the grammar part ID
//        iter_mm_idx2grammarpt->second.m_wGrammarPartID =
//            iter_stdmap_wGrammarPartID2wSuperordinateID->second ;
//        wGrammarPartID = iter_mm_idx2grammarpt->second.m_wGrammarPartID ;
        wGrammarPartID = iter_mm_token_idx2p_grammarpt->second->m_wGrammarPartID ;
      }
//    }while( bReplaced ) ;
//    ++ iter_mm_idx2grammarpt ;
    ++ iter_mm_token_idx2p_grammarpt ;
#ifdef _DEBUG
    ++ dwMapIndex ;
#endif
  }
  DEBUG_COUTN("InsertSuperordinateGrammarPart end")
  return bReplaced ;
}

bool ParseByRise::ReplaceGrammarPartIDsBySuperordinate()
{
  bool bReplacedGrammarPartIDsBySuperordinate = false ;
  m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel.clear() ;
  m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel.clear() ;

  //  std::multimap<DWORD, GrammarPart>::iterator
//    iter_mm_rightmostidx2grammarptLeftGrammarPart =
//    .begin() ;
//  InsertSuperordinateGrammarPart( //m_stdmultimap_dwRightmostIndex2grammarpart,
//    m_stdmultimap_dwRightmostIndex2p_grammarpart ,
//    false ) ;
  bReplacedGrammarPartIDsBySuperordinate = InsertSuperordinateGrammarPart(
    //m_stdmultimap_dwLeftmostIndex2grammarpart
    m_stdmultimap_dwLeftmostIndex2p_grammarpart , true ) ;
  if( bReplacedGrammarPartIDsBySuperordinate )
  {
    m_stdmultimap_dwRightmostIndex2p_grammarpart.insert(
      m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel.begin() ,
      m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel.end()
      ) ;
    m_stdmultimap_dwLeftmostIndex2p_grammarpart.insert(
      m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel.begin() ,
      m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel.end()
      ) ;
  }
  return bReplacedGrammarPartIDsBySuperordinate ;
}

void ParseByRise::ResolveGrammarRulesForAllParseLevels()
{
  //Use a multimap because at an index x more than 1 grammar part may exist.
  //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
  //"verb".
  std::multimap<DWORD, GrammarPart> stdmultimap_dwLeftmostIndex2grammarpart ;
  std::multimap<DWORD, GrammarPart> stdmultimap_dwRightmostIndex2grammarpart ;
  WORD wstdmultimap_wLeftmostIndex2grammarpartSize ;
//  std::multimap<DWORD, GrammarPart> *
//    p_stdmultimap_dwLeftmostIndex2grammarpart =
//    & //parsebyrise.
//    m_stdmultimap_dwLeftmostIndex2grammarpart ;
//  std::multimap<DWORD, GrammarPart> *
//    p_stdmultimap_dwRightmostIndex2grammarpart =
//    & //parsebyrise.
//    m_stdmultimap_dwRightmostIndex2grammarpart ;
//
//  std::multimap<DWORD, GrammarPart> *
//    p_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate ;
//  std::multimap<DWORD, GrammarPart> *
//    p_stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;
//  std::multimap<DWORD, GrammarPart>
//    stdmultimap_dwLeftmostIndex2grammarpartSuperordinate ;
//  std::multimap<DWORD, GrammarPart>
//    stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;

  std::multimap<DWORD, GrammarPart>
    stdmultimap_dwRightmostIndex2grammarpartEmpty ;

  std::map< DWORD, std::multimap<DWORD, GrammarPart> >
    map_stdmultimap_dwLeftmostIndex2grammarpart ;
  std::map< DWORD, std::multimap<DWORD, GrammarPart> >
    map_stdmultimap_dwRightmostIndex2grammarpart ;
  DWORD dwSize ;
  m_dwMapIndex = 0 ;
  bool bReplacedGrammarPartIDsBySuperordinate ;
  BYTE byGrammarRuleApplied ;
  do //Replace by superordinate grammar part ID loop.
  {
  do //Apply grammar rule loop.
  {
//    stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.clear() ;
//    stdmultimap_dwRightmostIndex2grammarpartSuperordinate.clear() ;
    //Add the new applied rules/ grammar parts later (else there may be
    //problems with the multimap iterator inside the resolve method)
    wstdmultimap_wLeftmostIndex2grammarpartSize =
      stdmultimap_dwLeftmostIndex2grammarpart.size() ;
//    p_stdmultimap_dwRightmostIndex2grammarpartSuperordinate =
//      & (
//        map_stdmultimap_dwLeftmostIndex2grammarpart.insert(
//          std::pair< DWORD, std::multimap<DWORD, GrammarPart> >
//          ( (DWORD) parsebyrise.m_dwMapIndex
//            , stdmultimap_dwRightmostIndex2grammarpartEmpty
//          )
//        )
//      .first->second
//      ) ;
//    p_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate =
//      & (map_stdmultimap_dwRightmostIndex2grammarpart.insert(
//      std::pair< DWORD, std::multimap<DWORD, GrammarPart> >
//        ( (DWORD) parsebyrise.m_dwMapIndex
//          , stdmultimap_dwRightmostIndex2grammarpartEmpty
//        )
//      ).first->second
//      ) ;
    ++ //parsebyrise.
      m_dwMapIndex ;
//    ReplaceGrammarPartIDsBySuperordinate() ;
    //parsebyrise.ResolveGrammarRules(
    byGrammarRuleApplied = ResolveGrammarRules(
      //Maintaining 2 maps with both leftmost and rightmost indexes should be faster
      //when searching for neighboured grammar parts:
      //  0    1       2      3      <-index
      //"the vacuum cleaner sucks."
      // +----------------+
      //   e.g. current grammar part: rightmost index: 2: find neighbour with
      //     leftmost index "3".

//      //Use a multimap because at an index x more than 1 grammar part may exist.
//      //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
//      //"verb".
//      * p_stdmultimap_dwLeftmostIndex2grammarpart
//      , * p_stdmultimap_dwRightmostIndex2grammarpart
//      //, * p_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate
//      , stdmultimap_dwLeftmostIndex2grammarpartSuperordinate
//      //, * p_stdmultimap_dwRightmostIndex2grammarpartSuperordinate
//      , stdmultimap_dwRightmostIndex2grammarpartSuperordinate
      ) ;

    #ifdef _DEBUG
      dwSize = //parsebyrise.
//        m_stdmultimap_dwRightmostIndex2grammarpart.size() ;
        m_stdmultimap_dwRightmostIndex2p_grammarpart.size() ;
      DEBUG_COUTN("size of rightmost list token index multimap:" << dwSize )
      dwSize = //parsebyrise.
//        m_stdmultimap_dwLeftmostIndex2grammarpart.size() ;
        m_stdmultimap_dwLeftmostIndex2p_grammarpart.size() ;
      DEBUG_COUTN("size of leftmost list token index multimap:" << dwSize )
//      dwSize = stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.size() ;
//      dwSize = stdmultimap_dwRightmostIndex2grammarpartSuperordinate.size() ;
    #endif
    //Join the grammar parts and the superordinate grammar parts, i.e.
    //for "the vacuum cleaner sucks" join the superordinate grammar part
    // "definite_article_noun" for
    //  -"the" (definite_article) +
    //  -"vacuum cleaner" (noun)
    // so that the next time the rule "definite_article_noun + verb = clause"
    // for "the vacuum cleaner sucks" can be applied.
    //parsebyrise.
//    m_stdmultimap_dwRightmostIndex2grammarpart.insert(
//      stdmultimap_dwRightmostIndex2grammarpartSuperordinate.begin( ),
//      stdmultimap_dwRightmostIndex2grammarpartSuperordinate.end( )
//      );
      DEBUG_COUT("resolving grammar/ parse level " << m_dwMapIndex << "\n" )
  }
  //while no grammar rule for "left + right = parent" applied.
  while( //parsebyrise.m_stdmultimap_dwLeftmostIndex2grammarpart.size() >
      //wstdmultimap_wLeftmostIndex2grammarpartSize
    //! p_stdmultimap_dwRightmostIndex2grammarpartSuperordinate->empty()
//    ! stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.empty()
    byGrammarRuleApplied
    ) ;
  //e.g. now at "the car and the cat" ...
  //              \ /         \  /
  //          def_noun      def_noun
  //
  //...and no more grammar rules can be applied.
  //BUT there is a rule for "subj_or_obj_enum_ele" + "conj" =
  //  "subj_or_obj_enum_ele,conj"
  // and "subj_or_obj_enum_ele" is a superordinate grammar ID for "def_noun"


    bReplacedGrammarPartIDsBySuperordinate =
        ReplaceGrammarPartIDsBySuperordinate() ;
  }
  while( bReplacedGrammarPartIDsBySuperordinate );
  m_wParseLevel = 0 ;
  DEBUG_COUT("end of resolving grammar\n" )
#ifdef _DEBUG
//  dwSize = //parsebyrise.
//    m_stdmultimap_dwRightmostIndex2grammarpart.size() ;
//  dwSize = //parsebyrise.
//    m_stdmultimap_dwLeftmostIndex2grammarpart.size() ;
//  dwSize = stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.size() ;
//  dwSize = stdmultimap_dwRightmostIndex2grammarpartSuperordinate.size() ;
#endif
}
