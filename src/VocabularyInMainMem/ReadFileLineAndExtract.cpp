/*
 * ReadFileLineAndExtract.cpp
 *
 *  Created on: Sep 7, 2013
 *      Author: Stefan
 */

#include "VocabularyInMainMem/ReadFileLneAndExtract.hpp"
#include <UserInterface/I_UserInterface.hpp>

namespace VocabularyAccess
{

  ReadFileLineAndExtract::ReadFileLineAndExtract(
    I_UserInterface & ui, IVocabularyInMainMem * p_vocaccess)
    : m_TUchemnitzDictionaryReader(
      ui, p_vocaccess)
  {
    Init();
//    for(map_type::const_iterator c_iter =
//      m_std_mapDictLineNumber2EnglishWord.begin(); c_iter !=
//      m_std_mapDictLineNumber2EnglishWord.end(); c_iter ++)
//    {
//      delete [] c_iter->second;
//    }
  }

  ReadFileLineAndExtract::~ReadFileLineAndExtract()
  {
    clear();
  }

  /*void*/ /*VocabularyAndTranslation * */
  IVocabularyInMainMem::voc_container_type * ReadFileLineAndExtract::Insert(
    const char * wordBegin, int numChars, //void * p_v
    enum EnglishWord::English_word_class word_class, /*void * */
    VocabularyAndTranslation *& p_vocabularyandtranslation)
  {
    if( word_class > EnglishWord::pronoun ) //Last non-dictionary word class.
    {
      //VocabularyAndTranslation * p_vocandtransl =
      m_CharStringStdMap.InsertAsKeyAndAddVocabularyAttributes(
        wordBegin
        , (int &) numChars
        , word_class,
        p_vocabularyandtranslation);
    }
    else if(m_p_dictionaryReader)
    {
      char * pch = new char[numChars + 1];
      memcpy(pch, wordBegin, numChars);
      pch[numChars] = '\0';
      fastestUnsignedDataType currentLineNumber = m_p_dictionaryReader->
//        GetCurrentLineNumber();
        GetFileOffsetOfCurrentLine();
//      m_std_mapDictLineNumber2EnglishWord.insert(std::make_pair(
//        currentLineNumber, pch) );
      stl_container_type::iterator c_iter =
        m_englishWord2DictLineNumber.find(pch);
      if( c_iter == m_englishWord2DictLineNumber.end() )
      {
        c_iter = m_englishWord2DictLineNumber.insert(std::make_pair(
          std::string(pch), DictLineNumbers() ) ).first;
      }
      c_iter->second.m_dictLineNumbers.push_back(currentLineNumber);
      delete [] pch;
    }
    return NULL;
  }

  fastestUnsignedDataType ReadFileLineAndExtract::GetNumberOfAllocatedBytes()
  {
    fastestUnsignedDataType numberOfAllocatedBytes = 0;
//    map_type::const_iterator c_iter = m_std_mapDictLineNumber2EnglishWord.begin();
//    while( c_iter != m_std_mapDictLineNumber2EnglishWord.end() )
//    {
//      numberOfAllocatedBytes += ::strlen(c_iter->second) + 1;
//      ++ c_iter;
//    }
    stl_container_type::const_iterator c_iter = m_englishWord2DictLineNumber.begin();
    while( c_iter != m_englishWord2DictLineNumber.end() )
    {
      numberOfAllocatedBytes += //::strlen(c_iter->first) + 1;
        c_iter->first.length() + 1;
      const DictLineNumbers & dictLineNumbers = c_iter->second;
      DictLineNumbers::stl_container_type::const_iterator c_iterLineNumbers
        = dictLineNumbers.m_dictLineNumbers.begin();
      for( ; c_iterLineNumbers != dictLineNumbers.m_dictLineNumbers.end();
          ++ c_iterLineNumbers)
      {
        numberOfAllocatedBytes += sizeof(DictLineNumbers::dictLineNumberDataType);
      }
      ++ c_iter;
    }
    return numberOfAllocatedBytes;
  }

