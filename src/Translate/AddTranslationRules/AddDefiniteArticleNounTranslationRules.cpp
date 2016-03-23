/*
 * AddDefiniteArticleNounTranslationRules.cpp
 *
 *  Created on: Dec 20, 2010
 *      Author: Stefan
 */

//a definite article that preceeds a noun can be either singular or plural.
// especially for noun where the singular and plural form are the same, both
//should be an option for the German translation.
void //TranslateParseByRiseTree::
  AddDefiniteArticleNounTranslationRules()
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
  conditionMatchesEngSing.SetSyntaxTreePath( "def_article_noun.noun" ,
    mp_parsebyrise ) ;
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
      m_stdmap_p_translationrule2ConditionsAndTranslation.find(p_tr2) ;
  if( ci !=
      m_stdmap_p_translationrule2ConditionsAndTranslation.end() )
  {
    const Condition & cr_cond = ci->second.m_conditions.front() ;
    std::string stdstr = cr_cond.m_syntaxtreepath.
        GetAs_std_string() ;
    DEBUG_COUT("TranslateParseByRiseTree()--syntax tree path string from vectr: "
      << stdstr)
  }
#endif

  //TODO some nouns are singular in English but plural in German, e.g. "the news
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
