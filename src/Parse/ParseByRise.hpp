/** File:   ParseByRise.hpp
 * Author: Stefan
 * Created on 13. Februar 2010, 21:05  */

#ifndef _PARSEBYRISE_HPP
#define	_PARSEBYRISE_HPP

//#include <windef.h> //for DWORD
#include <Windows.h> //includes windef.h;for DWORD, WORD
#include <string> //class std::string
#include <map> //class std::map
#include <set> //class std::set
#include <Attributes/Token.h> //class PositionStringVector
#include "GrammarPart.hpp" //class GrammarPart
#include <hardware/CPU/fastest_data_type.h> // typedef fastestUnsignedDataType
#include <Controller/DictReaderAndVocAccess/dictReaderAndVocAccess_type.hpp>
#include "GrammarRule.hpp" //class GrammarRule
#include <data_structures/ByteArray.hpp>//class ByteArray

/** Forward declarations (faster than #include) */
class LetterNode;
class I_UserInterface ;
class TranslationControllerBase;
class VocabularyAndTranslation ;
class TranslationProcess;

namespace VTrans3
{
/** @brief According to the book "Computerlinguistik und Sprachtechnologie" 
 *  Eine Einführung, "2. überarbeitete und erweiterte Auflage", 
 *  ISBN 3-8274-1407-5, page 254
    this is a bottum-up parser 
 *  See also https://en.wikipedia.org/wiki/Bottom-up_parsing */
class BottomUpParser
{
//  TranslationControllerBase & m_r_translationcontrollerbase;
#ifndef TEST_MINI_XML
#endif //TEST_MINI_XML
public:
  /** The */
  /*static*/ dictReaderAndVocAccess_type s_dictReaderAndVocAccess;
  DWORD m_dwMapIndex ;
//private:
  //For showing (XML) error messages.
//  I_UserInterface & mr_userinterface ;
  I_UserInterface * m_p_userinterface ;
  TranslationProcess & m_r_translationProcess;
public:
  void GenerateXMLtreeFromParseTree(
    std::vector<GrammarPart *>::const_iterator
      c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex,
    /*std::string & std_strXML*/ ByteArray & byteArray
    ) const;
  void GenerateXMLtree(
    /*std::string & std_strXML*/ ByteArray & byteArray) const;
  enum InsertGrammarRuleReturnCodes
  {
    AllGrammarPartsAreKnown = 0,
    unknownLeftGrammarPart,
    unknownRightGrammarPart,
    unknownLeftAndRightGrammarPart
  };
  /* This map is for generalisation (grammar rule x IS A superclass grammar
  * rule y ) in order to minimize the grammar rules needed:
  * so we can define
  *  -"noun IS A subj_or_obj_enum_ele"
  *  -"gerund is a subj_or_obj_enum_ele"
  *  so we only need 1 grammar rule: "subj_or_obj_enum_ele" + "verb" = clause
  *  instead of 2 grammar rules:
  *   -"noun" + "verb" = clause
  *   -"gerund" + "verb" = clause */
  std::map<WORD,WORD> m_stdmap_wGrammarPartID2SuperordinateID ;
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
  //   "<0,grammar part "def_article_noun" for "the vacuum cleaner">
  //   "<0,grammar part "clause" for "the vacuum cleaner sucks">
//  std::multimap<DWORD, GrammarPart> m_stdmultimap_dwLeftmostIndex2grammarpart ;
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
  //   "<2,grammar part "def_article_noun" for "the vacuum cleaner">
  //   "<3,grammar part "clause" for "the vacuum cleaner sucks">
//  std::multimap<DWORD, GrammarPart> m_stdmultimap_dwRightmostIndex2grammarpart ;
  std::multimap<DWORD, GrammarPart *>
    m_stdmultimap_dwLeftmostIndex2p_grammarpart ;
  /** Contains the roots of the parse trees for a distinct token index. 
    * More than 1 parse tree may gave its right end at the sane token index even
    * after the Part-Of-Speech (=POS) tagging phase: e.g. for "love" the noun
    * love and the verb love are at the same index.
    * This is why this is a _multi_map. */
  std::multimap<DWORD, GrammarPart *>
    m_stdmultimap_dwRightmostIndex2p_grammarpart ;
  /** Mapping from left grammar rule ID to right grammar rule ID.*/
  std::multimap<WORD, WORD> m_stdmultimap_wGrammarPartID2wGrammarPartID ;
  std::multimap<WORD, WORD> m_stdmultimap_wGrammarPartID2SuperordinateID ;
  std::multimap<WORD, GrammarRule>
    m_stdmmap_wLeftChildGrammarPartID2SuperordinateGrammarRule ;
//  std::multimap<DWORD, GrammarPart *>
//    m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate ;
//  std::multimap<DWORD, GrammarPart* >
//    m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate ;
  std::multimap<DWORD, GrammarPart *>
    m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel ;
  std::multimap<DWORD, GrammarPart* >
    m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel ;

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
  typedef std::multimap<DWORD, GrammarPart *>::const_iterator
    c_iter_mmap_dw2p_grammarpart ;

//  std::multimap<DWORD, GrammarPart> *
//    mp_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate ;
//  std::multimap<DWORD, GrammarPart> *
//    mp_stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;
//  PositionstdstringVector m_psv ;
  PositionStringVector m_psv ;
  //Memorize the applied rules to enable a parse break condition.
  //TODO determining if a rule was applied yet by using a std::set is fast but
  // I does not consider that for a grammar part multiple rules may exist:
  //  -grammar part "the" + grammar part "noun" = def_article_noun.
  //  -grammar part "the" + grammar part "comperative" ("the faster the better")
  //    = the_comperative
  std::set<GrammarPart * > m_stdset_p_grammarpartAllSuperordinate ;
  //---
  //Better store the rule ID/ grammar part ID of applied rules in it:
  // to check that the rules aren't already contained for the next parse level:
  //e.g. 1st parse level:
  // "the vacuum cleaner sucks": add rule "def_article_noun" for
  //   "the vacuum cleaner" ("definite article" + "noun")
  // 2nd and later parse levels: rule "def_article_noun" already contained for
  //  leftmost index 0, rightmost index 2?: yes -> do not add to the list of
  // ALL grammar parts (that contains word classes plus applied rules)

