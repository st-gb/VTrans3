/*
 * FileAccessException.hpp
 *
 *  Created on: 25.03.2014
 *      Author: mr.sys
 */

#ifndef FILEACCESSEXCEPTION_HPP_
#define FILEACCESSEXCEPTION_HPP_

#include <FileSystem/IsRelativePath.hpp> //bool FileSystem::IsRelativePath(...)
#include <FileSystem/GetCurrentWorkingDir.hpp> //GetCurrentWorkingDir(...)

class FileAccessException
{
protected:
  std::string m_filePath;
  DWORD m_operatingSystemErrorCode;
public:
  FileAccessException();
  virtual
  ~FileAccessException();

  const std::string GetAbsoluteFilePathA() const
  {
    std::string std_strAbsoluteLogFilePath;
    if( FileSystem::IsRelativePathA(m_filePath.c_str() ) )
    {
      std::string std_strCurrentDirectory;
//      OperatingSystem::GetCurrentWorkingDirA_inl(std_strCurrentDirectory);
#ifdef _WIN32
      Windows::GetCurrentWorkingDirA_inl(std_strCurrentDirectory);
#else
      /*OperatingSystem*/Linux::GetCurrentWorkingDirA_inl(std_strCurrentDirectory);
#endif
      std_strAbsoluteLogFilePath += std_strCurrentDirectory += m_filePath;
      return std_strAbsoluteLogFilePath;
    }
    return m_filePath;
  }

  virtual const std::string GetErrorMessageA() const = 0;
};

#endif /* FILEACCESSEXCEPTION_HPP_ */
