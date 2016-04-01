#ifndef __ANDROID__
//TODO does this include make sense?:
  #include <bits/stringfwd.h>
#endif

#include "IVocabularyInMainMem.hpp"
#include "Controller/thread_type.hpp"
#include <Attributes/Word.hpp>
#include <IO/dictionary/DictionaryFileAccessException.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_DEBUG
#include <Controller/TranslationControllerBase.hpp>

extern TranslationControllerBase * g_p_translationcontrollerbase;

//IVocabularyInMainMem::s_arstdstrPersonalPronoun = {"hh", "j"} ;
//
//void IVocabularyInMainMem::InsertPersonalPronouns()
//{
//  Insert("I", personal_pronoun ) ;
//  Insert("you", personal_pronoun ) ;
//  Insert("he", personal_pronoun ) ;
//  Insert("she", personal_pronoun ) ;
//  Insert("it", personal_pronoun ) ;
//  Insert("we", personal_pronoun ) ;
//  Insert("you", personal_pronoun ) ;
//  Insert("they", personal_pronoun ) ;
//}

IVocabularyInMainMem::IVocabularyInMainMem()
  : m_numberOfVocPairs(0)
  , m_numberOfEnglishWords(0)
  , m_maxTokenToConsider(4) //"hand-held vacuum cleaner"
{
  //m_arstdstrPersonalPronoun = new std::string[NUMBER_OF_PERSONAL_PRONOUNS] ;
}

//IVocabularyInMainMem::~IVocabularyInMainMem()
//{
//  //delete [] m_arstdstrPersonalPronoun ;
//}

void IVocabularyInMainMem::InsertAuxiliaryVerbBe()
{
  GermanAuxiliaryVerb germanverbSein (
    "sein\nbin\nbist\nist\nsind\nseid\nsind\nwar\n"
    "warst\nwar\nwaren\nward\nwaren\ngewesen\n"//, GermanVerb::nominative
      );
  EnglishAuxiliaryVerb englishauxiliaryverbBe(
    "be\nam\nare\nis\nare\nare\nare\nwas\nwere\n"
    "was\nwere\nwere\nwere\nbeen\n" , false );

//  germanverbSein.m_bCase = GermanVerb::nominative ;
//  germanverbSein.m_bMove = 0 ;

  Insert("am", EnglishWord::am) ;
  Insert("are", EnglishWord::are) ;
  Insert("is", EnglishWord::is) ;

//  Insert(englishauxiliaryverbBe,germanverbSein, p_v) ;
}

void IVocabularyInMainMem::InsertAuxiliaryVerbHave()
{
  GermanAuxiliaryVerb germanverbHaben ("haben\nhabe\nhast\nhat\nhaben\nhabt\nhaben\n"
    "hatte\nhattest\nhatte\nhatten\nhattet\nhatten\ngehabt\n"//,
    //GermanVerb::nominative
      );
  EnglishAuxiliaryVerb englishauxiliaryverbHave(
    "have\nhave\nhave\nhas\nhave\nhave\nhave\nhad\nhad\nhad\nhad\nhad\nhad\n"
    "had\n" , false );

//  germanverbHaben.m_bCase = GermanVerb::nominative ;
//  germanverbHaben.m_bMove = 0 ;

    Insert("have", EnglishWord::/*haveInfinitive*/ auxiliary_verb
      );//, p_v) ;
//  VocabularyAndTranslation * p_vocabularyandtranslation =
//      (VocabularyAndTranslation *) p_v;
//  p_vocabularyandtranslation->m_byType = EnglishWord::haveInfinitive;
//  //void * p_v =
//  //TODO
//    Insert(englishauxiliaryverbHave, germanverbHaben, //p_v
//      p_vocabularyandtranslation) ;
}

void IVocabularyInMainMem::InsertAuxiliaryVerbWill()
{
  GermanAuxiliaryVerb germanverbWerden ("werden\nwerde\nwirst\nwird\nwerden\nwerdet\nwerden\n"
    "wurde\nwurdest\nwurde\nwurden\nwurdet\nwurden\ngeworden\n"//,
    //GermanVerb::nominative
      );
  EnglishAuxiliaryVerb englishauxiliaryverbWill(
    "will\nwill\nwill\nwill\nwill\nwill\nwould\nwould\nwould\nwould\nwould\nwould\nwould\n"
    "would\n" , false );

//  germanverbHaben.m_bCase = GermanVerb::nominative ;
//  germanverbHaben.m_bMove = 0 ;

  /*VocabularyAndTranslation * p_vocabularyandtranslation = */Insert("will",
    EnglishWord:://haveInfinitive
    auxiliary_verb
    );//, p_v) ;
//  VocabularyAndTranslation * p_vocabularyandtranslation =
//      (VocabularyAndTranslation *) p_v;
  //p_vocabularyandtranslation->m_byType = EnglishWord::will;

  //void * p_v =
  //TODO
//    Insert(englishauxiliaryverbWill, germanverbWerden, //p_v
//      p_vocabularyandtranslation) ;
}

