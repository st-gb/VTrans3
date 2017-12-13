/*
 * CharStringMap.cpp
 *
 *  Created on: 19.01.2013
 *      Author: Stefan
 */

#include "VocabularyInMainMem/CharStringStdMap/CharStringStdMap.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
//class VocabularyAndTranslation
#include <VocabularyInMainMem/VocabularyAndTranslation.hpp>

//Class variable definition (because it's a global variable):
VocablesForWord * CharStringStdMap::s_p_vocablesforwordLastForInsertedWord;
VocabularyAndTranslation * CharStringStdMap::s_p_vocabularyandtranslation = NULL;

void VocablesForWord::insert(
  VocabularyAndTranslation * const pvocabularyandtranslation)
{
  //if(m_psetvocabularyandtranslation)
    //m_psetvocabularyandtranslation->insert(*pvocabularyandtranslation);
//    if(m_p_setpvocabularyandtranslation)
//      m_p_setpvocabularyandtranslation->insert(pvocabularyandtranslation);
#ifdef _DEBUG
  voc_container_type * p_voc_container_type =
    & m_std_set_p_vocabularyandtranslation;
#endif
  m_std_set_p_vocabularyandtranslation.insert(pvocabularyandtranslation);
}

std::ostream & operator << (std::ostream & std_os,
  const VocablesForWord::voc_container_type & voc_container)
{
  VocablesForWord::voc_container_type::const_iterator c_iter =
    voc_container.begin();
  VocabularyAndTranslation::word_type const * p_word;
  while( c_iter != voc_container.end() )
  {
    p_word = (*c_iter)->m_arstrEnglishWord;
    if( p_word && p_word[0] )
    {
      std_os << p_word[0];
    }
  }
  return std_os;
}

CharStringStdMap::CharStringStdMap()
{
  // TODO Auto-generated constructor stub
}

CharStringStdMap::~CharStringStdMap()
{
  clear();
}

void CharStringStdMap::AddVocabularyAttributes(
  enum EnglishWord::English_word_class word_class,
  void * p_v)
{
  VocablesForWord * p_vocablesforword = (VocablesForWord *) p_v;
  if( p_vocablesforword)
  {
//    if( ! p_vocablesforword->m_p_setpvocabularyandtranslation )
//      p_vocablesforword->m_p_setpvocabularyandtranslation = new
//      VocablesForWord::voc_container_type;
//    if( p_vocablesforword->m_p_setpvocabularyandtranslation)
//      p_vocablesforword->m_p_setpvocabularyandtranslation->insert( new
//        VocabularyAndTranslation(word_class) );
    p_vocablesforword->insert( new VocabularyAndTranslation(word_class) );
  }
}

void CharStringStdMap::clear()
{
//  LOGN_DEBUG("begin")
  //  VocabularyAndTranslation * p_c_vocabularyandtranslation;
  VocabularyAndTranslation * p_vocabularyAndTranslation;
  /** Loop over all different words (each word may contain homographs) */
  for( map_type::const_iterator c_iter_map = m_charStringMap.begin() ;
      c_iter_map != m_charStringMap.end() ;
      ++ c_iter_map )
  {
#ifdef _DEBUG
    const std::string & st = c_iter_map->first;
#endif
    DEBUGN( "freeing memory for dictionary word \"" << c_iter_map->first << "\"")
    /** Loop over homographs for the same word/tokens. */
    for( /** "const_iterator" for Android NDK. */
      std::set<VocabularyAndTranslation *>::const_iterator
      iter_p_vocabularyandtranslation =
        c_iter_map->second.m_std_set_p_vocabularyandtranslation.begin() ;
      iter_p_vocabularyandtranslation !=
      c_iter_map->second.m_std_set_p_vocabularyandtranslation.end() ;
      ++ iter_p_vocabularyandtranslation )
    {
#ifdef _DEBUG
      VocabularyAndTranslation * p_vocabularyAndTranslation =
        * iter_p_vocabularyandtranslation;
      delete p_vocabularyAndTranslation;
#else
      delete * iter_p_vocabularyandtranslation;
#endif
//      -- ::wxGetApp().s_numberOfVocabularyPairs;
//      -- m_numberOfVocPairs;
//      -- m_numberOfEnglishWords;
      -- m_numberOfVocPairs;
#ifdef _DEBUG
      const fastestUnsignedDataType numberOfNonFundamentalWords = 
        m_numberOfEnglishWords - m_numberOfFundamentalWords;
      if( /*numberOfNonFundamentalWords*/ m_numberOfVocPairs < 0)
        LOGN_ERROR("# of words < 0")
#endif
    }
  }
  m_charStringMap.clear();
  map_type::size_type st = m_charStringMap.size();
  LOGN_DEBUG("end--map size:" << st)
}

