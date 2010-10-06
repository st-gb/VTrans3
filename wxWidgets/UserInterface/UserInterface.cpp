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

#include <IO.hpp> //class OneLinePerWordPair
#include <VocabularyInMainMem/LetterTree/LetterTree.hpp> //class LetterTree
#include <wxWidgets/Controller/wxStringHelper.hpp> //GetStdString(...)
#include <wxWidgets/UserInterface/UserInterface.hpp>
#include <string> //class std::string

extern LetterTree g_lettertree ;

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

  void UnLoadAndLoadDictionary(wxWindow * p_wxwindow)
  {
    wxFileDialog wxfiledialog(
      p_wxwindow ,
      "Choose a dictionary file" ,
      wxT("") , //defaultDir
      wxT("") //const wxString&  defaultFile = ""
      , wxT("*.txt") //const wxString&  wildcard = "*.*"
      , wxOPEN | wxFILE_MUST_EXIST //long style = 0,
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
      g_lettertree.DeleteCompleteList() ;
      //MUST be inserted, else some grammar rules can't be applied.
      g_lettertree.InsertFundamentalWords() ;
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
