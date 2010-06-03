/*
 * wxHTMLfileOutput.cpp
 *
 *  Created on: Jun 2, 2010
 *      Author: Stefan
 */
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/file.h>

#include "wxHTMLfileOutput.hpp"
#include <Translate/TranslateTreeTraverser.hpp>
#include <set>

wxHTMLfileOutput::wxHTMLfileOutput(
//  const std::vector<std::vector<TranslationAndConsecutiveID> > &
//    cr_stdvec_stdvecTranslationAndConsecutiveID
    const std::vector<std::vector<TranslationAndGrammarPart> > &
      cr_stdvec_stdvecTranslationAndGrammarPart
 )
// : mcr_stdvec_stdvecTranslationAndConsecutiveID (
//   cr_stdvec_stdvecTranslationAndConsecutiveID)
: mcr_stdvec_stdvecTranslationAndGrammarPart(
  cr_stdvec_stdvecTranslationAndGrammarPart)
{

}

void wxHTMLfileOutput::GetTokenIndicesForConsecutiveID(
  WORD wConsecutiveID ,
  WORD wTokenIndexToExclude ,
  std::set<WORD> & r_stdset_wTokenIndex
  )
{
  const GrammarPart * p_gp ;
  WORD wTokenIndex ;
  WORD wTranslationPossibilityIndex = 0 ;
  //loop for every possibility.
//  for( std::vector<std::vector<TranslationAndConsecutiveID> >::
  for( std::vector<std::vector<TranslationAndGrammarPart> >::
      const_iterator c_iter_vec_vecTransl_n_Cons =
//      mcr_stdvec_stdvecTranslationAndConsecutiveID.begin()
      mcr_stdvec_stdvecTranslationAndGrammarPart.begin()
//      , wTranslationPossibilityIndex = 0
      ;
      c_iter_vec_vecTransl_n_Cons !=
//        mcr_stdvec_stdvecTranslationAndConsecutiveID.end() ;
      mcr_stdvec_stdvecTranslationAndGrammarPart.end() ;
      ++ c_iter_vec_vecTransl_n_Cons , ++ wTranslationPossibilityIndex )
  {
    wTokenIndex = 0 ;
    //Loop for every token.
//    for( std::vector<TranslationAndConsecutiveID>::const_iterator
    for( std::vector<TranslationAndGrammarPart>::const_iterator
        c_iter_vecTransl_n_Cons =
            c_iter_vec_vecTransl_n_Cons->begin()
//        , wTokenIndex = 0
        ;
        c_iter_vecTransl_n_Cons !=
            c_iter_vec_vecTransl_n_Cons->end() ;
        ++ c_iter_vecTransl_n_Cons , ++ wTokenIndex )
    {
//      if( c_iter_vecTransl_n_Cons->m_wConsecutiveID == wConsecutiveID
      p_gp = c_iter_vecTransl_n_Cons->mp_grammarpart ;
      if( p_gp && p_gp->m_wConsecutiveID == wConsecutiveID
          && wTokenIndex != wTokenIndexToExclude )
      {
        r_stdset_wTokenIndex.insert(wTokenIndex) ;
      }
    }
  }
}

