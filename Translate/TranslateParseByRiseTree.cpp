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
#include <Translate/TranslateTreeTraverser.hpp>
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
  //  subject.def_article_noun.noun.German_gender]"
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
    std::vector<Condition> r_vec_condition =
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
  const ConditionsAndTranslation & r_cnt
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
  if( r_cnt.m_stdstrGermanTranslation ==  "" )
  {
    bAllConditionsMatch = true ;
    //if( r_cond.m_syntaxtreepath.IsPartOf(m_stdvec_wCurrentGrammarPartPath) )
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
//                BYTE by =
                  p_grammarpartLeaf->
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

void TranslateParseByRiseTree::AddVocAndTranslDefinitions()
{
  AddVocAndTranslDefinition(
    "main_verb",
    "Ger_main_verb_3rd_person_singular" , //attribute name to use as a map key value
    AttributeTypeAndPosAndSize::German, //language
    AttributeTypeAndPosAndSize::string, //attribute is a string or some bits
    //GERMAN_VERB_3RD_PERSON_SINGULAR_INDEX , //index
    VocabularyAndTranslation::third_person_singular ,
    1 // length
    );
  AddVocAndTranslDefinition(
    "main_verb",
    "Ger_main_verb_3rd_person_plural" , //attribute name to use as a map key value
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
    "German_noun_article" , //attribute name to use as a map key value
    AttributeTypeAndPosAndSize::German, //language
    AttributeTypeAndPosAndSize::bit, //attribute is a string or some bits
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
  AddVocAndTranslDefinition(
    "pers_pronoun",
    "pers_pronoun_German" , //attribute name to use as a map key value
    AttributeTypeAndPosAndSize::German, //language
    AttributeTypeAndPosAndSize::string, //attribute is a string or some bits
    0 , //index
    1 // length
    );
}

//This rule exists especially for cases where the singular and plural are the
//same for a noun (e.g. "1 sheep"; "2 sheep")
void TranslateParseByRiseTree::Add3rdPersonPluralTranslationRules()
{
  ConditionsAndTranslation cntThirdPersPluralNoun;
  cntThirdPersPluralNoun.SetSyntaxTreePath( "plural_noun", mp_parsebyrise ) ;
  //="If subject is in third person plural, use German verb attribute value
  //for plural."
  cntThirdPersPluralNoun.m_stdstrAttributeName = "German_plural" ;
  cntThirdPersPluralNoun.m_byPersonIndex = //THIRD_PERSON_SINGULAR ;
      VocabularyAndTranslation::third_person_plural ;

  //must create on heap.
  TranslationRule * p_tr_singular_noun = new TranslationRule(
    "plural_noun" // For this syntax tree GrammarPart path in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_tr_singular_noun ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntThirdPersPluralNoun
  ) ;
  ConditionsAndTranslation cntThirdPersPluralVerb;
  cntThirdPersPluralVerb.SetSyntaxTreePath( "main_verb.3rdPersPluralClause",
    mp_parsebyrise ) ;
  //="If subject is in third person plural, use German verb attribute value
  //for plural."
  cntThirdPersPluralVerb.m_stdstrAttributeName =
      "Ger_main_verb_3rd_person_plural" ;
  cntThirdPersPluralVerb.m_byPersonIndex = //THIRD_PERSON_SINGULAR ;
      VocabularyAndTranslation::third_person_plural ;

  //must create on heap.
  TranslationRule * p_tr3rd_pers_sing_pres = new TranslationRule(
    "3rdPersPluralClause.main_verb" // For this syntax tree GrammarPart path in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_tr3rd_pers_sing_pres ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntThirdPersPluralVerb
  ) ;

//  Condition condMatchesSingular ;
//  condMatchesSingular.SetSyntaxTreePath(
//    "definite_article_singular.singular_noun" , mp_parsebyrise ) ;
//  //= If the token for grammar part "noun" matches the singular in the
//  // VocabularyAndTranslation object.
//  condMatchesSingular.m_stdstrAttributeName = "Eng_singular" ;

  ConditionsAndTranslation cntDefinitePluralArticle ;
  cntDefinitePluralArticle.m_stdstrGermanTranslation =
    //equals english plural -> translation is "die".
    "die" ;
  //must create on heap.
  TranslationRule * p_trDefinitePluralArticle = new TranslationRule(
    "definite_article_plural.definite_article" // For this syntax tree GrammarPart path
    //in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_trDefinitePluralArticle ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntDefinitePluralArticle
  ) ;
}

void TranslateParseByRiseTree::Add3rdPersonSingularTranslationRules()
{
  ConditionsAndTranslation cntThirdPersSingNoun;
  cntThirdPersSingNoun.SetSyntaxTreePath( "singular_noun", mp_parsebyrise ) ;
  //="If subject is in third person plural, use German verb attribute value
  //for plural."
  cntThirdPersSingNoun.m_stdstrAttributeName = "German_singular" ;
  cntThirdPersSingNoun.m_byPersonIndex = //THIRD_PERSON_SINGULAR ;
      VocabularyAndTranslation::third_person_singular ;

  //must create on heap.
  TranslationRule * p_tr_singular_noun = new TranslationRule(
    "singular_noun" // For this syntax tree GrammarPart path in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_tr_singular_noun ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntThirdPersSingNoun
  ) ;
  ConditionsAndTranslation cntThirdPersSingVerb;
  cntThirdPersSingVerb.SetSyntaxTreePath( "3rd_pers_sing_pres", mp_parsebyrise ) ;
  //="If subject is in third person plural, use German verb attribute value
  //for plural."
  cntThirdPersSingVerb.m_stdstrAttributeName = "Ger_main_verb_3rd_person_singular" ;
  cntThirdPersSingVerb.m_byPersonIndex = //THIRD_PERSON_SINGULAR ;
      VocabularyAndTranslation::third_person_singular ;

  //must create on heap.
  TranslationRule * p_tr3rd_pers_sing_pres = new TranslationRule(
    "3rd_pers_sing_pres" // For this syntax tree GrammarPart path in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_tr3rd_pers_sing_pres ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntThirdPersSingVerb
  ) ;

//  Condition condMatchesSingular ;
//  condMatchesSingular.SetSyntaxTreePath(
//    "definite_article_singular.singular_noun" , mp_parsebyrise ) ;
//  //= If the token for grammar part "noun" matches the singular in the
//  // VocabularyAndTranslation object.
//  condMatchesSingular.m_stdstrAttributeName = "Eng_singular" ;
  Condition condGermanArticleForNoun ;
  condGermanArticleForNoun.SetSyntaxTreePath(
    "definite_article_singular.singular_noun" ,
    mp_parsebyrise ) ;
  //= If the token for grammar part "noun" matches the singular in the
  // VocabularyAndTranslation object.
  condGermanArticleForNoun.m_stdstrAttributeName = "German_noun_article" ;
  condGermanArticleForNoun.m_byAttributeValue =
      //0==maennlich
      0 ;
  ConditionsAndTranslation cntDefiniteSingularMaleArticle ;
  cntDefiniteSingularMaleArticle.AddCondition(//condGermanMaleArticleForNoun
    condGermanArticleForNoun) ;
  cntDefiniteSingularMaleArticle.m_stdstrGermanTranslation =
      //equals english singular AND german article is "maennlich"-> translation
      //is "der".
      "der" ;
  //must create on heap.
  TranslationRule * p_trDefiniteSingularMaleArticle = new TranslationRule(
    "definite_article_singular.definite_article" // For this syntax tree GrammarPart path
    //in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_trDefiniteSingularMaleArticle ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntDefiniteSingularMaleArticle
  ) ;
//  cntDefiniteSingularMaleArticle.m_byPersonIndex = //THIRD_PERSON_PLURAL ;
//      VocabularyAndTranslation::third_person_singular ;

//  Condition condGermanFemaleArticleForNoun ;
//  condGermanFemaleArticleForNoun.SetSyntaxTreePath(
//    "definite_article_singular.singular_noun" ,
//    mp_parsebyrise ) ;
//  //= If the token for grammar part "noun" matches the singular in the
//  // VocabularyAndTranslation object.
//  condGermanFemaleArticleForNoun.m_stdstrAttributeName = "German_noun_article" ;
//  condGermanFemaleArticleForNoun.m_byAttributeValue =
  condGermanArticleForNoun.m_byAttributeValue =
      //1==weiblich
      1 ;
  ConditionsAndTranslation cntDefiniteSingularFemaleArticle ;
  cntDefiniteSingularFemaleArticle.AddCondition(condGermanArticleForNoun) ;
  cntDefiniteSingularFemaleArticle.m_stdstrGermanTranslation =
      //equals english singular AND german article is "maennlich"-> translation
      //is "der".
      "die" ;

  TranslationRule * p_trDefiniteSingularFemaleArticle = new TranslationRule(
    "definite_article_singular.definite_article" // For this syntax tree GrammarPart path
    //in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_trDefiniteSingularFemaleArticle ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntDefiniteSingularFemaleArticle
  ) ;
//  Condition condGermanNeuterArticleForNoun ;
//  condGermanNeuterArticleForNoun.SetSyntaxTreePath( "def_article_noun.noun" ,
//    mp_parsebyrise ) ;
//  //= If the token for grammar part "noun" matches the singular in the
//  // VocabularyAndTranslation object.
//  condGermanNeuterArticleForNoun.m_stdstrAttributeName = "German_noun_article" ;
//  condGermanNeuterArticleForNoun.m_byAttributeValue =
      condGermanArticleForNoun.m_byAttributeValue =
      //2==saechlich
      2 ;
  ConditionsAndTranslation cntDefiniteSingularNeuterArticle ;
  cntDefiniteSingularNeuterArticle.AddCondition(condGermanArticleForNoun) ;
  cntDefiniteSingularNeuterArticle.m_stdstrGermanTranslation =
      //equals english singular AND german article is "maennlich"-> translation
      //is "der".
      "das" ;

  TranslationRule * p_trDefiniteSingularNeutralArticle = new TranslationRule(
    "definite_article_singular.definite_article" // For this syntax tree GrammarPart path
    //in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_trDefiniteSingularNeutralArticle ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntDefiniteSingularNeuterArticle
  ) ;
}

//a definite article that preceeds a noun can be either singular or plural.
// especially for noun where the singular and plural form are the same, both
//should be an option for the German translation.
void TranslateParseByRiseTree::AddDefiniteArticleNounTranslationRules()
{
  Condition condDefArtNounMatchesEngPlur ;
  condDefArtNounMatchesEngPlur.SetSyntaxTreePath( "def_article_noun.noun" ,
    mp_parsebyrise ) ;
  //= If the token for grammar part "noun" matches the singular in the
  // VocabularyAndTranslation object.
  condDefArtNounMatchesEngPlur.m_stdstrAttributeName = "Eng_plural" ;
  ConditionsAndTranslation cntDefiniteArticlePlural ;
  cntDefiniteArticlePlural.AddCondition(condDefArtNounMatchesEngPlur) ;
#ifdef _DEBUG
  std::string stdstr = cntDefiniteArticlePlural.m_conditions.front().m_syntaxtreepath.
      GetAs_std_string() ;
  DEBUG_COUT("TranslateParseByRiseTree()--syntax tree path string from vector: "
    << stdstr)
#endif
  cntDefiniteArticlePlural.m_stdstrGermanTranslation = "die" ;
//  cntDefiniteArticlePlural.m_byPersonIndex = //THIRD_PERSON_PLURAL ;
//      VocabularyAndTranslation::third_person_plural ;
  //must create on heap.
  TranslationRule * p_tr = new TranslationRule(
    "def_article_noun.definite_article" // For this syntax tree GrammarPart path
    //in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_tr ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntDefiniteArticlePlural
  ) ;

  Condition condMatchesSingular ;
  condMatchesSingular.SetSyntaxTreePath( "def_article_noun.noun" , mp_parsebyrise ) ;
  //= If the token for grammar part "noun" matches the singular in the
  // VocabularyAndTranslation object.
  condMatchesSingular.m_stdstrAttributeName = "Eng_singular" ;
  Condition condGermanMaleArticleForNoun ;
  condGermanMaleArticleForNoun.SetSyntaxTreePath( "def_article_noun.noun" ,
    mp_parsebyrise ) ;
  //= If the token for grammar part "noun" matches the singular in the
  // VocabularyAndTranslation object.
  condGermanMaleArticleForNoun.m_stdstrAttributeName = "German_noun_article" ;
  condGermanMaleArticleForNoun.m_byAttributeValue = 0 ;
  ConditionsAndTranslation cntDefiniteSingularMaleArticle ;
  cntDefiniteSingularMaleArticle.AddCondition(condMatchesSingular) ;
  cntDefiniteSingularMaleArticle.AddCondition(condGermanMaleArticleForNoun) ;
#ifdef _DEBUG
  stdstr = cntDefiniteArticlePlural.m_conditions.front().m_syntaxtreepath.
      GetAs_std_string() ;
  DEBUG_COUT("TranslateParseByRiseTree()--syntax tree path string from vector: "
    << stdstr)
#endif
  cntDefiniteSingularMaleArticle.m_stdstrGermanTranslation =
      //equals english singular AND german article is "maennlich"-> translation
      //is "der".
      "der" ;
//  cntDefiniteSingularMaleArticle.m_byPersonIndex = //THIRD_PERSON_PLURAL ;
//      VocabularyAndTranslation::third_person_singular ;
  //must create on heap.
  TranslationRule * p_trDefiniteSingularArticle = new TranslationRule(
    "def_article_noun.definite_article" // For this syntax tree GrammarPart path
    //in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_trDefiniteSingularArticle ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntDefiniteSingularMaleArticle
  ) ;

  //TODO for nouns where the sing equals the plural (like "sheep"):
  //  (additional) condition should be "when indefinite article ("a"): singular"
  Condition conditionMatchesEngSing ;
  conditionMatchesEngSing.SetSyntaxTreePath( "def_article_noun.noun" , mp_parsebyrise ) ;
  conditionMatchesEngSing.m_stdstrAttributeName = "Eng_singular" ;
  ConditionsAndTranslation cntSingularNoun ;
  cntSingularNoun.AddCondition(conditionMatchesEngSing) ;
  cntSingularNoun.SetSyntaxTreePath( "def_article_noun.noun", mp_parsebyrise ) ;
  cntSingularNoun.m_stdstrAttributeName = "German_singular" ;
  cntSingularNoun.m_byPersonIndex = //THIRD_PERSON_SINGULAR ;
      VocabularyAndTranslation::third_person_singular ;
  TranslationRule * p_tr2 = new TranslationRule(
    "def_article_noun.noun" // For this syntax tree GrammarPart path in
    //syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_tr2 ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntSingularNoun
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

  //TODO some nouns are singualr in English but plural in German, e.g. "the news
  //is on." (singular) -> "Die Nachrichten (plural) fangen an."
  //Therefore a(n additional) condition for the appropriate bit value.
  Condition conditionMatchesEngPlural;
  conditionMatchesEngPlural.SetSyntaxTreePath( "noun" , mp_parsebyrise ) ;
  conditionMatchesEngPlural.m_stdstrAttributeName = "Eng_plural" ;
  ConditionsAndTranslation cntPluralNoun ;
  cntPluralNoun.AddCondition(conditionMatchesEngPlural) ;
  cntPluralNoun.SetSyntaxTreePath( "noun", mp_parsebyrise ) ;
  cntPluralNoun.m_stdstrAttributeName = "German_plural" ;
  cntPluralNoun.m_byPersonIndex = //THIRD_PERSON_SINGULAR ;
      VocabularyAndTranslation::third_person_plural ;

  //cond.SetAttributename("isSingular") ;
  //cond.SetCompareOperator("=") ;
  //cond.SetAttributeValue("1") ;
  //must create on heap.
  TranslationRule * p_trPluralNoun = new TranslationRule(
    "noun" // For this syntax tree GrammarPart path in
    //syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_trPluralNoun ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntPluralNoun
  ) ;
}


void TranslateParseByRiseTree::AddPersonalPronounTranslationRules()
{
  Condition conditonPersPronoun ;
  conditonPersPronoun.SetSyntaxTreePath( "pers_pronoun" , mp_parsebyrise ) ;
  conditonPersPronoun.m_stdstrAttributeName = "person_index" ;
  //TODO
//  cond3.m_wAttributeValue = THIRD_PERSON_PLURAL ;
  ConditionsAndTranslation cntPersPronounWordClass ;
  cntPersPronounWordClass.AddCondition(conditonPersPronoun) ;
  cntPersPronounWordClass.SetSyntaxTreePath( "pers_pronoun", mp_parsebyrise ) ;
  //="If subject is in third person plural, use German verb attribute value
  //for plural."
  cntPersPronounWordClass.m_stdstrAttributeName = //"pers_pronoun_person_index" ;
      "pers_pronoun_German" ;
  //cnt3.m_byPersonIndex = THIRD_PERSON_PLURAL ;
  //must create on heap.
  TranslationRule * p_trPersPronounWordClass = new TranslationRule(
    "pers_pronoun" // For this syntax tree GrammarPart path in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_trPersPronounWordClass ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntPersPronounWordClass
  ) ;

  ConditionsAndTranslation cntPersPronoun ;
//  cntPersPronoun.SetSyntaxTreePath( "he", mp_parsebyrise ) ;
//  //="If subject is in third person plural, use German verb attribute value
//  //for plural."
//  cntPersPronoun.m_stdstrAttributeName = //"pers_pronoun_person_index" ;
//      "pers_pronoun_German" ;
  cntPersPronoun.m_stdstrGermanTranslation = "er" ;
  //cnt3.m_byPersonIndex = THIRD_PERSON_PLURAL ;
  //must create on heap.
  TranslationRule * p_trPersPronoun = new TranslationRule(
    "he" // For this syntax tree GrammarPart path in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_trPersPronoun ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntPersPronoun
  ) ;
  cntPersPronoun.m_stdstrGermanTranslation = "sie" ;
  p_trPersPronoun = new TranslationRule(
    "she" // For this syntax tree GrammarPart path in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_trPersPronoun ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntPersPronoun
  ) ;
  cntPersPronoun.m_stdstrGermanTranslation = "es" ;
  p_trPersPronoun = new TranslationRule(
    "it" // For this syntax tree GrammarPart path in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_trPersPronoun ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntPersPronoun
  ) ;
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

  AddDefiniteArticleNounTranslationRules() ;
  AddPersonalPronounTranslationRules() ;

  ConditionsAndTranslation cntAnd ;
//  cntAnd.SetSyntaxTreePath( "and", mp_parsebyrise ) ;
  cntAnd.m_stdstrGermanTranslation = "und" ;
  //must create on heap.
  TranslationRule * p_trAnd = new TranslationRule(
    "and" // For this syntax tree GrammarPart path in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_trAnd ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntAnd
  ) ;

  //Condition for translating the predicate.
  Condition conditonSubject ;
  conditonSubject.SetSyntaxTreePath( "subject" , mp_parsebyrise ) ;
  conditonSubject.m_stdstrAttributeName = "person_index" ;
  //TODO
//  cond3.m_wAttributeValue = THIRD_PERSON_PLURAL ;
  ConditionsAndTranslation cntMainVerb ;
  cntMainVerb.AddCondition(conditonSubject) ;
  cntMainVerb.SetSyntaxTreePath( "main_verb", mp_parsebyrise ) ;
  //="If subject is in third person plural, use German verb attribute value
  //for plural."
  cntMainVerb.m_stdstrAttributeName = "Ger_main_verb_3rd_person_plural" ;
  //cnt3.m_byPersonIndex = THIRD_PERSON_PLURAL ;
  //must create on heap.
  TranslationRule * p_tr3 = new TranslationRule(
    "main_verb" // For this syntax tree GrammarPart path in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_tr3 ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntMainVerb
  ) ;
//  Condition conditonSubject ;
//  conditonSubject.SetSyntaxTreePath( "article_singular" , mp_parsebyrise ) ;
//  conditonSubject.m_stdstrAttributeName = "person_index" ;
//  //TODO
////  cond3.m_wAttributeValue = THIRD_PERSON_PLURAL ;

  Add3rdPersonPluralTranslationRules() ;
  Add3rdPersonSingularTranslationRules() ;

  AddVocAndTranslDefinitions() ;

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
  , std::string & stdstrWholeTransl
  )
{
//  BYTE byPersonIndex ;
  ParseByRise * mp_parsebyrise = & r_parsebyrise ;
//  std::string stdstrWholeTransl ;
  std::string stdstrTranslation ;
  DEBUG_COUT( "Translate(): \n" )
  if( mp_parsebyrise )
  {
    DWORD dwLeftMostTokenIndex = 0 ;
    GrammarPart * p_grammarpart ;
//    GrammarPart * p_grammarpartChild ;
    //int y = 10 ;
    std::multimap<DWORD, GrammarPart>::const_iterator citer ;
    std::multimap<DWORD, GrammarPart> &
      r_stdmultimap_dwLeftmostIndex2grammarpart = mp_parsebyrise->
      m_stdmultimap_dwLeftmostIndex2grammarpart ;
    //Before each draw in order to begin at x position "0".
    m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.clear() ;
    m_wParseLevel = 0 ;
    DEBUG_COUT( "Translate(): mp_parsebyrise != NULL\n")
    citer = r_stdmultimap_dwLeftmostIndex2grammarpart.begin() ;
    p_grammarpart = mp_parsebyrise->GetGrammarPartCoveringMostTokens(
        dwLeftMostTokenIndex ) ;
    if( p_grammarpart )
    {
      DEBUG_COUT( "Translate: GetGrammarPartCoveringMostTokens found \n" );

      ParseTreeTraverser::TranslateTreeTraverser translatetreetraverser(
        p_grammarpart
        , * mp_parsebyrise
        , * this
        );
      translatetreetraverser.Traverse() ;
      stdstrWholeTransl = translatetreetraverser.m_stdstrWholeTranslation ;
    }
  }
  DEBUG_COUT("translation: " << stdstrWholeTransl << "\n") ;
}

bool TranslateParseByRiseTree::TranslationRuleApplies( 
  std::string & r_stdstrTranslation ,
  BYTE & r_byPersonIndex
  // So it can be used with data from outside this class.
  //within the parse tree there is at a time only 1 current path in 1 direction:
  //from root node to the current node.
  // e.g.     "clause"
  //          /     \
  //    "subject"    \
  //     /            \
  //personal_pronoun  verb
  //->the current parse tree path may be (always beginning from root node "clause")
  // -"clause"
  // -"clause->subject"
  // -"clause->subject->personal_pronoun"
  // -"clause->verb"
  //but NOT:
  , const std::vector<WORD> & r_stdvec_wCurrentGrammarPartPath
  , const std::vector<GrammarPart * > & r_stdvec_p_grammarpartPath
  )
{
  bool bIdentical = false ;
  DEBUG_COUT("TranslationRuleApplies--beg\n")
  DEBUG_COUT("number of translation rules:" <<
      m_stdmap_translationrule2ConditionsAndTranslation.size() << "\n")
//   ;
//  m_stdvec_wCurrentGrammarPartPath ;
  for( std::map<TranslationRule * ,ConditionsAndTranslation>::const_iterator
      c_iter_p_translationrule2conditionsandtranslation =
        m_stdmap_translationrule2ConditionsAndTranslation.begin() ;
      c_iter_p_translationrule2conditionsandtranslation !=
        m_stdmap_translationrule2ConditionsAndTranslation.end() ;
      ++ c_iter_p_translationrule2conditionsandtranslation
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
        c_iter_p_translationrule2conditionsandtranslation->first->
        m_SyntaxTreePath.m_wNumberOfElements
        )
    {
      //Use a pointer, else the TranslationRule destructor is called.
      const TranslationRule * p_translationrule =
          c_iter_p_translationrule2conditionsandtranslation->first ;
      WORD wLenghtDiff = r_stdvec_wCurrentGrammarPartPath.size() -
          //iter->first->m_wNumberOfElements
          c_iter_p_translationrule2conditionsandtranslation->first->
          m_SyntaxTreePath.m_wNumberOfElements ;
      bIdentical = true ;
#ifdef _DEBUG
      std::string stdstrCurrentParseTreePath = GetSyntaxTreePathAsName(
        r_stdvec_wCurrentGrammarPartPath ) ;
      std::string stdstr2 = GetSyntaxTreePathAsName(
          //p_tr->m_ar_wElements, p_tr->m_wNumberOfElements
          p_translationrule->m_SyntaxTreePath.m_ar_wElements,
          p_translationrule->m_SyntaxTreePath.m_wNumberOfElements
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
          << r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
          << mp_parsebyrise->GetGrammarPartName(
              r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff ) )
          << "\n"
          )
        if( //p_tr->m_ar_wElements [ wIndex ] !=
            p_translationrule->m_SyntaxTreePath.m_ar_wElements [ wIndex ] !=
          r_stdvec_wCurrentGrammarPartPath.at(wIndex + wLenghtDiff )
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
        std::string stdstr = GetSyntaxTreePathAsName(
          r_stdvec_wCurrentGrammarPartPath ) ;
        if( stdstr == "clause.main_verb" )
          stdstr += "" ;
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
    //Release memory occupied by creation on heap.
    delete it->first ;
  }
  DEBUG_COUT("end of ~TranslateParseByRiseTree\n")
}