  //-Elements _must_ be objects (->not pointers) for "GrammarPart::operator <"
  //  in std::set::find( ...) ?!
  std::set<GrammarPart> m_stdset_grammarpartAllSuperordinate ;
  std::map<GrammarPart, GrammarPart *> m_allSuperordinateGrammarParts2pointerToThem ;

  //serves as a break condition so that is known that no more
  WORD m_wBiggestNumberOfTokensForAppliedGrammarRule ;
  WORD m_wNumberOfTokensForAppliedGrammarRule ;
  /** The hierarchy level (=maximum height of all parse trees) of (number of times grammar/parse rules have been applied).
    * Is incremented at the end of function "ResolveGrammarRules" .*/
  WORD m_wParseLevel ;
  PositionStringVector * m_pInputText;
public:
  //For the re-init of grammar to be like at the first init all grammar data
  // (grammar ID<-->grammar name mapping, grammar rules, ...
  //must be deleted first.
  void ClearAllGrammarStuff() ;
  /** Clears (empties) the previously generated parse tree.
  * This should be done for a next parse tree generation. */
  void ClearParseTree() ;

  void CreateInitialGrammarParts ( const std::string & cr_stdstrText ) ;
  void CreateParseTree(const std::string & cr_stdstrWholeInputText);

  void DeleteFromOutMostTokenIndexContainer(
    GrammarPart * p_grammarpartRootNode,
    std::multimap<DWORD, GrammarPart *> & r_std_multimap_dw2p_grammarpart,
    DWORD dwTokenIndex
    );
  //TODO possibly also pass left, right child (and superordinate)
  std::string GetErrorMessage(const enum InsertGrammarRuleReturnCodes );
  fastestUnsignedDataType GetNumberOfOverallParseTrees() const {
    return m_stdmultimap_dwLeftmostIndex2p_grammarpart.size(); }
  GrammarPart * GetParseTreeCoveringMostTokens(
    DWORD dwLeftMostTokenIndex ) ;
  void GetGrammarPartCoveringMostTokens(
    DWORD dwLeftmostTokenIndex
    , std::vector<GrammarPart *> & r_stdvec_p_grammarpart
    ) const;

