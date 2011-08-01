#include <bits/stringfwd.h>

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

  void * p_v = Insert("am", EnglishWord:://be1stPersonSingular
      auxiliary_verb//, p_v
      ) ;

  //void * p_v =
  Insert(englishauxiliaryverbBe,germanverbSein, p_v) ;
}

void IVocabularyInMainMem::InsertAuxiliaryVerbHave()
{
  GermanAuxiliaryVerb germanverbHaben ("haben\nhabe\nhast\nhat\nhaben\nhabt\nhaben\n"
    "hatte\nhattest\nhatte\nhatten\nhattet\nhatten\ngehabt\n4"//,
    //GermanVerb::nominative
      );
  EnglishAuxiliaryVerb englishauxiliaryverbHave(
    "have\nhave\nhave\nhas\nhave\nhave\nhave\nhad\nhad\nhad\nhad\nhad\nhad\n"
    "had\n" , false );


//  germanverbHaben.m_bCase = GermanVerb::nominative ;
//  germanverbHaben.m_bMove = 0 ;

  void * p_v = Insert("have", EnglishWord:://have2ndPersonSingular
      //haveInfinitive
      auxiliary_verb
      );//, p_v) ;
  VocabularyAndTranslation * p_vocabularyandtranslation =
      (VocabularyAndTranslation *) p_v;
  p_vocabularyandtranslation->m_byType = EnglishWord::haveInfinitive;
  //void * p_v =
      Insert(englishauxiliaryverbHave,germanverbHaben, p_v) ;
}

//Insert the words that are fundamental and thus needn't/ shouldn't stand in
//the dictionary.
void IVocabularyInMainMem::InsertFundamentalWords()
{
  //Inserts e.g. into a trie structure.
  Insert("the", EnglishWord::English_definite_article ) ;
  Insert("a", EnglishWord::English_indefinite_article ) ;

  Insert( "I" , EnglishWord::personal_pronoun_I ) ;
//  Insert( "you" , EnglishWord::personal_pronoun_you_sing ) ;
  Insert( "you" , EnglishWord::personal_pronoun_you ) ;
  Insert("he", EnglishWord::personal_pronoun_he ) ;
  Insert("she", EnglishWord::personal_pronoun_she) ;
  Insert("it", EnglishWord::personal_pronoun_it) ;
  Insert( "we" , EnglishWord::personal_pronoun_we ) ;
  Insert( "you" , EnglishWord::personal_pronoun_you_plur ) ;
  Insert( "they" , EnglishWord::personal_pronoun_they ) ;

  Insert( "myself" , EnglishWord::reflexive_pronoun_myself ) ;
  Insert( "yourself" , EnglishWord::reflexive_pronoun_yourself ) ;
  Insert("himself", EnglishWord::reflexive_pronoun_himself ) ;
  Insert("herself", EnglishWord::reflexive_pronoun_herself) ;
  Insert("itself", EnglishWord::reflexive_pronoun_itself) ;
  Insert( "ourselves" , EnglishWord::reflexive_pronoun_ourselves ) ;
  Insert( "yourselves" , EnglishWord::reflexive_pronoun_yourselves ) ;
  Insert( "themselves" , EnglishWord::reflexive_pronoun_themselves ) ;

//  Insert( "have" , EnglishWord::have ) ;

  //Because "and" is a special conjunction that changes the "person index" of
  // a subject/ object
  Insert("and", EnglishWord::conjunction_and ) ;

  InsertAuxiliaryVerbBe();

  InsertAuxiliaryVerbHave();

	InsertPersonalPronouns() ;
	//"her","him","me",...
  InsertPersonalPronounsObjectiveForm() ;

//  Insert("a", English_indefinite_article ) ;
//  Insert(",", comma ) ;
}
