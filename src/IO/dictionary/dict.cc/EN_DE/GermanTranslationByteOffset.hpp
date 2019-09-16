/** File:   dict_cc_EN_DE_reader.hpp
 * Author: sg
 * Created on 2. Januar 2018, 14:44  */

#ifndef DICT_CC_EN_DE_READER_HPP
#define DICT_CC_EN_DE_READER_HPP

#include <IO/dictionary/DictionaryReaderBase.hpp>
#include <set> //class std::map
#include <VocabularyInMainMem/VocabularyAndTranslation.hpp>
#include <Attributes/PositionString.hpp>
#include <FileSystem/File/native_File_type.hpp> //typedef native_File_type
#include <map>//class std::multimap
#include <VocabularyInMainMem/VocablesForWord.hpp>

#include "dict_cc_WordClasses.hpp"

/** Forward declarations: */
class IVocabularyInMainMem;

#define DICT_CC_LINE_END_CHAR 0xA
///A tab character separates the 3 parts (English, German, part of speech)
#define DICT_CC_PART_SEPARATOR_CHAR '\t'

namespace DictionaryReader { namespace dict_cc { namespace EN_DE {
class GermanTranslationByteOffset 
  : public DictionaryReader::DictionaryReaderBase
{
private:
  native_File_type m_dictionaryFile;
  unsigned numTabsInCurrentLine;
  unsigned currentByteOffset;
  unsigned numLines;
  signed fileSizeInBytes;
  std::string m_germanPart;

  ///An attribute is either
  ///-the English part
  ///-the German part
  ///-the part of speech
  std::string currentAttribute;/** Attributes are separated by a tabulator.*/
  typedef uint32_t indexType;
  /** A dict.cc dictionary has around ~1 M lines -> a 32 bit type (max. ~ 4 G 
   *  lines) is sufficient. 
   *  Use uint32_t rather than "unsigned" because the width of "unsigned" 
   *  depends on CPU architecture.
   *  "multimap" because there may be homographs, i.e. multiple words that are 
   *  equally written, e.g. "love" with "the love" and "to love" */
  //TODO the datatype could be changed to a (1 byte-aligned) 3 byte struct in 
  // order to save some more space.
  typedef std::multimap<std::string, indexType> container_type; 
  container_type m_germanTranslationByteOffsetIndex;
  
  typedef void (GermanTranslationByteOffset::*pfnTabCharType)();
  //typedef pfnTabCharType pfnEndOfLineType;
  typedef void (GermanTranslationByteOffset::*pfnEndOfLineType)(void *);
  ///If looking for a word in the source text this is the largest number of
  /// characters all tokens and space characters in between need to be searched.
  fastestUnsignedDataType m_longestEnglishEntryInChars;
  typedef std::map<std::string, enum dict_cc_WordClasses::WordClasses> 
    POSstring2POSenumContainerType;
  static POSstring2POSenumContainerType s_POSstring2POSenum;
public:
  ///Sample entry: "man [male]	Mann {m}	noun"
  enum parts {EnglishPart = 0, GermanPart, partOfSpeech};
    GermanTranslationByteOffset(IVocabularyInMainMem * p_vocabularyInMainMem);
    virtual ~GermanTranslationByteOffset();

  static void AddMapping(
    const char * const word,
    const enum dict_cc_WordClasses::WordClasses wordClasses)
  {
    s_POSstring2POSenum.insert(std::make_pair(word, wordClasses) );
  }
  static void FillPOSstring2dict_ccPOSenum();
    VocablesForWord::voc_container_type * findEnglishWord(
        const std::string & englishWord);
    std::set<VocabularyAndTranslation *> * findEnglishWord(
        const PositionStringVector & psvStringToSearch,
        DWORD & r_dwTokenIndex//,
//        const fastestUnsignedDataType endTokenIndex
        );
    void findRegardingLongestEnglishEntryInFile(
        const PositionStringVector & psvStringToSearch,
        DWORD & r_dwTokenIndex//,
        );
//    void findRegardingMaxTokenToConsider(
//        const PositionStringVector & psvStringToSearch,
//        DWORD & r_dwTokenIndex//,
//        );
  void GetCollectDictionaryStatisticsStatus(
    fastestUnsignedDataType & currentItemNo);
  void GetStatistics(
    std::map<enum EnglishWord::English_word_class, unsigned> & );
  ///Callback functions for indexing the vocabulary by the English word.
  void IdxGerTranslsByteOffsLineEnd(void *);
  void IdxGerTranslsByteOffsTabChar(
    /*const fastestUnsignedDataType numTabsInCurrentLine*/);
  ///Callback functions for dictionary statistics.
  void GetStatsLineEnd(void *);
  void GetStatsTabChar();
  
  void getGermanAndPOSpart(
    const fastestUnsignedDataType byteOffsetOfGermanPart,
    std::string & germanPart, 
    std::string & POSpart);
  std::string GetGermanTranslationColumnContent(
    const fastestUnsignedDataType & byteOffsetOfGermanPart);
//  std::set<enum dict_cc_WordClasses::WordClasses> 
  std::set<EnglishWord::English_word_class>
    GetPartOfSpeeches(
    const std::string & POSstring, const std::string & germanPart);
//  std::set<enum dict_cc_WordClasses::WordClasses>
  std::set<EnglishWord::English_word_class>
    GetPartOfSpeeches(const fastestUnsignedDataType byteOffsetOfGermanPart);
    bool open(const std::string & std_strDictFilePath );
    void readWholeFile(pfnTabCharType, pfnEndOfLineType,
	  void * endOfLineFnParam,
	  const fastestUnsignedDataType attrIdx);
  void GetPOSstring(std::string & POSstring);
  void GetPOSstring(
    const fastestUnsignedDataType byteOffsetOfGermanPart, 
    std::string & POSstring);
    fastestUnsignedDataType GetNumberOfAllocatedBytes() { return 
      m_germanTranslationByteOffsetIndex.size() * sizeof(indexType); }
};
}}}

#endif /* DICT_CC_EN_DE_READER_HPP */
