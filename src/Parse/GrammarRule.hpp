/** File:   GrammarRule.hpp
 * Author: sg
 * Created on 5. November 2017, 14:58 */

#ifndef GRAMMARRULE_HPP
#define GRAMMARRULE_HPP

class GrammarRule
{
public:
  //e.g. grammar part ID for "the" and for "noun" refer to the same
  //superordinate grammar part ID for "def_article_noun".
  WORD m_wSuperordinateGrammarRuleID ;
  WORD m_wRightChildGrammarPartID ;
  GrammarRule( WORD wGrammarRuleID , WORD wSuperordinateGrammarRuleID )
  {
    m_wRightChildGrammarPartID = wGrammarRuleID ;
    m_wSuperordinateGrammarRuleID = wSuperordinateGrammarRuleID ;
  }
};

#endif /* GRAMMARRULE_HPP */

