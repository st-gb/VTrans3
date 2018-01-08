//Ohne das folgende include/wenn Pfad in den Eigenschaften dieser Datei 
//unter C/C++->vorkompilierte Header->"PHC durch..." falsch ist: 
//"fatal error C1010: Unerwartetes Dateiende während der Suche nach dem
// vorkompilierten Header.[...]"
#ifdef _MSC_VER //MSVC compiler
  //#include "../../StdAfx.h"
#endif
#include <Attributes/Word.hpp> //class Word, EnglishWord, EnglishNoun
#include <Attributes/EnglishWord.hpp> //for class EnglishWord's enum
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
//header file for this VocabularyAndTranslation class
#include "VocabularyAndTranslation.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h> //DEBUG_COUTN(...)
/** For MicroSoft compilers do not include windef.h directly, else various 
 *  errors like "No Target Architecture" */
#ifdef _MSC_VER 
  #include <Windows.h>//includes windef.h;for BYTE
#else
  #include <windef.h> //for BYTE
#endif
#include <typeinfo> //for typeid()
/** SUPPRESS_UNUSED_VARIABLE_WARNING(...) */
#include <compiler/GCC/suppress_unused_variable.h>
/** GCC_DIAG_OFF(...), //GCC_DIAG_ON(...) */
#include <compiler/GCC/enable_disable_write_strings_warning.h>
////TODO
//#include <wxWidgets/VTransApp.hpp> //wxGetApp(...)
#include <Controller/TranslationControllerBase.hpp> //class TranslationControllerBase

#define SET_FREED_MEM_TO_NULL

/** static variable definitions */
VTrans3::BottomUpParser * VocabularyAndTranslation::s_p_bottomUpParser;
I_UserInterface * VocabularyAndTranslation::s_p_userinterface;

  const VocabularyAndTranslation::ArraySizes VocabularyAndTranslation::
    s_arraysizes [] = {
      //noun
      { NUMBER_OF_STRINGS_FOR_ENGLISH_NOUN, NUMBER_OF_STRINGS_FOR_GERMAN_NOUN, 2},
      //main verb
      { NUMBER_OF_STRINGS_FOR_ENGLISH_MAIN_VERB,
        NUMBER_OF_STRINGS_FOR_GERMAN_MAIN_VERB, 2},
      //adverb:
      { NUMBER_OF_STRINGS_FOR_ENGLISH_ADVERB,
        NUMBER_OF_STRINGS_FOR_GERMAN_ADVERB, 0} ,
      //adjective:
      { NUMBER_OF_STRINGS_FOR_ENGLISH_ADJECTIVE,
        NUMBER_OF_STRINGS_FOR_GERMAN_ADJECTIVE, 0}
    };

  const char * VocabularyAndTranslation::s_ar_chWordClass[] = {
    //By "(char *)" : avoid g++ error message
    //"warning: deprecated conversion from string constant to 'char*'"
    (char *) "noun"
    , "main verb"
    , "adjective"
  } ;
//enum EnglishWord::English_word_class GetWordClass(BYTE byVocabularyType)
//{
//
//}

//void VocabularyAndTranslation::GetNumberOfArrayElements(
//  EnglishWord::English_word_class engWordClass,
//  ArraySizes & arr_sz
//  )
//{
//   arr_sz.m_byArraySizeForByteArray = GetNumberOfArrayElements(
//     engWordClass,
//     arr_sz.m_byArraySizeForEnglishWord,
//     arr_sz.m_byArraySizeForGermanWord);
//}

