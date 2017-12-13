/** GrammarRuleFileReader.hpp
 *  Created on: 22.12.2013
 *      Author: mr.sys  */

#ifndef GRAMMARRULEFILEREADER_HPP_
#define GRAMMARRULEFILEREADER_HPP_

/** Forward declarations: */
//class TranslationControllerBase;
class I_UserInterface;
namespace VTrans3 {
  class BottomUpParser;
}
namespace VTrans3
{
  namespace MiniXML
  {

    class GrammarRuleFileReader
    {
//      static TranslationControllerBase * s_p_translationController;
      static I_UserInterface * s_p_userInterface;
    public:
      GrammarRuleFileReader(//TranslationControllerBase & r_translationController
        I_UserInterface * p_userInterface,
        BottomUpParser &);
      virtual
      ~GrammarRuleFileReader();
      bool ProcessXML(const //std::string & cr_stdstrFilePath
        char * const);
      
      static VTrans3::BottomUpParser * s_p_parseByRise;
    };

  } /* namespace MiniXML */
} /* namespace VTrans3 */

#endif /* GRAMMARRULEFILEREADER_HPP_ */
