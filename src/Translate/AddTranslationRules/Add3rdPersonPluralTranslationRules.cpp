/*
 * Add3rdPersonPluralTranslationRules.cpp
 *
 *  Created on: Dec 19, 2010
 *      Author: Stefan
 */

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
  TranslationRule * p_tr_plural_noun = new TranslationRule(
    "plural_noun" // For this syntax tree GrammarPart path in syntax tree.
    , mp_parsebyrise ) ;
  p_tr_plural_noun->SetConsecutiveIDSyntaxTreePath(
    "definite_article_plural.plural_noun" ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_tr_plural_noun ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntThirdPersPluralNoun
  ) ;
  ConditionsAndTranslation cntThirdPersPluralVerb;
  cntThirdPersPluralVerb.SetSyntaxTreePath( //"main_verb.3rdPersPluralClause",
    "mainVerbInf0Obj" ,//.3rdPersPluralClause" ,
    mp_parsebyrise ) ;
  //="If subject is in third person plural, use German verb attribute value
  //for plural."
  cntThirdPersPluralVerb.m_stdstrAttributeName =
      "Ger_main_verb_3rd_person_plural" ;
  cntThirdPersPluralVerb.m_byPersonIndex = //THIRD_PERSON_SINGULAR ;
      VocabularyAndTranslation::third_person_plural ;

  //Creating on heap is faster when inserting into a container: only the pointer
  //address is copied into the container instead of a whole object.
  TranslationRule * p_tr3rd_pers_plur_pres = new TranslationRule(
    // For this syntax tree GrammarPart path in syntax tree.
    //"3rdPersPluralClause.main_verb"
    "3rdPersPluralClause.mainVerbInf0Obj"
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_tr3rd_pers_plur_pres ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntThirdPersPluralVerb
  ) ;
  //Creating on heap is faster when inserting into a container: only the pointer
  //address is copied into the container instead of a whole object.
  TranslationRule * p_tr3rd_pers_plur_pres1obj = new TranslationRule(
    // For this syntax tree GrammarPart path in syntax tree.
    //"3rdPersPluralClause.main_verb"
    "3rdPersPluralClause1Obj.mainVerbInf1Obj"
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_tr3rd_pers_plur_pres1obj ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntThirdPersPluralVerb
  ) ;
  //Creating on heap is faster when inserting into a container: only the pointer
  //address is copied into the container instead of a whole object.
  TranslationRule * p_tr3rd_pers_plur_pres2obj = new TranslationRule(
    // For this syntax tree GrammarPart path in syntax tree.
    //"3rdPersPluralClause.main_verb"
    "3rdPersPluralClause1Obj.mainVerbInf2Obj"
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_tr3rd_pers_plur_pres2obj ,
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
  p_trDefinitePluralArticle->SetConsecutiveIDSyntaxTreePath(
    "definite_article_plural.definite_article" ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_trDefinitePluralArticle ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntDefinitePluralArticle
  ) ;
}