/*void * */ VocablesForWord::voc_container_type * CharStringStdMap::find(
    const PositionStringVector & psv,
//      const PositionstdstringVector & psv,
    DWORD & r_dwTokenIndex
    )
{
//  VocablesForWord * p_vocablesforword = NULL;
  VocablesForWord::voc_container_type * p_voc_container_type = NULL;
  VTrans::string_type word;
  const int numTokens = psv.size();
  map_type::const_iterator c_iter, c_iterMaxWordMatch = m_charStringMap.end();
  unsigned tokenIndex = r_dwTokenIndex;
  const unsigned beginTokenIndex = r_dwTokenIndex;

  word = psv.at(tokenIndex).m_Str;
  c_iter = m_charStringMap.find( word );
  if( c_iter != m_charStringMap.end() )
    c_iterMaxWordMatch = c_iter;
  ++ tokenIndex;
  if( tokenIndex < numTokens )
  {
    do
    {
      word += " ";
      word += psv.at(tokenIndex).m_Str;
      c_iter = m_charStringMap.find( word );
      if( c_iter != m_charStringMap.end() )
      {
        //          //Do not break here--"hand-held vacuum" does not exist in dict but
        //          //"hand-held vacuum cleaner"
        //            break;
        c_iterMaxWordMatch = c_iter;
        r_dwTokenIndex = tokenIndex;
      }
      ++ tokenIndex;
    }while( tokenIndex < numTokens &&
        tokenIndex - beginTokenIndex < m_maxTokenToConsider );
  }
//  r_dwTokenIndex = tokenIndex - 1;
  if( c_iterMaxWordMatch != m_charStringMap.end() )
  {
//    VocablesForWord * p_vocablesforword = (VocablesForWord *)
//      & c_iterMaxWordMatch->second;
//    p_voc_container_type = & p_vocablesforword->
//      m_std_set_p_vocabularyandtranslation;
    VocablesForWord & vocablesforword = (VocablesForWord &)
      c_iterMaxWordMatch->second;
    p_voc_container_type = //c_iter->second.m_p_setpvocabularyandtranslation;
      (VocablesForWord::voc_container_type *) & //c_iter->second.
      vocablesforword.m_std_set_p_vocabularyandtranslation;
    IVocabularyInMainMem::OutputVocs(p_voc_container_type);
  }
  return p_voc_container_type;
}

fastestUnsignedDataType CharStringStdMap::GetNumberOfAllocatedBytes()
{
  fastestUnsignedDataType numberOfAllocatedBytes = 0;
  VocabularyAndTranslation * p_vocabularyAndTranslation;
  for( map_type::const_iterator c_iter_map = m_charStringMap.begin() ;
      c_iter_map != m_charStringMap.end() ;
      ++ c_iter_map )
  {
#ifdef _DEBUG
    const std::string & st = c_iter_map->first;
    const char * p_ch = st.c_str();
    numberOfAllocatedBytes += st.size()
        /*+ sizeof(std::basic_string<char>::_Rep_base)*/;
#endif
    DEBUGN( "freeing memory for dictionary word \"" << c_iter_map->first << "\"")
    /** "const_iterator" for Android NDK. */
    for( std::set<VocabularyAndTranslation *>::const_iterator
      iter_p_vocabularyandtranslation =
        c_iter_map->second.m_std_set_p_vocabularyandtranslation.begin() ;
      iter_p_vocabularyandtranslation !=
      c_iter_map->second.m_std_set_p_vocabularyandtranslation.end() ;
      ++ iter_p_vocabularyandtranslation )
    {
#ifdef _DEBUG
      VocabularyAndTranslation * p_vocabularyAndTranslation =
        * iter_p_vocabularyandtranslation;
      numberOfAllocatedBytes += p_vocabularyAndTranslation->GetNumberOfBytes();
#else
      numberOfAllocatedBytes += (* iter_p_vocabularyandtranslation)->GetNumberOfBytes();
#endif
//      -- ::wxGetApp().s_numberOfVocabularyPairs;
//      -- m_numberOfVocPairs;
//      -- m_numberOfEnglishWords;
    }
  }
  return numberOfAllocatedBytes;
}

