/* 
 * File:   MainConfigFileReaderBase.hpp
 * Author: mr.sys
 *
 * Created on 30. Mai 2014, 01:19
 */

#ifndef MAINCONFIGFILEREADERBASE_HPP
#define	MAINCONFIGFILEREADERBASE_HPP

/** typedef attributeType, typedef ConfigurationHandler_type */
#include <Controller/ConfigurationHandler_type.hpp> 

class MainConfigFileReaderBase {
  VTrans3::ConfigurationReader<attributeType> & m_r_configurationReader;
public:
  MainConfigFileReaderBase(ConfigurationHandler_type & configurationReader);
  //MainConfigFileReaderBase(const MainConfigFileReaderBase& orig);
  virtual ~MainConfigFileReaderBase();
  
  void HandleGrammartPartColourXMLelement(attributeType & xmlElement);
  void HandleReadGrammarRuleFileXMLelement(attributeType & xmlElement );
#if USE_TRANSLATION_RULES
  void HandleReadTranslationRuleFileXMLelement(attributeType & xmlElement );
  void HandleReadVocabularyAttributeDefinitionFileXMLelement(
    attributeType & xmlElement );
#endif
  void openingXMLelement(const char * const xmlElementName, attributeType & xmlElement);
private:

};

#endif	/* MAINCONFIGFILEREADERBASE_HPP */

