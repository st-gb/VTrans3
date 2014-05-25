/*
 * DrawParseTreeTraverser.cpp
 *
 *  Created on: May 12, 2010
 *      Author: Stefan
 */
//Include wxWidgets at first to avoid wxWidgets 2.9 errors like
//"error: cannot convert 'const TCHAR*' to 'const WCHAR*' for argument '2' to
//'HWND__* CreateDialogParamW(HINSTANCE__*, const WCHAR*, HWND__*, BOOL (*)
//(HWND__*, UINT, WPARAM, LPARAM), LPARAM)' "
#include <wx/dc.h> //class wxDC
//#include <wx/gdicmn.h>
#include <wx/string.h> //class wxString
#include <wx/font.h> //class wxFont

#include "DrawParseTreeTraverser.hpp"
//#include <Parse/GrammarPart.hpp>
#include <Parse/ParseByRise.hpp> //class ParseByRise
#include <preprocessor_macros/logging_preprocessor_macros.h> //DEBUG_COUTN(...)
#include <wxWidgets/VTransApp.hpp> //::wxGetApp()
//GetwxString_inline(...)
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
#ifdef _WIN32
  //avoid g++ error like "'class wxDC' has no member named 'DrawTextW'"
  #include <wx/msw/winundef.h>
#endif

/** for wxWidgets::GetwxString_Inline */
using namespace wxWidgets;

