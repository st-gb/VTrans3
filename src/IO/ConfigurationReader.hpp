/** ConfigurationReader.hpp
 *  Created on: 09.12.2013
 *      Author: mr.sys  */

#ifndef CONFIGURATIONREADER_HPP_
#define CONFIGURATIONREADER_HPP_

#include <string> //class std::string
#include <sstream> //std::istringstream
#include <map> //class std::map
#include <Attributes/GUIattributes.hpp> //class GUIattributes
#include <data_structures/Trie/NodeTrie/NodeTrie.hpp> //class NodeTrie
#include <stdint.h> //for uint32_t

/** Forward declarations: */
//class TranslationControllerBase;
class I_UserInterface;
namespace VTrans3 {
  class BottomUpParser;
}
class TranslationRule;

namespace VTrans3
{
  /** Base class for reading (translation rule, ...) configuration files.
   *  May be derived e.g. by SAX2 reader (Apache Xerces or MiniXML) or by a
   *  reader for another file format or by completely another implementation.*/
  template <typename attributeType>
  class ConfigurationReader
  {
  protected:
  public:
//    TranslationControllerBase & m_translationController;
    static I_UserInterface * m_p_UserInterface;
    GUIattributes m_GUIattributes;
    typedef NodeTrie<uint32_t> grammarPartName2ColourContainer_type;
//    NodeTrie<uint32_t> m_nodetrie_ui32GrammarPartName2colour;
    std::map<std::string, uint32_t> m_ui32GrammarPartName2colourContainer;    
    std::string m_stdstrVocabularyFilePath;
    /** The current configuration file (grammer rule, translation rule, ...) 
      * currently being processed. */
    static std::string m_std_strCurrentConfigfile;
    BottomUpParser & m_parseByRise;
  #ifndef TEST_MINI_XML
    static std::map<TranslationRule *, std::string>
      m_std_map_p_translationrule2filepath;
    static std::map<std::string, std::string> m_std_map_grammarRuleName2filepath;
  #endif //#ifndef TEST_MINI_XML

    ConfigurationReader(/* TranslationControllerBase * */ I_UserInterface *, 
      BottomUpParser & parseByRise );
    virtual
    ~ConfigurationReader();

    virtual void ReadGrammarRuleFile(const std::string & cr_stdstrFilePath) = 0;
    virtual bool ReadMainConfigFile(const std::string & cr_stdstrFilePath) = 0;
#if USE_TRANSLATION_RULES
    virtual void ReadVocAttributeDefinitionFile(
      const std::string & cr_stdstrFilePath) = 0;
    virtual void ReadTranslationRuleFile(const std::string & cr_stdstrFilePath) = 0;
#endif

    /** virtual template methods are impossible */
    /*template <typename XMLelementType>*/ virtual bool GetAttributeValue(
      /*XMLelementType & xmlElement,*/
        attributeType & attribute,
        std::string &, const char * const) = 0;
//    template <//typename XMLelementType,
//      typename T> virtual
//      bool ConvertAttributeValue(
//        /*XMLelementType & XMLelement ,*/
//        T & r_templateType,
//        const char * const cpc_chAttributeName//,
//        ) = 0;

    /** template idea for converting string to other data type
    * from http://www.codeguru.com/forum/showthread.php?t=231056:
    * _Must_ be _defined_ here because of the template type: the templates type gets
    *  replaced by the actual data type. When the declaration is separated from
    *  the definition (what it usual) the template type is replaced by the actual
    * parameter, but the definition's template type is _not_ being replaced.
    * So it gets an error message like
    * "undefined reference to `bool XercesAttributesHelper::getValue<std::string>
    * (std::string&, char*) const'"
    * Also see here: http://www.codeproject.com/KB/cpp/templatesourceorg.aspx
    *
    * @return true: success */
    template <//typename attributeType,
      typename T>
     static bool ConvertAttributeValue(
       attributeType & attribute ,
  //     mxml_node_s * & XMLelement ,
       ConfigurationReader & configReader,
       T & r_templateType,
       const char * const cpc_chAttributeName//,
    //   //e.g. "std::boolalpha"
    //   std::ios_base * (*f)(std::ios_base&) = NULL
      )
    {
      std::string std_strAttributeValue;
      bool success = configReader.GetAttributeValue(
        attribute,
        std_strAttributeValue,
        // const XMLCh *const qName
        cpc_chAttributeName
        //"number"
        ) ;
      //If the attribute exists.
      if(success)
      {
          std::istringstream iss(std_strAttributeValue);

          //atof returned 1 for "1.2" on Linux on German because the decimal
          //point is a "," in German.
          //http://stackoverflow.com/questions/1333451/c-locale-independent-atof:
          //"localeconv (in <locale.h>) returns a pointer to struct whose
          //decimal_point member contains that value. Note that the pointer
          //is valid until the next localeconv() or setlocale() – "
          //fAtofResult = (float) atof(pchAttributeValue ) ;
          /**Ensure a "." is interpreted as decimal point no matter what the
          * current locale is. */
          iss.imbue( std::locale("C") );

  //        return ! ( iss //>> f
  //          >> r_templateType
  //          ).fail();
  //        if( f)
  //          iss >> f;
  //        std::boolalpha
  #ifdef _DEBUG
          iss >> r_templateType;
          bool fail = iss.fail();
          bool bGood = /*iss.good()*/ ! fail;
  #else
          bool bGood = ! ( iss //>> f
              >> r_templateType
              ).fail();
  #endif
          return bGood;
        //      else
        //        byReturn = XERCES_ATTRIBUTE_VALUE_DOES_NOT_EXIST ;
    //        else
    //          byReturn = XERCES_ERROR_CONVERTING_ATTRIBUTE_VALUE_TO_C_STRING ;
      }
      return false ;
    }
  };

  template <typename attributeType>
    ConfigurationReader<attributeType>::ConfigurationReader(
      /*TranslationControllerBase * p_translationController*/
      I_UserInterface * p_userInterface, BottomUpParser & parseByRise )
    : /*m_translationController( * p_translationController )*/
      //m_p_UserInterface(p_userInterface)
    m_parseByRise(parseByRise)
//  , m_ui32GrammarPartName2colourContainer(256, -1)
  {
//    s_p_translationController = & r_translationController;
    m_p_UserInterface = p_userInterface;
  }

  template <typename attributeType> ConfigurationReader<attributeType>::
    ~ConfigurationReader<attributeType>()
  {
    // TODO Auto-generated destructor stub
  }
} /* namespace VTrans3 */

/** https://stackoverflow.com/questions/7108914/what-should-happen-to-template-class-static-member-variables-with-definition-in */
template<typename attributeType> std::map<std::string, std::string> 
  VTrans3::ConfigurationReader<attributeType>::m_std_map_grammarRuleName2filepath;

#endif /* CONFIGURATIONREADER_HPP_ */
