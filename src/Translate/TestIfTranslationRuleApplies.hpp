/*
 * TestIfTranslationRuleApplies.hpp
 *
 *  Created on: 13.12.2012
 *      Author: Stefan
 */

#ifndef TESTIFTRANSLATIONRULEAPPLIES_HPP_
#define TESTIFTRANSLATIONRULEAPPLIES_HPP_

////Base class DirectingLeavesMultipleIterTraverser
//#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>
#include <ParseTree/KeepTrackOfCurrentParseTreePath.hpp>

/** Forward declarations. (faster than to include header files) */
class TranslateParseByRiseTree;
namespace VTrans3 { class BottomUpParser; }

namespace ParseTreeTraverser
{
  /** Class can be used e.g. to test if a certain translation rule matches the
   * current parse tree.*/
  class TestIfTranslationRuleApplies
    : public //DirectingLeavesMultipleIterTraverser
      KeepTrackOfCurrentParseTreePath
  {
    TranslateParseByRiseTree & mr_translateparsebyrisetree;
  public:
    bool m_bTranslationRuleApplies;
    TestIfTranslationRuleApplies(
      const GrammarPart * p_grammarpartStartNode
      , VTrans3::BottomUpParser &
      , TranslateParseByRiseTree & r_translateparsebyrisetree
      );
    virtual
    ~TestIfTranslationRuleApplies();
    void LeaveFound();
  };

} /* namespace ParseTreeTraverser */
#endif /* TESTIFTRANSLATIONRULEAPPLIES_HPP_ */
