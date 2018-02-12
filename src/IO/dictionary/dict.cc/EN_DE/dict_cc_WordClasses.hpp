/** File:   dict_cc_WordClasses.hpp
 * Author: sg
 * Created on 5. Januar 2018, 15:55  */

#ifndef DICT_CC_WORDCLASSES_HPP
#define DICT_CC_WORDCLASSES_HPP

#include <Attributes/EnglishWord.hpp> //enum EnglishWord::English_word_class

namespace dict_cc_WordClasses
{
  enum WordClasses {adjective, adverb,conjunction, noun, past_participle, pronoun, notSet};
  
  class EnglishWordClassFromPOSconverter
  {
  public:
    static std::map<enum WordClasses, enum EnglishWord::English_word_class> s_POS2englishWord;
    /** from http://www.learncpp.com/cpp-tutorial/812-static-member-functions/ */
    // nested class named _init
    class _init 
    {
    public:
        void AddMapping(const WordClasses wordClasses, 
            const EnglishWord::English_word_class english_word_class )
        {
            s_POS2englishWord.insert(std::make_pair(wordClasses, 
                english_word_class));
            
        }
        _init() // the _init constructor will initialize our static variable
        {
            AddMapping(adjective, EnglishWord::adjective);
            AddMapping(adverb, EnglishWord::adverb);
            AddMapping(conjunction, EnglishWord::conjunction);
            AddMapping(noun, EnglishWord::noun);
            AddMapping(past_participle, EnglishWord::mainVerbPastParticiple1Obj);
            AddMapping(pronoun, EnglishWord::pronoun);
        }
    };
    static std::set<enum EnglishWord::English_word_class> GetEnglishWordClass(
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
    static _init s_initializer; // we'll use this static object to ensure the _init constructor is called
  };
  //  EnglishWordClassFromPOSconverter::_init EnglishWordClassFromPOSconverter::s_initializer;
}


#endif /* DICT_CC_WORDCLASSES_HPP */

