/** File:   ConsoleTranslationController.cpp
 * Author: sg
 * Created on 9. Mai 2017, 11:20 */

#include "ConsoleTranslationController.hpp"
#include "Controller/character_string/ConvertStdStringToTypename.hpp"
#include <data_structures/ByteArray.hpp>
#include <IO/dictionary/OpenDictFileException.hpp>
#include <FileSystem/GetCurrentWorkingDir.hpp>

CommandLineOption/*<char>*/ ConsoleTranslationController::s_commandLineOptions [] = {
//   <<text to translate>> 
  {"numThreads", "# of threads for 3rd translation step", SetNumTranslationThreads},
  {"3rdTranslStepFunction", "function to execute as third translation step", 
    Set3rdTranslationStepFunction},
  {"numIters", "# iterations of 3rd translation step", SetNumIterationsFor3rdTranslationStep},
  {""}
};
//void SetLogLevel(const char * const value)
//{
//  Logger::GetAsNumber(value);
//}

ConsoleTranslationController::ConsoleTranslationController() {
//  s_cmdLineOpt2Function.insert("logLevel", & SetLogLevel);
//  s_cmdLineOpt2Function.insert("logLevel", SetLogLevel);
  ValuesAndFunction valuesAndFunctionLogLevel = { (char *) "", & SetLogLevel };
  s_settingsName2valueAndFunction.insert(
    std::make_pair("logLevel", valuesAndFunctionLogLevel) );
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
//    LogLevel::info /*warning*/
    LogLevel::warning
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

void ConsoleTranslationController::OutputStatistics()
{
  const int numJobs = m_multiThreadedTranslation.m_jobNumber2time.size();
  std::cout << " # jobs executed:" << numJobs << std::endl;
  std::map<fastestUnsignedDataType, long double> & jobNumber2time =
    m_multiThreadedTranslation.m_jobNumber2time;
  VTrans3::MultiThreadedTranslation::jobNumber2CPUtime_type & jobNumber2CPUtime =
    m_multiThreadedTranslation.m_threadIndex2CPUtime;
  long double averageJob = 0.0d;
//  struct timespec _timespec;
  for( fastestUnsignedDataType jobIndex = 0; jobIndex < numJobs; ++jobIndex )
  {
//    std::cout << "job #" << jobIndex << " took " << 
////          consoleTranslationController.m_multiThreadedTranslation.m_threadTimes[threadIndex]
//      jobNumber2time[jobIndex]
//      << " seconds";
    /** from http://man7.org/linux/man-pages/man3/pthread_getcpuclockid.3.html */
//    if(clock_gettime(jobNumber2CPUtime[threadIndex], &_timespec) != -1)
    //from https://linux.die.net/man/2/getrusage
//    std::cout << std::endl;
    averageJob += jobNumber2time[jobIndex];
  }
  averageJob /= numJobs;
  std::cout << "average job took " << averageJob << "s" << std::endl;

  for( fastestUnsignedDataType threadIndex = 0; threadIndex <
    m_multiThreadedTranslation.GetNumberOfThreads(); ++threadIndex )
  {
      struct timeval & r_timeval_User = jobNumber2CPUtime[threadIndex].ru_utime;
      struct timeval & r_timeval_System = jobNumber2CPUtime[threadIndex].ru_stime;
    std::cout << "for thread #" << threadIndex <<  ": CPU time:" //_timespec.tv_sec << "."
  //      << (_timespec.tv_nsec / 1000000) << "s";
      << "user:" << r_timeval_User.tv_sec << "s, " << r_timeval_User.tv_usec / 1000 << "ms "
      << "system:" << r_timeval_System.tv_sec << "s, " << r_timeval_System.tv_usec / 1000 << "ms"
      << std::endl;
  }
}

void ConsoleTranslationController::OutputUsage()
{
  std::cout << "possible options:" << std::endl;
  TranslationControllerBase::settingsName2ValueAndFunction_type::const_iterator
    c_iterSettingsName2ValueAndFunction = TranslationControllerBase::s_settingsName2valueAndFunction.begin();
  for( ; c_iterSettingsName2ValueAndFunction != TranslationControllerBase::s_settingsName2valueAndFunction.end(); 
      c_iterSettingsName2ValueAndFunction ++ )
  {
    std::string optionName = c_iterSettingsName2ValueAndFunction->first;
    std::cout << optionName << " ";
  }
//  std::cout << "\nusage: <<text to translate>> >># threads for 3rd translation "
//    "step<< >>function<< >># iterations of 3rd translation step<< >>log level<<" << std::endl;
  std::cout << "\npossible functions:" << std::endl;
  for(std::map<std::string, TranslateParseByRiseTree::ProcessParseTree_type>::
      const_iterator c_iter = s_functionName2function.begin() ; 
      c_iter != s_functionName2function.end(); c_iter ++
      )
  {
      std::cout << c_iter->first << std::endl;
  }
  
  std::cout << "possible log levels: warning, error " << std::endl;
  //TODO implement an iterator for trie
//  NodeTrieNode<BYTE>::const_ * p_ntn = Logger::s_nodetrieLogLevelStringToNumber.begin();
}

void ProcessCommandLineArgs(int argc, char *  argv[])
{
  for( int argumentIndex = 1; argumentIndex < argc ; argumentIndex ++ )
  {
    char * pointerToEquationSign = strchr(argv[argumentIndex], '=');
    if( pointerToEquationSign != NULL )
    {
      std::string optionName = std::string(argv[argumentIndex], 
        pointerToEquationSign - argv[argumentIndex]);

      TranslationControllerBase::settingsName2ValueAndFunction_type::const_iterator c_iter = 
        TranslationControllerBase::s_settingsName2valueAndFunction.find(optionName);
      if( c_iter != TranslationControllerBase::s_settingsName2valueAndFunction.end() )
      {
        std::string optionValue = std::string(pointerToEquationSign + 1);
        c_iter->second.p_function( (const char * const) optionValue.c_str() );
      }
      else
      {
        std::cerr << "unknown option name \"" << optionName << "\"" << std::endl;
        ConsoleTranslationController::OutputUsage();
      }
    }
  }
}

int main(int argc, char *  argv[])
{
//  std::cout << argc << std::endl;
  ConsoleTranslationController::CreateMappingBetweenFunctionNameAndFunction();
  Logger::CreateLogLevelStringToNumberMapping();
  OpenLogFile(argv[0]);
  
  ConsoleTranslationController consoleTranslationController;
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
      g_p_translationcontrollerbase = & consoleTranslationController;
      ProcessCommandLineArgs(argc, argv);
      
      consoleTranslationController.Init("configuration/VTrans_main_config.xml");
      ByteArray byteArrayXMLparseTree;
      fastestUnsignedDataType maxNumThreads = consoleTranslationController.
        s_numParallelTranslationThreads;
      std::cout << "before Translating \"" << consoleTranslationController.m_englishText << "\",max." 
        << maxNumThreads << " threads" << std::endl;
      
      consoleTranslationController.TranslateAsXMLgetAverageTimes(
        consoleTranslationController.m_englishText.c_str(),
        byteArrayXMLparseTree, 
        ConsoleTranslationController::s_3rdStepTranslationFunction,
        ConsoleTranslationController::s_num3rdTranslationStepIterations
        );
      std::string std_strParseTreeAsIndentedXML = ::GetParseTreeAsIndentedXML(
        consoleTranslationController.m_parsebyrise );
      
      std::cout << "after translate:" << std_strParseTreeAsIndentedXML;
      std::cout << "with max." << maxNumThreads;
#ifdef COMPILE_WITH_OPENMP
      std::cout << " OpenMP";
#else
      std::cout << " own thread pool";
#endif //#ifdef COMPILE_LOGGER_WITH_OPENMP
      std::cout << " threads" << std::endl;
      std::cout << "after executing: function \"" << ConsoleTranslationController::
        GetFunctionName(ConsoleTranslationController::s_3rdStepTranslationFunction) << "\" for 3rd step" << std::endl;
      std::cout << "average (" << ConsoleTranslationController::s_num3rdTranslationStepIterations << "x) apply transl. rules took:" << consoleTranslationController.
        m_numThreadsAndTimeDuration[applyTranslRules].timeDurationInSeconds << "s" << std::endl;
      
      consoleTranslationController.OutputStatistics();
      std::cout << "building parse tree took:" << consoleTranslationController.
        m_numThreadsAndTimeDuration[buildParseTrees].timeDurationInSeconds << "s" << std::endl;
    }catch(VTrans3::OpenDictFileException & odfe )
    {
      std::cout << "error opening dictionary file: \"" << 
        consoleTranslationController.m_stdstrVocabularyFilePath << "\"" 
          << "error code:" << odfe.m_openError << 
        odfe.GetErrorMessageA() << std::endl;
    }
  }
  else
    ConsoleTranslationController::OutputUsage();
  return 0;
}
