/*
 * MainFrame.cpp
 *
 *  Created on: Sep 28, 2010
 *      Author: Stefan
 */
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wxWidgets/VTransApp.hpp> //::wxGetApp()

//DISable warning, from
//http://stackoverflow.com/questions/59670/how-to-get-rid-of-deprecated-conversion-from-string-constant-to-char-warning
// : "I believe passing -Wno-write-strings to gcc will suppress this warning."
#pragma GCC diagnostic ignored "-Wwrite-strings"
#include <bitmaps/translate_bitmap.xpm> //for array translate_bitmap_xpm
#include <bitmaps/VT_icon.xpm> // array "VT_icon_xpm"
//ENable warning
#pragma GCC diagnostic warning "-Wwrite-strings"

#include "MainFrame.hpp" //class MainFrame
#include <wxWidgets/UserInterface/wxParseTreePanel.hpp> //class wxParseTreePanel
#include <wxWidgets/user_interface_control_actions.h> //for enum

//#include <wx/string.h> //for class wxString
#include <wx/splitter.h> //class wxSplitterWindow
#include <wx/event.h> //EVT_MENU

namespace wxWidgets
{

//  BEGIN_EVENT_TABLE(MainFrame, wxDialog)
//    EVT_MENU( ID_Translate , MainFrame::OnTranslateButton)
//    EVT_MENU_HIGHLIGHT( ID_Translate , MainFrame::OnTranslateButton)
//  END_EVENT_TABLE()

  inline void AddViewMenu(wxMenuBar * p_wxmenubar)
  {
    wxMenu * p_wxmenu = new wxMenu( /*wxT("translate")*/ ) ;
    p_wxmenu->AppendCheckItem(ID_ShowGrammarPartMemoryAddress,
      wxT("show memory address of grammar part ") );
    p_wxmenu->AppendCheckItem(ID_ShowTranslatedWord,
      wxT("show translated word") );
    /** http://docs.wxwidgets.org/trunk/classwx_menu_item.html#a742aa5bb0d3faa020e7b3bd66e336499 : 
     * "Optionally you can specify also an accelerator string appending a tab 
     * character \t followed by a valid key combination (e.g. CTRL+V)." */
    p_wxmenu->Append( ID_DecreaseFontSize, 
      wxT("&decrease parse tree panel's font size\tCTRL+-") );
    p_wxmenu->Append( ID_IncreaseFontSize, 
      wxT("&increase parse tree panel's font size\tCTRL++") );
    p_wxmenubar->Append( p_wxmenu , wxT("&view")) ;
  }
  
  inline void AddLoggingMenu(wxMenuBar * p_wxmenubar)
  {
    wxMenu * p_wxmenu = new wxMenu( /*wxT("translate")*/ ) ;
    p_wxmenu->AppendRadioItem (ID_LogLevel_Debug, wxT("debug") );
    p_wxmenu->AppendRadioItem (ID_LogLevel_Warning, wxT("warning") );
    p_wxmenubar->Append( p_wxmenu , wxT("&log level")) ;
  }
  
  inline void AddSettingsMenu(wxMenuBar * p_wxmenubar)
  {
    wxMenu * p_wxmenu = new wxMenu( /*wxT("translate")*/ ) ;
    p_wxmenu->AppendCheckItem(ID_Translate_On_Text_Changes, 
      wxT("translate on text changes") );
    p_wxmenu->Check(ID_Translate_On_Text_Changes, 
      ::wxGetApp().m_GUIattributes.m_translateOnTextChanges);
    //p_wxmenubar->Append( p_wxmenu , wxT("&settings")) ;

    p_wxmenu->AppendCheckItem(ID_Double_Buffer_ParseTreePanel, 
      wxT("double buffer parse tree panel") );    
    p_wxmenubar->Append( p_wxmenu , wxT("&settings")) ;
  }
  
