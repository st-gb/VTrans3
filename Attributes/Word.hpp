#if !defined (WORD_H_INCLUDED)
	    #define WORD_H_INCLUDED

#include <Controller/string_type.hpp>//for typedef VTrans::string_type

//#ifdef _WINDOWS
#ifdef _MSC_VER //MS Compiler (internal MSVC preprocessor macro)
	//#include <atlstr.h>//This include is needed for "CString" when compiling as DLL.
#else //#ifdef _MSC_VER
	//#include <typedefs.h>
	//#include <StdString.h>
  //#include <MFC_Compatibility/CString.hpp>
  //using namespace MFC_Compatibility ; //for class CString in MFC_Compatibility
#endif

#include <windef.h>

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
};

class EnglishWord
{
public:
    //Because this is a translation for English->German the dictionary resp.
  //the order of the words is primarily important for English words / word
  //attributes.
  //For the
  //TODO: if the word class shares its attributes with another word class it
  // must NOT free the memory for the attributes. Else this is done more than
  // once-> a potential error.
  enum English_word_class
  {
    //These types are for building the parse tree (these are the leaves of the
    // tree) / for excluding incorrect grammatical construction when
    //building the parse tree (could be also possible afterwards, but that
    // would be less efficient because a lot of (parts of) parse trees can
    //  be unnecessarily incorrect then--the building of the tree was for
    // nothing).
    noun = //0
      //ENGLISH_NOUN
      //starting with the same indices as in the vocabulary file
      49
    , main_verb //Vollverb
    , adjective
    , adverb
    , preposition
    , pronoun
    , auxiliary_verb
    , conjunction
    //after the same indices as in the vocabulary file
    , conjunction_and
    , English_definite_article
    , English_indefinite_article
    , personal_pronoun
    , personal_pronoun_objective_form //"her","him",...
    , personal_pronoun_I
    , personal_pronoun_you_sing
    , personal_pronoun_he
    , personal_pronoun_she
    , personal_pronoun_it
    , personal_pronoun_we
    , personal_pronoun_you_plur
    , personal_pronoun_they
    //see http://en.wikipedia.org/wiki/Reflexive_pronoun:
    , reflexive_pronoun_myself
    , reflexive_pronoun_yourself
    , reflexive_pronoun_himself
    , reflexive_pronoun_herself
    , reflexive_pronoun_itself
    , reflexive_pronoun_ourselves
    , reflexive_pronoun_yourselves
    , reflexive_pronoun_themselves
    //For parsing (constructing a grammar rule ) "a >>singular<<"
    //(if the rule was just "a >>noun<<", then
    // "a cars" would also be possible.
    , singular
    , plural_noun
    //For parsing (constructing a grammar rule ) "the sheep >>3rd_pers_sing_pres"
    //(if the rule was just "the >>noun<< >>main verb<<", then the
    //  "person index" -> "finite verb form" match could
    // be done after the parse tree was created at first.
    // If using a rule ">>mainVerbAllows0object3rdPersonSingularPresent<<
    //And so "The cars sits" could not be a parse tree by using the rule
    //  "the >>singular<< >>3rd_pers_sing_pres<<"
    // would also be possible parse tree at first.
    , mainVerbAllows0object3rdPersonSingularPresent
    , mainVerbAllows1object3rdPersonSingularPresent
    , mainVerbAllows2objects3rdPersonSingularPresent
    , main_verb_allows_0object_progressive_form //e.g. "have been walking"
    , main_verb_allows_1object_progressive_form //e.g. "am hitting you"
    , main_verb_allows_2objects_progressive_form //e.g. "was giving you a car"
    , main_verb_allows_0object_infinitive
    , main_verb_allows_1object_infinitive
    , main_verb_allows_2objects_infinitive
    , beyond_last_entry
  } ;
  BYTE m_byIndex ;
  //For ability to iterate over all of the word's strings.
  virtual bool GetNextString( std::string & r_stdstr ) = 0 ;
  //Needed for English words to determine the word class resp. for
  //Insertion into trie from a derived class of this class.
  virtual BYTE GetWordClass() = 0 ;
  void InitGetNextString()
  {
    m_byIndex = 0 ;
  }
};

