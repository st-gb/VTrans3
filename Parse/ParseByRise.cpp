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

extern LetterTree g_lettertree ;

ParseByRise::ParseByRise()
  : m_wNumberOfSuperordinateRules( 0 )
  , m_dwMapIndex(0)
{
  InsertFundamentalRuleIDs() ;
}

ParseByRise::ParseByRise(const ParseByRise& orig) {
}

ParseByRise::~ParseByRise() {
}

void ParseByRise::InitGrammar()
{
//  m_stdmultimap_wGrammarPartID2wGrammarPartID.insert()
  InsertGrammarRule(
    EnglishWord::English_definite_article
    , EnglishWord::noun
    , "noun_construct"
    ) ;

}

void ParseByRise::InsertFundamentalRuleIDs()
{
//  for( BYTE by = 0 ; by < NUMBER_OF_ENGLISH_WORD_CLASSES ; ++ by )
//  {
//
//  }
  //InsertGrammarRule( EnglishWord::noun, "noun" ) ;
  InsertGrammarRule( EnglishWord::English_definite_article, EnglishWord::noun,
    "noun_construct") ;
  InsertGrammarRule(
    "noun_construct"
    , EnglishWord::main_verb
    , "clause"
    ) ;
}

void ParseByRise::InsertGrammarRule(
  const char * cp_chLeftGrammarRuleName
  , WORD wGrammarRuleIDRight
  , //std::string
  const char * cp_ch
  )
{
  std::map<std::string,WORD>::const_iterator iter =
    m_stdmap_RuleName2RuleID.find(
    cp_chLeftGrammarRuleName ) ;
  if( iter != m_stdmap_RuleName2RuleID.end() )
  {
    WORD wGrammarRuleIDLeft = iter->second ;
    InsertGrammarRule(
      wGrammarRuleIDLeft
      , wGrammarRuleIDRight
      , cp_ch ) ;
  }
}

void ParseByRise::InsertGrammarRule(
  WORD wGrammarRuleIDLeft
  , WORD wGrammarRuleIDRight
  , //std::string
  const char * cp_ch
  )
{
  //When parsing, cpmare the neighboured grammar parts' IDs for a shared
  //superordinate grammar part ID.
  m_stdmultimap_wGrammarPartID2SuperordinateID.insert(
    std::pair<WORD,WORD> (wGrammarRuleIDLeft, m_wNumberOfSuperordinateRules)
    ) ;
  m_stdmultimap_wGrammarPartID2SuperordinateGrammarRule.insert(
    std::pair<WORD,GrammarRule> (wGrammarRuleIDLeft,
      GrammarRule(wGrammarRuleIDRight,m_wNumberOfSuperordinateRules)
      )
    ) ;
  
  m_stdmultimap_wGrammarPartID2wGrammarPartID.insert(
    std::pair<WORD,WORD> (wGrammarRuleIDLeft, wGrammarRuleIDRight)
    ) ;

  m_stdmap_wRuleID2RuleName.insert( std::pair<WORD,std::string>
    ( m_wNumberOfSuperordinateRules, std::string( cp_ch) )
    ) ;
  m_stdmap_RuleName2RuleID.insert( std::pair<std::string,WORD>
    ( std::string( cp_ch) , m_wNumberOfSuperordinateRules )
    ) ;
  ++ m_wNumberOfSuperordinateRules ;
}

void ParseByRise::InsertGrammarRule(WORD wGrammarRuleID
  , const char * cp_ch )
{
  //GrammarRule()
  m_stdmap_wRuleID2RuleName.insert( std::pair<WORD,std::string>
    ( wGrammarRuleID, std::string( cp_ch) )
    ) ;
}

