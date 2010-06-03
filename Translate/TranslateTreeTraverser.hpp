/*
 * TranslateTreeTraverser.hpp
 *
 *  Created on: May 17, 2010
 *      Author: Stefan
 */

#ifndef TRANSLATETREETRAVERSER_HPP_
#define TRANSLATETREETRAVERSER_HPP_

#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>
#include <Parse/ParseByRise.hpp>
#include <map>
#include <vector>

//pre-declaration (faster than to include)
class ParseByRise ;
class TranslateParseByRiseTree ;

class TranslationAndGrammarPart
{
public:
  TranslationAndGrammarPart(
    std::string & r_stdstrTranslation ,
    const GrammarPart * p_grammarpart
    )
  {
    m_stdstrTranslation = r_stdstrTranslation ;
    mp_grammarpart = p_grammarpart ;
  }
  std::string m_stdstrTranslation ;
  const GrammarPart * mp_grammarpart ;
} ;

class TranslationAndConsecutiveID
{
public:
  TranslationAndConsecutiveID(
    std::string & r_stdstrTranslation ,
    WORD wConsecutiveID
    )
  {
    m_stdstrTranslation = r_stdstrTranslation ;
    m_wConsecutiveID = wConsecutiveID ;
  }
  std::string m_stdstrTranslation ;
  WORD m_wConsecutiveID ;
} ;

namespace ParseTreeTraverser
{
  class TranslateTreeTraverser
  : public DirectingLeavesMultipleIterTraverser
  {
    TranslateParseByRiseTree & mr_translateparsebyrisetree ;
    std::map<GrammarPart *,WORD>
      m_map_grammarpartRightUnprocessedChild2wParseTreeLevel ;
    std::vector<WORD> m_vec_wGrammarPartIDcurrentParsePath ;
    std::vector<GrammarPart *> m_stdvec_p_grammarpartPath ;

    //Necessary because these are the direct node neighbours of the predicate.
    // and a translation rule is defined that refers it /
    // to summarize the person index of the subtree of a node with 1 of these
    // IDs.
    WORD m_wSubjectGrammarPartID ;
    WORD m_wArticleSingularGrammarPartID ;
    WORD m_wPluralNounGrammarPartID ;
    ParseByRise & mr_parsebyrise ;
  public:
    WORD m_wConsecutiveID ;
    std::string m_stdstrWholeTranslation ;
    std::vector<TranslationAndGrammarPart> m_stdvecTranslationAndGrammarPart ;
    std::vector<TranslationAndConsecutiveID>
      m_stdvec_translation_and_consecutive_id ;
    TranslateTreeTraverser(
      const GrammarPart * p_grammarpartStartNode
      , ParseByRise & r_parsebyrise
      , TranslateParseByRiseTree & r_translateparsebyrisetree
      );
    virtual
      ~TranslateTreeTraverser();
    void BeforeBeginAtRoot() ;
    //In order to keep track of the current parse tree path.
    void CurrentNodeIsLastAddedRightChild() ;
    inline void HandlePossibleSubject() ;
    void LeaveFound() ;
    //In order to keep track of the current parse tree path.
    void ParseTreePathAdded() ;
//    void RightChildAdded(WORD wCurrentParseTreeLevel) ;
    void UnprocessedHighestLevelNodeFound() ;
    void SetSameConsecutiveIDforLeaves(
        const GrammarPart * p_grammarpart) ;
  };
}

#endif /* TRANSLATETREETRAVERSER_HPP_ */