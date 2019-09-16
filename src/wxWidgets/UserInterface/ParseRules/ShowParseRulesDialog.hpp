/* ShowParseRulesDialog.hpp
 *  Created on: 22.03.2014
 *   Author: mr.sys */

#ifndef SHOWPARSERULESDIALOG_HPP_
#define SHOWPARSERULESDIALOG_HPP_

#include <wxWidgets/UserInterface/ShowRulesDialogBase.hpp>
#include <hardware/CPU/fastest_data_type.h>///fastestUnsignedDataType

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
  void UpdateTitle(const fastestUnsignedDataType numOverallRules,
    const fastestUnsignedDataType numFilteredRules);
  };

} /* namespace wxWidgets */

#endif /* SHOWPARSERULESDIALOG_HPP_ */
