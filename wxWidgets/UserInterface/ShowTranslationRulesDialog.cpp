/*
 * ShowTranslationRulesDialog.cpp
 *
 *  Created on: 09.12.2012
 *      Author: Stefan
 */

#include "wxWidgets/UserInterface/ShowTranslationRulesDialog.hpp"
#include <wx/sizer.h> //class wxSizer
#include <wx/button.h> //class wxButton
#include <wx/combobox.h> //class wxCombobox
#include <wx/listbox.h> //class wxListbox
#include <wx/textctrl.h> //class wxTextCtrl
//class TranslationAndGrammarPart
#include <Attributes/TranslationAndGrammarPart.hpp>
//convertToStdString(...)
#include <Controller/character_string/convertFromAndToStdString.hpp>
#include "TranslationRulesListCtrl.hpp" //class class TranslationRulesListCtrl
#include <Translate/Condition.hpp> //class Condition
//class ConditionsAndTranslation
#include <Translate/ConditionsAndTranslation.hpp>
//class ParseTreeTraverser::TestIfTranslationRuleApplies
#include <Translate/TestIfTranslationRuleApplies.hpp>
#include <Translate/TranslationRule.hpp> //class TranslationRule
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>
#include <wxWidgets/VTransApp.hpp> //wxGetApp()
//getwxString()
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
#include <map> //class std::map

namespace VTrans
{
  using namespace VTrans;
  BEGIN_EVENT_TABLE(ShowTranslationRulesDialog, wxDialog)
    EVT_SIZE(ShowTranslationRulesDialog::OnSize)
    EVT_TEXT(ShowTranslationRulesDialog::translRuleTextCtrl,
      ShowTranslationRulesDialog::ChangeTranslRulePath)
    EVT_LIST_ITEM_SELECTED(ShowTranslationRulesDialog::translRuleList,
      ShowTranslationRulesDialog::SelectSyntaxTreePath)
    EVT_BUTTON(ShowTranslationRulesDialog::TestTranslRule,
      ShowTranslationRulesDialog::OnTestTranslRule)
  END_EVENT_TABLE()

  ShowTranslationRulesDialog::ShowTranslationRulesDialog(
    wxWindow * parent,
    wxString choices[],
    int arraySize
    )
    : wxDialog(
      parent //wxWindow * parent
       ,wxID_ANY //wxWindowID id
       , wxT("translation rules") //const wxString & title//,
       , wxDefaultPosition //const wxPoint& pos = wxDefaultPosition,
       , wxDefaultSize //const wxSize& size = wxDefaultSize,
//       long style = wxDEFAULT_DIALOG_STYLE,
       , wxRESIZE_BORDER | wxDEFAULT_DIALOG_STYLE
//       const wxString& name = wxDialogNameStr
      )
  {
    Create(choices, arraySize);
  }

  ShowTranslationRulesDialog::~ShowTranslationRulesDialog()
  {
    // TODO Auto-generated destructor stub
  }

  void ShowTranslationRulesDialog::AddConditionToGUI(const Condition & cond)
  {
    wxStaticText * p_wxstatictext = new wxStaticText( this, wxID_ANY,
      wxT("condition:") +
      getwxString(cond.m_syntaxtreepath.GetAs_std_string() + " " +
        cond.m_stdstrAttributeName + "=" +
        convertToStdString<WORD>(cond.m_byAttributeValue) )
      );
//      m_std_vecConditionControls.push_back(p_wxstatictext);
    m_p_wxsizer->Add( p_wxstatictext);
    m_std_vec_window.push_back(p_wxstatictext);
  }

  void ShowTranslationRulesDialog::AddTranslation(
    const ConditionsAndTranslation & cnt)
  {
    const std::string * p_std_str;
    wxString wxstr;
    if(cnt.m_stdstrGermanTranslation != "")
    {
      wxstr = wxT("German translation:");
      p_std_str = & cnt.m_stdstrGermanTranslation;
    }
    else if( cnt.m_stdstrAttributeName != "" )
    {
      wxstr = wxT("attribute name:");
      p_std_str = & cnt.m_stdstrAttributeName;
    }
//    else if( cnt.m_stdstrAttributeName != "" )
    wxStaticText * p_wxstatictext = new wxStaticText( this, wxID_ANY,
      wxstr + getwxString( * p_std_str ) );
    m_p_wxsizer->Add(p_wxstatictext);
    m_std_vec_window.push_back(p_wxstatictext);
  }

