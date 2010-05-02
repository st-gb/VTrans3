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

extern LetterTree g_lettertree ;

//Serves for the purpose that an applied grammar rule should not be added more
//than once.
//std::set::find() return valid iterator if grammarpart1 < grammarpart2 = false
// and grammarpart2 < grammarpart1 = false
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
          && m_dwRightmostIndex < cr_gpCompare.m_dwRightmostIndex )
          bLess = true ;
      }
  }
//  if( m_wGrammarPartID < cr_gpCompare.m_wGrammarPartID &&
//      m_dwLeftmostIndex < cr_gpCompare.m_dwLeftmostIndex &&
//      m_dwRightmostIndex < cr_gpCompare.m_dwRightmostIndex
//      )
//    bLess = true ;

  return bLess ;
}

ParseByRise::ParseByRise()
  :
   //m_wNumberOfSuperordinateRules( 0 )
  //,
  m_dwMapIndex(0)
{
  m_wNumberOfSuperordinateRules = EnglishWord::beyond_last_entry ;
  InsertFundamentalRuleIDs() ;
}

ParseByRise::ParseByRise(const ParseByRise& orig) {
}

ParseByRise::~ParseByRise() {
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

//Clears (empties) the previously generated parse tree.
//This should be done for a next parse tree generation .
void ParseByRise::ClearParseTree()
 {
  m_stdset_grammarpartAllSuperordinate.clear() ;
  m_wBiggestNumberOfTokensForAppliedGrammarRule = 0 ;
   m_stdmultimap_dwRightmostIndex2grammarpart.clear() ;
   m_stdmultimap_dwLeftmostIndex2grammarpart.clear() ;
   m_stdset_p_grammarpartAllSuperordinate.clear() ;
 }

//Creates the leafs of possible parse trees ausgehend from the source text.
// Leaves are word classes, e.g. nouns.
// If a grammar rule applies later then these leaves can make up a parse trees:
// the vacuum cleaner sucks.
// +-+ +------------+ +---+    <- leaves
//   \ /              /
// noun_construct    /           <- tree struct: parse tree
//              \   /
//              clause       <- tree struct: parse tree
void ParseByRise::CreateInitialGrammarParts (std::string & stdstrText )
{
  //PositionstdstringVector psv ;
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
      m_stdmultimap_dwRightmostIndex2grammarpart.size() ;
    dwSize = //parsebyrise.
      m_stdmultimap_dwLeftmostIndex2grammarpart.size() ;
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
  DWORD dwLeftMostTokenIndex )
{
  DWORD dwNumberOfTokensCoveredMax = 0 ;
  DWORD dwNumberOfTokensCoveredCurrent ;
  GrammarPart * p_gp = NULL ;
  //std::multimap<DWORD, GrammarPart>::const_iterator iter ;
  std::pair<c_iter_mmap_dw2grammarpart,c_iter_mmap_dw2grammarpart>
    stdpair_iter =
    m_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
    dwLeftMostTokenIndex) ;
#ifdef _DEBUG
  WORD wNumberOfGrammapartsStartingAtPos = 0 ;
#endif
  for( c_iter_mmap_dw2grammarpart iterCurrent = stdpair_iter.first ;
      iterCurrent != stdpair_iter.second ; ++ iterCurrent )
  {
#ifdef _DEBUG
    ++ wNumberOfGrammapartsStartingAtPos ;
#endif
    dwNumberOfTokensCoveredCurrent = iterCurrent->second.m_dwRightmostIndex -
      dwLeftMostTokenIndex
      //to include grammar parts with 1 token
      + 1 ;
    if( dwNumberOfTokensCoveredCurrent > dwNumberOfTokensCoveredMax )
    {
      p_gp = (GrammarPart *) & iterCurrent->second ;
      dwNumberOfTokensCoveredMax = dwNumberOfTokensCoveredCurrent ;
    }
  }
#ifdef _DEBUG
  if( ! p_gp )
    {
    int i = 0 ;
    }
#endif
  return p_gp ;
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
//  +-----+   +-----+   / \
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
  m_psv.at(dwLeftmostIndex) ;
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
    , "noun_construct"
    ) ;

}

