/*
 * DirectingLeavesInSingleIterationTraverser.cpp
 *
 *  Created on: 25.09.2011
 *      Author: Stefan
 */


/*
 * DirectingLeavesInSingleIterationTraverser.cpp
 *
 *  Created on: May 6, 2010
 *      Author: Stefan
 */

#include <ParseTree/DirectingLeavesInSingleIterationTraverser.hpp>
#include <Parse/GrammarPart.hpp>
#include <Parse/ParseByRise.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h> //DEBUG_COUT(...)
//#include <rest.h> //DEBUG_COUT

namespace ParseTreeTraverser
{
  char * DirectingLeavesInSingleIterationTraverser::s_ar_chDirection [] =
    { //By explicitely casting to "(char *)" : avoid Linux g++ warning:
      // "deprecated conversion from string constant to ‘char*’"
      (char *) "middle",
      //By explicitely casting to "(char *)" : avoid Linux g++ warning:
      // "deprecated conversion from string constant to ‘char*’"
      (char *) "left",
      //By explicitely casting to "(char *)" : avoid Linux g++ warning:
      // "deprecated conversion from string constant to ‘char*’"
      (char *) "right"
    };
  DirectingLeavesInSingleIterationTraverser::DirectingLeavesInSingleIterationTraverser(
    const GrammarPart * p_grammarpartStartNode
    , const VTrans3::BottomUpParser * p_bottomUpParser
    )
    :
    m_bTraverseTree( true ) ,
    m_grammarpartpointer_and_parselevelCurrent (
      p_grammarpartStartNode , 0 )
    , mp_grammarpartStartNode ( p_grammarpartStartNode )
    , m_wParseLevel( 0)
  {
    mp_parsebyrise = p_bottomUpParser ;
  }

  DirectingLeavesInSingleIterationTraverser::~DirectingLeavesInSingleIterationTraverser()
  {
    // TODO Auto-generated destructor stub
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
  void DirectingLeavesInSingleIterationTraverser::Traverse()
  {
//    const GrammarPart * p_grammarpart = mp_grammarpartStartNode ;
    BeforeBeginAtRoot() ;
    TraverseParseTree() ;
//    m_grammarpartpointer_and_parselevelCurrent = GrammarPartPointerAndParseLevel(
//        mp_grammarpartStartNode , 0 ) ;
  }

  inline void DirectingLeavesInSingleIterationTraverser::
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
    LOGN_DEBUG(//"DirectingLeavesInSingleIterationTraverser::PopElementsTillNextRightChild()--"
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

  inline void DirectingLeavesInSingleIterationTraverser::
    ProcessLastAddedRightNode()
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
    {
      WORD wCurrentParseTreeLevel = m_grammarpartpointer_and_parselevelCurrent.
        m_wParseLevel ;
      do
      {
        ParseTreePathPopped();
      }while( wCurrentParseTreeLevel
        //Use post decrement because if in parse level "1" then it is the
        //2nd parse level (-> 2 parse level must be popped)
        --);
      //mcp_grammarpartCurrent = NULL ;
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart = NULL ;
    }
  }

  void DirectingLeavesInSingleIterationTraverser::TraverseParseTree()
  {
    LOGN_DEBUG("begin")
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
//#ifdef COMPILE_WITH_LOG
        WORD wParseTreeLevelRightChild =
          m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel + 1 ;
        GrammarPart * p_grammarpartRightChild =
          m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
          mp_grammarpartRightChild ;
        LOGN_DEBUG("Adding " << p_grammarpartRightChild
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
      }
      else //(Left) leave of parse tree found.
      {
    //          setFinalNodesOfCurrentParseLevel.insert( ) ;
        //Can be used as a callback method in subclasses of this class.
        LOGN_DEBUG("LeaveFound" )
        LeaveFound() ;
        //The pointer may be set to NULL by TransformTreeTraverser: the tree is modified--
        //do not continue to process, else SIGSEV in
        //PopElementsTillNextRightChild(...) occured
        if( m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart )
          ProcessLastAddedRightNode() ;
      }
      LOGN_DEBUG( "" << " nodes to process size="
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
}
