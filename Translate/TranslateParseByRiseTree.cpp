/*
 * TranslateParseByRiseTree.cpp
 *
 *  Created on: 25.02.2010
 *      Author: Stefan
 */
#include <Attributes/EnglishWord.hpp> //for EnglishWord::UnknownWord
#include <Parse/ParseByRise.hpp> //class ParseByRise
#include <UserInterface/I_UserInterface.hpp> //for I_UserInterface::Message(...)
//class AttributeTypeAndPosAndSize
#include <Translate/AttributeTypeAndPosAndSize.hpp>
//class ParseTreeTraverser::TestIfTranslationRuleApplies
#include <Translate/TestIfTranslationRuleApplies.hpp>
//#include <Translate/SummarizePersonIndex.hpp>//class SummarizePersonIndex
//header file of this TranslateParseByRiseTree class
#include <Translate/TranslateParseByRiseTree.hpp>
#include <Translate/Translationrule.hpp>//class TranslationRule
//class ParseTreeTraverser::DoTranslateTreeTraverser
#include <Translate/TranslateTreeTraverser.hpp>
//class ParseTreeTraverser::TranslatedTreeTraverser
#include <Translate/TranslatedTreeTraverser.hpp>
//class VocabularyAndTranslation
#include <VocabularyInMainMem/VocabularyAndTranslation.hpp>
#include <Xerces/ReadViaSAX2.hpp> //ReadViaSAX2InitAndTermXerces(...)
//class SAX2TranslationRuleHandler
#include <Xerces/SAX2TranslationRuleHandler.hpp>
//SUPPRESS_UNUSED_VARIABLE_WARNING(...)
#include <preprocessor_macros/suppress_unused_variable.h>

#include <string>//class std::string

std::map<std::string, AttributeTypeAndPosAndSize > *
  ConditionsAndTranslation::sp_stdmap_AttrName2VocAndTranslAttrDef ;
ParseByRise * ConditionsAndTranslation::sp_parsebyrise ;

//E.g. add "German_definite_article" , "der\ndie\ndas\ndie"
void TranslateParseByRiseTree::AddTranslationArray(
  const std::string & r_stdstrTranslationArrayName,
  const std::string & r_stdstrNewlineTerminatedArrayElements
  )
{
  std::string stdstrCurrentElement ;
  std::vector<std::string> vec ;
  WORD wStringStart = 0 ;
  WORD wNumberOfElements = 1 ;
  for( WORD i = 0 ; i < r_stdstrNewlineTerminatedArrayElements.length() ; ++ i )
  {
    if( r_stdstrNewlineTerminatedArrayElements[i] == '\n' || i ==
        r_stdstrNewlineTerminatedArrayElements.length() - 1 )
    {
      stdstrCurrentElement = r_stdstrNewlineTerminatedArrayElements.substr(
          wStringStart, i - wStringStart ) ;
      vec.push_back( stdstrCurrentElement ) ;
      ++ wNumberOfElements ;
      wStringStart = i + 1 ;
    }
  }
  m_stdmap_stdstrTranslationArrayName2vec_stdstrArrayElements.insert(
    std::pair<std::string,std::vector<std::string> >
      (r_stdstrTranslationArrayName, vec)
      );
}

//E.g.
//AddTranslationRule("subject.def_article_noun.definite_article",
//  "German_definite_article[subject.def_article_noun.noun.German_gender]")
//because all definite articles in a subject should be translated to the
// undeclinated? (Nominativ) versions "der", "die" , "das"

//AddTranslationRule("subject.def_article_noun.definite_article" ,
//  "def_article_noun.noun,translateAsSingular")

//e.g. "the news" (singular) is translated as "Nachrichten" (Plural)
//AddTranslationRule(
//  "subject.def_article_noun.definite_article" ,
//  "def_article_noun.noun.English.translateAsPlural=1" //English attribute
//  "def_article_noun.noun.German.plural") //attribute of German word.

//AddTranslationRule(
//  "subject.def_article_noun.noun.definite_article" ,
//  "noun.English.translateSingularAsPlural=1" //English attribute (condition)
//  "\"die\"") //attribute of German word.

//AddTranslationRule(
//  "subject.def_article_noun.noun.definite_article" ,
//  "noun.English.isSingular=1" //English attribute (condition)
//  "noun.English.translateSingularAsPlural=0" //English attribute (condition)
//  "noun.German.gender=0" //German attribute (condition)
//  "\"der\"") //attribute of German word.
void TranslateParseByRiseTree::AddTranslationRule(
  const std::string & r_stdstrSyntaxTreePath ,
  //const std::string & r_stdstrEnglishAttributeNameAndValue ,
  const std::string & r_stdstrConditions ,
  const std::string & r_stdstrGermanTranslation
  //  subject.def_article_noun.noun.German_gender]"
  )
{
////    TranslationRule tr(ar_wElements, vec_wElements.size() ) ;

//    //Must create on heap, else its d'tor is called.
//    TranslationRule * p_tr = new TranslationRule(ar_wElements,
//        vec_wElements.size() ) ;
      //Must create on heap, else its d'tor is called.
    try
    {
    TranslationRule * p_translation_rule = new TranslationRule(
      r_stdstrSyntaxTreePath, mp_parsebyrise ) ;

//    DEBUG_COUT( "added translation rule for syntax tree path \"" <<
//        GetSyntaxTreePathAsName( ar_wElements, vec_wElements.size() ) <<
//        "\"\n" )
    ConditionsAndTranslation conditions_and_translation ;
#ifdef _DEBUG
    std::pair<std::map<TranslationRule*,ConditionsAndTranslation>::iterator,
      bool> _pair =
#endif
    m_stdmap_p_translationrule2ConditionsAndTranslation.insert(
      std::pair<TranslationRule *,ConditionsAndTranslation>
      ( //tr
        //Create here, else ar_wElements is deleted in TranslationRule d'tor?!
        //TranslationRule(ar_wElements, vec_wElements.size() )
        p_translation_rule , conditions_and_translation
      )
      ) ;
#ifdef _DEBUG
    if( _pair.second )
    {
      const TranslationRule & r_tr = *_pair.first->first ;
      LOGN_DEBUG( "added translation rule for syntax tree path \"" <<
        GetSyntaxTreePathAsName(
          //r_tr.m_ar_wElements, r_tr.m_wNumberOfElements
          r_tr.m_syntaxtreepathCompareWithCurrentPath.m_ar_wElements,
          r_tr.m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements
        ) << "\"" /*"\n"*/
         )
    }
#endif
    }
    catch( const GetGrammarPartIDexception & c_r_getgrammarpartidexception )
    {
        mr_i_userinterface.Message(
          "Error adding translation rule for STP \"" +
          r_stdstrSyntaxTreePath + "\" : "
          "unknown grammar part ID \"" +
             c_r_getgrammarpartidexception.m_stdstr + " \""
          );
    }
    catch( std::exception & e )
    {
      LOGN_ERROR("std::exception")
    }
//  }
}

