/*
 * ParseTree2XMLtreeTraverser.cpp
 *
 *  Created on: 24.09.2011
 *      Author: Stefan
 */

#include <Controller/character_string/stdstring_format.hpp>
#include "IO/ParseTree2XMLtreeTraverser.hpp"
#include <Parse/GrammarPart.hpp> //class GrammarPart
#include <Parse/ParseByRise.hpp> //class ParseByRise
#include <Controller/character_string/ISO_8859_1.hpp>

namespace ParseTreeTraverser
{

  ParseTree2XMLtreeTraverser::ParseTree2XMLtreeTraverser(
    const GrammarPart * p_grammarpartStartNode
    , ParseByRise & r_parsebyrise
    )
    :
//     ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
     ParseTreeTraverser::DirectingLeavesInSingleIterationTraverser(
      p_grammarpartStartNode ,
      & r_parsebyrise
      )
  {
    // TODO Auto-generated constructor stub

  }

  ParseTree2XMLtreeTraverser::~ParseTree2XMLtreeTraverser()
  {
    // TODO Auto-generated destructor stub
  }

  void ParseTree2XMLtreeTraverser::CurrentNodeIsLastAddedRightChild()
  {
    //http://www.cplusplus.com/reference/stl/vector/resize/:
    //"If sz is smaller than the current vector size, the content is reduced
    //to its first sz elements, the rest being dropped."

    //The last added right child may have more than 1 level difference to
    //the last left child:
    //
    // "he"
    //  |
    //  he                 "is"
    //  \                   |
    //  personal_pronoun  be_3rd_person_singular
    //                \  /
    //            adverbial_clause
    m_std_vec_p_grammarpart_and_parselevelCurrentParseTreePath.resize(
      //if the right node was at parse level 1 (2nd level), then 1 element
      //should remain.
      m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel ) ;

    m_byteArray.add("<grammar_part name=\"");
      /** Grammar part names can't be XML element names because they (may)
      * contain numbers/ digits. So make it an attribute value." */
    m_byteArray.add( mp_parsebyrise->GetGrammarPartName(
      m_grammarpartpointer_and_parselevelCurrent.
      m_p_grammarpart->m_wGrammarPartID).c_str() );
    m_byteArray.add("\">");
//    m_byteArray.add(std_strXML.c_str(), std_strXML.length() );

    m_std_vec_p_grammarpart_and_parselevelCurrentParseTreePath.push_back(
      m_grammarpartpointer_and_parselevelCurrent ) ;
  }

//  void ParseTree2XMLtreeTraverser::AddToUTF8sequence(
//    const BYTE * const arby, fastestUnsignedDataType utf8arraySizeInByte)
//  {
////    m_pchUTF8_XML
//    if( utf8arraySizeInByte == 0)
//      utf8arraySizeInByte = ::strlen( arby);
//    m_byteArray.add(arby, utf8arraySizeInByte);
//  }

  void ParseTree2XMLtreeTraverser::LeaveFound()
  {
    GrammarPart * p_grammarpart = m_grammarpartpointer_and_parselevelCurrent.
      m_p_grammarpart;
    const std::string & stdstrTranslation = p_grammarpart->m_stdstrTranslation;
  #ifdef _DEBUG
    const char * strTranslation = stdstrTranslation.c_str();
  #endif

    fastestUnsignedDataType utf8arraySizeInByte;
    const BYTE * arbyTranslationAsUTF8 = ISO_8859_1::GetAsUTF8(
      stdstrTranslation.c_str(), utf8arraySizeInByte);
//    std::string std_strTranslationInUTF8 = std::string( (const char *) arbyTranslationAsUTF8);
    m_byteArray.add("<word translation=\"");
//    AddToUTF8sequence(std_str.c_str(), std_str.length() );
    //    AddToUTF8sequence(arbyTranslationAsUTF8, utf8arraySizeInByte);
    m_byteArray.add(arbyTranslationAsUTF8, utf8arraySizeInByte);
    delete [] arbyTranslationAsUTF8;
    m_byteArray.add("\" concatenation_ID=\"");
    m_byteArray.add( ::convertToStdString(
      p_grammarpart->m_ui32ConcatenationID).c_str() );
    m_byteArray.add("\"/>");
//    m_byteArray.add(std_str.c_str(), std_str.length() );
//    delete [] arbyTranslationAsUTF8;
  }
  void ParseTree2XMLtreeTraverser::ParseTreePathAdded()
  {
    m_byteArray.add("<grammar_part name=\"");
      //Grammar part names can't be XML element names because they (may)
      //contain numbers/ digits. So make it an attribute value.
    m_byteArray.add(mp_parsebyrise->GetGrammarPartName(
      m_grammarpartpointer_and_parselevelCurrent.
      m_p_grammarpart->m_wGrammarPartID).c_str() );
    m_byteArray.add("\">");
//    m_byteArray.add(std_strXML.c_str(), std_strXML.length() );

    m_std_vec_p_grammarpart_and_parselevelCurrentParseTreePath.push_back(
      m_grammarpartpointer_and_parselevelCurrent);
  }
  void ParseTree2XMLtreeTraverser::ParseTreePathPopped()
  {
//    GrammarPartPointerAndParseLevel & r_grammarpartpointerandparselevel =
    m_std_vec_p_grammarpart_and_parselevelCurrentParseTreePath.back();
    m_byteArray.add( //"</" + mp_parsebyrise->GetGrammarPartName(
      //r_grammarpartpointerandparselevel.
      //m_p_grammarpart->m_wGrammarPartID) + ">";
      "</grammar_part>");
//    m_byteArray.add(std_strXML.c_str(), std_strXML.length() );

    m_std_vec_p_grammarpart_and_parselevelCurrentParseTreePath.pop_back() ;
  }
} /* namespace ParseTreeTraverser */
