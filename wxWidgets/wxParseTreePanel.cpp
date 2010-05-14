/*
 * wxParseTreePanel.cpp
 *
 *  Created on: 22.02.2010
 *      Author: Stefan
 */

#include <wxWidgets/wxParseTreePanel.h>
#include <Parse/ParseByRise.hpp> //for class GrammerPart
#include <map> //multimap
#include <unistd.h> //sleep()
#include "DrawParseTreeTraverser.h"

BEGIN_EVENT_TABLE(wxParseTreePanel, wxPanel)
  EVT_PAINT  (wxParseTreePanel::OnPaint)
END_EVENT_TABLE()

//wxParseTreePanel::wxParseTreePanel() {
//	// TODO Auto-generated constructor stub
//
//}
wxParseTreePanel::wxParseTreePanel(
    wxWindow* parent
  )
  :wxPanel(
    parent
    )
{
  mp_parsebyrise = NULL ;
}

wxParseTreePanel::wxParseTreePanel(
    wxWindow* parent,
     wxWindowID id //= wxID_ANY
     , const wxPoint& pos //= wxDefaultPosition
     , const wxSize& size //= wxDefaultSize
     , long style //= wxTAB_TRAVERSAL,
     , const wxString& name //= "panel"
  )
  :wxPanel(
      parent,
      id,
      pos,
      size,
      style ,
      name )
{
//  Connect(wxEVT_PAINT, wxPaintEventHandler(wxParseTreePanel::OnPaint));
  mp_parsebyrise = NULL ;
}

//wxParseTreePanel::wxParseTreePanel( wxWindow* parent )
//  : wxPanel(parent)
//{
//
//}

wxParseTreePanel::~wxParseTreePanel() {
	// TODO Auto-generated destructor stub
}

void wxParseTreePanel::DrawGrammarPartName(
    GrammarPart * p_pg ,
    wxDC & r_wxdc ,
    WORD & r_wHorizTextCenter
    )
{
  if( p_pg )
  {
    DWORD dwXcoordinate ;
    const std::string & r_stdstrGrammarPartName = //citer->second.
      //m_stdstrGrammarPartName ;
      mp_parsebyrise->GetGrammarPartName( p_pg->m_wGrammarPartID ) ;
    std::map<WORD,DWORD>::const_iterator c_iter =
      m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.
      find( m_wParseLevel) ;
    if( c_iter != m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.
        end()
      )
    {
      dwXcoordinate = c_iter->second ;
      dwXcoordinate += 10 ;
    }
    else
      dwXcoordinate = 0 ;
    r_wxdc.DrawText( //citer->second.m_stdstrGrammarPartName
      r_stdstrGrammarPartName
       , wxPoint( dwXcoordinate ,m_wParseLevel * 20) ) ;

    wxSize wxsizeText = r_wxdc.GetTextExtent( wxString(r_stdstrGrammarPartName) ) ;
    dwXcoordinate += wxsizeText.x ;
    std::pair<std::map<WORD,DWORD>::iterator,bool> pair_iter_bool =
      m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.insert(
        std::pair<WORD,DWORD>( m_wParseLevel, dwXcoordinate )
        ) ;
    //http://www.cplusplus.com/reference/stl/map/insert/:
    //"The pair::second element in the pair is set to true if a new element was
    //inserted or false if an element with the same value existed."
    if( pair_iter_bool.second == false )
    {
      pair_iter_bool.first->second = dwXcoordinate ;
    }
    r_wHorizTextCenter = dwXcoordinate - ( wxsizeText.x / 2 ) ;
    m_stdmap_p_grammarpart2wCenter.insert(
        std::pair<GrammarPart *,WORD>( p_pg, r_wHorizTextCenter )
        ) ;
  }
}

void wxParseTreePanel::DrawGrammarPartNameAndPossiblyToken(
    GrammarPart * p_pg ,
    wxDC & r_wxdc ,
    WORD & r_wHorizTextCenter
    )
{
  if( p_pg )
  {
    DWORD dwXcoordinate ;
    const std::string & r_stdstrGrammarPartName = //citer->second.
      //m_stdstrGrammarPartName ;
      mp_parsebyrise->GetGrammarPartName( p_pg->m_wGrammarPartID ) ;
    std::map<WORD,DWORD>::const_iterator c_iter =
      m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.
      find( m_wParseLevel) ;
    if( c_iter != m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.
        end()
      )
    {
      dwXcoordinate = c_iter->second ;
      dwXcoordinate += 10 ;
    }
    else
      dwXcoordinate = 0 ;
    r_wxdc.DrawText( //citer->second.m_stdstrGrammarPartName
      r_stdstrGrammarPartName
       , wxPoint( dwXcoordinate ,m_wParseLevel * 20) ) ;
    if( mp_parsebyrise->GrammarPartIDIsWordClass(p_pg->m_wGrammarPartID) )
    {
      std::string stdstrTokens = GetBetweenAsStdString(
          mp_parsebyrise->m_psv, p_pg->m_dwLeftmostIndex,
          p_pg->m_dwRightmostIndex ) ;
      r_wxdc.DrawText( //citer->second.m_stdstrGrammarPartName
          stdstrTokens
         , wxPoint( dwXcoordinate ,m_wParseLevel * 20 + 10 ) ) ;
    }
    wxSize wxsizeText = r_wxdc.GetTextExtent( wxString(r_stdstrGrammarPartName) ) ;
    dwXcoordinate += wxsizeText.x ;
    std::pair<std::map<WORD,DWORD>::iterator,bool> pair_iter_bool =
      m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.insert(
        std::pair<WORD,DWORD>( m_wParseLevel, dwXcoordinate )
        ) ;
    //http://www.cplusplus.com/reference/stl/map/insert/:
    //"The pair::second element in the pair is set to true if a new element was
    //inserted or false if an element with the same value existed."
    if( pair_iter_bool.second == false )
    {
      pair_iter_bool.first->second = dwXcoordinate ;
    }
    r_wHorizTextCenter = dwXcoordinate - ( wxsizeText.x / 2 ) ;
    m_stdmap_p_grammarpart2wCenter.insert(
        std::pair<GrammarPart *,WORD>( p_pg, r_wHorizTextCenter )
        ) ;
  }
}