void TranslateParseByRiseTree::AddTranslationRule(
  TranslationRule * p_translationrule,
  const ConditionsAndTranslation & rc_cnt
  //const std::string & r_stdstrGermanTranslation
  //  subject.def_article_noun.noun.German_gender]"
  )
{
#ifdef _DEBUG
    std::pair<std::map<TranslationRule*,ConditionsAndTranslation>::iterator,bool>
      _pair =
#endif
    m_stdmap_p_translationrule2ConditionsAndTranslation.insert(
      std::pair<TranslationRule *,ConditionsAndTranslation>
      ( //tr
        //Create here, else ar_wElements is deleted in TranslationRule d'tor?!
        //TranslationRule(ar_wElements, vec_wElements.size() )
        p_translationrule , rc_cnt
      )
      ) ;
    m_stdmap_translationrule2ConditionsAndTranslation.insert(
      std::pair<TranslationRule,ConditionsAndTranslation>
        (
          *p_translationrule , rc_cnt
        )
      ) ;

#ifdef _DEBUG
    if( _pair.second )
    {
      const TranslationRule & r_tr = *_pair.first->first ;
      LOGN_DEBUG( "added translation rule for syntax tree path \"" <<
        GetSyntaxTreePathAsName(
          //r_tr.m_ar_wElements, r_tr.m_wNumberOfElements
          r_tr.m_syntaxtreepathCompareWithCurrentPath.m_ar_wElements,
          r_tr.m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements
          ) << "\"" /*"\n"*/
         )
    }
#endif
}

//e.g. "noun.German.plural" , AttributeTypeAndPosAndSize::string ,
//   0, 0
void TranslateParseByRiseTree::AddVocAndTranslDefinition(
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
  )
{
  LOGN_DEBUG( //"TranslateParseByRiseTree::AddVocAndTranslDefinition("
    /*FULL_FUNC_NAME << */
    r_stdstrWordClass << ","
    << r_stdstrWordClassAndAttributeName )
  WORD wGrammarPartID ;
  //e.g. get corresponding ID for word class "noun".
  if( mp_parsebyrise->GetGrammarPartID(r_stdstrWordClass,wGrammarPartID) )
  {
    LOGN_DEBUG("Found GetGrammarPartID for word class" << r_stdstrWordClass )
    AttributeTypeAndPosAndSize attributetypeandposandsize(
      byType, wIndex,
      wLenght, wGrammarPartID, byLanguage) ;
    m_stdmap_AttrName2VocAndTranslAttrDef.insert(
      std::pair<std::string, AttributeTypeAndPosAndSize>
        (
        r_stdstrWordClassAndAttributeName
        , attributetypeandposandsize
        )
      ) ;
  }
//#ifdef _DEBUG
  else
  {
    mr_i_userinterface.Message(
      "For inserting voc attr def.: word class \"" + r_stdstrWordClass
      + "\" is not avaible");
    LOGN_DEBUG( /*FULL_FUNC_NAME << */
      "Couldn't find GetGrammarPartID for word class \""
      << r_stdstrWordClass << "\"")
  }
//#endif
}

bool TranslateParseByRiseTree::AllConditionsMatch(
  const ConditionsAndTranslation & r_conditions_and_translation
  // So it can be used with data from outside this class.
  //, const std::vector<WORD> & r_stdvec_wGrammarPartPath
  , const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath
  )
{
  bool bAllConditionsMatch = false ;
//  int i = 0 ;
  //TODO implement this
//        if( r_cnt.conditions.m_byCompareType == ConditionsAndTranslation::equals
//          && AttributeTypeAndPosAndSize::English && rightof("==") equals(
//          grammarpart.string &&
//          mp_parsebyrise->psv.getTokens(
//              grammarpart.index, index2) )
  if( r_conditions_and_translation.m_conditions.size() > 0 )
  {
    GrammarPart * p_grammarpartLeaf ;
    bAllConditionsMatch = true ;
//    const Condition & cr_condition =
//      r_conditions_and_translation.m_conditions.front() ;
//    std::vector<Condition> r_vec_condition =
    const std::vector<Condition> & r_vec_condition =
      r_conditions_and_translation.m_conditions ;
    for( std::vector<Condition>::const_iterator c_iter =
        r_vec_condition.begin() ;
        c_iter != r_vec_condition.end() ;
        ++ c_iter
        )
    {
      const Condition & cr_condition = *c_iter ;
      //if( r_cond.m_syntaxtreepath.IsPartOf(m_stdvec_wCurrentGrammarPartPath) )
      //TODO get pointer to leaf node of r_cond.m_syntaxtreepath
      p_grammarpartLeaf = cr_condition.m_syntaxtreepath.GetLeaf(
        r_stdvec_p_grammarpartPath ) ;
      if( p_grammarpartLeaf &&
        p_grammarpartLeaf->m_pvocabularyandtranslation )
      {
        //if( p_grammarpartLeaf )
        {
          LOGN_DEBUG("grammar part leaf found:" <<
            mp_parsebyrise->GetGrammarPartName(
            p_grammarpartLeaf->m_wGrammarPartID) << "\n")
        }
        std::map<std::string,AttributeTypeAndPosAndSize>::const_iterator
          c_iter_stdstrSttrName2atapas =
              m_stdmap_AttrName2VocAndTranslAttrDef.find(
            cr_condition.m_stdstrAttributeName ) ;

        //The attribute name (e.g. "German_singular" exists in the map
        if( c_iter_stdstrSttrName2atapas !=
          m_stdmap_AttrName2VocAndTranslAttrDef.end()
          )
        {
          //p_grammarpartLeaf->m_psetpvocabularyandtranslation  ;
          //if( p_grammarpartLeaf->m_psetpvocabularyandtranslation )
          {
            const AttributeTypeAndPosAndSize & r_atapas =
              c_iter_stdstrSttrName2atapas->second ;
            switch( r_atapas.m_byAttrDataType )
            {
              case AttributeTypeAndPosAndSize::string :
              {
                if( r_atapas.m_byLanguage ==
                  AttributeTypeAndPosAndSize::English )
                {
                  std::string & r_stdstrAttrVal = p_grammarpartLeaf->
                    m_pvocabularyandtranslation->m_arstrEnglishWord[
                    r_atapas.m_wIndex ] ;
                  //std::string & r_stdstrTextTokens =
                  std::string stdstrTextTokens ;
                    mp_parsebyrise->GetTokensAsSpaceSeparatedString(
                    p_grammarpartLeaf->m_dwLeftmostIndex,
                    p_grammarpartLeaf->m_dwRightmostIndex ,
                    stdstrTextTokens
                    ) ;
                  if( stdstrTextTokens != ""  && stdstrTextTokens ==
                    r_stdstrAttrVal )
                  {
                    //bIdentical = true ;
                  }
                  else
                  {
                    bAllConditionsMatch = false ;
                    //break ;
                    return false ;
                  }
                }
              }
              break ;
              case AttributeTypeAndPosAndSize::bit :
                if( r_atapas.m_byLanguage ==
                  AttributeTypeAndPosAndSize::German )
                {
                  BYTE by = p_grammarpartLeaf->
                    m_pvocabularyandtranslation->m_arbyAttribute[
                    r_atapas.m_wIndex ] ;
                  if( cr_condition.m_byAttributeValue != by )
                    //bAllConditionsMatch = false ;
                    return false ;
                }
                break;
            }//switch
          }
          //if( r_cond.m_byCompareType == Condition::equals )
        }
      }
      else
        return false ;
    } //loop over all conditions
  }
  else
    //0 conditions, e.g. for "3rd person singular finite verb" grammar part
    bAllConditionsMatch = true ;
  return bAllConditionsMatch ;
}

