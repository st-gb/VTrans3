/*
 * BinarySearchInDictFile.hpp
 *
 *  Created on: 11.12.2013
 *      Author: mr.sys
 */

#ifndef BINARYSEARCHINDICTFILE_HPP_
#define BINARYSEARCHINDICTFILE_HPP_

#include <VocabularyInMainMem/IVocabularyInMainMem.hpp>
#include <IO/dictionary/TUchemnitz/EngWordSorted1st/BinarySearchInDictFile.hpp>
//#include <map> //class std::map
//#include <VocabularyInMainMem/VocablesForWord.hpp> //class VocablesForWord
#include <VocabularyInMainMem/CharStringStdMap/CharStringStdMap.hpp>

//class DictionaryReader::TUchemnitzEngWordSorted1st::BinarySearchInDictFile;

namespace VTrans3
{
  namespace VocabularyAccess
  {
    namespace TUchemnitzEngWordSorted1st
    {
      /** Searches in the vocabulary container first and if not found it
       * searches via the dictionary reader. */
      class BinarySearchInDictFile
        : public IVocabularyInMainMem
      {
        CharStringStdMap m_charStringStdMap;
        //Used for holding attribute data for translation.
        CharStringStdMap m_charStringStdMapTemp;
        typedef std::map<std::string, VocablesForWord> map_type;
//        map_type m_charStringMap;
//        DictionaryReader::TUchemnitzEngWordSorted1st::BinarySearchInDictFile & m_r_dictReader;
        DictionaryReader::TUchemnitz::EngWordSorted1st::BinarySearchInDictFile * m_p_dictReader;
      public:
        BinarySearchInDictFile();
        virtual
        ~BinarySearchInDictFile();

        /*virtual void * */ std::set<VocabularyAndTranslation *> * find(
          const PositionStringVector & psv,
      //      const PositionstdstringVector & psv,
          DWORD & r_dwTokenIndex
          );
        virtual void
          //void *
          Insert(EnglishWord & ew , GermanWord & gw, //void * p_v
            VocabularyAndTranslation * p_vocabularyandtranslation
            ) {} ;
        void AddVocabularyAttributes(EnglishWord::English_word_class, void*) {};
        void clear();
        fastestUnsignedDataType GetNumberOfAllocatedBytes() { return 0; }
        unsigned GetNumberOfEnglishWords();
        void GetCollectDictionaryStatisticsStatus(fastestUnsignedDataType & currentItemNo);
        void GetStatistics(//fastestUnsignedDataType *, 
          //const fastestUnsignedDataType numArrayEles
          std::map<enum EnglishWord::English_word_class, unsigned> &
            englishWordClass2CounterMap
          );
        /** For inserting fundamental words like "the" etc. */
//        VocabularyAndTranslation*
        IVocabularyInMainMem::voc_container_type * Insert(const char*,
          int, EnglishWord::English_word_class, /*void* */
          VocabularyAndTranslation *& p_vocabularyandtranslation);
//        void SetDictionaryReader(DictionaryReader::TUchemnitzEngWordSorted1st::BinarySearchInDictFile & m_r_dictReader)
//        {
//        }
        void SetDictionaryReader(
          DictionaryReader::DictionaryReaderBase * p_dictionaryReader)
        {
          m_p_dictionaryReader = p_dictionaryReader;
          m_p_dictReader =
            (DictionaryReader::TUchemnitz::EngWordSorted1st::BinarySearchInDictFile *) p_dictionaryReader;
        }
      };
    }
  }
} /* namespace VTrans3 */

#endif /* BINARYSEARCHINDICTFILE_HPP_ */
