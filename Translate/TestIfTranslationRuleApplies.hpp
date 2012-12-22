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
#include <Parse/KeepTrackOfCurrentParseTreePath.hpp>

class TranslateParseByRiseTree;

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
      , ParseByRise & r_parsebyrise
      , TranslateParseByRiseTree & r_translateparsebyrisetree
      );
    virtual
    ~TestIfTranslationRuleApplies();
    void LeaveFound();
  };

} /* namespace ParseTreeTraverser */
#endif /* TESTIFTRANSLATIONRULEAPPLIES_HPP_ */
