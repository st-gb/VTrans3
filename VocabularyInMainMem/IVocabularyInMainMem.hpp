#pragma once //incl. guard

#include <string>
#include <windef.h> //for BYTE etc.
#include <Word.hpp>

#include "LetterTree/VocabularyAndTranslation.hpp" //class EnglishWord

#define NUMBER_OF_PERSONAL_PRONOUNS 7

  class IVocabularyInMainMem
  {
  public:
    static std::string s_arstdstrPersonalPronoun [] ; //= {"hh", "j"} ;
  //Because this is a translation for English->German the dictionary resp.
  //the order of the words is primarily important for English words / word
  //attributes.
  //For the
  enum English_word_class
  {
    noun = //0
      ENGLISH_NOUN
    , main_verb //Vollverb
    , adjective
    , adverb
    , preposition
    , pronoun
    , auxiliary_verb
    , conjunction
    , English_definite_article
    , English_indefinite_article
  } ;
    //Make methods pure virtual ("virtual [...] = 0" ) to avoid
    //linker error "LNK 2001" in MSVC++.
//    virtual BYTE GetGermanArticle() = 0 ;
//    virtual std::string GetGermanPlural() = 0 ;
//    virtual WORD GetNumberOfNouns() = 0 ;
//    virtual BYTE IsSingular() = 0 ;
//    virtual BYTE GetTranslationType() = 0 ;
//    virtual std::string GetGermanSingular() = 0 ;
    IVocabularyInMainMem() ;
//    virtual void NextNoun() = 0 ;
    void InsertFundamentalWords() ;
    //For inserting fundamental words etc.
    virtual void Insert(std::string stdstr, BYTE byWordClass )
      //If not "= 0 ":
      //"undefined reference to `vtable for IVocabularyInMainMem'" when linking
      //with g++ .
      = 0 ;
    virtual void Insert(EnglishWord & ew , GermanWord & gw ) = 0 ;
  };
