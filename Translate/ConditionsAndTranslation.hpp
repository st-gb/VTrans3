/* 
 * File:   ConditionsAndTranslation.hpp
 * Author: Stefan
 *
 * Created on 5. April 2010, 23:13
 */

#ifndef _CONDITIONSANDTRANSLATION_HPP
#define	_CONDITIONSANDTRANSLATION_HPP

#include "Condition.hpp"

//For a translation to German there may be 1 least 1 condition:
// e.g. for translating the English definite article "the" to the German
// "das" the German noun that belongs to it must be singular and the gender
// must be "saechlich".
class ConditionsAndTranslation
{
public:
  enum compare_type { equals } ;
  BYTE m_byCompareType ;
  //The person index that belongs to the conditions. e.g. person index
  //for 3rd person singular belongs to a singular noun.
  BYTE m_byPersonIndex ;
  //std::vector<std::string> conditions ;
  std::vector<Condition> m_conditions ;
  std::string //m_stdstrGermanTranslation ;
    m_stdstrAttributeName ;
  std::string m_stdstrGermanTranslation ;
  SyntaxTreePath m_syntaxtreepath ;
  void AddCondition(Condition & cond)
  {
    m_conditions.push_back(cond) ;
  }
  ConditionsAndTranslation()
    : m_byPersonIndex (0)
  {

  }
  #ifdef _DEBUG
  ~ConditionsAndTranslation()
  {
    int i = 0 ;
    DEBUG_COUT("~ConditionsAndTranslation\n")
  }
  #endif //#ifdef _DEBUG

  void SetSyntaxTreePath(const char * p_ch_SyntaxTreePath
     , ParseByRise * p_parsebyrise )
  {
    std::string stdstrSyntaxTreePath (p_ch_SyntaxTreePath) ;
    m_syntaxtreepath.CreateGrammarPartIDArray( //r_stdstrSyntaxTreePath
        stdstrSyntaxTreePath , p_parsebyrise  ) ;
  }
};

#endif	/* _CONDITIONSANDTRANSLATION_HPP */

