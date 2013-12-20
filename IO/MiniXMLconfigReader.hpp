/*
 * MiniXMLconfigReader.hpp
 *
 *  Created on: 08.12.2013
 *      Author: mr.sys
 */

#ifndef MINIXMLCONFIGREADER_HPP_
#define MINIXMLCONFIGREADER_HPP_

#include <string> //class std::string
#include <IO/ConfigurationReader.hpp> //base class VTrans3::ConfigurationReader

class TranslationControllerBase;

namespace MiniXML
{
  /** Class for reading configuration files via the "MiniXML" library/ framework.
   * (see http://www.msweet.org/documentation/project3/Mini-XML.pdf)
   *  MiniXML is easier to compile than Apache Xerces. So it ought to be used
   *  for Android NDK. */
  class MiniXMLconfigReader
    : public VTrans3::ConfigurationReader
  {
    TranslationControllerBase & m_translationController;
  public:
    MiniXMLconfigReader(TranslationControllerBase & r_translationController);
    virtual
    ~MiniXMLconfigReader();
    void ReadGrammarRuleFile(const std::string & cr_stdstrFilePath );
    bool ReadMainConfigFile(const std::string & cr_stdstrFilePath );
    void ReadVocAttributeDefinitionFile(
      const std::string & cr_stdstrFilePath);
    void ReadTranslationRuleFile(const std::string & cr_stdstrFilePath) ;
  };

} /* namespace MiniXML */

#endif /* MINIXMLCONFIGREADER_HPP_ */
