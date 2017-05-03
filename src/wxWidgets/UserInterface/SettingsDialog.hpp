/** File:   SettingsDialog.hpp
 * Author: sg
 * Created on 1. Mai 2017, 23:31  */

#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <wx/dialog.h>

class wxComboBox;
class wxListBox;

class SettingsDialog
  : public wxDialog
{
public:
  enum IDs { settingsNameListBox = 1, applySettingButton};
  SettingsDialog();
  SettingsDialog(const SettingsDialog& orig);
  virtual ~SettingsDialog();
  void BuildGUI();
private:
  wxListBox * m_p_settingNameListBox;
  wxComboBox * m_p_settingValueComboBox;

  void OnSettingsNameSelected(wxCommandEvent& );
  void OnApplySetting(wxCommandEvent & evt);
  DECLARE_EVENT_TABLE()
};

#endif /* SETTINGSDIALOG_HPP */

