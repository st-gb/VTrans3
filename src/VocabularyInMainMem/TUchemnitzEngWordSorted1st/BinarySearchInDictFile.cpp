/*
 * BinarySearchInDictFile.cpp
 *
 *  Created on: 11.12.2013
 *      Author: mr.sys
 */

#include <VocabularyInMainMem/TUchemnitzEngWordSorted1st/BinarySearchInDictFile.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h>
#include <UserInterface/I_UserInterface.hpp>

namespace VTrans3
{
  namespace VocabularyAccess
  {
    namespace TUchemnitzEngWordSorted1st
    {
      BinarySearchInDictFile::BinarySearchInDictFile()
//        : m_r_dictReader(* this)
      {
        // TODO Auto-generated constructor stub
      }

      BinarySearchInDictFile::~BinarySearchInDictFile()
      {
        LOGN_DEBUG("end")
      }

      void BinarySearchInDictFile::clearTemporaryEntries() {
        LOGN_DEBUG("before emptying the temporary words map")
        m_charStringStdMapTemp.clear();
      }
      
      void BinarySearchInDictFile::clear()
      {
        LOGN_DEBUG("before emptying the fundamental words map")
        m_charStringStdMap.clear();
        LOGN_DEBUG("before emptying the temporary words map")
        m_charStringStdMapTemp.clear();
        const I_File::CloseError closeResult = m_p_dictReader->close();
        if( closeResult == I_File::closingFileFailed )
          LOGN_ERROR("closing dict file \"" << m_p_dictReader->GetFilePath() 
            << "\" failed.")
        else
          LOGN_DEBUG("closing dict file \"" << m_p_dictReader->GetFilePath() 
            << "\" succeeded.")
      }
      
      /*virtual void * */ std::set<VocabularyAndTranslation *> *
        BinarySearchInDictFile::find(
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
          p_voc_container = m_charStringStdMapTemp.find(psv, r_dwTokenIndex);
          if( p_voc_container )
          {
            LOGN_SUCCESS("word found in temporary words map")
            return p_voc_container;
          }
          else
     //        m_dictReader.findEnglishWord(psv, r_dwTokenIndex);
            return m_p_dictReader->findEnglishWord(psv, r_dwTokenIndex);
        }
      }

      unsigned BinarySearchInDictFile::GetNumberOfEnglishWords()
      {
        const fastestUnsignedDataType mapSize = m_charStringStdMap.
          GetNumberOfEnglishWords();
        const fastestUnsignedDataType tempMapSize = m_charStringStdMapTemp.
          GetNumberOfEnglishWords();
        return mapSize + tempMapSize;
      }

      void BinarySearchInDictFile::GetCollectDictionaryStatisticsStatus(
        fastestUnsignedDataType & currentItemNo)
      {
        m_p_dictReader->GetCollectDictionaryStatisticsStatus(currentItemNo);
      }
      
      void BinarySearchInDictFile::GetStatistics(
//        fastestUnsignedDataType * Representations,
//        const fastestUnsignedDataType numArrayEles
        std::map<enum EnglishWord::English_word_class, unsigned> &
          englishWordClass2CounterMap
        )
      {
        const fastestUnsignedDataType fileSizeInBytes = 
          m_p_dictReader->GetFileSizeInBytes();
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

      /** For inserting fundamental words like "the" etc. */
//      VocabularyAndTranslation*
      IVocabularyInMainMem::voc_container_type * BinarySearchInDictFile::Insert(
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
    }
  }

} /* namespace VTrans3 */
