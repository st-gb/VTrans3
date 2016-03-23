/* 
 * File:   Condition.hpp
 * Author: Stefan
 *
 * Created on 5. April 2010, 23:15
 */

#ifndef _CONDITION_HPP
#define	_CONDITION_HPP

#include <windef.h> //for BYTE
#include <string> //std::string
//#include <preprocessor_macros/logging_preprocessor_macros.h> //DEBUG_COUT(...)
#include <Translate/SyntaxTreePath.hpp>//class SyntaxTreePath

/** Condition for translation. E.g. that could mean:
* current grammar part: English definite article.
* condition: "If token from noun grammar part equals the plural of this
*  vocabulary translate the article with "die" */
class Condition
{
public:
  enum compare_operator { equals } ;
  BYTE m_byCompareType ;  //e.g. "equals" etc.
  /** Whether the condition refers to English or German vocabulary. */
  BYTE byLanguage ;
  /** The attribute name that should be used as a key to an
  * AttributeTypeAndPosAndSize object stored somewhere. */
  std::string m_stdstrAttributeName ;
  BYTE m_byAttributeValue ;
  SyntaxTreePath m_syntaxtreepath ;

  //void SetSyntaxTreePath(std::string & r_stdstrSyntaxTreePath)
  BYTE SetSyntaxTreePath(const char * p_ch_SyntaxTreePath
     , ParseByRise * p_parsebyrise
     , std::string & r_std_strUnknownGrammarPartID
     )
  {
    std::string stdstrSyntaxTreePath (p_ch_SyntaxTreePath) ;
    std::string std_strUnknownGrammarPartID;
    return m_syntaxtreepath.CreateGrammarPartIDArray( //r_stdstrSyntaxTreePath
        stdstrSyntaxTreePath , p_parsebyrise , r_std_strUnknownGrammarPartID ) ;
  }
  //By defining a default constructor avoid g++ warning "In member function
  // `void TranslateParseByRiseTree::AddTranslationRule(TranslationRule*,
  // const ConditionsAndTranslation&)':
  // c:/mingw/bin/../lib/gcc/mingw32/3.4.5/../../../../include/c++/3.4.5/bits/
  // stl_uninitialized.h:82: warning: '__cur' might be used uninitialized in
  // this function.
  Condition() {}
  #ifdef _DEBUG
  ~Condition()
  {
//    int i = 0 ;
//    DEBUG_COUT("~Condition\n")
  }
  #endif //#ifdef _DEBUG
};

#endif	/* _CONDITION_HPP */

