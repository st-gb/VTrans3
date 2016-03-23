/*
 * DupliceParseTree.hpp
 *
 *  Created on: 21.03.2014
 *      Author: mr.sys
 */

#ifndef DUPLICEPARSETREE_HPP_
#define DUPLICEPARSETREE_HPP_

//#include "DirectingLeavesMultipleIterTraverser.hpp"
#include "KeepTrackOfCurrentParseTreePath.hpp"
#include <vector> //class std::vector

class GrammarPart;

namespace ParseTreeTraverser
{

  class DuplicateParseTree
    : public //DirectingLeavesMultipleIterTraverser
      KeepTrackOfCurrentParseTreePath
  {
    std::vector<GrammarPart *> m_stdvec_p_grammarpartRightNodeCopyToProcess;
    GrammarPart * m_p_currentParent;
  public:
    GrammarPart * m_p_rootOfDuplicatedSubTree;
    DuplicateParseTree(
      const GrammarPart * p_grammarpartStartNode
      , ParseByRise & r_parsebyrise
  //    , TranslateParseByRiseTree & r_translateparsebyrisetree
      );
    virtual
    ~DuplicateParseTree();

//    void ParseTreePathAdded();
    void CurrentNodeIsLastAddedRightChild();
    void EnteredLeftChild();
    void EnteredRightChild();
    void RightChildAdded(WORD wParseTreeLevelRightChild);
//    void RightChildAdded( unsigned short);
    void MemorizedRightChildToProcess();
  };

} /* namespace ParseTreeTraverser */

#endif /* DUPLICEPARSETREE_HPP_ */
