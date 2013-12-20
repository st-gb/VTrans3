/*
 * SAX2MainConfigHandler.cpp
 *
 *  Created on: Nov 28, 2010
 *      Author: Stefan
 */
//class TranslationControllerBase
#include <Controller/TranslationControllerBase.hpp>
//#include <IO/IO.hpp> //OneLinePerWordPair::LoadWords()
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
    m_sax2grammarrulehandler ( r_translationcontrollerbase.m_parsebyrise ) ,
    m_sax2transformationrulehandler( r_translationcontrollerbase ) ,
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

  void SAX2MainConfigHandler::HandleGrammartPartColourXMLelement(
      const XERCES_CPP_NAMESPACE::Attributes & c_r_xercesc_attributes)
  {
    std::string std_strGrammarPartName ;
    if( XercesAttributesHelper::GetAttributeValue(
      c_r_xercesc_attributes,
      "name",
      std_strGrammarPartName)
      == XercesAttributesHelper::getting_attribute_value_succeeded
      )
    {
      DWORD dwValue;
      XercesAttributesHelper xercesattributeshelper;
      if( xercesattributeshelper.GetAttributeValue(
          c_r_xercesc_attributes,
          "colour", //const char * lpctstrAttrName,
          dwValue //WORD & r_wValue
          ) == XercesAttributesHelper::getting_attribute_value_succeeded
        )
      {
          m_r_translationcontrollerbase.m_nodetrie_ui32GrammarPartName2colour.
            insert_inline(
            (BYTE *) std_strGrammarPartName.c_str(),
            std_strGrammarPartName.length(),
            dwValue
            );
      }
    }
  }

  void SAX2MainConfigHandler::HandleGUI_XMLelement(
    const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes )
  {
    std::string stdstrPath ;
    if( XercesAttributesHelper::GetAttributeValue(
      cr_xercesc_attributes,
      "draw_pointer_addresses",
      m_r_translationcontrollerbase.m_GUIattributes.m_bShowGrammarPartAddress)
      == XercesAttributesHelper::getting_attribute_value_succeeded
      )
    {

    }
  }

  void SAX2MainConfigHandler::HandleReadGrammarRuleFileXMLelement(
      const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes )
  {
    std::string stdstrPath ;
    if( XercesAttributesHelper::GetAttributeValue(
      cr_xercesc_attributes,
      "path",
      stdstrPath)
      )
    {
      LOGN_DEBUG("got path: " << stdstrPath )
      m_r_translationcontrollerbase.ReadGrammarRuleFile(
        /*m_sax2grammarrulehandler ,*/ stdstrPath ) ;
    }
    else
      LOGN_ERROR("Failed to get path.")
  }

  void SAX2MainConfigHandler::HandleReadTransformationRuleFileXMLelement(
    const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes )
  {
    std::string stdstrPath ;
    if( XercesAttributesHelper::GetAttributeValue(
      cr_xercesc_attributes,
      "path",
      stdstrPath)
      )
    {
      LOGN_DEBUG("got path: " << stdstrPath )
      ReadXMLfile( m_sax2transformationrulehandler , stdstrPath ) ;
    }
    else
      LOGN_ERROR("Failed to get path.")
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
      LOGN_DEBUG("got path: " << stdstrPath )
      m_r_translationcontrollerbase.ReadTranslationRuleFile(
        /*m_sax2translationrulehandler ,*/ stdstrPath ) ;
    }
    else
      LOGN_ERROR("Failed to get path.")
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
      LOGN_DEBUG("got path: " << stdstrPath )
//      // <> 0 = error
//      if( //ReadViaSAX2InitAndTermXerces(
//          ! ReadXMLfile_Inline(
//          //"translation_rules.xml",
//          stdstrPath.c_str() ,
//      //    p_sax2grammarrulehandler ,
//          & sax2vocattributedefinitionhandler ,
//          stdwstrErrorMessage
//          )
//        )
//      {
//        LOGN("Successfully read voc attrib def file \"" << stdstrPath
//          << "\"" )
//  //          mr_i_userinterface.Message( wstr ) ;
//      }
//      else
//      {
//        LOGN("Failed to read voc attrib def file \"" << stdstrPath
//          << "\"" )
//        mp_translateparsebyrisetree->mr_i_userinterface.Message(
//          stdwstrErrorMessage ) ;
//      }
      m_r_translationcontrollerbase.ReadVocAttributeDefinitionFile(
        /*sax2vocattributedefinitionhandler ,*/ stdstrPath ) ;
    }
    else
      LOGN_ERROR("Failed to get path.")
  }

  void SAX2MainConfigHandler::ReadXMLfile(
    XERCES_CPP_NAMESPACE::DefaultHandler & r_xerces_defaulthandler ,
    const std::string & cr_stdstrFilePath
    )
  {
    LOGN_DEBUG(//"TranslationControllerBase::ReadTranslationRuleFile( "
      "\"" << cr_stdstrFilePath << "\"" /*")"*/ )
    std::wstring stdwstrErrorMessage ;
    // <> 0 = error
    if( //ReadViaSAX2InitAndTermXerces(
        ! Apache::Xerces::ReadXMLfileInitAndTermXerces_Inline(
        //"translation_rules.xml",
        cr_stdstrFilePath.c_str() ,
    //    p_sax2grammarrulehandler ,
        & r_xerces_defaulthandler ,
        stdwstrErrorMessage
        )
      )
    {
      LOGN_DEBUG("Successfully read XML file \"" << cr_stdstrFilePath
        << "\"" )
    //          mr_i_userinterface.Message( wstr ) ;
    }
    else
    {
      LOGN_ERROR("Failed to read XML file \"" << cr_stdstrFilePath
        << "\"" )
      m_r_translationcontrollerbase.Message(
        stdwstrErrorMessage ) ;
    }
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
      LOGN_DEBUG( "XML element: " << pchXMLelementName );
      m_strElementName = std::string(pchXMLelementName) ;
      //LOG( "uri:" << uri << " localname:" << localname << " qname:" <<
      // qname << endl );
      if( m_strElementName == "grammar_part" )
        HandleGrammartPartColourXMLelement(cr_xercesc_attributes);
      else if( m_strElementName == "drawing" )
        HandleGUI_XMLelement(cr_xercesc_attributes);
      else if( m_strElementName == "grammar_rule_file" )
      {
        HandleReadGrammarRuleFileXMLelement( cr_xercesc_attributes ) ;
      }
      else if( m_strElementName == "transformation_rule_file" )
      {
        HandleReadTransformationRuleFileXMLelement( cr_xercesc_attributes ) ;
      }
      else if( m_strElementName == "translation_rule_file" )
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
          LOGN_DEBUG("got vocabulary_file path: " << //stdstrFilePath
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
