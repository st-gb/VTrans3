/** File:   ConsoleTranslationController.cpp
 * Author: sg
 * Created on 9. Mai 2017, 11:20 */

#include "ConsoleTranslationController.hpp"
#include "Controller/character_string/ConvertStdStringToTypename.hpp"
#include <data_structures/ByteArray.hpp>
#include <IO/dictionary/OpenDictFileException.hpp>
#include <FileSystem/GetCurrentWorkingDir.hpp>

std::map<std::string, TranslateParseByRiseTree::ProcessParseTree_type> 
  ConsoleTranslationController::s_functionName2function;

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
    LogLevel::info /*warning*/
//    LogLevel::warning
    ) ;
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

void ConsoleTranslationController::Insert(
  TranslateParseByRiseTree::ProcessParseTree_type pfnProcessParseTree,
  const char * const str)
{
  s_functionName2function.insert(std::make_pair(str, pfnProcessParseTree) );
  //TODO can't be inserted because of "<" operator needed for std::map's key?
//  s_function2functionName.insert(std::make_pair(pfnProcessParseTree, 
//    str) );
}

void ConsoleTranslationController::CreateMappingBetweenFunctionNameAndFunction()
{
  Insert( & TranslateParseByRiseTree::NonBusySleep, "non-busy sleep");
  Insert( & TranslateParseByRiseTree::ProvokeCacheMisses, "ProvokeCacheMisses");
  Insert( & TranslateParseByRiseTree::TranslateParseTree, "applyTranslRules");
  Insert( & TranslateParseByRiseTree::DummyTranslateParseTree, "dummy");  
}

std::string ConsoleTranslationController::GetFunctionName(
  TranslateParseByRiseTree::ProcessParseTree_type pfnProcessParseTree)
{
  for(std::map<std::string, TranslateParseByRiseTree::ProcessParseTree_type>::
      const_iterator c_iter = s_functionName2function.begin() ; 
      c_iter != s_functionName2function.end(); c_iter ++
      )
  {
    if( c_iter->second == pfnProcessParseTree )
      return c_iter->first;
  }
  return "?";
}

void OutputStatistics(ConsoleTranslationController & consoleTranslationController)
{
  int numJobs = consoleTranslationController.m_multiThreadedTranslation.m_jobNumber2time.size();
  std::cout << " # jobs executed:" << numJobs << std::endl;
  std::map<fastestUnsignedDataType, long double> & jobNumber2time =
    consoleTranslationController.m_multiThreadedTranslation.m_jobNumber2time;
  long double averageJob = 0.0d;
  for( fastestUnsignedDataType threadIndex = 0; threadIndex < numJobs; ++threadIndex )
  {
    std::cout << "job #" << threadIndex << " took " << 
//          consoleTranslationController.m_multiThreadedTranslation.m_threadTimes[threadIndex]
      jobNumber2time[threadIndex]
      << " seconds" << std::endl;
    averageJob += jobNumber2time[threadIndex];
  }
  averageJob /= numJobs;
  std::cout << "average job took " << averageJob << "s" << std::endl;
}

void ConsoleTranslationController::OutputUsage()
{
  std::cout << "usage: <<text to translate>> >># threads for 3rd translation "
    "step<< >>function<< >># iterations of 3rd translation step" << std::endl;
  std::cout << "possible functions:" << std::endl;
  for(std::map<std::string, TranslateParseByRiseTree::ProcessParseTree_type>::
      const_iterator c_iter = s_functionName2function.begin() ; 
      c_iter != s_functionName2function.end(); c_iter ++
      )
  {
      std::cout << c_iter->first << std::endl;
  }
}

int main(int argc, char *  argv[])
{
  std::cout << argc << std::endl;
  ConsoleTranslationController::CreateMappingBetweenFunctionNameAndFunction();
  OpenLogFile(argv[0]);
  
  if( argc > 1 )
  {
    if( strcmp(argv[1], "") == 0 )
      ConsoleTranslationController::OutputUsage();
    else
    try
    {
      std::string std_strCurrentWorkingDir;
      OperatingSystem::GetCurrentWorkingDirA_inl(std_strCurrentWorkingDir);
//      LOGN_INFO("current dir is:\"" << std_strCurrentWorkingDir << "\"")
      std::cout << std_strCurrentWorkingDir << std::endl;
      
      fastestUnsignedDataType num3rdTranslationStepIterations = 1;
      ConsoleTranslationController consoleTranslationController;
      TranslateParseByRiseTree::ProcessParseTree_type pfnProcessParseTree =
        & TranslateParseByRiseTree::ProvokeCacheMisses;
      if( argc > 2 )
      {
        int numThreads;
        if( ! ConvertCharStringToTypename(numThreads, argv[2] ) )
          std::cerr << " error converting ";
        consoleTranslationController.m_multiThreadedTranslation.
          SetNumberOfThreads(numThreads);
        /** Call this method as early as possible as thread creation may take some
         *  (the more threads the more time). */
        consoleTranslationController.m_multiThreadedTranslation.StartThreadsAsync();
        if( argc > 3 )
        {
          std::map<std::string, TranslateParseByRiseTree::
            ProcessParseTree_type>::const_iterator c_iter = 
            ConsoleTranslationController::s_functionName2function.find(argv[3]);
          if( c_iter != ConsoleTranslationController::s_functionName2function.end() )
            pfnProcessParseTree = c_iter->second;
          if( argc > 4 )
          {
            if( ! ConvertCharStringToTypename(num3rdTranslationStepIterations, argv[4]) )
              num3rdTranslationStepIterations = 1;
          }
        }
      }
      g_p_translationcontrollerbase = & consoleTranslationController;
      consoleTranslationController.Init("configuration/VTrans_main_config.xml");
      ByteArray byteArray;
      fastestUnsignedDataType maxNumThreads = consoleTranslationController.
        m_multiThreadedTranslation.GetNumberOfThreads();
      std::cout << "before Translating \"" << argv[1] << "\",max." 
        << maxNumThreads << " threads" << std::endl;
      
      consoleTranslationController.TranslateAsXMLgetAverageTimes(
        argv[1], 
        byteArray, 
        pfnProcessParseTree,
        num3rdTranslationStepIterations
        );
      std::string std_strParseTreeAsIndentedXML = ::GetParseTreeAsIndentedXML(
        consoleTranslationController.m_parsebyrise );
      
      std::cout << "after translate:" << std_strParseTreeAsIndentedXML;
      std::cout << "with max. " << maxNumThreads << "threads" << std::endl;
      std::cout << "after executing: function \"" << ConsoleTranslationController::
        GetFunctionName(pfnProcessParseTree) << "\" for 3rd step" << std::endl;
      std::cout << "average (" << num3rdTranslationStepIterations << "x) apply transl. rules took:" << consoleTranslationController.
        m_numThreadsAndTimeDuration[applyTranslRules].timeDurationInSeconds << "s" << std::endl;
      
      OutputStatistics(consoleTranslationController);
      std::cout << "building parse tree took:" << consoleTranslationController.
        m_numThreadsAndTimeDuration[buildParseTrees].timeDurationInSeconds << "s" << std::endl;
    }catch(VTrans3::OpenDictFileException & odfe )
    {
      std::cout << "dd" << std::endl;
    }
  }
  else
    ConsoleTranslationController::OutputUsage();
  return 0;
}