void wxParseTreePanel::DrawGrammarPartChildren(
  GrammarPart * p_grammarpart ,
  wxDC & r_wxdc
  )
{
  GrammarPart * p_grammarpartChild = p_grammarpart->mp_grammarpartLeftChild ;
  WORD wChildHorizTextCenter ;
  WORD wHorizTextCenter = 0 ;
  std::map<GrammarPart *,WORD>::iterator iter =
      m_stdmap_p_grammarpart2wCenter.find( p_grammarpart) ;
  DEBUG_COUT("DrawGrammarPartChildren " << p_grammarpart << "\n")
  if( iter != m_stdmap_p_grammarpart2wCenter.end() )
    wHorizTextCenter = iter->second ;
  if( //p_grammarpart->mp_grammarpartLeftChild
      p_grammarpartChild )
   {
#ifdef _DEBUG
    const std::string & r_stdstrGrammarPartName = //citer->second.
       //m_stdstrGrammarPartName ;
        mp_parsebyrise->GetGrammarPartName(
       //p_grammarpart->mp_grammarpartLeftChild->
       p_grammarpartChild->m_wGrammarPartID ) ;
#endif
     DrawGrammarPartNameAndPossiblyToken(//p_grammarpart->mp_grammarpartLeftChild
         p_grammarpartChild ,r_wxdc, wChildHorizTextCenter ) ;
//          p_pg = p_pg->mp_grammarpartLeftChild ;
     DrawGrammarPartParentToChildLine(wHorizTextCenter,wChildHorizTextCenter
         , r_wxdc );
   }
//   else //Left leave of parse tree found.
//   {
//     std::vector<GrammarPart *>::iterator iter = stdvecNodesToProcess.end() ;
//     p_pg = *iter ;
//     DrawGrammarPartNameAndPossiblyToken( *iter , wxpaintdc) ;
//     stdvecNodesToProcess.pop_back() ;
//     -- m_wParseLevel ;
//   }
  p_grammarpartChild = p_grammarpart->mp_grammarpartRightChild ;
   if( p_grammarpart->mp_grammarpartRightChild )
   {
     //GrammarPart * p_grammarpartChild = p_grammarpart->mp_grammarpartRightChild ;
#ifdef _DEBUG
    const std::string & r_stdstrGrammarPartName = //citer->second.
       //m_stdstrGrammarPartName ;
      mp_parsebyrise->GetGrammarPartName(
       //p_grammarpart->mp_grammarpartRightChild->
          p_grammarpartChild->m_wGrammarPartID ) ;
#endif
     DrawGrammarPartNameAndPossiblyToken( //p_grammarpart->mp_grammarpartRightChild,
       p_grammarpartChild, r_wxdc, wChildHorizTextCenter ) ;

     //Draw the line from grammar part parent to grammar part child.
     DrawGrammarPartParentToChildLine(wHorizTextCenter,wChildHorizTextCenter
       , r_wxdc );
     DEBUG_COUT( "DrawGrammarPartChildren--pushinG right child "
       << mp_parsebyrise->GetGrammarPartName(
         //p_grammarpart->mp_grammarpartRightChild->
         p_grammarpartChild->m_wGrammarPartID )
       << p_grammarpartChild
       << " current size="
       << m_stdvecNodesToProcess.size()
       << "\n" )
     //Add to process possible children of the right nodes later.
     m_stdvecNodesToProcess.push_back( //p_grammarpart->mp_grammarpartRightChild
       p_grammarpartChild ) ;
     //usleep(100000) ;
//          ++ m_wParseLevel ;
   }
}

void wxParseTreePanel::DrawGrammarPartParentToChildLine(
  WORD wParentHorizTextCenter,
  WORD wChildHorizTextCenter
  , wxDC & r_wxdc )
{
  r_wxdc.DrawLine( wParentHorizTextCenter, ( m_wParseLevel - 1 )* 20 + 10 ,
    wChildHorizTextCenter , m_wParseLevel * 20 + 10 ) ;
}

void wxParseTreePanel::DrawParseTree( ParseByRise & r_parsebyrise )
{
//  m_wParseLevel = 0 ;
//  m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.clear() ;
  mp_parsebyrise = & r_parsebyrise ;
  Refresh() ;
}

