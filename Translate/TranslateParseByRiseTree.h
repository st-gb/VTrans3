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

class GrammarPart ;
class ParseByRise ;

class AttributeTypeAndPosAndSize
{
  enum attribute_type { string, bit } ;
  enum language { German, English } ;
public:
  //attribute is a string or some bits
  BYTE m_byType ;
  //String or bit index
  WORD m_wIndex ;
  //bit length
  WORD m_wLenght ;
  AttributeTypeAndPosAndSize( BYTE byType, WORD wIndex, WORD wLenght)
  {
    m_byType = byType ;
    m_wIndex = wIndex ;
    m_wLenght = wLenght ;
  }
} ;

class ConditionsAndTranslation
{
  enum compare_type { equals } ;
  BYTE m_byCompareType ;
  std::vector<std::string> conditions ;
};

class TranslationRule
{
public:
  WORD * m_ar_wElements ;
  WORD m_wNumberOfElements ;
  TranslationRule( WORD * ar_wElements , WORD wNumberOfElements)
  {
    m_ar_wElements = ar_wElements ;
    m_wNumberOfElements = wNumberOfElements ;
  }
  ~TranslationRule( )
  {
    DEBUG_COUT("~TranslationRule (destructor)\n")
    //Release mem.
    delete [] m_ar_wElements ;
  }
  bool operator < ( const TranslationRule & r) const
  {
    if( m_wNumberOfElements < r.m_wNumberOfElements )
      return true ;
    else if( m_wNumberOfElements > r.m_wNumberOfElements )
      return false ;
    else //same number of elements
    {
      for( WORD wIndex = 0 ; wIndex < m_wNumberOfElements ; ++ wIndex )
      {
        if( m_ar_wElements[ wIndex ] < r.m_ar_wElements[ wIndex ] )
          return true ;
        else if ( m_ar_wElements[ wIndex ] > r.m_ar_wElements[ wIndex ] )
          return false ;
      }
      //here: Contents are identical.
      return false ;
    }
  }
};

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
  std::string GetSyntaxTreePathAsName( //ParseByRise & r_parsebyrise
   std::vector<WORD> & ) ;
  std::string GetSyntaxTreePathAsName(WORD * ar_wGrammarPartPath,
      WORD wLength) ;
  TranslateParseByRiseTree(ParseByRise & r_parsebyrise );
  ~TranslateParseByRiseTree();
  void Translate( ParseByRise & r_parsebyrise ) ;
  bool TranslationRuleApplies() ;
};

#endif /* TRANSLATEPARSEBYRISETREE_H_ */
