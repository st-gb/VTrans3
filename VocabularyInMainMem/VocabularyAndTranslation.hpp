#pragma once
#ifndef VOCABULARY_AND_TRANSLATION_HPP
#define VOCABULARY_AND_TRANSLATION_HPP

#include <string>
#include <string.h> //memcpy(...), strlen(...)
//#include "AutomDelWord.hpp"
//  //use a relative path, else "VC/include/IO.h" is used
//  #include <IO/IO.hpp>
  //for WORD_TYPE_NOUN, WORD_TYPE_MAIN_VERB
  #include <IO/dictionary/word_class_characters.h>
#include <Attributes/EnglishWord.hpp> //EnglishWord::English_word_class
#include <fastest_data_type.h> //fastestUnsignedDataType

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
  /** infinitive, simple past, past participle, 3rd person present */
  #define NUMBER_OF_STRINGS_FOR_ENGLISH_MAIN_VERB 4
  //infinitive + 6*present + 6*past + past participle + imperative singualar & 
  //imperative plural = 1 + 6 + 6 + 1 + 2 = 16
  #define NUMBER_OF_STRINGS_FOR_GERMAN_MAIN_VERB 16


/** forward decl. */
class LetterNode ;
class I_UserInterface;
class TranslationControllerBase;

//class Word ;
#include <Attributes/Word.hpp>

//  /*static*/ /*std::string*/ char * g_ar_stdstrWordClass [];// = {
//    "noun"
//    , "main verb"
//    , "adjective"
//  } ;