std::string TranslateParseByRiseTree::GetTranslationEquivalent(
  const ConditionsAndTranslation & r_condition_and_translation
  , const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath
  )
{
  LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "begin" )
  bool bAllConditionsMatch = false ;
  std::string stdstr ;
//  int i = 0 ;
  //TODO implement this
//        if( r_cnt.conditions.m_byCompareType == ConditionsAndTranslation::equals
//          && AttributeTypeAndPosAndSize::English && rightof("==") equals(
//          grammarpart.string &&
//          mp_parsebyrise->psv.getTokens(
//              grammarpart.index, index2) )

  //If no simple replacement (like use "die" for "the" + "English_plural"
  if( r_condition_and_translation.m_stdstrGermanTranslation ==  "" )
  {
    bAllConditionsMatch = true ;
    //if( r_cond.m_syntaxtreepath.IsPartOf(m_stdvec_wCurrentGrammarPartPath) )
    //TODO get pointer to leaf node of r_cond.m_syntaxtreepath
    GrammarPart * p_grammarpartLeaf = r_condition_and_translation.
        m_syntaxtreepath.GetLeaf( r_stdvec_p_grammarpartPath) ;
    if( p_grammarpartLeaf &&
      p_grammarpartLeaf->m_pvocabularyandtranslation )
    {
      //if( p_grammarpartLeaf )
      {
        //DEBUG_COUT("GetTranslationEquivalent()--"
        LOGN_DEBUG(
          "grammar part leaf found:" <<
          mp_parsebyrise->GetGrammarPartName(
          p_grammarpartLeaf->m_wGrammarPartID) << "\n")
      }
      std::map<std::string,AttributeTypeAndPosAndSize>::const_iterator
        c_iter_stdstrSttrName2atapas = m_stdmap_AttrName2VocAndTranslAttrDef.find(
          r_condition_and_translation.m_stdstrAttributeName ) ;
      if( c_iter_stdstrSttrName2atapas !=
        m_stdmap_AttrName2VocAndTranslAttrDef.end() )
      {
        //p_grammarpartLeaf->m_psetpvocabularyandtranslation  ;
        //if( p_grammarpartLeaf->m_psetpvocabularyandtranslation )
        {
          const AttributeTypeAndPosAndSize & r_atapas =
            c_iter_stdstrSttrName2atapas->second ;
          switch( r_atapas.m_byAttrDataType )
          {
            case AttributeTypeAndPosAndSize::string :
            {
              LOGN_DEBUG("data type for choosing attribute value is string")
              switch( r_atapas.m_byLanguage )
              {
                case AttributeTypeAndPosAndSize::English :
                {
                  std::string & r_stdstrAttrVal = p_grammarpartLeaf->
                    m_pvocabularyandtranslation->m_arstrEnglishWord[
                    r_atapas.m_wIndex ] ;
                  //std::string & r_stdstrTextTokens =
                  return r_stdstrAttrVal ;
                }
                  break ;
                case AttributeTypeAndPosAndSize::German :
                  {
                  LOGN_DEBUG("language for choosing attribute value is "
                    "German, index:" << r_atapas.m_wIndex )
                  std::string & r_stdstrAttrVal = p_grammarpartLeaf->
                    m_pvocabularyandtranslation->m_arstrGermanWord[
                    r_atapas.m_wIndex ] ;
                  //std::string & r_stdstrTextTokens =
                  return r_stdstrAttrVal ;
                  }
                  break ;
              }
            }
            break ;
            case AttributeTypeAndPosAndSize::bit :
              if( r_atapas.m_byLanguage ==
                AttributeTypeAndPosAndSize::German )
              {
                BYTE by =
                  p_grammarpartLeaf->
                  m_pvocabularyandtranslation->m_arbyAttribute[
                  r_atapas.m_wIndex ] ;
                SUPPRESS_UNUSED_VARIABLE_WARNING(by)
              }
              break ;
          }//switch
        }
        //if( r_cond.m_byCompareType == Condition::equals )
      }
      else
//        DEBUG_COUTN("GetTranslationEquivalent()--"
        LOGN_DEBUG("attribute name" <<
          r_condition_and_translation.m_stdstrAttributeName <<
          "not found" )
    }
  }
  else
  {
    stdstr = r_condition_and_translation.m_stdstrGermanTranslation ;
  }
  return stdstr ;
}

void GetDativeFormForGermanPluralNoun( std::string & r_stdstr )
{
  WORD wStringLen = r_stdstr.length() ;
  // z.B. bei "Frauen" KEIN "n" anhaengen (because the rightmost char
  // already is an "n") .
  if( wStringLen > 0 && r_stdstr.at(wStringLen - 1) != 'n' )
    //Ich vertraue Tiere>>n<<.
    //Ich vertraue Männer>>n<<.
    r_stdstr += "n" ;
}

void GetDativeFormForGermanSingularNoun( std::string & r_stdstr )
{
  WORD wStringLen = r_stdstr.length() ;
  // "e" at the end -> "en"
  if( wStringLen > 0 && r_stdstr.at(wStringLen - 1) == 'e' )
    //Ich vertraue dem Junge>>n<<.
    r_stdstr += "n" ;
}

//void TranslateParseByRiseTree::AddAndConjunctionTranslationRule()
//{
//  ConditionsAndTranslation cntAnd ;
//  //  cntAnd.SetSyntaxTreePath( "and", mp_parsebyrise ) ;
//  cntAnd.m_stdstrGermanTranslation = "und" ;
//  //must create on heap.
//  TranslationRule * p_trAnd = new TranslationRule(
//    "and" // For this syntax tree GrammarPart path in syntax tree.
//    , mp_parsebyrise ) ;
//  AddTranslationRule(
//    //"def_article_noun.definite_article" ,
//    p_trAnd ,
//    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
//    //cond ,
//    //, ""
//    cntAnd
//  ) ;
//}