void wxHTMLfileOutput::writeFile(
//  const std::vector<std::vector<TranslationAndConsecutiveID> > &
//    cr_stdvec_stdvecTranslationAndConsecutiveID
  const std::vector<std::vector<TranslationAndGrammarPart> > &
    cr_stdvec_stdvecTranslationAndGrammarPart
  , wxString wxstrFile
  )
{
//   cr_stdvec_stdvecTranslationAndConsecutiveID ;
  {
    bool bStringContained = false ;
    wxString wxstrHTML = wxT("<html><body>\n") ;
    wxstrHTML += "<head>\n" ;
    wxstrHTML += "<script type=\"text/javascript\">\n" ;
    //http://de.selfhtml.org/javascript/intro.htm:
    //"Wenn Sie dies verhindern möchten, sollten Sie den JavaScript-Code
    //zwischen <script type="text/javascript"> und </script>  in einen Seite
    //HTML-Kommentar  einschließen."
    wxstrHTML += "<!--\n"
        "function selected_options_to_textarea() {\n "
        //"document.transl.transl_text.value = \"lllsd\" ;\n "
        " strAllToken = \"\";\n"
        "for( var i=0; i<document.formTranslationsSelects.elements.length ; i ++ )\n"
        "{\n"
          "strAllToken += "
//          "document.transl.select0.options["
//          "document.transl.select0.selectedIndex].value ;\n "
          "document.formTranslationsSelects.elements[i].value + \" \" ;\n"
        "}\n"
        "document.formTextArea.textareaTranslatedText.value = strAllToken;\n"
        //"alert(\"Das Quadrat von \" ); \n"
//        "alert(\"Das Quadrat von \" ); \n"
    "}\n"
    "//-->\n"
    "</script>\n" ;
    wxstrHTML += "</head>\n" ;
    WORD wIndex = 0 ;
    std::set<std::string> stdset_stdstr ;
    WORD wStringLength ;
    std::string::size_type stdstr_sizeFindPos ;
    std::string stdstrToken ;
    wxstrHTML += "<select name=\"t\" size=\"2\">\n" ;
    wxstrHTML += "<option>" ;
    wxstrHTML += "test" ;
    wxstrHTML += "</option>\n" ;
    wxstrHTML += "</select>\n" ;

    wxstrHTML += "<form name=\"formTranslationsSelects\" action=\"\">\n" ;
    //Important break cond.
    bStringContained = false ;
    wIndex = 0 ;

//    std::vector<TranslationAndConsecutiveID>::const_iterator ar_c_iter_vec [
//      cr_stdvec_stdvecTranslationAndConsecutiveID.size() ] ;
    std::vector<TranslationAndGrammarPart>::const_iterator ar_c_iter_vec [
      cr_stdvec_stdvecTranslationAndGrammarPart.size() ] ;
    //loop for every possibility.
//    for( std::vector<std::vector<TranslationAndConsecutiveID> >::
    for( std::vector<std::vector<TranslationAndGrammarPart> >::
        const_iterator c_iter_vec_vecTransl_n_Cons =
//        cr_stdvec_stdvecTranslationAndConsecutiveID.begin() ;
        cr_stdvec_stdvecTranslationAndGrammarPart.begin() ;
        c_iter_vec_vecTransl_n_Cons !=
//          cr_stdvec_stdvecTranslationAndConsecutiveID.end() ;
        cr_stdvec_stdvecTranslationAndGrammarPart.end() ;
        ++ c_iter_vec_vecTransl_n_Cons , ++ wIndex )
    {
      ar_c_iter_vec[wIndex] = c_iter_vec_vecTransl_n_Cons->begin() ;
    }
    const GrammarPart * p_grammarpart ;
    WORD wTokenIndex = 0 ;
    WORD wTranslationPossibilityIndex ;
    WORD wConsecutiveID ;
    std::string stdstrWholeTranslation ;
    bool bMultipleOptions = false ;
    std::set<WORD> stdset_wTokenIndex ;
    do
    {
      stdset_wTokenIndex.clear() ;
      stdset_stdstr.clear() ;
      wTranslationPossibilityIndex = 0 ;
      //loop for every possibility.
//      for( std::vector<std::vector<TranslationAndConsecutiveID> >::
      for( std::vector<std::vector<TranslationAndGrammarPart> >::
          const_iterator c_iter_vec_vecTransl_n_Cons =
//          cr_stdvec_stdvecTranslationAndConsecutiveID.begin()
          cr_stdvec_stdvecTranslationAndGrammarPart.begin()
//          , wTranslationPossibilityIndex = 0
          ;
          c_iter_vec_vecTransl_n_Cons !=
//            cr_stdvec_stdvecTranslationAndConsecutiveID.end() ;
          cr_stdvec_stdvecTranslationAndGrammarPart.end() ;
          ++ c_iter_vec_vecTransl_n_Cons , ++ wTranslationPossibilityIndex )
      {
        if( ar_c_iter_vec[wTranslationPossibilityIndex] !=
            c_iter_vec_vecTransl_n_Cons->end() )
        {
//          wConsecutiveID = ar_c_iter_vec[wTranslationPossibilityIndex]->
//              m_wConsecutiveID ;
          p_grammarpart = ar_c_iter_vec[wTranslationPossibilityIndex]->
              mp_grammarpart ;
          if( p_grammarpart )
          {
            wConsecutiveID = p_grammarpart->m_wConsecutiveID ;
            if( wConsecutiveID != MAXWORD )
            {
              GetTokenIndicesForConsecutiveID(wConsecutiveID, wTokenIndex ,
                stdset_wTokenIndex ) ;
            }
          }
  #ifdef _DEBUG
          const std::string & r_stdstrToken =
              ar_c_iter_vec[wTranslationPossibilityIndex]->m_stdstrTranslation ;
          stdset_stdstr.insert( r_stdstrToken ) ;
  #else
          stdset_stdstr.insert( ar_c_iter_vec[wTranslationPossibilityIndex]->
            m_stdstrTranslation ) ;
  #endif
          ++ ar_c_iter_vec[wTranslationPossibilityIndex] ;
        }
      }
      if( stdset_stdstr.size() > 0 )
      {
        stdstrWholeTranslation += *stdset_stdstr.begin() + " " ;
        if( stdset_stdstr.size() > 1 )
        {
          bMultipleOptions = true ;
          //http://de.selfhtml.org/html/formulare/auswahl.htm:
          //"Jede Auswahlliste sollte einen internen Bezeichnernamen erhalten,
          //und zwar mit dem Attribut name."
          wxstrHTML += "<select " ;
          wxstrHTML += wxString::Format("name=\"select%u\" ", wTokenIndex ) ;
          //Get all token indices that where the selection should be
          //connected to this token index.
          if( stdset_wTokenIndex.size() > 0 )
          {
            wxstrHTML += "onchange=\"" ;
            for( std::set<WORD>::const_iterator c_iter =
                stdset_wTokenIndex.begin() ;
                c_iter != stdset_wTokenIndex.end() ; ++ c_iter )
            {
              wxstrHTML += "document.formTranslationsSelects.select" +
                  wxString::Format("%u", *c_iter) +
                  ".selectedIndex = this.selectedIndex"
                  " ; selected_options_to_textarea()"
                  "\""
                  ;
            }
//            wxstrHTML += "alert(\"hhh\")\"" ;
//            wxstrHTML += "alert(this.selectedIndex)\"" ;
//            wxstrHTML += "this.selectedIndex=1\"" ;
          }
          wxstrHTML += " size=1>\n" ;
          for( std::set<std::string>::const_iterator c_iterToken =
              stdset_stdstr.begin() ;
              c_iterToken != stdset_stdstr.end() ;
              ++ c_iterToken
              )
          {
            wxstrHTML += "<option>" ;
            wxstrHTML += *c_iterToken ;
            wxstrHTML += "</option>\n" ;
          }
          wxstrHTML += "</select>\n" ;
        }
        else
        {
          wxstrHTML += *stdset_stdstr.begin() ;
          wxstrHTML += "<select " ;
          wxstrHTML += wxString::Format("name=\"select%u\" ", wTokenIndex ) ;
          wxstrHTML += " style=\"visibility:hidden;\" >\n" ;
          wxstrHTML += "<option>" ;
          wxstrHTML += *stdset_stdstr.begin() ;
          wxstrHTML += "</option>\n" ;
          wxstrHTML += "</select>" ;
        }
      }
      ++ wTokenIndex ;
    }while( ! stdset_stdstr.empty() ) ;
    wxstrHTML += "</form>\n" ;

    wxstrHTML +=
      "<p>\n"
      "<form name=\"formTextArea\" action=\"\">\n" ;
    if( bMultipleOptions )
    {
      wxstrHTML +=
        "<input type=\"button\" name=\"option_to_textfield\" "
          "value=\"to textfield\"onclick="
          "\""
  //        "this.form.transl_text.value='sas'"
            "selected_options_to_textarea()"
          "\""
        ">\n" //end of <input>
        "<br>\n"
        ;
    }
    wxstrHTML +=
      "<textarea cols=\"20\" rows=\"4\" name=\"textareaTranslatedText\">\n" ;
//    if( ! bMultipleOptions )
//    {
      wxstrHTML += stdstrWholeTranslation ;
//    }
    wxstrHTML +=
      "</textarea>\n"
      "</form>"
      "</p>"
      ;
    wxstrHTML += wxT(
      "</body>\n"
      "</html>") ;
  //    mp_wxhtmlwindow->SetPage(wxstrHTML) ;
    wxFile wxfileHTML( wxstrFile , wxFile::write ) ;
    wxfileHTML.Write(wxstrHTML) ;
    wxfileHTML.Close() ;
  }
}

