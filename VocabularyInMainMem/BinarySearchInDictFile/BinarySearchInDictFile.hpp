/*
 * BinarySearchInDictFile.hpp
 *
 *  Created on: Aug 31, 2013
 *      Author: Stefan
 */

#ifndef BINARYSEARCHINDICTFILE_HPP_
#define BINARYSEARCHINDICTFILE_HPP_

//#include "../IVocabularyInMainMem.hpp" //base class
#include "../CharMappedDictionary.hpp" //base class
//#include <InputOutput/LockableFile.hpp>
#include <fstream> //class std::ifstream
#include <fastest_data_type.h> //typedef fastestUnsignedDataType

#define NUMBER_OF_LETTERS 52

class BytePosAndNextChar
{
public:
  unsigned m_bytePos;
  BytePosAndNextChar * m_nextLetter;
  BytePosAndNextChar()
    : m_nextLetter(NULL) { }
};

namespace VocabularyAccess
{

  class BinarySearchInDictFile
    : public /*IVocabularyInMainMem*/ VTrans::CharMappedDictionary
  {
  private:
    CharStringStdMap m_CharStringStdMap;
//    unsigned m_bytePos[NUMBER_OF_LETTERS];
    BytePosAndNextChar m_BytePosAndNextChar[NUMBER_OF_LETTERS];
//    LockableFile m_englishDictionary;
    std::ifstream m_englishDictionary;
    typedef std::map</*std::string*/ char *, /*fastestUnsignedDataType*/
      char *> map_type;
    map_type m_std_mapEnglishWord2BytePosInDictFile;
    void search(const VTrans::string_type & word );
  public:
    BinarySearchInDictFile();
    virtual
    ~BinarySearchInDictFile();

//    BuildPrefixTable();
    VocablesForWord::voc_container_type find(
    //    const VTrans::string_type & englishWord
        const PositionStringVector & psv, DWORD & r_dwTokenIndex
        );
  };

} /* namespace VocabularyAccess */
#endif /* BINARYSEARCHINDICTFILE_HPP_ */
