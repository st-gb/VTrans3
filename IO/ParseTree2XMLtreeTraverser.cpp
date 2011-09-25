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
    m_std_strXML += "<" + mp_parsebyrise->GetGrammarPartName(
      m_grammarpartpointer_and_parselevelCurrent.
      m_p_grammarpart->m_wGrammarPartID) + ">";
    m_std_vec_p_grammarpart_and_parselevelCurrentParseTreePath.push_back(
      m_grammarpartpointer_and_parselevelCurrent ) ;
  }
  void ParseTree2XMLtreeTraverser::LeaveFound()
  {
    GrammarPart * p_grammarpart = m_grammarpartpointer_and_parselevelCurrent.
      m_p_grammarpart;
    m_std_strXML += "<word"
      " translation=\"" + p_grammarpart->m_stdstrTranslation + "\""
      + " concatenation_ID=\"" + ::convertToStdString(p_grammarpart->
        m_ui32ConcatenationID) + "\""
      "/>";
  }
  void ParseTree2XMLtreeTraverser::ParseTreePathAdded()
  {
    m_std_strXML += "<" + mp_parsebyrise->GetGrammarPartName(
      m_grammarpartpointer_and_parselevelCurrent.
      m_p_grammarpart->m_wGrammarPartID) + ">";
    m_std_vec_p_grammarpart_and_parselevelCurrentParseTreePath.push_back(
      m_grammarpartpointer_and_parselevelCurrent);
  }
  void ParseTree2XMLtreeTraverser::ParseTreePathPopped()
  {
    GrammarPartPointerAndParseLevel & r_grammarpartpointerandparselevel =
      m_std_vec_p_grammarpart_and_parselevelCurrentParseTreePath.back();
    m_std_strXML += "</" + mp_parsebyrise->GetGrammarPartName(
      r_grammarpartpointerandparselevel.
      m_p_grammarpart->m_wGrammarPartID) + ">";
    m_std_vec_p_grammarpart_and_parselevelCurrentParseTreePath.pop_back() ;
  }
} /* namespace ParseTreeTraverser */
