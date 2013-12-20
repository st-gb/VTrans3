/*
 * TUchemnitzDictReaderAnd.hpp
 *
 *  Created on: 11.12.2013
 *      Author: mr.sys
 */

#ifndef TUCHEMNITZDICTREADERAND_HPP_
#define TUCHEMNITZDICTREADERAND_HPP_

#include <IO/dictionary/TUchemnitzEngWordSorted1st/BinarySearchInDictFile.hpp>
#include <VocabularyInMainMem/TUchemnitzEngWordSorted1st/BinarySearchInDictFile.hpp>
#include <Attributes/PositionString.hpp>

namespace VTrans3
{

  class TUchemnitzEngWordSorted1stAndBinarySearch
  {
    VTrans3::VocabularyAccess::TUchemnitzEngWordSorted1st::BinarySearchInDictFile
      m_vocAccess;
    DictionaryReader::TUchemnitzEngWordSorted1st::BinarySearchInDictFile m_dictReader;
  public:
    TUchemnitzEngWordSorted1stAndBinarySearch();
    virtual
    ~TUchemnitzEngWordSorted1stAndBinarySearch();
    IVocabularyInMainMem & GetVocAccess() { return m_vocAccess; }
    bool loadDictionary(const std::string & );
    IVocabularyInMainMem::voc_container_type * lookUpEnglishWord(
      PositionStringVector & c_r_positionStringVector,
      DWORD & dwTokenIndexRightMost);
  };

} /* namespace VTrans3 */

#endif /* TUCHEMNITZDICTREADERAND_HPP_ */
