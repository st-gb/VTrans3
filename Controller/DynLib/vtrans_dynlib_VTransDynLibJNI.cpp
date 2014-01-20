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
#include <FileSystem/SetCurrentWorkingDir.hpp>

//#define JNIEXPORT EXPORT

#ifndef _Included_vtrans_dynlib_VTransDynLibJNI
#define _Included_vtrans_dynlib_VTransDynLibJNI

#ifdef __cplusplus
	/** For avoiding C++ name mangling. */
	extern "C" {
	#define JNI_ENV_ACCESS(x) x
    #define JNI_ENV_POINTER(x)
#else
  #include <malloc.h>
  #define JNI_ENV_ACCESS(x) (*x)
  #define JNI_ENV_POINTER(x) x,
#endif

/*
 * Class:     vtrans_dynlib_VTransDynLibJNI
 * Method:    Init
 * Signature: (Ljava/lang/String;)B
 */
//JNIEXPORT
extern "C" jbyte /*JNICALL*/ Java_vtrans_dynlib_VTransDynLibJNI_Init
  (JNIEnv * p_jni_env, //jobject thiz
    jclass jc,
    jstring jstrMainCfgFile,
    jstring jstrCfgFilesRootPath,
    jstring jstrCurrentWorkingDir)
{
  jboolean jb = /*false*/ JNI_FALSE;
  const char * strCurrentWorkingDir = JNI_ENV_ACCESS(p_jni_env)->GetStringUTFChars(
    JNI_ENV_POINTER(p_jni_env)
    jstrCurrentWorkingDir, //NULL
	& jb);
  ::SetCurrentWorkingDir_inl(strCurrentWorkingDir);
  LOGN( //"Java_vtrans_1dynlib_VTransDynLibJNI_Init"
    FULL_FUNC_NAME << "--begin")
//  const jbyte * str;
  const char * strMainConfigFile;
  const char * strConfigFilesRootPath;
  //from http://java.sun.com/docs/books/jni/html/objtypes.html:
  strMainConfigFile = //( *
  JNI_ENV_ACCESS(p_jni_env)->GetStringUTFChars(//jni_env,
	  JNI_ENV_POINTER(p_jni_env)
      jstrMainCfgFile, //NULL
      & jb);
  strConfigFilesRootPath = JNI_ENV_ACCESS(p_jni_env)->GetStringUTFChars(
	  JNI_ENV_POINTER(p_jni_env)
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
    "before ReleaseStringUTFChars " << strMainConfigFile )
  JNI_ENV_ACCESS(p_jni_env)->ReleaseStringUTFChars(
    JNI_ENV_POINTER(p_jni_env)
    jstrMainCfgFile, strMainConfigFile);
  LOGN_DEBUG(//"Java_vtrans_1dynlib_VTransDynLibJNI_Init" FULL_FUNC_NAME <<
    "before ReleaseStringUTFChars " << strConfigFilesRootPath )
  JNI_ENV_ACCESS(p_jni_env)->ReleaseStringUTFChars(
    JNI_ENV_POINTER(p_jni_env)
    jstrCfgFilesRootPath,
    strConfigFilesRootPath);
  LOGN_DEBUG("before ReleaseStringUTFChars " << strCurrentWorkingDir )
  JNI_ENV_ACCESS(p_jni_env)->ReleaseStringUTFChars(
    JNI_ENV_POINTER(p_jni_env)
    jstrCurrentWorkingDir,
    strCurrentWorkingDir);
  LOGN(//"Java_vtrans_1dynlib_VTransDynLibJNI_Init" FULL_FUNC_NAME <<
    "return " << (unsigned) jInitResult)
  return jInitResult;
}

#ifndef TEST_MINI_XML
/*
 * Class:     vtrans_dynlib_VTransDynLibJNI
 * Method:    Translate
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_vtrans_dynlib_VTransDynLibJNI_Translate
  (JNIEnv * p_jni_env, //jobject,
    jclass jc, jstring jstrEnglishText)
{
  LOGN( //"Java_vtrans_1dynlib_VTransDynLibJNI_Translate"
    FULL_FUNC_NAME << "--begin")
  jboolean jb = /*false*/ JNI_FALSE;
//  const jbyte * str;
  const char * str;
  //from http://java.sun.com/docs/books/jni/html/objtypes.html:
  str = JNI_ENV_ACCESS(p_jni_env)->GetStringUTFChars(
	  JNI_ENV_POINTER(p_jni_env)
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
  JNI_ENV_ACCESS(p_jni_env)->ReleaseStringUTFChars(
    JNI_ENV_POINTER(p_jni_env)
    jstrEnglishText, str);
  jstring jstr = JNI_ENV_ACCESS(p_jni_env)->NewStringUTF(
	  JNI_ENV_POINTER(p_jni_env)
	  ar_chTranslation);
#ifdef __cplusplus
  delete [] ar_chTranslation;
#else
  //TODO free may only be called when allocated via malloc(...)?!
  free(ar_chTranslation);
#endif
  LOGN("Java_vtrans_1dynlib_VTransDynLibJNI_Translate--end")
  return jstr;
}

/*
 * Class:     vtrans_dynlib_VTransDynLibJNI
 * Method:    FreeMemory
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_vtrans_dynlib_VTransDynLibJNI_FreeMemory
  (JNIEnv * p_jni_env, //jobject
    jclass jc)
{
  LOGN("Java_vtrans_1dynlib_VTransDynLibJNI_FreeMemory--begin")
  FreeMemory();
}
#endif //TEST_MINI_XML

#ifdef __cplusplus
}
#endif
#endif
