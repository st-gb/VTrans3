#if !defined (IO_H_INCLUDED)
	    #define IO_H_INCLUDED

#include <fstream> //for ofstream
#include "xmlwriter.h"
#include "rest.h" //for class SentenceAndValidityAndProperName
#include "Word.hpp" //for class Word

#define STRING_DELIMITER '9'
#define NUMBER_OF_CHARS_FOR_ENG_NOUN_ATTS 3
#define WORD_TYPE_CHAR_INDEX 0
#define WORD_TYPE_NOUN '1'
#define WORD_TYPE_MAIN_VERB '2' //Vollverb
#define WORD_TYPE_ADJECTIVE '3'
#define WORD_TYPE_ADVERB '4'
#define WORD_TYPE_PREPOSITION '5'
#define WORD_TYPE_PRONOUN '6'
#define WORD_TYPE_AUX_VERB '7'
#define WORD_TYPE_CONJUNCTION '8'
//Used for: IDs for German word types are English word type + NUMBER_OF_WORD_TYPES.
#define NUMBER_OF_WORD_TYPES 10
#define STRING_INDEX_FOR_INIFINITIVE 0

//const char * readInputText(const std::string & strFilePath) ;
Word *    extract(const CString & str,BYTE bEnglishWord,int & ret) ;
BYTE      readInputText(const std::string & strFilePath, std::string & str) ;
LPCSTR                      UTF8toASCII(const char * str);
void                        writeToOutputStream(std::ostream & rofstreamTranslToGerman,
                            //std::vector<Range> & vecRange,std::vector<CStringVector> & vecstrvec
                            std::vector<SentenceAndValidityAndProperName> & vecsentenceandvalidityandpropername,
                            xmlwriter & MyXml
                            );

#endif //!defined (IO_H_INCLUDED)
