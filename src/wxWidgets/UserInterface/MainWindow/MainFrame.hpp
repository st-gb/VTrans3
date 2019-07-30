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

/** Forward declarations (faster than #include) */
class wxMenu;
class wxMenuBar ;
class wxParseTreePanel ;
class wxSplitterWindow ;
class wxToolBar ;
class wxToolBarToolBase;

namespace wxWidgets
{
  class MainFrame
    : public wxFrame, public virtual MainWindowBase
  {
    //using wxPanel::Bind"
    wxMenuBar * mp_wxmenubar ;
    std::vector<wxMenuItem *> m_dictionaryRelatedMenuItems;
    wxMenu * p_wxmenuDictionary;
    wxMenu * m_p_wxmenuAction;
    wxToolBar * mp_wxtoolbar ;
    wxMenuItem * m_p_wxMenuItemTranslate;
    wxMenuItem * m_p_wxMenuItemUnloadDictionary;
    void AddMenuBar() ;
    wxToolBarToolBase * AddBitmapButtonAndTooltip(
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
      VTransApp &,
      const wxString & cr_wxstrTitle = wxEmptyString,
      const wxPoint & pos = wxDefaultPosition,
      const wxSize & size = wxDefaultSize
      );
    virtual
    ~MainFrame();

    void DisableDoTranslateControls();
    void EnableDictAccessingActions(const bool enable);
    void SetDictionaryFilePath(const wxString & wxstr);
    void UpdateAfterTranslation();
    #include "EventHandlerFunctions.hpp"
  };

}

#endif /* MAINFRAME_HPP_ */
