/*
 * BinarySearchInDictFile.cpp
 *
 *  Created on: 11.12.2013
 *      Author: mr.sys
 */

#include <VocabularyInMainMem/TUchemnitzEngWordSorted1st/BinarySearchInDictFile.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h>

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

      void BinarySearchInDictFile::clear()
      {
        LOGN_DEBUG("before emptying the fundamental words map")
        m_charStringStdMap.clear();
        LOGN_DEBUG("before emptying the temporary words map")
        m_charStringStdMapTemp.clear();
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
            return p_voc_container;
          else
     //        m_dictReader.findEnglishWord(psv, r_dwTokenIndex);
            return m_p_dictReader->findEnglishWord(psv, r_dwTokenIndex);
        }
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
        LOGN_DEBUG("begin " << wordBegin << " " << word_class << " "
            << p_vocabularyandtranslation)
        VocablesForWord::voc_container_type * p_voc_container = NULL;
        if( word_class > EnglishWord::pronoun ) //Last non-dictionary word class.
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
