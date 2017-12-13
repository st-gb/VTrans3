/** File:   GUIattributes.hpp
 * Author: sg
 * Created on 7. November 2017, 22:08  */

#ifndef GUIATTRIBUTES_HPP
#define GUIATTRIBUTES_HPP

#include <hardware/CPU/fastest_data_type.h> //fastestUnsignedDataType

class GUIattributes
{
public:
  bool m_bShowGrammarPartAddress ;
  bool m_doubleBufferParseTreePanel;
  bool m_bShowTranslation ;
  bool m_translateOnTextChanges;
  fastestUnsignedDataType m_concatenationIDcolour;
  fastestUnsignedDataType m_fontSizeInPoint;
  fastestUnsignedDataType m_minFontSizeInPoint;
  std::string m_std_strGrammarPartIDcolor;
  std::string m_std_strConcatenationIDcolor;
  std::string m_std_strGrammarPartMemoryAddressColor;
  
  GUIattributes()
    : m_bShowGrammarPartAddress( true /*false*/ )
      , m_doubleBufferParseTreePanel(false)
      , m_bShowTranslation(false)
      , m_translateOnTextChanges(false)
      , m_std_strGrammarPartIDcolor("#FF0000")
      , m_std_strConcatenationIDcolor("#0000FF")
      , m_std_strGrammarPartMemoryAddressColor("#FF0000")
      , m_fontSizeInPoint(8)
      , m_minFontSizeInPoint(5)
      , m_concatenationIDcolour(255)
  {}
  
  void SetConcatenationIDcolour(fastestUnsignedDataType concatenationIDcolour)
  {
    m_concatenationIDcolour = concatenationIDcolour;
  }
  
  void SetPointSizeOfParseTreePanel(const fastestUnsignedDataType fontSizeInPoint) {
    m_fontSizeInPoint = fontSizeInPoint;
  }
  void SetMinFontSizeInPointOfParseTreePanel(
    const fastestUnsignedDataType fontSizeInPoint) {
    m_minFontSizeInPoint = fontSizeInPoint;
  }
};

#endif /* GUIATTRIBUTES_HPP */
