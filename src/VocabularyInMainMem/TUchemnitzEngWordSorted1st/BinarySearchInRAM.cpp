/** File:   BinarySearchInRAM.cpp
 * Author: sg
 * Created on 6. August 2017, 10:59  */

#include "BinarySearchInRAM.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h>

namespace VTrans3
{
  namespace VocabularyAccess
  {
    namespace TUchemnitzEngWordSorted1st
    {
BinarySearchInRAM::BinarySearchInRAM() {
}

BinarySearchInRAM::BinarySearchInRAM(const BinarySearchInRAM& orig) {
}

BinarySearchInRAM::~BinarySearchInRAM() {
}

      /*virtual void * */ std::set<VocabularyAndTranslation *> *
        BinarySearchInRAM::find(
        const PositionStringVector & psv,
    //      const PositionstdstringVector & psv,
        DWORD & r_dwTokenIndex
        )
      {
        VocablesForWord::voc_container_type * p_voc_container =
          m_charStringStdMap.find(psv, r_dwTokenIndex);
        if( p_voc_container )
        {
          LOGN_SUCCESS("word found in fundamental words map")
    //      VocablesForWord::voc_container_type * p_voc_container =
    //        m_CharStringStdMap.find(psv, r_dwTokenIndex);
    //      if( p_voc_container)
    //      {
//            numVocs = p_voc_container->size();
            return p_voc_container;
    //      }
        }
        else
        {
     //        m_dictReader.findEnglishWord(psv, r_dwTokenIndex);
          //TODO
          return m_p_dictReader->findEnglishWord(psv, r_dwTokenIndex);
        }
      }

      void BinarySearchInRAM::clearTemporaryEntries() {
        LOGN_DEBUG("before emptying the temporary words map")
        m_charStringStdMapTemp.clear();
      }
      
      void BinarySearchInRAM::GetStatistics(
//        fastestUnsignedDataType * Representations,
//        const fastestUnsignedDataType numArrayEles
        std::map<enum EnglishWord::English_word_class, unsigned> &
          englishWordClass2CounterMap
        )
      {
//        const fastestUnsignedDataType fileSizeInBytes = 
//          m_p_dictReader->GetFileSizeInBytes();
//
//        char wordKind [9];
//        int numWordKindChars = 3;        
//        s_nodetrieWordKind.contains_inline((BYTE*) ((wordKind)),
//                numWordKindChars /*- 1*/, true);

        m_p_dictReader->GetStatistics(//Representations, numArrayEles
          englishWordClass2CounterMap);
        
//        fastestUnsignedDataType fileSizeInBytes, numberOfBytesCurrentlyProcessed;
//        mp_userinterface->UpdateDictionaryStatistics(fileSizeInBytes, 
//          numberOfBytesCurrentlyProcessed);
      }
                                
      IVocabularyInMainMem::voc_container_type * BinarySearchInRAM::Insert(
        const char* wordBegin,
        int numChars,
        EnglishWord::English_word_class word_class,
//        void * pv
        VocabularyAndTranslation *& p_vocabularyandtranslation
        )
      {
        LOGN_DEBUG("begin " << wordBegin << " word class:" << word_class
            << " voc&transl ptr:" << p_vocabularyandtranslation)
        VocablesForWord::voc_container_type * p_voc_container = NULL;
        if( word_class >
          /** Last dictionary word class. */
          EnglishWord::/*pronoun*/ main_verb_past )
        {
          LOGN_DEBUG("inserting into fundamental words map")
          p_voc_container =
            m_charStringStdMap.InsertAsKeyAndAddVocabularyAttributes(
            wordBegin
            , (int &) numChars
            , word_class,
            /*pv*/ p_vocabularyandtranslation
            );
        }
        else
        {
          LOGN_DEBUG("inserting into temporary map")
          p_voc_container = m_charStringStdMapTemp.
            InsertAsKeyAndAddVocabularyAttributes(
            wordBegin
            , (int &) numChars
            , word_class,
            p_vocabularyandtranslation
            );
        }
        LOGN_DEBUG("return " << p_voc_container)
        return p_voc_container;
      }

std::/*ostream &*/string BinarySearchInRAM::getFundamentalWords(
  /*std::ostream& os*/ /*std::string & str*/)
{
  std::ostringstream os;
  Word wordEnglish, wordGerman;
  wordEnglish.p_iter = NULL;
  std::string searchStr("");
  m_charStringStdMap.GetNextOccurance(searchStr, & wordEnglish, & wordGerman);
  for( ; wordEnglish.p_iter;)
  {
    std::string str(( char *) wordEnglish.p_word, wordEnglish.numBytes);
    os << str << "*" << m_charStringStdMap.m_iter->second.
      m_std_set_p_vocabularyandtranslation.size() << " ";
    m_charStringStdMap.GetNextOccurance(searchStr, & wordEnglish, & wordGerman);
  }
  std::string str = os.str();
  return /*os*/str;
}
    }
  }
}
