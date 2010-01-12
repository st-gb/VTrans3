#if !defined (WORD_H_INCLUDED)
	    #define WORD_H_INCLUDED

//#ifdef _WINDOWS
#ifdef _MSC_VER //MS Compiler (internal MSVC preprocessor macro)
	#include <atlstr.h>//This include is needed for "CString" when compiling as DLL.
#else //#ifdef _MSC_VER
	#include <typedefs.h>
	//#include <StdString.h>
  //#include <MFC_Compatibility/CString.hpp>
  using namespace MFC_Compatibility ; //for class CString in MFC_Compatibility
#endif
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
	//wenn die Vokabel ein integraler Bestandteil der �bersetzung ist, wird
	//m_bIntegral auf TRUE gesetzt diese Vokabeln werden in der Funktion
	//LoadVocables() hinzugef�gt. Die Vokabeln, die das Attribut integral 
	//haben (m_bIntegral=TRUE) sollen nicht in der Vokabeldatei (zur Zeit:
	//"vocs.txt") gespeichert werden, denn sonst nehmen sie den doppelten 
	//Speicherplatz im Hauptspeicher ein
	BYTE m_bIntegral;
	//wird - so glaube ich - f�r Run Time Type Information ben�tigt
	virtual void virtfunc(){};
	BOOL operator==(Word * pWordFirst);
};

class EnglishAdjective:public Word
{
public:
	//das Adverb, das aus diesem Adjektiv gebildet wird. Bespiel: Positiv: 
	//"slow" Adverb: "slowly"; Positiv: "good" Adverb: "well"
	CString m_strAdverb;
	//die erste Steigerungsform. Bespiel: Positiv: "slow" Komperativ: 
	//"slower"; Positiv: "good" Adverb: "better"
	CString m_strComperativ;
	//die Grundform des Adjektivs. Bespiel: "slow","good"
	CString m_strPositiv;
	//die zweite Steigerungsform. Bespiel: Positiv: "slow" Komperativ: 
	//"slowest"; Positiv: "good" Adverb: "best"
	CString m_strSuperlativ;
	char m_regulary;
	//gibt an, ob die Steigerungsformen durch "more" und "most" erreicht
	//werden
	BYTE m_bMoreMost;
	//die Stelle, an der sich ein Adjektiv befinden darf, 0: vor einem 
	//Substantiv und als Umstandsbestimmung der Art und Weise, 1: nur vor 
	//einem Substantiv 2: nur als Umstandsbestimmung der Art und Weise
	//Beispiel f�r ein Adjektiv, das nur vor einem Substantiv stehen 
	//darf: "sick": "I am a sick boy."
	//Beispiel f�r ein Adjektiv, das nur als Umstandsbestimmung der Art und 
	//Weise benutzt werden darf: "ill": "I am ill."
	BYTE m_bAllowedPlace;
	EnglishAdjective(){m_bIntegral=FALSE; m_bMoreMost=0;m_bAllowedPlace=0;}
};

#define BEFORE_FULL_VERB_OR_AFTER_FINITE_AUXILIARY_VERB 0
#define DESCRIBING_ADVJECTIVE 1
#define REFERRING_WHOLE_CLAUSE 2
#define QUESTION_WORD 4
#define ADVERB_OF_TIME 5
#define ASK_FOR_SUBJECT 6
class EnglishAdverb:public Word
{
public:
	CString m_strWord;
	//m_bType: 0: Adverb befindet sich zwischen Subjekt und Verb oder nach 
	//der finiten Form des Verbes 'to be' wie z.B. Adverbien der H�ufigkeit
	//(always,never,often) 1: Adverb bezieht sich auf ein Adjektiv oder 
	//Adverb (well, slowly, hard) 2: Adverb bezieht sich auf ganzen Satz 
	//und steht am Satzbeginn (Fortunately, Unfortunately) 3: Adverb bezieht
	//sich auf ganzen Satz und steht am Satzende 4: Adverb ist ein Fragewort
	//5: Adverb der Zeit
	BYTE m_bType;
	EnglishAdverb(){m_bIntegral=FALSE;}
};

class EnglishAuxiliaryVerb:public Word // englisches Hilfsverb
{
public:
	CString m_strWords[15]; // Bsp.: I am, you are, he is; I was, you were; been
	EnglishAuxiliaryVerb(const CString &,bool bAuxiliaryVerb);
	EnglishAuxiliaryVerb(){m_bIntegral=FALSE;}//EnglishAuxiliaryVerb); // Kopierkonstruktor
};

class EnglishConjunction:public Word // englisches Bindewort (Konjunktion)
{
public:
	CString m_strWord;
	EnglishConjunction(){m_bIntegral=FALSE;}
};

//f�r m_bTranslationType:
  //Bedeutet: Singular im Englischen ist auch Singular im Deutschen, 
  // Plural im Englischen ist auch Plural im Deutschen,
  #define SAME_GRAMMATICAL_NUMBER_BETWEEN_ENGLISH_AND_GERMAN 0
  #define SINGULAR_IN_ENGLISH_PLURAL_IN_GERMAN 1
  #define PLURAL_IN_ENGLISH_SINGULAR_IN_GERMAN 2
