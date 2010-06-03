/*
 * SetSameConsecutiveIDforLeaves.cpp
 *
 *  Created on: Jun 2, 2010
 *      Author: Stefan
 */

#include <Translate/SetSameConsecutiveIDforLeaves.hpp>
#include <Parse/GrammarPart.hpp>

namespace ParseTreeTraverser
{

  SetSameConsecutiveIDforLeaves::SetSameConsecutiveIDforLeaves(
    const GrammarPart * p_grammarpartStartNode ,
    ParseByRise * p_parsebyrise ,
    WORD wConsecutiveID
    )
    : ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
      p_grammarpartStartNode ,
      p_parsebyrise
      )
  {
    m_wConsecutiveID = wConsecutiveID ;
  }

  SetSameConsecutiveIDforLeaves::~SetSameConsecutiveIDforLeaves()
  {
  }

  void SetSameConsecutiveIDforLeaves::LeaveFound()
  {
    m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
      m_wConsecutiveID = m_wConsecutiveID ;

  }
}
