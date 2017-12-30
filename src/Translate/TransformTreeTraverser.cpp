/*
 * TransformTreeTraverser.cpp
 *
 *  Created on: Dec 20, 2010
 *      Author: Stefan
 */

#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
#include <Parse/ParseByRise.hpp> //class ParseByRise
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
//class ParseTreeTraverser::InsertIntoTreeTransverser
#include <Translate/InsertIntoTreeTransverser.hpp>
#include <Translate/TransformTreeTraverser.hpp>

namespace ParseTreeTraverser
{
  void TransformTreeTraverser::BeforeBeginAtRoot()
  {
    m_stdvector_p_grammarpartCurrentParseTreePath.clear() ;
    LOGN_DEBUG(//"TransformTreeTransverser::BeforeBeginAtRoot()--"
      "current parse tree path: " << m_r_bottomUpParser.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
  }

  void TransformTreeTraverser::CurrentNodeIsLastAddedRightChild()
  {
    m_stdvector_p_grammarpartCurrentParseTreePath.push_back(
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ) ;
    LOGN_DEBUG(//"TransformTreeTraverser::CurrentNodeIsLastAddedRightChild()--"
      "current parse tree path: " << m_r_bottomUpParser.GetPathAs_std_string(
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
    LOGN_DEBUG(//"Get2ndLastGrammarPart() "
      "for: " << m_r_bottomUpParser.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath )
      )
    std::vector<GrammarPart *>::reverse_iterator c_rev_iter =
      m_stdvector_p_grammarpartCurrentParseTreePath.rbegin() ;
    if( c_rev_iter != m_stdvector_p_grammarpartCurrentParseTreePath.rend()
      )
    {
      LOGN_DEBUG( //"TransformTreeTraverser::Get2ndLastGrammarPart()--"
        "current grammar part: " << m_r_bottomUpParser.GetGrammarPartName(
          ( * c_rev_iter)->m_wGrammarPartID )
        )
      ++ c_rev_iter ;
      if( c_rev_iter != m_stdvector_p_grammarpartCurrentParseTreePath.rend() )
      {
        LOGN_DEBUG( //"TransformTreeTraverser::Get2ndLastGrammarPart()--"
          "current grammar part: " << m_r_bottomUpParser.GetGrammarPartName(
            ( * c_rev_iter)->m_wGrammarPartID )
          )
        p_gp = * c_rev_iter ;
      }
    }
    return p_gp ;
  }

  void TransformTreeTraverser::LeaveFound()
  {
    std::string std_strSyntaxTreePath = m_r_bottomUpParser.GetPathAs_std_string(
      m_stdvector_p_grammarpartCurrentParseTreePath);
    LOGN_DEBUG(//"TransformTreeTraverser::LeaveFound()--"
      "current parse tree path: " << std_strSyntaxTreePath
      )
    PossiblyMoveParseTreeBranch();
  }

  GrammarPart * * GetChildOfParentGrammarPartWhereToInsertOrToExchange(
    ParseTreeTraverser::InsertIntoTreeTransverser & insertintotreetransverser
    )
  {
    GrammarPart * * p_p_grammarpartChildOfParentGrammarPartWhereToInsert =
      NULL;
    switch( insertintotreetransverser.m_byDirection )
    {
    case ParseTreeTraverser::DirectingLeavesMultipleIterTraverser::
      Left :
        LOGN_DEBUG(//"TransformTreeTraverser::MoveParseTreeBranch()--"
          "insert branch into left child node of the parent grammar part of "
          "the grammar part to insert.")
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
        LOGN_DEBUG(//"TransformTreeTraverser::MoveParseTreeBranch()--"
          "insert branch into right child node of the parent grammar part of "
          "the grammar part to insert.")
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
    //TODO ?! :
//    case ParseTreeTraverser::DirectingLeavesMultipleIterTraverser::RootNodeOrMiddle:
    }
    return p_p_grammarpartChildOfParentGrammarPartWhereToInsert;
  }

  void TransformTreeTraverser::MoveParseTreeBranch(
    ParseTreeTraverser::InsertIntoTreeTransverser & insertintotreetransverser,
//    GrammarPart * p_grammarpartParentOfGrammarPartToMove ,
    GrammarPart * p_grammarpartBranchToMove ,
    const TransformationRule & r_transformationrule
    )
  {
    LOGN_DEBUG( //FULL_FUNC_NAME << "--"
      "insertintotreetransverser."
      "m_p_grammarpartParentOfCurrentGrammarPart:" <<
      insertintotreetransverser.m_p_grammarpartParentOfCurrentGrammarPart )
    if( insertintotreetransverser.
        m_p_grammarpartParentOfCurrentGrammarPart )
    {
      std::string std_strParentOfCurrentGrammarPart =
        m_r_bottomUpParser.GetGrammarPartName( insertintotreetransverser.
        m_p_grammarpartParentOfCurrentGrammarPart->m_wGrammarPartID );
      WORD wDirection = (WORD) insertintotreetransverser.m_byDirection;
      LOGN_DEBUG( //FULL_FUNC_NAME << "--"
        "Direction where to move child to:"
        << //c_iter_syntaxtreepath2transformationrule->second.
          //m_bInsertLeftChild
        wDirection << " (" << DirectingLeavesMultipleIterTraverser::
        s_ar_chDirection[wDirection] << ")"
        )
  //            if( ! bIsLeftChild )
//       GrammarPart * & r_p_grammarpartParentOfGrammarPartToMove =
//        insertintotreetransverser.m_p_grammarpartParentOfCurrentGrammarPart ;

      //Stores the address of a pointer.
      GrammarPart * * p_p_grammarpartChildOfParentGrammarPartWhereToInsert =
        //NULL ;
        GetChildOfParentGrammarPartWhereToInsertOrToExchange(
          insertintotreetransverser);

      //TODO what if pointer p_p_grammarpartChildOfParentGrammarPartWhereToInsert
      //is NULL??
      if( p_p_grammarpartChildOfParentGrammarPartWhereToInsert )
      {
        std::string std_strChildOfParentGrammarPartWhereToInsert =
          m_r_bottomUpParser.GetGrammarPartName(
          ( * p_p_grammarpartChildOfParentGrammarPartWhereToInsert)->
          m_wGrammarPartID );
        LOGN_DEBUG(//"TransformTreeTraverser::MoveParseTreeBranch(...)"
          //FULL_FUNC_NAME << "--"
          "p_p_grammarpartChildOfParentGrammarPartWhereToInsert: "
          << p_p_grammarpartChildOfParentGrammarPartWhereToInsert
          <<", * p_p_grammarpartChildOfParentGrammarPartWhereToInsert:"
          << * p_p_grammarpartChildOfParentGrammarPartWhereToInsert
          << ", name:" << std_strChildOfParentGrammarPartWhereToInsert
          )
        LOGN_DEBUG( //"TransformTreeTraverser::MoveParseTreeBranch(...)"
          //FULL_FUNC_NAME << "--"
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
        LOGN_DEBUG(//"TransformTreeTraverser::MoveParseTreeBranch(...)--"
          "Inserting in-between node for the moved node to fit into: writing"
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

        LOGN_DEBUG(//"TransformTreeTraverser::MoveParseTreeBranch(...)--"
          "setting p_p_grammarpartChildOfParentGrammarPartWhereToInsert->"
          "mp_grammarpartLeftChild to: "
          << m_r_bottomUpParser.GetGrammarPartName(
            insertintotreetransverser.
            m_p_grammarpartChildOfGrammarPartToInsert->
            m_wGrammarPartID )
          << " mp_grammarpartRightChild to: "
          << m_r_bottomUpParser.GetGrammarPartName(
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
  }

  void TransformTreeTraverser::PossiblyMoveParseTreeBranch()
  {
    std::vector<WORD> stdvec_wGrammarPartID ;
  //    GetGrammarPartPathAsGrammarPartIDvector( stdvec_wGrammarPartID ) ;

  //    if( //m_stdstrParseTreeToSearchFor ==
  //        AnyTransformationParseTreePathMatches()
  //      )
  //    {
  #ifdef COMPILE_WITH_LOG
    std::string stdstrCurrentParseTreePath = m_r_bottomUpParser.
      GetPathAs_std_string( m_stdvector_p_grammarpartCurrentParseTreePath) ;
  //      m_r_parsebyrise.GetPathAs_std_string( stdvec_wGrammarPartID ) ;
    SyntaxTreePath::GetAsGrammarPartIDvector(
      m_stdvector_p_grammarpartCurrentParseTreePath , stdvec_wGrammarPartID ) ;

  //    std::map<std::string,TransformationRule>::const_iterator c_iter =
  //      m_stdmap_stdstrTransformationRule2transformationrule.find(
  //      stdstrCurrentParseTreePath
  //      ) ;
    LOGN_DEBUG(//"TransformTreeTraverser::ParseTreePathAdded()"
      //FULL_FUNC_NAME << "--"
      "current parse tree path: "
      << stdstrCurrentParseTreePath )
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
    LOGN_DEBUG(//"TransformTreeTraverser::ParseTreePathAdded()"
      //FULL_FUNC_NAME << "--"
      "m_bMoveParseTreeBranch:" << m_bMoveParseTreeBranch )
    if( m_bMoveParseTreeBranch )
    {
      std::map<SyntaxTreePath,TransformationRule>::const_iterator
        c_iter_syntaxtreepath2transformationrule =
        m_stdmap_syntaxtreepath2transformationrule.begin() ;

      while( c_iter_syntaxtreepath2transformationrule !=
          m_stdmap_syntaxtreepath2transformationrule.end()
        )
      {
        //Transformation's syntax tree path matches current parse tree path.
        if( c_iter_syntaxtreepath2transformationrule->first.Matches(
          //ar_wGrammarPartID,
            stdvec_wGrammarPartID )
          )
        {
          const TransformationRule & c_r_transformationrule =
              c_iter_syntaxtreepath2transformationrule->second;
          MoveOrExchangeParseTreePath(c_r_transformationrule);
        }
        ++ c_iter_syntaxtreepath2transformationrule ;
      } //end while loop.
    } //if( m_bMoveParseTreeBranch )
    LOGN_DEBUG( //FULL_FUNC_NAME << "--
      "end" )
  }

  void TransformTreeTraverser::ExchangeSyntaxTreePathes(
    BYTE byDirectionOfGrammarPartToMoveOrToExchange,
    GrammarPart * p_grammarpartParentOfGrammarPartToMoveOrToExchange,
    GrammarPart * p_grammarpartBranchToMoveOrToExchange,
    ParseTreeTraverser::InsertIntoTreeTransverser & r_insertintotreetransverser
    )
  {
    GrammarPart * * p_p_grammarpartChildOfParentGrammarPartToExchange =
      GetChildOfParentGrammarPartWhereToInsertOrToExchange(
      r_insertintotreetransverser);

    std::string std_str2ndChildGrammarPartName = m_r_bottomUpParser.GetGrammarPartName(
        (* p_p_grammarpartChildOfParentGrammarPartToExchange)->
        m_wGrammarPartID );
    std::string std_str2ndParentGrammarPartName =
        m_r_bottomUpParser.GetGrammarPartName( r_insertintotreetransverser.
              m_p_grammarpartParentOfGrammarPartToInsert->m_wGrammarPartID);

    std::string std_str1stParentGrammarPartName =
        m_r_bottomUpParser.GetGrammarPartName(
        p_grammarpartParentOfGrammarPartToMoveOrToExchange->m_wGrammarPartID);
    std::string std_str1stChildGrammarPartName = m_r_bottomUpParser.
        GetGrammarPartName(
        p_grammarpartBranchToMoveOrToExchange->
        m_wGrammarPartID );
    GrammarPart * * p_p_grammarpart1stChildToExchange = NULL;
    GrammarPart * p_grammarpart1stChildToExchange = NULL;
    switch(byDirectionOfGrammarPartToMoveOrToExchange)
    {
    case DirectingLeavesMultipleIterTraverser::Left:
      p_p_grammarpart1stChildToExchange =
        //Address of child pointer.
        & p_grammarpartParentOfGrammarPartToMoveOrToExchange->
        mp_grammarpartLeftChild;
      break;
    case DirectingLeavesMultipleIterTraverser::Right:
      p_p_grammarpart1stChildToExchange =
        //Address of child pointer.
        & p_grammarpartParentOfGrammarPartToMoveOrToExchange->
        mp_grammarpartRightChild;
      break;
    }
    if( p_p_grammarpart1stChildToExchange )
    {
      //Save pointer to 1st child (is overwritten afterwards)
      p_grammarpart1stChildToExchange = * p_p_grammarpart1stChildToExchange;
      //Write address of 2nd child to address of 1st child pointer.
      * p_p_grammarpart1stChildToExchange =
//          r_insertintotreetransverser.
//                 m_p_grammarpartParentOfGrammarPartToInsert
        * p_p_grammarpartChildOfParentGrammarPartToExchange;
      //Side of parent's branch where the 2nd child is located.
      switch(r_insertintotreetransverser.m_byDirection)
      {
      case DirectingLeavesMultipleIterTraverser::Left:
        r_insertintotreetransverser.
          m_p_grammarpartParentOfGrammarPartToInsert->mp_grammarpartLeftChild
          =
          //Address of 1st child
//          * p_p_grammarpart1stChildToExchange;
          p_grammarpart1stChildToExchange;
        break;
      case DirectingLeavesMultipleIterTraverser::Right:
        r_insertintotreetransverser.
          m_p_grammarpartParentOfGrammarPartToInsert->mp_grammarpartRightChild
          =
          //Address of 1st child
//          * p_p_grammarpart1stChildToExchange;
          p_grammarpart1stChildToExchange;
        break;
      }
    }
  }

  void TransformTreeTraverser::MoveOrExchangeParseTreePath(
      const TransformationRule & c_r_transformationrule)
  {
    LOGN_DEBUG( //FULL_FUNC_NAME <<
      "Transformation parse tree path found.")
  //        bool bNodeToBeChildIsLeftChild ;
    try
    {
      const SyntaxTreePath * p_syntaxtreepath = NULL;
      //-> Exchange the 2 syntax tree branches.
      if( c_r_transformationrule.m_syntaxtreepath2nd.m_wNumberOfElements > 0 )
        p_syntaxtreepath = & c_r_transformationrule.m_syntaxtreepath2nd;
      else if (
          //! c_r_transformationrule.m_stdstrParseTreePathWhereToInsert.
          //empty()
        c_r_transformationrule.m_syntaxtreepathWhereToInsert.
        m_wNumberOfElements > 0
        )
        p_syntaxtreepath = & c_r_transformationrule.
          m_syntaxtreepathWhereToInsert;

      //May throw an exception because of "SyntaxTreePath"'s constructor.
      ParseTreeTraverser::InsertIntoTreeTransverser
        insertintotreetransverser(
  //          cr_transformationrule ,
        c_r_transformationrule ,
        * p_syntaxtreepath,
        m_p_grammarpartStartNode ,
        m_r_bottomUpParser
        ) ;

      GrammarPart * p_grammarpartChildOfGrammarPartWhereToInsertOrToExchange =
        FindParseTreePathWhereToInsert(
  //          c_iter_syntaxtreepath2transformationrule->second
  //          .m_stdstrParseTreePathWhereToInsert
  //          , bNodeToBeChildIsLeftChild
          insertintotreetransverser
        ) ;
      GrammarPart * p_grammarpartParentOfGrammarPartToMoveOrToExchange =
        //Get the parent grammar part of the current grammar part.
        Get2ndLastGrammarPart() ;
      if( p_grammarpartParentOfGrammarPartToMoveOrToExchange &&
          p_grammarpartChildOfGrammarPartWhereToInsertOrToExchange
        )
      {
        std::string std_strGrammarPartName;
        std_strGrammarPartName = m_r_bottomUpParser.GetGrammarPartName(
            p_grammarpartChildOfGrammarPartWhereToInsertOrToExchange->
            m_wGrammarPartID );

        std::string std_strChildOfGrammarPartToInsert =
            std_strGrammarPartName;
        LOGN_DEBUG( //"TransformTreeTraverser::ParseTreePathAdded()-- "
          //FULL_FUNC_NAME << "--"
          "grammar part to be a child of the grammar part to insert: "
          << std_strGrammarPartName
          )
        std_strGrammarPartName = m_r_bottomUpParser.GetGrammarPartName(
          insertintotreetransverser.
          m_p_grammarpartParentOfCurrentGrammarPart->m_wGrammarPartID );
        LOGN_DEBUG( //"TransformTreeTraverser::ParseTreePathAdded()-- "
            //FULL_FUNC_NAME << "--"
          "parent grammar part of the grammar part to move: "
          << std_strGrammarPartName
          )
        std::string std_strParentOfGrammarPartToMove =
            m_r_bottomUpParser.GetGrammarPartName(
            p_grammarpartParentOfGrammarPartToMoveOrToExchange->m_wGrammarPartID );
        GrammarPart * p_grammarpartBranchToMoveOrToExchange = NULL ;

  //            SetChildNodeAtGrammarPartToMoveToNULL(
        BYTE byDirectionOfGrammarPartToMoveOrToExchange =
          GetGrammarPartToMoveOrToExchange(
          p_grammarpartParentOfGrammarPartToMoveOrToExchange ,
          p_grammarpartBranchToMoveOrToExchange ) ;
  //            p_grammarpartBranchToMove =
  //              m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ;
        LOGN_DEBUG( //"TransformTreeTraverser::ParseTreePathAdded()"
          /* FULL_FUNC_NAME << "--" */
          "p_grammarpartBranchToMove:" <<
          p_grammarpartBranchToMoveOrToExchange
          << " & p_grammarpartBranchToMove:"
          << & p_grammarpartBranchToMoveOrToExchange )
        if( p_grammarpartBranchToMoveOrToExchange )
        {
          if( c_r_transformationrule.m_syntaxtreepath2nd.m_wNumberOfElements > 0 )
            ExchangeSyntaxTreePathes(
              byDirectionOfGrammarPartToMoveOrToExchange,
              p_grammarpartParentOfGrammarPartToMoveOrToExchange,
              p_grammarpartBranchToMoveOrToExchange,
//              p_grammarpartParentOfGrammarPartToMoveOrToExchange
              insertintotreetransverser
              );
          else
          {
//            MoveSyntaxTreePath();

            std_strGrammarPartName = m_r_bottomUpParser.GetGrammarPartName(
                p_grammarpartBranchToMoveOrToExchange->m_wGrammarPartID );
            LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "p_grammarpartBranchToMove:"
              << std_strGrammarPartName
              )
            MoveParseTreeBranch(
              insertintotreetransverser,
    //                p_grammarpartParentOfGrammarPartToMove ,
              p_grammarpartBranchToMoveOrToExchange ,
  //            c_iter_syntaxtreepath2transformationrule->second
              c_r_transformationrule
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
              p_grammarpartParentOfGrammarPartToMoveOrToExchange //,
              //p_grammarpartBranchToMove
              ) ;
          }
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
    catch( const GetGrammarPartIDexception & c_r_getgrammarpartidexception )
    {
        mp_bottomUpParser->m_p_userinterface->Message(
          "Error moving parse tree branch \"" +
//          c_iter_syntaxtreepath2transformationrule->second.
          c_r_transformationrule.
          m_stdstrParseTreePathWhereToInsert + "\" : "
          "unknown grammar part ID \"" +
             c_r_getgrammarpartidexception.m_stdstr + " \""
          );
    }
    LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "end")
  }

  //Called after:
  // -the first node (leave node) was added.
  // -a left node directing the root node was added.
  void TransformTreeTraverser::ParseTreePathAdded()
  {
//    LOGN("TransformTreeTraverser::ParseTreePathAdded() begin")
    m_stdvector_p_grammarpartCurrentParseTreePath.push_back(
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ) ;

    PossiblyMoveParseTreeBranch();

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
    LOGN_DEBUG(//"TransformTreeTraverser::ParseTreePathPopped()--"
      "current parse tree "
      "path: " << m_r_bottomUpParser.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
  }

  void TransformTreeTraverser::RightChildAdded( unsigned short)
  {
    LOGN_DEBUG(//"TransformTreeTraverser::RightChildAdded()--"
      "current parse tree "
      "path: " << m_r_bottomUpParser.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
  }

  BYTE TransformTreeTraverser::GetGrammarPartToMoveOrToExchange(
    GrammarPart * p_grammarpartParentOfGrammarPartToMove ,
    GrammarPart * & p_r_grammarpartBranchToMove
    )
  {
    BYTE by = DirectingLeavesMultipleIterTraverser::RootNodeOrMiddle;

    if( p_grammarpartParentOfGrammarPartToMove->mp_grammarpartLeftChild
        == m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart
        )
    {
      LOGN_DEBUG("TransformTreeTraverser::GetGrammarPartToMove"
        "(...)--grammar part to move is left child.")
      p_r_grammarpartBranchToMove =
        p_grammarpartParentOfGrammarPartToMove->mp_grammarpartLeftChild ;
      by = DirectingLeavesMultipleIterTraverser::Left;
    }
    else
      if( p_grammarpartParentOfGrammarPartToMove->
          mp_grammarpartRightChild ==
          m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart )
      {
        LOGN_DEBUG(//"TransformTreeTraverser::GetGrammarPartToMove(...)--"
          "grammar part to move is right child.")
        p_r_grammarpartBranchToMove = p_grammarpartParentOfGrammarPartToMove->
          mp_grammarpartRightChild ;
        by = DirectingLeavesMultipleIterTraverser::Right;
      }
    return by;
  }

  void TransformTreeTraverser::SetChildNodeAtGrammarPartToMoveToNULL(
    GrammarPart * p_grammarpartParentOfGrammarPartToMove
    )
  {
    if( p_grammarpartParentOfGrammarPartToMove->mp_grammarpartLeftChild
        == m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart )
    {
      LOGN_DEBUG(//"TransformTreeTraverser::"
        //"SetChildNodeAtGrammarPartToMoveToNULL(...)--"
        "grammar part to move is left child, overwriting value:"
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
      LOGN_DEBUG(//"TransformTreeTraverser::"
        //"SetChildNodeAtGrammarPartToMoveToNULL(...)--"
        "setting value:"
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
        LOGN_DEBUG(//"TransformTreeTraverser::"
          //"SetChildNodeAtGrammarPartToMoveToNULL(...)--"
          "grammar part to move is right child, overwriting value:"
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
    VTrans3::BottomUpParser & r_bottomUpParser
    )
    :
    ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
     p_grammarpartStartNode ,
     & r_bottomUpParser
     ) ,
    m_bMoveParseTreeBranch( true ) ,
    m_p_grammarpartStartNode( p_grammarpartStartNode ) ,
//    m_stdmap_stdstrTransformationRule2transformationrule (
//      cr_stdmap_stdstrTransformationRule2transformationrule) ,
    m_stdmap_syntaxtreepath2transformationrule(
      cr_stdmap_syntaxtreepath2transformationrule ) ,
    m_r_bottomUpParser( r_bottomUpParser )
  {

  }

  TransformTreeTraverser::~TransformTreeTraverser()
  {
    // TODO Auto-generated destructor stub
  }
  void TransformTreeTraverser::UnprocessedHighestLevelNodeFound()
  {
    LOGN_DEBUG(//"TransformTreeTraverser::UnprocessedHighestLevelNodeFound()--"
      "current parse tree path: " << m_r_bottomUpParser.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
  }
}
