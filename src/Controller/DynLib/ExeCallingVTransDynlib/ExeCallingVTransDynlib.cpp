#include <data_structures/ByteArray.hpp>
//see https://linux.die.net/man/3/dlopen
#include <dlfcn.h> //dlopen(...), 
#include <iostream>
#include <IO/dictionary/OpenDictFileException.hpp>

#pragma message("Link with -ldl")

static void * g_VTransDynLibHandle = NULL;

typedef unsigned char (*VTransDynLibInit_type)(const char * p_chMainConfigFilePath, 
  const char * p_chConfigFilesRootPath);
typedef unsigned char * (*VTransDynLibTranslate_type)(const char * englishText);
typedef void (*VTransDynLibTranslateAsXML_type)(const char * englishText, ByteArray &);

/** 
 */
int main(int argc, char** argv)
{
  if( argc < 4 )
  {
    std::cerr << "error: too few arguments. Usage: VTrans dyn lib" << std::endl;
    return 1;
  }
  g_VTransDynLibHandle = dlopen(argv[1], RTLD_LAZY);
  if( g_VTransDynLibHandle == NULL)
  {
    std::cerr << "could not load VTrans dynamic library for path" << argv[1] << std::endl;
    return 1;
  }
  {
    char * p_chMainConfigFilePath = argv[2];
    char * p_chConfigFilesRootPath = argv[3];
    VTransDynLibInit_type pfnVTransDynLibInit = //*(void **) (&VTransDynLibInit)
      (VTransDynLibInit_type) dlsym(g_VTransDynLibHandle, "Init");
    
    unsigned InitRetCode = (*pfnVTransDynLibInit)(p_chMainConfigFilePath, 
      p_chConfigFilesRootPath);
    std::cout << "InitRetCode:" << InitRetCode << std::endl;
    
    /** Must be allocated on the heap so that the internal buffer is not 
     *   released. */
    ByteArray * p_byteArray = new ByteArray();
    try
    {            
      VTransDynLibTranslateAsXML_type pfnVTransDynLibTranslateAsXML = //*(void **) (&VTransDynLibInit)
        (VTransDynLibTranslateAsXML_type) dlsym(g_VTransDynLibHandle, "TranslateAsXML");
      (*pfnVTransDynLibTranslateAsXML)("the man", * p_byteArray);
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