  void MainFrame::AddMenuBar()
  {
    mp_wxmenubar = new wxMenuBar() ;
    wxMenu * p_wxmenu = new wxMenu() ;
    
    wxMenuItem * p_wxmenuitem;
    m_p_wxmenuAction = new wxMenu( /*wxT("translate")*/ ) ;
    m_p_wxMenuItemTranslate = m_p_wxmenuAction->Append( ID_Translate, wxT("translate") );
    m_p_wxmenuAction->Append( ID_Settings, wxT("settings...") );
    m_p_wxmenuAction->Append( ID_AddGrammarRules, wxT("add grammar rules") );
    m_p_wxmenuAction->Append( ID_RemoveGrammarRules, wxT("remove all grammar rules") );
    mp_wxmenubar->Append( m_p_wxmenuAction , wxT("&action")) ;

    p_wxmenuDictionary = new wxMenu();
    p_wxmenuitem = p_wxmenuDictionary->Append( ID_LookupWord, wxT("&lookup word") );
    m_dictionaryRelatedMenuItems.push_back(p_wxmenuitem);
    m_dictionaryRelatedMenuItems.push_back(p_wxmenuDictionary->Append( 
      ID_ShowDictionaryStatistics,  wxT("&statistics") ) );
    m_p_wxMenuItemUnloadDictionary = p_wxmenuDictionary->Append(
      ID_UnloadDictionary, wxT("&unload") );
    m_dictionaryRelatedMenuItems.push_back(m_p_wxMenuItemUnloadDictionary);
    
    mp_wxmenubar->Append( p_wxmenuDictionary , wxT("&dictionary")) ;

    AddViewMenu(mp_wxmenubar);
    AddSettingsMenu(mp_wxmenubar);
    AddLoggingMenu(mp_wxmenubar);

    SetMenuBar( mp_wxmenubar );
  }
  
  wxToolBarToolBase * MainFrame::AddBitmapButtonAndTooltip(
      enum user_interface_control_actions itemID, 
      const char * const XPMbitmapData [],
      const wxChar toolTip []//, wxSizer * p_wxsizer
    )
  {
    return mp_wxtoolbar->AddTool(
      itemID, //ID_Translate
      toolTip, //wxT("translate")
      //wxBitmap( translate_bitmap_xpm)
      XPMbitmapData,
      toolTip
//      , wxNullBitmap, wxITEM_NORMAL,
//      _T("New file"), _T("This is help for new file tool")
      );
  }

  void MainFrame::DisableDoTranslateControls()
  {
     m_p_wxMenuItemTranslate->SetText( wxT("stop translation") );
  }
  
  void MainFrame::EnableDictAccessingActions(const bool enable)
  {
    wxString helpText;
    if( enable && ::wxGetApp().m_GUIattributes.m_translateOnTextChanges )
      Connect(ID_InputText, wxEVT_COMMAND_TEXT_UPDATED //wxEventType eventType, 
        , wxTextEventHandler(wxWidgets::MainFrame::OnInputTextChanges) );
    else
    {
      if( ! ::wxGetApp().m_GUIattributes.m_translateOnTextChanges )
      {
        helpText = wxT("disabled because currently collecting dictionary statistics");
  //      Disconnect (wxEVT_COMMAND_MENU_SELECTED, //wxEventType eventType, 
  //        wxWidgets::MainFrame::OnTranslateButton //wxObjectEventFunction function
  //        //, wxObject *userData=NULL, wxEvtHandler *eventSink=NULL
  //        );
  //      Disconnect (ID_InputText, wxEVT_COMMAND_TEXT_UPDATED //wxEventType eventType, 
  //        );
        Disconnect(ID_InputText, wxEVT_COMMAND_TEXT_UPDATED //wxEventType eventType, 
          , wxTextEventHandler(wxWidgets::MainFrame::OnInputTextChanges) );
      }
    }
    std::vector<wxMenuItem *>::const_iterator c_iter = m_dictionaryRelatedMenuItems.begin();
    while( c_iter != m_dictionaryRelatedMenuItems.end() )
    {
      (* c_iter)->SetHelp(helpText);
      ++ c_iter;
    }
    mp_wxtoolbar->EnableTool(ID_Translate, enable);
    mp_wxtoolbar->EnableTool(ID_LoadDictionary, enable);
    //ID_AddTranslationRules , 
    //TODO disable "translate" button, disable text events
    //EVT_TEXT(ID_InputText, 
    m_p_wxmenuAction->Enable(ID_Translate, enable);
    p_wxmenuDictionary->Enable(ID_LookupWord, enable);
//      p_wxmenuDictionary->GetSetHelp(helpText);
    p_wxmenuDictionary->Enable(ID_UnloadDictionary, enable);
    p_wxmenuDictionary->Enable(ID_ShowDictionaryStatistics, enable);
  }

