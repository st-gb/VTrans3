/*
 * dynlib_main.cpp
 *
 *  Created on: Jan 17, 2011
 *      Author: Stefan
 */
#include <Attributes/TranslationAndGrammarPart.hpp>
#include <Attributes/TranslationResult.hpp> //class TranslationResult
#include <Controller/character_string/stdstring_format.hpp>//to_stdstring(...)
#include <Controller/Logger/Logger.hpp> //class Logger
#include <Controller/GetErrorMessageFromLastErrorCode.hpp>
#include <data_structures/ByteArray.hpp> //class ByteArray
#include <FileSystem/GetCurrentWorkingDir.hpp>
//class CSS::LogFormatter::Log4jFormatter
#include <Controller/Logger/Formatter/Log4jFormatter.hpp>
#include <Controller/TranslationControllerBase.hpp>
#include <preprocessor_macros/export_function_symbols.h>
//LOGN(...), g_logger
#include <preprocessor_macros/logging_preprocessor_macros.h>
#include <IO/GenerateXMLtreeFromParseTree.hpp>
//#include <InputOutput/XML/OutputXMLindented.hpp>

//Logger g_logger;
//TranslationControllerBase g_translationcontrollerbase;
TranslationControllerBase * g_p_translationcontrollerbase = NULL ;

//http://tldp.org/HOWTO/Program-Library-HOWTO/miscellaneous.html#INIT-AND-CLEANUP
//Libraries should export initialization and cleanup routines using the gcc
// __attribute__((constructor)) and
// __attribute__((destructor)) function attributes.
// See the gcc info pages for information on these.
//Constructor routines are executed before dlopen returns (or before main() is
// started if the library is loaded at load time).
//Destructor routines are executed before dlclose returns (or after exit() or
// completion of main() if the library is loaded at load time).
// The C prototypes for these functions are:
//
//  void __attribute__ ((constructor)) my_init(void);
//  void __attribute__ ((destructor)) my_fini(void);
//
//Shared libraries must not be compiled with the gcc arguments
//``-nostartfiles'' or ``-nostdlib''. If those arguments are used, the constructor/destructor routines will not be executed (unless special measures are taken).

void __attribute__ ((constructor)) my_init(void)
{
//  LogLevel::CreateLogLevelStringToNumberMapping();
}

//int main(void)
//{
//  LogLevel::CreateLogLevelStringToNumberMapping();
//}

EXPORT void Stop()
{
  g_p_translationcontrollerbase->Stop();
}

EXPORT void FreeMemory()
{
  LOGN("begin")
  if( g_p_translationcontrollerbase)
  {
    delete g_p_translationcontrollerbase;
    g_p_translationcontrollerbase = NULL;
  }
  LOGN("end")
}

void __attribute__ ((destructor)) my_fini(void)
{
  FreeMemory();
}

/**
 * @return: result of initialization: 0=success, else error.
 */
