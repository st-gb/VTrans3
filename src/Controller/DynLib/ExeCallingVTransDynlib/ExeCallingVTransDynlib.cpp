///from common_sourcecode:
#include <data_structures/ByteArray.hpp>///class ByteArray
#include <FileSystem/GetCurrentWorkingDir.hpp>

//see https://linux.die.net/man/3/dlopen
#include <dlfcn.h> //dlopen(...), 
#include <iostream>

#include <Controller/TranslateControllerBaseReturnCodes.h>
#include <IO/dictionary/OpenDictFileException.hpp>

#pragma message("Link with -ldl")

static void * g_VTransDynLibHandle = NULL;

typedef unsigned char (*VTransDynLibInit_type)(
  const char * p_chMainConfigFilePath, 
  const char * p_chConfigFilesRootPath,
  const char * p_chLogFilePath);
//typedef void (*VTransDynLibTranslate_type)(const char * englishText, char ** );
typedef char * (*VTransDynLibTranslateAsXML_char_array_type)(
  const char * englishText );
typedef void (*VTransDynLibTranslateAsXML_type)(
  const char * englishText,
  ByteArray &);

static const char chDefaultDynLibFilePath [] = "src/VTrans_dynlib";
static const char chDefaultMainCfgFilePath [] = 
  "configuration/VTrans_main_config.xnl";
static const char chDefaultCfgFilesRootPath [] = "configuration";
static const char chDefaultLogFilePath [] = ".";

static const char * dynLibFilePath;
static const char * p_chMainConfigFilePath;
static const char * p_chConfigFilesRootPath;
static const char * p_chLogFilePath;

void handleDefltDynLibFilePath()
{
  std::cout << "using default dynanmic library file path:" << 
    chDefaultMainCfgFilePath << std::endl;
  dynLibFilePath = chDefaultDynLibFilePath;
}

void handleDefltMainCfgFilePath()
{
  std::cout << "using default main default config file path:" << 
    chDefaultMainCfgFilePath << std::endl;
  p_chMainConfigFilePath = chDefaultMainCfgFilePath;
}

void handleDefltCfgFilesRootPath()
{
  std::cout << "using default config files root path:" << 
    chDefaultCfgFilesRootPath << std::endl;
  p_chConfigFilesRootPath = chDefaultCfgFilesRootPath;
}

enum mainFnRetCodes { success, loadDynLibFailed };

/** 
 */
int main(int argc, char** argv)
{
  //TODO add coloured output (for errors) (via Logger class)
  if( argc > 1 )
  {
//    std::cerr << "error: too few arguments. Parameters needed at least: "
//      ">>VTrans dyn lib<< >>root dir<<" << std::endl;
    dynLibFilePath = argv[1];
    if( argc > 2 )
    {
      p_chMainConfigFilePath = argv[2];
      if( argc > 3 )
      {
        p_chConfigFilesRootPath = argv[3];
      }
      else
        handleDefltCfgFilesRootPath();
    }
    else
      handleDefltMainCfgFilePath();
//    return 1;
  }
  else
    handleDefltDynLibFilePath();

  std::string std_strCurrentWorkingDir;
  OperatingSystem::GetCurrentWorkingDirA_inl(std_strCurrentWorkingDir);
  std::cout << "cwd:" << std_strCurrentWorkingDir << std::endl;
  g_VTransDynLibHandle = dlopen(dynLibFilePath, RTLD_LAZY);
  if( g_VTransDynLibHandle == NULL)
  {
    std::cerr << "could not load VTrans dynamic library for path" << 
      dynLibFilePath << std::endl;
    return loadDynLibFailed;
  }
  else
    std::cout << "successfully loaded VTrans dynamic library for path" <<
      dynLibFilePath << std::endl;
  {
    p_chLogFilePath = chDefaultLogFilePath;
    VTransDynLibInit_type pfnVTransDynLibInit = //*(void **) (&VTransDynLibInit)
      (VTransDynLibInit_type) dlsym(g_VTransDynLibHandle, "Init");
    std::cout << "calling VTrans dyn lib's \"Init\" function with params " << 
      p_chMainConfigFilePath << " , " << p_chConfigFilesRootPath << std::endl;
    std::cout << "initializing translation system (incl. load dict)" <<
      std::endl;
    TranslationControllerBaseClass::InitFunction::Init_return_codes InitRetCode
      = (TranslationControllerBaseClass::InitFunction::Init_return_codes) 
      (*pfnVTransDynLibInit)(
        p_chMainConfigFilePath,
        p_chConfigFilesRootPath,
        p_chLogFilePath);
    std::cout << "End initializing translation system (incl. load dict)" <<
      std::endl;
    
    if( InitRetCode != TranslationControllerBaseClass::InitFunction::success)
      //;//LOGN_ERROR("error calling DynLib's \"Init\" fn)
      return InitRetCode;
    std::cout << "InitRetCode:" << InitRetCode //<< " meaning:" 
      << std::endl;
    
    /** Must be allocated on the heap so that the internal buffer is not 
     *   released. */
    ByteArray * p_byteArray = new ByteArray();
    try
    {
      VTransDynLibTranslateAsXML_type pfnVTransDynLibTranslateAsXML = //*(void **) (&VTransDynLibInit)
        (VTransDynLibTranslateAsXML_type) dlsym(g_VTransDynLibHandle,
        "TranslateAsXML");
      (*pfnVTransDynLibTranslateAsXML)("the man", * p_byteArray);

      VTransDynLibTranslateAsXML_char_array_type pfnVTransDynLibTranslateAsXML_char_array = 
        (VTransDynLibTranslateAsXML_char_array_type) dlsym(g_VTransDynLibHandle,
        "TranslateAsXML_char_array");
//      char * germanTranslation = (*pfnVTransDynLibTranslate)("the man");
      char * germanTranslation;
      germanTranslation = (*pfnVTransDynLibTranslateAsXML_char_array)("the man");//, & germanTranslation);
      std::cout << "germanTranslation:" << germanTranslation << std::endl;
    }
    catch(VTrans3::OpenDictFileException)
    {
//      std::string absoluteDictFilePath = FileSystem::GetAbsolutePathA(
//        std_strDictFilePath.c_str());
    }
    ((void (*)() ) dlsym(g_VTransDynLibHandle, "FreeMemory") )();
    std::cout << "after calling FreeMemory" << std::endl;
    
    std::cout << p_byteArray->GetArray() << std::endl;

    int ret = dlclose(g_VTransDynLibHandle);  
    std::cout << "after unloading dyn lib" << std::endl;
  }
  return g_VTransDynLibHandle != NULL;
}
