#if !defined (IO_H_INCLUDED)
	    #define IO_H_INCLUDED

#include <fstream> //for ofstream
//#include "xmlwriter.h"
#include "rest.h" //for class SentenceAndValidityAndProperName
#include <Attributes/Word.hpp> //for class Word
#include <set> //std::set

//The following characters are used in the vocabulary file:
#define STRING_DELIMITER '9'
#define NUMBER_OF_CHARS_FOR_ENG_NOUN_ATTS 3
#define WORD_TYPE_CHAR_INDEX 0
#define WORD_TYPE_NOUN '1'
#define WORD_TYPE_MAIN_VERB '2' //Vollverb
#define WORD_TYPE_ADJECTIVE '3'
#define WORD_TYPE_ADVERB '4'
#define WORD_TYPE_PREPOSITION '5'
#define WORD_TYPE_PRONOUN '6'
#define WORD_TYPE_AUX_VERB '7'
#define WORD_TYPE_CONJUNCTION '8'
//Used for: IDs for German word types are English word type + NUMBER_OF_WORD_TYPES.
#define NUMBER_OF_WORD_TYPES 10
#define STRING_INDEX_FOR_INFINITIVE 0

//const char * readInputText(const std::string & strFilePath) ;

//Forward declarations.
class LetterNode ;
class LetterTree ;
class VocabularyAndTranslation ;

class OneLinePerWordPair
{
  //Use an enum to because that lets the values automatically be unique.
  enum verb_string_indices
  {
    string_index_for_inifinitive = 0 ,
    string_index_for_past_tense ,
    string_index_for_past_participle
  };
  //Use an enum to because that lets the values automatically be unique.
  enum verb_byte_attribiute_indices
  {
    movement_verb = 0 ,
    auxiliary_verb_type ,
    grammatical_case ,
    reflexive_or_not
  };
private :
//  static VocabularyAndTranslation * s_pvocabularyandtranslation ; //= NULL ;
public:
  static LetterTree * s_p_lettertree ;
  static DWORD s_dwNumberOfVocabularyPairs ;
  static //Word *
    void extract(
    const VTrans::string_type & str
    , BYTE bEnglishWord
    , int & ret
    ) ;
  static void InsertEnglishAdjective(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertEnglishAdverb(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertEnglishAuxiliaryVerb(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertEnglishConjunction(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertEnglishMainVerb(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    , std::set<LetterNode *> stdsetpletternodeLastStringChar
    ) ;
  static void InsertEnglishNoun(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertEnglishPreposition(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertEnglishPronoun(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertGermanNoun(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertGermanMainVerb(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertGermanAdjective(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertGermanAdverb(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertGermanPreposition(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertGermanPronoun(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertGermanAuxiliaryVerb(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
  static void InsertGermanConjunction(
    const std::string & strCurrentWordData
    , BYTE byVocabularyType
    ) ;
//  static inline void HandleVocabularyAndTranslationPointerInsertion(
//    std::set<LetterNode *> & stdsetpletternodeLastStringChar
//    , LetterNode * pletternodeCurrent
//    //, VocabularyAndTranslation * pvocabularyandtranslation
//    , bool  bInsertNewVocabularyAndTranslation
//    , BYTE byVocabularyType
//    ) ;
//  static //inline
//    void InsertIntoTrieAndHandleVocabularyAndTranslation(
//      std::set<LetterNode *> & stdsetpletternodeLastStringChar
//    //, LetterNode * pletternodeCurrent
//    //, VocabularyAndTranslation * pvocabularyandtranslation
//    , bool & bInsertNewVocabularyAndTranslation
//    , BYTE byVocabularyType
//    , const std::string & str
//    , int nLength
//    , int nIndexOf1stChar
//    ) ;
  static BYTE LoadWords(//WordNode * wn
    std::string & r_strWordsFilePath );
  inline static BYTE WordFileWordClassValueToZeroBasedIndex(
    BYTE byWordFileWordClassValue ) ;
} ; //end class

BYTE      readInputText(const std::string & strFilePath, std::string & str) ;
//LPCSTR                      UTF8toASCII(const char * str);
#ifndef __MINGW32__ //MinGW has no iconv
//void                        writeToOutputStream(std::ostream & rofstreamTranslToGerman,
//  //std::vector<Range> & vecRange,std::vector<CStringVector> & vecstrvec
//  std::vector<SentenceAndValidityAndProperName> & vecsentenceandvalidityandpropername,
//  xmlwriter & MyXml
//  );
#endif //#ifndef __MINGW32__ //MinGW has no iconv

#endif //!defined (IO_H_INCLUDED)
