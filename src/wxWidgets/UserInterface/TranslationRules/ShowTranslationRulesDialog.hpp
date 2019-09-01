/*
 * ShowTranslationRulesDialog.hpp
 *
 *  Created on: 09.12.2012
 *      Author: Stefan
 */

#ifndef SHOWTRANSLATIONRULESDIALOG_HPP_
#define SHOWTRANSLATIONRULESDIALOG_HPP_

#include <wxWidgets/UserInterface/ShowRulesDialogBase.hpp> //Base class

class Condition;
class ConditionsAndTranslation;
class TranslationRule;

namespace VTrans
{

  class ShowTranslationRulesDialog
    : public /*wxDialog*/ wxWidgets::ShowRulesDialogBase
  {
  public:
    TranslationRule * m_p_translationruleSelected;
    ConditionsAndTranslation * m_p_conditionsandtranslation;

    ShowTranslationRulesDialog(wxWindow * parent/*, wxString choices[],
      int arraySize*/);
    virtual
    ~ShowTranslationRulesDialog();


    void AddTranslation( const ConditionsAndTranslation & cnt);
    void CreateRulesListControl();

    void TestRule();

    void AddConditionToGUI(const Condition & cond);

    void OnSelectListItem(wxListEvent & evt)
    {
      const wxString & label = evt.GetLabel();
//      wxBell();
      ShowConditionsAndTranslation(//evt.GetText()
        label);
    }
    void ShowConditionsAndTranslation(const wxString & text);
  };

} /* namespace VTrans */
#endif /* SHOWTRANSLATIONRULESDIALOG_HPP_ */
