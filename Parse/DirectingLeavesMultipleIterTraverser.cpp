/*
 * DirectingLeavesMultipleIterTraverser.cpp
 *
 *  Created on: May 6, 2010
 *      Author: Stefan
 */

#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>
#include <Parse/GrammarPart.hpp>
#include <Parse/ParseByRise.hpp>
#include <rest.h> //DEBUG_COUT

namespace ParseTreeTraverser
{

  DirectingLeavesMultipleIterTraverser::DirectingLeavesMultipleIterTraverser(
    const GrammarPart * p_grammarpartStartNode
    , ParseByRise * p_parsebyrise
    )
    :
    m_grammarpartpointer_and_parselevelCurrent (
      p_grammarpartStartNode , 0 )
    , mp_grammarpartStartNode ( p_grammarpartStartNode )
    , m_wParseLevel( 0)
  {
    mp_parsebyrise = p_parsebyrise ;
  }

  DirectingLeavesMultipleIterTraverser::~DirectingLeavesMultipleIterTraverser()
  {
    // TODO Auto-generated destructor stub
  }

//  const GrammarPart *
  GrammarPartPointerAndParseLevel *
  DirectingLeavesMultipleIterTraverser::
    GetNextRightGrammarPartNotProcessedYet()
  {
//    const  GrammarPart * p_grammarpart = NULL ;
    GrammarPartPointerAndParseLevel * p_grammarpartpointerandparselevel = NULL ;
    if( //! m_stdvec_p_grammarpartRightNodeToProcess.empty()
        ! m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.empty()
      )
    {
      //std::vector<GrammarPart *>::iterator iter =
//      p_grammarpart =
//          m_stdvec_p_grammarpartRightNodeToProcess.
//        //"Returns a read/write reference to the data at the last
//        //*  element of the %vector."
//        back() ;
      p_grammarpartpointerandparselevel =
          & m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.back() ;
      //      bRightChild = true ;
      //      vecCurrentParseTreePath.push_back(p_grammarpart) ;
      //p_grammarpart = *iter ;
      DEBUG_COUT ( "DrawLeavesOfParseTree--popping " <<
        mp_parsebyrise->GetGrammarPartName(
//        p_grammarpart->m_wGrammarPartID
        p_grammarpartpointerandparselevel->m_p_grammarpart->m_wGrammarPartID
        )
//        << p_grammarpart
        << p_grammarpartpointerandparselevel->m_p_grammarpart
      //            << " current size=" << m_stdvecNodesToProcess.size()
        << "\n" )

      //
      //
      //e.g.: def_article_noun
      //        /       \_ ( if "\"= last char:g++ warning:"multi-line comment")
      //     article  noun
      //when at article: parse level is 1,
      //because "noun" was the last pushed it is the next
      //        m_wParseLevel = m_stdvecNodesToProcess.size() + 1 ;
      //          DrawGrammarPartNameAndPossiblyToken( *iter , wxpaintdc) ;
//      m_stdvec_p_grammarpartRightNodeToProcess.pop_back() ;
      m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.pop_back() ;
      //std::cout.flush() ;
      //usleep(100000) ;
      //          -- m_wParseLevel ;
    }
//    else
//      p_grammarpart = NULL ;
    return //p_grammarpart ;
      p_grammarpartpointerandparselevel ;
  }

  //Traverses the parse tree multiple times beginning from a specific node:
  //  in this example the iterations start at "subject"
  //  subject
  //   /
  // def_article_noun
  //  /        \       (if "\"= last char:g++ warning: "multi-line comment" )
  //article  noun
  //
  // When the algorithm reaches a node
  //  -whose 2 children have been processed yet
  //     the node stores the value
  //  -whose 1 child has been processed yet, 2nd child is NULL
  //  -that is a leaf (article or noun in the ex. above)
  //    if a word class that implies a "personindex" (like "I"->1st pers. sing.)
  //    store that person index value in the GrammarPart
  void DirectingLeavesMultipleIterTraverser::Traverse()
  {
    const GrammarPart * p_grammarpart = mp_grammarpartStartNode ;
    BeforeBeginAtRoot() ;
    ProcessLeavesOfParseTree() ;
//    m_grammarpartpointer_and_parselevelCurrent = GrammarPartPointerAndParseLevel(
//        mp_grammarpartStartNode , 0 ) ;
    if( //If the root has not been drawn yet (for trees that have only 1 node
        //that is root and leaf at the same time ).
        //If this case is not catched this may result in an endless loop
        //below.
      m_stdset_p_grammarpartProcessedYet.find( p_grammarpart ) ==
        m_stdset_p_grammarpartProcessedYet.end()
      )
    {
      do
      {
        BeforeBeginAtRoot() ;
        ProcessNextParseTreeLevelDirectingRoot(
          //p_grammarpart
          ) ;
      }
      while( //m_stdmap_p_grammarpart2HorizCenter.size() < dwNumberOfAlreadyDrawnItems
        //While the root has not been drawn yet.
        m_stdset_p_grammarpartProcessedYet.find( p_grammarpart ) ==
          m_stdset_p_grammarpartProcessedYet.end()
        ) ;
    }
  }

