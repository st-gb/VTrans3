/** File:   TUchemnitzEngWordSorted1stAndBinarySearchInRAM.hpp
 * Author: sg
 * Created on 5. August 2017, 23:52  */

#ifndef TUCHEMNITZENGWORDSORTED1STANDBINARYSEARCHINRAM_HPP
#define TUCHEMNITZENGWORDSORTED1STANDBINARYSEARCHINRAM_HPP

#include <IO/dictionary/TUchemnitz/EngWordSorted1st/eachAttributeInSingleLine/BinarySearchInRAM.hpp>
#include <VocabularyInMainMem/TUchemnitzEngWordSorted1st/BinarySearchInRAM.hpp>
#include <Attributes/PositionString.hpp>

namespace VTrans3
{
  
class TUchemnitzEngWordSorted1stAndBinarySearchInRAM
{
public:
  VTrans3::VocabularyAccess::TUchemnitzEngWordSorted1st::BinarySearchInRAM
    m_vocAccess;
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
private:
  };

} /* namespace VTrans3 */


#endif /* TUCHEMNITZENGWORDSORTED1STANDBINARYSEARCHINRAM_HPP */

