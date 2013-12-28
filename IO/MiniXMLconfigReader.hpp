/*
 * MiniXMLconfigReader.hpp
 *
 *  Created on: 08.12.2013
 *      Author: mr.sys
 */

#ifndef MINIXMLCONFIGREADER_HPP_
#define MINIXMLCONFIGREADER_HPP_

#include <string> //class std::string
#include <IO/ConfigurationReader.hpp> //base class VTrans3::ConfigurationReader
#include <mxml.h> //typedef mxml_sax_cb_t
#include <sstream> //std::istringstream

class TranslationControllerBase;

namespace VTrans3
{
namespace MiniXML
{

  /** Class for reading configuration files via the "MiniXML" library/ framework.
   * (see http://www.msweet.org/documentation/project3/Mini-XML.pdf)
   *  MiniXML is easier to compile than Apache Xerces. So it ought to be used
   *  for Android NDK. */
  class MiniXMLconfigReader
    : public VTrans3::ConfigurationReader<mxml_node_s *>
  {
//    TranslationControllerBase & m_translationController;
  public:
    /** For accessing the TranslationControllerBase object easily from
     * global functions / MiniXML callback functions. */
    static TranslationControllerBase * s_p_translationController;
    MiniXMLconfigReader(TranslationControllerBase & r_translationController);
    virtual
    ~MiniXMLconfigReader();

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
    template <//typename XMLelementType,
      typename T>
     bool ConvertAttributeValue(
//       XMLelementType & XMLelement ,
       mxml_node_s * & XMLelement ,
       T & r_templateType,
       const char * const cpc_chAttributeName//,
    //   //e.g. "std::boolalpha"
    //   std::ios_base * (*f)(std::ios_base&) = NULL
      )
    {
      std::string std_strAttributeValue;
      bool success = GetAttributeValue(
        XMLelement,
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
          bool bGood = iss.good();
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

//    template <typename XMLelementType> /*virtual*/ bool GetAttributeValue(
//      XMLelementType & xmlElement, std::string &, const char * const) /*= 0*/;
    bool GetAttributeValue(
      mxml_node_s * & xmlElement, std::string &, const char * const) /*= 0*/;
    /** Provides showing an error message in case of an error for loading an
     * arbitrary XML file via MiniXML. */
    bool ReadFile(const char * const cr_stdstrFilePath, mxml_sax_cb_t sax);
    void ReadGrammarRuleFile(const std::string & cr_stdstrFilePath );
    bool ReadMainConfigFile(const std::string & cr_stdstrFilePath );
    void ReadVocAttributeDefinitionFile(
      const std::string & cr_stdstrFilePath);
    void ReadTranslationRuleFile(const std::string & cr_stdstrFilePath) ;
  };
} /* namespace MiniXML */
}

#endif /* MINIXMLCONFIGREADER_HPP_ */
