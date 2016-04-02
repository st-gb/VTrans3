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
    enum ColumnIDs { leftChild = 0, ParseRuleName, FilePath};
    enum ContextMenuItemIDs{ ID_CopyRuleFilePathToClipBoard };
    int m_itemIndexOfContextMenu;
 public:
    ParseRulesListCtrl(wxWindow * parent, int ID);
    virtual
    ~ParseRulesListCtrl();

    unsigned GetNumberOfParseRules() const;
    void OnRightClick(wxListEvent & event);
    void OnCopyRuleFilePathToClipBoard(wxCommandEvent & cmdevt);
    wxString OnGetItemText(long item, long column) const;
    wxString GetItemText_noFilter(long int, long int) const;
    
    DECLARE_EVENT_TABLE()
  };

} /* namespace wxWidgets */

#endif /* PARSERULESLISTCTRL_HPP_ */
