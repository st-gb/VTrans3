/** File:   dict_cc_EN_DE_reader.cpp
 * Author: sg
 * Created on 2. Januar 2018, 14:44  */

#include "GermanTranslationByteOffset.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h>//LOGN_DEBUG
#include <OperatingSystem/GetLastErrorCode.hpp>
#include <IO/dictionary/DictionaryFileAccessException.hpp>
#include <IO/dictionary/OpenDictFileException.hpp>
#include <map>//class OpenDictFileException
#include <VocabularyInMainMem/IVocabularyInMainMem.hpp>//class IVocabularyInMainMem
#include "dict_cc_WordClasses.hpp"
#include "VocabularyInMainMem/VocablesForWord.hpp"

std::map<enum dict_cc_WordClasses::WordClasses, enum EnglishWord::English_word_class> dict_cc_WordClasses::EnglishWordClassFromPOSconverter::s_POS2englishWord; // define our static member variable
dict_cc_WordClasses::EnglishWordClassFromPOSconverter::_init dict_cc_WordClasses::EnglishWordClassFromPOSconverter::s_initializer;

namespace DictionaryReader { namespace dict_cc { namespace EN_DE {

GermanTranslationByteOffset::POSstring2POSenumContainerType 
  GermanTranslationByteOffset::s_POSstring2POSenum;

GermanTranslationByteOffset::GermanTranslationByteOffset(
  IVocabularyInMainMem * p_vocabularyInMainMem)
  : DictionaryReader::DictionaryReaderBase(p_vocabularyInMainMem)
    , m_longestEnglishEntryInChars(0)
{
  if( s_POSstring2POSenum.empty() )
    Build_POSstring2POSenum();
}

GermanTranslationByteOffset::~GermanTranslationByteOffset() {
}

void GermanTranslationByteOffset::Build_POSstring2POSenum()
{
  s_POSstring2POSenum.insert(std::make_pair("adj", dict_cc_WordClasses::adjective) );
  s_POSstring2POSenum.insert(std::make_pair("adv", dict_cc_WordClasses::adverb) );
  s_POSstring2POSenum.insert(std::make_pair("noun", dict_cc_WordClasses::noun) );
  s_POSstring2POSenum.insert(std::make_pair("past-p", dict_cc_WordClasses::past_participle) );
}

/** Do not load anything into memory. */
bool GermanTranslationByteOffset::open(const std::string & std_strDictFilePath )
{
  LOGN_DEBUG("Opening file " << std_strDictFilePath)
//      m_englishDictionary.open(std_strDictFilePath.c_str(),
//        std::ios_base::in | std::ios_base::binary );
  enum I_File::OpenResult openResult = m_dictionaryFile.OpenA(
    std_strDictFilePath.c_str(),
    I_File::readOnly
    );
//      bool dictFileIsOpen = m_englishDictionary.is_open();
  bool dictFileIsOpen = openResult == I_File::success;
  if( /*dictFileIsOpen*/ openResult == I_File::success )
  {
    LOGN_DEBUG("Dict file is open")
//        m_englishDictionary.seekg(0, std::ios_base::end);
//        m_fileSizeInBytes = m_englishDictionary.tellg();
    m_fileSizeInBytes = m_dictionaryFile.GetFileSizeInBytes();
    if( m_fileSizeInBytes == -1 )
    {
      const DWORD lastError = OperatingSystem::GetLastErrorCode();
      DictionaryFileAccessException dictionaryFileAccessException(
        DictionaryFileAccessException::getFileSize, lastError,
        std_strDictFilePath.c_str() );
      LOGN_ERROR("error getting dictionary file size->throwing an exception")
      throw dictionaryFileAccessException;
    }
    IndexByteOffsetOfGermanTranslations();
  }
  else //Or throw enum I_File::OpenError openError
  {
  LOGN_ERROR("error loading dictionary->throwing an exception")
    //TODO catch exception
    throw VTrans3::OpenDictFileException(openResult);
  }
  return dictFileIsOpen;
}

//void findEnglish()
//{
//  
//}

void GermanTranslationByteOffset::IndexByteOffsetOfGermanTranslations()
{
  if( m_dictionaryFile.IsOpen() )
  {
    try
    {
    unsigned numTabsInCurrentLine = 0;
    std::string currentAttribute;/** Attributes are separated by a tabulator.*/
    int currentByte = m_dictionaryFile.ReadByte();
    unsigned currentByteOffset = 0;
    while( currentByte != -1 )
    {
      switch( currentByte)
      {
        case '\t' :
          if( numTabsInCurrentLine == 0)
          {
            m_germanTranslationByteOffsetIndex.insert(
              std::make_pair(currentAttribute, currentByteOffset + 1) );
            if( currentAttribute.length() > m_longestEnglishEntryInChars)
              m_longestEnglishEntryInChars = currentAttribute.length();
            currentAttribute = "";
          }
          numTabsInCurrentLine ++;
          break;
        case 0xA : /** newline character */
          numTabsInCurrentLine = 0;
          break;
        default:
          if( numTabsInCurrentLine == 0)
            currentAttribute += (char) currentByte;
      }
      currentByteOffset ++;
      currentByte = m_dictionaryFile.ReadByte();
    }
    }
    catch( const FileReadException & fileReadException)
    {
      if( fileReadException.GetResult() != I_File::endOfFileReached )
        throw fileReadException;
    }
    /** Reached end of file -> set to begin*/
    enum I_File::SeekResult seekResult = m_dictionaryFile.
      SeekFilePointerPosition(0);
  }
}

void GermanTranslationByteOffset::GetPOSstring(
  const fastestUnsignedDataType byteOffsetOfGermanPart, 
  std::string & POSstring)
{
  enum I_File::SeekResult seekResult = m_dictionaryFile.
    SeekFilePointerPosition(byteOffsetOfGermanPart);
  if( seekResult == I_File::successfullySeeked)
  try
  {
  unsigned numTabsInCurrentLine = 0;
  std::string currentAttribute;/** Attributes are separated by a tabulator.*/
  int currentByte = m_dictionaryFile.ReadByte();
  while( currentByte != -1 )
  {
    switch( currentByte)
    {
//      case '\t' :
//        if( numTabsInCurrentLine == 0)
//        {
//          currentAttribute = "";
//        }
//        numTabsInCurrentLine ++;
//        break;
      case 0xA : /** newline character */
        POSstring = currentAttribute;
        return;
        break;
      default:
//        if( numTabsInCurrentLine == 1)
          currentAttribute += (char) currentByte;
    }
    currentByte = m_dictionaryFile.ReadByte();
  }
  }
  catch( const FileReadException & fileReadException)
  {
    if( fileReadException.GetResult() != I_File::endOfFileReached )
      throw fileReadException;
  }
}

inline std::vector<std::string> getPOStokens(const std::string & POSstring)
{
  int beginIndexOfToken = 0;
  int indexOfBeginOfSpaceChar = POSstring.find(' ', 0);
  std::string token;
  std::vector<std::string> tokenVector;
  while( indexOfBeginOfSpaceChar != std::string::npos )
  {
    beginIndexOfToken = indexOfBeginOfSpaceChar + 1;
    token = POSstring.substr(beginIndexOfToken, indexOfBeginOfSpaceChar);
    tokenVector.push_back(token);
    indexOfBeginOfSpaceChar = POSstring.find(' ', beginIndexOfToken);
  }
  token = POSstring.substr(beginIndexOfToken, POSstring.length());
  tokenVector.push_back(token);
  return tokenVector;
}

/** A dict.cc line may contain multiple word classes, e.g. "adj pres-p" */
std::set<enum dict_cc_WordClasses::WordClasses> GermanTranslationByteOffset::GetPartOfSpeeches(
  const fastestUnsignedDataType byteOffsetOfGermanPart)
{
  std::string POSstring;
  std::set<enum dict_cc_WordClasses::WordClasses> wordClasses;
  GetPOSstring(byteOffsetOfGermanPart, POSstring);
  std::vector<std::string> POStokens = getPOStokens(POSstring);

  std::vector<std::string>::const_iterator POSstringIter = POStokens.begin();
  while( POSstringIter != POStokens.end() )
  {
    /** Empty word class string: idiom*/
    POSstring2POSenumContainerType::const_iterator iter = 
      s_POSstring2POSenum.find(*POSstringIter);
    if( iter != s_POSstring2POSenum.end() )
    {
      wordClasses.insert(iter->second);
    }
    POSstringIter++;
  }
  return wordClasses;
}

void GermanTranslationByteOffset::findRegardingLongestEnglishEntryInFile(
  const PositionStringVector & psvStringToSearch,
  DWORD & r_dwTokenIndex//,
  )
{
  fastestUnsignedDataType endTokenIndex = r_dwTokenIndex;
  std::string searchInDictionary = psvStringToSearch.GetBetweenAsStdString(
    r_dwTokenIndex, endTokenIndex);
  //TODO psvStringToSearch may be < m_longestEnglishEntryInChars
  while( searchInDictionary.length() <= m_longestEnglishEntryInChars )
  {
    container_type::const_iterator citer = m_germanTranslationByteOffsetIndex.
      find(searchInDictionary);
    if( citer != m_germanTranslationByteOffsetIndex.end() )
    {
      fastestUnsignedDataType byteOffsetOfGermanPart = citer->second;
      std::set<enum dict_cc_WordClasses::WordClasses> partOfSpeech = 
        GetPartOfSpeeches(byteOffsetOfGermanPart);
//      std::string germanTranslation = GetGermanTranslation(byteOffsetOfGermanPart);
    }
    endTokenIndex ++;
    searchInDictionary = psvStringToSearch.GetBetweenAsStdString(r_dwTokenIndex, endTokenIndex);
  }
}

//void GermanTranslationByteOffset::findRegardingMaxTokenToConsider(
//  const PositionStringVector & psvStringToSearch,
//  DWORD & r_dwTokenIndex//,
//  )
//{
//  
//  for( fastestUnsignedDataType numCurrentTokens = 0;
//    numCurrentTokens <= IVocabularyInMainMem::m_maxTokenToConsider; numCurrentTokens ++ )
//  {
//    std::string searchInDictionary = psvStringToSearch.GetBetweenAsStdString(
//      r_dwTokenIndex, r_dwTokenIndex + numCurrentTokens);
//    container_type::const_iterator citer = m_germanTranslationByteOffsetIndex.
//      find(searchInDictionary);
//    if( citer != m_germanTranslationByteOffsetIndex.end() )
//    {
//      fastestUnsignedDataType byteOffsetOfGermanPart = citer->second;
//    }
//  }
//}

std::string GermanTranslationByteOffset::GetGermanTranslation(
  fastestUnsignedDataType & byteOffsetOfGermanPart)
{
  enum I_File::SeekResult seekResult = m_dictionaryFile.
    SeekFilePointerPosition(byteOffsetOfGermanPart);
  if( seekResult == I_File::successfullySeeked)
  try
  {
  unsigned numTabsInCurrentLine = 0;
  std::string currentAttribute;/** Attributes are separated by a tabulator.*/
  int currentByte = m_dictionaryFile.ReadByte();
  while( currentByte != -1 )
  {
    switch( currentByte)
    {
      case '\t' :
//        numTabsInCurrentLine ++;
        byteOffsetOfGermanPart ++;
        return currentAttribute;
        break;
      default:
//        if( numTabsInCurrentLine == 0)
        currentAttribute += (char) currentByte;
        byteOffsetOfGermanPart ++;
        break;
    }
    currentByte = m_dictionaryFile.ReadByte();
  }
  }
  catch( const FileReadException & fileReadException)
  {
    if( fileReadException.GetResult() != I_File::endOfFileReached )
      throw fileReadException;
  }
  return "";
}

VocablesForWord::voc_container_type * GermanTranslationByteOffset::findEnglishWord(
  const std::string & englishWord
  )
{
  VocablesForWord::voc_container_type * p_vocContainer = 
    new VocablesForWord::voc_container_type;
  VocablesForWord::voc_container_type & vocContainer = * p_vocContainer;
  container_type::const_iterator citer = m_germanTranslationByteOffsetIndex.
    find(englishWord);
  if( citer != m_germanTranslationByteOffsetIndex.end() )
  {
    fastestUnsignedDataType byteOffsetOfGermanPart = citer->second;
    std::string germanTranslation = GetGermanTranslation(byteOffsetOfGermanPart);
    std::set<enum dict_cc_WordClasses::WordClasses> partOfSpeechContainer = GetPartOfSpeeches(
      byteOffsetOfGermanPart);
    std::set<EnglishWord::English_word_class> englishWordClassContainer = 
      dict_cc_WordClasses::EnglishWordClassFromPOSconverter::
      GetEnglishWordClass(partOfSpeechContainer);
    for( std::set<EnglishWord::English_word_class>::const_iterator englishWordClassIter =
      englishWordClassContainer.begin(); 
      englishWordClassIter != englishWordClassContainer.end() ; 
      englishWordClassIter++ )
    {
      VocabularyAndTranslation * p_vocabularyAndTranslation = new 
        VocabularyAndTranslation(*englishWordClassIter);
      p_vocabularyAndTranslation->SetGermanWord(germanTranslation, 0);
      vocContainer.insert(p_vocabularyAndTranslation);
    }
  }
  if( p_vocContainer->size() > 0 )
    return p_vocContainer;
  return NULL;
}

/** @brief searches for word(s) from @param psvStringToSearch in dictionary 
 *   beginning from (token) index @r_dwTokenIndex. A word may contain more than
 *   1 token. */
IVocabularyInMainMem::voc_container_type * GermanTranslationByteOffset::findEnglishWord(
    const PositionStringVector & psvStringToSearch,
    DWORD & r_dwTokenIndex//,
//        const fastestUnsignedDataType endTokenIndex
    )
{
  const fastestUnsignedDataType numberOfTokens = psvStringToSearch.size();
  enum I_File::SeekResult seekRes = m_dictionaryFile.SeekFilePointerPosition(0);
  
//  findRegardingLongestEnglishEntryInFile();
//  findRegardingMaxTokenToConsider(psvStringToSearch, r_dwTokenIndex);
  
  IVocabularyInMainMem::voc_container_type * p_valid_voc_container = NULL, 
    * p_voc_container = NULL;
//  typedef std::vector< std::set<fastestUnsignedDataType> > offsetVec;
//
////      fastestUnsignedDataType byteOffsetOfVocable =
//  VocabularyAndTranslation * p_vocandtransl = NULL;
//  std::set<fastestUnsignedDataType> byteOffsetsOfVocData;
//  /** vector containing sets of byte offsets for vocabulary with same 
//   *    # of tokens.
//   *    1st element: byte offsets of voc data with 1 token */
//  std::vector< std::set<fastestUnsignedDataType> > byteOffsetsOfVocDataVec;
//  PositionStringVector::cmp comp;// = PositionStringVector;
//  fastestUnsignedDataType numToken = 1;
//  
//  fastestUnsignedDataType lowByteOffset = 0, highByteOffset = m_fileSizeInBytes;
//  do
//  {
//    byteOffsetsOfVocData.clear();
//    findEnglishWord(
//      psvStringToSearch,
//      r_dwTokenIndex,
//      numToken,
//      byteOffsetsOfVocData
//      );
//    if( byteOffsetsOfVocData.size() > 0 )
//      byteOffsetsOfVocDataVec.push_back(byteOffsetsOfVocData);
//    ++ numToken;
//    if( r_dwTokenIndex + numToken > psvStringToSearch.size() )
//      break;
//  }while(byteOffsetsOfVocData.size() > 0 );
//#ifdef _DEBUG
//  const int vecsz = byteOffsetsOfVocDataVec.size();
//  LOGN_DEBUG("# vectors:" << vecsz)
//#endif
//  /** Use vector containing the most tokens.
//   *  Must use "reverse_iterator" rather than "const_reverse_iterator"
//   *  for Android NDK. */
//  offsetVec::reverse_iterator
//    /** Start with the word containing the most tokens. */
//    c_rev_iter = byteOffsetsOfVocDataVec.rbegin();
//  if( c_rev_iter != byteOffsetsOfVocDataVec.rend() )
//  {
//    const std::set<fastestUnsignedDataType> & r_byteOffsetsOfVocData = * c_rev_iter;
//#ifdef _DEBUG
//    const int vecsz = r_byteOffsetsOfVocData.size();
//    LOGN_DEBUG("# offset data for same # token:" << vecsz)
//#endif
//    std::set<fastestUnsignedDataType>::const_iterator c_iter =
//      r_byteOffsetsOfVocData.begin();
//    fastestUnsignedDataType byteOffsetOfVocable;
//    while(c_iter != r_byteOffsetsOfVocData.end() )
//    {
//      byteOffsetOfVocable = * c_iter;
//      //TODO it seems that more than 1 container is created for the same word "car"
//      // for the dict.cc dictionary
//      p_voc_container = extractVocable(
//        byteOffsetOfVocable,
//        //p_voc_container,
//        p_vocandtransl);
//      /** If word kind can't be determined the p_voc_container is NULL */
//      if( p_voc_container )
//        p_valid_voc_container = p_voc_container;
//      ++ c_iter;
//    }
//    //++ c_rev_iter;
//  }
////      if()
  return /*NULL*/ p_valid_voc_container;
}

}}}