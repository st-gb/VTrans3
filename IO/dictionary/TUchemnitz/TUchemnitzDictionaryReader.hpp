/*
 * TUchemnitzDictionaryReader.hpp
 *
 *  Created on: 05.01.2013
 *      Author: Stefan
 */

#ifndef TUCHEMNITZDICTIONARYREADER_HPP_
#define TUCHEMNITZDICTIONARYREADER_HPP_

#include <data_structures/Trie/NodeTrie/NodeTrie.hpp>
#include <Attributes/EnglishWord.hpp> //class EnglishWord
#include <set> //class std::set
#include <fastest_data_type.h> //typedef fastestUnsignedDataType
#include "../DictionaryReaderBase.hpp" //class DictionaryReaderBase
#include <fstream>
#include <Controller/GetErrorMessageFromLastErrorCode.hpp>
#include <string> //class std::string

#define INDEX_OF_LAST_SMALL_LETTER_IN_ASCII 128

//fwd decl.
//class LetterNode;
//class LetterTree;
class I_UserInterface;
class EnglishWord;
class IVocabularyInMainMem;
class VocabularyAndTranslation;
//namespace std
//{
//  class string;
//}
//class EnglishWord
//{
//  enum English_word_class;
//};

class WordData
{
public:
  enum { isNotSet = -1};
  signed m_charIndexOfBegin, m_charIndexOfEnd;
  char * begin;
//  const std::string & m_std_strWholeEntry;

//  WordData(//const std::string & std_strWholeEntry,
//    unsigned charIndexOfBegin,
//    unsigned charIndexOfEnd)
//    : //m_std_strWholeEntry(std_strWholeEntry),
//      m_charIndexOfBegin(charIndexOfBegin),
//      m_charIndexOfEnd(charIndexOfEnd)
//  {
//  }
  WordData()
    : m_charIndexOfBegin(isNotSet)
      , m_charIndexOfEnd(isNotSet)
    //, begin(NULL)
    {}
  bool BeginIsNotSet() const { return m_charIndexOfBegin == isNotSet; }
  bool EndIsNotSet() const { return m_charIndexOfEnd == isNotSet; }
  bool BeginIsSet() const { return ! BeginIsNotSet(); }
  bool EndIsSet() const { return ! EndIsNotSet(); }
  int GetStringLength() const { return m_charIndexOfEnd - m_charIndexOfBegin; }
  std::string getStdString();
  void Reset()
  {
    m_charIndexOfBegin = isNotSet;
    m_charIndexOfEnd = isNotSet;
  }
};

std::ostream & operator << (std::ostream & o, const WordData & wd);

