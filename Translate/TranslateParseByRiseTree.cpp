/*
 * TranslateParseByRiseTree.cpp
 *
 *  Created on: 25.02.2010
 *      Author: Stefan
 */

#include <Parse/ParseByRise.hpp> //class ParseByRise
#include <UserInterface/I_UserInterface.hpp> //for I_UserInterface::Message(...)
//class AttributeTypeAndPosAndSize
#include <Translate/AttributeTypeAndPosAndSize.hpp>
//#include <Translate/SummarizePersonIndex.hpp>//class SummarizePersonIndex
//header file of this TranslateParseByRiseTree class
#include <Translate/TranslateParseByRiseTree.hpp>
#include <Translate/Translationrule.hpp>//class TranslationRule
//class ParseTreeTraverser::TranslateTreeTraverser
#include <Translate/TranslateTreeTraverser.hpp>
//class VocabularyAndTranslation
#include <VocabularyInMainMem/LetterTree/VocabularyAndTranslation.hpp>
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
//  bool bNewEle = false ;
//  std::string stdstrCurrentElement ;
//  std::vector<WORD> vec_wElements ;
//  WORD wStringStart = 0 ;
//  WORD wGrammarPartID ;
//  WORD wNumberOfElements = 1 ;
//  for( WORD i = 0 ; i < r_stdstrSyntaxTreePath.length() ; ++ i )
//  {
//    if( i == r_stdstrSyntaxTreePath.length() - 1 )
//    {
//      stdstrCurrentElement = r_stdstrSyntaxTreePath.substr(
//          wStringStart, ( i + 1 ) - wStringStart ) ;
//      bNewEle = true ;
//    }
//    if( r_stdstrSyntaxTreePath[i] == '.' )
//    {
//      stdstrCurrentElement = r_stdstrSyntaxTreePath.substr(
//          wStringStart, i - wStringStart ) ;
//      bNewEle = true ;
//    }
//    if( bNewEle )
//    {
//      ++ wNumberOfElements ;
//      wStringStart = i + 1 ;
//      if( mp_parsebyrise->GetGrammarPartID( stdstrCurrentElement ,
//          wGrammarPartID )
//        )
//      {
//        vec_wElements.push_back(wGrammarPartID) ;
//      }
//      bNewEle = false ;
//    }
//  }
//  WORD * ar_wElements = new WORD [ vec_wElements.size() ] ;
//  if( ar_wElements )
//  {
//    WORD wIndex = 0 ;
//    for( std::vector<WORD>::const_iterator iter = vec_wElements.begin() ;
//        iter != vec_wElements.end() ; ++ iter )
//    {
//      //DEBUG("curr") ;
//      ar_wElements[ wIndex ++ ] = *iter ;
//    }
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
      DEBUG_COUT( "added translation rule for syntax tree path \"" <<
          GetSyntaxTreePathAsName(
          //r_tr.m_ar_wElements, r_tr.m_wNumberOfElements
          r_tr.m_syntaxtreepathCompareWithCurrentPath.m_ar_wElements,
          r_tr.m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements
              ) <<
          "\"\n" )
    }
#endif
    }
    catch( std::exception & e )
    {
      DEBUG_COUT("std::exception")
    }
//  }
}

void TranslateParseByRiseTree::AddTranslationRule(
  TranslationRule * p_tr,
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
        p_tr , rc_cnt
      )
      ) ;
    m_stdmap_translationrule2ConditionsAndTranslation.insert(
      std::pair<TranslationRule,ConditionsAndTranslation>
        (
          *p_tr , rc_cnt
        )
      ) ;

