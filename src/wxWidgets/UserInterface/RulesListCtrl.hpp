/*
 * RulesListCtrl.hpp
 *
 *  Created on: 22.03.2014
 *      Author: mr.sys
 */

#ifndef RULESLISTCTRL_HPP_
#define RULESLISTCTRL_HPP_

#include <wx/listctrl.h> //Base class wxListCtrl

class wxWindow;

namespace wxWidgets
{

  class RulesListCtrl
    : public wxListCtrl
  {
  protected:
    wxString m_wxstrFilter;
  public:
    RulesListCtrl(wxWindow * parent, wxWindowID ID);
    virtual
    ~RulesListCtrl();

    /**@see http://wiki.wxwidgets.org/WxListCtrl#Minimal_virtual_list_example_to_get_started
    * "Overload virtual method of wxListCtrl to provide text data for virtual list" */
    virtual wxString OnGetItemText(long item, long column) const { return wxT(""); }
    virtual wxString GetItemText_noFilter(long item, long column) const { return wxT(""); }
    virtual void SetFilter(const wxString & filter) { };
  };

} /* namespace wxWidgets */

#endif /* RULESLISTCTRL_HPP_ */