TranslateParseByRiseTree::TranslateParseByRiseTree(
  ParseByRise & r_parsebyrise
  , I_UserInterface & r_i_userinterface
  )
  :
  m_ui32ConcatenationID(//0
      1)
  , mr_i_userinterface (r_i_userinterface)
{
  //DEBUG_COUTN("TranslateParseByRiseTree(ParseByRise &,I_UserInterface &) "
  LOGN_DEBUG(
    "begin")
  // TODO Auto-generated constructor stub
  // e.g. "If I see you" ( adverb, clause)
  std::string strEnglish = "adverb_at_clause_begin , clause" ;

  //translates to: "Falls ich dich sehe." -> S O P
  std::string strGerman = "adverb_at_clause_begin\nGerman_subject\nobject\n"
    "predicate" ;
  mp_parsebyrise = & r_parsebyrise ;
  ConditionsAndTranslation::sp_parsebyrise = & r_parsebyrise ;
  ConditionsAndTranslation::sp_stdmap_AttrName2VocAndTranslAttrDef =
    & m_stdmap_AttrName2VocAndTranslAttrDef ;

//  AddDefiniteArticleNounTranslationRules() ;
//  AddPersonalPronounTranslationRules() ;

//  AddAndConjunctionTranslationRule() ;

//  //Condition for translating the predicate.
//  Condition conditonSubject ;
//  conditonSubject.SetSyntaxTreePath( "subject" , mp_parsebyrise ) ;
//  conditonSubject.m_stdstrAttributeName = "person_index" ;

  //TODO
//  cond3.m_wAttributeValue = THIRD_PERSON_PLURAL ;
//  ConditionsAndTranslation cntMainVerb ;
//  cntMainVerb.AddCondition(conditonSubject) ;
//  cntMainVerb.SetSyntaxTreePath( "main_verb", mp_parsebyrise ) ;
//  //="If subject is in third person plural, use German verb attribute value
//  //for plural."
//  cntMainVerb.m_stdstrAttributeName = "Ger_main_verb_3rd_person_plural" ;
//  //cnt3.m_byPersonIndex = THIRD_PERSON_PLURAL ;
//  //must create on heap.
//  TranslationRule * p_tr3 = new TranslationRule(
//    "main_verb" // For this syntax tree GrammarPart path in syntax tree.
//    , mp_parsebyrise ) ;
//  AddTranslationRule(
//    //"def_article_noun.definite_article" ,
//    p_tr3 ,
//    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
//    //cond ,
//    //, ""
//    cntMainVerb
//  ) ;
//  Condition conditonSubject ;
//  conditonSubject.SetSyntaxTreePath( "article_singular" , mp_parsebyrise ) ;
//  conditonSubject.m_stdstrAttributeName = "person_index" ;
//  //TODO
////  cond3.m_wAttributeValue = THIRD_PERSON_PLURAL ;

//  Add3rdPersonPluralTranslationRules() ;
//  Add3rdPersonSingularTranslationRules() ;
//  AddObjectTranslationRules() ;

//  AddVocAndTranslDefinitions() ;

  //AddRule( "adverb_at_clause_begin" , "clause" )
  //AddTransformationRule(strEnglish, strGerman) ;

//      At first: get person index og subject:
//      personindex = GetPersonIndex( grammarpartSubject ) ;
//
//      //IDEA for transformation syntax: Access subtrees of the parse tree by a
//      //point notation/ operator
//      "<translationRule>"
//      "<clause>"
//        "<predicate.bendVerb[ subject.personIndex ]" ;
//      "</clause>"
//  "<def_article_noun>" //noun_cobnstruct: definte article + noun
//    "<German_article[ German_noun.gender_id ]  German_noun[English_noun.sing_or_plural] " ;
//  "</def_article_noun>"
//    "<definite_def_article_noun>" //noun_cobnstruct: definte article + noun
//      "<German_def_article[ German_noun.gender ]  German_noun[English_noun.sing_or_plural] " ;
//    "</definite_def_article_noun>"
//    "<indefinite_def_article_noun>" //noun_cobnstruct: definte article + noun
//      "<German_indef_article[ German_noun.gender ]  German_noun[English_noun.sing_or_plural] " ;
//    "</indefinite_def_article_noun>"
//
//    //"put" may be present or past: "he put the glasses on the table"->
//      //"Er stellt ..." or "Er stelltE..."
//      for( < possible_tenses.size() )
//      <clause.tense == PRESENT>
//      <predicate.flectiveverb[personindex]>
//
//      //man koennte das auch als DB-Abfrage machen:
//      //select ... where German_def_article.gender_id == German_noun.gender_id
//      // German_noun[English_noun.sing_or_plural]
//   "you love us": you: possible person indices: 2nd person sing.,
//
//  "out:predicate.bendVerb[ subject.personIndex ]" ;

  //AddTranslationArray("German_definite_article","der\ndie\ndas\ndie") ;
  //AddTranslationArray("German_definite_article_dativ","dem\nder\ndem\nden") ;
  //  ein kleinER Mann,  kleinE Frau ,  kleinES Kind
  //AddTranslationArray("German_nominativ_suffix","er\ne\nes");
  //AddTranslationRule("subject.def_article_noun.adjective",
  //  "subject.def_article_noun.adjective+German_nominativ_suffix[
  //  subject.def_article_noun.noun.German_gender]")

  //AddTranslationRule("subject.def_article_noun.adjective",
  //  "subject.def_article_noun.adjective+German_nominativ_suffix[
  //  subject.def_article_noun.noun.German_gender]")
  //DEBUG_COUTN("TranslateParseByRiseTree(ParseByRise &,I_UserInterface &) "
  LOGN_DEBUG(
    "end")
}

//void TranslateParseByRiseTree::ApplyTransformationRules(
//    ParseByRise & r_parsebyrise)
//{
//
//}

void TranslateParseByRiseTree::FreeMemoryForTranslationRules()
{
  LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "begin" )
//  std::multimap<TranslationRule,ConditionsAndTranslation> &
//    r_stdmmap_translationrule2conditionsandtranslation =
//    //wxGetApp().m_translateparsebyrisetree.
//    m_stdmap_translationrule2ConditionsAndTranslation ;
//  for( std::multimap<TranslationRule,ConditionsAndTranslation>::const_iterator
//      c_iter_stdmmap_translationrule2conditionsandtranslation =
//      r_stdmmap_translationrule2conditionsandtranslation.begin() ;
//      c_iter_stdmmap_translationrule2conditionsandtranslation !=
//      r_stdmmap_translationrule2conditionsandtranslation.end() ;
//      ++ c_iter_stdmmap_translationrule2conditionsandtranslation
//      )
//    //Was created on the heap.
//    delete & c_iter_stdmmap_translationrule2conditionsandtranslation->first ;
  for( std::map<TranslationRule *,ConditionsAndTranslation>::const_iterator
      it = m_stdmap_p_translationrule2ConditionsAndTranslation.begin() ; it !=
      m_stdmap_p_translationrule2ConditionsAndTranslation.end() ; ++ it )
  {
    LOGN_DEBUG("freeing memory for translation rule " << it->first )
    //Release memory occupied by creation on heap.
    delete it->first ;
  }
  m_stdmap_translationrule2ConditionsAndTranslation.clear() ;
  m_stdmap_p_translationrule2ConditionsAndTranslation.clear() ;

  m_std_multimapConcatenationID2p_translationrule.clear();
  LOGN( /*FULL_FUNC_NAME <<*/ "end" )
}

