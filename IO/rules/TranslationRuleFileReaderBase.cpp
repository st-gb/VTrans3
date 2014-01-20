/*
 * TranslationRuleFileReaderBase.cpp
 *
 *  Created on: 23.12.2013
 *      Author: mr.sys
 */

#include <Controller/TranslationControllerBase.hpp> //class TranslationControllerBase
#include <IO/UnknownGrammarPartNameException.hpp>
#include <IO/rules/TranslationRuleFileReaderBase.hpp>
#include <IO/ConfigurationReader.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_DEBUG(...)
#include <Parse/ParseByRise.hpp> //class ParseByRise
#include <Translate/TranslationRule.hpp>//class TranslationRule
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>
#include <UserInterface/I_UserInterface.hpp>//class I_UserInterface

#ifdef COMPILE_AS_EXECUTABLE
extern TranslationControllerBase * g_p_translationcontrollerbase;
#endif //#ifdef COMPILE_AS_EXECUTABLE

namespace VTrans3
{
  /** From http://www.codeproject.com/Articles/48575/How-to-define-a-template-class-in-a-h-file-and-imp*/
  //template <typename XMLelementType>
  TranslationRuleFileReaderBase
//  <XMLelementType>
  ::TranslationRuleFileReaderBase(
    TranslateParseByRiseTree & r_translateparsebyrise ,
    ParseByRise & r_parsebyrise
    , I_UserInterface & r_userinterface
//    , TranslationControllerBase & r_translationController
    , //VTrans3::ConfigurationReader
    ConfigurationHandler_type & configurationReader,
    TranslationControllerBase * p_translationControllerBase
    )
    :
    m_bConcatenatedTranslationRules(false)
    , mr_i_userinterface( r_userinterface)
    , mr_parsebyrise ( r_parsebyrise )
    , mr_translateparsebyrise (r_translateparsebyrise)
    , m_ui32ConcatenationID(0)
    , m_r_configurationReader(configurationReader)
  {

  }

  /** From http://www.codeproject.com/Articles/48575/How-to-define-a-template-class-in-a-h-file-and-imp*/
//  template <typename XMLelementType>
  TranslationRuleFileReaderBase
//  <XMLelementType>
  ::~TranslationRuleFileReaderBase()
  {
    // TODO Auto-generated destructor stub
  }

