#pragma once

#include <string>
//#include "AutomDelWord.hpp"
//  //use a relative path, else "VC/include/IO.h" is used
//  #include <IO/IO.hpp>
  //for WORD_TYPE_NOUN, WORD_TYPE_MAIN_VERB
  #include <IO/dictionary/word_class_characters.h>

  #define BIT_POSITION_FOR_TRANSLATION_TYPE 2

  //#define WORD_TYPE_NOUN '1'
  //#define WORD_TYPE_MAIN_VERB '2' //Vollverb
  //Positive, comparative, superlative : e.g. "good", "better", "best"
  #define NUMBER_OF_STRINGS_FOR_ENGLISH_ADJECTIVE 3
  #define NUMBER_OF_STRINGS_FOR_GERMAN_ADJECTIVE 3
  #define NUMBER_OF_STRINGS_FOR_ENGLISH_ADVERB 1
  #define NUMBER_OF_STRINGS_FOR_GERMAN_ADVERB 1
  #define NUMBER_OF_STRINGS_FOR_ENGLISH_NOUN 2 //2 strings: singular and plural
  #define NUMBER_OF_STRINGS_FOR_GERMAN_NOUN 2 //2 strings: singular and plural
  #define ENGLISH_NOUN WORD_TYPE_NOUN
  #define ENGLISH_MAIN_VERB WORD_TYPE_MAIN_VERB
  //infinitive, simple past, past participle, 3rd person present
  #define NUMBER_OF_STRINGS_FOR_ENGLISH_MAIN_VERB 4
  //infinitive + 6*present + 6*past + past participle + imperative singualar & 
  //imperative plural = 1 + 6 + 6 + 1 + 2 = 16
  #define NUMBER_OF_STRINGS_FOR_GERMAN_MAIN_VERB 16


//forward decl.
class LetterNode ;
class I_UserInterface;

//class Word ;
#include <Attributes/Word.hpp>

    static std::string g_ar_stdstrWordClass [] = {
      "noun"
      , "main verb"
      , "adjective"
    } ;


//  BYTE g_arbyWordType2ArraySizeIndex [100];

  //This class stands for a pair of English and German vocabulary.
  class VocabularyAndTranslation
  {
  public:
    //For both English and German.
    enum nounArrayIndices{ SingularArrayIndex = 0, PluralArrayIndex };
    struct ArraySizes
    {
      unsigned char m_byArraySizeForEnglishWord;
      unsigned char m_byArraySizeForGermanWord;
      unsigned char m_byArraySizeForByteArray;
    };
    static const ArraySizes s_arraysizes [];

    static I_UserInterface * s_p_userinterface;
//    struct ArraySizes
//    {
//      unsigned m_uiArraySizeForEnglishWord;
//      unsigned m_uiArraySizeForGermanWord;
//      unsigned m_uiArraySizeForByteArray;
//    };
//    static const ArraySizes s_arraysizes [] = {
//      //noun
//      {2,2,2}
//    };
//    //Count how often this object is referred to from other LetterNodes.
//    unsigned m_referenceCount;
    BYTE m_byType ;
    BYTE * m_arbyAttribute ;
//    enum array_sizes
//    {
//
//    };
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
    enum German_main_verb_indices
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
      array_index_for_case = 0 ,
      array_index_for_reflexive
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

    static void Init()
    {
//      g_arbyWordType2ArraySizeIndex[ (int) (ENGLISH_NOUN) ] = 0;
    }

    void GetAttributeValue(BYTE byIndex);

    BYTE GetGermanArticle()
    {
      return m_arbyAttribute[1] ;
    }

    std::string GetEnglishString(BYTE byIndex) const;
    std::string GetGermanString(BYTE byIndex) const;

    BYTE GetNumberOfNeededObjects()
    {
      return m_arbyAttribute[0] ;
    }

//    void GetWord(//Word & word
//      AutomDelWord & r_automdelword ) ;

    std::string GetWordTypeAsStdStr()
    {
      return g_ar_stdstrWordClass[ m_byType - WORD_TYPE_NOUN ] ;
    }

    void PointToAttributeData(
      const VocabularyAndTranslation * const p_vocabularyandtranslationPointTo)
    {
      m_arstrEnglishWord =
          p_vocabularyandtranslationPointTo->m_arstrEnglishWord ;
      m_arstrGermanWord=
          p_vocabularyandtranslationPointTo->m_arstrGermanWord ;
      m_arbyAttribute=
          p_vocabularyandtranslationPointTo->m_arbyAttribute ;
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
