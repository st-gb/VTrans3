/*
 * SAX2TransformationRuleHandler.cpp
 *
 *  Created on: Dec 21, 2010
 *      Author: Stefan
 */
//class TranslationControllerBase
#include <Controller/TranslationControllerBase.hpp>
#include <Parse/ParseByRise.hpp> //class ParseByRise
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
#include <Translate/SyntaxTreePath.hpp> //class SyntaxTreePath
#include <Translate/TransformationRule.hpp> //class TransformationRule
//XercesAttributesHelper::GetAttributeValue(...)
#include <Xerces/XercesAttributesHelper.hpp>
#include <Xerces/SAX2TransformationRuleHandler.hpp>
//for XERCES_CPP_NAMESPACE::XMLString::transcode(...)
#include <xercesc/util/xmlstring.hpp>

#include <string> //std::string

namespace Xerces
{

  SAX2TransformationRuleHandler::SAX2TransformationRuleHandler(
//    ParseByRise & r_parsebyrise
    TranslationControllerBase & r_translationcontrollerbase
    )
    :
    m_r_translationcontrollerbase( r_translationcontrollerbase) //,
    //m_r_parsebyrise ( //r_parsebyrise
//      r_translationcontrollerbase.m_parsebyrise )
  {
    LOGN("SAX2TransformationRuleHandler()")
  }

  SAX2TransformationRuleHandler::~SAX2TransformationRuleHandler()
  {
    // TODO Auto-generated destructor stub
  }

  void SAX2TransformationRuleHandler::startElement
    (
    const XMLCh * const cpc_xmlchURI,
    const XMLCh * const cpc_xmlchLocalName,
    const XMLCh * const cpc_xmlchQualifiedName,
    const //Use namespace prefix here for clarifications that Attributes is
      //part of / belongs to Xerces
      XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes
    )
  {
    LOGN("SAX2TransformationRuleHandler::startElement begin")
    char * pchXMLelementName = XERCES_CPP_NAMESPACE::XMLString::transcode(
      cpc_xmlchLocalName);
    if( pchXMLelementName )
    {
      std::string stdstrXMLelementName( pchXMLelementName ) ;
      if( stdstrXMLelementName == "transformation_rule" )
      {
        TransformationRule transformationrule ;
        std::string stdstrSyntaxTreePathOfBranchToMove ;
        std::string stdstrPlaceNearSyntaxTreePath ;
        std::string stdstrDirection ;
        if( XercesAttributesHelper::GetAttributeValue(
            cr_xercesc_attributes ,
            "syntax_tree_path" ,
            stdstrSyntaxTreePathOfBranchToMove
            ) == XercesAttributesHelper::getting_attribute_value_succeeded
           &&
             XercesAttributesHelper::GetAttributeValue(
             cr_xercesc_attributes ,
             "place_near_syntax_tree_path" ,
             stdstrPlaceNearSyntaxTreePath
             ) == XercesAttributesHelper::getting_attribute_value_succeeded
           &&
             XercesAttributesHelper::GetAttributeValue(
             cr_xercesc_attributes ,
             "syntax_tree_path" ,
             stdstrDirection
             ) == XercesAttributesHelper::getting_attribute_value_succeeded
          )
        {
          transformationrule.m_bInsertLeftChild =
            ( stdstrDirection == "left" ) ?
            true : false ;
          transformationrule.m_stdstrParseTreePathWhereToInsert =
              stdstrPlaceNearSyntaxTreePath ;
          m_r_translationcontrollerbase.
            m_stdmap_syntaxtreepath2transformationrule.insert(
            std::make_pair(
              SyntaxTreePath( stdstrSyntaxTreePathOfBranchToMove ,
                //& m_r_parsebyrise
                & m_r_translationcontrollerbase.m_parsebyrise ) ,
              transformationrule
              )
            ) ;
        }
      }
      //Release memory AFTER comparing.
      XERCES_CPP_NAMESPACE::XMLString::release( & pchXMLelementName);
    }
  }
}
