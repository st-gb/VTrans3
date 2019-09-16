/** File:   dict_cc_EN_DE_reader.cpp
 * Author: sg
 * Created on 2. Januar 2018, 14:44  */

///from common_sourcecode:
#include "GermanTranslationByteOffset.hpp"
#include <FileSystem/GetAbsolutePath.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h>//LOGN_DEBUG
#include <OperatingSystem/GetLastErrorCode.hpp>

#include <IO/dictionary/DictionaryFileAccessException.hpp>
#include <IO/dictionary/OpenDictFileException.hpp>
#include <map>//class OpenDictFileException
#include <VocabularyInMainMem/IVocabularyInMainMem.hpp>//class IVocabularyInMainMem
#include "dict_cc_WordClasses.hpp"
#include "VocabularyInMainMem/VocablesForWord.hpp"
#include <UserInterface/I_UserInterface.hpp>
#include <InputOutput/GetCharInACSIIcodePage850.hpp>
#include <FileSystem/File/FileReadException.hpp>
#include <regex> //std::regex

///Static member variables definition
std::map<enum dict_cc_WordClasses::WordClasses, enum EnglishWord::
 English_word_class> dict_cc_WordClasses::EnglishWordClassFromPOSconverter::
 s_POS2englishWord;
dict_cc_WordClasses::EnglishWordClassFromPOSconverter::_init
 dict_cc_WordClasses::EnglishWordClassFromPOSconverter::s_initializer;

