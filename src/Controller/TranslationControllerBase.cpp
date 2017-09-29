/*
 * TranslationControllerBase.cpp
 *
 *  Created on: Dec 6, 2010
 *      Author: Stefan
 */

/** Must be the 1st inclusion, else compilation error in GCC files <time.h>
 *  indirectly included from this header file: ~ "undefined asctime", ... */
#include <InputOutput/XML/OutputXMLindented_inl.hpp> //OutputXMLindented(...)
#include <InputOutput/ReadFileContent/ReadFileContent.hpp>
#include <Controller/character_string/stdtstr.hpp> //GetStdString_Inline(...)
#include <OperatingSystem/multithread/GetCurrentThreadNumber.hpp>
#include <Controller/TranslationControllerBase.hpp>
#include <Controller/time/GetTickCount.hpp>
#include <data_structures/ByteArray.hpp>
#include <data_structures/UTF8string.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h> //
//GenerateXMLtreeFromParseTree(...)
#include <IO/GenerateXMLtreeFromParseTree.hpp>
//#include <IO/dictionary/VTransDictFormatReader.hpp> //class OneLinePerWordPair
//class TUchemnitzDictionaryReader
//#include <IO/dictionary/TUchemnitzDictionaryReader.hpp>
#include <Controller/DictReaderAndVocAccess/TUchemnitzEngWordSorted1stAndBinarySearch.hpp>
////class TUchemnitzDictEngWord2LineNumberReader
//#include <IO/dictionary/TUchemnitzDictEngWord2LineNumberReader.hpp>
//class ParseTreeTraverser::TransformTreeTraverser
#include <Translate/TransformTreeTraverser.hpp>
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
//#ifdef /*__ANDROID__*/ __unix__
  //SetCurrentDirectory(...)
  #include <FileSystem/GetCurrentWorkingDir.hpp>
  #include <FileSystem/SetCurrentWorkingDir.hpp>
#include <hardware/CPU/GetNumberOfLogicalCPUcores.h>
#include <FileSystem/file_exists.h>

#include "Controller/character_string/ConvertStdStringToTypename.hpp"
#ifdef PARALLELIZE_TRANSLATION
#include "multi_threaded_translation/nativeThreads.hpp"
#endif //#ifdef PARALLELIZE_TRANSLATION
//#else
//  //platformstl::filesystem_traits<char>::set_current_directory(...)
//  #include <platformstl/filesystem/current_directory.hpp>
//  //#include <unixstl/filesystem/filesystem_traits.hpp>
//  #include <platformstl/filesystem/filesystem_traits.hpp>
//#endif
#ifdef COMPILE_WITH_OPENMP
  #include <omp.h>
#endif

#ifndef MAX_PATH
  #define MAX_PATH 2000
#endif

/** Static variables need also to be defined in 1 source file. */
I_UserInterface * SyntaxTreePath::sp_userinterface ;
I_UserInterface * VocabularyAndTranslation::s_p_userinterface;
fastestUnsignedDataType TranslationControllerBase::s_numParallelTranslationThreads = 1;
TranslationControllerBase::settingsName2ValueAndFunction_type TranslationControllerBase::s_settingsName2valueAndFunction;
#ifdef EVALUATE_PROCESSING
std::map<std::string, TranslateParseByRiseTree::ProcessParseTree_type> 
  TranslationControllerBase::s_functionName2function;
TranslateParseByRiseTree::ProcessParseTree_type TranslationControllerBase::
  s_3rdStepTranslationFunction = & TranslateParseByRiseTree::TranslateParseTree;
fastestUnsignedDataType TranslationControllerBase::s_num3rdTranslationStepIterations = 1;
#endif //#ifdef EVALUATE_PROCESSING


extern TranslationControllerBase * g_p_translationcontrollerbase;
//LetterTree VTransApp::s_dictionary ;
///*LetterTree*/dictionary_type TranslationControllerBase::s_dictionary
  //(::wxGetApp(), NULL)
  ;
//dictReaderAndVocAccess_type TranslationControllerBase::s_dictReaderAndVocAccess;

//#if defined __cplusplus
//extern "C" {
//#endif

void MakeFemale(std::string & r_stdstr )
{
  //e.g. "klein" E
  r_stdstr += "e";
}

void MakeMale(std::string & r_stdstr )
{
  //e.g. "klein" "ER"
  r_stdstr += "er";
}

/** Make German genus? from positive e.g. "klein" "ES"
 * @param r_stdstr: word stem/ positive form of an adverb */
void MakeNeutral(std::string & r_stdstr )
{
  //e.g. "klein" "ES"
  r_stdstr += "es";
}

#ifndef TEST_MINI_XML
/** Make 1st person singular */
void Make1stPersonSing(std::string & //r_std_strWordStem
  r_std_strInfinitive)
{
  std::string std_strWordStem;
  GermanVerb::GetWordStemFromInfinitive(
    r_std_strInfinitive.c_str(),
    r_std_strInfinitive.length(),
    std_strWordStem );
  //e.g. "arbeit" "E"
  r_std_strInfinitive = GermanVerb::GetPresentFiniteForm(std_strWordStem,
    (enum GermanVerb::person_indices) (GermanVerb::firstPersonSing ) );
}

