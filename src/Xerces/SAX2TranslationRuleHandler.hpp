/*
 * SAX2TranslationRuleHandler.hpp
 *
 *  Created on: Jun 12, 2010
 *      Author: Stefan
 */

#ifndef SAX2TRANSLATIONRULEHANDLER_HPP_
#define SAX2TRANSLATIONRULEHANDLER_HPP_

#include <xercesc/sax2/Attributes.hpp> //for "XERCES_CPP_NAMESPACE::Attributes"
//Base class XERCES_CPP_NAMESPACE::DefaultHandler
#include <xercesc/sax2/DefaultHandler.hpp>
#include <IO/rules/TranslationRuleFileReaderBase.hpp> //base class

////Needed for verzichten auf the exact namespace.
//XERCES_CPP_NAMESPACE_USE

class SAX2TranslationRuleHandler
  : public XERCES_CPP_NAMESPACE::DefaultHandler
    , public VTrans3::TranslationRuleFileReaderBase
{
private:
    const XERCES_CPP_NAMESPACE::Locator * m_pc_locator;

    inline void GetLineNumber(std::string & std_strLine );
    inline std::string GetColumnNumber();
    inline std::string GetLineNumber();
    inline std::string GetFilePathAndFileLocation();

    void ShowMessageToUser(const char * const cpc_chMessage);
    void ShowMessageToUser(const std::string & c_r_std_strMessage);
    void GetAttributeNameOrTranslationString(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes);
    void GetStringTransformationFunctionName(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes);
    void GetParentNodeInsertionDirection(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes);
    void GetChildNodeInsertionDirection(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes);
    void GetSyntaxTreePathWhereToInsertTranslation(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes);
public:
    SAX2TranslationRuleHandler(
      TranslateParseByRiseTree & ,
      ParseByRise & ,
      I_UserInterface & r_userinterface
      , ConfigurationHandler_type & configurationReader);
    virtual ~SAX2TranslationRuleHandler();
    void endElement(
      const XMLCh * const cpc_xmlchURI,
      const XMLCh * const cpc_xmlchLocalName,
      const XMLCh * const cpc_xmlchQualifiedName);
    inline void HandleTranslationRuleElementName(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes);
    void Process(const std::string & filePath);
    void setDocumentLocator(
      const XERCES_CPP_NAMESPACE::Locator * const cpc_locator);
    void startElement(
      const XMLCh * const cpc_xmlchURI,
      const XMLCh * const cpc_xmlchLocalName,
      const XMLCh * const cpc_xmlchQualifiedName,
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes);
};

#endif /* SAX2TRANSLATIONRULEHANDLER_HPP_ */
