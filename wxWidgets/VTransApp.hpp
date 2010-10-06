/* 
 * File:   VTransApp.hpp
 * Author: Stefan
 *
 * Created on 20. Februar 2010, 14:02
 */

#ifndef _VTRANSAPP_HPP
#define	_VTRANSAPP_HPP

#include <wx/app.h>//base class wxApp
#include <UserInterface/I_UserInterface.hpp>//base class I_UserInterface

class VTransApp 
  : public wxApp
  , public I_UserInterface
{
public:
  virtual bool OnInit();
  VTransApp();
  VTransApp(const VTransApp& orig);
  void Message( const std::string & cr_stdstr ) ;
  void Message( const std::wstring & cr_stdwstr ) ;
  virtual ~VTransApp();
private:

};

//http://docs.wxwidgets.org/2.6/wx_rttimacros.html#declareapp:
//"It creates the declaration className& wxGetApp(void)."
DECLARE_APP(VTransApp)

#endif	/* _VTRANSAPP_HPP */

