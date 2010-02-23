/* 
 * File:   VTransApp.hpp
 * Author: Stefan
 *
 * Created on 20. Februar 2010, 14:02
 */

#ifndef _VTRANSAPP_HPP
#define	_VTRANSAPP_HPP

#include <wx/app.h>

class VTransApp 
  : public wxApp
{
public:
  virtual bool OnInit();
  VTransApp();
  VTransApp(const VTransApp& orig);
  virtual ~VTransApp();
private:

};

#endif	/* _VTRANSAPP_HPP */

