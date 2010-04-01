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
    wxPaintDC & wxpaintdc ,
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
    wxpaintdc.DrawText( //citer->second.m_stdstrGrammarPartName
      r_stdstrGrammarPartName
       , wxPoint( dwXcoordinate ,m_wParseLevel * 20) ) ;
    wxSize wxsizeText = wxpaintdc.GetTextExtent( wxString(r_stdstrGrammarPartName) ) ;
    dwXcoordinate += wxsizeText.x ;
    m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.insert(
      std::pair<WORD,DWORD>( m_wParseLevel, dwXcoordinate )
      ) ;
    r_wHorizTextCenter = dwXcoordinate - ( wxsizeText.x / 2 ) ;
    m_stdmap_p_grammarpart2wCenter.insert(
        std::pair<GrammarPart *,WORD>( p_pg, r_wHorizTextCenter )
        ) ;
  }
}

void wxParseTreePanel::DrawGrammarPartChildren( GrammarPart * p_grammarpart ,
    wxPaintDC & r_wxpaintdc )
{
  GrammarPart * p_grammarpartChild = p_grammarpart->mp_grammarpartLeftChild ;
  WORD wChildHorizTextCenter ;
  WORD wHorizTextCenter = 0 ;
  std::map<GrammarPart *,WORD>::iterator iter =
      m_stdmap_p_grammarpart2wCenter.find( p_grammarpart) ;
  DEBUG_COUT("DrawGrammarPartChildren\n")
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
     DrawGrammarPartName(//p_grammarpart->mp_grammarpartLeftChild
         p_grammarpartChild ,r_wxpaintdc, wChildHorizTextCenter ) ;
//          p_pg = p_pg->mp_grammarpartLeftChild ;
     DrawGrammarPartParentToChildLine(wHorizTextCenter,wChildHorizTextCenter
         , r_wxpaintdc );
   }
//   else //Left leave of parse tree found.
//   {
//     std::vector<GrammarPart *>::iterator iter = stdvecNodesToProcess.end() ;
//     p_pg = *iter ;
//     DrawGrammarPartName( *iter , wxpaintdc) ;
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
     DrawGrammarPartName( //p_grammarpart->mp_grammarpartRightChild,
       p_grammarpartChild, r_wxpaintdc, wChildHorizTextCenter ) ;

     //Draw the line from grammar part parent to grammar part child.
     DrawGrammarPartParentToChildLine(wHorizTextCenter,wChildHorizTextCenter
       , r_wxpaintdc );
     DEBUG_COUT( "DrawGrammarPartChildren--pushinG "
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
  WORD wParentHorizTextCenter, WORD wChildHorizTextCenter
  , wxPaintDC & r_wxpaintdc )
{
  r_wxpaintdc.DrawLine( wParentHorizTextCenter, ( m_wParseLevel - 1 )* 20 + 10 ,
    wChildHorizTextCenter , m_wParseLevel * 20 + 10 ) ;
}

void wxParseTreePanel::DrawParseTree( ParseByRise & r_parsebyrise )
{
//  m_wParseLevel = 0 ;
//  m_stdmap_wParseLevelIndex2dwRightEndOfRightmostTokenName.clear() ;
  mp_parsebyrise = & r_parsebyrise ;
  Refresh() ;
}

void wxParseTreePanel::OnPaint(wxPaintEvent & event)
{

  wxPaintDC wxpaintdc(this);
//  PrepareDC(wxpaintdc);
  if( mp_parsebyrise )
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
    citer = r_stdmultimap_dwLeftmostIndex2grammarpart.begin() ;
    p_grammarpart = mp_parsebyrise->GetGrammarPartCoveringMostTokens(
        dwLeftMostTokenIndex ) ;
    if( p_grammarpart )
    {

      const std::string & r_stdstrGrammarPartName = //citer->second.
        //m_stdstrGrammarPartName ;
        mp_parsebyrise->GetGrammarPartName( p_grammarpart->m_wGrammarPartID ) ;
      wxpaintdc.DrawText( //citer->second.m_stdstrGrammarPartName
        r_stdstrGrammarPartName
         , wxPoint(200,0) ) ;
      m_stdmap_p_grammarpart2wCenter.insert( std::pair<GrammarPart *,WORD>
        (p_grammarpart, 200)) ;
      ++ m_wParseLevel ;
      do
      {
//        if( p_pg->mp_grammarpartLeftChild )
//        {
//          DrawGrammarPartName(p_pg->mp_grammarpartLeftChild,wxpaintdc) ;
////          p_pg = p_pg->mp_grammarpartLeftChild ;
//        }
//        else //Left leave of parse tree found.
//        {
//          std::vector<GrammarPart *>::iterator iter = stdvecNodesToProcess.end() ;
//          p_pg = *iter ;
//          DrawGrammarPartName( *iter , wxpaintdc) ;
//          stdvecNodesToProcess.pop_back() ;
//          -- m_wParseLevel ;
//        }
//        if( p_pg->mp_grammarpartRightChild )
//        {
//          DrawGrammarPartName(p_pg->mp_grammarpartRightChild, wxpaintdc) ;
//          //Add to process possible children of the right nodes later.
//          stdvecNodesToProcess.push_back(p_pg->mp_grammarpartRightChild) ;
////          ++ m_wParseLevel ;
//        }
        DrawGrammarPartChildren( p_grammarpart , wxpaintdc ) ;
        if( p_grammarpart->mp_grammarpartLeftChild )
        {
          p_grammarpart = p_grammarpart->mp_grammarpartLeftChild ;
          ++ m_wParseLevel ;
        }
        else //Left leave of parse tree found.
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
            p_grammarpart->m_wGrammarPartID ) << " current size=" <<
                m_stdvecNodesToProcess.size() << "\n" )
//          DrawGrammarPartName( *iter , wxpaintdc) ;
          m_stdvecNodesToProcess.pop_back() ;
          //std::cout.flush() ;
          //usleep(100000) ;
          -- m_wParseLevel ;
        }
      }
      while( ! m_stdvecNodesToProcess.empty() ) ;
      DEBUG_COUT("OnPaint--no more nodes to process")
    }
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