namespace DictionaryReader { namespace dict_cc { namespace EN_DE {

GermanTranslationByteOffset::POSstring2POSenumContainerType 
  GermanTranslationByteOffset::s_POSstring2POSenum;

GermanTranslationByteOffset::GermanTranslationByteOffset(
  IVocabularyInMainMem * p_vocabularyInMainMem)
  : DictionaryReader::DictionaryReaderBase(p_vocabularyInMainMem)
    , m_longestEnglishEntryInChars(0)
{
  if( s_POSstring2POSenum.empty() )
    FillPOSstring2dict_ccPOSenum();
}

GermanTranslationByteOffset::~GermanTranslationByteOffset() {
}

///Build a mapping from the part of speeches part of a dictionary line to an
/// enum value.
void GermanTranslationByteOffset::FillPOSstring2dict_ccPOSenum()
{
  AddMapping("adj", dict_cc_WordClasses::adjective);
  AddMapping("adv", dict_cc_WordClasses::adverb);
  AddMapping("conj", dict_cc_WordClasses::conjunction);
  AddMapping("noun", dict_cc_WordClasses::noun);
  AddMapping("past-p", dict_cc_WordClasses::past_participle);
  AddMapping("prep", dict_cc_WordClasses::preposition);
  AddMapping("pron", dict_cc_WordClasses::pronoun);
}

/** Do not load anything into memory. */
bool GermanTranslationByteOffset::open(const std::string & std_strDictFilePath )
{
  LOGN_DEBUG("Opening file \"" << std_strDictFilePath << "\"")
//      m_englishDictionary.open(std_strDictFilePath.c_str(),
//        std::ios_base::in | std::ios_base::binary );
  //TODO 552 bytes in 1 blocks are still reachable in loss record 1 of 1
  //==5832==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
  //==5832==    by 0x567AE49: __fopen_internal (iofopen.c:65)
  //==5832==    by 0x567AE49: fopen@@GLIBC_2.2.5 (iofopen.c:89)
  //==5832==    by 0x25A978: Linux::File::OpenA(char const*, I_File::OpenMode) (File.hpp:96)
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
    readWholeFile(
      ///https://stackoverflow.com/questions/18145874/passing-a-pointer-to-a-class-member-function-as-a-parameter/18152250
      &GermanTranslationByteOffset::IdxGerTranslsByteOffsTabChar,
      &GermanTranslationByteOffset::IdxGerTranslsByteOffsLineEnd,
      0,///dummy value
      EnglishPart
      );
  }
  else //Or throw enum I_File::OpenError openError
  {
    LOGN_ERROR("error loading dictionary \"" << std_strDictFilePath <<
      "\"->throwing an exception")
    const std::string absFilePath = FileSystem::GetAbsolutePathA(
      std_strDictFilePath.c_str() );
    //TODO catch exception
    throw VTrans3::OpenDictFileException(openResult, absFilePath);
  }
  return dictFileIsOpen;
}

//void findEnglish()
//{
//  
//}

void GermanTranslationByteOffset::IdxGerTranslsByteOffsLineEnd(void * p_v)
{
  m_p_vocaccess->GetUserInterface()->m_translationProcess.SetStatus(
    VTrans::loadDictionary,
    "",
    (float) currentByteOffset / (float) fileSizeInBytes * INT_MAX
    );
}

void GermanTranslationByteOffset::IdxGerTranslsByteOffsTabChar(
  //const fastestUnsignedDataType numTabsInCurrentLine
  )
{
  if( numTabsInCurrentLine == EnglishPart)
  {
    m_germanTranslationByteOffsetIndex.insert(
      std::make_pair(currentAttribute, currentByteOffset + 1) );
    if( currentAttribute.length() > m_longestEnglishEntryInChars)
      m_longestEnglishEntryInChars = currentAttribute.length();
    currentAttribute = "";
  }
  numTabsInCurrentLine ++;
}

void GermanTranslationByteOffset::GetStatsLineEnd(void * p_v)
{
  std::map<enum EnglishWord::English_word_class, unsigned> &
    englishWordClass2CounterMap = *
    (std::map<enum EnglishWord::English_word_class, unsigned> *) p_v;
  
//  std::set<enum dict_cc_WordClasses::WordClasses> partOfSpeechContainer =
  std::set<EnglishWord::English_word_class> englishWordClassContainer =
    GetPartOfSpeeches(m_germanPart, currentAttribute);
//  std::set<EnglishWord::English_word_class> englishWordClassContainer =
//    dict_cc_WordClasses::EnglishWordClassFromPOSconverter::
//    GetEnglishWordClass(partOfSpeechContainer);
  for( std::set<EnglishWord::English_word_class>::const_iterator 
    englishWordClassIter = englishWordClassContainer.begin();
    englishWordClassIter != englishWordClassContainer.end() ;
    englishWordClassIter++ )
  {
    ///The last attribute only is the part of speech, e.g. "noun". To get more
    /// detailed information, e.g. whether it is the singular of a noun the 
    /// 2nd tab-separated part of a dictionary line has to be processed.
    const EnglishWord::English_word_class English_word_class =
      *englishWordClassIter;
    englishWordClass2CounterMap[English_word_class] ++;
  }
  //TODO: neccessary?
  m_p_vocaccess->GetUserInterface()->m_translationProcess.SetStatus(
    VTrans::loadDictionary,//TOOD use enum value "dictStats"
    "",
    (float) currentByteOffset / (float) fileSizeInBytes * INT_MAX
    );
}

void GermanTranslationByteOffset::GetStatsTabChar(
  //const fastestUnsignedDataType numTabsInCurrentLine
  )
{
  if( numTabsInCurrentLine == GermanPart)
  {
    m_germanPart = currentAttribute;
  }
  currentAttribute = "";
  numTabsInCurrentLine ++;
}

void GermanTranslationByteOffset::readWholeFile(
  pfnTabCharType tabCharLineCallBack,
  pfnEndOfLineType endOfLineCallBack,
  void * p_v,
  const fastestUnsignedDataType attrIdx)
{
  if( m_dictionaryFile.IsOpen() )
  {
    fileSizeInBytes = m_dictionaryFile.GetFileSizeInBytes();
    try
    {
    numTabsInCurrentLine = 0;
    int currentByte = m_dictionaryFile.ReadByte();
    currentByteOffset = 0;
    unsigned numLines = 0;
    while( currentByte != -1 )
    {
      switch( currentByte)
      {
        case DICT_CC_PART_SEPARATOR_CHAR :
          (this->*tabCharLineCallBack)(/*numTabsInCurrentLine*/);
          break;
        case DICT_CC_LINE_END_CHAR : /** newline character */
          numTabsInCurrentLine = 0;
          ++ numLines;
          ///Only set the status for every thousand line (else too much overhead).
          //TODO alternative:set status if x time has passed.
          if( numLines % 1000 == 0)
            //TODO This leads to calling the statistics callback function not
            // for every time.
            (this->*endOfLineCallBack)(p_v);
          break;
        default:
          if( numTabsInCurrentLine == attrIdx)
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
  std::string & POSstring)
{
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
      case DICT_CC_LINE_END_CHAR : /** newline character */
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

///Gets the part of speech string from the part of speech column of a 
/// dictionary line.
void GermanTranslationByteOffset::GetPOSstring(
  const fastestUnsignedDataType byteOffsetOfGermanPart, 
  std::string & POSstring)
{
  enum I_File::SeekResult seekResult = m_dictionaryFile.
    SeekFilePointerPosition(byteOffsetOfGermanPart);
  if( seekResult == I_File::successfullySeeked)
    GetPOSstring(POSstring);
}

/** A dict.cc line may contain multiple word classes, e.g. "adj pres-p" */
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

///\param germanPart: the german tab-separated part of a dictionary line
/// e.g. "Mann {m}" of line "man [male]	Mann {m}	noun"
GermanNoun::grammatical_gender getNounGenderViaRegex(
  const std::string & germanPart)
{
  //TODO create this statically once so it does not need to be done for
  // every noun.
  std::regex nounGenderRegex("\\{.*\\}");
  std::smatch regex_match;
  GermanNoun::grammatical_gender gender = GermanNoun::genderNotSet;
  std::regex_search(germanPart,regex_match, nounGenderRegex);
  if( regex_match.size() > 0 )///regex is contained
  {
    int regexMatchStringStart = regex_match.position(0);
    int regexMatchStringLength = regex_match.length(0);
    ///Remove the gender data ( "{"[...]"}" ) from the string.
//    germanTranslation = germanTranslation.erase(regexMatchStringStart,
//      regexMatchStringLength);
    std::string nounGenderString = regex_match.str(0);
    DictionaryReaderBase::nounGenderString2genderEnumContainerType::
      const_iterator citer = DictionaryReaderBase::
      s_nounGenderString2genderEnum.find(nounGenderString);
    if(citer != DictionaryReaderBase::s_nounGenderString2genderEnum.end() )
    { //Gender found.
      gender = citer->second;
    }
  }
  return gender;
}

///Called from GetStatistics() and GetPartOfSpeeches(fastestUnsignedDataType)
//std::set<enum dict_cc_WordClasses::WordClasses>
std::set<enum EnglishWord::English_word_class>
  GermanTranslationByteOffset::GetPartOfSpeeches(
  const std::string & germanPart,/**contains gender information for nouns*/
  const std::string & POSstring/**POS=part of speech*/
  //TODO additional parameter
  //, const std::vector<std::string> & grammaticalInfo
  )
{
//  std::set<enum dict_cc_WordClasses::WordClasses> wordClasses;
  std::set<enum EnglishWord::English_word_class> wordClasses;
  std::vector<std::string> POStokens = getPOStokens(POSstring);

  std::vector<std::string>::const_iterator POSstringIter = POStokens.begin();
  while( POSstringIter != POStokens.end() )
  {
    /** Empty word class string: idiom*/
    POSstring2POSenumContainerType::const_iterator POSstring2dict_ccPOSenumIter
      = s_POSstring2POSenum.find(*POSstringIter);
    if( POSstring2dict_ccPOSenumIter != s_POSstring2POSenum.end() )
    {
      const enum dict_cc_WordClasses::WordClasses dict_ccPOS = 
        POSstring2dict_ccPOSenumIter->second;
      if(dict_ccPOS == dict_cc_WordClasses::noun)
      {
        GermanNoun::grammatical_gender gender = getNounGenderViaRegex(
          //TODO grammaticalInfo already contains the data in "{}"
          germanPart);
        switch(gender)
         {
         case GermanNoun::masculine:
         case GermanNoun::feminine:
         case GermanNoun::neuter:
//          wordClasses.insert(singNoun);
            wordClasses.insert (EnglishWord::singular); break;
         case GermanNoun::plural:
            wordClasses.insert(EnglishWord::plural_noun); break;
         }
      }
      else
      {
//        wordClasses.insert(POSstring2dict_ccPOSenumIter->second);
        enum EnglishWord::English_word_class wordClass = dict_cc_WordClasses::
          EnglishWordClassFromPOSconverter::GetEngPOS(dict_ccPOS);
        wordClasses.insert(wordClass);
      }
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
      const fastestUnsignedDataType byteOffsetOfGermanPart = citer->second;
//      std::set<enum dict_cc_WordClasses::WordClasses> partOfSpeech = 
//      std::set<EnglishWord::English_word_class> englishWordClassContainer =
//        //TODO m_germanPart is for statistics
//        GetPartOfSpeeches(m_germanPart, byteOffsetOfGermanPart);
//      std::string germanTranslation = GetGermanTranslation(byteOffsetOfGermanPart);
    }
    endTokenIndex ++;
    searchInDictionary = psvStringToSearch.GetBetweenAsStdString(r_dwTokenIndex,
      endTokenIndex);
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

/** @return the content of the column that contains the translation.
 *    The line may also contain other information like the grammatical case 
 *    ,gender for nouns etc. **/
std::string GermanTranslationByteOffset::GetGermanTranslationColumnContent(
  const fastestUnsignedDataType & byteOffsetOfGermanPart)
{
  enum I_File::SeekResult seekResult = m_dictionaryFile.
    SeekFilePointerPosition(byteOffsetOfGermanPart);
  fastestUnsignedDataType byteOffset = byteOffsetOfGermanPart;
  if( seekResult == I_File::successfullySeeked)
  try
  {
  unsigned numTabsInCurrentLine = 0;
  std::string currentAttribute;/** Attributes are separated by a tabulator.*/
  int currentByte = m_dictionaryFile.ReadByte();
  unsigned char ASCIIcodePage850Char;
  bool isByteOfMultiByteUTF8sequence;
  while( currentByte != -1 )
  {
    switch( currentByte)
    {
      case DICT_CC_PART_SEPARATOR_CHAR :
//        numTabsInCurrentLine ++;
        byteOffset ++;
        
        return currentAttribute;
        break;
      default:
//        if( numTabsInCurrentLine == 0)
        isByteOfMultiByteUTF8sequence = 
          UTF8byteSequence::IsByteOfMultiByteUTF8sequence(currentByte);
        if( isByteOfMultiByteUTF8sequence )
        {
          ASCIIcodePage850Char = ISO_8859_1::GetFromUTF8(currentByte);
          if( ASCIIcodePage850Char != 0 )
          {
            switch(ASCIIcodePage850Char)
            {
              case ISO_8859_1::ae :
                currentAttribute += "ae"; break;
              case ISO_8859_1::oe :
                currentAttribute += "oe"; break;
              case ISO_8859_1::ue :
                currentAttribute += "ue"; break;
              case ISO_8859_1::Ae :
                currentAttribute += "Ae"; break;
              case ISO_8859_1::Oe :
                currentAttribute += "Oe"; break;
              case ISO_8859_1::Ue :
                currentAttribute += "Ue"; break;
              case ISO_8859_1::sz :
                currentAttribute += "sz"; break;
            }
          }
        }
        else
         currentAttribute += (char) currentByte;
        byteOffset ++;
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

void GermanTranslationByteOffset::getGermanAndPOSpart(
  const fastestUnsignedDataType byteOffsetOfGermanPart,
  std::string & germanPart, 
  std::string & POSpart)
{
  germanPart = GetGermanTranslationColumnContent(byteOffsetOfGermanPart);
  GetPOSstring(POSpart);
}

///Gets the gender (masc, fem, neut, plur) for a German noun.
//\param germanPart: the middle part/column separated by tabular character in a
// dictionary line
GermanNoun::grammatical_gender GetGrammaticalGender(
  std::string & germanPart)
{
  GermanNoun::grammatical_gender gender = GermanNoun::genderNotSet;
  /** The gender is  often located after the last space char. */
  //TODO the gender is not always located after the last space char:
  // "Bock {m} [Technik]"
  std::string::size_type lastSpaceCharIndex = germanPart.find_last_of('{');
  if( lastSpaceCharIndex != std::string::npos )
  {
    const std::string nounGenderString = germanPart.substr(
      lastSpaceCharIndex + 1);
//    std::string germanPart = germanPart.substr(0, lastSpaceCharIndex);
    
    DictionaryReaderBase::nounGenderString2genderEnumContainerType::
      const_iterator citer = DictionaryReaderBase::
      s_nounGenderString2genderEnum.find(nounGenderString);
    if(citer != DictionaryReaderBase::s_nounGenderString2genderEnum.end() )
    {
      gender = citer->second;
    }
  }
  return gender;
}

///For getting part of speeches we need the German part of a dictionary line
/// as well as the part of speech part: for nouns the info whether this is singular
/// or plural is in the german part: "man [male]	Mann {m}	noun"
//std::set<enum dict_cc_WordClasses::WordClasses>
//std::set<EnglishWord::English_word_class> 
//  GermanTranslationByteOffset::GetPartOfSpeeches(
//  const std::string & germanPart,
//  const fastestUnsignedDataType //byteOffsetOfGermanPart
//    POSpartbyteOffset)
//{
//  std::string POSstring;
//  GetPOSstring(POSpartbyteOffset, POSstring);
////  if( POSstring == "noun")
////    GetGrammaticalGender(germanPart);
//  return GetPartOfSpeeches(POSstring, germanPart);
//}

std::string GetGermanTranslation(const std::string & germanTranslationColumnContent)
{
  std::string germanTranslation;
  /** entry e.g. "gegenüber [+Dat.]"
  *  replace grammatical information in "[.*]" by "" */
  //http://www.cplusplus.com/reference/regex/regex_replace/
  //http://www.informit.com/articles/article.aspx?p=2064649&seqNum=6
   std::regex regex("\\[.*\\]");
   std::string str = std::regex_replace (germanTranslationColumnContent,regex,"");
   //http://www.cplusplus.com/reference/regex/regex_search/
   
//    while (std::regex_search (germanTranslationColumnContent,regex_match,regex)) {
//        regex_match.
//    for (auto x:regex_match) std::cout << x << " ";
////      std::cout << std::endl;
//      s = regex_match.suffix().str();
//    }
//    std::regex regex2("^[\\ ]*.*[\\ ]$");
  std::regex regex2("[\\ ]*$");
  /** Remove space chars at start and end of string. */
  str = std::regex_replace(str,regex2,"");
  return str;
}

std::string extractGrammarInfo(
  const std::string & germanPart, 
  std::regex & grammaticalInfoRegex,
  std::vector<std::string> & grammaticalInfo)
{
  std::smatch regex_match;
  std::regex_search(germanPart,regex_match,grammaticalInfoRegex);
  //http://www.cplusplus.com/reference/regex/match_results/position/
  ///Traverse all regex matches
  for (unsigned regexMatchIndex = 0; regexMatchIndex < regex_match.size(); 
    ++regexMatchIndex)
  {
    int regexMatchStringStart = regex_match.position(regexMatchIndex);
    int regexMatchStringLength = regex_match.length(regexMatchIndex);
    std::string grammarInfo = germanPart.substr(regexMatchStringStart, 
      regexMatchStringLength);
    grammaticalInfo.push_back(grammarInfo);
  }
  std::string gerPartWoutGrammarInfo = std::regex_replace(germanPart, 
    grammaticalInfoRegex,"");
  return gerPartWoutGrammarInfo;
}

///Gets
///-the German word/translation
///-grammatical info like noun gender
std::string TokenizeGermanPart(const std::string & germanPart,
  std::vector<std::string> & grammaticalInfo)
{
  std::string germanWord;
  /** entry e.g. "gegenüber [+Dat.]"
  *  replace grammatical information in "[.*]" by "" */
  //http://www.cplusplus.com/reference/regex/regex_replace/
  //http://www.informit.com/articles/article.aspx?p=2064649&seqNum=6
  std::regex grammaticalInfoRegex("\\[.*\\]");
  std::string gerPartWoutGrammarInfo = extractGrammarInfo(germanPart,
    grammaticalInfoRegex, grammaticalInfo);
  
  std::regex grammaticalInfoRegex2("\\{.*\\}");
  gerPartWoutGrammarInfo = extractGrammarInfo(gerPartWoutGrammarInfo,
    grammaticalInfoRegex2, grammaticalInfo);  
  
  std::regex spaceCharsAtEndRegex("[\\ ]*$");
  /** Remove space chars at start and end of string. */
  germanWord = std::regex_replace(gerPartWoutGrammarInfo,spaceCharsAtEndRegex,"");  
  return germanWord;
}

///Adds grammatical information from the dictionary to a 
/// VocabularyAndTranslation object.
std::string AddData(
  std::string & germanPart, 
  VocabularyAndTranslation * p_vocabularyAndTranslation)
{
//    std::string germanTranslation;
//   std::string germanTranslation = germanTranslationColumnContent;
//  std::regex regexForGrammaticalInfo("\\[.*\\]");
//  std::smatch regex_match;
//  std::regex_search(germanPart,regex_match,regexForGrammaticalInfo);
   
  std::vector<std::string> grammaticalInfo;
  //TODO regex "{.*}" is done before in getGermanAndPOSpart(...)
  std::string germanWord = TokenizeGermanPart(germanPart, grammaticalInfo);

   //http://www.cplusplus.com/reference/regex/match_results/position/
//   ///Traverse all regex matches
//   for(unsigned regexMatchIndex = 0; regexMatchIndex < regex_match.size(); 
//    ++regexMatchIndex)
   for(auto grammarInfo : grammaticalInfo)
   {
//    std::cout << "match " << i << " (" << regex_match[i] << ") ";
//    int regexMatchStringStart = regex_match.position(regexMatchIndex);
//    int regexMatchStringLength = regex_match.length(regexMatchIndex);

    GCC_DIAG_OFF(switch)
    switch(p_vocabularyAndTranslation->m_englishWordClass)
    {
      case EnglishWord::preposition :
      case EnglishWord::main_verb :
//        std::string regexMatchString = regex_match.str(regexMatchIndex);
        //TODO add other grammatical cases
        if( /*regexMatchString*/grammarInfo.find("Dat.") != std::string::npos)
        {
          p_vocabularyAndTranslation->m_arbyAttribute[0] = GermanVerb::dative;
        }
        break;
    }
    GCC_DIAG_ON(switch)

//    germanTranslation.erase(regexMatchStringStart, regexMatchStringLength);
//    std::cout << "at position " << regex_match.position(i) << std::endl; 
  }
//  ///Removes regexForGrammaticalInfo from German part
//  std::string germanTranslation = std::regex_replace(
//    germanPart, regexForGrammaticalInfo, "");
  
  GCC_DIAG_OFF(switch)
  switch(p_vocabularyAndTranslation->m_englishWordClass)
  {
    case EnglishWord::noun : case EnglishWord::singular :
    {
//        GermanNoun::grammatical_gender gender = GetGrammaticalGender(germanTranslationColumnContent);
      const GermanNoun::grammatical_gender gender = 
        //TODO replace by searching in "grammaticalInfo"
        //TODO regex "{.*}" is done before in getGermanAndPOSpart(...)
        getNounGenderViaRegex(germanPart);
      p_vocabularyAndTranslation->SetAttributeValue(0, gender);
    }
    break;
  }
  GCC_DIAG_ON(switch)

//  std::regex spaceCharsAtEndRegex("[\\ ]*$");
//  /** Remove space chars at start and end of string. */
//  germanTranslation = std::regex_replace(germanTranslation,spaceCharsAtEndRegex,"");

  //   regex_match::string::const_iterato
  return /*germanTranslation*/germanWord;
}

void GermanTranslationByteOffset::GetCollectDictionaryStatisticsStatus(
  fastestUnsignedDataType & currentItemNo)
{
  currentItemNo = m_dictionaryFile.GetCurrentFilePointerPosition();
}

void GermanTranslationByteOffset::GetStatistics(
  std::map<enum EnglishWord::English_word_class, unsigned> &
	englishWordClass2CounterMap)
{
  readWholeFile(
    ///https://stackoverflow.com/questions/18145874/passing-a-pointer-to-a-class-member-function-as-a-parameter/18152250
    &GermanTranslationByteOffset::GetStatsTabChar,
    &GermanTranslationByteOffset::GetStatsLineEnd,
    & englishWordClass2CounterMap,
    partOfSpeech
    );
  #ifdef _DEBUG
    int i = 0;
  #endif
}

VocablesForWord::voc_container_type * GermanTranslationByteOffset::findEnglishWord(
  const std::string & englishWord
  )
{
  VocablesForWord::voc_container_type * p_vocContainer = 
    new VocablesForWord::voc_container_type;
  VocablesForWord::voc_container_type & vocContainer = * p_vocContainer;
//  container_type::const_iterator citer = m_germanTranslationByteOffsetIndex.
//    find(englishWord);
//  if( citer != m_germanTranslationByteOffsetIndex.end() )
  std::pair<container_type::const_iterator, container_type::const_iterator> 
    firstAndLastFound = m_germanTranslationByteOffsetIndex.equal_range(englishWord);
//  int num = firstAndLastFound.second - firstAndLastFound.first; 
#ifdef _DEBUG
  int numberOfSameWords = m_germanTranslationByteOffsetIndex.count(englishWord);
#endif
  for(container_type::const_iterator citer = firstAndLastFound.first; 
    citer != firstAndLastFound.second; ++citer)
  {
    fastestUnsignedDataType byteOffsetOfGermanPart = citer->second;
//    const std::string gerTranslColContent =
//      GetGermanTranslationColumnContent(byteOffsetOfGermanPart);
    std::string gerTranslColContent;
    std::string POSpart;
    getGermanAndPOSpart(byteOffsetOfGermanPart,
      gerTranslColContent, POSpart);
    
    std::string germanTranslation = GetGermanTranslation(gerTranslColContent);
    //TODO replace by:
//    std::vector<std::string> grammaticalInfo;
//    std::string germanTranslation = TokenizeGermanPart(gerTranslColContent,
//      grammaticalInfo);
    
//    std::set<enum dict_cc_WordClasses::WordClasses> partOfSpeechContainer =
    std::set<EnglishWord::English_word_class> englishWordClassContainer =
      GetPartOfSpeeches(gerTranslColContent, /*byteOffsetOfGermanPart*/POSpart);
//    std::set<EnglishWord::English_word_class> englishWordClassContainer = 
//      dict_cc_WordClasses::EnglishWordClassFromPOSconverter::
//      GetEnglishWordClass(partOfSpeechContainer);
    for( std::set<EnglishWord::English_word_class>::const_iterator 
      englishWordClassIter = englishWordClassContainer.begin(); 
      englishWordClassIter != englishWordClassContainer.end() ; 
      englishWordClassIter++ )
    {
      const EnglishWord::English_word_class English_word_class = 
        *englishWordClassIter;
      VocabularyAndTranslation * p_vocabularyAndTranslation = new 
        VocabularyAndTranslation(English_word_class);
      
//      if(English_word_class == EnglishWord::noun)
//      {
//        GermanNoun::grammatical_gender gender = GetGrammaticalGender(germanTranslationColumnContent);
//        p_vocabularyAndTranslation->SetAttributeValue(0, gender);
//      }
//      p_vocabularyAndTranslation->SetGermanWord(germanTranslation, 0);
      germanTranslation = AddData(gerTranslColContent, p_vocabularyAndTranslation);
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
