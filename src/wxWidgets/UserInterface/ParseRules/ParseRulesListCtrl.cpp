/*
 * ParseRulesListCtrl.cpp
 *
 *  Created on: 22.03.2014
 *      Author: mr.sys
 */

#include <wxWidgets/UserInterface/ParseRules/ParseRulesListCtrl.hpp>
#include <map> //class std::map
#include <wxWidgets/VTransApp.hpp> //wxGetApp()
//wxWidgets::getwxString()
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
//ShowRulesDialogBase::rulesList
#include <wxWidgets/UserInterface/ShowRulesDialogBase.hpp>

#include <wx/menu.h> //class wxMenu
#include <wx/dataobj.h> //wxTextDataObject
#include <wx/clipbrd.h> //wxTheClipboard

namespace wxWidgets
{

  BEGIN_EVENT_TABLE(ParseRulesListCtrl, wxListCtrl)
  //http://stackoverflow.com/questions/14487102/wxwidgets-contextmenu-popup
//    EVT_CONTEXT_MENU(
  EVT_LIST_ITEM_RIGHT_CLICK(
    ShowRulesDialogBase::rulesList,
    ParseRulesListCtrl::OnRightClick)
//  EVT_BUTTON(ID_CopySTPtoClipBoard,
  EVT_MENU(ID_CopyRuleFilePathToClipBoard,
    ParseRulesListCtrl::OnCopyRuleFilePathToClipBoard )
  END_EVENT_TABLE()
  
  ParseRulesListCtrl::ParseRulesListCtrl(wxWindow * parent, int ID)
//    : wxListCtrl(parent, //wxID_ANY
//      ID, wxDefaultPosition, wxDefaultSize,
//      wxLC_REPORT | wxLC_VIRTUAL | wxLC_EDIT_LABELS)
    : wxWidgets::RulesListCtrl(parent, ID)
  {
    // Add first column
    wxListItem col0;
    col0.SetId(leftChild);
    col0.SetText( wxT("left child grammar part") );
    col0.SetWidth(300);
    InsertColumn(leftChild, col0);
    col0.SetId(ParseRuleName);
    col0.SetText( wxT("superordinate grammar part") );
    col0.SetWidth(300);
    InsertColumn(ParseRuleName, col0);
    wxListItem col1;
    col1.SetId(FilePath);
    col1.SetText( wxT("file path") );
    col1.SetWidth(200);
    InsertColumn(FilePath, col1);

    SetItemCount( GetNumberOfParseRules() );
  }
  
  unsigned ParseRulesListCtrl::GetNumberOfParseRules() const {
    const fastestUnsignedDataType numItems = wxGetApp().m_parsebyrise.
      m_stdmap_wGrammarPartID2SuperordinateID.size() + wxGetApp().m_parsebyrise.
      m_stdmultimap_wGrammarPartID2SuperordinateID.size();
    return numItems;
  }

  ParseRulesListCtrl::~ParseRulesListCtrl()
  {
    // TODO Auto-generated destructor stub
  }

  std::string GetRelativeFilePath(unsigned superordinateGrammarRuleID)
  {
    std::map<WORD, std::string>::const_iterator c_iterRuleID2RuleName =
      wxGetApp().m_parsebyrise.m_stdmap_wRuleID2RuleName.find(
      superordinateGrammarRuleID);
    if( c_iterRuleID2RuleName !=
        wxGetApp().m_parsebyrise.m_stdmap_wRuleID2RuleName.end() )
    {
      const std::string grammarRuleName =  c_iterRuleID2RuleName->second;
      std::map<std::string, std::string>::const_iterator
        c_iterGrammarRuleName2filePath =
        //TODO make reference to the std::map in constructor and use here
        wxGetApp().m_configurationHandler.m_std_map_grammarRuleName2filepath.find(
        grammarRuleName
        );
      if( c_iterGrammarRuleName2filePath !=
        //TODO make reference to the std::map in constructor and use here
        wxGetApp().m_configurationHandler.m_std_map_grammarRuleName2filepath.end() )
      {
//                  wxString filePath = //(c_iterGrammarRuleName2filePath->second.c_str() );
//                    GetwxString_Inline(c_iterGrammarRuleName2filePath->second);
        const std::string & std_strGrammarRuleFilePath =
            c_iterGrammarRuleName2filePath->second;
        //return GetwxString_Inline(std_strGrammarRuleFilePath);
        return std_strGrammarRuleFilePath;
      }
    }
    return //wxT("not in rule name-> file path map");
      "not in rule name-> file path map";
  }