std::string TranslateParseByRiseTree::GetSyntaxTreePathAsName(
  //ParseByRise & r_parsebyrise
  const std::vector<WORD> & r_stdvec_wGrammarPartPath
  )
{
  WORD wSize = r_stdvec_wGrammarPartPath.size() ;
  WORD wIndex = 0 ;
  std::string str ;
  if( mp_parsebyrise )
  {
    for( std::vector<WORD>::const_iterator iter =
      r_stdvec_wGrammarPartPath.begin() ;
        iter < r_stdvec_wGrammarPartPath.end() ; ++ iter )
    {
      str += mp_parsebyrise->GetGrammarPartName( *iter ) ;
      //If not the last element.
      if( wIndex + 1 < wSize )
        str += "." ;
      ++ wIndex ;
    }
  }
  return str ;
}

std::string TranslateParseByRiseTree::GetSyntaxTreePathAsName(
  //ParseByRise & r_parsebyrise
  WORD * ar_wGrammarPartPath, WORD wLength
  )
{
  std::string str ;
  if( mp_parsebyrise )
  {
//    DEBUG_COUT( "GetSyntaxTreePathAsName--"
    LOGN_DEBUG("length: " << wLength << "\n" )
    for( WORD wIndex = 0 ; wIndex < wLength ; ++ wIndex )
    {
//      DEBUG_COUT( "GetSyntaxTreePathAsName--"
      LOGN_DEBUG("element: " <<
          ar_wGrammarPartPath[ wIndex ] <<
          mp_parsebyrise->GetGrammarPartName(ar_wGrammarPartPath[ wIndex ])
          << "\n" )
      str += mp_parsebyrise->GetGrammarPartName(
        ar_wGrammarPartPath[ wIndex ] ) ;
      //If not the last element.
      if( wIndex + 1 < wLength )
        str += "." ;
    }
  }
  return str ;
}

void TranslateParseByRiseTree::SetSameConsecutiveIDforConnectedTranslationRules(
    TranslationRule * p_translationrule,
    const std::vector<GrammarPart * > & r_stdvec_p_grammarpartPath
    )
{
  //         //Get all other translation rule that match its connection ID.
  std::pair<std_multimap_uint32_t2p_translationrule::const_iterator,
    std_multimap_uint32_t2p_translationrule::const_iterator>
    std_pair_1stToLastTranslationRuleHavingConcatenationID =
    m_std_multimapConcatenationID2p_translationrule.equal_range(
    p_translationrule->m_ui32ConcatenationID);
  //           //For all translation rules with the same ID that match a branch of
  //           //the current parse: add the same concatenation ("Verkettung" /
  //           //"Verknüpfung") ID
  //           //(there may be more than 1 connected words for the same connection ID
  //           //(=the same connected translation rules):
  //           // e.g. for "you love and you like" "you" and "love" are connected and
  //           // "you" and "like" are connected.
  std_multimap_uint32_t2p_translationrule::const_iterator
    c_iterCurrent =
        std_pair_1stToLastTranslationRuleHavingConcatenationID.first;
  if( c_iterCurrent !=
      std_pair_1stToLastTranslationRuleHavingConcatenationID.second )
  {
    bool bAssignConcatenationID = false;
  //               ++ m_ui32ConcatenationID ;
    do
    {
        TranslationRule * p_translationrule = c_iterCurrent->second;
        GrammarPart * p_grammarpartLeaf = p_translationrule->
            m_syntaxtreepathCompareWithCurrentPath.GetLeaf(
          r_stdvec_p_grammarpartPath ) ;
        std::string std_strConcatenatedTranslationRuleSyntaxTreePath =
          GetSyntaxTreePathAsName(
          //p_tr->m_ar_wElements, p_tr->m_wNumberOfElements
          p_translationrule->m_syntaxtreepathCompareWithCurrentPath.
          m_ar_wElements,
          p_translationrule->m_syntaxtreepathCompareWithCurrentPath.
          m_wNumberOfElements
          ) ;
        if( p_grammarpartLeaf)
        {
          //A leaf already may have an ID: from a translation rule
          //path match (2 or more translation rules are connected, so
          //a translation rule path match from the connected
          //translation rule may have happened before)
          if( p_grammarpartLeaf->m_ui32ConcatenationID == //MAXWORD
              GrammarPart::unconnected)
            bAssignConcatenationID = true;
          if( bAssignConcatenationID )
          {
              //Write this concatenation ID to the parse tree leaf.
            p_grammarpartLeaf->m_ui32ConcatenationID =
  //                       ++ m_ui32ConcatenationID ;
              //All should get the same ID.
              m_ui32ConcatenationID;
          }
        }
        ++ c_iterCurrent;
    }while( c_iterCurrent !=
      std_pair_1stToLastTranslationRuleHavingConcatenationID.second );
    if( bAssignConcatenationID)
      //For the next assignment.
      ++ m_ui32ConcatenationID;
  }
}

void TranslateParseByRiseTree::ProcessParseTree(
//  ParseByRise & r_parsebyrise
//  , std::string & stdstrWholeTransl
  //,
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
  )
{
  ParseByRise * p_parsebyrise = //& r_parsebyrise ;
    mp_parsebyrise;
  //  std::string stdstrWholeTransl ;
  std::string stdstrTranslation ;
  std::vector<GrammarPart *>
    stdvec_p_grammarpartCoveringMostTokensAtTokenIndex ;
  LOGN_DEBUG( "begin" )
  m_std_vector_std_vector_p_grammarpartCoveringMostTokensAtTokenIndex.clear();
  if( p_parsebyrise )
  {
    DWORD dwLeftMostTokenIndex = 0 ;
  //    GrammarPart * p_grammarpart ;
  //    GrammarPart * p_grammarpartChild ;
    //int y = 10 ;
    //  typedef std::multimap<DWORD, GrammarPart >
    //  stdmmap_token_index2grammarpart ;
    typedef std::multimap<DWORD, GrammarPart *>
      stdmmap_token_index2grammarpart ;
    stdmmap_token_index2grammarpart::const_iterator citer ;
    stdmmap_token_index2grammarpart &
      r_stdmultimap_dwLeftmostIndex2grammarpart = p_parsebyrise->
      //m_stdmultimap_dwLeftmostIndex2grammarpart ;
      m_stdmultimap_dwLeftmostIndex2p_grammarpart ;
    //Reset to initial before each translation.
    m_ui32ConcatenationID = 1;
    do
    {
      //Before each draw in order to begin at x position "0".
      m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.clear() ;
      m_wParseLevel = 0 ;
      LOGN_DEBUG( "mp_parsebyrise != NULL\n")
      citer = r_stdmultimap_dwLeftmostIndex2grammarpart.begin() ;
      //p_grammarpart =
        p_parsebyrise->GetGrammarPartCoveringMostTokens(
          dwLeftMostTokenIndex ,
          stdvec_p_grammarpartCoveringMostTokensAtTokenIndex
          ) ;

      m_std_vector_std_vector_p_grammarpartCoveringMostTokensAtTokenIndex.
        push_back(stdvec_p_grammarpartCoveringMostTokensAtTokenIndex);
  #ifdef _DEBUG
      WORD wSize = stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.size();
  #endif //#ifdef _DEBUG
  //      WORD wConsecutiveID = 0 ;

      //A vector of sentences that each contains a vector of words.
      std::vector<std::vector<TranslationAndGrammarPart> >
        stdvec_stdvecTranslationAndGrammarPart;
      //Loop over all parse trees beginning at token index
      //"dwLeftMostTokenIndex".
      for( std::vector<GrammarPart *>::const_iterator
          c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex =
          stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.begin() ;
          c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex <
          stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.end() ;
          ++ c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex
         )
      {
        //( * this->pfnProcessParseTree)
        (this->* pfnProcessParseTree)(
          * c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex,
          stdvec_stdvecTranslationAndGrammarPart);
      } //loop for all parse trees beginning at same token index

      r_stdvec_stdvec_stdvecTranslationAndGrammarPart.push_back(
        stdvec_stdvecTranslationAndGrammarPart);

      if( stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.empty() )
        dwLeftMostTokenIndex = 0;
      else
        dwLeftMostTokenIndex =
          stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.at(0)->
          m_dwRightmostIndex + 1;
      LOGN_DEBUG( //"TranslateParseByRiseTree::Translate(...)"
        /*FULL_FUNC_NAME <<*/ "dwLeftMostTokenIndex:"
        << dwLeftMostTokenIndex )
    }
    while( dwLeftMostTokenIndex );
  }//if( p_parsebyrise )
}

