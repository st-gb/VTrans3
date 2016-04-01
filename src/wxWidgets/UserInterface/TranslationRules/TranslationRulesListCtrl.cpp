/*
 * TranslationRulesListCtrl.cpp
 *
 *  Created on: 10.12.2012
 *      Author: Stefan
 */

//Class TranslationRulesListCtrl
#include "TranslationRulesListCtrl.hpp"
#include <map> //std::map
#include <Translate/TranslationRule.hpp> //class TranslationRule
//class ConditionsAndTranslation
#include <Translate/ConditionsAndTranslation.hpp>
#include <wxWidgets/VTransApp.hpp> //wxGetApp()
//wxWidgets::getwxString()
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
//ShowRulesDialogBase::rulesList
#include <wxWidgets/UserInterface/ShowRulesDialogBase.hpp>

#include <wx/clipbrd.h> //wxTheClipboard
#include <wx/dataobj.h> //wxTextDataObject
#include <wx/filefn.h> //::wxGetCwd()
#include <wx/menu.h> //class wxMenu

/** for wxWidgets::GetwxString_Inline */
using namespace wxWidgets;

namespace VTrans
{

  BEGIN_EVENT_TABLE(TranslationRulesListCtrl, wxListCtrl)
  //http://stackoverflow.com/questions/14487102/wxwidgets-contextmenu-popup
//    EVT_CONTEXT_MENU(
  EVT_LIST_ITEM_RIGHT_CLICK(
    ShowRulesDialogBase::rulesList,
    TranslationRulesListCtrl::OnRightClick)
//  EVT_BUTTON(ID_CopySTPtoClipBoard,
  EVT_MENU(ID_CopySTPtoClipBoard,
    TranslationRulesListCtrl::OnCopySTPtoClipBoard )
  EVT_MENU(ID_CopyRuleFilePathToClipBoard,
    TranslationRulesListCtrl::OnCopyRuleFilePathToClipBoard )
  END_EVENT_TABLE()

  TranslationRulesListCtrl::TranslationRulesListCtrl(wxWindow * parent, int ID)
//    : wxListCtrl(parent, //wxID_ANY
//      ID, wxDefaultPosition, wxDefaultSize,
//      wxLC_REPORT | wxLC_VIRTUAL | wxLC_EDIT_LABELS)
    : wxWidgets::RulesListCtrl(parent, //wxID_ANY
      ID)
  {
    // Add first column
    wxListItem syntaxTreePathColumn;
    syntaxTreePathColumn.SetId(SyntaxTreePath);
    syntaxTreePathColumn.SetText( wxT("syntax tree path") );
    syntaxTreePathColumn.SetWidth(500);
    InsertColumn(SyntaxTreePath, syntaxTreePathColumn);

    wxListItem translationRuleFilePathColumn;
    translationRuleFilePathColumn.SetId(FilePath);
    translationRuleFilePathColumn.SetText( wxT("file path") );
    translationRuleFilePathColumn.SetWidth(200);
    InsertColumn(FilePath, translationRuleFilePathColumn);

    SetItemCount(wxGetApp().
      m_translateparsebyrisetree.
      m_stdmap_p_translationrule2ConditionsAndTranslation.size());
  }

  TranslationRulesListCtrl::~TranslationRulesListCtrl()
  {
    // TODO Auto-generated destructor stub
  }

  wxString GetRelativeFilePath(const TranslationRule * c_p_translationrule)
  {
    std::map<TranslationRule *, std::string>::const_iterator
      c_iterTranslRule2filePath =
      wxGetApp().m_std_map_p_translationrule2filepath.find(
      (TranslationRule *) c_p_translationrule);
    if( c_iterTranslRule2filePath !=
      wxGetApp().m_std_map_p_translationrule2filepath.end() )
    {
//      wxString filePath = //(c_iterTranslRule2filePath->second.c_str() );
//        GetwxString_Inline(c_iterTranslRule2filePath->second);
//      return filePath.Right(filePath.length() - currWorkDirLen);
      
      const std::string & std_strTranslationRuleFilePath =
        c_iterTranslRule2filePath->second.c_str();

      const int lastSlashIndex = std_strTranslationRuleFilePath.rfind('/');
      const std::string std_strTranslationRuleFileName =
        std_strTranslationRuleFilePath.substr(lastSlashIndex + 1);

      const wxString translationRuleFileName//(c_iterTranslRule2filePath->second.c_str() );
        = GetwxString_Inline( /*std_strGrammarRuleFilePath*/
          std_strTranslationRuleFileName );
      return translationRuleFileName;
    }
    //else
    //  break;
    return wxT("/");
  }
  
