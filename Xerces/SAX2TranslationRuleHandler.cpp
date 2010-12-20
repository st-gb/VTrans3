/*
 * SAX2TranslationRuleHandler.cpp
 *
 *  Created on: Jun 12, 2010
 *      Author: Stefan
 */

#include <Controller/character_string/stdstring_format.hpp> //to_stdstring(...)
//#include <Translate/ConditionsAndTranslation.hpp>
#include <Translate/TranslationRule.hpp>//class TranslationRule
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>
#include <UserInterface/I_UserInterface.hpp>//class I_UserInterface
//header file for this SAX2TranslationRuleHandler class
#include <Xerces/SAX2TranslationRuleHandler.hpp>
#include <Xerces/XercesAttributesHelper.hpp>//ConvertXercesAttributesValue(...)
#include <Xerces/XercesString.hpp> //Xerces::ToStdString(const XMLCh * )

#include <xercesc/sax/Locator.hpp> //class XERCES_CPP_NAMESPACE::Locator
#include <xercesc/sax2/Attributes.hpp>//class  XERCES_CPP_NAMESPACE::Attributes
//for XERCES_CPP_NAMESPACE::XMLString::transcode(...)
#include <xercesc/util/xmlstring.hpp>
#include <string.h> //for wcscmp (const wchar_t*, const wchar_t*);
#include <string> //class std::string

#define TRANSLATION_RULE_XML_ELEMENT_ANSI "translation_rule"

SAX2TranslationRuleHandler::SAX2TranslationRuleHandler(
  TranslateParseByRiseTree & r_translateparsebyrise ,
  ParseByRise & r_parsebyrise
  , I_UserInterface & r_userinterface
  )
  :
  mr_i_userinterface( r_userinterface)
  , mr_parsebyrise ( r_parsebyrise )
  , mr_translateparsebyrise (r_translateparsebyrise)
{

}

SAX2TranslationRuleHandler::~SAX2TranslationRuleHandler()
{
  // TODO Auto-generated destructor stub
}

//This callback function is needed because a translation rule can be added
// at first when all inner possible conditions were got.
void SAX2TranslationRuleHandler::endElement(
  const XMLCh * const cpc_xmlchURI,
  const XMLCh * const cpc_xmlchLocalName,
  const XMLCh * const cpc_xmlchQualifiedName
  )
{
  if(
//      //http://www.gnu.org/s/libc/manual/html_node/
//      // String_002fArray-Comparison.html:
//      //"If the two strings are equal, wcscmp returns 0."
       //wcscmp(...) does not work with wide chars unlike 2 byte (e.g. 4 byte).
//     ! wcscmp ( cpc_xmlchLocalName , L"translation_rule" )

      //Compare 4 byte wide chars under Linux, 2 byte wide chars under Windows.
      ! Xerces::ansi_or_wchar_string_compare(
          cpc_xmlchLocalName,
          ANSI_OR_WCHAR( //TRANSLATION_RULE_XML_ELEMENT_ANSI
          "translation_rule" )
        )
    )
  {
    LOGN( //TRANSLATION_RULE_XML_ELEMENT_ANSI
      "translation_rule" << " element" )
    if( ! m_stdstrTranslationRuleSyntaxTreePath.empty() )
    {
      TranslationRule * p_translationrule ;
      try
      {
        p_translationrule = new TranslationRule(
          m_stdstrTranslationRuleSyntaxTreePath
          , & mr_parsebyrise ) ;
        LOGN("SAX2TranslationRuleHandler::endElement(...)--adding translation "
          "rule")
        mr_translateparsebyrise.AddTranslationRule(
          p_translationrule
          , m_conditionsandtranslation ) ;
      }
      catch( //std::string e
//          GetGrammarPartIDexception & e
          std::exception & e)
      {
        //http://www.parashift.com/c++-faq-lite/exceptions.html#faq-17.8:
        //"If a constructor throws an exception, the object's destructor is not
        // run."
        //If a constructor throws an exc. a dyn. created obj. does not exist?
//        delete p_translationrule ;
//        mr_i_userinterface.Message( "\"" + e.m_stdstr + "\" is an unknown"
//          "grammar part name") ;
      }
    }
    m_conditionsandtranslation.m_conditions.clear() ;
    m_conditionsandtranslation.m_stdstrAttributeName = "" ;
    m_conditionsandtranslation.m_stdstrGermanTranslation = "" ;
  }
}

void SAX2TranslationRuleHandler::setDocumentLocator(
  const XERCES_CPP_NAMESPACE::Locator * const cpc_locator )
{
  m_pc_locator = cpc_locator ;
}