EXPORT BYTE
  Init(
  const char * p_chMainConfigFilePath,
  //Root path where the rule file pathes contained in the main config file
  //are appended to.
  const char * const p_chConfigFilesRootPath
  )
{
//  LogLevel::CreateLogLevelStringToNumberMapping();
  //g_logger.SetFormatter(new CSS::LogFormatter::Log4jFormatter(& g_logger) );
//  LOG_LOGGER_NAME_THREAD_UNSAFE(g_logger, "Init--begin")
  LOGN("begin")
//  LOGN("Init--begin")
#ifdef /*COMPILE_WITH_LOG*/ USE_OWN_LOGGER
  const char * const logFormat = "log4j";
  //TODO use other path than this one as log file directory
  std::string stdstrLogFilePath = p_chConfigFilesRootPath;
  stdstrLogFilePath += "/VTransDynlib_log.";
  stdstrLogFilePath += logFormat;
  bool bFileIsOpen = g_logger.OpenFileA(stdstrLogFilePath, logFormat, 4000,
    LogLevel::debug) ;
#ifdef __linux__
#ifndef __ANDROID__
  g_logger.AddConsoleLogEntryWriter();
#endif
#endif
  //g_logger.
  if( ! bFileIsOpen )
  {
    std::cerr << "Failed to open the log file:"
      << GetErrorMessageFromLastErrorCodeA() << std::endl;
    return TranslationControllerBaseClass::InitFunction::creatingLogFileFailed;
  }
  LOGN_INFO("compile time:" << __DATE__ << " " << __TIME__ )
#endif
  std::string std_strCurrentWorkingDir;
  OperatingSystem::GetCurrentWorkingDirA_inl(std_strCurrentWorkingDir);
  LOGN_INFO("current dir is:\"" << std_strCurrentWorkingDir << "\"")
  //Create on heap because of g_logger access that causes a crash when the log
  //file has not been opened yet?!
  g_p_translationcontrollerbase = new TranslationControllerBase();
  g_p_translationcontrollerbase->m_std_strOriginalCurrWorkDir = std_strCurrentWorkingDir;


  std::string stdstrMainConfigFilePath(p_chMainConfigFilePath);
  std::string stdstrConfigFilesRootPath(p_chConfigFilesRootPath);

//  std::string::size_type stdstdstrsize_typeLastSlash =
//    stdstrMainConfigFilePath.find_last_of('/');
//  std::string::size_type stdstdstrsize_typeLastBackSlash =
//    stdstrMainConfigFilePath.find_last_of('\\');
//  LOGN("Index of last slash:" << stdstdstrsize_typeLastSlash)
//  LOGN("Index of last backslash:" << stdstdstrsize_typeLastBackSlash)
//
//  WORD wIndexOfLastSlashOrBackSlash = stdstdstrsize_typeLastSlash !=
//    std::string::npos ? stdstdstrsize_typeLastSlash : 0;
//  if( stdstdstrsize_typeLastBackSlash != std::string::npos &&
//      stdstdstrsize_typeLastBackSlash > wIndexOfLastSlashOrBackSlash)
//    wIndexOfLastSlashOrBackSlash = stdstdstrsize_typeLastBackSlash;

//  g_p_translationcontrollerbase->SetCurrentDirToConfigFilesRootPath(
//    stdstrConfigFilesRootPath);

  BYTE byReturn = //g_translationcontrollerbase.Init(//"VTrans_main_config.xml"
    g_p_translationcontrollerbase->Init(
    stdstrMainConfigFilePath);
//  delete g_p_translationcontrollerbase;
  LOGN("return " << (fastestUnsignedDataType) byReturn
    //<< TranslationControllerBaseClass::InitFunction::retCodeDescriptions[byReturn]
    )
  return byReturn;
}

#ifndef TEST_MINI_XML

//GenerateXMLtreeFromParseTree()
//{
//  stdstrAllPossibilities += "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
//    "<sentences>";
//  //Loop for all sentenceS beginning at the same token index.
//  for( std::vector< std::vector <std::vector <TranslationAndGrammarPart> > >
//      ::const_iterator
//      c_iter_stdvec_stdvec_translationandgrammarpartSentenceAtSameTokenIndex =
//      stdvec_stdvecTranslationAndGrammarPart.begin() ;
//      c_iter_stdvec_stdvec_translationandgrammarpartSentenceAtSameTokenIndex !=
//        stdvec_stdvecTranslationAndGrammarPart.end()
//      ; ++ c_iter_stdvec_stdvec_translationandgrammarpartSentenceAtSameTokenIndex
//      )
//  {
//    stdstrAllPossibilities += "<sentence>";
//    //Loop for each word vector of sentence beginn.
//    for( std::vector <std::vector <TranslationAndGrammarPart> >::
//        const_iterator
//        c_iter_stdvec_WordAtSameTokenIndex =
//        c_iter_stdvec_stdvec_translationandgrammarpartSentenceAtSameTokenIndex->begin() ;
//        c_iter_stdvec_WordAtSameTokenIndex !=
//            c_iter_stdvec_stdvec_translationandgrammarpartSentenceAtSameTokenIndex->end()
//        ; ++ c_iter_stdvec_WordAtSameTokenIndex
//        )
//    {
//      //Loop for each word beginning at the same .
//      for( std::vector <TranslationAndGrammarPart> ::
//          const_iterator
//          c_iter_stdvec_Word =
//            c_iter_stdvec_WordAtSameTokenIndex->begin() ;
//          c_iter_stdvec_Word !=
//            c_iter_stdvec_WordAtSameTokenIndex->end()
//          ; ++ c_iter_stdvec_Word
//          )
//      {
//      LOGN("::TranslateAsXML(...) TranslationAndGrammarPart iterator--"
//        "c_iter_stdvec_translationandgrammarpart->mp_grammarpart:" <<
//        c_iter_stdvec_Word->mp_grammarpart )
//  //      stdstrAllPossibilities += "<word><translation>";
//      WORD wGrammarPartID = c_iter_stdvec_Word->
//        mp_grammarpart->m_wGrammarPartID;
//      stdstrAllPossibilities += "<word><translation "
//        "grammar_part_ID=\"" + convertToStdString( wGrammarPartID ) + "\"" +
//        " grammar_part_name=\"" + g_p_translationcontrollerbase->m_parsebyrise.
//        GetGrammarPartName( wGrammarPartID) + "\""
//        ">";
//      stdstrAllPossibilities += c_iter_stdvec_Word->
//        m_stdstrTranslation;
//      stdstrAllPossibilities += "</translation></word>";
//      }
//    }
//    stdstrAllPossibilities += "</sentence>";
//  }
//  stdstrAllPossibilities += "</sentences>";
//}

