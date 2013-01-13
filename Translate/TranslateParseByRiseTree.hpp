/*
 * TranslateParseByRiseTree.h
 *
 *  Created on: 25.02.2010
 *      Author: Stefan
 */

#ifndef TRANSLATEPARSEBYRISETREE_H_
#define TRANSLATEPARSEBYRISETREE_H_

#include <map>//class std::map
#include <stdint.h> //for uint32_t
#include <vector>//class std::vector
#include <windef.h> //DWORD
//#include "SyntaxTreePath.hpp"
#include "ConditionsAndTranslation.hpp" //class ConditionsAndTranslation
//#include "TranslationRule.hpp"

//use  a power of 2 in order to enable all values at the same time
//(for different BIT indices)
#define FIRST_PERSON_SINGULAR 1 //2^0 = 1
#define SECOND_PERSON_SINGULAR 2 //2^1 = 2
#define THIRD_PERSON_SINGULAR 4 //2^2 = 4
#define FIRST_PERSON_PLURAL 8 //2^3 = 8
#define SECOND_PERSON_PLURAL 16 //2^4 = 16
#define THIRD_PERSON_PLURAL 32 //2^5 = 32

//Forward declarations
class AttributeTypeAndPosAndSize ;
class GrammarPart ;
class I_UserInterface ;
class ParseByRise ;
class TranslationAndGrammarPart ;
class TranslationAndConsecutiveID ;
class TranslationRule ;

//Translates the parse tree of type "ParseByRise"
class TranslateParseByRiseTree
{
  uint32_t m_ui32ConcatenationID;
  WORD m_wParseLevel ;
  std::map<WORD,DWORD>
    m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName ;
  std::map<std::string,std::vector<std::string> >
    m_stdmap_stdstrTranslationArrayName2vec_stdstrArrayElements ;
  std::vector<GrammarPart *> m_stdvecNodesToProcess ;
public:
  bool m_bTranslationRuleApplies;
  I_UserInterface & mr_i_userinterface ;
  ParseByRise * mp_parsebyrise ;
private:
  //Sores the current path within the syntax tree of grammar part IDs.
  //So it can be compared whether a translation rule exists for it.
  //E.g. a translation rule "subject.def_article_noun.noun" exists.
  //Example: at first it starts at "clause" . Then "clause.subject"
  // "clause.subject.def_article_noun" -> "clause.subject.def_article_noun.noun".
  //the rule applies to the subtree "subject.def_article_noun.noun"
  std::vector<WORD> m_stdvec_wCurrentGrammarPartPath ;
  std::vector<GrammarPart *> m_stdvec_p_grammarpartPath ;
public:
  //e.g. define std::string "Eng_singular",
  // AttributeTypeAndPosAndSize ( string, 0 ,1 )
  //what means that the string data inside the VocabularyAndTranslation object
  //  at string array index 0 is defined as "English_singular".
  std::map<std::string,AttributeTypeAndPosAndSize>
    m_stdmap_AttrName2VocAndTranslAttrDef ;
  std::map<TranslationRule *,ConditionsAndTranslation>
    m_stdmap_p_translationrule2ConditionsAndTranslation ;
  typedef std::multimap<uint32_t,TranslationRule *>
    std_multimap_uint32_t2p_translationrule;
  std_multimap_uint32_t2p_translationrule
    m_std_multimapConcatenationID2p_translationrule;
//private:
  //use a multimap because more than 1 translation rule may have the same syntax
  //tree path: the rules for translating a singular article for an object for
  //each noun gender male, neuter and female have all the same syntax tree path:
  //  "3rdPersPluralClauseWith1Obj.obj.*.definite_article_singular."
  //  "definite_article"
  //Else using a std::map because of "TranslationRule::operator <" only
  // 1 rule can with the inserted.
  std::multimap<
    //Use object rather then a pointer because then the "TranslationRule::<"
    //operator that is important for applying translation rules is used.
    TranslationRule,ConditionsAndTranslation>
    m_stdmap_translationrule2ConditionsAndTranslation ;

  std::vector<std::vector<GrammarPart *> >
    m_std_vector_std_vector_p_grammarpartCoveringMostTokensAtTokenIndex;
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
  void FreeMemoryForVocAttrDefs()
  {
    m_stdmap_AttrName2VocAndTranslAttrDef.clear() ;
  }
  void FreeMemoryForTranslationRules() ;
  std::string GetSyntaxTreePathAsName( //ParseByRise & r_parsebyrise
   const std::vector<WORD> & ) ;
  std::string GetSyntaxTreePathAsName(WORD * ar_wGrammarPartPath,
      WORD wLength) ;
  std::string GetTranslationEquivalent(
    const ConditionsAndTranslation & r_cnt
    , const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath
    ) ;
  void SetSameConsecutiveIDforConnectedTranslationRules(
      TranslationRule * p_translationrule,
      const std::vector<GrammarPart * > & r_stdvec_p_grammarpartPath
      );
  TranslateParseByRiseTree(ParseByRise & r_parsebyrise ,
    I_UserInterface & r_i_userinterface );
  ~TranslateParseByRiseTree();

