/*
 * SAX2MainConfigHandler.hpp
 *
 *  Created on: Nov 28, 2010
 *      Author: Stefan
 */

#ifndef SAX2MAINCONFIGHANDLER_HPP_
#define SAX2MAINCONFIGHANDLER_HPP_

//class "XERCES_CPP_NAMESPACE::Attributes"
#include <xercesc/sax2/Attributes.hpp>
//class "XERCES_CPP_NAMESPACE::DefaultHandler
#include <xercesc/sax2/DefaultHandler.hpp>
#include <string> //class std::string
//class Xerces::SAX2GrammarRuleHandler
#include <Xerces/SAX2GrammarRuleHandler.hpp>
//class Xerces::SAX2TranslationRuleHandler
#include <Xerces/SAX2TranslationRuleHandler.hpp>
//class Xerces::SAX2TransformationRuleHandler
#include <Xerces/SAX2TransformationRuleHandler.hpp>

//class SAX2TranslationRuleHandler ;
class TranslateParseByRiseTree ;
class TranslationControllerBase ;

namespace Xerces
{
  class SAX2MainConfigHandler
    :
    public XERCES_CPP_NAMESPACE::DefaultHandler
  {
  public:
    std::string m_stdstrVocabularyFilePath ;
//    TranslateParseByRiseTree & mr_translateparsebyrisetree ;
    TranslateParseByRiseTree * mp_translateparsebyrisetree ;
    TranslationControllerBase & m_r_translationcontrollerbase ;
    SAX2GrammarRuleHandler m_sax2grammarrulehandler ;
    SAX2TransformationRuleHandler m_sax2transformationrulehandler ;
    SAX2TranslationRuleHandler m_sax2translationrulehandler ;
    std::string m_strElementName ;
    inline void HandleGrammartPartColourXMLelement(
      const XERCES_CPP_NAMESPACE::Attributes & c_r_xercesc_attributes);
    inline void HandleGUI_XMLelement(
      const XERCES_CPP_NAMESPACE::Attributes & c_r_xercesc_attributes);
    inline void HandleReadGrammarRuleFileXMLelement(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes ) ;
    inline void HandleReadTransformationRuleFileXMLelement(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes ) ;
    inline void HandleReadTranslationRuleFileXMLelement(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes ) ;
    inline void HandleReadVocabularyAttributeDefinitionFileXMLelement(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes ) ;
    void ReadXMLfile(
      XERCES_CPP_NAMESPACE::DefaultHandler & r_xerces_defaulthandler ,
      const std::string & cr_stdstrFilePath
      ) ;
    SAX2MainConfigHandler(
//      TranslateParseByRiseTree & r_translateparsebyrisetree
      TranslationControllerBase & r_translationcontrollerbase
      );
    virtual
    ~SAX2MainConfigHandler();
    void startElement(
      const XMLCh * const cpc_xmlchURI,
      const XMLCh * const cpc_xmlchLocalName,
      const XMLCh * const cpc_xmlchQualifiedName,
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes
      ) ;
  };
}

#endif /* SAX2MAINCONFIGHANDLER_HPP_ */
