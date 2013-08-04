#ifndef BISON_TOKENIZER
# define BISON_TOKENIZER

#include <ctype.h>
//#include "../Token.h" //for PositionCStringVector
#include "../VocabularyInMainMem/LetterTree/LetterTree.hpp" //for VocabularyAndTranslation
#include <set> //for std::set<VocabularyAndTranslation *>
//for b4_namespace::BisonEnglishParser::token::yytokentype
//#include "simple_clause_c_pp.tab.cpp" 
//#include "../Token.h"
#include "simple_clause_c_pp.tab.hpp" //for class BisonEnglishParser

class LetterTree ;
//class b4_namespace::BisonEnglishParser ;
extern LetterTree g_lettertree ;

PositionstdstringVector g_posstdstrvec ;
DWORD g_dwTokenIndex ;
std::set<VocabularyAndTranslation *> g_setpvocabularyandtranslation ;

using namespace b4_namespace ;
typedef b4_namespace::BisonEnglishParser::token token;
//typedef BisonEnglishParser::token token;

namespace b4_namespace
{
  class BisonEnglishParser ;
//typedef BisonEnglishParser::token token;

}


int yylex (void)
{
 //int c;
 char ch ;
 std::string str ;

 /* Skip white space.  */
 /*while ((c = getchar ()) == ' ' || c == '\t')
   ;*/
   
 /* Process numbers.  */
 /*if (c == '.' || isdigit (c))
   {
     ungetc (c, stdin);
     scanf ("%lf", &yylval);
     return NUM;
   }*/
 /* Return end-of-input.  */
 /*if (c == EOF)
   return 0;
 while (!istream.eof() )
 {
   istream>>ch ;
   if(ch == ' ' || ch == '\t')
     if( !str.isEmpty() )
     {
       DWORD dwTokenIndex ;
       std::set<VocabularyAndTranslation *> 
         setpvocabularyandtranslation ;
       //This should be textually after EOF if-branch
       if( lettertree.isSingular(
         posstdstrvec, dwTokenIndex ,
         setpvocabularyandtranslation ) )
       {
         //enumele.push_back( setpvocabularyandtranslation )
         return SINGULAR_NOUN ;
       }
       str.empty() ;
     }
   else
   {
     str += ch ;
   }
 }*/
 if( g_lettertree.IsPlural(
   g_posstdstrvec, g_dwTokenIndex ,
   g_setpvocabularyandtranslation ) )
 {
   //enumele.push_back( setpvocabularyandtranslation )
   return token::SINGULAR_NOUN ;
 }
 if( g_lettertree.IsSingular(
   g_posstdstrvec, g_dwTokenIndex ,
   g_setpvocabularyandtranslation ) )
 {
   //enumele.push_back( setpvocabularyandtranslation )
   return b4_namespace::BisonEnglishParser::token::PLURAL_NOUN ;
 }
 
 /* Return a single char.  */
 return 0;
}

//} //namespace b4_namespace

#endif BISON_TOKENIZER
