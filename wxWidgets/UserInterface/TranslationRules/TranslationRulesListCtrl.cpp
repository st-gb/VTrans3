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
#include <wx/filefn.h> //::wxGetCwd()

/** for wxWidgets::GetwxString_Inline */
using namespace wxWidgets;

namespace VTrans
{

  TranslationRulesListCtrl::TranslationRulesListCtrl(wxWindow * parent, int ID)
//    : wxListCtrl(parent, //wxID_ANY
//      ID, wxDefaultPosition, wxDefaultSize,
//      wxLC_REPORT | wxLC_VIRTUAL | wxLC_EDIT_LABELS)
    : wxWidgets::RulesListCtrl(parent, //wxID_ANY
      ID)
  {
    // Add first column
    wxListItem col0;
    col0.SetId(0);
    col0.SetText( wxT("syntax tree path") );
    col0.SetWidth(500);
    InsertColumn(SyntaxTreePath, col0);
    wxListItem col1;
    col1.SetId(1);
    col1.SetText( wxT("file path") );
    col1.SetWidth(200);
    InsertColumn(FilePath, col1);

    SetItemCount(wxGetApp().
      m_translateparsebyrisetree.
      m_stdmap_p_translationrule2ConditionsAndTranslation.size());
  }

  TranslationRulesListCtrl::~TranslationRulesListCtrl()
  {
    // TODO Auto-generated destructor stub
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
            std::map<TranslationRule *, std::string>::const_iterator
              c_iterTranslRule2filePath =
              wxGetApp().m_std_map_p_translationrule2filepath.find(
              (TranslationRule *) c_p_translationrule);
            if( c_iterTranslRule2filePath !=
              wxGetApp().m_std_map_p_translationrule2filepath.end() )
            {
              wxString filePath = //(c_iterTranslRule2filePath->second.c_str() );
                GetwxString_Inline(c_iterTranslRule2filePath->second);
              return filePath.Right(filePath.length() - currWorkDirLen);
            }
            else
              break;
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
} /* namespace VTrans */
