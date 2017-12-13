/*
 * SAX2TranslationRuleHandler.cpp
 *
 *  Created on: Jun 12, 2010
 *      Author: Stefan
 */

//convertFromAndToStdString(...)
#include <Controller/character_string/convertFromAndToStdString.hpp>
//#include <Translate/ConditionsAndTranslation.hpp>
#ifdef COMPILE_AS_EXECUTABLE
  //#include <wxWidgets/VTransApp.hpp> //wxGetApp()
  #include <Translate/TranslateParseByRiseTree.hpp> //
  #include <UserInterface/I_UserInterface.hpp> //
#endif //#ifdef COMPILE_AS_EXECUTABLE

/** header file for this SAX2TranslationRuleHandler class */
#include <Xerces/SAX2TranslationRuleHandler.hpp>
#include <Xerces/XercesAttributesHelper.hpp>//ConvertXercesAttributesValue(...)
#include <Xerces/XercesString.hpp> //Xerces::ToStdString(const XMLCh * )
#include <Xerces/ReadViaSAX2.hpp>

#include <xercesc/sax/Locator.hpp> //class XERCES_CPP_NAMESPACE::Locator
#include <xercesc/sax2/Attributes.hpp>//class  XERCES_CPP_NAMESPACE::Attributes
//for XERCES_CPP_NAMESPACE::XMLString::transcode(...)
#include <xercesc/util/XMLString.hpp>

#include <string.h> //for wcscmp (const wchar_t*, const wchar_t*);
#include <string>

//#include "wxWidgets/UserInterface/UserInterface.hpp" //class std::string
#include <Translate/TranslationRule.hpp>//class TranslationRule

extern TranslationControllerBase * g_p_translationcontrollerbase;

SAX2TranslationRuleHandler::SAX2TranslationRuleHandler(
  TranslateParseByRiseTree & r_translateparsebyrise ,
  BottomUpParser & r_bottomUpParser
  , I_UserInterface & r_userinterface
  , ConfigurationHandler_type & configurationReader
  )
  : VTrans3::TranslationRuleFileReaderBase(
    r_translateparsebyrise
    , r_bottomUpParser
    , r_userinterface
    , configurationReader,
    g_p_translationcontrollerbase )
{

}

SAX2TranslationRuleHandler::~SAX2TranslationRuleHandler()
{
  // TODO Auto-generated destructor stub
}

/** @brief This callback function is needed because a translation rule can be
* added at first when all inner possible conditions were got. */
void SAX2TranslationRuleHandler::endElement(
  const XMLCh * const cpc_xmlchURI,
  const XMLCh * const cpc_xmlchLocalName,
  const XMLCh * const cpc_xmlchQualifiedName
  )
{
  if(
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
    EndTranslationRuleXMLelement();
  }
  else if(
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

/*template <typename XMLelementType>
  bool SAX2TranslationRuleHandler::GetAttributeValue(
    XMLelementType & xmlElement,
    const char * const attributeName,
    std::string & std_strXMLattributeValue
    )
{
  return XercesAttributesHelper::getValue(
    xmlElement,
    attributeName,
    std_strXMLattributeValue
    );
//  return std_strXMLattributeValue;
}*/

void SAX2TranslationRuleHandler::GetAttributeNameOrTranslationString(
  const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes)
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
      ATTRIBUTE_NAME_ANSI_STRING
      )
    )
  {
    LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "Successfully got attribute_name for "
      "translation_rule: \"" << m_stdstrTranslationRuleAttributeName << "\"")
    //The translation definition may refer a definition of an vocabulary
    //attribute definition (e.g. "German_noun_plural: string attribute
    //type, string at string array index 2).
    if( mr_translateparsebyrise.m_stdmap_AttrName2VocAndTranslAttrDef.
      find( m_stdstrTranslationRuleAttributeName) ==
      mr_translateparsebyrise.m_stdmap_AttrName2VocAndTranslAttrDef.end()
      )
    {
      ShowMessageToUser(
        "The translation attribute definition "
        "for the name \"" + m_stdstrTranslationRuleAttributeName
        + "\" is not available."
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
//        , L"translation"
      , TRANSLATION_ANSI_STRING
      )
    )
  {
    LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "Successfully got attribute value for "
      "\"translation\":" << m_stdstrTranslation )
    m_conditionsandtranslation.m_stdstrGermanTranslation =
        m_stdstrTranslation ;
  }
  else
  {
    ShowMessageToUser("neither \"" ATTRIBUTE_NAME_ANSI_STRING "\" nor \""
      TRANSLATION_ANSI_STRING "\" specified for STP \"" +
      m_stdstrTranslationRuleSyntaxTreePath + "\""
      );
  }
}