/** @return: length of attribute array */
fastestUnsignedDataType VocabularyAndTranslation::GetNumberOfArrayElements(
  /*const*/ EnglishWord::English_word_class grammarPartID,
  fastestUnsignedDataType & numEngWords,
  fastestUnsignedDataType & numGerWords
  )
{
  EnglishWord::English_word_class engWordClass = EnglishWord::
    MapGrammarPartIDtoWordClass(grammarPartID);

  switch(grammarPartID)
  {
    case EnglishWord::auxiliary_verb:
      numEngWords = NUMBER_OF_STRINGS_FOR_GERMAN_MAIN_VERB;
      numGerWords = NUMBER_OF_STRINGS_FOR_GERMAN_MAIN_VERB ;
      return 2 ;
      break;
    case EnglishWord::conjunction:
    case EnglishWord::mainVerbPastParticiple1Obj:
      numEngWords = 1;
      numGerWords = 1;
      return 1 ;
      break;
    //case LetterTree::personal_pronoun :
    case EnglishWord::personal_pronoun :
      numEngWords = 1 ;
      numGerWords = 1 ;
      return 1 ;
      break;
    case EnglishWord::personal_pronoun_objective_form :
      numEngWords = 1 ;
      numGerWords = 1 ;
      return 1 ;
      break;
      //Only the singular (for parsing "indefinite article" + "singular"
      // ( if the rule was "indefinite article" + "noun",
      //  "indefinite article" + "plural" which is wrong would also be possible)
      // singular type is only needed for parsing. It shares the same attr as
      // the noun.
    default:
      //For vocabulary types that just refer another vocandtransl object's
      // attributes and for types that do not need (e.g. "definite article")
      //these attributes etc.
  //    m_pword = new Word() ;
      numEngWords = 0 ;
      numGerWords = 0 ;
      break;
//      return 0;
  }
  if( engWordClass <= EnglishWord:://auxiliary_verb
//      adverb
      adjective
      )
  {
    const ArraySizes & c_r_arraysizes = s_arraysizes[engWordClass];
    numEngWords = c_r_arraysizes.m_byArraySizeForEnglishWord ;
    numGerWords = c_r_arraysizes.m_byArraySizeForGermanWord ;
    //byArraySizeForGer = NUMBER_OF_STRINGS_FOR_GERMAN_NOUN ;
    return c_r_arraysizes.m_byArraySizeForByteArray;
  }
  return 0;
}

void VocabularyAndTranslation::AddFiniteVerbFormsFromGermanInfinitive(
  const std::string & std_strInfinitive)
{
  std::string std_strWordStem;
  GermanVerb::GetWordStemFromInfinitive(
    std_strInfinitive, std_strWordStem);
  std::string std_strFiniteForm;
  for( fastestUnsignedDataType /*enum GermanVerb::person_indices*/ person_index =
      GermanVerb::firstPersonSing;
      person_index < GermanVerb::beyondLastPerson;
      ++ person_index
     )
  {
    switch(person_index)
    {
    /** Infinitive is equal to 1st and 3rd persons plural:
     *   wandern->wir wandern, sie wandern
     *   laufen wir laufen sie laufen
     *   gehen wir gehen sie gehen
     *   arbeiten: wir arbeiten sie arbeiten */
    case GermanVerb::firstPersonPlur:
    case GermanVerb::thirdPersonPlur:
      SetGermanWord(
        std_strInfinitive,
        /** index 0 is infinitive->add "1". */
        person_index + 1
        );
      break;
    default:
      std_strFiniteForm = GermanVerb::GetPresentFiniteForm(
        std_strWordStem,
        (enum GermanVerb::person_indices) person_index
        );
      SetGermanWord(
        std_strFiniteForm,
        /** index 0 is infinitive->add "1". */
        person_index + 1
        );
    } //switch
  }
}

/** irregular: "können" -> ich/er kann */
void VocabularyAndTranslation::AddFiniteVerbFormsFromIrregularGerman1stPersSing(
  const std::string & std_strIrregularGerman1stPersSing)
{
  std::string std_strFiniteForm;
//  for( fastestUnsignedDataType /*enum GermanVerb::person_indices*/ person_index =
//      GermanVerb::firstPersonSing;
//      person_index < GermanVerb::beyondLastPerson;
//      ++ person_index
//     )
//  {
//    /** 3rd person singular is already passed as parameter. */
//    if( person_index == GermanVerb::thirdPersonSing )
//      SetGermanWord(
//        std_strIrregularGerman1stPersSing,
//        /** index 0 is infinitive->add "1". */
//        GermanVerb::arrayIndexFor3rdPersSingPres
//        );
//    else
//    {
//      std_strFiniteForm = std_strIrregularGerman1stPersSing +
//        GermanVerb::s_irregularVerbPresentSingularPersonEndings[person_index];
//      SetGermanWord(
//        std_strFiniteForm,
//        /** index 0 is infinitive->add "1". */
//        person_index + 1
//        );
//    }
//  }
  SetGermanWord(
      std_strIrregularGerman1stPersSing,
    /** index 0 is infinitive->add "1". */
    GermanVerb::firstPersonSing + 1
    );

  fastestUnsignedDataType person_index = GermanVerb::secondPersonSing;
  std_strFiniteForm = std_strIrregularGerman1stPersSing +
    GermanVerb::s_irregularVerbPresentSingularPersonEndings[person_index];
  SetGermanWord(
    std_strFiniteForm,
    /** index 0 is infinitive->add "1". */
    person_index + 1
    );

  SetGermanWord(
      std_strIrregularGerman1stPersSing,
    /** index 0 is infinitive->add "1". */
    GermanVerb::thirdPersonSing + 1
    );
}

