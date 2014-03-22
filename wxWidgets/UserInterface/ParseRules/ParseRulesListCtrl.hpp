/*
 * ParseRulesListCtrl.hpp
 *
 *  Created on: 22.03.2014
 *      Author: mr.sys
 */

#ifndef PARSERULESLISTCTRL_HPP_
#define PARSERULESLISTCTRL_HPP_

#include "../RulesListCtrl.hpp"

namespace wxWidgets
{

  class ParseRulesListCtrl
    : public wxWidgets::RulesListCtrl
  {
    enum ColumnIDs { ParseRuleName = 0, FilePath};
  public:
    ParseRulesListCtrl(wxWindow * parent, int ID);
    virtual
    ~ParseRulesListCtrl();

    wxString OnGetItemText(long item, long column) const;
    wxString GetItemText_noFilter(long int, long int) const;
  };

} /* namespace wxWidgets */

#endif /* PARSERULESLISTCTRL_HPP_ */