  void ShowTranslationRulesDialog::Create(
    wxString choices[],
    int arraySize
    )
  {
    m_p_translationruleslistctrl = new VTrans::TranslationRulesListCtrl(this,
      translRuleList);
    m_p_translationruleslistctrl->SetData(choices, arraySize);

    m_p_wxsizer = new wxBoxSizer(wxVERTICAL);
  //    wxComboBox * p_wxcombobox = new wxComboBox(this, wxID_ANY, "",
  //      wxDefaultPosition, wxDefaultSize, arraySize, choices);
  //    wxListBox * p_wxlistbox = new wxListBox(this, wxID_ANY,
  //      wxDefaultPosition, wxDefaultSize, arraySize, choices,
  //      wxLB_HSCROLL | wxLB_NEEDED_SB);
  //    wxListCtrl * p_wxlistctrl = new wxListCtrl();
    m_p_wxtextctrl = new wxTextCtrl(this, translRuleTextCtrl);
    wxButton * p_wxbuttonTest = new wxButton( this, TestTranslRule,
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
      m_p_translationruleslistctrl
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

  void ShowTranslationRulesDialog::OnTestTranslRule(wxCommandEvent & evt)
  {
//    const wxString & wxstrText = m_p_wxtextctrl->//GetLineText (0
//      ///*long lineNo*/);
//      GetValue();
//    BYTE byPersonIndex;
    std::string stdstrTranslation;
//    wxGetApp().m_translateparsebyrisetree.TranslationRuleApplies(
//      stdstrTranslation //std::string & r_stdstrTranslation
//      , byPersonIndex //BYTE & r_byPersonIndex
//      // So it can be used with data from outside this class.
//      , const std::vector<WORD> & r_stdvec_wGrammarPartPath
//      , const std::vector<GrammarPart * > & r_stdvec_p_grammarpartPath
//  //    , WORD & r_wConsecutiveID
//      , //const
//      GrammarPart * & p_grammarpart
//      , m_p_translationruleSelected
//      ) ;
    std::string stdstrWholeInputText ;
    wxGetApp().GetSourceText( stdstrWholeInputText) ;
    wxGetApp().m_parsebyrise.CreateParseTree(stdstrWholeInputText);
    std::vector<std::string> r_stdvec_stdstrWholeTransl;
  //  //A vector of sentences that each contains a vector of words.
  //  , std::vector<std::vector<TranslationAndGrammarPart> > &
  //    r_stdvec_stdvecTranslationAndGrammarPart
    //A vector of sentences that begin at the same token index
    // (sentences that begin at the same token index:
    // vector of sentences that each contains a vector of words).
    std::vector <std::vector <std::vector <TranslationAndGrammarPart> > >
      r_stdvec_stdvec_stdvecTranslationAndGrammarPart;
    wxGetApp().m_translateparsebyrisetree.m_bTranslationRuleApplies = false;

    wxGetApp().m_p_showtranslationrulesdialog = this;

    wxGetApp().m_translateparsebyrisetree.ProcessParseTree(
  //    r_parsebyrise,
      r_stdvec_stdstrWholeTransl,
      r_stdvec_stdvec_stdvecTranslationAndGrammarPart,
      //(TranslateParseTree)()//r_stdvec_stdvec_stdvecTranslationAndGrammarPart)
      //syntax ( "& >>class name<<::>>method name<<" from
      //see http://stackoverflow.com/questions/4832275/c-typedef-member-function-signature-syntax
      & TranslateParseByRiseTree::TestIfTranslationRuleApplies
      );
//    testiftranslationruleapplies.Traverse();
    if( wxGetApp().m_translateparsebyrisetree.m_bTranslationRuleApplies )
      wxGetApp().Message( "selected transl. rule applies to parse tree" );
    else
      wxGetApp().Message( "selected transl. rule does not apply to parse tree" );
  }

  void ShowTranslationRulesDialog::ShowConditionsAndTranslation(
    const wxString & text)
  {
//    wxSizerItem * p_wxsi;
//    int itemCount = m_p_wxsizer->GetItemCount();
//    while( itemCount > 3)
//    {
//      p_wxsi = m_p_wxsizer->GetItem(3);
//      m_p_wxsizer->Remove(3);
//      wxWindow * p_wxwindow = p_wxsi->GetWindow();
//      if(p_wxwindow)
//        p_wxwindow->Destroy();
//      itemCount = m_p_wxsizer->GetItemCount();
//    }
    std::vector<wxWindow *>::const_iterator c_iter_window = m_std_vec_window.begin();
    wxWindow * p_wxwindow;
    while( c_iter_window != m_std_vec_window.end() )
    {
      p_wxwindow = * c_iter_window;
      m_p_wxsizer->Detach(p_wxwindow);
      ++ c_iter_window;
      p_wxwindow->Destroy();
    }
    m_std_vec_window.clear();
    wxString wxstr, wxstrSyntaxTreePath;
    std::map<TranslationRule *,ConditionsAndTranslation> &
      r_stdmap_p_translationrule2ConditionsAndTranslation = wxGetApp().
      m_translateparsebyrisetree.
      m_stdmap_p_translationrule2ConditionsAndTranslation;
//    int arraySize = r_stdmap_p_translationrule2ConditionsAndTranslation.size();
    std::map<TranslationRule *,ConditionsAndTranslation>::const_iterator c_iter =
        r_stdmap_p_translationrule2ConditionsAndTranslation.begin();
//    unsigned arrayIndex = 0;
    while( c_iter != r_stdmap_p_translationrule2ConditionsAndTranslation.end() )
    {
      const TranslationRule * c_p_translationrule =
        c_iter->first;
  //      r_translationrule.mp_parsebyrise->Get
      wxstrSyntaxTreePath = getwxString( c_p_translationrule->
        m_syntaxtreepathCompareWithCurrentPath.GetAs_std_string() );
      if( wxstrSyntaxTreePath.Cmp(text) == 0 )
      {
        m_p_translationruleSelected = (TranslationRule *) c_p_translationrule;
        m_p_conditionsandtranslation = (ConditionsAndTranslation *)
          & c_iter->second;
        const ConditionsAndTranslation & cnt = c_iter->second;
        AddTranslation(cnt);
        std::vector<Condition>::const_iterator c_iterConditions =
          cnt.m_conditions.begin();
        while( c_iterConditions != c_iter->second.m_conditions.end() )
        {
          AddConditionToGUI( * c_iterConditions);
          ++ c_iterConditions;
        }
        Layout();
        break;
//        m_stdstrAttributeName
      }
      ++ c_iter;
    }
//    return wxT("/");
  }
} /* namespace VTrans */
