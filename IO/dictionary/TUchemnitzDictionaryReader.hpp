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

//fwd decl.
class LetterTree;
class I_UserInterface;
class EnglishWord;
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
  {}
  bool BeginIsNotSet() { return m_charIndexOfBegin == isNotSet; }
  bool EndIsNotSet() { return m_charIndexOfEnd == isNotSet; }
  bool BeginIsSet() { return ! BeginIsNotSet(); }
  bool EndIsSet() { return ! EndIsNotSet(); }
  void Reset()
  {
    m_charIndexOfBegin = isNotSet;
    m_charIndexOfEnd = isNotSet;
  }
};

class TUchemnitzDictionaryReader
{
  unsigned m_dictionaryFileLineNumber;
  typedef void (* extractVocable )(//char *
//    const std::string &
    const char * array, unsigned numChars, unsigned charIndex,
    const unsigned pipeCount );
  typedef void (* insertVocable )(//char *
//    const std::string &
    const char * array,
    const WordData englishWords[10], const WordData germanWords[10]);
public:
  TUchemnitzDictionaryReader(I_UserInterface &);

  /** static-> no need to (implicitly) pass an object pointer */
  static void read();
  static NodeTrie<insertVocable> s_nodetrieWordKind;
  static LetterTree * s_p_lettertree;
  static I_UserInterface * s_p_i_userinterface;

  static void extractSingleEntry(const char * array, unsigned numChars
    //const std::string &
    );
  static void extractVocables(const char * filePath);

  static void Insert1stEnglishWord(
//    const std::string & strCurrentWordData,
    const char * array,
    unsigned firstWordChar,
    unsigned nIndexOfCurrentChar,
    enum EnglishWord::English_word_class word_class);

  static void Insert1stGermanWord(
//    const std::string & strCurrentWordData,
    const char * array,
    unsigned nCharIndexOf1stWordChar,
    unsigned nIndexOfCurrentChar);

  static void InsertAdverb(
    const char * array,
    const WordData englishWords[10], const WordData germanWords[10]);
  static void InsertAdverb(//const char * array
    const char * ar_ch, unsigned numChars,
    unsigned charIndex, const unsigned germanPipeCount);

  static void InsertAdjective(
//    const std::string & strCurrentWordData,
    const char * array,
    const WordData englishWords[10], const WordData germanWords[10]);
  static void ExtractVocables(const char * array, unsigned numChars,
//    const std::string &
    unsigned charIndex, const unsigned pipeCount,
    insertVocable pfn);

  inline static std::istream::int_type UTF8toGermanASCII(
    std::ifstream & dictFile,
    char ar_ch[200] );
};

#endif /* TUCHEMNITZDICTIONARYREADER_HPP_ */
