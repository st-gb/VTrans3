/*
 * TUchemnitzDictEngWord1stReader.hpp
 *
 *  Created on: Dec, 2013
 *      Author: Stefan
 */

#ifndef TUCHEMNITZDICTENGWORD1STREADER_HPP_
#define TUCHEMNITZDICTENGWORD1STREADER_HPP_

#include <IO/dictionary/DictionaryReaderBase.hpp> //class DictionaryReaderBase
#include <fstream> //class std::ifstream
#include <Attributes/PositionString.hpp>
#include <data_structures/Trie/NodeTrie/NodeTrie.hpp>
#include <set> //class std::set
#include <map> //class std::map
#include <Attributes/EnglishWord.hpp> //enum EnglishWord::English_word_class
//IVocabularyInMainMem::voc_container_type
#include <VocabularyInMainMem/IVocabularyInMainMem.hpp>
/** for enum TUchemnitzDictionary::wordKinds */
#include <IO/dictionary/TUchemnitz/TUchemnitzDictionary.hpp>
//#include <FileSystem/File/File.hpp>
#include <FileSystem/File/native_File_type.hpp> //typedef native_File_type

/** Forward decl. */
//class IVocabularyInMainMem;
class VocabularyAndTranslation;

namespace DictionaryReader
{
  namespace TUchemnitz
  {
    namespace EngWordSorted1st
    {
      /** For a TU Chemnitz formatted dictionary where the lines begin with
       *  English words. So the words are sorted (and a binary search may be
       *  applied->very fast search) as needed for English->German translation.
       *
       *  This reader especially exists for low memory devices such as Android
       *  smartphones: it should be combined with a vocabulary access class
       *  that only holds temporary vocabulary information in memory. */
      class BinarySearchInDictFile
        : public DictionaryReader::DictionaryReaderBase
      {
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
        PositionStringVector::cmp ContainsEnglishWord(
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
        native_File_type m_dictFile;
        IVocabularyInMainMem * m_p_vocabularyAccess;
      protected:
        static NodeTrie<enum TUchemnitzDictionary::wordKinds> s_nodetrieWordKind;
      public:
        BinarySearchInDictFile(IVocabularyInMainMem &);
        virtual
        ~BinarySearchInDictFile();
        void addTrieNodes();
        IVocabularyInMainMem::voc_container_type * extractVocable(
          const fastestUnsignedDataType,
          //IVocabularyInMainMem::voc_container_type & voc_type
          VocabularyAndTranslation * p_vocabularyandtranslation
          );
        I_File::CloseError close() { return m_dictFile.Close(); }
        bool open(const std::string & std_str);
        void read();
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
        fastestUnsignedDataType GetByteOffsetOfVocDataBegin(bool &);
      };
    }
  }
} /* namespace DictionaryReader */
#endif /* TUCHEMNITZDICTENGWORD1STREADER_HPP_ */