/** @brief inserts the character string for the current word into the map
 *  */
//VocabularyAndTranslation * /*void * */
IVocabularyInMainMem::voc_container_type * CharStringStdMap::Insert(
//  const std::string & std_strWord, BYTE byWordClass
  const char * wordBegin, int numChars, //void * p_v
    enum EnglishWord::English_word_class word_class,
//    bool insertNewVocAtts
//    void *& p_v
    VocabularyAndTranslation *& p_vocabularyandtranslation
  )
{
  LOGN_DEBUG("begin-\"" << std::string(wordBegin, numChars)
  << "\" voc and tranl ptr:" << p_vocabularyandtranslation)
//  VocabularyAndTranslation * p_vocabularyandtranslation = //NULL;
//    (VocabularyAndTranslation *) p_v;
//  map_type::iterator iter = m_charStringMap.find(std_strWord);
//  if( iter == m_charStringMap.end() ) //not in map
//  {
//  }
//  else
//  {
//    //TODO
////    iter->second.m_p_setpvocabularyandtranslation->insert();
//  }
//  VocablesForWord vocablesforword;
  std::pair<map_type::iterator, bool> pair_iter_and_inserted =
    //"bool that is true if the pair was actually inserted"
    m_charStringMap.insert(
    std::make_pair(//std_strWord
      std::string(wordBegin, numChars), //vocablesforword
      VocablesForWord() ) );
//  if( pair_iter_and_inserted.second == true )
//  {
//
//  }
//  LOGN_DEBUG("iterator " << pair_iter_and_inserted.first << " end:"
//    << m_charStringMap.end() )
  map_type::iterator iter = pair_iter_and_inserted.first;
  const bool insertedIntoContainer = pair_iter_and_inserted.second;
  LOGN_DEBUG("inserted into map?:" << (insertedIntoContainer ? "yes" : "no") )
  if( /*insertedIntoContainer &&*/ iter != m_charStringMap.end() )
  {
    VocablesForWord & r_vocablesforword = pair_iter_and_inserted.first->second;
    if( /*insertNewVocAtts*/ ! p_vocabularyandtranslation )
      p_vocabularyandtranslation = new VocabularyAndTranslation(word_class);
//    p_v = p_vocabularyandtranslation;

//    p_vocabularyandtranslation->GetNumberOfBytes();

//    vocablesforword.m_p_setpvocabularyandtranslation->insert(
//      p_vocabularyandtranslation );
    r_vocablesforword.insert(p_vocabularyandtranslation );
    IVocabularyInMainMem::voc_container_type * p_voc_container =
      & r_vocablesforword.m_std_set_p_vocabularyandtranslation;
    LOGN_DEBUG("return " << p_voc_container)
    return p_voc_container;
  }
//  bool bInsertNewVocabularyAndTranslation = true ;
//  std::set<VocablesForWord *> stdsetpletternodeLastStringChar ;
//  return HandleVocabularyAndTranslationPointerInsertion(
//    stdsetpletternodeLastStringChar,
//    bInsertNewVocabularyAndTranslation,
//    byWordClass);
  LOGN_DEBUG("return NULL")
  return //p_vocabularyandtranslation;
    NULL;
}

