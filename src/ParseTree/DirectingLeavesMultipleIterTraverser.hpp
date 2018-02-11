/*
 * DirectingLeavesMultipleIterTraverser.h
 *
 *  Created on: May 6, 2010
 *      Author: Stefan
 */

#ifndef DIRECTINGLEAVESMULTIPLEITERTRAVERSER_H_
#define DIRECTINGLEAVESMULTIPLEITERTRAVERSER_H_

#include <set> //class std::set
#include <vector> //class std::vector

/** Forward declarations. */
class GrammarPart ;
namespace VTrans3 {
class BottomUpParser; }

typedef unsigned short WORD ;

//class GrammarPartPointerAndParseLevel
#include <Parse/GrammarPartPointerAndParseLevel.hpp>

//A parse tree is a tree of the allowed syntax.
namespace ParseTreeTraverser
{
  /** Traverses a parse tree from root to the leaves in multiple iterations.*/
  class DirectingLeavesMultipleIterTraverser
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
    VTrans3::BottomUpParser * mp_bottomUpParser;
  //  std::vector<const GrammarPart *> m_stdvec_p_grammarpartRightNodeToProcess ;
    std::vector<GrammarPartPointerAndParseLevel>
      m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess ;
    //The list of nodes that have been processed yet for a previous/ higher
    //parse tree level: if for e.g.
    // the car
    //   \ /
    // def_article_noun
    // both "the" and "car" have been processed yet, the traversal for the
    //next level should only reach until "def_article_noun" and neither "car"
    //nor "the".
    std::set<const GrammarPart*> m_stdset_p_grammarpartProcessedYet ;
    WORD m_wParseLevel ;
    //e.g. gives the poss. to clear containers that track the current parse tree
    //path.
    //Must be "virtual" because this is a callback method for subclasses.
    virtual void BeforeBeginAtRoot() {} ;

    //Called after reaching a left leaf.
    //Must be "virtual" because this is a callback method for subclasses.
    virtual void CurrentNodeIsLastAddedRightChild() {} ;

    DirectingLeavesMultipleIterTraverser(
        const GrammarPart * p_grammarpart ,
        VTrans3::BottomUpParser *);
    virtual
    ~DirectingLeavesMultipleIterTraverser();
//    void GetGrammarPartPathAsGrammarPartIDvector( std::vector<WORD> & ) ;
//    WORD * GetGrammarPartPathAsGrammarPartIDarray() ;
//    const GrammarPart * GetNextRightGrammarPartNotProcessedYet() ;
    GrammarPartPointerAndParseLevel * GetNextRightGrammarPartNotProcessedYet() ;

    //Must be "virtual" because this is a callback method for subclasses.
    virtual void ParseTreePathAdded() {} ;
    virtual void EnteredLeftChild() { };
    virtual void EnteredRightChild() { };
    virtual void MemorizedRightChildToProcess() {};
    //Must be "virtual" because this is a callback method for subclasses.
    virtual void ParseTreePathPopped() { } ;
    inline void PopElementsTillNextRightChild(//WORD wNumberOfElementsToPop
      GrammarPartPointerAndParseLevel *
        p_grammarpartpointerandparselevelRightChild
      ) ;
    inline void ProcessLastAddedRightNode() ;
    /** This method is useful e.g. for drawing a parse tree beginning with leaves. */
    void ProcessLeavesOfParseTree() ;
    /** This method is useful e.g. for drawing a parse tree beginning with leaves. */
    void ProcessNextParseTreeLevelDirectingRoot(
        //const GrammarPart * p_grammarpart
        ) ;
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
    //The traverser traverses the tree until 1 or 2 processed node(s) was/ were
    //found: e.g.
    //
    //    def_noun    <-when arrived here: "the" and "car" were processed-> call
    //     / \           "UnprocessedHighestLevelNodeFound"
    //   the car  <-both were processed yet.
    //
    //This is useful for e.g. drawing the parse tree.
    virtual void UnprocessedHighestLevelNodeFound() {}
  };
}

#endif /* DIRECTINGLEAVESMULTIPLEITERTRAVERSER_H_ */