class GermanWord
{
  
};

class EnglishAdjective
  : public Word
{
public:
	//das Adverb, das aus diesem Adjektiv gebildet wird. Bespiel: Positiv: 
	//"slow" Adverb: "slowly"; Positiv: "good" Adverb: "well"
	VTrans::string_type m_strAdverb;
	//die erste Steigerungsform. Bespiel: Positiv: "slow" Komperativ: 
	//"slower"; Positiv: "good" Adverb: "better"
	VTrans::string_type m_strComperativ;
	//die Grundform des Adjektivs. Bespiel: "slow","good"
	VTrans::string_type m_strPositiv;
	//die zweite Steigerungsform. Bespiel: Positiv: "slow" Komperativ: 
	//"slowest"; Positiv: "good" Adverb: "best"
	VTrans::string_type m_strSuperlativ;
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
class EnglishAdverb
  : public Word
{
public:
	VTrans::string_type m_strWord;
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

#define NUMBER_OF_ENGLISH_AUX_STRINGS 15

class EnglishAuxiliaryVerb
  : public Word // englisches Hilfsverb
  //For ability to iterate over all of the word's strings.
  , public EnglishWord
{
public:
  bool GetNextString( std::string & r_stdstr ) ;
  inline BYTE GetWordClass() ;
  // Bsp.: I am, you are, he is; I was, you were; been
  //15 strings: 6 person indexes * 2 tenses (present, past) + past participle
  // + 2 * imperative = 12 + 1 + 2
	VTrans::string_type m_strWords[NUMBER_OF_ENGLISH_AUX_STRINGS];
	EnglishAuxiliaryVerb(const VTrans::string_type &,bool bAuxiliaryVerb);
	EnglishAuxiliaryVerb(){m_bIntegral=FALSE;}//EnglishAuxiliaryVerb); // Kopierkonstruktor
};

class EnglishConjunction:public Word // englisches Bindewort (Konjunktion)
{
public:
	VTrans::string_type m_strWord;
	EnglishConjunction(){m_bIntegral=FALSE;}
};

//f�r m_bTranslationType:
  //Bedeutet: Singular im Englischen ist auch Singular im Deutschen, 
  // Plural im Englischen ist auch Plural im Deutschen,
  #define SAME_GRAMMATICAL_NUMBER_BETWEEN_ENGLISH_AND_GERMAN 0
  #define SINGULAR_IN_ENGLISH_PLURAL_IN_GERMAN 1
  #define PLURAL_IN_ENGLISH_SINGULAR_IN_GERMAN 2
class EnglishNoun
  : public Word
{
public:
	VTrans::string_type m_strSingular;
	VTrans::string_type m_strPlural;
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
	VTrans::string_type m_strWord;
	EnglishPreposition(){m_bIntegral=FALSE;}
};

class EnglishPronoun:public Word
{
public:
	VTrans::string_type m_strSingular;
	VTrans::string_type m_strPlural;
	//BYTE type; // Typ:. Adverbien der H�ufigkeit (always,never,often), 
	// der Art (well, slowly, hard), Adverbien, die sich auf einen ganzen Satz
	// beziehen (Fortunately, Unfortunately), ...
	EnglishPronoun(){m_bIntegral=FALSE;}
};

class EnglishVerb
  : public Word
  , public EnglishWord
{
public:
  enum object_type {
    no_object
    , _1_object
    , _2_objects //e.g.: "to give somebody something"
    };
	VTrans::string_type m_strInfinitive;
	VTrans::string_type m_strPastTense;
	VTrans::string_type m_strPastParticiple; //Partizip Perfekt
	VTrans::string_type m_strPreposition;
	BYTE m_bAllowsIngForm;
	//wenn nicht "0": das Verb kann eine Verlaufsform (progressive) haben
	BYTE m_byDynamic;
	BYTE m_bAllowsToPlusInfinitive;
	BYTE m_bAuxiliaryType;
  BYTE m_byObjectType ;
	//m_bDescriptionType gibt an, ob ein Wort, das dieses Verb beschreibt,
	//ein Adjektiv oder ein Adverb ist
	BYTE m_bDescriptionType;
	EnglishVerb(){m_bIntegral=FALSE;}
  EnglishVerb( BYTE byObjectType ) { m_byObjectType = byObjectType ; }
  //"static" should be faster because no "this" pointer is IMPLICITELY copied
  // as function argument.
  //inline
  static void Get3rdPersonForm( VTrans::string_type & r_vtransstr ) ;
  static void GetProgressiveForm( VTrans::string_type & r_vtransstr ) ;
  //For ability to iterate over all of the word's strings.
  bool GetNextString( std::string & r_stdstr ) { return false ; } ;
  //Needed for English words to determine the word class resp. for
  //Insertion into trie from a derived class of this class.
  BYTE GetWordClass() { return main_verb ; } ;
};

class GermanAdjective:public Word
{
public:
	VTrans::string_type m_strPositiv; // z.B. hoch
	VTrans::string_type m_strComperativ; // z.B. h�her
	VTrans::string_type m_strSuperlativ; // z.B. am h�chsten
	VTrans::string_type m_strWortstamm; // wichtig f�r: hoch - ein HOHes Haus (der Wortstamm hierbei
	// ist hoh)
	GermanAdjective(){m_bIntegral=FALSE;}
};

class GermanAdverb:public Word
{
public:
	VTrans::string_type m_strWord;
	GermanAdverb(){m_bIntegral=FALSE;}
};

class GermanAuxiliaryVerb:public Word // deutsches Hilfsverb
{
public:
	VTrans::string_type m_strWords[15]; // Bsp.: I am, you are, he is; I was, you were; been
	GermanAuxiliaryVerb(const VTrans::string_type &);
	GermanAuxiliaryVerb(){m_bIntegral=FALSE;}
};

class GermanConjunction:public Word // englisches Bindewort (Konjunktion)
{
public:
	VTrans::string_type m_strWord;
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
	VTrans::string_type m_strSingular;
	VTrans::string_type m_strPlural;
	BYTE m_bArticle;
	GermanNoun(){m_bIntegral=FALSE;}
	//virtual virtfunc(){};
};

class GermanPreposition:public Word
{
public:
  BYTE m_byCase;
	VTrans::string_type m_strWord;
	GermanPreposition(){m_bIntegral=FALSE; m_byCase=0;}
};

class GermanPronoun:public Word
{
public:
	VTrans::string_type m_strWord;
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

//Class for both Auxiliary and main verb because they both have the same
//attributes.
class GermanVerb
  : public Word //deutsches Vollverb (engl.>verb< heisst Vollverb)
  , public GermanWord
{
public:
  enum e_case {
    nominative = 1
    , genitive
    , dativ
    , accusativ
    //"Ich gab ihm ein Buch."
    //          \/   \    /
    //       dative accusative
    , dativ_and_accusativ
    } ;
	VTrans::string_type m_strWords[16];
  //Für Uebersetzung: ob "wo" oder "wohin":
  //"true": Verb der Bewegung (für die Uebersetzung: "Wohin ging er?")
  //"false": kein Verb der Bewegung ("Wo stand er?")
	bool m_bMove;
	bool m_bBe; // "true": Hilfsverb ist "sein"
	bool m_bAuxiliaryVerb;
	//Der Fall: 0=kein,1=jemandeM (3. Fall),2=jemandeN (4.Fall),3=3. und 4. Fall
  //4=Genitiv
	BYTE m_bCase;
	bool m_bReflexive; //reflexives Verb.
	VTrans::string_type m_strPreposition;

  void CreateFromString(const VTrans::string_type & str) ;
	GermanVerb(const VTrans::string_type & );
	GermanVerb(const VTrans::string_type & , e_case );
	GermanVerb()
  {
    m_bAuxiliaryVerb = FALSE;
    m_bReflexive = FALSE;
		m_bIntegral = FALSE;
  }
};

//extern WordNode * m_pWordFirst;
//extern WordNode * g_pWordNodeLast;
//extern WordList wordList;
class WordNode ;

void InitDictionary() ;

#endif //!defined (WORD_H_INCLUDED)