void wxHTMLfileOutput::writeFile(
  std::vector<std::string> & stdvec_stdstrWholeTransl
  , wxString wxstrFile
  )
{
  if( stdvec_stdstrWholeTransl.size() > 0 )
  {
    bool bStringContained = false ;
    DWORD ardwFindPos[stdvec_stdstrWholeTransl.size()] ;
    memset(ardwFindPos,0, sizeof(DWORD)* stdvec_stdstrWholeTransl.size() ) ;
    wxString wxstrHTML = wxT("<html><body>") ;
    WORD wIndex = 0 ;
    std::set<std::string> stdset_stdstr ;
    WORD wStringLength ;
    std::string::size_type stdstr_sizeFindPos ;
    std::string stdstrToken ;
    wxstrHTML += "<select name=\"t\" size=\"2\">\n" ;
    wxstrHTML += "<option>" ;
    wxstrHTML += "test" ;
    wxstrHTML += "</option>\n" ;
    wxstrHTML += "</select>\n" ;

    do
    {
      //Important break cond.
      bStringContained = false ;
      wIndex = 0 ;
      for( std::vector<std::string>::const_iterator c_iter_stdvec_stdstr =
          stdvec_stdstrWholeTransl.begin() ;
          c_iter_stdvec_stdstr != stdvec_stdstrWholeTransl.end()
          ; ++ c_iter_stdvec_stdstr
          )
      {
        stdstr_sizeFindPos = c_iter_stdvec_stdstr->find(' ',
          ardwFindPos[wIndex] ) ;
        if( stdstr_sizeFindPos != std::string::npos )
        {
          bStringContained = true ;
          wStringLength = stdstr_sizeFindPos - ardwFindPos[wIndex]
  //            // if e.g. found at pos 0 starting from 0, we have to add to
  //            // string size 1.
  //            + 1
            ;
  #ifdef _DEBUG
          stdstrToken = c_iter_stdvec_stdstr->substr(
            ardwFindPos[wIndex] , wStringLength ) ;
          stdset_stdstr.insert( stdstrToken ) ;
  #else
          stdset_stdstr.insert( c_iter_stdvec_stdstr->substr(
            ardwFindPos[wIndex] , wStringLength ) ) ;
  #endif
          ardwFindPos[wIndex] =
              //Next time start search 1 char after the found char.
              stdstr_sizeFindPos + 1 ;
        }
        ++ wIndex ;
  //        c_iter_stdvec_stdstr +
      }
      if( stdset_stdstr.size() > 0 )
      {
        if( stdset_stdstr.size() > 1 )
        {
          //http://de.selfhtml.org/html/formulare/auswahl.htm:
          //"Jede Auswahlliste sollte einen internen Bezeichnernamen erhalten,
          //und zwar mit dem Attribut name."
          wxstrHTML += "<select " ;
          //Get all token indices that where the selection should be
          //connected to this token index.
          //
          wxstrHTML += "size=1>\n" ;
          for( std::set<std::string>::const_iterator c_iterToken =
              stdset_stdstr.begin() ;
              c_iterToken != stdset_stdstr.end() ;
              ++ c_iterToken
              )
          {
            wxstrHTML += "<option>" ;
            wxstrHTML += *c_iterToken ;
            wxstrHTML += "</option>\n" ;
          }
          wxstrHTML += "</select>\n" ;
        }
        else
          wxstrHTML += *stdset_stdstr.begin() ;
      }
      stdset_stdstr.clear() ;
    }while(bStringContained) ;
    wxstrHTML += wxT("</body></html>") ;
  //    mp_wxhtmlwindow->SetPage(wxstrHTML) ;
    wxFile wxfileHTML( wxstrFile , wxFile::write ) ;
    wxfileHTML.Write(wxstrHTML) ;
    wxfileHTML.Close() ;
  }
}

wxHTMLfileOutput::~wxHTMLfileOutput()
{
  // TODO Auto-generated destructor stub
}
