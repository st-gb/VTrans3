/*
 * MainFrame.hpp
 *
 *  Created on: Sep 28, 2010
 *      Author: Stefan
 */

#ifndef MAINFRAME_HPP_
#define MAINFRAME_HPP_

#include <wx/frame.h> //for base class wxFrame

#include <Parse/ParseByRise.hpp> //class ParseByRise
#include "MainWindowBase.hpp"

//Forward declarations (faster than #include)
class wxMenuBar ;
class wxParseTreePanel ;
class wxSplitterWindow ;
class wxToolBar ;

namespace wxWidgets
{
  class MainFrame
    : public wxFrame, public MainWindowBase
  {
    wxMenuBar * mp_wxmenubar ;
    wxToolBar * mp_wxtoolbar ;
    void AddMenuBar() ;
    void AddInputAndOutputControls();
    void AddBitmapButtonAndTooltip(
      enum user_interface_control_actions itemID, 
      const char * const XPMbitmapData [], const wxChar toolTip []//, wxSizer * p_wxsizer
      );

//    ParseByRise m_parsebyrise ;
//    wxBoxSizer * p_boxsizerOuter ;
//    wxTextCtrl * mp_textctrlEnglishText;
//    wxTextCtrl * mp_textctrlGermanText ;
//    //wxPanel * mp_wxparsetreepanel;
//    wxParseTreePanel * mp_wxparsetreepanel;
//    wxPanel * mp_wxpanelTop;
//    wxSplitterWindow * mp_wxsplitterwindow ;
  public:
    MainFrame(
      const wxString & cr_wxstrTitle = wxEmptyString,
      const wxPoint & pos = wxDefaultPosition,
      const wxSize & size = wxDefaultSize
      );
    virtual
    ~MainFrame();
#include "EventHandlerFunctions.hpp"
  };

}

#endif /* MAINFRAME_HPP_ */