  void DirectingLeavesMultipleIterTraverser::ProcessLeavesOfParseTree()
  {
//    m_stdvec_p_grammarpartRightNodeToProcess.clear();
    m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.clear() ;
//    const GrammarPart * p_grammarpart = mp_grammarpartStartNode;
//    mcp_grammarpartCurrent = mp_grammarpartStartNode;
    m_grammarpartpointer_and_parselevelCurrent = GrammarPartPointerAndParseLevel(
        mp_grammarpartStartNode , 0 ) ;
    //for the root element
    ParseTreePathAdded() ;
    do
    {
      //TODO SIGSEV here
      if( //mcp_grammarpartCurrent->mp_grammarpartRightChild
          m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
          mp_grammarpartRightChild
        )
      {
        //Add to process possible children of the right nodes later.
//        m_stdvec_p_grammarpartRightNodeToProcess.push_back(
//            mcp_grammarpartCurrent->mp_grammarpartRightChild  ) ;
        m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.push_back(
            GrammarPartPointerAndParseLevel(
            m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
            mp_grammarpartRightChild ,
//            m_wParseLevel + 1
            m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel + 1
            )
            ) ;
      }
      if( //mcp_grammarpartCurrent->mp_grammarpartLeftChild
          m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
          mp_grammarpartLeftChild  )
      {
        ++ m_wParseLevel ;
//        mcp_grammarpartCurrent =
//            mcp_grammarpartCurrent->mp_grammarpartLeftChild ;
        m_grammarpartpointer_and_parselevelCurrent.SetGrammarPartPointerAndParseLevel(
          m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
          mp_grammarpartLeftChild ,
          //m_wParseLevel
          ++ m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel
          ) ;
        //Can be used as a callback method in subclasses of this class to keep
        // track of the current parse tree path:
        //    clause
        //   /      \  _ (if "\"= last char:g++ warning:"multi-line comment")
        // subject verb
        // before node "clause", now at node "subject"-> parse tree path is
        //  "clause"->"subject" now
        ParseTreePathAdded() ;
      }
      else //(Left) leave of parse tree found.
      {
    //          setFinalNodesOfCurrentParseLevel.insert( ) ;
        m_stdset_p_grammarpartProcessedYet.insert(
//            mcp_grammarpartCurrent
            m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart
          ) ;
        //Can be used as a callback method in subclasses of this class.
        LeaveFound() ;
//        UnprocessedHighestLevelNodeFound() ;
  //      if( bRightChild )
  //      { //At this point: both the left and the right child have been drawn.
  //        //So the parent can be drawn at their horizontal center now.
  //      }
        if( //! m_stdvec_p_grammarpartRightNodeToProcess.empty()
          ! m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.empty()
          )
        {
          //TODO handle correctly if just 1 child exists:
          // e.g. subject
          //      /
          //    subj_enum_ele
          //     /
          // def_article_noun
          //Then more than 1 node must be popped from the current parse tree
          // path: "def_article_moun" and "subj_enum_ele"

          //Can be used as a callback method in subclasses of this class to keep
          // track of the current parse tree path:
          //    clause
          //   /      \   (if "\"= last char:g++ warning:"multi-line comment")
          // subject verb
          // before at node "subject", now at node "clause"-> parse tree path is
          //  "clause" now
          ParseTreePathPopped() ;
        //std::vector<GrammarPart *>::iterator iter =
//          mcp_grammarpartCurrent =
//          m_stdvec_p_grammarpartRightNodeToProcess.
//          //"Returns a read/write reference to the data at the last
//          //*  element of the %vector."
//          back() ;
          m_grammarpartpointer_and_parselevelCurrent =
            m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.back() ;
          //Can be used as a callback method in subclasses of this class to keep
          // track of the current parse tree path:
          //    clause
          //   /      \    (if "\"= last char:g++ warning:"multi-line comment")
          // subject verb
          // before at node "subject", now at node "verb"-> parse tree path is
          //  "clause"->"verb" now
//          ParseTreePathAdded() ;
          CurrentNodeIsLastAddedRightChild() ;

  //      bRightChild = true ;
  //      vecCurrentParseTreePath.push_back(p_grammarpart) ;
        //p_grammarpart = *iter ;
        DEBUG_COUT ( "DrawLeavesOfParseTree--popping " <<
          mp_parsebyrise->GetGrammarPartName(
            //mcp_grammarpartCurrent->m_wGrammarPartID
            m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
              m_wGrammarPartID
              )
//          << mcp_grammarpartCurrent
          << m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart
    //            << " current size=" << m_stdvec_p_grammarpartRightNodeToProcess.size()
          << "\n" )
        //e.g.: def_article_noun
        //        /       \ (if "\"= last char:g++ warning:"multi-line comment")
        //     article  noun
        //when at article: parse level is 1,
        //because "noun" was the last pushed it is the next
    //        m_wParseLevel = m_stdvec_p_grammarpartRightNodeToProcess.size() + 1 ;
//        m_stdvec_p_grammarpartRightNodeToProcess.pop_back() ;
        m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.pop_back() ;
        //std::cout.flush() ;
        //usleep(100000) ;
    //          -- m_wParseLevel ;
        }
        else
          //mcp_grammarpartCurrent = NULL ;
          m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart = NULL ;
      }
      DEBUG_COUT ( "DrawLeavesOfParseTree--"
          << " nodes to process size="
//          << m_stdvec_p_grammarpartRightNodeToProcess.size()
          << m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.size()
          << "\n" )
    //        usleep(1000000) ;
    }
    while( //! m_stdvec_p_grammarpartRightNodeToProcess.empty()
      //m_wParseLevel > 0
      // mcp_grammarpartCurrent
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart
     ) ;
  }

