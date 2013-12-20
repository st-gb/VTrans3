#ifndef __ANDROID__
//TODO does this include make sense?:
  #include <bits/stringfwd.h>
#endif

#include "IVocabularyInMainMem.hpp"
#include <Attributes/Word.hpp>

//IVocabularyInMainMem::s_arstdstrPersonalPronoun = {"hh", "j"} ;
//
//void IVocabularyInMainMem::InsertPersonalPronouns()
//{
//  Insert("I", personal_pronoun ) ;
//  Insert("you", personal_pronoun ) ;
//  Insert("he", personal_pronoun ) ;
//  Insert("she", personal_pronoun ) ;
//  Insert("it", personal_pronoun ) ;
//  Insert("we", personal_pronoun ) ;
//  Insert("you", personal_pronoun ) ;
//  Insert("they", personal_pronoun ) ;
//}

IVocabularyInMainMem::IVocabularyInMainMem()
  : m_numberOfVocPairs(0)
  , m_numberOfEnglishWords(0)
  , m_maxTokenToConsider(4) //"hand-held vacuum cleaner"
{
  //m_arstdstrPersonalPronoun = new std::string[NUMBER_OF_PERSONAL_PRONOUNS] ;
}

//IVocabularyInMainMem::~IVocabularyInMainMem()
//{
//  //delete [] m_arstdstrPersonalPronoun ;
//}

void IVocabularyInMainMem::InsertAuxiliaryVerbBe()
{
  GermanAuxiliaryVerb germanverbSein (
    "sein\nbin\nbist\nist\nsind\nseid\nsind\nwar\n"
    "warst\nwar\nwaren\nward\nwaren\ngewesen\n"//, GermanVerb::nominative
      );
  EnglishAuxiliaryVerb englishauxiliaryverbBe(
    "be\nam\nare\nis\nare\nare\nare\nwas\nwere\n"
    "was\nwere\nwere\nwere\nbeen\n" , false );

//  germanverbSein.m_bCase = GermanVerb::nominative ;
//  germanverbSein.m_bMove = 0 ;

//  void * p_v = NULL;
//  IVocabularyInMainMem::voc_container_type * p_voc_container = NULL;
  VocabularyAndTranslation * p_vocabularyandtranslation = NULL;
  /*VocabularyAndTranslation * p_vocabularyandtranslation = */Insert("am",
    EnglishWord:://be1stPersonSingular
//      auxiliary_verb
    am
    , /*p_v p_voc_container*/  p_vocabularyandtranslation
    ) ;
  //p_v
  /*p_vocabularyandtranslation = */Insert("are", EnglishWord:://be1stPersonSingular
//      auxiliary_verb
    are
    , /*p_v p_voc_container */ p_vocabularyandtranslation
    ) ;
  //p_v
  /*p_vocabularyandtranslation = */Insert("is", EnglishWord:://be1stPersonSingular
//      auxiliary_verb
    is
    , /*p_v p_voc_container */ p_vocabularyandtranslation
    ) ;

  //void * p_v =
//  Insert(englishauxiliaryverbBe,germanverbSein, p_v) ;
}

void IVocabularyInMainMem::InsertAuxiliaryVerbHave()
{
  GermanAuxiliaryVerb germanverbHaben ("haben\nhabe\nhast\nhat\nhaben\nhabt\nhaben\n"
    "hatte\nhattest\nhatte\nhatten\nhattet\nhatten\ngehabt\n"//,
    //GermanVerb::nominative
      );
  EnglishAuxiliaryVerb englishauxiliaryverbHave(
    "have\nhave\nhave\nhas\nhave\nhave\nhave\nhad\nhad\nhad\nhad\nhad\nhad\n"
    "had\n" , false );


//  germanverbHaben.m_bCase = GermanVerb::nominative ;
//  germanverbHaben.m_bMove = 0 ;

  VocabularyAndTranslation * p_vocabularyandtranslation = NULL;
  //void * p_v
  /*VocabularyAndTranslation * p_vocabularyandtranslation = */Insert("have",
      EnglishWord:://haveInfinitive
      auxiliary_verb, p_vocabularyandtranslation
      );//, p_v) ;
//  VocabularyAndTranslation * p_vocabularyandtranslation =
//      (VocabularyAndTranslation *) p_v;
//  p_vocabularyandtranslation->m_byType = EnglishWord::haveInfinitive;
//  //void * p_v =
//  //TODO
//    Insert(englishauxiliaryverbHave, germanverbHaben, //p_v
//      p_vocabularyandtranslation) ;
}

