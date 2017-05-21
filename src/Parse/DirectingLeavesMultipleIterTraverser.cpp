/** DirectingLeavesMultipleIterTraverser.cpp
 *  Created on: May 6, 2010
 *      Author: Stefan */

#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>
#include <Parse/GrammarPart.hpp>
#include <Parse/ParseByRise.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h> //DEBUG_COUT(...)
//#include <rest.h> //DEBUG_COUT

//TODO just for testing if log statements affect the parallel translation
#ifdef LOGN_DEBUG
  #undef LOGN_DEBUG
  #define LOGN_DEBUG(to_ostream) ; /*empty->do not log*/
#endif

namespace ParseTreeTraverser
{
  char * DirectingLeavesMultipleIterTraverser::s_ar_chDirection [] =
    { //By "(char *)" : avoid g++ error message
      //"warning: deprecated conversion from string constant to 'char*'"
      (char *) "middle", (char *) "left", (char *) "right"
    };
  DirectingLeavesMultipleIterTraverser::DirectingLeavesMultipleIterTraverser(
    const GrammarPart * p_grammarpartStartNode
    , ParseByRise * p_parsebyrise
    )
    :
    m_bTraverseTree( true ) ,
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

//  WORD * DirectingLeavesMultipleIterTraverser::
//    GetGrammarPartPathAsGrammarPartIDarray( )
//  {
//    WORD * ar_wGrammarPartID = new WORD[
//      m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.size() ];
//    if( ar_wGrammarPartID )
//    {
//      WORD wArrayIndex = 0 ;
//      std::vector<GrammarPartPointerAndParseLevel>::const_iterator c_iter =
//        m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.begin() ;
//      while( c_iter != m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.
//          end()
//          )
//      {
//        ar_wGrammarPartID[ wArrayIndex ++ ] = c_iter->m_p_grammarpart->
//          m_wGrammarPartID ;
//        ++ c_iter ;
//      }
//    }
//  }
//
//  void DirectingLeavesMultipleIterTraverser::
//    GetGrammarPartPathAsGrammarPartIDvector(
//      std::vector<WORD> & stdvec_wGrammarPartID )
//  {
////    std::vector<WORD> stdvec_wGrammarPartID ;
//    std::vector<GrammarPartPointerAndParseLevel>::const_iterator c_iter =
//      m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.begin() ;
//    while( c_iter != m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.
//        end()
//        )
//    {
//      stdvec_wGrammarPartID.push_back( c_iter->m_p_grammarpart->
//        m_wGrammarPartID ) ;
//      ++ c_iter ;
//    }
//  }

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
      //DEBUG_COUT ( "DrawLeavesOfParseTree--popping " <<
      LOGN_DEBUG(
        mp_parsebyrise->GetGrammarPartName(
//        p_grammarpart->m_wGrammarPartID
        p_grammarpartpointerandparselevel->m_p_grammarpart->m_wGrammarPartID
        )
//        << p_grammarpart
        << p_grammarpartpointerandparselevel->m_p_grammarpart
      //            << " current size=" << m_stdvecNodesToProcess.size()
        //<< "\n"
        )

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

