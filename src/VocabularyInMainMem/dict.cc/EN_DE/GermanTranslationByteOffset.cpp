/** File:   dict_cc_EN_DE.cpp
 * Author: sg
 * Created on 2. Januar 2018, 17:07  */

#include "GermanTranslationByteOffset.hpp"
#include <IO/dictionary/dict.cc/EN_DE/GermanTranslationByteOffset.hpp>

namespace VTrans3
{
  namespace VocabularyAccess
  {
    namespace dict_cc
    {
      namespace EN_DE {
GermanTranslationByteOffset::GermanTranslationByteOffset() {
}

GermanTranslationByteOffset::~GermanTranslationByteOffset() {
}

IVocabularyInMainMem::voc_container_type * GermanTranslationByteOffset::findEnglishWord(
  const VTrans::string_type & englishWord
  )
{
  return m_p_dictionaryReader->findEnglishWord(englishWord);
}

/*virtual void * */ std::set<VocabularyAndTranslation *> *
  GermanTranslationByteOffset::find(
  const PositionStringVector & psv,
//      const PositionstdstringVector & psv,
  DWORD & r_dwTokenIndex
  )
{
//  m_p_dictionaryReader->findEnglishWord(psv, r_dwTokenIndex);
  return FindEnglishWord(psv, r_dwTokenIndex);
}
  fastestUnsignedDataType GermanTranslationByteOffset::GetNumberOfAllocatedBytes() { 
    if(m_p_dictionaryReader)
      return m_p_dictionaryReader->GetNumberOfAllocatedBytes();
  }
      }
    }
  }
}
