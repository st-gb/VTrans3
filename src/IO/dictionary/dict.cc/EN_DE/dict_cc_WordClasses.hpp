/** File:   dict_cc_WordClasses.hpp
 * Author: sg
 * Created on 5. Januar 2018, 15:55  */

#ifndef DICT_CC_WORDCLASSES_HPP
#define DICT_CC_WORDCLASSES_HPP

#include <Attributes/EnglishWord.hpp> //enum EnglishWord::English_word_class

namespace dict_cc_WordClasses
{
  enum WordClasses {adjective, adverb,conjunction, noun, past_participle, 
    preposition, pronoun, notSet};
  
class EnglishWordClassFromPOSconverter
{
 public:
  static std::map<enum WordClasses, enum EnglishWord::English_word_class>
    s_POS2englishWord;
  /**from http://www.learncpp.com/cpp-tutorial/812-static-member-functions/ 
     section "C++ does not support static constructors" */
  ///Nested class named _init
  class _init//TODO rename class because the name is identical to a dynlib
    //function name
  {
    public:
    void AddMapping(const WordClasses wordClasses, 
      const EnglishWord::English_word_class english_word_class )
    {
      s_POS2englishWord.insert(std::make_pair(wordClasses, english_word_class));
    }
    _init() // the _init constructor will initialize our static variable
    {
      AddMapping(adjective, EnglishWord::adjective);
      AddMapping(adverb, EnglishWord::adverb);
      AddMapping(conjunction, EnglishWord::conjunction);
      AddMapping(noun, EnglishWord::noun);
      //TODO why map to 1 object _main_ verb?
      AddMapping(past_participle, EnglishWord::mainVerbPastParticiple1Obj);
      AddMapping(preposition, EnglishWord::preposition);
      AddMapping(pronoun, EnglishWord::pronoun);
    }
  };///class
  static std::set<enum EnglishWord::English_word_class> GetEngPOSes(
    const std::set<enum dict_cc_WordClasses::WordClasses> & wordClassContainer)
  {
      std::set<enum EnglishWord::English_word_class> english_word_classes;
      std::set<enum dict_cc_WordClasses::WordClasses>::const_iterator iter =
        wordClassContainer.begin();
      for( ; iter != wordClassContainer.end() ; iter++)
      {
        english_word_classes.insert(s_POS2englishWord.find(*iter)->second);
      }
      return english_word_classes;
  }
  static enum EnglishWord::English_word_class GetEngPOS(
    const enum dict_cc_WordClasses::WordClasses wordClass) {
    return s_POS2englishWord.find(wordClass)->second;
  }
  /**from http://www.learncpp.com/cpp-tutorial/812-static-member-functions/*/
  ///This static object is used to ensure the _init constructor is called.
  static _init s_initializer;
};///class
  //  EnglishWordClassFromPOSconverter::_init EnglishWordClassFromPOSconverter::s_initializer;
}///namespace

#endif /* DICT_CC_WORDCLASSES_HPP */