WORD wxParseTreePanel::DrawLeavesOfParseTree(
    wxDC & r_wxdc ,
    //std::map<GrammarPart *,WORD> & map_p_grammarpart2HorizCenter
    GrammarPart * p_grammarpart ,
    //This is relevant if more than 1 parse tree for the text.
    WORD wLeftEndOfLeftmostTokenInPixels
    )
{
  m_stdvecNodesToProcess.clear();
//  GrammarPart * p_grammarpart ;
  WORD wStringWidthLeftChild ;
  WORD wXcoord = //0
      wLeftEndOfLeftmostTokenInPixels ;
  WORD wTextWidthInPixels ;
  wxSize wxsizeString ;
  wxString wxstrGrammarPartName ;
  do
  {
    if( p_grammarpart->mp_grammarpartRightChild )
    {
      //Add to process possible children of the right nodes later.
      m_stdvecNodesToProcess.push_back( //p_grammarpart->mp_grammarpartRightChild
          p_grammarpart->mp_grammarpartRightChild  ) ;
//      wStringWidthRightChild = GetGrammarPartNameExtent( wxpaintdc,
//          p_grammarpart->mp_grammarpartRightChild,
//          wxstrGrammarPartName ).GetWidth() ;
    }
    if( p_grammarpart->mp_grammarpartLeftChild )
    {
      p_grammarpart = p_grammarpart->mp_grammarpartLeftChild ;
//      wStringWidthLeftChild = GetGrammarPartNameExtent( wxpaintdc,
//          p_grammarpart->mp_grammarpartLeftChild ,
//          wxstrGrammarPartName ).GetWidth() ;
//      bRightChild = false ;
  //        ++ m_wParseLevel ;
    }
    else //(Left) leave of parse tree found.
    {
  //          setFinalNodesOfCurrentParseLevel.insert( ) ;
  //        DrawGrammarPartNameAndPossiblyToken( p_grammarpart, wxpaintdc
  //           , wHorizTextCenter ) ;
      wxString wxstrTokens ;
//      wxString wxstrGrammarPartName(r_stdstrGrammarPartName) ;
      wxsizeString = GetGrammarPartNameExtent( r_wxdc, p_grammarpart,
          wxstrGrammarPartName ) ;
      wTextWidthInPixels = wxsizeString.GetWidth() ;
      wxsizeString = GetTokenExtent( r_wxdc, p_grammarpart, wxstrTokens ) ;
      if ( wxsizeString.GetWidth() > wTextWidthInPixels )
        wTextWidthInPixels = wxsizeString.GetWidth() ;

  //          DrawGrammarPartToken( p_grammarpart, wxpaintdc
  //              , wHorizTextCenter ) ;
  //          DrawGrammarPartName( p_grammarpart, wxpaintdc
  //              , wHorizTextCenter ) ;
      r_wxdc.DrawText( wxstrTokens , wXcoord, 0 ) ;
      r_wxdc.DrawText( wxstrGrammarPartName , wXcoord,
          wxsizeString.GetHeight() ) ;
      DrawGrammarPartAttributes draw_grammar_part_attributes(
          wXcoord + wTextWidthInPixels / 2 , m_wParseLevel ) ;
      m_stdmap_p_grammarpart2HorizCenter.insert(
        //std::pair<GrammarPart *,WORD>
//          (p_grammarpart, wXcoord + wWidth / 2 )
        std::pair<GrammarPart *,DrawGrammarPartAttributes>
        (p_grammarpart, draw_grammar_part_attributes )
        ) ;
      wXcoord += wTextWidthInPixels + 10 ;
//      if( bRightChild )
//      { //At this point: both the left and the right child have been drawn.
//        //So the parent can be drawn at their horizontal center now.
//      }
      if( ! m_stdvecNodesToProcess.empty() )
      {
      //std::vector<GrammarPart *>::iterator iter =
      p_grammarpart =
        m_stdvecNodesToProcess.
        //"Returns a read/write reference to the data at the last
        //*  element of the %vector."
        back() ;
//      bRightChild = true ;
//      vecCurrentParseTreePath.push_back(p_grammarpart) ;
      //p_grammarpart = *iter ;
      DEBUG_COUT ( "DrawLeavesOfParseTree--popping " <<
        mp_parsebyrise->GetGrammarPartName(
        p_grammarpart->m_wGrammarPartID )
        << p_grammarpart
  //            << " current size=" << m_stdvecNodesToProcess.size()
        << "\n" )

      //
      //
      //e.g.: noun_construct
      //        /       \
      //     article  noun
      //when at article: parse level is 1,
      //because "noun" was the last pushed it is the next
  //        m_wParseLevel = m_stdvecNodesToProcess.size() + 1 ;
  //          DrawGrammarPartNameAndPossiblyToken( *iter , wxpaintdc) ;
      m_stdvecNodesToProcess.pop_back() ;
      //std::cout.flush() ;
      //usleep(100000) ;
  //          -- m_wParseLevel ;
      }
      else
        p_grammarpart = NULL ;
    }
    DEBUG_COUT ( "DrawLeavesOfParseTree--m_wParseLevel: " << m_wParseLevel
        << " nodes to process size="
        << m_stdvecNodesToProcess.size() << "\n" )
  //        usleep(1000000) ;
  }
  while( //! m_stdvecNodesToProcess.empty()
      //m_wParseLevel > 0
      p_grammarpart ) ;
  return wXcoord ;
}