  void TranslationRuleFileReaderBase::EndTranslationRuleXMLelement()
  {
    LOGN_DEBUG( //TRANSLATION_RULE_XML_ELEMENT_ANSI
      //"SAX2TranslationRuleHandler::endElement(...)--"
      "\"translation_rule\" element" )
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
            ShowMessageToUser( "unknown grammar part name \"" +
                std_strUnknownGrammarPartID + "\" in STP \"" +
                m_std_strSyntaxTreePathForInsertionForTranslation +
                "\"\n"
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

        LOGN_DEBUG(//"SAX2TranslationRuleHandler::endElement(...)--"
          "adding translation "
          "rule\"" << m_stdstrTranslationRuleSyntaxTreePath << "\"" )
        mr_translateparsebyrise.AddTranslationRule(
          p_translationrule
          , m_conditionsandtranslation ) ;
#ifdef COMPILE_AS_EXECUTABLE
//        const std::string & std_strSystemId = Xerces::ToStdString(
//          m_pc_locator->getSystemId() );
        g_p_translationcontrollerbase->m_std_map_p_translationrule2filepath.insert(
          std::make_pair(p_translationrule, m_std_strFilePath) );
#endif //#ifdef COMPILE_AS_EXECUTABLE
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
      catch(VTrans::UnknownGrammarPartNameException & exc)
      {
//        mr_i_userinterface.Message(
//          );
        ShowMessageToUser(
          "Error adding translation rule for Syntax Tree Path \"" +
          m_stdstrTranslationRuleSyntaxTreePath + "\" : "
          "unknown grammar part name \"" +
          exc.GetGrammarPartPath() + " \""
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
        LOGN_ERROR(//"SAX2TranslationRuleHandler::endElement(...)--"
          "creating translation "
          "rule for \"" << m_stdstrTranslationRuleSyntaxTreePath
          << "\" failed." /*, LogLevel::error*/ )
        ShowMessageToUser(
          "SAX2TranslationRuleHandler::endElement(...)--creating translation "
          "rule for \"" + m_stdstrTranslationRuleSyntaxTreePath
          + "\" failed.");
      }
    }
    m_conditionsandtranslation.clear();
  }

//  template <typename XMLelementType>
    void TranslationRuleFileReaderBase::HandleBeginOfConditionXMLelement(
    /*XMLelementType*/ attributeType & xmlElement)
  {
    if( m_stdstrTranslationRuleAttributeName != "" || m_stdstrTranslation != ""
        )
    {
    //      BYTE byAttributeValue ;
      //Use an _integer_ data type for the string to be converted to a number.
      WORD wAttributeValue ;
      if( m_r_configurationReader.GetAttributeValue(
          xmlElement,
          m_stdstrConditionSyntaxTreePath,
          "syntax_tree_path"
          )
        )
      {
        LOGN_DEBUG("Got attribute value for \"condition\" element's syntax_tree_path:"
          "\"" << m_stdstrConditionSyntaxTreePath << "\"" )

        if( m_stdstrConditionSyntaxTreePath != ""
            && m_r_configurationReader.GetAttributeValue(
              xmlElement ,
              m_stdstrConditionAttributeName,
            //Use attribute name "German_plural" from the
            // VocabularyAndTranslation data.
            //L"attribute_name"
            "attribute_name"
            )
          )
        {
          LOGN_DEBUG("Got attribute value for \"condition\" element's "
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
            ShowMessageToUser( "The translation attribute definition "
              "for the name \"" + m_stdstrConditionAttributeName
              + "\" is not available."
              ) ;
          }

          if( m_stdstrConditionAttributeName != "" )
          {
            bool outputErrorMessage = false;
            //            XercesAttributesHelper::getValue(
            //          attrs ,
            //          m_stdstrConditionByteAttrValue,
            //          //Use attribute name "German_plural" from the
            //          // VocabularyAndTranslation data.
            //          L"byte_attribute_value"
            //          )
            bool convertingSucceeded =
              VTrans3::ConfigurationReader<attributeType>::
              ConvertAttributeValue//<//BYTE
              //WORD>
//                  <WORD>
              (
              xmlElement ,
              m_r_configurationReader,
              //byAttributeValue ,
              wAttributeValue ,
              //L"byte_attribute_value"
              "byte_attribute_value"
              );
            if( convertingSucceeded )
              SetConditionAttributesAndAddCondition(wAttributeValue);
            else
            {
              convertingSucceeded =
                m_r_configurationReader.GetAttributeValue(
                  xmlElement ,
                  m_stdstrConditionStringAttrValue ,
                  //Use attribute name "German_plural" from the
                  // VocabularyAndTranslation data.
    //              L"string_attribute_value"
                  "string_attribute_value"
                  );
              if( convertingSucceeded )
                SetConditionAttributesAndAddCondition(wAttributeValue);
              else
                outputErrorMessage = true;
            }
            if(outputErrorMessage)
            {
              LOGN_ERROR("Neither a \"byte_attribute_value\" nor a "
                "\"string_attribute_value\" attribute"
                "-> NOT adding condition to the rule." /*, LogLevel::error*/ )
              ShowMessageToUser( "Neither a \"byte_attribute_value\" nor a "
                  "\"string_attribute_value\" attribute\n"
                  "-> NOT adding condition to the rule."
                ) ;
            }
          }
        }
      }
    }
    else
      LOGN_ERROR("condition element--neither \"attribute_name\" attribute value nor "
        "\"translation\" attribute value" /*, LogLevel::error*/ )
  }

//  void TranslationRuleFileReaderBase::Process(const std::string & filePath)
//  {
//
//  }

  void TranslationRuleFileReaderBase::SetConditionAttributesAndAddCondition(
    const WORD wAttributeValue)
  {
    LOGN_DEBUG("\"byte_attribute_value\" or "
      "\"string_attribute_value\" attribute for STP " <<
      m_stdstrConditionSyntaxTreePath)
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
      ShowMessageToUser( "unknown grammar part name \n\"" +
        std_strUnknownGrammarPartID + "\"\n for STP \n\"" +
        m_stdstrConditionSyntaxTreePath + "\"\n"
        ) ;
    }
    cond.m_stdstrAttributeName = m_stdstrConditionAttributeName ;
  //        if( m_stdstrConditionByteAttrValue != "" )
    cond.m_byAttributeValue = //byAttributeValue ;
        wAttributeValue ;
    LOGN_DEBUG(//"SAX2TranslationRuleHandler::startElement(...)--"
      "adding condition" /*, LogLevel::debug*/)
    m_conditionsandtranslation.AddCondition( cond ) ;
  }
} /* namespace VTrans3 */
