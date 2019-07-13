/** GermanWord.cpp
 *  Created on: 31.07.2011
 *      Author: Stefan */
#include <vector>
#include <VocabularyInMainMem/VocabularyAndTranslation.hpp>

#include "GermanWord.hpp"

GermanWord GermanWord::GetGermanWord(VocabularyAndTranslation * p)
{
  GCC_DIAG_OFF(switch)
  switch(p->m_englishWordClass)
  {
    case EnglishWord::adjective :
      return GermanAdjective(p);
  }
  GCC_DIAG_ON(switch)
  return GermanWord();
}

std::vector<std::string> GermanWord::GetInflectionForms(VocabularyAndTranslation * p)
{
  GCC_DIAG_OFF(switch)
//  EnglishWord::English_word_class = p->m_englishWordClass;
  switch(p->m_englishWordClass)
  {
    case EnglishWord::singular :
    case EnglishWord::plural_noun :
    case EnglishWord::noun :
      return GermanNoun().GetInflectionForms(p);
    case EnglishWord::adjective_positiveForm :
      return GermanAdjective().GetInflectionForms(p);
      
    /** These word classes have no inflection */
    case EnglishWord::mainVerbPastParticiple0Obj :
    case EnglishWord::mainVerbPastParticiple1Obj :
    case EnglishWord::mainVerbPastParticiple2Obj :
    case EnglishWord::adverb :
    case EnglishWord::pronoun :
    case EnglishWord::preposition :
      if(p->m_arstrGermanWord)
      {
        std::vector<std::string> v;
        v.push_back(p->m_arstrGermanWord[0]);
        return v;
      }
  }
  GCC_DIAG_ON(switch)
  return std::vector<std::string>();
}
  
/** https://de.wikipedia.org/wiki/Germanisches_schwaches_Verb :
*  "Die schwachen Verben sind in den heutigen germanischen Sprachen weitaus 
 *  zahlreicher als die starken. Im Gegensatz zu letzteren sind sie außerdem 
 * noch produktiv, d. h. für Neubildungen geeignet.*/

/** see https://de.wikipedia.org/wiki/Regelm%C3%A4%C3%9Figes_Verb
* "Die Anzahl der regelmäßigen Verben übertrifft diejenige der unregelmäßigen 
* deutlich. Neubildungen gehen fast immer als regelmäßige Verben in die 
* deutsche Sprache ein (vgl. surfen, bomben, telefonieren, röntgen usw.). 
* Trotz ihrer quantitativen Überlegenheit sind sie in Texten seltener 
* anzutreffen als starke Verben, denn gerade viele der häufig gebrauchten 
* Verben sind stark." */

/** Definitions of static variables. */
const char * const GermanVerb::presentPersonEndings [] = {
  "e" //gehE
  , "st" //gehST
  , "t" //er gehT
  , "en" //wir gehEN
  , "t" //ihr gehT
  , "en"//sie gehEN
  };
const char * const GermanVerb::s_irregularVerbPresentSingularPersonEndings [] = {
  "" //kann
  , "st" //kannST
  , "" //er kann
  };

/** For verbs with word stem ending with an "e", e.g. word stem "arbeite" for
 *  infinitive "arbeiten" */
const char * const GermanVerb::presentPersonEndings2 [] = {
  "" //ich arbeite
  , "st" //du arbeiteST
  , "t" //er arbeiteT
  , "n" //wir arbeiteN
  , "t" //ihr arbeiteT
  , "en" //wir arbeiteN
  };
const char * const GermanVerb::pastPersonEndings [] = {
  "" //ging, arbeitete
  , "st" //gingST, arbeiteteST
  , "" //er ging, arbeitete
  , "en" //gingEN, arbeitetEN
  , "t" //ihr gingT, arbeiteteT
  , "en" //sie gingEN, arbeitetEN
  };

GermanAdjective::GermanAdjective(VocabularyAndTranslation * p)
{ 
  m_bIntegral = FALSE;
  if(p->m_arstrGermanWord )
  {
    m_strPositiv = p->m_arstrGermanWord[0];
    m_strComperativ = p->m_arstrGermanWord[1];
    m_strSuperlativ = p->m_arstrGermanWord[2];
  }
}

/** @param str: positive, comperative or superlative */
std::vector<std::string> GermanAdjective::GetInflectionForms(const std::string & str)
{
  //TODO are all cases included? (genitive, ...)
  std::vector<std::string> inflectionForms;
  inflectionForms.push_back(str); 
  
  /** ------ nominative , accusative ---*/
  /** Der hohE Mann. (ich mag) die hohE Frau. Das hohE Kind. Eine hohE Frau. */
  inflectionForms.push_back(str + "e");
  /** Ein hohER Mann. */
  inflectionForms.push_back(str + "er"); 
  /** Ein hohES Kind. */
  inflectionForms.push_back(str + "es");
  
  /** ------ genitive , dative, accusative ---*/
  /** (ich vertraue/mag) des/dem/den hohEN Mann(es) 
   *  (ich vertraue/mag)  der hohEN Frau
   *  (ich vertraue/mag) des/dem hohEN Kind(es) */
  inflectionForms.push_back(str + "en");
  return inflectionForms;
}

