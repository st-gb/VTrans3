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

/** IGNORE_WRITE_STRINGS_WARNING, ENABLE_WRITE_STRINGS_WARNING */
#include <compiler/GCC/enable_disable_write_strings_warning.h>
IGNORE_WRITE_STRINGS_WARNING
#include <bitmaps/VT_icon.xpm> // array "VT_icon_xpm"
ENABLE_WRITE_STRINGS_WARNING

//#include <IO/IO.hpp> //OneLinePerWordPair::LoadWords()
#include <Controller/Logger/Logger.hpp> //class Logger
#include <Controller/Logger/Formatter/Log4jFormatter.hpp> //class Log4jFormatter
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN()
#include <Translate/SyntaxTreePath.hpp>// SyntaxTreePath::sp_userinterface
//getwxString(...)
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
#include <wxWidgets/UserInterface/MainWindow/MainFrame.hpp>//class wxWidgets::MainFrame
//ShowMultipleFileSelectionDialog(...)
#include <wxWidgets/UserInterface/UserInterface.hpp>
//#include <wxWidgets/UserInterface/MainWindow/wxTextInputDlg.hpp>//class wxTextInputDlg
//class wxTextControlDialog
#include <wxWidgets/UserInterface/wxTextControlDialog.hpp>
//#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>//class LetterTree

//#include <Xerces/ReadXMLfile.hpp> //ReadXMLfile_Inline(...)
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

using namespace wxWidgets;

TranslationControllerBase * g_p_translationcontrollerbase = NULL ;

IMPLEMENT_APP(VTransApp)

DEFINE_LOCAL_EVENT_TYPE(MessageEvent)

BEGIN_EVENT_TABLE( VTransApp, wxApp )
  EVT_COMMAND(wxID_ANY, MessageEvent, VTransApp::OnMessage)
END_EVENT_TABLE()

VTransApp::VTransApp()
  :
  m_messageIsShown(false),
  m_atLeast1MessageToShow(false),
//  m_parsebyrise( * this ) ,
//  m_translateparsebyrisetree(
//    m_parsebyrise
//    , * this
//    )
  m_p_mainWindow(NULL)//,
  //,m_wxiconVTrans( VT_icon_xpm )
{
  g_p_translationcontrollerbase = this;
  m_parsebyrise.SetUserInterface( this ) ;
}

//VTransApp::VTransApp(const VTransApp& orig) {
//}

VTransApp::~VTransApp()
{
  LOGN_DEBUG(//"VTransApp::~VTransApp()"
    "")
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
  
//  wxTextInputDlg * p_wx_text_input_dialog = new wxTextInputDlg(
//    NULL,
//    * ( //(TranslationControllerBase *)
//      this)
//    //, wxID_ANY, wxString("gg"), wxPoint(50,50), wxSize(400,400),
//    //wxDEFAULT_DIALOG_STYLE | wxCLOSE_BOX
//    );
  /*m_p_wx_text_input_dialog*/ //m_p_mainWindow = p_wx_text_input_dialog ;
  m_p_mainWindow = new MainFrame();
  //      wxWidgets::MainFrame * p_mainframe = new wxWidgets::MainFrame(
  //        wxT("dfd"), wxPoint(0,0) , wxSize(400,400)
  //        ) ;
  if( /*p_wx_text_input_dialog*/ m_p_mainWindow
  //         && p_mainframe
    )
  {
    (/*(wxFrame *)*/ m_p_mainWindow)->Show(true);
    //m_p_mainWindow->SetEventHandler( (wxWidgets::MainWindowBase*) m_p_mainWindow);
  //       p_mainframe->Show() ;
    InsertIntoVocabularyIntoMemory_Async(
      m_p_mainWindow, //wxWindow * p_wxwindowParent,
      //const std::string & std_strFilePath
      m_stdstrVocabularyFilePath
      );
  }
  else
  {
    std::cerr << "error: couldn't create window\n" ;
    LOGN_ERROR("couldn't create main window")
  }
}

