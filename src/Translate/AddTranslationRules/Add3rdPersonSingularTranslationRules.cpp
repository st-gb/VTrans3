/*
 * Add3rdPersonSingularTranslationRules.cpp
 *
 *  Created on: Dec 19, 2010
 *      Author: Stefan
 */

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
  cntThirdPersSingVerb.SetSyntaxTreePath( "MainVerb3rdPersSingPres0Obj",
    mp_parsebyrise ) ;
  //="If subject is in third person plural, use German verb attribute value
  //for plural."
  cntThirdPersSingVerb.m_stdstrAttributeName = "Ger_main_verb_3rd_person_singular" ;
  cntThirdPersSingVerb.m_byPersonIndex = //THIRD_PERSON_SINGULAR ;
      VocabularyAndTranslation::third_person_singular ;

  //must create on heap.
  TranslationRule * p_tr3rd_pers_sing_pres = new TranslationRule(
    "MainVerb3rdPersSingPres0Obj" // For this syntax tree GrammarPart path in syntax tree.
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
      VocabularyAndTranslation::noun_gender_male ;
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
      VocabularyAndTranslation::noun_gender_female ;
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
