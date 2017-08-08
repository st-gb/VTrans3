/** File:   BinarySearchInDictData.hpp
 * Author: sg
 * Created on 6. August 2017, 13:18  */

#ifndef BINARYSEARCHINDICTDATA_HPP
#define BINARYSEARCHINDICTDATA_HPP

#include <map> //class std::map
#include <IO/dictionary/DictionaryReaderBase.hpp> //class DictionaryReaderBase
#include <Attributes/PositionString.hpp>
#include <data_structures/Trie/NodeTrie/NodeTrie.hpp>
#include <set> //class std::set
#include <Attributes/EnglishWord.hpp> //enum EnglishWord::English_word_class
//IVocabularyInMainMem::voc_container_type
#include <VocabularyInMainMem/IVocabularyInMainMem.hpp>
/** for enum TUchemnitzDictionary::wordKinds */
#include <IO/dictionary/TUchemnitz/TUchemnitzDictionary.hpp>
//#include <FileSystem/File/File.hpp>
#include <FileSystem/File/native_File_type.hpp> //typedef native_File_type
#include <limits.h> //UINT_MAX

/** Minimal 6 characters are needed for an entry: <English word>::<German word>
 *  2 characters for "::", 2 characters for each English and German word.*/
#define NUM_MIN_CHARS_PER_VOC_ENTRY 6

/** Forward declaration(s) */
//class IVocabularyInMainMem;
class VocabularyAndTranslation;

namespace DictionaryReader
{
  namespace TUchemnitz
  {
    namespace EngWordSorted1st
    {
      /*inline*/ void HandleEndOfToken(
        char word [],
        fastestUnsignedDataType & charIndex,
        PositionStringVector & psvDictFile,
        fastestUnsignedDataType & tokenIndex
        );
      void HandleEndOfWord(
        char word [],
        fastestUnsignedDataType & charIndex,
        PositionStringVector & psvDictFile,
        fastestUnsignedDataType & tokenIndex,
        bool & compareVectors
  //      ,const char currentChar
        );
      