//void
////   VocabularyAndTranslation *
//CharStringStdMap::Insert(
//    EnglishWord & ew , GermanWord & gw, //void * p_v
//    VocabularyAndTranslation * p_vocabularyandtranslation
//    )
//{
////     bool bInsertNewVocabularyAndTranslation = true ;
//  std::string std_strWord ;
//  std::set<LetterNode *> stdsetpletternodeLastStringChar ;
//  ew.InitGetNextString() ;
////     VocabularyAndTranslation * p_vocabularyandtranslation = //new
//////         VocabularyAndTranslation(ew.GetWordClass() );
////       (VocabularyAndTranslation *) p_v;
//  unsigned uiIndex = 0;
//  while( ew.GetNextString(std_strWord) )
//  {
////       InsertIntoTrieAndHandleVocabularyAndTranslation(
////         stdsetpletternodeLastStringChar
////         , bInsertNewVocabularyAndTranslation //bool & bInsertNewVocabularyAndTranslation
////         , ew.GetWordClass()
////         , std_strWord
////         , std_strWord.length()
////         , 0
////         ) ;
//    InsertAsKeyAndHandleVocabularyAttributes(
//      std_strWord.c_str(),
//      std_strWord.length(), ew.GetWordClass() );
//      p_vocabularyandtranslation->m_arstrEnglishWord[uiIndex] = std_strWord;
//      ++ uiIndex;
//  }
//  uiIndex = 0;
//  while( gw.GetNextString(std_strWord) )
//  {
//#ifdef _DEBUG
//      const char * p_ch = std_strWord.c_str();
//#endif
//      p_vocabularyandtranslation->m_arstrGermanWord[uiIndex] = std_strWord;
//      ++ uiIndex;
//  }
////     return (void *) //s_pvocabularyandtranslation;
////         p_vocabularyandtranslation;
//}

//inline //void
//  VocabularyAndTranslation *
//  CharStringStdMap::HandleVocabularyAndTranslationPointerInsertion(
//  //This set is to ensure that for identical strings for the SAME vocabulary
//  //not 2 or more VocAndTransl object should be inserted into the same
//  //LetterNode of the last character.
//  std::set<VocablesForWord *> & stdsetpletternodeLastStringChar
//  //, VocabularyAndTranslation * pvocabularyandtranslation
//  , bool bInsertNewVocabularyAndTranslation
//  , BYTE byVocabularyType
// )
//{
//  //If already a word with the same string was inserted into the Trie
//  //(e.g. "to love" before and now the noun "the love")
//  //m_psetpvocabularyandtranslation is not NULL. Else it must be created.
//  if( ! s_p_vocablesforwordLastForInsertedWord->m_p_setpvocabularyandtranslation )
//    //{
//    //pletternodeCurrent->m_psetvocabularyandtranslation = new
//      //std::set<VocabularyAndTranslation>(//0,new Word(),new Word()
//    s_p_vocablesforwordLastForInsertedWord->m_p_setpvocabularyandtranslation = new
//      std::set<VocabularyAndTranslation *>(//0,new Word(),new Word()
//      ) ;
//  //if( pletternodeCurrent->m_psetvocabularyandtranslation )
//
//  //If allocating was successful / if exits yet.
//  if( s_p_vocablesforwordLastForInsertedWord->m_p_setpvocabularyandtranslation )
//  {
//    if(bInsertNewVocabularyAndTranslation)
//    {
//      std::pair <std::set<VocabularyAndTranslation *>::iterator, bool>
//        pairisetandbool =
//        //pletternodeCurrent->m_psetvocabularyandtranslation->insert(
//            s_p_vocablesforwordLastForInsertedWord->
//            m_p_setpvocabularyandtranslation->insert(
//          //VocabularyAndTranslation(byVocabularyType //+
//          ////bGermanVocabulary * NUMBER_OF_WORD_TYPES
//          //)
//          new VocabularyAndTranslation(byVocabularyType )
//          ) ;
//      s_p_vocabularyandtranslation =
//        ////Pointer to VocabularyAndTranslation object/reference.
//        //&
//        //VocabularyAndTranslation object/reference.
//        *(pairisetandbool.first) ;
//      stdsetpletternodeLastStringChar.insert( s_p_vocablesforwordLastForInsertedWord ) ;
//    }
//    else
//      if(
//        //If the LetterNode-pointer does NOT exist in the std::set yet:
//        //If for instance for the verb "love" the simple past
//        //"loved" was inserted
//        //then for the past participle
//        //"loved" that has the same LetterNode pointer
//        //there should not be inserted a VocabularyAndTranslation pointer again.
//        stdsetpletternodeLastStringChar.find( s_p_vocablesforwordLastForInsertedWord ) ==
//        stdsetpletternodeLastStringChar.end()
//        )
//      {
//        s_p_vocablesforwordLastForInsertedWord->insert( //pvocabularyandtranslation
//          s_p_vocabularyandtranslation ) ;
//        stdsetpletternodeLastStringChar.insert( s_p_vocablesforwordLastForInsertedWord ) ;
//      }
//  }
//  return s_p_vocabularyandtranslation;
//}
