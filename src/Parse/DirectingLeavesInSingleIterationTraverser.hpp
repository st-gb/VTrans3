/*
 * DirectingLeavesInSingleIterationTraverser.hpp
 *
 *  Created on: 25.09.2011
 *      Author: Stefan
 */

#ifndef DIRECTINGLEAVESTRAVERSER_HPP_
#define DIRECTINGLEAVESTRAVERSER_HPP_

#include <vector> //class std::vector

#include "GrammarPartPointerAndParseLevel.hpp"

class GrammarPart ;
class ParseByRise ;

typedef unsigned short WORD ;

//A parse tree is a tree of the allowed syntax.
namespace ParseTreeTraverser
{
  //Traverses the parse tree (every node) in 1 iteration.
  //One application for this is the creation of an XML tree representing
  //the parse tree: therefore subclass this class an implement:
  // -CurrentNodeIsLastAddedRightChild() for an opening XML tag for a right child
  // -ParseTreePathAdded() for an opening XML tag for a left child
  // -ParseTreePathPopped() for an closing XML tag
  class DirectingLeavesInSingleIterationTraverser
  {
  public:
    enum directionOfCurrentNodeRelatedToParentNode
    {
      RootNodeOrMiddle,
      Left,
      Right
    };
    static char * s_ar_chDirection [] ;
    bool m_bTraverseTree ; //Set to "false" to terminate further traversing.
    //This node can be used in subclassed when callback methods are called:
    // then this means that the affect node is this node.
  //  const GrammarPart * mcp_grammarpartCurrent ;
    GrammarPartPointerAndParseLevel m_grammarpartpointer_and_parselevelCurrent ;
    const GrammarPart * mp_grammarpartStartNode ;
    ParseByRise * mp_parsebyrise ;
  //  std::vector<const GrammarPart *> m_stdvec_p_grammarpartRightNodeToProcess ;
    std::vector<GrammarPartPointerAndParseLevel>
      m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess ;
    WORD m_wParseLevel ;

    //e.g. gives the possibility to clear containers that track the current parse tree
    //path.
    //Must be "virtual" because this is a callback method for subclasses.
    virtual void BeforeBeginAtRoot() {} ;

    //Called after reaching a left leaf.
    //Must be "virtual" because this is a callback method for subclasses.
    virtual void CurrentNodeIsLastAddedRightChild() {} ;
    DirectingLeavesInSingleIterationTraverser(
        const GrammarPart * p_grammarpart ,
        ParseByRise * p_parsebyrise );
    virtual
    ~DirectingLeavesInSingleIterationTraverser();
//    void GetGrammarPartPathAsGrammarPartIDvector( std::vector<WORD> & ) ;
//    WORD * GetGrammarPartPathAsGrammarPartIDarray() ;
//    const GrammarPart * GetNextRightGrammarPartNotProcessedYet() ;
    GrammarPartPointerAndParseLevel * GetNextRightGrammarPartNotProcessedYet() ;

    //Must be "virtual" because this is a callback method for subclasses.
    virtual void ParseTreePathAdded() {} ;
    //Must be "virtual" because this is a callback method for subclasses.
    virtual void ParseTreePathPopped() { } ;
    inline void PopElementsTillNextRightChild(//WORD wNumberOfElementsToPop
      GrammarPartPointerAndParseLevel *
        p_grammarpartpointerandparselevelRightChild
      ) ;
    inline void ProcessLastAddedRightNode() ;
    void TraverseParseTree() ;
    //e.g.:
    //   the car and
    //    \  /   /
    // def_noun /
    //        \/
    //    def_noun_conj
    // we start at "def_noun_conj", go left as long as possible, then go
    // right.
    //  -at "def_noun_conj": in RightChildAdded(0) of
    //    this class' subclass store pointer to node for ["and", 0]
    //    in a map.
    //  -at "def_noun": in RightChildAdded(1) of
    //    this class' subclass store pointer to node for ["car", 1]
    //    in a map.
    // at "the" the parse tree path is:
    //   def_noun_conj->def_noun->article
    //  1.because this is a leaf: next node is the node that was the
    //     last added to this list of the unprocessed right children:
    //     right child node "car".
    //  2. search in the node2parselevel map for "car"
    //     ->parselevel "1" -> shorten parse tree path to
    //     "def_noun_conj->def_noun", the current parse tree is
    //     def_noun_conj->def_noun->noun
    // then at "car": def_noun_conj->def_noun->noun
    //   then go to "and", the topmost in the stack of right children.
    //     : if we kept track by implementing RightChildAdded(WORD)
    //       we could have stored a mapping node->parse level
    //       and so we could have shortened "def_noun_conj->def_noun->noun"
    //      to "def_noun_conj" (because parse level was 0/1 at
    //       "def_noun_conj" when "and" was added to the right nodes
    //        not processed yet
    //   then at "and": def_noun_conj->conj
    //Must be "virtual" because this is a callback method for subclasses.
    virtual void RightChildAdded( unsigned short) {} ;
    void Traverse() ;
    //Must be "virtual" because this is a callback method for subclasses.
    virtual void LeaveFound() {}
  };
}

#endif /* DIRECTINGLEAVESTRAVERSER_HPP_ */
