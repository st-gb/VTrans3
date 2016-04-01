/*
 * SAX2configReader.hpp
 *
 *  Created on: 08.12.2013
 *      Author: mr.sys
 */

#ifndef SAX2CONFIGREADER_HPP_
#define SAX2CONFIGREADER_HPP_

class I_UserInterface;
//Class SAX2GrammarRuleHandler
//#include <Xerces/SAX2GrammarRuleHandler.hpp>
//#include <Xerces/SAX2MainConfigHandler.hpp> //class SAX2MainConfigHandler
//Class SAX2TranslationRuleHandler
//#include <Xerces/SAX2TranslationRuleHandler.hpp>
//Class SAX2TranslationRuleHandler
//#include <Xerces/SAX2VocAttributeDefintionHandler.hpp>

#include <IO/ConfigurationReader.hpp> //base class VTrans3::ConfigurationReader
#include <xercesc/sax2/Attributes.hpp>

namespace Apache
{
  namespace Xerces
  {

    class SAX2configReader
      : public VTrans3::ConfigurationReader<XERCES_CPP_NAMESPACE::Attributes>
    {
      TranslationControllerBase & m_translationController;
      //SAX2TranslationRuleHandler m_sax2translationrulehandler ;
      //SAX2GrammarRuleHandler m_sax2grammarrulehandler;
      //SAX2VocAttributeDefinitionHandler m_sax2vocattributedefinitionhandler;
//      I_UserInterface & m_userInterface;
    public:
      SAX2configReader(TranslationControllerBase & translationController);
      virtual
      ~SAX2configReader();
      
      bool GetAttributeValue(
      /*XMLelementType & xmlElement,*/
        XERCES_CPP_NAMESPACE::Attributes & attribute,
        std::string &, const char * const);
      void ReadGrammarRuleFile(const std::string & cr_stdstrFilePath );
      bool ReadMainConfigFile(const std::string & cr_stdstrFilePath );
      void ReadVocAttributeDefinitionFile(
        const std::string & cr_stdstrFilePath);
      void ReadTranslationRuleFile(const char * const cr_stdstrFilePath);
    };

  } /* namespace Xerces */
} /* namespace Apache */

#endif /* SAX2CONFIGREADER_HPP_ */
