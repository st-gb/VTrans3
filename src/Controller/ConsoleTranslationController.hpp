/* * File:   ConsoleTranslationController.hpp
 * Author: sg
 * Created on 9. Mai 2017, 11:20 */

#ifndef CONSOLETRANSLATIONCONTROLLER_HPP
#define CONSOLETRANSLATIONCONTROLLER_HPP

#include "TranslationControllerBase.hpp"

class ConsoleTranslationController
  : public TranslationControllerBase
{
public:
  ConsoleTranslationController();
  ConsoleTranslationController(const ConsoleTranslationController& orig);
  virtual ~ConsoleTranslationController();
private:

};

#endif /* CONSOLETRANSLATIONCONTROLLER_HPP */