class TUchemnitzDictionaryReader
  : public DictionaryReader::DictionaryReaderBase
{
  //German entry:
  //"arbeiten {vi} (an) | arbeitend | gearbeitet | arbeitet | arbeitete ::"
  enum GermanVerbArrayIndices { Infinitive = 0,
    Progressive, PastParticiple,
    ThirdPersSingPres, ThirdPersSingPast };
  enum NounArrayIndices { Singular = 0, Plural };
  unsigned m_dictionaryFileLineNumber;
  typedef void insertWordReturnType;
  typedef insertWordReturnType (* extractVocable )(//char *
//    const std::string &
    const char * array, unsigned numChars, unsigned charIndex,
    const unsigned pipeCount );
  std::ifstream m_dictFile;
public:
  typedef void (TUchemnitzDictionaryReader::*insertVocable
      )(//char *
//    const std::string &
    const char * array,
    const WordData englishWords[10], const WordData germanWords[10]);

  TUchemnitzDictionaryReader()
    : DictionaryReaderBase(NULL) { }
  TUchemnitzDictionaryReader(I_UserInterface &
    , IVocabularyInMainMem * p_vocaccess);
//  TUchemnitzDictionaryReader(I_UserInterface &);

  bool OpenDictFile(const char * const filePath)
  {
    //If loading another dict file.
    if( m_dictFile.is_open() )
    {
      m_dictFile.close();
    }
    m_dictFile.open(filePath);
    std::string str = ::GetErrorMessageFromLastErrorCodeA();
    return m_dictFile.is_open();
  }

  bool SetFileOffSet(const fastestUnsignedDataType offset)
  {
    m_dictFile.seekg(offset, std::ios_base::beg);
    return m_dictFile.good();
  }

  void SetVocabularyAccess(IVocabularyInMainMem * p)
  {
    m_p_vocaccess = p;
  }

  /** static-> no need to (implicitly) pass an object pointer */
//  static void read();
  static NodeTrie<insertVocable> s_nodetrieWordKind;
//  static LetterTree * s_p_lettertree;
  static IVocabularyInMainMem * s_p_vocinmainmem;
  static I_UserInterface * s_p_i_userinterface;

  /*static*/ virtual void extractSingleEntry(const char * array, unsigned numChars
    //const std::string &
    );
  /*static*/ bool extractVocables(const char * filePath);
  /*static*/ bool read(const char * filePath)
    { return extractVocables(filePath); }

  //static
    /*LetterNode * */ VocabularyAndTranslation * Insert1stEnglishWord(
//    std::set<LetterNode *> & std_setpletternodeLastStringChar,
//    const std::string & strCurrentWordData,
    const char * array,
//    unsigned firstWordChar,
//    unsigned nIndexOfCurrentChar,
    unsigned stringLen,
    enum EnglishWord::English_word_class word_class);

  /*static*/ void InsertEnglishWord(
    const char * const arrayWordBegin,
    unsigned arrayIndex,
    unsigned stringLen,
    const VocabularyAndTranslation * const
      p_vocabularyandtranslation
    );
  static void InsertGermanWord(
//    const std::string & strCurrentWordData,
    const char * const arrayWordBegin,
    unsigned arrayIndex,
//    unsigned nCharIndexOf1stWordChar,
    unsigned stringLen
//    unsigned nIndexOfCurrentChar
    , VocabularyAndTranslation * p_vocandtransl
    );
  /*static*/ void InsertEnglishWord(
    const char * ar_ch,
    const WordData & germanWord,
    unsigned vocAndTranslArrayIndex,
    const VocabularyAndTranslation * const
      p_vocabularyandtranslation
    );
  static void InsertGermanWord(
    const char * ar_ch,
    const WordData & germanWord,
    unsigned vocAndTranslArrayIndex,
    VocabularyAndTranslation * p_vocandtransl
    );
  /*static*/ void InsertAdverb(
    const char * array,
    const WordData englishWords[10], const WordData germanWords[10]);
  /*static*/ void InsertAdverb(//const char * array
    const char * ar_ch, unsigned numChars,
    unsigned charIndex, const unsigned germanPipeCount);
  /*static*/ void InsertAdjective(
//    const std::string & strCurrentWordData,
    const char * array,
    const WordData englishWords[10], const WordData germanWords[10]);
  /*static*/ insertWordReturnType InsertFeminineNoun(
    const char * ar_ch,
    const WordData englishWords[10], const WordData germanWords[10]);
  /*static*/ void InsertMasculineNoun(
    const char * ar_ch,
    const WordData englishWords[10], const WordData germanWords[10]);
  /*static*/ void InsertNeutralNoun(
    const char * ar_ch,
    const WordData englishWords[10], const WordData germanWords[10]);
  /*static*/ /*void*/ VocabularyAndTranslation * InsertNoun(
    const char * ar_ch,
    const WordData englishWords[10], const WordData germanWords[10]);
  /*static*/ void InsertFiniteGermanVerbForms(
    const char * ar_ch, const WordData germanWords[],
    VocabularyAndTranslation * p_vocandtransl);
  /*static*/ void InsertIntransitiveVerb(
    const char * ar_ch,
    const WordData englishWords[10], const WordData germanWords[10]);
  inline /*static*/ void InsertGermanVerbWords(
    const char * const ar_ch, const WordData germanWords[10],
    VocabularyAndTranslation * p_vocandtransl);
  inline /*static*/ void InsertEnglishVerbWords(
//    std::set<LetterNode *> & std_setpletternodeLastStringChar,
//    std::set<std::string> & std_setVocableWords,
    const char * ar_ch,
    const WordData englishWords[10],
    enum EnglishWord::English_word_class english_word_class
    , const VocabularyAndTranslation * const p_vocandtranslAllocated);
  /*static*/ void InsertVerb(
    const char * ar_ch,
    const WordData englishWords[10], const WordData germanWords[10],
    enum EnglishWord::English_word_class english_word_class);
  /*static*/ void InsertTransitiveVerb(
    const char * ar_ch,
    const WordData englishWords[10], const WordData germanWords[10]);
  /*static*/ void InsertAndReferToExistingVocData(
//    std::set<LetterNode *> & std_setpletternodeLastStringChar,
//    std::set<std::string> & std_setVocableWords,
    const char * ar_ch,
    enum EnglishWord::English_word_class word_class,
//    unsigned vocAndTranslArrayIndex,
    const WordData & englishWord,
    const VocabularyAndTranslation * const p_vocandtranslAllocated
    );

  /*static*/ void ExtractVocables(
    const char * array,
    unsigned numChars,
//    const std::string &
    unsigned charIndex,
    const unsigned pipeCount,
    const unsigned semicolCountInsidePipeCharRangeFor1stWord,
    insertVocable pfn,
    TUchemnitzDictionaryReader & tuchemnitzdictionaryreader
    );

  /*inline static*/ std::istream::int_type UTF8toGermanASCII(
    std::ifstream & dictFile,
    char ar_ch[200] );
  /* inline static*/ std::istream::int_type UTF8toGermanASCII(
    char ar_ch[200] )
  {
    return UTF8toGermanASCII(m_dictFile, ar_ch);
  }

  void SetUserInterface(I_UserInterface * p)
  {
    s_p_i_userinterface = p;
  }
};

#endif /* TUCHEMNITZDICTIONARYREADER_HPP_ */
