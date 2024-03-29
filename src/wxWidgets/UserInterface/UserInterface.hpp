/*
 * UserInterface.hpp
 *
 *  Created on: Sep 29, 2010
 *      Author: Stefan
 */

#ifndef USERINTERFACE_HPP_
#define USERINTERFACE_HPP_

//Forward declarations (faster than #include)
class wxArrayString ;
class wxString ;
class wxWindow ;

namespace wxWidgets
{

  class UserInterface
  {
  public:
    UserInterface();
    virtual
    ~UserInterface();
  };
  int ShowMultipleFileSelectionDialog(
    const wxString & cr_wxstrTitle ,
    wxArrayString & wxarraystringPaths ,
    wxWindow *,
    const wxString & c_r_wxInitialDirForFileSelecion
    ) ;
  void InsertIntoVocabularyIntoMemory_Async(
    wxWindow * p_wxwindowParent,
    const std::string & std_strFilePath
    );
  void LoadOrReloadDictionary(
    wxWindow * p_wxwindow,
    const wxString & wxstrVocabularyDirPath,
    const wxString & wxstrVocabularyFileName) ;
}

#endif /* USERINTERFACE_HPP_ */
