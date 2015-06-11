/*
 * VocAttributeDefintionHandler.cpp
 *
 *  Created on: 22.12.2013
 *      Author: mr.sys
 */

#include <IO/MiniXML/VocAttributeDefintionHandler.hpp>
#include <Controller/TranslationControllerBase.hpp>
#include <Controller/character_string/ConvertStdStringToTypename.hpp>
#include <IO/MiniXMLconfigReader.hpp> //class MiniXMLconfigReader::s_p_translationController

namespace VTrans3
{
  namespace MiniXML
  {
    TranslationControllerBase * VocAttributeDefintionHandler::s_p_translationController = NULL;

    VocAttributeDefintionHandler::VocAttributeDefintionHandler(
        TranslationControllerBase & r_translationController )
    {
      s_p_translationController = & r_translationController;
    }

    VocAttributeDefintionHandler::~VocAttributeDefintionHandler()
    {
      // TODO Auto-generated destructor stub
    }

    template <typename typenameConvertFromString> inline void
      ConvertToTypename(
        typenameConvertFromString & r_typename_convert_from_string,
        const std::string & strAttributeName)
    {
      if( ConvertStdStringToTypename(r_typename_convert_from_string,
          strAttributeName) == NS_ConvertStdStringToTypename::failed )
      {
        std::ostringstream stdoss;
        stdoss << "Converting \"" << strAttributeName << "\" (to a number) failed";
        //TODO line and column number in message
        MiniXML::MiniXMLconfigReader::s_p_translationController->Message(
          stdoss.str() );
      }
    }

    namespace VocAttributeDefintionFile
    {
      /** see http://www.msweet.org/documentation/project3/Mini-XML.html#4_7 */
      void sax_cb(
        mxml_node_t * node,
        mxml_sax_event_t event,
        void * data
        )
      {
    	LOGN_DEBUG("begin")
        const char * xmlElementName = mxmlGetElement(node);
    	if( xmlElementName == NULL )
          LOGN_DEBUG("element name: NULL")
        else
        {
    	LOGN_DEBUG("element:" << xmlElementName)
  //        if (event == MXML_SAX_ELEMENT_CLOSE)
  //        {
  //          if( ::strcmp(xmlElementName, "VTrans_main_cfg") == 0 )
  //          {
  //              int i = 0;
  //          }
  //        }
        if (event == MXML_SAX_ELEMENT_OPEN)
        {
          if( ::strcmp(xmlElementName, "vocabulary_attribute_definiton") == 0 )
          {
            WORD wAttributeIndex ;
            WORD wAttributeLength ;
            WORD wAttributeType ;
            WORD wLanguage = AttributeTypeAndPosAndSize::beyondLastElement;
            const char * const strWordClass =
              mxmlElementGetAttr(node, "word_class");
//            if( ! strWordClass )

            const char * const strAttributeName =
              mxmlElementGetAttr(node, "attribute_name");
            const char * const strLanguage =
              mxmlElementGetAttr(node, "language");

            const char * const strAttributeType =
              mxmlElementGetAttr(node, "attribute_type");
//            if( ! ConvertStdStringToTypename(wAttributeType,
//              strAttributeType) )
////                 Message();
//              ; //strAttributeType = "";

            const char * const strAttributeIndex =
              mxmlElementGetAttr(node, "attribute_index");
            ConvertToTypename(wAttributeIndex, strAttributeIndex);

            const char * const strAttributeLength =
              mxmlElementGetAttr(node, "attribute_length");
            ConvertToTypename(wAttributeLength, strAttributeLength);

            if( strcmp(strLanguage , "German") == 0 )
              wLanguage = AttributeTypeAndPosAndSize::German ;
            else if( strcmp(strLanguage , "English") == 0 )
              wLanguage = AttributeTypeAndPosAndSize::English ;
            else
              VocAttributeDefintionHandler::s_p_translationController->
                Message("language not ele {\"English\";\"German\"}") ;

            if( strcmp(strAttributeType , "string") == 0 )
              wAttributeType = AttributeTypeAndPosAndSize::string ;
            else
              wAttributeType = AttributeTypeAndPosAndSize::bit ;

            if( strWordClass && strAttributeName )
            {
              std::string stdstrWordClass(strWordClass);

              MiniXML::MiniXMLconfigReader::s_p_translationController->
                m_translateparsebyrisetree.AddVocAndTranslDefinition(
                strWordClass ,
                strAttributeName ,
                wLanguage ,
                wAttributeType ,
                wAttributeIndex ,
                wAttributeLength
                ) ;
            }
          }
        }
		}
      }
    }
  } /* namespace MiniXML */
} /* namespace VTrans3 */