/** Make German past participle from infinitive.
 *   sehen->GEsehen  ansehen->anGEsehen
 *   speichern->GEspeicherT schiCKen->GEschickT tauSCHen -> GEtauschT
 *   klauen->GEklauT
 *   reiten –> geritten
 *   */
void MakePastParticiple(std::string & //r_std_strWordStem
  r_std_strInfinitive)
{
  //ending consonant + "en" -> "ge" + infinitive
  //if begin = preposition ("an", "zu" etc.): preposition + "ge" + inf.
  std::string std_strWordStem;
  GermanVerb::GetWordStemFromInfinitive( r_std_strInfinitive.c_str(),
    r_std_strInfinitive.length(), std_strWordStem );
  //TODO get past participle, but not finite simple past form
  //e.g. "arbeit" "E"
  r_std_strInfinitive = GermanVerb::GetPastFiniteForm(
    std_strWordStem.c_str(), std_strWordStem.length(),
    (enum GermanVerb::person_indices) (GermanVerb::firstPersonSing ) );
}
#endif //#ifndef TEST_MINI_XML
//#if defined __cplusplus
//}
//#endif

void SetLogLevel(const char * const value)
{
  std::string logLevel;
  try
  {
    if( strcmp(value, "disable") == 0 )
      logLevel = "warning";
    else if( strcmp(value, "enable") == 0 )
      logLevel = "debug";
    else
      logLevel = value;
    if( ! logLevel.empty() )
      g_logger.SetLogLevel(logLevel);
  }
  catch(NS_NodeTrie::NotInContainerException & e)
  {
    LOGN_ERROR("log level " << logLevel << " to set is not contained in node trie.")
  }
}

void SetInput(const char * const value)
{
  if( FileExists(value) )
  {
//    std::string str;
//    uint8_t * buffer;
//    fastestUnsignedDataType length;
    ByteArray byteArray;
    if( ReadFileContent(value, /*buffer, length*/ byteArray) == 1 )
      g_p_translationcontrollerbase->m_englishText = std::string( 
        (char *) byteArray.GetArray(), byteArray.GetSize() );
  }
  else
    g_p_translationcontrollerbase->m_englishText = std::string(value);
}

void SetNumTranslationThreads(const char * const value)
{
  fastestUnsignedDataType numParallelTranslationThreads;
  if( ConvertCharStringToTypename(numParallelTranslationThreads, value) )
  {
    std::cout << "setting # translation threads to " << value << std::endl;
    g_p_translationcontrollerbase->SetNumberOfParallelTranslationThreads(
      numParallelTranslationThreads);
  }
  else
    std::cerr << " error converting # of translation threads from \"" << value 
      << "\" to a number" << std::endl;
}

void Set3rdTranslationStepFunction(const char value[])
{
  std::map<std::string, TranslateParseByRiseTree::
    ProcessParseTree_type>::const_iterator c_iter =
    TranslationControllerBase::s_functionName2function.find(value);
  if( c_iter != TranslationControllerBase::s_functionName2function.end() )
    TranslationControllerBase::s_3rdStepTranslationFunction = c_iter->second;
}

void SetNumIterationsFor3rdTranslationStep(const char value[])
{
  if( ! ConvertCharStringToTypename(
    TranslationControllerBase::s_num3rdTranslationStepIterations, value) )
    std::cerr << "error converting \"" << value 
      << "\" to a number in SetNumIterationsFor3rdTranslationStep" << std::endl;
}

void AddSettingsName2ValueAndFunctionMapping()
{
  ValuesAndFunction valuesAndFunction = { (char *) "disable\tenable", & SetLogLevel};
  TranslationControllerBase::s_settingsName2valueAndFunction.insert(
    std::make_pair("logging", valuesAndFunction) );

  ValuesAndFunction valuesAndFunction2 = { (char *) "", & SetNumTranslationThreads };
  TranslationControllerBase::s_settingsName2valueAndFunction.insert(
    std::make_pair("#translThreads", valuesAndFunction2) );
  
  ValuesAndFunction valuesAndFunction3 = { (char *) "", & SetInput};
  TranslationControllerBase::s_settingsName2valueAndFunction.insert(
    std::make_pair("input", valuesAndFunction3) );
  
  ValuesAndFunction valuesAndFunction4 = { (char *) "", & SetNumIterationsFor3rdTranslationStep};
  TranslationControllerBase::s_settingsName2valueAndFunction.insert(
    std::make_pair("#num3rdTranslStepIterations", valuesAndFunction4) );
}

TranslationControllerBase::TranslationControllerBase()
  :
//  m_multiThreadedTranslation( OperatingSystem::GetNumberOfLogicalCPUcores() ),
//  m_boost_threadpool(8),
  m_vbContinue(true),
