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
#include <wx/msgdlg.h>

//class TranslationControllerBase
#include <Controller/TranslationControllerBase.hpp>
#include <IO/dictionary/VTransDictFormatReader.hpp> //class OneLinePerWordPair
//class TUchemnitzDictionaryReader
#include <IO/dictionary/TUchemnitzDictionaryReader.hpp>
//#include <VocabularyInMainMem/LetterTree/LetterTree.hpp> //class LetterTree
#include <Controller/thread_type.hpp> //typedef VTrans::thread_type
//GetStdString(...)
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
#include <wxWidgets/UserInterface/UserInterface.hpp>
#include <wxWidgets/VTransApp.hpp> //::wxGetApp()
#include <string> //class std::string

//extern LetterTree g_lettertree ;

////Static variables need also to be defined in the source file.
//LetterTree TranslationControllerBase::s_dictionary ;

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
    LOGN(//"wxTextInputDlg::ShowMultipleFileSelectionDialog(" <<
      cr_wxstrTitle
      //<< ")"
      )
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

  DWORD THREAD_FUNCTION_CALLING_CONVENTION LoadDictionary(void * p_v )
  {
    const char * const dictFilePath = (const char * const) p_v;
    if( dictFilePath )
    {
      TUchemnitzDictionaryReader::extractVocables( dictFilePath);
      return 0;
    }
    return 1;
  }

  void UnLoadAndLoadDictionary(wxWindow * p_wxwindow)
  {
    wxFileDialog wxfiledialog(
      p_wxwindow ,
      wxT("Choose a dictionary file") ,
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
      std::string std_strFilePath = GetStdString( wxstrFullPath ) ;
      wxString wxstrLabel = p_wxwindow->GetLabel() ;
//      ::wxMessageBox( wxT("freeing memory for existing vocabulary") ) ;
      p_wxwindow->SetLabel( wxT("freeing memory for existing vocabulary") ) ;
//      g_lettertree.DeleteCompleteList() ;
      TranslationControllerBase::s_dictionary./*DeleteCompleteList()*/clear();
      //MUST be inserted, else some grammar rules can't be applied.
//      g_lettertree.InsertFundamentalWords() ;
      TranslationControllerBase::s_dictionary.InsertFundamentalWords() ;
//      OneLinePerWordPair::s_dwNumberOfVocabularyPairs = 0 ;
      ::wxGetApp().s_numberOfVocabularyPairs = 0;
//      wxMessageDialog * p_dlg = new wxMessageDialog( //NULL,
//        p_wxwindow ,
//        wxT("inserting vocabulary into memory")) ;
      p_wxwindow->SetLabel( wxT("inserting vocabulary into memory") ) ;
      {
//        p_dlg->Show( true ) ;
//        OneLinePerWordPair::LoadWords( std_strFilePath ) ;
        TUchemnitzDictionaryReader tcdr(::wxGetApp() );
        ::wxGetApp().StartTimer();
//        VTrans::thread_type thread;
//        thread.start(LoadDictionary, std_strFilePath.c_str() );
        TUchemnitzDictionaryReader::extractVocables( std_strFilePath.c_str());
//        if( ::wxMessageBox( wxT("loading vocs"), wxT(""), wxOK | wxCANCEL) ==
//            wxID_CANCEL )
//          tcdr.CancelLoading();
        ::wxGetApp().EndTimer();
//        p_dlg->Destroy();
      }
      p_wxwindow->SetLabel( wxstrLabel ) ;
    }
  }
}
