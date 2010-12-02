//Ohne das folgende include/wenn Pfad in den Eigenschaften dieser Datei 
//unter C/C++->vorkompilierte Header->"PHC durch..." falsch ist: 
//"fatal error C1010: Unerwartetes Dateiende während der Suche nach dem
// vorkompilierten Header.[...]"
#include "../../StdAfx.h"
#include <Attributes/Word.hpp> //class Word, EnglishWord, EnglishNoun
//header file for this VocabularyAndTranslation class
#include "VocabularyAndTranslation.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h> //DEBUG_COUTN(...)
#include <windef.h> //for BYTE
#include <typeinfo> //for typeid()
//SUPPRESS_UNUSED_VARIABLE_WARNING(...)
#include <preprocessor_macros/suppress_unused_variable.h>

#define SET_FREED_MEM_TO_NULL

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
  case EnglishWord::main_verb_allows_0object_infinitive:
  case EnglishWord::main_verb_allows_1object_infinitive:
  case EnglishWord::main_verb_allows_2objects_infinitive:
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
  case EnglishWord::personal_pronoun_objective_form :
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
    //For vocabulary types that just refer another vocandtransl object's
    // attributes and for types that do not need (e.g. "definite article")
    //these attributes etc.
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

//#define _DEBUG_FREEING_MEM

VocabularyAndTranslation::~VocabularyAndTranslation()
{
  //if ( m_pword )
  //  delete m_pword ;
//  if( m_pwordTranslation )
//    delete m_pwordTranslation ;

  switch( m_byType )
  {
  // singular type is only needed for parsing. It shares the same attr as
  // the noun. Because for the noun the storage is freed it should NOT be done again
  // for the singular.
  case EnglishWord::singular :
  case EnglishWord::plural_noun :
  case EnglishWord::mainVerbAllows0object3rdPersonSingularPresent :
  case EnglishWord::mainVerbAllows1object3rdPersonSingularPresent :
  case EnglishWord::mainVerbAllows2objects3rdPersonSingularPresent :
    break ;
  default:
    DEBUG_COUTN("freeing voc type" << (WORD) m_byType)
//    assert(m_arstrEnglishWord) ;
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
#ifdef SET_FREED_MEM_TO_NULL
      m_arstrEnglishWord = NULL ;
#endif
    }
//    assert(m_arstrGermanWord) ;
    if(m_arstrGermanWord)
    {
      delete [] m_arstrGermanWord ;
#ifdef SET_FREED_MEM_TO_NULL
      m_arstrGermanWord = NULL ;
#endif
    }
//    assert(m_arbyAttribute) ;
    if(m_arbyAttribute)
    {
      delete [] m_arbyAttribute ;
#ifdef SET_FREED_MEM_TO_NULL
      m_arbyAttribute = NULL ;
#endif
    }
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
      //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//      #pragma GCC diagnostic ignored  "-Wunused"
      int i = 0 ;
      SUPPRESS_UNUSED_VARIABLE_WARNING(i)
    }
#endif //#ifdef _DEBUG
  }
  //return 
}