void SAX2TranslationRuleHandler::startElement
  (
  const XMLCh * const cpc_xmlchURI,
  const XMLCh * const cpc_xmlchLocalName,
  const XMLCh * const cpc_xmlchQualifiedName,
  const //Use namespace prefix here for clarifications that Attributes is
    //part of / belongs to Xerces
    XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes
  )
{
  LOGN("SAX2TranslationRuleHandler::startElement begin")
  char * pchXMLelementName = XERCES_CPP_NAMESPACE::XMLString::transcode(
    cpc_xmlchLocalName);
  if( pchXMLelementName )
  {
    LOGN("XML element:" << pchXMLelementName )
    m_strElementName = std::string(pchXMLelementName) ;
    if( m_strElementName == "translation_rule" )
    {
      //If this path corresponds to the path of the _current_ syntax
      //tree path then it is checked whether conditons (if exist) are true and
      //if yes: applies the translation.
      if( XercesAttributesHelper::getValue(
        cr_xercesc_attributes ,
        m_stdstrTranslationRuleSyntaxTreePath ,
        "syntax_tree_path"
        )
      )
      {
        if( XercesAttributesHelper::getValue(
            cr_xercesc_attributes ,
            m_stdstrTranslationRuleAttributeName,
            //This is the name for the attribute value to choose for
            //translation if the syntax tree path and the conditions match:
            // e.g. attribute_name is "German_plural", current syntax tree
            // path is "definite_plural.plural", translation rule's syntax
            // tree path is "plural" then:
            // use attribute name "German_plural" from the
            // VocabularyAndTranslation data.
            "attribute_name"
            )
          )
        {
          //The translation definition may refer a definition of an vocabulary
          //attribute definition (e.g. "German_noun_plural: string attribute
          //type, string at string array index 2).
          if( mr_translateparsebyrise.m_stdmap_AttrName2VocAndTranslAttrDef.
            find( m_stdstrTranslationRuleAttributeName) ==
            mr_translateparsebyrise.m_stdmap_AttrName2VocAndTranslAttrDef.end()
            )
          {
            mr_i_userinterface.Message( "The translation attribute definition "
              "for the name \"" + m_stdstrTranslationRuleAttributeName
              + "\" is not available.") ;
          }
          m_conditionsandtranslation.m_stdstrAttributeName =
              m_stdstrTranslationRuleAttributeName ;
        }
        else if ( XercesAttributesHelper::getValue(
            cr_xercesc_attributes ,
            m_stdstrTranslation
            //Using directly a word for translation rather than an attribute
            // value of a VocabularyAndTranslation object is an alternative.
            , L"translation"
            )
          )
          m_conditionsandtranslation.m_stdstrGermanTranslation =
              m_stdstrTranslation ;
      }
    }
    //Release memory AFTER comparing.
    XERCES_CPP_NAMESPACE::XMLString::release( & pchXMLelementName);
  }
  if( //http://www.gnu.org/s/libc/manual/html_node/
      //String_002fArray-Comparison.html:
      //"If the two strings are equal, wcscmp returns 0."
      //wcscmp(...) does not work with wide chars unlike 2 byte (e.g. 4 byte).
//     ! wcscmp ( cpc_xmlchLocalName , L"condition" )
     //Compare 4 byte wide chars under Linux, 2 byte wide chars under Windows.
     ! Xerces::ansi_or_wchar_string_compare(
         cpc_xmlchLocalName ,
         ANSI_OR_WCHAR("condition")
         )
    )
  {
    if( m_stdstrTranslationRuleAttributeName != "" || m_stdstrTranslation != ""
        )
    {
//      BYTE byAttributeValue ;
      //Use an _integer_ data type for the string to be converted to a number.
      WORD wAttributeValue ;
      if( XercesAttributesHelper::getValue(
        cr_xercesc_attributes ,
        m_stdstrConditionSyntaxTreePath ,
        L"syntax_tree_path"
        )
        && m_stdstrConditionSyntaxTreePath != ""
        && XercesAttributesHelper::getValue(
          cr_xercesc_attributes ,
          m_stdstrConditionAttributeName,
          //Use attribute name "German_plural" from the
          // VocabularyAndTranslation data.
          L"attribute_name"
          )
        && m_stdstrConditionAttributeName != ""
        && (
//            XercesAttributesHelper::getValue(
//          attrs ,
//          m_stdstrConditionByteAttrValue,
//          //Use attribute name "German_plural" from the
//          // VocabularyAndTranslation data.
//          L"byte_attribute_value"
//          )
            ConvertXercesAttributesValue<//BYTE
              WORD>(
              cr_xercesc_attributes ,
              //byAttributeValue ,
              wAttributeValue ,
              L"byte_attribute_value"
              )
        ||
          XercesAttributesHelper::getValue(
          cr_xercesc_attributes ,
          m_stdstrConditionStringAttrValue ,
          //Use attribute name "German_plural" from the
          // VocabularyAndTranslation data.
          L"string_attribute_value"
          )
        )
        )
      {
        Condition cond ;
        if( cond.SetSyntaxTreePath( m_stdstrConditionSyntaxTreePath.c_str() ,
          & mr_parsebyrise ) == SyntaxTreePath::unknown_grammar_part_name
          )
        {
          //TODO error message
//          cr_xercesc_attributes.
          mr_i_userinterface.Message( "unknown grammar part name in document\n"
            + Xerces::ToStdString( m_pc_locator->//getPublicId()
              getSystemId() )
            + "\nin line:"
            + to_stdstring<XMLFileLoc>( m_pc_locator->getLineNumber() )
            + ", column:"
            + to_stdstring<XMLFileLoc>( m_pc_locator->getColumnNumber() )
            ) ;
        }
        cond.m_stdstrAttributeName = m_stdstrConditionAttributeName ;
//        if( m_stdstrConditionByteAttrValue != "" )
        cond.m_byAttributeValue = //byAttributeValue ;
            wAttributeValue ;
        LOGN("SAX2TranslationRuleHandler::startElement(...)--adding condition")
        m_conditionsandtranslation.AddCondition( cond ) ;
      }
    }
  }
}