GrammarPart * wxParseTreePanel::GetNextRightGrammarPartNotProcessedYet()
{
  GrammarPart * p_grammarpart = NULL ;
  if( ! m_stdvecNodesToProcess.empty() )
  {
    //std::vector<GrammarPart *>::iterator iter =
    p_grammarpart =
      m_stdvecNodesToProcess.
      //"Returns a read/write reference to the data at the last
      //*  element of the %vector."
      back() ;
    //      bRightChild = true ;
    //      vecCurrentParseTreePath.push_back(p_grammarpart) ;
    //p_grammarpart = *iter ;
    DEBUG_COUT ( "DrawLeavesOfParseTree--popping " <<
      mp_parsebyrise->GetGrammarPartName(
      p_grammarpart->m_wGrammarPartID )
      << p_grammarpart
    //            << " current size=" << m_stdvecNodesToProcess.size()
      << "\n" )

    //
    //
    //e.g.: noun_construct
    //        /       \
    //     article  noun
    //when at article: parse level is 1,
    //because "noun" was the last pushed it is the next
    //        m_wParseLevel = m_stdvecNodesToProcess.size() + 1 ;
    //          DrawGrammarPartNameAndPossiblyToken( *iter , wxpaintdc) ;
    m_stdvecNodesToProcess.pop_back() ;
    //std::cout.flush() ;
    //usleep(100000) ;
    //          -- m_wParseLevel ;
  }
  else
    p_grammarpart = NULL ;
  return p_grammarpart ;
}

//wParseLevel: used to calculate the y coordinate for drawing the
//grammar part name.
void wxParseTreePanel::DrawNextParseTreeLevelDirectingRoot(
//  wxPaintDC & wxpaintdc ,
  wxDC & r_wxdc ,
  WORD wParseLevel ,
  GrammarPart * p_grammarpart
  )
{
//  bool bAtLeast1ItemDrawn = false ;
  m_stdvecNodesToProcess.clear();
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
//  WORD wHorizCenterInPixelsOfLeftChild ;
//  WORD wHorizCenterInPixelsOfRightChild ;
  wxSize wxsizeString ;
  wxString wxstrGrammarPartName ;
  do
  {
    iterRight = m_stdmap_p_grammarpart2HorizCenter.find(
        p_grammarpart->mp_grammarpartRightChild ) ;
    iterLeft = m_stdmap_p_grammarpart2HorizCenter.find(
        p_grammarpart->mp_grammarpartLeftChild ) ;

    if(
        //If 2 already drawn children found.
        //e.g. "the car"
        //       \  /
        //     noun_construct
        //when at "noun_construct"
        iterRight != m_stdmap_p_grammarpart2HorizCenter.end()
      && iterLeft != m_stdmap_p_grammarpart2HorizCenter.end()
      )
    {
//      wHorizCenterInPixelsOfLeftChild =
//        iterLeft->second.m_wHorizCenterInPixels ;
//      wHorizCenterInPixelsOfRightChild =
//        iterRight->second.m_wHorizCenterInPixels ;
      p_dgpaLeft = & iterLeft->second ;
      p_dgpaRight = & iterRight->second ;
      //Calculate the middle of the current node from the middle of the left
      //and the right child.
      wMiddleBetweenLeftAndRightChild =
        //iterLeft->second + ( iterRight->second - iterLeft->second ) / 2 ;
        iterLeft->second.m_wHorizCenterInPixels +
        ( iterRight->second.m_wHorizCenterInPixels -
        iterLeft->second.m_wHorizCenterInPixels ) / 2 ;
      wxsizeString = GetGrammarPartNameExtent( r_wxdc, p_grammarpart,
          wxstrGrammarPartName ) ;
      wLeftTextEndInPixels = wMiddleBetweenLeftAndRightChild -
          ( wxsizeString.GetWidth() / 2 ) ;

      wxstrGrammarPartName += wxString::Format( "%u",
        p_grammarpart->m_byPersonIndex ) ;

      r_wxdc.DrawText( wxstrGrammarPartName ,
        wLeftTextEndInPixels ,
        wParseLevel * wxsizeString.GetHeight()
        ) ;

      r_wxdc.DrawLine(
        p_dgpaLeft->m_wHorizCenterInPixels ,
        ( p_dgpaLeft->m_wParseLevel + 1 ) * wxsizeString.GetHeight() ,
        wMiddleBetweenLeftAndRightChild ,
        wParseLevel * wxsizeString.GetHeight() + 10
        ) ;
      r_wxdc.DrawLine(
        p_dgpaRight->m_wHorizCenterInPixels ,
        (p_dgpaRight->m_wParseLevel + 1 ) * wxsizeString.GetHeight() ,
        wMiddleBetweenLeftAndRightChild ,
        wParseLevel * wxsizeString.GetHeight() + 10
        ) ;
//      bAtLeast1ItemDrawn = true ;
      //The children inside the container are not needed anymore now.
      //Also searching
      //inside the container th next time is faster is less elements are
      //inside it.
      m_stdmap_p_grammarpart2HorizCenter.erase(iterLeft) ;
      m_stdmap_p_grammarpart2HorizCenter.erase(iterRight) ;
      DrawGrammarPartAttributes draw_grammar_part_attributes(
          wMiddleBetweenLeftAndRightChild , wParseLevel ) ;
      m_stdmap_p_grammarpart2HorizCenter.insert(
//        std::pair<GrammarPart*,WORD>
//        (p_grammarpart,wMiddleBetweenLeftAndRightChild)
        std::pair<GrammarPart*,DrawGrammarPartAttributes>
        (p_grammarpart,draw_grammar_part_attributes)
        ) ;
      do
      {
        //Do NOT follow children for the grammar part! they have been drawn yet.
        p_grammarpart = GetNextRightGrammarPartNotProcessedYet() ;
        //No more unprocessed right grammar parts.
        if( p_grammarpart == NULL )
          //break ;
          return ;
        iterRight = m_stdmap_p_grammarpart2HorizCenter.find(
            p_grammarpart ) ;
        //e.g. for "the anal and the vacuum cleaner and" the 1st right node
        //would be for "and". But this has been drawn yet, so continue with the
        //next right child.
      } while( //exists in the list
          iterRight != m_stdmap_p_grammarpart2HorizCenter.end() ) ;
    }
    else //Not both left and right child that have already been drawn found.
    {
      if( p_grammarpart->mp_grammarpartRightChild )
      {
        iterRight = m_stdmap_p_grammarpart2HorizCenter.find(
            p_grammarpart->mp_grammarpartRightChild ) ;
        if( //is not in list
            iterRight == m_stdmap_p_grammarpart2HorizCenter.end() )
        {
          //Add to process possible children of the right nodes later.
          m_stdvecNodesToProcess.push_back( //p_grammarpart->mp_grammarpartRightChild
              p_grammarpart->mp_grammarpartRightChild  ) ;
    //      wStringWidthRightChild = GetGrammarPartNameExtent( wxpaintdc,
    //          p_grammarpart->mp_grammarpartRightChild,
    //          wxstrGrammarPartName ).GetWidth() ;
        }
      }
      if( p_grammarpart->mp_grammarpartLeftChild )
      {
        iterLeft = m_stdmap_p_grammarpart2HorizCenter.find(
            p_grammarpart->mp_grammarpartLeftChild ) ;
        if( //is not in list
            iterLeft == m_stdmap_p_grammarpart2HorizCenter.end() )
        {
          p_grammarpart = p_grammarpart->mp_grammarpartLeftChild ;
    //      wStringWidthLeftChild = GetGrammarPartNameExtent( wxpaintdc,
    //          p_grammarpart->mp_grammarpartLeftChild ,
    //          wxstrGrammarPartName ).GetWidth() ;
    //      bRightChild = false ;
      //        ++ m_wParseLevel ;
        }
        else
          //Left child was drawn, but not right child yet:
          // the car        sucks
          //  \  /           /
          // noun_construct /
          //             \ /
          //           clause
          // when at "clause" and "the", "car" , and "noun_construct" have
          // been drawn yet.
        {
          do
          {
            //Do NOT follow children for the grammar part! they have been drawn yet.
            p_grammarpart = GetNextRightGrammarPartNotProcessedYet() ;
            //No more unprocessed right grammar parts.
            if( p_grammarpart == NULL )
              //break ;
              return ;
            iterRight = m_stdmap_p_grammarpart2HorizCenter.find(
                p_grammarpart ) ;
            //e.g. for "the anal and the vacuum cleaner and" the 1st right node
            //would be for "and". But this has been drawn yet, so continue with the
            //next right child.
          } while( //exists in the list
              iterRight != m_stdmap_p_grammarpart2HorizCenter.end() ) ;

       }
      }
      else //(Left) leave of parse tree found.
      {
    //          setFinalNodesOfCurrentParseLevel.insert( ) ;
    //        DrawGrammarPartNameAndPossiblyToken( p_grammarpart, wxpaintdc
    //           , wHorizTextCenter ) ;
        wxString wxstrTokens ;
        WORD wWidth ;
//  //      wxString wxstrGrammarPartName(r_stdstrGrammarPartName) ;
//        wxsizeString = GetGrammarPartNameExtent( wxpaintdc, p_grammarpart,
//            wxstrGrammarPartName ) ;
//        wWidth = wxsizeString.GetWidth() ;
//        wxsizeString = GetTokenExtent( wxpaintdc, p_grammarpart, wxstrTokens ) ;
//        if ( wxsizeString.GetWidth() > wWidth )
//          wWidth = wxsizeString.GetWidth() ;
//
//    //          DrawGrammarPartName( p_grammarpart, wxpaintdc
//    //              , wHorizTextCenter ) ;
//        wxpaintdc.DrawText( wxstrGrammarPartName , wXcoord,
//            wxsizeString.GetHeight() ) ;
//        m_stdmap_p_grammarpart2HorizCenter.insert(std::pair<GrammarPart *,WORD>
//          (p_grammarpart,wXcoord + wWidth/2)
//          ) ;
//        wXcoord += wWidth + 10 ;
//  //      if( bRightChild )
//  //      { //At this point: both the left and the right child have been drawn.
//  //        //So the parent can be drawn at their horizontal center now.
//  //      }
      }
    } //Not both left and right child that have already been drawn found.
    DEBUG_COUT ( "DrawNextParseTreeLevelDirectingRoot--m_wParseLevel: "
      << m_wParseLevel
      << " nodes to process size="
      << m_stdvecNodesToProcess.size() << "\n" )
  //        usleep(1000000) ;
  }
  while( //! m_stdvecNodesToProcess.empty()
      //m_wParseLevel > 0
      p_grammarpart ) ;
}

