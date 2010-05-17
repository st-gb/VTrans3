//Ohne das folgende include/wenn Pfad in den Eigenschaften dieser Datei 
//unter C/C++->vorkompilierte Header->"PHC durch..." falsch ist: 
//"fatal error C1010: Unerwartetes Dateiende w�hrend der Suche nach dem vorkompilierten Header.[...]"
#include "../../StdAfx.h"
#include "../../Word.hpp"
#include "VocabularyAndTranslation.hpp"
#include <windef.h> //for BYTE
#include <typeinfo> //for typeid()

VocabularyAndTranslation::VocabularyAndTranslation(BYTE byVocabularyType)
{
  m_byType = byVocabularyType ;
  BYTE byArraySizeForEng = 0 ;
  //BYTE byArraySizeForGer = 0 ;
  switch(byVocabularyType)
  {
  case ENGLISH_NOUN:
//    m_pword = new EnglishNoun() ;
    m_arstrEnglishWord = new std::string[NUMBER_OF_STRINGS_FOR_ENGLISH_NOUN] ;
    m_arstrGermanWord = new std::string[NUMBER_OF_STRINGS_FOR_GERMAN_NOUN] ;
    byArraySizeForEng = NUMBER_OF_STRINGS_FOR_ENGLISH_NOUN ;
    //byArraySizeForGer = NUMBER_OF_STRINGS_FOR_GERMAN_NOUN ;
    m_arbyAttribute = new BYTE[2] ;
    memset(m_arbyAttribute,0,2) ;
    break;
  case ENGLISH_MAIN_VERB:
    m_arstrEnglishWord = new std::string[NUMBER_OF_STRINGS_FOR_ENGLISH_MAIN_VERB] ;
    m_arstrGermanWord = new std::string[NUMBER_OF_STRINGS_FOR_GERMAN_MAIN_VERB] ;
    m_arbyAttribute = new BYTE[2] ;
    break;
  case WORD_TYPE_CONJUNCTION:
    m_arstrEnglishWord = new std::string[1] ;
    m_arstrGermanWord = new std::string[1] ;
    m_arbyAttribute = new BYTE[1] ;
    break;
  //case LetterTree::personal_pronoun :
  case EnglishWord::personal_pronoun :
    m_arstrEnglishWord = new std::string[1] ;
    m_arstrGermanWord = new std::string[1] ;
    m_arbyAttribute = new BYTE[1] ;
    break;
    //Only the singular (for parsing "indefinite article" + "singular"
    // ( if the rule was "indefinite article" + "noun",
    //  "indefinite article" + "plural" which is wrong would also be possible)
    // singular type is only needed for parsing. It shares the same attr as
    // the noun.
//  case EnglishWord::singular :
//    m_arstrEnglishWord = new std::string[1] ;
//    m_arstrGermanWord = new std::string[1] ;
//    m_arbyAttribute = new BYTE[1] ;
//    break;

  //case GERMAN_NOUN:
  //  m_pword = new GermanNoun() ;
  //  m_arstrGermanWord = new std::string[NUMBER_OF_STRINGS_FOR_GERMAN_NOUN] ;
  //  break;
  default:
//    m_pword = new Word() ;
    m_arstrEnglishWord = NULL ;
    m_arstrGermanWord = NULL ;
    m_arbyAttribute = NULL ;
  }
#ifdef COMPILE_WITH_REFERENCE_TO_LAST_LETTER_NODE
  m_arpletternodeLastEngChar = new LetterNode * [byArraySizeForEng];
#endif// #ifdef COMPILE_WITH_REFERENCE_TO_LAST_LETTER_NODE
  //m_arpletternodeLastGerChar = new LetterNode * [byArraySizeForGer];
  //Important for the destructor: init. with "NULL".
  //m_pword = NULL ;
//  m_pwordTranslation = NULL ;
}

VocabularyAndTranslation::~VocabularyAndTranslation()
{
  //if ( m_pword )
  //  delete m_pword ;
//  if( m_pwordTranslation )
//    delete m_pwordTranslation ;

  // singular type is only needed for parsing. It shares the same attr as
  // the noun. Because for the noun the storage is freed it should NOT be done again
  // for the singular.
  if( m_byType != EnglishWord::singular &&
      m_byType != EnglishWord::third_person_singular_present
      )
  {
    if(m_arstrEnglishWord)
    {
  #ifdef _DEBUG_FREEING_MEM
      TRACE("~VocabularyAndTranslation()--%x ",this);
      BYTE byNumWords = g_arbyNumberOfEnglishWords[ m_byType - ENGLISH_NOUN ] ;
      for(int i = 0 ; i < byNumWords ; i++ )
        TRACE("\"%s\"", m_arstrEnglishWord[i].c_str() ) ;
      TRACE("\n") ;
  #endif
      delete [] m_arstrEnglishWord ;
    }
    if(m_arstrGermanWord)
      delete [] m_arstrGermanWord ;
    if(m_arbyAttribute)
      delete [] m_arbyAttribute ;
  #ifdef COMPILE_WITH_REFERENCE_TO_LAST_LETTER_NODE
    delete [] m_arpletternodeLastEngChar ;
  #endif //#ifdef COMPILE_WITH_REFERENCE_TO_LAST_LETTER_NODE
  //delete m_arpletternodeLastGerChar ;
  }
}

//Word 
void VocabularyAndTranslation::GetWord(//Word & word
  AutomDelWord & r_automdelword )
{
  //Word 
  switch( m_byType )
  {
  case ENGLISH_NOUN:
    //m_pword = new EnglishNoun() ;
    EnglishNoun en ;
    Word word ;
    //The object needs to be dynamically created (=on the heap), else a 
    //downcast (cast to a superclass) with correct typeid is not possible.
    EnglishNoun * p_en = new EnglishNoun();
    en.m_strSingular = m_arstrEnglishWord[0].c_str() ;
    //m_arstrGermanWord = new std::string[NUMBER_OF_STRINGS_FOR_GERMAN_NOUN] ;
    //byArraySizeForEng = NUMBER_OF_STRINGS_FOR_ENGLISH_NOUN ;
    ////byArraySizeForGer = NUMBER_OF_STRINGS_FOR_GERMAN_NOUN ;
    //m_arbyAttribute = new BYTE[2] ;
    //word = dynamic_cast<Word &>(en) ;
    //word = en ;
    p_en->m_bType = ENGLISH_NOUN ;
    r_automdelword.SetWord(p_en) ;
#ifdef _DEBUG
    if( typeid(//*pWordNode->m_pWord
      //word 
      //* r_automdelword.GetWord() 
      * p_en 
      ) == typeid(EnglishNoun)
      )
    {
      int i = 0 ;
    }
#endif //#ifdef _DEBUG
  }
  //return 
}
