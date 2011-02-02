/*
 * dynlib_main.cpp
 *
 *  Created on: Jan 17, 2011
 *      Author: Stefan
 */
#include <Attributes/TranslationAndGrammarPart.hpp>
#include <preprocessor_macros/export_function_symbols.h>
#include <Controller/TranslationControllerBase.hpp>
#include <Controller/Logger/Logger.hpp> //class Logger

Logger g_logger;
//TranslationControllerBase g_translationcontrollerbase;
TranslationControllerBase * g_p_translationcontrollerbase = NULL ;

EXPORT void FreeMemory()
{
  LOGN("FreeMemory--begin")
  if( g_p_translationcontrollerbase)
  {
    delete g_p_translationcontrollerbase;
    g_p_translationcontrollerbase = NULL;
  }
  LOGN("FreeMemory--begin")
}

/**
 * @return: result of initialization: 0=success, else error.
 */
EXPORT BYTE
  Init(
  const char * p_chMainConfigFilePath,
  //Root path where the rule file pathes contained in the main config file
  //are appended to.
  const char * p_chConfigFilesRootPath
  )
{
  LOGN("Init--begin")
  std::string stdstrLogFilePath = "VTrans_log.txt" ;
  g_logger.OpenFile2(stdstrLogFilePath) ;
  //Create on heap because of g_logger access that causes a crash when the log
  //file has not been opened yet?!
  g_p_translationcontrollerbase = new TranslationControllerBase();

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

  std::string stdstrMainConfigFileFullDirectoryPath =
    //stdstrMainConfigFilePath.substr( 0, wIndexOfLastSlashOrBackSlash);
    stdstrConfigFilesRootPath;

  LOGN("Before setting current directory to \"" //main config file's full path"
    << stdstrMainConfigFileFullDirectoryPath << "\"")
  ::SetCurrentDirectory(stdstrMainConfigFileFullDirectoryPath.c_str() );
  LOGN("After setting current directory to \"" //main config file's full path"
    << stdstrMainConfigFileFullDirectoryPath << "\"")
  BYTE byReturn = //g_translationcontrollerbase.Init(//"VTrans_main_config.xml"
    g_p_translationcontrollerbase->Init(
    stdstrMainConfigFilePath);
//  delete g_p_translationcontrollerbase;
  LOGN("Init--return " << (WORD) byReturn)
  return byReturn;
}

/**
 * @return: translated text as 0-terminated character array.
 */
EXPORT char * Translate(const char * p_chEnglishText)
{
  char * ar_chTranslation;
  std::string stdstrWholeInputText(p_chEnglishText);
  std::string stdstrAllPossibilities ;
  std::vector<std::string> stdvec_stdstrWholeTransl;
  std::vector<std::vector<TranslationAndGrammarPart> >
    stdvec_stdvecTranslationAndGrammarPart;
  g_p_translationcontrollerbase->Translate(
    stdstrWholeInputText,
    stdvec_stdstrWholeTransl,
    stdvec_stdvecTranslationAndGrammarPart
    );
  for( std::vector<std::string>::const_iterator c_iter_stdvec_stdstr =
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
  return ar_chTranslation;
}

/**
 * @return: XML data that might be used for InterProcess Communication (can be
 *  sent to the presentation component/ web interface).
 */
EXPORT char * TranslateAsXML(const char * p_chEnglishText//,
  //WORD * p_wNuberOfBytes
  )
{
  char * ar_chTranslation;
  std::string stdstrWholeInputText(p_chEnglishText);
  std::string stdstrAllPossibilities ;
  std::vector<std::string> stdvec_stdstrWholeTransl;
  std::vector<std::vector<TranslationAndGrammarPart> >
    stdvec_stdvecTranslationAndGrammarPart;
  g_p_translationcontrollerbase->Translate(
    stdstrWholeInputText,
    stdvec_stdstrWholeTransl,
    stdvec_stdvecTranslationAndGrammarPart
    );
  stdstrAllPossibilities += "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
    "<sentences>";
  //Loop for all sentenceS.
  for( std::vector<std::vector<TranslationAndGrammarPart> >::const_iterator
      c_iter_stdvec_stdvec_translationandgrammarpart =
      stdvec_stdvecTranslationAndGrammarPart.begin() ;
      c_iter_stdvec_stdvec_translationandgrammarpart !=
        stdvec_stdvecTranslationAndGrammarPart.end()
      ; ++ c_iter_stdvec_stdvec_translationandgrammarpart
      )
  {
    stdstrAllPossibilities += "<sentence>";
    //Loop for each sentence.
    for( std::vector<TranslationAndGrammarPart>::const_iterator
        c_iter_stdvec_translationandgrammarpart =
        c_iter_stdvec_stdvec_translationandgrammarpart->begin() ;
        c_iter_stdvec_translationandgrammarpart !=
            c_iter_stdvec_stdvec_translationandgrammarpart->end()
        ; ++ c_iter_stdvec_translationandgrammarpart
        )
    {
      stdstrAllPossibilities += "<word><translation>";
      stdstrAllPossibilities += c_iter_stdvec_translationandgrammarpart->
        m_stdstrTranslation;
      stdstrAllPossibilities += "</translation></word>";
    }
    stdstrAllPossibilities += "</sentence>";
  }
  stdstrAllPossibilities += "</sentences>";
  ar_chTranslation = new char[stdstrAllPossibilities.length() + 1];
  if( ar_chTranslation )
  {
    memcpy(ar_chTranslation, stdstrAllPossibilities.c_str(),
      stdstrAllPossibilities.length() );
    ar_chTranslation[ stdstrAllPossibilities.length()] = '\0';
  }
  return ar_chTranslation;
}
