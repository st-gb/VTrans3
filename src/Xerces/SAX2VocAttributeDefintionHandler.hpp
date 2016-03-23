/*
 * SAX2VocAttributeDefinitionHandler.hpp
 *
 *  Created on: Dec 4, 2010
 *      Author: Stefan
 */

#ifndef SAX2VOCATTRIBUTEDEFINTIONHANDLER_HPP_
#define SAX2VOCATTRIBUTEDEFINTIONHANDLER_HPP_

#include <xercesc/sax2/Attributes.hpp> //for "XERCES_CPP_NAMESPACE::Attributes"
//Base class XERCES_CPP_NAMESPACE::DefaultHandler
#include <xercesc/sax2/DefaultHandler.hpp>

//Forward declarations.
class I_UserInterface ;
class TranslateParseByRiseTree ;

namespace Xerces
{
  class SAX2VocAttributeDefinitionHandler
  : public XERCES_CPP_NAMESPACE::DefaultHandler
  {
    bool bSuccess ;
    XERCES_CPP_NAMESPACE::Locator const * m_pc_locator ;
    I_UserInterface & m_r_i_userinterface ;
    TranslateParseByRiseTree & mr_translateparsebyrisetree ;
  public:
    void GettingXMLattributeValueFailed(
      const std::string & cr_stdstrXMLattributeName ) ;
    void setDocumentLocator( const XERCES_CPP_NAMESPACE::Locator *
      const cpc_locator ) ;
    SAX2VocAttributeDefinitionHandler(
      TranslateParseByRiseTree & cr_translateparsebyrisetree
      , I_UserInterface & r_i_userinterface);
    virtual
    ~SAX2VocAttributeDefinitionHandler();
    void startElement(
      const XMLCh * const cpc_xmlchURI,
      const XMLCh * const cpc_xmlchLocalName,
      const XMLCh * const cpc_xmlchQualifiedName,
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes
    );
  };
}

#endif /* SAX2VOCATTRIBUTEDEFINTIONHANDLER_HPP_ */
