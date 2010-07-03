/*
 * SAX2TranslationRuleHandler.hpp
 *
 *  Created on: Jun 12, 2010
 *      Author: Stefan
 */

#ifndef SAX2TRANSLATIONRULEHANDLER_HPP_
#define SAX2TRANSLATIONRULEHANDLER_HPP_

#include <xercesc/sax2/Attributes.hpp> //for "xercesc_2_8::Attributes"
#include <xercesc/sax2/DefaultHandler.hpp>
#include <Translate/ConditionsAndTranslation.hpp>

//Needed for verzichten auf the exact namespace.
XERCES_CPP_NAMESPACE_USE

class ParseByRise ;
class I_UserInterface ;

class SAX2TranslationRuleHandler
  : public XERCES_CPP_NAMESPACE::DefaultHandler
{
private:
  ConditionsAndTranslation m_conditionsandtranslation;
  I_UserInterface & mr_i_userinterface ;
  ParseByRise & mr_parsebyrise ;
  TranslateParseByRiseTree & mr_translateparsebyrise ;
  std::string m_strElementName ;
  std::string m_stdstrConditionSyntaxTreePath ;
  std::string m_stdstrConditionByteAttrValue ;
  std::string m_stdstrConditionStringAttrValue ;
  std::string m_stdstrConditionAttributeName ;
  std::string m_stdstrTranslationRuleAttributeName ;
  std::string m_stdstrTranslation ;
  std::string m_stdstrTranslationRuleSyntaxTreePath ;
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
    const XMLCh* const uri,
    const XMLCh* const localname,
    const XMLCh* const qname
  );

  void startElement(
      const   XMLCh* const    uri,
      const   XMLCh* const    localname,
      const   XMLCh* const    qname,
      const   XERCES_CPP_NAMESPACE::Attributes&     attrs
//        XercesAttributesHelper &
  );
//  void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException&);
};

#endif /* SAX2TRANSLATIONRULEHANDLER_HPP_ */
