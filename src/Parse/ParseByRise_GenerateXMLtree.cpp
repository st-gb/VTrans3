#include "ParseByRise.hpp"
#include <IO/ParseTree2XMLtreeTraverser.hpp>
#include <Controller/TranslationProcess.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
#include <Controller/character_string/convertFromAndToStdString.hpp>

namespace VTrans3 {
  void BottomUpParser::GenerateXMLtreeFromParseTree(
    std::vector<GrammarPart *>::const_iterator
      c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex,
    /*std::string & std_strXML*/ ByteArray & byteArray
    ) const
  {
    ParseTreeTraverser::ParseTree2XMLtreeTraverser parsetree2xmltreetraverser(
      //        p_grammarpart
      * c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex
      , * this
      );
  //  translatetreetraverser.m_wConsecutiveID = wConsecutiveID;

  //  //The transformation of the tree may lead to a crash/
  //  // only the leaves need to be processed-> set to "false".
  //  parsetree2xmltreetraverser.m_bTraverseTree = false ;

    parsetree2xmltreetraverser.Traverse();
//    std::string & std_strXML = parsetree2xmltreetraverser.m_std_strXML;
    const ByteArray & inner = parsetree2xmltreetraverser.m_byteArray;
    byteArray.add(inner.GetArray(), inner.GetSize() );
  }

  void BottomUpParser::GenerateXMLtree(
  //  std::string & std_strXML
    ByteArray & byteArray
    ) const
  {
    std::vector<GrammarPart *>
      stdvec_p_grammarpartCoveringMostTokensAtTokenIndex ;
  //  std::string std_strXML;
    m_r_translationProcess.SetStatus(
      VTrans::generateXMLtreeFromParseTree,
      ""
      );
    fastestUnsignedDataType leftMostTokenIndex = 0 ;
//    GrammarPart * p_grammarpart ;
//    GrammarPart * p_grammarpartChild ;
    //int y = 10 ;
    //  typedef std::multimap<DWORD, GrammarPart >
    //  stdmmap_token_index2grammarpart ;
    typedef std::multimap<DWORD, GrammarPart *>
      stdmmap_token_index2grammarpart ;
    stdmmap_token_index2grammarpart::const_iterator citer ;
    const stdmmap_token_index2grammarpart &
      r_stdmultimap_dwLeftmostIndex2grammarpart = 
      //m_stdmultimap_dwLeftmostIndex2grammarpart ;
      m_stdmultimap_dwLeftmostIndex2p_grammarpart ;

    std::string std_strXML;
    //de.wikipedia.org/wiki/Codepage_850
    //http://de.wikipedia.org/wiki/ISO_8859-1:
    //http://www.torsten-horn.de/techdocs/encoding.htm
    // ISO-8859-1:  e.g. "ö"=0xf6
    const char * const encoding = /*"ISO-8859-1"*/ "UTF-8";
    byteArray.add("<?xml version=\"1.0\" encoding=\"");
    byteArray.add(encoding);
    byteArray.add("\" ?>");
//      byteArray.add(std_strXML.c_str(), std_strXML.length() );

    /** Topmost XML element. */
    byteArray.add("<translated_text>");
//    //Reset to initial before each translation.
//    m_ui32ConcatenationID = 1;
    do
    {
//      //Before each draw in order to begin at x position "0".
//      m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.clear() ;
//      m_wParseLevel = 0 ;
//      DEBUG_COUT( "Translate(): p_parsebyrise != NULL\n")
      byteArray.add("<translations token_index=\"");
      byteArray.add( ::convertToStdString(leftMostTokenIndex).c_str() );
      byteArray.add("\">");
//        byteArray.add(std_strXML.c_str(), std_strXML.length() );

      citer = r_stdmultimap_dwLeftmostIndex2grammarpart.begin() ;
      //p_grammarpart =
      GetGrammarPartCoveringMostTokens(
        leftMostTokenIndex ,
        stdvec_p_grammarpartCoveringMostTokensAtTokenIndex
        ) ;

#ifdef _DEBUG
      GCC_DIAG_OFF(unused-variable)
      WORD wSize = stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.size();
      GCC_DIAG_ON(unused-variable)
#endif //#ifdef _DEBUG
//      WORD wConsecutiveID = 0 ;

      //Loop over all parse trees beginning at token index
      //"leftMostTokenIndex".
      for( std::vector<GrammarPart *>::const_iterator
          c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex =
          stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.begin() ;
          c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex <
          stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.end() ;
          ++ c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex
         )
      {
        byteArray.add("<translation>");
        GenerateXMLtreeFromParseTree(
          c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex,
          /*std_strXML*/ byteArray
          );
        byteArray.add("</translation>");
      } //loop for all parse trees beginning at same token index
      byteArray.add("</translations>");

      if( stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.empty() )
        leftMostTokenIndex = 0;
      else
        leftMostTokenIndex =
          stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.at(0)->
          m_dwRightmostIndex + 1;
      LOGN("leftMostTokenIndex:" << leftMostTokenIndex )
    }
    while( leftMostTokenIndex );
    byteArray.add("</translated_text>"); //Topmost XML element.
  }
}