#ifdef _DEBUG
    if( _pair.second )
    {
      const TranslationRule & r_tr = *_pair.first->first ;
      DEBUG_COUT( "added translation rule for syntax tree path \"" <<
          GetSyntaxTreePathAsName(
          //r_tr.m_ar_wElements, r_tr.m_wNumberOfElements
          r_tr.m_syntaxtreepathCompareWithCurrentPath.m_ar_wElements,
          r_tr.m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements
              ) <<
          "\"\n" )
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
  DEBUGN("TranslateParseByRiseTree::AddVocAndTranslDefinition("
    << r_stdstrWordClass << ","
    << r_stdstrWordClassAndAttributeName )
  WORD wGrammarPartID ;
  //e.g. get corresponding ID for word class "noun".
  if( mp_parsebyrise->GetGrammarPartID(r_stdstrWordClass,wGrammarPartID) )
  {
    DEBUGN("Found GetGrammarPartID for word class" << r_stdstrWordClass )
    AttributeTypeAndPosAndSize attributetypeandposandsize( byType, wIndex,
      wLenght, wGrammarPartID, byLanguage) ;
    m_stdmap_AttrName2VocAndTranslAttrDef.insert(
      std::pair<std::string, AttributeTypeAndPosAndSize>
        (
        r_stdstrWordClassAndAttributeName
        , attributetypeandposandsize
        )
      ) ;
  }
#ifdef _DEBUG
  else
    DEBUGN("Couldn't find GetGrammarPartID for word class"
      << r_stdstrWordClass )
#endif
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
          DEBUG_COUT("grammar part leaf found:" <<
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
        DEBUG_COUT("GetTranslationEquivalent()--grammar part leaf found:" <<
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
              DEBUG_COUTN("data type for choosing attribute value is string")
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
                  DEBUG_COUTN("language for choosing attribute value is "
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
          }//switch
        }
        //if( r_cond.m_byCompareType == Condition::equals )
      }
      else
        DEBUG_COUTN("GetTranslationEquivalent()--attribute name" <<
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
  : mr_i_userinterface (r_i_userinterface)
{
  DEBUG_COUTN("TranslateParseByRiseTree(ParseByRise &,I_UserInterface &) "
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

//  //Must create on heap, else the callback functions like "startElement"
//  //aren't called?!
////  SAX2TranslationRuleHandler * p_sax2translationrulehandler = new
////    SAX2TranslationRuleHandler( * this, * mp_parsebyrise ,
////    mr_i_userinterface ) ;
//  SAX2TranslationRuleHandler sax2translationrulehandler ( * this,
//    * mp_parsebyrise , mr_i_userinterface ) ;
//  std::wstring wstr ;
//  // <> 0 = error
//  if( ReadViaSAX2InitAndTermXerces(
//      "translation_rules.xml",
//  //    p_sax2translationrulehandler ,
//      & sax2translationrulehandler ,
//      wstr
//      )
//    )
//  {
//    mr_i_userinterface.Message( wstr ) ;
//  }

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
  DEBUG_COUTN("TranslateParseByRiseTree(ParseByRise &,I_UserInterface &) "
    "end")
}

//void TranslateParseByRiseTree::ApplyTransformationRules(
//    ParseByRise & r_parsebyrise)
//{
//
//}

void TranslateParseByRiseTree::FreeMemoryForTranslationRule()
{
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
    LOGN("freeing memory for translation rule " << it->first )
    //Release memory occupied by creation on heap.
    delete it->first ;
  }
  m_stdmap_translationrule2ConditionsAndTranslation.clear() ;
  m_stdmap_p_translationrule2ConditionsAndTranslation.clear() ;
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
    DEBUG_COUT( "GetSyntaxTreePathAsName--length: " << wLength << "\n" )
    for( WORD wIndex = 0 ; wIndex < wLength ; ++ wIndex )
    {
      DEBUG_COUT( "GetSyntaxTreePathAsName--element: " <<
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

//Getting the "person index":
// e.g. "I, you and the car suck."
//  "I"
void TranslateParseByRiseTree::Translate(
  ParseByRise & r_parsebyrise
//  , std::string & stdstrWholeTransl
  , std::vector<std::string> & r_stdvec_stdstrWholeTransl
  //A vector of sentences that each contains a vector of words.
  , std::vector<std::vector<TranslationAndGrammarPart> > &
    r_stdvec_stdvecTranslationAndGrammarPart
//  , std::vector<std::vector<TranslationAndConsecutiveID> > &
//    r_stdvec_stdvecTranslationAndConsecutiveID
  )
{
//  BYTE byPersonIndex ;
  ParseByRise * mp_parsebyrise = & r_parsebyrise ;
//  std::string stdstrWholeTransl ;
  std::string stdstrTranslation ;
  std::vector<GrammarPart *> stdvec_p_grammarpart ;
  DEBUG_COUT( "Translate(): \n" )
  if( mp_parsebyrise )
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
      r_stdmultimap_dwLeftmostIndex2grammarpart = mp_parsebyrise->
      //m_stdmultimap_dwLeftmostIndex2grammarpart ;
      m_stdmultimap_dwLeftmostIndex2p_grammarpart ;
    do
    {
      //Before each draw in order to begin at x position "0".
      m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.clear() ;
      m_wParseLevel = 0 ;
      DEBUG_COUT( "Translate(): mp_parsebyrise != NULL\n")
      citer = r_stdmultimap_dwLeftmostIndex2grammarpart.begin() ;
      //p_grammarpart =
        mp_parsebyrise->GetGrammarPartCoveringMostTokens(
          dwLeftMostTokenIndex ,
          stdvec_p_grammarpart
          ) ;
      WORD wConsecutiveID = 0 ;
      for( std::vector<GrammarPart *>::const_iterator c_iter_p_grammarpart =
          stdvec_p_grammarpart.begin() ; c_iter_p_grammarpart <
          stdvec_p_grammarpart.end() ; ++ c_iter_p_grammarpart
         )
      {
  //    if( p_grammarpart )
  //    {
        DEBUG_COUT( "Translate: GetGrammarPartCoveringMostTokens found \n" );

        ParseTreeTraverser::TranslateTreeTraverser translatetreetraverser(
  //        p_grammarpart
          * c_iter_p_grammarpart
          , * mp_parsebyrise
          , * this
          );
        translatetreetraverser.m_wConsecutiveID = wConsecutiveID;

        //The transformation of the tree may lead to a crash/
        // only the leaves need to be processed-> set to "false".
        translatetreetraverser.m_bTraverseTree = false ;

        translatetreetraverser.Traverse() ;
        wConsecutiveID = translatetreetraverser.m_wConsecutiveID ;
  //      stdstrWholeTransl = translatetreetraverser.m_stdstrWholeTranslation ;
        r_stdvec_stdstrWholeTransl.push_back( translatetreetraverser.
          m_stdstrWholeTranslation) ;
        r_stdvec_stdvecTranslationAndGrammarPart.push_back(
  //      r_stdvec_stdvecTranslationAndConsecutiveID.push_back(
          translatetreetraverser.m_stdvecTranslationAndGrammarPart
  //        m_stdvec_translation_and_consecutive_id
          ) ;
      }
      if( stdvec_p_grammarpart.empty() )
        dwLeftMostTokenIndex = 0;
      else
        dwLeftMostTokenIndex = stdvec_p_grammarpart.at(0)->m_dwRightmostIndex
          + 1;
    }
    while( dwLeftMostTokenIndex );
  }
//  DEBUG_COUT("translation: " << stdstrWholeTransl << "\n") ;
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
  bool bIdentical = false ;
  DEBUG_COUT("TranslationRuleApplies--beg\n")
  DEBUG_COUT("number of translation rules:" <<
      m_stdmap_translationrule2ConditionsAndTranslation.size() << "\n")
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
    if ( //m_stdvec_wCurrentGrammarPartPath.size()
        r_stdvec_wCurrentGrammarPartPath.size() >= //iter->first->m_wNumberOfElements
        //c_iter_p_translationrule2conditionsandtranslation->first->
        c_rev_iter_translationrule2conditionsandtranslation->first.
        m_syntaxtreepathCompareWithCurrentPath.m_wNumberOfElements
        )
    {
      //Use a pointer, else the TranslationRule destructor is called.
      const TranslationRule * p_translationrule =
        //c_iter_p_translationrule2conditionsandtranslation->first ;
        & c_rev_iter_translationrule2conditionsandtranslation->first ;
#ifdef _DEBUG
      std::string stdstrCurrentParseTreePath = GetSyntaxTreePathAsName(
        r_stdvec_wCurrentGrammarPartPath ) ;
      std::string stdstr2 = GetSyntaxTreePathAsName(
          //p_tr->m_ar_wElements, p_tr->m_wNumberOfElements
          p_translationrule->m_syntaxtreepathCompareWithCurrentPath.
          m_ar_wElements,
          p_translationrule->m_syntaxtreepathCompareWithCurrentPath.
          m_wNumberOfElements
          ) ;
      if( stdstrCurrentParseTreePath == "clause.main_verb" || stdstr2 == "main_verb" )
        //Just for setting a breakpoint.
        stdstrCurrentParseTreePath += "" ;
      std::cout << "Comparing " << stdstrCurrentParseTreePath << " and " <<
          stdstr2 << "\n" ;
#endif
//      std::vector<WORD>::const_iterator c_iter_wGrammarPartPath =
//          r_stdvec_wGrammarPartPath ;
      //DEBUG_COUT
      bIdentical = //p_translationrule->Matches(
          p_translationrule->m_syntaxtreepathCompareWithCurrentPath.Matches(
        r_stdvec_wCurrentGrammarPartPath ) ;
      if( bIdentical )
      {
//        DEBUG_COUT
        LOGN("the current syntax tree path matches the rule' syntax "
          "tree path")
//        GrammarPart * p_grammarpartClosestToTreeRoot =
//            r_stdvec_p_grammarpartPath.at(wIndex + wLenghtDiff ) ;
#ifdef _DEBUG
        std::string stdstr = GetSyntaxTreePathAsName(
          r_stdvec_wCurrentGrammarPartPath ) ;
        if( stdstr == "clause.main_verb" )
          stdstr += "" ;
#endif
        const ConditionsAndTranslation & r_cnt =
          //c_iter_p_translationrule2conditionsandtranslation->second ;
          c_rev_iter_translationrule2conditionsandtranslation->second ;
        if( //AllConditionsMatch(
//            r_cnt ,
//            r_stdvec_p_grammarpartPath )
            r_cnt.AllConditionsMatch( r_stdvec_p_grammarpartPath )
          )
        {
          std::string stdstrCurrentParseTreePath = GetSyntaxTreePathAsName(
            r_stdvec_wCurrentGrammarPartPath ) ;
          LOGN("All conditions match for STP \"" << stdstrCurrentParseTreePath
            << "\"" )
           //std::string str =
           r_stdstrTranslation = //GetTranslationEquivalent(
              //r_cnt ,
              //r_stdvec_p_grammarpartPath
//              ) ;
              r_cnt.GetTranslationEquivalent(
                r_stdvec_p_grammarpartPath.back() ) ;
//           r_wConsecutiveID = p_translationrule->ConsecutiveID(
           rp_grammarpartWithConsecutiveID = p_translationrule->
               GetGrammarPartWithConsecutiveID(
             r_stdvec_p_grammarpartPath ) ;
           r_byPersonIndex = r_cnt.m_byPersonIndex ;
           return true ;
        }
      }
      DEBUG_COUT("TranslationRuleApplies--after loop\n")
    }
  }
  DEBUG_COUT("TranslationRuleApplies--end\n")
  return bIdentical ;
}

TranslateParseByRiseTree::~TranslateParseByRiseTree()
{
  LOGN("begin of ~TranslateParseByRiseTree\n")
  FreeMemoryForTranslationRule() ;
  LOGN("end of ~TranslateParseByRiseTree\n")
}
