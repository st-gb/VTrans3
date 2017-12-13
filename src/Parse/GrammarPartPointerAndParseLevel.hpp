/*
 * GrammarPartPointerAndParseLevel.hpp
 *
 *  Created on: 25.09.2011
 *      Author: Stefan
 */

#ifndef GRAMMARPARTPOINTERANDPARSELEVEL_HPP_
#define GRAMMARPARTPOINTERANDPARSELEVEL_HPP_
#include <stddef.h> //NULL

class GrammarPart ;
namespace VTrans3 { 
  class BottomUpParser;
}
typedef unsigned short WORD ;

class GrammarPartPointerAndParseLevel
{
public:
  GrammarPart * m_p_grammarpart ;
  WORD m_wParseLevel ;
  GrammarPartPointerAndParseLevel()
    : m_p_grammarpart( NULL )
    , m_wParseLevel( 0 )
  {

  }
  GrammarPartPointerAndParseLevel(
    const GrammarPart * p_grammarpart ,
    WORD wParseLevel
    )
    : m_p_grammarpart( (GrammarPart *) p_grammarpart )
    , m_wParseLevel( wParseLevel )
  {

  }
  void SetGrammarPartPointerAndParseLevel(
    GrammarPart * p_grammarpart ,
    WORD wParseLevel
    )
  {
    m_p_grammarpart = p_grammarpart ;
    m_wParseLevel = wParseLevel ;
  }
} ;

#endif /* GRAMMARPARTPOINTERANDPARSELEVEL_HPP_ */