void VTransApp::Message( const std::string & cr_stdstr /*, unsigned threadID*/ )
{
  /** Avoid putting to much "show message" events/ showing too much messages at
  * once: else if this message is called very often then a lot of message
  * queue entries would be inserted-> a lot of dialogs are shown. */
//  if( ! m_messageIsShown )
  {
    const DWORD dwCurrentThreadNumer = OperatingSystem::GetCurrentThreadNumber();
    if( dwCurrentThreadNumer == m_GUIthreadID)
    {
      wxString wxstrMessage = wxWidgets::getwxString( cr_stdstr);
      ShowMessage(wxstrMessage);
    }
    else
    {
      /** Pauses here if a dialog is currently being shown modally until it is
       * closed. */
      /** Avoid putting to much "show message" events/ showing too much messages at
      * once: else if this message is called very often then a lot of message
      * queue entries would be inserted-> a lot of dialogs are shown. */
      m_critSecShowMessage.Enter();
      //TODO Multiple "show message dialog" messages are added before a dialog
      //is shown.
//      if( m_atLeast1MessageToShow )
//        //Waits until dialog is closed.
//        m_conditionShowMessage.Wait();
      //Another than GUI thread->put into message queue to avoid program crash.
      wxCommandEvent wxcommand_event(MessageEvent);

      wxcommand_event.SetString(GetwxString_Inline(cr_stdstr));
    //    AsyncMessage(cr_stdstr);
    //    QueueEvent  (       wxEvent *       event   );
      //FIXME: http://docs.wxwidgets.org/trunk/classwx_evt_handler.html#a0737c6d2cbcd5ded4b1ecdd53ed0def3
      //"[...] can't be used to post events from worker threads for the event
      //objects with wxString fields (i.e. in practice most of them) [...]"
      AddPendingEvent(wxcommand_event);
      m_critSecShowMessage.Leave();
    }
  }
}

void VTransApp::Message( const std::wstring & cr_std_wstr )
{
//  ::wxMessageBox( getwxString( cr_stdwstr) ) ;
  wxString wxstrMessage = ::getwxString( cr_std_wstr);

  wxTextControlDialog wxtextcontroldialog(
    wxstrMessage);
  wxtextcontroldialog.ShowModal();
}

//void VTransApp::DictionaryFileLine(unsigned ui)
//{
////  SetTitle
//}

void VTransApp::GetSourceText(std::string & std_string)
{
  m_p_mainWindow->GetEntireInputText(std_string);
}