  /** Get text if NO filter is applied.*/
  wxString TranslationRulesListCtrl::GetItemText_noFilter(long item,
    long column) const
  {
    wxString wxstrSyntaxTreePath;
    std::map<TranslationRule *,ConditionsAndTranslation> &
      r_stdmap_p_translationrule2ConditionsAndTranslation = wxGetApp().
      m_translateparsebyrisetree.
      m_stdmap_p_translationrule2ConditionsAndTranslation;
    std::map<TranslationRule *,ConditionsAndTranslation>::const_iterator
      c_iterTranslationRule =
      r_stdmap_p_translationrule2ConditionsAndTranslation.begin();

    unsigned arrayIndex = 0;
    unsigned currWorkDirLen = ::wxGetCwd().length();
    while( c_iterTranslationRule !=
      r_stdmap_p_translationrule2ConditionsAndTranslation.end() )
    {
      if( arrayIndex == item)
      {
        const TranslationRule * c_p_translationrule =
          c_iterTranslationRule->first;
        wxstrSyntaxTreePath = GetwxString_Inline( c_p_translationrule->
          m_syntaxtreepathCompareWithCurrentPath.GetAs_std_string() );
        switch(column)
        {
          case SyntaxTreePath:
            return wxstrSyntaxTreePath;
          case FilePath:
            return GetRelativeFilePath(c_p_translationrule);
        }
      }
      ++ c_iterTranslationRule;
      ++ arrayIndex;
    }
    return wxT("/");
  }

  //Overload virtual method of wxListCtrl to provide text data for virtual list
  wxString TranslationRulesListCtrl::OnGetItemText(long item, long column) const
  {
    wxString wxstr, wxstrSyntaxTreePath;
    if( m_wxstrFilter.empty() )
    {
      return GetItemText_noFilter(item, column);
    }
    else
    {
      std::map<TranslationRule *,ConditionsAndTranslation> &
        r_stdmap_p_translationrule2ConditionsAndTranslation = wxGetApp().
        m_translateparsebyrisetree.
        m_stdmap_p_translationrule2ConditionsAndTranslation;
  //    int arraySize = r_stdmap_p_translationrule2ConditionsAndTranslation.size();
  //    wxString choices[arraySize];
      std::map<TranslationRule *,ConditionsAndTranslation>::const_iterator
        c_iter = r_stdmap_p_translationrule2ConditionsAndTranslation.begin();
      unsigned arrayIndex = 0;
      unsigned currWorkDirLen = ::wxGetCwd().length();
      while( c_iter != r_stdmap_p_translationrule2ConditionsAndTranslation.end() )
      {
        const TranslationRule * c_p_translationrule =
          c_iter->first;
    //      r_translationrule.mp_parsebyrise->Get
        wxstrSyntaxTreePath = getwxString( c_p_translationrule->
          m_syntaxtreepathCompareWithCurrentPath.GetAs_std_string() );
        if( wxstrSyntaxTreePath.Contains(m_wxstrFilter) )
        {
          if( item == arrayIndex )
            switch(column)
            {
              case SyntaxTreePath:
                return wxstrSyntaxTreePath;
              case FilePath:
                std::map<TranslationRule *, std::string>::const_iterator
                  c_iterTranslRule2filePath =
                  wxGetApp().m_std_map_p_translationrule2filepath.find(
                  (TranslationRule *) c_p_translationrule);
                if( c_iterTranslRule2filePath !=
                  wxGetApp().m_std_map_p_translationrule2filepath.end() )
                {
                  const std::string & std_strGrammarRuleFilePath =
                    c_iterTranslRule2filePath->second.c_str();

                  const int lastSlashIndex = std_strGrammarRuleFilePath.rfind('/');
                  const std::string std_strGrammarRuleFileName =
                    std_strGrammarRuleFilePath.substr(lastSlashIndex + 1);

                  const wxString grammarRuleFileName//(c_iterTranslRule2filePath->second.c_str() );
                    = GetwxString_Inline( /*std_strGrammarRuleFilePath*/
                      std_strGrammarRuleFileName );
//                  grammarRuleFileName.Last('');
                  return //filePath.Right(filePath.length() - currWorkDirLen);
                    //getwxString(std_strGrammarRuleFileName.c_str() );
                    grammarRuleFileName;
                }
                else
                  break;
            }
          ++ arrayIndex;
        }
        ++ c_iter;
      }
    }
    return wxT("/");
  }

