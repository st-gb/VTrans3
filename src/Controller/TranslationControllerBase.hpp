/** TranslationControllerBase.hpp
 *  Created on: Dec 6, 2010
 *      Author: Stefan  */

#ifndef TRANSLATIONCONTROLLERBASE_HPP_
#define TRANSLATIONCONTROLLERBASE_HPP_

#include <Attributes/TranslationResult.hpp> //class TranslationResult
#include <Controller/dictionary_type.hpp> //typedef dictionary_type
#include <Controller/TranslationProcess.hpp> //
#include "TranslateControllerBaseReturnCodes.h" //enum Init_return_codes
#include <Parse/ParseByRise.hpp> //class ParseByRise
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
#include <Translate/TransformationRule.hpp> //class TransformationRule
#ifdef PARALLELIZE_TRANSLATION
#include <multi_threaded_translation/nativeThreads.hpp>
#endif //#ifdef PARALLELIZE_TRANSLATION
//#include <boost/threadpool.hpp> //boost::threadpool
#ifndef TEST_MINI_XML
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>
#endif
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
//#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>//class LetterTree
//#include <xercesc/util/XercesVersion.hpp> //XERCES_CPP_NAMESPACE
#include "ConfigurationHandler_type.hpp"
#include "TranslationProcess.hpp"
#include <hardware/CPU/fastest_data_type.h>//fastestUnsignedDataType

#include <string> //class std::string
#include <vector> //class std::vector

/**Forward declarations: */
class SAX2GrammarRuleHandler ;
class ByteArray;
class SAX2TranslationRuleHandler ;
namespace Xerces
{
  class SAX2VocAttributeDefinitionHandler ;
}
//class std::string ;
namespace XERCES_CPP_NAMESPACE
{
  class DefaultHandler ;
}

struct ValuesAndFunction
{
  char * values;
  void ( * p_function) (const char * const );
};

struct NumThreadsAndTimeDuration
{
  fastestUnsignedDataType numThreads;
  float timeDurationInSeconds;
};

enum {buildParseTrees = 0, transform, applyTranslRules, beyondLastValue};

std::string GetParseTreeAsIndentedXML(const VTrans3::BottomUpParser &);

#define EVALUATE_PROCESSING

void SetLogLevel(const char * const value);
void SetNumTranslationThreads(const char * const value);
#ifdef EVALUATE_PROCESSING /** Only needed for testing. */
void Set3rdTranslationStepFunction(const char * const value);
void SetNumIterationsFor3rdTranslationStep(const char * const value);
#endif //#ifdef EVALUATE_PROCESSING