//@return true: continue to run the (main loop of ) this program.
bool VTransApp::HandleCommandLineArgs()
{
  WRITE_TO_LOG_FILE_AND_STDOUT_NEWLINE("usage: <this_program> "
    "<path to main config_file> <path to root directory for all config files>")
//  bool bRet = false ;
  if( argc > MainConfigFilePathProgArgIndex )
  {
    //Needed for SyntaxTreePath::CreateGrammarPartIDArray(...).
    SyntaxTreePath::sp_userinterface = this ;
    std::string stdstrFilePath =//(
      //"germanNounsFromTUdictInVTransFormatVeryShort.txt") ;
      GetStdString( wxString(argv[MainConfigFilePathProgArgIndex]) ) ;
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

    if( argc > CurrWorkDirProgArgIndex )
    {
      LOGN_DEBUG("Before setting current directory to \""
        //main config file's full path"
        << argv[2] << "\"")
      ::wxSetWorkingDirectory(argv[CurrWorkDirProgArgIndex] );
      LOGN("After setting current directory to \""
        //main config file's full path"
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
      wxString::Format( wxT("Error loading vocabulary file \"%s\""
      "\%s\" ")
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
  LOGN_DEBUG(//"wxApp-derived::OnExit() "
    "begin")
//  if( m_p_wx_text_input_dialog )
//  {
//    m_p_wx_text_input_dialog->Destroy() ;
//  }
  LOGN_DEBUG(//"wxApp-derived::OnExit() "
    "return 0")
  return 0 ;
}

/** @brief for showing messages from threads other than the main/GUI thread.
 *   (if showing/creating a GUI/ window from another than the main/GUI thread
 *    the process crashes/ may crash) */
void VTransApp::OnMessage(wxCommandEvent & event)
{
  wxString wxstr = event.GetString();
  ShowMessage(wxstr);
}

bool VTransApp::OnInit()
{
  m_wxiconVTrans = wxIcon( VT_icon_xpm );
  std::string stdstrLogFilePath = "VTrans_log.txt" ;
//  g_logger.SetLogLevel(/*LogLevel::debug*/ "debug");
  g_logger.m_logLevel = LogLevel::debug;
  try
  {
    g_logger.OpenFileA(stdstrLogFilePath, "log4j", 4000, LogLevel::debug) ;
  }
  catch(OpeningLogFileException & oe)
  {
    wxString wxstrCurrWorkDir = ::wxGetCwd();
    const wxString wxstrMessage =  wxString(wxT("Opening log file \"") ) +
        wxstrCurrWorkDir +
        wxWidgets::getwxString(stdstrLogFilePath) + wxT("\"failed:") +
        wxWidgets::getwxString( oe.GetErrorMessage() );
    ShowMessage(wxstrMessage);
  }
  const std::vector<FormattedLogEntryProcessor *> & formattedLogEntryProcessors = 
    g_logger.GetFormattedLogEntryProcessors();
//  if( formattedLogEntryProcessors.size() > 0 )
//  {
//    FormattedLogEntryProcessor * formattedLogEntryProcessor =
//      formattedLogEntryProcessors.front();
//    formattedLogEntryProcessor->SetFormatter(
//      new CSS::LogFormatter::Log4jFormatter( formattedLogEntryProcessor
//    ) );
//  }
//  //Add a string as filter, else _nothing_ will be logged (cause is unknown).
//  std::string stdstr = "sdsdsd" ;
//  g_logger.AddExcludeFromLogging( stdstr ) ;
  LOGN_DEBUG(/*"VTransApp::OnInit()"*/ "")
  //ParseByRise parsebyrise ;

  return HandleCommandLineArgs() ;
//  return false ;
}

//TODO make usable for class ConfigReaderBase
//void VTransApp::ProcessSelectedXMLfiles(
//  XERCES_CPP_NAMESPACE::DefaultHandler & r_xercesc_defaulthandler ,
//  const wxString & r_wxstrTitle,
//  const wxString & c_r_wxInitialDirForFileSelecion
//  )
//{
//  wxArrayString wxarraystringPaths ;
//  if( wxWidgets::ShowMultipleFileSelectionDialog(
//      r_wxstrTitle ,
//      wxarraystringPaths ,
//      NULL,
//      ::wxGetCwd()
//      ) == wxID_OK
//    )
//  {
//    std::string stdstrFilePath ;
//    wxString wxstrFullPath ;
//    for( size_t size_tArrayIndex = 0 ;
//        size_tArrayIndex < wxarraystringPaths.GetCount() ;
//        ++ size_tArrayIndex
//       )
//    {
//      wxstrFullPath = wxarraystringPaths.Item(size_tArrayIndex ) ;
//      stdstrFilePath = GetStdString( wxstrFullPath ) ;
//      std::string stdstrFilePath = GetStdString( wxstrFullPath ) ;
//      wxGetApp().ReadXMLfile(
//        r_xercesc_defaulthandler ,
//        stdstrFilePath ) ;
//    }
//  }
//}

inline void VTransApp::ShowMessage(const wxString & wxstrMessage)
{
//  if( threadID == m_GUIthreadID)
//  {
  m_critSecShowMessage.Enter();
  m_messageIsShown = true;
  /** Show wxTextControlDialog because so the message can be copied. */
  wxTextControlDialog wxtextcontroldialog(
    wxstrMessage);

  wxtextcontroldialog.ShowModal();
  m_messageIsShown = false;
  m_critSecShowMessage.Leave();
//  }
//  else
}

void VTransApp::ShowInvalidVocabularyFileFormatMessage(
  const VTrans::string_type & strWordFile,
  DWORD dwOffsetOfBeginOfEntry,
  DWORD dwOffset
  )
{
  wxString wxstrCwd = wxGetCwd();

  wxstrCwd += wxT("\\");
  wxstrCwd += GetwxString_Inline(strWordFile);

  wxString wxstrMessage = wxString::Format(
    wxT("Die Datei \n\"%s\"\n\nenthält kein gültiges "
    "Format oder sonstiger Fehler zwischen\nOffset (=Position in Byte ab "
    "Dateibeginn) %u (dezimal)\nund Offset (=Position in Byte ab Dateibeginn) "
    "%u (dezimal).\n\nDas Laden der Vokabeln wird beendet.\nVersuchen Sie, den "
    "Fehler in der Dateistruktur zu beheben."),
    wxstrCwd.c_str(),
    //strWordFile,
    dwOffsetOfBeginOfEntry, dwOffset);

  ::wxMessageBox( wxstrMessage );
}

void VTransApp::EndTimer()
{
  m_p_mainWindow->EndTimer();
}

void VTransApp::StartTimer()
{
  m_p_mainWindow->StartTimer();
}
