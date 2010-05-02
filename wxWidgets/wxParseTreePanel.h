/*
 * wxParseTreePanel.h
 *
 *  Created on: 22.02.2010
 *      Author: Stefan
 */

#ifndef WXPARSETREEPANEL_H_
#define WXPARSETREEPANEL_H_

#include <wx/panel.h>
#include <wx/dcclient.h> //for class wxPaintDC
#include <map> //std::map
#include <vector> //std::vector
#include <windef.h> //for WORD

class ParseByRise ;
class GrammarPart ;

class wxParseTreePanel
  : public wxPanel
{
    WORD m_wParseLevel ;
    ParseByRise * mp_parsebyrise ;
    std::map<WORD,DWORD>
      m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName ;
    std::map<GrammarPart *,WORD> m_stdmap_p_grammarpart2wCenter ;
    std::vector<GrammarPart *> m_stdvecNodesToProcess ;
    std::map<GrammarPart *,WORD> m_stdmap_p_grammarpart2HorizCenter;
public:
//	wxParseTreePanel();
  wxParseTreePanel(
      wxWindow* parent
    ) ;
	wxParseTreePanel(
	  wxWindow* parent,
	  wxWindowID id = wxID_ANY,
	  const wxPoint& pos = wxDefaultPosition,
	  const wxSize& size = wxDefaultSize,
	  long style = wxTAB_TRAVERSAL,
	  const wxString& name = "panel"
    ) ;
//	wxParseTreePanel(wxWindow* parent ) ;
	virtual ~wxParseTreePanel();
	void DrawGrammarPartChildren( GrammarPart * p_pg , wxPaintDC & wxpaintdc ) ;
	void DrawGrammarPartParentToChildLine(
	  WORD wParentHorizTextCenter, WORD wChildHorizTextCenter
	  , wxPaintDC & r_wxpaintdc ) ;
  void DrawGrammarPartName(
      GrammarPart * p_pg ,
      wxPaintDC & wxpaintdc ,
      WORD & wHorizTextCenter
      ) ;
	void DrawGrammarPartNameAndPossiblyToken(
	    GrammarPart * p_pg ,
	    wxPaintDC & wxpaintdc ,
	    WORD & wHorizTextCenter
	    ) ;
	void DrawGrammarPartToken(
	    GrammarPart * p_pg ,
	    wxPaintDC & wxpaintdc ,
	    WORD & r_wHorizTextCenter
	    ) ;
	void DrawLeavesOfParseTree( wxPaintDC & wxpaintdc ,
//	    std::map<GrammarPart *,WORD> & map_p_grammarpart2HorizCenter
	    GrammarPart * p_grammarpart
	    ) ;
	void DrawNextParseTreeLevelDirectingRoot(
//	  wxPaintDC & wxpaintdc ,
    wxDC & r_wxdc ,
	  WORD wParseLevel ,
	  GrammarPart * p_grammarpart
	  ) ;
	void DrawParseTree( ParseByRise & r_parsebyrise ) ;
  void DrawParseTreeBeginningFromLeaves(wxPaintDC & wxpaintdc ) ;
	void DrawParseTreeBeginningFromRoots(wxPaintDC & wxpaintdc ) ;
	GrammarPart * GetNextRightGrammarPartNotProcessedYet() ;
	//void
	wxSize GetTokenExtent(
	    wxPaintDC & wxpaintdc ,
	    GrammarPart * p_pg ,
	    //wxSize & wxsizeText
      wxString & wxstr
	    ) ;
  //void
	wxSize GetGrammarPartNameExtent(
    //wxPaintDC & wxpaintdc ,
    wxDC & r_wxdc ,
    GrammarPart * p_pg ,
    //wxSize & wxsizeText
    wxString & wxstr
    ) ;
	void OnPaint(wxPaintEvent & event) ;
	DECLARE_EVENT_TABLE()
};

#endif /* WXPARSETREEPANEL_H_ */