  /** Traverses the parse tree multiple times beginning from a specific node:
  *  in this example the iterations start at "subject"
  *  subject
  *   /
  * def_article_noun
  *  /        \       (if "\"= last char:g++ warning: "multi-line comment" )
  * article  noun
  *
  * When the algorithm reaches a node
  *  -whose 2 children have been processed yet
  *     the node stores the value
  *  -whose 1 child has been processed yet, 2nd child is NULL
  *  -that is a leaf (article or noun in the ex. above)
  *    if a word class that implies a "personindex" (like "I"->1st pers. sing.)
  *    store that person index value in the GrammarPart */
  void DirectingLeavesMultipleIterTraverser::Traverse()
  {
    const GrammarPart * p_grammarpart = mp_grammarpartStartNode ;
    BeforeBeginAtRoot() ;
    ProcessLeavesOfParseTree() ;
//    m_grammarpartpointer_and_parselevelCurrent = GrammarPartPointerAndParseLevel(
//        mp_grammarpartStartNode , 0 ) ;
    if( m_bTraverseTree &&
      //If the root has not been drawn yet (for trees that have only 1 node
        //that is root and leaf at the same time ).
        //If this case is not caught this may result in an endless loop
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

  inline void DirectingLeavesMultipleIterTraverser::
    PopElementsTillNextRightChild(//WORD wNumberOfElementsToPop
      GrammarPartPointerAndParseLevel *
        p_grammarpartpointerandparselevelRightChild
      )
  {
    WORD wCurrentParseTreeLevel = m_grammarpartpointer_and_parselevelCurrent.
      m_wParseLevel ;
    GrammarPart * p_grammmarpartCurrent =
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ;
    GrammarPart * p_grammmarpartRightChild =
      p_grammarpartpointerandparselevelRightChild->m_p_grammarpart ;
    WORD wParseTreeLevelOfNextRightChild =
      //m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.back().
      p_grammarpartpointerandparselevelRightChild->
      m_wParseLevel ;
    WORD wNumberOfElementsToPop = wCurrentParseTreeLevel -
      wParseTreeLevelOfNextRightChild
      //If right child is added at level 0, it gets level 1; if then left
      //child is leave it als has level 1; 1-1 = 0, but 1 element should be
      //poppped, so add a value of "1".
      + 1 ;
    LOGN_DEBUG(//"DirectingLeavesMultipleIterTraverser::"
      //"PopElementsTillNextRightChild()--"
      "should pop " << wNumberOfElementsToPop << " elements from "
      << mp_parsebyrise->GetGrammarPartName(
        p_grammmarpartCurrent->m_wGrammarPartID )
      << " (" << p_grammmarpartCurrent << ",level:" << wCurrentParseTreeLevel
      << ") to "
      << mp_parsebyrise->GetGrammarPartName(
        p_grammmarpartRightChild->m_wGrammarPartID )
      << "(" << p_grammmarpartRightChild << ",level:"
      << wParseTreeLevelOfNextRightChild << ")."
        )
//    if( wNumberOfElementsToPop )
    do
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

  //      bRightChild = true ;
  //      vecCurrentParseTreePath.push_back(p_grammarpart) ;
      //p_grammarpart = *iter ;

  //        DEBUG_COUT
//      DEBUGN( "DrawLeavesOfParseTree--popping " <<
//        mp_parsebyrise->GetGrammarPartName(
//          //mcp_grammarpartCurrent->m_wGrammarPartID
//          m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
//            m_wGrammarPartID
//            )
//    //          << mcp_grammarpartCurrent
//        << m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart
//    //            << " current size=" << m_stdvec_p_grammarpartRightNodeToProcess.size()
//        << "\n" )
    }
    while( -- wNumberOfElementsToPop ) ;
  }

  inline void DirectingLeavesMultipleIterTraverser::ProcessLastAddedRightNode()
  {
    //Not every node has 2 child nodes: it may be just 1 child node:
    //definite_article singular_noun
    //               \ /
    //    definite_article_singular
    //               |
    //          article_singular <- has just 1 child node.

    //Leave found->all left branches/ nodes from root to this leave were
    //processed -> Process the right node next to this leave node in
    //directing the root node.
  //        UnprocessedHighestLevelNodeFound() ;
  //      if( bRightChild )
  //      { //At this point: both the left and the right child have been drawn.
  //        //So the parent can be drawn at their horizontal center now.
  //      }
    if( //! m_stdvec_p_grammarpartRightNodeToProcess.empty()
      ! m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.empty()
      )
    {
      PopElementsTillNextRightChild(//wNumberOfElementsToPop
        & m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.back()
        ) ;

      m_grammarpartpointer_and_parselevelCurrent =
        m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.
        //"Returns a read/write reference to the data at the last
        //*  element of the %vector."
        back() ;

      //Can be used as a callback method in subclasses of this class to keep
      // track of the current parse tree path:
      //    clause
      //   /      \    (if "\"= last char:g++ warning:"multi-line comment")
      // subject verb
      // before at node "subject", now at node "verb"-> parse tree path is
      //  "clause"->"verb" now
  //          ParseTreePathAdded() ;
      CurrentNodeIsLastAddedRightChild() ;
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

  void DirectingLeavesMultipleIterTraverser::ProcessLeavesOfParseTree()
  {
    LOGN_DEBUG(//"DirectingLeavesMultipleIterTraverser::ProcessLeavesOfParseTree() "
      //FULL_FUNC_NAME << "--"
      "begin")
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
      LOGN_DEBUG( //FULL_FUNC_NAME << "--"
        "m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart:"
        << m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart
        )
      if( m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart != NULL )
      {
        LOGN_DEBUG( " m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->"
            "mp_grammarpartRightChild:"
          << m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
            mp_grammarpartRightChild
          << " m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->"
            "mp_grammarpartLeftChild:"
          << m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
            mp_grammarpartLeftChild )
        //TODO_ SIGSEV here
        if( //mcp_grammarpartCurrent->mp_grammarpartRightChild
            m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
            mp_grammarpartRightChild
          )
        {
  //#ifdef COMPILE_WITH_LOG
          WORD wParseTreeLevelRightChild =
            m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel + 1 ;
          GrammarPart * p_grammarpartRightChild =
            m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
            mp_grammarpartRightChild ;

          LOGN_DEBUG(//"DirectingLeavesMultipleIterTraverser::"
            //"ProcessLeavesOfParseTree()--"
            "Adding " << p_grammarpartRightChild
            << "(" << mp_parsebyrise->GetGrammarPartName(
              p_grammarpartRightChild->m_wGrammarPartID) << ",level:"
              << wParseTreeLevelRightChild << ")"
            )
          //Add to process possible children of the right nodes later.
  //        m_stdvec_p_grammarpartRightNodeToProcess.push_back(
  //            mcp_grammarpartCurrent->mp_grammarpartRightChild  ) ;
          m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.push_back(
            GrammarPartPointerAndParseLevel(
              p_grammarpartRightChild ,
  //            m_wParseLevel + 1
              wParseTreeLevelRightChild
              )
            ) ;
//          MemorizedRightChildToProcess();
          RightChildAdded(wParseTreeLevelRightChild);
  //#endif
        }
          if( //mcp_grammarpartCurrent->mp_grammarpartLeftChild
              m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
              mp_grammarpartLeftChild  )
          {
            ++ m_wParseLevel ;
    //        mcp_grammarpartCurrent =
    //            mcp_grammarpartCurrent->mp_grammarpartLeftChild ;
            m_grammarpartpointer_and_parselevelCurrent.
              SetGrammarPartPointerAndParseLevel(
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
            EnteredLeftChild();
          }
          else //(Left) leave of parse tree found.
          {
        //          setFinalNodesOfCurrentParseLevel.insert( ) ;
            m_stdset_p_grammarpartProcessedYet.insert(
    //            mcp_grammarpartCurrent
                m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart
              ) ;
            //Can be used as a callback method in subclasses of this class.
            LOGN_DEBUG(//"DirectingLeavesMultipleIterTraverser::"
              //"ProcessLeavesOfParseTree()--"
              "LeaveFound" )
            LeaveFound() ;
            //The pointer may be set to NULL by TransformTreeTraverser: the tree is modified--
            //do not continue to process, else SIGSEV in
            //PopElementsTillNextRightChild(...) occured
            if( m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart )
              ProcessLastAddedRightNode() ;
          }
      }
      //DEBUG_COUT ( "DrawLeavesOfParseTree--" <<
      LOGN_DEBUG(
        " nodes to process size="
//          << m_stdvec_p_grammarpartRightNodeToProcess.size()
        << m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.size()
        //<< "\n"
        )
    //        usleep(1000000) ;
    }
    while( //! m_stdvec_p_grammarpartRightNodeToProcess.empty()
      //m_wParseLevel > 0
      // mcp_grammarpartCurrent
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart
     ) ;
  }

  //This method is e.g. useful for drawing a level of parse tree nodes.
  void DirectingLeavesMultipleIterTraverser::
    ProcessNextParseTreeLevelDirectingRoot(
//    const GrammarPart * p_grammarpart
    )
  {
    LOGN_DEBUG(//"DirectingLeavesMultipleIterTraverser::"
      //"ProcessNextParseTreeLevelDirectingRoot() "
      "begin")
    bool bProcessedYet = false ;
    m_grammarpartpointer_and_parselevelCurrent =
      GrammarPartPointerAndParseLevel( mp_grammarpartStartNode , 0 ) ;
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
          //Or no right child (->only a superclass grammar rule).
          || ! p_grammarpartpointerandparselevel->m_p_grammarpart->
          mp_grammarpartRightChild
          )
        {
          //The children inside the container are not needed anymore now because
          //its parent will be inserted.
          //Also searching
          //inside the container the next time is faster the less elements are
          //inside it, e.g. for:
          //"the" "car"
          //    \ /
          //def_article_noun
          // -> delete "the" and "car" from the "processed yet" list.
          m_stdset_p_grammarpartProcessedYet.erase(iter_p_grammarpartLeft) ;

          if( //*iter_p_grammarpartRight
              bProcessedYet )
            m_stdset_p_grammarpartProcessedYet.erase(iter_p_grammarpartRight) ;
          //Call the callback funktion that might be overridden in a
          //subclass if this class.
          UnprocessedHighestLevelNodeFound() ;
          bGetNextUnprocessedRightChild = true ;
          m_stdset_p_grammarpartProcessedYet.insert( //p_grammarpart
            //mp_grammarpartLeftChild->m_p_grammarpart
            p_grammarpartpointerandparselevel->m_p_grammarpart ) ;
        }
        else//->not(Right child is processed yet or right child is NULL).
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
            EnteredRightChild();
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
          GrammarPart * p_grammarpartRightChild =
            p_grammarpartpointerandparselevel->m_p_grammarpart->
            mp_grammarpartRightChild ;
          WORD wParseTreeLevelRightChild = p_grammarpartpointerandparselevel->
            m_wParseLevel + 1 ;
          LOGN_DEBUG(//"DirectingLeavesMultipleIterTraverser::ProcessNextParseTree"
            //"LevelDirectingRoot()--"
            "Adding " << p_grammarpartRightChild
            << "(" << mp_parsebyrise->GetGrammarPartName(
              p_grammarpartRightChild->m_wGrammarPartID) << ",level:"
            << wParseTreeLevelRightChild << ")"
            )
          //Add to process possible children of the right nodes later.
//            m_stdvec_p_grammarpartRightNodeToProcess.push_back( //p_grammarpart->mp_grammarpartRightChild
//              p_grammarpart->mp_grammarpartRightChild  ) ;
          m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.push_back(
              GrammarPartPointerAndParseLevel(
              //p_grammarpart->mp_grammarpartRightChild
              p_grammarpartRightChild
              , //m_wParseLevel
              wParseTreeLevelRightChild
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
            EnteredLeftChild();
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
          LOGN_DEBUG(//"DirectingLeavesMultipleIterTraverser::"
            //"ProcessNextParseTreeLevelDirectingRoot()--"
            "LeaveFound" )
          LeaveFound() ;
          bGetNextUnprocessedRightChild = true ;
        }
//        }
//        else //(Left) leave of parse tree found.
//        {
//        }
//        bGetNextUnprocessedRightChild = true ;
      } ////left node has not been processed yet.
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

        PopElementsTillNextRightChild(//wNumberOfElementsToPop
          p_grammarpartpointerandparselevel
          ) ;

        //Copy from found node.
        m_grammarpartpointer_and_parselevelCurrent =
            * p_grammarpartpointerandparselevel ;
  //            p_grammarpart wparselevel
        m_wCurrentParseTreeLevel = p_grammarpartpointerandparselevel->
          m_wParseLevel ;
        CurrentNodeIsLastAddedRightChild() ;
      }
      //DEBUG_COUT ( "DrawNextParseTreeLevelDirectingRoot-- " <<
      LOGN_DEBUG(
        " nodes to process size="
//        << m_stdvec_p_grammarpartRightNodeToProcess.size()
        << m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess.size()
        //<< "\n"
        )
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
