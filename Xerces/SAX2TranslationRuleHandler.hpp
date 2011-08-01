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
//class ConditionsAndTranslation
#include <Translate/ConditionsAndTranslation.hpp>

////Needed for verzichten auf the exact namespace.
//XERCES_CPP_NAMESPACE_USE

//Forward declarations.
class ParseByRise ;
class I_UserInterface ;
class TranslateParseByRiseTree ;

class SAX2TranslationRuleHandler
  : public XERCES_CPP_NAMESPACE::DefaultHandler
{
private:
  XERCES_CPP_NAMESPACE::Locator const * m_pc_locator ;
  ConditionsAndTranslation m_conditionsandtranslation;
  I_UserInterface & mr_i_userinterface ;
  ParseByRise & mr_parsebyrise ;
  TranslateParseByRiseTree & mr_translateparsebyrise ;
  std::string m_strElementName ;
  std::string m_stdstrConditionSyntaxTreePath ;
  std::string m_stdstrConditionByteAttrValue ;
  std::string m_stdstrConditionStringAttrValue ;
  std::string m_stdstrConditionAttributeName ;
  std::string m_std_strGrammarPartName;
  std::string m_std_strParentNodeGrammarPartName;
  std::string m_stdstrTranslationRuleAttributeName ;
  std::string m_stdstrTranslation ;
  std::string m_stdstrTranslationRuleSyntaxTreePath ;
  std::string m_std_strSyntaxTreePathForInsertionForTranslation;
  unsigned m_uiTranslationInsertion;
  unsigned m_uiParentNodeInsertion;
public:
  SAX2TranslationRuleHandler(
    TranslateParseByRiseTree &
    , ParseByRise &
    , I_UserInterface & r_userinterface
    );
  virtual
  ~SAX2TranslationRuleHandler();

  //If all conditions were added at first the translation rule can be added.
  void endElement
  (
    const XMLCh* const cpc_xmlchURI,
    const XMLCh* const cpc_xmlchLocalName,
    const XMLCh* const cpc_xmlchQualifiedName
  );
  inline void HandleConditionXMLelement(
    const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes) ;
  inline void HandleTranslationRuleElementName(
    const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes) ;
  void setDocumentLocator( const XERCES_CPP_NAMESPACE::Locator *
    const cpc_locator ) ;
  void startElement(
    const XMLCh * const cpc_xmlchURI,
    const XMLCh * const cpc_xmlchLocalName,
    const XMLCh * const cpc_xmlchQualifiedName,
    const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes
  );
//  void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException&);
};

#endif /* SAX2TRANSLATIONRULEHANDLER_HPP_ */
