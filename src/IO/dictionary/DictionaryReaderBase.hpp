/** DictionaryReaderBase.hpp
 *  Created on: Sep 4, 2013
 *      Author: Stefan */

#ifndef DICTIONARYREADERBASE_HPP_
#define DICTIONARYREADERBASE_HPP_

#include <hardware/CPU/fastest_data_type.h> //typedef fastestUnsignedDataType
#include <string>
#include <Controller/character_string/ISO_8859_1.hpp>
#include <stdint.h> //uint8_t
#include <Attributes/GermanWord.hpp> //class GermanWord
#include <map> //class std::map

inline bool isValidEnglishWordCharacter(uint8_t character)
{
    /** Use parentheses around the '&&' expression to avoid compiler warning */
  if( ( character > 0x60 /** "a" */ && character < 0x7A /** "z" */ ) ||
      character == '-' /** e.g. "fork-lift" */ )
    return true;
  return false;
}

inline bool isValidGermanWordCharacter(uint8_t character)
{
  /** Use parentheses around the '&&' expression to avoid compiler warning */
  if( ( character >= 'A' && character <= 'Z' ) ||
      /** Use parentheses around the '&&' expression to avoid compiler warning */
      ( character >= 'a' && character <= 'z' ) )
    return true;
  else
    switch(character) /** "switch" is faster than "if" */
    {
        case ISO_8859_1::ae:
        case ISO_8859_1::Ae:
        case ISO_8859_1::oe:
        case ISO_8859_1::Oe:
        case ISO_8859_1::ue:
        case ISO_8859_1::Ue:
        case ISO_8859_1::sz: /** "ß" */
        case '-': /** e.g. "fork-lift" */
        return true;
    }
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

    typedef std::map<std::string, GermanNoun::grammatical_gender> 
      nounGenderString2genderEnumContainerType;
    static nounGenderString2genderEnumContainerType s_nounGenderString2genderEnum;

    fastestUnsignedDataType GetCurrentLineNumber() {
      return m_currentLineNumber; }
    /** For displaying a progress status */
    fastestUnsignedDataType GetFileOffsetOfCurrentLine() {
      return //m_numBytesRead;
        m_currentLineBeginFileOffset;
    }
    void Build_NounGenderString2genderEnum();
//    virtual void extractVocable() = 0;
    const char * const GetFilePath() { return m_std_strFilePath.c_str(); }
//    SetUserInterface(I_UserInterface *)
  };

} /* namespace DictionaryReader */
#endif /* DICTIONARYREADERBASE_HPP_ */
