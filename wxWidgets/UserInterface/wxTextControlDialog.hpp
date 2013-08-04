/*
 * wxTextControlDialog.hpp
 *
 *  Created on: 11.08.2011
 *      Author: Stefan
 */

#ifndef WXTEXTCONTROLDIALOG_HPP_
#define WXTEXTCONTROLDIALOG_HPP_

//#include <bitmaps/VT_icon.xpm> // array "VT_icon_xpm"
#include <wx/dialog.h>
//SUPPRESS_UNUSED_VARIABLE_WARNING
#include <preprocessor_macros/suppress_unused_variable.h>

class wxTextControlDialog
  : public wxDialog
{
public:
    wxTextControlDialog(const wxString & c_r_wxstrMessage)
      : wxDialog(
          NULL,
          wxID_ANY,
          wxT("Translator message"), //const wxString& title
          wxDefaultPosition, //const wxPoint& pos = wxDefaultPosition,
          wxDefaultSize, //const wxSize& size = wxDefaultSize,
          wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
          )
    {
      //SetIcon( ::wxGetApp().m_wxiconVTrans ) ;
      wxTextCtrl * p_wxtextctrl = new wxTextCtrl(
        this //wxWindow* parent
        , wxID_ANY //wxWindowID id
        , c_r_wxstrMessage //const wxString& value = ""
        , wxDefaultPosition //const wxPoint& pos = wxDefaultPosition,
        , wxDefaultSize //const wxSize& size = wxDefaultSize
        , wxTE_READONLY | wxTE_MULTILINE //long style = 0
        );
      SUPPRESS_UNUSED_VARIABLE_WARNING(p_wxtextctrl)
      //  wxd.AddChild( p_wxtextctrl);
      Layout(); //stretch to the whole client window.
    }
};

#endif /* WXTEXTCONTROLDIALOG_HPP_ */