  wxString GetFileName(unsigned superordinateGrammarRuleID)
  {
    const std::string & std_strGrammarRuleFilePath = GetRelativeFilePath(
      superordinateGrammarRuleID);
    
    const int lastSlashIndex = std_strGrammarRuleFilePath.rfind('/');
    if( lastSlashIndex != std::string::npos )
    {
      const std::string std_strFileName =
        std_strGrammarRuleFilePath.substr(lastSlashIndex + 1);

      const wxString grammarRuleFileName//(c_iterTranslRule2filePath->second.c_str() );
        = GetwxString_Inline( /*std_strGrammarRuleFilePath*/
          std_strFileName );
//                  grammarRuleFilePath.Last('');
//                  return //filePath.Right(filePath.length() - currWorkDirLen);
//                    fileName;
//                  return filePath.Right(filePath.length() - currWorkDirLen);
      return grammarRuleFileName;
    }
    return wxT("");
  }

bool ParseRulesListCtrl::isFiltered(std::map<WORD,WORD>::const_iterator
  c_parseTreeNodeID2SuperordinateIDiter) const
{
  const fastestUnsignedDataType leftRuleID =
    c_parseTreeNodeID2SuperordinateIDiter->first;
  wxString wxstrLeftGrammarRuleName = wxWidgets::getwxString(
    wxGetApp().m_parsebyrise.GetGrammarPartName(leftRuleID) );
  if(wxstrLeftGrammarRuleName.Contains(m_wxstrFilter) )
    return true;
  return false;
}

  /** Get text if NO filter is applied.*/
  wxString ParseRulesListCtrl::GetItemText(long item, long column) const
  {
	VTrans3::BottomUpParser & bottomUpParser = wxGetApp().m_parsebyrise;
    const std::map<WORD, WORD> & c_r_stdmap_wGrammarPartID2SuperordinateID =
      bottomUpParser./** Stores only left child->superordinate */
      m_stdmap_wGrammarPartID2SuperordinateID;
    std::map<WORD,WORD>::const_iterator c_iter =
      c_r_stdmap_wGrammarPartID2SuperordinateID.begin();

    const int arraySize = c_r_stdmap_wGrammarPartID2SuperordinateID.size();

    fastestUnsignedDataType arrayIndex = 0;
    wxString wxstrGrammarRuleName;
    const fastestUnsignedDataType currWorkDirLen = ::wxGetCwd().length();
    while( c_iter != c_r_stdmap_wGrammarPartID2SuperordinateID.end() )
    {
      if(/**Every string contains the empty string, so only check if filtered
		   if the filter is non-empty.*/ ( ! m_wxstrFilter.IsEmpty()
		 && isFiltered(c_iter) ) || m_wxstrFilter.IsEmpty() )
      {
      if( arrayIndex == item)
      {
        const WORD superordinateGrammarRuleID = c_iter->second;
        switch(column)
        {
          case leftChild :
            {
            const WORD leftRuleID = c_iter->first;
            wxstrGrammarRuleName = wxWidgets::getwxString(
			  bottomUpParser.GetGrammarPartName(leftRuleID) );
            return wxstrGrammarRuleName;
          }
          case ParseRuleName:
            wxstrGrammarRuleName = wxWidgets::getwxString(
              bottomUpParser.GetGrammarPartName(superordinateGrammarRuleID) );
            return wxstrGrammarRuleName;
          case FilePath:
            {
              wxString fileName = GetFileName(superordinateGrammarRuleID);
              return fileName;
            }
        }
      }
      ++ arrayIndex;///Only increment index if the entry is included by filter
      }
      ++ c_iter;
    }
    std::multimap<WORD, WORD>::const_iterator mmap_c_iter = 
      wxGetApp().m_parsebyrise.m_stdmultimap_wGrammarPartID2SuperordinateID.begin();
    while( mmap_c_iter != wxGetApp().m_parsebyrise.m_stdmultimap_wGrammarPartID2SuperordinateID.end() )
    {
      if( arrayIndex == item)
      {
        const WORD superordinateGrammarRuleID = mmap_c_iter->second;
        switch(column)
        {
          case leftChild :
            {
            const WORD leftRuleID = mmap_c_iter->first;
            wxstrGrammarRuleName = wxWidgets::getwxString(
              wxGetApp().m_parsebyrise.GetGrammarPartName(leftRuleID) );
            return wxstrGrammarRuleName;
          }
          case ParseRuleName:
            wxstrGrammarRuleName = wxWidgets::getwxString(
              wxGetApp().m_parsebyrise.GetGrammarPartName(superordinateGrammarRuleID) );
            return wxstrGrammarRuleName;
          case FilePath:
            return GetFileName(superordinateGrammarRuleID);
        }
      }
      ++ mmap_c_iter;
      ++ arrayIndex;
    }
    return wxT("/");
  }
  
