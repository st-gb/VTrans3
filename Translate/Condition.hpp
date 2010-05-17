/* 
 * File:   Condition.hpp
 * Author: Stefan
 *
 * Created on 5. April 2010, 23:15
 */

#ifndef _CONDITION_HPP
#define	_CONDITION_HPP

//Condition for translation. E.g. that could mean:
//current grammar part: English definite article.
//condition: "If token from noun grammar part equals the plural of this
//  vocabulary translate the article with "die"
class Condition
{
public:
  enum compare_operator { equals } ;
  BYTE m_byCompareType ;  //e.g. "equals" etc.
  //Whether the condition refers to English or German vocabulary.
  BYTE byLanguage ;
  //The attribute name that should be used as a key to an
  //AttributeTypeAndPosAndSize object stored somewhere.
  std::string m_stdstrAttributeName ;
  BYTE m_byAttributeValue ;
  SyntaxTreePath m_syntaxtreepath ;

  //void SetSyntaxTreePath(std::string & r_stdstrSyntaxTreePath)
  void SetSyntaxTreePath(const char * p_ch_SyntaxTreePath
     , ParseByRise * p_parsebyrise )
  {
    std::string stdstrSyntaxTreePath (p_ch_SyntaxTreePath) ;
    m_syntaxtreepath.CreateGrammarPartIDArray( //r_stdstrSyntaxTreePath
        stdstrSyntaxTreePath , p_parsebyrise  ) ;
  }
  #ifdef _DEBUG
  ~Condition()
  {
    int i = 0 ;
    DEBUG_COUT("~Condition\n")
  }
  #endif //#ifdef _DEBUG
};

#endif	/* _CONDITION_HPP */

