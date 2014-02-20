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

namespace wxWidgets
{

//  BEGIN_EVENT_TABLE(MainFrame, wxDialog)
//    EVT_MENU( ID_Translate , MainFrame::OnTranslateButton)
//    EVT_MENU_HIGHLIGHT( ID_Translate , MainFrame::OnTranslateButton)
//  END_EVENT_TABLE()

  void MainFrame::AddMenuBar()
  {
    mp_wxmenubar = new wxMenuBar() ;
    wxMenu * p_wxmenu ;
    p_wxmenu = new wxMenu( /*wxT("translate")*/ ) ;
    p_wxmenu->Append( ID_Translate, wxT("translate") );
    p_wxmenu->Append( ID_Settings, wxT("settings...") );
    mp_wxmenubar->Append( p_wxmenu , wxT("&action")) ;

    p_wxmenu = new wxMenu();
    p_wxmenu->Append( ID_LookupWord, wxT("&lookup word") );
    p_wxmenu->Append( ID_ShowDictionaryStatistics,  wxT("&statistics") );
    p_wxmenu->Append( ID_UnloadDictionary, wxT("&unload") );
    mp_wxmenubar->Append( p_wxmenu , wxT("&dictionary")) ;

    p_wxmenu = new wxMenu( /*wxT("translate")*/ ) ;
    p_wxmenu->AppendCheckItem(ID_ShowGrammarPartMemoryAddress,
      wxT("show memory address of grammar part ") );
    p_wxmenu->AppendCheckItem(ID_ShowGrammarTranslatedWord,
      wxT("show translated word") );
    mp_wxmenubar->Append( p_wxmenu , wxT("&view")) ;

    SetMenuBar( mp_wxmenubar );
  }
  
  void MainFrame::AddBitmapButtonAndTooltip(
      enum user_interface_control_actions itemID, 
      const char * const XPMbitmapData [],
      const wxChar toolTip []//, wxSizer * p_wxsizer
    )
  {
    mp_wxtoolbar->AddTool(
      itemID, //ID_Translate
      toolTip, //wxT("translate")
      //wxBitmap( translate_bitmap_xpm)
      XPMbitmapData,
      toolTip
//      , wxNullBitmap, wxITEM_NORMAL,
//      _T("New file"), _T("This is help for new file tool")
      );
  }
      
  MainFrame::MainFrame(
    const wxString & cr_wxstrTitle,
    const wxPoint & cr_wxpointPos,
    const wxSize & cr_wxsize
    )
    :
    wxFrame(
      (wxFrame *) NULL,
      wxID_ANY,
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