//Getting the "person index":
// e.g. "I, you and the car suck."
//  "I"
void TranslateParseByRiseTree::Translate(
  ParseByRise & r_parsebyrise
//  , std::string & stdstrWholeTransl
  , std::vector<std::string> & r_stdvec_stdstrWholeTransl

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
  )
{
  LOGN_DEBUG(//"TranslateParseByRiseTree::Translate(...) "
    "begin")
//  BYTE byPersonIndex ;
  ProcessParseTree(
//    r_parsebyrise,
    r_stdvec_stdstrWholeTransl,
    r_stdvec_stdvec_stdvecTranslationAndGrammarPart,
    //(TranslateParseTree)()//r_stdvec_stdvec_stdvecTranslationAndGrammarPart)
    //syntax ( "& >>class name<<::>>method name<<" from
    //see http://stackoverflow.com/questions/4832275/c-typedef-member-function-signature-syntax
    & TranslateParseByRiseTree::TranslateParseTree
    );
//  DEBUG_COUT("translation: " << stdstrWholeTransl << "\n") ;
  LOGN_DEBUG(//"TranslateParseByRiseTree::Translate(...) "
    "end")
}

#ifdef COMPILE_AS_EXECUTABLE //only needed in GUI
void TranslateParseByRiseTree::TestIfTranslationRuleApplies(
  GrammarPart * p_grammarpart
  //A vector of sentences that each contains a vector of words.
  , std::vector<std::vector<TranslationAndGrammarPart> > &
    r_stdvec_stdvecTranslationAndGrammarPart
  )
{
  ParseTreeTraverser::TestIfTranslationRuleApplies
    testiftranslationruleapplies(p_grammarpart,
      * mp_parsebyrise, * this);
  //The transformation of the tree may lead to a crash/
  // only the leaves need to be processed-> set to "false".
  testiftranslationruleapplies.m_bTraverseTree = false ;

  testiftranslationruleapplies.Traverse() ;
  if( testiftranslationruleapplies.m_bTranslationRuleApplies )
    m_bTranslationRuleApplies = true;
}
#endif //#ifdef COMPILE_AS_EXECUTABLE

void TranslateParseByRiseTree::TranslateParseTree(
//  std::vector<GrammarPart *>::const_iterator
//    c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex
  GrammarPart * p_grammarpart
  //A vector of sentences that each contains a vector of words.
  , std::vector<std::vector<TranslationAndGrammarPart> > &
    r_stdvec_stdvecTranslationAndGrammarPart
  )
{
#ifdef _DEBUG
  DWORD dw =
//    (* c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex)->
    p_grammarpart->
    m_dwRightmostIndex;
#endif //#ifdef _DEBUG
  //    if( p_grammarpart )
  //    {
  LOGN_DEBUG( "GetGrammarPartCoveringMostTokens found \n" );

  ParseTreeTraverser::DoTranslateTreeTraverser translatetreetraverser(
    //        p_grammarpart
//    * c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex
    p_grammarpart
    , * mp_parsebyrise
    , * this
    );
//  translatetreetraverser.m_wConsecutiveID = wConsecutiveID;

  //The transformation of the tree may lead to a crash/
  // only the leaves need to be processed-> set to "false".
  translatetreetraverser.m_bTraverseTree = false ;

  translatetreetraverser.Traverse() ;

  //The tree may have been modified (tree branches moved or inserted)
  //So get all leaves from left to right for the whole tree.
  ParseTreeTraverser::TranslatedTreeTraverser translated_treetraverser(
//    * c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex,
    p_grammarpart,
    mp_parsebyrise
    );
  translated_treetraverser.Traverse();


//  wConsecutiveID = translatetreetraverser.m_wConsecutiveID ;
  //      stdstrWholeTransl = translatetreetraverser.m_stdstrWholeTranslation ;

//  r_stdvec_stdstrWholeTransl.push_back( //translatetreetraverser.
//    translated_treetraverser.
//    m_std_strWholeTranslation) ;
  r_stdvec_stdvecTranslationAndGrammarPart.push_back(
    //      r_stdvec_stdvecTranslationAndConsecutiveID.push_back(
    //          translatetreetraverser.m_stdvecTranslationAndGrammarPart
    translated_treetraverser.m_std_vector_translationandgrammarpart
    //        m_stdvec_translation_and_consecutive_id
    ) ;

  //There may be multiple _identical_ concatenated translations:
  //"the fan"
  //  \   /
  //DefiniteArticleSingularNoun  <- 1st, "der" and "Anhänger" are connected

  //"the fan"
  //  \   /
  //DefiniteArticleSingularNoun
  //    |
  //3rdPersSingEnumEle  <- 2nd, "der" and "Anhänger" are connected

  //But only let choose 1 possibility

  //translation of 1st tree:
  //  "der" (token index:0, concatenated ID=1)
  //  "Anhänger" (token index:1, concatenated ID=1)
  //translation of 2nd tree:
  //  "der" (token index:0, concatenated ID=2)
  // "Anhänger"(token index:0, concatenated ID=2)
  //
  //per token: a set of token indices this token is concatenated with
  // e.g. for 1st tree: "der": {1}, "Anhänger": {0}
  //  for 2nd tree: "der": {1}, "Anhänger": {0}

  //or: for each tree: sets of concatenated token indices:
  //   for 1st tree: {0,1} for 2nd tree: {0,1}
  //  then compare the tokens at these indices

  //        //Translation rules with the same connection ID should get the same
  //        //ID for the German translation.
  //        //If the current parse tree path matches a translation rule's path:
  //        //get all other translation rule that match its connection ID.
  //        ParseTreeTraverser::ConnectedIDtreetraverser connectedIDtreetraverser(
  //  //        p_grammarpart
  //          * c_iter_p_grammarpartCoveringMostTokensATokentIndex
  //          , * mp_parsebyrise
  //          , * this
  //          );
}

