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
#include <bitmaps/translate_bitmap.xpm> //for array translate_bitmap_xpm
#include <wxWidgets/UserInterface/MainFrame.hpp> //class MainFrame
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
    p_wxmenu = new wxMenu( wxT("translate") ) ;
    mp_wxmenubar->Append( p_wxmenu , wxT("tarn")) ;
    SetMenuBar( mp_wxmenubar );
  }

  MainFrame::MainFrame(
    const wxString & cr_wxstrTitle,
    const wxPoint & cr_wxpointPos,
    const wxSize & cr_wxsize
    )
    :
    wxFrame(
      (wxFrame *) NULL,
      -1,
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
    , m_parsebyrise( ::wxGetApp() )
  {
    mp_wxsplitterwindow = new wxSplitterWindow( this, wxID_ANY ) ;
  //  mp_wxpanelTop = new wxPanel( mp_wxsplitterwindow ) ;

    mp_wxparsetreepanel = new //wxPanel(
      wxParseTreePanel(
      //new wxParseTreePanel(
      //this
      mp_wxsplitterwindow
      //(wxFrame*) mp_wxsplitterwindow
      , wxID_ANY
      , wxDefaultPosition
      , wxDefaultSize,
      wxTAB_TRAVERSAL
      );

    wxPanel * m_panelSplitterTop = new wxPanel(
      //parent window
      //this
      mp_wxsplitterwindow
  //    , wxID_ANY,
  //    wxDefaultPosition,
  //    wxDefaultSize,
  //    wxTAB_TRAVERSAL
      );

     //wxGridBagSizer * p_gridbagsizerSplitterTop = new wxGridBagSizer( ) ;
      wxFlexGridSizer * p_flexgridsizerSplitterTop = new wxFlexGridSizer( 0 ) ;
      p_flexgridsizerSplitterTop->SetFlexibleDirection( wxVERTICAL ) ;
      wxBoxSizer * p_boxsizerSplitterTop = new wxBoxSizer( //wxHORIZONTAL
        wxVERTICAL ) ;

    m_panelSplitterTop->SetSizer(
      //p_gridbagsizerSplitterTop
  //    p_flexgridsizerSplitterTop
      p_boxsizerSplitterTop
      ) ;

    mp_textctrlEnglishText = new wxTextCtrl(
      //this
  //    mp_wxsplitterwindow
      m_panelSplitterTop
      , wxID_ANY,
      wxEmptyString,
      wxDefaultPosition,
      wxDefaultSize,
      wxHSCROLL | wxTE_MULTILINE | wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB
      );
    mp_textctrlGermanText = new wxTextCtrl(
      //this
  //    mp_wxsplitterwindow
      m_panelSplitterTop
      , wxID_ANY,
      wxEmptyString,
      wxDefaultPosition,
      wxDefaultSize,
      wxHSCROLL | wxTE_MULTILINE | wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB
      );

    //p_gridbagsizerSplitterTop->Add( m_textCtrl7, 1, wxEXPAND | wxALL, 5 );
  //  p_flexgridsizerSplitterTop->Add(
    p_boxsizerSplitterTop->Add(
      mp_textctrlEnglishText,
      1,
      wxEXPAND | wxALL
      , 5
      );
  //  p_flexgridsizerSplitterTop->Add(
    p_boxsizerSplitterTop->Add(
      mp_textctrlGermanText,
      1,
      wxEXPAND | wxALL
      , 5
      );

  //  p_flexgridsizerSplitterTop->Add(
  //  p_boxsizerSplitterTop->Add(
  //    p_wxbutton ,
  //    0, wxALL, 5 );

    //wxFlexGridSizer * p_flexgridsizerOuter;
    //p_flexgridsizerOuter = new wxFlexGridSizer( 2, 1, 0, 0 );
    wxGridSizer * p_gridsizerOuter;
    p_gridsizerOuter = new wxGridSizer(
      1, //rows
      1, //cols
      0,
      0 );
    p_boxsizerOuter = new wxBoxSizer( wxVERTICAL ) ;

  //  gSizer3->Add( m_textCtrl7, 1, wxALL|wxEXPAND, 5 );

    mp_wxsplitterwindow->SplitHorizontally(
      m_panelSplitterTop
  //    mp_textctrlEnglishText
      , mp_wxparsetreepanel
      ) ;
  //  mp_wxsplitterwindow->SplitHorizontally(
  //    //m_panelSplitterTop
  //    mp_textctrlGermanText
  //    , mp_wxparsetreepanel
  //    ) ;

    //gSizer3->Add( m_panel1, 1, wxEXPAND | wxALL, 5 );
    //gSizer3->Add( p_wxbutton, 0 , wxALL, 5 );
    //p_gridsizerOuter->Add(
    p_boxsizerOuter->Add(
      mp_wxsplitterwindow
      //m_textCtrl7
      //p_wxbutton
      , 1
        //0
      , wxEXPAND |
        wxBOTTOM
      , 2 );
  //  p_boxsizerOuter->Add(
  //    m_panel1
  //    //m_textCtrl7
  //    //p_wxbutton
  //    , 1
  //      //0
  //    , wxEXPAND |
  //      wxBOTTOM
  //    , 2 );

    //m_panelSplitterTop->Layout() ;
    SetSizer( //p_gridsizerOuter
      p_boxsizerOuter
      );
    SetAutoLayout(true);
    Layout();
    AddMenuBar() ;
    //from wxWidgets' toolbar sample:
    mp_wxtoolbar = new wxToolBar(
      this,
      wxID_ANY //,
//      wxDefaultPosition, wxDefaultSize,
//      style
      );
    mp_wxtoolbar->SetMargins(4, 4);
    mp_wxtoolbar->AddTool(
      ID_Translate,
      _T("translate"),
      wxBitmap( translate_bitmap_xpm)
//      , wxNullBitmap, wxITEM_NORMAL,
//      _T("New file"), _T("This is help for new file tool")
      );
    SetToolBar( mp_wxtoolbar ) ;
    // "after adding the buttons to the toolbar, must call Realize() to reflect
    // the changes"
    mp_wxtoolbar->Realize();
  }

  MainFrame::~MainFrame()
  {
    // TODO Auto-generated destructor stub
  }

}
