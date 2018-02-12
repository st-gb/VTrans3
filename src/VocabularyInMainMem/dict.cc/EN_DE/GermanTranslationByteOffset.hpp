/** File:   dict_cc_EN_DE.hpp
 * Author: sg
 * Created on 2. Januar 2018, 17:07  */

#ifndef DICT_CC_EN_DE_HPP
#define DICT_CC_EN_DE_HPP

#include <VocabularyInMainMem/IVocabularyInMainMem.hpp> //base class
#include <VocabularyInMainMem/CharStringStdMap/CharStringStdMap.hpp>

/** Forward declarations: */
namespace DictionaryReader { namespace dict_cc { namespace EN_DE {
  class GermanTranslationByteOffset; } } }

namespace VTrans3
{
  namespace VocabularyAccess
  {
    namespace dict_cc
    {
        namespace EN_DE {
class GermanTranslationByteOffset
  : public IVocabularyInMainMem
{
  CharStringStdMap m_charStringStdMap;
public:
    GermanTranslationByteOffset();
    virtual ~GermanTranslationByteOffset();
    
  /*void * */ std::set<VocabularyAndTranslation *> * find(
      const PositionStringVector & psv,
//      const PositionstdstringVector & psv,
      DWORD & r_dwTokenIndex
      );
  void AddVocabularyAttributes(
    enum EnglishWord::English_word_class word_class,
    void * p_v) {}
  void clear() {}
  fastestUnsignedDataType GetNumberOfAllocatedBytes();
  voc_container_type * findEnglishWord(
    const VTrans::string_type & englishWord);
  void GetStatistics(
//    fastestUnsignedDataType wordClass[], 
//    const fastestUnsignedDataType numArrayEles
    std::map<enum EnglishWord::English_word_class, unsigned> &
      englishWordClass2CounterMap
    ) {}
    void /* void * */
      Insert(EnglishWord & ew , GermanWord & gw, //void * p_v
        VocabularyAndTranslation * p_vocabularyandtranslation
        ) {}
    /** Is called for inserting fundamental words etc. */
    IVocabularyInMainMem::voc_container_type * Insert(
      const char * wordBegin, int numChars, //void * p_v
      enum EnglishWord::English_word_class, /*void *&*/
      VocabularyAndTranslation *& p_vocabularyandtranslation);
    void SetDictionaryReader(
      DictionaryReader::dict_cc::EN_DE::GermanTranslationByteOffset * 
        p_dictionaryReader)
    {
        m_p_dictionaryReader = p_dictionaryReader;
    }
private:
  DictionaryReader::dict_cc::EN_DE::GermanTranslationByteOffset * m_p_dictionaryReader;
};
        }
    }
  }
}

#endif /* DICT_CC_EN_DE_HPP */
