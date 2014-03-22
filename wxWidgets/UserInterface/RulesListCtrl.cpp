/*
 * RulesListCtrl.cpp
 *
 *  Created on: 22.03.2014
 *      Author: mr.sys
 */

#include <wxWidgets/UserInterface/RulesListCtrl.hpp>

namespace wxWidgets
{

  RulesListCtrl::RulesListCtrl(wxWindow * parent, wxWindowID ID)
    : wxListCtrl(
      parent, //wxID_ANY
      ID, wxDefaultPosition, wxDefaultSize,
      wxLC_REPORT | wxLC_VIRTUAL | wxLC_EDIT_LABELS)
  {
    // TODO Auto-generated constructor stub

  }

  RulesListCtrl::~RulesListCtrl()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace wxWidgets */
