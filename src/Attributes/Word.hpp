#if !defined (WORD_H_INCLUDED)
	    #define WORD_H_INCLUDED

#include <Controller/string_type.hpp> /** for typedef VTrans::string_type */
#include <tchar.h> //for TCHAR

//#ifdef _WINDOWS
#ifdef _MSC_VER //MS Compiler (internal MSVC preprocessor macro)
	//#include <atlstr.h>//This include is needed for "CString" when compiling as DLL.
#else //#ifdef _MSC_VER
	//#include <typedefs.h>
	//#include <StdString.h>
  //#include <MFC_Compatibility/CString.hpp>
  //using namespace MFC_Compatibility ; //for class CString in MFC_Compatibility
#endif

//#include <windef.h> //BYTE
#include <Windows.h>//includes windef.h;for BYTE

//#include "VocabularyInMainMem/LetterTree/VocabularyAndTranslation.hpp" //for BYTE

//extern VocNode * m_first;
//Makro definieren mit Wert, der der ASCII-Code f�r "1" ist (damit man die 
//W�rterbuch-Datei mit einem normalen Editor bearbeiten kann)
#define ASCII_CODE_FOR_DIGIT_1 49
#define ASCII_CODE_FOR_DIGIT_0 48
#define EAV_ARRAY_INDEX_FOR_INFINITIVE 0
#define EAV_ARRAY_INDEX_FOR_PAST_PARTICIPLE 13

class Word
{
public:
  //Define destructor to prevent g++ warnings in subclasses like
  // " `class EnglishAdverb' has virtual functions but non-virtual destructor"
  virtual ~Word() { }
	//wenn die Vokabel ein integraler Bestandteil der �bersetzung ist, wird
	//m_bIntegral auf TRUE gesetzt diese Vokabeln werden in der Funktion
	//LoadVocables() hinzugef�gt. Die Vokabeln, die das Attribut integral 
	//haben (m_bIntegral=TRUE) sollen nicht in der Vokabeldatei (zur Zeit:
	//"vocs.txt") gespeichert werden, denn sonst nehmen sie den doppelten 
	//Speicherplatz im Hauptspeicher ein
	BYTE m_bIntegral;
	//wird - so glaube ich - fuer Run Time Type Information benoetigt
	//sonst g++ Fehlermeldungen "cannot dynamic_cast `pWordCompare' (of type
	//`class Word*') to type `class GermanAdjective*' (source type is not
	//polymorphic)"
	virtual void virtfunc(){};
	BOOL operator==(Word * pWordFirst);
  void * p_iter;///e.g. pointer to an STL iterator or an array address
  void * p_word;///data
  unsigned numBytes;///number of bytes for word data
};

//#include "EnglishWord.hpp" //class EnglishWord

//extern WordNode * m_pWordFirst;
//extern WordNode * g_pWordNodeLast;
//extern WordList wordList;
class WordNode ;

BYTE IsConsonant( char ch );
BYTE IsVowel(TCHAR ch);

void InitDictionary() ;

class Adjective
{
public:
  /** see https://en.wikipedia.org/wiki/Comparison_(grammar)#Morphological_comparison */
  enum comparison { positive, comperative, superlative};
};

#endif //!defined (WORD_H_INCLUDED)
