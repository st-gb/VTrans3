/*
 * TUchemnitzDictEngWord2LineNumberReader.hpp
 *
 *  Created on: Sep 7, 2013
 *      Author: Stefan
 */

#ifndef TUCHEMNITZDICTENGWORD2LINENUMBERREADER_HPP_
#define TUCHEMNITZDICTENGWORD2LINENUMBERREADER_HPP_

#include <IO/dictionary/TUchemnitzDictionaryReader.hpp> //base class

namespace VocabularyAccess
{
  class ReadFileLineAndExtract;
}

namespace DictionaryReader
{
  /** A dictionary reader that creates an index : English word -> byte pos
   *   (offset) of dictionary record. */
  class TUchemnitzDictEngWord2LineNumberReader
    : public TUchemnitzDictionaryReader
  {
    static NodeTrie</*GermanWord::German_word_class*/
      EnglishWord::English_word_class>
      s_nodetrieWordKind2EngWordClassEnum;
    /*IVocabularyInMainMem*/ VocabularyAccess::ReadFileLineAndExtract & m_r_ivocabularyinmainmem;
  public:
    TUchemnitzDictEngWord2LineNumberReader(
      I_UserInterface & r_userInterface,
//      IVocabularyInMainMem *
      VocabularyAccess::ReadFileLineAndExtract *
      );
    virtual
    ~TUchemnitzDictEngWord2LineNumberReader();

    static void CreateWordKindString2wordKindEnumMapping();
    bool read(const char * dictionaryFilePath);
    void extractSingleEntry(const char * array,
      const unsigned numCharsForCurrentDictLine);
    void InsertEnglishWord(
        const char * const dictionaryLine,
        unsigned & englishVocableBeginCharIndex,
        unsigned englishWordLength,
        unsigned & charIndex,
        const NodeTrieNode</*GermanWord::German_word_class*/
          EnglishWord::English_word_class> * const p_ntnValidWordKind,
        const bool englishPart,
//        const unsigned numChars
        unsigned & englishWordIndex
        );
  };

} /* namespace DictionaryReader */
#endif /* TUCHEMNITZDICTENGWORD2LINENUMBERREADER_HPP_ */
