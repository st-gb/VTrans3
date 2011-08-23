/* 
 * File:   VTransApp.hpp
 * Author: Stefan
 *
 * Created on 20. Februar 2010, 14:02
 */

#ifndef _VTRANSAPP_HPP
#define	_VTRANSAPP_HPP

#include <wx/app.h>//base class wxApp
#include <Controller/TranslationControllerBase.hpp>
#include <UserInterface/I_UserInterface.hpp>//base class I_UserInterface
#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>//class LetterTree
#include <xercesc/util/XercesVersion.hpp> //XERCES_CPP_NAMESPACE

//Fwd decl.
class wxTextInputDlg ;
namespace XERCES_CPP_NAMESPACE
{
  class DefaultHandler ;
}

class VTransApp 
  : public wxApp
  //, public I_UserInterface
  , public TranslationControllerBase
{
public:
//  static LetterTree s_lettertree ;
  wxTextInputDlg * m_p_wx_text_input_dialog ;
//  std::string m_stdstrVocabularyFilePath ;
  void CreateAndShowMainWindow() ;
  bool HandleCommandLineArgs() ;
  virtual int OnExit();
  virtual bool OnInit();
  VTransApp();
//  VTransApp(const VTransApp& orig);
  void LoadingVocabularyFileFailed(const std::string & cr_stdstrFilePath);
  void Message( const std::string & cr_stdstr ) ;
  void Message( const std::wstring & cr_stdwstr ) ;
  void ProcessSelectedXMLfiles(
    XERCES_CPP_NAMESPACE::DefaultHandler & r_xercesc_defaulthandler ,
    const wxString & r_wxstrTitle,
    const wxString & c_r_wxInitialDirForFileSelecion
    );
  virtual ~VTransApp();
private:

};

//http://docs.wxwidgets.org/2.6/wx_rttimacros.html#declareapp:
//"It creates the declaration className& wxGetApp(void)."
DECLARE_APP(VTransApp)

#endif	/* _VTRANSAPP_HPP */

