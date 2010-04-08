/* 
 * File:   VTransApp.cpp
 * Author: Stefan
 * 
 * Created on 20. Februar 2010, 14:02
 */

#include "VTransApp.hpp"
#include <wx/wx.h>
#include "wxTextInputDlg.h"
#include <IO.hpp> //OneLinePerWordPair::LoadWords()
#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>
#include <fstream> //for std::ofstream

std::ofstream ofstreamLogFile ;
extern LetterTree g_lettertree ;

IMPLEMENT_APP(VTransApp)

VTransApp::VTransApp() {
}

VTransApp::VTransApp(const VTransApp& orig) {
}

VTransApp::~VTransApp() {
}

bool VTransApp::OnInit()
{
  //ParseByRise parsebyrise ;
  if( argc > 1 )
  {
    std::string str(//"germanNounsFromTUdictInVTransFormatVeryShort.txt") ;
        argv[1] ) ;
    g_lettertree.InsertFundamentalWords() ;
    if( OneLinePerWordPair::LoadWords( //pWordNodeCurrent
         str )
      )
    {
      wxTextInputDlg * frame = new wxTextInputDlg(
       NULL
       //, wxID_ANY, wxString("gg"), wxPoint(50,50), wxSize(400,400),
       //wxDEFAULT_DIALOG_STYLE | wxCLOSE_BOX
       );
     if( frame )
       frame->Show(true);
     else
       std::cerr << "error: couldn't create window\n" ;
     return true;
    }
    else
    {
      wxString wxstrCwd = wxGetCwd() ;
      ::wxMessageBox( wxString::Format("Error loading vocabulary file \"%s\\%s\" "
        "->exiting", wxstrCwd.c_str(), str.c_str() )
        ) ;
    }
  }
  else
  {
    ::wxMessageBox( wxT("No vocabulary file specified as 1st command line "
      "argument ->exiting" ) ) ;
  }
  return false ;
}
