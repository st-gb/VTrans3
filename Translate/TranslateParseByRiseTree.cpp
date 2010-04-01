/*
 * TranslateParseByRiseTree.cpp
 *
 *  Created on: 25.02.2010
 *      Author: Stefan
 */

#include <Parse/ParseByRise.hpp>
#include <Translate/TranslateParseByRiseTree.h>
#include <VocabularyInMainMem/LetterTree/VocabularyAndTranslation.hpp>

#include <string>

//E.g. add "German_definite_article" , "der\ndie\ndas\ndie"
void TranslateParseByRiseTree::AddTranslationArray(
  const std::string & r_stdstrTranslationArrayName,
  const std::string & r_stdstrNewlineTerminatedArrayElements )
{
  std::string stdstrCurrentElement ;
  std::vector<std::string> vec ;
  WORD wStringStart = 0 ;
  WORD wNumberOfElements = 1 ;
  for( WORD i = 0 ; i < r_stdstrNewlineTerminatedArrayElements.length() ; ++ i )
  {
    if( r_stdstrNewlineTerminatedArrayElements[i] == '\n' || i ==
        r_stdstrNewlineTerminatedArrayElements.length() - 1 )
    {
      stdstrCurrentElement = r_stdstrNewlineTerminatedArrayElements.substr(
          wStringStart, i - wStringStart ) ;
      vec.push_back( stdstrCurrentElement ) ;
      ++ wNumberOfElements ;
      wStringStart = i + 1 ;
    }
  }
  m_stdmap_stdstrTranslationArrayName2vec_stdstrArrayElements.insert(
    std::pair<std::string,std::vector<std::string> >
      (r_stdstrTranslationArrayName, vec)
      );
}

//E.g.
//AddTranslationRule("subject.noun_construct.definite_article",
//  "German_definite_article[subject.noun_construct.noun.German_gender]")
//because all definite articles in a subject should be translated to the
// undeclinated? (Nominativ) versions "der", "die" , "das"

//AddTranslationRule("subject.noun_construct.definite_article" ,
//  "noun_construct.noun,translateAsSingular")

//e.g. "the news" (singular) is translated as "Nachrichten" (Plural)
//AddTranslationRule(
//  "subject.noun_construct.definite_article" ,
//  "noun_construct.noun.English.translateAsPlural=1" //English attribute
//  "noun_construct.noun.German.plural") //attribute of German word.

//AddTranslationRule(
//  "subject.noun_construct.noun.definite_article" ,
//  "noun.English.translateSingularAsPlural=1" //English attribute (condition)
//  "\"die\"") //attribute of German word.

