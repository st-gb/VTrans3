/** InsertIntoTreeTransverser.cpp
 *  Created on: Dec 21, 2010
 *      Author: Stefan  */
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
#include <Parse/ParseByRise.hpp> //class BottomUpParser
#include <Translate/InsertIntoTreeTransverser.hpp>
#include <Translate/SyntaxTreePath.hpp> //class SyntaxTreePath
#include <Translate/TransformationRule.hpp> //class TransformationRule

namespace ParseTreeTraverser
{
  /** Used in TransformTreeTraverser */
  InsertIntoTreeTransverser::InsertIntoTreeTransverser(
    const TransformationRule & cr_transformationrule ,
    const GrammarPart * p_grammarpartStartNode ,
    VTrans3::BottomUpParser & r_bottomUpParser
    )
    :
    ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
      p_grammarpartStartNode ,
      & r_bottomUpParser
      ) ,
    m_cr_transformationrule ( cr_transformationrule ) ,
    m_r_bottomUpParser( r_bottomUpParser ) ,
    m_p_grammarpartChildOfGrammarPartToInsert( NULL ) ,
    m_p_grammarpartParentOfGrammarPartToInsert( NULL ) ,
    m_syntaxtreepath(
      cr_transformationrule.m_stdstrParseTreePathWhereToInsert,
      & r_bottomUpParser)
  {
    LOGN_DEBUG(//"InsertIntoTreeTransverser(...)--" <<
      cr_transformationrule.m_stdstrParseTreePathWhereToInsert )
//    SyntaxTreePath syntaxtreepath ;
//    syntaxtreepath.CreateGrammarPartIDArray(
//      cr_transformationrule.m_stdstrParseTreePathWhereToInsert,
//      r_parsebyrise
//      ) ;
  }

  InsertIntoTreeTransverser::InsertIntoTreeTransverser(
    const TransformationRule & cr_transformationrule ,
    const SyntaxTreePath & c_r_syntaxtreepath,
    const GrammarPart * p_grammarpartStartNode ,
    VTrans3::BottomUpParser & r_bottomUpParser
    )
    :
    ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
      p_grammarpartStartNode ,
      & r_bottomUpParser
      ) ,
    m_cr_transformationrule ( cr_transformationrule ) ,
    m_r_bottomUpParser( r_bottomUpParser ) ,
    m_p_grammarpartChildOfGrammarPartToInsert( NULL ) ,
    m_p_grammarpartParentOfGrammarPartToInsert( NULL ) ,
    m_syntaxtreepath(
//      c_r_stdstrParseTreePath,
//      & r_parsebyrise
      c_r_syntaxtreepath
      )
  {
    LOGN_DEBUG( //"InsertIntoTreeTransverser(...)--"
      ""
        //FULL_FUNC_NAME
//      << cr_transformationrule.m_stdstrParseTreePathWhereToInsert
        )
//    SyntaxTreePath syntaxtreepath ;
//    syntaxtreepath.CreateGrammarPartIDArray(
//      cr_transformationrule.m_stdstrParseTreePathWhereToInsert,
//      r_parsebyrise
//      ) ;
  }

  InsertIntoTreeTransverser::~InsertIntoTreeTransverser()
  {
    LOGN_DEBUG( //FULL_FUNC_NAME
      ""
      )
  }

  void InsertIntoTreeTransverser::BeforeBeginAtRoot()
  {
    m_stdvector_p_grammarpartCurrentParseTreePath.clear() ;
  }

  void InsertIntoTreeTransverser::CheckIfGrammarPartPathMatches(
    BYTE byDirection )
  {
    std::vector<WORD> stdvec_wGrammarPartID ;
    SyntaxTreePath::GetAsGrammarPartIDvector(
      m_stdvector_p_grammarpartCurrentParseTreePath , stdvec_wGrammarPartID ) ;
    if( m_syntaxtreepath.Matches(
        //ar_wGrammarPartID,
          stdvec_wGrammarPartID )
      )
    {
      LOGN_DEBUG(//"InsertIntoTreeTransverser::ParseTreePathAdded()"
        //FULL_FUNC_NAME << "--"
        "m_syntaxtreepath.Matches(...)" )
      m_p_grammarpartChildOfGrammarPartToInsert =
       m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ;
      LOGN_DEBUG(//"InsertIntoTreeTransverser::CheckIfGrammarPartPathMatches(...)"
        //FULL_FUNC_NAME << "--"
        "m_p_grammarpartChildOfGrammarPartToInsert:"
        << m_r_bottomUpParser.GetGrammarPartName(
          m_p_grammarpartChildOfGrammarPartToInsert->m_wGrammarPartID )
        )
//      m_p_grammarpartParentOfGrammarPartToInsert =
//        m_p_grammarpartParentOfCurrentGrammarPart ;
      if( m_stdvector_p_grammarpartCurrentParseTreePath.size() > 1 )
        m_p_grammarpartParentOfGrammarPartToInsert =
        //2nd last element.
        m_stdvector_p_grammarpartCurrentParseTreePath.at (
          m_stdvector_p_grammarpartCurrentParseTreePath.size() - 2 ) ;
      LOGN_DEBUG(//"InsertIntoTreeTransverser::CheckIfGrammarPartPathMatches(...)"
        //FULL_FUNC_NAME << "--"
        "m_p_grammarpartParentOfGrammarPartToInsert:"
        << m_p_grammarpartParentOfGrammarPartToInsert
        )
      if( m_p_grammarpartParentOfGrammarPartToInsert )
        LOGN_DEBUG(//"InsertIntoTreeTransverser::CheckIfGrammarPartPathMatches(...)"
          //FULL_FUNC_NAME << "--"
          "m_p_grammarpartParentOfGrammarPartToInsert:"
          << m_r_bottomUpParser.GetGrammarPartName(
            m_p_grammarpartParentOfGrammarPartToInsert->m_wGrammarPartID )
          )

      m_byDirection = byDirection ;
      m_bTraverseTree = false ;
      //End loop in super class
      //DirectingLeavesMultipleIterTraverser::ProcessLeavesOfParseTree()
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart = NULL ;
    }
    LOGN_DEBUG( //FULL_FUNC_NAME << "--"
        "end" )
  }

  void InsertIntoTreeTransverser::CurrentNodeIsLastAddedRightChild()
  {
    m_stdvector_p_grammarpartCurrentParseTreePath.push_back(
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ) ;
    LOGN_DEBUG(//"InsertIntoTreeTransverser::CurrentNodeIsLastAddedRight"
      //"Child()--"
      "current parse tree path: "
      << m_r_bottomUpParser.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
    CheckIfGrammarPartPathMatches(
      ParseTreeTraverser::DirectingLeavesMultipleIterTraverser::Right) ;
  }

  void InsertIntoTreeTransverser::ParseTreePathAdded()
  {
    BYTE byDirection ;
//    LOGN("InsertIntoTreeTransverser::ParseTreePathAdded()")
    if( m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel )
      byDirection = ParseTreeTraverser::DirectingLeavesMultipleIterTraverser::
        Left ;
    else //Parse level is 0 -> this is the root node.
      byDirection = ParseTreeTraverser::DirectingLeavesMultipleIterTraverser::
        RootNodeOrMiddle ;

    if( ! m_stdvector_p_grammarpartCurrentParseTreePath.empty() )
      m_p_grammarpartParentOfCurrentGrammarPart =
        m_stdvector_p_grammarpartCurrentParseTreePath.back() ;
    m_stdvector_p_grammarpartCurrentParseTreePath.push_back(
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ) ;

    std::string stdstrCurrentParseTreePath = m_r_bottomUpParser.
      GetPathAs_std_string( m_stdvector_p_grammarpartCurrentParseTreePath) ;
    LOGN_DEBUG(//"InsertIntoTreeTransverser::ParseTreePathAdded()--"
      "current parse tree path: " << stdstrCurrentParseTreePath ) ;

    CheckIfGrammarPartPathMatches(byDirection) ;
    LOGN_DEBUG( //FULL_FUNC_NAME << "--"
      "end" )
  }

  void InsertIntoTreeTransverser::ParseTreePathPopped()
  {
    m_stdvector_p_grammarpartCurrentParseTreePath.pop_back() ;
    LOGN_DEBUG(//"InsertIntoTreeTransverser::ParseTreePathPopped()--"
      "current parse tree path: "
      << m_r_bottomUpParser.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
  }

}
