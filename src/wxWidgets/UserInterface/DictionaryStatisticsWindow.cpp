/* 
 * File:   DictionaryStatisticsWindow.cpp
 * Author: mr.sys
 * 
 * Created on 13. Oktober 2014, 23:22
 */

#include "DictionaryStatisticsWindow.hpp"
///** typedef nativeThread_type  */
//#include <multithread/nativeThreadType.hpp>
#include "MainWindow/MainFrame.hpp"
#include <wx/stattext.h> //class wxStaticText
#include <wx/textctrl.h> //class wxTextCtrl
#include <wx/sizer.h> //class wxBoxSizer
#include <wxWidgets/VTransApp.hpp>
#include <wx/timer.h> //EVT_TIMER
#include <stdexcept> //class std::out_of_range
#include <hardware/CPU/atomic/memory_barrier.h>
#include <wx/msgdlg.h> //wxMessageBox(...)

std::string DictionaryStatisticsWindow::s_getStatusString = "DictionaryStatistics";
extern TranslationControllerBase * g_p_translationcontrollerbase;

/** This event is intended to be posted after getting dict stats in a non-GUI 
 *  thread for ending the timer in GUI thread. */
DEFINE_LOCAL_EVENT_TYPE(EndedGetDictStatsEvent)

BEGIN_EVENT_TABLE(DictionaryStatisticsWindow, wxDialog)
  EVT_COMMAND(wxID_ANY, EndedGetDictStatsEvent, 
    DictionaryStatisticsWindow::OnEndedGetDictStats)
  EVT_CLOSE( DictionaryStatisticsWindow::OnClose)
  EVT_TIMER( ID_Timer, DictionaryStatisticsWindow::OnTimerEvent)
END_EVENT_TABLE()

DictionaryStatisticsWindow::DictionaryStatisticsWindow () 
  : wxDialog(
      NULL,
      wxID_ANY,
      wxT("dictionary statistics"), //const wxString& title
      wxDefaultPosition, //const wxPoint& pos = wxDefaultPosition,
      wxDefaultSize, //const wxSize& size = wxDefaultSize,
      wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
      )
{
  BuildGUI();
}

DictionaryStatisticsWindow::DictionaryStatisticsWindow (const DictionaryStatisticsWindow& orig) { }

DictionaryStatisticsWindow::~DictionaryStatisticsWindow () { }

void DictionaryStatisticsWindow::OnEndedGetDictStats(wxCommandEvent & event)
{
  m_wxtimer.Stop();
  ::wxGetApp().m_p_mainWindow->EnableDictAccessingActions(true);
  UpdateUI();
}

void DictionaryStatisticsWindow::OnTimerEvent(wxTimerEvent & e)
{
//  struct tm time;
//  g_p_translationcontrollerbase->GetStatus(s_getStatusString, time);
  wxWidgets::VTransApp & vt = ::wxGetApp();
  fastestUnsignedDataType itemNo;
  //memory_barrier ();
  vt.s_dictReaderAndVocAccess.m_vocAccess.GetCollectDictionaryStatisticsStatus(itemNo);
   SetTitle(wxString::Format(wxT("current item no: %u"), itemNo) );
   
  UpdateUI();
}

void DictionaryStatisticsWindow::UpdateUI()
{
  try
  {
    memory_barrier();
//    unsigned & ui = m_collectDictStatsThreadFuncParams.englishWordClass2CounterMap.at(
//      EnglishWord::plural_noun);
//    m_p_pluralNounStaticText->SetLabel(wxString::Format(wxT("%u"), ui));
//    unsigned & ui2 = m_collectDictStatsThreadFuncParams.englishWordClass2CounterMap.at(
//      EnglishWord::singular);
//    m_p_singularNounStaticText->SetLabel(wxString::Format(wxT("%u"), ui2));
    std::vector<EnglishWordCount>::const_iterator c_iter = 
      m_englishWordCountVector.begin();
    std::map<enum EnglishWord::English_word_class, unsigned> &
      r_englishWordClass2CounterMap = m_collectDictStatsThreadFuncParams.
      englishWordClass2CounterMap;

    wxTextCtrl * m_p_wxCurrentStaticText = NULL;
    while( c_iter != m_englishWordCountVector.end() )
    {
      const EnglishWordCount & r_englishWordCount = *c_iter;
      unsigned & ui = r_englishWordClass2CounterMap.at(r_englishWordCount.m_English_word_class);
      m_p_wxCurrentStaticText = r_englishWordCount.m_p_wxStaticText;
      //m_p_wxCurrentStaticText->SetEditable(/*bool editable*/ true);
      m_p_wxCurrentStaticText->ChangeValue(wxString::Format(wxT("%u"), ui));
      //m_p_wxCurrentStaticText->SetEditable(/*bool editable*/ false);
      ++ c_iter;
    }
  }
  catch(const std::out_of_range & e)
  {
#ifdef _DEBUG
    int i = 0;
    i ++;
#endif
  }
}

