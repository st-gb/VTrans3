/*
 * DrawParseTreeTraverser.h
 *
 *  Created on: May 12, 2010
 *      Author: Stefan
 */

#ifndef DRAWPARSETREETRAVERSER_H_
#define DRAWPARSETREETRAVERSER_H_

//class DrawGrammarPartAttributes
#include <Attributes/DrawGrammarPartAttributes.hpp>
//base class ParseTreeTraverser::DirectingLeavesMultipleIterTraverser
#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>
#include <map> //class std::map
//#include <wx/dc.h>
#include <wx/gdicmn.h> //class wxSize

//Forward declarations
class GrammarPart ;
class wxDC ;

class DrawParseTreeTraverser
  : public ParseTreeTraverser::DirectingLeavesMultipleIterTraverser
{
public:
  bool m_bShowGrammarPartAddress ;
  ParseByRise * mp_parsebyrise ;
  WORD m_wCurrentParseTreeLeftEndInPixels ;
  WORD m_wParseLevelCountedFromRoot ;
  wxDC * mp_wxdc ;
  DrawParseTreeTraverser(
    wxDC * p_wxdcm ,
    ParseByRise * p_parsebyrise ,
    GrammarPart * p_grammarpart
    );
  virtual
  ~DrawParseTreeTraverser();
  std::map<GrammarPart *,DrawGrammarPartAttributes>
    m_stdmap_p_grammarpart2HorizCenter;

  wxSize GetGrammarPartNameExtent(
  //  wxPaintDC & wxpaintdc ,
    wxDC & r_wxdc ,
    GrammarPart * p_pg ,
    //wxSize & wxsizeText
    wxString & wxstr
    ) ;
  wxSize GetTokenExtent(
      wxDC & r_wxdc ,
      GrammarPart * p_pg ,
  //    wxSize & wxsizeText
      wxString & wxstr
      ) ;
  void BeforeBeginAtRoot() ;
//  {
//    //Important. else pathes with previous node(s) (->too long) are created.
////    m_vecGrammarPartID.clear() ;
////    m_stdvec_p_grammarpartPath.clear() ;
//  }
  void CurrentNodeIsLastAddedRightChild() ;
  void LeaveFound() ;
  void ParseTreePathAdded() ;
//    void RightChildAdded(WORD wCurrentParseTreeLevel) ;
  void UnprocessedHighestLevelNodeFound() ;
};

#endif /* DRAWPARSETREETRAVERSER_H_ */
