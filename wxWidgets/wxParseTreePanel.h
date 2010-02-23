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
  :public wxPanel
{
    WORD m_wParseLevel ;
    ParseByRise * mp_parsebyrise ;
    std::map<WORD,DWORD>
      m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName ;
    std::vector<GrammarPart *> m_stdvecNodesToProcess ;
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
	void DrawGrammarPartName(GrammarPart * p_pg ,
	    wxPaintDC & wxpaintdc ) ;
	void DrawParseTree( ParseByRise & r_parsebyrise ) ;
	void OnPaint(wxPaintEvent & event) ;
	DECLARE_EVENT_TABLE()
};

#endif /* WXPARSETREEPANEL_H_ */
