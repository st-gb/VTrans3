/*
 * DictionaryFileAccessException.hpp
 *
 *  Created on: 25.03.2014
 *      Author: mr.sys
 */

#ifndef DICTIONARYFILEACCESSEXCEPTION_HPP_
#define DICTIONARYFILEACCESSEXCEPTION_HPP_

//::GetErrorMessageFromLastErrorCode()
#include <Controller/GetErrorMessageFromLastErrorCode.hpp>
#include <Controller/character_string/convertFromAndToStdString.hpp>
//#include <IO/FileAccessException.hpp>
#include <FileSystem/File/FileException.hpp> //class FileException

class DictionaryFileAccessException
  : public //FileAccessException
  FileException
{
public:
  enum action { seek, read, getFileSize, getCurrentFilePointerPosition, extractWordType};
  /** The action that causeed the error. */
  enum action m_action;
  uint64_t m_seekOffset;

  DictionaryFileAccessException() { }
  
  DictionaryFileAccessException(const FileException & fe, const enum action action ) 
    : FileException(fe)
    , m_action(action)
   { }

  DictionaryFileAccessException(
    const enum action action,
    const DWORD operatingSystemErrorCode,
    const char * const filePath,
    uint64_t seekOffset = 0)
    : FileException(filePath)
  {
    m_action = action;
    m_operatingSystemErrorCode = operatingSystemErrorCode;
//    m_filePath = GetStdWstring(filePath);
    m_seekOffset = seekOffset;
  }

  virtual
  ~DictionaryFileAccessException() { };
  
  inline const std::string GetErrorMessageA() const
  {
    std::string std_strErrorMessage;
    switch(m_action)
    {
    case seek:
      std_strErrorMessage = "seeking to offset " + convertToStdString(m_seekOffset) + " in";
      break;
    case getFileSize:
      std_strErrorMessage = "getting file size for";
      break;
    case getCurrentFilePointerPosition:
      std_strErrorMessage = "getting current file pointer position for";
      break;
    case read:
      std_strErrorMessage = "reading";
      break;
      //array to small to store chars between '{' and '}' (word type)
    case extractWordType:
      std_strErrorMessage = "extracting word type at byte offset " + convertToStdString(m_seekOffset);
      break;
    default:
      std_strErrorMessage = "other action regarding";
      break;
   }
    const std::string std_strAbsoluteLogFilePath = /*GetAbsoluteFilePathA();*/

    std_strErrorMessage += " dictionary file \"" + std_strAbsoluteLogFilePath + "\" failed:";
    const std::string std_strErrorMessageFromErrorCode =
      ::GetErrorMessageFromErrorCodeA(m_operatingSystemErrorCode);
    std_strErrorMessage += std_strErrorMessageFromErrorCode;
    return std_strErrorMessage;
  }
};

#endif /* DICTIONARYFILEACCESSEXCEPTION_HPP_ */
