/*
 * AddPersonalPronounTranslationRules.cpp
 *
 *  Created on: Dec 19, 2010
 *      Author: Stefan
 */

void //TranslateParseByRiseTree::
  AddPersonalPronounTranslationRules()
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
  cntPersPronoun.m_stdstrGermanTranslation = "ich" ;
  //cnt3.m_byPersonIndex = THIRD_PERSON_PLURAL ;
  //must create on heap.
  TranslationRule * p_trPersPronoun = new TranslationRule(
    "I" // For this syntax tree GrammarPart path in syntax tree.
    , mp_parsebyrise ) ;
  AddTranslationRule(
    //"def_article_noun.definite_article" ,
    p_trPersPronoun ,
    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
    //cond ,
    //, ""
    cntPersPronoun
  ) ;
//  cntPersPronoun.m_stdstrGermanTranslation = "du" ;
//  //cnt3.m_byPersonIndex = THIRD_PERSON_PLURAL ;
//  //must create on heap.
//  p_trPersPronoun = new TranslationRule(
//    "you" // For this syntax tree GrammarPart path in syntax tree.
//    , mp_parsebyrise ) ;
//  AddTranslationRule(
//    //"def_article_noun.definite_article" ,
//    p_trPersPronoun ,
//    //"def_article_noun.noun.English.isSingular=1" //English attribute (condition)
//    //cond ,
//    //, ""
//    cntPersPronoun
//  ) ;
  cntPersPronoun.m_stdstrGermanTranslation = "er" ;
  //cnt3.m_byPersonIndex = THIRD_PERSON_PLURAL ;
  //must create on heap.
  p_trPersPronoun = new TranslationRule(
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
