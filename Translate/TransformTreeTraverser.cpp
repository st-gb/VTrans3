/*
 * TransformTreeTraverser.cpp
 *
 *  Created on: Dec 20, 2010
 *      Author: Stefan
 */

#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
#include <Parse/ParseByRise.hpp> //class ParseByRise
//class ParseTreeTraverser::InsertIntoTreeTransverser
#include <Translate/InsertIntoTreeTransverser.hpp>
#include <Translate/TransformTreeTraverser.hpp>

namespace ParseTreeTraverser
{
  void TransformTreeTraverser::BeforeBeginAtRoot()
  {
    m_stdvector_p_grammarpartCurrentParseTreePath.clear() ;
    LOGN("TransformTreeTransverser::BeforeBeginAtRoot()--current parse tree "
      "path: " << m_r_parsebyrise.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
  }

  void TransformTreeTraverser::CurrentNodeIsLastAddedRightChild()
  {
    m_stdvector_p_grammarpartCurrentParseTreePath.push_back(
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ) ;
    LOGN("TransformTreeTraverser::CurrentNodeIsLastAddedRightChild()"
      "--current parse tree path: " << m_r_parsebyrise.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
  }

  GrammarPart * TransformTreeTraverser::FindParseTreePathWhereToInsert(
//    const TransformationRule & cr_transformationrule ,
//    bNodeToBeChildIsLeftChild
    //const
    ParseTreeTraverser::InsertIntoTreeTransverser &
      insertintotreetransverser
    )
  {
//    ParseTreeTraverser::InsertIntoTreeTransverser insertintotreetransverser(
//      cr_transformationrule ,
//      m_p_grammarpartStartNode ,
//      m_r_parsebyrise
//      ) ;
    insertintotreetransverser.Traverse() ;
    return insertintotreetransverser.m_p_grammarpartChildOfGrammarPartToInsert ;
  }

  GrammarPart * TransformTreeTraverser::Get2ndLastGrammarPart()
  {
    GrammarPart * p_gp = NULL ;
    LOGN("Get2ndLastGrammarPart() for: "
      << m_r_parsebyrise.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath )
      )
    std::vector<GrammarPart *>::reverse_iterator c_rev_iter =
      m_stdvector_p_grammarpartCurrentParseTreePath.rbegin() ;
    if( c_rev_iter != m_stdvector_p_grammarpartCurrentParseTreePath.rend()
      )
    {
      LOGN( "TransformTreeTraverser::Get2ndLastGrammarPart()-- current "
        "grammar part: " << m_r_parsebyrise.GetGrammarPartName(
          ( * c_rev_iter)->m_wGrammarPartID )
        )
      ++ c_rev_iter ;
      if( c_rev_iter != m_stdvector_p_grammarpartCurrentParseTreePath.rend() )
      {
        LOGN( "TransformTreeTraverser::Get2ndLastGrammarPart()-- current "
          "grammar part: " << m_r_parsebyrise.GetGrammarPartName(
            ( * c_rev_iter)->m_wGrammarPartID )
          )
        p_gp = * c_rev_iter ;
      }
    }
    return p_gp ;
  }

  void TransformTreeTraverser::LeaveFound()
  {
    LOGN("TransformTreeTraverser::LeaveFound()--current parse tree "
      "path: " << m_r_parsebyrise.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
  }

  void TransformTreeTraverser::MoveParseTreeBranch(
    ParseTreeTraverser::InsertIntoTreeTransverser & insertintotreetransverser,
//    GrammarPart * p_grammarpartParentOfGrammarPartToMove ,
    GrammarPart * p_grammarpartBranchToMove ,
    const TransformationRule & r_transformationrule
    )
  {
    if( insertintotreetransverser.
        m_p_grammarpartParentOfCurrentGrammarPart )
    {
      LOGN("Direction where to move child to:"
        << //c_iter_syntaxtreepath2transformationrule->second.
          //m_bInsertLeftChild
        (WORD) insertintotreetransverser.m_byDirection
        )
  //            if( ! bIsLeftChild )
       GrammarPart * & r_p_grammarpartParentOfGrammarPartToMove =
        insertintotreetransverser.m_p_grammarpartParentOfCurrentGrammarPart ;
      //Stores the address of a pointer.
      GrammarPart * * p_p_grammarpartChildOfParentGrammarPartWhereToInsert =
        NULL ;
      switch( insertintotreetransverser.m_byDirection )
      {
      case ParseTreeTraverser::DirectingLeavesMultipleIterTraverser::
        Left :
          LOGN("TransformTreeTraverser::MoveParseTreeBranch()--insert "
            "branch into left child node of the parent grammar part of the "
            "grammar part to insert.")
//          r_p_grammarpartParentOfGrammarPartToMove->mp_grammarpartLeftChild =
//            insertintotreetransverser.
//            m_p_grammarpartChildOfGrammarPartToInsert ;
//          r_p_grammarpartParentOfGrammarPartToMove->mp_grammarpartRightChild
//            = p_grammarpartParentOfGrammarPartToMove ;
          //Store the address of the right child node pointer.
          p_p_grammarpartChildOfParentGrammarPartWhereToInsert =
            & //r_p_grammarpartParentOfGrammarPartToMove->
            insertintotreetransverser.
            m_p_grammarpartParentOfGrammarPartToInsert->
            mp_grammarpartLeftChild ;
          break ;
      case ParseTreeTraverser::DirectingLeavesMultipleIterTraverser::
        Right :
          LOGN("TransformTreeTraverser::MoveParseTreeBranch()--insert "
            "branch into right child node of the parent grammar part of the "
            "grammar part to insert.")
//                insertintotreetransverser.
//                  m_p_grammarpartChildOfGrammarPartToInsert ;
          //Store the address of the right child node pointer.
          p_p_grammarpartChildOfParentGrammarPartWhereToInsert =
            & //r_p_grammarpartParentOfGrammarPartToMove->
            insertintotreetransverser.
            m_p_grammarpartParentOfGrammarPartToInsert->
            mp_grammarpartRightChild ;
//          r_p_grammarpartParentOfGrammarPartToMove->
//            = p_grammarpartParentOfGrammarPartToMove ;
          break ;
      }
      LOGN("TransformTreeTraverser::MoveParseTreeBranch(...)--"
        "p_p_grammarpartChildOfParentGrammarPartWhereToInsert: "
        << p_p_grammarpartChildOfParentGrammarPartWhereToInsert
        <<", * p_p_grammarpartChildOfParentGrammarPartWhereToInsert:"
        << * p_p_grammarpartChildOfParentGrammarPartWhereToInsert
        << ", name:" << m_r_parsebyrise.GetGrammarPartName(
          ( * p_p_grammarpartChildOfParentGrammarPartWhereToInsert)->
          m_wGrammarPartID )
        )
      LOGN("TransformTreeTraverser::MoveParseTreeBranch(...)--"
        "Move child to left?:" << r_transformationrule.m_bInsertLeftChild )

      GrammarPart * p_grammarpartLeftChildOfCreatedNode = NULL ;
      GrammarPart * p_grammarpartRightChildOfCreatedNode = NULL ;
      if( r_transformationrule.m_bInsertLeftChild )
      {
        p_grammarpartLeftChildOfCreatedNode = p_grammarpartBranchToMove ;
        p_grammarpartRightChildOfCreatedNode = insertintotreetransverser.
          m_p_grammarpartChildOfGrammarPartToInsert ;
      }
      else
      {
        p_grammarpartLeftChildOfCreatedNode = insertintotreetransverser.
          m_p_grammarpartChildOfGrammarPartToInsert ;
        p_grammarpartRightChildOfCreatedNode = p_grammarpartBranchToMove ;
      }
      GrammarPart * p_grammarpartToInsert = new
        GrammarPart(
//          ( * p_p_grammarpartChildOfParentGrammarPartWhereToInsert->
//              m_dwLeftmostIndex
          p_grammarpartLeftChildOfCreatedNode->m_dwLeftmostIndex ,
          p_grammarpartRightChildOfCreatedNode->m_dwRightmostIndex ,
          insertintotreetransverser.m_p_grammarpartChildOfGrammarPartToInsert->
          m_wGrammarPartID
          ) ;
      LOGN("TransformTreeTraverser::MoveParseTreeBranch(...)"
        "--Inserting in-between node for the moved node to fit into: writing"
        "value:" << p_grammarpartToInsert
        << "into address:"
        << p_p_grammarpartChildOfParentGrammarPartWhereToInsert
        )
      //
      //        obj   ->   p_pg
      //      \/          \/
      //e.g. clause     clause

      //Insert the created node:
      //Write the address of the created node into the parent's left oder
      //right child.
      * p_p_grammarpartChildOfParentGrammarPartWhereToInsert =
        p_grammarpartToInsert ;
      LOGN("TransformTreeTraverser::MoveParseTreeBranch(...)--"
        "setting p_p_grammarpartChildOfParentGrammarPartWhereToInsert->"
        "mp_grammarpartLeftChild to: "
        << m_r_parsebyrise.GetGrammarPartName(
          insertintotreetransverser.
          m_p_grammarpartChildOfGrammarPartToInsert->
          m_wGrammarPartID )
        << " mp_grammarpartRightChild to: "
        << m_r_parsebyrise.GetGrammarPartName(
          p_grammarpartBranchToMove->m_wGrammarPartID )
        )
        //                  obj  adverb
        //                     \/
        //        p_pg   ->   p_pg
        //      \/          \/
        //e.g. clause     clause
      //( * p_p_grammarpartChildOfParentGrammarPartWhereToInsert)->
      p_grammarpartToInsert->
        mp_grammarpartLeftChild = p_grammarpartLeftChildOfCreatedNode ;
      //( * p_p_grammarpartChildOfParentGrammarPartWhereToInsert)->
      p_grammarpartToInsert->
        mp_grammarpartRightChild = //r_p_grammarpartParentOfGrammarPartToMove ;
          p_grammarpartRightChildOfCreatedNode ;
    }
  }
  //Called after:
  // -the first node (leave node) was added.
  // -a left node directing the root node was added.
  void TransformTreeTraverser::ParseTreePathAdded()
  {
//    LOGN("TransformTreeTraverser::ParseTreePathAdded() begin")
    m_stdvector_p_grammarpartCurrentParseTreePath.push_back(
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ) ;

    std::vector<WORD> stdvec_wGrammarPartID ;
//    GetGrammarPartPathAsGrammarPartIDvector( stdvec_wGrammarPartID ) ;

//    if( //m_stdstrParseTreeToSearchFor ==
//        AnyTransformationParseTreePathMatches()
//      )
//    {
#ifdef COMPILE_WITH_LOG
    std::string stdstrCurrentParseTreePath = m_r_parsebyrise.
      GetPathAs_std_string( m_stdvector_p_grammarpartCurrentParseTreePath) ;
//      m_r_parsebyrise.GetPathAs_std_string( stdvec_wGrammarPartID ) ;
    SyntaxTreePath::GetAsGrammarPartIDvector(
      m_stdvector_p_grammarpartCurrentParseTreePath , stdvec_wGrammarPartID ) ;

//    std::map<std::string,TransformationRule>::const_iterator c_iter =
//      m_stdmap_stdstrTransformationRule2transformationrule.find(
//      stdstrCurrentParseTreePath
//      ) ;
    LOGN("TransformTreeTraverser::ParseTreePathAdded() current parse tree "
      "path: " << stdstrCurrentParseTreePath ) ;
#else
//    std::map<std::string,TransformationRule>::const_iterator
//      c_iter_syntaxtreepath2transformationrule =
//      m_stdmap_stdstrTransformationRule2transformationrule.find(
//        m_r_parsebyrise.GetPathAs_std_string(
//          m_stdvector_p_grammarpartCurrentParseTreePath)
//        ) ;
#endif
//    std::map<WORD *,TransformationRule>::const_iterator c_iter =
      //m_stdmap_ar_wTransformationRuleParseTreePath2transformationrule.begin() ;
    LOGN("TransformTreeTraverser::ParseTreePathAdded()"
      "--m_bMoveParseTreeBranch:" << m_bMoveParseTreeBranch )
    if( m_bMoveParseTreeBranch )
    {
      std::map<SyntaxTreePath,TransformationRule>::const_iterator
        c_iter_syntaxtreepath2transformationrule =
        m_stdmap_syntaxtreepath2transformationrule.begin() ;
      while( c_iter_syntaxtreepath2transformationrule !=
          m_stdmap_syntaxtreepath2transformationrule.end()
        )
      {
        if( c_iter_syntaxtreepath2transformationrule->first.Matches(
          //ar_wGrammarPartID,
            stdvec_wGrammarPartID )
          )
        {
          LOGN("Transformation parse tree path found.")
  //        bool bNodeToBeChildIsLeftChild ;
          ParseTreeTraverser::InsertIntoTreeTransverser
            insertintotreetransverser(
  //          cr_transformationrule ,
            c_iter_syntaxtreepath2transformationrule->second ,
            m_p_grammarpartStartNode ,
            m_r_parsebyrise
            ) ;
          GrammarPart * p_grammarpartChildOfGrammarPartToInsert =
            FindParseTreePathWhereToInsert(
  //          c_iter_syntaxtreepath2transformationrule->second
  //          .m_stdstrParseTreePathWhereToInsert
  //          , bNodeToBeChildIsLeftChild
              insertintotreetransverser
            ) ;
          GrammarPart * p_grammarpartParentOfGrammarPartToMove =
            //Get the parent grammar part of the current grammar part.
            Get2ndLastGrammarPart() ;
          if( p_grammarpartParentOfGrammarPartToMove &&
              p_grammarpartChildOfGrammarPartToInsert
            )
          {
            LOGN( "TransformTreeTraverser::ParseTreePathAdded()-- "
              "grammar part to be a child of the grammar part to insert: "
              << m_r_parsebyrise.GetGrammarPartName(
                p_grammarpartChildOfGrammarPartToInsert->m_wGrammarPartID )
              )
            LOGN( "TransformTreeTraverser::ParseTreePathAdded()-- "
              "parent grammar part of the grammar part to move: "
              << m_r_parsebyrise.GetGrammarPartName(
                insertintotreetransverser.
                m_p_grammarpartParentOfCurrentGrammarPart->m_wGrammarPartID )
              )
            GrammarPart * p_grammarpartBranchToMove = NULL ;
//            SetChildNodeAtGrammarPartToMoveToNULL(
            GetGrammarPartToMove(
              p_grammarpartParentOfGrammarPartToMove ,
              p_grammarpartBranchToMove ) ;
//            p_grammarpartBranchToMove =
//              m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ;
            LOGN("TransformTreeTraverser::ParseTreePathAdded()"
              "--p_grammarpartBranchToMove:" << p_grammarpartBranchToMove
              << " & p_grammarpartBranchToMove:"
              << & p_grammarpartBranchToMove )
            if( p_grammarpartBranchToMove )
            {
              LOGN("p_grammarpartBranchToMove:" <<
                m_r_parsebyrise.GetGrammarPartName(
                p_grammarpartBranchToMove->m_wGrammarPartID ) )
              MoveParseTreeBranch(
                insertintotreetransverser,
//                p_grammarpartParentOfGrammarPartToMove ,
                p_grammarpartBranchToMove ,
                c_iter_syntaxtreepath2transformationrule->second
                ) ;
              //Set to NULL _after_ moving because this may happen:
              // adverb main_verb     main_verb adverb
              //      \ /          ->         \ /
              //    middle                 main_verb
              //                              |
              //                            middle
              //
              // : if set to NULL before moving: middle->right_child: moved to
              //  middle->left_child, middle->right_child set to NULL ;
              //   so middle->right_child which is _NULL_ might be used
              SetChildNodeAtGrammarPartToMoveToNULL(
                p_grammarpartParentOfGrammarPartToMove //,
                //p_grammarpartBranchToMove
                ) ;
            }
            //The branch was moved and because this tree is further traversed
            //the moved branch may match again if it is on the right hand side
            //of the current tree path -> Do not move a second time.
            m_bMoveParseTreeBranch = false ;
            m_bTraverseTree = false ;
            //End loop in super class
            //DirectingLeavesMultipleIterTraverser::ProcessLeavesOfParseTree()
            m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart = NULL ;
          }
        }
        ++ c_iter_syntaxtreepath2transformationrule ;
      } //end while loop.
    } //if( m_bMoveParseTreeBranch )

//    WORD * ar_wGrammarPartID = SyntaxTreePath::
//      GetGrammarPartPathAsGrammarPartIDarray() ;
//    if( ar_wGrammarPartID )
//    if( c_iter != m_stdmap_stdstrTransformationRule2transformationrule.end()
//      )
//    if( SyntaxTreePath::Matches(//ar_wGrammarPartID,
//        stdvec_wGrammarPartID )
//      )
//    {
//      LOGN("Transformation parse tree path found.")
////      FindParseTreePathWhereToInsert( c_iter->second.
////        m_stdstrParseTreePathWhereToInsert) ;
//    }
//    }
  }

  void TransformTreeTraverser::ParseTreePathPopped()
  {
    m_stdvector_p_grammarpartCurrentParseTreePath.pop_back() ;
    LOGN("TransformTreeTraverser::ParseTreePathPopped()--current parse tree "
      "path: " << m_r_parsebyrise.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
  }

  void TransformTreeTraverser::RightChildAdded( unsigned short)
  {
    LOGN("TransformTreeTraverser::RightChildAdded()--current parse tree "
      "path: " << m_r_parsebyrise.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
  }

  void TransformTreeTraverser::GetGrammarPartToMove(
    GrammarPart * p_grammarpartParentOfGrammarPartToMove ,
    GrammarPart * & p_r_grammarpartBranchToMove
    )
  {
    if( p_grammarpartParentOfGrammarPartToMove->mp_grammarpartLeftChild
        == m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart )
    {
      LOGN("TransformTreeTraverser::GetGrammarPartToMove"
        "(...)--grammar part to move is left child.")
      p_r_grammarpartBranchToMove =
        p_grammarpartParentOfGrammarPartToMove->mp_grammarpartLeftChild ;
    }
    else
      if( p_grammarpartParentOfGrammarPartToMove->
          mp_grammarpartRightChild ==
          m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart )
      {
        LOGN("TransformTreeTraverser::GetGrammarPartToMove"
          "(...)--grammar part to move is right child.")
        p_r_grammarpartBranchToMove = p_grammarpartParentOfGrammarPartToMove->
          mp_grammarpartRightChild ;
      }
  }

  void TransformTreeTraverser::SetChildNodeAtGrammarPartToMoveToNULL(
    GrammarPart * p_grammarpartParentOfGrammarPartToMove
    )
  {
    if( p_grammarpartParentOfGrammarPartToMove->mp_grammarpartLeftChild
        == m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart )
    {
      LOGN("TransformTreeTraverser::SetChildNodeAtGrammarPartToMoveToNULL"
        "(...)--grammar part to move is left child, overwriting value:"
      << p_grammarpartParentOfGrammarPartToMove->mp_grammarpartLeftChild
      << "at its address:"
      << & p_grammarpartParentOfGrammarPartToMove->mp_grammarpartLeftChild
      << " with value:"
      << p_grammarpartParentOfGrammarPartToMove->mp_grammarpartRightChild
      )
      //If just 1 child then it should use the left child pointer (needed for
      //traversing the tree).
      p_grammarpartParentOfGrammarPartToMove->mp_grammarpartLeftChild
        = p_grammarpartParentOfGrammarPartToMove->mp_grammarpartRightChild ;
      LOGN("TransformTreeTraverser::SetChildNodeAtGrammarPartToMoveToNULL"
        "(...)--setting value:"
        << p_grammarpartParentOfGrammarPartToMove->mp_grammarpartRightChild
        << " at address:"
        << & p_grammarpartParentOfGrammarPartToMove->mp_grammarpartRightChild
        << " to NULL.")
      //Right child moved to the left, so set the right child to NULL.
      p_grammarpartParentOfGrammarPartToMove->mp_grammarpartRightChild = NULL ;
    }
    else
      if( p_grammarpartParentOfGrammarPartToMove->
          mp_grammarpartRightChild ==
          m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart )
      {
        LOGN("TransformTreeTraverser::SetChildNodeAtGrammarPartToMoveToNULL"
          "(...)--grammar part to move is right child, overwriting value:"
          << p_grammarpartParentOfGrammarPartToMove->mp_grammarpartRightChild
          << "at its address:"
          << & p_grammarpartParentOfGrammarPartToMove->mp_grammarpartRightChild
          << " with NULL.")
        p_grammarpartParentOfGrammarPartToMove->mp_grammarpartRightChild
          = NULL ;
      }
  }

  TransformTreeTraverser::TransformTreeTraverser(
    //const std::string & cr_stdstrParseTreeToSearchFor
//    const std::map<std::string,TransformationRule> &
//      cr_stdmap_stdstrTransformationRule2transformationrule ,
    const std::map<SyntaxTreePath,TransformationRule> &
      cr_stdmap_syntaxtreepath2transformationrule ,
    const GrammarPart * p_grammarpartStartNode ,
    ParseByRise & r_parsebyrise
    )
    :
    ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
     p_grammarpartStartNode ,
     & r_parsebyrise
     ) ,
    m_bMoveParseTreeBranch( true ) ,
    m_p_grammarpartStartNode( p_grammarpartStartNode ) ,
//    m_stdmap_stdstrTransformationRule2transformationrule (
//      cr_stdmap_stdstrTransformationRule2transformationrule) ,
    m_stdmap_syntaxtreepath2transformationrule(
      cr_stdmap_syntaxtreepath2transformationrule ) ,
    m_r_parsebyrise( r_parsebyrise )
  {

  }

  TransformTreeTraverser::~TransformTreeTraverser()
  {
    // TODO Auto-generated destructor stub
  }
  void TransformTreeTraverser::UnprocessedHighestLevelNodeFound()
  {
    LOGN("TransformTreeTraverser::UnprocessedHighestLevelNodeFound()"
      "--current parse tree path: " << m_r_parsebyrise.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
  }
}
