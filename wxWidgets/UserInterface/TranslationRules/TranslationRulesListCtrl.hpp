/*
 * TranslationRulesListCtrl.hpp
 *
 *  Created on: 10.12.2012
 *      Author: Stefan
 */

#ifndef TRANSLATIONRULESLISTCTRL_HPP_
#define TRANSLATIONRULESLISTCTRL_HPP_

#include "../RulesListCtrl.hpp" //Base class

namespace VTrans
{

  class TranslationRulesListCtrl
    : public /*wxListCtrl*/ wxWidgets::RulesListCtrl
  {
    enum ColumnIDs { SyntaxTreePath = 0, FilePath};
    enum ContextMenuItemIDs{ ID_CopySTPtoClipBoard = 5, ID_CopyRuleFilePathToClipBoard };
  public:
    int m_itemIndexOfContextMenu;
    TranslationRulesListCtrl(wxWindow * parent, int ID);
    virtual
    ~TranslationRulesListCtrl();

    void SetData(wxString choices[], int arraySize)
    {
    }
    wxString OnGetItemText(long item, long column) const;
    void SetFilter(const wxString&);
    wxString GetItemText_noFilter(long item, long column) const;
    void OnRightClick(wxListEvent & evt);
    void OnCopySTPtoClipBoard(wxCommandEvent &);
    void OnCopyRuleFilePathToClipBoard(wxCommandEvent &);

    DECLARE_EVENT_TABLE()
  };

} /* namespace VTrans */
#endif /* TRANSLATIONRULESLISTCTRL_HPP_ */
