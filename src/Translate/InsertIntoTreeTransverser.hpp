/** InsertIntoTreeTransverser.hpp
 *  Created on: Dec 21, 2010
 *      Author: Stefan  */

#ifndef INSERTINTOTREETRANSVERSER_HPP_
#define INSERTINTOTREETRANSVERSER_HPP_

//Base class DirectingLeavesMultipleIterTraverser
#include <ParseTree/DirectingLeavesMultipleIterTraverser.hpp>
#include <Translate/SyntaxTreePath.hpp> //class SyntaxTreePath

//Forward declarations.
class GrammarPart ;
namespace VTrans3 { class BottomUpParser; }
class TransformationRule ;

namespace ParseTreeTraverser
{

  class InsertIntoTreeTransverser
    : public DirectingLeavesMultipleIterTraverser
  {
  public:
    BYTE m_byDirection ;
    const TransformationRule & m_cr_transformationrule ;
    VTrans3::BottomUpParser & m_r_bottomUpParser;
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
      VTrans3::BottomUpParser &
      );
    InsertIntoTreeTransverser(
      const TransformationRule & cr_transformationrule ,
//      const std::string & c_r_stdstrParseTreePath,
      const SyntaxTreePath & c_r_syntaxtreepath,
      const GrammarPart * p_grammarpartStartNode ,
      VTrans3::BottomUpParser &
      );
    virtual
    ~InsertIntoTreeTransverser();
    void ParseTreePathAdded() ;
    void ParseTreePathPopped() ;
  };

}

#endif /* INSERTINTOTREETRANSVERSER_HPP_ */
