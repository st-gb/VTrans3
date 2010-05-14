/*
 * DrawParseTreeTraverser.cpp
 *
 *  Created on: May 12, 2010
 *      Author: Stefan
 */

#include <wxWidgets/DrawParseTreeTraverser.h>
#include <Parse/GrammarPart.hpp>
#include <Parse/ParseByRise.hpp>
#include <wx/gdicmn.h>
#include <wx/string.h>

DrawParseTreeTraverser::DrawParseTreeTraverser(
  wxDC * p_wxdc ,
  ParseByRise * p_parsebyrise ,
  GrammarPart * p_grammarpart
  )
  : ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
    p_grammarpart ,
    p_parsebyrise
    )
  , mp_wxdc ( p_wxdc )
  , mp_parsebyrise (p_parsebyrise)
  , m_wParseLevelCountedFromRoot(0)
  , m_wCurrentParseTreeLeftEndInPixels(0)
{
  // TODO Auto-generated constructor stub

}

DrawParseTreeTraverser::~DrawParseTreeTraverser()
{
  // TODO Auto-generated destructor stub
}

void DrawParseTreeTraverser::BeforeBeginAtRoot()
{
  //Important. else pathes with previous node(s) (->too long) are created.
//  m_vecGrammarPartID.clear() ;
//  m_stdvec_p_grammarpartPath.clear() ;
  m_wParseLevelCountedFromRoot ++ ;
}
void DrawParseTreeTraverser::CurrentNodeIsLastAddedRightChild()
{

}
void DrawParseTreeTraverser::LeaveFound()
{
  WORD wStringWidthLeftChild ;
  WORD wXcoord = 0
      //wLeftEndOfLeftmostTokenInPixels
      ;
  WORD wTextWidthInPixels ;
  wxSize wxsizeString ;
  wxString wxstrGrammarPartName ;

  GrammarPart * p_grammarpart = m_grammarpartpointer_and_parselevelCurrent.
      m_p_grammarpart ;
  wxString wxstrTokens ;
//      wxString wxstrGrammarPartName(r_stdstrGrammarPartName) ;
  wxsizeString = GetGrammarPartNameExtent( * mp_wxdc, p_grammarpart,
      wxstrGrammarPartName ) ;
  wTextWidthInPixels = wxsizeString.GetWidth() ;

  wxsizeString = GetTokenExtent( * mp_wxdc, p_grammarpart, wxstrTokens ) ;
  if ( wxsizeString.GetWidth() > wTextWidthInPixels )
    wTextWidthInPixels = wxsizeString.GetWidth() ;

//          DrawGrammarPartToken( p_grammarpart, wxpaintdc
//              , wHorizTextCenter ) ;
//          DrawGrammarPartName( p_grammarpart, wxpaintdc
//              , wHorizTextCenter ) ;
  mp_wxdc->DrawText( wxstrTokens , //wXcoord
    m_wCurrentParseTreeLeftEndInPixels , 0 ) ;

  wxstrGrammarPartName += wxString::Format( "%u",
    p_grammarpart->m_byPersonIndex ) ;

  mp_wxdc->DrawText(
    wxstrGrammarPartName , //wXcoord,
    m_wCurrentParseTreeLeftEndInPixels ,
    wxsizeString.GetHeight()
    ) ;
  DrawGrammarPartAttributes draw_grammar_part_attributes(
      //wXcoord + wTextWidthInPixels / 2 ,
    m_wCurrentParseTreeLeftEndInPixels + wTextWidthInPixels / 2 ,
      //m_wParseLevel
    1 ) ;
  m_wCurrentParseTreeLeftEndInPixels += wTextWidthInPixels + 10 ;
  m_stdmap_p_grammarpart2HorizCenter.insert(
    //std::pair<GrammarPart *,WORD>
//          (p_grammarpart, wXcoord + wWidth / 2 )
    std::pair<GrammarPart *,DrawGrammarPartAttributes>
    ( p_grammarpart ,
        draw_grammar_part_attributes )
    ) ;
}
void DrawParseTreeTraverser::ParseTreePathAdded()
{

}

wxSize DrawParseTreeTraverser::GetGrammarPartNameExtent(
//  wxPaintDC & wxpaintdc ,
  wxDC & r_wxdc ,
  GrammarPart * p_pg ,
  //wxSize & wxsizeText
  wxString & wxstr
  )
{
  const std::string & r_stdstrGrammarPartName = //citer->second.
    //m_stdstrGrammarPartName ;
    mp_parsebyrise->GetGrammarPartName( p_pg->m_wGrammarPartID ) ;
  wxstr = wxString( r_stdstrGrammarPartName ) ;
  wxSize wxsizeText = r_wxdc.GetTextExtent( wxstr ) ;
  return wxsizeText ;
}

wxSize DrawParseTreeTraverser::GetTokenExtent(
    wxDC & r_wxdc ,
    GrammarPart * p_pg ,
//    wxSize & wxsizeText
    wxString & wxstr
    )
{
  const std::string & r_stdstrTokens = //citer->second.
     GetBetweenAsStdString(
     mp_parsebyrise->m_psv, p_pg->m_dwLeftmostIndex,
     p_pg->m_dwRightmostIndex ) ;
  wxstr = wxString(r_stdstrTokens) ;
  wxSize wxsizeText = r_wxdc.GetTextExtent( wxstr ) ;
  return wxsizeText ;
}

