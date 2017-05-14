/*
 * TranslationRuleFileReader.cpp
 *
 *  Created on: 23.12.2013
 *      Author: mr.sys
 */

#include <IO/MiniXML/TranslationRuleFileReader.hpp>
#include <IO/MiniXML/MiniXMLconfigReader.hpp> //MiniXMLconfigReader::ReadFile(...)
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_DEBUG(...)
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>
#include <Translate/TranslationRule.hpp>//class TranslationRule
#include <UserInterface/I_UserInterface.hpp>//class I_UserInterface
#include <compiler/GCC/enable_disable_write_strings_warning.h> //GCC_DIAG

/** for void VTrans3::TranslationRuleFileReaderBase::HandleBeginOfConditionXMLelement */
//#include <IO/rules/TranslationRuleFileReaderBase.cpp>

extern TranslationControllerBase * g_p_translationcontrollerbase;

namespace VTrans3
{
  namespace MiniXML
  {
    TranslationRuleFileReader * g_p_translationRuleFileReader = NULL;

    TranslationRuleFileReader::TranslationRuleFileReader(
      TranslateParseByRiseTree & r_translateparsebyrise ,
      ParseByRise & r_parsebyrise
      , I_UserInterface & r_userinterface
      , ConfigurationHandler_type & configurationReader
      )
      : //configurationReader(* g_p_translationcontrollerbase),
        VTrans3::TranslationRuleFileReaderBase(
        r_translateparsebyrise
        , r_parsebyrise
        , r_userinterface
        , configurationReader,
        g_p_translationcontrollerbase
        )
//      , VTrans3::MiniXML::MiniXMLconfigReader( * g_p_translationcontrollerbase)
    {
      g_p_translationRuleFileReader = this;
    }

    TranslationRuleFileReader::~TranslationRuleFileReader()
    {
      // TODO Auto-generated destructor stub
    }

