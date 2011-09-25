/*
 * SAX2TranslationRuleHandler.cpp
 *
 *  Created on: Jun 12, 2010
 *      Author: Stefan
 */

//convertFromAndToStdString(...)
#include <Controller/character_string/convertFromAndToStdString.hpp>
#include <Parse/ParseByRise.hpp> //class ParseByRise
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
  m_bConcatenatedTranslationRules(false)
  , mr_i_userinterface( r_userinterface)
  , mr_parsebyrise ( r_parsebyrise )
  , mr_translateparsebyrise (r_translateparsebyrise)
  , m_ui32ConcatenationID(0)
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
      "SAX2TranslationRuleHandler::endElement(...)--translation_rule"
      << " element" )
    if( ! m_stdstrTranslationRuleSyntaxTreePath.empty() )
    {
      TranslationRule * p_translationrule ;
      try
      {
        p_translationrule = new TranslationRule(
          m_stdstrTranslationRuleSyntaxTreePath
          , & mr_parsebyrise ) ;
        if( m_bConcatenatedTranslationRules)
        {
          //To find transl rules concatenated to the current one at translation.
          // e.g.   the              fan
          //        definite_article singular_noun
          //                       \ /
          //           definite_article_singular
          //
          //<concatenated_translation_rules>
          //  <translation_rule
          //  syntax_tree_path="definite_article_singular.definite_article" />
          //  <translation_rule
          //   syntax_tree_path="definite_article_singular.singular_noun" />
          //</concatenated_translation_rules>

          //in program memory:
          // TranslationRule tr1[..m_ui32ConcatenationID=0,
          //  m_syntax_tree_path = "definite_article_singular.definite_article"
          // TranslationRule tr2[..m_ui32ConcatenationID=0
          //  m_syntax_tree_path = "definite_article_singular.singular_noun"

          //inserting to leaf for "the" :
            mr_translateparsebyrise.
              m_std_multimapConcatenationID2p_translationrule.insert(
              std::make_pair( m_ui32ConcatenationID, p_translationrule)
            );
          p_translationrule->m_ui32ConcatenationID = m_ui32ConcatenationID;
        }
        if( ! m_std_strSyntaxTreePathForInsertionForTranslation.empty() )
        {
          std::string std_strUnknownGrammarPartID;
          if( p_translationrule->m_syntaxtreepathInsertionForTranslation.
              CreateGrammarPartIDArray(
                m_std_strSyntaxTreePathForInsertionForTranslation
                , p_translationrule->mp_parsebyrise
                , std_strUnknownGrammarPartID
              ) ==
                SyntaxTreePath::unknown_grammar_part_name
            )
            mr_i_userinterface.Message( "unknown grammar part name \"" +
                std_strUnknownGrammarPartID + "\" in STP \"" +
                m_std_strSyntaxTreePathForInsertionForTranslation +
                "\" in document\n"
              + Xerces::ToStdString( m_pc_locator->//getPublicId()
                getSystemId() )
              + "\nin line:"
              + convertToStdString<XMLFileLoc>( m_pc_locator->getLineNumber() )
              + ", column:"
              + convertToStdString<XMLFileLoc>( m_pc_locator->getColumnNumber() )
              ) ;
          m_std_strSyntaxTreePathForInsertionForTranslation = "";
        }
        p_translationrule->m_bySideWhereToInsertChildNode =
            m_uiTranslationInsertion;
        p_translationrule->m_bySideWhereToInsertParentNode =
            m_uiParentNodeInsertion;

//        p_translationrule->m_std_strParentNodeGrammarPartName =
        if( ! mr_parsebyrise.GetGrammarPartID(
            m_std_strParentNodeGrammarPartName,
            p_translationrule->m_uiParentNodeGrammarPartID
            )
          )
          p_translationrule->m_uiParentNodeGrammarPartID = 65534;
        if( ! mr_parsebyrise.GetGrammarPartID(
            m_std_strGrammarPartName,
            p_translationrule->m_uiChildNodeGrammarPartID
            )
          )
          p_translationrule->m_uiChildNodeGrammarPartID = 65534;

        LOGN("SAX2TranslationRuleHandler::endElement(...)--adding translation "
          "rule\"" << m_stdstrTranslationRuleSyntaxTreePath << "\"" )
        mr_translateparsebyrise.AddTranslationRule(
          p_translationrule
          , m_conditionsandtranslation ) ;
      }
      catch( const GetGrammarPartIDexception & c_r_getgrammarpartidexception )
      {
          mr_i_userinterface.Message(
            "Error adding translation rule for STP \"" +
            m_stdstrTranslationRuleSyntaxTreePath + "\" : "
            "unknown grammar part ID \"" +
               c_r_getgrammarpartidexception.m_stdstr + " \""
            );
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
        LOGN("SAX2TranslationRuleHandler::endElement(...)--creating translation "
          "rule for \"" << m_stdstrTranslationRuleSyntaxTreePath
          << "\" failed." )
      }
    }
    m_conditionsandtranslation.m_conditions.clear() ;
    m_conditionsandtranslation.m_stdstrAttributeName = "" ;
    m_conditionsandtranslation.m_stdstrGermanTranslation = "" ;
  }
  else if( //http://www.gnu.org/s/libc/manual/html_node/
      //String_002fArray-Comparison.html:
      //"If the two strings are equal, wcscmp returns 0."
      //wcscmp(...) does not work with wide chars unlike 2 byte (e.g. 4 byte).
//     ! wcscmp ( cpc_xmlchLocalName , L"condition" )
     //Compare 4 byte wide chars under Linux, 2 byte wide chars under Windows.
     ! Xerces::ansi_or_wchar_string_compare(
         cpc_xmlchLocalName ,
         ANSI_OR_WCHAR("concatenated_translation_rules")
         )
    )
  {
    m_bConcatenatedTranslationRules = false;
    ++ m_ui32ConcatenationID;
  }
}

