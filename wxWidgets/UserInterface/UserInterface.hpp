/*
 * UserInterface.hpp
 *
 *  Created on: Sep 29, 2010
 *      Author: Stefan
 */

#ifndef USERINTERFACE_HPP_
#define USERINTERFACE_HPP_

//Forward declarations (faster than #include)
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
  void UnLoadAndLoadDictionary(wxWindow * p_wxwindow) ;
}

#endif /* USERINTERFACE_HPP_ */