//AddTranslationRule(
//  "subject.noun_construct.noun.definite_article" ,
//  "noun.English.isSingular=1" //English attribute (condition)
//  "noun.English.translateSingularAsPlural=0" //English attribute (condition)
//  "noun.German.gender=0" //German attribute (condition)
//  "\"der\"") //attribute of German word.
void TranslateParseByRiseTree::AddTranslationRule(
  const std::string & r_stdstrSyntaxTreePath ,
  //const std::string & r_stdstrEnglishAttributeNameAndValue ,
  const std::string & r_stdstrConditions ,
  const std::string & r_stdstrGermanTranslation
  //  subject.noun_construct.noun.German_gender]"
  )
{
  bool bNewEle = false ;
  std::string stdstrCurrentElement ;
  std::vector<WORD> vec_wElements ;
  WORD wStringStart = 0 ;
  WORD wGrammarPartID ;
  WORD wNumberOfElements = 1 ;
  for( WORD i = 0 ; i < r_stdstrSyntaxTreePath.length() ; ++ i )
  {
    if( i == r_stdstrSyntaxTreePath.length() - 1 )
    {
      stdstrCurrentElement = r_stdstrSyntaxTreePath.substr(
          wStringStart, ( i + 1 ) - wStringStart ) ;
      bNewEle = true ;
    }
    if( r_stdstrSyntaxTreePath[i] == '.' )
    {
      stdstrCurrentElement = r_stdstrSyntaxTreePath.substr(
          wStringStart, i - wStringStart ) ;
      bNewEle = true ;
    }
    if( bNewEle )
    {
      ++ wNumberOfElements ;
      wStringStart = i + 1 ;
      if( mp_parsebyrise->GetGrammarPartID( stdstrCurrentElement ,
          wGrammarPartID )
        )
      {
        vec_wElements.push_back(wGrammarPartID) ;
      }
      bNewEle = false ;
    }
  }
  WORD * ar_wElements = new WORD [ vec_wElements.size() ] ;
  if( ar_wElements )
  {
    WORD wIndex = 0 ;
    for( std::vector<WORD>::const_iterator iter = vec_wElements.begin() ;
        iter != vec_wElements.end() ; ++ iter )
    {
      //DEBUG("curr") ;
      ar_wElements[ wIndex ++ ] = *iter ;
    }
//    TranslationRule tr(ar_wElements, vec_wElements.size() ) ;
    //Must create on heap, else its d'tor is called.
    TranslationRule * p_tr = new TranslationRule(ar_wElements,
        vec_wElements.size() ) ;
    DEBUG_COUT( "added translation rule for syntax tree path \"" <<
        GetSyntaxTreePathAsName( ar_wElements, vec_wElements.size() ) <<
        "\"\n" )
    ConditionsAndTranslation c ;
#ifdef _DEBUG
    std::pair<std::map<TranslationRule*,ConditionsAndTranslation>::iterator,bool>
      _pair =
#endif
    m_stdmap_translationrule2ConditionsAndTranslation.insert(
      std::pair<TranslationRule *,ConditionsAndTranslation>
      ( //tr
        //Create here, else ar_wElements is deleted in TranslationRule d'tor?!
        //TranslationRule(ar_wElements, vec_wElements.size() )
        p_tr , c
      )
      ) ;
#ifdef _DEBUG
    if( _pair.second )
    {
      const TranslationRule & r_tr = *_pair.first->first ;
      DEBUG_COUT( "added translation rule for syntax tree path \"" <<
          GetSyntaxTreePathAsName( r_tr.m_ar_wElements, r_tr.m_wNumberOfElements ) <<
          "\"\n" )
    }
#endif
  }
}

//e.g. "noun.German.plural" , AttributeTypeAndPosAndSize::string ,
//   0, 0
void TranslateParseByRiseTree::AddVocAndTranslDefinition(
  const std::string & r_stdstrWordClass ,
  //e.g. "noun.German.plural"
  const std::string & r_stdstrWordClassAndAttributeName ,
  //->the definition either applies to English or German.
  BYTE byLanguage ,
  //attribute is a string or some bits
  BYTE byType ,
  //String or bit index
  WORD wIndex ,
  //bit length
  WORD wLenght
  )
{
  AttributeTypeAndPosAndSize a( byType, wIndex, wLenght) ;
  m_stdmap_AttrName2VocAndTranslAttrDef.insert(
    std::pair<std::string, AttributeTypeAndPosAndSize>
    (
    r_stdstrWordClassAndAttributeName
    , a
    )
    ) ;
}

