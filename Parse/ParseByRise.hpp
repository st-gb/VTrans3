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

//class GrammarRule
class GrammarPart
{
public:
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
  //Use strings to show grammar parts in the user interface as a feature.
  std::string m_stdstrGrammarPartName ;
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
    mp_grammarpartLeftChild = NULL ;
    mp_grammarpartRightChild = NULL ;
  }

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

class ParseByRise {
//private:
public:
  std::multimap<DWORD, GrammarPart> m_stdmultimap_dwLeftmostIndex2grammarpart ;
  std::multimap<DWORD, GrammarPart> m_stdmultimap_dwRightmostIndex2grammarpart ;
  std::multimap<WORD, WORD> m_stdmultimap_wGrammarPartID2wGrammarPartID ;
  std::multimap<WORD, WORD> m_stdmultimap_wGrammarPartID2SuperordinateID ;
  std::multimap<WORD, GrammarRule>
    m_stdmultimap_wGrammarPartID2SuperordinateGrammarRule ;
  std::map<WORD, std::string> m_stdmap_wRuleID2RuleName ;
  std::map<std::string,WORD> m_stdmap_RuleName2RuleID ;
  std::map<WORD, std::multimap<WORD, GrammarRule> >
    m_mapindex2stdmap_stdmultimap_dwLeftmostIndex2grammarpart ;
  std::map<WORD, std::multimap<WORD, GrammarRule> >::iterator
    m_iter_mapindex2stdmap_stdmultimap_dwLeftmostIndex2grammarpart ;
  WORD m_wNumberOfSuperordinateRules ;
  std::map<WORD, std::multimap<WORD, GrammarRule> >
    m_mapindex2stdmap_stdmultimap_dwRightmostIndex2grammarpart ;
  std::map<WORD, std::multimap<WORD, GrammarRule> >::iterator
    m_iter_mapindex2stdmap_stdmultimap_dwRightmostIndex2grammarpart ;
  std::multimap<DWORD, GrammarPart> *
    mp_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate ;
  std::multimap<DWORD, GrammarPart> *
    mp_stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;
  DWORD m_dwMapIndex ;
  //Memorize the applied rules to enable a parse break condition.
  //TODO determining if a rule was applied yet by using a std::set is fast but
  // I does not consider that for a grammar part multiple rules may exist:
  //  -grammar part "the" + grammar part "noun" = noun_construct.
  //  -grammar part "the" + grammar part "comperative" ("the faster the better")
  //    = the_comperative
  std::set<GrammarPart * > m_stdset_grammarpartAllSuperordinate ;
public:
  void InitGrammar() ;

  void InsertFundamentalRuleIDs() ;
  
  void InsertGrammarRule(
    WORD wGrammarRuleIDLeft
    , WORD wGrammarRuleIDRight
    , //std::string
    const char * cp_ch
    ) ;
  void InsertGrammarRule(
    const char * cp_chLeftGrammarRuleName
    , WORD wGrammarRuleIDRight
    , //std::string
    const char * cp_ch
    ) ;
  void InsertGrammarRule(WORD wGrammarRuleID
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
  void StoreWordTypeAndGermanTranslation(
    PositionstdstringVector & psv
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

