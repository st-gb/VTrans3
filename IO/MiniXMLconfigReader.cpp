/*
 * MiniXMLconfigReader.cpp
 *
 *  Created on: 08.12.2013
 *      Author: mr.sys
 */

#include <IO/MiniXMLconfigReader.hpp>
#include <mxml.h>
#include <Controller/character_string/ConvertStdStringToTypename.hpp>
#include <IO/MiniXML/MainConfigFileReader.hpp>
#include <IO/MiniXML/GrammarRuleFileReader.hpp>
#include <IO/MiniXML/TranslationRuleFileReader.hpp>
#include <IO/MiniXML/VocAttributeDefintionHandler.hpp>
#include <Controller/TranslationControllerBase.hpp>

namespace VTrans3 {
namespace MiniXML
{
  TranslationControllerBase * MiniXMLconfigReader::s_p_translationController = NULL;

  MiniXMLconfigReader::MiniXMLconfigReader(
      TranslationControllerBase & r_translationController)
    : VTrans3::ConfigurationReader<mxml_node_t *>( & r_translationController)
//      m_translationController(r_translationController)
  {
    s_p_translationController = & r_translationController;
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
    return attributeValue;
  }

  bool MiniXMLconfigReader::ReadFile(const //std::string & cr_stdstrFilePath
    char * const cr_stdstrFilePath,
    mxml_sax_cb_t saxCallBack
    )
  {
    bool fileOpenSucceeded = false;
    FILE * fp = fopen(cr_stdstrFilePath/*.c_str()*/, "r");
    if( fp == NULL)
    {
      std::string cwd = s_p_translationController->GetCurrentWorkingDir();
      s_p_translationController->Message("Failed to open file "
        + cwd + cr_stdstrFilePath);
    }
    else
    {
      mxml_node_t rootXMLnode;
      void * sax_data;
      mxml_node_t * mxml_node_tLoadFileRes = ::mxmlSAXLoadFile(
        //see http://www.msweet.org/documentation/project3/Mini-XML.pdf
        NULL, //& rootXMLnode,
        fp,
        /** Callback function or MXML_NO_CALLBACK */
        MXML_NO_CALLBACK,
  //      MiniXML::MainConfigFile::loadFileCallBackFunction,
        saxCallBack, //SAX callback or MXML_NO_CALLBACK
        NULL //sax_data
        );
//      //First node or NULL if the file could not be read.
//      if( mxml_node_tLoadFileRes != NULL )
        fileOpenSucceeded = true;
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
    VTrans3::MiniXML::GrammarRuleFileReader grammarRuleFileReader(m_translationController);
    /*return */grammarRuleFileReader.ProcessXML(cr_stdstrFilePath.c_str() );
  }
#endif// #ifndef TEST_MINI_XML
  bool MiniXMLconfigReader::ReadMainConfigFile(
    const std::string & cr_stdstrFilePath )
  {
	LOGN_DEBUG("begin")
    MiniXML::MainConfigFileReader mainConfigFileReader(m_translationController);
    return mainConfigFileReader.ProcessXML(cr_stdstrFilePath);
  }
#ifndef TEST_MINI_XML
  void MiniXMLconfigReader::ReadVocAttributeDefinitionFile(
    const std::string & cr_stdstrFilePath)
  {
	LOGN_DEBUG("begin")
//    VTrans3::MiniXML::VocAttributeDefintionHandler
//      vocAttributeDefintionHandler(m_translationController);
    ReadFile(cr_stdstrFilePath.c_str(), VTrans3::MiniXML::VocAttributeDefintionFile::sax_cb);
  }
  void MiniXMLconfigReader::ReadTranslationRuleFile(const std::string & cr_stdstrFilePath)
  {
    TranslationRuleFileReader translationRuleFileReader(
        s_p_translationController->m_translateparsebyrisetree
        , s_p_translationController->m_parsebyrise
        , * s_p_translationController
        , * this
        );
//    ReadFile(cr_stdstrFilePath.c_str(), VTrans3::MiniXML::ReadTranslationRuleFile::sax_cb);
    translationRuleFileReader.Process(cr_stdstrFilePath);
  }
#endif //#ifndef TEST_MINI_XML
} /* namespace MiniXML */
}