  void TranslationRulesListCtrl::SetFilter(const wxString & filter)
  {
    m_wxstrFilter = filter;
    int previousItemCount = GetItemCount();
    if( filter.empty() )
    {
      SetItemCount( wxGetApp().m_translateparsebyrisetree.
        m_stdmap_p_translationrule2ConditionsAndTranslation.size() );
    }
    else
    {
      wxString wxstr, wxstrSyntaxTreePath;
      std::map<TranslationRule *,ConditionsAndTranslation> &
        r_stdmap_p_translationrule2ConditionsAndTranslation = wxGetApp().
        m_translateparsebyrisetree.
        m_stdmap_p_translationrule2ConditionsAndTranslation;
  //    int arraySize = r_stdmap_p_translationrule2ConditionsAndTranslation.size();
  //    wxString choices[arraySize];
      std::map<TranslationRule *,ConditionsAndTranslation>::const_iterator c_iter =
          r_stdmap_p_translationrule2ConditionsAndTranslation.begin();
      unsigned arrayIndex = 0;
      while( c_iter != r_stdmap_p_translationrule2ConditionsAndTranslation.end() )
      {
        const TranslationRule * c_p_translationrule =
          c_iter->first;
    //      r_translationrule.mp_parsebyrise->Get
        wxstrSyntaxTreePath = getwxString( c_p_translationrule->
          m_syntaxtreepathCompareWithCurrentPath.GetAs_std_string() );
        if( wxstrSyntaxTreePath.Contains(m_wxstrFilter) )
        {
  //          if( item == arrayIndex )
  //            return wxstrSyntaxTreePath;
          ++ arrayIndex;
        }
        ++ c_iter;
      }
      //This should reflect your data
      SetItemCount(arrayIndex);
    }
    //Avoid ../src/generic/listctrl.cpp(2640): assert "lineTo < GetItemCount()"
    //failed in RefreshLines(): invalid line range
    if( previousItemCount < GetItemCount() )
    {
      //The contents are not visually updated after a filter change
      // (but if the is redrawn).
      RefreshItems(0,//lineFrom
        previousItemCount /*lineTo*/ );
    }
  }

  void TranslationRulesListCtrl::OnCopySTPtoClipBoard(wxCommandEvent & cmdevt)
  {
    wxString wxstr = OnGetItemText(m_itemIndexOfContextMenu, SyntaxTreePath);
    //http://docs.wxwidgets.org/trunk/classwx_clipboard.html
    if (wxTheClipboard->Open())
    {
      // This data objects are held by the clipboard,
      // so do not delete them in the app.
      wxTheClipboard->SetData( new wxTextDataObject(wxstr) );
      wxTheClipboard->Close();
    }
  }

  void TranslationRulesListCtrl::OnCopyRuleFilePathToClipBoard(wxCommandEvent & cmdevt)
  {
    wxString wxstr = OnGetItemText(m_itemIndexOfContextMenu, FilePath);
    //http://docs.wxwidgets.org/trunk/classwx_clipboard.html
    if (wxTheClipboard->Open())
    {
      // This data objects are held by the clipboard,
      // so do not delete them in the app.
      wxTheClipboard->SetData( new wxTextDataObject(wxstr) );
      wxTheClipboard->Close();
    }
  }

  void TranslationRulesListCtrl::OnRightClick(wxListEvent & event)
  {
    //from http://stackoverflow.com/questions/14487102/wxwidgets-contextmenu-popup
    // Show popupmenu at position
    wxMenu menu(wxT("Test"));
    //TODO implement copying to clipboard
    const int col = event.GetColumn();
    m_itemIndexOfContextMenu = event.GetIndex();
//    switch( col)
//    {
//    case SyntaxTreePath:
      menu.Append(ID_CopySTPtoClipBoard, wxT("&copy syntax tree path to clipboard"));
//      break;
//    case FilePath:
      menu.Append(ID_CopyRuleFilePathToClipBoard, wxT("&copy file path to clipboard"));
//      break;
//    }
    PopupMenu(&menu, event.GetPoint());
  }
} /* namespace VTrans */
