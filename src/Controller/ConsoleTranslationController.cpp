/** File:   ConsoleTranslationController.cpp
 * Author: sg
 * Created on 9. Mai 2017, 11:20 */

#include "ConsoleTranslationController.hpp"
#include "Controller/character_string/ConvertStdStringToTypename.hpp"
#include <data_structures/ByteArray.hpp>
#include <IO/dictionary/OpenDictFileException.hpp>
#include <FileSystem/GetCurrentWorkingDir.hpp>

ConsoleTranslationController::ConsoleTranslationController() {
}

ConsoleTranslationController::ConsoleTranslationController(const ConsoleTranslationController& orig) {
}

ConsoleTranslationController::~ConsoleTranslationController() {
}

TranslationControllerBase * g_p_translationcontrollerbase;

void OpenLogFile(const char * const logFilePrefix)
{
//  #ifdef /*COMPILE_WITH_LOG*/ USE_OWN_LOGGER
  const char * const logFormat = "log4j";
  //TODO use other path than this one as log file directory
  std::string stdstrLogFilePath = logFilePrefix;
  stdstrLogFilePath += "_log.";
//  stdstrLogFilePath += logFormat;
  bool bFileIsOpen = g_logger.OpenFileA(stdstrLogFilePath, logFormat, 4000,
    LogLevel::info /*warning*/) ;
#ifdef __linux__
#ifndef __ANDROID__
//  g_logger.AddConsoleLogEntryWriter();
#endif
#endif
//  g_logger.
  if( ! bFileIsOpen )
  {
//    std::cerr << "Failed to open the log file:"
//      << GetErrorMessageFromLastErrorCodeA() << std::endl;
//    return;
  }
}

int main(int argc, char *  argv[])
{
  std::cout << argc << std::endl;
  OpenLogFile(argv[0]);
  
  if( argc > 1 )
  {
    try
    {
      std::string std_strCurrentWorkingDir;
      OperatingSystem::GetCurrentWorkingDirA_inl(std_strCurrentWorkingDir);
//      LOGN_INFO("current dir is:\"" << std_strCurrentWorkingDir << "\"")
      std::cout << std_strCurrentWorkingDir << std::endl;
      
      ConsoleTranslationController consoleTranslationController;
      if( argc > 2 )
      {
        int numThreads;
        ConvertCharStringToTypename(numThreads,argv[2]);
        consoleTranslationController.m_multiThreadedTranslation.
          SetNumberOfThreads(numThreads);
      }
      g_p_translationcontrollerbase = & consoleTranslationController;
      consoleTranslationController.Init("configuration/VTrans_main_config.xml");
      ByteArray byteArray;
      std::cout << "before Translating \"" << argv[1] << "\",max." << consoleTranslationController.
        m_multiThreadedTranslation.GetNumberOfThreads() << " threads" << std::endl;
      
      consoleTranslationController.TranslateAsXML(argv[1], byteArray);
      std::string std_strParseTreeAsIndentedXML = ::GetParseTreeAsIndentedXML(
        consoleTranslationController.m_parsebyrise );
      
      std::cout << "after translate:" << std_strParseTreeAsIndentedXML << std::endl;
      std::cout << "apply transl. rules took:" << consoleTranslationController.
        m_numThreadsAndTimeDuration[applyTranslRules].timeDurationInSeconds << "s" << std::endl;
    }catch(VTrans3::OpenDictFileException & odfe )
    {
      std::cout << "dd" << std::endl;
    }
  }
  return 0;
}
