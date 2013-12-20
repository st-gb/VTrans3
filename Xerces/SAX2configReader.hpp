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
#include <Xerces/SAX2GrammarRuleHandler.hpp>
#include <Xerces/SAX2MainConfigHandler.hpp> //class SAX2MainConfigHandler
//Class SAX2TranslationRuleHandler
#include <Xerces/SAX2TranslationRuleHandler.hpp>
//Class SAX2TranslationRuleHandler
#include <Xerces/SAX2VocAttributeDefintionHandler.hpp>

namespace Apache
{
  namespace Xerces
  {

    class SAX2configReader
    {
      TranslationControllerBase & m_translationController;
      SAX2TranslationRuleHandler m_sax2translationrulehandler ;
//      I_UserInterface & m_userInterface;
    public:
      SAX2configReader(TranslationControllerBase & translationController);
      virtual
      ~SAX2configReader();
      void ReadGrammarRuleFile(const std::string & cr_stdstrFilePath );
      bool ReadMainConfigFile(const std::string & cr_stdstrFilePath );
      void ReadVocAttributeDefinitionFile(
        const std::string & cr_stdstrFilePath);
    };

  } /* namespace Xerces */
} /* namespace Apache */

#endif /* SAX2CONFIGREADER_HPP_ */
