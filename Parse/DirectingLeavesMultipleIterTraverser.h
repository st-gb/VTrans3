/*
 * DirectingLeavesMultipleIterTraverser.h
 *
 *  Created on: May 6, 2010
 *      Author: Stefan
 */

#ifndef DIRECTINGLEAVESMULTIPLEITERTRAVERSER_H_
#define DIRECTINGLEAVESMULTIPLEITERTRAVERSER_H_

#include <set>
#include <vector>

class GrammarPart ;
class ParseByRise ;

typedef unsigned short WORD ;

class GrammarPartPointerAndParseLevel
{
public:
  GrammarPart * m_p_grammarpart ;
  WORD m_wParseLevel ;
  GrammarPartPointerAndParseLevel()
    : m_p_grammarpart( NULL )
    , m_wParseLevel( 0 )
  {

  }
  GrammarPartPointerAndParseLevel(
    const GrammarPart * p_grammarpart ,
    WORD wParseLevel
    )
    : m_p_grammarpart( (GrammarPart *) p_grammarpart )
    , m_wParseLevel( wParseLevel )
  {

  }
  void SetGrammarPartPointerAndParseLevel(
    GrammarPart * p_grammarpart ,
    WORD wParseLevel
    )
  {
    m_p_grammarpart = p_grammarpart ;
    m_wParseLevel = wParseLevel ;
  }
} ;

namespace ParseTreeTraverser
{
  class DirectingLeavesMultipleIterTraverser
  {
  public:
    //This node can be used in subclassed when callback methods are called:
    // then this means that the affect node is this node.
  //  const GrammarPart * mcp_grammarpartCurrent ;
    GrammarPartPointerAndParseLevel m_grammarpartpointer_and_parselevelCurrent ;
    const GrammarPart * mp_grammarpartStartNode ;
    ParseByRise * mp_parsebyrise ;
  //  std::vector<const GrammarPart *> m_stdvec_p_grammarpartRightNodeToProcess ;
    std::vector<GrammarPartPointerAndParseLevel>
      m_stdvec_p_grammarpart_and_parselevelRightNodeToProcess ;
    std::set<const GrammarPart*> m_stdset_p_grammarpartProcessedYet ;
    WORD m_wParseLevel ;
    virtual void BeforeBeginAtRoot() {} ;
    virtual void CurrentNodeIsLastAddedRightChild() {} ;
    DirectingLeavesMultipleIterTraverser(
        const GrammarPart * p_grammarpart ,
        ParseByRise * p_parsebyrise );
    virtual
    ~DirectingLeavesMultipleIterTraverser();
//    const GrammarPart * GetNextRightGrammarPartNotProcessedYet() ;
    GrammarPartPointerAndParseLevel * GetNextRightGrammarPartNotProcessedYet() ;
    virtual void ParseTreePathAdded() {} ;
    virtual void ParseTreePathPopped() { } ;
    void ProcessLeavesOfParseTree() ;
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
    virtual void RightChildAdded( unsigned short) {} ;
    void Traverse() ;
    //virtual because this is the callback method for the subclasses.
    virtual void LeaveFound() {}
    virtual void UnprocessedHighestLevelNodeFound() {}
  };
}

#endif /* DIRECTINGLEAVESMULTIPLEITERTRAVERSER_H_ */