//Beginning from the parse tree leaves the grammar part names (and for
//leaves also the tokens) should be drawn:
//  the car and the cat suck
//   \  /
//
//

//the tree is traversed starting from the root (clause):
//                                clause
//                                /     \
//     noun_constr,conj,noun_constr     verb
//              /             \
// noun_constr,conj          noun_constr
//    /          \               /\
// noun_constr conjunction      /  \
//   /      \        |         /    \
// article  noun     |        /      \
// the      car     and     the      cat
// At first, directing leaves take the left child and store the right child
//  in the nodes that should be processed later.
// The list for right children: [verb;noun_constr;conjunction;noun]
// when the 1st leave is reached.
// When a leaf is found, draw its token and its grammar part name.
//  Additionally, when a grammar part name is drawn store the horizontal position
// because its parent should be drawn in the middle of its children and
//  a line should be drawn from child to parent.
void wxParseTreePanel::DrawParseTreeBeginningFromLeaves(
    wxDC & r_wxdc )
{
  DWORD dwNumberOfAlreadyDrawnItems = 0 ;
  DWORD dwLeftMostTokenIndex = 0 ;
  GrammarPart * p_grammarpart ;
  int y = 10 ;
  std::multimap<DWORD, GrammarPart>::const_iterator citer ;
  std::multimap<DWORD, GrammarPart> &
    r_stdmultimap_dwLeftmostIndex2grammarpart = mp_parsebyrise->
    m_stdmultimap_dwLeftmostIndex2grammarpart ;
  //Before each draw in order to begin at x position "0".
  m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.clear() ;
  m_wParseLevel = 0 ;
  m_stdvecNodesToProcess.clear() ;
  citer = r_stdmultimap_dwLeftmostIndex2grammarpart.begin() ;
  //A text (especially the longer the more probable) may contain more than 1
  //contigous parse tree: e.g. if only parse rule
  //"definite article + noun" = noun_construct, text "the man the woman":
  //       the man the woman
  //         \ /     \ /
  //noun_construct noun_construct <- 2 parse trees for the text
  p_grammarpart = mp_parsebyrise->GetGrammarPartCoveringMostTokens(
      dwLeftMostTokenIndex ) ;
  std::set<GrammarPart * > setFinalNodesOfCurrentParseLevel ;
//  std::map<GrammarPart *,WORD> map_p_grammarpart2HorizCenter;
  //E.g. for "the car"
  //           \  /
  //         noun_construct
  //when at "the": insert horiz center + noun_construct
  //when at "car": insert horiz center + noun_construct
  std::map<GrammarPart *,WORD> mapParentNode2HorizChildCenter;
  //At every time only 1 path of the tree is the current.
  //e.g. for
  // "the car     sucks."
  //   \ /          /
  //noun_construct /
  //     \        /
  //      -clause-
  //
  // the current path may be "the->noun_construct" when the current node is
  //  "noun_construct".
  std::vector<GrammarPart * > vecCurrentParseTreePath ;
  //stores the string width for every node of the parse tree path.
  std::set<WORD> setStringWidthsOfCurrentParseTreePath ;
  WORD wXcoord = 0 ;
  wxSize wxsizeString ;
//  wxSize wxsizeStringLeftChild ;
//  wxSize wxsizeStringRightChild ;
  WORD wStringWidthLeftChild ;
  WORD wStringWidthRightChild ;
  WORD wBeginOfCurrentLeftEndOfLeftmostTokenOfTreeCoveringMostTokens = 0 ;
  bool bRightChild = false ;
  m_stdmap_p_grammarpart2HorizCenter.clear() ;

  WORD wCurrentParseTreeLeftEndInPixels = 0 ;
//  while( p_grammarpart )
//  while( bDrawText )
  //Draw leaves of parse trees AND token that do not belong to a parse tree.
  while ( dwLeftMostTokenIndex < mp_parsebyrise->m_psv.size() )
  {
    //Is NULL if the token does not belong to a parse tree.
    if( p_grammarpart )
    {
      dwLeftMostTokenIndex = p_grammarpart->m_dwRightmostIndex + 1 ;
      DEBUG_COUT("wxParseTreePanel::DrawParseTreeBeginningFromLeaves--"
        "topmost grammar part from token indices ["
        //<< dwLeftMostTokenIndex << ":"
        << p_grammarpart->m_dwLeftmostIndex << ":"
        << p_grammarpart->m_dwRightmostIndex << "] :"
        << p_grammarpart
        << "\n" )
      WORD wHorizTextCenter = 0 ;
      const std::string & r_stdstrGrammarPartName = //citer->second.
        //m_stdstrGrammarPartName ;
        mp_parsebyrise->GetGrammarPartName( p_grammarpart->m_wGrammarPartID ) ;
      wxString wxstrGrammarPartName(r_stdstrGrammarPartName) ;
      wxsizeString = GetGrammarPartNameExtent( r_wxdc, p_grammarpart,
          wxstrGrammarPartName ) ;
      setStringWidthsOfCurrentParseTreePath.insert( wxsizeString.GetWidth()) ;
      vecCurrentParseTreePath.push_back(p_grammarpart) ;

//      wBeginOfCurrentLeftEndOfLeftmostTokenOfTreeCoveringMostTokens =
//          DrawLeavesOfParseTree( r_wxdc, //m_stdmap_p_grammarpart2HorizCenter
//          p_grammarpart ,
//          wBeginOfCurrentLeftEndOfLeftmostTokenOfTreeCoveringMostTokens ) ;
//      while(DrawNextParseTreeLevelDirectingRoot() ) ;
      m_wParseLevel =  2 ;
//      if( //If the root has not been drawn yet (for trees that have only 1 node
//          //that is root and leaf at the same time ).
//          //If this case is not catched this may result in an endless loop
//          //below.
//        m_stdmap_p_grammarpart2HorizCenter.find( p_grammarpart ) ==
//          m_stdmap_p_grammarpart2HorizCenter.end()
//        )
//      {
//        do
//        {
//          dwNumberOfAlreadyDrawnItems = m_stdmap_p_grammarpart2HorizCenter.size() ;
//          DrawNextParseTreeLevelDirectingRoot(
//            r_wxdc, //2
//            m_wParseLevel ++ , p_grammarpart) ;
//        }
//        while( //m_stdmap_p_grammarpart2HorizCenter.size() < dwNumberOfAlreadyDrawnItems
//          //While the root has not been drawn yet.
//          m_stdmap_p_grammarpart2HorizCenter.find( p_grammarpart ) ==
//            m_stdmap_p_grammarpart2HorizCenter.end()
//          ) ;
//      }
      DrawParseTreeTraverser dptt(
        & r_wxdc ,
        mp_parsebyrise ,
        p_grammarpart
        );
      dptt.m_wCurrentParseTreeLeftEndInPixels =
          wCurrentParseTreeLeftEndInPixels ;
      dptt.Traverse() ;
      wCurrentParseTreeLeftEndInPixels =
          dptt.m_wCurrentParseTreeLeftEndInPixels ;
      //Set to zero, else infinite loop when the tree is drawn the 2nd time
      //because it would start with the value "1" and would not get "0".
      m_wParseLevel = 0 ;
      m_stdvecNodesToProcess.clear() ;
      DEBUG_COUT("DrawParseTreeBeginningFromLeaves--no more nodes to process"
          << "\n")
    }
    else //token not part of a parse tree.
    {
      VTrans::string_type & r_vtrans_str =
        mp_parsebyrise->m_psv.at(dwLeftMostTokenIndex).m_Str ;
      r_wxdc.DrawText( r_vtrans_str ,
        wBeginOfCurrentLeftEndOfLeftmostTokenOfTreeCoveringMostTokens , 0 ) ;
      wBeginOfCurrentLeftEndOfLeftmostTokenOfTreeCoveringMostTokens +=
        10 + r_wxdc.GetTextExtent( r_vtrans_str ).GetWidth() ;
      ++ dwLeftMostTokenIndex ;
    }
    //A text (especially the longer the more probable) may contain more than 1
    //contigous parse tree: e.g. if only parse rule
    //"definite article + noun" = noun_construct, text "the man the woman":
    //       the man the woman
    //         \ /     \ /
    //noun_construct noun_construct <- 2 parse trees for the text
    p_grammarpart =
        //Get the next tree.
      mp_parsebyrise->GetGrammarPartCoveringMostTokens(
      dwLeftMostTokenIndex ) ;
//        NULL ;
//    if( ! p_grammarpart )
//      if( mp_parsebyrise->m_psv.size() > dwLeftMostTokenIndex )
//      bDrawText
  }
}

