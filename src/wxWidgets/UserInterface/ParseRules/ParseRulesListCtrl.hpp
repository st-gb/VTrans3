/* ParseRulesListCtrl.hpp
 *  Created on: 22.03.2014
 *      Author: mr.sys */

#ifndef PARSERULESLISTCTRL_HPP_
#define PARSERULESLISTCTRL_HPP_

#include "../RulesListCtrl.hpp"///base class wxWidgets::RulesListCtrl
#include <map>///class std::map
#include <Windows.h>///includes windef.h;for WORD
#include <hardware/CPU/fastest_data_type.h>///fastestUnsignedDataType

///Forward declarations
namespace wxWidgets { class ShowParseRulesDialog; }

namespace wxWidgets
{

class ParseRulesListCtrl
  : public wxWidgets::RulesListCtrl
{
    enum ColumnIDs { leftChild = 0/*, rightChild*/, ParseRuleName, FilePath};
    enum ContextMenuItemIDs{ ID_CopyRuleFilePathToClipBoard };
    int m_itemIndexOfContextMenu;
  wxWidgets::ShowParseRulesDialog * dialog;
 public:
    ParseRulesListCtrl(/*wxWindow*/wxWidgets::ShowParseRulesDialog * parent,
      int ID);
    virtual
    ~ParseRulesListCtrl();

  unsigned CountFilteredItems() const;
    unsigned GetNumberOfParseRules() const;
    void OnRightClick(wxListEvent & event);
    void OnCopyRuleFilePathToClipBoard(wxCommandEvent & cmdevt);
    wxString OnGetItemText(long item, long column) const;
    wxString GetItemText(long int, long int) const;
    
  bool isFiltered(const fastestUnsignedDataType leftRuleID) const;
  void SetFilter(const wxString&);
    DECLARE_EVENT_TABLE()
};

} /* namespace wxWidgets */

#endif /* PARSERULESLISTCTRL_HPP_ */
