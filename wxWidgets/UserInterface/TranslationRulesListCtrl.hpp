/*
 * TranslationRulesListCtrl.hpp
 *
 *  Created on: 10.12.2012
 *      Author: Stefan
 */

#ifndef TRANSLATIONRULESLISTCTRL_HPP_
#define TRANSLATIONRULESLISTCTRL_HPP_

#include <wx/listctrl.h> //Base class wxListCtrl

namespace VTrans
{

  class TranslationRulesListCtrl
    : public wxListCtrl
  {
    enum ColumnIDs { SyntaxTreePath = 0, FilePath};
    wxString m_wxstrFilter;
  public:
    TranslationRulesListCtrl(wxWindow * parent, int ID);
    virtual
    ~TranslationRulesListCtrl();

    wxString GetItemText_noFilter(long item, long column) const;
    /**@see http://wiki.wxwidgets.org/WxListCtrl#Minimal_virtual_list_example_to_get_started
    * "Overload virtual method of wxListCtrl to provide text data for virtual list" */
    wxString OnGetItemText(long item, long column) const;

    void SetData(wxString choices[], int arraySize)
    {
    }

    void SetFilter(const wxString & filter);
  };

} /* namespace VTrans */
#endif /* TRANSLATIONRULESLISTCTRL_HPP_ */
