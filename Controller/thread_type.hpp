/*
 * thread_type.hpp
 *
 *  Created on: 21.01.2013
 *      Author: Stefan
 */

#ifndef THREAD_TYPE_HPP_
#define THREAD_TYPE_HPP_

namespace VTrans {

#ifdef _WIN32
  #include <Windows/multithread/Thread.hpp>
  typedef Windows_API::Thread thread_type;
#endif

}  // namespace name

#endif /* THREAD_TYPE_HPP_ */