TranslateParseByRiseTree::TranslateParseByRiseTree(
  ParseByRise & r_parsebyrise )
{
  // TODO Auto-generated constructor stub
  // e.g. "If I see you" ( adverb, clause)
  std::string strEnglish = "adverb_at_clause_begin , clause" ;

  //translates to: "Falls ich dich sehe." -> S O P
  std::string strGerman = "adverb_at_clause_begin\nGerman_subject\nobject\n"
    "predicate" ;
  mp_parsebyrise = & r_parsebyrise ;

  AddTranslationRule(
    "noun_construct.definite_article" ,
    "noun.English.isSingular=1" //English attribute (condition)
    , ""
  ) ;

//  AddVocAndTranslDefinition(
//    "noun",
//    AttributeTypeAndPosAndSize::German,
//    AttributeTypeAndPosAndSize::string,
//    1 ,
//    0
//    );

  //AddRule( "adverb_at_clause_begin" , "clause" )
  //AddTransformationRule(strEnglish, strGerman) ;

//      At first: get person index og subject:
//      personindex = GetPersonIndex( grammarpartSubject ) ;
//
//      //IDEA for transformation syntax: Access subtrees of the parse tree by a
//      //point notation/ operator
//      "<translationRule>"
//      "<clause>"
//        "<predicate.bendVerb[ subject.personIndex ]" ;
//      "</clause>"
//  "<noun_construct>" //noun_cobnstruct: definte article + noun
//    "<German_article[ German_noun.gender_id ]  German_noun[English_noun.sing_or_plural] " ;
//  "</noun_construct>"
//    "<definite_noun_construct>" //noun_cobnstruct: definte article + noun
//      "<German_def_article[ German_noun.gender ]  German_noun[English_noun.sing_or_plural] " ;
//    "</definite_noun_construct>"
//    "<indefinite_noun_construct>" //noun_cobnstruct: definte article + noun
//      "<German_indef_article[ German_noun.gender ]  German_noun[English_noun.sing_or_plural] " ;
//    "</indefinite_noun_construct>"
//
//    //"put" may be present or past: "he put the glasses on the table"->
//      //"Er stellt ..." or "Er stelltE..."
//      for( < possible_tenses.size() )
//      <clause.tense == PRESENT>
//      <predicate.flectiveverb[personindex]>
//
//      //man koennte das auch als DB-Abfrage machen:
//      //select ... where German_def_article.gender_id == German_noun.gender_id
//      // German_noun[English_noun.sing_or_plural]
//   "you love us": you: possible person indices: 2nd person sing.,
//
//  "out:predicate.bendVerb[ subject.personIndex ]" ;

  //AddTranslationArray("German_definite_article","der\ndie\ndas\ndie") ;
  //AddTranslationArray("German_definite_article_dativ","dem\nder\ndem\nden") ;
  //  ein kleinER Mann,  kleinE Frau ,  kleinES Kind
  //AddTranslationArray("German_nominativ_suffix","er\ne\nes");
  //AddTranslationRule("subject.noun_construct.adjective",
  //  "subject.noun_construct.adjective+German_nominativ_suffix[
  //  subject.noun_construct.noun.German_gender]")

  //AddTranslationRule("subject.noun_construct.adjective",
  //  "subject.noun_construct.adjective+German_nominativ_suffix[
  //  subject.noun_construct.noun.German_gender]")
}

//void TranslateParseByRiseTree::ApplyTransformationRules(
//    ParseByRise & r_parsebyrise)
//{
//
//}

std::string TranslateParseByRiseTree::GetSyntaxTreePathAsName(
  //ParseByRise & r_parsebyrise
  std::vector<WORD> & r_stdvec_wGrammarPartPath
  )
{
  std::string str ;
  if( mp_parsebyrise )
  {
    for( std::vector<WORD>::const_iterator iter =
      r_stdvec_wGrammarPartPath.begin() ;
        iter < r_stdvec_wGrammarPartPath.end() ; ++ iter )
    {
      str += mp_parsebyrise->GetGrammarPartName( *iter ) + "." ;
    }
  }
  return str ;
}

std::string TranslateParseByRiseTree::GetSyntaxTreePathAsName(
  //ParseByRise & r_parsebyrise
  WORD * ar_wGrammarPartPath, WORD wLength
  )
{
  std::string str ;
  if( mp_parsebyrise )
  {
    DEBUG_COUT( "GetSyntaxTreePathAsName--length: " << wLength << "\n" )
    for( WORD wIndex = 0 ; wIndex < wLength ; ++ wIndex )
    {
      DEBUG_COUT( "GetSyntaxTreePathAsName--element: " <<
          ar_wGrammarPartPath[ wIndex ] <<
          mp_parsebyrise->GetGrammarPartName(ar_wGrammarPartPath[ wIndex ])
          << "\n" )
      str += mp_parsebyrise->GetGrammarPartName( ar_wGrammarPartPath[ wIndex ] )
        + "." ;
    }
  }
  return str ;
}

