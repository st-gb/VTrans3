/*
 * vtrans_dynlib_VTransDynLibJNI.c
 *
 *  Created on: Jan 18, 2011
 *      Author: Stefan
 *
 * see http://www.mingw.org/node/41:
 * compile with_
 *   preprocessor macro "-D_JNI_IMPLEMENTATION_",
 *   linker option "-Wl,--kill-at"
 */

#include <jni.h> //JNIEXPORT etc.

/* Header for class vtrans_dynlib_VTransDynLibJNI */
//#include <preprocessor_macros/export_function_symbols.h>
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
#include "dynlib_main.h" //Init(...) etc.
#include <stddef.h> //for "NULL"

//#define JNIEXPORT EXPORT

#ifndef _Included_vtrans_dynlib_VTransDynLibJNI
#define _Included_vtrans_dynlib_VTransDynLibJNI
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     vtrans_dynlib_VTransDynLibJNI
 * Method:    Init
 * Signature: (Ljava/lang/String;)B
 */
JNIEXPORT
jbyte JNICALL Java_vtrans_1dynlib_VTransDynLibJNI_Init
  (JNIEnv * p_jni_env, //jobject thiz
    jclass jc,
    jstring jstrMainCfgFile,
    jstring jstrCfgFilesRootPath)
{
  LOGN( //"Java_vtrans_1dynlib_VTransDynLibJNI_Init"
    FULL_FUNC_NAME << "--begin")
  jboolean jb = false;
//  const jbyte * str;
  const char * strMainConfigFile;
  const char * strConfigFilesRootPath;
  //from http://java.sun.com/docs/books/jni/html/objtypes.html:
  strMainConfigFile = //( *
    p_jni_env//)
    ->GetStringUTFChars(//jni_env,
      jstrMainCfgFile, //NULL
      & jb);
  strConfigFilesRootPath = p_jni_env->GetStringUTFChars(//jni_env,
    jstrCfgFilesRootPath, //NULL
    & jb);
  LOGN( //"Java_vtrans_1dynlib_VTransDynLibJNI_Translate"
    FULL_FUNC_NAME << "\n--main config file: " << strMainConfigFile
    << "strConfigFilesRootPath:" << strConfigFilesRootPath )
  if (strMainConfigFile == NULL || strConfigFilesRootPath == NULL)
  {
    return //NULL; /* OutOfMemoryError already thrown */
      100;
  }

//  const char * p_chMainConfigFilePath =
//    (* jni_env).GetStringChars(//jni_env,
//      jstrMainCfgFile,//0
//      & jb
//      );
  jbyte jInitResult = Init(//p_chMainConfigFilePath
    strMainConfigFile, strConfigFilesRootPath);
  //jni_env->ReleaseStringChars(jstrMainCfgFile,p_chMainConfigFilePath);
  //( *
  LOGN_DEBUG(//"Java_vtrans_1dynlib_VTransDynLibJNI_Init" FULL_FUNC_NAME <<
    "before ReleaseStringUTFChars" << strMainConfigFile )
  p_jni_env//)
    ->ReleaseStringUTFChars(//jni_env,
    jstrMainCfgFile, strMainConfigFile);
  LOGN_DEBUG(//"Java_vtrans_1dynlib_VTransDynLibJNI_Init" FULL_FUNC_NAME <<
    "before ReleaseStringUTFChars" << strConfigFilesRootPath )
  p_jni_env//)
    ->ReleaseStringUTFChars(//jni_env,
    jstrCfgFilesRootPath, strConfigFilesRootPath);
  LOGN(//"Java_vtrans_1dynlib_VTransDynLibJNI_Init" FULL_FUNC_NAME <<
    "return " << jInitResult)
  return jInitResult;
}

/*
 * Class:     vtrans_dynlib_VTransDynLibJNI
 * Method:    Translate
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_vtrans_1dynlib_VTransDynLibJNI_Translate
  (JNIEnv * p_jni_env, //jobject,
    jclass jc, jstring jstrEnglishText)
{
  LOGN( //"Java_vtrans_1dynlib_VTransDynLibJNI_Translate"
    FULL_FUNC_NAME << "--begin")
  jboolean jb = false;
//  const jbyte * str;
  const char * str;
  //from http://java.sun.com/docs/books/jni/html/objtypes.html:
  str = //( *
    p_jni_env//)
    ->GetStringUTFChars(//jni_env,
      jstrEnglishText, //NULL
      & jb);
  LOGN(//"Java_vtrans_1dynlib_VTransDynLibJNI_Translate"
    FULL_FUNC_NAME <<
    "--English text from "
    "Java string:\"" << str << "\"")
  if (str == NULL)
  {
    return //NULL; /* OutOfMemoryError already thrown */
      NULL;
  }
//  const char * p_chMainConfigFilePath =
//    (* jni_env).GetStringChars(//jni_env,
//      jstrMainCfgFile,//0
//      & jb
//      );
  char * ar_chTranslation = //Translate(//p_chMainConfigFilePath
    TranslateAsXML(
    str);
  LOGN("Java_vtrans_1dynlib_VTransDynLibJNI_Translate--transl. result:"
    << ar_chTranslation)
  //jni_env->ReleaseStringChars(jstrMainCfgFile,p_chMainConfigFilePath);
  //( *
  p_jni_env//)
    ->ReleaseStringUTFChars(//jni_env,
    jstrEnglishText, str);
  jstring jstr = p_jni_env->NewStringUTF(ar_chTranslation);
  delete [] ar_chTranslation;
  LOGN("Java_vtrans_1dynlib_VTransDynLibJNI_Translate--end")
  return jstr;
}

/*
 * Class:     vtrans_dynlib_VTransDynLibJNI
 * Method:    FreeMemory
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_vtrans_1dynlib_VTransDynLibJNI_FreeMemory
  (JNIEnv *, //jobject
    jclass jc)
{
  LOGN("Java_vtrans_1dynlib_VTransDynLibJNI_FreeMemory--begin")
  FreeMemory();
}

#ifdef __cplusplus
}
#endif
#endif
