/*
 * wxParseTreePanel.h
 *
 *  Created on: 22.02.2010
 *      Author: Stefan
 */

#ifndef WXPARSETREEPANEL_H_
#define WXPARSETREEPANEL_H_

#ifdef _WIN32
//http://www.wxwidgets.org/docs/faqmsw.htm#asuffix:
//"If you get errors like no matching function for call to
//'wxDC::DrawTextA(const char[5], int, int)' or similar ones for the other
//functions, i.e. the compiler error messages mention the function with the
//'A' suffix while you didn't use it in your code, the explanation is that you
//had included <windows.h> header which redefines many symbols to have such
//suffix (or 'W' in the Unicode builds).
//
//The fix is to either not include <windows.h> at all or include
//"wx/msw/winundef.h" immediately after it. "
//If not included: ~"undefined reference to wxWindow::[...] in DEBUG version"
  #include "wx/msw/winundef.h"
#endif //#ifdef _WIN32
#include <wx/panel.h> //base class wxPanel
#include <wx/dcmemory.h> //class wxMemoryDC
#include <wx/gdicmn.h> //class wxSize
//#include <wx/dcclient.h> //for class wxPaintDC
#include <map> //class std::map
#include <vector> //class std::vector
#include <windef.h> //for WORD
////class DrawGrammarPartAttributes
//#include <Attributes/DrawGrammarPartAttributes.hpp>
#include <fastest_data_type.h> //typedef fastestUnsignedDataType

//Forward declarations (faster than #include)
class GrammarPartLocationWithinWindow ;
class GrammarPart ;
class ParseByRise ;
class wxBitmap;
class wxDC ;

