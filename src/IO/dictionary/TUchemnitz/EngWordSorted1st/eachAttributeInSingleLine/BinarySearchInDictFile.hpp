/** TUchemnitzDictEngWord1stReader.hpp
 *  Created on: Dec, 2013
 *      Author: Stefan   */

#ifndef TUCHEMNITZDICTENGWORD1STREADER_HPP_
#define TUCHEMNITZDICTENGWORD1STREADER_HPP_

//#include <fstream> //class std::ifstream
#include "BinarySearchInDictData.hpp" //base class

namespace DictionaryReader
{
  namespace TUchemnitz
  {
    namespace EngWordSorted1st
    {
    namespace EachAttributeInSingleLine
    {
      /** For a TU Chemnitz formatted dictionary where the lines begin with
       *  English words. So the words are sorted (and a binary search may be
       *  applied->very fast search) as needed for English->German translation.
       *
       *  This reader especially exists for low memory devices such as Android
       *  smartphones: it should be combined with a vocabulary access class
       *  that only holds temporary vocabulary information in memory. */
      class BinarySearchInDictFile
        : public BinarySearchInDictData
      {
      protected:
        int GetCurrentDictDataOffset();
      public:
        BinarySearchInDictFile(IVocabularyInMainMem &);
        virtual
        ~BinarySearchInDictFile();

        I_File::CloseError close() { return m_dictFile.Close(); }
        bool open(const std::string & std_str);
        fastestUnsignedDataType GetFileSizeInBytes() const 
          { return m_fileSizeInBytes; }
        void read();
        int GetNextByte();
        I_File::ReadResult ReadByteBuffer(
          uint8_t buffer [],
		  const fastestUnsignedDataType & numBytesToRead,
		  fastestUnsignedDataType & numBytesRead
		  );
        bool SetDictDataOffset(const fastestUnsignedDataType byteOffset);
        void GetCollectDictionaryStatisticsStatus(fastestUnsignedDataType & itemNo)
        {
          itemNo = GetCurrentDictDataOffset();
        }
      };
    }
    }
  }
} /* namespace DictionaryReader */
#endif /* TUCHEMNITZDICTENGWORD1STREADER_HPP_ */
