/* 
 * File:   MainConfigFileReaderBase.cpp
 * Author: mr.sys
 * 
 * Created on 30. Mai 2014, 01:19
 */

#include "MainConfigFileReaderBase.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_DEBUG(...)
#include <Controller/character_string/ConvertStdStringToTypename.hpp>
#include <Controller/TranslationControllerBase.hpp> //class TranslationControllerBase
#include <C_standard_library/string_compare_return_codes.h>

MainConfigFileReaderBase::MainConfigFileReaderBase (
  ConfigurationHandler_type & configurationReader)
  : m_r_configurationReader(configurationReader)
{
}

//MainConfigFileReaderBase::MainConfigFileReaderBase (const MainConfigFileReaderBase& orig) { }

//  inline bool TranslationRuleFileReaderBase::HasByteAttributeValue(
//    attributeType & xmlElement, std::string & std_strAttributeValue)
//  {
//    const bool hasByteAttributeValue = m_r_configurationReader.
//      GetAttributeValue(
//        xmlElement,
//        std_strAttributeValue,
//        "byte_attribute_value" // const XMLCh *const qNames
//        ) ;
//    return hasByteAttributeValue;
//  }

#ifndef TEST_MINI_XML
  void MainConfigFileReaderBase::HandleGrammartPartColourXMLelement(
      attributeType & xmlElement)
  {
    std::string std_strGrammarPartName;
    std::string std_strColour;
    m_r_configurationReader.GetAttributeValue(xmlElement, std_strGrammarPartName,
      "name");
      //strGrammarPartName = std_strAttributeValue.c_str();
    if( std_strGrammarPartName != "" )
    {
      m_r_configurationReader.GetAttributeValue(xmlElement, std_strColour,
        "colour");        
      if( std_strColour != "" )
      {
        const char * const strGrammarPartName =  std_strGrammarPartName.c_str ();
        unsigned dwValue;
        ConvertStdStringToTypename(dwValue, std_strColour );
        try
        {
          m_r_configurationReader.m_ui32GrammarPartName2colourContainer.
//          insert_inline(
//          (BYTE *) strGrammarPartName,
//          ::strlen(strGrammarPartName ),
//          dwValue
//          );
          insert( std::make_pair(
            std_strGrammarPartName,
            dwValue)
          );
      }catch( const NS_NodeTrie::RootNodeNotInitalizedException & e)
      {
        LOGN_ERROR("NS_NodeTrie::RootNodeNotInitalizedException")
      }
      }
    }
  }

  bool MainConfigFileReaderBase::HandleReadGrammarRuleFileXMLelement( 
    attributeType & xmlElement )
  {
    bool success = false;
    std::string std_strGrammarRuleFilePath;
    m_r_configurationReader.GetAttributeValue(xmlElement, 
      std_strGrammarRuleFilePath, "path");
    if( std_strGrammarRuleFilePath != "" )
    {
      LOGN_DEBUG("grammar rule file path:" << std_strGrammarRuleFilePath)
//            strVocabularyFilePath
      success = m_r_configurationReader.ReadGrammarRuleFile(
        std_strGrammarRuleFilePath);
    }
    return success;
  }

#if USE_TRANSLATION_RULES
  void MainConfigFileReaderBase::HandleReadTranslationRuleFileXMLelement(
    attributeType & xmlElement )
  {
    std::string std_strTranslationRuleFilePath;
    m_r_configurationReader.GetAttributeValue(xmlElement, 
      std_strTranslationRuleFilePath, "path");
    if( std_strTranslationRuleFilePath != "" )
    {
      LOGN_DEBUG("translation rule file path:" << std_strTranslationRuleFilePath)
//            strVocabularyFilePath
//      m_r_configurationReader.m_translationController.ReadTranslationRuleFile(
//        std_strTranslationRuleFilePath);
      m_r_configurationReader.ReadTranslationRuleFile(
        std_strTranslationRuleFilePath);
    }
  }

  void MainConfigFileReaderBase::HandleReadVocAttrDefFileXMLele(
      attributeType & xmlElement )
  {
    std::string std_strVocabularyAttributeDefinitionFilePath;
    m_r_configurationReader.GetAttributeValue(xmlElement, 
      std_strVocabularyAttributeDefinitionFilePath, "path");
    if( std_strVocabularyAttributeDefinitionFilePath != "" )
    {
      LOGN_DEBUG("vocabulary_attribute_definition file path:"
        << std_strVocabularyAttributeDefinitionFilePath)
//            strVocabularyFilePath
      m_r_configurationReader.ReadVocAttributeDefinitionFile(
        std_strVocabularyAttributeDefinitionFilePath);
    }
  }
#endif 
#endif //#ifndef TEST_MINI_XML

fastestUnsignedDataType GetFontSizeInPoint(
  const std::string & std_strFontSizeXMLattributeValue)
{
  const fastestSignedDataType stringLength = 
    std_strFontSizeXMLattributeValue.length();
  std::string unit = "pt";
  unsigned fontSizeInPoint = 0;
  if( stringLength > 2 && 
     std_strFontSizeXMLattributeValue.substr(stringLength - unit.length ()) 
                                             == unit)
  {
    const std::string std_strFontSizeInPoint = std_strFontSizeXMLattributeValue.
      substr(0, stringLength - unit.length () );
    ConvertStdStringToTypename(fontSizeInPoint, std_strFontSizeInPoint );
  }
  return fontSizeInPoint;
}

