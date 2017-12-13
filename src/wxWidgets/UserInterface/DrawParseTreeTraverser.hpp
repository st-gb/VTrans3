/*
 * DrawParseTreeTraverser.h
 *
 *  Created on: May 12, 2010
 *      Author: Stefan
 */

#ifndef DRAWPARSETREETRAVERSER_H_
#define DRAWPARSETREETRAVERSER_H_

//class DrawGrammarPartAttributes
#include "DrawGrammarPartAttributes.hpp"
#include <Attributes/GrammarPartLocationWithinWindow.hpp>
//base class ParseTreeTraverser::DirectingLeavesMultipleIterTraverser
#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>
#include <map> //class std::map
//#include <wx/dc.h>
#include <wx/gdicmn.h> //class wxSize
#include <hardware/CPU/fastest_data_type.h> //typedef fastestUnsignedDataType

//Forward declarations
class GrammarPart ;
class wxDC ;

class DrawParseTreeTraverser
  : public ParseTreeTraverser::DirectingLeavesMultipleIterTraverser
{
public:
  BottomUpParser * mp_parsebyrise ;
  wxColor m_wxcolor;
  fastestUnsignedDataType m_stringHeigth;
  fastestUnsignedDataType m_currentParseTreeLeftEndInPixels ;
  //TODO make type fastestUnsignedDataType
  WORD m_wParseLevelCountedFromRoot ;
  wxDC * mp_wxdc ;
  DrawParseTreeTraverser(
    wxDC * p_wxdcm ,
    BottomUpParser *,
    GrammarPart * p_grammarpart
    );
  virtual
  ~DrawParseTreeTraverser();
  std::map<GrammarPart *,GrammarPartLocationWithinWindow>
    m_stdmap_p_grammarpart2HorizCenter;

  inline fastestUnsignedDataType DrawTextAndAddXpos(
    const wxString & wxstr,
    fastestUnsignedDataType leftEndInPixels, 
    const fastestUnsignedDataType stringHeightInPixels,
    const bool doDraw = true,
    const wxColor & wxcolor = *wxBLACK
    );
  fastestUnsignedDataType DrawGrammarPartAtts(
    const DrawGrammarPartAttributes & drawGrammarPartAttributes,
    /*const*/ fastestUnsignedDataType leftEndInPixels,
    const fastestUnsignedDataType stringHeight);
  wxSize GetGrammarPartNameExtent(
  //  wxPaintDC & wxpaintdc ,
    wxDC & r_wxdc ,
    GrammarPart * p_pg ,
    //wxSize & wxsizeText
//    wxString & wxstr,
//    wxString & wxstrGrammarPartMemoryAddress
    DrawGrammarPartAttributes & drawGrammarPartAttributes
    ) ;
  wxSize GetSourceTextTokenExtent(
      wxDC & r_wxdc ,
      GrammarPart * p_pg ,
  //    wxSize & wxsizeText
      wxString & wxstr
      ) ;
  fastestUnsignedDataType GetWidthInPixels(const wxString & wxstr) const;
  void BeforeBeginAtRoot() ;
//  {
//    //Important. else pathes with previous node(s) (->too long) are created.
////    m_vecGrammarPartID.clear() ;
////    m_stdvec_p_grammarpartPath.clear() ;
//  }
  void CurrentNodeIsLastAddedRightChild() ;
  fastestUnsignedDataType DrawRectangle(const fastestUnsignedDataType widthOfLeafInPixels);
  void LeaveFound() ;
  void ParseTreePathAdded() ;
//    void RightChildAdded(WORD wCurrentParseTreeLevel) ;
  void UnprocessedHighestLevelNodeFound() ;

private:
  void
  DrawLineFromRightChildToParent(
    fastestUnsignedDataType & middleBetweenLeftAndRightChild,
    std::map<GrammarPart*, GrammarPartLocationWithinWindow>::iterator& iterLeft,
    std::map<GrammarPart*, GrammarPartLocationWithinWindow>::iterator& iterRight,
    fastestUnsignedDataType & childY,
    GrammarPartLocationWithinWindow* p_dgpaRight,
//    wxSize& wxsizeString,
    fastestUnsignedDataType & thisY);
};

#endif /* DRAWPARSETREETRAVERSER_H_ */
