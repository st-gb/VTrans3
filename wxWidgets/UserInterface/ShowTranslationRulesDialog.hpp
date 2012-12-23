/*
 * ShowTranslationRulesDialog.hpp
 *
 *  Created on: 09.12.2012
 *      Author: Stefan
 */

#ifndef SHOWTRANSLATIONRULESDIALOG_HPP_
#define SHOWTRANSLATIONRULESDIALOG_HPP_

#include <wx/dialog.h> //class wxDialog
#include <wx/stattext.h> //class wxStaticText
#include "TranslationRulesListCtrl.hpp" //class TranslationRulesListCtrl
#include <vector> //class std::vector

class Condition;
class ConditionsAndTranslation;
class TranslationRule;
class TranslationRulesListCtrl;
class wxTextCtrl;
namespace VTrans
{
  class TranslationRulesListCtrl;
}

namespace VTrans
{

  class ShowTranslationRulesDialog
    : public wxDialog
  {
    enum windowIDs{ translRuleTextCtrl, translRuleList, TestTranslRule };
    VTrans::TranslationRulesListCtrl * m_p_translationruleslistctrl;
    wxTextCtrl * m_p_wxtextctrl;
    wxBoxSizer * m_p_wxsizer;
    std::vector<wxWindow *> m_std_vec_window;
  public:
    TranslationRule * m_p_translationruleSelected;
    ConditionsAndTranslation * m_p_conditionsandtranslation;
    ShowTranslationRulesDialog(wxWindow * parent, wxString choices[],
      int arraySize);
    virtual
    ~ShowTranslationRulesDialog();

    void AddTranslation( const ConditionsAndTranslation & cnt);
    void OnSize(wxSizeEvent & evt)
    {
      Layout();
    }
    void OnTestTranslRule(wxCommandEvent & evt);
    void ChangeTranslRulePath(wxCommandEvent & event)
    {
      const wxString & wxstrText = m_p_wxtextctrl->//GetLineText (0
        ///*long lineNo*/);
        GetValue();
      m_p_translationruleslistctrl->SetFilter(wxstrText);
    }
    void SelectSyntaxTreePath(wxListEvent & evt)
    {
      const wxString & label = evt.GetLabel();
//      wxBell();
      ShowConditionsAndTranslation(//evt.GetText()
        label);
    }

    void AddConditionToGUI(const Condition & cond);

    void ShowConditionsAndTranslation(const wxString & text);
    DECLARE_EVENT_TABLE()
  };

} /* namespace VTrans */
#endif /* SHOWTRANSLATIONRULESDIALOG_HPP_ */