void TranslateParseByRiseTree::Translate( ParseByRise & r_parsebyrise )
{
  ParseByRise * mp_parsebyrise = & r_parsebyrise ;
  std::cout << "Translate(): \n";
  if( mp_parsebyrise )
  {
    DWORD dwLeftMostTokenIndex = 0 ;
    GrammarPart * p_grammarpart ;
    GrammarPart * p_grammarpartChild ;
    //int y = 10 ;
    std::multimap<DWORD, GrammarPart>::const_iterator citer ;
    std::multimap<DWORD, GrammarPart> &
      r_stdmultimap_dwLeftmostIndex2grammarpart = mp_parsebyrise->
      m_stdmultimap_dwLeftmostIndex2grammarpart ;
    //Before each draw in order to begin at x position "0".
    m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.clear() ;
    m_wParseLevel = 0 ;
    std::cout << "Translate(): mp_parsebyrise != NULL\n" ;
    citer = r_stdmultimap_dwLeftmostIndex2grammarpart.begin() ;
    p_grammarpart = mp_parsebyrise->GetGrammarPartCoveringMostTokens(
        dwLeftMostTokenIndex ) ;
    if( p_grammarpart )
    {
      std::cout << "Translate: GetGrammarPartCoveringMostTokens found \n" ;
//      const std::string & r_stdstrGrammarPartName = //citer->second.
//        //m_stdstrGrammarPartName ;
//        mp_parsebyrise->GetGrammarPartName( p_grammarpart->m_wGrammarPartID ) ;
//      wxpaintdc.DrawText( //citer->second.m_stdstrGrammarPartName
//        r_stdstrGrammarPartName
//         , wxPoint(200,0) ) ;
      m_stdvec_wGrammarPartPath.push_back( p_grammarpart->m_wGrammarPartID ) ;
      ++ m_wParseLevel ;
      std::cout << "current syntax tree path:" << GetSyntaxTreePathAsName(
        //mp_parsebyrise
        m_stdvec_wGrammarPartPath ) << "\n" ;
      do
      {
        std::cout << "current syntax tree path:" << GetSyntaxTreePathAsName(
          //r_parsebyrise
            m_stdvec_wGrammarPartPath ) << "\n" ;
        //DrawGrammarPartChildren( p_grammarpart , wxpaintdc ) ;
        if( p_grammarpart->mp_grammarpartRightChild )
         {
          p_grammarpartChild = p_grammarpart->mp_grammarpartRightChild ;
//           std::cout << "pushinG "
//             << mp_parsebyrise->GetGrammarPartName(
//               //p_grammarpart->mp_grammarpartRightChild->
//               p_grammarpartChild->m_wGrammarPartID )
//             << p_grammarpartChild
//             << " current size="
//             << m_stdvecNodesToProcess.size()
//             << "\n" ;
           //Add to process possible children of the right nodes later.
           m_stdvecNodesToProcess.push_back( //p_grammarpart->mp_grammarpartRightChild
             p_grammarpartChild ) ;
         }
        if( p_grammarpart->mp_grammarpartLeftChild )
        {
          p_grammarpart = p_grammarpart->mp_grammarpartLeftChild ;
          m_stdvec_wGrammarPartPath.push_back(
              p_grammarpart->m_wGrammarPartID ) ;
//          std::cout << "current syntax tree path:" << GetSyntaxTreePathAsName(
//            r_parsebyrise ) << "\n" ;
          ++ m_wParseLevel ;
        }
        else //No further child->Left _leave_ of parse tree found.
        {
          std::cout << "Translate(): current grammar part type:" <<
              mp_parsebyrise->GetGrammarPartName(
              p_grammarpart->m_wGrammarPartID ) << " current size=" <<
                  m_stdvecNodesToProcess.size() << "\n" ;
          //ApplyTranslationRule( ) ;
          if( TranslationRuleApplies() )
            std::cout << "translation rule applies\n" ;
          switch( p_grammarpart->m_wGrammarPartID )
          {
          case EnglishWord::noun:
            std::cout << "translation:" <<
              p_grammarpart->m_psetpvocabularyandtranslation->
              m_arstrGermanWord[0] <<
              "\n" ;
          }
          m_stdvec_wGrammarPartPath.pop_back() ;
          //std::vector<GrammarPart *>::iterator iter =
          //Assign previously stored right child.
          p_grammarpart =
            m_stdvecNodesToProcess.
            //"Returns a read/write reference to the data at the last
            //*  element of the %vector."
            back() ;
//          std::cout << "current syntax tree path:" << GetSyntaxTreePathAsName(
//            r_parsebyrise ) << "\n" ;

          m_stdvec_wGrammarPartPath.push_back(
              p_grammarpart->m_wGrammarPartID ) ;
          //p_grammarpart = *iter ;
          std::cout << "Translate(): popping " <<
              mp_parsebyrise->GetGrammarPartName(
              p_grammarpart->m_wGrammarPartID ) << " current size=" <<
                  m_stdvecNodesToProcess.size() << "\n" ;
//          DrawGrammarPartName( *iter , wxpaintdc) ;
          m_stdvecNodesToProcess.pop_back() ;
          //std::cout.flush() ;
          //usleep(100000) ;
          -- m_wParseLevel ;
        }
      }
      while( ! m_stdvecNodesToProcess.empty() ) ;
    }
  }
}

