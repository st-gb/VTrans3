/* * File:   ConsoleTranslationController.hpp
 * Author: sg
 * Created on 9. Mai 2017, 11:20 */

#ifndef CONSOLETRANSLATIONCONTROLLER_HPP
#define CONSOLETRANSLATIONCONTROLLER_HPP

#include "TranslationControllerBase.hpp"
#include <map> //class std::map
#include <Process/CommandLineOption.hpp>

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
  void OutputStatistics();
  static void CreateMappingBetweenFunctionNameAndFunction();
  static void OutputUsage();
  static CommandLineOption/*<char>*/ s_commandLineOptions [] ;
  static std::map<TranslateParseByRiseTree::ProcessParseTree_type, std::string> 
    s_function2functionName;
  //TODO maybe better use TranslationControllerBase::s_settingsName2ValueAndFunction instead
  typedef void (/*ConsoleTranslationController::*/*ProcessCommandLineOption_type )();
  std::map<std::string, ProcessCommandLineOption_type> s_cmdLineOpt2Function;
  static std::string GetFunctionName(
    TranslateParseByRiseTree::ProcessParseTree_type pfnProcessParseTree);
  void UpdateLoadDictStatus(const float);
  void waitSeconds(int);
};

#endif /* CONSOLETRANSLATIONCONTROLLER_HPP */

