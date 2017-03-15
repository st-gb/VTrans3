/*
 * thread_type.hpp
 *
 *  Created on: 21.01.2013
 *      Author: Stefan
 */

#ifndef THREAD_TYPE_HPP_
#define THREAD_TYPE_HPP_

#ifdef _WIN32
  #include <Windows/multithread/Thread.hpp>
  typedef Windows_API::Thread thread_type;
#elif defined(__linux__)
  #include <OperatingSystem/POSIX/multithread/pthreadBasedI_Thread.hpp>
//  using namespace Linux;
//  typedef pthreadBasedI_Thread thread_type;
#else
  #include <wxWidgets/multithread/wxThreadBasedI_Thread.hpp>
  typedef wxThreadBasedI_Thread thread_type;
#endif

namespace VTrans {

#ifdef _WIN32
  typedef Windows_API::Thread thread_type;
#elif defined(__linux__)
//  using namespace POSIX;
  typedef POSIX::pthreadBasedI_Thread thread_type;
#else
  typedef wxThreadBasedI_Thread thread_type;
#endif
}  // namespace name

#endif /* THREAD_TYPE_HPP_ */
