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

class wxParseTreePanel
  :public wxPanel
{
public:
//	wxParseTreePanel();
	wxParseTreePanel(
	  wxWindow* parent,
	  wxWindowID id = wxID_ANY,
	  const wxPoint& pos = wxDefaultPosition,
	  const wxSize& size = wxDefaultSize,
	  long style = wxTAB_TRAVERSAL,
	  const wxString& name = "panel"
    ) ;
	wxParseTreePanel(wxWindow* parent ) ;
	virtual ~wxParseTreePanel();
	void OnPaint(wxPaintEvent & event) ;
	DECLARE_EVENT_TABLE()
};

#endif /* WXPARSETREEPANEL_H_ */