std::vector<std::string> GermanAdjective::GetInflectionForms()
{
  std::vector<std::string> inflectionForms;

  std::vector<std::string> positiveInflectionForms = GetInflectionForms(
    /*Adjective::positive,*/ m_strPositiv);
  
  inflectionForms.insert( inflectionForms.end(), 
    positiveInflectionForms.begin(), positiveInflectionForms.end());
  
  inflectionForms.push_back(m_strComperativ);
  /** Der höherE Mann. Die höherE Frau. Das höherE Kind. */
  inflectionForms.push_back(m_strComperativ + "e");
  /** Ein höherER Mann. */
  inflectionForms.push_back(m_strComperativ + "er"); 
  /** Ein höherES Kind. */
  inflectionForms.push_back(m_strComperativ + "es");
    
  inflectionForms.push_back(m_strSuperlativ);
  /** Der höchstE Mann. Die höchstE Frau. Das höchstE Kind. */
  inflectionForms.push_back(m_strSuperlativ + "e");
  /** Ein höchstER Mann. */
  inflectionForms.push_back(m_strSuperlativ + "er"); 
  /** Ein höchstES Kind. */
  inflectionForms.push_back(m_strSuperlativ + "es");
  return inflectionForms;
}  

std::vector<std::string> GermanAdjective::GetInflectionForms(VocabularyAndTranslation * p)
{
  if(p->m_arstrGermanWord)
    GCC_DIAG_OFF(switch)
    switch(p->m_englishWordClass)
    {
      case EnglishWord::adjective_positiveForm :
//      case EnglishWord::adjective_comperativeForm :
//      case EnglishWord::adjective_superlativeForm :
        return GermanAdjective().GetInflectionForms(p->m_arstrGermanWord[0]);
    }
    GCC_DIAG_ON(switch)
  return std::vector<std::string>();
}

GermanAuxiliaryVerb::GermanAuxiliaryVerb(const VTrans::string_type & str)
{
  WORD i=0;
  int start=0;
  WORD wIndex=0;
  m_bIntegral=TRUE;
  while( i < str.length() )
  {
    if(str[i]=='\n')
    {
      m_strWords[wIndex++] = //str.Mid(start,i-start);
        str.substr( start ,i - start );
      start = i + 1;
    }
    i++;
  }
}

bool GermanAuxiliaryVerb::GetNextString( std::string & r_stdstr )
{
  bool bSucc = false ;
  if( m_byIndex < 15 )
  {
    r_stdstr = m_strWords [ m_byIndex ++ ] ;
    bSucc = true ;
  }
  return bSucc ;
}

std::vector<std::string> GermanNoun::GetInflectionForms(const std::string & str)
{
  std::vector<std::string> inflectionForms;
  /** Accusative. */
  inflectionForms.push_back(str);  
  
  if( str.size() > 0 )
  {
    std::string strEnding = str.substr(str.size() - 1 );
    char lastChar = strEnding[0];
    /** On vowel end no "s" is appended?!: "der Schal der Frau" */
//    if( IsConsonant(lastChar) )
    {
      switch( lastChar )
      {
          /** see http://mein-deutschbuch.de/genitiv.html:
          * "Nomen im Plural sowie alle feminine Nomen bleiben im Genitiv unverändert." */
          case 's' :
            /** Haus -> Hauses. */
            /** genitive form: des MannES. */
            inflectionForms.push_back(str + "es");
            break;
          case 'r' :
            /** genitive form: des HerrN.NachbarN */
            inflectionForms.push_back(str + "n");
            break;
        default:
          inflectionForms.push_back(str + "s");          
      }
    }
  }
  return inflectionForms;
}

std::vector<std::string> GermanNoun::GetInflectionForms(VocabularyAndTranslation * p)
{
  std::vector<std::string> vec;
  if(p->m_arstrGermanWord)
  {
    vec.push_back(p->m_arstrGermanWord[0]);
    GCC_DIAG_OFF(switch)
    switch(p->m_englishWordClass)
    {
      case EnglishWord::noun :
      case EnglishWord::singular :
        fastestUnsignedDataType article = p->m_arbyAttribute[0];
        /** see http://mein-deutschbuch.de/genitiv.html:
        *  "Nomen im Plural sowie alle feminine Nomen bleiben im Genitiv unverändert." */
        if( article == der || article == das )
        {
          std::vector<std::string> inflectionForms = GermanNoun().GetInflectionForms(p->m_arstrGermanWord[0]);
#ifdef _DEBUG
          const int numInflectionForms = inflectionForms.size();
#endif
          vec.insert(vec.end(), inflectionForms.begin(), inflectionForms.end());
        }
    }
    GCC_DIAG_ON(switch)
  }
  return vec;
}

void GermanVerb::CreateFromString(const VTrans::string_type & str)
{
  WORD i=0;
  int start=0;
  WORD wIndex=0;
  m_bIntegral=TRUE;
  while( i < str.length() )
  {
    if( wIndex == 14 )
      m_bCase = str[i] - 50 ;
    if( str[i] == '\n' )
    {
      m_strWords[wIndex++] = //str.Mid(start,i-start);
        str.substr(start,i-start);
      start=i+1;
    }
    i++;
  }
  m_bAuxiliaryVerb=TRUE;
  m_bReflexive=FALSE;
}

GermanVerb::GermanVerb(const VTrans::string_type & str)
{
  CreateFromString(str) ;
}

GermanVerb::GermanVerb(const VTrans::string_type & str , e_case cas)
{
  CreateFromString(str) ;
  m_bCase = cas ;
}
