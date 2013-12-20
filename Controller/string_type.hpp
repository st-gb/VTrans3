/* 
 * File:   string_type.hpp
 * Author: Stefan
 *
 * Created on 20. Januar 2010, 02:09
 */

#ifndef _STRING_TYPE_HPP
#define	_STRING_TYPE_HPP

//stackoverflow.com/questions/18844626/type-stdstring-could-not-be-resolved
// "include <cstring> too"
//#if !defined(__linux__) && !defined(_WIN32)
//	#include <cstring>
//#endif
#include <string> //class std::string

namespace VTrans
{
  //typedef std::wstring string_type ;
  typedef std::string string_type ;
};

#endif	/* _STRING_TYPE_HPP */