void wxParseTreePanel::DrawParseTreeBeginningFromRoots(
    wxDC & r_wxdc )
{
  DWORD dwLeftMostTokenIndex = 0 ;
  GrammarPart * p_grammarpart ;
  int y = 10 ;
  std::multimap<DWORD, GrammarPart>::const_iterator citer ;
  std::multimap<DWORD, GrammarPart> &
    r_stdmultimap_dwLeftmostIndex2grammarpart = mp_parsebyrise->
    m_stdmultimap_dwLeftmostIndex2grammarpart ;
  //Before each draw in order to begin at x position "0".
  m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.clear() ;
  m_wParseLevel = 0 ;
  m_stdvecNodesToProcess.clear() ;
  citer = r_stdmultimap_dwLeftmostIndex2grammarpart.begin() ;
  p_grammarpart = mp_parsebyrise->GetGrammarPartCoveringMostTokens(
      dwLeftMostTokenIndex ) ;
  DEBUG_COUT("wxParseTreePanel::OnPaint--topmost grammar part:"
      << p_grammarpart
      << "\n")
  if( p_grammarpart )
  {

  const std::string & r_stdstrGrammarPartName = //citer->second.
    //m_stdstrGrammarPartName ;
    mp_parsebyrise->GetGrammarPartName( p_grammarpart->m_wGrammarPartID ) ;
  wxString wxstr(r_stdstrGrammarPartName) ;
  r_wxdc.DrawText( //citer->second.m_stdstrGrammarPartName
    //r_stdstrGrammarPartName
    wxstr
     , wxPoint(200,0) ) ;
  m_stdmap_p_grammarpart2wCenter.insert( std::pair<GrammarPart *,WORD>
    (p_grammarpart, 200)) ;
  ++ m_wParseLevel ;
  do
  {
  //        if( p_pg->mp_grammarpartLeftChild )
  //        {
  //          DrawGrammarPartNameAndPossiblyToken(p_pg->mp_grammarpartLeftChild,wxpaintdc) ;
  ////          p_pg = p_pg->mp_grammarpartLeftChild ;
  //        }
  //        else //Left leave of parse tree found.
  //        {
  //          std::vector<GrammarPart *>::iterator iter = stdvecNodesToProcess.end() ;
  //          p_pg = *iter ;
  //          DrawGrammarPartNameAndPossiblyToken( *iter , wxpaintdc) ;
  //          stdvecNodesToProcess.pop_back() ;
  //          -- m_wParseLevel ;
  //        }
  //        if( p_pg->mp_grammarpartRightChild )
  //        {
  //          DrawGrammarPartNameAndPossiblyToken(p_pg->mp_grammarpartRightChild, wxpaintdc) ;
  //          //Add to process possible children of the right nodes later.
  //          stdvecNodesToProcess.push_back(p_pg->mp_grammarpartRightChild) ;
  ////          ++ m_wParseLevel ;
  //        }
    DrawGrammarPartChildren( p_grammarpart , r_wxdc ) ;
    if( p_grammarpart->mp_grammarpartLeftChild )
    {
      p_grammarpart = p_grammarpart->mp_grammarpartLeftChild ;
      ++ m_wParseLevel ;
    }
    else //Left leave of parse tree found.
    {
      if( ! m_stdvecNodesToProcess.empty() )
      {
      //std::vector<GrammarPart *>::iterator iter =
      p_grammarpart =
        m_stdvecNodesToProcess.
        //"Returns a read/write reference to the data at the last
        //*  element of the %vector."
        back() ;
      //p_grammarpart = *iter ;
      DEBUG_COUT ( "OnPaint--popping " <<
        mp_parsebyrise->GetGrammarPartName(
        p_grammarpart->m_wGrammarPartID )
        << p_grammarpart
  //            << " current size=" << m_stdvecNodesToProcess.size()
        << "\n" )

      //
      //
      //e.g.: noun_construct
      //        /       \
      //     article  noun
      //when at article: parse level is 1,
      //because "noun" was the last pushed it is the next
      m_wParseLevel = m_stdvecNodesToProcess.size() + 1 ;
  //          DrawGrammarPartNameAndPossiblyToken( *iter , wxpaintdc) ;
      m_stdvecNodesToProcess.pop_back() ;
      //std::cout.flush() ;
      //usleep(100000) ;
  //          -- m_wParseLevel ;
      }
      else
        p_grammarpart = NULL ;
    }
    DEBUG_COUT ( "DrawParseTreeBeginningFromRoots--m_wParseLevel: " << m_wParseLevel
        << " nodes to process size="
        << m_stdvecNodesToProcess.size() << "\n" )
  //        usleep(1000000) ;
  }
  while( //! m_stdvecNodesToProcess.empty()
      //m_wParseLevel > 0
      p_grammarpart ) ;
  //Set to zero, else infinite loop when the tree is drawn the 2nd time
  //because it would start with the value "1" and would not get "0".
  m_wParseLevel = 0 ;
  m_stdvecNodesToProcess.clear() ;
  DEBUG_COUT("OnPaint--no more nodes to process" << "\n")
  }
}

