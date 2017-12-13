/*
 * TUchemnitzEngWordSorted1stAndBinarySearch.cpp
 *
 *  Created on: 11.12.2013
 *      Author: mr.sys
 */

#include <Controller/DictReaderAndVocAccess/TUchemnitzEngWordSorted1stAndBinarySearch.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h>

namespace VTrans3
{

  TUchemnitzEngWordSorted1stAndBinarySearchInDictFile::TUchemnitzEngWordSorted1stAndBinarySearchInDictFile()
    :  m_dictReader(m_vocAccess)
  {
  }

  TUchemnitzEngWordSorted1stAndBinarySearchInDictFile::~TUchemnitzEngWordSorted1stAndBinarySearchInDictFile()
  {
    LOGN_DEBUG("begin")
  }

  bool TUchemnitzEngWordSorted1stAndBinarySearchInDictFile::loadDictionary(
      const std::string & std_strDictionaryFilePath)
  {
    LOGN_DEBUG("begin")
//    return m_dictReader.read();
    return m_dictReader.open(std_strDictionaryFilePath);
  }

  IVocabularyInMainMem::voc_container_type *
    TUchemnitzEngWordSorted1stAndBinarySearchInDictFile::lookUpEnglishWord(
    PositionStringVector & c_r_positionStringVector,
    DWORD & dwTokenIndexRightMost)
  {
    return m_vocAccess.find(c_r_positionStringVector, dwTokenIndexRightMost);
//    return NULL;
  }

} /* namespace VTrans3 */
