/*
 * ShowRulesDialogBase.hpp
 *
 *  Created on: 22.03.2014
 *      Author: mr.sys
 */

#ifndef SHOWRULESDIALOGBASE_HPP_
#define SHOWRULESDIALOGBASE_HPP_

#include <wx/dialog.h> //class wxDialog
#include <wx/stattext.h> //class wxStaticText

///** class class TranslationRulesListCtrl */
//#include <wxWidgets/UserInterface/TranslationRules/TranslationRulesListCtrl.hpp>
#include <wxWidgets/UserInterface/RulesListCtrl.hpp>
#include <vector> //class std::vector

class TranslationRulesListCtrl;
class wxBoxSizer;
class wxTextCtrl;
namespace wxWidgets
{
  class RulesListCtrl;
}

namespace wxWidgets
{

  class ShowRulesDialogBase
    : public wxDialog
  {
  protected:
    std::vector<wxWindow *> m_std_vec_window;
//    VTrans::TranslationRulesListCtrl * m_p_translationruleslistctrl;
    wxWidgets::RulesListCtrl * m_p_rulesListCtrl;
    wxTextCtrl * m_p_wxtextctrl;
    wxBoxSizer * m_p_wxsizer;
  public:
    enum windowIDs{ ruleTextCtrl, rulesList, testRule };
    ShowRulesDialogBase(
      wxWindow * parent,
      wxString choices[],
      int arraySize,
      const wxString & title);

    virtual
    ~ShowRulesDialogBase();

    void Create(
      wxString choices[],
      int arraySize
      );

    void OnChangeRulePath(wxCommandEvent & event);
    virtual void OnSelectListItem(wxListEvent & evt) { };
    void OnTestRule(wxCommandEvent & evt) { TestRule(); }
    void OnSize(wxSizeEvent & evt)
    {
      Layout();
    }
    virtual void TestRule() = 0;
    virtual void CreateRulesListControl() = 0;

    DECLARE_EVENT_TABLE()
  };

} /* namespace wxWidgets */

#endif /* SHOWRULESDIALOGBASE_HPP_ */