  void ReadFileLineAndExtract::GetStatistics(
      fastestUnsignedDataType wordClass[] )
  {
    for( unsigned i = 0; i < EnglishWord::beyond_last_entry; ++i )
    {
      wordClass[i] = 0;
    }
    stl_container_type::const_iterator
      c_iterEnglishWord2DictFileLineBeginOffset = m_englishWord2DictLineNumber.begin();
    VocablesForWord::voc_container_type * p_voc_container;
    unsigned word_class[EnglishWord::beyond_last_entry];
    fastestUnsignedDataType numChars;
    char ar_chDictionaryLine[10000];
    while( c_iterEnglishWord2DictFileLineBeginOffset != m_englishWord2DictLineNumber.end() )
    {
      const DictLineNumbers & dictLineNumbers =
        c_iterEnglishWord2DictFileLineBeginOffset->second;
      DictLineNumbers::stl_container_type::const_iterator c_iterLineNumbers
        = dictLineNumbers.m_dictLineNumbers.begin();
      for( ; c_iterLineNumbers != dictLineNumbers.m_dictLineNumbers.end();
          ++ c_iterLineNumbers)
      {
#ifdef _DEBUG
        const fastestUnsignedDataType dictFileLineBeginOffset =
          * c_iterLineNumbers;
        if( ! m_TUchemnitzDictionaryReader.SetFileOffSet( dictFileLineBeginOffset) )
#else
        if( ! m_TUchemnitzDictionaryReader.SetFileOffSet( * c_iterLineNumbers) )
#endif
          mp_userinterface->Message("failed to seek file pointer for reading dict line->extracting vocs");
//        m_p_dictionaryReader->extractVocable();
        numChars = m_TUchemnitzDictionaryReader.UTF8toGermanASCII(
          ar_chDictionaryLine );
        m_TUchemnitzDictionaryReader.extractSingleEntry(ar_chDictionaryLine,
          numChars);
      }
      const std::string & std_strWord =
        c_iterEnglishWord2DictFileLineBeginOffset->first;
//      DWORD s = 0, e = std_strWord.length();
      PositionStringVector psv;
      std::string::size_type spaceCharIndex = std_strWord.find(' ');
      std::string::size_type tokenBeginCharIndex = 0;
      while( spaceCharIndex != std::string::npos)
      {
        const std::string::size_type len = spaceCharIndex - tokenBeginCharIndex;
        std::string token = std_strWord.substr(tokenBeginCharIndex, len);
        PositionString ps(token, tokenBeginCharIndex, len);
        psv.push_back( ps);
        tokenBeginCharIndex = spaceCharIndex + 1;
        spaceCharIndex = std_strWord.find(' ', spaceCharIndex + 1);
      }
      const std::string::size_type len = std_strWord.length() - tokenBeginCharIndex;
      std::string token = std_strWord.substr(tokenBeginCharIndex, len);
      PositionString ps(std_strWord.substr(tokenBeginCharIndex), tokenBeginCharIndex, len);
      psv.push_back( ps);
      DWORD r_dwTokenIndex = 0;
      p_voc_container = m_CharStringStdMap.find(psv, r_dwTokenIndex);
      if( p_voc_container)
      {
        VocablesForWord::voc_container_type::const_iterator iter =
          p_voc_container->begin();
        while( iter != p_voc_container->end() )
        {
          VocabularyAndTranslation * p_ = * iter;
          word_class[p_->m_englishWordClass] ++;
        }
      }
      /** Release memory.*/
      m_CharStringStdMap.clear();
      ++ c_iterEnglishWord2DictFileLineBeginOffset;
    }
//    return 0;
  }

  fastestUnsignedDataType ReadFileLineAndExtract::GetNumberOfVerbs()
  {

  }

  void ReadFileLineAndExtract::clear()
  {
//    map_type::const_iterator c_iter = m_std_mapDictLineNumber2EnglishWord.begin();
//    while( c_iter != m_std_mapDictLineNumber2EnglishWord.end() )
//    {
//      delete [] c_iter->second;
//      -- m_numberOfVocPairs;
//      ++ c_iter;
//    }
//    m_std_mapDictLineNumber2EnglishWord.clear();
    stl_container_type::const_iterator c_iter = m_englishWord2DictLineNumber.begin();
    while( c_iter != m_englishWord2DictLineNumber.end() )
    {
      const DictLineNumbers & dictLineNumbers = c_iter->second;
      DictLineNumbers::stl_container_type::const_iterator c_iterLineNumbers
        = dictLineNumbers.m_dictLineNumbers.begin();
      for( ; c_iterLineNumbers != dictLineNumbers.m_dictLineNumbers.end();
          ++ c_iterLineNumbers)
      {
        -- m_numberOfVocPairs;
      }
//      delete [] c_iter->first;
      ++ c_iter;
    }
    m_englishWord2DictLineNumber.clear();
    m_CharStringStdMap.clear();
  }