#ifndef TEST_MINI_XML
  m_parsebyrise( * this ) ,
#endif
  m_nodetrie_ui32GrammarPartName2colour(256, 0),
#ifndef TEST_MINI_XML
  m_translateparsebyrisetree(
    m_parsebyrise
    , * this
    ),
#endif  //#ifndef TEST_MINI_XML
  m_configurationHandler(*this),
  m_dictionarySuccessfullyLoaded(false)
{
#ifdef PARALLELIZE_TRANSLATION
  SetNumberOfParallelTranslationThreads(OperatingSystem::
    GetNumberOfLogicalCPUcores() );
//  m_multiThreadedTranslation.CreateAndStartThreads();
//  /** Call this method as early as possible as thread creation may take some
//   *  (the more threads the more time). */
//  m_multiThreadedTranslation.StartThreadsAsync();
#endif
  GrammarPart::s_p_parseByRise = & m_parsebyrise;
#ifndef TEST_MINI_XML
  VocabularyAndTranslation::s_p_translationControllerBase = this;
#endif
#ifndef __ANDROID__
  /** Can be used for executing GUI operations: if a GUI control action (e.g.
   *  showing a dialog) should be performed then the caller's thread can be
   *  compared to this thread ID and only if it is the same the GUI action is
   *  executed.*/
  m_GUIthreadID = OperatingSystem::GetCurrentThreadNumber();
#endif
//  m_nodetrie_ui32GrammarPartName2colour.Create(256);
//  OneLinePerWordPair::s_p_userinterface = this;

  
  ConditionsAndTranslation::s_p_translationControllerBase = this;
#ifndef TEST_MINI_XML
  std::string std_str = "makeFemale";
  ConditionsAndTranslation::s_std_mapFunctionName2Function.insert(
    std::make_pair(std_str, //(pfnTransformString)
    & MakeFemale) );
  std_str = "makeMale";
  ConditionsAndTranslation::s_std_mapFunctionName2Function.insert(
    //"makeFemale"
    std::make_pair(std_str, & MakeMale) );
  std_str = "makeNeutral";
  ConditionsAndTranslation::s_std_mapFunctionName2Function.insert(
    //"makeFemale",
    std::make_pair(std_str, & MakeNeutral) );
  std_str = "make1stPersonSing";
  ConditionsAndTranslation::s_std_mapFunctionName2Function.insert(
    std::make_pair(std_str, & Make1stPersonSing) );
  std_str = "makePastParticiple";
  ConditionsAndTranslation::s_std_mapFunctionName2Function.insert(
    std::make_pair(std_str, & MakePastParticiple) );
#endif //#ifndef TEST_MINI_XML
  AddSettingsName2ValueAndFunctionMapping();
}

TranslationControllerBase::~TranslationControllerBase()
{
  LOGN_DEBUG("begin")
  // TODO Auto-generated destructor stub
#ifndef TEST_MINI_XML
  IVocabularyInMainMem & r_vocAccess = s_dictReaderAndVocAccess.GetVocAccess();
  r_vocAccess.clear(); //DeleteCompleteList();
#endif //TEST_MINI_XML
  LOGN_DEBUG("end")
}

bool TranslationControllerBase::CurrentThreadIsGUIthread()
{
  const DWORD currentThreadNumber = OperatingSystem::GetCurrentThreadNumber();
  if( currentThreadNumber == m_GUIthreadID )
    return true;
  return false;
}

