/* 
 * File:   VTransApp.cpp
 * Author: Stefan
 * 
 * Created on 20. Februar 2010, 14:02
 */

#include "VTransApp.hpp"

#include <wx/wx.h>
#include <wx/dialog.h> //class wxDialog
#include <wx/textctrl.h> //class wxTextCtrl

//#include <IO/IO.hpp> //OneLinePerWordPair::LoadWords()
#include <Controller/Logger/Logger.hpp> //class Logger
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN()
#include <Translate/SyntaxTreePath.hpp>// SyntaxTreePath::sp_userinterface
//getwxString(...)
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
#include <wxWidgets/UserInterface/MainFrame.hpp>//class wxWidgets::MainFrame
//ShowMultipleFileSelectionDialog(...)
#include <wxWidgets/UserInterface/UserInterface.hpp>
#include <wxWidgets/UserInterface/wxTextInputDlg.hpp>//class wxTextInputDlg
//class wxTextControlDialog
#include <wxWidgets/UserInterface/wxTextControlDialog.hpp>
//#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>//class LetterTree

#include <Xerces/ReadXMLfile.hpp> //ReadXMLfile_Inline(...)
//#include <Xerces/SAX2MainConfigHandler.hpp> //class SAX2MainConfigHandler

//#include <fstream> //for std::ofstream

//std::ofstream ofstreamLogFile ;
//extern LetterTree g_lettertree ;

////Static variables need also to be defined in the source file.
////LetterTree VTransApp::s_lettertree ;
//LetterTree TranslationControllerBase::s_lettertree ;

//LetterTree & g_lettertree = VTransApp::s_lettertree ;

//The global logger object is already in the preprocessor_macros.cpp file.
//Logger g_logger ;

//I_UserInterface * SyntaxTreePath::sp_userinterface ;

IMPLEMENT_APP(VTransApp)

VTransApp::VTransApp()
  :
//  m_parsebyrise( * this ) ,
//  m_translateparsebyrisetree(
//    m_parsebyrise
//    , * this
//    )
  m_p_wx_text_input_dialog(NULL)
{
  m_parsebyrise.SetUserInterface( this ) ;
}

//VTransApp::VTransApp(const VTransApp& orig) {
//}

VTransApp::~VTransApp()
{
  LOGN("VTransApp::~VTransApp()")
  //TODO solve program crash:
//      Thread [1] (Suspended: Signal 'SIGSEGV' received. Description: Segmentation fault.)
//      8 <symbol is not available> 0x77b08c19
//      7 g_logger()  0x00735050
//      6 g_logger()  0x00735054
//      5 <symbol is not available> 0x00000001
//      4 <symbol is not available> 0x50000063
//      3 <symbol is not available> 0x02240000
//      2 <symbol is not available> 0x735ffd7a
//      1 <symbol is not available> 0x00000000
  //-> because ~Lettertrie uses a logger object by "LOGN" although the logger
  // is already destroyed.
//  g_lettertree.DeleteCompleteList() ;
}

void VTransApp::CreateAndShowMainWindow()
{
  wxTextInputDlg * p_wx_text_input_dialog = new wxTextInputDlg(
   NULL
   //, wxID_ANY, wxString("gg"), wxPoint(50,50), wxSize(400,400),
   //wxDEFAULT_DIALOG_STYLE | wxCLOSE_BOX
   );
  m_p_wx_text_input_dialog = p_wx_text_input_dialog ;
  //      wxWidgets::MainFrame * p_mainframe = new wxWidgets::MainFrame(
  //        wxT("dfd"), wxPoint(0,0) , wxSize(400,400)
  //        ) ;
  if( p_wx_text_input_dialog
  //         && p_mainframe
    )
  {
    p_wx_text_input_dialog->Show(true);
    //If this dialog is closed the app should exit.
    SetTopWindow( p_wx_text_input_dialog) ;
  //       p_mainframe->Show() ;
  }
  else
    std::cerr << "error: couldn't create window\n" ;
}

void VTransApp::Message( const std::string & cr_stdstr )
{
//  ::wxMessageBox( getwxString( cr_stdstr ) , wxT("Translator message") ) ;
//  ::wxMe
  wxDialog wxd(NULL, wxID_ANY, wxT("Translator message") );

  wxString wxstrMessage = ::getwxString( cr_stdstr);
//  wxTextCtrl * p_wxtextctrl = new wxTextCtrl(
//    & wxd //wxWindow* parent
//    , wxID_ANY //wxWindowID id
//    , wxstrMessage //const wxString& value = ""
//    , wxDefaultPosition //const wxPoint& pos = wxDefaultPosition,
//    , wxDefaultSize //const wxSize& size = wxDefaultSize
//    , wxTE_READONLY | wxTE_MULTILINE //long style = 0
//    );
////  wxd.AddChild( p_wxtextctrl);
//  wxd.Layout(); //stretch to the whole client window.
//  wxd.ShowModal();
  wxTextControlDialog wxtextcontroldialog(
    wxstrMessage);
  wxtextcontroldialog.ShowModal();
}

