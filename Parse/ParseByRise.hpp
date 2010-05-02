/* 
 * File:   ParseByRise.hpp
 * Author: Stefan
 *
 * Created on 13. Februar 2010, 21:05
 */

#ifndef _PARSEBYRISE_HPP
#define	_PARSEBYRISE_HPP

#include <windef.h> //for DWORD
#include <string> //std::string
#include <map>
#include <set>
#include "Token.h" //for PositionstdstringVector

class VocabularyAndTranslation ;

//class GrammarRule
//idea: model the syntax tree of the source text as a binary tree structure.
//e.g.:
//the vacuum cleaner  sucks.
//--------------clause---------
//              /     \
//-noun_construct--- -main_verb
// /     \
//DA  ----noun------  DA=Definite Article
//
//e.g.:
//the car and the cat suck.
//--------------clause---------
//              /     \
//      EE_CJ_EE       \
//     /       |        \
//    /      CJ_EE      |
//   /       /   \      |
//--NC--  -CJ ---NC-- -main_verb   NC=Noun_Construct CJ=ConJunction
//
//e.g.
//the car , the dog and the cat suck.
//--------------clause---------
//              /     \
//      EE_C_EE_CJ_EE
//            /     \
//      EE_C_EE      \           EE=Enumeration element
//     /     |        \
//    /    C_EE       CJ_EE      |    C=Comma
//   /     /   \      /   \      |
//--NC--- C ---NC-- -CJ ---NC-- main_verb   NC=Noun_Construct CJ=ConJunction
//
//e.g.
//I  , he , it and the cat suck.
//--------------clause---------
//              /     \
//      EE_C_EE_CJ_EE
//            /     \
//     EE_C_EE      \           EE=Enumeration element
//     /  |
//    /  C_EE
//   /   /  \       \         PP=Personal Pronoun
//  / C_EE C_EE       CJ_EE      |    C=Comma
// / /  \  / \   /   \      |
//PP C PP C PP -CJ ---NC-- main_verb   NC=Noun_Construct CJ=ConJunction
class GrammarPart
{
public:
  BYTE m_byPersonIndex ;
  //The region indexes are important for applying grammar rules:
  // 0     1      2     3  4     <-indexes of tokens
  //The vacuum cleaner is big.
  //      \       /
  //    leftmost: 1, rightmost: 2
  //  \     |
  //aricle noun  <-rule: article + noun = noun_construct
  //
  //The vacuum cleaner
  //+----------------+
  //    noun_construct: leftmost: 0, rightmost:2
  //
  DWORD m_dwLeftmostIndex ;
  DWORD m_dwRightmostIndex ;
  //for translation: refer to the subordinate elements.
  //e.g. when parsing: "the man" , rule: article + noun = noun_construct:
  //noun_construct->leftchild = article
  //noun_construct->rightchild = noun
  GrammarPart * mp_grammarpartLeftChild ;
  GrammarPart * mp_grammarpartRightChild ;
//  //Use strings to show grammar parts in the user interface as a feature.
//  std::string m_stdstrGrammarPartName ;
  //Important for translating into the destination language.
  VocabularyAndTranslation * m_pvocabularyandtranslation ;
  //Additionally use numbers corresponding to the rule name because they can
  //be compared faster than strings.
  WORD m_wGrammarPartID ;

  void AddLeftChild(GrammarPart & r_grammarpart)
  {
    mp_grammarpartLeftChild = & r_grammarpart ;
  }
  void AddRightChild(GrammarPart & r_grammarpart)
  {
    mp_grammarpartRightChild = & r_grammarpart ;
  }

  void BuildTokenVector(
    const std::string & stdstrText
//    , PositionstdstringVector & psv
    , PositionStringVector & psv
    ) ;

//  std::string GetGrammarPartName()
//  {
//    return
//  }

  GrammarPart(DWORD dwTokenIndexLeftMost, DWORD dwTokenIndexRightMost )
  {
    Init() ;
    m_dwLeftmostIndex = dwTokenIndexLeftMost ;
    m_dwRightmostIndex = dwTokenIndexRightMost ;
  }

  GrammarPart(DWORD dwTokenIndexLeftMost, DWORD dwTokenIndexRightMost ,
    WORD wGrammarPartID )
  {
    Init() ;
    m_dwLeftmostIndex = dwTokenIndexLeftMost ;
    m_dwRightmostIndex = dwTokenIndexRightMost ;
    m_wGrammarPartID = wGrammarPartID ;
  }

  void Init()
  {
    m_byPersonIndex = 0 ;
    mp_grammarpartLeftChild = NULL ;
    mp_grammarpartRightChild = NULL ;
  }

  //Define a < operator in order to insert into a container like std::set.
  bool operator < (const GrammarPart & ) const ;

  void setGrammarPartID(WORD wGrammarPartID )
  {
    m_wGrammarPartID = wGrammarPartID ;
  }
};

