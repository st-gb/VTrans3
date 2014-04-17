/*
 * GermanWord.hpp
 *
 *  Created on: 31.07.2011
 *      Author: Stefan
 */

#ifndef GERMANWORD_HPP_
#define GERMANWORD_HPP_

#include "Word.hpp"
#include "IterableWords.hpp"
#include "EnglishWord.hpp" //EnglishWord::English_word_class
//GCC_DIAG_OFF(...), GCC_DIAG_ON(...)
#include <compiler/GCC/enable_disable_warning.h>
#include <string.h> //strcmp()

class GermanWord
  //For ability to iterate over all of the word's strings.
  : public Word,
    public IterableWords
{
public:
  enum German_word_class
  {
    MasculineNoun,
    FeminineNoun,
    NeutralNoun,
    IntransitiveVerb,
    TransitiveVerb,
    adjective,
    adverb
  };
  /** For ability to iterate over all of the word's strings. */
  virtual bool GetNextString( std::string & r_stdstr ) = 0 ;
    //{ return false; }
};

class GermanAdjective
  : public Word
{
public:
  VTrans::string_type m_strPositiv; // z.B. hoch
  VTrans::string_type m_strComperativ; // z.B. h�her
  VTrans::string_type m_strSuperlativ; // z.B. am h�chsten
  VTrans::string_type m_strWortstamm; // wichtig f�r: hoch - ein HOHes Haus (der Wortstamm hierbei
  // ist hoh)
  GermanAdjective(){m_bIntegral=FALSE;}
};

class GermanAdverb
  : public Word
{
public:
  VTrans::string_type m_strWord;
  GermanAdverb(){m_bIntegral=FALSE;}
};

class GermanAuxiliaryVerb
  : //public Word // deutsches Hilfsverb
