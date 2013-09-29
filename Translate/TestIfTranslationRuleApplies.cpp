/*
 * TestIfTranslationRuleApplies.cpp
 *
 *  Created on: 13.12.2012
 *      Author: Stefan
 */

#include "Translate/TestIfTranslationRuleApplies.hpp"
//class TranslateParseByRiseTree
//#include <wx/msw/winundef.h>
/** class ShowTranslationRulesDialog */
#include <wxWidgets/UserInterface/TranslationRules/ShowTranslationRulesDialog.hpp>
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>
#include <Translate/TranslationRule.hpp> //class TranslationRule
#include <wxWidgets/VTransApp.hpp> //wxGetApp()

namespace ParseTreeTraverser
{

  TestIfTranslationRuleApplies::TestIfTranslationRuleApplies(
    const GrammarPart * p_grammarpartStartNode
    , ParseByRise & r_parsebyrise
    , TranslateParseByRiseTree & r_translateparsebyrisetree
    )
    :
     ParseTreeTraverser:://DirectingLeavesMultipleIterTraverser(
      KeepTrackOfCurrentParseTreePath(
      p_grammarpartStartNode ,
      r_parsebyrise
      )
//    , m_r_parsebyrise (r_parsebyrise)
    , mr_translateparsebyrisetree( r_translateparsebyrisetree )
    , m_bTranslationRuleApplies(false)
  {

  }

  TestIfTranslationRuleApplies::~TestIfTranslationRuleApplies()
  {
    // TODO Auto-generated destructor stub
  }

  void TestIfTranslationRuleApplies::LeaveFound()
  {
    BYTE byPersonIndex ;
    GrammarPart * p_grammarpartWithConsecutiveID ;
    std::string stdstrTranslation ;
    TranslationRule * p_translationrule = wxGetApp().
      m_p_showtranslationrulesdialog->m_p_translationruleSelected;
    std::string std_strTranslationRuleSyntaxTreePath = p_translationrule->
      m_syntaxtreepathCompareWithCurrentPath.GetAs_std_string();
    if( mr_translateparsebyrisetree.TranslationRuleApplies(
        stdstrTranslation ,
        byPersonIndex ,
        m_vec_wGrammarPartIDcurrentParsePath ,
        m_stdvector_p_grammarpartCurrentParseTreePath ,
  //        wConsecutiveID
        p_grammarpartWithConsecutiveID
        , p_translationrule
        , * wxGetApp().m_p_showtranslationrulesdialog->m_p_conditionsandtranslation
        , std_strTranslationRuleSyntaxTreePath
        )
      )
    {
      m_bTranslationRuleApplies = true;
    }
  }
} /* namespace ParseTreeTraverser */
