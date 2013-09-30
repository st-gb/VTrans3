/*
 * BinarySearchInDictFile.cpp
 *
 *  Created on: Aug 31, 2013
 *      Author: Stefan
 */

#include "VocabularyInMainMem/BinarySearchInDictFile/BinarySearchInDictFile.hpp"
#include <IO/dictionary/TUchemnitzDictSeparatorChars.h>

namespace VocabularyAccess
{

  BinarySearchInDictFile::BinarySearchInDictFile(
    /*const char * const dictionaryFilePath*/)
  {
//    m_englishDictionary.open(dictionaryFilePath);
//    BuildPrefixTable();

    /** Is inserted into when calling TUchemnitzDictionaryReader::
     *   extractSingleEntry(...)*/
    TUchemnitzDictionaryReader::s_p_vocinmainmem = & m_CharStringStdMap;
  }

  BinarySearchInDictFile::~BinarySearchInDictFile()
  {
    // TODO Auto-generated destructor stub
  }

//  VocablesForWord::voc_container_type BinarySearchInDictFile::find(
////    const VTrans::string_type & englishWord
//    const PositionStringVector & psv, DWORD & r_dwTokenIndex
//    )
//  {
//    VocablesForWord::voc_container_type voc_container;
//
//    voc_container
//    return voc_container;
//  }

  void extractVocable(const char * const p_chDictionaryLine)
  {
//    char ch = * p_chDictionaryLine;
//    while( ch)
//    {
//      switch(ch)
//      {
//        case ENGLISH_GERMAN_SEPERATOR_CHAR :
//        case SYNONYM_SEPERATOR_CHAR :
//        case SAME_VOCABLE_SEPERATOR_CHAR :
//          //TODO possibly store size in value of the "m_std_mapEnglishWord2BytePosInDictFile"
//          //container
//          break;
//        default:
//          break;
//      }
//    }
    size_t stringSizeInChar = strlen(p_chDictionaryLine);
    /** Indirectly calls IVocInMainMem::InsertAsKeyAndAddVocabularyAttributes(...) */
    TUchemnitzDictionaryReader::extractSingleEntry(
      p_chDictionaryLine, stringSizeInChar
      );
  }

  void BinarySearchInDictFile::search(const VTrans::string_type & word )
  {
    map_type::const_iterator c_iter = m_std_mapEnglishWord2BytePosInDictFile.
      find( (char * const) word.c_str() );
    if( c_iter != m_std_mapEnglishWord2BytePosInDictFile.end() )
    {
      extractVocable(c_iter->second);
    }
  }

  /** @brief Called when parsing/ translating.
   *  @return must be held in memory (until ...) because is referred to when translating */
  VocablesForWord::voc_container_type BinarySearchInDictFile::find(
      const PositionStringVector & psv,
  //      const PositionstdstringVector & psv,
      DWORD & r_dwTokenIndex
      )
  {
  //  VocablesForWord * p_vocablesforword = NULL;
    VocablesForWord::voc_container_type * p_voc_container_type = NULL;
    VTrans::string_type word;
    const int numTokens = psv.size();
    unsigned tokenIndex = r_dwTokenIndex;

    word = psv.at(tokenIndex).m_Str;
    ++ tokenIndex;
    while( tokenIndex < numTokens );
    {

    }
    if( tokenIndex < numTokens )
    {
      do
      {
        search(word);
        m_lfile
        word += " ";
        word += psv.at(tokenIndex).m_Str;
        c_iter = m_charStringMap.find( word );
        if( c_iter == m_charStringMap.end() )
          break;
        c_iterMaxWordMatch = c_iter;
        r_dwTokenIndex = tokenIndex;
        ++ tokenIndex;
      }while( tokenIndex < numTokens );
    }
  //  r_dwTokenIndex = tokenIndex - 1;
    if( c_iterMaxWordMatch != m_charStringMap.end() )
    {
  //    VocablesForWord * p_vocablesforword = (VocablesForWord *)
  //      & c_iterMaxWordMatch->second;
  //    p_voc_container_type = & p_vocablesforword->
  //      m_std_set_p_vocabularyandtranslation;
      VocablesForWord & vocablesforword = (VocablesForWord &)
        c_iterMaxWordMatch->second;
      p_voc_container_type = //c_iter->second.m_p_setpvocabularyandtranslation;
        (VocablesForWord::voc_container_type *) & //c_iter->second.
        vocablesforword.m_std_set_p_vocabularyandtranslation;
    }
    return p_voc_container_type;
  }
} /* namespace VocabularyAccess */
