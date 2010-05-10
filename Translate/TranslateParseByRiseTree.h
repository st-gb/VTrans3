/*
 * TranslateParseByRiseTree.h
 *
 *  Created on: 25.02.2010
 *      Author: Stefan
 */

#ifndef TRANSLATEPARSEBYRISETREE_H_
#define TRANSLATEPARSEBYRISETREE_H_

#include <map>
#include <vector>
#include <windef.h> //DWORD
#include "SyntaxTreePath.hpp"
#include "ConditionsAndTranslation.hpp"
//#include "TranslationRule.hpp"

class GrammarPart ;
class ParseByRise ;
class TranslationRule ;

#define FIRST_PERSON_SINGULAR 1 //2^0 = 1
#define SECOND_PERSON_SINGULAR 2 //2^1 = 2
#define THIRD_PERSON_SINGULAR 4 //2^2 = 4
#define FIRST_PERSON_PLURAL 8 //2^3 = 8
#define SECOND_PERSON_PLURAL 16 //2^4 = 16
#define THIRD_PERSON_PLURAL 32 //2^5 = 32

//This class models an easy access to the data in a VocabularyAndTranslation
//object.
//So one can define an attribute name, bit start etc. with an object of this
//class. E.g. define "Eng_singular", index "0", length 1.
//And then when used.
class AttributeTypeAndPosAndSize
{
public:
  enum attribute_type { string, bit } ;
  enum language { German, English } ;
  BYTE m_byLanguage ;
  //attribute is a string or some bits (=whether the indedx refers to the
  // string or byte array)
  BYTE m_byAttrDataType ;
  BYTE m_byWordClass ; //e.g. ID for word class "noun",
  //String or bit index
  WORD m_wIndex ;
  //bit length of the attribute data.
  WORD m_wLenght ;
  AttributeTypeAndPosAndSize( BYTE byType, WORD wIndex, WORD wLenght,
    BYTE byWordClass , BYTE byLanguage )
  {
    m_byLanguage = byLanguage ;
    m_byWordClass = byWordClass ;
    m_byAttrDataType = byType ;
    m_wIndex = wIndex ;
    m_wLenght = wLenght ;
  }
} ;

//Translates the parse tree of type "ParseByRise"
class TranslateParseByRiseTree
{
  WORD m_wParseLevel ;
  std::map<WORD,DWORD>
    m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName ;
  std::map<std::string,std::vector<std::string> >
    m_stdmap_stdstrTranslationArrayName2vec_stdstrArrayElements ;
  std::vector<GrammarPart *> m_stdvecNodesToProcess ;
  ParseByRise * mp_parsebyrise ;
  //Sores the current path within the syntax tree of grammar part IDs.
  //So it can be compared whether a translation rule exists for it.
  //E.g. a translation rule "subject.noun_construct.noun" exists.
  //Example: at first it starts at "clause" . Then "clause.subject"
  // "clause.subject.noun_construct" -> "clause.subject.noun_construct.noun".
  //the rule applies to the subtree "subject.noun_construct.noun"
  std::vector<WORD> m_stdvec_wGrammarPartPath ;
  std::vector<GrammarPart *> m_stdvec_p_grammarpartPath ;

  //e.g. define std::string "Eng_singular",
  // AttributeTypeAndPosAndSize ( string, 0 ,1 )
  //what means that the string data inside the VocabularyAndTranslation object
  //  at string array index 0 is defined as "English_singular".
  std::map<std::string,AttributeTypeAndPosAndSize>
    m_stdmap_AttrName2VocAndTranslAttrDef ;
  std::map<TranslationRule *,ConditionsAndTranslation>
    m_stdmap_translationrule2ConditionsAndTranslation ;
public:
  void AddVocAndTranslDefinition(
    //e.g. "noun.German.plural"
    //const std::string & r_stdstrWordClassAndAttributeName ,
    const std::string & r_stdstrWordClass ,
    //e.g. "noun.German.plural"
    const std::string & r_stdstrWordClassAndAttributeName ,
    //->the definition either applies to English or German.
    BYTE byLanguage ,
    //attribute is a string or some bits
    BYTE byType ,
    //String or bit index
    WORD wIndex ,
    //bit length
    WORD wLenght
    ) ;
  void AddTranslationArray(
    const std::string & r_stdstrTranslationArrayName,
    const std::string & r_stdstrNewlineTerminatedArrayElements ) ;
  void AddTranslationRule(
    const std::string & r_stdstrSyntaxTreePath ,
    const std::string & r_stdstrGermanTranslationRule ,
    const std::string & r_
    ) ;
  void AddTranslationRule(
    TranslationRule * p_tr,
    const ConditionsAndTranslation & rc_cnt ) ;
  bool AllConditionsMatch( const ConditionsAndTranslation & r_cnt
    // So it can be used with data from outside this class.
    //, const std::vector<WORD> & r_stdvec_wGrammarPartPath
    , const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath
    ) ;
  std::string GetSyntaxTreePathAsName( //ParseByRise & r_parsebyrise
   const std::vector<WORD> & ) ;
  std::string GetSyntaxTreePathAsName(WORD * ar_wGrammarPartPath,
      WORD wLength) ;
  std::string GetTranslationEquivalent(
    const ConditionsAndTranslation & r_cnt
    , const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath
    ) ;
  TranslateParseByRiseTree(ParseByRise & r_parsebyrise );
  ~TranslateParseByRiseTree();
  void Translate( ParseByRise & r_parsebyrise ) ;
  bool TranslationRuleApplies(
    std::string & r_stdstrTranslation
    , BYTE & r_byPersonIndex
    // So it can be used with data from outside this class.
    , const std::vector<WORD> & r_stdvec_wGrammarPartPath
    , const std::vector<GrammarPart * > & r_stdvec_p_grammarpartPath
    ) ;
};

#endif /* TRANSLATEPARSEBYRISETREE_H_ */
