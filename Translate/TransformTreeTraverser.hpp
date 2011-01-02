/*
 * TransformTreeTraverser.hpp
 *
 *  Created on: Dec 20, 2010
 *      Author: Stefan
 */

#ifndef TRANSFORMTREETRANSVERSER_HPP_
#define TRANSFORMTREETRANSVERSER_HPP_

//Base class DirectingLeavesMultipleIterTraverser
#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>
//class ParseTreeTraverser::InsertIntoTreeTransverser
#include <Translate/InsertIntoTreeTransverser.hpp>
#include <Translate/SyntaxTreePath.hpp> //class SyntaxTreePath
#include <Translate/TransformationRule.hpp> //class TransformationRule

#include <map> //class std::map
#include <string> //class std::strings

//Forward declarations.
class GrammarPart ;
class ParseByRise ;

namespace ParseTreeTraverser
{

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
    ParseByRise & m_r_parsebyrise ;
    std::vector<GrammarPart *>  m_stdvector_p_grammarpartCurrentParseTreePath;
    void BeforeBeginAtRoot() ;
    void CurrentNodeIsLastAddedRightChild() ;
    GrammarPart * FindParseTreePathWhereToInsert(
//      const TransformationRule & cr_transformationrule ,
//      bNodeToBeChildIsLeftChild
      //const
      ParseTreeTraverser::InsertIntoTreeTransverser &
        cr_insertintotreetransverser
      ) ;
    GrammarPart * Get2ndLastGrammarPart() ;
    void GetGrammarPartToMove(
      GrammarPart * p_grammarpartParentOfGrammarPartToMove ,
      GrammarPart * & p_r_grammarpartBranchToMove
      ) ;
    void LeaveFound() ;
    void MoveParseTreeBranch(
      ParseTreeTraverser::InsertIntoTreeTransverser & insertintotreetransverser,
      GrammarPart * p_grammarpartParentOfGrammarPartToMove ,
      const TransformationRule & r_transformationrule
      ) ;
    void ParseTreePathAdded() ;
    void ParseTreePathPopped() ;
    void RightChildAdded( unsigned short) ;
    void SetChildNodeAtGrammarPartToMoveToNULL(
      GrammarPart * p_grammarpartParentOfGrammarPartToMove) ;
    TransformTreeTraverser(
//      const std::map<std::string,TransformationRule> & ,
      const std::map<SyntaxTreePath,TransformationRule> &
        cr_stdmap_syntaxtreepath2transformationrule ,
      const GrammarPart * p_grammarpartStartNode ,
      ParseByRise & r_parsebyrise
      );
    virtual
    ~TransformTreeTraverser();
    void UnprocessedHighestLevelNodeFound() ;
  };

}

#endif /* TRANSFORMTREETRANSVERSER_HPP_ */