class GrammarRule
{
public:
  //e.g. grammar part ID for "the" and for "noun" refer to the same
  //superordinate grammar part ID for "noun_construct".
  WORD m_wSuperordinateGrammarRuleID ;
  WORD m_wGrammarRuleID ;
  GrammarRule( WORD wGrammarRuleID , WORD wSuperordinateGrammarRuleID )
  {
    m_wGrammarRuleID = wGrammarRuleID ;
    m_wSuperordinateGrammarRuleID = wSuperordinateGrammarRuleID ;
  }
} ;

class ParseByRise
{
//private:
public:
  //This list stores the leftmost indices of the grammar parts:
  // initially only word classes are grammar parts:
  //   0    1       2      3      <- token index
  // "the vacuum cleaner sucks".
  // -> insert into the container:
  //  "<0,grammar part for "the">
  //  "<1,grammar part for "vacuum cleaner">
  //  "<3,grammar part for "sucks">
  // afterwards after grammar rules have been applied, the list becomes the
  //   leftmost indices of the superordinate grammar parts:
  //  insert:
  //   "<0,grammar part "noun_construct" for "the vacuum cleaner">
  //   "<0,grammar part "clause" for "the vacuum cleaner sucks">
  std::multimap<DWORD, GrammarPart> m_stdmultimap_dwLeftmostIndex2grammarpart ;
  //This list stores the rightmost indices of the grammar parts:
  // initially only word classes are grammar parts:
  //   0    1       2      3      <- token index
  // "the vacuum cleaner sucks".
  // -> insert into the container:
  //  "<0,grammar part for "the">
  //  "<2,grammar part for "vacuum cleaner">
  //  "<3,grammar part for "sucks">
  // afterwards after grammar rules have been applied, the list becomes the
  //   rightmost indices of the superordinate grammar parts:
  //  insert:
  //   "<2,grammar part "noun_construct" for "the vacuum cleaner">
  //   "<3,grammar part "clause" for "the vacuum cleaner sucks">
  std::multimap<DWORD, GrammarPart> m_stdmultimap_dwRightmostIndex2grammarpart ;
  std::multimap<WORD, WORD> m_stdmultimap_wGrammarPartID2wGrammarPartID ;
  std::multimap<WORD, WORD> m_stdmultimap_wGrammarPartID2SuperordinateID ;
  std::multimap<WORD, GrammarRule>
    m_stdmultimap_wGrammarPartID2SuperordinateGrammarRule ;
  std::map<WORD, std::string> m_stdmap_wRuleID2RuleName ;
  std::map<std::string,WORD> m_stdmap_RuleName2RuleID ;

  std::map<WORD, std::multimap<WORD, GrammarRule> >
    m_stdmap_wParseLevelIndex2stdmultimap_dwLeftmostIndex2grammarpart ;
  std::map<WORD, std::multimap<WORD, GrammarRule> >::iterator
    m_iter_stdmap_wParseLevelIndex2stdmultimap_dwLeftmostIndex2grammarpart ;
  WORD m_wNumberOfSuperordinateRules ;
  std::map<WORD, std::multimap<WORD, GrammarRule> >
    m_stdmap_wParseLevelIndex2stdmultimap_dwRightmostIndex2grammarpart ;
  std::map<WORD, std::multimap<WORD, GrammarRule> >::iterator
    m_iter_stdmap_wParseLevelIndex2stdmultimap_dwRightmostIndex2grammarpart ;
  typedef std::multimap<DWORD, GrammarPart>::const_iterator
    c_iter_mmap_dw2grammarpart ;

  std::multimap<DWORD, GrammarPart> *
    mp_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate ;
  std::multimap<DWORD, GrammarPart> *
    mp_stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;
  DWORD m_dwMapIndex ;
//  PositionstdstringVector m_psv ;
  PositionStringVector m_psv ;
  //Memorize the applied rules to enable a parse break condition.
  //TODO determining if a rule was applied yet by using a std::set is fast but
  // I does not consider that for a grammar part multiple rules may exist:
  //  -grammar part "the" + grammar part "noun" = noun_construct.
  //  -grammar part "the" + grammar part "comperative" ("the faster the better")
  //    = the_comperative
  std::set<GrammarPart * > m_stdset_p_grammarpartAllSuperordinate ;
  //---
  //Better store the rule ID/ grammar part ID of applied rules in it:
  // to check that the rules aren't already contained for the next parse level:
  //e.g. 1st parse level:
  // "the vacuum cleaner sucks": add rule "noun_construct" for
  //   "the vacuum cleaner" ("definite article" + "noun")
  // 2nd and later parse levels: rule "noun_construct" already contained for
  //  leftmost index 0, rightmost index 2?: yes -> do not add to the list of
  // ALL grammar parts (that contains word classes plus applied rules)
  std::set<GrammarPart> m_stdset_grammarpartAllSuperordinate ;

  //serves as a break condition so that is known that no more
  WORD m_wBiggestNumberOfTokensForAppliedGrammarRule ;
  WORD m_wNumberOfTokensForAppliedGrammarRule ;
public:
  //Clears (empties) the previously generated parse tree.
  //This should be done for a next parse tree generation .
  void ClearParseTree() ;

