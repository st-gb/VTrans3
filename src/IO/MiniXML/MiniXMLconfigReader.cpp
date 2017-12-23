/*
 * MiniXMLconfigReader.cpp
 *
 *  Created on: 08.12.2013
 *      Author: mr.sys
 */

#include <IO/MiniXML/MiniXMLconfigReader.hpp>
#include <mxml.h>
#include <Controller/character_string/ConvertStdStringToTypename.hpp>
#include <IO/MiniXML/MainConfigFileReader.hpp>
#include <IO/MiniXML/GrammarRuleFileReader.hpp>
#include <IO/MiniXML/TranslationRuleFileReader.hpp>
#include <IO/MiniXML/VocAttributeDefintionHandler.hpp>
#include <Controller/TranslationControllerBase.hpp>
#include <FileSystem/GetCurrentWorkingDir.hpp>

namespace VTrans3 {
namespace MiniXML
{
//  TranslationControllerBase * MiniXMLconfigReader::s_p_traNslationController = NULL;
//  I_UserInterface * MiniXMLconfigReader::s_p_userInterface = NULL;

  MiniXMLconfigReader::MiniXMLconfigReader(
//      TranslationControllerBase & r_translationController
    I_UserInterface * p_userInterface, 
    BottomUpParser & bottomUpParser
    )
    : VTrans3::ConfigurationReader<mxml_node_t *>( /* & r_translationController*/ 
        p_userInterface, bottomUpParser)
//      m_translationController(r_translationController)
  {
//    s_p_translationController = & r_translationController;
//    s_p_userInterface = & p_userInterface;
  }

  MiniXMLconfigReader::~MiniXMLconfigReader()
  {
    // TODO Auto-generated destructor stub
  }

//  namespace GrammarRuleFile
//  {
//    /** see http://www.msweet.org/documentation/project3/Mini-XML.html#4_7 */
//    void sax_cb(
//      mxml_node_t * node,
//      mxml_sax_event_t event,
//      void * data
//      )
//    {
//       if (event == MXML_SAX_ELEMENT_OPEN)
//       {
//
//       }
//    }
//  }

//  template <typename XMLelementType> /*virtual*/ bool MiniXMLconfigReader::GetAttributeValue(
//    XMLelementType & xmlElement,
//    std::string & std_strAttributeValue,
//    const char * const attributeName) /*= 0*/
//  {
//    const char * const attributeValue = mxmlElementGetAttr(xmlElement, attributeName);
//    if( attributeValue)
//      std_strAttributeValue = attributeValue;
//    return attributeValue;
//  }

  bool MiniXMLconfigReader::GetAttributeValue(
    mxml_node_t * & xmlElement,
    std::string & std_strAttributeValue,
    const char * const attributeName) /*= 0*/
  {
    const char * const attributeValue = mxmlElementGetAttr(xmlElement, attributeName);
    if( attributeValue)
      std_strAttributeValue = attributeValue;
    return attributeValue != NULL;
  }

  bool MiniXMLconfigReader::ReadFile(const //std::string & cr_stdstrFilePath
    char * const cr_stdstrFilePath,
    mxml_sax_cb_t saxCallBack
    )
  {
    bool fileOpenSucceeded = false;
    FILE * p_file = fopen(cr_stdstrFilePath/*.c_str()*/, "r");
    if( p_file == NULL)
    {
      std::string cwd;// = s_p_translationController->GetCurrentWorkingDir();
      OperatingSystem::GetCurrentWorkingDirA_inl(cwd);
    //TODO output/show  error message
      /*s_p_translationController*/m_p_UserInterface->Message("Failed to open file "
        + cwd + cr_stdstrFilePath);
    }
    else
    {
      mxml_node_t rootXMLnode;
      void * sax_data;
      /*s_p_translationController*/m_std_strCurrentConfigfile = cr_stdstrFilePath;
      mxml_node_t * mxml_node_tLoadFileRes = ::mxmlSAXLoadFile(
        //see http://www.msweet.org/documentation/project3/Mini-XML.pdf
        NULL, //& rootXMLnode,
        p_file,
        /** Callback function or MXML_NO_CALLBACK */
        MXML_NO_CALLBACK,
  //      MiniXML::MainConfigFile::loadFileCallBackFunction,
        saxCallBack, //SAX callback or MXML_NO_CALLBACK
        NULL //sax_data
        );
//      //First node or NULL if the file could not be read.
//      if( mxml_node_tLoadFileRes != NULL )
      fileOpenSucceeded = true;
      fclose(p_file); /** Release memory (if not: memory leak?!) */
    }
    return fileOpenSucceeded;
  }
#ifndef TEST_MINI_XML
  void MiniXMLconfigReader::ReadGrammarRuleFile(
      const std::string & cr_stdstrFilePath )
  {
//    MiniXML::ReadGrammarRuleFile(cr_stdstrFilePath, );
//    FILE * fp = fopen(cr_stdstrFilePath.c_str(), "r");
//    mxml_node_t * top;
//    void * sax_data;
//    mxml_node_t * mxml_node_tLoadFileRes = ::mxmlSAXLoadFile(
//      top,
//      fp,
//      MXML_NO_CALLBACK, //Callback function or MXML_NO_CALLBACK
//      MiniXML::GrammarRuleFile::sax_cb, //SAX callback or MXML_NO_CALLBACK
//      sax_data
//      );
//    //First node or NULL if the file could not be read.
//    if( mxml_node_tLoadFileRes == NULL )
//      ;
    VTrans3::MiniXML::GrammarRuleFileReader grammarRuleFileReader(
      /*m_translationController*/ m_p_UserInterface, m_parseByRise);
    /*return */grammarRuleFileReader.ProcessXML(cr_stdstrFilePath.c_str() );
  }
#endif// #ifndef TEST_MINI_XML

  void error_callback(const char * message)
  {
    const std::string std_strMessage = "in file \"" + MiniXMLconfigReader::
      /*s_p_translationController*/m_std_strCurrentConfigfile + "\" : " + message;
    LOGN_ERROR(std_strMessage)
    MiniXMLconfigReader::/*s_p_translationController*/m_p_UserInterface->
      Message(std_strMessage);
  }

  bool MiniXMLconfigReader::ReadMainConfigFile(
    const std::string & cr_stdstrFilePath )
  {
  	LOGN_DEBUG("begin")
    MiniXML::MainConfigFileReader mainConfigFileReader(
      /*m_translationController*/ m_p_UserInterface, *this);
    mxmlSetErrorCallback(error_callback);
    return mainConfigFileReader.ProcessXML(cr_stdstrFilePath);
  }
} /* namespace MiniXML */
}
