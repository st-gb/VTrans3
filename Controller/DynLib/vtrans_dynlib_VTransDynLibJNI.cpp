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
#include <Controller/character_string/format_as_string.hpp>
#include "dynlib_main.h" //Init(...) etc.
#include <character_string/modifiedUTF8.hpp>
#include <data_structures/ByteArray.hpp> //class ByteArray
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

//http://developer.android.com/training/articles/perf-jni.html#native_libraries
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    /** Create the log level mapping here because it should be created before
     *  the "Settings" can be called (because Settings can be called with
     *  parameters "logging" and "disable". */
    LogLevel::CreateLogLevelStringToNumberMapping();

    // Get jclass with env->FindClass.
    // Register methods with env->RegisterNatives.

    return JNI_VERSION_1_2;
}

/* Class:     vtrans_dynlib_VTransDynLibJNI
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
  OperatingSystem::SetCurrentWorkingDirA_inl(strCurrentWorkingDir);
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

void ReleaseArrayMemory(char * const array)
{
  #ifdef __cplusplus
    delete [] array;
  #else
    //TODO free may only be called when allocated via malloc(...)?!
    free(array);
  #endif
}

/*
 * Class:     vtrans_dynlib_VTransDynLibJNI
 * Method:    Translate
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_vtrans_dynlib_VTransDynLibJNI_Translate
  (JNIEnv * p_jni_env, //jobject,
    jclass jc,
    jstring jstrEnglishText)
{
  LOGN( //"Java_vtrans_1dynlib_VTransDynLibJNI_Translate" FULL_FUNC_NAME << "--"
    "begin")
  jboolean jb = /*false*/ JNI_FALSE;
//  const jbyte * str;
  const char * str;
  //from http://java.sun.com/docs/books/jni/html/objtypes.html:
  str = JNI_ENV_ACCESS(p_jni_env)->GetStringUTFChars(
    JNI_ENV_POINTER(p_jni_env)
    jstrEnglishText, //NULL
    & jb);
  LOGN(//"Java_vtrans_1dynlib_VTransDynLibJNI_Translate" FULL_FUNC_NAME << "--"
    "English text from "
    "Java string:\"" << str << "\"")
  if (str == NULL)
  {
    LOGN_ERROR("string returned by GetStringUTFChars is NULL")
    return //NULL; /* OutOfMemoryError already thrown */
      NULL;
  }
//  const char * p_chMainConfigFilePath =
//    (* jni_env).GetStringChars(//jni_env,
//      jstrMainCfgFile,//0
//      & jb
//      );
//  char * ar_chTranslation = //Translate(//p_chMainConfigFilePath
//  const ByteArray byteArray =
  ByteArray byteArray(512);
    TranslateAsXML(
    str, byteArray);
//  LOGN("transl. result:" << ar_chTranslation)
  //jni_env->ReleaseStringChars(jstrMainCfgFile,p_chMainConfigFilePath);
  JNI_ENV_ACCESS(p_jni_env)->ReleaseStringUTFChars(
    JNI_ENV_POINTER(p_jni_env)
    jstrEnglishText, str);

  const unsigned arraySizeWithoutTerminatingNullChar = byteArray.GetSize();
  const unsigned arraySizePlusTerminatingNullChar =
    arraySizeWithoutTerminatingNullChar + 1
//    ModifiedUTF8::byteSizeOfTerminatingNullChar;
    ;
  char * ar_chTranslation = new char[ arraySizePlusTerminatingNullChar
    /** For terminating the string. */ ];
  ::memcpy(ar_chTranslation, byteArray.GetArray(),
    arraySizeWithoutTerminatingNullChar );
  ar_chTranslation[arraySizeWithoutTerminatingNullChar ] = '\0';
//  ModifiedUTF8::AppendNullCharacter(ar_chTranslation +
//    arraySizeWithoutTerminatinfNullChar );

  std::string std_str = format_output_data( (const BYTE *) ar_chTranslation,
    arraySizePlusTerminatingNullChar, 80);
  LOGN_DEBUG( std_str)

  jstring jstr = JNI_ENV_ACCESS(p_jni_env)->NewStringUTF(
    JNI_ENV_POINTER(p_jni_env)
    /** http://stackoverflow.com/questions/12127817/android-ics-4-0-ndk-newstringutf-is-crashing-down-the-app:
    * "Strings that you pass to NewStringUTF() need to be valid Modified UTF-8." */
    /*ar_chTranslation*/ /*(const char *) byteArray.GetArray()*/
    ar_chTranslation);
  ReleaseArrayMemory(/*(void *)*/ ar_chTranslation);
  LOGN("end")
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
  LOGN("begin")
  FreeMemory();
}

