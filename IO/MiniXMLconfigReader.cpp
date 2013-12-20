/*
 * MiniXMLconfigReader.cpp
 *
 *  Created on: 08.12.2013
 *      Author: mr.sys
 */

#include <IO/MiniXMLconfigReader.hpp>
#include <mxml.h>
#include <Controller/character_string/ConvertStdStringToTypename.hpp>
#include <IO/MiniXML/ReadMainConfigFile.hpp>
#include <Controller/TranslationControllerBase.hpp>

namespace MiniXML
{

  MiniXMLconfigReader::MiniXMLconfigReader(
      TranslationControllerBase & r_translationController)
    : m_translationController(r_translationController)
  {
    // TODO Auto-generated constructor stub

  }

  MiniXMLconfigReader::~MiniXMLconfigReader()
  {
    // TODO Auto-generated destructor stub
  }

  namespace GrammarRuleFile
  {
    /** see http://www.msweet.org/documentation/project3/Mini-XML.html#4_7 */
    void sax_cb(
      mxml_node_t * node,
      mxml_sax_event_t event,
      void * data
      )
    {
       if (event == MXML_SAX_ELEMENT_OPEN)
       {

       }
    }
  }


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
  }
  bool MiniXMLconfigReader::ReadMainConfigFile(
    const std::string & cr_stdstrFilePath )
  {
    MiniXML::ReadMainConfigFile mainConfigFileReader(m_translationController);
    return mainConfigFileReader.ProcessXML(cr_stdstrFilePath);
  }
  void MiniXMLconfigReader::ReadVocAttributeDefinitionFile(
    const std::string & cr_stdstrFilePath)
  {

  }
  void MiniXMLconfigReader::ReadTranslationRuleFile(const std::string & cr_stdstrFilePath)
  {

  }
} /* namespace MiniXML */