bool ParseByRise:://GrammarRuleAppliesTo(
  InsertIfGrammarRuleAppliesTo(
//  std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator & iter
//  , std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator &
//    iterRightOf
  //Maintaining 2 maps with both leftnost and rightmost indexes should be faster
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
  std::map<WORD, std::string>::const_iterator ci_stdmap_wRuleID2stdstrRuleName ;
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
      r_grammarrule.m_wGrammarRuleID ==
       wRightGrammarPartIDForRule
      && m_stdset_grammarpartAllSuperordinate.find(
        & iter_mm_rightmostidx2grammarptLeftGrammarPart->second ) ==
        m_stdset_grammarpartAllSuperordinate.end()
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
      //For the translation: add children
      grammarpart.AddLeftChild(
        iter_mm_rightmostidx2grammarptLeftGrammarPart->second ) ;
      grammarpart.AddRightChild(
        iter_wLeftmostIndex2grammarpartRightGrammarPart->second ) ;
      m_stdset_grammarpartAllSuperordinate.insert(
        & iter_mm_rightmostidx2grammarptLeftGrammarPart->second ) ;
      m_stdset_grammarpartAllSuperordinate.insert(
        & iter_wLeftmostIndex2grammarpartRightGrammarPart->second ) ;

    //  m_stdmultimap_.insert( iterLeftGrammarPart->first , )

      //The map with the leftmost indexes can be used for tranlation:
      //  1. iterate over all GrammarPart beginning at index i
      //  2. use the GrammarPart that starts at index i and covers the most tokens,
      //    i.e. has the righmost token index of all GrammarPart elements
      //    starting at index i
      //m_stdmultimap_dwLeftmostIndex2grammarpart.insert(
      //m_iter_mapindex2stdmap_stdmultimap_dwLeftmostIndex2grammarpart->second.
      mp_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate->
        insert(
        std::pair<DWORD, GrammarPart> (
        //iterLeftGrammarPart->first
        //wLeftGrammarPartIDForRule
        dwLeftMostTokenIndexOfRule
        , grammarpart )
      ) ;
      //m_stdmultimap_dwRightmostIndex2grammarpart.insert(
      //m_iter_mapindex2stdmap_stdmultimap_dwLeftmostIndex2grammarpart->second.
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
      if( ci_stdmap_wRuleID2stdstrRuleName != m_stdmap_wRuleID2RuleName.end() )
        std::cout << "rule \"" << ci_stdmap_wRuleID2stdstrRuleName->second <<
          "\" found\n";
      #endif
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

  //Maintaining 2 maps with both leftnost and rightmost indexes should be faster
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
//  m_iter_mapindex2stdmap_stdmultimap_dwLeftmostIndex2grammarpart =
//    m_mapindex2stdmap_stdmultimap_dwLeftmostIndex2grammarpart.insert(
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

      //e.g. roken 1: "the" -> article, token2: "man" -> noun
      // article + noun = noun_construct
      //if( RulesAppliesTo( iter, iterLeftGrammarPart ) )
      for( iter_mm_dword2grammarpart
        iter_wLeftmostIndex2grammarpartRightGrammarPart =
        //Iterator with 1st key with searched value.
        stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart.first ;
        iter_wLeftmostIndex2grammarpartRightGrammarPart !=
        //Iterator beyond last key with searched value.
        stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart.second ;
        ++ iter_wLeftmostIndex2grammarpartRightGrammarPart )
      {
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

void ParseByRise::StoreWordTypeAndGermanTranslation(
  PositionstdstringVector & psv
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
  std::multimap<DWORD, GrammarPart> & r_stdmultimap_wLeftmostIndex2grammarpart
    = m_stdmultimap_dwLeftmostIndex2grammarpart ;
  std::multimap<DWORD, GrammarPart> & r_stdmultimap_wRightmostIndex2grammarpart
    = m_stdmultimap_dwRightmostIndex2grammarpart ;
  std::set<VocabularyAndTranslation *> setpvocabularyandtranslation ;
  DWORD dwTokenIndexRightMost = dwTokenIndex ;
  LetterNode * p_letternode = g_lettertree.searchAndReturnLetterNode( psv,
    //If "vacuum cleaner" and wTokenIndex is "0" it gets "1"
    dwTokenIndexRightMost );
  //If the word was found.
  if( p_letternode )
  {
    std::cout << "word found in dictionary." ;
    std::set<VocabularyAndTranslation *> * psetpvocabularyandtranslation =
      NULL ;
    psetpvocabularyandtranslation = p_letternode->
      m_psetpvocabularyandtranslation ;
    if( psetpvocabularyandtranslation )
    {
//      stdmap_wIndex2p_set_p_vocabularyandtranslation.insert(
//        std::pair<WORD>, std::set<VocabularyAndTranslation *> *>
//        (wTokenIndex,psetpvocabularyandtranslation) ) ;

      GrammarPart grammarPart(dwTokenIndex, dwTokenIndexRightMost) ;

      //GetVocabulariesContainingWordClass(psetpvocabularyandtranslation,
      //  setpvocabularyandtranslation);

      //Loop for all vocabularies having at least 1 word that
      //equals the current token.
      //Because "love" may be either a noun or a verb
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
        //r_stdmapwLeftmostIndex2grammarpart.insert(
        r_stdmultimap_wLeftmostIndex2grammarpart.insert(
          std::pair<WORD, GrammarPart>
            ( dwTokenIndex, grammarPart )
          ) ;
        //r_stdmapwRightmostIndex2grammarpart.insert(
        r_stdmultimap_wRightmostIndex2grammarpart.insert(
          std::pair<WORD, GrammarPart>
            ( dwTokenIndexRightMost , grammarPart )
          ) ;
      }
    }
  }
}
