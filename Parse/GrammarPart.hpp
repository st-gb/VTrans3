/*
 * GrammarPart.hpp
 *
 *  Created on: May 5, 2010
 *      Author: Stefan
 */

#ifndef GRAMMARPART_HPP_
#define GRAMMARPART_HPP_

#include <windef.h> //for BYTE, DWORD etc.
#include "Token.h" //class PositionStringVector
#include "Word.hpp" //class EnglishWord

//pre-declare (so no include of a file that declares this class->faster compilation)
class VocabularyAndTranslation ;

//class GrammarRule
//idea: model the syntax tree of the source text as a binary tree structure.
//e.g.:
//the vacuum cleaner  sucks.
//--------------clause---------
//              /     \
//-def_article_noun--- -main_verb
// /     \
//DA  ----noun------  DA=Definite Article
//
//e.g.:
//the car and the cat suck.
//--------------clause---------
//              /     \
//      EE_CJ_EE       \
//     /       |        \
//    /      CJ_EE      |
//   /       /   \      |
//--NC--  -CJ ---NC-- -main_verb   NC=def_article_noun CJ=ConJunction
//
//e.g.
//the car , the dog and the cat suck.
//--------------clause---------
//              /     \
//      EE_C_EE_CJ_EE
//            /     \
//      EE_C_EE      \           EE=Enumeration element
//     /     |        \
//    /    C_EE       CJ_EE      |    C=Comma
//   /     /   \      /   \      |
//--NC--- C ---NC-- -CJ ---NC-- main_verb   NC=def_article_noun CJ=ConJunction
//
//e.g.
//I  , he , it and the cat suck.
//--------------clause---------
//              /     \
//      EE_C_EE_CJ_EE
//            /     \
//     EE_C_EE      \           EE=Enumeration element
//     /  |
//    /  C_EE
//   /   /  \       \         PP=Personal Pronoun
//  / C_EE C_EE       CJ_EE      |    C=Comma
// / /  \  / \   /   \      |
//PP C PP C PP -CJ ---NC-- main_verb   NC=def_article_noun CJ=ConJunction
class GrammarPart
{
public:
  BYTE m_byPersonIndex ;
  //The region indexes are important for applying grammar rules:
  // 0     1      2     3  4     <-indexes of tokens
  //The vacuum cleaner is big.
  //      \       /
  //    leftmost: 1, rightmost: 2
  //  \     |
  //aricle noun  <-rule: article + noun = def_article_noun
  //
  //The vacuum cleaner
  //+----------------+
  //    def_article_noun: leftmost: 0, rightmost:2
  //
  DWORD m_dwLeftmostIndex ;
  DWORD m_dwRightmostIndex ;
  //for translation: refer to the subordinate elements.
  //e.g. when parsing: "the man" , rule: article + noun = def_article_noun:
  //def_article_noun->leftchild = article
  //def_article_noun->rightchild = noun
  GrammarPart * mp_grammarpartLeftChild ;
#ifdef COMPILE_WITH_POINTER_TO_PARENT
  GrammarPart * mp_grammarpartParent ;
#endif
  GrammarPart * mp_grammarpartRightChild ;
//  //Use strings to show grammar parts in the user interface as a feature.
//  std::string m_stdstrGrammarPartName ;
  //Important for translating into the destination language.
  VocabularyAndTranslation * m_pvocabularyandtranslation ;
  //Additionally use numbers corresponding to the rule name because they can
  //be compared faster than strings.
  WORD m_wGrammarPartID ;
  //for listboxes in GUI->same selection for multiple listboxes.
  WORD m_wConsecutiveID ;
//  VTrans::string_type m_vtrans_strTranslation ;
  std::string m_stdstrTranslation ;

  void AddLeftChild(GrammarPart & r_grammarpart) ;

  void AddRightChild(GrammarPart & r_grammarpart) ;

  void BuildTokenVector(
    const std::string & stdstrText
//    , PositionstdstringVector & psv
    , PositionStringVector & psv
    ) ;

//  std::string GetGrammarPartName()
//  {
//    return
//  }

  GrammarPart(DWORD dwTokenIndexLeftMost, DWORD dwTokenIndexRightMost ) ;

  GrammarPart(
    DWORD dwTokenIndexLeftMost,
    DWORD dwTokenIndexRightMost ,
    WORD wGrammarPartID ) ;

  //Code that should be executed by every constructor.
  void Init() ;

  //Define a < operator in order to insert into a container like std::set.
  bool operator < (const GrammarPart & ) const ;

//  inline
  void SetGrammarPartID(WORD wGrammarPartID ) ;

};

#endif /* GRAMMARPART_HPP_ */
