/*
 * DictionaryReaderBase.hpp
 *
 *  Created on: Sep 4, 2013
 *      Author: Stefan
 */

#ifndef DICTIONARYREADERBASE_HPP_
#define DICTIONARYREADERBASE_HPP_

#include <fastest_data_type.h> //typedef fastestUnsignedDataType
#include <string>
//
class IVocabularyInMainMem;

namespace DictionaryReader
{

  class DictionaryReaderBase
  {
  protected:
    std::string m_std_strFilePath;
    //TODO check if static functions are really faster
    /*static*/ fastestUnsignedDataType m_numBytesRead;
    fastestUnsignedDataType m_currentLineBeginFileOffset;
    fastestUnsignedDataType m_fileSizeInBytes;
    fastestUnsignedDataType m_currentLineNumber;
  public:
    IVocabularyInMainMem * m_p_vocaccess;
    DictionaryReaderBase(IVocabularyInMainMem *);
    virtual
    ~DictionaryReaderBase();

    fastestUnsignedDataType GetCurrentLineNumber() {
      return m_currentLineNumber; }
    /** For displaying a progress status */
    fastestUnsignedDataType GetFileOffsetOfCurrentLine() {
      return //m_numBytesRead;
        m_currentLineBeginFileOffset;
    }
//    virtual void extractVocable() = 0;
    const char * const GetFilePath() { return m_std_strFilePath.c_str(); }
//    SetUserInterface(I_UserInterface *)
  };

} /* namespace DictionaryReader */
#endif /* DICTIONARYREADERBASE_HPP_ */
