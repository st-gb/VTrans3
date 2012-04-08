#if !defined (IO_H_INCLUDED)
	    #define IO_H_INCLUDED

#include <fstream> //for ofstream
//#include "xmlwriter.h"
#include "rest.h" //for class SentenceAndValidityAndProperName
#include <Attributes/Word.hpp> //for class Word
#include <set> //std::set
//#include <wxWidgets/VTransApp.hpp> //class wxWidgets::VTransApp

#include "word_class_characters.h"

//Used for: IDs for German word types are English word type + NUMBER_OF_WORD_TYPES.
#define NUMBER_OF_WORD_TYPES 10
#define STRING_INDEX_FOR_INFINITIVE 0

//const char * readInputText(const std::string & strFilePath) ;

//Forward declarations.
class LetterNode ;
class LetterTree ;
class I_UserInterface;
class VocabularyAndTranslation ;

namespace wxWidgets
{
  class VTransApp;
}

class OneLinePerWordPair
{
  enum language { English = 0, German };
    enum verb_string_indices{
      string_index_for_inifinitive = 0,
      string_index_for_past_tense,
      string_index_for_past_participle};
    enum verb_byte_attribiute_indices{
      movement_verb = 0,
      auxiliary_verb_type,
      grammatical_case,
      reflexive_or_not};
private:
//  static wxWidgets::VTransApp * s_p_userinterface;
public:
    static I_UserInterface * s_p_userinterface;
    static LetterTree *s_p_lettertree;
    static DWORD s_dwNumberOfVocabularyPairs;
    static void extract(const VTrans::string_type & str, BYTE bEnglishWord,
      int & ret);
    static void InsertEnglishAdjective(const std::string & strCurrentWordData,
      BYTE byVocabularyType);
    static void InsertEnglishAdverb(const std::string & strCurrentWordData,
      BYTE byVocabularyType);
    static void InsertEnglishAuxiliaryVerb(
      const std::string & strCurrentWordData, BYTE byVocabularyType);
    static void InsertEnglishConjunction(
      const std::string & strCurrentWordData, BYTE byVocabularyType);
    static void InsertEnglishMainVerb(const std::string & strCurrentWordData,
      BYTE byVocabularyType,
      std::set<LetterNode*> stdsetpletternodeLastStringChar);
    static BYTE InsertEnglishNoun(const std::string & strCurrentWordData,
      BYTE byVocabularyType);
    static void InsertEnglishPreposition(
      const std::string & strCurrentWordData, BYTE byVocabularyType);
    static void InsertEnglishPronoun(const std::string & strCurrentWordData,
      BYTE byVocabularyType);
    static void InsertGermanNoun(const std::string & strCurrentWordData,
      BYTE byVocabularyType);
    static void InsertGermanMainVerb(const std::string & strCurrentWordData,
      BYTE byVocabularyType);
    static void InsertGermanAdjective(const std::string & strCurrentWordData,
      BYTE byVocabularyType);
    static void InsertGermanAdverb(const std::string & strCurrentWordData,
      BYTE byVocabularyType);
    static void InsertGermanPreposition(const std::string & strCurrentWordData,
      BYTE byVocabularyType);
    static void InsertGermanPronoun(const std::string & strCurrentWordData,
      BYTE byVocabularyType);
    static void InsertGermanAuxiliaryVerb(
      const std::string & strCurrentWordData, BYTE byVocabularyType);
    static void InsertGermanConjunction(
      const std::string & strCurrentWordData, BYTE byVocabularyType);
    static BYTE LoadWords(std::string & r_strWordsFilePath);
    inline static BYTE WordFileWordClassValueToZeroBasedIndex(
      BYTE byWordFileWordClassValue);
private:
    static inline BYTE InsertEnglishVocable(
      const VTrans::string_type & strCurrentWordData,
      std::set<LetterNode*> & stdsetpletternodeLastStringChar);
    static inline BYTE InsertGermanVocable(
      const VTrans::string_type & strCurrentWordData) ;
} ; //end class

BYTE      readInputText(const std::string & strFilePath, std::string & str) ;
//LPCSTR                      UTF8toASCII(const char * str);

#endif //!defined (IO_H_INCLUDED)
