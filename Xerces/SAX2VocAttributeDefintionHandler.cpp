/*
 * SAX2VocAttributeDefinitionHandler.cpp
 *
 *  Created on: Dec 4, 2010
 *      Author: Stefan
 */
//convertFromAndToStdString(...)
#include <Controller/character_string/convertFromAndToStdString.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
#include <Translate/TranslateParseByRiseTree.hpp>
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
#include <Xerces/SAX2VocAttributeDefintionHandler.hpp>
#include <Xerces/XercesAttributesHelper.hpp> //class XercesAttributesHelper
#include <Xerces/XercesString.hpp> //Xerces::ToStdString(...)

#include <xercesc/sax/Locator.hpp>//class XERCES_CPP_NAMESPACE::Locator
#include <xercesc/util/Xerces_autoconf_config.hpp> // XMLFileLoc

namespace Xerces
{

  SAX2VocAttributeDefinitionHandler::SAX2VocAttributeDefinitionHandler(
    TranslateParseByRiseTree & cr_translateparsebyrisetree ,
    I_UserInterface & r_i_userinterface )
    :
    m_r_i_userinterface( r_i_userinterface )
    , mr_translateparsebyrisetree( cr_translateparsebyrisetree )
  {
  }

  SAX2VocAttributeDefinitionHandler::~SAX2VocAttributeDefinitionHandler()
  {
    // TODO Auto-generated destructor stub
  }

  void SAX2VocAttributeDefinitionHandler::GettingXMLattributeValueFailed(
    const std::string & cr_stdstrXMLattributeName )
  {
    bSuccess = false ;
    m_r_i_userinterface.Message( "no \"" + cr_stdstrXMLattributeName +
      "\" XML attribute name in document\n"
      + Xerces::ToStdString( m_pc_locator->//getPublicId()
        getSystemId() )
      + "\nin line:"
      + convertToStdString<XMLFileLoc>( m_pc_locator->getLineNumber() )
      + ", column:"
      + convertToStdString<XMLFileLoc>( m_pc_locator->getColumnNumber() )
      ) ;
  }

  void SAX2VocAttributeDefinitionHandler::setDocumentLocator(
    const XERCES_CPP_NAMESPACE::Locator * const cpc_locator )
  {
    m_pc_locator = cpc_locator ;
  }

  void SAX2VocAttributeDefinitionHandler::startElement
    (
    const XMLCh * const cpc_xmlchURI,
    const XMLCh * const cpc_xmlchLocalName,
    const XMLCh * const cpc_xmlchQualifiedName,
    const //Use namespace prefix here for clarifications that the "Attributes"
      //class is part of / belongs to Xerces.
      XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes
    )
  {
    char * pchXMLelementName = XERCES_CPP_NAMESPACE::XMLString::transcode(
      cpc_xmlchLocalName);
    if( pchXMLelementName )
    {
      std::string stdstrElementName = std::string(pchXMLelementName) ;
      LOGN("SAX2VocAttributeDefinitionHandler::startElement:"
        << stdstrElementName )
      if( stdstrElementName == "vocabulary_attribute_definiton" )
      {
//        bool bSuccess = true ;
        bSuccess = true ;
        std::string stdstrAttributeName ;
        std::string stdstrAttributeType ;
        std::string stdstrLanguage ;
        std::string stdstrWordClass ;
        WORD wAttributeIndex ;
        WORD wAttributeLength ;
        WORD wAttributeType ;
        WORD wLanguage ;

        std::string stdstrXMLattributeName = "word_class" ;
        if( XercesAttributesHelper::GetAttributeValue(
            cr_xercesc_attributes ,
            stdstrXMLattributeName.c_str() ,
            stdstrWordClass )
            == XercesAttributesHelper::getting_attribute_value_failed
          )
        {
          GettingXMLattributeValueFailed(stdstrXMLattributeName) ;
        }

        stdstrXMLattributeName = "attribute_name" ;
        //e.g. "German_plural"
        if( XercesAttributesHelper::GetAttributeValue(
            cr_xercesc_attributes ,
            stdstrXMLattributeName.c_str() ,
            stdstrAttributeName
            )
            == XercesAttributesHelper::getting_attribute_value_failed
          )
        {
          GettingXMLattributeValueFailed( stdstrXMLattributeName ) ;
        }


        stdstrXMLattributeName = "language" ;
        //->the definition either applies to English or German.
        if( XercesAttributesHelper::GetAttributeValue(
            cr_xercesc_attributes ,
            stdstrXMLattributeName.c_str() ,
            stdstrLanguage
            )
            == XercesAttributesHelper::getting_attribute_value_failed
          )
        {
          GettingXMLattributeValueFailed( stdstrXMLattributeName ) ;
        }

        stdstrXMLattributeName = "attribute_type" ;
        //attribute is a string or some bits
        if( XercesAttributesHelper::GetAttributeValue(
            cr_xercesc_attributes ,
            stdstrXMLattributeName.c_str() ,
            stdstrAttributeType
            )
            == XercesAttributesHelper::getting_attribute_value_failed
          )
        {
          GettingXMLattributeValueFailed( stdstrXMLattributeName ) ;
        }

        stdstrXMLattributeName = "attribute_index" ;
        //String or bit index
        if( XercesAttributesHelper::GetAttributeValue(
            cr_xercesc_attributes ,
            stdstrXMLattributeName.c_str() ,
            wAttributeIndex
            )
            == XercesAttributesHelper::getting_attribute_value_failed
          )
        {
          GettingXMLattributeValueFailed( stdstrXMLattributeName ) ;
        }

        stdstrXMLattributeName = "attribute_length" ;
        //Bit length
        if( XercesAttributesHelper::GetAttributeValue(
            cr_xercesc_attributes ,
            stdstrXMLattributeName.c_str() ,
            wAttributeLength
            )
            == XercesAttributesHelper::getting_attribute_value_failed
          )
        {
          GettingXMLattributeValueFailed( stdstrXMLattributeName ) ;
        }

        if( stdstrLanguage == "German" )
          wLanguage = AttributeTypeAndPosAndSize::German ;
        else if( stdstrLanguage == "English" )
          wLanguage = AttributeTypeAndPosAndSize::English ;
        else
          m_r_i_userinterface.Message("language not ele {\"English\";"
            "\"German\"}") ;
        if( stdstrAttributeType == "string" )
          wAttributeType = AttributeTypeAndPosAndSize::string ;
        else
          wAttributeType = AttributeTypeAndPosAndSize::bit ;

        if( bSuccess )
          mr_translateparsebyrisetree.AddVocAndTranslDefinition(
            stdstrWordClass.c_str() ,
            stdstrAttributeName.c_str() ,
            wLanguage ,
            wAttributeType ,
            wAttributeIndex ,
            wAttributeLength
            ) ;
      }
      //Release memory AFTER comparing.
      XERCES_CPP_NAMESPACE::XMLString::release( & pchXMLelementName);
    }
  }
}