  void DirectingLeavesMultipleIterTraverser::
    ProcessNextParseTreeLevelDirectingRoot(
//    const GrammarPart * p_grammarpart
    )
  {
    bool bProcessedYet = false ;
    m_grammarpartpointer_and_parselevelCurrent = GrammarPartPointerAndParseLevel(
        mp_grammarpartStartNode , 0 ) ;
    //for the root node.
    ParseTreePathAdded() ;
    GrammarPartPointerAndParseLevel * p_grammarpartpointerandparselevel =
        & m_grammarpartpointer_and_parselevelCurrent ;
    std::set<const GrammarPart*>::iterator iter_p_grammarpartLeft,
      iter_p_grammarpartRight ;
//    m_stdvec_p_grammarpartRightNodeToProcess.clear();
    m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.clear() ;
    WORD m_wCurrentParseTreeLevel = 0 ;
    bool bGetNextUnprocessedRightChild = false ;
    do
    {
      bGetNextUnprocessedRightChild = false ;
      iter_p_grammarpartRight = m_stdset_p_grammarpartProcessedYet.find(
        //p_grammarpart->mp_grammarpartRightChild
        p_grammarpartpointerandparselevel->m_p_grammarpart->
        mp_grammarpartRightChild ) ;
      iter_p_grammarpartLeft = m_stdset_p_grammarpartProcessedYet.find(
        //p_grammarpart->mp_grammarpartLeftChild
        p_grammarpartpointerandparselevel->m_p_grammarpart->
          mp_grammarpartLeftChild
          ) ;

      if( //node has been processed yet/ in "processed yet" list .
        iter_p_grammarpartLeft != m_stdset_p_grammarpartProcessedYet.end()
        //          //If 2 already processed children found.
        //          //e.g. "the car"
        //          //       \  /
        //          //     def_article_noun
        //          //when at "def_article_noun"
        //&& iterRight != m_stdset_p_grammarpartProcessedYet.end()
        )
      {
        bProcessedYet = iter_p_grammarpartRight !=
            m_stdset_p_grammarpartProcessedYet.end() ;
  //      bAtLeast1ItemDrawn = true ;
        if( //right child is processed yet.
          bProcessedYet
          //right iterator may be NULL:
          //   subj_or_obj_ee
          //           |
          // definite_article_noun <- left child,  no right child
//          || *iter_p_grammarpartRight == NULL
          || ! p_grammarpartpointerandparselevel->m_p_grammarpart->
          mp_grammarpartRightChild
          )
        {
          //The children inside the container are not needed anymore now because
          //its parent will be inserted.
          //Also searching
          //inside the container the next time is faster is less elements are
          //inside it.
          m_stdset_p_grammarpartProcessedYet.erase(iter_p_grammarpartLeft) ;

          if( //*iter_p_grammarpartRight
              bProcessedYet )
            m_stdset_p_grammarpartProcessedYet.erase(iter_p_grammarpartRight) ;
          UnprocessedHighestLevelNodeFound() ;
          bGetNextUnprocessedRightChild = true ;
          m_stdset_p_grammarpartProcessedYet.insert( //p_grammarpart
            //mp_grammarpartLeftChild->m_p_grammarpart
            p_grammarpartpointerandparselevel->m_p_grammarpart ) ;
        }
        else
        {
          if( //p_grammarpart->mp_grammarpartRightChild
              p_grammarpartpointerandparselevel->m_p_grammarpart->
              mp_grammarpartRightChild
            )
          {
//            p_grammarpartpointerandparselevel =
//              p_grammarpartpointerandparselevel->m_p_grammarpart->
//              mp_grammarpartRightChild ;
//            p_grammarpartpointerandparselevel->m_p_grammarpart =
//                p_grammarpartpointerandparselevel->m_p_grammarpart->
////                mp_grammarpartLeftChild ;
//                mp_grammarpartRightChild ;
//            p_grammarpartpointerandparselevel->m_wParseLevel ++ ;
            m_grammarpartpointer_and_parselevelCurrent.
              SetGrammarPartPointerAndParseLevel(
                p_grammarpartpointerandparselevel->m_p_grammarpart->
                mp_grammarpartRightChild
                ,
                m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel + 1
                ) ;
            p_grammarpartpointerandparselevel =
                & m_grammarpartpointer_and_parselevelCurrent ;
            ++ m_wCurrentParseTreeLevel ;
            ParseTreePathAdded() ;
          }
          else
            bGetNextUnprocessedRightChild = true ;
        }
      }
      else //left node has not been processed yet.
      {
//        iterRight = m_stdset_p_grammarpartProcessedYet.find(
//            //p_grammarpart->mp_grammarpartRightChild
//          p_grammarpartpointerandparselevel->m_p_grammarpart->
//          mp_grammarpartRightChild
//          ) ;
        if( //is not in list-> not processed yet
          iter_p_grammarpartRight == m_stdset_p_grammarpartProcessedYet.end()
          //If grammar part pointer is NOT NULL (May be NULL !).
//          && *iterRight
          && p_grammarpartpointerandparselevel->m_p_grammarpart->
          mp_grammarpartRightChild
          )
        {
          //Add to process possible children of the right nodes later.
//            m_stdvec_p_grammarpartRightNodeToProcess.push_back( //p_grammarpart->mp_grammarpartRightChild
//              p_grammarpart->mp_grammarpartRightChild  ) ;
          m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.push_back(
            GrammarPartPointerAndParseLevel(
            //p_grammarpart->mp_grammarpartRightChild
            p_grammarpartpointerandparselevel->m_p_grammarpart->
              mp_grammarpartRightChild
            , //m_wParseLevel
            p_grammarpartpointerandparselevel->m_wParseLevel
            )
            ) ;
          //This is important for keeping track of the current parse tree
          //path:
          RightChildAdded(m_wCurrentParseTreeLevel) ;
    //      wStringWidthRightChild = GetGrammarPartNameExtent( wxpaintdc,
    //          p_grammarpart->mp_grammarpartRightChild,
    //          wxstrGrammarPartName ).GetWidth() ;
        }
        if( //p_grammarpart->mp_grammarpartLeftChild
            p_grammarpartpointerandparselevel->m_p_grammarpart->
            mp_grammarpartLeftChild
          )
        {
//          iterLeft = m_stdset_p_grammarpartProcessedYet.find(
//              //p_grammarpart->mp_grammarpartLeftChild
//            p_grammarpartpointerandparselevel->m_p_grammarpart->
//            mp_grammarpartLeftChild
//            ) ;
//          if( //is not in list
//              iterLeft == m_stdset_p_grammarpartProcessedYet.end() )
//          {
//            p_grammarpart = p_grammarpart->mp_grammarpartLeftChild ;
//            p_grammarpartpointerandparselevel->m_p_grammarpart =
//                p_grammarpartpointerandparselevel->m_p_grammarpart->
//                mp_grammarpartLeftChild ;
//            p_grammarpartpointerandparselevel->m_wParseLevel ++ ;
            m_grammarpartpointer_and_parselevelCurrent.
              SetGrammarPartPointerAndParseLevel(
                p_grammarpartpointerandparselevel->m_p_grammarpart->
                mp_grammarpartLeftChild ,
                p_grammarpartpointerandparselevel->m_wParseLevel + 1
                ) ;
            p_grammarpartpointerandparselevel =
                & m_grammarpartpointer_and_parselevelCurrent ;
            ++ m_wCurrentParseTreeLevel ;
            ParseTreePathAdded() ;
      //      wStringWidthLeftChild = GetGrammarPartNameExtent( wxpaintdc,
      //          p_grammarpart->mp_grammarpartLeftChild ,
      //          wxstrGrammarPartName ).GetWidth() ;
      //      bRightChild = false ;
        //        ++ m_wParseLevel ;
        }
        else
          //Left child was processed, but not right child yet:
          // the car        sucks
          //  \  /           /
          // def_article_noun /
          //             \ /
          //           clause
          // when at "clause" and "the", "car" , and "def_article_noun" have
          // been processed yet in previous iterations.
        {
          m_stdset_p_grammarpartProcessedYet.insert( //p_grammarpart
            //mp_grammarpartLeftChild->m_p_grammarpart
            p_grammarpartpointerandparselevel->m_p_grammarpart ) ;
          LeaveFound() ;
          bGetNextUnprocessedRightChild = true ;
        }
//        }
//        else //(Left) leave of parse tree found.
//        {
//        }
//        bGetNextUnprocessedRightChild = true ;
      }
//      else ////Not both left and right child that have already been  found.
//      { //Left child has not already been processed found.
//
//
//      } //Not both left and right child that have already been drawn found.
      if( bGetNextUnprocessedRightChild )
      {
        do
        {
          //Do NOT follow children for the grammar part! they have been drawn yet.
  //              p_grammarpart = GetNextRightGrammarPartNotProcessedYet() ;
          p_grammarpartpointerandparselevel =
            GetNextRightGrammarPartNotProcessedYet() ;
          //No more unprocessed right grammar parts.
          if( //p_grammarpart == NULL
            p_grammarpartpointerandparselevel == NULL )
            //break ;
            return ;
          iter_p_grammarpartRight = m_stdset_p_grammarpartProcessedYet.find(
              //p_grammarpart
            p_grammarpartpointerandparselevel->m_p_grammarpart ) ;
          // assert( iterRight == m_stdset_p_grammarpartProcessedYet.end() ) ;

          //e.g. for "the anal and the vacuum cleaner and" the 1st right node
          //            \  /    /
          //         def_noun
          //
          //would be for "anal". But this has been processed yet in the 1st pass
          //, so continue with the next right child.
        } while( //while the node exists in the list
            iter_p_grammarpartRight != m_stdset_p_grammarpartProcessedYet.end()
            ) ;
        //Copy from found node.
        m_grammarpartpointer_and_parselevelCurrent =
            * p_grammarpartpointerandparselevel ;
  //            p_grammarpart wparselevel
        m_wCurrentParseTreeLevel = p_grammarpartpointerandparselevel->
          m_wParseLevel ;
        CurrentNodeIsLastAddedRightChild() ;
      }
      DEBUG_COUT ( "DrawNextParseTreeLevelDirectingRoot-- "
        << " nodes to process size="
//        << m_stdvec_p_grammarpartRightNodeToProcess.size()
        << m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.size()
        << "\n" )
    //        usleep(1000000) ;
    }
    while( //! m_stdvecNodesToProcess.empty()
      //m_wParseLevel > 0
      //p_grammarpart
      //mp_grammarpartLeftChild->m_p_grammarpart
//      p_grammarpartpointerandparselevel->m_p_grammarpart
      p_grammarpartpointerandparselevel
      ) ;
  }
}
