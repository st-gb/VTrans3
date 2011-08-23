/*
 * UserInterface.cpp
 *
 *  Created on: Sep 29, 2010
 *      Author: Stefan
 */
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <wx/filedlg.h> //wxFD_OPEN / wxOPEN

//class TranslationControllerBase
#include <Controller/TranslationControllerBase.hpp>
#include <IO/IO.hpp> //class OneLinePerWordPair
#include <VocabularyInMainMem/LetterTree/LetterTree.hpp> //class LetterTree
//GetStdString(...)
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
#include <wxWidgets/UserInterface/UserInterface.hpp>
#include <string> //class std::string

//extern LetterTree g_lettertree ;

////Static variables need also to be defined in the source file.
//LetterTree TranslationControllerBase::s_lettertree ;

namespace wxWidgets
{

  UserInterface::UserInterface()
  {
    // TODO Auto-generated constructor stub

  }

  UserInterface::~UserInterface()
  {
    // TODO Auto-generated destructor stub
  }

  int ShowMultipleFileSelectionDialog(
    const wxString & cr_wxstrTitle ,
    wxArrayString & wxarraystringPaths ,
    wxWindow * p_wxwindow,
    const wxString & c_r_wxInitialDirForFileSelecion
    )
  {
    LOGN("wxTextInputDlg::ShowMultipleFileSelectionDialog(" << cr_wxstrTitle
      << ")" )
    wxFileDialog wxfiledialog(
      //this ,
      p_wxwindow ,
      cr_wxstrTitle ,
//      wxT("") , //defaultDir
      c_r_wxInitialDirForFileSelecion,
      wxT("") //const wxString&  defaultFile = ""
      //" A wildcard, such as "*.*" or
      // "BMP files (*.bmp)|*.bmp|GIF files (*.gif)|*.gif". "
      , wxT("XML files(*.xml)|*.xml|all files(*.*)|*.*") //const wxString&  wildcard = "*.*"
      , //wxOPEN
        //wxWidgets 2.9 has no "wxOPEN"
        wxFD_OPEN
        //"For open dialog only: allows selecting multiple files."
        | wxFD_MULTIPLE
        | //wxFILE_MUST_EXIST //long style = 0,
        //wxWidgets 2.9 has no "wxFILE_MUST_EXIST"
        wxFD_FILE_MUST_EXIST
      ) ;
    int n = wxfiledialog.ShowModal() ;
    if( n == wxID_OK )
    {
      wxfiledialog.
        //http://docs.wxwidgets.org/2.8/wx_wxfiledialog.html
        // #wxfiledialoggetpaths:
        // "Fills the array paths with the full paths of the files chosen.
        //This function should only be used with the dialogs which have
        //wxFD_MULTIPLE style, use GetPath for the others."
        GetPaths(wxarraystringPaths) ;
    }
    return n ;
  }

  void UnLoadAndLoadDictionary(wxWindow * p_wxwindow)
  {
    wxFileDialog wxfiledialog(
      p_wxwindow ,
      "Choose a dictionary file" ,
      wxT("") , //defaultDir
      wxT("") //const wxString&  defaultFile = ""
      , wxT("*.txt") //const wxString&  wildcard = "*.*"
      , //wxOPEN | wxFILE_MUST_EXIST //long style = 0,
      //wxWidgets 2.9 has no "wxOPEN" or "wxFILE_MUST_EXIST"
      wxFD_OPEN | wxFD_FILE_MUST_EXIST //long style = 0,
      ) ;
    if( wxfiledialog.ShowModal() == wxID_OK )
    {
      wxString wxstrFullPath = wxfiledialog.
        //http://docs.wxwidgets.org/2.8/wx_wxfiledialog.html
        // #wxfiledialoggetpath:
        // "Returns the full path (directory and filename) of the selected file."
        GetPath() ;
      std::string stdstr = GetStdString( wxstrFullPath ) ;
      wxString wxstrLabel = p_wxwindow->GetLabel() ;
//      ::wxMessageBox( wxT("freeing memory for existing vocabulary") ) ;
      p_wxwindow->SetLabel( wxT("freeing memory for existing vocabulary") ) ;
//      g_lettertree.DeleteCompleteList() ;
      TranslationControllerBase::s_lettertree.DeleteCompleteList() ;
      //MUST be inserted, else some grammar rules can't be applied.
//      g_lettertree.InsertFundamentalWords() ;
      TranslationControllerBase::s_lettertree.InsertFundamentalWords() ;
      OneLinePerWordPair::s_dwNumberOfVocabularyPairs = 0 ;
//      wxMessageDialog * p_dlg = new wxMessageDialog( //NULL,
//        p_wxwindow ,
//        wxT("inserting vocabulary into memory")) ;
      p_wxwindow->SetLabel( wxT("inserting vocabulary into memory") ) ;
      {
//        p_dlg->Show( true ) ;
        OneLinePerWordPair::LoadWords( stdstr ) ;
//        p_dlg->Destroy();
      }
      p_wxwindow->SetLabel( wxstrLabel ) ;
    }
  }
}