DrawParseTreeTraverser::DrawParseTreeTraverser(
  wxDC * p_wxdc ,
  ParseByRise * p_parsebyrise ,
  GrammarPart * p_grammarpart
  )
  : ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
    p_grammarpart ,
    p_parsebyrise
    )
  , mp_parsebyrise (p_parsebyrise)
  , m_currentParseTreeLeftEndInPixels(0)
  , m_wParseLevelCountedFromRoot(0)
  , mp_wxdc ( p_wxdc )
{
//  wxFont wxfont = p_wxdc->GetFont() ;
//  wxfont.SetPointSize( 8 ) ;
//  p_wxdc->SetFont( wxfont) ;
  m_stringHeigth = p_wxdc->GetTextExtent(wxT("H")).
    //TODO should be height?
    GetWidth();
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

/** @brief draws
 *  (-grammar part id)
 *  -grammar part name
 *  (-memory address)
 *  */
inline void DrawParseTreeTraverser::DrawGrammarPartAtts(
  const wxString & wxstrGrammarPartName,
  const wxString & wxstrGrammarPartMemoryAddress,
  /*const*/ fastestUnsignedDataType leftEndInPixels,
  const fastestUnsignedDataType stringHeightInPixels
  )
{
  mp_wxdc->DrawText(
    wxstrGrammarPartName , //wXcoord,
    leftEndInPixels ,
    stringHeightInPixels
    ) ;
  if( ! wxstrGrammarPartMemoryAddress.IsEmpty() )
  {
    leftEndInPixels += mp_wxdc->GetTextExtent(wxstrGrammarPartName).GetWidth();
    m_wxcolor.Set( GetwxString_Inline(
      ::wxGetApp().m_GUIattributes.m_std_strGrammarPartIDcolor) );
    mp_wxdc->SetTextForeground(m_wxcolor);
    mp_wxdc->DrawText(
      wxstrGrammarPartMemoryAddress , //wXcoord,
      leftEndInPixels ,
      stringHeightInPixels
      ) ;
    m_wxcolor.Set( wxT("#000000") );
    mp_wxdc->SetTextForeground(m_wxcolor);
  }
}

/** @brief a leave is a token? */
void DrawParseTreeTraverser::LeaveFound()
{
//  WORD wStringWidthLeftChild ;
//  WORD wXcoord = 0
      //wLeftEndOfLeftmostTokenInPixels
      ;
  fastestSignedDataType textWidthInPixels ;
  wxSize wxsizeString, wxsizeSourceTextTokens, wxsizeGermanTranslation;
  wxString wxstrGrammarPartName ;
  wxString wxstrGrammarPartMemoryAddress ;

  GrammarPart * p_grammarpart = m_grammarpartpointer_and_parselevelCurrent.
      m_p_grammarpart ;
  const std::string & std_strGrammarPartName = mp_parsebyrise->
    GetGrammarPartName(p_grammarpart->m_wGrammarPartID);
  LOGN_DEBUG(//"DrawParseTreeTraverser::LeaveFound()--" <<
    std_strGrammarPartName )
  wxString wxstrSourceTextTokens ;
//      wxString wxstrGrammarPartName(r_stdstrGrammarPartName) ;
  wxsizeString = GetGrammarPartNameExtent( * mp_wxdc, p_grammarpart,
    wxstrGrammarPartName, wxstrGrammarPartMemoryAddress ) ;
  textWidthInPixels = wxsizeString.GetWidth() ;

  wxsizeSourceTextTokens = GetSourceTextTokenExtent( * mp_wxdc, p_grammarpart,
    wxstrSourceTextTokens ) ;

//  GetGermanTranslationExtent();
  wxString wxstrGermanTranslation;
  if( ::wxGetApp().m_GUIattributes.m_bShowTranslation )
  {
    wxstrGermanTranslation = //GetwxString_Inline(p_pg->m_stdstrTranslation);
      wxString(p_grammarpart->m_stdstrTranslation.c_str(), wxConvISO8859_1);
    wxsizeGermanTranslation = mp_wxdc->GetTextExtent( wxstrGermanTranslation ) ;
  }
  wxsizeString = wxsizeSourceTextTokens + wxsizeGermanTranslation;

  if ( wxsizeString.GetWidth() > textWidthInPixels )
    textWidthInPixels = wxsizeString.GetWidth() ;

//  wxString wxstrGrammarPartID = wxString::Format( wxT("%u_") ,
//    p_grammarpart->m_wGrammarPartID ) ;
//
//  wxsizeString = mp_wxdc->GetTextExtent( wxstrGrammarPartID ) ;
//  textWidthInPixels += wxsizeString.GetWidth() ;
//  wxstrGrammarPartName = wxstrGrammarPartID + wxstrGrammarPartName ;

//          DrawGrammarPartToken( p_grammarpart, wxpaintdc
//              , wHorizTextCenter ) ;
//          DrawGrammarPartName( p_grammarpart, wxpaintdc
//              , wHorizTextCenter ) ;

  int yCoord = //So the it is drawn below the previous alternative parse tree.
    /*wxsizeString.GetHeight()*/ m_stringHeigth *
    //Was increased before by "BeforeBeginAtRoot".
    (m_wParseLevelCountedFromRoot - 1);

  wxPen wxpenDraw(* wxBLUE//int width = 1, int style = wxSOLID
    );
  mp_wxdc->SetPen( wxpenDraw);
  mp_wxdc->DrawRectangle(m_currentParseTreeLeftEndInPixels, yCoord,
    textWidthInPixels, /*wxsizeString.GetHeight()*/ m_stringHeigth );

  mp_wxdc->SetPen( * wxBLACK_PEN);

  m_wxcolor.Set( GetwxString_Inline(
    ::wxGetApp().m_GUIattributes.m_std_strGrammarPartIDcolor) );
  mp_wxdc->SetTextForeground(m_wxcolor);
#ifdef _DEBUG
  const wxFont & font = mp_wxdc->GetFont(); 
  const int fontPointSize = font.GetPointSize(); 
#endif
  mp_wxdc->DrawText( wxstrGermanTranslation , //wXcoord
    m_currentParseTreeLeftEndInPixels + wxsizeSourceTextTokens.x,
    //0
    yCoord
    ) ;

  m_wxcolor.Set( wxT("#000000") );
  mp_wxdc->SetTextForeground(m_wxcolor);
//  mp_wxdc->Set
  mp_wxdc->DrawText( wxstrSourceTextTokens , //wXcoord
    m_currentParseTreeLeftEndInPixels ,
    //0
    yCoord
    ) ;
//  int yCoordBelow = yCoord + wxsizeString.GetHeight();
//  mp_wxdc->DrawLine(m_currentParseTreeLeftEndInPixels, yCoordBelow,
//    m_currentParseTreeLeftEndInPixels + textWidthInPixels, yCoordBelow );
//  wxstrGrammarPartName += wxString::Format( "%u",
//    p_grammarpart->m_byPersonIndex ) ;

  DrawGrammarPartAtts(
    wxstrGrammarPartName,
    wxstrGrammarPartMemoryAddress,
    m_currentParseTreeLeftEndInPixels,
    /*wxsizeString.GetHeight()*/ m_stringHeigth
    //So the it is drawn below the previous alternative parse tree.
    * (m_wParseLevelCountedFromRoot)
    );

  DrawGrammarPartAttributes draw_grammar_part_attributes(
      //wXcoord + textWidthInPixels / 2 ,
    m_currentParseTreeLeftEndInPixels + textWidthInPixels / 2 ,
      //m_wParseLevel
    //1
    m_wParseLevelCountedFromRoot
    ) ;
  m_currentParseTreeLeftEndInPixels += textWidthInPixels + 10 ;
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

/** @ brief gets size in pixels of the grammar part name */
wxSize DrawParseTreeTraverser::GetGrammarPartNameExtent(
//  wxPaintDC & wxpaintdc ,
  wxDC & r_wxdc ,
  GrammarPart * p_grammarpart ,
  //wxSize & wxsizeText
  wxString & wxstrGrammarPartName,
  wxString & wxstrGrammarPartMemoryAddress
  )
{
  bool bShowGrammarPartAddressInHex = true ;
  const std::string & r_stdstrGrammarPartName = //citer->second.
    //m_stdstrGrammarPartName ;
    mp_parsebyrise->GetGrammarPartName( p_grammarpart->m_wGrammarPartID ) ;
  //if( m_bShowGrammarPartID )
  wxString wxstrGrammarPartID = wxString::Format( wxT("%u_") ,
    p_grammarpart->m_wGrammarPartID ) ;
  wxstrGrammarPartName = wxstrGrammarPartID + //wxString( r_stdstrGrammarPartName ) ;
    GetwxString_Inline(r_stdstrGrammarPartName);
  if( ::wxGetApp().m_GUIattributes.m_bShowGrammarPartAddress )
  {
    //hex. addresses are easier to compare with values in debugger mode.
    if( bShowGrammarPartAddressInHex )
      wxstrGrammarPartMemoryAddress = wxString::Format( wxT("%lx") ,
        (DWORD) p_grammarpart ) ;
    else
      wxstrGrammarPartMemoryAddress = wxString::Format( wxT("%lu") ,
        (DWORD) p_grammarpart ) ;

  }
  wxstrGrammarPartName += wxString::Format( wxT("%u"),
    p_grammarpart->m_byPersonIndex ) ;

  wxSize wxsizeText = r_wxdc.GetTextExtent( wxstrGrammarPartName );
  m_stringHeigth = wxsizeText.GetHeight();
  const int overallWidth = wxsizeText.GetWidth() +
    r_wxdc.GetTextExtent(wxstrGrammarPartMemoryAddress).GetWidth();
  wxsizeText.SetWidth( overallWidth);
  return wxsizeText ;
}

wxSize DrawParseTreeTraverser::GetSourceTextTokenExtent(
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
  wxstr = //wxString(r_stdstrTokens) ;
    GetwxString_Inline(r_stdstrTokens);
//  if( ::wxGetApp().m_GUIattributes.m_bShowTranslation )
//    wxstr += //GetwxString_Inline(p_pg->m_stdstrTranslation);
//      wxString(p_pg->m_stdstrTranslation.c_str(), wxConvISO8859_1);
  wxSize wxsizeText = r_wxdc.GetTextExtent( wxstr ) ;
  return wxsizeText ;
}

//    void RightChildAdded(WORD wCurrentParseTreeLevel) ;
inline void DrawParseTreeTraverser::DrawLineFromRightChildToParent(
  fastestUnsignedDataType & middleBetweenLeftAndRightChild,
  std::map<GrammarPart*, DrawGrammarPartAttributes>::iterator & iterLeft,
  std::map<GrammarPart*, DrawGrammarPartAttributes>::iterator & iterRight,
  fastestUnsignedDataType & childY,
  DrawGrammarPartAttributes * p_dgpaRight,
//  wxSize& wxsizeString,
  fastestUnsignedDataType & thisY
  )
{
  middleBetweenLeftAndRightChild = iterLeft->second.m_wHorizCenterInPixels
    + (iterRight->second.m_wHorizCenterInPixels
    - iterLeft->second.m_wHorizCenterInPixels) / 2;
  childY = (p_dgpaRight->m_wParseLevel + 1) * //wxsizeString.GetHeight();
    m_stringHeigth;
  thisY = m_wParseLevelCountedFromRoot * /*wxsizeString.GetHeight()*/
    m_stringHeigth + 10;
  mp_wxdc->DrawLine(p_dgpaRight->m_wHorizCenterInPixels, childY,
      middleBetweenLeftAndRightChild, thisY);
}

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
//  WORD wStringWidthLeftChild ;
//  WORD wXcoord = 0 ;
  fastestUnsignedDataType middleBetweenLeftAndRightChild ;
  fastestUnsignedDataType leftTextEndInPixels ;
  fastestUnsignedDataType thisY, childY ;
//  WORD wHorizCenterInPixelsOfLeftChild ;
//  WORD wHorizCenterInPixelsOfRightChild ;
  wxSize wxsizeString ;
  wxString wxstrGrammarPartName ;
  wxString wxstrGrammarPartMemoryAddress;

  GrammarPart * p_grammarpart = m_grammarpartpointer_and_parselevelCurrent.
      m_p_grammarpart ;
  iterRight = m_stdmap_p_grammarpart2HorizCenter.find(
      p_grammarpart->mp_grammarpartRightChild ) ;
  iterLeft = m_stdmap_p_grammarpart2HorizCenter.find(
      p_grammarpart->mp_grammarpartLeftChild ) ;

  p_dgpaLeft = & iterLeft->second ;
  p_dgpaRight = & iterRight->second ;
  wxsizeString = GetGrammarPartNameExtent(
    * mp_wxdc,
    p_grammarpart,
    wxstrGrammarPartName,
    wxstrGrammarPartMemoryAddress
    ) ;
  if( p_grammarpart->
      mp_grammarpartRightChild
    )
    {
      DrawLineFromRightChildToParent(
        middleBetweenLeftAndRightChild,
        iterLeft,
        iterRight, childY, p_dgpaRight, //wxsizeString,
        thisY
        ) ;
  }
  else
    middleBetweenLeftAndRightChild = iterLeft->second.m_wHorizCenterInPixels ;
  leftTextEndInPixels = middleBetweenLeftAndRightChild -
      ( wxsizeString.GetWidth() / 2 ) ;
  if( leftTextEndInPixels > 65000 )
    leftTextEndInPixels = 0 ;
  DEBUG_COUTN("wxstrGrammarPartName:" << wxstrGrammarPartName.c_str()
    << " m_wParseLevelCountedFromRoot:" << m_wParseLevelCountedFromRoot
    << " m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel:"
    << m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel
    )

//  wxstrGrammarPartName += wxString::Format( "%u",
//    p_grammarpart->m_byPersonIndex ) ;

  DrawGrammarPartAtts(
    wxstrGrammarPartName ,
    wxstrGrammarPartMemoryAddress,
    leftTextEndInPixels ,
    //m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel *
    m_wParseLevelCountedFromRoot * /*wxsizeString.GetHeight()*/ m_stringHeigth
    ) ;

  childY = ( p_dgpaLeft->m_wParseLevel + 1 ) * //wxsizeString.GetHeight()
    m_stringHeigth;
  thisY = //m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel *
      m_wParseLevelCountedFromRoot *
      /*wxsizeString.GetHeight()*/ m_stringHeigth + 10 ;
  mp_wxdc->DrawLine(
    p_dgpaLeft->m_wHorizCenterInPixels ,
    childY ,
    middleBetweenLeftAndRightChild ,
    thisY
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
      middleBetweenLeftAndRightChild ,
      //m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel
      m_wParseLevelCountedFromRoot
      ) ;
  m_stdmap_p_grammarpart2HorizCenter.insert(
//        std::pair<GrammarPart*,WORD>
//        (p_grammarpart,middleBetweenLeftAndRightChild)
    std::pair<GrammarPart*,DrawGrammarPartAttributes>
    (p_grammarpart,draw_grammar_part_attributes)
    ) ;
}
