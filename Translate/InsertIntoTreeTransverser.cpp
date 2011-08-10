/*
 * InsertIntoTreeTransverser.cpp
 *
 *  Created on: Dec 21, 2010
 *      Author: Stefan
 */
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
#include <Parse/ParseByRise.hpp> //class ParseByRise
#include <Translate/InsertIntoTreeTransverser.hpp>
#include <Translate/SyntaxTreePath.hpp> //class SyntaxTreePath
#include <Translate/TransformationRule.hpp> //class TransformationRule

namespace ParseTreeTraverser
{

  InsertIntoTreeTransverser::InsertIntoTreeTransverser(
    const TransformationRule & cr_transformationrule ,
    const GrammarPart * p_grammarpartStartNode ,
    ParseByRise & r_parsebyrise
    )
    :
    ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
      p_grammarpartStartNode ,
      & r_parsebyrise
      ) ,
    m_cr_transformationrule ( cr_transformationrule ) ,
    m_r_parsebyrise( r_parsebyrise ) ,
    m_p_grammarpartChildOfGrammarPartToInsert( NULL ) ,
    m_p_grammarpartParentOfGrammarPartToInsert( NULL ) ,
    m_syntaxtreepath(
      cr_transformationrule.m_stdstrParseTreePathWhereToInsert,
      & r_parsebyrise)
  {
    LOGN("InsertIntoTreeTransverser(...)--"
      << cr_transformationrule.m_stdstrParseTreePathWhereToInsert )
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
    ParseByRise & r_parsebyrise
    )
    :
    ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
      p_grammarpartStartNode ,
      & r_parsebyrise
      ) ,
    m_cr_transformationrule ( cr_transformationrule ) ,
    m_r_parsebyrise( r_parsebyrise ) ,
    m_p_grammarpartChildOfGrammarPartToInsert( NULL ) ,
    m_p_grammarpartParentOfGrammarPartToInsert( NULL ) ,
    m_syntaxtreepath(
//      c_r_stdstrParseTreePath,
//      & r_parsebyrise
      c_r_syntaxtreepath
      )
  {
    LOGN( //"InsertIntoTreeTransverser(...)--"
        FULL_FUNC_NAME
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
    // TODO Auto-generated destructor stub
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
      LOGN("InsertIntoTreeTransverser::ParseTreePathAdded()--"
        "m_syntaxtreepath.Matches(...)" )
      m_p_grammarpartChildOfGrammarPartToInsert =
       m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ;
      LOGN("InsertIntoTreeTransverser::CheckIfGrammarPartPathMatches(...)"
        "--m_p_grammarpartChildOfGrammarPartToInsert:"
        << m_r_parsebyrise.GetGrammarPartName(
          m_p_grammarpartChildOfGrammarPartToInsert->m_wGrammarPartID )
        )
//      m_p_grammarpartParentOfGrammarPartToInsert =
//        m_p_grammarpartParentOfCurrentGrammarPart ;
      if( m_stdvector_p_grammarpartCurrentParseTreePath.size() > 1 )
        m_p_grammarpartParentOfGrammarPartToInsert =
        //2nd last element.
        m_stdvector_p_grammarpartCurrentParseTreePath.at (
          m_stdvector_p_grammarpartCurrentParseTreePath.size() - 2 ) ;
      LOGN("InsertIntoTreeTransverser::CheckIfGrammarPartPathMatches(...)"
        "--m_p_grammarpartParentOfGrammarPartToInsert:"
        << m_p_grammarpartParentOfGrammarPartToInsert
        )
      if( m_p_grammarpartParentOfGrammarPartToInsert )
        LOGN("InsertIntoTreeTransverser::CheckIfGrammarPartPathMatches(...)"
          "--m_p_grammarpartParentOfGrammarPartToInsert:"
          << m_r_parsebyrise.GetGrammarPartName(
            m_p_grammarpartParentOfGrammarPartToInsert->m_wGrammarPartID )
          )

      m_byDirection = byDirection ;
      m_bTraverseTree = false ;
      //End loop in super class
      //DirectingLeavesMultipleIterTraverser::ProcessLeavesOfParseTree()
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart = NULL ;
    }
  }

  void InsertIntoTreeTransverser::CurrentNodeIsLastAddedRightChild()
  {
    m_stdvector_p_grammarpartCurrentParseTreePath.push_back(
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ) ;
    LOGN("InsertIntoTreeTransverser::CurrentNodeIsLastAddedRightChild()"
      "--current parse tree path: "
      << m_r_parsebyrise.GetPathAs_std_string(
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

    std::string stdstrCurrentParseTreePath = m_r_parsebyrise.
      GetPathAs_std_string( m_stdvector_p_grammarpartCurrentParseTreePath) ;
    LOGN("InsertIntoTreeTransverser::ParseTreePathAdded()--current parse tree "
      "path: " << stdstrCurrentParseTreePath ) ;

    CheckIfGrammarPartPathMatches(byDirection) ;
  }

  void InsertIntoTreeTransverser::ParseTreePathPopped()
  {
    m_stdvector_p_grammarpartCurrentParseTreePath.pop_back() ;
    LOGN("InsertIntoTreeTransverser::ParseTreePathPopped()"
      "--current parse tree path: "
      << m_r_parsebyrise.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
  }

}