bool TranslateParseByRiseTree::TranslationRuleApplies( 
  std::string & r_stdstrTranslation ,
  BYTE & r_byPersonIndex
  // So it can be used with data from outside this class.
  //within the parse tree there is at a time only 1 current path in 1 direction:
  //from root node to the current node.
  // e.g.     "clause"
  //          /     \ (if "\"= last char:g++ warning:"multi-line comment")
  //    "subject"    \  _
  //     /            \ (if "\"= last char:g++ warning:"multi-line comment")
  //personal_pronoun  verb
  //->the current parse tree path may be (always beginning from root node "clause")
  // -"clause"
  // -"clause->subject"
  // -"clause->subject->personal_pronoun"
  // -"clause->verb"
  //but NOT:
  , const std::vector<WORD> & r_stdvec_wCurrentGrammarPartPath
  , const std::vector<GrammarPart * > & r_stdvec_p_grammarpartPath
//  , WORD & r_wConsecutiveID
  //reference to pointer address to >>const<<ant content
  , //const GrammarPart * & rp_grammarpartWithConsecutiveID
    GrammarPart * & rp_grammarpartWithConsecutiveID
  , TranslationRule * p_translationrule
  , const ConditionsAndTranslation & r_cnt
  , const std::string & std_strTranslationRuleSyntaxTreePath
  )
{
  bool bAtLeast1TranslationRuleApplies = false;
  bool bTranslationRuleSTPmatchesCurrentSTP = false ;
  bTranslationRuleSTPmatchesCurrentSTP = //p_translationrule->Matches(
    p_translationrule->m_syntaxtreepathCompareWithCurrentPath.Matches(
    r_stdvec_wCurrentGrammarPartPath ) ;
  if( bTranslationRuleSTPmatchesCurrentSTP )
  {
//        DEBUG_COUT
    LOGN_DEBUG( /*FULL_FUNC_NAME <<*/
      "the current syntax tree path matches the rule's syntax tree path"
      << " \"" << std_strTranslationRuleSyntaxTreePath << "\"")
//        GrammarPart * p_grammarpartClosestToTreeRoot =
//            r_stdvec_p_grammarpartPath.at(wIndex + wLenghtDiff ) ;
#ifdef _DEBUG
    std::string stdstr = GetSyntaxTreePathAsName(
      r_stdvec_wCurrentGrammarPartPath ) ;
    if( stdstr == "clause.main_verb" )
      stdstr += "" ;
#endif
    if( //AllConditionsMatch(
//            r_cnt ,
//            r_stdvec_p_grammarpartPath )
        r_cnt.AllConditionsMatch( r_stdvec_p_grammarpartPath )
      )
    {
      std::string stdstrCurrentParseTreePath = GetSyntaxTreePathAsName(
        r_stdvec_wCurrentGrammarPartPath ) ;
      LOGN_DEBUG("All conditions match for STP \"" << stdstrCurrentParseTreePath
        << "\"" )
      WORD wNumberOfElements = p_translationrule->
        m_syntaxtreepathInsertionForTranslation.m_wNumberOfElements;
      LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "All conditions match for STP \""
        << stdstrCurrentParseTreePath
        << "\"" )
       //std::string str =
      if( wNumberOfElements > 0)
      {
        p_translationrule->Insert(r_stdvec_p_grammarpartPath,
//                  r_stdstrTranslation
          r_cnt.m_stdstrGermanTranslation);
      }
      else
      {
        if( r_stdstrTranslation != "" )
          r_stdstrTranslation += "|";
         r_stdstrTranslation += //GetTranslationEquivalent(
            //r_cnt ,
            //r_stdvec_p_grammarpartPath
//              ) ;
            r_cnt.GetTranslationEquivalent(
              r_stdvec_p_grammarpartPath.back() ) ;

//             std::vector<GrammarPart * >::const_reverse_iterator
//               c_rev_iter_p_grammarpart
//               = r_stdvec_p_grammarpartPath.rbegin();
//             if( c_rev_iter_p_grammarpart != r_stdvec_p_grammarpartPath.rend()
//                 )

//             if( r_stdvec_p_grammarpartPath.empty() )
//             {
//                 const GrammarPart * & r_p_grammarpart =
//                   r_stdvec_p_grammarpartPath.back();
//                 //Problem:
//                 //if a grammar rule affects more than 1 grammar part, e.g.:
//                 //"sheep" (singular_noun) and "sheep" (plural_noun):
//                 //when translating "the sheep" and the translation rule then matches
//                 //"definite_article_singular.singular_plural" and the article is
//                 //translated to "die" it is overwritten by "das" from
//                 //"definite_article_singular.singular_noun" because the grammar part
//                 //is the same/ shared/ referenced by the two ones.
//                 //possible solution:
//                 //-store multiple translations per grammar part
//                 //-clone grammar part (branches).
//                 if( ! //c_rev_iter_p_grammarpart->m_stdstrTranslation.empty()
//                     r_p_grammarpart->m_stdstrTranslation.empty()
//                     )
//                 {
//                   //Must create on heap.
//                   GrammarPart * p_grammarpartCopy = new GrammarPart(
//                     //** c_rev_iter_p_grammarpart
//                     * r_p_grammarpart
//                     );
////                     r_stdvec_p_grammarpartPath.erase(c_rev_iter_p_grammarpart);
//                   //MinGW 4.5.2's "stl_vector.h" :
//                   //"It shrinks the %vector by one"
//                   r_stdvec_p_grammarpartPath.pop_back();
//                   r_stdvec_p_grammarpartPath.push_back(p_grammarpartCopy);
//                 }
//             }
         LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "adding translation \""
          << r_stdstrTranslation << "\"" )
      }
//           r_wConsecutiveID = p_translationrule->ConsecutiveID(
       rp_grammarpartWithConsecutiveID = p_translationrule->
           GetGrammarPartWithConsecutiveID(
         r_stdvec_p_grammarpartPath ) ;

       SetSameConsecutiveIDforConnectedTranslationRules(p_translationrule,
           r_stdvec_p_grammarpartPath);

       r_byPersonIndex = r_cnt.m_byPersonIndex ;

//           return true ;
       bAtLeast1TranslationRuleApplies = true;
    }
  }
  return bAtLeast1TranslationRuleApplies;
}

