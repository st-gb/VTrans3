/*
 * wxParseTreePanel.cpp
 *
 *  Created on: 22.02.2010
 *      Author: Stefan
 */

#include <wxWidgets/wxParseTreePanel.h>

BEGIN_EVENT_TABLE(wxParseTreePanel, wxPanel)
  EVT_PAINT  (wxParseTreePanel::OnPaint)
END_EVENT_TABLE()

//wxParseTreePanel::wxParseTreePanel() {
//	// TODO Auto-generated constructor stub
//
//}

wxParseTreePanel::wxParseTreePanel(
    wxWindow* parent,
     wxWindowID id //= wxID_ANY
     , const wxPoint& pos //= wxDefaultPosition
     , const wxSize& size //= wxDefaultSize
     , long style //= wxTAB_TRAVERSAL,
     , const wxString& name //= "panel"
  )
  :wxPanel(
      parent,
      id,
      pos,
      size,
      style ,
      name )
{
//  Connect(wxEVT_PAINT, wxPaintEventHandler(wxParseTreePanel::OnPaint));
}

wxParseTreePanel::wxParseTreePanel( wxWindow* parent )
  : wxPanel(parent)
{

}

wxParseTreePanel::~wxParseTreePanel() {
	// TODO Auto-generated destructor stub
}

void wxParseTreePanel::OnPaint(wxPaintEvent & event)
{

  wxPaintDC wxpaintdc(this);
//  PrepareDC(wxpaintdc);
  wxpaintdc.DrawLine(wxPoint(10,10),wxPoint(199,100)) ;
  wxpaintdc.SetBackground(*wxBLUE_BRUSH);
}