void SAX2TranslationRuleHandler::HandleConditionXMLelement(
  const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes)
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
      )
    {
      LOGN("Got attribute value for \"condition\" element's syntax_tree_path:"
        "\"" << m_stdstrConditionSyntaxTreePath << "\"" )
      if( m_stdstrConditionSyntaxTreePath != ""
          && XercesAttributesHelper::getValue(
          cr_xercesc_attributes ,
          m_stdstrConditionAttributeName,
          //Use attribute name "German_plural" from the
          // VocabularyAndTranslation data.
          L"attribute_name"
          )
        )
      {
        LOGN("Got attribute value for \"condition\" element's "
          "\"attribute_name\":"
          "\"" << m_stdstrConditionAttributeName << "\"" )
        //The translation definition may refer a definition of an vocabulary
        //attribute definition (e.g. "German_noun_plural: string attribute
        //type, string at string array index 2).
        if( mr_translateparsebyrise.m_stdmap_AttrName2VocAndTranslAttrDef.
          find( m_stdstrConditionAttributeName) ==
          mr_translateparsebyrise.m_stdmap_AttrName2VocAndTranslAttrDef.end()
          )
        {
          mr_i_userinterface.Message( "The translation attribute definition "
            "for the name \"" + m_stdstrConditionAttributeName
            + "\" is not available."
            "It is referenced in in document\n"
            + Xerces::ToStdString( m_pc_locator->//getPublicId()
              getSystemId() )
            + "\nin line:"
            + convertToStdString<XMLFileLoc>( m_pc_locator->getLineNumber() )
            + ", column:"
            + convertToStdString<XMLFileLoc>( m_pc_locator->getColumnNumber() )
            ) ;
        }
        if( m_stdstrConditionAttributeName != ""
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
          LOGN("\"byte_attribute_value\" or "
            "\"string_attribute_value\" attribute")
          Condition cond ;
          std::string std_strUnknownGrammarPartID;
          if( cond.SetSyntaxTreePath(
                m_stdstrConditionSyntaxTreePath.c_str() ,
                & mr_parsebyrise ,
                std_strUnknownGrammarPartID
              ) == SyntaxTreePath::unknown_grammar_part_name
            )
          {
            //TODO error message
      //          cr_xercesc_attributes.
            mr_i_userinterface.Message( "unknown grammar part name \n\"" +
              std_strUnknownGrammarPartID + "\"\n for STP \n\"" +
              m_stdstrConditionSyntaxTreePath + "\"\n in document\n"
              + Xerces::ToStdString( m_pc_locator->//getPublicId()
                getSystemId() )
              + "\nin line:"
              + convertToStdString<XMLFileLoc>( m_pc_locator->getLineNumber() )
              + ", column:"
              + convertToStdString<XMLFileLoc>( m_pc_locator->getColumnNumber() )
              ) ;
          }
          cond.m_stdstrAttributeName = m_stdstrConditionAttributeName ;
      //        if( m_stdstrConditionByteAttrValue != "" )
          cond.m_byAttributeValue = //byAttributeValue ;
              wAttributeValue ;
          LOGN("SAX2TranslationRuleHandler::startElement(...)--adding condition")
          m_conditionsandtranslation.AddCondition( cond ) ;
        }
        else
        {
          LOGN("Neither a \"byte_attribute_value\" nor a "
            "\"string_attribute_value\" attribute")
          mr_i_userinterface.Message( "Neither a \"byte_attribute_value\" nor a "
              "\"string_attribute_value\" attribute\n"
              "-> NOT adding condition to the rule."
            "It is referenced in in document\n"
            + Xerces::ToStdString( m_pc_locator->//getPublicId()
              getSystemId() )
            + "\nin line:"
            + convertToStdString<XMLFileLoc>( m_pc_locator->getLineNumber() )
            + ", column:"
            + convertToStdString<XMLFileLoc>( m_pc_locator->getColumnNumber() )
            ) ;
        }
      }
    }
  }
  else
    LOGN("condition element--neither \"attribute_name\" attribute value nor "
        "\"translation\" attribute value")
}

