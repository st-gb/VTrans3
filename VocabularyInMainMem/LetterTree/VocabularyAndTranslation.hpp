#pragma once

#include <string>
#include "../AutomDelWord.hpp"
  //use a relative path, else "VC/include/IO.h" is used
  #include "../../IO.h" //for WORD_TYPE_NOUN, WORD_TYPE_MAIN_VERB

  #define BIT_POSITION_FOR_TRANSLATION_TYPE 2

  //#define WORD_TYPE_NOUN '1'
  //#define WORD_TYPE_MAIN_VERB '2' //Vollverb
  #define NUMBER_OF_STRINGS_FOR_ENGLISH_NOUN 2 //2 strings: singular and plural
  #define NUMBER_OF_STRINGS_FOR_GERMAN_NOUN 2 //2 strings: singular and plural
  #define ENGLISH_NOUN WORD_TYPE_NOUN
  #define ENGLISH_MAIN_VERB WORD_TYPE_MAIN_VERB
  #define NUMBER_OF_STRINGS_FOR_ENGLISH_MAIN_VERB 3//inf., simple past, past part.
  #define NUMBER_OF_STRINGS_FOR_GERMAN_MAIN_VERB 14//inf.+ 6*present+ 6*past+past part.

class LetterNode ;
class Word ;

  //This class stands for a pair of English and German vocabulary.
  class VocabularyAndTranslation
  {
  public:
    BYTE m_byType ;
    BYTE * m_arbyAttribute ;
#ifdef COMPILE_WITH_REFERENCE_TO_LAST_LETTER_NODE
    //Currently only for English needed because it's only needed 
    //for looking up English words.
    LetterNode ** m_arpletternodeLastEngChar ;
    //LetterNode ** m_arpletternodeLastGerChar ;
#endif //COMPILE_WITH_REFERENCE_TO_LAST_LETTER_NODE
    Word * m_pword ;
    Word m_word ;
    Word * m_pwordTranslation ;
    std::string * m_arstrEnglishWord ;
    std::string * m_arstrGermanWord ;
    //LetterNode * m_arpletternodeBeginOfWord ;
    VocabularyAndTranslation(
      BYTE byType,
      //const
      Word & rword,
      //const
      Word & rwordTranslation)
    {
      m_pword = & rword ;
      m_pwordTranslation = & rwordTranslation ;
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

    //by: only the lowest 2 bit should be used.
    void SetNounTranslationType(BYTE by)
    {
      m_arbyAttribute[0] |= (by >> BIT_POSITION_FOR_TRANSLATION_TYPE) ;
    }

    //for std::set::insert(): there must NOT be: " (this<right)==false && (right<this)==false"
    bool operator <(const VocabularyAndTranslation & right) const
    {
      //return m_pword < right.m_pword ;
      return &m_word < &right.m_word ;
    }
  }; //end class