class wxParseTreePanel
  : public wxPanel
{
  fastestUnsignedDataType m_pointSizeOfFont;
    WORD m_wParseLevel ;
    wxSize m_wxsizeClientRect;
    ParseByRise * mp_parsebyrise ;
    wxBitmap * m_p_wxbitmapBuffer;
    wxBitmap m_wxbitmapBuffer;
    wxMemoryDC m_wxmemorydc;
    std::map<WORD,DWORD>
      m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName ;
    std::map<GrammarPart *,WORD> m_stdmap_p_grammarpart2wCenter ;
    std::vector<GrammarPart *> m_stdvecNodesToProcess ;
    //std::map<GrammarPart *,WORD> m_stdmap_p_grammarpart2HorizCenter;
    std::map<GrammarPart *,GrammarPartLocationWithinWindow>
      m_stdmap_p_grammarpart2HorizCenter;
public:
//	wxParseTreePanel();
  wxParseTreePanel(
      wxWindow* parent
    ) ;
	wxParseTreePanel(
	  wxWindow * parent,
	  wxWindowID id = wxID_ANY,
	  const wxPoint& pos = wxDefaultPosition,
	  const wxSize& size = wxDefaultSize,
	  long style = wxTAB_TRAVERSAL,
	  const wxString & name = wxT("panel")
    ) ;
//	wxParseTreePanel(wxWindow* parent ) ;
	virtual ~wxParseTreePanel();
  
  void DecreaseFontSizeBy1Point();
  void IncreaseFontSizeBy1Point();
	void DrawGrammarPartChildren( GrammarPart * p_pg ,
	  //Use base class wxDC, so subclasses of it can be used as actual parameters.
	  //This allows e.g. to use either a wxMemoryDC (-> no flicker) or a
	  //wxPaintDC.
	  wxDC & r_wxdc ) ;
	void DrawGrammarPartParentToChildLine(
	  WORD wParentHorizTextCenter,
	  WORD wChildHorizTextCenter
    //Use base class wxDC, so subclasses of it can be used as actual parameters.
    //This allows e.g. to use either a wxMemoryDC (-> no flicker) or a
    //wxPaintDC.
	  , wxDC & r_wxdc ) ;
  void DrawGrammarPartName(
      GrammarPart * p_pg ,
      //Use base class wxDC, so subclasses of it can be used as actual parameters.
      //This allows e.g. to use either a wxMemoryDC (-> no flicker) or a
      //wxPaintDC (original DC for OnPaint(), may flicker ).
      wxDC & r_wxdc ,
      WORD & wHorizTextCenter
      ) ;
	void DrawGrammarPartNameAndPossiblyToken(
	    GrammarPart * p_pg ,
      //Use base class wxDC, so subclasses of it can be used as actual parameters.
      //This allows e.g. to use either a wxMemoryDC (-> no flicker) or a
      //wxPaintDC (original DC for OnPaint(), may flicker ).
	    wxDC & r_wxdc ,
	    WORD & wHorizTextCenter
	    ) ;
	void DrawGrammarPartToken(
	    GrammarPart * p_pg ,
	    //Use base class wxDC, so subclasses of it can be used as actual parameters.
	    //This allows e.g. to use either a wxMemoryDC (-> no flicker) or a
	    //wxPaintDC (original DC for OnPaint(), may flicker ).
	    wxDC & r_wxdc ,
	    WORD & r_wHorizTextCenter
	    ) ;
	WORD DrawLeavesOfParseTree(
	    //Use base class wxDC, so subclasses of it can be used as actual parameters.
	    //This allows e.g. to use either a wxMemoryDC (-> no flicker) or a
	    //wxPaintDC (original DC for OnPaint(), may flicker ).
	    wxDC & r_wxdc ,
//	    std::map<GrammarPart *,WORD> & map_p_grammarpart2HorizCenter
	    GrammarPart * p_grammarpart ,
	    WORD wLeftEndOfLeftmostTokenInPixels
	    ) ;
	void DrawNextParseTreeLevelDirectingRoot(
//	  wxPaintDC & wxpaintdc ,
	    //Use base class wxDC, so subclasses of it can be used as actual parameters.
	    //This allows e.g. to use either a wxMemoryDC (-> no flicker) or a
	    //wxPaintDC (original DC for OnPaint(), may flicker ).
    wxDC & r_wxdc ,
	  WORD wParseLevel ,
	  GrammarPart * p_grammarpart
	  ) ;
	void DrawParseTree( ParseByRise & r_parsebyrise
            /*wxDC & r_wxdc*/) ;
  void DrawParseTreeBeginningFromLeaves(
    //Use base class wxDC, so subclasses of it can be used as actual parameters.
    //This allows e.g. to use either a wxMemoryDC (-> no flicker) or a
    //wxPaintDC (original DC for OnPaint(), may flicker ).
    wxDC & r_wxdc ) ;
  void DrawParseTreeBeginningFromLeavesWithClientDC();
	void DrawParseTreeBeginningFromRoots(
    //Use base class wxDC, so subclasses of it can be used as actual parameters.
    //This allows e.g. to use either a wxMemoryDC (-> no flicker) or a
    //wxPaintDC (original DC for OnPaint(), may flicker ).
    wxDC & r_wxdc ) ;
	GrammarPart * GetNextRightGrammarPartNotProcessedYet() ;
	//void
	wxSize GetSourceTextTokenExtent(
    //Use base class wxDC, so subclasses of it can be used as actual parameters.
    //This allows e.g. to use either a wxMemoryDC (-> no flicker) or a
    //wxPaintDC (original DC for OnPaint(), may flicker ).
    wxDC & r_wxdc ,
    GrammarPart * p_pg ,
    //wxSize & wxsizeText
    wxString & wxstr
    ) ;
  //void
	wxSize GetGrammarPartNameExtent(
    //wxPaintDC & wxpaintdc ,
    //Use base class wxDC, so subclasses of it can be used as actual parameters.
    //This allows e.g. to use either a wxMemoryDC (-> no flicker) or a
    //wxPaintDC (original DC for OnPaint(), may flicker ).
    wxDC & r_wxdc ,
    GrammarPart * p_pg ,
    //wxSize & wxsizeText
    wxString & wxstr
    ) ;
  void OnContextMenu(wxContextMenuEvent & evt);
	void OnPaint(wxPaintEvent & event) ;
	void OnSize(wxSizeEvent & evt);
  void OnPopupClick(wxCommandEvent & evt);
  void OnContextMenuMouseButtonDown(wxMouseEvent & evt);
	DECLARE_EVENT_TABLE()
};

#endif /* WXPARSETREEPANEL_H_ */
