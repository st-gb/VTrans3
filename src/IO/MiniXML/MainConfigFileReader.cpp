/*
 * MainConfigFileReader.cpp
 *
 *  Created on: 09.12.2013
 *      Author: mr.sys
 */

#include <Controller/TranslationControllerBase.hpp>
#include <IO/MiniXML/MainConfigFileReader.hpp>
#include <mxml.h>
#include <Controller/character_string/ConvertStdStringToTypename.hpp>
#include <string.h> // strcmp

namespace VTrans3 {
namespace MiniXML
{
//  TranslationControllerBase * MainConfigFileReader::s_p_translationController;
  I_UserInterface * MainConfigFileReader::s_p_UserInterface;

  MainConfigFileReader::MainConfigFileReader(
//    TranslationControllerBase & r_translationController,
    I_UserInterface * p_UserInterface,
    ConfigurationHandler_type & configurationReader)
    : MainConfigFileReaderBase(configurationReader)
     , m_successfullyProcessedAllReferredFiles(true)
  {
//    s_p_translationController = & r_translationController;
    s_p_UserInterface = p_UserInterface;
  }

  MainConfigFileReader::~MainConfigFileReader()
  {
    // TODO Auto-generated destructor stub
  }

  namespace MainConfigFile
  {
    /** from mxml-2.7/doc/mxml.html#LOAD_CALLBACKS */
    mxml_type_t loadFileCallBackFunction(mxml_node_t *node)
    {
      const char *type;

     /*
      * You can lookup attributes and/or use the
      * element name, hierarchy, etc...
      */

//      type = mxmlElementGetAttr(node, "type");
//      if (type == NULL)
        type = mxmlGetElement(node);

//      if (!strcmp(type, "integer"))
//        return (MXML_INTEGER);
//      else if (!strcmp(type, "opaque"))
//        return (MXML_OPAQUE);
//      else if (!strcmp(type, "real"))
//        return (MXML_REAL);
//      else
        return (MXML_TEXT);
    }

    /** see http://www.msweet.org/documentation/project3/Mini-XML.html#4_7 */
    void sax_callback(
      mxml_node_t * node,
      mxml_sax_event_t event,
      void * data
      )
    {
//      LOGN_DEBUG("begin--node:" << node )
     MainConfigFileReader & mainConfigFileReader = * (MainConfigFileReader *) data;
//      std::cout << "begin--node:" << node << std::endl;
      const char * xmlElementName = mxmlGetElement(node);
      if( xmlElementName == NULL )
    	  LOGN_DEBUG("element name:NULL")
      else
      {
    	  LOGN_DEBUG("element name:" << xmlElementName)
//      std::cout << "element nameXX:" << xmlElementName << std::endl;
//#ifndef __linux__
//      if (event == MXML_SAX_ELEMENT_CLOSE)
//      {
//        if( ::strcmp(xmlElementName, "VTrans_main_cfg") == 0 )
//        {
//            int i = 0;
//        }
//      }
        if (event == MXML_SAX_ELEMENT_OPEN)
        {
//#ifdef COMPILE_AS_EXECUTABLE
////        else if( m_strElementName == "drawing" )
////          HandleGUI_XMLelement(cr_xercesc_attributes);
//        else
//#endif
//      }
////#endif
          if( ! mainConfigFileReader.openingXMLelement(xmlElementName, node) )
            mainConfigFileReader.m_successfullyProcessedAllReferredFiles =
              false;
        }
      }
    }
  } //namespace MainConfigFile

  ///\return: 
  bool MainConfigFileReader::ProcessXML(const std::string & cr_stdstrFilePath)
  {
    bool fileOpenSucceeded = false;
    FILE * fp = fopen(cr_stdstrFilePath.c_str(), "r");
    if( fp != NULL)///May also be non-NULL if the path is a directory.
    {
//      mxml_node_t rootXMLnode;
//      void * sax_data;
      mxml_node_t * mxml_node_tLoadFileRes = ::mxmlSAXLoadFile(
        //see http://www.msweet.org/documentation/project3/Mini-XML.pdf
        NULL, //& rootXMLnode,
        fp,
        /** Callback function or MXML_NO_CALLBACK */
        MXML_NO_CALLBACK,
  //      MiniXML::MainConfigFile::loadFileCallBackFunction,
        MiniXML::MainConfigFile::sax_callback, //SAX callback or MXML_NO_CALLBACK
        /** Is passed by MiniXML framework to the SAX callback function as 3rd 
         *  parameter. */
        this //NULL //sax_data
        );
//      //First node or NULL if the file could not be read.
//      if( mxml_node_tLoadFileRes != NULL )
       if( m_successfullyProcessedAllReferredFiles //&&
         /// is e.g. NULL if the path is a directory?.
         /*mxml_node_tLoadFileRes != NULL*/)
         fileOpenSucceeded = true;
      fclose(fp);
    }
//    else
//      unsigned long int errCode = OperatingSystem::GetLastErrorCode();
    return fileOpenSucceeded;
  }
} /* namespace MiniXML */
}
