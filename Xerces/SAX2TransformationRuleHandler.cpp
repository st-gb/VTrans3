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
//for Xerces::ConvertXercesStringToStdWstring(...)
#include <Xerces/XercesString.hpp>
#include <Xerces/SAX2TransformationRuleHandler.hpp>

#include <xercesc/sax/Locator.hpp> //class XERCES_CPP_NAMESPACE::Locator
//for XERCES_CPP_NAMESPACE::XMLString::transcode(...)
#include <xercesc/util/XMLString.hpp>

#include <string> //std::string

#define SECOND_SYNTAX_TREE_LITERAL "second_syntax_tree_path"

#define L(x) L##x

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

  //A second STP when 2 branches should be exchanged by each other.
  //(e.g. exchange "have" and "seen" at "The car you have seen"
  //                                                   \  /
  bool SAX2TransformationRuleHandler::PossiblyAdd2ndSyntaxTreePath(
    const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes,
    TransformationRule & r_transformationrule
    )
  {
    bool b2ndSyntaxTreePathAdded = false;
    std::string stdstr2ndSyntaxTreePathOfBranchToMove;
    if( XercesAttributesHelper::GetAttributeValue(
        cr_xercesc_attributes ,
        SECOND_SYNTAX_TREE_LITERAL ,
        stdstr2ndSyntaxTreePathOfBranchToMove
        ) == XercesAttributesHelper::getting_attribute_value_succeeded
      )
    {
      try
      {
        SyntaxTreePath stp(
           stdstr2ndSyntaxTreePathOfBranchToMove ,
           //& m_r_parsebyrise
           & m_r_translationcontrollerbase.m_parsebyrise );
        r_transformationrule.m_syntaxtreepath2nd = //(
//          = SyntaxTreePath(
//              stdstr2ndSyntaxTreePathOfBranchToMove ,
//          //& m_r_parsebyrise
//          & m_r_translationcontrollerbase.m_parsebyrise );
          stp;
        b2ndSyntaxTreePathAdded = true;
      }
      catch( const GetGrammarPartIDexception & c_r_getgrammarpartidexception )
      {
        m_r_translationcontrollerbase.Message(
          "Error getting Syntax Tree Path for \"" +
          stdstr2ndSyntaxTreePathOfBranchToMove + "\" : "
          "unknown grammar part ID \"" +
             c_r_getgrammarpartidexception.m_stdstr + " \""
          );
      }
    }
    return b2ndSyntaxTreePathAdded;
  }

  bool SAX2TransformationRuleHandler::PossiblyAddSyntaxTreePathWhereToInsert(
    const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes,
    TransformationRule & r_transformationrule
    )
  {
    bool bSyntaxTreePathWhereToInsertAdded = false;
    std::string stdstrPlaceNearSyntaxTreePath ;
    if( XercesAttributesHelper::GetAttributeValue(
        cr_xercesc_attributes ,
        "place_near_syntax_tree_path" ,
        stdstrPlaceNearSyntaxTreePath
        ) == XercesAttributesHelper::getting_attribute_value_succeeded
      )
    {
      r_transformationrule.m_stdstrParseTreePathWhereToInsert =
          stdstrPlaceNearSyntaxTreePath ;
      try
      {
        r_transformationrule.m_syntaxtreepathWhereToInsert =
          SyntaxTreePath( stdstrPlaceNearSyntaxTreePath ,
          //& m_r_parsebyrise
          & m_r_translationcontrollerbase.m_parsebyrise );
      }
      catch( const GetGrammarPartIDexception & c_r_getgrammarpartidexception )
      {
        m_r_translationcontrollerbase.Message(
          "Error getting Syntax Tree Path for \"" +
          stdstrPlaceNearSyntaxTreePath + "\" : "
          "unknown grammar part ID \"" +
             c_r_getgrammarpartidexception.m_stdstr + " \""
          );
      }
      bSyntaxTreePathWhereToInsertAdded = true;
    }
    return bSyntaxTreePathWhereToInsertAdded;
  }

  void SAX2TransformationRuleHandler::setDocumentLocator(
    const XERCES_CPP_NAMESPACE::Locator * const cpc_locator )
  {
    m_pc_locator = cpc_locator ;
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
        std::string stdstrNameOfGrammarPartToInsert;
        std::string stdstrDirection ;
        if( XercesAttributesHelper::GetAttributeValue(
            cr_xercesc_attributes ,
            "syntax_tree_path" ,
            stdstrSyntaxTreePathOfBranchToMove
            ) == XercesAttributesHelper::getting_attribute_value_succeeded
           &&
             XercesAttributesHelper::GetAttributeValue(
             cr_xercesc_attributes ,
             "direction" ,
             stdstrDirection
             ) == XercesAttributesHelper::getting_attribute_value_succeeded
          )
        {
          bool b2ndSyntaxTreePathAdded =
            PossiblyAdd2ndSyntaxTreePath(cr_xercesc_attributes,
              transformationrule
            );
          bool bSyntaxTreePathWhereToInsert =
            PossiblyAddSyntaxTreePathWhereToInsert(cr_xercesc_attributes,
            transformationrule);

          if( b2ndSyntaxTreePathAdded && bSyntaxTreePathWhereToInsert )
          {
            std::wostringstream stdwos;
            stdwos << L"for transformation rule:\n"
                L"in document " << Xerces::ConvertXercesStringToStdWstring(
                    m_pc_locator->getSystemId() )
                << L"\nin line " << m_pc_locator->getLineNumber()
                << L", column " << m_pc_locator->getColumnNumber()
                << L"\nwarning: both attributes "
                L"\"" << //L(SECOND_SYNTAX_TREE_LITERAL)
                L"second_syntax_tree_path"
                << L"\" and "
                L"\"place_near_syntax_tree_path\" specified";
            m_r_translationcontrollerbase.Message( stdwos.str()
                );
          }
           if( XercesAttributesHelper::GetAttributeValue(
               cr_xercesc_attributes ,
               "name_of_grammar_part_to_insert" ,
  //             stdstrNameOfGrammarPartToInsert
               transformationrule.m_stdstrNameOfGrammarPartToInsert
               ) == XercesAttributesHelper::getting_attribute_value_succeeded
             )
           {
            WORD wGrammarPartID = 0;
            if( ! m_r_translationcontrollerbase.m_parsebyrise.GetGrammarPartID(
                transformationrule.m_stdstrNameOfGrammarPartToInsert,
                wGrammarPartID)
                )
              wGrammarPartID = 1;
           }
          transformationrule.m_bInsertLeftChild =
            ( stdstrDirection == "left" ) ?
            true : false ;
          try
          {
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
          catch( const GetGrammarPartIDexception & c_r_getgrammarpartidexception )
          {
              m_r_translationcontrollerbase.Message(
                "Error inserting transformation rule for STP \"" +
                stdstrSyntaxTreePathOfBranchToMove + "\" : "
                "unknown grammar part ID \"" +
                   c_r_getgrammarpartidexception.m_stdstr + " \""
                );
          }
        }
      }
      //Release memory AFTER comparing.
      XERCES_CPP_NAMESPACE::XMLString::release( & pchXMLelementName);
    }
  }
}