BYTE TranslationControllerBase::Init(const std::string & cr_stdstrMainConfigFilePath)
{
  LOGN_DEBUG("begin")
//TODO : uncomment?
//  if(cr_stdstrMainConfigFilePath.empty() )
//    std_strMainConfigFilePath = "configuration/VTrans_main_config.xml";
  VocabularyAndTranslation::s_p_userinterface = this;
  /** Needed for SyntaxTreePath::CreateGrammarPartIDArray(...).*/
  SyntaxTreePath::sp_userinterface = this ;
#ifndef TEST_MINI_XML
  IVocabularyInMainMem & r_VocAccess = s_dictReaderAndVocAccess.GetVocAccess();
  r_VocAccess.SetUserInterface(this);
  r_VocAccess.InsertFundamentalWords() ;
#endif  //#ifndef TEST_MINI_XML

  bool readMainConfigFileSucceeded = true;
//#ifndef __ANDROID__
  readMainConfigFileSucceeded = ReadMainConfigFile(cr_stdstrMainConfigFilePath);
//#endif
  if( readMainConfigFileSucceeded )
  {
#ifdef TEST_MINI_XML
  return 4;
#else
    m_std_strMainConfigFilePath = cr_stdstrMainConfigFilePath;
    if( /*m_configurationHandler.*/m_stdstrVocabularyFilePath.empty() )
    {
      Message( "error: The vocabulary file path is empty") ;
#ifndef COMPILE_AS_EXECUTABLE
      return TranslationControllerBaseClass::InitFunction::
        vocabularyFilePathIsEmpty;
#endif
    }
//    else
    {
#ifndef __ANDROID__
      SetCurrentDirToOriginalCurrentWorkingDir();
#endif

//      TUchemnitzDictionaryReader::s_p_vocinmainmem = & s_dictionary ;

//      DictionaryReader dictReader;
      CreateAndShowMainWindow() ;
  #ifdef COMPILE_AS_EXECUTABLE
  #else
      std::string std_strCurrentWorkingDir;
      OperatingSystem::GetCurrentWorkingDirA_inl(std_strCurrentWorkingDir);
      LOGN_INFO("current dir is:\"" << std_strCurrentWorkingDir << "\"")
//	TUchemnitzDictionaryReader dictReader(* this, & s_dictionary );
//	VTrans3::TUchemnitzEngWordSorted1stAndBinarySearch
//	  tuchemnitzengwordsorted1standbinarysearch;
//      DictionaryReader::TUchemnitzDictEngWord2LineNumberReader dictReader(
//        * this, & s_dictionary);
//      TUchemnitzDictEngWord1stReader & dictReader = (* this, s_dictionary);
  //    StartTimer();
      bool bLoadingDictFileSucceeded = /*TUchemnitzDictionaryReader::*/ //tcdr.extractVocables(
//        dictReader
//        tuchemnitzengwordsorted1standbinarysearch
        s_dictReaderAndVocAccess
        .loadDictionary(
//      bool b = dictReader::read(
        m_stdstrVocabularyFilePath.c_str() );
      LOGN_INFO( "dictionary file is open:" << (bLoadingDictFileSucceeded ? "yes" : "no") )
      LOGN_INFO( "# of vocable pairs:" << s_numberOfVocabularyPairs )

      if( ! bLoadingDictFileSucceeded )
        return TranslationControllerBaseClass::InitFunction::loadingVocabularyFileFailed;
  #endif //#ifndef COMPILE_AS_EXECUTABLE
  ////    StartTimer();
    }
#endif
  }
  else
    return TranslationControllerBaseClass::InitFunction::loadingMainConfigFileFailed;
  LOGN_INFO("waiting for termination of the thread starter thread.")
#ifdef PARALLELIZE_TRANSLATION
  /** Ensure all threads for multithreaded translation have been created
   *  before we return. */
  m_multiThreadedTranslation.m_asyncThreadStarterThread.WaitForTermination();
#endif
  LOGN_INFO("AFTER waiting for termination of the thread starter thread.")
  return TranslationControllerBaseClass::InitFunction::success;
}

enum VTrans::StatusCode TranslationControllerBase::GetStatus( /*const std::string & str*/
  std::string & str, struct tm & time)
{
  LOGN_DEBUG("begin")
  VTrans::StatusCode statusCode;

  m_critSecStatus.Enter();
  if( str == "dictionaryStatistics" )
  {

  }
  else
  {
    m_currentStatus.GetItem(str);
    statusCode = m_currentStatus.GetCode();
    m_currentStatus.GetTime(time);
  }
  m_critSecStatus.Leave();
  LOGN_DEBUG("return status code:" << statusCode << " " << str)
  return statusCode;
}

unsigned TranslationControllerBase::GetStatus2( /*const std::string & str*/
  const std::string & str, struct tm & time, ByteArray & byteArray
  //void * pData
  )
{
  LOGN_DEBUG("begin")
  VTrans::StatusCode statusCode;

  if( str == "dictionaryStatistics" )
  {
    const unsigned sizeInBytes = sizeof(m_collectDictionaryStatisticsStatus);
    unsigned char * bytes = new unsigned char[sizeInBytes];
    m_critSecStatus.Enter();
    memcpy(bytes, & m_collectDictionaryStatisticsStatus, sizeInBytes);
    m_critSecStatus.Leave();
    byteArray.add(bytes, sizeInBytes);
  }
  else
  {
    m_critSecStatus.Enter();
//    m_currentStatus.GetItem(str);
    statusCode = m_currentStatus.GetCode();
    m_currentStatus.GetTime(time);
    m_critSecStatus.Leave();
  }
  LOGN_DEBUG("return status code:" << statusCode << " " << str)
  return statusCode;
}

void TranslationControllerBase::SetNumberOfParallelTranslationThreads(
  fastestUnsignedDataType numberOfParallelTranslationThreads)
{
  m_numThreadsAndTimeDuration[applyTranslRules].numThreads = 
    numberOfParallelTranslationThreads;
#ifdef PARALLELIZE_TRANSLATION
#ifdef COMPILE_WITH_OPENMP
  omp_set_num_threads(numberOfParallelTranslationThreads);
#else //#ifdef COMPILE_WITH_OPENMP
  m_multiThreadedTranslation.SetNumberOfThreads(
    numberOfParallelTranslationThreads);
  /** Call this method as early as possible as thread creation may take some
   *  (the more threads the more) time. */
  m_multiThreadedTranslation.StartThreadsAsync();
#endif //#ifdef COMPILE_WITH_OPENMP
#endif //#ifdef PARALLELIZE_TRANSLATION
}

