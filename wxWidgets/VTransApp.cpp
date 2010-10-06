/* 
 * File:   VTransApp.cpp
 * Author: Stefan
 * 
 * Created on 20. Februar 2010, 14:02
 */

#include "VTransApp.hpp"

#include <wx/wx.h>

#include <IO.hpp> //OneLinePerWordPair::LoadWords()
#include <Controller/Logger/Logger.hpp> //class Logger
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN()
#include <Translate/SyntaxTreePath.hpp>// SyntaxTreePath::sp_userinterface
#include <wxWidgets/Controller/wxStringHelper.hpp> //getwxString(...)
#include <wxWidgets/UserInterface/MainFrame.hpp>//class wxWidgets::MainFrame
#include <wxWidgets/UserInterface/wxTextInputDlg.hpp>//class wxTextInputDlg
#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>//class LetterTree

//#include <fstream> //for std::ofstream

//std::ofstream ofstreamLogFile ;
extern LetterTree g_lettertree ;
Logger g_logger ;

I_UserInterface * SyntaxTreePath::sp_userinterface ;

IMPLEMENT_APP(VTransApp)

VTransApp::VTransApp() {
}

VTransApp::VTransApp(const VTransApp& orig) {
}

VTransApp::~VTransApp() {
}

void VTransApp::Message( const std::string & cr_stdstr )
{
  ::wxMessageBox( getwxString( cr_stdstr ) ) ;
}

void VTransApp::Message( const std::wstring & cr_stdwstr )
{
  ::wxMessageBox( getwxString( cr_stdwstr) ) ;
}

bool VTransApp::OnInit()
{
  std::string stdstrLogFilePath = "VTrans_log.txt" ;
  g_logger.OpenFile2(stdstrLogFilePath) ;
  //Add a string as filter, else _nothing_ will be logged (cause is unknown).
  std::string stdstr = "sdsdsd" ;
  g_logger.AddExcludeFromLogging( stdstr ) ;
  LOGN("VTransApp::OnInit()")
  //ParseByRise parsebyrise ;
  if( argc > 1 )
  {
    std::string str(//"germanNounsFromTUdictInVTransFormatVeryShort.txt") ;
        argv[1] ) ;
    g_lettertree.InsertFundamentalWords() ;
    if( OneLinePerWordPair::LoadWords( //pWordNodeCurrent
         str )
      )
    {
      wxTextInputDlg * p_wx_text_input_dialog = new wxTextInputDlg(
       NULL
       //, wxID_ANY, wxString("gg"), wxPoint(50,50), wxSize(400,400),
       //wxDEFAULT_DIALOG_STYLE | wxCLOSE_BOX
       );
//      wxWidgets::MainFrame * p_mainframe = new wxWidgets::MainFrame(
//        wxT("dfd"), wxPoint(0,0) , wxSize(400,400)
//        ) ;
     if( p_wx_text_input_dialog
//         && p_mainframe
       )
     {
       SyntaxTreePath::sp_userinterface = this ;
       p_wx_text_input_dialog->Show(true);
//       p_mainframe->Show() ;
     }
     else
       std::cerr << "error: couldn't create window\n" ;
     return true;
    }
    else
    {
      wxString wxstrCwd = wxGetCwd() ;
      ::wxMessageBox( wxString::Format("Error loading vocabulary file \"%s\\%s\" "
        "->exiting", wxstrCwd.c_str(), str.c_str() )
        ) ;
    }
  }
  else
  {
    ::wxMessageBox( wxT("No vocabulary file specified as 1st command line "
      "argument ->exiting" ) ) ;
  }
  return false ;
}
