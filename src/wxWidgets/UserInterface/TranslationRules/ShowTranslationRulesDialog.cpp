/*
 * ShowTranslationRulesDialog.cpp
 *
 *  Created on: 09.12.2012
 *      Author: Stefan
 */

/** Include as first so that "Yield" from "winbase.h" (MSW) is not defined
 * before */
#include <wxWidgets/VTransApp.hpp> //wxGetApp()
#include "ShowTranslationRulesDialog.hpp"

//class TranslationAndGrammarPart
#include <Attributes/TranslationAndGrammarPart.hpp>
//convertToStdString(...)
#include <Controller/character_string/convertFromAndToStdString.hpp>
#include <Translate/Condition.hpp> //class Condition
//class ConditionsAndTranslation
#include <Translate/ConditionsAndTranslation.hpp>
//class ParseTreeTraverser::TestIfTranslationRuleApplies
#include <Translate/TestIfTranslationRuleApplies.hpp>
#include <Translate/TranslationRule.hpp> //class TranslationRule
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>
//getwxString()
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
#include <map> //class std::map
#include "TranslationRulesListCtrl.hpp"
#include <wx/sizer.h> //class wxBoxSizer

/** for wxWidgets::GetwxString_Inline */
using namespace wxWidgets;

namespace VTrans
{

  ShowTranslationRulesDialog::ShowTranslationRulesDialog(
    wxWindow * parent/*,
    wxString choices[],
    int arraySize*/
    )
    : ShowRulesDialogBase(
       parent //wxWindow * parent
       /*, choices
       , arraySize*/
       , wxT("translation rules") //const wxString & title//,
//       const wxString& name = wxDialogNameStr
      )
      , m_p_translationruleSelected(0)
      , m_p_conditionsandtranslation(0)
  {
    Create(/*choices, arraySize*/);
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
        convertToStdString<
         //TODO change to a faster (fastestUnsignedDataType) data type?!
         WORD>(cond.m_byAttributeValue) )
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

  void ShowTranslationRulesDialog::CreateRulesListControl()
  {
    m_p_rulesListCtrl = new VTrans::TranslationRulesListCtrl(
      this, rulesList);
  }

  void ShowTranslationRulesDialog::TestRule(/*wxCommandEvent & evt*/)
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
    TranslationResult r_translationResult;
    wxGetApp().m_translateparsebyrisetree.m_bTranslationRuleApplies = false;

    wxGetApp().m_p_showtranslationrulesdialog = this;

    //TODO crash on empty translation rule
    wxGetApp().m_translateparsebyrisetree.ProcessParseTree(
  //    r_parsebyrise,
      r_stdvec_stdstrWholeTransl,
      r_translationResult,
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
