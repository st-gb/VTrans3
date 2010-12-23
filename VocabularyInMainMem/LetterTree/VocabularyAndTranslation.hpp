#pragma once

#include <string>
#include "../AutomDelWord.hpp"
  //use a relative path, else "VC/include/IO.h" is used
  #include <IO/IO.hpp> //for WORD_TYPE_NOUN, WORD_TYPE_MAIN_VERB

  #define BIT_POSITION_FOR_TRANSLATION_TYPE 2

  //#define WORD_TYPE_NOUN '1'
  //#define WORD_TYPE_MAIN_VERB '2' //Vollverb
  #define NUMBER_OF_STRINGS_FOR_ENGLISH_ADVERB 1
  #define NUMBER_OF_STRINGS_FOR_GERMAN_ADVERB 1
  #define NUMBER_OF_STRINGS_FOR_ENGLISH_NOUN 2 //2 strings: singular and plural
  #define NUMBER_OF_STRINGS_FOR_GERMAN_NOUN 2 //2 strings: singular and plural
  #define ENGLISH_NOUN WORD_TYPE_NOUN
  #define ENGLISH_MAIN_VERB WORD_TYPE_MAIN_VERB
  #define NUMBER_OF_STRINGS_FOR_ENGLISH_MAIN_VERB 3//inf., simple past, past part.
  //infinitive + 6*present + 6*past + past participle + imperative singualar & 
  //imperative plural = 1 + 6 + 6 + 1 + 2 = 16
  #define NUMBER_OF_STRINGS_FOR_GERMAN_MAIN_VERB 16

class LetterNode ;
//class Word ;
#include <Attributes/Word.hpp>

    static std::string g_ar_stdstrWordClass [] = {
      "noun"
      , "main verb"
      , "adjective"
    } ;

  //This class stands for a pair of English and German vocabulary.
  class VocabularyAndTranslation
  {
  public:
    BYTE m_byType ;
    BYTE * m_arbyAttribute ;
    enum word_class
    {
      noun = 0
      , main_verb //Vollverb
      , adjective
      , adverb
      , preposition
      , pronoun
      , auxiliary_verb
      , conjunction
    } ;
    enum German_main_verb
    {
      infinitive = 0
      , first_person_singular
      , second_person_singular
      , third_person_singular
      , first_person_plural
      , second_person_plural
      , third_person_plural
    } ;
    enum German_main_verb_byte_array_index
    {
      array_index_for_case = 0
    };
    enum German_noun_gender
    {
      noun_gender_male = 0
      , noun_gender_female
      , noun_gender_neuter
    };
    enum German_verb_case
    {
      no_object = 0
      , dative
      , dative_and_accusative
      , accusative
    } ;

#ifdef COMPILE_WITH_REFERENCE_TO_LAST_LETTER_NODE
    //Currently only for English needed because it's only needed 
    //for looking up English words.
    LetterNode ** m_arpletternodeLastEngChar ;
    //LetterNode ** m_arpletternodeLastGerChar ;
#endif //COMPILE_WITH_REFERENCE_TO_LAST_LETTER_NODE

//    Word * m_pword ;
//    Word m_word ;
//    Word * m_pwordTranslation ;
    //std::string * m_arstrEnglishWord ;
    VTrans::string_type * m_arstrEnglishWord ;
    //std::string * m_arstrGermanWord ;
    VTrans::string_type * m_arstrGermanWord ;
    //LetterNode * m_arpletternodeBeginOfWord ;
    VocabularyAndTranslation(
      BYTE byType,
      //const
      Word & rword,
      //const
      Word & rwordTranslation)
    {
//      m_pword = & rword ;
//      m_pwordTranslation = & rwordTranslation ;
    }

    VocabularyAndTranslation(BYTE byVocabularyType) ;

    ~VocabularyAndTranslation() ;

    BYTE GetGermanArticle()
    {
      return m_arbyAttribute[1] ;
    }

    BYTE GetNumberOfNeededObjects()
    {
      return m_arbyAttribute[0] ;
    }

    void GetWord(//Word & word
      AutomDelWord & r_automdelword ) ;

    std::string GetWordTypeAsStdStr()
    {
      return g_ar_stdstrWordClass[ m_byType - WORD_TYPE_NOUN ] ;
    }

    //by: only the lowest 2 bit should be used.
    void SetNounTranslationType(BYTE by)
    {
      m_arbyAttribute[0] |= (by >> BIT_POSITION_FOR_TRANSLATION_TYPE) ;
    }

    //for std::set::insert(): there must NOT be:
    //" (this<right)==false && (right<this)==false"
    bool operator <(const VocabularyAndTranslation & right) const
    {
      //return m_pword < right.m_pword ;
//      return &m_word < &right.m_word ;
      return this < & right ;
    }
  }; //end class
