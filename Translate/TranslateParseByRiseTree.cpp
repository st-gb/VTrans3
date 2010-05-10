/*
 * TranslateParseByRiseTree.cpp
 *
 *  Created on: 25.02.2010
 *      Author: Stefan
 */

#include <Parse/ParseByRise.hpp>
#include <Translate/SummarizePersonIndex.hpp>
#include <Translate/TranslateParseByRiseTree.h>
#include <Translate/Translationrule.hpp>
#include <VocabularyInMainMem/LetterTree/VocabularyAndTranslation.hpp>

#include <string>

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
//AddTranslationRule("subject.noun_construct.definite_article",
//  "German_definite_article[subject.noun_construct.noun.German_gender]")
//because all definite articles in a subject should be translated to the
// undeclinated? (Nominativ) versions "der", "die" , "das"

//AddTranslationRule("subject.noun_construct.definite_article" ,
//  "noun_construct.noun,translateAsSingular")

//e.g. "the news" (singular) is translated as "Nachrichten" (Plural)
//AddTranslationRule(
//  "subject.noun_construct.definite_article" ,
//  "noun_construct.noun.English.translateAsPlural=1" //English attribute
//  "noun_construct.noun.German.plural") //attribute of German word.

//AddTranslationRule(
//  "subject.noun_construct.noun.definite_article" ,
//  "noun.English.translateSingularAsPlural=1" //English attribute (condition)
//  "\"die\"") //attribute of German word.

//AddTranslationRule(
//  "subject.noun_construct.noun.definite_article" ,
//  "noun.English.isSingular=1" //English attribute (condition)
//  "noun.English.translateSingularAsPlural=0" //English attribute (condition)
//  "noun.German.gender=0" //German attribute (condition)
//  "\"der\"") //attribute of German word.
void TranslateParseByRiseTree::AddTranslationRule(
  const std::string & r_stdstrSyntaxTreePath ,
  //const std::string & r_stdstrEnglishAttributeNameAndValue ,
  const std::string & r_stdstrConditions ,
  const std::string & r_stdstrGermanTranslation
  //  subject.noun_construct.noun.German_gender]"
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
    TranslationRule * p_tr = new TranslationRule( r_stdstrSyntaxTreePath
        , mp_parsebyrise ) ;

//    DEBUG_COUT( "added translation rule for syntax tree path \"" <<
//        GetSyntaxTreePathAsName( ar_wElements, vec_wElements.size() ) <<
//        "\"\n" )
    ConditionsAndTranslation c ;
#ifdef _DEBUG
    std::pair<std::map<TranslationRule*,ConditionsAndTranslation>::iterator,bool>
      _pair =
#endif
    m_stdmap_translationrule2ConditionsAndTranslation.insert(
      std::pair<TranslationRule *,ConditionsAndTranslation>
      ( //tr
        //Create here, else ar_wElements is deleted in TranslationRule d'tor?!
        //TranslationRule(ar_wElements, vec_wElements.size() )
        p_tr , c
      )
      ) ;
#ifdef _DEBUG
    if( _pair.second )
    {
      const TranslationRule & r_tr = *_pair.first->first ;
      DEBUG_COUT( "added translation rule for syntax tree path \"" <<
          GetSyntaxTreePathAsName(
          //r_tr.m_ar_wElements, r_tr.m_wNumberOfElements
          r_tr.m_SyntaxTreePath.m_ar_wElements,
          r_tr.m_SyntaxTreePath.m_wNumberOfElements
              ) <<
          "\"\n" )
    }
#endif
//  }
}

