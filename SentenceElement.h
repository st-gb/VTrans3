#if !defined (SENTENCEELEMENT_H_INCLUDED)
	#define SENTENCEELEMENT_H_INCLUDED

  #include <vector>
  #include "Word.hpp" //for class WordNode, EnglishAdjective etc.
  #include "rest.h" //for CStringVector etc.
  #include "GetAndTestWord.h" //for "class PointerToEnglishNoun" etc.
  #include "Token.h" //for class PositionCStringVector
  #ifdef USE_LETTER_TREE
    #include "VocabularyInMainMem/LetterTree/LetterTree.hpp"
  #endif
  #ifndef _WINDOWS
    #include "typedefs.h" //for TRACE
  #endif //#ifndef _WINDOWS

  class EnglishNounRecord
  {
  public:
	  BOOL bDefiniteArticle;
	  BOOL bIndefiniteArticle;
	  CStringVector m_Adjectives;
	  CStringVector m_Noun;
	  std::vector<PointerToEnglishNoun> m_vecPointerToEnglishNoun;
	  std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> 
		  m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated;
	  std::vector<WordNode *>m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated;
	  std::vector<EnglishAdjective*> m_vecpEnglishAdjective;
	  std::vector<BYTE> m_vecAdjectiveType;
	  WordNode * 
		  m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated;
	  WordNode * m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated;
	  BYTE m_personalPronoun;
	  BYTE m_possessivePronoun;
    //char * arstrInflectionSuffix [4];// ={"em","er","e","en"};
	  EnglishNounRecord()
	  {
		  bDefiniteArticle=FALSE;
		  bIndefiniteArticle=FALSE;
		  m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated=NULL;
		  m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated=
			  NULL;
		  m_personalPronoun=0;
		  m_possessivePronoun=0;
	  };
  };

  class OfPhrase
  {
	  //EnumerationElement m_ee;
  public:
	  EnglishNounRecord m_Enr;
  };

  class DefiniteArticleNoun
  {
  public:
	  CStringVector m_Noun;
	  std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> 
		  m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated;
  };

  class Gerund; // vorher-Deklaration, Definition folgt weiter unten
  class ToPlusInfinitive; // vorher-Deklaration, Definition folgt weiter unten
  class RelativeClause; // vorher-Deklaration, Definition folgt weiter unten
  class Object; // vorher-Deklaration, Definition folgt weiter unten

  class EnumerationElement
  {
  public:
	  //wird bei Eigennamen oder nicht erkannten Sachen verwendet
	  CString m_plainText;
  #ifdef USE_LETTER_TREE
    //EngNounsLetterTree * m_pengnounslettertree ;
    EngNounsLetterTree m_engnounslettertree ;
    void AddVocabularyAndTranslation( VocabularyAndTranslation * pvoc)
    {
      m_engnounslettertree.Insert(pvoc) ;
    }
  #endif
	  CStringVector m_Adjectives;
	  CStringVector m_Noun;
	  CStringVector m_vecStrPlainText;
	  CStringVector m_strWords;
	  BOOL m_bArticle;
	  BOOL m_bDefiniteArticle;
	  BOOL m_bIndefiniteArticle;
	  BYTE m_personalPronoun;
	  BYTE m_possessivePronoun;
	  DefiniteArticleNoun * m_pDan;
	  EnglishNounRecord * m_pEnr;
	  Gerund * m_pGerund;
	  OfPhrase * m_pOfPhrase;
	  //ein Relativsatz bezieht sich auf das kleinste Element des
	  //Satzes (also ein Aufzählungselement des Subjektes zum
	  //Beispiel), und nicht nur auf das gesamte Subjekt
	  RelativeClause * m_pRelativeClause;
	  ToPlusInfinitive * m_pToPlusInfinitive;
	  EnumerationElement()
	  {
  #ifdef USE_LETTER_TREE
    //m_pengnounslettertree = NULL ;
  #endif
		  m_bArticle=FALSE;
		  m_bDefiniteArticle=FALSE;
		  m_bIndefiniteArticle=FALSE;
		  m_pDan=NULL;
		  m_pEnr=NULL;
		  m_pOfPhrase=NULL;
		  m_pGerund=NULL;
		  m_pRelativeClause=NULL;
		  m_pToPlusInfinitive=NULL;
		  m_personalPronoun=0;
		  m_possessivePronoun=0;
	  };
	  EnumerationElement(const CString & rStr)
	  { // für Eigennamen
  #ifdef USE_LETTER_TREE
    //m_pengnounslettertree = NULL ;
  #endif
		  m_plainText=rStr;
		  m_pOfPhrase=NULL;
	  }
  #ifdef _DEBUG
    ~ EnumerationElement()
    {
      TRACE("EnumerationElement(address of this object:%x) dtor",this);
    }
  #endif
  };

  class MiddleEnumerationElement
  {
  public:
	  CString m_strAdverbOfManner;
	  CString m_strAdverbRefersToAdjectiveOrAdverb;
  };

  class SentenceElement // SentenceElement muß textuell über den Subklassen
  { // stehen, alphabetische Ordnung nicht möglich
	  virtual void virtFunc(){};
  public:
    DWORD m_dwErrorCode;
  };

  class Object:public SentenceElement
  {
	  public:
	  std::vector<EnumerationElement> m_vecEnumerationElement;
  #ifdef USE_LETTER_TREE
    std::vector<EnumerationElement *> m_vecpenumerationelement ;
    void AppendEnumEle()
    {
      //EnumerationElement enumele ;
      //m_vecEnumerationElement.push_back(enumele) ;
      m_vecpenumerationelement.push_back(new EnumerationElement() ) ;
    }
  #endif//#ifdef USE_LETTER_TREE
  };

  //Vergleich; Beispielsatz: "I am older than you."
  //															 |				    |
  //															 +-Vergleich--+
  //(in diesem Fall: "older than you"; zu Deutsch: "älter als du")
  //ein Vergleich beinhaltet IMMER auch ein Objekt (in diesem Fall "du")
  class Comparison
  {
  public:
	  WordNode * m_pWordNodeWhereAnEnglishComperativeIsEncapsulated;
	  Object m_Object;
	  Comparison()
	  {
		  m_pWordNodeWhereAnEnglishComperativeIsEncapsulated=NULL;
	  }
  };

  //Gleichheit; Beispielsatz: I am as old as you.
  //															 |					 |
  //                               +Gleichheit-+
  //(in diesem Fall: "as old as you"; zu Deutsch: "genauso alt wie du")
  //eine Gleichheit beinhaltet IMMER auch ein Objekt (in diesem Fall "du")
  class Similarity
  {
  public:
	  WordNode * m_pWordNodeWhereAnEnglishPositiveIsEncapsulated;
	  Object * m_pObject;
	  Similarity()
	  {
		  m_pWordNodeWhereAnEnglishPositiveIsEncapsulated=NULL;
		  m_pObject=NULL;
	  }
  };

  //MannerEnumerationElement: Ausfzählungselement einer Umstandsbestimmung der
  //Art und Weise:
  // "I am intelligent and old."

  //Umstandsbestimmung der Art und Weise: "intelligent and old"
  //1. Aufzählungselement einer Umstandsbestimmung der Art und Weise:
  //"intelligent"
  //2. Aufzählungselement einer Umstandsbestimmung der Art und Weise:
  //"old"
  class MannerEnumerationElement
  {
  public:
	  WordNode * m_pWordNodeAdverbOfManner;
	  WordNode * m_pWordNodeAdverbRefersToAdjectiveOrAdverb;
	  Similarity * m_pSimilarity;
	  Comparison * m_pComparison;
	  MannerEnumerationElement()
	  {
		  TRACE("MannerEnumerationElement()\n");
		  m_pComparison=NULL;
		  m_pSimilarity=NULL;
		  m_pWordNodeAdverbOfManner=NULL;
		  m_pWordNodeAdverbRefersToAdjectiveOrAdverb=NULL;
	  };
  };

  // Umstandsbestimmung der Art und Weise (man kann sie bestimmen, 
  //indem man nach ihr mit "wie" fragt. Beispiel: "Er läuft langsam." 
  //"Wie läuft er?" -Umstandsbestimmung der Art und Weise: "langsam" 
  //Im Englischen: "He walks slowly." "slowly" ist dabei die Umstandsbestimmung der 
  //Art und Weise
  class Manner
  {
  public:
	  std::vector<MannerEnumerationElement> m_vecMannerEnumerationElement;
  };

  class ByAgentEnumerationElement
  {
  public:
	  EnglishNounRecord * m_pEnr;
	  DefiniteArticleNoun * m_pDan;
	  CString m_strPlainText;
	  CStringVector m_vecstrAdjectives;
	  CStringVector m_vecstrNoun;
	  BYTE m_personalPronoun;
	  BYTE m_possessivePronoun;
	  CStringVector m_vecstrWords;
	  BOOL m_bArticle;
	  BOOL m_bDefiniteArticle;
	  BOOL m_bIndefiniteArticle;
	  OfPhrase * m_pOfPhrase;
	  ByAgentEnumerationElement()
	  {
		  m_pEnr=NULL;
		  m_pDan=NULL;
		  m_pOfPhrase=NULL;
		  m_bArticle=FALSE;
		  m_bDefiniteArticle=FALSE;
		  m_bIndefiniteArticle=FALSE;
		  m_personalPronoun=0;
		  m_possessivePronoun=0;
	  };
	  ByAgentEnumerationElement(const CString & rStr)
	  { // für Eigennamen
		  m_strPlainText=rStr;
		  m_pOfPhrase=NULL;
	  }
  };

  class ByAgent
  {
  public:
	  std::vector<ByAgentEnumerationElement> m_enumerationElements;
  };

  /*class SentenceElement // SentenceElement muß textuell über den Subklassen
  { // stehen, alphabetische Ordnung nicht möglich
	  virtual virtFunc(){};
  };*/

  /*class Object:public SentenceElement
  {
	  public:
	  std::vector<EnumerationElement> m_enumerationElements;
  };*/


  class Subject:public SentenceElement
  {
  public:
	  std::vector<EnumerationElement> m_vecEnumerationElement;
	  BYTE m_personIndex;
	  Subject()
	  {
		  m_personIndex=0;
	  }
  #ifdef USE_LETTER_TREE
    std::vector<EnumerationElement *> m_vecpenumerationelement ;
	  ~Subject()
    {
      std::vector<EnumerationElement *>::iterator iter ;
      for(iter = m_vecpenumerationelement.begin() ; iter != 
        m_vecpenumerationelement.end() ; iter ++ )
        delete *iter ;
    }
    void AppendEnumEle()
    {
      //EnumerationElement enumele ;
      //TRACE("AppendEnumEle--address of obj to insert: %x\n", &enumele) ;
      //m_vecEnumerationElement.push_back(enumele) ;
      //Insert a pointer. When I inserted objects (a copy was 
      //created by push_back() ) there were problems.
      m_vecpenumerationelement.push_back(new EnumerationElement()) ;
      //m_vecEnumerationElement.push_back(EnumerationElement() ) ;
      //TODO: the person index may have changed due to 
      //additional enum elem.
      //if(m_personIndex == 
    }
  #endif//#ifdef USE_LETTER_TREE
  };

  class Place
  {
  public:
	  std::vector<EnumerationElement> m_vecEnumerationElement;
	  WordNode * m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated;
	  Place()
	  {
		  m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated=NULL;
	  }
  };

  class Gerund // Gerundium
  {
  public:
	  BOOL m_bActiveForm;
	  BOOL m_bNegation;
	  BYTE m_bTense;
	  BYTE m_bMode; // Modus: 1=normal,2=Passiv(being called),3=Perfekt(having persuaded)
	  CString m_strPredicate;
	  Object * m_pObject;
	  Place * m_pPlace;
	  Subject * m_pPersonalSubject;
	  WordNode * m_pWordNodeWhereAnEnglishVerbIsEncapsulated;
	  WordNode *	m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated;
	  Gerund()
	  {
		  m_bMode=0;
		  m_bActiveForm=TRUE;
		  m_bNegation=FALSE;
		  m_pObject=NULL;
		  m_pPersonalSubject=NULL;
		  m_pPlace=NULL;
		  m_bTense=1;
		  m_pWordNodeWhereAnEnglishVerbIsEncapsulated=NULL;
		  m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated=NULL;
	  }
  };

  class ToPlusInfinitive // "to" und direkt dahinter ein Infinitiv 
  { // zum Beispiel: "to open"
  public:
	  Object * m_pObject;
	  Place * m_pPlace;
	  BYTE m_bNegation;
	  WordNode * m_pWordNodeWhereAnEnglishVerbIsEncapsulated;
	  WordNode *	m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated;
	  ToPlusInfinitive()
	  {
		  m_bNegation=FALSE;
		  m_pObject=NULL;
		  m_pPlace=NULL;
		  m_pWordNodeWhereAnEnglishVerbIsEncapsulated=NULL;
		  m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated=NULL;
	  }
  };

  //Die Klasse "Predicate" gibt es, das es mehrere Möglichkeiten für 
  //Prädikate  in 1nem Haupt- oder Nebensatz geben kann: "The glasses are 
  //broken." -> "Die Brille ist kaputt." (Prädikat: "ist") und "Die Brille
  //wird zerbrochen." (Prädikat: "wird zerbrochen")
  //ein Prädikat hat - außer bei der einfachen Zukunft, denn bei der Zukunft
  //wird das "will" als Zeitform angesehen und gespeichert, und bei passiven
  //Sätzen ohne ein modales Hilfsverb - eine gebeugte 
  //Verbform ("gV",gebeugte Verbform im Deutschen, also keinen Infinitiv) 
  //und zusätzlich kann es noch einen 
  //Infinitiv oder Partizip Perfekt ("IPP") haben.
  //denn bei Kontruktionen, bei denen das Verb im Deutschen IMMER eindeutig
  //ist (Beispiel: das Passiv: im Deutschen immer eine gebeugte Verbform 
  //des Hilfsverbes "werden"), braucht man dafür nicht extra eine weitere 
  //Vokabel zu speichern (es sollen nur die NÖTIGSTEN Daten gespeichert 
  //werden;die gebeugte Verbform bezieht sich auf das deutsche Prädikat):
  //SIMPLE_PRESENT:
  //"I like you." gV: like IPP:keiner ("Ich mag dich.")
  //"I can like you." gV: can IPP: like ("Ich kann dich mögen.")
  //"I am liked." gv: keine IPP: liked ("Ich werde gemocht.")
  //"I can be liked." gV: can IPP: liked ("Ich kann gemocht werden.")
  //SIMPLE_PAST:
  //"I liked you." gV: liked IPP:keiner ("Ich mochte dich.")
  //"I was liked." gV: keine IPP: liked ("Ich wurde gemocht.")
  //FUTURE:
  //"I will like you." gV: keine IPP: like ("Ich werde dich mögen.")
  //"I will be liked." gV: keine IPP: liked ("Ich werde gemocht werden.")
  //FUTURE CONTINOUS:
  //"I will be walking." gV: keine IPP: walking ("Ich werde gelaufen sein.")
  //"I will be being liked." gV: keine IPP: liked ("Ich werde gemocht worden sein.")
  //FUTURE PERFECT:
  //"I will have liked you." gV: keine IPP: liked ("Ich werde dich gemocht haben.")
  //"I will have been liked." gV: keine IPP: liked ("Ich werde gemocht worden sein.")
  //PRESENT_PROGRESSIVE
  //"I am walking." gV: walking IPP: keins ("Ich laufe (gerade).")
  //"I am being liked." gV: keine IPP: liked ("Ich werde (gerade) gemocht.")
  //PAST_PROGRESSIVE
  //"I was walking." gV: walking IPP: keins ("Ich lief (gerade).")
  //"I was being liked." gV: keins IPP: liked ("Ich wurde (gerade) gemocht.")
  //PRESENT_PERFECT
  //"I have liked you." gV: keine IPP:liked ("Ich habe dich gemocht.")
  //"I can have seen you." gV: can IPP:seen ("Ich kann dich gesehen haben.")
  //"I have been liked." gv: keine IPP: liked ("Ich bin gemocht worden.")
  //"I can have been liked." gv: can IPP: liked ("Ich kann gemocht worden sein.")
  //PAST_PERFECT
  //"I had liked you." gV: keine IPP:liked ("Ich hatte dich gemocht.")
  //"I could have seen you." gV: could IPP:seen ("Ich konnte dich gesehen haben.")
  //"I had been liked." gv: keine IPP: liked ("Ich war gemocht worden.")
  //"I could have been liked." gv: could IPP: liked ("Ich konnte gemocht worden sein.")
  //PRESENT_PERFECT_PROGRESSIVE
  //"I have been liking you." gV: keine IPP:liking ("Ich habe dich (gerade) gemocht.")
  //"I can have been liking you." gV: can IPP: liking ("Ich kann dich (gerade) gemocht haben.")
  //"I was been liking." gv: keine IPP: liking ("Ich bin (gerade) gemocht worden.")
  //"I can have been liking." gV: can IPP: liking ("Ich kann (gerade) gemocht worden sein.")
  //PAST_PERFECT_PROGRESSIVE
  //"I had been liking you." gV: keine IPP:liking ("Ich hatte dich (gerade) gemocht.")
  //"I could have been liking you." gV: could IPP: liking ("Ich konnte dich (gerade) gemocht haben.")
  //"I had been liking." gv: keine IPP: liking ("Ich war (gerade) gemocht worden.")
  //"I could have been liking." gV: could IPP: liking ("Ich konnte (gerade) gemocht worden sein.")
  class Predicate
  {
  public:
	  //Verneinung (wenn ein "not" im Prädikat steht); 0: keine Verneinung
	  //sonst: Verneinung
	  BYTE m_bNegation;
	  //0: Passivsatz sonst: Aktivsatz
	  BYTE m_bActiveClause;
	  //die Zeitform des Prädikats von Wert "1" bis Wert "8"
	  //1: SIMPLE_PRESENT 
	  //2: SIMPLE_PAST
	  //3: FUTURE
	  //4: PRESENT_PROGRESSIVE
	  //5: PAST_PROGRESSIVE
	  //6: PRESENT_PERFECT
	  //7: PRESENT_PERFECT_PROGRESSIVE
	  //8: SIMPLE_PRESENT_OR_SIMPLE_PAST
	  BYTE m_bTense;
	  //ein WordNode-Zeiger, der ein englisches Verb, dessen deutsches Pendant
	  //eine gebeugte Verbform ist, die für eine korrekte Übersetzung notwendig 
	  //ist ("have" bei "I have been." ist zum Beispiel NICHT notwendig, da 
	  //man durch die Zeitform "Perfekt" weiss, dass man eine gebeugte Verbform 
	  //von "haben" im Deutschen dafür braucht), kapselt
	  WordNode * m_pWordNodeWhereTheGermanTranslationIsABendVerb;
	  //ein WordNode-Zeiger, der ein englisches Verb, dessen deutsches Pendant
	  //KEINE gebeugte Verbform ist, die für eine korrekte Übersetzung notwendig 
	  //ist ("being" bei "I am being used." ist zum Beispiel NICHT notwendig, da 
	  //man durch das Attribut "Aktivsatz" weiss, dass man eine gebeugte Verbform 
	  //von "werden" im Deutschen dafür braucht), kapselt
	  WordNode * m_pWordNodeWhereTheGermanTranslationIsANonBendVerb;
	  //WordNode * m_pWordNodeWhereAnEnglishVerbIsEncapsulated;
	  //WordNode * m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated;
	  //englisches Adverb der Häufigkeit
	  WordNode * m_pWordNodeWhereAnEnglishAdverbIsEncapsulated;
	  WordNode * m_pWordNodeWhereAnEnglishAdverbDescribingAVerbIsEncapsulated;
  #ifdef USE_LETTER_TREE
    //Gebeugtes Verb (nur 1 Verb kann gebeugt sein; auch wenn bei 3. Pers. Singular
    //kein "s" angehängt, kann es trotzdem gebeugt sein, nämlich bei modalen
    //Hilfsverben; es muss 1 gebeugtes Verb
    //in einem gültigen Predikat geben).
    VocabularyAndTranslation * m_pvocabularyandtranslationInflectedVerb ;
    VocabularyAndTranslation * m_pvocabularyandtranslationNotInflectedVerb ;
    //Store conjection for the possibility of enumerated predicats, e.g.
    //"He NEITHER runs NOR jumps." "He walks AND runs.",
    //"He runs, jumps or walks and goes."
    VocabularyAndTranslation * m_pvocabularyandtranslationConjection ;
  #endif//#ifdef USE_LETTER_TREE
	  Predicate()
      //C++ style initialisations:
      : m_pvocabularyandtranslationConjection(NULL)
	  {
      m_bActiveClause=1;
		  m_bTense=0;
		  m_bNegation=0;
		  m_pWordNodeWhereAnEnglishAdverbIsEncapsulated=NULL;
		  //m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated=NULL;
		  //m_pWordNodeWhereAnEnglishVerbIsEncapsulated=NULL;
		  m_pWordNodeWhereTheGermanTranslationIsABendVerb=NULL;
		  m_pWordNodeWhereTheGermanTranslationIsANonBendVerb=NULL;
		  m_pWordNodeWhereAnEnglishAdverbDescribingAVerbIsEncapsulated=NULL;
  #ifdef USE_LETTER_TREE
      m_pvocabularyandtranslationInflectedVerb = NULL ;
      m_pvocabularyandtranslationNotInflectedVerb = NULL ;
      m_pvocabularyandtranslationConjection = NULL ;
  #endif//#ifdef USE_LETTER_TREE
	  }
  #ifdef USE_LETTER_TREE
    bool IsValid()
    {
      if(m_pvocabularyandtranslationInflectedVerb )
        return true;
      return false ;
    }
    BYTE GetNumberOfNeededObjects()
    {
      BYTE byNumberOfNeededObjects = 255 ;
      if(m_pvocabularyandtranslationNotInflectedVerb)
        byNumberOfNeededObjects = 
          m_pvocabularyandtranslationNotInflectedVerb->
          GetNumberOfNeededObjects() ;
      else
        byNumberOfNeededObjects = 
          m_pvocabularyandtranslationInflectedVerb->
          GetNumberOfNeededObjects() ;
      return byNumberOfNeededObjects ;
    }
  #endif//#ifdef USE_LETTER_TREE
  };

  class RelativeClause:public SentenceElement
  {
  public:
	  BYTE m_bRefersToSubject;
	  BYTE m_bBe;
	  BYTE m_bNegation;
	  BYTE m_bWordOrder;
	  BYTE m_bTense;
	  CString m_strPredicate;
	  Object m_Object;
	  Predicate m_Predicate;
	  Subject m_Subject;
	  std::vector<EnglishAdverb> m_vecEnglishAdverb;
	  std::vector<MiddleEnumerationElement> m_vecMiddleEnumerationElement;
	  RelativeClause()
	  {
		  m_bRefersToSubject=TRUE;
		  m_bBe=FALSE;
		  m_bNegation=FALSE;
		  m_bWordOrder=0;
		  //m_subject=NULL;
		  //m_object=NULL;
	  };
  };

  #define CLAUSE_WITH_PROPER_NAME 1
  #define CLAUSE_WITHOUT_PROPER_NAME 2
  #define CLAUSE_EXCLUSIVELY_CONTAINS_KNOWN_WORDS 1
  #define CLAUSE_CONTAINS_AT_LEAST_1_UNKNOWN_WORD 2
  //ein "clause" ist ein Teilsatz wie Haupt- oder Nebensatz 
  //(Nebensatz OHNE Relativsatz)
  //Also ein nicht-zusammengesetzter Satz.
  class Clause:public SentenceElement
  {
  public:
	  BYTE m_bBe;
	  BYTE m_bNegation;
	  BYTE m_bActiveClause; //Aktiv- oder Passivsatz
	  BYTE m_bAdverbOfTimeAtTheBeginning;
	  BYTE m_bWordOrder; // deutscher Satzbau, kann SPO, SOP oder POS sein
	  //BYTE satzbau;
	  BYTE m_bTense; //Zeitform
	  ByAgent * m_pByAgent;
	  CString m_strPredicate;
	  Gerund * m_pGerund;
	  Place * m_pPlace;
	  Object * m_pObjectFirst; //1tes mögliches Objekt
    //2tes mögliches Objekt (nur bei bestimmten Verben, wie "to give": 
    //"I gave him a book." : "him"=1tes Objekt, "a book" = 2tes Objekt
	  Object * m_pObjectSecond; 
	  Manner * m_pManner;
    //TODO remove--a relative clause is related to a subject (enumeration
    //element) or object (enumeration element), and not to a whole clause
	  RelativeClause * m_pRelativeClause;
	  std::vector<EnglishAdverb> m_vecEnglishAdverb;
	  std::vector<MiddleEnumerationElement> m_vecMiddleEnumerationElement;
	  std::vector<Predicate> m_vecPredicate;
  #ifdef USE_LETTER_TREE
    std::vector<Predicate *> m_vecppredicate;
  #endif
	  std::vector<WordNode *> m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated;
	  Subject * m_pSubject;
	  Subject * m_pPersonalSubject;
	  WordNode * m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated;
	  WordNode * m_pWordNodeWhereAnEnglishVerbIsEncapsulated;
	  WordNode * m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated;
	  WordNode * m_pWordNodeWhereAnEnglishPredicateIsEncapsulated;
	  WordNode * m_pWordNodeWhereAnEnglishQuestionWordIsEncapsulated;
	  WordNode * m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated;
	  WordNode * m_pWordNodeWhereAnEnglishAdverbIsEncapsulated;
	  Clause()
	  {
		  m_bBe=FALSE;
		  m_bNegation=FALSE;
		  m_bAdverbOfTimeAtTheBeginning=TRUE;
		  m_bActiveClause=TRUE;
		  m_pByAgent=NULL;
		  m_pGerund=NULL;
		  m_pManner=NULL;
		  m_pObjectFirst=NULL;
		  m_pObjectSecond=NULL;
		  m_pPersonalSubject=NULL;
		  m_pPlace=NULL;
		  m_pSubject=NULL;
		  m_pWordNodeWhereAnEnglishAdverbIsEncapsulated=NULL;
		  m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=NULL;
		  m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated=NULL;
		  m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated=NULL;
		  m_pWordNodeWhereAnEnglishPredicateIsEncapsulated=NULL;
		  m_pWordNodeWhereAnEnglishQuestionWordIsEncapsulated=NULL;
		  m_pWordNodeWhereAnEnglishVerbIsEncapsulated=NULL;
		  m_pRelativeClause=NULL;
		  m_bWordOrder=0;
		  //m_subject=NULL;
		  //m_object=NULL;
	  };
    BYTE 
      //Soll ohne Subjekt und Objekt parsen, damit diese Funktion für Relativ-Sätze
      //(The man you liked yesterday: Rel-Satz: "you liked yesterday" und für Frage-Sätze, die nach einem
      //Objekt fragen(Who likes you on the table?), benutzt werden kann.
      ParseClauseWithoutSubjectAndObject(
      Clause & subsentence,
	    const PositionCStringVector & psv,std::vector<Range> & rVecRange,
	    const DWORD & dwFlags//,BYTE bContainsFirstTokenOfSentence
      );
    BYTE ParseQuestionClause2(const PositionCStringVector & psv,
  	  std::vector<Range> & rVecRange,const DWORD & dwFlags);

  #ifdef USE_LETTER_TREE
    EnumerationElement * AddEnumEleToSubjOrObj(//bool bPredicateOccured
      )
    {
      EnumerationElement * pee = NULL;
      //if(bPredicateOccured)
      //if(m_vecPredicate.size() > 0)
      if(m_vecppredicate.size() > 0)
      {
        if(! m_pObjectFirst)
          m_pObjectFirst = new Object();
        //m_pObject->m_vecEnumerationElement.push_back() ;
        m_pObjectFirst->AppendEnumEle() ;
        //pee = & m_pObjectFirst->m_vecEnumerationElement.back() ;
        pee = m_pObjectFirst->m_vecpenumerationelement.back() ;
      }
      else
      //Word order is "SPO";->no predicate yet->add enum ele to subject.
      {
        if(! m_pSubject)
          m_pSubject = new Subject() ;
        //EnumerationElement enumele ;
        //m_pSubject->m_vecEnumerationElement.push_back(enumele) ;
        m_pSubject->AppendEnumEle() ;
        //pee = & m_pSubject->m_vecEnumerationElement.back() ;
        pee = m_pSubject->m_vecpenumerationelement.back() ;
      }
      return pee;
    }
  #endif//#ifdef USE_LETTER_TREE
  };//class Clause

  class ClauseWithoutSubject:public SentenceElement
  {
	  public:
	  Object m_Object;
	  std::vector<EnglishAdverb> m_vecEnglishAdverb;
	  std::vector<MiddleEnumerationElement> m_vecMiddleEnumerationElements;
	  CString m_strPredicate;
	  BYTE m_bTense;
	  BYTE m_bBe;
	  BYTE m_bNegation;
	  BYTE m_bWordOrder;
	  BYTE m_personIndex;
	  ClauseWithoutSubject()
	  {
		  m_bBe=FALSE;
		  m_bNegation=FALSE;
		  m_bWordOrder=0;
		  m_personIndex=0;
		  //m_subject=NULL;
		  //m_object=NULL;
	  };
  };

  class Sentence
  {
	  public:
	  BYTE m_bWordOrder;
	  std::vector<Clause> m_vecClause;
	  std::vector<CString> m_vecConjunction;
	  std::vector<EnglishAdverb> m_vecEnglishAdverb;
	  std::vector<SentenceElement *> m_vecSentenceElement;
    void AddClause(Clause & clause)
    {
      m_vecClause.push_back(clause);
    }
  };

  class Question
  {
  public:
	  std::vector<Clause> m_vecClause;
	  std::vector<CString> m_vecConjunction;
	  std::vector<EnglishAdverb> m_vecEnglishAdverb;
	  std::vector<SentenceElement *> m_vecSentenceElement;
	  BYTE m_bWordOrder;
	  BYTE m_bTense;
  };

#endif //!defined (SENTENCEELEMENT)
