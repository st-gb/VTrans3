/*
 * ShowParseRulesDialog.cpp
 *
 *  Created on: 22.03.2014
 *      Author: mr.sys
 */

#include "ShowParseRulesDialog.hpp"
#include <wxWidgets/UserInterface/ParseRules/ParseRulesListCtrl.hpp>

namespace wxWidgets
{

  ShowParseRulesDialog::ShowParseRulesDialog(
    wxWindow * parent/*,
    wxString choices[],
    int arraySize*/
    )
    : ShowRulesDialogBase(
       parent //wxWindow * parent
       /*, choices
       , arraySize*/
       , wxT("parse rules") //const wxString & title//,
//       const wxString& name = wxDialogNameStr
      )
  {
    //wxString str = GetTitle();
    SetTitle( wxString::Format(wxT("%u parse rules"), 
      ( (ParseRulesListCtrl*)m_p_rulesListCtrl)->GetNumberOfParseRules() )
      );
    Create(/*choices, arraySize*/);
  }

  ShowParseRulesDialog::~ShowParseRulesDialog()
  {
    // TODO Auto-generated destructor stub
  }

  void ShowParseRulesDialog::CreateRulesListControl()
  {
    m_p_rulesListCtrl = new ParseRulesListCtrl(this, rulesList);
  }
} /* namespace wxWidgets */