    void TranslationRuleFileReader::GetAttributeNameOrTranslationString(
      mxml_node_t * node)
    {
      const char * const strTranslationRuleAttributeName =
        /* This is the name for the attribute value to choose for
        /* /translation if the syntax tree path and the conditions match:
        * e.g. attribute_name is "German_plural", current syntax tree
        * path is "definite_plural.plural", translation rule's syntax
        * tree path is "plural" then:
        * use attribute name "German_plural" from the
        * VocabularyAndTranslation data. */
        ::mxmlElementGetAttr(node, ATTRIBUTE_NAME_ANSI_STRING);

      /** Using directly a word for translation rather than an attribute
      * value of a VocabularyAndTranslation object is an alternative. */
      const char * const strTranslation = ::mxmlElementGetAttr(node,
        TRANSLATION_ANSI_STRING);

      if( strTranslationRuleAttributeName )
      {
        m_stdstrTranslationRuleAttributeName = strTranslationRuleAttributeName;
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
          //TODO
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
      else if ( strTranslation )
      {
        m_stdstrTranslation = strTranslation;
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

    void TranslationRuleFileReader::GetChildNodeInsertionDirection(
        mxml_node_t * node)
    {
      std::string std_strInsertionForTranslationDirection;
      const char * const strInsertionForTranslationDirection =
        ::mxmlElementGetAttr(node, "child_node_insertion_direction");
      if( strInsertionForTranslationDirection )
      {
        if( strcmp(strInsertionForTranslationDirection, "left") == 0)
          m_uiTranslationInsertion = TranslationRule::left;
        else
          if( strcmp(strInsertionForTranslationDirection, "right") == 0 )
            m_uiTranslationInsertion = TranslationRule::right;
      }
    }

    void TranslationRuleFileReader::GetParentNodeInsertionDirection(
        mxml_node_t * node)
    {
      const char * const strInsertionForParentNode =
        ::mxmlElementGetAttr(node, "parent_node_insertion_direction");
      if( strInsertionForParentNode )
      {
        if( strcmp(strInsertionForParentNode, "left") == 0 )
          m_uiParentNodeInsertion = TranslationRule::left;
        else
          if( strcmp(strInsertionForParentNode, "right") == 0 )
            m_uiParentNodeInsertion = TranslationRule::right;
      }
    }

    void TranslationRuleFileReader::GetStringTransformationFunctionName(
      mxml_node_t * node)
    {
      const char * const strStringTransformationFunctionName =
       ::mxmlElementGetAttr(node, "string_transformation_function_name" );
      if( strStringTransformationFunctionName )
      {
        std::map<std::string, pfnTransformString>::const_iterator c_iter
          = ConditionsAndTranslation::s_std_mapFunctionName2Function.find(
          strStringTransformationFunctionName);
        if( c_iter != ConditionsAndTranslation::
          s_std_mapFunctionName2Function.end() )
        {
          LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "assigning function \"" <<
            strStringTransformationFunctionName << "\" for current "
            "conditionsandtranslation object" )
          m_conditionsandtranslation.m_pfn_TransformString = c_iter->second;
        }
      }
    }

    void TranslationRuleFileReader::GetSyntaxTreePathWhereToInsertTranslation(
        mxml_node_t * node)
    {
      const char * const strSyntaxTreePathForInsertionForTranslation =
        /** Using directly a word for translation rather than an attribute
        * value of a VocabularyAndTranslation object is an alternative. */
         ::mxmlElementGetAttr(node, "syntax_tree_path_where_to_insert_translation");
      if ( strSyntaxTreePathForInsertionForTranslation )
      {
        m_std_strSyntaxTreePathForInsertionForTranslation =
          strSyntaxTreePathForInsertionForTranslation;
        LOGN_DEBUG("Successfully got attribute value for "
          "\"syntax_tree_path_where_to_insert_translation\":"
          << m_std_strSyntaxTreePathForInsertionForTranslation )
    //      m_conditionsandtranslation.m_stdstrGermanTranslation =
    //          m_stdstrTranslation ;
      }
      else
        m_std_strSyntaxTreePathForInsertionForTranslation = "";
    }

    void TranslationRuleFileReader::HandleTranslationRuleElementName(
      mxml_node_t * node)
    {
      /** If this path corresponds to the path of the _current_ syntax
      * tree path then it is checked whether conditions (if exist) are true and
      * if yes: applies the translation. */
      const char * const translationRulesyntaxTreePath =
        ::mxmlElementGetAttr(node, "syntax_tree_path");
      if( translationRulesyntaxTreePath)
      {
        m_stdstrTranslationRuleSyntaxTreePath = translationRulesyntaxTreePath;
        LOGN_DEBUG( /*FULL_FUNC_NAME << "--" <<*/ /*GetFilePathAndFileLocation()
          << */ " Successfully got syntax tree path:\""
          << m_stdstrTranslationRuleSyntaxTreePath << "\"")

        GetStringTransformationFunctionName(node);

        GetAttributeNameOrTranslationString(node);

        GetSyntaxTreePathWhereToInsertTranslation(node);

        GetChildNodeInsertionDirection(node);

        GetParentNodeInsertionDirection(node);

        const char * const strParentNodeGrammarPartName =
          mxmlElementGetAttr(node, "parent_node_grammar_part_name");

        if( ! strParentNodeGrammarPartName )
          m_std_strParentNodeGrammarPartName = "";

        const char * const strGrammarPartName = ::mxmlElementGetAttr(
          node, "grammar_part_name");
        if( strGrammarPartName )
          m_std_strGrammarPartName = strGrammarPartName;
        else
          m_std_strGrammarPartName = "";
      }
    }

    void TranslationRuleFileReader::Process(const std::string & filePath)
    {
      m_std_strFilePath = filePath;
      MiniXMLconfigReader::ReadFile(filePath.c_str(),
        VTrans3::MiniXML::ReadTranslationRuleFile::SAX_callback);
    }

    void TranslationRuleFileReader::ShowMessageToUser(
      const /*char * const*/ std::string & cpc_chMessage)
    {
      mr_i_userinterface.Message(
        //Needed for string concatenation: 2 char pointers can't be concatenated
        //via "+".
        std::string( cpc_chMessage) //+
//        "\nIt is referenced in in document\n"
//        + Xerces::ToStdString( m_pc_locator->getSystemId() )
//        + "\nin line:" + GetLineNumber()
//        + ", column:" + GetColumnNumber()
        );
    }

    namespace ReadTranslationRuleFile
    {
      /** see http://www.msweet.org/documentation/project3/Mini-XML.html#4_7 */
      void SAX_callback(
        mxml_node_t * node,
        mxml_sax_event_t event,
        void * data
        )
      {
        const char * xmlElementName = mxmlGetElement(node);
  //        if (event == MXML_SAX_ELEMENT_CLOSE)
  //        {
  //          if( ::strcmp(xmlElementName, "VTrans_main_cfg") == 0 )
  //          {
  //              int i = 0;
  //          }
  //        }
        GCC_DIAG_OFF(switch)
        switch(event )
        {
          case MXML_SAX_ELEMENT_CLOSE :
            if( ::strcmp(xmlElementName, "translation_rule") == 0 )
              g_p_translationRuleFileReader->EndTranslationRuleXMLelement(node);
            else
              if( ::strcmp(xmlElementName, "concatenated_translation_rules")
                == 0 )
            {
              g_p_translationRuleFileReader->m_bConcatenatedTranslationRules = false;
              ++ g_p_translationRuleFileReader->m_ui32ConcatenationID;
            }
            break;
          case MXML_SAX_ELEMENT_OPEN :
            if( ::strcmp(xmlElementName, "translation_rule") == 0 )
            {
              g_p_translationRuleFileReader->HandleTranslationRuleElementName(node);
            }
            else
            {

            }
              if( ::strcmp(xmlElementName, "concatenated_translation_rules") == 0 )
                g_p_translationRuleFileReader->m_bConcatenatedTranslationRules = true;
              else if( ::strcmp(xmlElementName, "condition") == 0 )
              {
                g_p_translationRuleFileReader->HandleBeginOfConditionXMLelement(node);
              }
            break;
        }
        GCC_DIAG_ON(switch)
      }
    }
  } /* namespace MiniXML */
} /* namespace VTrans3 */
