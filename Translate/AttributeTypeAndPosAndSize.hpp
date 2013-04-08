/*
 * AttributeTypeAndPosAndSize.hpp
 *
 *  Created on: Jun 7, 2010
 *      Author: Stefan
 */

#ifndef ATTRIBUTETYPEANDPOSANDSIZE_HPP_
#define ATTRIBUTETYPEANDPOSANDSIZE_HPP_

#include <windef.h> //BYTE, WORD

//This class models an easy access to the data in a VocabularyAndTranslation
//object.
//So one can define an attribute name, bit start etc. with an object of this
//class. E.g. define "Eng_singular", index "0", length 1.
//And then when used.
class AttributeTypeAndPosAndSize
{
public:
  enum attribute_type { string, bit } ;
  enum language { German, English, beyondLastElement } ;
  static const char * const s_ar_chLanguage[];
  //attribute is a string or some bits (=whether the index refers to the
  // string or byte array)
  BYTE m_byAttrDataType ;
  BYTE m_byLanguage ;
  BYTE m_byWordClass ; //e.g. ID for word class "noun",
  //String or bit index
  WORD m_wIndex ;
  //bit length of the attribute data.
  WORD m_wLenght ;
  //By defining a default constructor avoid g++ warning "In member function
  // `void TranslateParseByRiseTree::AddTranslationRule(TranslationRule*,
  // const ConditionsAndTranslation&)':
  // c:/mingw/bin/../lib/gcc/mingw32/3.4.5/../../../../include/c++/3.4.5/bits/
  // stl_uninitialized.h:82: warning: '__cur' might be used uninitialized in
  // this function.
  AttributeTypeAndPosAndSize() {}
  AttributeTypeAndPosAndSize(
    BYTE byType,
    WORD wIndex,
    WORD wLenght,
    BYTE byWordClass ,
    BYTE byLanguage // Language that is referred by the other parameters.
    ) ;
  const char * GetLanguageAsString() const { return s_ar_chLanguage[m_byLanguage]; }
//  {
//    m_byLanguage = byLanguage ;
//    m_byWordClass = byWordClass ;
//    m_byAttrDataType = byType ;
//    m_wIndex = wIndex ;
//    m_wLenght = wLenght ;
//  }
} ;

#endif /* ATTRIBUTETYPEANDPOSANDSIZE_HPP_ */
