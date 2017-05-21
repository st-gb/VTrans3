/*
 * TranslationControllerBase.hpp
 *
 *  Created on: Dec 6, 2010
 *      Author: Stefan
 */

#ifndef TRANSLATIONCONTROLLERBASE_HPP_
#define TRANSLATIONCONTROLLERBASE_HPP_

#include <Attributes/TranslationResult.hpp> //class TranslationResult
#include <Controller/dictionary_type.hpp> //typedef dictionary_type
#include <Controller/DictReaderAndVocAccess/dictReaderAndVocAccess_type.hpp>
#include "TranslateControllerBaseReturnCodes.h" //enum Init_return_codes
#include <data_structures/Trie/NodeTrie/NodeTrie.hpp> //class NodeTrie
#include <Parse/ParseByRise.hpp> //class ParseByRise
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
#include <Translate/TransformationRule.hpp> //class TransformationRule
#include <OperatingSystem/multithread/nativeCriticalSectionType.hpp>
#include <multi_threaded_translation/nativeThreads.hpp>
//#include <boost/threadpool.hpp> //boost::threadpool
#include <OperatingSystem/time/GetCurrentTime.hpp>
#ifndef TEST_MINI_XML
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>
#endif
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
//#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>//class LetterTree
//#include <xercesc/util/XercesVersion.hpp> //XERCES_CPP_NAMESPACE
#include "ConfigurationHandler_type.hpp"
#include <hardware/CPU/fastest_data_type.h>//fastestUnsignedDataType

#include <map> //class std::map
#include <stdint.h> //for uint32_t
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

class GUIattributes
{
public:
  bool m_bShowGrammarPartAddress ;
  bool m_doubleBufferParseTreePanel;
  bool m_bShowTranslation ;
  bool m_translateOnTextChanges;
  fastestUnsignedDataType m_concatenationIDcolour;
  fastestUnsignedDataType m_fontSizeInPoint;
  fastestUnsignedDataType m_minFontSizeInPoint;
  std::string m_std_strGrammarPartIDcolor;
  std::string m_std_strConcatenationIDcolor;
  std::string m_std_strGrammarPartMemoryAddressColor;
  
  GUIattributes()
    : m_bShowGrammarPartAddress( true /*false*/ )
      , m_doubleBufferParseTreePanel(false)
      , m_bShowTranslation(false)
      , m_translateOnTextChanges(false)
      , m_std_strGrammarPartIDcolor("#FF0000")
      , m_std_strConcatenationIDcolor("#0000FF")
      , m_std_strGrammarPartMemoryAddressColor("#FF0000")
      , m_fontSizeInPoint(8)
      , m_minFontSizeInPoint(5)
      , m_concatenationIDcolour(255)
  {}
  
  void SetConcatenationIDcolour(DWORD dw)
  {
    m_concatenationIDcolour = dw;
  }
  
  void SetPointSizeOfParseTreePanel(const fastestUnsignedDataType fontSizeInPoint) {
    m_fontSizeInPoint = fontSizeInPoint;
  }
  void SetMinFontSizeInPointOfParseTreePanel(
    const fastestUnsignedDataType fontSizeInPoint) {
    m_minFontSizeInPoint = fontSizeInPoint;
  }
};

namespace VTrans
{
  enum StatusCode {not_set, lookUpWordInDictBeginningFromTokenIndex,
	translationRules, transformParseTree, generateXMLtreeFromParseTree
    };
  const char * const g_statusMessages [] = { "not set", 
    "lookUpWordInDictBeginningFromTokenIndex", "translationRules", 
    "transformParseTree", "generateXMLtreeFromParseTree"};
  class CurrentStatus
  {
    private:
    StatusCode m_code;
    std::string m_item;
    struct tm m_time;
    public:
    CurrentStatus() : m_code(not_set)
    {

    }
    StatusCode GetCode() { return m_code; }
    void GetItem(std::string & str ) { str = m_item; }
    void GetTime(struct tm & time ) { time = m_time; }
    void Set(StatusCode code, const char * const pch)
    {
      m_item = std::string(pch);
      m_code = code;
      OperatingSystem::GetCurrentTime(m_time);
    }
  };
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

std::string GetParseTreeAsIndentedXML(const ParseByRise & parsebyrise);

/** @brief Base class for e.g. wxApp and CWinApp-derived classes. */
class TranslationControllerBase
  : public I_UserInterface
{
  nativeCriticalSection_type m_critSecStatus;
  VTrans::CurrentStatus m_currentStatus;
protected:
  enum ProgramArgumentIndices { MainConfigFilePathProgArgIndex = 1,
    CurrWorkDirProgArgIndex};
public:
  long double m_translationDurationInSeconds;
//  float m_applyTranslationRulesInSeconds;
  NumThreadsAndTimeDuration m_numThreadsAndTimeDuration[3];

#define PARALLELIZE_TRANSLATION
#ifdef PARALLELIZE_TRANSLATION
//    fastestUnsignedDataType numThreadsRunning = 0;
    //TODO to get the # of CPU cores: https://developer.android.com/ndk/guides/cpu-features.html
    //get # CPU cores avail. to this process: http://stackoverflow.com/questions/4586405/get-number-of-cpus-in-linux-using-c
//    fastestUnsignedDataType numCPUcoresAvailable = g_p_translationcontrollerbase->s_numParallelTranslationThreads;
  VTrans3::MultiThreadedTranslation m_multiThreadedTranslation;
//  boost::threadpool::pool m_boost_threadpool;
#endif
  
  std::string m_std_strCurrentConfigfile;
  volatile bool m_vbContinue;
#ifndef TEST_MINI_XML
  std::map<TranslationRule *, std::string>
    m_std_map_p_translationrule2filepath;
  std::map<std::string, std::string> m_std_map_grammarRuleName2filepath;
#endif //#ifndef TEST_MINI_XML
//  dictReaderAndVocAccess_type m_dictReaderAndVocAccess;
  std::string m_std_strOriginalCurrWorkDir;
  ConfigurationHandler_type m_configurationHandler;
  fastestUnsignedDataType m_GUIthreadID;
//  static dictionary_type s_dictionary ;
  typedef std::map<std::string, ValuesAndFunction> settingsName2ValueAndFunction_type;
  static settingsName2ValueAndFunction_type s_settingsName2valueAndFunction;
  static fastestUnsignedDataType s_numParallelTranslationThreads;
#ifndef TEST_MINI_XML
  /*static*/ dictReaderAndVocAccess_type s_dictReaderAndVocAccess;
#endif //TEST_MINI_XML
  NodeTrie<uint32_t> m_nodetrie_ui32GrammarPartName2colour;
#ifndef TEST_MINI_XML
  ParseByRise m_parsebyrise ;
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
  GUIattributes m_GUIattributes;
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
  /** (Sub class) implementation may set title of a window to the status passed. */
  virtual void SetStatus(enum VTrans::StatusCode, /*const std::string & str*/
    const char * const);
  enum VTrans::StatusCode GetStatus( /*const std::string & str*/
    std::string &, struct tm &);
  unsigned GetStatus2( /*const std::string & str*/
    const std::string & str, struct tm & time, ByteArray & byteArray
    /*void * pData*/);

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
    TranslateParseByRiseTree::ProcessParseTree_type,
    fastestUnsignedDataType numIterations
    ) ;
#endif //#ifndef TEST_MINI_XML
//  virtual void StartTimer();
};

#endif /* TRANSLATIONCONTROLLERBASE_HPP_ */
