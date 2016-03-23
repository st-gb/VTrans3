/*
 * dynlib_main.h
 *
 *  Created on: Jan 18, 2011
 *      Author: Stefan
 */

#ifndef DYNLIB_MAIN_H_
#define DYNLIB_MAIN_H_

#include <preprocessor_macros/export_function_symbols.h>
//#include <data_structures/ByteArray.hpp>

/** Forward decl.*/
class ByteArray;

typedef unsigned char BYTE;

EXPORT void FreeMemory();
EXPORT void Stop();

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
/** @param item: detailed message of current status: current word/ translation
 *    rule path etc.
 *  @param time: time when the last status was set
 * @return: status of current translation: must be a number because a status
 *   message may be shown in different languages
 */
EXPORT BYTE GetStatus(std::string & item, struct tm & time);
EXPORT BYTE GetStatus2( /*const std::string & str*/
  const std::string & str, struct tm & time, ByteArray & byteArray);
EXPORT void Settings(const char * cp_chName, const char * cp_chValue);
/**
 * @return: translated text as 0-terminated character array.
 */
EXPORT char * Translate(const char * p_chEnglishText);
/**
 * @return: XML data that might be used for InterProcess Communication (can be
 *  sent to the presentation component/ web interface).
 */
EXPORT /*char * */ /*ByteArray*/ void TranslateAsXML(const char * p_chEnglishText,
  ByteArray &);
#endif /* DYNLIB_MAIN_H_ */
