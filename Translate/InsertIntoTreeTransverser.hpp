/*
 * InsertIntoTreeTransverser.hpp
 *
 *  Created on: Dec 21, 2010
 *      Author: Stefan
 */

#ifndef INSERTINTOTREETRANSVERSER_HPP_
#define INSERTINTOTREETRANSVERSER_HPP_

//Base class DirectingLeavesMultipleIterTraverser
#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>
#include <Translate/SyntaxTreePath.hpp> //class SyntaxTreePath

//Forward declarations.
class GrammarPart ;
class ParseByRise ;
class TransformationRule ;

namespace ParseTreeTraverser
{

  class InsertIntoTreeTransverser
    : public DirectingLeavesMultipleIterTraverser
  {
  public:
    BYTE m_byDirection ;
    const TransformationRule & m_cr_transformationrule ;
    ParseByRise & m_r_parsebyrise ;
    GrammarPart * m_p_grammarpartChildOfGrammarPartToInsert ;
    GrammarPart * m_p_grammarpartParentOfGrammarPartToInsert ;
    GrammarPart * m_p_grammarpartParentOfCurrentGrammarPart ;
    std::vector<GrammarPart *> m_stdvector_p_grammarpartCurrentParseTreePath;
    SyntaxTreePath m_syntaxtreepath ;

    void BeforeBeginAtRoot() ;
    void CheckIfGrammarPartPathMatches( BYTE byDirection ) ;
    void CurrentNodeIsLastAddedRightChild() ;
    InsertIntoTreeTransverser(
      const TransformationRule & cr_transformationrule ,
      const GrammarPart * p_grammarpartStartNode ,
      ParseByRise & r_parsebyrise
      );
    virtual
    ~InsertIntoTreeTransverser();
    void ParseTreePathAdded() ;
    void ParseTreePathPopped() ;
  };

}

#endif /* INSERTINTOTREETRANSVERSER_HPP_ */
