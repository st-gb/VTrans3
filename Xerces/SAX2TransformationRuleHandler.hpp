/*
 * SAX2TransformationRuleHandler.hpp
 *
 *  Created on: Dec 21, 2010
 *      Author: Stefan
 */

#ifndef SAX2TRANSFORMATIONRULEHANDLER_HPP_
#define SAX2TRANSFORMATIONRULEHANDLER_HPP_

#include <xercesc/sax2/Attributes.hpp> //for "XERCES_CPP_NAMESPACE::Attributes"
//Base class XERCES_CPP_NAMESPACE::DefaultHandler
#include <xercesc/sax2/DefaultHandler.hpp>

class ParseByRise ;
class TranslationControllerBase ;

namespace Xerces
{

  class SAX2TransformationRuleHandler
    : public XERCES_CPP_NAMESPACE::DefaultHandler
  {
//    ParseByRise & m_r_parsebyrise ;
    TranslationControllerBase & m_r_translationcontrollerbase ;
  public:
    SAX2TransformationRuleHandler(//ParseByRise & r_parsebyrise
      TranslationControllerBase & r_translationcontrollerbase );
    virtual
    ~SAX2TransformationRuleHandler();
    bool PossiblyAdd2ndSyntaxTreePath(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes,
      TransformationRule & r_transformationrule
      );
    bool PossiblyAddSyntaxTreePathWhereToInsert(
     const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes,
     TransformationRule & r_transformationrule
     );
    void startElement(
      const XMLCh * const cpc_xmlchURI,
      const XMLCh * const cpc_xmlchLocalName,
      const XMLCh * const cpc_xmlchQualifiedName,
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes
    );
  };

}

#endif /* SAX2TRANSFORMATIONRULEHANDLER_HPP_ */