  bool GrammarPartDoesNotAlreadyExist(GrammarPart * p_grammarpart);
  bool GetGrammarPartID( const std::string & r_str , WORD & wID ) ;

  std::string GetGrammarPartName(WORD wRuleID ) const;

  fastestUnsignedDataType GetNumberOfLargestParseTrees();
  void GetLargestParseTrees(std::vector<GrammarPart *> &
    stdvec_p_grammarpartCoveringMostTokensAtTokenIndex);
  std::string GetPathAs_std_string( const std::vector<WORD> & ) ;
  std::string GetPathAs_std_string(
    const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath ) ;

  std::string GetAsIndentedXML() const;

  void GetRuleNames(
    WORD wSuperordinateGrammarPartID,
    GrammarPart * p_grammarpartLeft,
    GrammarPart * p_grammarpartRight
    );
  //return: bitfield: 1 bit for every single person Index (1st person singular...3rd
  //persion  plural)
  BYTE GetSubjectPersonIndex( GrammarPart * p_grammarpart) ;

  void GetTokensAsSpaceSeparatedString(
    DWORD dwLeftmostIndex,
    DWORD dwRightmostIndex ,
    std::string & r_stdstr ) ;
  bool GrammarPartIDIsWordClass( WORD wGrammarPartID ) ;

  void Init();
  void InitGrammar() ;
  void InitGrammarRules() ;