void SAX2TranslationRuleHandler::HandleTranslationRuleElementName(
  const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes)
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
    LOGN("Successfully got syntax tree path:"
      << m_stdstrTranslationRuleSyntaxTreePath )
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
      LOGN("Successfully got attribute_name for translation_rule:"
        << m_stdstrTranslationRuleAttributeName )
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
          + "\" is not available."
          "It is referenced in in document\n"
          + Xerces::ToStdString( m_pc_locator->//getPublicId()
            getSystemId() )
          + "\nin line:"
          + convertToStdString<XMLFileLoc>( m_pc_locator->getLineNumber() )
          + ", column:"
          + convertToStdString<XMLFileLoc>( m_pc_locator->getColumnNumber() )
          ) ;
      }
      m_conditionsandtranslation.m_stdstrAttributeName =
          m_stdstrTranslationRuleAttributeName ;
    } //_either_ "attribute_name" (from main memory vocabulary attributes)
      //or "translation" (direct translation/ uses a string as translation)
    else if ( XercesAttributesHelper::getValue(
        cr_xercesc_attributes ,
        m_stdstrTranslation
        //Using directly a word for translation rather than an attribute
        // value of a VocabularyAndTranslation object is an alternative.
        , L"translation"
        )
      )
    {
      LOGN("Successfully got attribute value for \"translation\":"
        << m_stdstrTranslation )
      m_conditionsandtranslation.m_stdstrGermanTranslation =
          m_stdstrTranslation ;
    }
    if ( XercesAttributesHelper::getValue(
        cr_xercesc_attributes ,
        m_std_strSyntaxTreePathForInsertionForTranslation
        //Using directly a word for translation rather than an attribute
        // value of a VocabularyAndTranslation object is an alternative.
        , L"syntax_tree_path_where_to_insert_translation"
        )
      )
    {
      LOGN("Successfully got attribute value for "
        "\"syntax_tree_path_where_to_insert_translation\":"
        << m_std_strSyntaxTreePathForInsertionForTranslation )
//      m_conditionsandtranslation.m_stdstrGermanTranslation =
//          m_stdstrTranslation ;
    }
    else
      m_std_strSyntaxTreePathForInsertionForTranslation = "";
//    if( ! ConvertXercesAttributesValue<//BYTE
//  //      WORD
//        unsigned >(
//        cr_xercesc_attributes ,
//        //byAttributeValue ,
//        m_uiTranslationInsertion ,
//        L"insertion_direction"
//        )
//      )
//      m_uiTranslationInsertion = 0;
    std::string std_strInsertionForTranslationDirection;
    if( XercesAttributesHelper::getValue(
        cr_xercesc_attributes ,
        std_strInsertionForTranslationDirection,
        L"child_node_insertion_direction"
        )
      )
    {
        if( std_strInsertionForTranslationDirection == "left")
          m_uiTranslationInsertion = TranslationRule::left;
        else if( std_strInsertionForTranslationDirection == "right")
          m_uiTranslationInsertion = TranslationRule::right;
    }
    std::string std_strInsertionForParentNode;
    if( XercesAttributesHelper::getValue(
        cr_xercesc_attributes ,
        std_strInsertionForParentNode,
        L"parent_node_insertion_direction"
        )
      )
    {
        if( std_strInsertionForParentNode == "left")
          m_uiParentNodeInsertion = TranslationRule::left;
        else if( std_strInsertionForParentNode == "right")
          m_uiParentNodeInsertion = TranslationRule::right;
    }
    if( ! XercesAttributesHelper::getValue(
        cr_xercesc_attributes ,
        m_std_strParentNodeGrammarPartName,
        L"parent_node_grammar_part_name"
        )
      )
      m_std_strParentNodeGrammarPartName = "";
    if( ! XercesAttributesHelper::getValue(
        cr_xercesc_attributes ,
        m_std_strGrammarPartName,
        L"grammar_part_name"
        )
      )
      m_std_strGrammarPartName = "";
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
  if( pchXMLelementName)
  {
    LOGN("XML element:" << pchXMLelementName )
    //Release memory AFTER comparing.
    XERCES_CPP_NAMESPACE::XMLString::release( & pchXMLelementName);
  }
  if( ! Xerces::ansi_or_wchar_string_compare(
        cpc_xmlchLocalName ,
        ANSI_OR_WCHAR("translation_rule")
        )
    )
  {
//    m_strElementName = std::string(pchXMLelementName) ;
//    if( m_strElementName == "translation_rule" )
//    {
      HandleTranslationRuleElementName(cr_xercesc_attributes) ;
//    }
  }
  else
    if( //http://www.gnu.org/s/libc/manual/html_node/
        //String_002fArray-Comparison.html:
        //"If the two strings are equal, wcscmp returns 0."
        //wcscmp(...) does not work with wide chars unlike 2 byte (e.g. 4 byte).
  //     ! wcscmp ( cpc_xmlchLocalName , L"condition" )
       //Compare 4 byte wide chars under Linux, 2 byte wide chars under Windows.
       ! Xerces::ansi_or_wchar_string_compare(
           cpc_xmlchLocalName ,
           ANSI_OR_WCHAR("concatenated_translation_rules")
           )
      )
      m_bConcatenatedTranslationRules = true;
    else if( //http://www.gnu.org/s/libc/manual/html_node/
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
    HandleConditionXMLelement(cr_xercesc_attributes);
  }
}
