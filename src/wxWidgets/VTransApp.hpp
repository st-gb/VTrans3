/* 
 * File:   VTransApp.hpp
 * Author: Stefan
 *
 * Created on 20. Februar 2010, 14:02
 */

#ifndef _VTRANSAPP_HPP
#define	_VTRANSAPP_HPP

#ifdef _WIN32
  /** Undefine "#define Yield()" in "winbase.h" for virtual bool
   *   wxAppConsole::Yield(bool )  */
  #include <wx/msw/winundef.h>
#endif
#include <wx/app.h>//base class wxApp
#include <wx/icon.h> //class wxIcon

//Base class TranslationControllerBase
#include <Controller/TranslationControllerBase.hpp>
//#include <UserInterface/I_UserInterface.hpp>//base class I_UserInterface
//#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>//class LetterTree
//#include <xercesc/util/XercesVersion.hpp> //XERCES_CPP_NAMESPACE
#include <string> //class std::string
#include <Controller/multithread/nativeCriticalSectionType.hpp>
//class wxTextControlDialog
#include <wxWidgets/UserInterface/wxTextControlDialog.hpp>

#include "UserInterface/DictionaryStatisticsWindow.hpp"

//#include "UserInterface/MainWindow/MainWindowBase.hpp"

//Forward declarations.
class wxTextInputDlg ;
class wxIcon;
namespace XERCES_CPP_NAMESPACE
{
  class DefaultHandler ;
}
namespace VTrans
{
  class ShowTranslationRulesDialog;
}
class TranslationRule;
class DictionaryStatisticsWindow;
namespace wxWidgets
{
  class MainFrame;
  class MainWindowBase;
}

namespace wxWidgets
{
  class VTransApp
    : public wxApp
    //, public I_UserInterface
    , public TranslationControllerBase
  {
  public:
    /** "volatile" for multithreaded access to this variable */
    volatile bool m_messageIsShown;
    bool m_atLeast1MessageToShow;
//    condition_type;
    nativeCriticalSection_type m_critSecShowMessage;
    DictionaryStatisticsWindow * m_p_dictionaryStatisticsWindow;
    wxIcon m_wxiconVTrans;
    VTrans::ShowTranslationRulesDialog * m_p_showtranslationrulesdialog;
  //  static LetterTree s_lettertree ;
//    wxTextInputDlg * m_p_wx_text_input_dialog ;
    /*wxWidgets::MainWindowBase*/ wxWidgets::MainFrame * m_p_mainWindow;
    /** For buffering before calling new thread */
    std::string m_std_strLastSelectedDictFilePath;
  //  std::string m_stdstrVocabularyFilePath ;

    VTransApp();
    virtual ~VTransApp();

    void CreateAndShowMainWindow() ;
    void DisableDictAccessingActions();
    bool GetProgramIconFromFile(wxIcon & r_wxicon );
    void GetSourceText(std::string & std_string);
    bool HandleCommandLineArgs() ;
    /** Override in order to catch all log file access exceptions that arise
     *  from (in)directly be called via a GUI message/ action. */
    void HandleEvent(
      wxEvtHandler *handler,
      wxEventFunction func,
      wxEvent& event) const;
    virtual int OnExit();
    virtual bool OnInit();
  //  VTransApp(const VTransApp& orig);
    void LoadingVocabularyFileFailed(const std::string & cr_stdstrFilePath,
        const std::string & std_strErrorMessage);
    void Message( const std::string & cr_stdstr/*, unsigned threadID*/ ) ;
    void Message( const std::wstring & cr_stdwstr ) ;
    //TODO make usable for class ConfigReaderBase
//    void ProcessSelectedXMLfiles(
//      XERCES_CPP_NAMESPACE::DefaultHandler & r_xercesc_defaulthandler ,
//      const wxString & r_wxstrTitle,
//      const wxString & c_r_wxInitialDirForFileSelecion
//      );
    void ShowInvalidVocabularyFileFormatMessage(
      const VTrans::string_type & strWordFile,
      DWORD dwOffsetOfBeginOfEntry,
      DWORD dwOffset
      );
    inline int ShowMessage(const wxString &,
      const fastestUnsignedDataType flags = wxTextControlDialog::OK_Button);
    void EndTimer();
    void EndedGetDictStats() {
      m_p_dictionaryStatisticsWindow->EndedGetDictStats();
    };
    void UpdateAfterTranslation();
    void StartTimer();
    void SetDictionaryFilePath(const wxString & wxstr );
    void SetStatus(
      enum VTrans::StatusCode statusCode,
    //  const std::string & str
      const char * const pch
      );
    void OnMessage(wxCommandEvent &);
    void OnLoadingDictFileFailed(wxCommandEvent &);
    void OnSetStatus(wxCommandEvent &);
    /** Called by the dictionary access class to inform the UI (because it may 
     * take some seconds) about the current progress. */
    void UpdateDictionaryStatistics(
      const fastestUnsignedDataType fileSizeInBytes,
      const fastestUnsignedDataType numberOfBytesCurrentlyProcessed);
  private:
    DECLARE_EVENT_TABLE();
  };
}

//http://docs.wxwidgets.org/2.6/wx_rttimacros.html#declareapp:
//"It creates the declaration className& wxGetApp(void)."
DECLARE_APP(wxWidgets::VTransApp)

BEGIN_DECLARE_EVENT_TYPES()
  DECLARE_LOCAL_EVENT_TYPE(MessageEvent, wxNewEventType() )
  DECLARE_LOCAL_EVENT_TYPE(SetStatusEvent, wxNewEventType() )
END_DECLARE_EVENT_TYPES()

#endif	/* _VTRANSAPP_HPP */

