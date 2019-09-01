/*
 * ShowParseRulesDialog.hpp
 *
 *  Created on: 22.03.2014
 *      Author: mr.sys
 */

#ifndef SHOWPARSERULESDIALOG_HPP_
#define SHOWPARSERULESDIALOG_HPP_

#include <wxWidgets/UserInterface/ShowRulesDialogBase.hpp>

namespace wxWidgets
{

  class ShowParseRulesDialog
    : public wxWidgets::ShowRulesDialogBase
  {
  public:
    ShowParseRulesDialog(
      wxWindow * parent/*,
      wxString choices[],
      int arraySize*/
      );
    virtual
    ~ShowParseRulesDialog();
    void CreateRulesListControl();
    void TestRule() { }
  };

} /* namespace wxWidgets */

#endif /* SHOWPARSERULESDIALOG_HPP_ */