/*
 * Class:     vtrans_dynlib_VTransDynLibJNI
 * Method:    Settings
 * Signature: ()V
 */
JNIEXPORT jbyte JNICALL Java_vtrans_dynlib_VTransDynLibJNI_Settings
  (JNIEnv * p_jni_env, //jobject
    jclass jc,
    jstring jstrName,
    jstring jstrValue
    )
{
  LOGN_DEBUG("begin")
  jboolean jb = /*false*/ JNI_FALSE;
  const char * cp_chName, * cp_chValue;
  /** from http://java.sun.com/docs/books/jni/html/objtypes.html: */
  cp_chName = JNI_ENV_ACCESS(p_jni_env)->GetStringUTFChars(
    JNI_ENV_POINTER(p_jni_env)
    jstrName, //NULL
    & jb);
  LOGN(// FULL_FUNC_NAME << "--"
    "name  from Java string:\"" << cp_chName << "\"")
  if (cp_chName == NULL)
  {
    LOGN_ERROR("string returned by GetStringUTFChars is NULL")
    return //NULL; /* OutOfMemoryError already thrown */
      1;
  }
  /** from http://java.sun.com/docs/books/jni/html/objtypes.html: */
  cp_chValue = JNI_ENV_ACCESS(p_jni_env)->GetStringUTFChars(
    JNI_ENV_POINTER(p_jni_env)
    jstrValue, //NULL
    & jb);
  LOGN(// FULL_FUNC_NAME << "--"
    "name  from Java string:\"" << cp_chName << "\"")
  if (cp_chValue == NULL)
  {
    LOGN_ERROR("string returned by GetStringUTFChars is NULL")
    return //NULL; /* OutOfMemoryError already thrown */
      2;
  }
  Settings(cp_chName, cp_chValue);
  JNI_ENV_ACCESS(p_jni_env)->ReleaseStringUTFChars(
    JNI_ENV_POINTER(p_jni_env)
    jstrName, cp_chName);
  JNI_ENV_ACCESS(p_jni_env)->ReleaseStringUTFChars(
    JNI_ENV_POINTER(p_jni_env)
    jstrValue, cp_chValue);
  LOGN_DEBUG("end")
  return 0;
}

/*
 * Class:     vtrans_dynlib_VTransDynLibJNI
 * Method:    Stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_vtrans_dynlib_VTransDynLibJNI_Stop
  (JNIEnv * p_jni_env, //jobject
    jclass jc)
{
  LOGN_DEBUG("begin")
  Stop();
  LOGN_DEBUG("end")
}

inline jmethodID GetAppendMethodID(JNIEnv * p_jni_env,
    jobject jobjItem)
{
  //from http://www.javaworld.com/article/2077554/learn-java/java-tip-54--returning-data-in-reference-arguments-via-jni.html

  // Obtain the Java StringBuffer class handle that corresponds to the
  // Java StringBuffer object handle.
  jclass jclazz = p_jni_env->GetObjectClass (jobjItem);
  LOGN_DEBUG("calling GetMethodID")
  // "Obtain the method ID for the StringBuffer append method which takes
  // a StringBuffer object reference argument and returns a String object
  // reference."
  jmethodID methodID = p_jni_env->GetMethodID (
    jclazz,
    "append",
    "(Ljava/lang/String;)Ljava/lang/StringBuffer;");
  // If this method does not exist then return.
  if (methodID == 0)
  {
    LOGN_ERROR("Java method ID is 0")
    //return -2;
  }
  return methodID;
}

/*
 * Class:     vtrans_dynlib_VTransDynLibJNI
 * Method:    FreeMemory
 * Signature: ()V
 */