void TranslateParseByRiseTree::AddTranslationRule(
  TranslationRule * p_tr,
  const ConditionsAndTranslation & rc_cnt
  //const std::string & r_stdstrGermanTranslation
  //  subject.noun_construct.noun.German_gender]"
  )
{
#ifdef _DEBUG
    std::pair<std::map<TranslationRule*,ConditionsAndTranslation>::iterator,bool>
      _pair =
#endif
    m_stdmap_translationrule2ConditionsAndTranslation.insert(
      std::pair<TranslationRule *,ConditionsAndTranslation>
      ( //tr
        //Create here, else ar_wElements is deleted in TranslationRule d'tor?!
        //TranslationRule(ar_wElements, vec_wElements.size() )
        p_tr , rc_cnt
      )
      ) ;
#ifdef _DEBUG
    if( _pair.second )
    {
      const TranslationRule & r_tr = *_pair.first->first ;
      DEBUG_COUT( "added translation rule for syntax tree path \"" <<
          GetSyntaxTreePathAsName(
          //r_tr.m_ar_wElements, r_tr.m_wNumberOfElements
          r_tr.m_SyntaxTreePath.m_ar_wElements,
          r_tr.m_SyntaxTreePath.m_wNumberOfElements
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
  WORD wID ;
  //e.g. get coorresponding ID for word class "noun".
  if( mp_parsebyrise->GetGrammarPartID(r_stdstrWordClass,wID) )
  {
    AttributeTypeAndPosAndSize a( byType, wIndex, wLenght, wID , byLanguage) ;
    m_stdmap_AttrName2VocAndTranslAttrDef.insert(
      std::pair<std::string, AttributeTypeAndPosAndSize>
      (
      r_stdstrWordClassAndAttributeName
      , a
      )
      ) ;
  }
}

bool TranslateParseByRiseTree::AllConditionsMatch(
  const ConditionsAndTranslation & r_conditions_and_translation
  // So it can be used with data from outside this class.
  //, const std::vector<WORD> & r_stdvec_wGrammarPartPath
  , const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath
  )
{
  bool bAllConditionsMatch = false ;
  int i = 0 ;
  //TODO implement this
//        if( r_cnt.conditions.m_byCompareType == ConditionsAndTranslation::equals
//          && AttributeTypeAndPosAndSize::English && rightof("==") equals(
//          grammarpart.string &&
//          mp_parsebyrise->psv.getTokens(
//              grammarpart.index, index2) )
  if( r_conditions_and_translation.m_conditions.size() > 0 )
  {
    bAllConditionsMatch = true ;
    const Condition & cr_condition =
        r_conditions_and_translation.m_conditions.front() ;
    //if( r_cond.m_syntaxtreepath.IsPartOf(m_stdvec_wGrammarPartPath) )
    //TODO get pointer to leaf node of r_cond.m_syntaxtreepath
    GrammarPart * p_grammarpartLeaf = cr_condition.m_syntaxtreepath.GetLeaf(
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

              }

          }//switch
        }
        //if( r_cond.m_byCompareType == Condition::equals )
      }
    }
  }
  return bAllConditionsMatch ;
}

std::string TranslateParseByRiseTree::GetTranslationEquivalent(
  const ConditionsAndTranslation & r_cnt
  , const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath
  )
{
  bool bAllConditionsMatch = false ;
  std::string stdstr ;
  int i = 0 ;
  //TODO implement this
//        if( r_cnt.conditions.m_byCompareType == ConditionsAndTranslation::equals
//          && AttributeTypeAndPosAndSize::English && rightof("==") equals(
//          grammarpart.string &&
//          mp_parsebyrise->psv.getTokens(
//              grammarpart.index, index2) )

  //If no simple replacement (like use "die" for "the" + "English_plural"
  if( r_cnt.m_stdstrGermanTranslation ==  "" )
  {
    bAllConditionsMatch = true ;
    //if( r_cond.m_syntaxtreepath.IsPartOf(m_stdvec_wGrammarPartPath) )
    //TODO get pointer to leaf node of r_cond.m_syntaxtreepath
    GrammarPart * p_grammarpartLeaf = r_cnt.m_syntaxtreepath.GetLeaf(
      r_stdvec_p_grammarpartPath) ;
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
        c_iter_stdstrSttrName2atapas = m_stdmap_AttrName2VocAndTranslAttrDef.find(
          r_cnt.m_stdstrAttributeName ) ;
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
                BYTE by = p_grammarpartLeaf->
                  m_pvocabularyandtranslation->m_arbyAttribute[
                  r_atapas.m_wIndex ] ;

              }
          }//switch
        }
        //if( r_cond.m_byCompareType == Condition::equals )
      }
    }
  }
  else
  {
    stdstr = r_cnt.m_stdstrGermanTranslation ;
  }
  return stdstr ;
}