/** @brief Base class for e.g. wxApp and CWinApp-derived classes. */
class TranslationControllerBase
  : public I_UserInterface
  //TODO also inherit from class BilingProcBase ?
{
//  VTrans::CurrentStatus m_currentStatus;
//  TranslationProcess m_translationProcess;
protected:
  enum ProgramArgumentIndices { MainConfigFilePathProgArgIndex = 1,
    CurrWorkDirProgArgIndex};
public:
  long double m_translationDurationInSeconds;
//  float m_applyTranslationRulesInSeconds;
  NumThreadsAndTimeDuration m_numThreadsAndTimeDuration[3];

  std::string m_englishText;
#ifdef EVALUATE_PROCESSING 
  static fastestUnsignedDataType s_num3rdTranslationStepIterations;
  static std::map<std::string, TranslateParseByRiseTree::ProcessParseTree_type> 
    s_functionName2function;
  static TranslateParseByRiseTree::ProcessParseTree_type s_3rdStepTranslationFunction;
#endif //#ifdef EVALUATE_PROCESSING
//#define PARALLELIZE_TRANSLATION
#ifdef PARALLELIZE_TRANSLATION
//    fastestUnsignedDataType numThreadsRunning = 0;
    //TODO to get the # of CPU cores: https://developer.android.com/ndk/guides/cpu-features.html
    //get # CPU cores avail. to this process: http://stackoverflow.com/questions/4586405/get-number-of-cpus-in-linux-using-c
//    fastestUnsignedDataType numCPUcoresAvailable = g_p_translationcontrollerbase->s_numParallelTranslationThreads;
  VTrans3::MultiThreadedTranslation m_multiThreadedTranslation;
//  boost::threadpool::pool m_boost_threadpool;
#endif
  
//  dictReaderAndVocAccess_type m_dictReaderAndVocAccess;
  std::string m_std_strOriginalCurrWorkDir;
  ConfigurationHandler_type m_configurationHandler;
  fastestUnsignedDataType m_GUIthreadID;
//  static dictionary_type s_dictionary ;
  typedef std::map<std::string, ValuesAndFunction> settingsName2ValueAndFunction_type;
  static settingsName2ValueAndFunction_type s_settingsName2valueAndFunction;
  static fastestUnsignedDataType s_numParallelTranslationThreads;
#ifndef TEST_MINI_XML
  VTrans3::BottomUpParser m_parsebyrise ;
#endif
  std::string m_stdstrVocabularyFilePath ;
//  std::map<std::string,TransformationRule>
//    m_stdmap_stdstrTransformationRule2transformationrule ;
#ifndef TEST_MINI_XML
  std::map<SyntaxTreePath,TransformationRule>
    m_stdmap_syntaxtreepath2transformationrule ;
  TranslateParseByRiseTree m_translateparsebyrisetree ;
#endif //#ifndef TEST_MINI_XML
  std::string m_std_strMainConfigFilePath;
  bool m_dictionarySuccessfullyLoaded;

  TranslationControllerBase();
  virtual
  ~TranslationControllerBase();

  //TODO possibly rename to a more general/generic/ abstract name like "PostInit"
  virtual void CreateAndShowMainWindow() {};
  template <typename XMLelementType> bool GetAttributeValue(
      XMLelementType & xmlElement, const char * const, std::string &);
  std::string GetCurrentWorkingDir();

  fastestUnsignedDataType GetNumberOfParseTrees(
    TranslationResult & translationResult);
  /** Get (German) input text. Must be "virtual" for polymorphism. */
  virtual void GetSourceText(std::string & ) {};
  bool CurrentThreadIsGUIthread();
  BYTE Init(const std::string & cr_stdstrMainConfigFilePath) ;
  virtual void LoadingVocabularyFileFailed(
    const std::string & cr_stdstrFilePath) {} // = 0;
  //These virtual methods need to be re-declared (and implemented in
  //subclasses), else error like this: "pure virtual method called".
  virtual void Message(const std::string & cr_stdstr ) //= 0;
  {
    LOGN(cr_stdstr)
  }
  virtual void Message(const std::wstring & cr_stdwstr ) //= 0;
  {

  }

  void SetNumberOfParallelTranslationThreads(fastestUnsignedDataType);
  virtual void ShowInvalidVocabularyFileFormatMessage(
    const VTrans::string_type & strWordFile,
    DWORD dwOffsetOfBeginOfEntry,
    DWORD dwOffset
    )// = 0; 
    {  }
  /** used by dyn lib and console application. */
  void TranslateAsXML(const char * p_chEnglishText, ByteArray & byteArray);
  
  void TranslateAsXMLgetAverageTimes(
    const char * p_chEnglishText, 
    ByteArray & byteArray,
    TranslateParseByRiseTree::ProcessParseTree_type pfnProcessParseTree,
    fastestUnsignedDataType numIterations);
  void ReadGrammarRuleFile(
//    SAX2GrammarRuleHandler & r_sax2grammarrulehandler ,
    const std::string & cr_stdstrFilePath
    ) ;
  bool ReadMainConfigFile(const std::string & cr_stdstrFilePath ) ;
  void ReadTranslationRuleFile(
//    SAX2TranslationRuleHandler & r_sax2translationrulehandler ,
    const std::string & cr_stdstrFilePath
    ) ;
  void ReadVocAttributeDefinitionFile(
//    Xerces::SAX2VocAttributeDefinitionHandler &
//      sax2vocattributedefinitionhandler ,
    const std::string & cr_stdstrFilePath
    ) ;
  void ReadXMLfile(
    XERCES_CPP_NAMESPACE::DefaultHandler & r_xercesc_defaulthandler ,
    const std::string & cr_stdstrFilePath
    ) ;
  void SetCurrentDirToConfigFilesRootPath(
    const std::string & c_r_stdstrConfigFilesRootPath);
  void SetCurrentDirToOriginalCurrentWorkingDir();
  void Settings(const char * cp_chName, const char * cp_chValue);
  void Stop();
  void ResetVocabularyInMainMemToFundamentalWordsOnly();
#ifndef TEST_MINI_XML
  void Transform() ;
  void Translate(
    const std::string & cr_stdstrWholeInputText ,
    std::vector<std::string> & r_stdvec_stdstrWholeTransl ,
//    TranslationResult & translationResult
    TranslateParseByRiseTree::ProcessParseTree_type = & TranslateParseByRiseTree::TranslateParseTree,
    fastestUnsignedDataType numIterations = 1
    ) ;
#endif //#ifndef TEST_MINI_XML
//  virtual void StartTimer();
};

#endif /* TRANSLATIONCONTROLLERBASE_HPP_ */