void IVocabularyInMainMem::InsertAuxiliaryVerbWill()
{
  GermanAuxiliaryVerb germanverbWerden ("werden\nwerde\nwirst\nwird\nwerden\nwerdet\nwerden\n"
    "wurde\nwurdest\nwurde\nwurden\nwurdet\nwurden\ngeworden\n"//,
    //GermanVerb::nominative
      );
  EnglishAuxiliaryVerb englishauxiliaryverbWill(
    "will\nwill\nwill\nwill\nwill\nwill\nwould\nwould\nwould\nwould\nwould\nwould\nwould\n"
    "would\n" , false );


//  germanverbHaben.m_bCase = GermanVerb::nominative ;
//  germanverbHaben.m_bMove = 0 ;

  VocabularyAndTranslation * p_vocabularyandtranslation = NULL;
  //void * p_v
  /*VocabularyAndTranslation * p_vocabularyandtranslation = */Insert("will",
    EnglishWord:://haveInfinitive
    auxiliary_verb, p_vocabularyandtranslation
    );//, p_v) ;
//  VocabularyAndTranslation * p_vocabularyandtranslation =
//      (VocabularyAndTranslation *) p_v;
  //p_vocabularyandtranslation->m_byType = EnglishWord::will;

  //void * p_v =
  //TODO
//    Insert(englishauxiliaryverbWill, germanverbWerden, //p_v
//      p_vocabularyandtranslation) ;
}

//Insert the words that are fundamental and thus needn't/ shouldn't stand in
//the dictionary.
void IVocabularyInMainMem::InsertFundamentalWords()
{
//  IVocabularyInMainMem::voc_container_type * p_voc_container = NULL;
  VocabularyAndTranslation * p_vocabularyandtranslation = NULL;
  Insert("unknownWord", EnglishWord::UnknownWord, /*NULL*/ p_vocabularyandtranslation);
  //Inserts e.g. into a trie structure.
  Insert("the", EnglishWord::English_definite_article, p_vocabularyandtranslation ) ;
  Insert("a", EnglishWord::English_indefinite_article, p_vocabularyandtranslation) ;

  Insert( "I" , EnglishWord::personal_pronoun_I, p_vocabularyandtranslation) ;
//  Insert( "you" , EnglishWord::personal_pronoun_you_sing ) ;
  Insert( "you" , EnglishWord::personal_pronoun_you, p_vocabularyandtranslation) ;
  Insert("he", EnglishWord::personal_pronoun_he, p_vocabularyandtranslation) ;
  Insert("she", EnglishWord::personal_pronoun_she, p_vocabularyandtranslation) ;
  Insert("it", EnglishWord::personal_pronoun_it, p_vocabularyandtranslation) ;
  Insert( "we" , EnglishWord::personal_pronoun_we, p_vocabularyandtranslation) ;
  Insert( "you" , EnglishWord::personal_pronoun_you_plur, p_vocabularyandtranslation) ;
  Insert( "they" , EnglishWord::personal_pronoun_they, p_vocabularyandtranslation) ;

  Insert( "myself" , EnglishWord::reflexive_pronoun_myself, p_vocabularyandtranslation) ;
  Insert( "yourself" , EnglishWord::reflexive_pronoun_yourself, p_vocabularyandtranslation) ;
  Insert("himself", EnglishWord::reflexive_pronoun_himself, p_vocabularyandtranslation) ;
  Insert("herself", EnglishWord::reflexive_pronoun_herself, p_vocabularyandtranslation) ;
  Insert("itself", EnglishWord::reflexive_pronoun_itself, p_vocabularyandtranslation) ;
  Insert( "ourselves" , EnglishWord::reflexive_pronoun_ourselves, p_vocabularyandtranslation) ;
  Insert( "yourselves" , EnglishWord::reflexive_pronoun_yourselves, p_vocabularyandtranslation) ;
  Insert( "themselves" , EnglishWord::reflexive_pronoun_themselves, p_vocabularyandtranslation) ;

//  Insert( "have" , EnglishWord::have ) ;

  //Because "and" is a special conjunction that changes the "person index" of
  // a subject/ object
  Insert("and", EnglishWord::conjunction_and, p_vocabularyandtranslation) ;

  InsertAuxiliaryVerbBe();

  InsertAuxiliaryVerbHave();
  InsertAuxiliaryVerbWill();

	InsertPersonalPronouns() ;
	//"her","him","me",...
  InsertPersonalPronounsObjectiveForm() ;

//  Insert("a", English_indefinite_article ) ;
//  Insert(",", comma ) ;
}