  VocablesForWord::voc_container_type * ReadFileLineAndExtract::find(
//    const VTrans::string_type & word
    const PositionStringVector & psv,
    DWORD & r_dwTokenIndex
    )
  {
    int numVocs;
//    if( c_iter == m_englishWord2DictLineNumber.end() )
    VocablesForWord::voc_container_type * p_voc_container =
      m_CharStringStdMap.find(psv, r_dwTokenIndex);
    if( p_voc_container )
    {
//      VocablesForWord::voc_container_type * p_voc_container =
//        m_CharStringStdMap.find(psv, r_dwTokenIndex);
//      if( p_voc_container)
//      {
        numVocs = p_voc_container->size();
        return p_voc_container;
//      }
    }
    else
    {
      VocablesForWord::voc_container_type * p_voc_container_type = NULL;
      VTrans::string_type word;
      const int numTokens = psv.size();
      stl_container_type::const_iterator c_iterDictLineBeginByteOffset,
        c_iterMaxWordMatch =
        m_englishWord2DictLineNumber.end();
      unsigned currentTokenIndex = r_dwTokenIndex;
      const fastestUnsignedDataType beginTokenIndex = r_dwTokenIndex;

      word = psv.at(currentTokenIndex).m_Str;
      c_iterDictLineBeginByteOffset = //m_charStringMap.find( word );
        m_englishWord2DictLineNumber.find(word);
      if( c_iterDictLineBeginByteOffset != m_englishWord2DictLineNumber.end() )
        c_iterMaxWordMatch = c_iterDictLineBeginByteOffset;
      ++ currentTokenIndex;
      if( currentTokenIndex < numTokens )
      {
        do
        {
          word += " ";
          word += psv.at(currentTokenIndex).m_Str;
          c_iterDictLineBeginByteOffset = m_englishWord2DictLineNumber.find( word );
          if( c_iterDictLineBeginByteOffset != m_englishWord2DictLineNumber.end() )
          {
            c_iterMaxWordMatch = c_iterDictLineBeginByteOffset;
            r_dwTokenIndex = currentTokenIndex;
          }
//          else
//          //Do not break here--"hand-held vacuum" does not exist in dict but
//          //"hand-held vacuum cleaner"
//            break;
          ++ currentTokenIndex;
        }while( currentTokenIndex < numTokens &&
            currentTokenIndex - beginTokenIndex < m_maxTokenToConsider );
      }

//      stl_container_type::iterator c_iterDictLineBeginByteOffset =
//        m_englishWord2DictLineNumber.find( word );
//      if( c_iterDictLineBeginByteOffset != m_englishWord2DictLineNumber.end() )
      if( c_iterMaxWordMatch != m_englishWord2DictLineNumber.end() )
      {
        const DictLineNumbers & dictLineNumbers = c_iterMaxWordMatch->second;
        DictLineNumbers::stl_container_type::const_iterator c_iterLineNumbers
          = dictLineNumbers.m_dictLineNumbers.begin();
        char ar_chDictionaryLine[10000];
        fastestUnsignedDataType numChars;
  //      VocablesForWord::voc_container_type * p_voc_container =
  //        m_CharStringStdMap.find(psv, r_dwTokenIndex);
  //      //Do not insert into "m_CharStringStdMap" multiple times
  //      if( ! p_voc_container )
        {
          for( ; c_iterLineNumbers != dictLineNumbers.m_dictLineNumbers.end();
              ++ c_iterLineNumbers)
          {
    //        m_englishWord2DictFilOffset.
            if( ! m_TUchemnitzDictionaryReader.SetFileOffSet(*c_iterLineNumbers) )
              mp_userinterface->Message("failed to seek file pointer for reading dict line->extracting vocs");
    //        m_p_dictionaryReader->extractVocable();
            numChars = m_TUchemnitzDictionaryReader.UTF8toGermanASCII(
              ar_chDictionaryLine );
            m_TUchemnitzDictionaryReader.extractSingleEntry(ar_chDictionaryLine,
              numChars);
          }
          //May be modified above if word has more than 1 token.
          r_dwTokenIndex = beginTokenIndex;
          p_voc_container = m_CharStringStdMap.find(psv, r_dwTokenIndex);
          if( p_voc_container)
            numVocs = p_voc_container->size();
        }
      }
      return p_voc_container;
    }
    return NULL;
  }
} /* namespace VocabularyAccess */