  void CreateInitialGrammarParts (std::string & stdstrText ) ;

  GrammarPart * GetGrammarPartCoveringMostTokens(
    DWORD dwLeftMostTokenIndex ) ;

  bool GetGrammarPartID( const std::string & r_str , WORD & wID )
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

  std::string GetGrammarPartName(WORD wRuleID ) //;
  {
    std::string stdstrRuleName ;
    std::map<WORD,std::string>::const_iterator iter =
      m_stdmap_wRuleID2RuleName.find( wRuleID ) ;
    if( iter != m_stdmap_wRuleID2RuleName.end() )
      return iter->second ;
    return std::string("") ;
  }

  //return: bitfield: 1 bit for every single person Index (1st person singular...3rd
  //persion  plural)
  BYTE GetSubjectPersonIndex( GrammarPart * p_grammarpart) ;

  void GetTokensAsSpaceSeparatedString(
    DWORD dwLeftmostIndex,
    DWORD dwRightmostIndex ,
    std::string & r_stdstr ) ;
  bool GrammarPartIDIsWordClass( WORD wGrammarPartID ) ;

  void InitGrammar() ;

  void InsertFundamentalRuleIDs() ;
  
  void InsertGrammarRule(
    WORD wGrammarRuleIDLeft
    , WORD wGrammarRuleIDRight
    , //std::string
    const char * cp_ch
    ) ;
  void InsertGrammarRule(
    WORD wGrammarRuleIDLeft ,
    const char * cp_chRightGrammarRuleName
    , //std::string
    const char * cp_chSuperordinateGrammarRuleName
    ) ;
  WORD InsertGrammarRule(
    const char * cp_chLeftGrammarRuleName
    , WORD wGrammarRuleIDRight
    , //std::string
    const char * cp_ch
    ) ;
  void InsertGrammarRule(WORD wGrammarRuleID
    , const char * cp_ch ) ;
  void InsertGrammarRule(
    const char * cp_chLeftGrammarRuleName
    , const char * cp_chRightGrammarRuleName
    , //std::string
    const char * cp_chSuperordinateGrammarRuleName
    ) ;
  void InsertGrammarRule(
    const char * cp_chLeftGrammarRuleName
    , const char * cp_chRightGrammarRuleName
    , //std::string
    //EXISTING rule / grammar part ID
    WORD wSuperordinateGrammarRuleID ) ;
  void InsertRuleIDsForWordClasses() ;
  void InsertRuleID2NameMapping( WORD wGrammarRuleID
      , const char * cp_ch ) ;
  ParseByRise();
  ParseByRise(const ParseByRise& orig);
  bool InsertIfGrammarRuleAppliesTo(
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
//    std::map<DWORD, GrammarPart> & r_stdmapwLeftmostIndex2grammarpart
//    , std::map<DWORD, GrammarPart> & r_stdmapwRightmostIndex2grammarpart
//    WORD wLeftGrammarPartIDForRule
//    , WORD wRightGrammarPartIDForRule
    std::multimap<DWORD, GrammarPart>::iterator iterLeftGrammarPart
    , std::multimap<DWORD, GrammarPart>::iterator iterRightGrammarPart
  ) ;
//  void InsertIntoPossibilityTree(
//  //  std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator & iter
//  //  , std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator &
//  //    iterRightOf
//    std::multimap<DWORD, GrammarPart>::iterator iterLeftGrammarPart
//    , std::multimap<DWORD, GrammarPart>::iterator iterRightGrammarPart
//    , WORD wGrammarPartIDOfRule
//    ) ;
  void ResolveGrammarRules(
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

//    //Use a multimap because at an index x more than 1 grammar part may exist.
//    //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
//    //"verb".
    std::multimap<DWORD, GrammarPart> &
      r_stdmultimap_dwLeftmostIndex2grammarpart
    , std::multimap<DWORD, GrammarPart> &
      r_stdmultimap_dwRightmostIndex2grammarpart
    , std::multimap<DWORD, GrammarPart> &
      r_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate
    , std::multimap<DWORD, GrammarPart> &
      r_stdmultimap_dwRightmostIndex2grammarpartSuperordinate
    ) ;

  void ResolveGrammarRulesForAllParseLevels() ;

  void StoreWordTypeAndGermanTranslation(
//    PositionstdstringVector & psv
    PositionStringVector & psv
    , DWORD wTokenIndex
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
//    , std::map<DWORD, GrammarPart> & r_stdmapwLeftmostIndex2grammarpart
//    , std::map<DWORD, GrammarPart> & r_stdmapwRightmostIndex2grammarpart
    //Use a multimap because at an index x more than 1 grammar part may exist.
    //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
    //"verb".
//    , std::multimap<DWORD, GrammarPart> & r_stdmultimap_wLeftmostIndex2grammarpart
//    , std::multimap<DWORD, GrammarPart> & r_stdmultimap_wRightmostIndex2grammarpart
    ) ;
  virtual ~ParseByRise();
private:

};

#endif	/* _PARSEBYRISE_HPP */

