/*
 * ParseRulesListCtrl.hpp
 *
 *  Created on: 22.03.2014
 *      Author: mr.sys
 */

#ifndef PARSERULESLISTCTRL_HPP_
#define PARSERULESLISTCTRL_HPP_

#include "../RulesListCtrl.hpp"
#include <map>///class std::map
#include <Windows.h>///includes windef.h;for WORD

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

  unsigned CountFilteredItems() const;
    unsigned GetNumberOfParseRules() const;
    void OnRightClick(wxListEvent & event);
    void OnCopyRuleFilePathToClipBoard(wxCommandEvent & cmdevt);
    wxString OnGetItemText(long item, long column) const;
    wxString GetItemText(long int, long int) const;
    
  bool isFiltered(std::map<WORD,WORD>::const_iterator
    c_parseTreeNodeID2SuperordinateIDiter) const;
  void SetFilter(const wxString&);
    DECLARE_EVENT_TABLE()
  };

} /* namespace wxWidgets */

#endif /* PARSERULESLISTCTRL_HPP_ */