/** Add finite past verb form from 3rd person singular past.
 *  So words may be added although they are missing in the dictionary.
 *  So e.g. add:
 *   -"gingen" from "ging"
 *  But remember: so only _regular_ word forms are added appropriately(!)
 *  */
void VocabularyAndTranslation::AddFiniteVerbFormsFromGerman3rdPersPast(
  const std::string & std_strWordStem)
{
  std::string std_strFiniteForm;
  for( fastestUnsignedDataType /*enum GermanVerb::person_indices*/ person_index =
      GermanVerb::firstPersonSing;
      person_index <= GermanVerb::thirdPersonPlur;
      ++ person_index
     )
  {
    /** 3rd person singular is already passed as parameter. */
    if( person_index == GermanVerb::thirdPersonSing )
      SetGermanWord(
        std_strWordStem,
        /** index 0 is infinitive->add "1". */
        GermanVerb::arrayIndexFor3rdPersSingPres
        );
    else
    {
      std_strFiniteForm = GermanVerb::GetPastFiniteForm(
        std_strWordStem,
        (enum GermanVerb::person_indices) person_index
        );
      SetGermanWord(
        std_strFiniteForm,
        /** index 0 is infinitive->add "1". */
        person_index + 1
        );
    }
  }
}

/** Add finite verb form from 3rd person singular present.
 *  So words may be added although they are missing in the dictionary.
 *  So e.g. add:
 *   -"gehe" from "geht"
 *  But remember: so only _regular_ word forms are added appropriately(!)
 *  */
void VocabularyAndTranslation::AddFiniteVerbFormsFromGerman3rdPersPresent(
  const std::string & germanWord)
{
  std::string std_strWordStem;
  GermanVerb::GetWordStemFrom3rdPersonPresent(
    germanWord.c_str(),
    germanWord.length(),
//    grammarPartID,
    std_strWordStem);
  std::string std_strFiniteForm;
  for( fastestUnsignedDataType /*enum GermanVerb::person_indices*/ person_index =
      GermanVerb::firstPersonSing;
      person_index <= GermanVerb::thirdPersonPlur;
      ++ person_index
     )
  {
    /** 3rd person singular is already passed as parameter. */
    if( person_index == GermanVerb::thirdPersonSing )
      SetGermanWord(
        germanWord,
        /** index 0 is infinitive->add "1". */
        GermanVerb::arrayIndexFor3rdPersSingPres
        );
    else
    {
      std_strFiniteForm = GermanVerb::GetPresentFiniteForm(
        std_strWordStem,
        (enum GermanVerb::person_indices) person_index
        );
      SetGermanWord(
        std_strFiniteForm,
        /** index 0 is infinitive->add "1". */
        person_index + 1
        );
    }
  }
}

/** Add words to a word family from a single word.
 *  So words may be added although they are missing in the dictionary.
 *  So e.g. add:
 *   -a plural from a singular: from "Katze" : add "Katzen"
 *   -finite verb form from infinitive: "geht" from "gehen"
 *   -infinitive
 *  But remember: so only _regular_ word forms are added appropriately(!)
 *  */
