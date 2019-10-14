/** File:   BinarySearchInRAM.cpp
 * Author: sg
 * Created on 5. August 2017, 22:39  */

#include "BinarySearchInRAM.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_DEBUG(...)
#include <OperatingSystem/GetLastErrorCode.hpp>
#include <IO/dictionary/DictionaryFileAccessException.hpp>
#include <IO/dictionary/OpenDictFileException.hpp>//class OpenDictFileException

namespace DictionaryReader
{
  namespace TUchemnitz
  {
  namespace EngWordSorted1st
  {
    namespace EachAttributeInSingleLine
    {

//BinarySearchInRAM::BinarySearchInRAM(const BinarySearchInRAM& orig) {
//}

BinarySearchInRAM::~BinarySearchInRAM() {
  if(m_dictionaryData)
    delete [] m_dictionaryData;
}

/** Do not load anything into memory. */
bool BinarySearchInRAM::open(const std::string & std_strDictFilePath )
{
  LOGN_DEBUG("Opening file " << std_strDictFilePath)
//      m_englishDictionary.open(std_strDictFilePath.c_str(),
//        std::ios_base::in | std::ios_base::binary );
  enum I_File::OpenResult openResult = m_dictFile.OpenA(
    std_strDictFilePath.c_str(),
    I_File::readOnly
    );
//      bool dictFileIsOpen = m_englishDictionary.is_open();
  bool dictFileIsOpen = openResult == I_File::success;
  if( /*dictFileIsOpen*/ openResult == I_File::success )
  {
    LOGN_DEBUG("Dict file is open")
//        m_englishDictionary.seekg(0, std::ios_base::end);
//        m_fileSizeInBytes = m_englishDictionary.tellg();
    m_fileSizeInBytes = m_dictFile.GetFileSizeInBytes();
    if( m_fileSizeInBytes == -1 )
    {
      const DWORD lastError = OperatingSystem::GetLastErrorCode();
      DictionaryFileAccessException dictionaryFileAccessException(
        DictionaryFileAccessException::getFileSize, lastError,
        std_strDictFilePath.c_str() );
      LOGN_ERROR("error getting dictionary file size->throwing an exception")
      throw dictionaryFileAccessException;
    }
//    m_p_vocaccess->
    m_dictionaryData = new uint8_t[m_fileSizeInBytes];
    if( m_dictionaryData )
    {
      fastestUnsignedDataType numberOfBytesRead;
      m_dictFile.SeekFilePointerPosition(0);
      I_File::ReadResult readResult = m_dictFile.Read(m_dictionaryData, 
        m_fileSizeInBytes, numberOfBytesRead);
      if( readResult != I_File::successfullyRead )
        return false;
//        throw VTrans3::OpenDictFileException(openError);
    }
    else //TODO throw an exception? This may be better than passing return 
      //codes over multiple function hierarchies (high call stack), e.g.:
      //throw VTrans3::DictFileException(errorAllocRAM)
      dictFileIsOpen = false;
//        m_englishDictionary.seekg(0, std::ios_base::beg);
//        m_dictFile.Seek(0);
  }
  else //Or throw enum I_File::OpenError openError
  {
  LOGN_ERROR("error loading dictionary->throwing an exception")
    //TODO catch exception
    throw VTrans3::OpenDictFileException(openResult);
  }
  return dictFileIsOpen;
}
    }
  }
  }
}