//TODO use enum errorCodes as return value?
bool MainConfigFileReaderBase::openingXMLelement(
  const char * const xmlElementName, attributeType & xmlElement)
{
  bool success = false;
    if( xmlElementName == NULL )
      LOGN_DEBUG("element name:NULL")
    else
    {
      LOGN_DEBUG("element name:" << xmlElementName)
//      std::cout << "element nameXX:" << xmlElementName << std::endl;
//#ifndef __linux__
//      if (event == MXML_SAX_ELEMENT_CLOSE)
//      {
//        if( ::strcmp(xmlElementName, "VTrans_main_cfg") == 0 )
//        {
//            int i = 0;
//        }
//      }
		//TODO change to: BUILD_WITH_USER_INTERFACE
#ifdef COMPILE_AS_EXECUTABLE
      if( ::strcmp(xmlElementName, "grammar_part") == 
         C_standard_library::strcmp::stringsAreEqual )
      {
        HandleGrammartPartColourXMLelement(xmlElement);
      }
      else if( ::strcmp(xmlElementName, "main_window") == 
              C_standard_library::strcmp::stringsAreEqual )
      {
        
      }
      else if( ::strcmp(xmlElementName, "parse_tree_panel") == 
              C_standard_library::strcmp::stringsAreEqual )
      {
        std::string std_strFontSizeXMLattributeValue;
        
        std::string & strConcatenationIDcolor =
          m_r_configurationReader.m_GUIattributes.
          m_std_strConcatenationIDcolor;
        m_r_configurationReader.GetAttributeValue(xmlElement, 
          strConcatenationIDcolor, "concatenation_id_color");
        unsigned dwValue;
        if( ConvertStdStringToTypename(dwValue, strConcatenationIDcolor ) )
          m_r_configurationReader.m_GUIattributes.
            SetConcatenationIDcolour(dwValue);

        m_r_configurationReader.GetAttributeValue(xmlElement, 
          std_strFontSizeXMLattributeValue, "font_size");
        fastestUnsignedDataType fontSizeInPoint = GetFontSizeInPoint(
          std_strFontSizeXMLattributeValue);
        if( fontSizeInPoint)
          m_r_configurationReader.m_GUIattributes.
            SetPointSizeOfParseTreePanel(fontSizeInPoint);
        
        m_r_configurationReader.GetAttributeValue(xmlElement, 
          std_strFontSizeXMLattributeValue, "minimum_font_size");
        fontSizeInPoint = GetFontSizeInPoint(std_strFontSizeXMLattributeValue);
        if( fontSizeInPoint)
          m_r_configurationReader.m_GUIattributes.
            SetMinFontSizeInPointOfParseTreePanel(fontSizeInPoint);
      }
        //TODO implement in base class of "MainConfigFileReader" or
        // MiniXML-specific/ enable this feature
//        else if( m_strElementName == "drawing" )
//          HandleGUI_XMLelement(cr_xercesc_attributes);
      else
#endif
      if( ::strcmp(xmlElementName, "grammar_rule_file") == 
         C_standard_library::strcmp::stringsAreEqual )
      {
        success = HandleReadGrammarRuleFileXMLelement( xmlElement ) ;
      }
//         else if( m_strElementName == "transformation_rule_file" )
//         {
//           HandleReadTransformationRuleFileXMLelement( cr_xercesc_attributes ) ;
//         }
#if USE_TRANSLATION_RULES
      else if( ::strcmp(xmlElementName, "translation_rule_file") == 
              C_standard_library::strcmp::stringsAreEqual )
      {
        HandleReadTranslationRuleFileXMLelement( xmlElement) ;
        //int i = 0;
      }
      else if( ::strcmp(xmlElementName, "vocabulary_attribute_definition_file")
               == C_standard_library::strcmp::stringsAreEqual )
      {
        HandleReadVocAttrDefFileXMLele(xmlElement ) ;
      }
#endif
     else if( ::strcmp(xmlElementName, "vocabulary_file") == 
             C_standard_library::strcmp::stringsAreEqual )
     {
       std::string std_strVocabularyFilePath;
       m_r_configurationReader.GetAttributeValue(xmlElement, 
         std_strVocabularyFilePath, "path");

       LOGN_DEBUG("Dict file path:" << std_strVocabularyFilePath)
       if( std_strVocabularyFilePath != "" )
       {
         //LOGN_DEBUG("Dict file path:" << strVocabularyFilePath)
//            strVocabularyFilePath
         m_r_configurationReader.
           /*m_configurationHandler.*/m_stdstrVocabularyFilePath =
           std_strVocabularyFilePath;
         success = true;
       }
     }
   }
  return //success;
    true;
}

MainConfigFileReaderBase::~MainConfigFileReaderBase () { }
