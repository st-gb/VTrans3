/** File:   BinarySearchInRAM.hpp
 * Author: sg
 * Created on 6. August 2017, 10:59  */

#ifndef VOCABULARYACCESS_BINARYSEARCHINRAM_HPP
#define VOCABULARYACCESS_BINARYSEARCHINRAM_HPP

#include <VocabularyInMainMem/IVocabularyInMainMem.hpp> //base class
#include <VocabularyInMainMem/CharStringStdMap/CharStringStdMap.hpp>
#include <IO/dictionary/TUchemnitz/EngWordSorted1st/eachAttributeInSingleLine/BinarySearchInRAM.hpp>

namespace VTrans3
{
  namespace VocabularyAccess
  {
    namespace TUchemnitzEngWordSorted1st
    {
class BinarySearchInRAM
  : public IVocabularyInMainMem
{
private:
      DictionaryReader::TUchemnitz::EngWordSorted1st::
        EachAttributeInSingleLine::BinarySearchInRAM * m_p_dictReader;
  /** Used for holding findamental (auxiliary verbs "be", "have", "will") 
    *  for tenses etc.) data for translation. */
   CharStringStdMap m_charStringStdMap;
        /** Used for holding attribute data for translation. */
        CharStringStdMap m_charStringStdMapTemp;
public:
  BinarySearchInRAM();
  BinarySearchInRAM(const BinarySearchInRAM& orig);
  virtual ~BinarySearchInRAM();
  void clearTemporaryEntries();
  
        /*virtual void * */ std::set<VocabularyAndTranslation *> * find(
          const PositionStringVector & psv,
      //      const PositionstdstringVector & psv,
          DWORD & r_dwTokenIndex
          );
        void GetStatistics(//fastestUnsignedDataType *, 
          //const fastestUnsignedDataType numArrayEles
          std::map<enum EnglishWord::English_word_class, unsigned> &
            englishWordClass2CounterMap
          );

        //TODO implement
        void clear() { };
        //TODO implement
        fastestUnsignedDataType GetNumberOfAllocatedBytes() { return 0;};
        void AddVocabularyAttributes(EnglishWord::English_word_class, void*) {};
        IVocabularyInMainMem::voc_container_type * Insert(const char*,
          int, EnglishWord::English_word_class, /*void* */
          VocabularyAndTranslation *& p_vocabularyandtranslation);
        void Insert(EnglishWord & ew , GermanWord & gw, //void * p_v
          VocabularyAndTranslation * p_vocabularyandtranslation
          ) {} ;
        void SetDictionaryReader(
          DictionaryReader::DictionaryReaderBase * p_dictionaryReader)
        {
          m_p_dictionaryReader = p_dictionaryReader;
          m_p_dictReader =
            (DictionaryReader::TUchemnitz::EngWordSorted1st::
            EachAttributeInSingleLine::BinarySearchInRAM *) p_dictionaryReader;
        }

};
    }
  }
}

#endif /* VOCABULARYACCESS_BINARYSEARCHINRAM_HPP */