void TranslationControllerBase::SetStatus(
  enum VTrans::StatusCode statusCode,
//  const std::string & str
  const char * const pch
  )
{
  LOGN_DEBUG("begin--status code:" << statusCode << " " << pch)
  m_critSecStatus.Enter();
  m_currentStatus.Set(statusCode, /*str*/ pch);
  m_critSecStatus.Leave();
}

void TranslationControllerBase::TranslateAsXML(const char * p_chEnglishText, ByteArray & byteArray)
{
  g_p_translationcontrollerbase->m_vbContinue = true;
  char * ar_chTranslation;
  std::string stdstrWholeInputText(p_chEnglishText);
  std::string stdstrAllPossibilities ;
  std::vector<std::string> stdvec_stdstrWholeTransl;
  TranslationResult translationResult;
  Translate(
    stdstrWholeInputText,
    stdvec_stdstrWholeTransl,
//    translationResult
    & TranslateParseByRiseTree::TranslateParseTree,
    1
    );

  stdstrAllPossibilities = "";
  IO::GenerateXMLtreeFromParseTree( & m_parsebyrise,
    /*stdstrAllPossibilities*/ byteArray);
//  ar_chTranslation = new char[stdstrAllPossibilities.length() + 1];
//  if( ar_chTranslation )
//  {
//    memcpy(ar_chTranslation, stdstrAllPossibilities.c_str(),
//      stdstrAllPossibilities.length() );
//    ar_chTranslation[ stdstrAllPossibilities.length()] = '\0';
//  }
}

void TranslationControllerBase::TranslateAsXMLgetAverageTimes(
  const char * p_chEnglishText, 
  ByteArray & byteArray,
  TranslateParseByRiseTree::ProcessParseTree_type pfnProcessParseTree,
  fastestUnsignedDataType numTimesExecute3rdTranslationStep
  )
{
  g_p_translationcontrollerbase->m_vbContinue = true;
  char * ar_chTranslation;
  std::string stdstrWholeInputText(p_chEnglishText);
  std::string stdstrAllPossibilities ;
  std::vector<std::string> stdvec_stdstrWholeTransl;
  TranslationResult translationResult;
  Translate(
    stdstrWholeInputText,
    stdvec_stdstrWholeTransl,
    pfnProcessParseTree,
    numTimesExecute3rdTranslationStep
    );

  stdstrAllPossibilities = "";
  IO::GenerateXMLtreeFromParseTree( & m_parsebyrise,
    /*stdstrAllPossibilities*/ byteArray);
//  ar_chTranslation = new char[stdstrAllPossibilities.length() + 1];
//  if( ar_chTranslation )
//  {
//    memcpy(ar_chTranslation, stdstrAllPossibilities.c_str(),
//      stdstrAllPossibilities.length() );
//    ar_chTranslation[ stdstrAllPossibilities.length()] = '\0';
//  }
}

#ifndef TEST_MINI_XML
void TranslationControllerBase::ReadGrammarRuleFile(
  //SAX2GrammarRuleHandler & r_sax2grammarrulehandler ,
  const std::string & cr_stdstrFilePath
  )
{
  m_configurationHandler.ReadGrammarRuleFile(cr_stdstrFilePath);
}
#endif //#ifndef TEST_MINI_XML

bool TranslationControllerBase::ReadMainConfigFile(
  const std::string & cr_stdstrFilePath )
{
  std::string std_strCurrentWorkingDir;
  OperatingSystem::GetCurrentWorkingDirA_inl(std_strCurrentWorkingDir);
  LOGN_DEBUG("begin--should open file \"" +  std_strCurrentWorkingDir +
    cr_stdstrFilePath )
  return m_configurationHandler.ReadMainConfigFile(cr_stdstrFilePath.c_str() );
}

#ifndef TEST_MINI_XML
void TranslationControllerBase::ReadTranslationRuleFile(
//  SAX2TranslationRuleHandler & r_sax2translationrulehandler ,
  const std::string & cr_stdstrFilePath
  )
{
  m_configurationHandler.ReadTranslationRuleFile(cr_stdstrFilePath.c_str() );
}

void TranslationControllerBase::ReadVocAttributeDefinitionFile(
//  Xerces::SAX2VocAttributeDefinitionHandler &
//    r_sax2vocattributedefinitionhandler ,
  const std::string & cr_stdstrFilePath
  )
{
  LOGN_DEBUG("begin")
  m_configurationHandler.ReadVocAttributeDefinitionFile(cr_stdstrFilePath);
}
#endif //#ifndef TEST_MINI_XML

