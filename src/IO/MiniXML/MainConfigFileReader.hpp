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

/** Forward declarations: */
//class TranslationControllerBase;
class I_UserInterface;

/** see http://www.msweet.org/documentation/project3/Mini-XML.pdf */
namespace VTrans3 {
namespace MiniXML
{
  class MainConfigFileReader
    : public MainConfigFileReaderBase
  {
  public:
//    static TranslationControllerBase * s_p_translationController;
    static I_UserInterface * s_p_UserInterface;
    MainConfigFileReader(//TranslationControllerBase & r_translationController,
      I_UserInterface * p_UserInterface,
      ConfigurationHandler_type & configurationReader);
    virtual
    ~MainConfigFileReader();
    bool ProcessXML(const std::string & cr_stdstrFilePath);
    ///True if all files referred in the main config file could be opened
    /// successfully.
    bool m_successfullyProcessedAllReferredFiles;
  };
} /* namespace MiniXML */
}

#endif /* READMAINCONFIGFILE_HPP_ */
