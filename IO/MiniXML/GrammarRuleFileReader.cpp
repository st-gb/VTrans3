/*
 * GrammarRuleFileReader.cpp
 *
 *  Created on: 22.12.2013
 *      Author: mr.sys
 */

#include <IO/MiniXML/GrammarRuleFileReader.hpp>
#include <string> //class std::string
 //class TranslationControllerBase
#include <Controller/TranslationControllerBase.hpp>
#include <Parse/ParseByRise.hpp>
#include <mxml.h>

namespace VTrans3
{
  namespace MiniXML
  {
    TranslationControllerBase * GrammarRuleFileReader::s_p_translationController = NULL;

    GrammarRuleFileReader::GrammarRuleFileReader(
        TranslationControllerBase & r_translationController )
    {
      s_p_translationController = & r_translationController;
    }

    GrammarRuleFileReader::~GrammarRuleFileReader()
    {
      // TODO Auto-generated destructor stub
    }

    namespace GrammarRuleFile
    {
      void InsertGrammarRule(
        const char * stdstrLeftChild,
        const char * stdstrRightChild,
        const char * stdstrSuperordinate
        )
      {
        enum ParseByRise::InsertGrammarRuleReturnCodes
          insertGrammarRuleReturnCode = GrammarRuleFileReader::s_p_translationController->
          m_parsebyrise.InsertGrammarRule(
          stdstrLeftChild //const char * cp_chLeftGrammarRuleName
          , stdstrRightChild //const char * cp_chRightGrammarRuleName
          , //std::string
          stdstrSuperordinate //const char * cp_chSuperordinateGrammarRuleName
          ) ;
//        std::wstring std_wstrMessage;
        if( insertGrammarRuleReturnCode != ParseByRise::AllGrammarPartsAreKnown )
        {
//          std_wstrMessage = L"In document \n\"" +
//            Xerces::ConvertXercesStringToStdWstring(m_pc_locator->
//              getSystemId() ) +
//            L"\"\n"
//            L" line:"
//            + GetStdWstring( convertToStdString(m_pc_locator->
//                getLineNumber() ) )
//            + L" column:"
//            + GetStdWstring( convertToStdString(m_pc_locator->
//                getColumnNumber() ) ) +
//            L" : grammar rule \"" + GetStdWstring(stdstrSuperordinate)
//            + L"\" was not added because: ";
          GrammarRuleFileReader::s_p_translationController->Message(
           "error in grammar rule file: was not added because: unknown grammar part" );
        }
      }

      /** see http://www.msweet.org/documentation/project3/Mini-XML.html#4_7 */
      void sax_cb(
        mxml_node_t * node,
        mxml_sax_event_t event,
        void * data
        )
      {
        const char * xmlElementName = mxmlGetElement(node);
//        if (event == MXML_SAX_ELEMENT_CLOSE)
//        {
//          if( ::strcmp(xmlElementName, "VTrans_main_cfg") == 0 )
//          {
//              int i = 0;
//          }
//        }
        if (event == MXML_SAX_ELEMENT_OPEN)
        {
          if( ::strcmp(xmlElementName, "grammar_rule") == 0 )
          {
//            HandleReadGrammarRuleFileXMLelement( node ) ;
            const char * const strLeftChildSyntaxTreePath =
              mxmlElementGetAttr(node, "left_child");
            const char * const strRightChildSyntaxTreePath =
              mxmlElementGetAttr(node, "right_child");
            const char * const strSuperOrdinateSyntaxTreePath =
              mxmlElementGetAttr(node, "superordinate");
            if( strLeftChildSyntaxTreePath != NULL &&
                strSuperOrdinateSyntaxTreePath )
            {
              if( strRightChildSyntaxTreePath )
                InsertGrammarRule(
                  strLeftChildSyntaxTreePath,
                  strRightChildSyntaxTreePath,
                  strSuperOrdinateSyntaxTreePath
                  );
              else
              {
                ParseByRise & parseByRise =
                  GrammarRuleFileReader::s_p_translationController->m_parsebyrise;
                enum ParseByRise::InsertGrammarRuleReturnCodes
                  insertGrammarRuleReturnCode = parseByRise.InsertSuperClassGrammarRule(
                    strLeftChildSyntaxTreePath , //cp_chSubclassGrammarRuleName
                    strSuperOrdinateSyntaxTreePath //cp_chSuperclassGrammarRuleName
                  ) ;
                if( insertGrammarRuleReturnCode != ParseByRise::AllGrammarPartsAreKnown )
                {
                  std::string insertGrammarRuleErrMsg = parseByRise.GetErrorMessage(
                    insertGrammarRuleReturnCode);
                  GrammarRuleFileReader::s_p_translationController->Message(
                   "error in grammar rule file: was not added because: unknown grammar part" );
                }
              }
            }
          }
        }
      }
    }

    bool GrammarRuleFileReader::ProcessXML(const //std::string & cr_stdstrFilePath
      char * const cr_stdstrFilePath)
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
          MiniXML::GrammarRuleFile::sax_cb, //SAX callback or MXML_NO_CALLBACK
          NULL //sax_data
          );
  //      //First node or NULL if the file could not be read.
  //      if( mxml_node_tLoadFileRes != NULL )
          fileOpenSucceeded = true;
      }
      return fileOpenSucceeded;
    }
  } /* namespace MiniXML */
} /* namespace VTrans3 */