void VocabularyAndTranslation::PossiblyGenerateAndAddGermanAttributes(
  const EnglishWord::English_word_class grammarPartID,
  const std::string & germanWord,
  const fastestUnsignedDataType attributeIndexInsideDictFile
  )
{
  LOGN_DEBUG("begin--grammar part ID:" << grammarPartID
    << " word:" << germanWord
    << " attributeIndexInsideDictFile:" << attributeIndexInsideDictFile)
  const EnglishWord::English_word_class engWordClass = EnglishWord::
    MapGrammarPartIDtoWordClass(grammarPartID);

  GCC_DIAG_OFF(switch)
  switch(/*engWordClass*/ grammarPartID)
  {
  case EnglishWord::main_verb_allows_0object_infinitive :
  case EnglishWord::main_verb_allows_1object_infinitive :
  case EnglishWord::main_verb_allows_2objects_infinitive :
    if( attributeIndexInsideDictFile == 0)
      AddFiniteVerbFormsFromGermanInfinitive(germanWord);
    else if(attributeIndexInsideDictFile == 1)
      AddFiniteVerbFormsFromIrregularGerman1stPersSing(germanWord);
    break;
//  case EnglishWord::main_verb :
    /** if inf. is added (index 0) then derive finite German verb forms
    * from inf. and add to voc and transl. */
//                  GermanVerb::GenerateRemainingAttributeValues(germanWordIndex, germanWord);
//    if( germanWordIndex == 0 )
//    {
//      std::string std_strWordStem;
//      GermanVerb::GetWordStem(
//        germanWord.c_str(),
//        germanWord.length(),
//        grammarPartID,
//        std_strWordStem);
  case EnglishWord::mainVerbAllows0object3rdPersonSingularPast :
  case EnglishWord::mainVerbAllows1object3rdPersonSingularPast :
  case EnglishWord::mainVerbAllows2objects3rdPersonSingularPast :
    AddFiniteVerbFormsFromGerman3rdPersPast(germanWord);
    break;
  case EnglishWord::mainVerbAllows0object3rdPersonSingularPresent :
  case EnglishWord::mainVerbAllows1object3rdPersonSingularPresent :
  case EnglishWord::mainVerbAllows2objects3rdPersonSingularPresent :
    AddFiniteVerbFormsFromGerman3rdPersPresent(germanWord);
    break;
//  default:
  }
  GCC_DIAG_ON(switch)
}

VocabularyAndTranslation::VocabularyAndTranslation(/*BYTE*/
  EnglishWord::English_word_class byVocabularyType)
{
  LOGN_DEBUG("begin--pointer to this object:" << this)
  m_englishWordClass = byVocabularyType ;
  BYTE byArraySizeForEng = 0 ;

  //BYTE byArraySizeForGer = 0 ;

//  GetWordClass(byVocabularyType);

  unsigned numByteArrEles, numEngWords, numGerWords;
  numByteArrEles = GetNumberOfArrayElements(byVocabularyType, numEngWords,
    numGerWords);
  if( numEngWords)
  {
    //TODO memory leak here according to valgrind
    m_arstrEnglishWord = new word_type[numEngWords];
    memset(m_arstrEnglishWord, 0, sizeof(word_type) * numEngWords);
  }
  else
    m_arstrEnglishWord = NULL;
  if( numGerWords)
  {
    //TODO memory leak here according to valgrind
    m_arstrGermanWord = new word_type[numGerWords];
    memset(m_arstrGermanWord, 0, sizeof(word_type) * numGerWords);
  }
  else
    m_arstrGermanWord = NULL;
  if( numByteArrEles)
  {
    m_arbyAttribute = new BYTE[numByteArrEles];
    memset(m_arbyAttribute,0,numByteArrEles) ;
  }
  else
    m_arbyAttribute = NULL;
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
  //TODO crash here
  FreeMemory();
}

std::string VocabularyAndTranslation::GetWordClassAsString() const
{
  std::string std_str;
  GCC_DIAG_OFF(switch)
  switch(m_englishWordClass)
  {
  case EnglishWord::English_definite_article:
    return "definite_article";
  case EnglishWord::beyond_last_entry:
    return "beyond_last_entry";
//    break;
  }
  GCC_DIAG_ON(switch)
  return "";
}

fastestUnsignedDataType VocabularyAndTranslation::GetNumberOfBytes()
{
  ArraySizes arraySizes;
  GetNumberOfArrayElements(m_englishWordClass, arraySizes);
//  if(arraySizes.m_byArraySizeForByteArray )

  fastestUnsignedDataType numBytesForPointers =
    arraySizes.m_byArraySizeForEnglishWord * sizeof(word_type) +
    arraySizes.m_byArraySizeForGermanWord * sizeof(word_type) +
    sizeof(word_type *) * /*German and English*/ 2 +
    sizeof(BYTE *) + arraySizes.m_byArraySizeForByteArray;
  for(int i = 0; i < arraySizes.m_byArraySizeForEnglishWord; i++)
  {
    if( m_arstrEnglishWord[i])
      numBytesForPointers += strlen(m_arstrEnglishWord[i]) + /* 0 char */ 1;
  }
  for(int i = 0; i < arraySizes.m_byArraySizeForGermanWord; i++)
  {
    if(m_arstrGermanWord[i])
      numBytesForPointers += strlen(m_arstrGermanWord[i]) + /* 0 char */ 1;
  }
  return numBytesForPointers;
}

