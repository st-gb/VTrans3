/*
 * UnknownGrammarPartNameException.hpp
 *
 *  Created on: 08.12.2012
 *      Author: Stefan
 */

#ifndef UNKNOWNGRAMMARPARTNAMEEXCEPTION_HPP_
#define UNKNOWNGRAMMARPARTNAMEEXCEPTION_HPP_

#include <string> //class std::string

namespace VTrans
{
  class UnknownGrammarPartNameException
  {
  private:
    const std::string m_std_strGrammarPartPath;
  public:
    UnknownGrammarPartNameException(const std::string & c_r_std_strGrammarPartPath);
    virtual
    ~UnknownGrammarPartNameException();
    const std::string & GetGrammarPartPath() const
      { return m_std_strGrammarPartPath; }
    const std::string GetErrorMessage() const {
      const std::string errorMessage =
       "unknown grammar part name/ path \"" + m_std_strGrammarPartPath + "\"";
      return errorMessage;
    }
  };

} /* namespace VTrans */
#endif /* UNKNOWNGRAMMARPARTNAMEEXCEPTION_HPP_ */
