/*
 * DoTranslateTreeTraverser.hpp
 *
 *  Created on: May 17, 2010
 *      Author: Stefan
 */

#ifndef TRANSLATETREETRAVERSER_HPP_
#define TRANSLATETREETRAVERSER_HPP_

#include <Attributes/TranslationAndGrammarPart.hpp>
//class TranslationAndConsecutiveID
#include <Attributes/TranslationAndConsecutiveID.hpp>
//Base class DirectingLeavesMultipleIterTraverser
#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>
#include <Parse/ParseByRise.hpp>
#include <map> //class std::map
#include <vector> //class std::vector

//Forward declaration (faster than to include)
class ParseByRise ;
class TranslateParseByRiseTree ;

namespace ParseTreeTraverser
{
  class DoTranslateTreeTraverser
    : public DirectingLeavesMultipleIterTraverser
  {
    ParseByRise & m_r_parsebyrise ;
    TranslateParseByRiseTree & mr_translateparsebyrisetree ;
    std::map<GrammarPart *,WORD>
      m_map_grammarpartRightUnprocessedChild2wParseTreeLevel ;
    std::vector<WORD> m_vec_wGrammarPartIDcurrentParsePath ;
    std::vector<GrammarPart *> m_stdvector_p_grammarpartCurrentParseTreePath ;

    //Necessary because these are the direct node neighbours of the predicate.
    // and a translation rule is defined that refers it /
    // to summarize the person index of the subtree of a node with 1 of these
    // IDs.
    WORD m_wSubjectGrammarPartID ;
    WORD m_wArticleSingularGrammarPartID ;
    WORD m_wPluralNounGrammarPartID ;
  public:
    WORD m_wConsecutiveID ;
    std::string m_stdstrWholeTranslation ;
    std::vector<TranslationAndGrammarPart> m_stdvecTranslationAndGrammarPart ;
    std::vector<TranslationAndConsecutiveID>
      m_stdvec_translation_and_consecutive_id ;

    //methods
    DoTranslateTreeTraverser(
      const GrammarPart * p_grammarpartStartNode
      , ParseByRise & r_parsebyrise
      , TranslateParseByRiseTree & r_translateparsebyrisetree
      );
    virtual
      ~DoTranslateTreeTraverser();
    void BeforeBeginAtRoot() ;
    //In order to keep track of the current parse tree path.
    void CurrentNodeIsLastAddedRightChild() ;
    inline void HandlePossibleSubject() ;
    //Callback function from superclass.
    void LeaveFound() ;
    //In order to keep track of the current parse tree path.
    void ParseTreePathAdded() ;
    void ParseTreePathPopped() ;
//    void RightChildAdded(WORD wCurrentParseTreeLevel) ;
    void UnprocessedHighestLevelNodeFound() ;
    void SetSameConsecutiveIDforLeaves(
        const GrammarPart * p_grammarpart) ;
  };
}

#endif /* TRANSLATETREETRAVERSER_HPP_ */