/** free memory allocated in VocabularyAndTranslation constructor. */
void VocabularyAndTranslation::FreeMemory()
{
  ArraySizes arraySizes;
  arraySizes.m_byArraySizeForByteArray = GetNumberOfArrayElements(
    m_englishWordClass,
    arraySizes.m_byArraySizeForEnglishWord,
    arraySizes.m_byArraySizeForGermanWord);
//  GCC_DIAG_OFF(switch)
//  switch( m_englishWordClass )
//  {
  //  // singular type is only needed for parsing. It shares the same attr as
  //  // the noun. Because for the noun the storage is freed it should NOT be done again
  //  // for the singular.
  //  case EnglishWord::singular :
  //  case EnglishWord::plural_noun :
  //  case EnglishWord::mainVerbAllows0object3rdPersonSingularPresent :
  //  case EnglishWord::mainVerbAllows1object3rdPersonSingularPresent :
  //  case EnglishWord::mainVerbAllows2objects3rdPersonSingularPresent :
  //  case EnglishWord::adjective_positiveForm:
  //    break ;
  //  default:
    
//  case EnglishWord::singular :
//  case EnglishWord::main_verb_allows_0object_infinitive:
//  case EnglishWord::main_verb_allows_1object_infinitive:
//  case EnglishWord::main_verb_allows_2objects_infinitive:
//  case EnglishWord::adjective_positiveForm:
//  case EnglishWord::adverb:
//  case EnglishWord::personal_pronoun:
//  case EnglishWord::auxiliary_verb:
//  case EnglishWord::personal_pronoun_objective_form:
//    DEBUG_COUTN("freeing voc type" << (WORD) m_englishWordClass)
  //    assert(m_arstrEnglishWord) ;
    if(m_arstrEnglishWord)
    {
#ifdef _DEBUG
      const std::map<WORD, std::string> & r_stdmap_wRuleID2RuleName =
        s_p_bottomUpParser->m_stdmap_wRuleID2RuleName;
      std::map<WORD, std::string>::const_iterator c_iterRuleID2RuleName =
        /*::wxGetApp()*/ r_stdmap_wRuleID2RuleName.find(m_englishWordClass);
      std::string std_strWordClassName = "";
      if( c_iterRuleID2RuleName != r_stdmap_wRuleID2RuleName.end() )
      {
        std_strWordClassName = c_iterRuleID2RuleName->first;
      }
      if(m_arstrEnglishWord[0])
        LOGN_DEBUG("freeing memory for " << * m_arstrEnglishWord[0]
          << " word class:" << std_strWordClassName)
      else
        LOGN_DEBUG("freeing memory for " << (void *) m_arstrEnglishWord[0]
          << " word class:" << m_englishWordClass << std_strWordClassName)
#endif
  #ifdef _DEBUG_FREEING_MEM
      TRACE("~VocabularyAndTranslation()--%x ",this);
      BYTE byNumWords = g_arbyNumberOfEnglishWords[ m_englishWordClass - ENGLISH_NOUN ] ;
      for(int i = 0 ; i < byNumWords ; i++ )
        TRACE("\"%s\"", m_arstrEnglishWord[i].c_str() ) ;
      TRACE("\n") ;
  #endif
      const fastestUnsignedDataType numEnglishWords = arraySizes.m_byArraySizeForEnglishWord;
      word_type englishWord;
      for(fastestUnsignedDataType i = 0 ; i < numEnglishWords ; i++ )
      {
        englishWord = m_arstrEnglishWord[i];
        if( englishWord)
        {
          delete [] englishWord;
        }
      }
      delete [] m_arstrEnglishWord ;
  #ifdef SET_FREED_MEM_TO_NULL
      m_arstrEnglishWord = NULL ;
  #endif
    }
  //    assert(m_arstrGermanWord) ;
    if(m_arstrGermanWord)
    {
      //Has a German word->is a word pair.
//      s_p_translationControllerBase->s_dictionary.DecreaseNumberOfVocPairs();
      const fastestUnsignedDataType numGermanWords = arraySizes.
        m_byArraySizeForGermanWord;
      word_type germanWord;
      for(fastestUnsignedDataType i = 0 ; i < numGermanWords ; i++ )
      {
        germanWord = m_arstrGermanWord[i];
        if(germanWord)
        {
          delete [] germanWord;
        }
      }
      //TODO runtime error: corrupted heap structure here
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
//    break;
//  }//switch
//  GCC_DIAG_ON(switch)
}
//GCC_DIAG_ON(-Wno-switch)

std::string VocabularyAndTranslation::GetEnglishWordAsStdString(
  const fastestUnsignedDataType englishWordArrayIndex) const
{
  if( m_englishWordClass <= EnglishWord::adverb )
  {
    if( englishWordArrayIndex < s_arraysizes[m_englishWordClass].m_byArraySizeForEnglishWord
  //    m_arstrEnglishWord
      )
    {
      DEBUGN("language for choosing attribute value is "
        "English, index:" << englishWordArrayIndex )
      word_type englishWord = m_arstrEnglishWord[englishWordArrayIndex ];
      int strLen = GetEnglishWordLength(englishWordArrayIndex);
      std::string r_stdstrAttrVal(englishWord, strLen);
      return r_stdstrAttrVal ;
    }
    else
    {
      s_p_userinterface->Message(
        "index for English string array is out of bounds");
    }
  }
  else
  {
    word_type englishWord = m_arstrEnglishWord[englishWordArrayIndex ];
    int strLen = GetEnglishWordLength(englishWordArrayIndex);
    std::string r_stdstrAttrVal(englishWord, strLen);
    return r_stdstrAttrVal ;
  }
  //std::string & r_stdstrTextTokens =
  return "";
}

void VocabularyAndTranslation::GetAttributeValue(BYTE byIndex)
{
  //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
  //                #pragma GCC diagnostic ignored  "-Wunused"
  if( m_englishWordClass <= EnglishWord::adverb )
  {
    if( byIndex < s_arraysizes[m_englishWordClass].m_byArraySizeForByteArray
  //    m_arbyAttribute
      )
    {
      BYTE by = m_arbyAttribute[byIndex ] ;
      SUPPRESS_UNUSED_VARIABLE_WARNING(by)
    }
    else
    {
      s_p_userinterface->Message(
        "index for attributes byte array is out of bounds");
    }
  }
  else
  {
    BYTE by = m_arbyAttribute[byIndex ] ;
    SUPPRESS_UNUSED_VARIABLE_WARNING(by)
  }
}

//VocabularyAndTranslation::word_type VocabularyAndTranslation::
//  GetAssembledGermanString(
//  const fastestUnsignedDataType germanWordIndex)
//{
//  std::string std_strWortstamm;
//
////  if( germanWordIndex >)
//
//  const word_type germanInfinitive = m_arstrGermanWord[GermanVerb::arrayIndexForInfinitive];
////    GermanVerb::GetWordStem(ar_ch, germanWords[Infinitive], std_strWortstamm);
////    GermanVerb::Get1stPersSing(ar_ch, germanWords[Infinitive] );
//  GermanVerb::GetWordStem( germanInfinitive,
//    strlen(germanInfinitive), std_strWortstamm );
//
//  if( germanWordIndex > GermanVerb::arrayIndexForInfinitive &&
//      germanWordIndex < GermanVerb::arrayIndexFor1stPersSingPast )
//  {
//    std::string std_strFiniteForm;
//    std_strFiniteForm = GermanVerb::GetPresentFiniteForm(
//      std_strWortstamm,
//      (enum GermanVerb::person_indices) (germanWordIndex - 1) );
//
//    if( index == GermanVerb::secondPersonSing ) //Skip 3rd person.
//      ++ index;
//  }
//
//  WordData & germanThirdPersPres = (WordData &) germanWords[ThirdPersSingPres];
//  GetFiniteWord(ar_ch, germanThirdPersPres);
//
//  WordData & germanThirdPersPast = (WordData &) germanWords[ThirdPersSingPast];
//  GetFiniteWord(ar_ch, germanThirdPersPast);
//
//  if( germanWordIndex > GermanVerb::arrayIndexFor3rdPersPlurPres &&
//      germanWordIndex < GermanVerb::arrayIndexForPastParticiple )
//  {
//    std::string thirdPersPast = std::string(ar_ch + germanThirdPersPast.
//      m_charIndexOfBegin, germanThirdPersPast.GetStringLength() );
//    for( unsigned index = GermanVerb::firstPersonSing;
//        index < GermanVerb::beyondLastPerson; ++ index)
//    {
//      std_strFiniteForm = GermanVerb::GetPastFiniteForm(//std_strWortstamm
//        ar_ch + germanThirdPersPast.m_charIndexOfBegin,
//        germanThirdPersPast.GetStringLength()
//        , (enum GermanVerb::person_indices) (GermanVerb::firstPersonSing + index)
//        );
//      if( index == GermanVerb::secondPersonSing ) //Skip 3rd person.
//        ++ index;
//    }
//  }
//}

VocabularyAndTranslation::word_type VocabularyAndTranslation::GetGermanString(
  const fastestUnsignedDataType germanWordIndex) const
{
//                  DEBUG_COUTN
  DEBUGN("language for choosing attribute value is "
    "German, index:" << germanWordIndex
    //<< "m_pfn_TransformString:" << m_pfn_TransformString
    )
  if( m_englishWordClass <= EnglishWord::adverb)
  {
    //TODO crashed here for parallel translation: m_englishWordClass was out of
    //range : "3009438744"
    if( germanWordIndex < s_arraysizes[ m_englishWordClass].m_byArraySizeForGermanWord
//      m_arstrGermanWord
      )
    {
      word_type & r_stdstrAttrVal = m_arstrGermanWord[germanWordIndex ] ;
      return r_stdstrAttrVal ;
    }
    else
    {
      s_p_userinterface->Message(
        "index for German string array is out of bounds");
    }
  }
  else
  {
    word_type & r_stdstrAttrVal = m_arstrGermanWord[germanWordIndex ] ;
    if( r_stdstrAttrVal == NULL )
      return (word_type) "";
    return r_stdstrAttrVal ;
  }
  /** the dictionary reader knows which attributes were inserted into the
   *   word arrays (e.g. if there is a 3rd person singular inside the dictionary)
   *   -> let it deliver the string. */
//  s_p_translationControllerBase->m_p_dictionaryReader->GetGermanString(
//    germanWordIndex, this);
  return (word_type) "";
}

std::ostream & operator << (std::ostream & std_ostream, 
    const VocabularyAndTranslation & vocabularyAndTranslation)
{
  EnglishWord::English_word_class word_class = EnglishWord::
    MapGrammarPartIDtoWordClass(vocabularyAndTranslation.m_englishWordClass);
  std_ostream << "grammar part ID:" << vocabularyAndTranslation.m_englishWordClass << "->word class:"
    << word_class << " " << vocabularyAndTranslation.GetWordClassAsString() << " Ger:";
  VocabularyAndTranslation::ArraySizes arraySizes;
  vocabularyAndTranslation.GetNumberOfArrayElements(arraySizes);
  if( vocabularyAndTranslation.m_arstrGermanWord && 
      arraySizes.m_byArraySizeForGermanWord > 0 )
  {
    std_ostream << "(";
    /** Ouput German (destination language) word attributes. */
    for(fastestUnsignedDataType attributeIndex = 0; attributeIndex < 
        arraySizes.m_byArraySizeForGermanWord; ++ attributeIndex )
    {
      if( vocabularyAndTranslation.m_arstrGermanWord[attributeIndex] )
        std_ostream << attributeIndex << ":" << 
        vocabularyAndTranslation.m_arstrGermanWord[attributeIndex] << "|";
      else
        std_ostream << attributeIndex << ":|";
    }
    std_ostream << ")"; 
  }
  std_ostream << " Eng:";
  if( vocabularyAndTranslation.m_arstrEnglishWord)
  {
    for(fastestUnsignedDataType index = 0; index < 
      arraySizes.m_byArraySizeForEnglishWord; ++ index )
    {
      std_ostream << "(" << index << ";" << vocabularyAndTranslation.
        m_arstrEnglishWord[index] << "|";
    }
  }
  return std_ostream;
}

//BYTE * diff(const char * const word, const fastestUnsignedDataType stringLen)
//{
//  BYTE * ar_by = new BYTE[stringLen];
//  const word_type firstGermanWord = m_arstrGermanWord[0];
//  BYTE byCurrentValue;
//  char firstWordCurrentChar;
//  if( firstGermanWord[0] != '\0' )
//  {
//    for(fastestUnsignedDataType i = 0; i < stringLen; ++i)
//    {
////          byCurrentValue
//      firstWordCurrentChar = firstGermanWord[i];
//      if( firstWordCurrentChar != '\0' )
//      {
//        switch(firstWordCurrentChar)
//        {
//          case 'a': //Mann - Männer = 0100 255-e,255-r
//            if( word[i] == 'ä' )// 0|1|0|0|
//              ar_by[i] = 1;
//            break;
//          case 'o': //Tochter - Töchter = 00100
//           if( word[i] == 'ö' )
//             ar_by[i] = 1;
//           break;
//          case 'u': //Bruder - Brüder = 00100
//           if( word[i] == 'ü' )
//             ar_by[i] = 1;
//           break;
//        }
//        ar_by[i] = firstGermanWord[i] - word[i];
//      }
//    }
//  }
//}

//void decompress()
//{
//
//}
//
//// 0 -> 0 (kein Unterschied)
//// 10 -> 1 (wenn a,u,o->Umlaut, sonst nächster Buchstabe)
//// 11 -> es folgen 7 Bit Differenzen
//// 128 -> string end
//// Mann -> Männer: 0|10|0|0|11|255-e|255-r|128
//BYTE * compress(const BYTE * const ar_by, const fastestUnsignedDataType len)
//{
//  unsigned char char_frequencies[256];
//  memset(char_frequencies, 0, 256);
////  BYTE by[];
//  BYTE byCurrentValue;
//  unsigned bitIndex = 0;
//  for(fastestUnsignedDataType i = 0; i < len; i++)
//  {
//    byCurrentValue = ar_by[0];
////    char_frequencies[byCurrentValue]++;
//    if( byCurrentValue == 0 )
//      ar_by[bitIndex] = 0
//  }
//  BYTE
//}
//
//void VocabularyAndTranslation::InsertGermanString(
//  char * ch, index )
//{
//  if( m_arstrGermanWord[0] && strlen(m_arstrGermanWord[0]) > 0 )
//  {
//    m_arstrGermanWord[index] = diff(m_arstrGermanWord[0], pch);
//    compress();
//  }
//}

////Word
//void VocabularyAndTranslation::GetWord(//Word & word
//  AutomDelWord & r_automdelword )
//{
//  //Word
//  switch( m_englishWordClass )
//  {
//  case ENGLISH_NOUN:
//    //m_pword = new EnglishNoun() ;
//    EnglishNoun en ;
//    Word word ;
//    //The object needs to be dynamically created (=on the heap), else a
//    //downcast (cast to a superclass) with correct typeid is not possible.
//    EnglishNoun * p_en = new EnglishNoun();
//    en.m_strSingular = m_arstrEnglishWord[0].c_str() ;
//    //m_arstrGermanWord = new std::string[NUMBER_OF_STRINGS_FOR_GERMAN_NOUN] ;
//    //byArraySizeForEng = NUMBER_OF_STRINGS_FOR_ENGLISH_NOUN ;
//    ////byArraySizeForGer = NUMBER_OF_STRINGS_FOR_GERMAN_NOUN ;
//    //m_arbyAttribute = new BYTE[2] ;
//    //word = dynamic_cast<Word &>(en) ;
//    //word = en ;
//    p_en->m_bType = ENGLISH_NOUN ;
//    r_automdelword.SetWord(p_en) ;
//#ifdef _DEBUG
//    if( typeid(//*pWordNode->m_pWord
//      //word
//      //* r_automdelword.GetWord()
//      * p_en
//      ) == typeid(EnglishNoun)
//      )
//    {
//      //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
////      #pragma GCC diagnostic ignored  "-Wunused"
//      int i = 0 ;
//      SUPPRESS_UNUSED_VARIABLE_WARNING(i)
//    }
//#endif //#ifdef _DEBUG
//  }
//  //return
//}
