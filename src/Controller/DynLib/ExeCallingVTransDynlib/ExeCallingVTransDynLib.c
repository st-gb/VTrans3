/** File:   ExeCallingVTransDynLib.c
 * Author: sg
 * Created on 10. September 2017, 21:29  */

#include <stdio.h>
#include <stdlib.h>
#include <data_structures/ByteArray.hpp>
//see https://linux.die.net/man/3/dlopen
#include <dlfcn.h> //dlopen(...), 

#pragma message("Link with -ldl")

static void * g_VTransDynLibHandle = NULL;

typedef unsigned char (*VTransDynLibInit_type)(const char * p_chMainConfigFilePath, 
        const char * p_chConfigFilesRootPath);

/** 
 */
int main(int argc, char** argv)
{
  g_VTransDynLibHandle = dlopen("VTrans", RTLD_LAZY);
  if( g_VTransDynLibHandle != NULL )
  {
    VTransDynLibInit_type * pfnVTransDynLibInit = //*(void **) (&VTransDynLibInit)
      (VTransDynLibInit_type) dlsym(g_VTransDynLibHandle, "Init");
    pfnVTransDynLibInit(p_chMainConfigFilePath, p_chConfigFilesRootPath);
    
    /** Must be allocated on the heap so that the internal buffer is not 
     *   released. */
    ByteArray * p_byteArray = new ByteArray();
      
    int ret = dlclose(g_VTransDynLibHandle);      
  }
  return g_VTransDynLibHandle != NULL;
}
