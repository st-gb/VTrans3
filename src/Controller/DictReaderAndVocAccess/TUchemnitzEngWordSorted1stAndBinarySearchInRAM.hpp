/** File:   TUchemnitzEngWordSorted1stAndBinarySearchInRAM.hpp
 * Author: sg
 * Created on 5. August 2017, 23:52  */

#ifndef TUCHEMNITZENGWORDSORTED1STANDBINARYSEARCHINRAM_HPP
#define TUCHEMNITZENGWORDSORTED1STANDBINARYSEARCHINRAM_HPP

#include <IO/dictionary/TUchemnitz/EngWordSorted1st/eachAttributeInSingleLine/BinarySearchInRAM.hpp>
#include <VocabularyInMainMem/TUchemnitzEngWordSorted1st/BinarySearchInRAM.hpp>
#include <Attributes/PositionString.hpp>
#include "I_DictReaderAndVocAccess.hpp"///class I_DictReaderAndVocAccess

namespace VTrans3
{
  typedef VTrans3::VocabularyAccess::TUchemnitzEngWordSorted1st::
    BinarySearchInRAM VocAccessType;
class TUchemnitzEngWordSorted1stAndBinarySearchInRAM
  : public I_DictReaderAndVocAccess
{
public:
  VocAccessType m_vocAccess;
  DictionaryReader::TUchemnitz::EngWordSorted1st::EachAttributeInSingleLine::
    BinarySearchInRAM m_dictReader;
  TUchemnitzEngWordSorted1stAndBinarySearchInRAM();
//  TUchemnitzEngWordSorted1stAndBinarySearchInRAM(const TUchemnitzEngWordSorted1stAndBinarySearchInRAM& orig);
  virtual ~TUchemnitzEngWordSorted1stAndBinarySearchInRAM();
  
  IVocabularyInMainMem & GetVocAccess() { return m_vocAccess; }
  bool loadDictionary(const std::string & );
  IVocabularyInMainMem::voc_container_type * lookUpEnglishWord(
    PositionStringVector & c_r_positionStringVector,
    DWORD & dwTokenIndexRightMost);
  virtual std::/*ostream &*/string getFundamentalWords(/*std::ostream & os*/);
private:
  };

} /* namespace VTrans3 */

#endif /* TUCHEMNITZENGWORDSORTED1STANDBINARYSEARCHINRAM_HPP */

