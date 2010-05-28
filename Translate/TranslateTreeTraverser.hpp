/*
 * TranslateTreeTraverser.hpp
 *
 *  Created on: May 17, 2010
 *      Author: Stefan
 */

#ifndef TRANSLATETREETRAVERSER_HPP_
#define TRANSLATETREETRAVERSER_HPP_

#include <Parse/DirectingLeavesMultipleIterTraverser.h>
#include <Parse/ParseByRise.hpp>
#include <map>
#include <vector>

//pre-declaration (faster than to include)
class ParseByRise ;
class TranslateParseByRiseTree ;

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
    std::string m_stdstrWholeTranslation ;
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
  };
}

#endif /* TRANSLATETREETRAVERSER_HPP_ */
