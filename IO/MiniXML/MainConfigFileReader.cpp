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
  TranslationControllerBase * MainConfigFileReader::s_p_translationController;

  MainConfigFileReader::MainConfigFileReader(
      TranslationControllerBase & r_translationController )
  {
    s_p_translationController = & r_translationController;
  }

  MainConfigFileReader::~MainConfigFileReader()
  {
    // TODO Auto-generated destructor stub
  }

  namespace MainConfigFile
  {
    void HandleGrammartPartColourXMLelement(
        mxml_node_t * node)
    {
      const char * const strGrammarPartName = mxmlElementGetAttr(node, "name");
      if( strGrammarPartName != NULL )
      {
        const char * const strColour = mxmlElementGetAttr(node, "colour");
        if( strColour != NULL )
        {
          unsigned dwValue;
          ConvertStdStringToTypename(dwValue, std::string(strColour) );
          MainConfigFileReader::s_p_translationController->m_nodetrie_ui32GrammarPartName2colour.
            insert_inline(
            (BYTE *) strGrammarPartName,
            ::strlen(strGrammarPartName ),
            dwValue
            );
        }
      }
    }
    void HandleReadGrammarRuleFileXMLelement( mxml_node_t * node )
    {
      const char * const strGrammarRuleFilePath =
        mxmlElementGetAttr(node, "path");
      if( strGrammarRuleFilePath != NULL )
      {
        LOGN_DEBUG("grammar rule file path:" << strGrammarRuleFilePath)
//            strVocabularyFilePath
        MainConfigFileReader::s_p_translationController->ReadGrammarRuleFile(
          strGrammarRuleFilePath);
      }
    }

    void HandleReadVocabularyAttributeDefinitionFileXMLelement(
        mxml_node_t * node )
    {
      const char * const strVocabularyAttributeDefinitionFilePath =
        mxmlElementGetAttr(node, "path");
      if( strVocabularyAttributeDefinitionFilePath != NULL )
      {
        LOGN_DEBUG("vocabulary_attribute_definition file path:"
          << strVocabularyAttributeDefinitionFilePath)
//            strVocabularyFilePath
        MainConfigFileReader::s_p_translationController->ReadVocAttributeDefinitionFile(
          strVocabularyAttributeDefinitionFilePath);
      }
    }

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
    void sax_cb(
      mxml_node_t * node,
      mxml_sax_event_t event,
      void * data
      )
    {
      const char * xmlElementName = mxmlGetElement(node);
//      if (event == MXML_SAX_ELEMENT_CLOSE)
//      {
//        if( ::strcmp(xmlElementName, "VTrans_main_cfg") == 0 )
//        {
//            int i = 0;
//        }
//      }
      if (event == MXML_SAX_ELEMENT_OPEN)
      {
#ifdef COMPILE_AS_EXECUTABLE
        if( ::strcmp(xmlElementName, "grammar_part") == 0 )
        {
          HandleGrammartPartColourXMLelement(node);
        }
        else if( m_strElementName == "drawing" )
          HandleGUI_XMLelement(cr_xercesc_attributes);
        else
#endif
        if( ::strcmp(xmlElementName, "grammar_rule_file") == 0 )
        {
          HandleReadGrammarRuleFileXMLelement( node ) ;
        }
//         else if( m_strElementName == "transformation_rule_file" )
//         {
//           HandleReadTransformationRuleFileXMLelement( cr_xercesc_attributes ) ;
//         }
         else if( ::strcmp(xmlElementName, "translation_rule_file") == 0 )
         {
//           HandleReadTranslationRuleFileXMLelement( cr_xercesc_attributes ) ;
             int i = 0;
         }
         else if( ::strcmp(xmlElementName, "vocabulary_attribute_definition_file") == 0 )
         {
           HandleReadVocabularyAttributeDefinitionFileXMLelement(
             node ) ;
         }
        else if( ::strcmp(xmlElementName, "vocabulary_file") == 0 )
        {
          const char * const strVocabularyFilePath =
            mxmlElementGetAttr(node, "path");
          if( strVocabularyFilePath != NULL )
          {
            LOGN_DEBUG("Dict file path:" << strVocabularyFilePath)
//            strVocabularyFilePath
            MainConfigFileReader::s_p_translationController->
              /*m_configurationHandler.*/m_stdstrVocabularyFilePath =
              strVocabularyFilePath;
          }
        }
     }
    }
  }

  bool MainConfigFileReader::ProcessXML(const std::string & cr_stdstrFilePath)
  {
    bool fileOpenSucceeded = false;
    FILE * fp = fopen(cr_stdstrFilePath.c_str(), "r");
    if( fp != NULL)
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
        MiniXML::MainConfigFile::sax_cb, //SAX callback or MXML_NO_CALLBACK
        NULL //sax_data
        );
//      //First node or NULL if the file could not be read.
//      if( mxml_node_tLoadFileRes != NULL )
        fileOpenSucceeded = true;
    }
    return fileOpenSucceeded;
  }
} /* namespace MiniXML */
}
