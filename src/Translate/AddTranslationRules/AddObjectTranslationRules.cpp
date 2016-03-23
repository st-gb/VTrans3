/*
 * AddObjectTranslationRules.cpp
 *
 *  Created on: Dec 20, 2010
 *      Author: Stefan
 */

void //TranslateParseByRiseTree::
  AddObjectTranslationRules()
{
  Condition conditonVerbIsDative ;
  conditonVerbIsDative.SetSyntaxTreePath( "3rdPersPluralClauseWith1Obj."
    "3rdPersPluralClause1Obj.mainVerbInf1Obj" , mp_parsebyrise ) ;
  conditonVerbIsDative.m_stdstrAttributeName = "grammatical_case" ;
  conditonVerbIsDative.m_byAttributeValue = VocabularyAndTranslation::dative ;

  ConditionsAndTranslation cntDefiniteArticlePlur ;
  cntDefiniteArticlePlur.AddCondition( conditonVerbIsDative ) ;
  //"I trust the men" -> "ich vertraue >>den<< Männern."
  cntDefiniteArticlePlur.m_stdstrGermanTranslation = "den" ;
  //must create on heap.
  TranslationRule * p_trObjArticle = new TranslationRule(
    // For this syntax tree GrammarPart path in syntax tree.
    "3rdPersPluralClauseWith1Obj.obj."
    //"obj_enum_ele.SubjOrObjEnumEle."
    //"3rdPersPlurSubjOrObjEnumEle."
//    "article_singular.definite_article_plural."
//    "definite_article"
    //"plural_noun_superclass."
    "*."
    "definite_article_plural.definite_article"
    , mp_parsebyrise ) ;
  AddTranslationRule(
    p_trObjArticle ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    cntDefiniteArticlePlur
  ) ;
  ConditionsAndTranslation cntObjectPlurNoun ;
  cntObjectPlurNoun.AddCondition( conditonVerbIsDative ) ;
  cntObjectPlurNoun.m_pfn_TransformString = ::GetDativeFormForGermanPluralNoun ;
  cntObjectPlurNoun.m_stdstrAttributeName = "German_plural" ;
  //must create on heap.
  TranslationRule * p_trPluralNounDative = new TranslationRule(
    // For this syntax tree GrammarPart path in syntax tree.
    "3rdPersPluralClauseWith1Obj.obj."
    //"obj_enum_ele.SubjOrObjEnumEle."
    //"3rdPersPlurSubjOrObjEnumEle."
//    "article_singular.definite_article_plural."
//    "definite_article"
    //"plural_noun_superclass."
    "*."
    //"definite_article_plural."
    "plural_noun"
    , mp_parsebyrise ) ;
  AddTranslationRule(
    p_trPluralNounDative ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    cntObjectPlurNoun
  ) ;

  Condition condGermanArticleForNoun ;
  condGermanArticleForNoun.SetSyntaxTreePath(
    "definite_article_singular.singular_noun" ,
    mp_parsebyrise ) ;
  //= If the token for grammar part "noun" matches the singular in the
  // VocabularyAndTranslation object.
  condGermanArticleForNoun.m_stdstrAttributeName = "German_noun_article" ;

  ConditionsAndTranslation cntDefiniteMaleArticleSing ;
  condGermanArticleForNoun.m_byAttributeValue =
      VocabularyAndTranslation::noun_gender_male ;
  cntDefiniteMaleArticleSing.AddCondition( conditonVerbIsDative ) ;
  cntDefiniteMaleArticleSing.AddCondition( condGermanArticleForNoun ) ;

  ConditionsAndTranslation cntObjectSingNoun ;
  cntObjectSingNoun.AddCondition( conditonVerbIsDative ) ;
  cntObjectSingNoun.AddCondition( condGermanArticleForNoun ) ;
  //"I trust the boy" -> "ich vertraue >>dem<< Junge>>n<<."
  cntObjectSingNoun.m_pfn_TransformString = ::GetDativeFormForGermanSingularNoun ;
  cntObjectSingNoun.m_stdstrAttributeName = "German_plural" ;
  //must create on heap.
  TranslationRule * p_trSingularNounDative = new TranslationRule(
    // For this syntax tree GrammarPart path in syntax tree.
    "3rdPersPluralClauseWith1Obj.obj."
    //"obj_enum_ele.SubjOrObjEnumEle."
    //"3rdPersPlurSubjOrObjEnumEle."
//    "article_singular.definite_article_plural."
//    "definite_article"
    //"plural_noun_superclass."
    "*."
    //"definite_article_plural."
    "singular_noun"
    , mp_parsebyrise ) ;
  AddTranslationRule(
    p_trSingularNounDative ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    cntObjectSingNoun
  ) ;
  //"I trust the boy" -> "ich vertraue >>dem<< Junge>>n<<."
  cntDefiniteMaleArticleSing.m_stdstrGermanTranslation = "dem" ;
  //must create on heap.
  p_trObjArticle = new TranslationRule(
    // For this syntax tree GrammarPart path in syntax tree.
    "3rdPersPluralClauseWith1Obj.obj."
    "*."
    "definite_article_singular.definite_article"
    , mp_parsebyrise ) ;
  AddTranslationRule(
    p_trObjArticle ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    cntDefiniteMaleArticleSing
  ) ;
  condGermanArticleForNoun.m_byAttributeValue =
      VocabularyAndTranslation::noun_gender_female ;
  //"I trust the woman" -> "ich vertraue >>der<< Frau."
  ConditionsAndTranslation cntDefiniteFemaleArticleSing ;
  cntDefiniteFemaleArticleSing.m_stdstrGermanTranslation = "der" ;
  cntDefiniteFemaleArticleSing.AddCondition( conditonVerbIsDative ) ;
  cntDefiniteFemaleArticleSing.AddCondition( condGermanArticleForNoun ) ;
  //must create on heap.
  p_trObjArticle = new TranslationRule(
    // For this syntax tree GrammarPart path in syntax tree.
    "3rdPersPluralClauseWith1Obj.obj."
    "*."
    "definite_article_singular.definite_article"
    , mp_parsebyrise ) ;
  AddTranslationRule(
    p_trObjArticle ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    cntDefiniteFemaleArticleSing
  ) ;
  condGermanArticleForNoun.m_byAttributeValue =
      VocabularyAndTranslation::noun_gender_neuter ;
  //"I trust the child" -> "ich vertraue >>dem<< Kind."
  ConditionsAndTranslation cntDefiniteNeuterArticleSing ;
  cntDefiniteNeuterArticleSing.m_stdstrGermanTranslation = "dem" ;
  cntDefiniteNeuterArticleSing.AddCondition( conditonVerbIsDative ) ;
  cntDefiniteNeuterArticleSing.AddCondition( condGermanArticleForNoun ) ;
  //must create on heap.
  p_trObjArticle = new TranslationRule(
    // For this syntax tree GrammarPart path in syntax tree.
    "3rdPersPluralClauseWith1Obj.obj."
    "*."
    "definite_article_singular.definite_article"
    , mp_parsebyrise ) ;
  AddTranslationRule(
    p_trObjArticle ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    cntDefiniteNeuterArticleSing
  ) ;
}