      /** This class is an interface/base class for implementations. */
class BinarySearchInDictData
  : public DictionaryReader::DictionaryReaderBase
{
  std::map<enum EnglishWord::English_word_class, unsigned> * 
    m_p_englishWordClass2CounterMap;
//      typedef void (TUchemnitzDictionaryReader::*insertVocable
//          )(//char *
//    //    const std::string &
//        const char * array,
//        const WordData englishWords[10], const WordData germanWords[10]);

  void AddAllOffsetsOfMatchingWords(
    const fastestUnsignedDataType closestBeforeNonMatchOffset,
    const PositionStringVector & psvStringToSearch,
//        const PositionStringVector & psvDictFile,
    DWORD & r_dwTokenIndex,
    const fastestUnsignedDataType numToken,
    std::set<fastestUnsignedDataType> & byteOffsetsOfVocData
    );
  virtual void AddGermanAttributes(
    std::map<unsigned, VocabularyAndTranslation *> & voc_containerVocsCreated,
    std::vector< std::vector <std::string> > & germanVocables);
//      VocabularyAndTranslation *
  /** @brief Needs to be virtual for overriding in a subclass. */
  virtual IVocabularyInMainMem::voc_container_type * AddVocable(
    const std::vector<std::string> & englishVocableWords,
    enum TUchemnitzDictionary::wordKinds wordKind,
//        enum EnglishWord::English_word_class word_class
    VocabularyAndTranslation *& p_vocabularyandtranslation
    );
  virtual PositionStringVector::cmp ContainsEnglishWord(
    const PositionStringVector & psvStringToSearch,
    DWORD & r_dwTokenIndex,
    const fastestUnsignedDataType numToken,
//        std::set<fastestUnsignedDataType> & byteOffsetsOfVocData,
    fastestUnsignedDataType & closestBeforeNonMatchOffset
    );
  void HandleSynonymSeparatorChar(
    const bool english,
    const unsigned pipeCount,
    const char word[100],
    //std::vector<std::vector<std::string> > englishVocables,
    std::vector<std::string> & englishVocables,
    std::vector< std::vector <std::string> > & germanVocables,
          unsigned & semicolCountInsideCurrentPipeCharRange,
          fastestUnsignedDataType & synonymIndex//,
  //        unsigned wordStart
          );
        enum TUchemnitzDictionary::wordKinds HandleClosingBrace(
          const fastestUnsignedDataType charIndex,
          fastestUnsignedDataType & kindOfWordStart,
          char wordKind[5]);
        bool
        CompareVectors(
          PositionStringVector::cmp & comp,
          const PositionStringVector & psvStringToSearch,
          fastestUnsignedDataType & hi,
          fastestUnsignedDataType & lo,
          bool & endSearchForCompareStringInCurrentVocData,
          const fastestUnsignedDataType numTokenForStringToSearch,
          PositionStringVector & psvDictFile,
          const DWORD & r_dwTokenIndex,
          fastestUnsignedDataType & byteOffset,
          fastestUnsignedDataType &,
          bool & breakWhile);

        /** static-> no need to (implicitly) pass an object pointer */
      //  static void read();
//        std::ifstream m_englishDictionary;
        IVocabularyInMainMem * m_p_vocabularyAccess;
protected:
        native_File_type m_dictFile;
          static NodeTrie<enum TUchemnitzDictionary::wordKinds> s_nodetrieWordKind;
public:
  BinarySearchInDictData(IVocabularyInMainMem & vocaccess)
      : DictionaryReaderBase(& vocaccess)  { addTrieNodes(); }
    
//  BinarySearchInDictData(const BinarySearchInDictData& orig);
  virtual ~BinarySearchInDictData();
  
        IVocabularyInMainMem::voc_container_type * extractVocable(
          const fastestUnsignedDataType,
          //IVocabularyInMainMem::voc_container_type & voc_type
          VocabularyAndTranslation * p_vocabularyandtranslation
          );
  /** Implementations must have the same semantics as reading from a file, i.e.
   *   the offset must be incremented. */
  virtual int GetNextByte() = 0;
        /** For files : set file pointer offset */
        virtual bool SetDictDataOffset(const fastestUnsignedDataType byteOffset) = 0;
        virtual int GetCurrentDictDataOffset() = 0;
  /** Implementations must have the same semantics as reading from a file, i.e.
   *   the offset must be heightened by numBytes. */
        virtual I_File::ReadResult ReadByteBuffer(
          uint8_t buffer [],
          fastestUnsignedDataType & numBytes) = 0;

        inline bool GetByteOffsetOfFirstVocable(
          fastestUnsignedDataType & byteOffsetOfVocable,
          bool & endSearchForCompareStringInCurrentVocData
          );
        fastestUnsignedDataType GetByteOffsetOfFirstMatchingWord(
          const PositionStringVector & psvStringToSearch,
          DWORD & r_dwTokenIndex,
          const fastestUnsignedDataType numToken,
          fastestUnsignedDataType closestBeforeNonMatchOffset,
          fastestUnsignedDataType byteOffsetOfVocable);
        /** @return 1 or multiple vocabulary pairs */
        void findEnglishWord(
          const PositionStringVector & psv,
          DWORD & r_dwTokenIndex,
          const fastestUnsignedDataType endTokenIndex,
          std::set<fastestUnsignedDataType> & byteOffsetsOfVocData);
        std::set<VocabularyAndTranslation *> * findEnglishWord(
          const PositionStringVector & psvStringToSearch,
          DWORD & r_dwTokenIndex//,
  //        const fastestUnsignedDataType endTokenIndex
          );

  void addTrieNodes();
        void GetStatistics(
//          fastestUnsignedDataType * Representations,
//          const fastestUnsignedDataType numArrayEles
          std::map<enum EnglishWord::English_word_class, unsigned> &
            englishWordClass2CounterMap
        );
        fastestUnsignedDataType GetByteOffsetOfNextVocDataBegin(bool &,
          /** Default value "UINT_MAX" leads to retrieve the current file
           *  pointer position. */
          fastestUnsignedDataType currentFileOffset = UINT_MAX);
private:

}; //BinarySearchInDictData
    }
  }
}

#endif /* BINARYSEARCHINDICTDATA_HPP */
