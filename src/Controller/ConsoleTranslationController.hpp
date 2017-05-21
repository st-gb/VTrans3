/* * File:   ConsoleTranslationController.hpp
 * Author: sg
 * Created on 9. Mai 2017, 11:20 */

#ifndef CONSOLETRANSLATIONCONTROLLER_HPP
#define CONSOLETRANSLATIONCONTROLLER_HPP

#include "TranslationControllerBase.hpp"
#include <map> //class std::map

class ConsoleTranslationController
  : public TranslationControllerBase
{
public:
  ConsoleTranslationController();
  ConsoleTranslationController(const ConsoleTranslationController& orig);
  virtual ~ConsoleTranslationController();
private:
  static void Insert(
    TranslateParseByRiseTree::ProcessParseTree_type pfnProcessParseTree,
    const char * const );
  
public:
  static void CreateMappingBetweenFunctionNameAndFunction();
  static std::map<std::string, TranslateParseByRiseTree::ProcessParseTree_type> 
    s_functionName2function;
  static std::map<TranslateParseByRiseTree::ProcessParseTree_type, std::string> 
    s_function2functionName;
  static std::string GetFunctionName(
    TranslateParseByRiseTree::ProcessParseTree_type pfnProcessParseTree);
};

#endif /* CONSOLETRANSLATIONCONTROLLER_HPP */