//  //For ability to iterate over all of the word's strings.
//  , public IterableWords
//    ,
    public GermanWord
{
public:
  virtual bool GetNextString( std::string & r_stdstr );
  inline BYTE GetWordClass(){ return 0;}

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

class GermanNoun: public Word
{
public:
  enum german_article { der = 0, die, das};
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
  //,
//  public GermanWord
{
public:
  static const char * const presentPersonEndings [];
  static const char * const presentPersonEndings2 [];
  static const char * const pastPersonEndings [];
  static const char * const s_irregularVerbPresentSingularPersonEndings [];

  enum person_indices { firstPersonSing = 0, secondPersonSing, thirdPersonSing,
    firstPersonPlur, secondPersonPlur, thirdPersonPlur, beyondLastPerson };
  enum german_verb_array_indices{
    arrayIndexForInfinitive = 0,
    arrayIndexFor1stPersSingPres,
    arrayIndexFor2ndPersSingPres,
    arrayIndexFor3rdPersSingPres,
    arrayIndexFor1stPersPlurPres,
    arrayIndexFor2ndPersPlurPres,
    arrayIndexFor3rdPersPlurPres,
    arrayIndexFor1stPersSingPast,
    arrayIndexFor2ndPersSingPast,
    arrayIndexFor3rdPersSingPast,
    arrayIndexFor1stPersPlurPast,
    arrayIndexFor2ndPersPlurPast,
    arrayIndexFor3rdPersPlurPast,
    arrayIndexForPastParticiple
    };
  enum e_case {
    nominative = 1
    , genitive
    , dativ
    , accusativ
    //"Ich gab ihm ein Buch."
    //          \/   \    /
    //       dative accusative
    , dativ_and_accusativ
    /** If a grammatical case is not given in the dictionary, it typically may
     *  be either dative or accusative. */
    , dative_or_accusative
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

  /** @param wordBegin: tense needs to be "simple present". */
  static void GetWordStemFrom3rdPersonPresent(
    const char * wordBegin,
    //const WordData & germanWord
    int len,
    std::string & std_strWortstamm
    )
  {
    unsigned minus = 0;
    if( len > 2 && *(wordBegin + len - 3) == 't' ) //arbeiTet -> arbeiT
      minus = 2;
    else /** e.g. "geHT" -> "geh" */
      minus = 1;
    //TODO for words that are prepended with a preposition:
    //  subtract preposition: e.g. "zukaufen": minus preposition "zu" ->
    //  "kaufen" -> "kaufe zu"
    std_strWortstamm = std::string(wordBegin //+ germanWord.m_charIndexOfBegin,
      , //germanWord.GetStringLength()
      len - minus);
  }

  /** @param wordBegin: tense needs to be "simple present". */
  static void GetWordStemFromInfinitive(
    const char * wordBegin,
    //const WordData & germanWord
    const int len,
    std::string & std_strWortstamm
    )
  {
    unsigned minus = 0;
    if( len > 3 )
    {
//      const std::string last2Chars = std::string(wordBegin + len - 2, 2);
      /** e.g. wandeRN, flunkeRN, meckeRN, moseRN */
      if( strcmp( (wordBegin + len - 2), "rn") == 0 ) //wanderN -> wanderST
        minus = 1;
      /** geHEN, seHEN, steHEN*/
      else if ( strcmp( (wordBegin + len - 3), "hen") == 0 )
        minus = 2;
      else //if( *(wordBegin + len - 3) == 't' ) //arbeiTen -> arbeiteST
        minus = 1;
//      else /** e.g. "geHen" -> "gehST" */
//        minus = 2;
    }
    //TODO for words that are prepended with a preposition:
    //  subtract preposition: e.g. "zukaufen": minus preposition "zu" ->
    //  "kaufen" -> "kaufe zu"
    std_strWortstamm = std::string(wordBegin //+ germanWord.m_charIndexOfBegin,
      , //germanWord.GetStringLength()
      len - minus);
  }

  static void GetWordStemFromInfinitive(
    const std::string word,
    //const WordData & germanWord
    std::string & std_strWortstamm
    )
  {
    GetWordStemFromInfinitive(
      word.c_str(),// const char * wordBegin,
      //const WordData & germanWord
      word.length(), //const int len,
      std_strWortstamm);
  }

  /** @param wordBegin: tense needs to be a verb. */
  static void GetWordStem(
    const char * wordBegin,
    //const WordData & germanWord
    int len,
    const EnglishWord::English_word_class grammarPartID,
    std::string & std_strWortstamm
    )
  {
    unsigned minus;
    GCC_DIAG_OFF(switch)
    switch(grammarPartID)
    {
    case EnglishWord::main_verb_allows_0object_past_form :
    case EnglishWord::main_verb_allows_1object_past_form:
    case EnglishWord::main_verb_allows_2objects_past_form:
      if( len > 3 && *(wordBegin + len - 4) == 't' ) //arbeiTete -> arbeiT
        minus = 3;
      else /** e.g. "ging" -> "ging" ("gingST, gingT, gingEN) */
        minus = 0;
      break;
    case EnglishWord::mainVerbAllows0object3rdPersonSingularPresent :
    case EnglishWord::mainVerbAllows1object3rdPersonSingularPresent:
    case EnglishWord::mainVerbAllows2objects3rdPersonSingularPresent:
      GetWordStemFrom3rdPersonPresent(wordBegin, len, std_strWortstamm);
      break;
    case EnglishWord::main_verb_allows_0object_infinitive:
    case EnglishWord::main_verb_allows_1object_infinitive:
    case EnglishWord::main_verb_allows_2objects_infinitive:
      GetWordStemFromInfinitive(wordBegin, len, std_strWortstamm);
      break;
    }
    GCC_DIAG_ON(switch)
  }

  //TODO umlauts: e.g. "laufen" -> du "lÄufst" ("a"->"ä")
  // kaufen kauft
//  static void HandleUmlauts(const std::string & wortStamm)
//  {
//    std::string::size_type pos = wortStamm.find("au");
//    if( )
//  }

  //TODO umlauts: e.g. "laufen" -> du "lÄufst" ("a"->"ä")
  /** @brief Get simple present finite verb form for a person index from a
   *   word stem.
   *  @param person_index: e.g. 2nd person plural */
  static std::string GetPresentFiniteForm(
    const std::string & wortStamm,
    const enum person_indices person_index)
  {
//    if( wortStamm.substr(wortStamm.length() - 1, 1) == "h" )
    const char lastChar = *(wortStamm.c_str() + wortStamm.length() - 1);
    switch( lastChar )
    {
    case 'b' : //geBen, lieBen
    case 'f' : //schärFen, kauFen
    case 'g' : //leGen
    case 'h' : //geHen, seHen, steHen
    case 'l' : //lalLen, gefalLen
    case 'm' : //miMen
    case 'n' : //nenNen, grieNen
    case 'p' : //popPen
    case 'r' : //fahRen,
      return wortStamm + presentPersonEndings[person_index];
//    default:
    }
    //MelDen ->EST, gedulDen
    return wortStamm + presentPersonEndings2[person_index];
  }
  static std::string GetPastFiniteForm(//const std::string & wortStamm,
    const char * const prefix,
    int len,
    enum person_indices person_index)
  {
    const std::string wortStamm(prefix, len);
//    if( prefix + len - 1 == 'h' ) //gehen, sehen, stehen
      return wortStamm + pastPersonEndings[person_index];
//    else
//      return wortStamm + pastPersonEndings[person_index];
  }
  static std::string GetPastFiniteForm(//const std::string & wortStamm,
    const std::string & wortStamm,
    enum person_indices person_index)
  {
    return GetPastFiniteForm(wortStamm, person_index);
  }
};

#endif /* GERMANWORD_HPP_ */
