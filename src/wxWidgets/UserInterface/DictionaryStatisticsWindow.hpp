/* 
 * File:   DictionaryStatisticsWindow.hpp
 * Author: mr.sys
 *
 * Created on 13. Oktober 2014, 23:22
 */

#ifndef DICTIONARYSTATISTICSWINDOW_HPP
#define	DICTIONARYSTATISTICSWINDOW_HPP

#include <wx/dialog.h> //base class wxDialog
#include <wx/timer.h>
#include <Attributes/CollectDictStatsThreadFuncParams.h>
#include "Controller/thread_type.hpp"
#include <vector> //class std::vector

class wxTextCtrl;
class wxBoxSizer;

class EnglishWordCount
{
public:
  wxTextCtrl * m_p_wxStaticText;
  enum EnglishWord::English_word_class m_English_word_class;
  EnglishWordCount(
    wxTextCtrl * p_wxTextCtrl, 
    enum EnglishWord::English_word_class English_word_class)
  {
    m_p_wxStaticText = p_wxTextCtrl;
    m_English_word_class = English_word_class;
  }
};

class DictionaryStatisticsWindow
  : public wxDialog
{
  wxTimer m_wxtimer;
  static std::string s_getStatusString;
  std::vector<EnglishWordCount> m_englishWordCountVector;
  VTrans::thread_type m_getStatisticsThread;
//  wxStaticText * m_p_singularNounStaticText;
//  wxStaticText * m_p_pluralNounStaticText;
  wxBoxSizer * m_p_boxsizerRows;
  wxBoxSizer * m_p_boxsizerLeftColumn;
  wxBoxSizer * m_p_boxsizerRightColumn;
  CollectDictStatsThreadFuncParams m_collectDictStatsThreadFuncParams;
public:
  /** IDs for the event table (assignment of event ID -> event handler function).
 *  Use an enum because it guarantees a unique number for each element. */
  enum user_interface_control_actions
  {
    ID_Timer
  };
  DictionaryStatisticsWindow();
  DictionaryStatisticsWindow(const DictionaryStatisticsWindow& orig);
  virtual ~DictionaryStatisticsWindow();
  
  void AddStaticText(
    const wxString & wxstr, 
    enum EnglishWord::English_word_class English_word_class
    /*, wxSizer * p_wxxsizer*/);
  void BuildGUI();
  void OnClose(wxCloseEvent & wxcmd);
  void OnEndedGetDictStats(wxCommandEvent &);
  void EndedGetDictStats();
  void GetStatistics();
  void OnTimerEvent(wxTimerEvent & e);
  void UpdateUI();
//private:
  DECLARE_EVENT_TABLE()
};

BEGIN_DECLARE_EVENT_TYPES()
  DECLARE_LOCAL_EVENT_TYPE(EndedGetDictStatsEvent, wxNewEventType())
END_DECLARE_EVENT_TYPES()

#endif	/* DICTIONARYSTATISTICSWINDOW_HPP */

