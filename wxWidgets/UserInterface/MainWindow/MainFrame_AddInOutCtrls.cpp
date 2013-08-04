
  void MainFrame::AddInputAndOutputControls()
  {
    mp_wxsplitterwindow = new wxSplitterWindow( (wxFrame *) this, wxID_ANY ) ;
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
//    mp_textctrlGermanText = new wxTextCtrl(
//      //this
//  //    mp_wxsplitterwindow
//      m_panelSplitterTop
//      , wxID_ANY,
//      wxEmptyString,
//      wxDefaultPosition,
//      wxDefaultSize,
//      wxHSCROLL | wxTE_MULTILINE | wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB
//      );

    //p_gridbagsizerSplitterTop->Add( m_textCtrl7, 1, wxEXPAND | wxALL, 5 );
  //  p_flexgridsizerSplitterTop->Add(
    p_boxsizerSplitterTop->Add(
      mp_textctrlEnglishText,
      1,
      wxEXPAND | wxALL
      , 5
      );
  //  p_flexgridsizerSplitterTop->Add(
//    p_boxsizerSplitterTop->Add(
//      mp_textctrlGermanText,
//      1,
//      wxEXPAND | wxALL
//      , 5
//      );

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
    ((wxFrame *)this)->SetSizer( //p_gridsizerOuter
      p_boxsizerOuter
      );
    ((wxFrame *)this)->SetAutoLayout(true);
    ((wxFrame *)this)->Layout();
  }
