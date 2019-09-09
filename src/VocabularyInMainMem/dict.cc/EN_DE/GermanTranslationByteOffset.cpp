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

/** @return : must be deleted by the caller of this function. */
/*virtual void * */ std::set<VocabularyAndTranslation *> *
  GermanTranslationByteOffset::find(
  const PositionStringVector & psv,
//      const PositionstdstringVector & psv,
  DWORD & r_dwTokenIndex
  )
{
  voc_container_type * p_vocContainerRAM = m_charStringStdMap.FindEnglishWord(
    psv, r_dwTokenIndex);
//  m_p_dictionaryReader->findEnglishWord(psv, r_dwTokenIndex);
  voc_container_type * p_vocContainerFile = FindEnglishWord(psv, r_dwTokenIndex);
  /** E.g. "had" for "hatte" in RAM and "had" for "gehabt" in file. */
  if( p_vocContainerRAM)
  {
    if(p_vocContainerFile)
    { /** Both contain vocables -> put them together  */
      voc_container_type * p_vocContainer = new voc_container_type();
      for( voc_container_type::const_iterator iter = p_vocContainerRAM->begin(); 
          iter != p_vocContainerRAM->end(); iter ++ )
      {
        p_vocContainer->insert(*iter);
      }
      for( voc_container_type::const_iterator iter = p_vocContainerFile->begin(); 
          iter != p_vocContainerFile->end(); iter ++ )
      {
        p_vocContainer->insert(*iter);
      }
      return p_vocContainer;
    }
    else /** p_vocContainerFile == NULL */
      return p_vocContainerRAM;
  }
  return p_vocContainerFile;
}

void GermanTranslationByteOffset::GetCollectDictionaryStatisticsStatus(
  fastestUnsignedDataType & currentItemNo)
{
  m_p_dictionaryReader->GetCollectDictionaryStatisticsStatus(currentItemNo);
}

void GermanTranslationByteOffset::GetStatistics(
  std::map<enum EnglishWord::English_word_class, unsigned> &
    englishWordClass2CounterMap
  )
{
  m_p_dictionaryReader->GetStatistics(englishWordClass2CounterMap);
}

  fastestUnsignedDataType GermanTranslationByteOffset::GetNumberOfAllocatedBytes() { 
    if(m_p_dictionaryReader)
      return m_p_dictionaryReader->GetNumberOfAllocatedBytes();
    return 0;
  }
  
  IVocabularyInMainMem::voc_container_type * GermanTranslationByteOffset::Insert(
    const char * wordBegin, int numChars, //void * p_v
    enum EnglishWord::English_word_class word_class, /*void *&*/
    VocabularyAndTranslation *& p_vocabularyandtranslation)
  {
    if( word_class >
      /** Last dictionary word class. */
      EnglishWord::/*pronoun*/ main_verb_past )
    {
//      LOGN_DEBUG("inserting into fundamental words map")
      VocablesForWord::voc_container_type * p_voc_container = NULL;
      p_voc_container =
        m_charStringStdMap.InsertAsKeyAndAddVocabularyAttributes(
        wordBegin
        , (int &) numChars
        , word_class,
        /*pv*/ p_vocabularyandtranslation
        );
    }
    return NULL;
  }
      }
    }
  }
}
