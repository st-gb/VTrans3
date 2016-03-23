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

  TUchemnitzEngWordSorted1stAndBinarySearch::TUchemnitzEngWordSorted1stAndBinarySearch()
    :  m_dictReader(m_vocAccess)
  {
    // TODO Auto-generated constructor stub

  }

  TUchemnitzEngWordSorted1stAndBinarySearch::~TUchemnitzEngWordSorted1stAndBinarySearch()
  {
    LOGN_DEBUG("begin")
  }

  bool TUchemnitzEngWordSorted1stAndBinarySearch::loadDictionary(
      const std::string & s)
  {
    LOGN_DEBUG("begin")
//    return m_dictReader.read();
    return m_dictReader.open(s);
  }

  IVocabularyInMainMem::voc_container_type *
    TUchemnitzEngWordSorted1stAndBinarySearch::lookUpEnglishWord(
    PositionStringVector & c_r_positionStringVector,
    DWORD & dwTokenIndexRightMost)
  {
    return m_vocAccess.find(c_r_positionStringVector, dwTokenIndexRightMost);
//    return NULL;
  }

} /* namespace VTrans3 */
