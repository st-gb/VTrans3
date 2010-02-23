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
  std::string str("germanNounsFromTUdictInVTransFormatVeryShort.txt") ;
    g_lettertree.InsertFundamentalWords() ;
    OneLinePerWordPair::LoadWords( //pWordNodeCurrent
       str ) ;
   wxTextInputDlg *frame = new wxTextInputDlg(
     NULL
     //, wxID_ANY, wxString("gg"), wxPoint(50,50), wxSize(400,400),
     //wxDEFAULT_DIALOG_STYLE | wxCLOSE_BOX
     );
   frame->Show(true);
   return true;
}
