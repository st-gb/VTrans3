/*
 * DictionaryReaderBase.hpp
 *
 *  Created on: Sep 4, 2013
 *      Author: Stefan
 */

#ifndef DICTIONARYREADERBASE_HPP_
#define DICTIONARYREADERBASE_HPP_

#include <hardware/CPU/fastest_data_type.h> //typedef fastestUnsignedDataType
#include <string>
#include <Controller/character_string/ISO_8859_1.hpp>
#include <stdint.h> //uint8_t

inline bool isValidEnglishWordCharacter(uint8_t character)
{
  if( character > 0x60 /** "a" */ && character < 0x7A /** "z" */ || 
      character == '-' /** e.g. "fork-lift" */ )
    return true;
  return false;
}

inline bool isValidGermanWordCharacter(uint8_t character)
{
  if( character > 'A' && character < 'Z' || 
      character > 0x60 /** "a" */ && character < 0x7A /** "z" */ || 
      character == ISO_8859_1::ae || character == ISO_8859_1::Ae ||
      character == ISO_8859_1::oe || character == ISO_8859_1::Oe ||
      character == ISO_8859_1::ue || character == ISO_8859_1::Ue ||
      character == ISO_8859_1::sz /** "ß" */ ||
      character == '-' /** e.g. "fork-lift" */ )
    return true;
  return false;
}

/** Forward declaration. */
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