TranslateParseByRiseTree::TranslateParseByRiseTree(
  ParseByRise & r_parsebyrise )
{
  // TODO Auto-generated constructor stub
  // e.g. "If I see you" ( adverb, clause)
  std::string strEnglish = "adverb_at_clause_begin , clause" ;

  //translates to: "Falls ich dich sehe." -> S O P
  std::string strGerman = "adverb_at_clause_begin\nGerman_subject\nobject\n"
    "predicate" ;
  mp_parsebyrise = & r_parsebyrise ;

  Condition cond ;
  cond.SetSyntaxTreePath( "noun_construct.noun" , mp_parsebyrise ) ;
  cond.m_stdstrAttributeName = "Eng_plural" ;
  ConditionsAndTranslation cnt ;
  cnt.AddCondition(cond) ;
#ifdef _DEBUG
  std::string stdstr = cnt.m_conditions.front().m_syntaxtreepath.
      GetAs_std_string() ;
  DEBUG_COUT("TranslateParseByRiseTree()--syntax tree path string from vectr: "
    << stdstr)
#endif
  cnt.m_stdstrGermanTranslation = "die" ;
  cnt.m_byPersonIndex = THIRD_PERSON_PLURAL ;
  //must create on heap.
  TranslationRule * p_tr = new TranslationRule(
    "noun_construct.definite_article" // For this syntax tree GrammarPart path
    //in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"noun_construct.definite_article" ,
    p_tr ,
    //"noun_construct.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cnt
  ) ;

  //TODO for nouns where the sing equals the plural (like "sheep"):
  //  (additional) condition should be "when indefinite article ("a"): singular"
  Condition cond2 ;
  cond2.SetSyntaxTreePath( "noun_construct.noun" , mp_parsebyrise ) ;
  cond2.m_stdstrAttributeName = "Eng_singular" ;
  ConditionsAndTranslation cnt2 ;
  cnt2.AddCondition(cond2) ;
  cnt2.SetSyntaxTreePath( "noun_construct.noun", mp_parsebyrise ) ;
  cnt2.m_stdstrAttributeName = "German_singular" ;
  cnt2.m_byPersonIndex = THIRD_PERSON_SINGULAR ;
  
  //cond.SetAttributename("isSingular") ;
  //cond.SetCompareOperator("=") ;
  //cond.SetAttributeValue("1") ;

  //must create on heap.
  TranslationRule * p_tr2 = new TranslationRule(
    "noun_construct.noun" // For this syntax tree GrammarPart path in
    //syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"noun_construct.definite_article" ,
    p_tr2 ,
    //"noun_construct.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cnt2
  ) ;
#ifdef _DEBUG
  std::map<TranslationRule *,ConditionsAndTranslation>::const_iterator ci =
      m_stdmap_translationrule2ConditionsAndTranslation.find(p_tr2) ;
  if( ci !=
      m_stdmap_translationrule2ConditionsAndTranslation.end() )
  {
    const Condition & cr_cond = ci->second.m_conditions.front() ;
    std::string stdstr = cr_cond.m_syntaxtreepath.
        GetAs_std_string() ;
    DEBUG_COUT("TranslateParseByRiseTree()--syntax tree path string from vectr: "
      << stdstr)
  }
#endif

  //Condition for translating the predicate.
  Condition cond3 ;
  cond3.SetSyntaxTreePath( "subject" , mp_parsebyrise ) ;
  cond3.m_stdstrAttributeName = "person_index" ;
  //TODO
//  cond3.m_wAttributeValue = THIRD_PERSON_PLURAL ;
  ConditionsAndTranslation cnt3 ;
  cnt3.AddCondition(cond3) ;
  cnt3.SetSyntaxTreePath( "main_verb", mp_parsebyrise ) ;
  //="If subject is in third person plural, use German verb attribute value
  //for plural."
  cnt3.m_stdstrAttributeName = "Ger_main_verb_3rd_person_plural" ;
  //cnt3.m_byPersonIndex = THIRD_PERSON_PLURAL ;
  //must create on heap.
  TranslationRule * p_tr3 = new TranslationRule(
    "main_verb" // For this syntax tree GrammarPart path in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"noun_construct.definite_article" ,
    p_tr3 ,
    //"noun_construct.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cnt3
  ) ;
  AddVocAndTranslDefinition(
    "main_verb",
    "Ger_main_verb_3rd_person_sing" , //attribute name to use as a map key value
    AttributeTypeAndPosAndSize::German, //language
    AttributeTypeAndPosAndSize::string, //attribute is a string or some bits
    //GERMAN_VERB_3RD_PERSON_SINGULAR_INDEX , //index
    VocabularyAndTranslation::third_person_singular ,
    1 // length
    );
  AddVocAndTranslDefinition(
    "main_verb",
    "Ger_main_verb_3rd_person_sing" , //attribute name to use as a map key value
    AttributeTypeAndPosAndSize::German, //language
    AttributeTypeAndPosAndSize::string, //attribute is a string or some bits
    //GERMAN_VERB_3RD_PERSON_PLURAL_INDEX , //index
    VocabularyAndTranslation::third_person_plural ,
    1 // length
    );
  AddVocAndTranslDefinition(
    "noun",
    "Eng_singular" , //attribute name to use as a map key value
    AttributeTypeAndPosAndSize::English, //language
    AttributeTypeAndPosAndSize::string, //attribute is a string or some bits
    0 , //index (singular is stored at index 0 in VocabularyAndTransaltion
      // string array)
    1 // length
    );
  AddVocAndTranslDefinition(
    "noun",
    "Eng_plural" , //attribute name to use as a map key value
    AttributeTypeAndPosAndSize::English, //language
    AttributeTypeAndPosAndSize::string, //attribute is a string or some bits
    1 , //index (plural is stored at index 1 in VocabularyAndTranslation
      // string array)
    1 // length
    );
  AddVocAndTranslDefinition(
    "noun",
    "German_singular" , //attribute name to use as a map key value
    AttributeTypeAndPosAndSize::German, //language
    AttributeTypeAndPosAndSize::string, //attribute is a string or some bits
    0 , //index
    1 // length
    );
  AddVocAndTranslDefinition(
    "noun",
    "German_plural" , //attribute name to use as a map key value
    AttributeTypeAndPosAndSize::German, //language
    AttributeTypeAndPosAndSize::string, //attribute is a string or some bits
    1 , //index
    1 // length
    );

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
//  "<noun_construct>" //noun_cobnstruct: definte article + noun
//    "<German_article[ German_noun.gender_id ]  German_noun[English_noun.sing_or_plural] " ;
//  "</noun_construct>"
//    "<definite_noun_construct>" //noun_cobnstruct: definte article + noun
//      "<German_def_article[ German_noun.gender ]  German_noun[English_noun.sing_or_plural] " ;
//    "</definite_noun_construct>"
//    "<indefinite_noun_construct>" //noun_cobnstruct: definte article + noun
//      "<German_indef_article[ German_noun.gender ]  German_noun[English_noun.sing_or_plural] " ;
//    "</indefinite_noun_construct>"
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
  //AddTranslationRule("subject.noun_construct.adjective",
  //  "subject.noun_construct.adjective+German_nominativ_suffix[
  //  subject.noun_construct.noun.German_gender]")

  //AddTranslationRule("subject.noun_construct.adjective",
  //  "subject.noun_construct.adjective+German_nominativ_suffix[
  //  subject.noun_construct.noun.German_gender]")
}