JNIEXPORT jbyte JNICALL Java_vtrans_dynlib_VTransDynLibJNI_GetStatus2
  (JNIEnv * p_jni_env, //jobject
    jclass jclazz,
    jstring statusItem,
    jobjectArray jobjArray)
{
  LOGN("begin")
  std::string item;
  struct tm time;
  ByteArray byteArray(256);
  BYTE byStatusCode = GetStatus2(item, time, byteArray);
  const unsigned numBytes = byteArray.GetCapacity();

  //http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/functions.html
  jbyteArray xmlBytes = p_jni_env->NewByteArray(numBytes /** jsize length*/);
  p_jni_env->SetObjectArrayElement(jobjArray,
      0 /*jsize index*/, xmlBytes /*jobject value*/);
  p_jni_env->SetByteArrayRegion(xmlBytes, 0, numBytes, (const jbyte * )
    byteArray.GetArray() );
  jobject jobjItem = p_jni_env->GetObjectArrayElement(/*JNIEnv *env,*/
    jobjArray /*jobjectArray array*/, 1 /*jsize index*/);

  jmethodID stringBufferAppendMethodID = GetAppendMethodID(p_jni_env, jobjItem);
  LOGN_DEBUG("calling NewStringUTF")
  // "Create a new Java String object and populate it with the environment
  // variable value.  Obtain a handle to this object."
  jstring jstrItem = p_jni_env->NewStringUTF( (const char *) item.c_str() );
  LOGN_DEBUG("calling CallObjectMethod")
  /** Call the StringBuffer object's append method passing the handle to
  * the new Java String object. */
  p_jni_env->CallObjectMethod (jobjItem, stringBufferAppendMethodID, jstrItem);
  /**If not called then SIGSEV? if this method is called to often? */
  p_jni_env->DeleteLocalRef(jstrItem);

  std::ostringstream oss;
  oss << time.tm_hour << time.tm_min << time.tm_sec;
  //TODO create java.util.Date object from struct tm rather than using a jstring
  const char * pch = oss.str().c_str();
//  jstrTime = JNI_ENV_ACCESS(p_jni_env)->NewStringUTF(
//        JNI_ENV_POINTER(p_jni_env)
//        pch );
  LOGN("return " << (fastestUnsignedDataType) byStatusCode )
  return byStatusCode;
}

/*
 * Class:     vtrans_dynlib_VTransDynLibJNI
 * Method:    FreeMemory
 * Signature: ()V
 */
JNIEXPORT jbyte JNICALL Java_vtrans_dynlib_VTransDynLibJNI_GetStatus
  (JNIEnv * p_jni_env, //jobject
    jclass jclazz,
    /*jstring*/ jobject jobjItem,
    jstring jstrTime)
{
  LOGN("begin")
  std::string item;
  struct tm time;
  BYTE byStatusCode = GetStatus(item, time);

//  jstrItem = JNI_ENV_ACCESS(p_jni_env)->NewStringUTF(
//	  JNI_ENV_POINTER(p_jni_env)
//	  item.c_str() );

  jmethodID stringBufferAppendMethodID = GetAppendMethodID(p_jni_env, jobjItem);
  LOGN_DEBUG("calling NewStringUTF")
  // "Create a new Java String object and populate it with the environment
  // variable value.  Obtain a handle to this object."
  jstring jstrItem = p_jni_env->NewStringUTF( (const char *) item.c_str() );
  LOGN_DEBUG("calling CallObjectMethod")
  /** Call the StringBuffer object's append method passing the handle to
  * the new Java String object. */
  p_jni_env->CallObjectMethod (jobjItem, stringBufferAppendMethodID, jstrItem);
  /**If not called then SIGSEV? if this method is called to often? */
  p_jni_env->DeleteLocalRef(jstrItem);

  std::ostringstream oss;
  oss << time.tm_hour << time.tm_min << time.tm_sec;
  //TODO create java.util.Date object from struct tm rather than using a jstring
  const char * pch = oss.str().c_str();
//  jstrTime = JNI_ENV_ACCESS(p_jni_env)->NewStringUTF(
//	  JNI_ENV_POINTER(p_jni_env)
//	  pch );
  LOGN("return " << (fastestUnsignedDataType) byStatusCode )
  return byStatusCode;
}
#endif //TEST_MINI_XML

#ifdef __cplusplus
}
#endif
#endif
