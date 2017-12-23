/** File:   TUchemnitzEngWordSorted1stAndBinarySearchInRAM.cpp
 * Author: sg
 * Created on 5. August 2017, 23:52  */

#include "TUchemnitzEngWordSorted1stAndBinarySearchInRAM.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_DEBUG(...)

namespace VTrans3
{  
TUchemnitzEngWordSorted1stAndBinarySearchInRAM::TUchemnitzEngWordSorted1stAndBinarySearchInRAM()
  : m_dictReader(m_vocAccess)
{
}

//TUchemnitzEngWordSorted1stAndBinarySearchInRAM::TUchemnitzEngWordSorted1stAndBinarySearchInRAM(
//  const TUchemnitzEngWordSorted1stAndBinarySearchInRAM& orig) {
//}

TUchemnitzEngWordSorted1stAndBinarySearchInRAM::~TUchemnitzEngWordSorted1stAndBinarySearchInRAM() {
}

bool TUchemnitzEngWordSorted1stAndBinarySearchInRAM::loadDictionary(
    const std::string & filePath)
{
  LOGN_DEBUG("begin")
//    return m_dictReader.read();
  return m_dictReader.open(filePath);
}

IVocabularyInMainMem::voc_container_type *
  TUchemnitzEngWordSorted1stAndBinarySearchInRAM::lookUpEnglishWord(
  PositionStringVector & c_r_positionStringVector,
  DWORD & dwTokenIndexRightMost)
{
  return m_vocAccess.find(c_r_positionStringVector, dwTokenIndexRightMost);
//    return NULL;
}
}/** namespace VTrans3 */