void wxParseTreePanel::DrawGrammarPartToken(
    GrammarPart * p_pg ,
    wxDC & r_wxdc ,
    WORD & r_wHorizTextCenter
    )
{
  if( p_pg )
  {
    DWORD dwXcoordinate ;
    std::map<WORD,DWORD>::const_iterator c_iter =
      m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.
      find( m_wParseLevel) ;
    if( c_iter != m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.
        end()
      )
    {
      dwXcoordinate = c_iter->second ;
      dwXcoordinate += 10 ;
    }
    else
      dwXcoordinate = 0 ;
    if( mp_parsebyrise->GrammarPartIDIsWordClass(p_pg->m_wGrammarPartID) )
    {
      const std::string & r_stdstrTokens = //citer->second.
          GetBetweenAsStdString(
          mp_parsebyrise->m_psv, p_pg->m_dwLeftmostIndex,
          p_pg->m_dwRightmostIndex ) ;
      r_wxdc.DrawText( //citer->second.m_stdstrGrammarPartName
          r_stdstrTokens
         , wxPoint( dwXcoordinate ,m_wParseLevel * 20 ) ) ;
      wxSize wxsizeText = r_wxdc.GetTextExtent( wxString(
          r_stdstrTokens) ) ;
      dwXcoordinate += wxsizeText.x ;
      std::pair<std::map<WORD,DWORD>::iterator,bool> pair_iter_bool =
        m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.insert(
          std::pair<WORD,DWORD>( m_wParseLevel, dwXcoordinate )
          ) ;
      //http://www.cplusplus.com/reference/stl/map/insert/:
      //"The pair::second element in the pair is set to true if a new element was
      //inserted or false if an element with the same value existed."
      if( pair_iter_bool.second == false )
      {
        pair_iter_bool.first->second = dwXcoordinate ;
      }
      r_wHorizTextCenter = dwXcoordinate - ( wxsizeText.x / 2 ) ;
      m_stdmap_p_grammarpart2wCenter.insert(
          std::pair<GrammarPart *,WORD>( p_pg, r_wHorizTextCenter )
          ) ;
    }
  }
}

