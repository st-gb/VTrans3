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
//#include <IO/dictionary/VTransDictFormatReader.hpp> //class OneLinePerWordPair
//class TUchemnitzDictionaryReader
//#include <IO/dictionary/TUchemnitzDictionaryReader.hpp>
//#include <IO/dictionary/TUchemnitzDictEngWord2LineNumberReader.hpp>
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
extern TranslationControllerBase * g_p_translationcontrollerbase;

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

  /** @brief This function is meant as parameter for starting a thread:
   *   thread.start(LoadDictionary_ThreadFunc, p_v);
   *  @param p_vParam the dictFilePath as char array (convert to
   *     "const char * const")
   *  @return Because this function should be called at thread startup this is
   *    the thread exit code.
   *    0 = success/ no error*/
  DWORD THREAD_FUNCTION_CALLING_CONVENTION LoadDictionary_ThreadFunc(
    void * p_vParam )
  {
    const char * const dictFilePath = (const char * const) p_vParam;
    /** obtain a copy (dictFilePath may be destroyed in other thread)*/
    std::string std_strDictFilePath = std::string(dictFilePath);
    if( dictFilePath )
    {
//      TUchemnitzDictionaryReader
//      //DictionaryReader::TUchemnitzDictEngWord2LineNumberReader
//	    tcdr(
//        /** this*/ ::wxGetApp(),
//        & ::wxGetApp().s_dictionary );
      /*TUchemnitzDictionaryReader:: tcdr.read(*/
      bool b = ::wxGetApp().s_dictReaderAndVocAccess.loadDictionary(
        std_strDictFilePath.c_str() );//extractVocables( dictFilePath);
      if( ! b )
        ::wxGetApp().LoadingVocabularyFileFailed(std_strDictFilePath);
      ::wxGetApp().EndTimer();
      return b;
    }
    return 1;
  }

  void InsertIntoVocabularyIntoMemory_Async(
    wxWindow * p_wxwindowParent,
    const std::string & std_strFilePath
    )
  {
    p_wxwindowParent->SetLabel( wxT("inserting vocabulary into memory") ) ;
//        p_dlg->Show( true ) ;
//        OneLinePerWordPair::LoadWords( std_strFilePath ) ;
//        TUchemnitzDictionaryReader tcdr(::wxGetApp(), & ::wxGetApp().s_dictionary);
    ::wxGetApp().StartTimer();
    VTrans::thread_type thread;
    thread.start(LoadDictionary_ThreadFunc, (void *) std_strFilePath.c_str() );
//        TUchemnitzDictionaryReader::extractVocables( std_strFilePath.c_str());
//        if( ::wxMessageBox( wxT("loading vocs"), wxT(""), wxOK | wxCANCEL) ==
//            wxID_CANCEL )
//          tcdr.CancelLoading();
//        p_dlg->Destroy();
  }

  void LoadOrReloadDictionary(
    wxWindow * p_wxwindowParent,
    const wxString & wxstrVocabularyDirPath,
    const wxString & wxstrVocabularyFileName)
  {
    wxFileDialog wxfiledialog(
      p_wxwindowParent
      , wxT("Choose a dictionary file")
      , wxstrVocabularyDirPath //wxT("") , //defaultDir
      , wxstrVocabularyFileName //wxT("") //const wxString&  defaultFile = ""
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
      /** Save in variable that lives longer than the new thread*/
      ::wxGetApp().m_std_strLastSelectedDictFilePath = GetStdString( wxstrFullPath ) ;
      std::string & std_strFilePath = ::wxGetApp().m_std_strLastSelectedDictFilePath;
      wxString wxstrLabel = p_wxwindowParent->GetLabel() ;
//      ::wxMessageBox( wxT("freeing memory for existing vocabulary") ) ;
      p_wxwindowParent->SetLabel( wxT("freeing memory for existing vocabulary") ) ;
//      g_lettertree.DeleteCompleteList() ;
      g_p_translationcontrollerbase->s_dictReaderAndVocAccess.m_vocAccess./*DeleteCompleteList()*/clear();
      //MUST be inserted, else some grammar rules can't be applied.
//      g_lettertree.InsertFundamentalWords() ;
      g_p_translationcontrollerbase->s_dictReaderAndVocAccess.m_vocAccess.InsertFundamentalWords() ;
//      OneLinePerWordPair::s_dwNumberOfVocabularyPairs = 0 ;
      ::wxGetApp().s_numberOfVocabularyPairs = 0;
//      wxMessageDialog * p_dlg = new wxMessageDialog( //NULL,
//        p_wxwindow ,
//        wxT("inserting vocabulary into memory")) ;
      InsertIntoVocabularyIntoMemory_Async(
        p_wxwindowParent,
        std_strFilePath
        );
      p_wxwindowParent->SetLabel( wxstrLabel ) ;
    }
  }
}
