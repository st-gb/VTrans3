///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

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

//#include "wx/html/webkit.h"
//#include <wx/filedlg.h>
//#include <wx/defs.h>
//#include <wx/frame.h>
//#include <wx/event.h> //for EVT_CLOSE
//#include <wx/html/htmlwin.h>
#include <wx/arrstr.h> //wxArrayString
#include <wx/bmpbuttn.h> //class wxBitmapButton
#include <wx/sizer.h> //class wxBoxSizer
#include <wx/splitter.h> //class wxSplitterWindow
#include <wx/timer.h> //for EVT_TIMER (,...?)

//#define COMPILE_WITH_TEXT_TO_SPEECH
#ifdef COMPILE_WITH_TEXT_TO_SPEECH
  #include <ax/ax_speech.h>
#endif

#include "wxTextInputDlg.hpp"
#include <IO/dictionary/VTransDictFormatReader.hpp> //class OneLinePerWordPair

#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN()

#include <Controller/character_string/convertFromAndToStdString.hpp>

#include <Translate/TranslateTreeTraverser.hpp> //TranslationAndGrammarPart
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>

#include <wxWidgets/wxHTMLfileOutput.hpp> //class wxHTMLfileOutput

#include <sstream>//class std::stringstream

using namespace wxWidgets ;

///////////////////////////////////////////////////////////////////////////

wxToolBarToolBase * wxTextInputDlg::AddBitmapButtonAndTooltip(
  enum user_interface_control_actions itemID, 
  const char * const XPMbitmapData [], const wxChar toolTip [])
{
  mp_wxbutton = new /*wxBitmap*/ wxBitmapButton(
//    //mp_wxsplitterwindow
//    //m_panelSplitterTop
//    //p_boxsizerOuter
    (wxDialog*) this,
    itemID,
    wxBitmap(XPMbitmapData)
    );
  mp_wxbutton->SetToolTip( toolTip ) ;
  m_p_boxsizerToolButtons->Add(mp_wxbutton, 0 //stretch factor. 0=do not stretch
    , wxBOTTOM, 2);
  return NULL;
}

wxTextInputDlg::wxTextInputDlg(
  wxWindow * p_wxwindowParent,
  TranslationControllerBase & r_translationcontrollerbase,
  wxWindowID wxwindow_id,
  const wxString & cr_wxstrTitle,
  const wxPoint & cr_wxpointWindowPosition,
  const wxSize & cr_wxsizeWindow,
  long style
  )
  : wxDialog( p_wxwindowParent, wxwindow_id, cr_wxstrTitle,
      cr_wxpointWindowPosition, cr_wxsizeWindow, style )
    , MainWindowBase(this)
//  , mp_wxhtmlwindow( NULL)
//  , m_parsebyrise( ::wxGetApp() )
//  , m_translationcontrollerbase(r_translationcontrollerbase)
//  , m_parsebyrise ( ::wxGetApp().m_parsebyrise )
{
  m_p_boxsizerToolButtons = new wxBoxSizer( wxHORIZONTAL ) ;
//	SetSizeHints( wxDefaultSize, wxDefaultSize );
	//wxIcon wxiconThisDialog( VT_icon_xpm ) ;
//	SetIcon( wxiconThisDialog ) ;

//  if( ! mp_wxhtmlwindow )
//  {
//    wxFrame * p_frame = new wxFrame(NULL,wxID_ANY,wxT("dd")) ;
//    p_frame->Show(true) ;
////    mp_wxwebkitctrl = new wxWebKitCtrl( p_frame , wxID_ANY ,
////      _T("http://www.wxwidgets.org"), wxDefaultPosition, wxSize(200, 200));
//    mp_wxhtmlwindow = new wxHtmlWindow( //p_frame
//      ) ;
//    mp_wxhtmlwindow->SetRelatedFrame( p_frame
//      //this
//      , wxT("HtmlHelp : %s") ) ;
//    wxString wxstrHTML = "<html><body>\n"
//                 "<h1>Error</h1>\n"
//             "Some error occurred :-H)\n" ;
//    wxstrHTML += "<form action=\"select.htm\">\n"
//        "<p>\n"
//          "<select name=\"top5\" size=\"3\">\n"
//            "<option>Heino</option>\n"
//            "<option>Tom Waits</option>\n"
//          "</select>\n"
//        "</p>\n"
//      "</form>\n"
//      "</body></hmtl>" ;
//    mp_wxhtmlwindow->SetPage(wxstrHTML);
//    mp_wxhtmlwindow->LoadPage(
////    mp_wxhtmlwindow->LoadFile(
//      wxT("http://de.selfhtml.org/html/formulare/anzeige/select_multiple.htm")
//      ) ;
//    mp_wxhtmlwindow->SetRelatedStatusBar(0);
//    mp_wxhtmlwindow->Show(true) ;
//  }
  mp_wxsplitterwindowOutmost = new wxSplitterWindow( (wxDialog *) this, wxID_ANY ) ;
  AddInputAndOutputPanels();
  AddToolButtons() ;
}

wxTextInputDlg::~wxTextInputDlg()
{
  LOGN("")
}

std::string wxTextInputDlg::GetAllTranslationPossibilites(
  const std::vector<std::string> & stdvec_stdstrWholeTransl ,
  const WordCompoundsAtSameTokenIndex &
    stdvec_stdvecTranslationAndGrammarPart
  )
{
  std::string stdstrAllPossibilities ;
  for( std::vector<std::string>::const_iterator c_iter_stdvec_stdstr =
      stdvec_stdstrWholeTransl.begin() ;
      c_iter_stdvec_stdstr != stdvec_stdstrWholeTransl.end()
      ; ++ c_iter_stdvec_stdstr
      )
  {
    stdstrAllPossibilities += *c_iter_stdvec_stdstr + "\n" ;
  }
  for( std::vector< std::vector<TranslationAndGrammarPart
  //      TranslationAndConsecutiveID
      > >::const_iterator
      c_iter_stdvec_stdvec =
      stdvec_stdvecTranslationAndGrammarPart.begin() ;
  //      stdvec_stdvecTranslationAndConsecutiveID.begin() ;
      c_iter_stdvec_stdvec != stdvec_stdvecTranslationAndGrammarPart.end() ;
  //          stdvec_stdvecTranslationAndConsecutiveID.end() ;
      ++ c_iter_stdvec_stdvec
      )
  {
    std::stringstream stdstrstream ;
    //translation token for the same poss.
    for( std::vector<TranslationAndGrammarPart
        //TranslationAndConsecutiveID
        >::const_iterator
        c_iter_stdvec =
            c_iter_stdvec_stdvec->begin() ;
        c_iter_stdvec != c_iter_stdvec_stdvec->end() ;
        ++ c_iter_stdvec
        )
    {
      stdstrstream << c_iter_stdvec->m_stdstrTranslation ;
      if( c_iter_stdvec->mp_grammarpart )
      {
        stdstrstream << " ID: "
            << c_iter_stdvec->mp_grammarpart->m_wConsecutiveID
          << " addr. with ID:"
          << std::ios::hex << c_iter_stdvec->mp_grammarpart << " " ;
      }
  //          << c_iter_stdvec->m_wConsecutiveID  ;
    }
    stdstrAllPossibilities += stdstrstream.str() + "\n";
  }
  return stdstrAllPossibilities ;
}
