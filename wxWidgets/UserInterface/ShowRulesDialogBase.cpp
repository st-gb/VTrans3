/*
 * ShowRulesDialog.cpp
 *
 *  Created on: 22.03.2014
 *      Author: mr.sys
 */

#include <wxWidgets/UserInterface/ShowRulesDialogBase.hpp>
//GetStdString(wxString &);
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>

#include <wx/sizer.h> //class wxSizer
#include <wx/button.h> //class wxButton, EVT_BUTTON
#include <wx/textctrl.h> //class wxTextCtrl, EVT_TEXT
#include <wx/listbox.h> //class wxListbox
#include <wx/combobox.h> //class wxCombobox

namespace wxWidgets
{
//  using namespace VTrans;
  BEGIN_EVENT_TABLE(ShowRulesDialogBase, wxDialog)
    EVT_SIZE(ShowRulesDialogBase::OnSize)
    EVT_TEXT(
      ShowRulesDialogBase::ruleTextCtrl,
      ShowRulesDialogBase::OnChangeRulePath)
    EVT_LIST_ITEM_SELECTED(
      ShowRulesDialogBase::rulesList,
      ShowRulesDialogBase::OnSelectListItem)
    EVT_BUTTON(ShowRulesDialogBase::testRule,
      ShowRulesDialogBase::OnTestRule)
  END_EVENT_TABLE()

  ShowRulesDialogBase::ShowRulesDialogBase(
    wxWindow * parent,
    wxString choices[],
    int arraySize,
    const wxString & title
    )
    : wxDialog(
      parent //wxWindow * parent
       ,wxID_ANY //wxWindowID id
       , title //const wxString & title//,
       , wxDefaultPosition //const wxPoint& pos = wxDefaultPosition,
       , wxDefaultSize //const wxSize& size = wxDefaultSize,
//       long style = wxDEFAULT_DIALOG_STYLE,
       , wxRESIZE_BORDER | wxDEFAULT_DIALOG_STYLE
//       const wxString& name = wxDialogNameStr
      )
  {
    // TODO Auto-generated constructor stub

  }

  ShowRulesDialogBase::~ShowRulesDialogBase()
  {
    // TODO Auto-generated destructor stub
  }

  void ShowRulesDialogBase::Create(
    wxString choices[],
    int arraySize
    )
  {
    CreateRulesListControl();
//    m_p_translationruleslistctrl->SetData(choices, arraySize);

    m_p_wxsizer = new wxBoxSizer(wxVERTICAL);
  //    wxComboBox * p_wxcombobox = new wxComboBox(this, wxID_ANY, "",
  //      wxDefaultPosition, wxDefaultSize, arraySize, choices);
  //    wxListBox * p_wxlistbox = new wxListBox(this, wxID_ANY,
  //      wxDefaultPosition, wxDefaultSize, arraySize, choices,
  //      wxLB_HSCROLL | wxLB_NEEDED_SB);
  //    wxListCtrl * p_wxlistctrl = new wxListCtrl();
    m_p_wxtextctrl = new wxTextCtrl(this, ruleTextCtrl);
    wxButton * p_wxbuttonTest = new wxButton( this, testRule,
      wxT("test"));
    p_wxbuttonTest->SetToolTip( wxT("test if rule applies to current parse "
      "tree") );
    wxBoxSizer * p_wxsizer = new wxBoxSizer(wxHORIZONTAL);
  //    m_p_wxsizer->Add(
    p_wxsizer->Add(
  //      p_wxcombobox
      m_p_wxtextctrl
      //new wxText
  ////      //http://docs.wxwidgets.org/2.6/wx_wxsizer.html#wxsizeradd:
  ////      //[...]can change its size in the main orientation of the wxBoxSizer -
  ////      //where 0 stands for not changeable[...]
        , 1
        , //wxEXPAND |
          wxALL
  //      //wxFIXED_MINSIZE //| wxLEFT | wxRIGHT
  //      wxALIGN_CENTER_VERTICAL
        , 0
//      //from http://docs.wxwidgets.org/2.8/wx_sizeroverview.html#boxsizerprogramming
//      , wxSizerFlags(1)//.Expand()
      );
    p_wxsizer->Add(p_wxbuttonTest, 0);
    p_wxsizer->Layout();
  //    m_p_wxsizer->Add(p_wxbuttonTest);
    m_p_wxsizer->Add(p_wxsizer, 0, wxEXPAND);
  //    m_p_wxsizer->Add( p_wxlistbox
  //      //http://docs.wxwidgets.org/2.6/wx_wxsizer.html#wxsizeradd:
  //      //[...]can change its size in the main orientation of the wxBoxSizer -
  //      //where 0 stands for not changeable[...]
  //      , 1
  //      , wxEXPAND |
  //      //wxFIXED_MINSIZE |
  //      wxALL
  //      , 0
  //      );
    m_p_wxsizer->Add(
      //p_wxlistctrl
      m_p_rulesListCtrl
      //http://docs.wxwidgets.org/2.6/wx_wxsizer.html#wxsizeradd:
      //[...]can change its size in the main orientation of the wxBoxSizer -
      //where 0 stands for not changeable[...]
      , 1
      , wxEXPAND |
      //wxFIXED_MINSIZE |
      wxALL
      , 0
      );
    SetSizer(m_p_wxsizer);
    Layout();
    SetAutoLayout(true /*bool autoLayout*/);
  }

  void ShowRulesDialogBase::OnChangeRulePath(wxCommandEvent & event)
  {
    const wxString & wxstrText = m_p_wxtextctrl->//GetLineText (0
      ///*long lineNo*/);
      GetValue();
#ifdef _DEBUG
    const std::string & std_strText = wxWidgets::GetStdString(wxstrText);
#endif
    m_p_rulesListCtrl->SetFilter(wxstrText);
  }
} /* namespace wxWidgets */
