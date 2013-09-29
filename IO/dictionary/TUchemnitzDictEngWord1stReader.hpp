/*
 * TUchemnitzDictEngWord1stReader.hpp
 *
 *  Created on: Aug 31, 2013
 *      Author: Stefan
 */

#ifndef TUCHEMNITZDICTENGWORD1STREADER_HPP_
#define TUCHEMNITZDICTENGWORD1STREADER_HPP_

class IVocabularyInMainMem;

namespace DictionaryReader
{
  /** For a TU Chemnitz formatted dictionary where the lines begin with
   *  English words. So the words are sorted (and a binary search may be
   *  applied) as needed for English->German translation. */
  class TUchemnitzDictEngWord1stReader
  {
    IVocabularyInMainMem * m_p_vocabularyAccess;
  public:
    TUchemnitzDictEngWord1stReader();
    virtual
    ~TUchemnitzDictEngWord1stReader();
    void read();
  };

} /* namespace DictionaryReader */
#endif /* TUCHEMNITZDICTENGWORD1STREADER_HPP_ */