//void TranslateParseByRiseTree::ApplyTransformationRules(
//    ParseByRise & r_parsebyrise)
//{
//
//}

std::string TranslateParseByRiseTree::GetSyntaxTreePathAsName(
  //ParseByRise & r_parsebyrise
  const std::vector<WORD> & r_stdvec_wGrammarPartPath
  )
{
  std::string str ;
  if( mp_parsebyrise )
  {
    for( std::vector<WORD>::const_iterator iter =
      r_stdvec_wGrammarPartPath.begin() ;
        iter < r_stdvec_wGrammarPartPath.end() ; ++ iter )
    {
      str += mp_parsebyrise->GetGrammarPartName( *iter ) + "." ;
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
      str += mp_parsebyrise->GetGrammarPartName( ar_wGrammarPartPath[ wIndex ] )
        + "." ;
    }
  }
  return str ;
}

//Getting the "person index":
// e.g. "I, you and the car suck."
//  "I"
void TranslateParseByRiseTree::Translate( ParseByRise & r_parsebyrise )
{
  BYTE byPersonIndex ;
  ParseByRise * mp_parsebyrise = & r_parsebyrise ;
  std::string stdstrWholeTransl ;
  std::string stdstrTranslation ;
  DEBUG_COUT( "Translate(): \n" )
  if( mp_parsebyrise )
  {
    DWORD dwLeftMostTokenIndex = 0 ;
    GrammarPart * p_grammarpart ;
    GrammarPart * p_grammarpartChild ;
    //int y = 10 ;
    std::multimap<DWORD, GrammarPart>::const_iterator citer ;
    std::multimap<DWORD, GrammarPart> &
      r_stdmultimap_dwLeftmostIndex2grammarpart = mp_parsebyrise->
      m_stdmultimap_dwLeftmostIndex2grammarpart ;
    //Before each draw in order to begin at x position "0".
    m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.clear() ;
    m_wParseLevel = 0 ;
    std::cout << "Translate(): mp_parsebyrise != NULL\n" ;
    citer = r_stdmultimap_dwLeftmostIndex2grammarpart.begin() ;
    p_grammarpart = mp_parsebyrise->GetGrammarPartCoveringMostTokens(
        dwLeftMostTokenIndex ) ;
    if( p_grammarpart )
    {
      DEBUG_COUT( "Translate: GetGrammarPartCoveringMostTokens found \n" );
//      const std::string & r_stdstrGrammarPartName = //citer->second.
//        //m_stdstrGrammarPartName ;
//        mp_parsebyrise->GetGrammarPartName( p_grammarpart->m_wGrammarPartID ) ;
//      wxpaintdc.DrawText( //citer->second.m_stdstrGrammarPartName
//        r_stdstrGrammarPartName
//         , wxPoint(200,0) ) ;

      m_stdvec_wGrammarPartPath.push_back( p_grammarpart->m_wGrammarPartID ) ;
      m_stdvec_p_grammarpartPath.push_back( p_grammarpart ) ;

      ++ m_wParseLevel ;
      DEBUG_COUT( "current syntax tree path:" << GetSyntaxTreePathAsName(
        //mp_parsebyrise
        m_stdvec_wGrammarPartPath ) << "\n" )
        
      WORD wSubjectID ;
//      if( ! mp_parsebyrise->GetGrammarPartID("subject", wSubjectID) )
//        wSubjectID = 65535 ;
      if( ! mp_parsebyrise->GetGrammarPartID("noun_construct", wSubjectID) )
        wSubjectID = 65535 ;
      do
      {
        //if subject: get person index (and store it in a GrammarPart
        //member variable)
        if( p_grammarpart->m_wGrammarPartID == wSubjectID )
          //Get the person index bitfield, i.e. 
        {
          //  "I" has person index "0",
          //  "you" has person indices "1" (singular, German: "du" ), 
          //     "4" (plural, German: "ihr"), "6" (German: "man")
//          mp_parsebyrise->GetSubjectPersonIndex( p_grammarpart);
          ParseTreeTraverser::SummarizePersonIndex spi(
            p_grammarpart ,
            mp_parsebyrise ,
            * this ) ;
          spi.Traverse() ;
          DEBUG_COUT( "result of getting persindex:"
            << (WORD) p_grammarpart->m_byPersonIndex )
        }
        DEBUG_COUT ( "current syntax tree path:" << GetSyntaxTreePathAsName(
          //r_parsebyrise
            m_stdvec_wGrammarPartPath ) << "\n" );
        //DrawGrammarPartChildren( p_grammarpart , wxpaintdc ) ;
        if( p_grammarpart->mp_grammarpartRightChild )
         {
          p_grammarpartChild = p_grammarpart->mp_grammarpartRightChild ;
//           std::cout << "pushinG "
//             << mp_parsebyrise->GetGrammarPartName(
//               //p_grammarpart->mp_grammarpartRightChild->
//               p_grammarpartChild->m_wGrammarPartID )
//             << p_grammarpartChild
//             << " current size="
//             << m_stdvecNodesToProcess.size()
//             << "\n" ;
           //Add to process possible children of the right nodes later.
           m_stdvecNodesToProcess.push_back( //p_grammarpart->mp_grammarpartRightChild
             p_grammarpartChild ) ;
         }
        if( p_grammarpart->mp_grammarpartLeftChild )
        {
          p_grammarpart = p_grammarpart->mp_grammarpartLeftChild ;

          m_stdvec_wGrammarPartPath.push_back(
              p_grammarpart->m_wGrammarPartID ) ;
          m_stdvec_p_grammarpartPath.push_back( p_grammarpart) ;

//          std::cout << "current syntax tree path:" << GetSyntaxTreePathAsName(
//            r_parsebyrise ) << "\n" ;
          ++ m_wParseLevel ;
        }
        else //No further child->Left _leave_ of parse tree found.
        {
          DEBUG_COUT( "Translate(): current grammar part type:" <<
              mp_parsebyrise->GetGrammarPartName(
              p_grammarpart->m_wGrammarPartID ) << " current size=" <<
                  m_stdvecNodesToProcess.size() << "\n" ) ;
          //ApplyTranslationRule( ) ;
#ifdef _DEBUG
          if( TranslationRuleApplies( stdstrTranslation , byPersonIndex ,
              m_stdvec_wGrammarPartPath ,
              m_stdvec_p_grammarpartPath )
            )
          {
            DEBUG_COUT( "translation rule applies\n" ) ;
            stdstrWholeTransl += stdstrTranslation + " " ;
            //TODO combine the person indices _until_ the parent is "subject"
            // because:
            // If clause with relative clause:
            // The man   that you fear    sucks.
            // ---S--- -relative clause-
            //                -S- --P-    --P--
            p_grammarpart->m_byPersonIndex = byPersonIndex ;
          }
#endif
          switch( p_grammarpart->m_wGrammarPartID )
          {
          case EnglishWord::noun:
            DEBUG_COUT(  "translation:" <<
              p_grammarpart->m_pvocabularyandtranslation->
              m_arstrGermanWord[0] <<
              "\n" ) ;
            break ;
//          case EnglishWord::main_verb
//            break ;
          }
          m_stdvec_wGrammarPartPath.pop_back() ;
          m_stdvec_p_grammarpartPath.pop_back() ;

          //std::vector<GrammarPart *>::iterator iter =
          //Assign previously stored right child.
          p_grammarpart =
            m_stdvecNodesToProcess.
            //"Returns a read/write reference to the data at the last
            //*  element of the %vector."
            back() ;
//          std::cout << "current syntax tree path:" << GetSyntaxTreePathAsName(
//            r_parsebyrise ) << "\n" ;

          m_stdvec_wGrammarPartPath.push_back(
              p_grammarpart->m_wGrammarPartID ) ;
          m_stdvec_p_grammarpartPath.push_back( p_grammarpart) ;

          //p_grammarpart = *iter ;
          DEBUG_COUT( "Translate(): popping " <<
              mp_parsebyrise->GetGrammarPartName(
              p_grammarpart->m_wGrammarPartID ) << " current size=" <<
                  m_stdvecNodesToProcess.size() << "\n" ) ;
//          DrawGrammarPartNameAndPossiblyToken( *iter , wxpaintdc) ;
          m_stdvecNodesToProcess.pop_back() ;
          //std::cout.flush() ;
          //usleep(100000) ;
          -- m_wParseLevel ;
        }
      }
      while( ! m_stdvecNodesToProcess.empty() ) ;
    }
  }
  DEBUG_COUT("translation: " << stdstrWholeTransl << "\n") ;
}

