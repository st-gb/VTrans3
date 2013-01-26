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

class GermanWord
  //For ability to iterate over all of the word's strings.
  : public Word,
    public IterableWords
{
public:
  //For ability to iterate over all of the word's strings.
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
  static const char * const pastPersonEndings [];

  enum person_indices { firstPersonSing = 0, secondPersonSing, thirdPersonSing,
    firstPersonPlur = 0, secondPersonPlur, thirdPersonPlur, beyondLastPerson };
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
  static void GetWortstamm(
    const char * wordBegin,
    //const WordData & germanWord
    int len
    , std::string & std_strWortstamm)
  {
    unsigned minus;
    if( len > 2 && *(wordBegin + len - 3) == 't' ) //arbeiTen -> arbeite ST
      minus = 1;
    else
      minus = 2;
    std_strWortstamm = std::string(wordBegin //+ germanWord.m_charIndexOfBegin,
      , //germanWord.GetStringLength()
      len - minus);
  }
  static std::string GetPresentFiniteForm(const std::string & wortStamm,
    enum person_indices person_index)
  {
//    if( wortStamm.substr(wortStamm.length() - 1, 1) == "h" )

    return wortStamm + presentPersonEndings[person_index];
  }
  static std::string GetPastFiniteForm(//const std::string & wortStamm,
    const char * const prefix, int len,
    enum person_indices person_index)
  {
    const std::string wortStamm(prefix, len);
    return wortStamm + pastPersonEndings[person_index];
  }
};

#endif /* GERMANWORD_HPP_ */
