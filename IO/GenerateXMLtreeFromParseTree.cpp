/*
 * GenerateXMLtreeFromParseTree.cpp
 *
 *  Created on: 24.09.2011
 *      Author: Stefan
 */
#include "ParseTree2XMLtreeTraverser.hpp"
#include <Parse/GrammarPart.hpp> //class GrammarPart
#include <Parse/ParseByRise.hpp> //class ParseByRise
#include <map> //class std::multimap
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
//convertToStdString(...)
#include <Controller/character_string/stdstring_format.hpp>

void GenerateXMLtreeFromParseTree(
  std::vector<GrammarPart *>::const_iterator
    c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex,
  ParseByRise * p_parsebyrise,
  std::string & std_strXML
  )
{
  ParseTreeTraverser::ParseTree2XMLtreeTraverser parsetree2xmltreetraverser(
    //        p_grammarpart
    * c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex
    , * p_parsebyrise
    );
//  translatetreetraverser.m_wConsecutiveID = wConsecutiveID;

//  //The transformation of the tree may lead to a crash/
//  // only the leaves need to be processed-> set to "false".
//  parsetree2xmltreetraverser.m_bTraverseTree = false ;

  parsetree2xmltreetraverser.Traverse() ;
  std_strXML += parsetree2xmltreetraverser.m_std_strXML;
}

void GenerateXMLtreeFromParseTree(ParseByRise * p_parsebyrise,
  std::string & std_strXML)
{
  std::vector<GrammarPart *>
    stdvec_p_grammarpartCoveringMostTokensAtTokenIndex ;
//  std::string std_strXML;
  if( p_parsebyrise )
  {
    DWORD dwLeftMostTokenIndex = 0 ;
//    GrammarPart * p_grammarpart ;
//    GrammarPart * p_grammarpartChild ;
    //int y = 10 ;
    //  typedef std::multimap<DWORD, GrammarPart >
    //  stdmmap_token_index2grammarpart ;
    typedef std::multimap<DWORD, GrammarPart *>
      stdmmap_token_index2grammarpart ;
    stdmmap_token_index2grammarpart::const_iterator citer ;
    stdmmap_token_index2grammarpart &
      r_stdmultimap_dwLeftmostIndex2grammarpart = p_parsebyrise->
      //m_stdmultimap_dwLeftmostIndex2grammarpart ;
      m_stdmultimap_dwLeftmostIndex2p_grammarpart ;
    std_strXML += "<translated_text>"; //Topmost XML element.
//    //Reset to initial before each translation.
//    m_ui32ConcatenationID = 1;
    do
    {
//      //Before each draw in order to begin at x position "0".
//      m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.clear() ;
//      m_wParseLevel = 0 ;
      DEBUG_COUT( "Translate(): p_parsebyrise != NULL\n")
      std_strXML += "<translations token_index=\"" +
        ::convertToStdString(dwLeftMostTokenIndex) +
        "\">";
      citer = r_stdmultimap_dwLeftmostIndex2grammarpart.begin() ;
      //p_grammarpart =
        p_parsebyrise->GetGrammarPartCoveringMostTokens(
          dwLeftMostTokenIndex ,
          stdvec_p_grammarpartCoveringMostTokensAtTokenIndex
          ) ;

#ifdef _DEBUG
      WORD wSize = stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.size();
#endif //#ifdef _DEBUG
//      WORD wConsecutiveID = 0 ;

      //Loop over all parse trees beginning at token index
      //"dwLeftMostTokenIndex".
      for( std::vector<GrammarPart *>::const_iterator
          c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex =
          stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.begin() ;
          c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex <
          stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.end() ;
          ++ c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex
         )
      {
        std_strXML += "<translation>";
        GenerateXMLtreeFromParseTree(
          c_iter_p_grammarpartParseTreeRootCoveringMostTokensAtTokenIndex,
          p_parsebyrise,
          std_strXML);
        std_strXML += "</translation>";
      } //loop for all parse trees beginning at same token index
      std_strXML += "</translations>";

      if( stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.empty() )
        dwLeftMostTokenIndex = 0;
      else
        dwLeftMostTokenIndex =
          stdvec_p_grammarpartCoveringMostTokensAtTokenIndex.at(0)->
          m_dwRightmostIndex + 1;
      LOGN("TranslateParseByRiseTree::Translate(...)--dwLeftMostTokenIndex:"
        << dwLeftMostTokenIndex )
    }
    while( dwLeftMostTokenIndex );
    std_strXML += "</translated_text>"; //Topmost XML element.
  }//if( p_parsebyrise )
}