/**
 * @return: XML data that might be used for InterProcess Communication (can be
 *  sent to the presentation component/ web interface).
 */
EXPORT /*char * */ void TranslateAsXML(const char * p_chEnglishText//,
  //WORD * p_wNuberOfBytes
  , ByteArray & byteArray
  )
{
  LOGN(/*"::TranslateAsXML(...)"*/ "begin")
//  ByteArray byteArray(512);
  g_p_translationcontrollerbase->m_vbContinue = true;
  char * ar_chTranslation;
  std::string stdstrWholeInputText(p_chEnglishText);
  std::string stdstrAllPossibilities ;
  std::vector<std::string> stdvec_stdstrWholeTransl;
//  std::vector< std::vector <std::vector <TranslationAndGrammarPart> > >
//    stdvec_stdvecTranslationAndGrammarPart;
  TranslationResult translationResult;
  g_p_translationcontrollerbase->Translate(
    stdstrWholeInputText,
    stdvec_stdstrWholeTransl,
    /*stdvec_stdvecTranslationAndGrammarPart*/ translationResult
    );

  stdstrAllPossibilities = "";
  IO::GenerateXMLtreeFromParseTree( & g_p_translationcontrollerbase->m_parsebyrise,
    /*stdstrAllPossibilities*/ byteArray);
  ar_chTranslation = new char[stdstrAllPossibilities.length() + 1];
  if( ar_chTranslation )
  {
    memcpy(ar_chTranslation, stdstrAllPossibilities.c_str(),
      stdstrAllPossibilities.length() );
    ar_chTranslation[ stdstrAllPossibilities.length()] = '\0';
  }
  LOGN("XML as tree: ")
//  OutputXMLindented( stdstrAllPossibilities.c_str(),
//    * g_logger.mp_ofstream );
  LOGN("")
  LOGN("::TranslateAsXML(...) end")
//  return /*ar_chTranslation*/ byteArray;
}

EXPORT BYTE GetStatus(std::string & item, struct tm & time)
{
  return g_p_translationcontrollerbase->GetStatus(item, time);
}

EXPORT BYTE GetStatus2( /*const std::string & str*/
  const std::string & str, struct tm & time, ByteArray & byteArray)
{
  return g_p_translationcontrollerbase->GetStatus2(str, time, byteArray);
}

EXPORT void Settings(const char * cp_chName, const char * cp_chValue)
{
  g_p_translationcontrollerbase->Settings(cp_chName, cp_chValue);
}

/**
 * @return: translated text as 0-terminated character array.
 */
EXPORT char * Translate(const char * p_chEnglishText)
{
  LOGN("::Translate(...) begin")
  char * ar_chTranslation;
  std::string stdstrWholeInputText(p_chEnglishText);
  std::string stdstrAllPossibilities ;
  std::vector<std::string> stdvec_stdstrWholeTransl;
//  std::vector< std::vector <std::vector <TranslationAndGrammarPart> > >
//    stdvec_stdvecTranslationAndGrammarPart;
  TranslationResult translationResult;
  g_p_translationcontrollerbase->Translate(
    stdstrWholeInputText,
    stdvec_stdstrWholeTransl,
    /*stdvec_stdvecTranslationAndGrammarPart*/ translationResult
    );
  for( std::vector< std::string> ::const_iterator
      c_iter_stdvec_stdstr =
      stdvec_stdstrWholeTransl.begin() ;
      c_iter_stdvec_stdstr != stdvec_stdstrWholeTransl.end()
      ; ++ c_iter_stdvec_stdstr
      )
  {
    stdstrAllPossibilities += * c_iter_stdvec_stdstr + "\n" ;
  }
  ar_chTranslation = new char[stdstrAllPossibilities.length() + 1];
  if( ar_chTranslation )
  {
    memcpy(ar_chTranslation, stdstrAllPossibilities.c_str(),
      stdstrAllPossibilities.length() );
    ar_chTranslation[ stdstrAllPossibilities.length()] = '\0';
  }
  LOGN("::Translate(...) end")
  return ar_chTranslation;
}
#endif //#ifdef TEST_MINI_XML