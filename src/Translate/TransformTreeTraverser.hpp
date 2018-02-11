/*
 * TransformTreeTraverser.hpp
 *
 *  Created on: Dec 20, 2010
 *      Author: Stefan
 */

#ifndef TRANSFORMTREETRANSVERSER_HPP_
#define TRANSFORMTREETRANSVERSER_HPP_

//Base class DirectingLeavesMultipleIterTraverser
#include <ParseTree/DirectingLeavesMultipleIterTraverser.hpp>//base class
//class ParseTreeTraverser::InsertIntoTreeTransverser
#include <Translate/InsertIntoTreeTransverser.hpp>
#include <Translate/SyntaxTreePath.hpp> //class SyntaxTreePath
#include <Translate/TransformationRule.hpp> //class TransformationRule

#include <map> //class std::map
#include <string> //class std::strings

/** Forward declarations (faster than to include files). */
class GrammarPart ;
namespace VTrans3 { class BottomUpParser; }

namespace ParseTreeTraverser
{
  /** A parse tree traverser implementation that is able to move subtrees of a
   * parse tree. This is necessary when the word order between English and
   * German differs, e.g. at "When I see you." (Subject Predicate Object) ->
   * "Wenn ich dich sehe" ( Subject Object Predicate).
   */
  class TransformTreeTraverser
    : public DirectingLeavesMultipleIterTraverser
  {
  public:
    bool m_bMoveParseTreeBranch ;
    const GrammarPart * m_p_grammarpartStartNode ;
//    const std::map<std::string,TransformationRule> &
//      m_stdmap_stdstrTransformationRule2transformationrule ;
//    const std::map<WORD *,TransformationRule> &
//      m_stdmap_ar_wTransformationRuleParseTreePath2transformationrule ;
    const std::map<SyntaxTreePath,TransformationRule> &
      m_stdmap_syntaxtreepath2transformationrule ;
    VTrans3::BottomUpParser & m_r_bottomUpParser;
    std::vector<GrammarPart *> m_stdvector_p_grammarpartCurrentParseTreePath;

    void BeforeBeginAtRoot() ;
    void CurrentNodeIsLastAddedRightChild() ;
    GrammarPart * FindParseTreePathWhereToInsert(
//      const TransformationRule & cr_transformationrule ,
//      bNodeToBeChildIsLeftChild
      //const
      ParseTreeTraverser::InsertIntoTreeTransverser &
        cr_insertintotreetransverser
      ) ;
    void ExchangeSyntaxTreePathes(
      BYTE byDirectionOfGrammarPartToMoveOrToExchange,
      GrammarPart * p_grammarpartParentOfGrammarPartToMoveOrToExchange,
      GrammarPart * p_grammarpartBranchToMoveOrToExchange,
//      GrammarPart * p_grammarpartParentOfGrammarPartToMoveOrToExchange
      ParseTreeTraverser::InsertIntoTreeTransverser & r_insertintotreetransverser
      );
    GrammarPart * Get2ndLastGrammarPart() ;
    BYTE GetGrammarPartToMoveOrToExchange(
      GrammarPart * p_grammarpartParentOfGrammarPartToMove ,
      GrammarPart * & p_r_grammarpartBranchToMove
      ) ;
    //Overrides/ implements base class' method.
    void LeaveFound() ;
    void MoveParseTreeBranch(
      ParseTreeTraverser::InsertIntoTreeTransverser & insertintotreetransverser,
      GrammarPart * p_grammarpartParentOfGrammarPartToMove ,
      const TransformationRule & c_r_transformationrule
      ) ;
    void MoveOrExchangeParseTreePath(
      const TransformationRule & c_r_transformationrule);
    void ParseTreePathAdded() ;
    void ParseTreePathPopped() ;
    void PossiblyMoveParseTreeBranch();
    void RightChildAdded( unsigned short) ;
    void SetChildNodeAtGrammarPartToMoveToNULL(
      GrammarPart * p_grammarpartParentOfGrammarPartToMove) ;
    TransformTreeTraverser(
//      const std::map<std::string,TransformationRule> & ,
      const std::map<SyntaxTreePath,TransformationRule> &
        cr_stdmap_syntaxtreepath2transformationrule ,
      const GrammarPart * p_grammarpartStartNode ,
      VTrans3::BottomUpParser &
      );
    virtual
    ~TransformTreeTraverser();
    void UnprocessedHighestLevelNodeFound() ;
  };

}

#endif /* TRANSFORMTREETRANSVERSER_HPP_ */
