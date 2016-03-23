/*
 * MainConfigFileReader.hpp
 *
 *  Created on: 09.12.2013
 *      Author: mr.sys
 */

#ifndef READMAINCONFIGFILE_HPP_
#define READMAINCONFIGFILE_HPP_

#include <string> //class std::string
#include <IO/configuration/MainConfigFileReaderBase.hpp>

class TranslationControllerBase;

/** see http://www.msweet.org/documentation/project3/Mini-XML.pdf */
namespace VTrans3 {
namespace MiniXML
{
  class MainConfigFileReader
    : public MainConfigFileReaderBase
  {
  public:
    static TranslationControllerBase * s_p_translationController;
    MainConfigFileReader(TranslationControllerBase & r_translationController,
      ConfigurationHandler_type & configurationReader);
    virtual
    ~MainConfigFileReader();
    bool ProcessXML(const std::string & cr_stdstrFilePath);
  };
} /* namespace MiniXML */
}

#endif /* READMAINCONFIGFILE_HPP_ */