//    void RightChildAdded(WORD wCurrentParseTreeLevel) ;
void DrawParseTreeTraverser::UnprocessedHighestLevelNodeFound()
{
//  m_stdvecNodesToProcess.clear();
//  GrammarPart * p_grammarpart ;
  //Must NOT be declared as const because it is used in std::map::erase() later
//  std::map<GrammarPart *,WORD>::iterator iterLeft ;
//  std::map<GrammarPart *,WORD>::iterator iterRight ;
  DrawGrammarPartAttributes * p_dgpaLeft ;
  DrawGrammarPartAttributes * p_dgpaRight ;
  std::map<GrammarPart *,DrawGrammarPartAttributes>::iterator iterLeft ;
  std::map<GrammarPart *,DrawGrammarPartAttributes>::iterator iterRight ;
  WORD wStringWidthLeftChild ;
  WORD wXcoord = 0 ;
  WORD wMiddleBetweenLeftAndRightChild ;
  WORD wLeftTextEndInPixels ;
  WORD wThisY, wChildY ;
//  WORD wHorizCenterInPixelsOfLeftChild ;
//  WORD wHorizCenterInPixelsOfRightChild ;
  wxSize wxsizeString ;
  wxString wxstrGrammarPartName ;

  GrammarPart * p_grammarpart = m_grammarpartpointer_and_parselevelCurrent.
      m_p_grammarpart ;
  iterRight = m_stdmap_p_grammarpart2HorizCenter.find(
      p_grammarpart->mp_grammarpartRightChild ) ;
  iterLeft = m_stdmap_p_grammarpart2HorizCenter.find(
      p_grammarpart->mp_grammarpartLeftChild ) ;

  p_dgpaLeft = & iterLeft->second ;
  p_dgpaRight = & iterRight->second ;
  wxsizeString = GetGrammarPartNameExtent(
    *mp_wxdc,
    p_grammarpart,
    wxstrGrammarPartName
    ) ;
  if( p_grammarpart->
      mp_grammarpartRightChild
    )
  {
    //Calculate the middle of the current node from the middle of the left
    //and the right child.
    wMiddleBetweenLeftAndRightChild =
      //iterLeft->second + ( iterRight->second - iterLeft->second ) / 2 ;
      iterLeft->second.m_wHorizCenterInPixels +
      ( iterRight->second.m_wHorizCenterInPixels -
      iterLeft->second.m_wHorizCenterInPixels ) / 2 ;
    wChildY = (p_dgpaRight->m_wParseLevel + 1 ) * wxsizeString.GetHeight() ;
    wThisY = //m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel *
        m_wParseLevelCountedFromRoot *
        wxsizeString.GetHeight() + 10 ;
    mp_wxdc->DrawLine(
      p_dgpaRight->m_wHorizCenterInPixels ,
      wChildY ,
      wMiddleBetweenLeftAndRightChild ,
      wThisY
      ) ;
  }
  else
    wMiddleBetweenLeftAndRightChild = iterLeft->second.m_wHorizCenterInPixels ;
  wLeftTextEndInPixels = wMiddleBetweenLeftAndRightChild -
      ( wxsizeString.GetWidth() / 2 ) ;
  if( wLeftTextEndInPixels > 65000 )
    wLeftTextEndInPixels = 0 ;
  DEBUG_COUTN("wxstrGrammarPartName:" << wxstrGrammarPartName.c_str()
    << " m_wParseLevelCountedFromRoot:" << m_wParseLevelCountedFromRoot
    << " m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel:"
    << m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel
    )

  wxstrGrammarPartName += wxString::Format( "%u",
    p_grammarpart->m_byPersonIndex ) ;

  mp_wxdc->DrawText( wxstrGrammarPartName ,
    wLeftTextEndInPixels ,
    //m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel *
    m_wParseLevelCountedFromRoot *
    wxsizeString.GetHeight()
    ) ;

  wChildY = ( p_dgpaLeft->m_wParseLevel + 1 ) * wxsizeString.GetHeight() ;
  wThisY = //m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel *
      m_wParseLevelCountedFromRoot *
      wxsizeString.GetHeight() + 10 ;
  mp_wxdc->DrawLine(
    p_dgpaLeft->m_wHorizCenterInPixels ,
    wChildY ,
    wMiddleBetweenLeftAndRightChild ,
    wThisY
    ) ;
//      bAtLeast1ItemDrawn = true ;
  //The children inside the container are not needed anymore now.
  //Also searching
  //inside the container th next time is faster is less elements are
  //inside it.
  m_stdmap_p_grammarpart2HorizCenter.erase(iterLeft) ;
  if( m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
      mp_grammarpartRightChild
    )
    m_stdmap_p_grammarpart2HorizCenter.erase(iterRight) ;
  DrawGrammarPartAttributes draw_grammar_part_attributes(
      wMiddleBetweenLeftAndRightChild ,
      //m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel
      m_wParseLevelCountedFromRoot
      ) ;
  m_stdmap_p_grammarpart2HorizCenter.insert(
//        std::pair<GrammarPart*,WORD>
//        (p_grammarpart,wMiddleBetweenLeftAndRightChild)
    std::pair<GrammarPart*,DrawGrammarPartAttributes>
    (p_grammarpart,draw_grammar_part_attributes)
    ) ;
}
