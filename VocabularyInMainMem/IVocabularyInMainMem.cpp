#include <bits/stringfwd.h>

#include "IVocabularyInMainMem.hpp"
#include "Word.hpp"

//IVocabularyInMainMem::s_arstdstrPersonalPronoun = {"hh", "j"} ;
//
//void IVocabularyInMainMem::InsertPersonalPronouns()
//{
//  Insert("I", personal_pronoun ) ;
//  Insert("you", personal_pronoun ) ;
//  Insert("a", personal_pronoun ) ;
//  Insert("a", English_indefinite_article ) ;
//  Insert("a", English_indefinite_article ) ;
//  Insert("a", English_indefinite_article ) ;
//  Insert("a", English_indefinite_article ) ;
//  Insert("a", English_indefinite_article ) ;
//
//}

IVocabularyInMainMem::IVocabularyInMainMem()
{
  //m_arstdstrPersonalPronoun = new std::string[NUMBER_OF_PERSONAL_PRONOUNS] ;
}

//IVocabularyInMainMem::~IVocabularyInMainMem()
//{
//  //delete [] m_arstdstrPersonalPronoun ;
//}

//Insert the words that are fundamental and thus needn't/ shouldn't stand in
//the dictionary.
void IVocabularyInMainMem::InsertFundamentalWords()
{
  //Inserts e.g. into a trie structure.
  Insert("the", English_definite_article ) ;
  Insert("a", English_indefinite_article ) ;
  GermanVerb germanverbSein ("sein\nbin\nbist\nist\nsind\nseid\nsind\nwar\n"
    "warst\nwar\nwaren\nward\nwaren\ngewesen\n", GermanVerb::nominative );
  EnglishAuxiliaryVerb englishauxiliaryverbBe(
		"be\nam\nare\nis\nare\nare\nare\nwas\nwere\n"
		"was\nwere\nwere\nwere\nbeen\n" , false );
  Insert(englishauxiliaryverbBe,germanverbSein) ;
	germanverbSein.m_bCase = GermanVerb::nominative ;
	germanverbSein.m_bMove = 0 ;

  Insert("a", English_indefinite_article ) ;
//  Insert(",", comma ) ;
//  InsertPersonalPronouns() ;
}