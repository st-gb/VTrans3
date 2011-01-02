/*
 * TranslationControllerBase.hpp
 *
 *  Created on: Dec 6, 2010
 *      Author: Stefan
 */

#ifndef TRANSLATIONCONTROLLERBASE_HPP_
#define TRANSLATIONCONTROLLERBASE_HPP_

#include <Parse/ParseByRise.hpp> //class ParseByRise
#include <Translate/TransformationRule.hpp> //class TransformationRule
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>//class LetterTree
#include <xercesc/util/XercesVersion.hpp> //XERCES_CPP_NAMESPACE

#include <map> //class std::map
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

//Base class for e.g. wxApp and CWinApp-derived classes.
class TranslationControllerBase
  : public I_UserInterface
{
public:
  static LetterTree s_lettertree ;
  ParseByRise m_parsebyrise ;
//  std::map<std::string,TransformationRule>
//    m_stdmap_stdstrTransformationRule2transformationrule ;
  std::map<SyntaxTreePath,TransformationRule>
    m_stdmap_syntaxtreepath2transformationrule ;
  TranslateParseByRiseTree m_translateparsebyrisetree ;
  void Init() ;
  //These virtual methods need to be re-declared (and implemented in
  //subclasses), else error like this: "pure virtual method called".
  virtual void Message(const std::string & cr_stdstr ) //= 0;
  {

  }
  virtual void Message(const std::wstring & cr_stdwstr ) //= 0;
  {

  }
  void ReadGrammarRuleFile(
    SAX2GrammarRuleHandler & r_sax2grammarrulehandler ,
    const std::string & cr_stdstrFilePath
    ) ;
  void ReadTranslationRuleFile(
    SAX2TranslationRuleHandler & r_sax2translationrulehandler ,
    const std::string & cr_stdstrFilePath
    ) ;
  void ReadVocAttributeDefinitionFile(
    Xerces::SAX2VocAttributeDefinitionHandler &
      sax2vocattributedefinitionhandler ,
    const std::string & cr_stdstrFilePath
    ) ;
  void ReadXMLfile(
    XERCES_CPP_NAMESPACE::DefaultHandler & r_xercesc_defaulthandler ,
    const std::string & cr_stdstrFilePath
    ) ;
  void Transform() ;
  void Translate(
    const std::string & cr_stdstrWholeInputText ,
    std::vector<std::string> & r_stdvec_stdstrWholeTransl ,
    std::vector<std::vector<TranslationAndGrammarPart> > &
      r_stdvec_stdvecTranslationAndGrammarPart
    ) ;
  TranslationControllerBase();
  virtual
  ~TranslationControllerBase();
};

#endif /* TRANSLATIONCONTROLLERBASE_HPP_ */
