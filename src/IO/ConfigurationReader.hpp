/*
 * ConfigurationReader.hpp
 *
 *  Created on: 09.12.2013
 *      Author: mr.sys
 */

#ifndef CONFIGURATIONREADER_HPP_
#define CONFIGURATIONREADER_HPP_

#include <string> //class std::string
#include <sstream> //std::istringstream

class TranslationControllerBase;

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
    TranslationControllerBase & m_translationController;
    std::string m_stdstrVocabularyFilePath ;
    ConfigurationReader(TranslationControllerBase *);
    virtual
    ~ConfigurationReader();

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
    ConfigurationReader<attributeType>::ConfigurationReader(TranslationControllerBase * p)
    : m_translationController (* p)
  {
//    s_p_translationController = & r_translationController;
  }

  template <typename attributeType> ConfigurationReader<attributeType>::
    ~ConfigurationReader<attributeType>()
  {
    // TODO Auto-generated destructor stub
  }
} /* namespace VTrans3 */

#endif /* CONFIGURATIONREADER_HPP_ */
