/*
 * DrawGrammarPartAttributes.hpp
 *
 *  Created on: May 12, 2010
 *      Author: Stefan
 */

#ifndef DRAWGRAMMARPARTATTRIBUTES_HPP_
#define DRAWGRAMMARPARTATTRIBUTES_HPP_

#include <windef.h> //WORD

class DrawGrammarPartAttributes
{
public:
  DrawGrammarPartAttributes(
    WORD wHorizCenterInPixels ,
    WORD wParseLevel
    )
  {
    m_wHorizCenterInPixels = wHorizCenterInPixels ;
    m_wParseLevel = wParseLevel ;
  }
  WORD m_wParseLevel ;
  WORD m_wHorizCenterInPixels ;
};

#endif /* DRAWGRAMMARPARTATTRIBUTES_HPP_ */
