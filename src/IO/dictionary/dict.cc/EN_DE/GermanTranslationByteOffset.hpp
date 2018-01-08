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

namespace DictionaryReader { namespace dict_cc { namespace EN_DE {
class GermanTranslationByteOffset 
  : public DictionaryReader::DictionaryReaderBase
{
private:
  native_File_type m_dictionaryFile;
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
  
  fastestUnsignedDataType m_longestEnglishEntryInChars;
  typedef std::map<std::string, dict_cc_WordClasses::WordClasses> POSstring2POSenumContainerType;
  static POSstring2POSenumContainerType s_POSstring2POSenum;
public:
    GermanTranslationByteOffset(IVocabularyInMainMem * p_vocabularyInMainMem);
    virtual ~GermanTranslationByteOffset();

    void Build_POSstring2POSenum();
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
    std::string GetGermanTranslation(
        fastestUnsignedDataType & byteOffsetOfGermanPart);
    std::set<enum dict_cc_WordClasses::WordClasses> GetPartOfSpeeches(
        const fastestUnsignedDataType byteOffsetOfGermanPart);
    bool open(const std::string & std_strDictFilePath );
    void IndexByteOffsetOfGermanTranslations();
    void GetPOSstring(
        const fastestUnsignedDataType byteOffsetOfGermanPart, 
        std::string & POSstring);
    fastestUnsignedDataType GetNumberOfAllocatedBytes() { return 
      m_germanTranslationByteOffsetIndex.size() * sizeof(indexType); }
};
}}}

#endif /* DICT_CC_EN_DE_READER_HPP */