//Insert the words that are fundamental and thus needn't/ shouldn't stand in
//the dictionary.
void IVocabularyInMainMem::InsertFundamentalWords()
{
//  IVocabularyInMainMem::voc_container_type * p_voc_container = NULL;
//  VocabularyAndTranslation * p_vocabularyandtranslation = NULL;
  Insert("unknownWord", EnglishWord::UnknownWord /*NULL*/ );
  //Inserts e.g. into a trie structure.
  Insert("the", EnglishWord::English_definite_article ) ;
  Insert("a", EnglishWord::English_indefinite_article) ;
  Insert("an", EnglishWord::English_indefinite_article) ;

  Insert( "I" , EnglishWord::personal_pronoun_I) ;
//  Insert( "you" , EnglishWord::personal_pronoun_you_sing ) ;
  Insert( "you" , EnglishWord::personal_pronoun_you) ;
  Insert("he", EnglishWord::personal_pronoun_he) ;
  Insert("she", EnglishWord::personal_pronoun_she) ;
  Insert("it", EnglishWord::personal_pronoun_it) ;
  Insert( "we" , EnglishWord::personal_pronoun_we) ;
  Insert( "you" , EnglishWord::personal_pronoun_you_plur) ;
  Insert( "they" , EnglishWord::personal_pronoun_they) ;

  Insert( "myself" , EnglishWord::reflexive_pronoun_myself) ;
  Insert( "yourself" , EnglishWord::reflexive_pronoun_yourself) ;
  Insert("himself", EnglishWord::reflexive_pronoun_himself) ;
  Insert("herself", EnglishWord::reflexive_pronoun_herself) ;
  Insert("itself", EnglishWord::reflexive_pronoun_itself) ;
  Insert( "ourselves" , EnglishWord::reflexive_pronoun_ourselves) ;
  Insert( "yourselves" , EnglishWord::reflexive_pronoun_yourselves) ;
  Insert( "themselves" , EnglishWord::reflexive_pronoun_themselves) ;

//  Insert( "have" , EnglishWord::have ) ;

  //Because "and" is a special conjunction that changes the "person index" of
  // a subject/ object
  Insert("and", EnglishWord::conjunction_and) ;

  InsertAuxiliaryVerbBe();

  InsertAuxiliaryVerbHave();
  InsertAuxiliaryVerbWill();

	InsertPersonalPronouns() ;
	//"her","him","me",...
  InsertPersonalPronounsObjectiveForm() ;

//  Insert("a", English_indefinite_article ) ;
//  Insert(",", comma ) ;
}

void IVocabularyInMainMem::OutputVocs(const voc_container_type & voc_container)
{
  LOGN_DEBUG("begin")
  voc_container_type::const_iterator c_iter_voc_container = voc_container.
    begin();
  VocabularyAndTranslation * p_vocabularyandtranslation;
  while( c_iter_voc_container != voc_container.end() )
  {
    p_vocabularyandtranslation = * c_iter_voc_container;
    if( p_vocabularyandtranslation)
    {
      //TODO solve problem outputting under Android
#ifndef __ANDROID__
      std::ostringstream oss;
      oss << * p_vocabularyandtranslation;
      LOGN_DEBUG( oss.str() )
#endif
    }
    ++ c_iter_voc_container;
  }
  LOGN_DEBUG("end")
}

void IVocabularyInMainMem::OutputVocs(const voc_container_type * p_voc_container)
{
  LOGN_DEBUG("begin--pointer to voc container :" << p_voc_container)
  if(p_voc_container)
    OutputVocs(* p_voc_container);
//  LOGN_DEBUG("end")
}

DWORD THREAD_FUNCTION_CALLING_CONVENTION CollectDictStatsThreadFunc(void * p_v)
{
  I_Thread::SetCurrentThreadName("CollectDictStats");
  try
  {
	  CollectDictStatsThreadFuncParams * p_collectDictStatsThreadFuncParams =
		(CollectDictStatsThreadFuncParams *) p_v;
	  if( p_collectDictStatsThreadFuncParams)
	  {
		IVocabularyInMainMem * pVocabularyInMainMem =
		  p_collectDictStatsThreadFuncParams->pIVocabularyInMainMem;
		pVocabularyInMainMem->GetStatistics(
		  p_collectDictStatsThreadFuncParams->englishWordClass2CounterMap);
	  }
  }catch(DictionaryFileAccessException & dictionaryFileAccessException ) {
    g_p_translationcontrollerbase->Message(dictionaryFileAccessException.GetErrorMessageA() );
  }
}

void IVocabularyInMainMem::GetStatisticsInSeparateThread(
  /*const*/ CollectDictStatsThreadFuncParams & collectDictStatsThreadFuncParams,
  VTrans::thread_type & thread)
{
//  VTrans::thread_type thread;
  collectDictStatsThreadFuncParams.pIVocabularyInMainMem = this;
  thread.start(CollectDictStatsThreadFunc, & collectDictStatsThreadFuncParams);
}