void TranslationControllerBase::ReadXMLfile(
  XERCES_CPP_NAMESPACE::DefaultHandler & r_xercesc_defaulthandler ,
  const std::string & cr_stdstrFilePath
  )
{
  LOGN("\"" << cr_stdstrFilePath << "\")" )
  std::wstring stdwstrErrorMessage ;
//  // <> 0 = error
//  if( //ReadViaSAX2InitAndTermXerces(
//      ! Apache::Xerces::ReadXMLfileInitAndTermXerces_Inline(
//      //"translation_rules.xml",
//      cr_stdstrFilePath.c_str() ,
//  //    p_sax2grammarrulehandler ,
//      & r_xercesc_defaulthandler ,
//      stdwstrErrorMessage
//      )
//    )
//  {
//    LOGN("Successfully read XML file \"" << cr_stdstrFilePath
//      << "\"" )
//  //          mr_i_userinterface.Message( wstr ) ;
//  }
//  else
//  {
//    LOGN_ERROR("Failed to read XML file \"" << cr_stdstrFilePath << "\"" )
//    m_translateparsebyrisetree.mr_i_userinterface.Message(
//      stdwstrErrorMessage ) ;
//  }
}

void TranslationControllerBase::SetCurrentDirToOriginalCurrentWorkingDir()
{
  LOGN("setting current dir to \"" << m_std_strOriginalCurrWorkDir << "\"")
//  platformstl::filesystem_traits<char>::set_current_directory(
//    m_std_strOriginalCurrWorkDir.c_str() );
  OperatingSystem::SetCurrentWorkingDirA_inl(m_std_strOriginalCurrWorkDir.c_str());
//  char buffer[MAX_PATH];
//  platformstl::filesystem_traits<char>::get_current_directory(buffer, MAX_PATH);
//  LOGN("current dir is now: " << buffer )

  m_std_strOriginalCurrWorkDir = /*std::string(buffer);*/ GetCurrentWorkingDir();
  LOGN("current dir is now: \"" << m_std_strOriginalCurrWorkDir << "\"")
}

std::string TranslationControllerBase::GetCurrentWorkingDir()
{
//  char buffer[MAX_PATH];
//  platformstl::filesystem_traits<char>::get_current_directory(buffer, MAX_PATH);
//  std::string std_strCurrWorkDir = /*std::string(buffer);*/
  std::string std_strCurrWorkDir;
  OperatingSystem::GetCurrentWorkingDirA_inl(std_strCurrWorkDir);
  return std_strCurrWorkDir;
}

fastestUnsignedDataType TranslationControllerBase::GetNumberOfParseTrees(
  TranslationResult & translationResult )
{
  fastestUnsignedDataType numParseTrees = 0;
  TranslationResult::const_iterator c_iterBeginTokenIndexOfParseTree =
    translationResult.begin();
//    for(fastestUnsignedDataType beginTokenIndexOfParseTree = 0;
//      beginTokenIndexOfParseTree <
//      stdvec_stdvec_stdvecTranslationAndGrammarPart.size() ;
//      ++ beginTokenIndexOfParseTree )
  while( c_iterBeginTokenIndexOfParseTree != translationResult.end() )
  {
    WordCompoundsAtSameTokenIndex::const_iterator
      c_iterParseTreeAtSameTokenIndex = c_iterBeginTokenIndexOfParseTree->begin();
    while( c_iterParseTreeAtSameTokenIndex !=
        c_iterBeginTokenIndexOfParseTree->end() )
    {
      WordCompound::const_iterator c_iterTranslationAndGrammarPart =
        c_iterParseTreeAtSameTokenIndex->begin();
      while( c_iterTranslationAndGrammarPart != c_iterParseTreeAtSameTokenIndex->end() )
      {
        ++ numParseTrees;
        ++ c_iterTranslationAndGrammarPart;
      }
      ++ c_iterParseTreeAtSameTokenIndex;
    }
    ++ c_iterBeginTokenIndexOfParseTree;
  }
  return numParseTrees;
}

inline void SetCurrentDirViaPlatformSTL(
  const std::string & c_r_stdstrConfigFilesRootPath)
{
  //  platformstl::current_directory cwd; (//char_type const *dir
  //    stdstrConfigFilesRootFullDirectoryPath.c_str() );

  //  char buffer[MAX_PATH];
  //  platformstl::filesystem_traits<char>::get_current_directory(buffer, MAX_PATH);
  //  m_std_strOriginalCurrWorkDir = std::string(buffer);
  //  platformstl::filesystem_traits<char>::set_current_directory(
  //    stdstrConfigFilesRootFullDirectoryPath.c_str());
  //  cwd.
}

