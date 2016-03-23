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
#include <IO/UnknownGrammarPartNameException.hpp>
#include <Parse/ParseByRise.hpp>
#include <mxml.h>

extern TranslationControllerBase * g_p_translationcontrollerbase;

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
        ParseByRise & parseByRise =
          GrammarRuleFileReader::s_p_translationController->m_parsebyrise;
        try
        {
          enum ParseByRise::InsertGrammarRuleReturnCodes
            insertGrammarRuleReturnCode = parseByRise.InsertGrammarRule(
            stdstrLeftChild //const char * cp_chLeftGrammarRuleName
            , stdstrRightChild //const char * cp_chRightGrammarRuleName
            , //std::string
            stdstrSuperordinate //const char * cp_chSuperordinateGrammarRuleName
            ) ;
  //        std::wstring std_wstrMessage;
          if( insertGrammarRuleReturnCode != ParseByRise::AllGrammarPartsAreKnown )
          {
            //TODO output line and column of error
//          std_wstrMessage = L" line:"            + L" column:"
//            L" : grammar rule \"" + GetStdWstring(stdstrSuperordinate)

            const std::string insertGrammarRuleErrMsg = parseByRise.GetErrorMessage(
              insertGrammarRuleReturnCode);
            const std::string message = "error in grammar rule file \"" +
              g_p_translationcontrollerbase->m_std_strCurrentConfigfile
              + "\"\n: was not added because: " +
              insertGrammarRuleErrMsg;
            GrammarRuleFileReader::s_p_translationController->Message(message);
          }
        }
        catch(const VTrans::UnknownGrammarPartNameException & ugpne)
        {
          const std::string message = "error in grammar rule file \"" +
            g_p_translationcontrollerbase->m_std_strCurrentConfigfile
            + "\"\n: \"" + stdstrSuperordinate + "\" was not added because:\n" +
//            insertGrammarRuleErrMsg;
            ugpne.GetErrorMessage();
          GrammarRuleFileReader::s_p_translationController->Message(message);
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
#ifdef COMPILE_AS_EXECUTABLE
                g_p_translationcontrollerbase->
                  m_std_map_grammarRuleName2filepath.insert(
                  std::make_pair(std::string(strSuperOrdinateSyntaxTreePath),
                    g_p_translationcontrollerbase->m_std_strCurrentConfigfile) );
#endif
              if( strRightChildSyntaxTreePath )
              {
                InsertGrammarRule(
                  strLeftChildSyntaxTreePath,
                  strRightChildSyntaxTreePath,
                  strSuperOrdinateSyntaxTreePath
                  );
              }
              else
              {
                ParseByRise & parseByRise =
                  GrammarRuleFileReader::s_p_translationController->m_parsebyrise;
                try
                {
                  enum ParseByRise::InsertGrammarRuleReturnCodes
                    insertGrammarRuleReturnCode = parseByRise.InsertSuperClassGrammarRule(
                      strLeftChildSyntaxTreePath , //cp_chSubclassGrammarRuleName
                      strSuperOrdinateSyntaxTreePath //cp_chSuperclassGrammarRuleName
                    ) ;
                  if( insertGrammarRuleReturnCode != ParseByRise::AllGrammarPartsAreKnown )
                  {
                    const std::string insertGrammarRuleErrMsg = parseByRise.GetErrorMessage(
                      insertGrammarRuleReturnCode);

                    const std::string message = "error in grammar rule file \"" +
                      g_p_translationcontrollerbase->m_std_strCurrentConfigfile
                      + "\": \"\n" +
                      strSuperOrdinateSyntaxTreePath + "\" was not added because:\n"
                        " unknown grammar part " +
                        insertGrammarRuleErrMsg;
                    GrammarRuleFileReader::s_p_translationController->Message(message);

                    GrammarRuleFileReader::s_p_translationController->Message(
                     "error in grammar rule file: was not added because: unknown grammar part" );
                  }
                }
                catch(const VTrans::UnknownGrammarPartNameException & ugpne)
                {
                  const std::string message = "error in grammar rule file \"" +
                    g_p_translationcontrollerbase->m_std_strCurrentConfigfile
                    + "\"\n: was not added because:\n" +
        //            insertGrammarRuleErrMsg;
                    ugpne.GetErrorMessage();
                  GrammarRuleFileReader::s_p_translationController->Message(message);
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
//#ifdef COMPILE_AS_EXECUTABLE
        // cr_stdstrFilePath = GetAbsoluteFilePath(cr_stdstrFilePath)
        g_p_translationcontrollerbase->m_std_strCurrentConfigfile =
          cr_stdstrFilePath;
//#endif
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
