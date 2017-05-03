/** File:   SettingsDialog.cpp
 * Author: sg
 * Created on 1. Mai 2017, 23:31 */

#include "SettingsDialog.hpp"
#include "Controller/TranslationControllerBase.hpp"
//#include "UserInterface.cpp"
//#include <wxWidgets/VTransApp.hpp>
#include <wx/button.h> //class wxButton
#include <wx/combobox.h> //class wxComboBox
#include <wx/listbox.h> ///class wxlistBox
#include <wx/sizer.h>
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
//#include <wxWidgets/VTransApp.hpp>
extern TranslationControllerBase * g_p_translationcontrollerbase;

BEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
  EVT_LISTBOX(settingsNameListBox, SettingsDialog::OnSettingsNameSelected )
  EVT_BUTTON(applySettingButton, SettingsDialog::OnApplySetting )
END_EVENT_TABLE()

SettingsDialog::SettingsDialog()
  : wxDialog(
      NULL,
      wxID_ANY,
      wxT("settings"), //const wxString& title
      wxDefaultPosition, //const wxPoint& pos = wxDefaultPosition,
      wxDefaultSize, //const wxSize& size = wxDefaultSize,
      wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
      )
{
  BuildGUI();
}

void SettingsDialog::BuildGUI()
{
  wxBoxSizer * p_boxsizer = new wxBoxSizer(wxVERTICAL);
  SetSizer(p_boxsizer);
  m_p_settingNameListBox = new wxListBox(this, settingsNameListBox);

  std::map<std::string, ValuesAndFunction>::const_iterator c_iter = 
    g_p_translationcontrollerbase->s_settingsName2valueAndFunction.begin();
  int i = 0;
  while( c_iter != g_p_translationcontrollerbase->
    s_settingsName2valueAndFunction.end() )
  {
    m_p_settingNameListBox->Insert(wxWidgets::GetwxString_Inline(c_iter->first), i );
    c_iter ++;
    i ++;
  }
  p_boxsizer->Add( m_p_settingNameListBox, 
    1 //1=stretchable in sizer's orientation
    , wxEXPAND //wxEXPAND=stretchable);
  m_p_settingValueComboBox = new wxComboBox(this, wxID_ANY);
  p_boxsizer->Add( m_p_settingValueComboBox, 
    0 //stretchable in sizer's orientation
    , wxEXPAND
    );
  p_boxsizer->Add( new wxButton(this, applySettingButton, wxT("apply") ) );
}

SettingsDialog::SettingsDialog(const SettingsDialog& orig) {
}

SettingsDialog::~SettingsDialog() {
}

void SettingsDialog::OnApplySetting(wxCommandEvent & evt)
{
  wxString wxstrSettingName = m_p_settingNameListBox->GetStringSelection();
  wxString wxstrSettingValue = m_p_settingValueComboBox->GetValue();
  std::string std_strSettingName = wxWidgets::GetStdString(wxstrSettingName);
  std::string std_strSettingValue = wxWidgets::GetStdString(wxstrSettingValue);
  g_p_translationcontrollerbase->Settings(
    std_strSettingName.c_str(), 
    std_strSettingValue.c_str()
    );
}

void SettingsDialog::OnSettingsNameSelected(wxCommandEvent & evt)
{
  const int selectionIndex = evt.GetSelection();
  m_p_settingValueComboBox->Clear(); /** Remove all entries. */
  
  std::map<std::string, ValuesAndFunction>::const_iterator c_iter = 
    g_p_translationcontrollerbase->s_settingsName2valueAndFunction.begin();
  int in = 0;
  while( c_iter != g_p_translationcontrollerbase->
    s_settingsName2valueAndFunction.end() )
  {
    if( in == selectionIndex)
    {
//      char * settingValues = c_iter->second.values;
      /** strtok(...) modifies its argument. So make a copy first. */
      std::string std_strSettingValues = c_iter->second.values;
      const char * settingValues = std_strSettingValues.c_str();
      //http://www.cplusplus.com/reference/cstring/strtok/
      char * settingValue = strtok( (char*) settingValues,"\t");// settingValues;
//      str
      while (settingValue != NULL)
      {
        m_p_settingValueComboBox->Append(wxWidgets::GetwxString_Inline(settingValue) );
        //http://www.cplusplus.com/reference/cstring/strchr/
        settingValue = strtok (NULL, "\t");// strchr(settingValue + 1, '\t');
      }//while (settingValue != NULL);
//      std::string v(values);
//      v.fin
      break;
    }
    in ++;
    c_iter ++;
  }
}
