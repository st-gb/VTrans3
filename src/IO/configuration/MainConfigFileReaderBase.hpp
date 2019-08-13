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
  
  enum errorCodes { noErrsOccuredReadingCfgFiles, error };
  
  void HandleGrammartPartColourXMLelement(attributeType & xmlElement);
  ///\return true:success
  bool HandleReadGrammarRuleFileXMLelement(attributeType & xmlElement );
#if USE_TRANSLATION_RULES
  void HandleReadTranslationRuleFileXMLelement(attributeType & xmlElement );
  void HandleReadVocAttrDefFileXMLele(
    attributeType & xmlElement );
#endif
  ///\return true:success
  bool openingXMLelement(const char * const xmlEleName, attributeType & xmlEle);
private:

};

#endif	/* MAINCONFIGFILEREADERBASE_HPP */