  void MainFrame::SetDictionaryFilePath(const wxString & wxstrDictionaryFilePath)
  {
    m_p_wxMenuItemUnloadDictionary->SetText( wxT("Unload ") +
      wxstrDictionaryFilePath);
    m_p_wxToolBarToolBaseLoadDictionary->SetShortHelp(
      wxT("(re-)load dictionary ") + wxstrDictionaryFilePath);
  }

  MainFrame::MainFrame(
    const wxString & cr_wxstrTitle,
    const wxPoint & cr_wxpointPos,
    const wxSize & cr_wxsize
    )
    :
    wxFrame(
      (wxFrame *) NULL,
      /*wxID_ANY*/ s_windowID,
      cr_wxstrTitle,
      cr_wxpointPos,
      cr_wxsize
      //http://docs.wxwidgets.org/2.6/wx_wxwindow.html#wxwindow:
      //"Use this style to force a complete redraw of the window whenever it is
      //resized instead of redrawing just the part of the window affected by
      //resizing. Note that this was the behaviour by default before 2.5.1
      //release and that if you experience redraw problems with code which
      //previously used to work you may want to try this. Currently this style
      //applies on GTK+ 2 and Windows only, and full repainting is always done
      //on other platforms."
      //kurz: Stil ist notwendig, um das ganze Diagramm neu zu zeichnen
      , wxFULL_REPAINT_ON_RESIZE //| wxCLIP_CHILDREN
        //Necessary for showing a title bar
        | wxDEFAULT_FRAME_STYLE
      )
    , MainWindowBase(this)
//    , m_parsebyrise( ::wxGetApp() )
  {
    wxIcon wxiconThisDialog( /*VT_icon_xpm*/ wxGetApp().m_wxiconVTrans ) ;
    ( (wxFrame*) this)->SetIcon( wxiconThisDialog ) ;
//    AddInputAndOutputControls();
    mp_wxsplitterwindow = new wxSplitterWindow( //NULL
      (wxFrame *) this, wxID_ANY ) ;
    AddInputAndOutputPanels();
    if( ::wxGetApp().m_GUIattributes.m_translateOnTextChanges )
    {
      Connect(ID_InputText, wxEVT_COMMAND_TEXT_UPDATED //wxEventType eventType, 
        , wxTextEventHandler(wxWidgets::MainFrame::OnInputTextChanges) );
    }
    CreateStatusBar();
    AddMenuBar() ;
    //from wxWidgets' toolbar sample:
    mp_wxtoolbar = //new wxToolBar(
//      /*(wxFrame*)*/ this,
//      wxID_ANY //,
////      wxDefaultPosition, wxDefaultSize,
////      style
//      );
      CreateToolBar();

    mp_wxtoolbar->SetMargins(0, 0);
//    mp_wxtoolbar->AddTool(
//      ID_Translate, //itemID,
//      wxT("translate"), //toolTip,
//      //wxBitmap( translate_bitmap_xpm)
//      translate_bitmap_xpm
////      , wxNullBitmap, wxITEM_NORMAL,
////      _T("New file"), _T("This is help for new file tool")
//      );
//    AddTranslateButton( NULL ) ;

    AddToolButtons();
    //SetToolBar( mp_wxtoolbar ) ;
    //http://docs.wxwidgets.org/2.8/wx_wxtoolbar.html#wxtoolbaraddtool:
    // "after adding the buttons to the toolbar, must call Realize() to reflect
    // the changes"
    mp_wxtoolbar->Realize();
  }

  MainFrame::~MainFrame()
  {
    // TODO Auto-generated destructor stub
  }

}