void SAX2TranslationRuleHandler::GetStringTransformationFunctionName(
  const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes)
{
  std::string std_strStringTransformationFunctionName;
  if(
      XercesAttributesHelper::getValue(
      cr_xercesc_attributes ,
      std_strStringTransformationFunctionName ,
      //Use attribute name "German_plural" from the
      // VocabularyAndTranslation data.
//              L"string_attribute_value"
      "string_transformation_function_name"
      )
    )
  {
    std::map<std::string, pfnTransformString>::const_iterator c_iter
      = ConditionsAndTranslation::s_std_mapFunctionName2Function.find(
      std_strStringTransformationFunctionName);
    if( c_iter != ConditionsAndTranslation::
      s_std_mapFunctionName2Function.end() )
    {
      LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "assigning function \"" <<
        std_strStringTransformationFunctionName << "\" for current "
        "conditionsandtranslation object" )
      m_conditionsandtranslation.m_pfn_TransformString =
        c_iter->second;
    }
  }
}

void SAX2TranslationRuleHandler::GetParentNodeInsertionDirection(
  const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes)
{
  std::string std_strInsertionForParentNode;
  if( XercesAttributesHelper::getValue(
      cr_xercesc_attributes,
      std_strInsertionForParentNode,
      "parent_node_insertion_direction")
    )
  {
    if(std_strInsertionForParentNode == "left")
      m_uiParentNodeInsertion = TranslationRule::left;
    else
      if(std_strInsertionForParentNode == "right")
        m_uiParentNodeInsertion = TranslationRule::right;
  }
}

void SAX2TranslationRuleHandler::GetChildNodeInsertionDirection(
  const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes)
{
  std::string std_strInsertionForTranslationDirection;
  if(XercesAttributesHelper::getValue(
      cr_xercesc_attributes,
      std_strInsertionForTranslationDirection,
      "child_node_insertion_direction")
    )
  {
    if(std_strInsertionForTranslationDirection == "left")
      m_uiTranslationInsertion = TranslationRule::left;
    else
      if(std_strInsertionForTranslationDirection == "right")
        m_uiTranslationInsertion = TranslationRule::right;
  }
}

void SAX2TranslationRuleHandler::GetSyntaxTreePathWhereToInsertTranslation(
  const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes)
{
  if ( XercesAttributesHelper::getValue(
      cr_xercesc_attributes ,
      m_std_strSyntaxTreePathForInsertionForTranslation
      //Using directly a word for translation rather than an attribute
      // value of a VocabularyAndTranslation object is an alternative.
//        , L"syntax_tree_path_where_to_insert_translation"
      , "syntax_tree_path_where_to_insert_translation"
      )
    )
  {
    LOGN_DEBUG("Successfully got attribute value for "
      "\"syntax_tree_path_where_to_insert_translation\":"
      << m_std_strSyntaxTreePathForInsertionForTranslation )
//      m_conditionsandtranslation.m_stdstrGermanTranslation =
//          m_stdstrTranslation ;
  }
  else
    m_std_strSyntaxTreePathForInsertionForTranslation = "";
}

