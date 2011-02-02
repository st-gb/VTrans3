/*
 * dynlib_main.h
 *
 *  Created on: Jan 18, 2011
 *      Author: Stefan
 */

#ifndef DYNLIB_MAIN_H_
#define DYNLIB_MAIN_H_

#include <preprocessor_macros/export_function_symbols.h>

typedef unsigned char BYTE;

EXPORT void FreeMemory();
/**
 * @return: result of initialization: 0=success, else error.
 */
EXPORT BYTE
  Init(
  const char * p_chMainConfigFilePath,
  //Root path where the rule file pathes contained in the main config file
  //are appended to.
  const char * p_chConfigFilesRootPath
  );
/**
 * @return: translated text as 0-terminated character array.
 */
EXPORT char * Translate(const char * p_chEnglishText);
/**
 * @return: XML data that might be used for InterProcess Communication (can be
 *  sent to the presentation component/ web interface).
 */
EXPORT char * TranslateAsXML(const char * p_chEnglishText);
#endif /* DYNLIB_MAIN_H_ */