void TranslationControllerBase::SetCurrentDirToConfigFilesRootPath(
  const std::string & c_r_stdstrConfigFilesRootPath)
{
  std::string stdstrConfigFilesRootFullDirectoryPath =
    //stdstrMainConfigFilePath.substr( 0, wIndexOfLastSlashOrBackSlash);
    c_r_stdstrConfigFilesRootPath;

  LOGN("Before setting current directory to \"" //main config file's full path"
    << stdstrConfigFilesRootFullDirectoryPath << "\"")
  //::SetCurrentDirectory(stdstrMainConfigFileFullDirectoryPath.c_str()
//  SetCurrentDirectory( (const char *) stdstrConfigFilesRootFullDirectoryPath.
//      c_str()
//    );
//#ifdef /*__ANDROID__*/ __unix__
  //::_chdir(dir);
  OperatingSystem::SetCurrentWorkingDirA_inl(c_r_stdstrConfigFilesRootPath.c_str() );
//#else
//#endif
  LOGN("After setting current directory to \"" //main config file's full path"
    << stdstrConfigFilesRootFullDirectoryPath << "\"")
}

/** Is called from another / the (graphical) UI thread. */
void TranslationControllerBase::Stop()
{
  //TODO check if volatile is sufficient or better use atomic function Compare and swap
  //TODO is all memory freed when the translation is cancelled?
  m_vbContinue = false;
}

#ifndef TEST_MINI_XML
void TranslationControllerBase::Transform()
{
  LOGN(//"TranslationControllerBase::Transform() "
      "begin")
  	SetStatus(
  	  VTrans::transformParseTree,
  	  ""
  	  );
  DWORD dwLeftMostTokenIndex = 0 ;
  std::vector<GrammarPart *> stdvec_p_grammarpartRootNode ;
  //TODO only processes the 1st parse tree(s) covering the most tokens -> 
  //  use "ProcessParseTrree() instead?!
  m_parsebyrise.GetGrammarPartCoveringMostTokens(
    dwLeftMostTokenIndex ,
    stdvec_p_grammarpartRootNode
    ) ;
  for( std::vector<GrammarPart *>::const_iterator c_iter_p_grammarpartRootNode
    = stdvec_p_grammarpartRootNode.begin() ; c_iter_p_grammarpartRootNode <
      stdvec_p_grammarpartRootNode.end() ; ++ c_iter_p_grammarpartRootNode
     )
  {
    ParseTreeTraverser::TransformTreeTraverser transformtreetransverser(
//      m_stdmap_stdstrTransformationRule2transformationrule ,
      m_stdmap_syntaxtreepath2transformationrule ,
      * c_iter_p_grammarpartRootNode ,
      m_parsebyrise
      ) ;
    transformtreetransverser.Traverse() ;
  }
  LOGN(//"TranslationControllerBase::Transform() "
    "end")
}

/** @brief Pass settings from a dynamic library etc. */
void TranslationControllerBase::Settings(
  const char * cp_chName, const char * cp_chValue)
{
  settingsName2ValueAndFunction_type::const_iterator c_iter = 
    s_settingsName2valueAndFunction.find(cp_chName);
  if( c_iter != s_settingsName2valueAndFunction.end() )
  {
    c_iter->second.p_function( (const char * const) cp_chValue);
  }
}

void TranslationControllerBase::ResetVocabularyInMainMemToFundamentalWordsOnly()
{
  /** Else too many words may be held in the temporary words map of a
  * BinarySearchInDictFile if translating too often with different words. */
  s_dictReaderAndVocAccess.m_vocAccess.clear();
  /** Is closed by "clear()" -> reopen */
  s_dictReaderAndVocAccess.m_dictReader.open(m_stdstrVocabularyFilePath);
  //  s_dictReaderAndVocAccess.loadDictionary(m_stdstrVocabularyFilePath);
  s_dictReaderAndVocAccess.m_vocAccess.InsertFundamentalWords();
}

std::string GetParseTreeAsIndentedXML(const ParseByRise & parsebyrise)
{
  std::string std_strXML, std_strIntendedXML;
  ByteArray byteArray; //crashes in parallel translation version in constructor
  IO::GenerateXMLtreeFromParseTree( (ParseByRise *) & parsebyrise, /*std_strXML*/ byteArray);
  const BYTE * const byteArrayBegin = byteArray.GetArray();
  const fastestUnsignedDataType byteArraySize = byteArray.GetSize();
  std_strXML = UTF8string::GetAsISO_8859_1StdString(byteArrayBegin,
    byteArraySize );
  std::ostringstream std_ostringstream;
  OutputXMLindented_inl(std_strXML.c_str(), std_ostringstream);
  std_strIntendedXML = std_ostringstream.str();
  return std_strIntendedXML;
}