  //Syntax (">>class name<<::*>>identifier<<") from
  //http://stackoverflow.com/questions/4832275/c-typedef-member-function-signature-syntax
  typedef void (TranslateParseByRiseTree::*ProcessParseTree_type )(
//    TranslateParseByRiseTree *,
  //  std::vector<GrammarPart *>::const_iterator
  //    c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex
    GrammarPart *
    , std::vector<std::vector<TranslationAndGrammarPart> > &
      r_stdvec_stdvecTranslationAndGrammarPart
    ) ;

  void ProcessParseTree(
    std::vector<std::string> & r_stdvec_stdstrWholeTransl
  //  //A vector of sentences that each contains a vector of words.
  //  , std::vector<std::vector<TranslationAndGrammarPart> > &
  //    r_stdvec_stdvecTranslationAndGrammarPart
    //A vector of sentences that begin at the same token index
    // (sentences that begin at the same token index:
    // vector of sentences that each contains a vector of words).
    , std::vector <std::vector <std::vector <TranslationAndGrammarPart> > > &
      r_stdvec_stdvec_stdvecTranslationAndGrammarPart
  //  , std::vector<std::vector<TranslationAndConsecutiveID> > &
  //    r_stdvec_stdvecTranslationAndConsecutiveID
    , ProcessParseTree_type pfnProcessParseTree
    );

  void Translate( ParseByRise & r_parsebyrise
//    , std::string & stdstrWholeTransl
    , std::vector<std::string> & r_stdvec_stdstrWholeTransl
//    //The outer vector is for the different translation possibilities ;
//    , std::vector<std::vector<TranslationAndGrammarPart> > &
//      r_stdvec_stdvecTranslationAndGrammarPart
    //A vector of sentences that begin at the same token index
    // (sentences that begin at the same token index:
    // vector of sentences that each contains a vector of words).
    , std::vector <std::vector <std::vector <TranslationAndGrammarPart> > > &
      r_stdvec_stdvec_stdvecTranslationAndGrammarPart
//    , std::vector<std::vector<TranslationAndConsecutiveID> > &
//      r_stdvec_stdvecTranslationAndConsecutiveID
    ) ;
#ifdef COMPILE_AS_EXECUTABLE
  void TestIfTranslationRuleApplies(
    //    std::vector<GrammarPart *>::const_iterator
    //      c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex
    GrammarPart * p_grammarpart
    //A vector of sentences that each contains a vector of words.
    , std::vector<std::vector<TranslationAndGrammarPart> > &
      r_stdvec_stdvecTranslationAndGrammarPart
    );
#endif //#ifdef COMPILE_AS_EXECUTABLE
  void TranslateParseTree(
//    std::vector<GrammarPart *>::const_iterator
//      c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex
    GrammarPart * p_grammarpart
    //A vector of sentences that each contains a vector of words.
    , std::vector<std::vector<TranslationAndGrammarPart> > &
      r_stdvec_stdvecTranslationAndGrammarPart
    );
  bool TranslationRuleApplies(
    std::string & r_stdstrTranslation
    , BYTE & r_byPersonIndex
    // So it can be used with data from outside this class.
    , const std::vector<WORD> & r_stdvec_wGrammarPartPath
    , const std::vector<GrammarPart * > & r_stdvec_p_grammarpartPath
//    , WORD & r_wConsecutiveID
    , //const
    GrammarPart * & p_grammarpart
    ) ;
  bool TranslationRuleApplies(
    std::string & r_stdstrTranslation
    , BYTE & r_byPersonIndex
    // So it can be used with data from outside this class.
    , const std::vector<WORD> & r_stdvec_wGrammarPartPath
    , const std::vector<GrammarPart * > & r_stdvec_p_grammarpartPath
//    , WORD & r_wConsecutiveID
    , //const
    GrammarPart * & p_grammarpart
    , TranslationRule * p_translationrule
    , const ConditionsAndTranslation & r_cnt
    , const std::string & std_strTranslationRuleSyntaxTreePath
    ) ;
};

#endif /* TRANSLATEPARSEBYRISETREE_H_ */
