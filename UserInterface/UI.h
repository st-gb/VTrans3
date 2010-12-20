#if !defined (UI_H_INCLUDED)
	    #define UI_H_INCLUDED

//#ifdef _WINDOWS
#ifdef _MSC_VER //MS Compiler
  //#include "Windows/VTrans.h"
  #include "MFC/VTrans.h"
#else
  #include <iostream>
  namespace UI
  {
    //Do nothing.
    void ShowWordFileError(DWORD dwOffsetOfBeginOfEntry, DWORD dwOffsetOfEndOfEntry) 
      { 
        cout<<"Error in file syntax of the words file between offset" << 
          dwOffsetOfBeginOfEntry << " and " << dwOffsetOfEndOfEntry ; 
      }
  } ;
#endif //#ifdef _WINDOWS
#endif //!defined (UI_H_INCLUDED)