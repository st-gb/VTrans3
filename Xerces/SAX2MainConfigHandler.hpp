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
#include <Xerces/SAX2TranslationRuleHandler.hpp>

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
    SAX2TranslationRuleHandler m_sax2translationrulehandler ;
    std::string m_strElementName ;
    inline void HandleReadTranslationRuleFileXMLelement(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes ) ;
    inline void HandleReadVocabularyAttributeDefinitionFileXMLelement(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes ) ;
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
