#include "ParseByRise.hpp"
#include <IO/ParseTree2XMLtreeTraverser.hpp>
#include <Controller/TranslationProcess.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
#include <Controller/character_string/convertFromAndToStdString.hpp>
#include <InputOutput/XML/OutputXMLindented_inl.hpp>

namespace VTrans3 {
  
  std::string BottomUpParser::GetAsIndentedXML() const
  {
    std::string std_strXML, std_strIntendedXML;
    ByteArray byteArray; //crashes in parallel translation version in constructor
    GenerateXMLtree( /*std_strXML*/ byteArray);
    const BYTE * const byteArrayBegin = byteArray.GetArray();
    const fastestUnsignedDataType byteArraySize = byteArray.GetSize();
    std_strXML = UTF8string::GetAsISO_8859_1StdString(byteArrayBegin,
      byteArraySize );
    std::ostringstream std_ostringstream;
    OutputXMLindented_inl(std_strXML.c_str(), std_ostringstream);
    std_strIntendedXML = std_ostringstream.str();
    return std_strIntendedXML;
  }

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

  ///\brief gets the parse tree as XML in UTF-8 format
  void BottomUpParser::GenerateXMLtree(
  //  std::string & std_strXML
    ByteArray & byteArray
    ) const
  {
    ///PTNs=Parse Tree Nodes, Tkn=Token, Idx=Index
    std::vector<GrammarPart *> p_PTNsCoveringMostTknsAtLeftTknIdx ;
  //  std::string std_strXML;
    m_r_translationProcess.SetStatus(
      VTrans::generateXMLtreeFromParseTree,
      "",
      -1
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
    GetGrammarPartCoveringMostTokens(
      leftMostTokenIndex ,
      p_PTNsCoveringMostTknsAtLeftTknIdx
      ) ;
  ///If there is at least 1 tree either:
  ///-at the first token
  ///-1 token right of the rightmost token index of the last tree(s)
    while(! p_PTNsCoveringMostTknsAtLeftTknIdx.empty() )
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

#ifdef _DEBUG
      GCC_DIAG_OFF(unused-variable)
      const fastestUnsignedDataType numParseTrees = 
        p_PTNsCoveringMostTknsAtLeftTknIdx.size();
      GCC_DIAG_ON(unused-variable)
#endif //#ifdef _DEBUG
//      WORD wConsecutiveID = 0 ;

      //Loop over all parse trees beginning at token index
      //"leftMostTokenIndex".
      for( std::vector<GrammarPart *>::const_iterator
          c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex =
          p_PTNsCoveringMostTknsAtLeftTknIdx.begin() ;
          c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex <
          p_PTNsCoveringMostTknsAtLeftTknIdx.end() ;
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

      if( p_PTNsCoveringMostTknsAtLeftTknIdx.empty() )
        leftMostTokenIndex = 0;//TODO unneccessary instruction?!
      else
        leftMostTokenIndex =
          p_PTNsCoveringMostTknsAtLeftTknIdx.at(0)->
          m_dwRightmostIndex + 1;
      LOGN("leftMostTokenIndex:" << leftMostTokenIndex )
      GetGrammarPartCoveringMostTokens(
        leftMostTokenIndex ,
        p_PTNsCoveringMostTknsAtLeftTknIdx
        ) ;
    }
    //while( leftMostTokenIndex );
    byteArray.add("</translated_text>"); //Topmost XML element.
  }
}