void SAX2TranslationRuleHandler::HandleTranslationRuleElementName(
  const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes)
{
  //If this path corresponds to the path of the _current_ syntax
  //tree path then it is checked whether conditions (if exist) are true and
  //if yes: applies the translation.
  if( XercesAttributesHelper::getValue(
      cr_xercesc_attributes ,
      m_stdstrTranslationRuleSyntaxTreePath ,
      "syntax_tree_path"
      )
    )
  {
    LOGN_DEBUG( /*FULL_FUNC_NAME << "--" <<*/ GetFilePathAndFileLocation()
      << " Successfully got syntax tree path:\""
      << m_stdstrTranslationRuleSyntaxTreePath << "\"")

    GetStringTransformationFunctionName(cr_xercesc_attributes);

    GetAttributeNameOrTranslationString(cr_xercesc_attributes);

    GetSyntaxTreePathWhereToInsertTranslation(cr_xercesc_attributes);

    GetChildNodeInsertionDirection(cr_xercesc_attributes);

    GetParentNodeInsertionDirection(cr_xercesc_attributes);

    if( ! XercesAttributesHelper::getValue(
        cr_xercesc_attributes,
        m_std_strParentNodeGrammarPartName,
        "parent_node_grammar_part_name")
      )
      m_std_strParentNodeGrammarPartName = "";

    if( ! XercesAttributesHelper::getValue(
        cr_xercesc_attributes,
        m_std_strGrammarPartName,
        "grammar_part_name"
        )
      )
      m_std_strGrammarPartName = "";
  }
}

void SAX2TranslationRuleHandler::Process(const std::string & filePath)
{
  m_std_strFilePath = filePath;
  unsigned char ch = ::ReadViaSAX2(
    filePath.c_str(), //const char * const cpc_chXMLfilePath ,
    this //XERCES_CPP_NAMESPACE::ContentHandler * const pc_contenthandler
    );
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
  LOGN_DEBUG(//"SAX2TranslationRuleHandler::startElement "
    "begin")
  char * pchXMLelementName = XERCES_CPP_NAMESPACE::XMLString::transcode(
    cpc_xmlchLocalName);
  if( pchXMLelementName)
  {
    LOGN_DEBUG("XML element:" << pchXMLelementName )
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
    if(
        //wcscmp(...) does not work with wide chars unlike 2 byte (e.g. 4 byte).
  //     ! wcscmp ( cpc_xmlchLocalName , L"condition" )
       //Compare 4 byte wide chars under Linux, 2 byte wide chars under Windows.
       ! Xerces::ansi_or_wchar_string_compare(
           cpc_xmlchLocalName ,
           ANSI_OR_WCHAR("concatenated_translation_rules")
           )
      )
      m_bConcatenatedTranslationRules = true;
    else if(
      //wcscmp(...) does not work with wide chars unlike 2 byte (e.g. 4 byte).
//     ! wcscmp ( cpc_xmlchLocalName , L"condition" )
     //Compare 4 byte wide chars under Linux, 2 byte wide chars under Windows.
      ! Xerces::ansi_or_wchar_string_compare(
         cpc_xmlchLocalName ,
         ANSI_OR_WCHAR("condition")
         )
    )
  {
    //HandleConditionXMLelement(cr_xercesc_attributes);
    HandleBeginOfConditionXMLelement( (XERCES_CPP_NAMESPACE::Attributes &) cr_xercesc_attributes);
  }
}

void SAX2TranslationRuleHandler::ShowMessageToUser(
  const std::string & c_r_std_strMessage)
{
  ShowMessageToUser( c_r_std_strMessage.c_str() );
}

void SAX2TranslationRuleHandler::ShowMessageToUser(
  const char * const cpc_chMessage)
{
  mr_i_userinterface.Message(
    //Needed for string concatenation: 2 char pointers can't be concatenated
    //via "+".
    std::string( cpc_chMessage) +
    "\nIt is referenced in in document\n"
    + Xerces::ToStdString( m_pc_locator->getSystemId() )
    + "\nin line:" + GetLineNumber()
    + ", column:" + GetColumnNumber()
    );
}

std::string SAX2TranslationRuleHandler::GetFilePathAndFileLocation()
{
  return "{" + Xerces::ToStdString( m_pc_locator->//getPublicId()
    getSystemId() ) + GetLineNumber() + ";" + GetColumnNumber() + "}";
}

std::string SAX2TranslationRuleHandler::GetColumnNumber()
{
  return convertToStdString<XMLFileLoc>(m_pc_locator->getColumnNumber());
}

std::string SAX2TranslationRuleHandler::GetLineNumber()
{
  return convertToStdString<XMLFileLoc>(m_pc_locator->getLineNumber());
}

void SAX2TranslationRuleHandler::GetLineNumber(std::string & std_strLine )
{
  std_strLine = convertToStdString<XMLFileLoc>(m_pc_locator->getLineNumber() );
}