  void ParseRulesListCtrl::OnCopyRuleFilePathToClipBoard(wxCommandEvent & cmdevt)
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

  void ParseRulesListCtrl::OnRightClick(wxListEvent & event)
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
      //menu.Append(ID_CopySTPtoClipBoard, wxT("&copy syntax tree path to clipboard"));
//      break;
//    case FilePath:
      menu.Append(ID_CopyRuleFilePathToClipBoard, wxT("&copy file path to clipboard"));
//      break;
//    }
    PopupMenu(&menu, event.GetPoint());
  }

  /** Override virtual method of wxListCtrl to provide text data for virtual list*/
  wxString ParseRulesListCtrl::OnGetItemText(long item, long column) const
  {
//    if( m_wxstrFilter.empty() )
//    {
    return GetItemText(item, column);
//    }
//    else
//      return GetItemText_Filtered(item, column);
//    return wxT("/");
  }

//TODO move to class BottomUpParser (fn name ~ CountFilteredParseRules) ? 
// because may be used for differen UIs
unsigned ParseRulesListCtrl::CountFilteredItems() const
{
  fastestUnsignedDataType numItems = 0;
  VTrans3::BottomUpParser & bottomUpParser = wxGetApp().m_parsebyrise;
//  fastestUnsignedDataType numItems = bottomUpParser.CountFilteredParseRules();
  
  const std::map<WORD, WORD> & c_r_stdmap_wGrammarPartID2SuperordinateID =
    bottomUpParser./** Stores only left child->superordinate */
    m_stdmap_wGrammarPartID2SuperordinateID;
  
  std::map<WORD,WORD>::const_iterator c_iter =
    c_r_stdmap_wGrammarPartID2SuperordinateID.begin();
  for( ; c_iter != c_r_stdmap_wGrammarPartID2SuperordinateID.end(); c_iter++)
  {
    if(isFiltered(c_iter) )
      numItems ++;
  }
  return numItems;
}

void ParseRulesListCtrl::SetFilter(const wxString & filter)
{
  const int previousItemCount = GetItemCount();
  m_wxstrFilter = filter;
  if( filter.empty() )
  {
    SetItemCount( GetNumberOfParseRules() );
  }
  else
  {
    const unsigned numFilteredItems = CountFilteredItems();
//    SetTitle( wxString::Format("parse rules:# overall:$d # filtered:%d",
//      GetNumberOfParseRules(), numFilteredItems) );
    SetItemCount(numFilteredItems);
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
} /* namespace wxWidgets */
