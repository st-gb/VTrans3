/* 
 * File:   ConditionsAndTranslation.hpp
 * Author: Stefan
 *
 * Created on 5. April 2010, 23:13
 */

#ifndef _CONDITIONSANDTRANSLATION_HPP
#define	_CONDITIONSANDTRANSLATION_HPP

#include "Condition.hpp" //class Condition
#include <map>//class std::map
#include <vector> //class std::vector
#include <preprocessor_macros/logging_preprocessor_macros.h> //DEBUG_COUT(...)
//class AttributeTypeAndPosAndSize
#include <Translate/AttributeTypeAndPosAndSize.hpp>
//#include <VocabularyInMainMem/LetterTree/VocabularyAndTranslation.hpp>

class GrammarPart ;

//Function pointer for a function that is able to change a string.
//That is important e.g. for changing "Männer" to "Männern" for
//"I trust the men."
//A function is more univerally usable than a Vorschrift/ Syntax: it can
// use a more variety because it is not restricted to pre-defined rules but
// used machine code.
typedef void ( * pfnTransformString) (std::string & r_stdstr ) ;

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
  pfnTransformString m_pfn_TransformString ;
  //A translation rule may have multiple / more than 1 condition:
  //e.g. for translating "the" to "das" into German,
  //-the German translation must be singular (do not rely on the English noun
  //  to be singular because e.g. "The glasses are red" = "Die Brille ist rot.")
  //-the German gender must be "saechlich"
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

  //for all objects the same
  static ParseByRise * sp_parsebyrise ;
  static std::map<std::string, //VocabularyAndTranslation
    AttributeTypeAndPosAndSize > *
    sp_stdmap_AttrName2VocAndTranslAttrDef ;
  void AddCondition(Condition & cond)
  {
    m_conditions.push_back(cond) ;
  }
  bool AllConditionsMatch(
    // So it can be used with data from outside this class.
    //, const std::vector<WORD> & r_stdvec_wGrammarPartPath
    const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath
    ) const ;
  ConditionsAndTranslation()
    : m_byPersonIndex (0)
    , m_pfn_TransformString( NULL)
  {

  }
  #ifdef _DEBUG
  ~ConditionsAndTranslation()
  {
//    int i = 0 ;
    DEBUG_COUT("~ConditionsAndTranslation\n")
  }
  #endif //#ifdef _DEBUG

  std::string GetTranslationEquivalent(
    //A leaf contain the needed pointer to a VocabularyAndTranslation object.
    const GrammarPart * const p_grammarpartLeaf
    ) const ;
  void SetSyntaxTreePath(const char * p_ch_SyntaxTreePath
     , ParseByRise * p_parsebyrise )
  {
    std::string std_strUnknownGrammarPartID;
    std::string stdstrSyntaxTreePath (p_ch_SyntaxTreePath) ;
    m_syntaxtreepath.CreateGrammarPartIDArray( //r_stdstrSyntaxTreePath
        stdstrSyntaxTreePath , p_parsebyrise, std_strUnknownGrammarPartID ) ;
  }
};

#endif	/* _CONDITIONSANDTRANSLATION_HPP */