bool TranslateParseByRiseTree::TranslationRuleApplies( )
{
  bool bIdentical = false ;
  DEBUG_COUT("TranslationRuleApplies--beg\n")
//   ;
//  m_stdvec_wGrammarPartPath ;
  for( std::map<TranslationRule * ,ConditionsAndTranslation>::const_iterator
      iter = m_stdmap_translationrule2ConditionsAndTranslation.begin() ;
      iter != m_stdmap_translationrule2ConditionsAndTranslation.end() ;
      ++ iter )
  {
    //E.g. m_stdvec_wGrammarPartPath is
    // "clause.subject.noun_construct.noun.definite_article", so it has the
    //  elements
    //  -"clause", "subject", "noun_construct", "noun", "definite_article",
    //  and so the size is 5.
    //now compare it e.g. to iter ( element of
    //  m_stdmap_translationrule2ConditionsAndTranslation ) that may be
    // "subject.noun_construct.noun.definite_article", i.e. a subtree of
    // m_stdvec_wGrammarPartPathw with just 4 elements.
    if ( m_stdvec_wGrammarPartPath.size() >= iter->first->m_wNumberOfElements )
    {
      //Use a pointer, else the TranslationRule destructor is called.
      const TranslationRule * p_tr = iter->first ;
      WORD wLenghtDiff = m_stdvec_wGrammarPartPath.size() -
          iter->first->m_wNumberOfElements ;
      bIdentical = true ;
#ifdef _DEBUG
      std::string stdstr = GetSyntaxTreePathAsName(m_stdvec_wGrammarPartPath ) ;
      std::string stdstr2 = GetSyntaxTreePathAsName(
          p_tr->m_ar_wElements, p_tr->m_wNumberOfElements ) ;
      std::cout << "Comparing " << stdstr << " and " << stdstr2 << "\n" ;
#endif
//      std::vector<WORD>::const_iterator c_iter_wGrammarPartPath =
//          m_stdvec_wGrammarPartPath ;
      //DEBUG_COUT
      //Compare from end to begin.
      for( WORD wIndex = p_tr->m_wNumberOfElements - 1 ;
          wIndex != MAXWORD  ; -- wIndex )
      {
        DEBUG_COUT("comparing " << p_tr->m_ar_wElements [ wIndex ] <<
          mp_parsebyrise->GetGrammarPartName(p_tr->m_ar_wElements [ wIndex ] )
          << " and "
          << m_stdvec_wGrammarPartPath.at(wIndex + wLenghtDiff )
          << mp_parsebyrise->GetGrammarPartName(
              m_stdvec_wGrammarPartPath.at(wIndex + wLenghtDiff ) )
          << "\n"
          )
        if( p_tr->m_ar_wElements [ wIndex ] !=
          m_stdvec_wGrammarPartPath.at(wIndex + wLenghtDiff )
          )
        {
          bIdentical = false ;
          DEBUG_COUT("TranslationRuleApplies--not identical\n")
          break ;
        }
      }
      if( bIdentical )
      {
        const ConditionsAndTranslation & r_cnt = iter->second ;
        int i = 0 ;
        //TODO implement this
//        if( r_cnt.conditions.m_byCompareType == ConditionsAndTranslation::equals
//          && AttributeTypeAndPosAndSize::English && rightof("==") equals(
//          grammarpart.string &&
//          mp_parsebyrise->psv.getTokens(
//              grammarpart.index, index2) )

      }
      DEBUG_COUT("TranslationRuleApplies--after loop\n")
    }
  }
  DEBUG_COUT("TranslationRuleApplies--end\n")
  return bIdentical ;
}

TranslateParseByRiseTree::~TranslateParseByRiseTree()
{
  DEBUG_COUT("begin of ~TranslateParseByRiseTree\n")
  for( std::map<TranslationRule *,ConditionsAndTranslation>::const_iterator
      it = m_stdmap_translationrule2ConditionsAndTranslation.begin() ; it !=
      m_stdmap_translationrule2ConditionsAndTranslation.end() ; ++ it )
  {
    //Release mem.
    delete it->first ;
  }
  DEBUG_COUT("end of ~TranslateParseByRiseTree\n")
}