void VTransApp::Message( const std::wstring & cr_std_wstr )
{
//  ::wxMessageBox( getwxString( cr_stdwstr) ) ;
  wxString wxstrMessage = ::getwxString( cr_std_wstr);

  wxTextControlDialog wxtextcontroldialog(
    wxstrMessage);
  wxtextcontroldialog.ShowModal();
}

//@return true: continue to run the (main loop of ) this program.
bool VTransApp::HandleCommandLineArgs()
{
  WRITE_TO_LOG_FILE_AND_STDOUT_NEWLINE("usage: <this_program> "
    "<path to main config_file> <path to root directory for all config files>")
//  bool bRet = false ;
  if( argc > 1 )
  {
    //Needed for SyntaxTreePath::CreateGrammarPartIDArray(...).
    SyntaxTreePath::sp_userinterface = this ;
    std::string stdstrFilePath(
      //"germanNounsFromTUdictInVTransFormatVeryShort.txt") ;
      argv[1] ) ;
//    g_lettertree.InsertFundamentalWords() ;

//    if( OneLinePerWordPair::LoadWords( //pWordNodeCurrent
//         stdstrFilePath )
//      )
//    {
//      CreateAndShowMainWindow() ;
//      if( argc > 2 )
//      {
//        stdstrFilePath = argv[2] ;
//        ReadMainConfigFile(stdstrFilePath) ;
//      }
//      return true;
//    }
//    else
//    {
//      wxString wxstrCwd = wxGetCwd() ;
//      ::wxMessageBox( wxString::Format("Error loading vocabulary file \"%s\\%s\" "
//        "->exiting", wxstrCwd.c_str(), stdstrFilePath.c_str() )
//        ) ;
//    }
//    m_translateparsebyrisetree.AddVocAndTranslDefinitions() ;

    if( argc > 2 )
    {
      LOGN("Before setting current directory to \"" //main config file's full path"
        << argv[2] << "\"")
      ::wxSetWorkingDirectory(argv[2] );
      LOGN("After setting current directory to \"" //main config file's full path"
        << argv[2] << "\"")
    }
    Init(stdstrFilePath) ;
  }
  else
  {
    ::wxMessageBox( wxT("No main config file specified as 1st command line "
      "argument ->exiting" )
      ) ;
  }
  //Return true to continue to run the (main loop of ) this program.
  return //false ;
    true ;
}

void VTransApp::LoadingVocabularyFileFailed(const std::string & stdstrFilePath)
{
  wxString wxstrCwd = wxGetCwd() ;
  ::wxMessageBox(
      wxString::Format( "Error loading vocabulary file \"%s\""
      "\%s\" "
      //"->exiting"
      ,
      wxstrCwd.c_str(),
      stdstrFilePath.c_str()
      )
    ) ;
}

//http://docs.wxwidgets.org/stable/wx_wxappoverview.html:
//"Another aspect of the application shutdown is OnExit which is called when
//the application exits but before wxWidgets cleans up its internal
//structures. You should delete all wxWidgets object that you created by
//the time OnExit finishes.
//In particular, do not destroy them from application class' destructor!"
int VTransApp::OnExit()
{
  LOGN("wxApp-derived::OnExit() begin")
//  if( m_p_wx_text_input_dialog )
//  {
//    m_p_wx_text_input_dialog->Destroy() ;
//  }
  LOGN("wxApp-derived::OnExit() return 0")
  return 0 ;
}

bool VTransApp::OnInit()
{
  std::string stdstrLogFilePath = "VTrans_log.txt" ;
  g_logger.OpenFile2(stdstrLogFilePath) ;
//  //Add a string as filter, else _nothing_ will be logged (cause is unknown).
//  std::string stdstr = "sdsdsd" ;
//  g_logger.AddExcludeFromLogging( stdstr ) ;
  LOGN("VTransApp::OnInit()")
  //ParseByRise parsebyrise ;

  return HandleCommandLineArgs() ;
//  return false ;
}

void VTransApp::ProcessSelectedXMLfiles(
  XERCES_CPP_NAMESPACE::DefaultHandler & r_xercesc_defaulthandler ,
  const wxString & r_wxstrTitle,
  const wxString & c_r_wxInitialDirForFileSelecion
  )
{
  wxArrayString wxarraystringPaths ;
  if( wxWidgets::ShowMultipleFileSelectionDialog(
      r_wxstrTitle ,
      wxarraystringPaths ,
      NULL,
      ::wxGetCwd()
      ) == wxID_OK
    )
  {
    std::string stdstrFilePath ;
    wxString wxstrFullPath ;
    for( size_t size_tArrayIndex = 0 ;
        size_tArrayIndex < wxarraystringPaths.GetCount() ;
        ++ size_tArrayIndex
       )
    {
      wxstrFullPath = wxarraystringPaths.Item(size_tArrayIndex ) ;
      stdstrFilePath = GetStdString( wxstrFullPath ) ;
      std::string stdstrFilePath = GetStdString( wxstrFullPath ) ;
      wxGetApp().ReadXMLfile(
        r_xercesc_defaulthandler ,
        stdstrFilePath ) ;
    }
  }
}
