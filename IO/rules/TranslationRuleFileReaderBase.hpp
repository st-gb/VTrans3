/*
 * TranslationRuleFileReaderBase.hpp
 *
 *  Created on: 23.12.2013
 *      Author: mr.sys
 */

#ifndef TRANSLATIONRULEFILEREADERBASE_HPP_
#define TRANSLATIONRULEFILEREADERBASE_HPP_

//class ConditionsAndTranslation
#include <Translate/ConditionsAndTranslation.hpp>
#include <string> //class std::string
//#include <IO/MiniXMLconfigReader.hpp> //base class VTrans3::MiniXML::MiniXMLconfigReader
#include <Controller/ConfigurationHandler_type.hpp>
#include <stdint.h> //uint32_t

#define TRANSLATION_RULE_XML_ELEMENT_ANSI "translation_rule"
#define ATTRIBUTE_NAME_ANSI_STRING "attribute_name"
#define TRANSLATION_ANSI_STRING "translation"

/** Forward declarations. */
class ParseByRise ;
class I_UserInterface ;
class TranslateParseByRiseTree ;
//namespace VTrans3 {
//  ConfigurationHandler_type;
//}

namespace VTrans3
{
//  template <typename XMLelementType>
  class TranslationRuleFileReaderBase
//    : public ConfigurationReader
  {
//    typedef typename XMLelementType XMLelementType;
  public:
    bool m_bConcatenatedTranslationRules;
    ConditionsAndTranslation m_conditionsandtranslation;
    I_UserInterface & mr_i_userinterface;
    ParseByRise & mr_parsebyrise;
    TranslateParseByRiseTree & mr_translateparsebyrise;
    std::string m_strElementName;
    std::string m_stdstrConditionSyntaxTreePath;
    std::string m_stdstrConditionByteAttrValue;
    std::string m_stdstrConditionStringAttrValue;
    std::string m_stdstrConditionAttributeName;
    std::string m_std_strGrammarPartName;
    std::string m_std_strParentNodeGrammarPartName;
    std::string m_stdstrTranslationRuleAttributeName;
    std::string m_stdstrTranslation;
    std::string m_stdstrTranslationRuleSyntaxTreePath;
    std::string m_std_strSyntaxTreePathForInsertionForTranslation;
    std::string m_std_strFilePath; /** Should be set in "Process(...)" */
    unsigned m_uiTranslationInsertion;
    unsigned m_uiParentNodeInsertion;
    uint32_t m_ui32ConcatenationID;
    VTrans3::ConfigurationReader<attributeType> & m_r_configurationReader;

  public:
    TranslationRuleFileReaderBase(
      TranslateParseByRiseTree & ,
      ParseByRise & ,
      I_UserInterface & r_userinterface
//      , TranslationControllerBase & r_translationController
      , //VTrans3::ConfigurationReader
        ConfigurationHandler_type & configurationReader
      , TranslationControllerBase * p_translationControllerBase
      );
    virtual
    ~TranslationRuleFileReaderBase();

//  protected:
    void EndTranslationRuleXMLelement();
    /*template <typename XMLelement_Type>*/ /*inline*/
      void HandleBeginOfConditionXMLelement(
      /*XMLelement_Type*/ attributeType & xmlElement);
    inline bool HasByteAttributeValue(
      attributeType & xmlElement, std::string & std_strAttributeValue);
    virtual void Process(const std::string & filePath) = 0;
    void SetConditionAttributesAndAddCondition(const WORD wAttributeValue);
    virtual void ShowMessageToUser(const std::string &) = 0;
  };
} /* namespace VTrans3 */

#endif /* TRANSLATIONRULEFILEREADERBASE_HPP_ */
