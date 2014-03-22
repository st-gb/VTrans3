/*
 * TranslationControllerBase.hpp
 *
 *  Created on: Dec 6, 2010
 *      Author: Stefan
 */

#ifndef TRANSLATIONCONTROLLERBASE_HPP_
#define TRANSLATIONCONTROLLERBASE_HPP_

#include <Controller/dictionary_type.hpp> //typedef dictionary_type
#include <Controller/DictReaderAndVocAccess/dictReaderAndVocAccess_type.hpp>
#include "TranslateControllerBaseReturnCodes.h" //enum Init_return_codes
#include <data_structures/Trie/NodeTrie/NodeTrie.hpp> //class NodeTrie
#include <Parse/ParseByRise.hpp> //class ParseByRise
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
#include <Translate/TransformationRule.hpp> //class TransformationRule
#include <Controller/multithread/nativeCriticalSectionType.hpp>
#include <OperatingSystem/time/GetCurrentTime.hpp>
#ifndef TEST_MINI_XML
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>
#endif
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
//#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>//class LetterTree
//#include <xercesc/util/XercesVersion.hpp> //XERCES_CPP_NAMESPACE
#include "ConfigurationHandler_type.hpp"

#include <map> //class std::map
#include <stdint.h> //for uint32_t
#include <string> //class std::string
#include <vector> //class std::vector

//Forward declarations:
class SAX2GrammarRuleHandler ;
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
  bool m_bShowTranslation ;
  std::string m_std_strGrammarPartIDcolor;
  std::string m_std_strGrammarPartMemoryAddressColor;
  GUIattributes()
    : m_bShowGrammarPartAddress( //true
      false)
      , m_bShowTranslation(false)
      , m_std_strGrammarPartIDcolor("#FF0000")
      , m_std_strGrammarPartMemoryAddressColor("#FF0000")
  {}
};

namespace VTrans
{
  enum StatusCode {not_set, lookUpWordInDictBeginningFromTokenIndex,
	translationRules, transformParseTree, generateXMLtreeFromParseTree
    };
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
  std::string m_std_strCurrentGrammarRuleFilePath;
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

  TranslationControllerBase();
  virtual
  ~TranslationControllerBase();

  //TODO possibly rename to a more general/generic/ abstract name like "PostInit"
  virtual void CreateAndShowMainWindow() {};
  template <typename XMLelementType> bool GetAttributeValue(
      XMLelementType & xmlElement, const char * const, std::string &);
  std::string GetCurrentWorkingDir();
  /** Get (German) input text. Must be "virtual" for polymorphism. */
  virtual void GetSourceText(std::string & ) {};
  BYTE Init(const std::string & cr_stdstrFilePath) ;
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

  void SetStatus(enum VTrans::StatusCode, /*const std::string & str*/
    const char * const);
  enum VTrans::StatusCode GetStatus( /*const std::string & str*/
    std::string &, struct tm &);

  virtual void ShowInvalidVocabularyFileFormatMessage(
    const VTrans::string_type & strWordFile,
    DWORD dwOffsetOfBeginOfEntry,
    DWORD dwOffset
    )// = 0;
  {

  }
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
#ifndef TEST_MINI_XML
  void Transform() ;
  void Translate(
    const std::string & cr_stdstrWholeInputText ,
    std::vector<std::string> & r_stdvec_stdstrWholeTransl ,
//    std::vector<std::vector<TranslationAndGrammarPart> > &
//      r_stdvec_stdvecTranslationAndGrammarPart
    //A vector of sentences that begin at the same token index
    // (sentences that begin at the same token index:
    // vector of sentences that each contains a vector of words).
    std::vector <std::vector <std::vector <TranslationAndGrammarPart> > > &
      r_stdvec_stdvec_stdvecTranslationAndGrammarPart
    ) ;
#endif //#ifndef TEST_MINI_XML
//  virtual void StartTimer();
};

#endif /* TRANSLATIONCONTROLLERBASE_HPP_ */
