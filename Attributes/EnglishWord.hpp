/*
 * EnglishWord.hpp
 *
 *  Created on: 31.07.2011
 *      Author: Stefan
 */

#ifndef ENGLISHWORD_HPP_
#define ENGLISHWORD_HPP_

#include "IterableWords.hpp"
#include "Word.hpp"

class EnglishWord:
  public Word,
  public IterableWords
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
    noun = 0
      //ENGLISH_NOUN
      //starting with the same indices as in the vocabulary file
      //49
    , main_verb //Vollverb
    , adverb
    , adjective
    , adjective_positiveForm
    , main_verb_allows_0object_infinitive
    , main_verb_allows_1object_infinitive
    , main_verb_allows_2objects_infinitive
    , preposition
    , pronoun
    , auxiliary_verb
    //for predicate/ tense
    , be1stPersonSingular
//    , have2ndPersonSingular
    , haveInfinitive
    , will

    , conjunction
    //after the same indices as in the vocabulary file
    , conjunction_and

    , English_definite_article
    , English_indefinite_article

//    , have

    , personal_pronoun
    , personal_pronoun_objective_form //"her","him",...
    , personal_pronoun_I
    , personal_pronoun_you
//    , personal_pronoun_you_sing
    , personal_pronoun_he
    , personal_pronoun_she
    , personal_pronoun_it
    , personal_pronoun_we
    , personal_pronoun_you_plur
    , personal_pronoun_they

    , personal_pronoun_me
    , personal_pronoun_objective_you //-> "dich"/ "dir" / "uns" /"Ihnen" in German
//    , personal_pronoun_you_sing
    , personal_pronoun_him
    , personal_pronoun_her
    , personal_pronoun_objective_it //
    , personal_pronoun_us
    , personal_pronoun_them

    //see http://en.wikipedia.org/wiki/Reflexive_pronoun:
    , reflexive_pronoun_myself
    , reflexive_pronoun_yourself
    , reflexive_pronoun_himself
    , reflexive_pronoun_herself
    , reflexive_pronoun_itself
    , reflexive_pronoun_ourselves
    , reflexive_pronoun_yourselves
    , reflexive_pronoun_themselves

    //for relative_pronoun + 3rdPersSingClauseAllows1Obj = relative_clause
    , relative_pronoun
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
    , main_verb_allows_0object_past_form
    , main_verb_allows_1object_past_form
    , main_verb_allows_2objects_past_form
    , mainVerbPastParticiple0Obj
    , mainVerbPastParticiple1Obj
    , mainVerbPastParticiple2Obj
    , main_verb_allows_0object_progressive_form //e.g. "have been walking"
    , main_verb_allows_1object_progressive_form //e.g. "am hitting you"
    , main_verb_allows_2objects_progressive_form //e.g. "was giving you a car"
    , main_verb_past

    , am
    , are
    , is
    , UnknownWord
    , beyond_last_entry
  } ;
  /** Needed for English words to determine the word class resp. for
  * insertion into trie from a derived class of this class. */
  virtual /*BYTE*/ English_word_class GetWordClass() = 0 ;
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
  : //public Word // englisches Hilfsverb
  //,
  public EnglishWord
  //For ability to iterate over all of the word's strings.
//  , public IterableWords
{
public:
  bool GetNextString( std::string & r_stdstr ) ;
  inline /*BYTE*/ English_word_class GetWordClass() ;
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
  : //public Word
  //,
  public EnglishWord
{
public:
  enum EnglishVerbArrayIndices { infinitive = 0, simple_past, past_participle,
    third_person_present, arraySize};
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
  /*BYTE*/ English_word_class GetWordClass() { return main_verb ; } ;

  static enum English_word_class Get3rdPersonVerbWordClass(
    BYTE byNumberOfObjectsAllowed)
  {
    enum English_word_class english_word_class;
    switch(byNumberOfObjectsAllowed)
    {
    case 0 :
      english_word_class = EnglishWord::mainVerbAllows0object3rdPersonSingularPresent ;
      break ;
    case 1 :
      english_word_class = EnglishWord::mainVerbAllows1object3rdPersonSingularPresent ;
      break ;
    case 2 :
      english_word_class = EnglishWord::mainVerbAllows2objects3rdPersonSingularPresent ;
      break ;
    }
    return english_word_class;
  }
};

#endif /* ENGLISHWORD_HPP_ */
