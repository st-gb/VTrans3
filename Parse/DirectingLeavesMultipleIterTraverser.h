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
    GrammarPartPointerAndParseLevel m_grammarpartpointer_and_parselevel ;
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
    virtual void RightChildAdded( unsigned short) {} ;
    void Traverse() ;
    //virtual because this is the callback method for the subclasses.
    virtual void LeaveFound() {}
    virtual void UnprocessedHighestLevelNodeFound() {}
  };
}

#endif /* DIRECTINGLEAVESMULTIPLEITERTRAVERSER_H_ */
