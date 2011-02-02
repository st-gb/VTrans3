/*
 * exported_function_declarations.h
 *
 *  Created on: Jan 18, 2011
 *      Author: Stefan
 */

#ifndef EXPORTED_FUNCTION_DECLARATIONS_H_
#define EXPORTED_FUNCTION_DECLARATIONS_H_

/**
 * @return: result of initialization: 0=success, else error.
 */
typedef unsigned char
  ( * Init_type)(
  const char * cp_chMainConfigFilePath
  );
typedef char * ( * Translate_type)(const char * p_chEnglishText);
typedef void ( * FreeMemory_type)();

#endif /* EXPORTED_FUNCTION_DECLARATIONS_H_ */
