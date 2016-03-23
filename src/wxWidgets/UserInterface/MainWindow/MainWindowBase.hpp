/* 
 * File:   MainWindowBase.hpp
 * Author: stefan
 *
 * Created on May 8, 2013, 10:07 AM
 */

#ifndef MAINWINDOWBASE_HPP
#define	MAINWINDOWBASE_HPP

//#include <wx/window.h>
#include <wx/toplevel.h> //Base class wxTopLevelWindow/ wxTopLevelWindowBase
#include <wx/timer.h> //class wxTimer

#include <Attributes/TranslationResult.hpp> //class TranslationResult
//for enum user_interface_control_actions
#include <wxWidgets/user_interface_control_actions.h>
#include <Controller/thread_type.hpp> //typedef VTrans::thread_tyoe
#include <wxWidgets/UserInterface/LogEntries/LogEntriesDialog.hpp>

class ParseByRise;
class TranslationControllerBase;
class wxBoxSizer;
class wxGermanTranslationPanel;
class wxParseTreePanel ;
class wxSplitterWindow ;
class wxTextCtrl ;
class wxToolBarToolBase;

namespace wxWidgets
{
  /** Base class of all possible main windows (subclasses of this class may/
   *  should additionally be a subclass of a wxToplevelWindow-derived class,
   *  e.g. class wxFrame ) */
  class MainWindowBase
    /** Base class of wxDialog and wxFrame for calling its common methods
       such as "SetTitle(...)". */
    //:
    //public virtual /*wxWindow*/ /*wxTopLevelWindowBase*/ //EVENT_HANDLER_BASE_CLASS_NAME
    //private
    //  wxTopLevelWindow
  {
    /** Make wxTopLevelWindow as member of this class instead of subclassing
     *  this class from class wxTopLevelWindow ,
     *  else subclasses of this class may both inherit from wxEvtHandler ->
     *  double inheritance problem (compile errors).*/
    wxTopLevelWindow * m_p_mainwindow;
  protected:
    /** Member variables for multi threaded translations. */
    VTrans::thread_type m_translateThread;
    std::string m_std_strWholeInputText;
    TranslationResult m_translationResult;
    /** Member variables for multi threaded translations END. */

    wxToolBarToolBase * m_p_wxToolBarToolBaseLoadDictionary;
    LogEntriesDialog * m_p_logEntriesDialog;
  public:
    static unsigned s_windowID;
    ParseByRise & m_parsebyrise;
    TranslationControllerBase & m_translationcontrollerbase;
  public:
    MainWindowBase(wxTopLevelWindow * p_mainwindow);
//    MainWindowBase(const MainWindowBase& orig);
    virtual ~MainWindowBase();
    
    void AddInputAndOutputPanels();
    void AddInputAndOutputControls();
    void AddToolButtons() ;
    void AddAddGrammarRulesButton( wxSizer * p_sizer ) ;
    void AddDrawParseTreeButton(wxSizer * p_sizer ) ;
    void AddLoadDictionaryButton( wxSizer * p_sizer);
    void AddRemoveGrammarRulesButton( wxSizer * p_sizer ) ;
    void AddRemoveVocAttrDefsButton( wxSizer * p_sizer ) ;
    void AddAddTranslationRulesButton( wxSizer * p_sizer ) ;
    void AddShowTranslationRulesButton( wxSizer * p_sizer ) ;
    void AddShowParseRulesButton( wxSizer * p_sizer ) ;
    void AddAddTransformationRulesButton( wxSizer * p_sizer ) ;
    void AddAddVocAttrDefsButton( wxSizer * p_sizer ) ;
    void AddRemoveTranslationRulesButton( wxSizer * p_sizer ) ;
    void AddRemoveTransformationRulesButton( wxSizer * p_sizer ) ;
    void AddResolve1ParseLevelButton( wxSizer * p_sizer ) ;
    void AddResolveSuperClassesButton( wxSizer * p_sizer ) ;
    void AddShowInformationButton( wxSizer * p_sizer ) ;
    void AddTranslateButton( wxSizer * p_sizer ) ;
//    void AddSettingsButton( wxSizer * p_sizer );
    void AddTruncateLogFileButton( wxSizer * p_sizer ) ;
    void AddReInitializeGrammarButton(wxSizer * p_wxsizer);
    void GetEntireInputText(std::string & r_stdstrInputText);
    void EndTimer()
    {
      m_wxtimer.Stop();
      m_p_mainwindow->SetTitle( wxT("VTrans") );
    }
    /** Implementations of this method in in subclasses should e.g. deactivate
     *  "do translate" buttons or menu items. */
    virtual void DisableDoTranslateControls() {};
    void StartTimer()
    {
      m_wxtimer.Start(1000);
      //http://docs.wxwidgets.org/stable/wx_wxtimer.html#wxtimersetowner:
      //"Associates the timer with the given owner object. When the timer is
      //running, the owner will receive timer events with id equal to id
      //specified here."
      m_wxtimer.SetOwner(/*this*/ m_p_mainwindow, ID_Timer) ;
    }
    void Translate();
  protected:
    VTrans::thread_type m_unloadDictAndCloseWindowThread;
    wxBoxSizer * p_boxsizerOuter;
    wxTextCtrl * mp_textctrlEnglishText;
    wxGermanTranslationPanel * m_p_wxgermantranslationpanel;
    wxParseTreePanel * mp_wxparsetreepanel;
    wxSplitterWindow * mp_wxsplitterwindow;
    virtual wxToolBarToolBase * AddBitmapButtonAndTooltip(
      enum user_interface_control_actions itemID, 
      const char * const XPMbitmapData [],
      const wxChar toolTip []//, wxSizer * p_wxsizer
      ) = 0;
  private:
    wxTimer m_wxtimer;
    
  };
}

BEGIN_DECLARE_EVENT_TYPES()
  DECLARE_LOCAL_EVENT_TYPE(UpdateAfterTranslationEvent, wxNewEventType())
END_DECLARE_EVENT_TYPES()

#endif	/* MAINWINDOWBASE_HPP */