bool TranslateParseByRiseTree::TranslationRuleApplies( 
  std::string & r_stdstrTranslation ,
  BYTE & r_byPersonIndex
  // So it can be used with data from outside this class.
  , const std::vector<WORD> & r_stdvec_wGrammarPartPath
  , const std::vector<GrammarPart * > & r_stdvec_p_grammarpartPath
  )
{
  bool bIdentical = false ;
  DEBUG_COUT("TranslationRuleApplies--beg\n")
  DEBUG_COUT("number of translation rules:" <<
      m_stdmap_translationrule2ConditionsAndTranslation.size() << "\n")
//   ;
//  m_stdvec_wGrammarPartPath ;
  for( std::map<TranslationRule * ,ConditionsAndTranslation>::const_iterator
      c_iter_p_translationrule2conditionsandtranslation =
        m_stdmap_translationrule2ConditionsAndTranslation.begin() ;
      c_iter_p_translationrule2conditionsandtranslation !=
        m_stdmap_translationrule2ConditionsAndTranslation.end() ;
      ++ c_iter_p_translationrule2conditionsandtranslation
      )
  {
    //E.g. m_stdvec_wGrammarPartPath is
    // "clause.subject.noun_construct.noun.definite_article", so it has the
    //  elements
    //  -"clause", "subject", "noun_construct", "noun", "definite_article",
    //  and so the size is 5.
    //now compare it e.g. to iter ( element of
    //  m_stdmap_translationrule2ConditionsAndTranslation ) that may be
    // "subject.noun_construct.noun.definite_article", i.e. a subtree of
    // m_stdvec_wGrammarPartPathw with just 4 elements.
    if ( //m_stdvec_wGrammarPartPath.size()
        r_stdvec_wGrammarPartPath.size() >= //iter->first->m_wNumberOfElements
        c_iter_p_translationrule2conditionsandtranslation->first->
        m_SyntaxTreePath.m_wNumberOfElements
        )
    {
      //Use a pointer, else the TranslationRule destructor is called.
      const TranslationRule * p_translationrule =
          c_iter_p_translationrule2conditionsandtranslation->first ;
      WORD wLenghtDiff = r_stdvec_wGrammarPartPath.size() -
          //iter->first->m_wNumberOfElements
          c_iter_p_translationrule2conditionsandtranslation->first->
          m_SyntaxTreePath.m_wNumberOfElements ;
      bIdentical = true ;
#ifdef _DEBUG
      std::string stdstr = GetSyntaxTreePathAsName(r_stdvec_wGrammarPartPath ) ;
      std::string stdstr2 = GetSyntaxTreePathAsName(
          //p_tr->m_ar_wElements, p_tr->m_wNumberOfElements
          p_translationrule->m_SyntaxTreePath.m_ar_wElements,
          p_translationrule->m_SyntaxTreePath.m_wNumberOfElements
          ) ;
      std::cout << "Comparing " << stdstr << " and " << stdstr2 << "\n" ;
#endif
//      std::vector<WORD>::const_iterator c_iter_wGrammarPartPath =
//          r_stdvec_wGrammarPartPath ;
      //DEBUG_COUT
      //Compare from end to begin.
      for( WORD wIndex = //p_tr->m_wNumberOfElements - 1 ;
          p_translationrule->m_SyntaxTreePath.m_wNumberOfElements - 1 ;
          wIndex != MAXWORD  ; -- wIndex )
      {
        DEBUG_COUT("comparing " << //p_tr->m_ar_wElements [ wIndex ] <<
            p_translationrule->m_SyntaxTreePath.m_ar_wElements [ wIndex ] <<
          mp_parsebyrise->GetGrammarPartName( //p_tr->m_ar_wElements [ wIndex ]
              p_translationrule->m_SyntaxTreePath.m_ar_wElements [ wIndex ]
          )
          << " and "
          << r_stdvec_wGrammarPartPath.at(wIndex + wLenghtDiff )
          << mp_parsebyrise->GetGrammarPartName(
              r_stdvec_wGrammarPartPath.at(wIndex + wLenghtDiff ) )
          << "\n"
          )
        if( //p_tr->m_ar_wElements [ wIndex ] !=
            p_translationrule->m_SyntaxTreePath.m_ar_wElements [ wIndex ] !=
          r_stdvec_wGrammarPartPath.at(wIndex + wLenghtDiff )
          )
        {
          bIdentical = false ;
          DEBUG_COUT("TranslationRuleApplies--not identical\n")
          break ;
        }
      }
      if( bIdentical )
      {
#ifdef _DEBUG
        std::string stdstr = GetSyntaxTreePathAsName(r_stdvec_wGrammarPartPath ) ;
#endif
        const ConditionsAndTranslation & r_cnt =
            c_iter_p_translationrule2conditionsandtranslation->second ;
        if( AllConditionsMatch(
            r_cnt ,
            r_stdvec_p_grammarpartPath )
          )
        {
           //std::string str =
           r_stdstrTranslation = GetTranslationEquivalent(
              r_cnt ,
              r_stdvec_p_grammarpartPath
              ) ;
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
  DEBUG_COUT("begin of ~TranslateParseByRiseTree\n")
  for( std::map<TranslationRule *,ConditionsAndTranslation>::const_iterator
      it = m_stdmap_translationrule2ConditionsAndTranslation.begin() ; it !=
      m_stdmap_translationrule2ConditionsAndTranslation.end() ; ++ it )
  {
    //Release mem.
    delete it->first ;
  }
  DEBUG_COUT("end of ~TranslateParseByRiseTree\n")
}
