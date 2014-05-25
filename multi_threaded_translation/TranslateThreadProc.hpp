/* 
 * File:   TranslateThreadProc.hpp
 * Author: mr.sys
 *
 * Created on 13. Mai 2014, 20:25
 */

#ifndef TRANSLATETHREADPROC_HPP
#define	TRANSLATETHREADPROC_HPP

#include <Controller/multithread/thread_function_calling_convention.h>
typedef unsigned long DWORD ;

namespace VTrans
{
  THREAD_FUNCTION_CALLING_CONVENTION DWORD TranslateThreadProc(void * p_vThreadParams);
}

#endif	/* TRANSLATETHREADPROC_HPP */