//  BYTE g_arbyWordType2ArraySizeIndex [100];

  /** @brief This class stands for a pair of English and German vocabulary.
   *   If multiple words have the same words (e.g. the "love", to "love")
   *   then each one has its VocabularyAndTranslation object. */
  class VocabularyAndTranslation
  {
  public:
    static /*std::string*/ const char * s_ar_chWordClass [];// = {
    typedef char word_type_for_new_allocator;
    typedef char * word_type;
    //For both English and German.
    enum nounArrayIndices{ SingularArrayIndex = 0, PluralArrayIndex };
    struct ArraySizes
    {
      typedef fastestUnsignedDataType data_type;
      ArraySizes(
        const data_type arraySizeForEnglishWord,
        const data_type arraySizeForGermanWord,
        const data_type arraySizeForByteArray
        )
        : m_byArraySizeForEnglishWord(arraySizeForEnglishWord),
          m_byArraySizeForGermanWord(arraySizeForGermanWord),
          m_byArraySizeForByteArray(arraySizeForByteArray)
      { }
      ArraySizes()
        : m_byArraySizeForEnglishWord(0),
          m_byArraySizeForGermanWord(0),
          m_byArraySizeForByteArray(0)
      { }
      data_type m_byArraySizeForEnglishWord;
      data_type m_byArraySizeForGermanWord;
      data_type m_byArraySizeForByteArray;
    };
    static TranslationControllerBase * s_p_translationControllerBase;
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
    /*BYTE*/ EnglishWord::English_word_class m_englishWordClass ;
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
    /*VTrans::string_type * */ word_type * m_arstrEnglishWord ;
    //std::string * m_arstrGermanWord ;
    /*VTrans::string_type * */ word_type * m_arstrGermanWord ;
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

    VocabularyAndTranslation(/*BYTE*/ EnglishWord::English_word_class
      byVocabularyType) ;

    ~VocabularyAndTranslation() ;

    fastestUnsignedDataType GetNumberOfBytes();
    void FreeMemory();
//    void GetNumberOfArrayElements(
//      EnglishWord::English_word_class engWordClass,
//      ArraySizes & arr_sz
//      );
    std::string GetWordClassAsString() const;
    static fastestUnsignedDataType GetNumberOfArrayElements(
      /*const*/ EnglishWord::English_word_class engWordClass,
      fastestUnsignedDataType & numEngWords,
      fastestUnsignedDataType & numGerWords
      );
    static void GetNumberOfArrayElements(
      const EnglishWord::English_word_class engWordClass,
      ArraySizes & arrSizes)
    {
      arrSizes.m_byArraySizeForByteArray = GetNumberOfArrayElements(
        engWordClass,
        arrSizes.m_byArraySizeForEnglishWord,
        arrSizes.m_byArraySizeForGermanWord);
    }
    void PossiblyGenerateAndAddGermanAttributes(
      const EnglishWord::English_word_class engWordClass,
      const std::string &
      );

    void GetNumberOfArrayElements(
      ArraySizes & arrSizes) const
    {
      EnglishWord::English_word_class word_class = EnglishWord::
        MapGrammarPartIDtoWordClass(m_englishWordClass);
      if( /*m_englishWordClass*/ word_class <= EnglishWord::adjective )
      {
        arrSizes.m_byArraySizeForByteArray = GetNumberOfArrayElements(
          m_englishWordClass,
          arrSizes.m_byArraySizeForEnglishWord,
          arrSizes.m_byArraySizeForGermanWord);
      }
    }

    /**  Lexik o   n
     *         111 110 <-dez
     * - Lexik a
     *         97  0   <-dez
     * = 00000 14  110
     *
     *   huff table (sorted by frequency):
     *   0 -> 1 (implicitely) (not saved/ stored/ written)
     *   14 (1byte)->"01"
     *   110 (1byte)->"001"
     *   0 <- marks the end of the table
     *
     *   ---14---- --110---- ---0---        14 -110
     *   1110 0000 0110 1110 0000000 1111 1 01 0 01  <- bitwise = 5 byte
     *
     *   decode:
     *   ---14---- --110---- ---0---        14 -110
     *   1110 0000 0110 1110 0000000 1111 1 01 0 01  <- bitwise = 5 byte
     *
     *   ->code table
     *   14->01
     *   110->001
     *   0->no more codes
     *   1->0
     *
     *   */
//    BYTE * diff(const char * const word, const fastestUnsignedDataType stringLen)
//    {
//      BYTE * ar_by = new BYTE[stringLen];
//      const word_type firstGermanWord = m_arstrGermanWord[0];
//      BYTE byCurrentValue;
//      char firstWordCurrentChar;
//      if( firstGermanWord[0] != '\0' )
//      {
//        for(fastestUnsignedDataType i = 0; i < stringLen; ++i)
//        {
////          byCurrentValue
//          firstWordCurrentChar = firstGermanWord[i];
//          if( firstWordCurrentChar != '\0' )
//          {
//            switch(firstWordCurrentChar)
//            {
//              case 'a': //Mann - Männer = 0100 255-e,255-r
//                if( word[i] == 'ä' )
//                  ar_by[i] = 1;
//                break;
//              case 'u': //Bruder - Brüder = 00100
//               if( word[i] == 'ü' )
//                 ar_by[i] = 1;
//               break;
//            }
//            ar_by[i] = firstGermanWord[i] - word[i];
//          }
//        }
//      }
//    }
//    BYTE * compress(const BYTE * const ar_by, const fastestUnsignedDataType len)
//    {
//      unsigned char char_frequencies[256];
//      memset(char_frequencies, 0, 256);
//      BYTE byCurrentValue;
//      for(fastestUnsignedDataType i = 0; i < len; i++)
//      {
//        byCurrentValue = ar_by[0];
//        char_frequencies[byCurrentValue]++;
//      }
//    }

    void SetAttributeValue(
        const fastestUnsignedDataType index,
        const fastestUnsignedDataType value)
    {
      ArraySizes arrsz;
      GetNumberOfArrayElements(arrsz);
      if( arrsz.m_byArraySizeForByteArray > 0 )
      {
        m_arbyAttribute[index] = value;
      }
    }

    void SetGermanWord(
      const char * const word,
      const fastestUnsignedDataType stringLen,
      fastestUnsignedDataType vocAndTranslArrayIndex)
    {
      m_arstrGermanWord[vocAndTranslArrayIndex] = new
        word_type_for_new_allocator [stringLen
         // string terminating 0 char.
          + 1];
      memcpy( (void *) m_arstrGermanWord[vocAndTranslArrayIndex],
        word,
        //+ string terminating 0 char.
        stringLen //+ 1
        );
      //set terminating 0 char.
      m_arstrGermanWord[vocAndTranslArrayIndex][stringLen] = '\0';
      //TODO?
//      if( m_arstrGermanWord[vocAndTranslArrayIndex][0] != '\0' )
//      {
//        m_arstrGermanWord[vocAndTranslArrayIndex][0] = diff()
//      }
    }

    static void Init()
    {
//      g_arbyWordType2ArraySizeIndex[ (int) (ENGLISH_NOUN) ] = 0;
    }

    void GetAttributeValue(BYTE byIndex);

    BYTE GetGermanArticle()
    {
      return m_arbyAttribute[1] ;
    }

    std::string GetEnglishWordAsStdString(const fastestUnsignedDataType byIndex) const;
    int GetEnglishWordLength(const fastestUnsignedDataType wordIndex) const
    {
      return ::strlen(m_arstrEnglishWord[wordIndex]);
    }

    word_type GetAssembledGermanString(const fastestUnsignedDataType index) const;
    word_type GetGermanString(const fastestUnsignedDataType index) const;
    std::string GetGermanWordAsStdString(const fastestUnsignedDataType index) const
    {
      std::string std_str(GetGermanString(index));
      return std_str;
    }

    BYTE GetNumberOfNeededObjects()
    {
      return m_arbyAttribute[0] ;
    }

//    void GetWord(//Word & word
//      AutomDelWord & r_automdelword ) ;

    std::string GetWordTypeAsStdStr()
    {
      return //g_ar_stdstrWordClass[ m_englishWordClass - WORD_TYPE_NOUN ] ;
        VocabularyAndTranslation::s_ar_chWordClass[ m_englishWordClass - WORD_TYPE_NOUN ];
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

    /** for std::set::insert(): there must NOT be:
    * " (this<right)==false && (right<this)==false" */
    bool operator <(const VocabularyAndTranslation & right) const
    {
      //return m_pword < right.m_pword ;
//      return &m_word < &right.m_word ;
      return this < & right ;
    }

    friend std::ostream & operator << (std::ostream & os, const VocabularyAndTranslation & obj);
  }; //end class

#endif //VOCABULARY_AND_TRANSLATION_HPP