void ParseByRise::InsertRuleIDsForWordClasses()
{
  InsertGrammarRule( EnglishWord::noun, "noun" ) ;
  InsertGrammarRule( EnglishWord::main_verb, "main_verb" ) ;
  InsertGrammarRule( EnglishWord::English_definite_article, "definite_article" ) ;
  InsertGrammarRule( EnglishWord::conjunction, "conjunction" ) ;
}

bool ParseByRise::GrammarPartIDIsWordClass( WORD wGrammarPartID )
{
  bool bGrammarPartIDIsWordClass = false ;
  switch( wGrammarPartID )
  {
  case EnglishWord::noun :
  case EnglishWord::main_verb :
  case EnglishWord::English_definite_article :
  case EnglishWord::conjunction :
    bGrammarPartIDIsWordClass = true ;
    break ;
  }
  return bGrammarPartIDIsWordClass ;
}

void ParseByRise::InsertFundamentalRuleIDs()
{
//  for( BYTE by = 0 ; by < NUMBER_OF_ENGLISH_WORD_CLASSES ; ++ by )
//  {
//
//  }
  //InsertGrammarRule( EnglishWord::noun, "noun" ) ;
  InsertRuleIDsForWordClasses() ;
  InsertGrammarRule( 
    EnglishWord::English_definite_article,
    EnglishWord::noun,
    "noun_construct") ;
  InsertGrammarRule(
    "noun_construct"
    , EnglishWord::main_verb
    , "clause"
    ) ;
  //TODO
  WORD wIDnoun_construct_conj = InsertGrammarRule(
    "noun_construct",
    EnglishWord::conjunction,
    "noun_construct_conj"
    ) ;
//  InsertGrammarRule(
//    EnglishWord::conjunction,
//    "noun_construct",
//    "conj_noun_construct"
//    ) ;
  InsertGrammarRule(
    "noun_construct_conj",
    "noun_construct",
    "noun_constr,conj,noun_constr"
    ) ;
//  InsertGrammarRule( // e.g. "the car and " + "the cat and the dog"
//    "noun_construct_conj",
//    "noun_constr,conj,noun_constr",
//    "noun_constr,conj,noun_constr"
//    ) ;
  InsertGrammarRule(
    //e.g. "the car and the car and"
    "noun_construct_conj",
    "noun_construct_conj",
    //"noun_construct_conj"
    wIDnoun_construct_conj
    ) ;
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
    DEBUG_COUT("inserting rule \"" << cp_chSuperordinateGrammarRuleName << "\" "
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
      c_iterLeft != m_stdmap_RuleName2RuleID.end()
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
      c_iterLeft != m_stdmap_RuleName2RuleID.end()
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
    //e.g.    noun_construct (superordinate grammar part)
    //        /          \
    //definite article  noun
    m_stdmultimap_wGrammarPartID2SuperordinateID.insert(
      std::pair<WORD,WORD> (wGrammarRuleIDLeft,
      //ID for added rule.
      wSuperordinateGrammarRuleID)
      ) ;
    m_stdmultimap_wGrammarPartID2SuperordinateGrammarRule.insert(
      std::pair<WORD,GrammarRule> (wGrammarRuleIDLeft,
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
//    InsertRuleID2NameMapping( m_wNumberOfSuperordinateRules
//      , cp_chGrammarPartName ) ;
  }
}

void ParseByRise::InsertGrammarRule(
  //A grammar part with this ID must exist when calling this function.
  WORD wGrammarRuleIDLeft
  //A grammar part with this ID must exist when calling this function.
  , WORD wGrammarRuleIDRight
  , //std::string
  //Name of the new rule to insert that consists of left and right existing
  //rules.
  const char * cp_chGrammarPartName
  )
{
  //When parsing, compare the neighboured grammar parts' IDs for a shared
  //superordinate grammar part ID.
  //e.g.    noun_construct (superordinate grammar part)
  //        /          \
  //definite article  noun
  m_stdmultimap_wGrammarPartID2SuperordinateID.insert(
    std::pair<WORD,WORD> (wGrammarRuleIDLeft,
    //ID for added rule.
    m_wNumberOfSuperordinateRules)
    ) ;
  m_stdmultimap_wGrammarPartID2SuperordinateGrammarRule.insert(
    std::pair<WORD,GrammarRule> (wGrammarRuleIDLeft,
      GrammarRule(wGrammarRuleIDRight,m_wNumberOfSuperordinateRules)
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
  InsertRuleID2NameMapping( m_wNumberOfSuperordinateRules
    , cp_chGrammarPartName ) ;
  ++ m_wNumberOfSuperordinateRules ;

}

void ParseByRise::InsertGrammarRule(WORD wGrammarRuleID
  , const char * cp_ch )
{
  //GrammarRule()
//  m_stdmap_wRuleID2RuleName.insert( std::pair<WORD,std::string>
//    ( wGrammarRuleID, std::string( cp_ch) )
//    ) ;
  InsertRuleID2NameMapping( wGrammarRuleID , cp_ch ) ;
}

void ParseByRise::InsertRuleID2NameMapping( WORD wGrammarRuleID
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
//  std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator & iter
//  , std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator &
//    iterRightOf
  //Maintaining 2 maps with both leftmost and rightmost indexes should be faster
  //when searching for neighboured grammar parts:
  //  0    1       2      3      <-index
  //"the vacuum cleaner sucks."
  // +----------------+
  //   e.g. current grammar part: rightmost index: 2: find neighbour with
  //     leftmost index "3".
//  std::map<DWORD, GrammarPart> & r_stdmapwLeftmostIndex2grammarpart
//  , std::map<DWORD, GrammarPart> & r_stdmapwRightmostIndex2grammarpart
//  WORD wLeftGrammarPartIDForRule
//  , WORD wRightGrammarPartIDForRule
  std::multimap<DWORD, GrammarPart>::iterator
    iter_mm_rightmostidx2grammarptLeftGrammarPart
  , std::multimap<DWORD, GrammarPart>::iterator
    iter_wLeftmostIndex2grammarpartRightGrammarPart
  )
{
  bool bRuleApplied = false ;
  DWORD dwLeftMostTokenIndexOfRule ;
  DWORD dwRightMostTokenIndexOfRule ;
#ifdef _DEBUG
  std::map<WORD, std::string>::const_iterator ci_stdmap_wRuleID2stdstrRuleName ;
  std::map<WORD, std::string>::const_iterator ci_stdmap_wLeftGrammarPartID2stdstrRuleName ;
  std::map<WORD, std::string>::const_iterator ci_stdmap_wRightGrammarPartID2stdstrRuleName ;
#endif
  WORD wLeftGrammarPartIDForRule =
    iter_mm_rightmostidx2grammarptLeftGrammarPart->second.m_wGrammarPartID ;
  WORD wRightGrammarPartIDForRule =
    iter_wLeftmostIndex2grammarpartRightGrammarPart->second.m_wGrammarPartID ;
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
    //e.g. for params passed "the", "car": left grammar part ID: "definite_article"
    //Grammar rule may be "definite_article" + "noun" = "noun_construct".
    // it gets all rules whose left grammar part ID is "definite_article"
    //(The grammar rules container isn't modified during parsing)
    m_stdmultimap_wGrammarPartID2SuperordinateGrammarRule.equal_range(
    wLeftGrammarPartIDForRule ) ;
  WORD wSuperordinateGrammarPartID ;

  //e.g. token 1: "the" -> article, token2: "man" -> noun
  // article + noun = noun_construct
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
    const GrammarRule & r_grammarrule = iterLeft2RightGrammarPartIDCurrent->
      second ;
    //Rule matches for 2 neighboured grammatical items.
    if( //iterLeft2RightGrammarPartIDCurrent->second ==

      //The 2 passed grammar parts belong to the same rule.
      r_grammarrule.m_wGrammarRuleID == wRightGrammarPartIDForRule
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
      bRuleApplied = true ;
      wSuperordinateGrammarPartID = r_grammarrule.m_wSuperordinateGrammarRuleID ;
      dwLeftMostTokenIndexOfRule =
        iter_mm_rightmostidx2grammarptLeftGrammarPart->second.
        m_dwLeftmostIndex ;
      dwRightMostTokenIndexOfRule =
        iter_wLeftmostIndex2grammarpartRightGrammarPart->second.
        m_dwRightmostIndex ;
      //TODO create on heap and only add pointer to it.
      GrammarPart grammarpart( //iterLeftGrammarPart->first, //wGrammarPartIDOfRule
        //wLeftGrammarPartIDForRule
        dwLeftMostTokenIndexOfRule
        , //wRightGrammarPartIDForRule
        dwRightMostTokenIndexOfRule
        , wSuperordinateGrammarPartID
        ) ;
      if( m_stdset_grammarpartAllSuperordinate.find( grammarpart ) ==
          m_stdset_grammarpartAllSuperordinate.end()
          )
      {
        //For the translation: add children
        grammarpart.AddLeftChild(
          iter_mm_rightmostidx2grammarptLeftGrammarPart->second ) ;
        grammarpart.AddRightChild(
          iter_wLeftmostIndex2grammarpartRightGrammarPart->second ) ;
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
          grammarpart ) ;
#ifdef _DEBUG
        //"false if the set already contained an element whose key had an
        //equivalent value in the ordering"
        if( pair_.second == false )
        {
          const GrammarPart & r_gp = *(pair_.first) ;
          int i = 0 ;
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
        mp_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate->
          insert(
          std::pair<DWORD, GrammarPart> (
          //iterLeftGrammarPart->first
          //wLeftGrammarPartIDForRule
          dwLeftMostTokenIndexOfRule
          , grammarpart )
        ) ;
        //m_stdmultimap_dwRightmostIndex2grammarpart.insert(
        //m_iter_stdmap_wParseLevelIndex2stdmultimap_dwLeftmostIndex2grammarpart->second.
        mp_stdmultimap_dwRightmostIndex2grammarpartSuperordinate->
          insert(
          std::pair<DWORD, GrammarPart> (
          //iterRightGrammarPart->first
          //wRightGrammarPartIDForRule
          dwRightMostTokenIndexOfRule
          , grammarpart )
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
        ci_stdmap_wRuleID2stdstrRuleName = m_stdmap_wRuleID2RuleName.find(
          wSuperordinateGrammarPartID) ;
        ci_stdmap_wLeftGrammarPartID2stdstrRuleName =
            m_stdmap_wRuleID2RuleName.find(
            iter_mm_rightmostidx2grammarptLeftGrammarPart->
            second.m_wGrammarPartID ) ;
        ci_stdmap_wRightGrammarPartID2stdstrRuleName =
          m_stdmap_wRuleID2RuleName.find(
          iter_wLeftmostIndex2grammarpartRightGrammarPart->
          second.m_wGrammarPartID ) ;
        GrammarPart & r_grammarpartLeft =
            iter_mm_rightmostidx2grammarptLeftGrammarPart->second ;
        GrammarPart & r_grammarpartRight =
            iter_wLeftmostIndex2grammarpartRightGrammarPart->second ;
        if( ci_stdmap_wRuleID2stdstrRuleName != m_stdmap_wRuleID2RuleName.end()
          && ci_stdmap_wRightGrammarPartID2stdstrRuleName !=
              m_stdmap_wRuleID2RuleName.end()
          && ci_stdmap_wLeftGrammarPartID2stdstrRuleName !=
              m_stdmap_wRuleID2RuleName.end()
          )
          std::cout << "rule \"" << ci_stdmap_wRuleID2stdstrRuleName->second
          << "(ID=" << wSuperordinateGrammarPartID << ")"
          <<  "\" found for "
          //"the token range"
          //<< dwLeftMostTokenIndexOfRule << "..."
          //<< dwRightMostTokenIndexOfRule
          << ci_stdmap_wLeftGrammarPartID2stdstrRuleName->second
          << "(ID=" << r_grammarpartLeft.m_wGrammarPartID << ")"
          << r_grammarpartLeft.m_dwLeftmostIndex
          << ","
          << r_grammarpartLeft.m_dwRightmostIndex
          << ";"
          << ci_stdmap_wRightGrammarPartID2stdstrRuleName->second
          << "(ID=" << r_grammarpartRight.m_wGrammarPartID << ")"
          << r_grammarpartRight.m_dwLeftmostIndex
          << ","
          << r_grammarpartRight.m_dwRightmostIndex
          //<< "]"
          << "\n" ;
        #endif
      }
      else
      {
        DEBUG_COUTN("grammar part " << GetGrammarPartName(
          grammarpart.m_wGrammarPartID ) << grammarpart.m_dwLeftmostIndex
          << "," <<
          grammarpart.m_dwRightmostIndex << " already applied to a rule yet")
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

//Minimizes, e.g. "article + noun" = "noun_construct"
void ParseByRise::ResolveGrammarRules(
//  std::map <WORD, std::set<VocabularyAndTranslation *> *> &
//    stdmap_wIndex2p_set_p_vocabularyandtranslation
//  std::vector <GrammarRule>, std::set<VocabularyAndTranslation *> *> &
//    stdmap_wIndex2p_set_p_vocabularyandtranslation

  //Maintaining 2 maps with both leftmost and rightmost indexes should be faster
  //when searching for neighboured grammar parts:
  //  0    1       2      3      <-index
  //"the vacuum cleaner sucks."
  // +----------------+
  //   e.g. current grammar part: rightmost index: 2: find neighbour with
  //     leftmost index "3".
//  std::map<DWORD, GrammarPart> & r_stdmapwLeftmostIndex2grammarpart
//  , std::map<DWORD, GrammarPart> & r_stdmapwRightmostIndex2grammarpart

  //Use a multimap because at an index x more than 1 grammar part may exist.
  //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
  //"verb".
  std::multimap<DWORD, GrammarPart> & r_stdmultimap_dwLeftmostIndex2grammarpart
  , std::multimap<DWORD, GrammarPart> & r_stdmultimap_dwRightmostIndex2grammarpart
//  , GrammarRule & grammarrule
//  , std::multimap<WORD, GrammarRule > & r_stdmultimap_wGrammarPartID2grammarrule
  , std::multimap<DWORD, GrammarPart> &
    r_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate
  , std::multimap<DWORD, GrammarPart> &
    r_stdmultimap_dwRightmostIndex2grammarpartSuperordinate
  )
{
//  WORD wGrammarIDOfRule ;
//  m_iter_stdmap_wParseLevelIndex2stdmultimap_dwLeftmostIndex2grammarpart =
//    m_stdmap_wParseLevelIndex2stdmultimap_dwLeftmostIndex2grammarpart.insert(
//    m_dwMapIndex
//    ,
//    )
  mp_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate =
    & r_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate ;
  mp_stdmultimap_dwRightmostIndex2grammarpartSuperordinate =
    & r_stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;
//  std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator iter =
//    stdmap_wIndex2p_set_p_vocabularyandtranslation.begin() ;

  //The left grammar part is the one whos rightmost token index is the
  //neighbour of the right part's leftmost token index:
  // 1     2       3      4    <-index
  //the vaccuum cleaner sucks.
  //    +-------------+
  //               ^
  //               |
  //      rightmost index
  std::multimap<DWORD, GrammarPart>::iterator
    iter_mm_rightmostidx2grammarptLeftGrammarPart =
    r_stdmultimap_dwRightmostIndex2grammarpart.begin() ;
    //m_stdmultimap_dwRightmostIndex2grammarpart.begin() ;

//  if ( iter != stdmap_wIndex2p_set_p_vocabularyandtranslation.end()
//    )
  {
//    std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator
//      iterRightOf = iter ;
//    iterRightOf ++ ;
    while( iter_mm_rightmostidx2grammarptLeftGrammarPart != //stdmap_wIndex2p_set_p_vocabularyandtranslation.end()
      //r_stdmapwRightmostIndex2grammarpart.end()
      r_stdmultimap_dwRightmostIndex2grammarpart.end()
      //m_stdmultimap_dwRightmostIndex2grammarpart.end()
      )
    {
      DEBUG_COUTN("current rightmost index:" <<
          iter_mm_rightmostidx2grammarptLeftGrammarPart->first )
      //Find the direct right neighbuored grammar part.
//      iterRightGrammarPart = r_stdmapwLeftmostIndex2grammarpart.find(
//        iterLeftGrammarPart->first + 1 )
      typedef std::multimap<DWORD, GrammarPart>::iterator
        iter_mm_dword2grammarpart ;
      std::pair< iter_mm_dword2grammarpart,iter_mm_dword2grammarpart> 
        //1st and last iterator having the same value within the multimap.
        stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart =
        r_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
        //m_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
        //Get all grammar parts that are direct neighbours of the left grammar
        //part.
        iter_mm_rightmostidx2grammarptLeftGrammarPart->first + 1 ) ;

      //e.g. token 1: "the" -> article, token2: "man" -> noun
      // article + noun = noun_construct
      //if( RulesAppliesTo( iter, iterLeftGrammarPart ) )
      for( iter_mm_dword2grammarpart
        //This iterator has all items that have the same leftmost token index
        //that is _directly_ right of the rightmost token index of the
        //current grammar part.
        iter_wLeftmostIndex2grammarpartRightGrammarPart =
        //Iterator with 1st key with searched value.
        stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart.first ;
        iter_wLeftmostIndex2grammarpartRightGrammarPart !=
        //Iterator beyond last key with searched value.
        stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart.second ;
        ++ iter_wLeftmostIndex2grammarpartRightGrammarPart )
      {
        DEBUG_COUTN("grammar part with leftmost index starting at:" <<
          iter_mm_rightmostidx2grammarptLeftGrammarPart->first + 1 << ":"
          << GetGrammarPartName(
          iter_mm_rightmostidx2grammarptLeftGrammarPart->second.m_wGrammarPartID )
          )
//        if( GrammarRuleAppliesTo(
//           iterLeftGrammarPart->second.m_wGrammarPartID
//           , iterRightGrammarPart->second.m_wGrammarPartID
//           , wGrammarIDOfRule
//            )
//          )
//        {
//
//          //E.g. insert "noun_construct".
//          //There may be more than 1 possibility to translate a text/ sentence.
//          //E.g. "the glasses are broken" : broken = "kaputt" oder Participle perfect
//          //of "to break" -> "gebrochen".
//          //So insert every possibility.
//          InsertIntoPossibilityTree(//dwIndexOf1stToken, dwIndexOfLastToken,
//            //iter, iterLeftGrammarPart
//            iterLeftGrammarPart
//            , iterRightGrammarPart
//            //Use numbers because they can be compared faster than strings.
//            //E.g. ID for "noun_construct"
//            , wIDofGrammaticalItem ) ;
//        }
        InsertIfGrammarRuleAppliesTo(
          iter_mm_rightmostidx2grammarptLeftGrammarPart
          , iter_wLeftmostIndex2grammarpartRightGrammarPart ) ;
      }
      ++ iter_mm_rightmostidx2grammarptLeftGrammarPart ;
    } //while
  }
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
  std::multimap<DWORD, GrammarPart> * p_stdmultimap_wLeftmostIndex2grammarpart
    = & m_stdmultimap_dwLeftmostIndex2grammarpart ;
  std::multimap<DWORD, GrammarPart> * p_stdmultimap_wRightmostIndex2grammarpart
    = & m_stdmultimap_dwRightmostIndex2grammarpart ;
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

      GrammarPart grammarPart(
        //Via the indices the tokens can be got later.
        dwTokenIndex, dwTokenIndexRightMost) ;

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
////          //(*iter)->m_arstrEnglishWord[0] ==
////          (*iter)->m_arpletternodeLastEngChar[ ARRAY_INDEX_FOR_SINGULAR ]
////          == p_letternode
////          )
//        //word type
//        (*iter)->m_byType ;
        //grammarPart.setRuleID( (*iter)->m_byType ) ;
        grammarPart.m_wGrammarPartID = (*iter)->m_byType ;
        //For accessing the vocabulary attributes later.
        grammarPart.m_pvocabularyandtranslation =
          (*iter) ;
        //r_stdmapwLeftmostIndex2grammarpart.insert(
        //r_stdmultimap_wLeftmostIndex2grammarpart.insert(
        p_stdmultimap_wLeftmostIndex2grammarpart->insert(
          std::pair<WORD, GrammarPart>
            ( dwTokenIndex, grammarPart )
          ) ;
        //r_stdmapwRightmostIndex2grammarpart.insert(
        //r_stdmultimap_wRightmostIndex2grammarpart.insert(
        p_stdmultimap_wRightmostIndex2grammarpart->insert(
          std::pair<WORD, GrammarPart>
            ( dwTokenIndexRightMost , grammarPart )
          ) ;
      }
    }
  }
  DWORD dwSize = //parsebyrise.
    m_stdmultimap_dwRightmostIndex2grammarpart.size() ;
  dwSize = //parsebyrise.
    m_stdmultimap_dwLeftmostIndex2grammarpart.size() ;
}

void ParseByRise::ResolveGrammarRulesForAllParseLevels()
{
  //Use a multimap because at an index x more than 1 grammar part may exist.
  //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
  //"verb".
  std::multimap<DWORD, GrammarPart> stdmultimap_dwLeftmostIndex2grammarpart ;
  std::multimap<DWORD, GrammarPart> stdmultimap_dwRightmostIndex2grammarpart ;
  WORD wstdmultimap_wLeftmostIndex2grammarpartSize ;
  std::multimap<DWORD, GrammarPart> *
    p_stdmultimap_dwLeftmostIndex2grammarpart =
    & //parsebyrise.
    m_stdmultimap_dwLeftmostIndex2grammarpart ;
  std::multimap<DWORD, GrammarPart> *
    p_stdmultimap_dwRightmostIndex2grammarpart =
    & //parsebyrise.
    m_stdmultimap_dwRightmostIndex2grammarpart ;

  std::multimap<DWORD, GrammarPart> *
    p_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate ;
  std::multimap<DWORD, GrammarPart> *
    p_stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;
  std::multimap<DWORD, GrammarPart>
    stdmultimap_dwLeftmostIndex2grammarpartSuperordinate ;
  std::multimap<DWORD, GrammarPart>
    stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;

  std::multimap<DWORD, GrammarPart>
    stdmultimap_dwRightmostIndex2grammarpartEmpty ;

  std::map< DWORD, std::multimap<DWORD, GrammarPart> >
    map_stdmultimap_dwLeftmostIndex2grammarpart ;
  std::map< DWORD, std::multimap<DWORD, GrammarPart> >
    map_stdmultimap_dwRightmostIndex2grammarpart ;
  DWORD dwSize ;
  m_dwMapIndex = 0 ;
  do
  {
    stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.clear() ;
    stdmultimap_dwRightmostIndex2grammarpartSuperordinate.clear() ;
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
    //parsebyrise.ResolveGrammarRules(
    ResolveGrammarRules(
    //  std::map <WORD, std::set<VocabularyAndTranslation *> *> &
    //    stdmap_wIndex2p_set_p_vocabularyandtranslation
    //  std::vector <GrammarRule>, std::set<VocabularyAndTranslation *> *> &
    //    stdmap_wIndex2p_set_p_vocabularyandtranslation

      //Maintaining 2 maps with both leftnost and rightmost indexes should be faster
      //when searching for neighboured grammar parts:
      //  0    1       2      3      <-index
      //"the vacuum cleaner sucks."
      // +----------------+
      //   e.g. current grammar part: rightmost index: 2: find neighbour with
      //     leftmost index "3".
    //  std::map<DWORD, GrammarPart> & r_stdmapwLeftmostIndex2grammarpart
    //  , std::map<DWORD, GrammarPart> & r_stdmapwRightmostIndex2grammarpart

//      //Use a multimap because at an index x more than 1 grammar part may exist.
//      //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
//      //"verb".
//      stdmultimap_wLeftmostIndex2grammarpart
      * p_stdmultimap_dwLeftmostIndex2grammarpart
      , * p_stdmultimap_dwRightmostIndex2grammarpart
//      , stdmultimap_wRightmostIndex2grammarpart
      //, GrammarRule & grammarrule
      //, std::multimap<WORD, GrammarRule > & r_stdmultimap_wGrammarPartID2grammarrule
      //, * p_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate
      , stdmultimap_dwLeftmostIndex2grammarpartSuperordinate
      //, * p_stdmultimap_dwRightmostIndex2grammarpartSuperordinate
      , stdmultimap_dwRightmostIndex2grammarpartSuperordinate
      ) ;
//    p_stdmultimap_dwLeftmostIndex2grammarpart =
//      p_stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;
//    p_stdmultimap_dwRightmostIndex2grammarpart =
//      p_stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;

    #ifdef _DEBUG
      dwSize = //parsebyrise.
        m_stdmultimap_dwRightmostIndex2grammarpart.size() ;
      DEBUG_COUTN("size of rightmost list token index multimap:" << dwSize )
      dwSize = //parsebyrise.
        m_stdmultimap_dwLeftmostIndex2grammarpart.size() ;
      DEBUG_COUTN("size of leftmost list token index multimap:" << dwSize )
      dwSize = stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.size() ;
      dwSize = stdmultimap_dwRightmostIndex2grammarpartSuperordinate.size() ;
    #endif
    //Join the grammar parts and the superordinate grammar parts, i.e.
    //for "the vacuum cleaner sucks" join the superordinate grammar part
    // "noun_construct" "the vacuum cleaner"
    // and "sucks" so that the next time the rule "subject + verb = clause"
    // can be applied.
    //parsebyrise.
    m_stdmultimap_dwRightmostIndex2grammarpart.insert(
      stdmultimap_dwRightmostIndex2grammarpartSuperordinate.begin( ),
      stdmultimap_dwRightmostIndex2grammarpartSuperordinate.end( )
      );
    //parsebyrise.
    m_stdmultimap_dwLeftmostIndex2grammarpart.insert(
      stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.begin( ),
      stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.end( )
      );
      DEBUG_COUT("resolving grammar/ parse level " << m_dwMapIndex << "\n" )
  }
  while( //parsebyrise.m_stdmultimap_dwLeftmostIndex2grammarpart.size() >
      //wstdmultimap_wLeftmostIndex2grammarpartSize
    //! p_stdmultimap_dwRightmostIndex2grammarpartSuperordinate->empty()
    ! stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.empty()
    ) ;
  DEBUG_COUT("end of resolving grammar\n" )
#ifdef _DEBUG
  dwSize = //parsebyrise.
    m_stdmultimap_dwRightmostIndex2grammarpart.size() ;
  dwSize = //parsebyrise.
    m_stdmultimap_dwLeftmostIndex2grammarpart.size() ;
  dwSize = stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.size() ;
  dwSize = stdmultimap_dwRightmostIndex2grammarpartSuperordinate.size() ;
#endif
}
