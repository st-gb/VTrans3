/*
 * SAX2MainConfigHandler.cpp
 *
 *  Created on: Nov 28, 2010
 *      Author: Stefan
 */
//class TranslationControllerBase
#include <Controller/TranslationControllerBase.hpp>
#include <IO/IO.hpp> //OneLinePerWordPair::LoadWords()
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>
#include <Xerces/SAX2MainConfigHandler.hpp>
//Class SAX2VocAttributeDefintionHandler
#include <Xerces/SAX2VocAttributeDefintionHandler.hpp>

//class XERCES_CPP_NAMESPACE::LocalFileInputSource
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <Xerces/ReadXMLfile.hpp> //ReadXMLfile(...)
//InitializeXerces(), TerminateXerces()
#include <Xerces/XercesHelperFunctions.hpp>
#include <Xerces/XercesAttributesHelper.hpp> //class XercesAttributesHelper
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface

namespace Xerces
{
  SAX2MainConfigHandler::SAX2MainConfigHandler(
    //for SAX2TranslationRuleHandler c'tor
//    TranslateParseByRiseTree & r_translateparsebyrisetree
    TranslationControllerBase & r_translationcontrollerbase
    )
    :
//    mr_translateparsebyrisetree( r_translateparsebyrisetree ) ,
    mp_translateparsebyrisetree( //& r_translateparsebyrisetree
      & r_translationcontrollerbase.m_translateparsebyrisetree ) ,
    m_r_translationcontrollerbase( r_translationcontrollerbase ) ,
    //Must create on heap, else the callback functions like "startElement"
    //aren't called?!
  //  SAX2TranslationRuleHandler * p_sax2grammarrulehandler = new
  //    SAX2TranslationRuleHandler( * this, * mp_parsebyrise ,
    // mr_i_userinterface ) ;
    m_sax2translationrulehandler (
//      mr_translateparsebyrisetree ,
      * mp_translateparsebyrisetree ,
      //* mr_translateparsebyrisetree.mp_parsebyrise ,
      * mp_translateparsebyrisetree->mp_parsebyrise ,
      //mr_translateparsebyrisetree.mr_i_userinterface
      mp_translateparsebyrisetree->mr_i_userinterface
      )
  {
    InitializeXerces() ;
  }

  SAX2MainConfigHandler::~SAX2MainConfigHandler()
  {
    TerminateXerces() ;
  }

  void SAX2MainConfigHandler::HandleReadTranslationRuleFileXMLelement(
    const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes )
  {
    std::string stdstrPath ;
    if( XercesAttributesHelper::GetAttributeValue(
      cr_xercesc_attributes,
      "path",
      stdstrPath)
      )
    {
      LOGN("got path: " << stdstrPath )
      m_r_translationcontrollerbase.ReadTranslationRuleFile(
        m_sax2translationrulehandler , stdstrPath ) ;
    }
    else
      LOGN("Failed to get path.")
  }

  void SAX2MainConfigHandler::
    HandleReadVocabularyAttributeDefinitionFileXMLelement(
    const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes )
  {
    std::string stdstrPath ;
    std::wstring stdwstrErrorMessage ;
    if( XercesAttributesHelper::GetAttributeValue(
      cr_xercesc_attributes,
      "path",
      stdstrPath)
      )
    {
      SAX2VocAttributeDefinitionHandler sax2vocattributedefinitionhandler(
        * mp_translateparsebyrisetree ,
        mp_translateparsebyrisetree->mr_i_userinterface ) ;
      LOGN("got path: " << stdstrPath )
      // <> 0 = error
      if( //ReadViaSAX2InitAndTermXerces(
          ! ReadXMLfile_Inline(
          //"translation_rules.xml",
          stdstrPath.c_str() ,
      //    p_sax2grammarrulehandler ,
          & sax2vocattributedefinitionhandler ,
          stdwstrErrorMessage
          )
        )
      {
        LOGN("Successfully read voc attrib def file \"" << stdstrPath
          << "\"" )
  //          mr_i_userinterface.Message( wstr ) ;
      }
      else
      {
        LOGN("Failed to read voc attrib def file \"" << stdstrPath
          << "\"" )
        mp_translateparsebyrisetree->mr_i_userinterface.Message(
          stdwstrErrorMessage ) ;
      }
    }
    else
      LOGN("Failed to get path.")
  }

  void SAX2MainConfigHandler::startElement(
    const XMLCh * const cpc_xmlchURI,
    const XMLCh * const cpc_xmlchLocalName,
    const XMLCh * const cpc_xmlchQualifiedName,
    const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes
    )
  {
    char * pchXMLelementName = XERCES_CPP_NAMESPACE::XMLString::transcode(
      cpc_xmlchLocalName);
    if( pchXMLelementName )
    {
      LOGN( "XML element: " << pchXMLelementName );
      m_strElementName = std::string(pchXMLelementName) ;
      //LOG( "uri:" << uri << " localname:" << localname << " qname:" <<
      // qname << endl );
      if( m_strElementName == "translation_rule_file" )
      {
        HandleReadTranslationRuleFileXMLelement( cr_xercesc_attributes ) ;
      }
      else if( m_strElementName == "vocabulary_attribute_definition_file" )
      {
        HandleReadVocabularyAttributeDefinitionFileXMLelement(
          cr_xercesc_attributes ) ;
      }
      else if( m_strElementName == "vocabulary_file" )
      {
//        std::string stdstrFilePath ;
        std::wstring stdwstrErrorMessage ;
        if( XercesAttributesHelper::GetAttributeValue(
          cr_xercesc_attributes,
          "path",
          m_stdstrVocabularyFilePath)
          )
        {
          LOGN("got vocabulary_file path: " << //stdstrFilePath
            m_stdstrVocabularyFilePath )
//        if( OneLinePerWordPair::LoadWords( //pWordNodeCurrent
//             stdstrFilePath )
//          )
//        {
//        }
//        else
//          I_UserInterface::ShowVocFileLoadError(
//            stdstrFilePath,
//            stdwstrErrorMessage
//            ) ;
        }
      }
      //Release memory AFTER comparing.
      XERCES_CPP_NAMESPACE::XMLString::release( & pchXMLelementName );
    }
  }
}