void DictionaryStatisticsWindow::AddStaticText(
  const wxString & wxstr,
  enum EnglishWord::English_word_class English_word_class
  //, wxSizer * p_wxxsizer
  )
{
  wxStaticText * p_wxStaticText = new wxStaticText(this, wxID_ANY, wxT("# ") + 
    wxstr + wxT(":") );
  wxTextCtrl * p_wxTextCtrlCounter = new wxTextCtrl(this, wxID_ANY, 
    wxT("xxxxx"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  
  EnglishWordCount englishWordCount(p_wxTextCtrlCounter, English_word_class );
  std::map<enum EnglishWord::English_word_class, unsigned> &
    r_englishWordClass2CounterMap = m_collectDictStatsThreadFuncParams.
    englishWordClass2CounterMap;
  r_englishWordClass2CounterMap.insert(std::make_pair(English_word_class, 0) );
  m_englishWordCountVector.push_back( englishWordCount);
  
  wxBoxSizer * m_p_boxsizerColumns = new wxBoxSizer( wxHORIZONTAL);
  m_p_boxsizerColumns->Add(
    p_wxStaticText,
    1,
    wxEXPAND | wxALL | wxALIGN_CENTER_VERTICAL 
    , 0
    );
  m_p_boxsizerColumns->Add(
    p_wxTextCtrlCounter,
    1,
    wxEXPAND | wxALL | wxALIGN_CENTER_VERTICAL 
    , 0
    );
  m_p_boxsizerRows->Add(m_p_boxsizerColumns );
}

void DictionaryStatisticsWindow::BuildGUI()
{
   wxBoxSizer * p_boxsizer = new wxBoxSizer( wxHORIZONTAL );
   SetSizer(p_boxsizer);
   //m_p_boxsizerLeftColumn = new wxBoxSizer( wxVERTICAL );
   //m_p_boxsizerRightColumn = new wxBoxSizer( wxVERTICAL );
   m_p_boxsizerRows = new wxBoxSizer( wxVERTICAL );
   
   AddStaticText(wxT("singular"), EnglishWord::singular);
   AddStaticText(wxT("plural"), EnglishWord::plural_noun);
   AddStaticText(wxT("intransitive verb"), EnglishWord::main_verb_allows_0object_infinitive);
   AddStaticText(wxT("transitive verb"), EnglishWord::main_verb_allows_1object_infinitive);
   AddStaticText(wxT("adjective"), EnglishWord::adjective);
//   p_boxsizerLeftColumn->Add(
//    new wxStaticText(this, wxID_ANY, wxT("# plural")),
//    1,
//    wxEXPAND | wxALL
//    , 5
//    );
//   m_p_pluralNounStaticText = new wxStaticText(this, wxID_ANY, wxT("xxxxxxx"));
//   m_p_singularNounStaticText = new wxStaticText(this, wxID_ANY, wxT("xxxxxx"));
//   p_boxsizerRightColumn->Add(
//    m_p_singularNounStaticText,
//    1,
//    wxEXPAND | wxALL
//    , 5
//    );
//   p_boxsizerRightColumn->Add(
//    m_p_pluralNounStaticText,
//    1,
//    wxEXPAND | wxALL
//    , 5
//    );
   //p_boxsizer->Add(m_p_boxsizerLeftColumn);
   //p_boxsizer->Add(m_p_boxsizerRightColumn);
   p_boxsizer->Add(m_p_boxsizerRows);
}

void DictionaryStatisticsWindow::EndedGetDictStats()
{
//  m_wxtimer.Stop();
// UpdateUIAsync();
  wxCommandEvent wxcommand_event(EndedGetDictStatsEvent);
  //    QueueEvent  (       wxEvent *       event   );
  //FIXME: http://docs.wxwidgets.org/trunk/classwx_evt_handler.html#a0737c6d2cbcd5ded4b1ecdd53ed0def3
  //"[...] can't be used to post events from worker threads for the event
  //objects with wxString fields (i.e. in practice most of them) [...]"
  AddPendingEvent(wxcommand_event);
}

void DictionaryStatisticsWindow::GetStatistics()
{
  const fastestUnsignedDataType numArrayEles = EnglishWord::beyond_last_entry;
  fastestUnsignedDataType numberOfWordClassRepresentations[numArrayEles];
  wxWidgets::VTransApp & vt = ::wxGetApp();
  m_wxtimer.Start(1000);
  m_wxtimer.SetOwner(this, ID_Timer) ;
  
//  std::map<enum EnglishWord::English_word_class, unsigned> &
//    r_englishWordClass2CounterMap = m_collectDictStatsThreadFuncParams.
//    englishWordClass2CounterMap;
//  r_englishWordClass2CounterMap.insert(std::make_pair(EnglishWord::singular, 0) );
//  r_englishWordClass2CounterMap.insert(std::make_pair(EnglishWord::plural_noun, 0) );
//  r_englishWordClass2CounterMap.insert(std::make_pair(EnglishWord::adjective, 0) );
  
  vt.s_dictReaderAndVocAccess.m_vocAccess.GetStatisticsInSeparateThread(//GetStatistics(
    //numberOfWordClassRepresentations, numArrayEles 
    //englishWordClass2CounterMap
    m_collectDictStatsThreadFuncParams,
    m_getStatisticsThread
    );
  
//  m_wxtimer.Stop();
  
  //http://docs.wxwidgets.org/stable/wx_wxtimer.html#wxtimersetowner:
  //"Associates the timer with the given owner object. When the timer is
  //running, the owner will receive timer events with id equal to id
  //specified here."
}

void DictionaryStatisticsWindow::OnClose( wxCloseEvent & wxcmd )
{
  LOGN("begin")
//  //Call delete to exit this application. (without it is not finished)
//  delete(this) ;
//  this->Destroy() ;
  if( m_getStatisticsThread.IsRunning() )
    wxMessageBox(wxT("can't close because currently getting statistics") );
  else
    Destroy() ;
//  Close() ;
  LOGN("end")
}
