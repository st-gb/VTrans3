/** File:   Original_dict_cc_file_formatAndGermanTranslationByteOffset.hpp
 * Author: sg
 * Created on 3. Januar 2018, 20:35  */

#ifndef ORIGINAL_DICT_CC_FILE_FORMATANDGERMANTRANSLATIONBYTEOFFSET_HPP
#define ORIGINAL_DICT_CC_FILE_FORMATANDGERMANTRANSLATIONBYTEOFFSET_HPP

#include <IO/dictionary/dict.cc/EN_DE/GermanTranslationByteOffset.hpp>
#include <VocabularyInMainMem/dict.cc/EN_DE/GermanTranslationByteOffset.hpp>
#include <Attributes/PositionString.hpp>

namespace VTrans3
{
class Original_dict_cc_EN_DE_AndGermanTranslationByteOffset
{
public:
  VTrans3::VocabularyAccess::dict_cc::EN_DE::GermanTranslationByteOffset m_vocAccess;
  DictionaryReader::dict_cc::EN_DE::GermanTranslationByteOffset m_dictReader;
  Original_dict_cc_EN_DE_AndGermanTranslationByteOffset()
    : m_dictReader(& m_vocAccess)
  {
    m_vocAccess.SetDictionaryReader(& m_dictReader);
  }
//  TUchemnitzEngWordSorted1stAndBinarySearchInRAM(const TUchemnitzEngWordSorted1stAndBinarySearchInRAM& orig);
  virtual ~Original_dict_cc_EN_DE_AndGermanTranslationByteOffset() {}
  
  IVocabularyInMainMem & GetVocAccess() { return m_vocAccess; }
  bool loadDictionary(const std::string & std_strDictionaryFilePath)
  {
    return m_dictReader.open(std_strDictionaryFilePath);
  }
  IVocabularyInMainMem::voc_container_type * lookUpEnglishWord(
    PositionStringVector & c_r_positionStringVector,
    DWORD & dwTokenIndexRightMost)
  {
    return m_vocAccess.find(c_r_positionStringVector, dwTokenIndexRightMost);
  }
private:
  };
} /* namespace VTrans3 */

#endif /* ORIGINAL_DICT_CC_FILE_FORMATANDGERMANTRANSLATIONBYTEOFFSET_HPP */