//void
wxSize wxParseTreePanel::GetGrammarPartNameExtent(
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

//void
wxSize wxParseTreePanel::GetTokenExtent(
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

void wxParseTreePanel::OnPaint(wxPaintEvent & event)
{

  wxPaintDC wxpaintdc(this);
  DEBUG_COUT("wxParseTreePanel::OnPaint--mp_parsebyrise:" << mp_parsebyrise
      << "\n")
//  PrepareDC(wxpaintdc);
  if( mp_parsebyrise )
  {
//    DrawParseTreeBeginningFromRoots(wxpaintdc) ;
    DrawParseTreeBeginningFromLeaves(wxpaintdc) ;

//    while( citer != r_stdmultimap_dwLeftmostIndex2grammarpart.end() )
//    {
//#ifdef _DEBUG
//      const GrammarPart & r_grammarpart = citer->second ;
//#endif
//      const std::string & r_stdstrGrammarPartName = //citer->second.
//        //m_stdstrGrammarPartName ;
//        mp_parsebyrise->GetGrammarPartName( r_grammarpart.m_wGrammarPartID ) ;
//      wxpaintdc.DrawText( //citer->second.m_stdstrGrammarPartName
//        r_stdstrGrammarPartName
//         , wxPoint(10,y) ) ;
//      ++ citer ;
//      y += 20 ;
//    }
  }
  else
  {
    wxpaintdc.DrawLine(wxPoint(10,10),wxPoint(199,100)) ;
    wxpaintdc.SetBackground(*wxBLUE_BRUSH);
  }
}