void TranslationControllerBase::Translate(
//  ParseByRise & r_parsebyrise ,
  const std::string & cr_stdstrWholeInputText ,
  std::vector<std::string> & r_stdvec_stdstrWholeTransl,
//  TranslationResult & r_translationResult
  TranslateParseByRiseTree::ProcessParseTree_type pfn_processParseTree,
  fastestUnsignedDataType numIterations
//  , std::vector<std::vector<TranslationAndConsecutiveID> > &
//    r_stdvec_stdvecTranslationAndConsecutiveID
  )
{
  LOGN(//"TranslationControllerBase::Translate(...) "
      "begin")
//  OperatingSystem::GetCurrentTimeInNanoSeconds();
//    OperatingSystem::GetTimeCountInNanoSeconds();
  TimeCountInNanosec_type 
    timeCountInNanoSecondsBeforeParseTreeGen, 
    timeCountInNanoSecondsAfterParseTreeGen, 
    timeCountInNanoSecondsParseTreeGen,
    timeCountInNanoSecondsBeforeTransforParseTree, 
    timeCountInNanoSecondsAfterTransforParseTree,
    timeCountInNanoSecondsTransformParseTree,
    timeCountInNanoSecondsBeforeTranslRules,
    timeCountInNanoSecondsAfterTranslRules,
    timeCountInNanoSecondsApplyTranslRules;
//#ifdef WIN32
//  Windows::GetTimeCountInNanoSeconds(timeCountInNanoSeconds);
//#else

  ResetVocabularyInMainMemToFundamentalWordsOnly();

//#ifndef __ANDROID__
  OperatingSystem::GetTimeCountInNanoSeconds(timeCountInNanoSecondsBeforeParseTreeGen);
//#endif
//#endif
  if(! m_vbContinue)
    return;
  m_parsebyrise.CreateParseTree(cr_stdstrWholeInputText);
  OperatingSystem::GetTimeCountInNanoSeconds(timeCountInNanoSecondsAfterParseTreeGen);
  timeCountInNanoSecondsParseTreeGen = timeCountInNanoSecondsAfterParseTreeGen -
    timeCountInNanoSecondsBeforeParseTreeGen;
  m_numThreadsAndTimeDuration[buildParseTrees].timeDurationInSeconds = (double) 
    timeCountInNanoSecondsParseTreeGen / 1000000000.0f;
  
  std::string std_strIndentedXML = GetParseTreeAsIndentedXML(
    m_parsebyrise);
  LOGN("parse tree as indented XML:\n" << std_strIndentedXML)

  //  RemoveDuplicateParseTrees();

  if(! m_vbContinue)
  	return;
  OperatingSystem::GetTimeCountInNanoSeconds(timeCountInNanoSecondsBeforeTransforParseTree);
  Transform() ;
  OperatingSystem::GetTimeCountInNanoSeconds(timeCountInNanoSecondsAfterTransforParseTree);
  timeCountInNanoSecondsTransformParseTree = 
    timeCountInNanoSecondsAfterTransforParseTree - 
    timeCountInNanoSecondsBeforeTransforParseTree;
  
//  TranslateParseByRiseTree translateParseByRiseTree(
//    m_parsebyrise
//    , ::wxGetApp()
//    ) ;
  TranslateParseByRiseTree & translateParseByRiseTree =
    //::wxGetApp().
    m_translateparsebyrisetree ;
//  DEBUG_COUT("before translation\n")

  if(! m_vbContinue)
  	return;
  OperatingSystem::GetTimeCountInNanoSeconds(timeCountInNanoSecondsBeforeTranslRules);
  TranslationResult r_translationResult;
  for( fastestUnsignedDataType applyTranslRuleIndex = 0; 
    applyTranslRuleIndex < numIterations ; applyTranslRuleIndex ++ )
  {
//  translateParseByRiseTree.Translate(
//    m_parsebyrise,
//    r_stdvec_stdstrWholeTransl,
//    r_translationResult
////    stdvec_stdvecTranslationAndConsecutiveID
//    ) ;
    translateParseByRiseTree.ProcessParseTree(
  //    r_parsebyrise,
      r_stdvec_stdstrWholeTransl,
      r_translationResult,
      //(TranslateParseTree)()//r_translationResult)
      //syntax ( "& >>class name<<::>>method name<<" from
      //see http://stackoverflow.com/questions/4832275/c-typedef-member-function-signature-syntax
      pfn_processParseTree
      );
    }  
  OperatingSystem::GetTimeCountInNanoSeconds(timeCountInNanoSecondsAfterTranslRules);
  timeCountInNanoSecondsApplyTranslRules =
    timeCountInNanoSecondsAfterTranslRules - timeCountInNanoSecondsBeforeTranslRules;
  timeCountInNanoSecondsApplyTranslRules /= (float) numIterations;
  m_numThreadsAndTimeDuration[applyTranslRules].timeDurationInSeconds = (double) 
    timeCountInNanoSecondsApplyTranslRules / 1000000000.0f;
  
  /** If not clearing and translating different words multiple times or long
    * texts then the main memory may get exhausted. */
  s_dictReaderAndVocAccess.GetVocAccess().clearTemporaryEntries();
//  std::string std_strXML;
  if(! m_vbContinue)
    return;
  std_strIndentedXML = GetParseTreeAsIndentedXML(
    m_parsebyrise);
//  LOGN("translation as indented XML:" << std_strIndentedXML)

//  LOGN( /*FULL_FUNC_NAME <<*/ "generated XML data:" << std_strXML)
  LOGN(//"TranslationControllerBase::Translate(...) "
      "end")
}
#endif
