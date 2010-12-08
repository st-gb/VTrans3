/*
 * TranslationControllerBase.hpp
 *
 *  Created on: Dec 6, 2010
 *      Author: Stefan
 */

#ifndef TRANSLATIONCONTROLLERBASE_HPP_
#define TRANSLATIONCONTROLLERBASE_HPP_

#include <Parse/ParseByRise.hpp> //class ParseByRise
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
#include <string> //class std::string

//Forward declarations:
class SAX2TranslationRuleHandler ;
//class std::string ;

//Base class for e.g. wxApp and CWinApp-derived classes.
class TranslationControllerBase
  : public I_UserInterface
{
public:
  ParseByRise m_parsebyrise ;
  TranslateParseByRiseTree m_translateparsebyrisetree ;
  void ReadTranslationRuleFile(
    SAX2TranslationRuleHandler & r_sax2translationrulehandler ,
    const std::string & cr_stdstrFilePath
    ) ;
  TranslationControllerBase();
  virtual
  ~TranslationControllerBase();
};

#endif /* TRANSLATIONCONTROLLERBASE_HPP_ */
