/*
 * GrammarRuleFileReader.hpp
 *
 *  Created on: 22.12.2013
 *      Author: mr.sys
 */

#ifndef GRAMMARRULEFILEREADER_HPP_
#define GRAMMARRULEFILEREADER_HPP_

class TranslationControllerBase;

namespace VTrans3
{
  namespace MiniXML
  {

    class GrammarRuleFileReader
    {
    public:
      static TranslationControllerBase * s_p_translationController;
      GrammarRuleFileReader(TranslationControllerBase & r_translationController);
      virtual
      ~GrammarRuleFileReader();
      bool ProcessXML(const //std::string & cr_stdstrFilePath
        char * const);
    };

  } /* namespace MiniXML */
} /* namespace VTrans3 */

#endif /* GRAMMARRULEFILEREADER_HPP_ */