bool TranslateParseByRiseTree::TranslationRuleApplies(
  std::string & r_stdstrTranslation ,
  BYTE & r_byPersonIndex
  // So it can be used with data from outside this class.
  //within the parse tree there is at a time only 1 current path in 1 direction:
  //from root node to the current node.
  // e.g.     "clause"
  //          /     \ (if "\"= last char:g++ warning:"multi-line comment")
  //    "subject"    \  _
  //     /            \ (if "\"= last char:g++ warning:"multi-line comment")
  //personal_pronoun  verb
  //->the current parse tree path may be (always beginning from root node "clause")
  // -"clause"
  // -"clause->subject"
  // -"clause->subject->personal_pronoun"
  // -"clause->verb"
  //but NOT:
  , const std::vector<WORD> & r_stdvec_wCurrentGrammarPartPath
  , const std::vector<GrammarPart * > & r_stdvec_p_grammarpartPath
//  , WORD & r_wConsecutiveID
  //reference to pointer address to >>const<<ant content
  , //const GrammarPart * & rp_grammarpartWithConsecutiveID
    GrammarPart * & rp_grammarpartWithConsecutiveID
  )
{
  bool bAtLeast1TranslationRuleApplies = false;
  LOGN_DEBUG("begin")
  LOGN_DEBUG("number of translation rules:" <<
    m_stdmap_translationrule2ConditionsAndTranslation.size() << "\n")
#ifdef _DEBUG
  std::string stdstrCurrentParseTreePath = GetSyntaxTreePathAsName(
    r_stdvec_wCurrentGrammarPartPath ) ;
#endif
  if( ! r_stdvec_p_grammarpartPath.empty() )
  {
    const GrammarPart & c_r_grammarpart = * r_stdvec_p_grammarpartPath.back();
    if( c_r_grammarpart.m_wGrammarPartID == EnglishWord::UnknownWord )
    {
      r_stdstrTranslation = c_r_grammarpart.m_stdstrTranslation;
      return true;
    }
  }
//   ;
//  m_stdvec_wCurrentGrammarPartPath ;
//  for( std::map<TranslationRule * ,ConditionsAndTranslation>::const_iterator
//      c_iter_p_translationrule2conditionsandtranslation =
//        m_stdmap_p_translationrule2ConditionsAndTranslation.begin() ;
//      c_iter_p_translationrule2conditionsandtranslation !=
//        m_stdmap_p_translationrule2ConditionsAndTranslation.end() ;
//      ++ c_iter_p_translationrule2conditionsandtranslation
//      )
  for( std::map<TranslationRule ,ConditionsAndTranslation>::
      //Using const_reverse_iterator produced a g++ error for comparison
      // " != std::map::rend()"
      //const_reverse_iterator
      reverse_iterator
      c_rev_iter_translationrule2conditionsandtranslation =
      //The container is also sorted by the length of the syntax it applies to.
      //It is senseful to apply the with the largest syntax tree path:
      //If e.g. the current syntax tree path is "obj_ele"."def_noun"."the"
      //and there are 2 translation rules with parse tree pathes
      // -"def_noun"."the"
      // -"obj_ele"."def_noun"."the"
      // then the larger path should be given priority:
      //  the translation "dem" for "the man" for "obj_ele"."def_noun"."the"
      // is more correct than "der" for "the man"
        m_stdmap_translationrule2ConditionsAndTranslation.rbegin() ;
      c_rev_iter_translationrule2conditionsandtranslation !=
        m_stdmap_translationrule2ConditionsAndTranslation.rend() ;
      ++ c_rev_iter_translationrule2conditionsandtranslation
      )
  {
    //E.g. m_stdvec_wCurrentGrammarPartPath is
    // "clause.subject.def_article_noun.noun.definite_article", so it has the
    //  elements
    //  -"clause", "subject", "def_article_noun", "noun", "definite_article",
    //  and so the size is 5.
    //now compare it e.g. to iter ( element of
    //  m_stdmap_translationrule2ConditionsAndTranslation ) that may be
    // "subject.def_article_noun.noun.definite_article", i.e. a subtree of
    // m_stdvec_wGrammarPartPathw with just 4 elements.

      //e.g. compare "*.1stPersSingPresentProgressive.mainVerbInf0Obj" with
      //"1stPersSingPresentProgressive.mainVerbInf0Obj"
//    if ( //m_stdvec_wCurrentGrammarPartPath.size()
//        r_stdvec_wCurrentGrammarPartPath.size() >= //iter->first->m_wNumberOfElements
//        //c_iter_p_translationrule2conditionsandtranslation->first->
//        c_rev_iter_translationrule2conditionsandtranslation->first.
//        m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements
//        )
    {
      //Use a pointer, else the TranslationRule destructor is called.
//      const TranslationRule * p_translationrule =
      TranslationRule * p_translationrule = (TranslationRule *)
        //c_iter_p_translationrule2conditionsandtranslation->first ;
        & c_rev_iter_translationrule2conditionsandtranslation->first ;
      std::string std_strTranslationRuleSyntaxTreePath =
//          GetSyntaxTreePathAsName(
//          //p_tr->m_ar_wElements, p_tr->m_wNumberOfElements
//          p_translationrule->m_syntaxtreepathCompareWithCurrentPath.
//          m_ar_wElements,
//          p_translationrule->m_syntaxtreepathCompareWithCurrentPath.
//          m_wNumberOfElements
//          ) ;
        p_translationrule->m_syntaxtreepathCompareWithCurrentPath.
          GetAs_std_string();
#ifdef _DEBUG
      if( stdstrCurrentParseTreePath == "clause.main_verb" ||
          std_strTranslationRuleSyntaxTreePath == "main_verb" )
        //Just for setting a breakpoint.
        stdstrCurrentParseTreePath += "" ;
      LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "Comparing " << stdstrCurrentParseTreePath
        << " and " <<
        std_strTranslationRuleSyntaxTreePath //<< "\n" ;
        )
#endif
//      std::vector<WORD>::const_iterator c_iter_wGrammarPartPath =
//          r_stdvec_wGrammarPartPath ;
      const ConditionsAndTranslation & r_cnt =
        //c_iter_p_translationrule2conditionsandtranslation->second ;
        c_rev_iter_translationrule2conditionsandtranslation->second ;
        if( TranslationRuleApplies(
          r_stdstrTranslation
          , r_byPersonIndex
          , r_stdvec_wCurrentGrammarPartPath
          , r_stdvec_p_grammarpartPath
          , rp_grammarpartWithConsecutiveID
          , p_translationrule
          , r_cnt
          , std_strTranslationRuleSyntaxTreePath
          ) )
         bAtLeast1TranslationRuleApplies = true;
      //DEBUG_COUT
      LOGN_DEBUG("after loop")
    }
  }
  LOGN_DEBUG("end")
  return //bIdentical ;
    bAtLeast1TranslationRuleApplies;
}

TranslateParseByRiseTree::~TranslateParseByRiseTree()
{
  LOGN_DEBUG(//" of ~TranslateParseByRiseTree\n"
    "begin")
  FreeMemoryForTranslationRules() ;
  LOGN_DEBUG(//" of ~TranslateParseByRiseTree\n"
    "end")
}