  inline void InsertGrammarRuleNamesForAuxiliaryVerbToBe();
  inline void InsertGrammarRuleNamesForObjectFormPersonalPronouns();
  inline void InsertGrammarRuleNamesForPersonalPronouns();
  inline void InsertGrammarRuleNamesForPossessiveAdjectives();
  inline void InsertGrammarRuleNamesForReflexivePronouns();

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
  enum InsertGrammarRuleReturnCodes InsertGrammarRule(
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

  void InsertGrammarPartForEverySameWord(
//    const LetterNode * p_letternode,
    std::set<VocabularyAndTranslation *> * std_set_p_vocabularyandtranslation,
    DWORD dwTokenIndex, DWORD dwTokenIndexRightMost);
  void InsertGrammarRulesFor3rdPersonSingular() ;
  void InsertGrammarPartID2NameMappingForWordClasses() ;
  void InsertIntoOverallLeftmostAndRightmostTokenIndexContainers(
    const std::multimap<DWORD, GrammarPart* > & std_multimap_dwLeftmostIndex2p_grammarpart,
    const std::multimap<DWORD, GrammarPart* > & std_multimap_dwRightmostIndex2p_grammarpart
    );
  void InsertIntoLeftmostAndRightmostTokenIndexContainersFor1ParseLevel(
    const fastestUnsignedDataType leftMostTokenIndex,
    const fastestUnsignedDataType rightMostTokenIndex,
    const GrammarPart * const p_grammarPart);
  inline void InsertIntoOutmostTokenIndexMaps(
    GrammarPart * p_grammarpart,
    DWORD dwLeftMostTokenIndexOfRule,
    DWORD dwRightMostTokenIndexOfRule
    );
  void InsertRule_ID2NameAndName2IDmapping( WORD wGrammarRuleID
      , const char * cp_ch ) ;
  WORD InsertSuperClassGrammarRule(
    WORD wSubclassGrammarRuleID
    , //std::string
    const char * cp_chSuperordinateGrammarRuleName
    ) ;
//  WORD
  enum InsertGrammarRuleReturnCodes InsertSuperClassGrammarRule(
    const char * cp_chSubclassGrammarRuleName
    , //std::string
    const char * cp_chSuperclassGrammarRuleName
    ) ;
//  ParseByRise() ;
//  ParseByRise( I_UserInterface & r_userinterface );
  BottomUpParser( /*TranslationControllerBase & r_translationcontrollerbase*/
    I_UserInterface * p_userinterface, TranslationProcess & );
//  ParseByRise(const ParseByRise& orig);
  /*bool*/ GrammarPart * InsertIntoSuperordinateGrammarPartContainer(
    GrammarPart * p_grammarPartToInsert);
  bool InsertIfGrammarRuleAppliesTo(
    //Maintaining 2 maps with both leftnost and rightmost indexes should be faster
    //when searching for neighboured grammar parts:
    //  0    1       2      3      <-index
    //"the vacuum cleaner sucks."
    // +----------------+
    //   e.g. current grammar part: rightmost index: 2: find neighbour with
    //     leftmost index "3".
//    std::multimap<DWORD, GrammarPart>::iterator iterLeftGrammarPart
//    , std::multimap<DWORD, GrammarPart>::iterator iterRightGrammarPart
    std::multimap<DWORD, GrammarPart *>::iterator iterLeftGrammarPart
    , std::multimap<DWORD, GrammarPart *>::iterator iterRightGrammarPart
  ) ;
//  void InsertIntoPossibilityTree(
//  //  std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator & iter
//  //  , std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator &
//  //    iterRightOf
//    std::multimap<DWORD, GrammarPart>::iterator iterLeftGrammarPart
//    , std::multimap<DWORD, GrammarPart>::iterator iterRightGrammarPart
//    , WORD wGrammarPartIDOfRule
//    ) ;
  void OutputSuperordinateGrammarParts();
  bool InsertSuperordinateGrammarPart(
//    std::multimap<DWORD, GrammarPart> & mm_idx2grammarpt
    std::multimap<DWORD, GrammarPart *> & mm_idx2grammarpt
    , bool bMemorizeInsertion
    ) ;
  bool PossiblyDuplicateSubTrees(
    GrammarPart * const p_grammarpartParent,
    GrammarPart * const p_grammarpartLeftChild,
    GrammarPart * const p_grammarpartRightChild
    );
  inline void PossiblyCreateNewLeaves(
    GrammarPart * p_grammarpart,
    GrammarPart * p_grammarpartLeftChild,
    GrammarPart * p_grammarpartRightChild
    );
  void Message(const std::wstring & cr_stdwstr ) ;
  bool Resolve1ParseLevel();
  BYTE ResolveGrammarRules(
    //Maintaining 2 maps with both leftnost and rightmost indexes should be faster
    //when searching for neighboured grammar parts:
    //  0    1       2      3      <-index
    //"the vacuum cleaner sucks."
    // +----------------+
    //   e.g. current grammar part: rightmost index: 2: find neighbour with
    //     leftmost index "3".

//    //Use a multimap because at an index x more than 1 grammar part may exist.
//    //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
//    //"verb".
//    std::multimap<DWORD, GrammarPart> &
//      r_stdmultimap_dwLeftmostIndex2grammarpart
//    , std::multimap<DWORD, GrammarPart> &
//      r_stdmultimap_dwRightmostIndex2grammarpart
//    , std::multimap<DWORD, GrammarPart> &
//      r_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate
//    , std::multimap<DWORD, GrammarPart> &
//      r_stdmultimap_dwRightmostIndex2grammarpartSuperordinate
    ) ;

  void RemoveParseTree(GrammarPart * p_grammarpartRootNode);
  void RemoveSuperordinateRulesFromRootNodes();
  bool ReplaceGrammarPartIDsBySuperordinate() ;
  void ResolveGrammarRulesForAllParseLevels() ;
  void SetUserInterface( I_UserInterface * p_userinterface)
  {
    m_p_userinterface = p_userinterface ;
  }
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
  void RemoveAllBetweenInRightIndexMap(
      std::multimap<DWORD, GrammarPart *>::iterator & iterFirstUnknownWord,
      std::multimap<DWORD, GrammarPart *>::iterator & iterLastUnknownWord);
  void PossiblySumUp(
    std::multimap<DWORD, GrammarPart *>::iterator & iterFirstUnknownWord,
    std::multimap<DWORD, GrammarPart *>::iterator & iterLastUnknownWord);
  void DeleteFromMultiMap(
    std::multimap<DWORD, GrammarPart *> & std_multimap_dwIndex2p_grammarpart);
  void SummarizeUnknownWords();
  virtual ~BottomUpParser();
private:
};
}

#endif	/* _PARSEBYRISE_HPP */