class EnglishNoun:public Word
{
public:
	CString m_strSingular;
	CString m_strPlural;
	//bestimmter Artikel (the) immer, nie oder nur wenn speziell
	BYTE m_bType;
	BYTE m_bTranslationType;
	//gibt an, ob das Substantiv z�hlbar ist. Beispiel: "water" ist unz�hlbar,
	//"I have two water." ergibt keinen Sinn. "children" ist z�hlbar:
	//"I have got three children." ist in Ordnung
	//Das Attribut dient nr zur �berpr�fung des Satzes und nicht zur 
	//�bersetzungsqulit�t
	BYTE m_bCountable;
	//virtual virtfunc(){};
	EnglishNoun(){m_bIntegral=FALSE;}
};

class EnglishPreposition:public Word
{
public:
	CString m_strWord;
	EnglishPreposition(){m_bIntegral=FALSE;}
};

class EnglishPronoun:public Word
{
public:
	CString m_strSingular;
	CString m_strPlural;
	//BYTE type; // Typ:. Adverbien der H�ufigkeit (always,never,often), 
	// der Art (well, slowly, hard), Adverbien, die sich auf einen ganzen Satz
	// beziehen (Fortunately, Unfortunately), ...
	EnglishPronoun(){m_bIntegral=FALSE;}
};

class EnglishVerb:public Word
{
public:
	CString m_strInfinitive;
	CString m_strPastTense;
	CString m_strPastParticiple; //Partizip Perfekt
	CString m_strPreposition;
	BYTE m_bAllowsIngForm;
	//wenn nicht "0": das Verb kann eine Verlaufsform (progressive) haben
	BYTE m_byDynamic;
	BYTE m_bAllowsToPlusInfinitive;
	BYTE m_bAuxiliaryType;
	//m_bDescriptionType gibt an, ob ein Wort, das dieses Verb beschreibt,
	//ein Adjektiv oder ein Adverb ist
	BYTE m_bDescriptionType;
	EnglishVerb(){m_bIntegral=FALSE;}
};

class GermanAdjective:public Word
{
public:
	CString m_strPositiv; // z.B. hoch
	CString m_strComperativ; // z.B. h�her
	CString m_strSuperlativ; // z.B. am h�chsten
	CString m_strWortstamm; // wichtig f�r: hoch - ein HOHes Haus (der Wortstamm hierbei
	// ist hoh)
	GermanAdjective(){m_bIntegral=FALSE;}
};

class GermanAdverb:public Word
{
public:
	CString m_strWord;
	GermanAdverb(){m_bIntegral=FALSE;}
};

class GermanAuxiliaryVerb:public Word // deutsches Hilfsverb
{
public:
	CString m_strWords[15]; // Bsp.: I am, you are, he is; I was, you were; been
	GermanAuxiliaryVerb(const CString &);
	GermanAuxiliaryVerb(){m_bIntegral=FALSE;}
};

class GermanConjunction:public Word // englisches Bindewort (Konjunktion)
{
public:
	CString m_strWord;
	//word order: Satzbau
	BYTE m_bWordOrder; // Satzbau: 0=Subjekt Pr�dikat [Objekt] 1=Subjekt [Objekt] Pr�dikat
	// Beispiele: If you like me. -> Falls du mich magst.
	//                                     S   O     P
	//  If you are intelligent. -> Falls du intelligent bist.
	//                                   S  Adverb       P
	GermanConjunction()
	{
		m_bWordOrder=0;
		m_bIntegral=FALSE;
	}
};

#define DER '1'
#define DIE '2'
#define DAS '3'
#define PLURAL 4
class GermanNoun:public Word
{
public:
	CString m_strSingular;
	CString m_strPlural;
	BYTE m_bArticle;
	GermanNoun(){m_bIntegral=FALSE;}
	//virtual virtfunc(){};
};

class GermanPreposition:public Word
{
public:
  BYTE m_byCase;
	CString m_strWord;
	GermanPreposition(){m_bIntegral=FALSE; m_byCase=0;}
};

class GermanPronoun:public Word
{
public:
	CString m_strWord;
	GermanPronoun(){m_bIntegral=FALSE;}
};

#define DATIVE 3
#define ACCUSATIVE 4
//z.B. to give: "He gave him a book."
//                       \ / \    /
//                   dative  accusative
#define DATIVE_AND_ACCUSATIVE 5
#define NOMINATIVE 0
#define GENITIVE 2
#define FIRST_PERSON_SINGULAR 1
class GermanVerb:public Word //deutsches Vollverb (engl.>verb< hei�t Vollverb)
{
public:
	CString m_strWords[16];
	BOOL m_bMove; // Verb der Bewegung
	BOOL m_bBe; // Hilfsverb: sein
	BOOL m_bAuxiliaryVerb;
	//Der Fall: 0=kein,1=jemandeM (3. Fall),2=jemandeN (4.Fall),3=3. und 4. Fall
  //4=Genitiv
	BYTE m_bCase;
	BOOL m_bReflexive;
	CString m_strPreposition;
	GermanVerb(const CString &);
	GermanVerb(){m_bAuxiliaryVerb=FALSE;m_bReflexive=FALSE;
		m_bIntegral=FALSE;};
};

//extern WordNode * m_pWordFirst;
//extern WordNode * g_pWordNodeLast;
//extern WordList wordList;
class WordNode ;

void LoadWords(WordNode * wn);
void InitDictionary() ;

#endif //!defined (WORD_H_INCLUDED)
