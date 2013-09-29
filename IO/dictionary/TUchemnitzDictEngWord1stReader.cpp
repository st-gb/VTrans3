/*
 * TUchemnitzDictEngWord1stReader.cpp
 *
 *  Created on: Aug 31, 2013
 *      Author: Stefan
 */

#include "IO/dictionary/TUchemnitzDictEngWord1stReader.hpp"
#include <VocabularyInMainMem/IVocabularyInMainMem.hpp>//class IVocabularyInMainMem
#include <IO/dictionary/TUchemnitzDictSeparatorChars.h>

namespace DictionaryReader
{

  TUchemnitzDictEngWord1stReader::TUchemnitzDictEngWord1stReader()
  {
    // TODO Auto-generated constructor stub

  }

  TUchemnitzDictEngWord1stReader::~TUchemnitzDictEngWord1stReader()
  {
    // TODO Auto-generated destructor stub
  }

  //TODO idea: because words are sorted lexicographically:
  // store english words: at 1st store number of 1st identical
  // characters:
  //  antimatter
  //  antimephitic
  // ->if antimatter is the 1st word inside the dictionary, store:
  // "antimatter", "5"(# 1st identical characters),"ephitic"
  void TUchemnitzDictEngWord1stReader::read()
  {
//    m_bytePos[letter] = bytePos;
    unsigned filePosInByte = 0;
    char ch;
    int i;
    i = m_englishDictionary.get();
    char word[100];
    BytePosAndNextChar * p_BytePosAndNextChar;
    fastestUnsignedDataType charIndex = 0;
    bool newLine = false;
    bool englishGermanSeperatorCharOccurred = false;
    while(i != eof)
    {
      ch = i;
      switch(ch)
      {
        case ENGLISH_GERMAN_SEPERATOR_CHAR :
          englishGermanSeperatorCharOccurred = true;
        case '\r':
        case '\n':
          newLine = true;
        case SYNONYM_SEPERATOR_CHAR :
        case SAME_VOCABLE_SEPERATOR_CHAR :
          if(englishGermanSeperatorCharOccurred)
          word[charIndex] = '\0';
          char * pch = new char[charIndex];
          memcpy(pch, word, charIndex + 1);
          m_p_vocabularyAccess->InsertAsKeyAndAddVocabularyAttributes(
            word,
            );
          charIndex = 0;
          break;
        default:
          word[charIndex ++] = ch;
          break;
      }
      i = m_arbyCharValueToArrayIndex[ch];
      p_BytePosAndNextChar = m_BytePosAndNextChar[i];
      p_BytePosAndNextChar->m_bytePos = filePosInByte;
      ++ filePosInByte;
      i = m_englishDictionary.get();
    }
  }
  /** Inserts the string as a key value to the container and adds the string
   *  as an attribute value.*/
  VocabularyAndTranslation * InsertAsKeyAndAddVocabularyAttributes(
    const char * ar_chWordBegin,
    unsigned stringLen,
    enum EnglishWord::English_word_class word_class
    )
  {
    /*void * p_v =*/ return Insert(ar_chWordBegin, stringLen, word_class, NULL);
//      AddVocabularyAttributes( word_class, p_v);
  }
  /** Save memory by referring to existing voc attributes. */
  VocabularyAndTranslation * InsertAsKeyAndReferToExistingVocData(
    enum EnglishWord::English_word_class word_class,
    const char * ar_chWordBegin,
    unsigned stringLen,
    const VocabularyAndTranslation * const p_vocandtranslAllocated
    )
  {
    VocabularyAndTranslation * p_vocandtransl =
      Insert(ar_chWordBegin, stringLen, word_class, NULL);
    if( p_vocandtransl )
    {
      p_vocandtransl->PointToAttributeData(p_vocandtranslAllocated);
      p_vocandtransl->m_englishWordClass = word_class;
    }
    return p_vocandtransl;
  };
} /* namespace DictionaryReader */
