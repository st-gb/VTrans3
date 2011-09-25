/*
 * ParseByRise_GrammarRule.cpp
 *
 *  Created on: 30.07.2011
 *      Author: Stefan
 */

#include "ParseByRise.hpp" //class ParseByRise
#include <Attributes/EnglishWord.hpp> //for class EnglishWord's English_word_class enum

// \brief Insert a grammar part <-> grammar part name mapping for word classes.
void ParseByRise::InsertGrammarPartID2NameMappingForWordClasses()
{
  InsertGrammarRule( EnglishWord::noun, "noun" ) ;
  InsertGrammarRule( EnglishWord::singular, "singular_noun" ) ;
  InsertGrammarRule( EnglishWord::plural_noun, "plural_noun" ) ;

//  InsertGrammarRule( EnglishWord::main_verb, "mainVerbInfinitive" ) ;
  InsertGrammarRule( EnglishWord::main_verb_allows_0object_infinitive ,
    "mainVerbInf0Obj" ) ;
  InsertGrammarRule( EnglishWord::main_verb_allows_1object_infinitive ,
    "mainVerbInf1Obj" ) ;
  InsertGrammarRule( EnglishWord::main_verb_allows_2objects_infinitive ,
    "mainVerbInf2Obj" ) ;

  InsertGrammarRule( EnglishWord::mainVerbAllows0object3rdPersonSingularPresent,
    "MainVerb3rdPersSingPres0Obj" ) ;
  InsertGrammarRule( EnglishWord::mainVerbAllows1object3rdPersonSingularPresent,
    "MainVerb3rdPersSingPres1Obj" ) ;
  InsertGrammarRule( EnglishWord::mainVerbAllows2objects3rdPersonSingularPresent,
    "MainVerb3rdPersSingPres2Obj" ) ;

  InsertGrammarRule( EnglishWord::main_verb_allows_0object_past_form,
      "MainVerbPast0Obj");
  InsertGrammarRule( EnglishWord::main_verb_allows_1object_past_form,
      "MainVerbPast1Obj");
  InsertGrammarRule( EnglishWord::main_verb_allows_2objects_past_form,
      "MainVerbPast2Obj");

  InsertGrammarRule( EnglishWord::mainVerbPastParticiple0Obj,
      "mainVerbPastParticiple0Obj");
  InsertGrammarRule( EnglishWord::mainVerbPastParticiple1Obj,
      "mainVerbPastParticiple1Obj");
  InsertGrammarRule( EnglishWord::mainVerbPastParticiple2Obj,
      "mainVerbPastParticiple2Obj");

  InsertGrammarRule( EnglishWord::main_verb_allows_0object_progressive_form,
    "MainVerbProgressive0Obj" ) ;
  InsertGrammarRule( EnglishWord::main_verb_allows_1object_progressive_form,
    "MainVerbProgressive1Obj" ) ;
  InsertGrammarRule( EnglishWord::main_verb_allows_2objects_progressive_form,
    "MainVerbProgressive2Obj" ) ;

  InsertGrammarRule( EnglishWord::adverb, "adverb" ) ;

  InsertGrammarRule( EnglishWord::English_definite_article,
    "definite_article" ) ;
  InsertGrammarRule( EnglishWord::English_indefinite_article,
    "indefinite_article" ) ;

//  InsertGrammarRule( EnglishWord::have, "have");
  InsertGrammarRule( EnglishWord::be1stPersonSingular, "be1stPersonSingular");
//  InsertGrammarRule( EnglishWord::have2ndPersonSingular,
//    "have2ndPersonSingular");
  InsertGrammarRule( EnglishWord::haveInfinitive,"haveInfinitive");

  InsertGrammarRuleNamesForPersonalPronouns();
  InsertGrammarRuleNamesForObjectFormPersonalPronouns();
  InsertGrammarRuleNamesForAuxiliaryVerbToBe();

  InsertGrammarRuleNamesForReflexivePronouns();

  InsertGrammarRule( EnglishWord::relative_pronoun, "relative_pronoun");

  InsertGrammarRule( EnglishWord::conjunction_and ,"and" ) ;
  InsertGrammarRule( EnglishWord::conjunction, "conjunction" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun, "pers_pronoun" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_objective_form ,
    "pers_pron_obj" ) ;

  InsertGrammarRule( EnglishWord::UnknownWord , "UnknownWord" ) ; // he/she/it is
}

void ParseByRise::InsertGrammarRuleNamesForAuxiliaryVerbToBe()
{
  InsertGrammarRule( EnglishWord::am , "am" ) ; //I am
  // you (2nd person singular and plural) are, we are, they are
  InsertGrammarRule( EnglishWord::are , "are" ) ;
  InsertGrammarRule( EnglishWord::is , "is" ) ; // he/she/it is
}

void ParseByRise::InsertGrammarRuleNamesForObjectFormPersonalPronouns()
{
  InsertGrammarRule( EnglishWord::personal_pronoun_me , "me" ) ;
//  InsertGrammarRule( EnglishWord::personal_pronoun_you_sing ,"you_sing" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_objective_you ,"objective_you" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_him , "him") ;
  InsertGrammarRule(EnglishWord::personal_pronoun_her , "her") ;
  InsertGrammarRule(EnglishWord::personal_pronoun_it, "objective_it" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_us, "us" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_them, "them" ) ;
}

void ParseByRise::InsertGrammarRuleNamesForPersonalPronouns()
{
  InsertGrammarRule( EnglishWord::personal_pronoun_I , "I" ) ;
//  InsertGrammarRule( EnglishWord::personal_pronoun_you_sing ,"you_sing" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_you ,"you" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_he , "he") ;
  InsertGrammarRule(EnglishWord::personal_pronoun_she , "she") ;
  InsertGrammarRule(EnglishWord::personal_pronoun_it, "it" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_we, "we" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_you_plur, "you_plur" ) ;
  InsertGrammarRule( EnglishWord::personal_pronoun_they, "they" ) ;
}

void ParseByRise::InsertGrammarRuleNamesForReflexivePronouns()
{
  InsertGrammarRule( EnglishWord::reflexive_pronoun_myself, "myself" ) ;
  InsertGrammarRule( EnglishWord::reflexive_pronoun_yourself, "yourself" ) ;
  InsertGrammarRule( EnglishWord::reflexive_pronoun_himself, "himself" ) ;
  InsertGrammarRule( EnglishWord::reflexive_pronoun_herself, "herself" ) ;
  InsertGrammarRule( EnglishWord::reflexive_pronoun_itself, "itself" ) ;
  InsertGrammarRule( EnglishWord::reflexive_pronoun_ourselves, "ourselves" ) ;
  InsertGrammarRule( EnglishWord::reflexive_pronoun_yourselves, "yourselves" );
  InsertGrammarRule( EnglishWord::reflexive_pronoun_themselves, "themselves") ;
}

// void ParseByRise::InsertArticleGrammarRules()
// {
//  InsertGrammarRule(
//    EnglishWord::English_definite_article,
//    EnglishWord::singular ,
//    "definite_article_singular"
//    ) ;
//  InsertGrammarRule(
//    EnglishWord::English_definite_article,
//    EnglishWord::plural_noun ,
//    "definite_article_plural"
//    ) ;
//  InsertGrammarRule(
//    EnglishWord::English_indefinite_article,
//    EnglishWord::singular ,
//    "indefinite_article_singular"
//    ) ;
//  }
//  InsertSuperClassGrammarRule(
//    "indefinite_article_singular"
//    , "article_singular"
//    ) ;
//  InsertSuperClassGrammarRule(
//    "definite_article_singular"
//    , "article_singular"
//    ) ;
