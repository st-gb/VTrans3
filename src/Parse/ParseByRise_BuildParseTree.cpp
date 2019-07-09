/** ParseByRise_BuildParseTree.cpp
 *  Created on: 18.08.2011
 *      Author: Stefan */

#include "ParseByRise.hpp"
//TranslationControllerBase::s_dictionary
#include <Controller/TranslationControllerBase.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h> //DEBUG_COUT(...)
/** SUPPRESS_UNUSED_VARIABLE_WARNING(...) */
#include <compiler/GCC/suppress_unused_variable.h>
#include <Controller/character_string/convertFromAndToStdString.hpp>
//#include <VocabularyInMainMem/LetterTree/LetterNode.hpp>//class LetterNode
//class VocabularyAndTranslation
#include <VocabularyInMainMem/VocabularyAndTranslation.hpp>
#include <Controller/TranslationProcess.hpp>//class TranslationProcess

#ifndef MAXWORD
  #define MAXWORD 65535
#endif

//class TranslationControllerBase;
//LetterTree TranslationControllerBase::s_dictionary;

namespace VTrans3 {
/** Does the Part-Of-Speech (word class) tagging.
 *  Creates the leafs of possible parse trees ausgehend from the source text.*/
// Leaves are word classes, e.g. nouns.
// If a grammar rule applies later then these leaves can make up a parse trees:
// the vacuum cleaner sucks.
// +-+ +------------+ +---+    <- leaves
//   \ /              /
// def_article_noun    /           <- tree struct: parse tree
//              \   /
//              clause       <- tree struct: parse tree
void BottomUpParser::CreateInitialGrammarParts ( const std::string &
  cr_stdstrText )
{
  //PositionstdstringVector psv ;
  m_wParseLevel = 1 ;
  m_psv.clear() ;
//  std::map<DWORD, GrammarPart> stdmapwLeftmostIndex2grammarpart ;
//  std::map<DWORD, GrammarPart> stdmapwRightmostIndex2grammarpart ;

//  BuildTokenVector(cr_stdstrText,m_psv) ;
  m_psv.BuildTokenVector(cr_stdstrText);
//  std::cout << "all tokens:" << m_psv << std::endl;
#ifdef _DEBUG
  DWORD dwSize ;
#endif
//  PositionstdstringVector::const_iterator iter = m_psv.begin() ;
  PositionStringVector::const_iterator iter = m_psv.begin() ;
  #ifdef _DEBUG
//  DEBUG_COUT( "tokens:\n" );
  #endif
  DWORD wTokenIndex = 0 ;
  //ParseByRise parsebyrise ;
  while( iter != m_psv.end() )
  {
    #ifdef _DEBUG
//    std::cout << "\"" << (*iter).m_Str << "\"\n" ;
    #endif
    //std::set<VocabularyAndTranslation *> setpvocabularyandtranslation ;

//   if( g_lettertree.IsSingular(
//     //searchIncludingInflections(
//     psv
//     , wTokenIndex ,
//     setpvocabularyandtranslation
//     ) )
//   {
//     outputGermanTranslation(setpvocabularyandtranslation) ;
//   }
    //outputWordTypeAndGermanTranslation(psv,wTokenIndex) ;
    //parsebyrise.StoreWordTypeAndGermanTranslation(
    StoreWordTypeAndGermanTranslation(
      m_psv //PositionstdstringVector & psv
      , wTokenIndex //DWORD dwTokenIndex
    //  , std::map <WORD, std::set<VocabularyAndTranslation *> *> &
    //    stdmap_wIndex2p_set_p_vocabularyandtranslation
    //  , std::vector<GrammarPart> & stdvector_grammarpart ;

      //Maintaining 2 maps with both leftnost and rightmost indexes should be faster
      //when searching for neighboured grammar parts:
      //  0    1       2      3      <-index
      //"the vacuum cleaner sucks."
      // +----------------+
      //   e.g. current grammar part: rightmost index: 2: find neighbour with
      //     leftmost index "3".
//      , stdmapwLeftmostIndex2grammarpart
//      , stdmapwRightmostIndex2grammarpart
//      , stdmultimap_wLeftmostIndex2grammarpart
//      , stdmultimap_wRightmostIndex2grammarpart
      ) ;
#ifdef _DEBUG
    dwSize = //parsebyrise.
//      m_stdmultimap_dwRightmostIndex2grammarpart.size() ;
      m_stdmultimap_dwRightmostIndex2p_grammarpart.size() ;
    dwSize = //parsebyrise.
//      m_stdmultimap_dwLeftmostIndex2grammarpart.size() ;
      m_stdmultimap_dwLeftmostIndex2p_grammarpart.size() ;
#endif
    ++ iter ;
    ++ wTokenIndex ;
  }
}

void BottomUpParser::RemoveAllBetweenInRightIndexMap(
  std::multimap<DWORD, GrammarPart *>::iterator & iterFirstUnknownWord,
  std::multimap<DWORD, GrammarPart *>::iterator & iterLastUnknownWord//,
//  const GrammarPart * p_grammarPart
  )
{
  std::multimap<DWORD, GrammarPart *>::iterator c_iterCurrentLeftMost =
    iterFirstUnknownWord;
  std::multimap<DWORD, GrammarPart *>::iterator c_iterCurrentRightMost;// =
//    m_stdmultimap_dwRightmostIndex2p_grammarpart;

  while( c_iterCurrentLeftMost !=
    m_stdmultimap_dwLeftmostIndex2p_grammarpart.end() )
  {
    //traverse right index vecor
    const GrammarPart * p_leftCurrentMostGrammarPart = c_iterCurrentLeftMost->second;
    c_iterCurrentRightMost = m_stdmultimap_dwRightmostIndex2p_grammarpart.
      find(p_leftCurrentMostGrammarPart->m_dwRightmostIndex);

    if( c_iterCurrentRightMost->second == p_leftCurrentMostGrammarPart )
    {
      m_stdmultimap_dwRightmostIndex2p_grammarpart.erase(c_iterCurrentRightMost);
    }

//    m_stdmultimap_dwLeftmostIndex2p_grammarpart.erase(c_iterCurrentLeftMost);

    /** Free memory for the grammarpart object ( it has been copied to a new,
     *  summed up mmap entry. */
    delete p_leftCurrentMostGrammarPart;
    if( c_iterCurrentLeftMost == iterLastUnknownWord )
      break;
    ++ c_iterCurrentLeftMost;
  }
}

void BottomUpParser::PossiblySumUp(
  std::multimap<DWORD, GrammarPart *>::iterator & iterFirstUnknownWord,
  std::multimap<DWORD, GrammarPart *>::iterator & iterLastUnknownWord
//  std::multimap<DWORD, GrammarPart *> & std_multimap_dwIndex2p_grammarpart,
//  const bool leftMostIndices
  )
{
  if( iterFirstUnknownWord != m_stdmultimap_dwLeftmostIndex2p_grammarpart.end()
    && iterLastUnknownWord != m_stdmultimap_dwLeftmostIndex2p_grammarpart.end()
    )
  {
    const fastestUnsignedDataType leftTokenIndex = iterFirstUnknownWord->first;
    const fastestUnsignedDataType rightTokenIndex = iterLastUnknownWord->first;
    GrammarPart * gp = new GrammarPart(
      leftTokenIndex,
      rightTokenIndex,
      iterFirstUnknownWord->second->m_wGrammarPartID);

    gp->m_stdstrTranslation = m_psv.GetBetweenAsStdString(
      leftTokenIndex,
      rightTokenIndex);

    RemoveAllBetweenInRightIndexMap(iterFirstUnknownWord, iterLastUnknownWord);

//    /** Free memory for the grammarpart object */
//    delete iterFirstUnknownWord->second;

#ifdef _DEBUG
    int leftIndexMapSize = m_stdmultimap_dwLeftmostIndex2p_grammarpart.size();
#endif
    //          std_multimap_dwIndex2p_grammarpart.equal_range()
    /** http://www.cplusplus.com/reference/map/map/erase/:
     * " including the element pointed by first but not the one pointed by last." */
    m_stdmultimap_dwLeftmostIndex2p_grammarpart.erase(
      iterFirstUnknownWord, ++ iterLastUnknownWord );
#ifdef _DEBUG
    leftIndexMapSize = m_stdmultimap_dwLeftmostIndex2p_grammarpart.size();
#endif
    /** Set to "not set".*/
    iterFirstUnknownWord = m_stdmultimap_dwLeftmostIndex2p_grammarpart.end();
    iterLastUnknownWord = m_stdmultimap_dwLeftmostIndex2p_grammarpart.end();
    //          m_stdmultimap_dwRightmostIndex2p_grammarpart.find()

//    if( leftMostIndices )
    m_stdmultimap_dwLeftmostIndex2p_grammarpart.insert(
        std::make_pair(gp->m_dwLeftmostIndex, gp) );
    m_stdmultimap_dwRightmostIndex2p_grammarpart.insert(
        std::make_pair(gp->m_dwRightmostIndex, gp) );
//    else
//      std_multimap_dwIndex2p_grammarpart.insert(
//        std::make_pair(gp->m_dwRightmostIndex, gp) );
  }
}

void BottomUpParser::DeleteFromMultiMap(
  std::multimap<DWORD, GrammarPart *> & std_multimap_dwIndex2p_grammarpart
//  , const bool leftMostIndices
  )
{
  std::multimap<DWORD, GrammarPart *>::iterator c_iterLeftMost =
      std_multimap_dwIndex2p_grammarpart.begin();
  std::multimap<DWORD, GrammarPart *>::iterator iterFirstUnknownWord =
      std_multimap_dwIndex2p_grammarpart.end();
  std::multimap<DWORD, GrammarPart *>::iterator iterPreviousUnknownWord =
      std_multimap_dwIndex2p_grammarpart.end();
  std::multimap<DWORD, GrammarPart *>::iterator iterLastUnknownWord =
      std_multimap_dwIndex2p_grammarpart.end();

#ifdef _DEBUG
  const int mapSize = std_multimap_dwIndex2p_grammarpart.size();
#endif

  GrammarPart * p_gpCurrentLeftMost = NULL, * p_gpPreviousLeftMost = NULL;
  bool firstUnknowWordAssigned = false, unknownWordsAreDirectNeighbours;
//  GrammarPart * p_gpFirst = NULL, * p_gpLast = NULL;
//  if( c_iterLeftMost != std_multimap_dwIndex2p_grammarpart.end() )
  {
    while(c_iterLeftMost != std_multimap_dwIndex2p_grammarpart.end() )
    {
      p_gpCurrentLeftMost = c_iterLeftMost->second;
      if( p_gpCurrentLeftMost->m_wGrammarPartID == EnglishWord::UnknownWord )
      {
        firstUnknowWordAssigned = iterFirstUnknownWord !=
          std_multimap_dwIndex2p_grammarpart.end();
//          summedUpUnknownWords;
        if( firstUnknowWordAssigned )
        {
          unknownWordsAreDirectNeighbours =
            p_gpPreviousLeftMost->m_dwLeftmostIndex + 1 ==
            p_gpCurrentLeftMost->m_dwLeftmostIndex;
          if( unknownWordsAreDirectNeighbours )
          {
            iterLastUnknownWord = c_iterLeftMost;
            iterPreviousUnknownWord = iterLastUnknownWord;
            p_gpPreviousLeftMost = iterPreviousUnknownWord->second;
//          /** Free memory for the grammarpart object */
//          delete iterLastUnknownWord->second;
          }
        }
        else
        {
          iterFirstUnknownWord = c_iterLeftMost;
//          p_gpFirst = p_gpPreviousLeftMost;
          iterPreviousUnknownWord = iterFirstUnknownWord;
          p_gpPreviousLeftMost = iterPreviousUnknownWord->second;
        }
      }
      else
      {
        PossiblySumUp(iterFirstUnknownWord, iterLastUnknownWord//,
//          std_multimap_dwIndex2p_grammarpart, leftMostIndices
          );
      }
      ++ c_iterLeftMost;
    }
  }
  PossiblySumUp(iterFirstUnknownWord, iterLastUnknownWord//,
//    std_multimap_dwIndex2p_grammarpart, leftMostIndices
    );
}

/** Sums up unknown words in a row to minimize the number of parse trees.
 *  If not done the parse tree can get extremely large if the source text 
 *  contains many unknown words and the parsing get very slow.
 * 
   e.g. for "Stefan Gebauer is a freak." : 
 *  "Stefan"       "Gebauer" 
 *  unknown word   unknown word
 *       \           /
 *     "Stefan Gebauer"
 *      unknown word
 */
void BottomUpParser::SummarizeUnknownWords()
{
  DeleteFromMultiMap(m_stdmultimap_dwLeftmostIndex2p_grammarpart/*, true*/);
#ifdef _DEBUG
  const int leftIndexMapSize = m_stdmultimap_dwLeftmostIndex2p_grammarpart.size();
  int rightIndexMapSize = m_stdmultimap_dwLeftmostIndex2p_grammarpart.size();
  ++ rightIndexMapSize;
#endif
//  DeleteFromMultiMap(m_stdmultimap_dwRightmostIndex2p_grammarpart, false);
}

void BottomUpParser::CreateParseTree(const std::string & cr_stdstrWholeInputText)
{
  ClearParseTree() ;
  //TODO: resolve contractions https://en.wikipedia.org/wiki/Contraction_(grammar)#English
  // (and abbreviations) first: "they’ll" -> they will
  //  "he’d" -> "he had", "he would"
  //TODO clitics (genitive "'s" ), slang forms like "gonna"
  CreateInitialGrammarParts ( cr_stdstrWholeInputText ) ;

  SummarizeUnknownWords();

  DEBUG_COUT("before resolving GrammarRulesForAllParseLevels \n")
  ResolveGrammarRulesForAllParseLevels() ;
//  bool atLeast1Resolved;
//  do
//  {
//    atLeast1Resolved = Resolve1ParseLevel();
//  }while(atLeast1Resolved);
    
  //TODO idea: before further processing: delete all parse tree that
  //are identical after removing the superordinate grammar rules:

  //the fan               the fan
  //  \ /               =  \  /
  // def_article_noun     def_article_noun
  //                        |
  //                      3rdPersSingEnumEle

  //So the translation would be faster, no double translations.
  //But on the other hand: if there are translation rules that include at least 1
  //of these superordinate rules, they wouldn't apply.
  RemoveSuperordinateRulesFromRootNodes();
  LOGN_DEBUG("end")
}

bool BottomUpParser::GrammarPartDoesNotAlreadyExist(GrammarPart * p_grammarpart)
{
  bool bGrammarPartDoesNotAlreadyExist =
    //Do not store 1 and the same grammar part more than once in
    // -the container "leftmost token index -> grammar part"
    // -the container "rightmost token index -> grammar part"#
    ////Do not do this because: if "the sheep and the sheep":
    //// first: "the sheep and the sheep"
    ////           \  /          \   /
    ////       def_singular    def_singular
    ////               \          /
    ////               3rdPersPlural
    //// Then another 3rdPersPlural would not be possible:
    ////      "the sheep and the sheep"
    ////           \  /          \   /
    ////       def_plural    def_plural
    ////               \          /
    ////               3rdPersPlural
    //m_stdset_grammarpartAllSuperordinate.find( //grammarpart
    m_allSuperordinateGrammarParts2pointerToThem.find(
      * p_grammarpart ) ==
      //m_stdset_grammarpartAllSuperordinate.end();
      m_allSuperordinateGrammarParts2pointerToThem.end();
  return bGrammarPartDoesNotAlreadyExist;
}

void BottomUpParser::GetRuleNames(
  WORD wSuperordinateGrammarPartID,
  GrammarPart * p_grammarpartLeft,
  GrammarPart * p_grammarpartRight
  )
{
//#ifdef _DEBUG
  std::map<WORD, std::string>::const_iterator
    ci_stdmap_wRuleID2stdstrRuleNameSuperordinate ;
  std::map<WORD, std::string>::const_iterator
    ci_stdmap_wLeftGrammarPartID2stdstrRuleName ;
  std::map<WORD, std::string>::const_iterator
    ci_stdmap_wRightGrammarPartID2stdstrRuleName ;
//#endif
  std::string stdstrSuperordinate = "" ;
  std::string stdstrRightChild = "";
  std::string stdstrLeftChild = "" ;

  ci_stdmap_wRuleID2stdstrRuleNameSuperordinate =
    m_stdmap_wRuleID2RuleName.find( wSuperordinateGrammarPartID) ;

  ci_stdmap_wLeftGrammarPartID2stdstrRuleName =
      m_stdmap_wRuleID2RuleName.find(
      p_grammarpartLeft->m_wGrammarPartID ) ;

  ci_stdmap_wRightGrammarPartID2stdstrRuleName =
    m_stdmap_wRuleID2RuleName.find(
    p_grammarpartRight->m_wGrammarPartID ) ;

  if( ci_stdmap_wRuleID2stdstrRuleNameSuperordinate !=
      m_stdmap_wRuleID2RuleName.end()
    )
    stdstrSuperordinate = ci_stdmap_wRuleID2stdstrRuleNameSuperordinate->
    second ;
  if( ci_stdmap_wRightGrammarPartID2stdstrRuleName !=
        m_stdmap_wRuleID2RuleName.end()
    )
   stdstrRightChild = ci_stdmap_wRightGrammarPartID2stdstrRuleName->
     second ;
  if( ci_stdmap_wLeftGrammarPartID2stdstrRuleName !=
      m_stdmap_wRuleID2RuleName.end()
    )
    stdstrLeftChild = ci_stdmap_wLeftGrammarPartID2stdstrRuleName->
    second ;
  //        DEBUG_COUT( "children rule \"" << stdstrSuperordinate
  //          << "(" << & grammarpart << ",ID=" << wSuperordinateGrammarPartID << ")"
  //          <<  "\" found for "
  //          //"the token range"
  //          //<< dwLeftMostTokenIndexOfRule << "..."
  //          //<< dwRightMostTokenIndexOfRule
  //          << stdstrLeftChild
  //          << "(" << & r_grammarpartLeft << "ID=" << r_grammarpartLeft.m_wGrammarPartID << ")"
  //          << r_grammarpartLeft.m_dwLeftmostIndex
  //          << ","
  //          << r_grammarpartLeft.m_dwRightmostIndex
  //          << ";"
  //          << stdstrRightChild
  //          << "(" << & r_grammarpartRight << ",ID=" << r_grammarpartRight.m_wGrammarPartID << ")"
  //          << r_grammarpartRight.m_dwLeftmostIndex
  //          << ","
  //          << r_grammarpartRight.m_dwRightmostIndex
  //          //<< "]"
  //          << "\n"
  //          );
  //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
  //        #pragma GCC diagnostic ignored  "-Wunused"
  //Useless instruction just for breakpoint possibility.
  int i = 0 ;
  SUPPRESS_UNUSED_VARIABLE_WARNING(i)
}

void BottomUpParser::OutputSuperordinateGrammarParts()
{
  #ifdef _DEBUG
  const int numSuperordinateGPs = //m_stdset_grammarpartAllSuperordinate.size();
    m_allSuperordinateGrammarParts2pointerToThem.size();
  std::ostringstream std_oss;
  std_oss << "# superordinate grammar parts: " << numSuperordinateGPs;
  m_r_translationProcess.SetStatus(VTrans::not_set, std_oss.str().c_str() );
  
  //std::set<GrammarPart>::const_iterator c_iter = 
  //  m_stdset_grammarpartAllSuperordinate.begin();
  std::map<GrammarPart, GrammarPart *>::const_iterator c_iter =  
    m_allSuperordinateGrammarParts2pointerToThem.begin ();
  fastestSignedDataType superOrdinateGrammarPartIndex = 0;
  while( c_iter != //m_stdset_grammarpartAllSuperordinate.end() 
    m_allSuperordinateGrammarParts2pointerToThem.end() )
  {
    const GrammarPart * p_grammarPart = c_iter->second;
    LOGN_DEBUG("superordinate grammar part # " << superOrdinateGrammarPartIndex 
      << ":" << p_grammarPart << " " << * p_grammarPart )
    ++ c_iter;
    ++ superOrdinateGrammarPartIndex;
  }
#endif
}

/** For different translations:
 *  e.g. for "the tiny car" :
 *               \/    /
 * defArticlePositive /
 *                \  /
 *                 \/
 *              defArticleNoun
 *   if subtree is NOT duplicated:
 *     German translations "Auto" and "Fahrkorb" share the SAME GrammarPart
 *       "defArticlePositive", so its translation gets the same:
 *    "der große Auto" <- assigned "the tiny" as left child for when here
 *    der große Fahrkorb
 *   only if subtree is duplicated:
 *    das große Auto <- assigned "the tiny" as left child for when here
 *    der große Fahrkorb */
bool BottomUpParser::PossiblyDuplicateSubTrees(
  GrammarPart * const p_grammarpartParent,
  GrammarPart * const p_grammarpartLeftChild,
  GrammarPart * const p_grammarpartRightChild
  )
{
  bool createdNewSubTree = false;
  GrammarPart * possibleNewSubTree;// =
//            p_grammarpartLeftChild->PossiblyDuplicateSubTree(*this);
  if( /*possibleNewSubTree*/
      p_grammarpartLeftChild->m_bAssignedAsChild )
  {
    possibleNewSubTree = p_grammarpartLeftChild->DuplicateSubTree(* this);
    createdNewSubTree = true;
//          p_grammarpartLeftChild = possibleNewSubTree;
//          p_grammarpartLeftChild = p_grammarpartLeftChild->DuplicateSubtree();
    p_grammarpartParent->SetLeftChild(* possibleNewSubTree);
  }
  else
    p_grammarpartLeftChild->m_bAssignedAsChild = true;
  if( p_grammarpartRightChild->m_bAssignedAsChild )
  {
    possibleNewSubTree = p_grammarpartRightChild->DuplicateSubTree(
//            (const ParseByRise &) (* this)
      * this);
    createdNewSubTree = true;
    p_grammarpartParent->SetRightChild(* possibleNewSubTree);
  }
  else
    p_grammarpartRightChild->m_bAssignedAsChild = true;
  return createdNewSubTree;
}

/*bool*/ GrammarPart * BottomUpParser::InsertIntoSuperordinateGrammarPartContainer(
  GrammarPart * p_grammarPartToInsert)
{
//  LOGN_DEBUG("inserting " << * p_grammarpartSuperordinate
//    << " into container for all superordinate grammar parts")
  LOGN_DEBUG("inserting " << p_grammarPartToInsert 
    << " " << * p_grammarPartToInsert
    << " with into container for all superordinate grammar parts")
  /** In order to not to add it again to the list of ALL grammar parts.*/
//#ifdef _DEBUG
  //std::pair<std::set<GrammarPart>::iterator,bool> pair_ =
  std::pair<std::map<GrammarPart, GrammarPart *>::iterator, bool> pair_ =
//#endif
  //m_stdset_grammarpartAllSuperordinate.insert(
//          //grammarpart
//          * p_grammarpartParent ) ;
    m_allSuperordinateGrammarParts2pointerToThem.insert( std::make_pair(
      * p_grammarPartToInsert, p_grammarPartToInsert) );

  LOGN_DEBUG("inserted into container? : " 
    << (pair_.second == true ? "yes" : "false") )
  LOGN_DEBUG("superordinate grammar parts after inserting into container:")
  OutputSuperordinateGrammarParts();

  //"false if the set already contained an element whose key had an
  //equivalent value in the ordering"
  if( /*valueAlreadyContained =*/  pair_.second == false )
   return pair_.first->second;
  return NULL;
}

bool BottomUpParser:://GrammarRuleAppliesTo(
  InsertIfGrammarRuleAppliesTo(
  //Maintaining 2 maps with both leftmost and rightmost indexes should be faster
  //when searching for neighboured grammar parts:
  //  0    1       2      3      <-index
  //"the vacuum cleaner sucks."
  // +----------------+
  //   e.g. current grammar part: rightmost index: 2: find neighbour with
  //     leftmost index "3".
//  std::multimap<DWORD, GrammarPart>::iterator
//    iter_mm_rightmostidx2grammarptLeftGrammarPart
//  , std::multimap<DWORD, GrammarPart>::iterator
//    iter_wLeftmostIndex2grammarpartRightGrammarPart
  std::multimap<DWORD, GrammarPart *>::iterator
    iter_mm_rightmostidx2p_grammarptLeftGrammarPart
  , std::multimap<DWORD, GrammarPart *>::iterator
    iter_wLeftmostIndex2p_grammarpartRightGrammarPart
  )
{
  bool bRuleApplied = false ;
  DWORD dwLeftMostTokenIndexOfRule ;
  DWORD dwRightMostTokenIndexOfRule ;
  WORD wLeftGrammarPartIDForRule =
//    iter_mm_rightmostidx2grammarptLeftGrammarPart->second.m_wGrammarPartID ;
    iter_mm_rightmostidx2p_grammarptLeftGrammarPart->second->m_wGrammarPartID ;
  WORD wRightGrammarPartIDForRule =
//    iter_wLeftmostIndex2grammarpartRightGrammarPart->second.m_wGrammarPartID ;
    iter_wLeftmostIndex2p_grammarpartRightGrammarPart->second->m_wGrammarPartID ;
  //for( GrammarRule )
//  //Using a 2-dimensional array if a fast way. dimension 1 should equal
//  //dimension 2. But on the other hand adding rules afterwards is not easy
//  //if the array size was exactly as the number of rules before.
//  WORD wGrammarRule = m_ararwGrammarRuleID[ wLeftGrammarPartIDForRule ]
//    [ wLeftGrammarPartIDForRule ] ;
//    //If a rule exists.
//  if( wGrammarRule != 0 )

  //typedef std::multimap<WORD, WORD>::const_iterator c_iter_mm ;
//  std::pair< c_iter_mm,c_iter_mm> stdpair1stAndLastLeft2RightGrammarPartID =
//    m_stdmultimap_wGrammarPartID2wGrammarPartID.equal_range(
//    wLeftGrammarPartIDForRule ) ;
  typedef std::multimap<WORD, GrammarRule>::const_iterator c_iter_mm ;
  std::pair< c_iter_mm,c_iter_mm> stdpair1stAndLastLeft2RightGrammarPartID =
    //Get all rules that match to the left grammar part ID.
    //e.g. for params passed "the", "car":
    // -left grammar part ID: "definite_article"
    // -grammar rule may be "definite_article" + "noun" = "def_article_noun".
    // it gets all rules whose left grammar part ID is "definite_article"
    //(The grammar rules container isn't modified during parsing)
    m_stdmmap_wLeftChildGrammarPartID2SuperordinateGrammarRule.equal_range(
    wLeftGrammarPartIDForRule ) ;
  WORD wSuperordinateGrammarPartID ;

  //e.g. token 1: "the" -> article, token2: "man" -> noun
  // article + noun = def_article_noun
  //if( RulesAppliesTo( iter, iterLeftGrammarPart ) )
//  for( c_iter_mm iterLeft2RightGrammarPartIDCurrent =
//    stdpair1stAndLastLeft2RightGrammarPartID.first ;
//    iterLeft2RightGrammarPartIDCurrent !=
//    stdpair1stAndLastLeft2RightGrammarPartID.second ;
//    ++ iterLeft2RightGrammarPartIDCurrent
//    )

  //Traverse all rules whose left grammar part ID is the same as for the passed
  //left grammar part.
  //(The grammar rules container isn't modified during parsing)
  for( c_iter_mm iterLeft2RightGrammarPartIDCurrent =
    stdpair1stAndLastLeft2RightGrammarPartID.first ;
    iterLeft2RightGrammarPartIDCurrent !=
    stdpair1stAndLastLeft2RightGrammarPartID.second ;
    ++ iterLeft2RightGrammarPartIDCurrent
    )
  {
    //grammar rule whose left child grammar part ID is the same as the passed
    //ID.
    const GrammarRule & r_grammarrule = iterLeft2RightGrammarPartIDCurrent->
      second ;
    /** Rule matches for 2 neighboured grammatical items. */
    if( //iterLeft2RightGrammarPartIDCurrent->second ==

      //The 2 passed grammar parts belong to the same rule:
      //e.g. rule "definite_noun": superordinate ID = 2, right child grammar part ID:1
        //  "the": ID = 0   "car" ID = 1
      r_grammarrule.m_wRightChildGrammarPartID == wRightGrammarPartIDForRule
//#ifdef _DEBUG
//      )
//      if(
//      //And the left grammar part was has not been applied to a rule yet.
//      //(this serves as a break condition)
//#else
//      &&
//#endif
//      m_stdset_p_grammarpartAllSuperordinate.find(
//        & iter_mm_rightmostidx2grammarptLeftGrammarPart->second ) ==
//        m_stdset_p_grammarpartAllSuperordinate.end()
      )
    {
//      InsertGrammarPartForAppliedRule();
//      bRuleApplied = true ;
      wSuperordinateGrammarPartID = r_grammarrule.m_wSuperordinateGrammarRuleID ;
      dwLeftMostTokenIndexOfRule =
//        iter_mm_rightmostidx2grammarptLeftGrammarPart->second.
//          m_dwLeftmostIndex ;
        iter_mm_rightmostidx2p_grammarptLeftGrammarPart->second->
        m_dwLeftmostIndex ;
      dwRightMostTokenIndexOfRule =
//        iter_wLeftmostIndex2grammarpartRightGrammarPart->second.
//          m_dwRightmostIndex ;
        iter_wLeftmostIndex2p_grammarpartRightGrammarPart->second->
        m_dwRightmostIndex ;

      LOGN_DEBUG("grammar rule matches: superordinate ID:"
        << wSuperordinateGrammarPartID << " name:"
        << GetGrammarPartName(wSuperordinateGrammarPartID)
        << " left token index:" << dwLeftMostTokenIndexOfRule
        << " right token index:" << dwRightMostTokenIndexOfRule
        )

      //GrammarPart grammarpart( //iterLeftGrammarPart->first, //wGrammarPartIDOfRule
      GrammarPart * p_grammarpartParent = new GrammarPart(
        //wLeftGrammarPartIDForRule
        dwLeftMostTokenIndexOfRule
        , //wRightGrammarPartIDForRule
        dwRightMostTokenIndexOfRule
        , wSuperordinateGrammarPartID
        ) ;
      GrammarPart * p_grammarpartLeftChild =
        iter_mm_rightmostidx2p_grammarptLeftGrammarPart->second;
      GrammarPart * p_grammarpartRightChild =
        iter_wLeftmostIndex2p_grammarpartRightGrammarPart->second;

      LOGN_DEBUG("grammar rule matches:"
        " left grammar part:" << p_grammarpartLeftChild
        << " " << GetGrammarPartName(p_grammarpartLeftChild->m_wGrammarPartID)
        << " right grammar part:" << p_grammarpartRightChild
        << " " << GetGrammarPartName(p_grammarpartRightChild->m_wGrammarPartID)
        )

      /** e.g. for the grammar part "def_article_noun" add "the" (article) and
      * "car" (noun)
      * In order to translate: add children; also for GrammarPart::"<" operator */
//      grammarpart.SetLeftChild(
      p_grammarpartParent->SetLeftChild(
//        iter_mm_rightmostidx2grammarptLeftGrammarPart->second ) ;
        * p_grammarpartLeftChild ) ;
//      grammarpart.SetRightChild(
      p_grammarpartParent->SetRightChild(
//        iter_wLeftmostIndex2grammarpartRightGrammarPart->second ) ;
        * p_grammarpartRightChild ) ;

      const bool grammarPartDoesNotAlreadyExist =
        GrammarPartDoesNotAlreadyExist(p_grammarpartParent);
      LOGN_DEBUG("grammarPartDoesNotAlreadyExist?:" <<
        grammarPartDoesNotAlreadyExist)
      /** This must be checked as a break criteria! Else resolving grammar rules
      * was endless. */
      if( grammarPartDoesNotAlreadyExist )
      {
          //In order to not to add it again to the list of ALL grammar parts
  #ifdef _DEBUG
          const int numSuperordinateGPs = //m_stdset_grammarpartAllSuperordinate.size();
            m_allSuperordinateGrammarParts2pointerToThem.size();
//          std::pair<std::set<GrammarPart>::iterator,bool> pair_ =
  #endif
        InsertIntoSuperordinateGrammarPartContainer(p_grammarpartParent);
//          InsertIntoOutmostTokenIndexMaps(
//            p_grammarpartParent,
//            dwLeftMostTokenIndexOfRule,
//            dwRightMostTokenIndexOfRule
//            );

        bool createdNewSubTree = PossiblyDuplicateSubTrees(
          p_grammarpartParent,
          p_grammarpartLeftChild,
          p_grammarpartRightChild
          );

//        GrammarPart & grammarpart = * p_grammarpartParent ;
        bRuleApplied = true ;
//        //e.g. for the grammar part "def_article_noun" add "the" (article) and
//        //"car" (noun)
//        //In order to translate: add children
//        grammarpart.SetLeftChild(
//          iter_mm_rightmostidx2grammarptLeftGrammarPart->second ) ;
//        grammarpart.SetRightChild(
//          iter_wLeftmostIndex2grammarpartRightGrammarPart->second ) ;
#ifdef COMPILE_WITH_LEFT2RIGHT_RELATIONSHIP
        //when getting the personindex this is a fast method:
        //the car and the cat suck.
        //  \ /    /   \  /
        //  nc(1) /   nc(2)
        //   \   /     /
        //   nc_conj  /
        //        \  /
        //     nc_conj_nc
        //        |
        //
        //insert left child to right child mappings for:
        // the -> car
        // nc(1) -> and
        // the  -> cat
        // nc_conj -> nc(2)
        // then when I want to know the person index:
        // the car -> singular -> 3rd pers sing; save "3rd person sing" in
        //    parent "nc(1)"
        //   -add "nc(1)" to a list for the next level,
        //  -continue with
        // the car + and : no change
        // the cat -> singular -> 3rd pers sing
        // "the cat and" + "the cat" : 3rd pers sing + 3rd pers sing = 3rd pers plur
        //
        m_stdmapgrammarpartLeftChild2grammarpartRightChild.insert(
          std::pair<GrammarPart *,GrammarPart *>(
          iter_mm_rightmostidx2grammarptLeftGrammarPart->second,
          iter_wLeftmostIndex2grammarpartRightGrammarPart->second
          )
          ) ;
#endif
        //Bad approach !? better simply check whether the number of tokens for
        //an applied rule got larger for each parse level?!
  //      //Memorize the grammar parts involved in applied rules as a break
  //      //condition for applying rules for all parse levels.
  //      m_stdset_p_grammarpartAllSuperordinate.insert(
  //        & iter_mm_rightmostidx2grammarptLeftGrammarPart->second ) ;
  //      m_stdset_p_grammarpartAllSuperordinate.insert(
  //        & iter_wLeftmostIndex2grammarpartRightGrammarPart->second ) ;

        if( createdNewSubTree )
        {
//        const bool valueAlreadyContained = 
        const GrammarPart * p_grammarPart =
          InsertIntoSuperordinateGrammarPartContainer(p_grammarpartParent);

//        //Must assign as child after inserting into the std::set, else
//        //endless loop?!
//        PossiblyCreateNewLeaves(
//          p_grammarpartParent,
//          p_grammarpartLeftChild,
//          p_grammarpartRightChild
//          );
#ifdef _DEBUG
        const int numSuperordinateGPs = //m_stdset_grammarpartAllSuperordinate.size();
          m_allSuperordinateGrammarParts2pointerToThem.size ();
        if( /*valueAlreadyContained*/ p_grammarPart )
        {
          //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//          #pragma GCC diagnostic ignored  "-Wunused"
          //const GrammarPart & r_gp = *(pair_.first) ;
          const GrammarPart & r_gp = /*pair_.first->first*/ * p_grammarPart;
          SUPPRESS_UNUSED_VARIABLE_WARNING(r_gp)
//          #pragma GCC diagnostic pop
          int i = 0 ;
          SUPPRESS_UNUSED_VARIABLE_WARNING(i)
//          DEBUG_COUT( "already contained")
        }
#endif
        }

        m_wNumberOfTokensForAppliedGrammarRule = dwRightMostTokenIndexOfRule -
            dwLeftMostTokenIndexOfRule ;
        if( m_wNumberOfTokensForAppliedGrammarRule >
          m_wBiggestNumberOfTokensForAppliedGrammarRule )
          m_wBiggestNumberOfTokensForAppliedGrammarRule =
              m_wNumberOfTokensForAppliedGrammarRule ;

      //  m_stdmultimap_.insert( iterLeftGrammarPart->first , )

//        if( createdNewSubTree )
          InsertIntoOutmostTokenIndexMaps(
            p_grammarpartParent,
            dwLeftMostTokenIndexOfRule,
            dwRightMostTokenIndexOfRule
            );

  //      //Now delete the 2 inner grammar parts as a break criteria to avoid
  //      //an endless loop. e.g. delete the token indices of
  //      // "the" and "vacuum cleaner" for ""the vacuum cleaner".
  //      //Delete the left of the 2 inner grammar parts.
  //      m_stdmultimap_dwRightmostIndex2grammarpart.erase(
  //        iter_mm_rightmostidx2grammarptLeftGrammarPart ) ;
  //      //Delete the right of the 2 inner grammar parts.
  //      m_stdmultimap_dwLeftmostIndex2grammarpart.erase(
  //        iter_wLeftmostIndex2grammarpartRightGrammarPart ) ;

        #ifdef _DEBUG
          GetRuleNames(
            wSuperordinateGrammarPartID,
            iter_mm_rightmostidx2p_grammarptLeftGrammarPart->second,
            iter_wLeftmostIndex2p_grammarpartRightGrammarPart->second
            );
        #endif
      }
      else
      {
        //already applied a rule to these children-> delete.
        delete p_grammarpartParent ;
//        DEBUG_COUTN("grammar part " << GetGrammarPartName(
//          grammarpart.m_wGrammarPartID ) << grammarpart.m_dwLeftmostIndex
//          << "," <<
//          grammarpart.m_dwRightmostIndex << " already applied to a rule yet")
      }
      //IDEA (e.g. as break condition when to end parsing):
      // -insert parse level
      // -insert number of tokens
      //Then the parse area that covers the most tokens is taken to translate.
      //Or maybe the highest parse level is taken for translation.

//      //possible break conditions:
//      //-if no more grammar rules can be applied, i.e. the size of the
//      //  grammar part container does not grow anymore.
//      m_stdmapGrammarPartSize2GrammarPart.insert(
//        std::pair<DWORD, GrammarPart>(
//        dwRightMostTokenIndexOfRule - dwLeftMostTokenIndexOfRule
//        )
//        ) ;

//      //for every rise level ( where a rule is supposed to be applied in order
//      //to get a larger area) a new map is created (and added to a container).
//      mp_stdmultimap_dwRightmostIndex2grammarpartCurrent->insert(
//        std::pair<DWORD, GrammarPart> (
//        //iterRightGrammarPart->first
//        wRightGrammarPartIDForRule
//        , grammarpart )
//      ) ;

//      //Erase in order to not apply the rule again (and to fullfill the break
//      //criteria when no rule could be applied.
//      m_stdmultimap_dwLeftmostIndex2grammarpart.erase(
//        iterLeftGrammarPart
//        , iterLeftGrammarPart ) ;
//      EraseEntry( m_stdmultimap_dwLeftmostIndex2grammarpart ,
//        iterLeftGrammarPart->first, iterLeftGrammarPart->second ) ;
//      m_stdmultimap_dwRightmostIndex2grammarpart.erase
    }
#ifdef _DEBUG
    else
    {
//      GrammarPart & r_gp = iter_mm_rightmostidx2grammarptLeftGrammarPart->
//          second ;
//      DEBUG_COUTN("grammar part " << GetGrammarPartName(
//        r_gp.m_wGrammarPartID ) << r_gp.m_dwLeftmostIndex << "," <<
//        r_gp.m_dwRightmostIndex << " already applied to a rule yet")
    }
#endif
  }
//  m_p_userinterface->
  return bRuleApplied ;
}

//Problem with this:
//if a grammar rule affects more than 1 grammar part, e.g.:
//"sheep" (singular_noun) and "sheep" (plural_noun):
//when translating "the sheep" and the translation rule then matches
//"definite_article_singular.singular_plural" and the article is
//translated to "die" it is overwritten by "das" from
//"definite_article_singular.singular_noun" because the grammar part
//is the same/ shared/ referenced by the two ones.
//So as solution: clone grammar part.
void BottomUpParser::InsertIntoOutmostTokenIndexMaps(
  GrammarPart * p_grammarpart,
  DWORD dwLeftMostTokenIndexOfRule,
  DWORD dwRightMostTokenIndexOfRule
  )
{
  LOGN_DEBUG("begin " << p_grammarpart << " " << dwLeftMostTokenIndexOfRule
    << " " << dwRightMostTokenIndexOfRule )
  //The map with the leftmost indexes can be used for translation:
  //  1. iterate over all GrammarPart beginning at index i
  //  2. use the GrammarPart that starts at index i and covers the most tokens,
  //    i.e. has the rightmost token index of all GrammarPart elements
  //    starting at index i
  //m_stdmultimap_dwLeftmostIndex2grammarpart.insert(
  //m_iter_stdmap_wParseLevelIndex2stdmultimap_dwLeftmostIndex2grammarpart->second.
//        mp_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate->
    
  InsertIntoLeftmostAndRightmostTokenIndexContainersFor1ParseLevel(
    dwLeftMostTokenIndexOfRule,
    dwRightMostTokenIndexOfRule,
    p_grammarpart
    );
  //m_stdmultimap_dwRightmostIndex2grammarpart.insert(
  //m_iter_stdmap_wParseLevelIndex2stdmultimap_dwLeftmostIndex2grammarpart->second.
//        mp_stdmultimap_dwRightmostIndex2grammarpartSuperordinate->
}

void OutputIndex2GrammpartMultiMap(
  const std::multimap<DWORD, GrammarPart* > & std_multimap_dwIndex2p_grammarpart,
  const char  * const p_ch)
{
  std::ostringstream std_oss;
  //m_r_translationcontrollerbase.SetStatus(VTrans::not_set, std_oss.str().c_str() );
  
  //std::set<GrammarPart>::const_iterator c_iter = 
  //  m_stdset_grammarpartAllSuperordinate.begin();
  const int numGrammarParts = //m_stdset_grammarpartAllSuperordinate.size();
    std_multimap_dwIndex2p_grammarpart.size();
  std::multimap<DWORD, GrammarPart *>::const_iterator c_iter =  
    std_multimap_dwIndex2p_grammarpart.begin ();
  fastestSignedDataType superOrdinateGrammarPartIndex = 0;
  LOGN_DEBUG("# " << p_ch << " grammar parts: " << numGrammarParts)
  while( c_iter != //m_stdset_grammarpartAllSuperordinate.end() 
    std_multimap_dwIndex2p_grammarpart.end() )
  {
    const GrammarPart * p_grammarPart = c_iter->second;
    LOGN_DEBUG( p_ch << " grammar part # " << superOrdinateGrammarPartIndex
      << ":" << p_grammarPart << " " << * p_grammarPart )
    ++ c_iter;
    ++ superOrdinateGrammarPartIndex;
  }
}

void BottomUpParser::InsertIntoOverallLeftmostAndRightmostTokenIndexContainers(
  const std::multimap<DWORD, GrammarPart* > & std_multimap_dwLeftmostIndex2p_grammarpart,
  const std::multimap<DWORD, GrammarPart* > & std_multimap_dwRightmostIndex2p_grammarpart
  )
{
 LOGN_DEBUG("begin")
  m_stdmultimap_dwLeftmostIndex2p_grammarpart.insert(
    std_multimap_dwLeftmostIndex2p_grammarpart.begin() ,
    std_multimap_dwLeftmostIndex2p_grammarpart.end()
    ) ;
  m_stdmultimap_dwRightmostIndex2p_grammarpart.insert(
    std_multimap_dwRightmostIndex2p_grammarpart.begin() ,
    std_multimap_dwRightmostIndex2p_grammarpart.end()
    ) ;

  #ifdef _DEBUG
  OutputIndex2GrammpartMultiMap(m_stdmultimap_dwLeftmostIndex2p_grammarpart,
    "overall LEFT");
  OutputIndex2GrammpartMultiMap(m_stdmultimap_dwRightmostIndex2p_grammarpart,
    "overall RIGHT");
  #endif
  LOGN_DEBUG("end")
}

void BottomUpParser::InsertIntoLeftmostAndRightmostTokenIndexContainersFor1ParseLevel(
  const fastestUnsignedDataType leftMostTokenIndex,
  const fastestUnsignedDataType rightMostTokenIndex,
  const GrammarPart * const p_grammarPart
  )
{
  m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel.
    insert(
    std::pair< WORD, GrammarPart *>
    (
      //Leftmost token index of the child grammar part.
      leftMostTokenIndex,
      (GrammarPart *) p_grammarPart
    )
    ) ;
  m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel.
    insert(
    std::pair< WORD, GrammarPart *>
    (
//                iter_mm_token_idx2p_grammarpt->first ,
      rightMostTokenIndex,
//                & grammarpartSuperordinate
      (GrammarPart *) p_grammarPart
    )
    ) ;
}

bool BottomUpParser::InsertSuperordinateGrammarPart(
//  std::multimap<DWORD, GrammarPart> & multmap_token_index2grammarpt
  std::multimap<DWORD, GrammarPart *> & r_multmap_token_index2p_grammarpt
  , bool bMemorizeInsertion
  )
{
  LOGN_DEBUG("begin" )
  bool bReplaced = false ;
//  DWORD dwMapIndex = 0 ;
#ifdef _DEBUG
  std::map<WORD, std::string>::const_iterator ci_stdmap_wRuleID2stdstrRuleName ;
  std::map<WORD, std::string>::const_iterator ci_stdmap_wLeftGrammarPartID2stdstrRuleName ;
#endif
  std::map<WORD,WORD>::iterator iter_stdmap_wGrammarPartID2wSuperordinateID ;
//  std::multimap<DWORD, GrammarPart>::iterator iter_mm_idx2grammarpt
  std::multimap<DWORD, GrammarPart *>::iterator iter_mm_token_idx2p_grammarpt
//    = r_multmap_token_index2p_grammarpt.begin() ;
    = m_stdmultimap_dwLeftmostIndex2p_grammarpart.begin() ;
  WORD wGrammarPartID ;

  while( //iter_mm_idx2grammarpt !=
      //multmap_token_index2grammarpt.end()
      iter_mm_token_idx2p_grammarpt !=
            r_multmap_token_index2p_grammarpt.end()
    )
  {
    wGrammarPartID = //iter_mm_idx2grammarpt->second.
        iter_mm_token_idx2p_grammarpt->second->
        m_wGrammarPartID ;
//    m_stdmultimap_wGrammarPartID2SuperordinateID.find(wGrammarPartID)
//          std::pair< iter_mm_dword2grammarpart,iter_mm_dword2grammarpart>
//            //1st and last iterator having the same value within the multimap.
//            stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart =
//                m_stdmultimap_wGrammarPartID2SuperordinateID.equal_range(
//            //m_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
//            //Get all grammar parts that are direct neighbours of the left grammar
//            //part.
//            iter_mm_rightmostidx2grammarptLeftGrammarPart->first + 1 ) ;
//
//          //e.g. token 1: "the" -> article, token2: "man" -> noun
//          // article + noun = def_article_noun
//          //if( RulesAppliesTo( iter, iterLeftGrammarPart ) )
//          for( iter_mm_dword2grammarpart
//            //This iterator has all items that have the same leftmost token index
//            //that is _directly_ right of the rightmost token index of the
//            //current grammar part.
//            iter_wLeftmostIndex2grammarpartRightGrammarPart =
//            //Iterator with 1st key with searched value.
//            stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart.first ;
//            iter_wLeftmostIndex2grammarpartRightGrammarPart !=
//            //Iterator beyond last key with searched value.
//            stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart.second ;
//            ++ iter_wLeftmostIndex2grammarpartRightGrammarPart )


    //loop for replacing the grammar part ID by the superordinate grammar part
    //ID: e.g.    "the car"
    //              \  /
    //  definite_article_noun
    //       |
    //  subj_or_obj_enum_ele  <-- superordinate
    //       |
    //   subject    <--superordinate
    //
    //  -> replaced the ID 2 times
//    do
//    {
//      bReplaced = false ;
      iter_stdmap_wGrammarPartID2wSuperordinateID =
          m_stdmap_wGrammarPartID2SuperordinateID.find(
        wGrammarPartID) ;
      if( iter_stdmap_wGrammarPartID2wSuperordinateID !=
          m_stdmap_wGrammarPartID2SuperordinateID.end()
        )
      {
#ifdef _DEBUG
       std::string stdstrGrammarPart = GetGrammarPartName(wGrammarPartID ) ;
       //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//       #pragma GCC diagnostic ignored  "-Wunused-variable"
       GrammarPart & r_grammarpartSubclass = //iter_mm_idx2grammarpt->second ;
           * iter_mm_token_idx2p_grammarpt->second ;
       SUPPRESS_UNUSED_VARIABLE_WARNING(r_grammarpartSubclass)
//       WORD wGrammarPartIDsubclass = iter_mm_idx2grammarpt->first ;
       WORD wGrammarPartIDsubclass = iter_mm_token_idx2p_grammarpt->first ;
       SUPPRESS_UNUSED_VARIABLE_WARNING(wGrammarPartIDsubclass)
#endif
        //e.g. "the car"
        //     def_noun
        //        |
        // subj_or_obj_ee
        // -> the indices for "subj_or_obj_ee" stay the same. So copy.
//        GrammarPart grammarpartSuperordinate(
//          iter_mm_idx2grammarpt->second ) ;
        GrammarPart * p_grammarpartSuperordinate = new GrammarPart(
          * iter_mm_token_idx2p_grammarpt->second ) ;
//        GrammarPart & grammarpartSuperordinate = * p_grammarpartSuperordinate ;
  //      iter_mm_rightmostidx2grammarptLeftGrammarPart->second.m_wGrammarPartID =
  //          iter_stdmap_w2w->second ;
        //e.g. subj_or_obj_ee->def_noun
//        grammarpartSuperordinate.mp_grammarpartLeftChild =
        p_grammarpartSuperordinate->mp_grammarpartLeftChild =
          //& iter_mm_idx2grammarpt->second ;
          iter_mm_token_idx2p_grammarpt->second ;
//        grammarpartSuperordinate.mp_grammarpartRightChild = NULL ;
        p_grammarpartSuperordinate->mp_grammarpartRightChild = NULL ;
#ifdef _DEBUG
       WORD wGrammarPartIDSuperordinate =
         iter_stdmap_wGrammarPartID2wSuperordinateID->second ;
       std::string stdstrGrammarPartIDSuperordinate =
           GetGrammarPartName(wGrammarPartIDSuperordinate) ;
//       grammarpartSuperordinate.m_wGrammarPartID = wGrammarPartIDSuperordinate ;
       p_grammarpartSuperordinate->m_wGrammarPartID =
           wGrammarPartIDSuperordinate ;
#else
//        grammarpartSuperordinate.m_wGrammarPartID =
        p_grammarpartSuperordinate->m_wGrammarPartID =
          iter_stdmap_wGrammarPartID2wSuperordinateID->second ;
#endif
  //      iter_mm_rightmostidx2grammarptLeftGrammarPart->second =
  //          grammarpartSuperordinate ;
  #ifdef _DEBUG
//        WORD wIndex = iter_mm_idx2grammarpt->first ;
    //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//    #pragma GCC diagnostic ignored  "-Wunused"
        WORD wIndex = iter_mm_token_idx2p_grammarpt->first ;
        SUPPRESS_UNUSED_VARIABLE_WARNING(wIndex)
//    #pragma GCC diagnostic pop
  #endif

      LOGN_DEBUG("current superordinate grammar part container content:")
      OutputSuperordinateGrammarParts();
        const GrammarPart & grammarPartSuperordinate = * p_grammarpartSuperordinate;
        std::map<GrammarPart, GrammarPart *>::const_iterator c_iter =
          m_allSuperordinateGrammarParts2pointerToThem.find (
  //          grammarpartSuperordinate
          grammarPartSuperordinate);        
        /*const*/ bool grammarPartDoesNotExistInContainter = c_iter ==
          //m_stdset_grammarpartAllSuperordinate.end()
          m_allSuperordinateGrammarParts2pointerToThem.end();
      LOGN_DEBUG("grammar part " << grammarPartSuperordinate 
        << " does not exist in superordinate container? " << 
        (grammarPartDoesNotExistInContainter ? "yes" : "no") )
      
//      grammarPartDoesNotExistInContainter =
//        InsertIntoSuperordinateGrammarPartContainer(p_grammarpartSuperordinate) 
//        == NULL;
      
      LOGN_DEBUG("grammar part does not exist in superordinate container? " << 
        (grammarPartDoesNotExistInContainter ? "yes" : "no") )

        //Do not store 1 and the same grammar part more than once in
        // -the container "leftmost token index -> grammar part"
        // -the container "rightmost token index -> grammar part"
        if( //m_stdset_grammarpartAllSuperordinate.find(
          grammarPartDoesNotExistInContainter )
        {
//          std::pair<c_iter_mmap_dw2p_grammarpart,c_iter_mmap_dw2p_grammarpart>
//            stdpair_iter =
//          m_stdmultimap_dwRightmostIndex2p_grammarpart.equal_range(
//            p_grammarpartSuperordinate->mp_grammarpartLeftChild->
//            m_dwRightmostIndex ) ;
//          for( //c_iter_mmap_dw2grammarpart iterCurrent
//              c_iter_mmap_dw2p_grammarpart iterCurrent = stdpair_iter.first ;
//              iterCurrent != stdpair_iter.second ; ++ iterCurrent )
//          {
//            if( iterCurrent->second ==
//                p_grammarpartSuperordinate->mp_grammarpartLeftChild )
//          }

          //TODO runtime error in
    //      DirectingLeavesMultipleIterTraverser::
    //        GetNextRightGrammarPartNotProcessedYet()
          //when inserted into the map

//          multmap_token_index2grammarpt.insert(
//            std::pair< WORD, GrammarPart >
//            (
//    //          iter_stdmap_w2w->first ,
//              iter_mm_idx2grammarpt->first ,
//              grammarpartSuperordinate
//            )
//            ) ;
          //http://msdn.microsoft.com/en-us/library/1ka6hya8%28v=VS.100%29.aspx:
          //"Inserting elements invalidates no iterators."
//          r_multmap_token_index2p_grammarpt.insert(
//            std::pair< WORD, GrammarPart *>
//            (
//    //          iter_stdmap_w2w->first ,
//              iter_mm_token_idx2p_grammarpt->first ,
//              & grammarpartSuperordinate
//            )
//            ) ;
#ifdef _DEBUG
          std::string stdstrSuperordinate ;
          std::string stdstrSubclassName ;
          WORD wSuperordinateGrammarPartID =
            iter_stdmap_wGrammarPartID2wSuperordinateID->second ;
          ci_stdmap_wRuleID2stdstrRuleName = m_stdmap_wRuleID2RuleName.find(
            wSuperordinateGrammarPartID) ;
          ci_stdmap_wLeftGrammarPartID2stdstrRuleName =
              m_stdmap_wRuleID2RuleName.find(
                wGrammarPartID ) ;
//          GrammarPart & r_grammarpartLeft =
////              iter_mm_idx2grammarpt->second ;
//            * iter_mm_token_idx2p_grammarpt->second ;
          if( ci_stdmap_wRuleID2stdstrRuleName !=
              m_stdmap_wRuleID2RuleName.end()
            )
            stdstrSuperordinate = ci_stdmap_wRuleID2stdstrRuleName->second ;
          if( ci_stdmap_wLeftGrammarPartID2stdstrRuleName !=
                m_stdmap_wRuleID2RuleName.end()
            )
            stdstrSubclassName = ci_stdmap_wLeftGrammarPartID2stdstrRuleName->
              second ;
          {
//            DEBUG_COUTN( "superclass rule \"" << stdstrSuperordinate
//            << "("
//            //<< & grammarpartSuperordinate
//            << p_grammarpartSuperordinate
//            << ",ID=" << wSuperordinateGrammarPartID << ")"
//            <<  "\" found for "
//            //"the token range"
//            //<< dwLeftMostTokenIndexOfRule << "..."
//            //<< dwRightMostTokenIndexOfRule
//            << stdstrSubclassName
//            << "(address:" << & r_grammarpartLeft
//            << ",mmap index:" << dwMapIndex
//            << "ID=" << r_grammarpartLeft.m_wGrammarPartID << ")"
//            << r_grammarpartLeft.m_dwLeftmostIndex
//            << ","
//            << r_grammarpartLeft.m_dwRightmostIndex
//            << ";"
//            //<< "]"
//            ) ;
          }
#endif
//          //Insert if at the second map, not when at the first map!
//          //(else it is NOT inserted into the 2nd map)
//          if( bMemorizeInsertion //->lefmost token index map
//              )
          {
            InsertIntoSuperordinateGrammarPartContainer(p_grammarpartSuperordinate);
          }
//          else
          {
          }
          InsertIntoLeftmostAndRightmostTokenIndexContainersFor1ParseLevel(
            iter_mm_token_idx2p_grammarpt->first,
            p_grammarpartSuperordinate->mp_grammarpartLeftChild->
              m_dwRightmostIndex,
            p_grammarpartSuperordinate);
          bReplaced = true ;
        }
        else
          delete p_grammarpartSuperordinate ;
        //just replace the grammar part ID
//        iter_mm_idx2grammarpt->second.m_wGrammarPartID =
//            iter_stdmap_wGrammarPartID2wSuperordinateID->second ;
//        wGrammarPartID = iter_mm_idx2grammarpt->second.m_wGrammarPartID ;
        wGrammarPartID = iter_mm_token_idx2p_grammarpt->second->m_wGrammarPartID ;
      }
//    }while( bReplaced ) ;
//    ++ iter_mm_idx2grammarpt ;
    ++ iter_mm_token_idx2p_grammarpt ;
#ifdef _DEBUG
//    ++ dwMapIndex ;
#endif
  }
  LOGN_DEBUG("return " << bReplaced)
  return bReplaced ;
}

//Create new child(ren) becaus of that:
void BottomUpParser::PossiblyCreateNewLeaves(
  GrammarPart * p_grammarpart,
  GrammarPart * p_grammarpartLeftChild,
  GrammarPart * p_grammarpartRightChild
  )
{
  if( GrammarPartIDIsWordClass(p_grammarpartLeftChild->m_wGrammarPartID)
      //IsLeaf(p_grammarpartLeftChild)
      //&& p_grammarpartLeftChild->m_bAssignedAsChild
    )
  {
    //Must Create after the check "GrammarPartDoesNotAlreadyExist(...)"
    //,else endless loop.
    p_grammarpartLeftChild = new GrammarPart( *p_grammarpartLeftChild );
    p_grammarpart->SetLeftChild( * p_grammarpartLeftChild);
  }
  if( GrammarPartIDIsWordClass(p_grammarpartRightChild->m_wGrammarPartID)
      //IsLeaf(p_grammarpartLeftChild)
      //&& p_grammarpartRightChild->m_bAssignedAsChild
    )
  {
    //Must Create after the check "GrammarPartDoesNotAlreadyExist(...)"
    //,else endless loop.
    p_grammarpartRightChild = new GrammarPart( *p_grammarpartRightChild );
    p_grammarpart->SetRightChild( *p_grammarpartRightChild);
  }
}

bool BottomUpParser::ReplaceGrammarPartIDsBySuperordinate()
{
  LOGN_DEBUG("begin")
  bool bReplacedGrammarPartIDsBySuperordinate = false ;
  m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel.clear() ;
  m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel.clear() ;

  //  std::multimap<DWORD, GrammarPart>::iterator
//    iter_mm_rightmostidx2grammarptLeftGrammarPart =
//    .begin() ;
//  InsertSuperordinateGrammarPart( //m_stdmultimap_dwRightmostIndex2grammarpart,
//    m_stdmultimap_dwRightmostIndex2p_grammarpart ,
//    false ) ;
  bReplacedGrammarPartIDsBySuperordinate = InsertSuperordinateGrammarPart(
    //m_stdmultimap_dwLeftmostIndex2grammarpart
    m_stdmultimap_dwLeftmostIndex2p_grammarpart , true ) ;
  if( bReplacedGrammarPartIDsBySuperordinate )
  {
    LOGN_DEBUG("inserting whole map of superordinate into other map " )
    InsertIntoOverallLeftmostAndRightmostTokenIndexContainers(
      m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel,
      m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel                              
      );
  }
  LOGN_DEBUG("return " << bReplacedGrammarPartIDsBySuperordinate )
  return bReplacedGrammarPartIDsBySuperordinate ;
}

bool BottomUpParser::Resolve1ParseLevel()
{
  ++ m_dwMapIndex ;
  bool grammarRuleApplied = ResolveGrammarRules();

  bool bReplacedGrammarPartIDsBySuperordinate = 
    ReplaceGrammarPartIDsBySuperordinate();
  return (grammarRuleApplied || bReplacedGrammarPartIDsBySuperordinate);
}

/** Applies grammar rules/Minimizes, e.g. "article + noun" = "def_article_noun" */
BYTE BottomUpParser::ResolveGrammarRules(
  //Maintaining 2 maps with both leftmost and rightmost indexes should be faster
  //when searching for neighboured grammar parts:
  //  0    1       2      3      <-index
  //"the vacuum cleaner sucks."
  // +----------------+
  //   e.g. current grammar part: rightmost index: 2: find neighbour with
  //     leftmost index "3".

  //Use a multimap because at an index x more than 1 grammar part may exist.
  //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
  //"verb".
//  std::multimap<DWORD, GrammarPart> & r_stdmultimap_dwLeftmostIndex2grammarpart
//  , std::multimap<DWORD, GrammarPart> & r_stdmultimap_dwRightmostIndex2grammarpart
//  , std::multimap<DWORD, GrammarPart> &
//    r_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate
//  , std::multimap<DWORD, GrammarPart> &
//    r_stdmultimap_dwRightmostIndex2grammarpartSuperordinate
  )
{
  BYTE byGrammarRuleApplied = 0 ;
  m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel.clear() ;
  m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel.clear() ;
//  WORD wGrammarIDOfRule ;
//  m_iter_stdmap_wParseLevelIndex2stdmultimap_dwLeftmostIndex2grammarpart =
//    m_stdmap_wParseLevelIndex2stdmultimap_dwLeftmostIndex2grammarpart.insert(
//    m_dwMapIndex
//    ,
//    )
//  mp_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate =
//    & r_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate ;
//  mp_stdmultimap_dwRightmostIndex2grammarpartSuperordinate =
//    & r_stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;
//  std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator iter =
//    stdmap_wIndex2p_set_p_vocabularyandtranslation.begin() ;

  //The left grammar part is the one whos rightmost token index is the
  //neighbour of the right part's leftmost token index:
  // 1     2       3      4    <-index
  //the vacuum cleaner sucks.
  //    +-------------+
  //               ^
  //               |
  //      rightmost index
//  std::multimap<DWORD, GrammarPart>::iterator
//    iter_mm_rightmostidx2grammarptLeftGrammarPart =
//    r_stdmultimap_dwRightmostIndex2grammarpart.begin() ;
  std::multimap<DWORD, GrammarPart *>::iterator
    iter_mm_rightmostidx2p_grammarptLeftGrammarPart =
    m_stdmultimap_dwRightmostIndex2p_grammarpart.begin() ;
    //m_stdmultimap_dwRightmostIndex2grammarpart.begin() ;

//  if ( iter != stdmap_wIndex2p_set_p_vocabularyandtranslation.end()
//    )
  {
//    std::map <WORD, std::set<VocabularyAndTranslation *> *>::iterator
//      iterRightOf = iter ;
//    iterRightOf ++ ;
    while( //iter_mm_rightmostidx2grammarptLeftGrammarPart !=
//      r_stdmultimap_dwRightmostIndex2grammarpart.end()
      iter_mm_rightmostidx2p_grammarptLeftGrammarPart !=
      m_stdmultimap_dwRightmostIndex2p_grammarpart.end()
      )
    {
//      DEBUG_COUTN("current rightmost index:" <<
//          iter_mm_rightmostidx2grammarptLeftGrammarPart->first )
      //Find the direct right neighbuored grammar part.
//      iterRightGrammarPart = r_stdmapwLeftmostIndex2grammarpart.find(
//        iterLeftGrammarPart->first + 1 )
//      typedef std::multimap<DWORD, GrammarPart>::iterator
//        iter_mm_dword2grammarpart ;
//      std::pair< iter_mm_dword2grammarpart,iter_mm_dword2grammarpart>
//        //1st and last iterator having the same value within the multimap.
//        stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart =
//        r_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
//        //m_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
//        //Get all grammar parts that are direct neighbours of the left grammar
//        //part.
//        iter_mm_rightmostidx2grammarptLeftGrammarPart->first + 1 ) ;
//
//      //e.g. token 1: "the" -> article, token2: "man" -> noun
//      // article + noun = def_article_noun
//      //if( RulesAppliesTo( iter, iterLeftGrammarPart ) )
//      for( iter_mm_dword2grammarpart
//        //This iterator has all items that have the same leftmost token index
//        //that is _directly_ right of the rightmost token index of the
//        //current grammar part.
//        iter_wLeftmostIndex2grammarpartRightGrammarPart =
//        //Iterator with 1st key with searched value.
//        stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart.first ;
//        iter_wLeftmostIndex2grammarpartRightGrammarPart !=
//        //Iterator beyond last key with searched value.
//        stdpair1stAndLastRangeIterwLeftmostIndex2grammarpart.second ;
//        ++ iter_wLeftmostIndex2grammarpartRightGrammarPart )
//      {
//        DEBUG_COUTN("grammar part with leftmost index starting at:" <<
//          iter_mm_rightmostidx2grammarptLeftGrammarPart->first + 1 << ":"
//          << GetGrammarPartName(
//          iter_mm_rightmostidx2grammarptLeftGrammarPart->second.m_wGrammarPartID )
//          )

      typedef std::multimap<DWORD, GrammarPart *>::iterator
        iter_mm_dword2p_grammarpart ;
      std::pair< iter_mm_dword2p_grammarpart,iter_mm_dword2p_grammarpart>
        //1st and last iterator having the same value within the multimap.
        stdpair1stAndLastRangeIterwLeftmostIndex2p_grammarpart =
        m_stdmultimap_dwLeftmostIndex2p_grammarpart.equal_range(
        //m_stdmultimap_dwLeftmostIndex2grammarpart.equal_range(
        //Get all grammar parts that are direct neighbours of the left grammar
        //part.
        iter_mm_rightmostidx2p_grammarptLeftGrammarPart->first + 1 ) ;

      //e.g. token 1: "the" -> article, token2: "man" -> noun
      // article + noun = def_article_noun
      //if( RulesAppliesTo( iter, iterLeftGrammarPart ) )
      for( iter_mm_dword2p_grammarpart
        //This iterator has all items that have the same leftmost token index
        //that is _directly_ right of the rightmost token index of the
        //current grammar part.
        iter_wLeftmostIndex2p_grammarpartRightGrammarPart =
        //Iterator with 1st key with searched value.
        stdpair1stAndLastRangeIterwLeftmostIndex2p_grammarpart.first ;
        iter_wLeftmostIndex2p_grammarpartRightGrammarPart !=
        //Iterator beyond last key with searched value.
        stdpair1stAndLastRangeIterwLeftmostIndex2p_grammarpart.second ;
        ++ iter_wLeftmostIndex2p_grammarpartRightGrammarPart )
      {
//        if( GrammarRuleAppliesTo(
//           iterLeftGrammarPart->second.m_wGrammarPartID
//           , iterRightGrammarPart->second.m_wGrammarPartID
//           , wGrammarIDOfRule
//            )
//          )
//        {
//
//          //E.g. insert "def_article_noun".
//          //There may be more than 1 possibility to translate a text/ sentence.
//          //E.g. "the glasses are broken" : broken = "kaputt" oder Participle perfect
//          //of "to break" -> "gebrochen".
//          //So insert every possibility.
//          InsertIntoPossibilityTree(//dwIndexOf1stToken, dwIndexOfLastToken,
//            //iter, iterLeftGrammarPart
//            iterLeftGrammarPart
//            , iterRightGrammarPart
//            //Use numbers because they can be compared faster than strings.
//            //E.g. ID for "def_article_noun"
//            , wIDofGrammaticalItem ) ;
//        }
//        InsertIfGrammarRuleAppliesTo(
//          iter_mm_rightmostidx2grammarptLeftGrammarPart
//          , iter_wLeftmostIndex2grammarpartRightGrammarPart ) ;
        if( InsertIfGrammarRuleAppliesTo(
          iter_mm_rightmostidx2p_grammarptLeftGrammarPart
          , iter_wLeftmostIndex2p_grammarpartRightGrammarPart ) )
          byGrammarRuleApplied = 1 ;
      }
//      ++ iter_mm_rightmostidx2grammarptLeftGrammarPart ;
      ++ iter_mm_rightmostidx2p_grammarptLeftGrammarPart ;
    } //while
  }
  if( byGrammarRuleApplied )
  {
    InsertIntoOverallLeftmostAndRightmostTokenIndexContainers(
      m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate1ParseLevel,
      m_stdmultimap_dwRightmostIndex2p_grammarpartSuperordinate1ParseLevel);
  //    //parsebyrise.
  //    m_stdmultimap_dwLeftmostIndex2grammarpart.insert(
  //      stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.begin( ),
  //      stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.end( )
  //      );
  }
  ++ m_wParseLevel ;
  return byGrammarRuleApplied ;
}

void BottomUpParser::ResolveGrammarRulesForAllParseLevels()
{
  //Use a multimap because at an index x more than 1 grammar part may exist.
  //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
  //"verb".
  std::multimap<DWORD, GrammarPart> stdmultimap_dwLeftmostIndex2grammarpart ;
  std::multimap<DWORD, GrammarPart> stdmultimap_dwRightmostIndex2grammarpart ;
  WORD wstdmultimap_wLeftmostIndex2grammarpartSize ;
//  std::multimap<DWORD, GrammarPart> *
//    p_stdmultimap_dwLeftmostIndex2grammarpart =
//    & //parsebyrise.
//    m_stdmultimap_dwLeftmostIndex2grammarpart ;
//  std::multimap<DWORD, GrammarPart> *
//    p_stdmultimap_dwRightmostIndex2grammarpart =
//    & //parsebyrise.
//    m_stdmultimap_dwRightmostIndex2grammarpart ;
//
//  std::multimap<DWORD, GrammarPart> *
//    p_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate ;
//  std::multimap<DWORD, GrammarPart> *
//    p_stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;
//  std::multimap<DWORD, GrammarPart>
//    stdmultimap_dwLeftmostIndex2grammarpartSuperordinate ;
//  std::multimap<DWORD, GrammarPart>
//    stdmultimap_dwRightmostIndex2grammarpartSuperordinate ;

  std::multimap<DWORD, GrammarPart>
    stdmultimap_dwRightmostIndex2grammarpartEmpty ;

  std::map< DWORD, std::multimap<DWORD, GrammarPart> >
    map_stdmultimap_dwLeftmostIndex2grammarpart ;
  std::map< DWORD, std::multimap<DWORD, GrammarPart> >
    map_stdmultimap_dwRightmostIndex2grammarpart ;
//  DWORD dwSize ;
  m_dwMapIndex = 0 ;
  bool bReplacedGrammarPartIDsBySuperordinate ;
  BYTE byGrammarRuleApplied ;
  do //Replace by superordinate grammar part ID loop.
  {
  do //Apply grammar rule loop.
  {
//    stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.clear() ;
//    stdmultimap_dwRightmostIndex2grammarpartSuperordinate.clear() ;
    //Add the new applied rules/ grammar parts later (else there may be
    //problems with the multimap iterator inside the resolve method)
    wstdmultimap_wLeftmostIndex2grammarpartSize =
      stdmultimap_dwLeftmostIndex2grammarpart.size() ;
//    p_stdmultimap_dwRightmostIndex2grammarpartSuperordinate =
//      & (
//        map_stdmultimap_dwLeftmostIndex2grammarpart.insert(
//          std::pair< DWORD, std::multimap<DWORD, GrammarPart> >
//          ( (DWORD) parsebyrise.m_dwMapIndex
//            , stdmultimap_dwRightmostIndex2grammarpartEmpty
//          )
//        )
//      .first->second
//      ) ;
//    p_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate =
//      & (map_stdmultimap_dwRightmostIndex2grammarpart.insert(
//      std::pair< DWORD, std::multimap<DWORD, GrammarPart> >
//        ( (DWORD) parsebyrise.m_dwMapIndex
//          , stdmultimap_dwRightmostIndex2grammarpartEmpty
//        )
//      ).first->second
//      ) ;
    ++ //parsebyrise.
      m_dwMapIndex ;
//    ReplaceGrammarPartIDsBySuperordinate() ;
    //parsebyrise.ResolveGrammarRules(
    byGrammarRuleApplied = ResolveGrammarRules(
      //Maintaining 2 maps with both leftmost and rightmost indexes should be faster
      //when searching for neighboured grammar parts:
      //  0    1       2      3      <-index
      //"the vacuum cleaner sucks."
      // +----------------+
      //   e.g. current grammar part: rightmost index: 2: find neighbour with
      //     leftmost index "3".

//      //Use a multimap because at an index x more than 1 grammar part may exist.
//      //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
//      //"verb".
//      * p_stdmultimap_dwLeftmostIndex2grammarpart
//      , * p_stdmultimap_dwRightmostIndex2grammarpart
//      //, * p_stdmultimap_dwLeftmostIndex2grammarpartSuperordinate
//      , stdmultimap_dwLeftmostIndex2grammarpartSuperordinate
//      //, * p_stdmultimap_dwRightmostIndex2grammarpartSuperordinate
//      , stdmultimap_dwRightmostIndex2grammarpartSuperordinate
      ) ;

    #ifdef _DEBUG
//      dwSize = //parsebyrise.
////        m_stdmultimap_dwRightmostIndex2grammarpart.size() ;
//        m_stdmultimap_dwRightmostIndex2p_grammarpart.size() ;
//      DEBUG_COUTN("size of rightmost list token index multimap:" << dwSize )
//      dwSize = //parsebyrise.
////        m_stdmultimap_dwLeftmostIndex2grammarpart.size() ;
//        m_stdmultimap_dwLeftmostIndex2p_grammarpart.size() ;
//      DEBUG_COUTN("size of leftmost list token index multimap:" << dwSize )
////      dwSize = stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.size() ;
////      dwSize = stdmultimap_dwRightmostIndex2grammarpartSuperordinate.size() ;
    #endif
    //Join the grammar parts and the superordinate grammar parts, i.e.
    //for "the vacuum cleaner sucks" join the superordinate grammar part
    // "definite_article_noun" for
    //  -"the" (definite_article) +
    //  -"vacuum cleaner" (noun)
    // so that the next time the rule "definite_article_noun + verb = clause"
    // for "the vacuum cleaner sucks" can be applied.
    //parsebyrise.
//    m_stdmultimap_dwRightmostIndex2grammarpart.insert(
//      stdmultimap_dwRightmostIndex2grammarpartSuperordinate.begin( ),
//      stdmultimap_dwRightmostIndex2grammarpartSuperordinate.end( )
//      );
      LOGN_DEBUG("resolving grammar/ parse level " << m_dwMapIndex /*<< "\n"*/ )
  }
  //while no grammar rule for "left + right = parent" applied.
  while( //parsebyrise.m_stdmultimap_dwLeftmostIndex2grammarpart.size() >
      //wstdmultimap_wLeftmostIndex2grammarpartSize
    //! p_stdmultimap_dwRightmostIndex2grammarpartSuperordinate->empty()
//    ! stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.empty()
    byGrammarRuleApplied
    ) ;
  //e.g. now at "the car and the cat" ...
  //              \ /         \  /
  //          def_noun      def_noun
  //
  //...and no more grammar rules can be applied.
  //BUT there is a rule for "subj_or_obj_enum_ele" + "conj" =
  //  "subj_or_obj_enum_ele,conj"
  // and "subj_or_obj_enum_ele" is a superordinate grammar ID for "def_noun"


    bReplacedGrammarPartIDsBySuperordinate =
        ReplaceGrammarPartIDsBySuperordinate() ;
  }
  while( bReplacedGrammarPartIDsBySuperordinate );
  m_wParseLevel = 0 ;
  LOGN_DEBUG("end of resolving grammar" )
#ifdef _DEBUG
//  dwSize = //parsebyrise.
//    m_stdmultimap_dwRightmostIndex2grammarpart.size() ;
//  dwSize = //parsebyrise.
//    m_stdmultimap_dwLeftmostIndex2grammarpart.size() ;
//  dwSize = stdmultimap_dwLeftmostIndex2grammarpartSuperordinate.size() ;
//  dwSize = stdmultimap_dwRightmostIndex2grammarpartSuperordinate.size() ;
#endif
}

void BottomUpParser::InsertGrammarPartForEverySameWord(
//  const LetterNode * p_letternode,
  std::set<VocabularyAndTranslation *> * std_set_p_vocabularyandtranslation,
  DWORD dwTokenIndex, DWORD dwTokenIndexRightMost
  )
{
  #ifdef _DEBUG
//  std::string std_str = psv.GetBetweenAsStdString( dwTokenIndex,
//    dwTokenIndexRightMost);
//  DEBUG_COUT( "word found in dictionary: " << std_str << "\n" )
  #endif
//  std::set<VocabularyAndTranslation *> * psetpvocabularyandtranslation =
//    NULL ;
//  psetpvocabularyandtranslation = p_letternode->
//    m_psetpvocabularyandtranslation ;
  if( std_set_p_vocabularyandtranslation )
  {
  //      stdmap_wIndex2p_set_p_vocabularyandtranslation.insert(
  //        std::pair<WORD>, std::set<VocabularyAndTranslation *> *>
  //        (wTokenIndex,psetpvocabularyandtranslation) ) ;

    //GrammarPart grammarPart(
  //      //Via the indices the tokens can be got later.
  //      dwTokenIndex, dwTokenIndexRightMost) ;

    //GetVocabulariesContainingWordClass(psetpvocabularyandtranslation,
    //  setpvocabularyandtranslation);

    //Loop for all vocabularies having at least 1 word that
    //equals the current token.
    //There may be more than 1 vocabulary for the same word:
    //e.g. "love" may be either a noun ("the love") or a verb ("to love").
  #ifdef _DEBUG
    int nNumbersOfWords = std_set_p_vocabularyandtranslation->size();
  #endif
    for(std::set<VocabularyAndTranslation *>::iterator vocAndTranslIter =
      std_set_p_vocabularyandtranslation->begin() ; vocAndTranslIter !=
      std_set_p_vocabularyandtranslation->end() ; vocAndTranslIter ++ )
    {
      //Create a grammar part object for _every_ single
      //VocabularyAndTranslation object.
      //Problem with this:
      //if a grammar rule affects more than 1 grammar part, e.g.:
      //"sheep" (singular_noun) and "sheep" (plural_noun):
      //when translating "the sheep" and the translation rule then matches
      //"definite_article_singular.singular_plural" and the article is
      //translated to "die" it is overwritten by "das" from
      //"definite_article_singular.singular_noun" because the grammar part
      //is the same/ shared/ referenced by the two ones.
      //possible solution:
      //-store multiple translations per grammar part
      //-clone grammar part (branches).

      /** Must be freed manually.*/
      GrammarPart * p_grammarPart = new GrammarPart(
        //Via the indices the tokens can be got later.
        dwTokenIndex, dwTokenIndexRightMost) ;
  //        //word type
  //        (*vocAndTranslIter)->m_englishWordClass ;
  //        grammarPart.m_wGrammarPartID = (*vocAndTranslIter)->m_englishWordClass ;
  //        grammarPart.SetGrammarPartID( (*vocAndTranslIter)->m_englishWordClass ) ;
//      VocabularyAndTranslation * p_vocabularyandtranslation =
//        * vocAndTranslIter;
      VocabularyAndTranslation & r_vocabularyandtranslation =
        * ( * vocAndTranslIter);
      p_grammarPart->SetGrammarPartID( r_vocabularyandtranslation.m_englishWordClass ) ;
      //For accessing the vocabulary attributes later for translating.
  //        grammarPart.m_pvocabularyandtranslation =
      p_grammarPart->m_pvocabularyandtranslation = & r_vocabularyandtranslation ;
      //r_stdmapwLeftmostIndex2grammarpart.insert(
      //r_stdmultimap_wLeftmostIndex2grammarpart.insert(
  //        p_stdmultimap_wLeftmostIndex2grammarpart->insert(
  //          std::pair<WORD, GrammarPart>
  //            ( dwTokenIndex, grammarPart )
  //          ) ;
#ifdef _DEBUG
      unsigned vocAndTranslType = r_vocabularyandtranslation.m_englishWordClass;
      const std::string & std_strGrammarPartName = GetGrammarPartName(
        vocAndTranslType );
      LOGN_DEBUG("inserting "
        << std_strGrammarPartName
        << "(ID)=" << vocAndTranslType
        << " address: " << p_grammarPart
        << " for token range " << dwTokenIndex << ":" << dwTokenIndexRightMost
        )
#endif
      m_stdmultimap_dwLeftmostIndex2p_grammarpart.insert(
        std::pair<WORD, GrammarPart *>
          ( dwTokenIndex, p_grammarPart )
        ) ;
      //r_stdmapwRightmostIndex2grammarpart.insert(
      //r_stdmultimap_wRightmostIndex2grammarpart.insert(
  //        p_stdmultimap_wRightmostIndex2grammarpart->insert(
  //          std::pair<WORD, GrammarPart>
  //            ( dwTokenIndexRightMost , grammarPart )
  //          ) ;
      m_stdmultimap_dwRightmostIndex2p_grammarpart.insert(
        std::pair<WORD, GrammarPart *>
          ( dwTokenIndexRightMost , p_grammarPart )
        ) ;
    }
  }
}

/** Inserts for every word found in the dictionary its leftmost token index
* into a list and its rightmost token index (because a word may have more
* than 1 token, e.g. "vacuum cleaner") into another list.
* (words are grammar part at the leaves of the parse tree) */
void BottomUpParser::StoreWordTypeAndGermanTranslation(
//  PositionstdstringVector & psv
  PositionStringVector & c_r_positionStringVector
  , DWORD dwTokenIndex
//  , std::map <WORD, std::set<VocabularyAndTranslation *> *> &
//    stdmap_wIndex2p_set_p_vocabularyandtranslation
//  , std::vector<GrammarPart> & stdvector_grammarpart ;

  //Maintaining 2 maps with both leftnost and rightmost indexes should be faster
  //when searching for neighboured grammar parts:
  //  0    1       2      3      <-index
  //"the vacuum cleaner sucks."
  // +----------------+
  //   e.g. current grammar part: rightmost index: 2: find neighbour with
  //     leftmost index "3".
//  , std::map<DWORD, GrammarPart> & r_stdmapwLeftmostIndex2grammarpart
//  , std::map<DWORD, GrammarPart> & r_stdmapwRightmostIndex2grammarpart
  //Use a multimap because at an index x more than 1 grammar part may exist.
  //E.g. for "I love you.": love" can be grammar part "noun" or grammar part
  //"verb".
//  , std::multimap<DWORD, GrammarPart> & r_stdmultimap_wLeftmostIndex2grammarpart
//  , std::multimap<DWORD, GrammarPart> & r_stdmultimap_wRightmostIndex2grammarpart
  )
{
  //When using references the container was empty after leaving this function.
//  std::multimap<DWORD, GrammarPart> & r_stdmultimap_wLeftmostIndex2grammarpart
//    = m_stdmultimap_dwLeftmostIndex2grammarpart ;
//  std::multimap<DWORD, GrammarPart> & r_stdmultimap_wRightmostIndex2grammarpart
//    = m_stdmultimap_dwRightmostIndex2grammarpart ;

//  std::multimap<DWORD, GrammarPart> * p_stdmultimap_wLeftmostIndex2grammarpart
//    = & m_stdmultimap_dwLeftmostIndex2grammarpart ;
//  std::multimap<DWORD, GrammarPart> * p_stdmultimap_wRightmostIndex2grammarpart
//    = & m_stdmultimap_dwRightmostIndex2grammarpart ;
//  std::set<VocabularyAndTranslation *> setpvocabularyandtranslation ;
  
//#if _TEST_WITH_DICTREADER_AND_VOCACCESS_TYPE
  
  m_pInputText = & c_r_positionStringVector;
  IVocabularyInMainMem::voc_container_type * p_std_set_p_vocabularyandtranslation ;
  DWORD dwTokenIndexRightMost = dwTokenIndex ;
  DWORD dwTokenIndexRightMostUnknownToken = 0;
//  const LetterNode * p_letternode;
  bool b1UnknownGrammarPartPerToken = true;
  bool bUnknownTokenFound = false;
  do
  {
    LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "current token:\""
      << c_r_positionStringVector.at(dwTokenIndex).m_Str << "\"" )
    //p_letternode = //g_lettertree.searchAndReturnLetterNode( psv,
    const std::string & item =
      c_r_positionStringVector.at(dwTokenIndexRightMost).m_Str +
      " beginning from token index #" +
      convertToStdString(dwTokenIndexRightMost);
    
    m_r_translationProcess.SetStatus(
      VTrans::lookUpWordInDictBeginningFromTokenIndex,
      item.c_str()
      );
    p_std_set_p_vocabularyandtranslation =
      //TranslationControllerBase:://s_dictionary.//searchAndReturnLetterNode(
      //find(
//      m_r_translationcontrollerbase.
      s_dictReaderAndVocAccess.lookUpEnglishWord(
        c_r_positionStringVector,
      //If "vacuum cleaner" and wTokenIndex is "0" before the call it gets "1".
      dwTokenIndexRightMost );
    //If the word was found.
    if( /*p_letternode*/ p_std_set_p_vocabularyandtranslation )
    {
      InsertGrammarPartForEverySameWord(//p_letternode,
        p_std_set_p_vocabularyandtranslation,
        dwTokenIndex, dwTokenIndexRightMost);
    }
    else
    {
      dwTokenIndexRightMostUnknownToken = dwTokenIndexRightMost;
      bUnknownTokenFound= true;
      if( b1UnknownGrammarPartPerToken )
        break;
    }
  }
  while( ! /*p_letternode*/ p_std_set_p_vocabularyandtranslation
    && dwTokenIndexRightMost < c_r_positionStringVector.size() - 1
    && /*m_r_translationcontrollerbase.*/m_r_translationProcess.Continue() );
  if( //dwTokenIndexRightMostUnknownToken
      bUnknownTokenFound)
  {
    GrammarPart * p_grammarPart = new GrammarPart(
      //Via the indices the tokens can be got later.
      dwTokenIndex, //dwTokenIndexRightMost
      dwTokenIndexRightMostUnknownToken) ;
    p_grammarPart->SetGrammarPartID( EnglishWord::UnknownWord ) ;
    p_grammarPart->m_stdstrTranslation = c_r_positionStringVector.
      GetBetweenAsStdString(
      dwTokenIndex,
        //dwTokenIndexRightMost
      dwTokenIndexRightMostUnknownToken);
    m_stdmultimap_dwLeftmostIndex2p_grammarpart.insert(
      std::pair<WORD, GrammarPart *>
        ( dwTokenIndex, p_grammarPart )
      ) ;
    m_stdmultimap_dwRightmostIndex2p_grammarpart.insert(
      std::pair<WORD, GrammarPart *>
        ( //dwTokenIndexRightMost
        dwTokenIndexRightMostUnknownToken, p_grammarPart )
      ) ;
  }
//#endif //#if _TEST_WITH_DICTREADER_AND_VOCACCESS_TYPE
  
  //  DWORD dwSize = //parsebyrise.
//    m_stdmultimap_dwRightmostIndex2grammarpart.size() ;
//  dwSize = //parsebyrise.
//    m_stdmultimap_dwLeftmostIndex2grammarpart.size() ;
}
}
