/* 
 * File:   ConditionsAndTranslation.hpp
 * Author: Stefan
 *
 * Created on 5. April 2010, 23:13
 */

#ifndef _CONDITIONSANDTRANSLATION_HPP
#define	_CONDITIONSANDTRANSLATION_HPP

#include "Condition.hpp"

//For a translation to German there may be at least 1 condition:
// e.g. for translating the English definite article "the" to the German
// "das" the German noun that belongs to it must be singular and the gender
// must be "saechlich":

class ConditionsAndTranslation
{
public:
  enum compare_type { equals } ;
  BYTE m_byCompareType ;
  //The person index that belongs to the conditions. e.g. person index
  //for 3rd person singular belongs to a singular noun.
  //This is important for getting the correct finite verb form when translating(?)
  BYTE m_byPersonIndex ;
  //std::vector<std::string> conditions ;
  std::vector<Condition> m_conditions ;
  //The attribute name to use for translation. this name must exist as key
  //value in the string -> AttributeTypeAndPosAndSize mapping.
  //If this name was found as key value in the map then the AttributeTypeAndPosAndSize
  // describes the attribute type (string etc.), index within (string) array
  // of the VocabularyAndTranslation object:
  //  e.g. map entry "singular" -> AttributeTypeAndPosAndSize(
  //    -type:string -> use the string array
  //    -language:English
  //    -type:index 0 -> use value from string array index 0, e.g. "man"
  //    )
  //Then for translation: just return the string value at index 0 of English strings.
  std::string //m_stdstrGermanTranslation ;
    m_stdstrAttributeName ;
  //If the token is not stored at the vocabulary info in memory
  //(e.g.:  |noun record|
  //         singular:  |  car
  //          plural    |  cars
  //), like for "the" -> "die" then this is the translation to take.
  std::string m_stdstrGermanTranslation ;
  //
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
//    int i = 0 ;
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

