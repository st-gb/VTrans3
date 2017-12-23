/** File:   BinarySearchInRAM.hpp
 * Author: sg
 * Created on 5. August 2017, 22:39  */

#ifndef BINARYSEARCHINRAM_HPP
#define BINARYSEARCHINRAM_HPP

#include "BinarySearchInDictData.hpp" //base class

namespace DictionaryReader
{
  namespace TUchemnitz
  {
  namespace EngWordSorted1st
  {
    namespace EachAttributeInSingleLine
    {
class BinarySearchInRAM
  : public DictionaryReader::TUchemnitz::EngWordSorted1st::
      EachAttributeInSingleLine::BinarySearchInDictData
{
  fastestUnsignedDataType m_dataOffset;
  uint8_t * m_dictionaryData;
public:
  BinarySearchInRAM(IVocabularyInMainMem & vocaccess)
    : BinarySearchInDictData(vocaccess)
     , m_dictionaryData(NULL)
  { 
      
  }
//  BinarySearchInRAM(const BinarySearchInRAM& orig);
  ~BinarySearchInRAM();
  
  bool open(const std::string & std_strDictFilePath );
  
  int GetNextByte() { return m_dictionaryData[m_dataOffset ++];}
  /** For files : set file pointer offset */
  bool SetDictDataOffset(const fastestUnsignedDataType byteOffset) {
    m_dataOffset = byteOffset;
    return true;
  }
  int GetCurrentDictDataOffset() { return m_dataOffset; }
  
  I_File::ReadResult ReadByteBuffer(
    uint8_t buffer [],
    const fastestUnsignedDataType & numBytesToRead,
	fastestUnsignedDataType & numBytesRead
	) {
    memcpy(buffer, m_dictionaryData + m_dataOffset, numBytesToRead);
	numBytesRead = numBytesToRead;
    m_dataOffset += numBytesToRead;
    return I_File::successfullyRead;
  }
  
private:
};
    }
  }
  }
}

#endif /* BINARYSEARCHINRAM_HPP */

