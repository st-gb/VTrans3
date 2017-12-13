/** DupliceParseTree.hpp
 *  Created on: 21.03.2014
 *      Author: mr.sys  */

#ifndef DUPLICEPARSETREE_HPP_
#define DUPLICEPARSETREE_HPP_

//#include "DirectingLeavesMultipleIterTraverser.hpp"
#include "KeepTrackOfCurrentParseTreePath.hpp"
#include <vector> //class std::vector
#include <hardware/CPU/fastest_data_type.h>//typedef fastestUnsignedDataType

/** Forward declarations */
class GrammarPart;

namespace ParseTreeTraverser
{

  class DuplicateParseTree
    : public //DirectingLeavesMultipleIterTraverser
      KeepTrackOfCurrentParseTreePath
  {
//#ifdef DEBUG
    static fastestUnsignedDataType s_numInstances;
//#endif
    std::vector<GrammarPart *> m_stdvec_p_grammarpartRightNodeCopyToProcess;
    GrammarPart * m_p_currentParent;
  public:
    GrammarPart * m_p_rootOfDuplicatedSubTree;
    DuplicateParseTree(
      const GrammarPart * p_grammarpartStartNode
      , VTrans3::BottomUpParser &
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
