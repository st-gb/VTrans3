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
//#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>//class LetterTree
#include <xercesc/util/XercesVersion.hpp> //XERCES_CPP_NAMESPACE
#include <string> //class std::string

//Fwd decl.
class wxTextInputDlg ;
namespace XERCES_CPP_NAMESPACE
{
  class DefaultHandler ;
}
namespace VTrans
{
  class ShowTranslationRulesDialog;
}
class TranslationRule;

namespace wxWidgets
{
  class VTransApp
    : public wxApp
    //, public I_UserInterface
    , public TranslationControllerBase
  {
  public:
    VTrans::ShowTranslationRulesDialog * m_p_showtranslationrulesdialog;
  //  static LetterTree s_lettertree ;
    wxTextInputDlg * m_p_wx_text_input_dialog ;
    std::map<TranslationRule *, std::string>
      m_std_map_p_translationrule2filepath;
  //  std::string m_stdstrVocabularyFilePath ;

    VTransApp();
    virtual ~VTransApp();

    void CreateAndShowMainWindow() ;
    void GetSourceText(std::string & std_string);
    bool HandleCommandLineArgs() ;
    virtual int OnExit();
    virtual bool OnInit();
  //  VTransApp(const VTransApp& orig);
    void LoadingVocabularyFileFailed(const std::string & cr_stdstrFilePath);
    void Message( const std::string & cr_stdstr ) ;
    void Message( const std::wstring & cr_stdwstr ) ;
    void ProcessSelectedXMLfiles(
      XERCES_CPP_NAMESPACE::DefaultHandler & r_xercesc_defaulthandler ,
      const wxString & r_wxstrTitle,
      const wxString & c_r_wxInitialDirForFileSelecion
      );
    void ShowInvalidVocabularyFileFormatMessage(
      const VTrans::string_type & strWordFile,
      DWORD dwOffsetOfBeginOfEntry,
      DWORD dwOffset
      );
    void EndTimer();
    void StartTimer();
  private:

  };
}

//http://docs.wxwidgets.org/2.6/wx_rttimacros.html#declareapp:
//"It creates the declaration className& wxGetApp(void)."
DECLARE_APP(wxWidgets::VTransApp)

#endif	/* _VTRANSAPP_HPP */

