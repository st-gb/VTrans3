/*
 * wxGermanTranslationPanel.cpp
 *
 *  Created on: 12.08.2011
 *      Author: Stefan
 */
#include "wxGermanTranslationPanel.hpp"

#include <wx/choice.h>
#include <wx/dc.h> //class wxDC
#include <wx/dcbuffer.h> //class wxBufferedPaintDC
#include <wx/dcclient.h> //for class wxPaintDC
#include <map> //class std::multimap
#include <set> //class std::set

#include <Attributes/EnglishWord.hpp> //enum EnglishWord::English_word_class
//class TranslationAndGrammarPart
#include <Attributes/TranslationAndGrammarPart.hpp>
#include <Parse/GrammarPart.hpp> //class GrammarPart
//getwxString(...)
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
#include <wxWidgets/VTransApp.hpp> //wxGetApp(...)

//http://docs.wxwidgets.org/stable/wx_eventhandlingoverview.html#eventhandlingoverview
BEGIN_EVENT_TABLE(wxGermanTranslationPanel, wxPanel)
  EVT_PAINT(wxGermanTranslationPanel::OnPaint)
  EVT_SIZE( wxGermanTranslationPanel::OnSize)
  EVT_ERASE_BACKGROUND(wxGermanTranslationPanel::OnEraseBackground)
END_EVENT_TABLE()

/** for wxWidgets::GetwxString_Inline */
using namespace wxWidgets;

wxGermanTranslationPanel::wxGermanTranslationPanel()
    : m_p_wxbitmapForMemoryDC(NULL)
{
//  wxChoice * p_wxchoice = new wxChoice(
//    this, //wxWindow *    parent,
//    wxID_ANY, // wxWindowID    id,
//    wxDefaultPosition, //const wxPoint &   pos = wxDefaultPosition,
//    wxDefaultSize //const wxSize & size = wxDefaultSize,
////    int   n = 0,
////    const wxString    choices[] = NULL,
////    long    style = 0,
////    const wxValidator &   validator = wxDefaultValidator,
////    const wxString &    name = wxChoiceNameStr
//    );
//  p_wxchoice->Get
//  Add()
}

void wxGermanTranslationPanel::Create()
{
  LOGN_DEBUG("begin")
  m_wxrectClient = GetClientRect();
  LOGN_DEBUG("width:" << m_wxrectClient.width << " height:" <<
    m_wxrectClient.height)
    const bool creationSucceeded = m_wxbitmapForMemoryDC.Create(
      m_wxrectClient.width, m_wxrectClient.height, -1);
  /** Only works with local wxMemoryDC object, not with member variable. */
  wxMemoryDC wxmemorydc(m_wxbitmapForMemoryDC);
  /** http://docs.wxwidgets.org/trunk/classwx_memory_d_c.html#a93d218796ba9359eb4aec2ae46a813e6:
  * "Use the wxDC::IsOk() member to test whether the constructor was successful
  * in creating a usable device context. Don't forget to select a bitmap into
  * the DC before drawing on it."  */
  if( wxmemorydc.IsOk() //&& m_wxrectClient.GetWidth() > 0 &&
      //m_wxrectClient.GetHeight() > 0
      )
  {
//    const bool creationSucceeded = m_wxbitmapForMemoryDC.Create(
//      m_wxrectClient.width, m_wxrectClient.height, -1);
    if( creationSucceeded )
    {
//      LOGN_DEBUG("before SelectObject")
//      m_wxmemorydc.SelectObject(wxNullBitmap);
      LOGN_DEBUG("before SelectObject")
//        if( m_p_wxbitmapForMemoryDC )
//          delete m_p_wxbitmapForMemoryDC ;
//      m_p_wxbitmapForMemoryDC = new wxBitmap(
//        m_wxrectClient.width,
//        m_wxrectClient.height,
//        //http://docs.wxwidgets.org/stable/wx_wxbitmap.html#wxbitmapctor:
//        //"A depth of -1 indicates the depth of the current screen or visual."
//        -1) ;

//      wxmemorydc.SelectObject( m_wxbitmapForMemoryDC);
//      m_wxmemorydc.SelectObject( //m_wxbitmapForMemoryDC
//        * m_p_wxbitmapForMemoryDC );
//      LOGN_DEBUG("before FloodFill")
      wxmemorydc.FloodFill(0,0, * wxWHITE,
        //* wxBLACK,
        wxFLOOD_BORDER //: the area to be flooded is bounded by the given colour.
        );
    //  m_wxbitmapForMemoryDC.S
      DrawTranslationAndCreateChoices(wxmemorydc);
      
      /** http://www.informit.com/articles/article.aspx?p=405047 :
       *  "When you have finished with the device context, you should call 
       *  SelectObject with wxNullBitmap to remove the association." */
      wxmemorydc.SelectObject(wxNullBitmap);
    }
    else
      LOGN_ERROR("creation of bitmap for memory DC failed")
  }
  else
    LOGN_ERROR("memory DC is not OK")
  LOGN_DEBUG("end")
}

void GetTokenIndex2TranslationAndConcatenationID(
  const std::vector<std::vector<TranslationAndGrammarPart> > &
    r_std_vector_stdvecTranslationAndGrammarPart,
//  std::multimap<unsigned, TranslationAndConcatenationID> &
//    std_map_uiTokenIndex2translationandconcatenationid
  std::multimap<unsigned, GrammarPartPointerAndConcatenationID> &
    std_multimap_uiTokenIndex2grammarpartpointerandconcatenationid
  )
{
  unsigned uiTokenIndex = 0;
  std::vector<std::vector<TranslationAndGrammarPart> >::const_iterator c_iter =
    //m_std_vector_stdvecTranslationAndGrammarPart.begin();
    r_std_vector_stdvecTranslationAndGrammarPart.begin();
  while( c_iter != //m_std_vector_stdvecTranslationAndGrammarPart.end()
      r_std_vector_stdvecTranslationAndGrammarPart.end()
      )
  {
    std::vector<TranslationAndGrammarPart>::const_iterator
      c_iterInner = c_iter->begin();
    uiTokenIndex = 0;
    while( c_iterInner != c_iter->end() )
    {
//        std_map_ui2std_str.insert(
//          std::make_pair(
//            uiTokenIndex,
//            c_iterInner->m_stdstrTranslation.c_str()
//            )
//          );
//      std_map_uiTokenIndex2translationandconcatenationid.insert(
//        std::make_pair(
//          uiTokenIndex,
//          TranslationAndConcatenationID(
//            c_iterInner->m_stdstrTranslation,
//            c_iterInner->mp_grammarpart->m_ui32ConcatenationID
//            )
//          )
//        );
      std_multimap_uiTokenIndex2grammarpartpointerandconcatenationid.insert(
          std::make_pair(
            uiTokenIndex,
            GrammarPartPointerAndConcatenationID(
              c_iterInner->mp_grammarpart,
              c_iterInner->mp_grammarpart->m_ui32ConcatenationID
              )
            )
          );
      ++ uiTokenIndex;
      ++ c_iterInner;
    }
    ++ c_iter;
  }
}

void wxGermanTranslationPanel::DrawParseTreesAtSameTokenIndex(
//  * c_iterAllSentencesAtSameTokenIndex
  const std::vector<std::vector<TranslationAndGrammarPart> > &
     std_vector_stdvecTranslationAndGrammarPart,
   wxDC & r_wxdc,
   wxCoord & wxcoordX,
   wxCoord & wxcoordY
  )
{
  LOGN_DEBUG("begin")
  unsigned uiTokenIndex = 0;
//  std::vector<std::vector<TranslationAndGrammarPart> >
//     std_vector_stdvecTranslationAndGrammarPart;

//  std::multimap<unsigned, TranslationAndConcatenationID>
//    std_map_uiTokenIndex2translationandconcatenationid;
  std::multimap<unsigned, GrammarPartPointerAndConcatenationID>
    std_multimap_uiTokenIndex2grammarpartpointerandconcatenationid;

  GetTokenIndex2TranslationAndConcatenationID(
//    * c_iterAllSentencesAtSameTokenIndex,
    std_vector_stdvecTranslationAndGrammarPart,
//    std_map_uiTokenIndex2translationandconcatenationid
    std_multimap_uiTokenIndex2grammarpartpointerandconcatenationid
    );

//  std::multimap<unsigned, const char *>::const_iterator c_iterToken =
//      std_map_ui2std_str.begin();

//  std::multimap<unsigned, TranslationAndConcatenationID>::const_iterator
//    c_iter_std_map_uiTokenIndex2translationandconcatenationid =
//    std_map_uiTokenIndex2translationandconcatenationid.begin();
  std::multimap<unsigned, GrammarPartPointerAndConcatenationID>::const_iterator
    c_iter_std_multimap_uiTokenIndex2grammarpartpointerandconcatenationid =
      std_multimap_uiTokenIndex2grammarpartpointerandconcatenationid.begin();

  uiTokenIndex =
    //Init with value <> 0 for the 1st comparison ("... == uiTokenIndex" )
    //to be false.
    0;
  //  std::set<std::string> std_set_std_strToken;
//  std::set<TranslationAndConcatenationID>
//    std_set_translationandconcatenationid;
  std::set<GrammarPartPointerAndConcatenationID>
    std_set_grammarpartpointerandconcatenationid;

  std::vector<wxString> std_vector_wxstr;
//  const char * p_chTranslation = NULL;
//  uint32_t ui32ConcatenationID = 0;
  wxArrayString wxarraystringTranslation;

//  const TranslationAndConcatenationID * p_translationandconcatenationid = NULL;
  const GrammarPartPointerAndConcatenationID *
    p_grammarpartpointerandconcatenationid = NULL;

  while( //c_iterToken != //std_map_ui2std_str.end()
//      c_iter_std_map_uiTokenIndex2translationandconcatenationid !=
//      std_map_uiTokenIndex2translationandconcatenationid.end()
      c_iter_std_multimap_uiTokenIndex2grammarpartpointerandconcatenationid !=
      std_multimap_uiTokenIndex2grammarpartpointerandconcatenationid.end()
      )
  {
    if( //c_iterToken->first == uiTokenIndex
//      c_iter_std_map_uiTokenIndex2translationandconcatenationid->first ==
      c_iter_std_multimap_uiTokenIndex2grammarpartpointerandconcatenationid->
        first ==
          uiTokenIndex
        )
    {
  //      std_vector_wxstr.push_back( wxString(c_iterInner->m_stdstrTranslation.
  //        c_str() ) );
  //      ui32ConcatenationID =
  //        c_iter_std_map_uiTokenIndex2translationandconcatenationid->second.
  //        m_ui32ConcatenationID;

//      p_translationandconcatenationid = &
//        c_iter_std_map_uiTokenIndex2translationandconcatenationid->second;
      p_grammarpartpointerandconcatenationid = &
        c_iter_std_multimap_uiTokenIndex2grammarpartpointerandconcatenationid->
        second;

  //      if( ui32ConcatenationID == 0 )
        //if a translated word has no concatenation and another such translated
        //word exists, there should not be double translations:
        //English: "the fan works"
        // "der(concatenation ID=2) Anhänger(concatenation ID=2) arbeitet(concatenation ID=0)" <- translation for 1st parse tree.
        // "das Gebläse arbeitet(concatenation ID=0)" <- translation for 2nd parse tree.
        // -> in std::set only 1 "arbeitet" for token index 2
//        std_set_translationandconcatenationid.insert(
//          * p_translationandconcatenationid );

        std_set_grammarpartpointerandconcatenationid.insert(
          * p_grammarpartpointerandconcatenationid);

  //      if( wxarraystringTranslation.insert() )
  //      wxarraystringTranslation.Add( wxString(c_iterToken->second) );
  //      std_set_std_strToken.insert( std::string(c_iterToken->second) )
    }
    else
    {
      uiTokenIndex = //c_iterToken->first;
//        c_iter_std_map_uiTokenIndex2translationandconcatenationid->first;
        c_iter_std_multimap_uiTokenIndex2grammarpartpointerandconcatenationid->
        first;
  //      if( wxarraystringTranslation.size() > 0
  //          //std_set_std_strToken.size() > 0
  //        )
      PossiblyAddChoice(//wxarraystringTranslation
        //std_set_translationandconcatenationid
        std_set_grammarpartpointerandconcatenationid,
        r_wxdc, wxcoordX, wxcoordY);
  //      std_vector_wxstr.clear();
      wxarraystringTranslation.Clear();
//      std_set_translationandconcatenationid.clear();
      std_set_grammarpartpointerandconcatenationid.clear();
  //      wxarraystringTranslation.Add( wxString(c_iterToken->second) );

//      std_set_translationandconcatenationid.insert(
//        c_iter_std_map_uiTokenIndex2translationandconcatenationid->second );
      std_set_grammarpartpointerandconcatenationid.insert(
        c_iter_std_multimap_uiTokenIndex2grammarpartpointerandconcatenationid->
        second);
  //      wxcoordX += 50;
    }
  //    ++ c_iterToken;
//    ++ c_iter_std_map_uiTokenIndex2translationandconcatenationid;
    ++ c_iter_std_multimap_uiTokenIndex2grammarpartpointerandconcatenationid;
  }
  //  if( wxarraystringTranslation.size() > 0 )
  PossiblyAddChoice(//wxarraystringTranslation
//    std_set_translationandconcatenationid
    std_set_grammarpartpointerandconcatenationid
    , r_wxdc, wxcoordX, wxcoordY);
  LOGN_DEBUG("end")
}

void wxGermanTranslationPanel::DrawParseTreesFromLeaves(wxDC & r_wxdc)
{
  LOGN_DEBUG("begin")
  wxCoord wxcoordX = 0;
  wxCoord wxcoordY = 0;
  std::vector<std::vector<std::vector<TranslationAndGrammarPart> > >
    ::const_iterator c_iterAllSentencesAtSameTokenIndex =
    m_stdvec_stdvec_stdvecTranslationAndGrammarPart.begin();

  while(c_iterAllSentencesAtSameTokenIndex !=
    m_stdvec_stdvec_stdvecTranslationAndGrammarPart.end()
    )
  {
    DrawParseTreesAtSameTokenIndex(* c_iterAllSentencesAtSameTokenIndex
      , r_wxdc, wxcoordX, wxcoordY);
    ++ c_iterAllSentencesAtSameTokenIndex;
  }
  LOGN_DEBUG("end")
}

void wxGermanTranslationPanel::DrawTranslationAndCreateChoices(wxDC & r_wxdc)
{
  LOGN_DEBUG("begin")
  //Delete existing choice windows
  bool bOk =
    //"returns true if ok"
    DestroyChildren();
  SUPPRESS_UNUSED_VARIABLE_WARNING(bOk)
  m_std_multimap_ui32ConcatenationID2p_wxchoice.clear();
  m_std_multimap_p_wxchoice2ui32ConcatenationID.clear();

  m_wxwindowidCurrent = 0;

  wxSize wxsizeText;
  wxString wxstrTranslation;
//  int x = 0;

//  std::multimap<unsigned, const char *> std_map_ui2std_str;

  DrawParseTreesFromLeaves(r_wxdc);

//  std::vector<std::vector<GrammarPart *> >::const_iterator
//    c_iterParseTreesAtSameTokenIndex =
//    m_p_std_vector_std_vector_p_grammarpartCoveringMostTokensAtTokenIndex->
//    begin();
//  while( c_iterParseTreesAtSameTokenIndex !=
//      m_p_std_vector_std_vector_p_grammarpartCoveringMostTokensAtTokenIndex.
//      end()
//      )
//  {
//    //Save token text extent size (width, height (choices are higher) of token
//    //in tree leaves?!
//    Get
//    if( )
//    ++ c_iterParseTreesAtSameTokenIndex;
//  }

//  //important when the choice controls are created after a resize?!
//  Layout();
//  Fit();
  LOGN_DEBUG("end")
}

void wxGermanTranslationPanel::DrawTranslationFromAllParseTrees(wxDC & r_wxdc)
{
  wxSize wxsizeText;
  wxString wxstrTranslation;
//  int x = 0;
//  wxCoord wxcoordY = 0;

//  std::vector<std::vector<TranslationAndGrammarPart> >::const_iterator c_iter =
//    m_std_vector_stdvecTranslationAndGrammarPart.begin();
//  while( c_iter != m_std_vector_stdvecTranslationAndGrammarPart.end() )
//  {
//      std::vector<TranslationAndGrammarPart>::const_iterator
//        c_iterInner = c_iter->begin();
//      while( c_iterInner != c_iter->end() )
//      {
//          wxstrTranslation =
//            //getwxString(...)/ wxConvUTF8(...) failed for German umlaut
//            //('ä'/ decimal 228)
//            //getwxString(c_iterInner->m_stdstrTranslation);
//              wxString(c_iterInner->m_stdstrTranslation.c_str() );
//          switch( c_iterInner->mp_grammarpart->m_wGrammarPartID)
//          {
//          case EnglishWord::singular:
//            r_wxdc.SetTextForeground( * wxGREEN);
//            break;
//          default:
//            r_wxdc.SetTextForeground( * wxBLACK);
//            break;
//          }
//          r_wxdc.DrawText( wxstrTranslation, x, wxcoordY );
//          wxsizeText = r_wxdc.GetTextExtent( wxstrTranslation ) ;
//          x += wxsizeText.x;
//          ++ c_iterInner;
//      }
//      x = 0;
//      wxsizeText = r_wxdc.GetTextExtent(
//        //the last token/ word may be empty-> text extent is (0;0):
//        //so get text extent from "W"
//        wxT("W") );
//      wxcoordY += wxsizeText.y;
//      ++ c_iter;
//  }
}

void GetColorAccordingToGrammarPart(
  const std::set<GrammarPartPointerAndConcatenationID> &
    std_set_grammarpartpointerandconcatenationid,
//  std::set<wxColour> & r_std_set_wxcolour
    std::set<uint32_t> & r_std_set_ui32Colour
  )
{
//  unsigned char m_red;
//   unsigned char m_blue;
//   unsigned char m_green;
  std::set<GrammarPartPointerAndConcatenationID>::const_iterator
    c_iter_std_set_grammarpartpointerandconcatenationid =
    std_set_grammarpartpointerandconcatenationid.begin();

  WORD wGrammarPartID = 0;
  while( c_iter_std_set_grammarpartpointerandconcatenationid !=
     std_set_grammarpartpointerandconcatenationid.end()
   )
  {
    wGrammarPartID = c_iter_std_set_grammarpartpointerandconcatenationid->
      m_p_grammarpartToken->m_wGrammarPartID;
//    switch(wGrammarPartID)
//    {
//    case EnglishWord::singular:
////      r_std_set_wxcolour.insert( * wxGREEN);
//      r_std_set_ui32Colour.insert( wxGREEN->GetPixel() );
//      break;
//    case EnglishWord::English_definite_article:
////      r_std_set_wxcolour.insert( * wxBLUE);
//      r_std_set_ui32Colour.insert( wxBLUE->GetPixel() );
//      break;
//    default:
////      r_std_set_wxcolour.insert( * wxBLACK);
//      r_std_set_ui32Colour.insert( wxBLACK->GetPixel() );
//      break;
//    }
    std::string std_strGrammarPartName = wxGetApp().m_parsebyrise.
      GetGrammarPartName(wGrammarPartID);
    NodeTrieNode<uint32_t> * p_ntn = wxGetApp().
      m_nodetrie_ui32GrammarPartName2colour.contains_inline(
      (unsigned char *) std_strGrammarPartName.c_str(),
      (NodeTrie<uint32_t>::size_type) std_strGrammarPartName.length(),
      true //bool bFullMatch
      );
    if( p_ntn )
      r_std_set_ui32Colour.insert( p_ntn->m_member);
    else
      r_std_set_ui32Colour.insert( wxBLACK->GetPixel() );
    ++ c_iter_std_set_grammarpartpointerandconcatenationid;
  }
}

void UnderlineGrammarParts(
  std::set<GrammarPartPointerAndConcatenationID> &
    std_set_grammarpartpointerandconcatenationid,
  wxDC & r_wxdc,
  wxCoord wxcoordXBeginOfToken,
  wxCoord wxcoordXEndOfToken,
//  wxCoord wxcoordY
  wxCoord wxcoordYBottom
  )
{
  wxPen r_wxpen = r_wxdc.GetPen();

  //    std::set<wxColour> std_set_wxcolour;
  std::set<uint32_t> std_set_ui32Colour;

  GetColorAccordingToGrammarPart(
      std_set_grammarpartpointerandconcatenationid, //std_set_wxcolour
      std_set_ui32Colour);
  //    std::set<wxColour>::const_iterator c_iter_std_set_wxcolour =
  //        std_set_wxcolour.begin();
  std::set<uint32_t>::const_iterator c_iter_std_set_ui32 =
    std_set_ui32Colour.begin();
  wxPen wxpenDraw(* wxGREEN//int width = 1, int style = wxSOLID
    );
  while( //c_iter_std_set_wxcolour != std_set_wxcolour.end()
      c_iter_std_set_ui32 != std_set_ui32Colour.end()
      )
  {
    wxpenDraw.SetColour( //* c_iter_std_set_wxcolour
      wxColour( * c_iter_std_set_ui32) );
    r_wxdc.SetPen(wxpenDraw);
    r_wxdc.DrawLine(
      wxcoordXBeginOfToken, //wxcoordYBeginOfToken
      //wxcoordY + 20
      wxcoordYBottom,
      wxcoordXEndOfToken,
      //wxcoordY + 20
      wxcoordYBottom
      );
//    wxcoordY += 5;
    wxcoordYBottom += 2;

  //      ++ c_iter_std_set_wxcolour;
    ++ c_iter_std_set_ui32;
  }
  r_wxdc.SetPen(r_wxpen);
}

void wxGermanTranslationPanel::AddChoice(
  wxWindowID wxwindowid,
  wxCoord & wxcoordX,
  wxCoord & wxcoordY,
  const wxArrayString & wxarraystringTranslation,
  wxCoord wxcoordYBottom,
  const bool bConnectedControl,
  uint32_t ui32ConcatenationID
  )
{
  wxChoice * p_wxchoice = new wxChoice(
    this, //wxWindow *    parent,
//      wxID_ANY, // wxWindowID    id,
    wxwindowid,
    wxPoint(wxcoordX, wxcoordY), //const wxPoint &   pos = wxDefaultPosition,
    wxDefaultSize, //const wxSize & size = wxDefaultSize,
    //    int   n = 0,
    //    const wxString    choices[] = NULL,
    //    long    style = 0,
    //    const wxValidator &   validator = wxDefaultValidator,
    //    const wxString &    name = wxChoiceNameStr
//        c_r_wxarraystringTranslation //const wxArrayString & choices,
    wxarraystringTranslation
    //long style=0,
    //const wxValidator & validator = wxDefaultValidator,
    //const wxString &name=wxChoiceNameStr
    );
  p_wxchoice->Select(0);
//      p_wxchoice->SetMinClientSize (const wxSize &size);
//      p_wxchoice->
  wxSize wxsizeChoiceControl = p_wxchoice->GetSize();
  wxcoordX += wxsizeChoiceControl.GetWidth();
  wxcoordYBottom = wxcoordY + wxsizeChoiceControl.GetHeight() + 2;
  if( bConnectedControl )
  {
    Connect( wxwindowid, wxEVT_COMMAND_CHOICE_SELECTED,
      wxCommandEventHandler(
        wxGermanTranslationPanel::SelectConnectedListEntries) );
    m_std_multimap_ui32ConcatenationID2p_wxchoice.insert( std::make_pair(
      ui32ConcatenationID, p_wxchoice)
      );
    m_std_multimap_p_wxchoice2ui32ConcatenationID.insert(
      std::make_pair(p_wxchoice, ui32ConcatenationID)
      );
  }
}

void wxGermanTranslationPanel::PossiblyAddChoice(
//    const wxArrayString & c_r_wxarraystringTranslation,
//  std::set<TranslationAndConcatenationID> &
//    std_set_translationandconcatenationid,
  std::set<GrammarPartPointerAndConcatenationID> &
    std_set_grammarpartpointerandconcatenationid,
  wxDC & r_wxdc,
  wxCoord & wxcoordX,
  wxCoord & wxcoordY
  )
{
  bool bConnectedControl = false;
  const char * p_chTranslation = NULL;
  uint32_t ui32ConcatenationID = 0;
  wxArrayString wxarraystringTranslation;
  wxSize wxsizeText;

  wxCoord wxcoordXBeginOfToken = wxcoordX;
//  wxCoord wxcoordYBeginOfToken = wxcoordY;

  wxWindowID wxwindowid = wxID_ANY;
//  std::set<TranslationAndConcatenationID>::const_iterator
//    c_iter_std_set_translationandconcatenationid =
//    std_set_translationandconcatenationid.begin();
//  int nSize = std_set_translationandconcatenationid.size();
  std::set<GrammarPartPointerAndConcatenationID>::const_iterator
    c_iter_std_set_grammarpartpointerandconcatenationid =
    std_set_grammarpartpointerandconcatenationid.begin();
//  int nSize = std_set_grammarpartpointerandconcatenationid.size();

  while( //c_iter_std_set_translationandconcatenationid !=
    //std_set_translationandconcatenationid.end()
      c_iter_std_set_grammarpartpointerandconcatenationid !=
          std_set_grammarpartpointerandconcatenationid.end()
    )
  {
    ui32ConcatenationID = //c_iter_std_set_translationandconcatenationid->
      c_iter_std_set_grammarpartpointerandconcatenationid->
      m_ui32ConcatenationID;
    if( ui32ConcatenationID != 65535 )
    {
      wxwindowid = ++ m_wxwindowidCurrent;
      bConnectedControl = true;
    }
    p_chTranslation = //c_iter_std_set_translationandconcatenationid->
//      m_std_strTranslation.c_str();
      c_iter_std_set_grammarpartpointerandconcatenationid->
      m_p_grammarpartToken->m_stdstrTranslation.c_str();
    const wxString wxstrGermanTranslation = //GetwxString_Inline( p_chTranslation);
      //wxString::FromAscii(p_chTranslation);
    /** @see http://de.wikipedia.org/wiki/ISO_8859-1 */
    /*wxstr =*/ wxString(p_chTranslation, wxConvISO8859_1);
    wxarraystringTranslation.Add( wxstrGermanTranslation );
//    ++ c_iter_std_set_translationandconcatenationid;
    ++ c_iter_std_set_grammarpartpointerandconcatenationid;
  }
  if( wxarraystringTranslation.size() > 0
      //std_set_std_strToken.size() > 0
    )
  {
    wxCoord wxcoordYBottom;
    if( //c_r_wxarraystringTranslation.size() > 1
      wxarraystringTranslation.size() > 1
      )
    {
      AddChoice(
        wxwindowid,
        wxcoordX,
        wxcoordY,
        wxarraystringTranslation,
        wxcoordYBottom,
        bConnectedControl,
        ui32ConcatenationID
        );
    }
    else //if( )
    {
      wxsizeText = r_wxdc.GetTextExtent( //c_r_wxarraystringTranslation[0]
        wxarraystringTranslation[0]) ;
      if( wxcoordX + wxsizeText.x > m_wxrectClient.width )
      {
        wxcoordY += wxsizeText.y;
        wxcoordX = 0;
      }
#ifdef _DEBUG
      const wxChar * c_p_wxch = wxarraystringTranslation[0].c_str();
      SUPPRESS_UNUSED_VARIABLE_WARNING(c_p_wxch)
#endif
      r_wxdc.DrawText( //c_r_wxarraystringTranslation[0]
        wxarraystringTranslation[0], wxcoordX
        , //0
        wxcoordY
        );
//      wxcoordX += wxsizeText.x + r_wxdc.GetTextExtent( wxT(" ") ).x;
      wxcoordX += wxsizeText.x;
      wxcoordYBottom = wxcoordY + wxsizeText.y;
    }

    UnderlineGrammarParts(std_set_grammarpartpointerandconcatenationid,
        r_wxdc, wxcoordXBeginOfToken, wxcoordX, //wxcoordY
        wxcoordYBottom);
    wxcoordX += r_wxdc.GetTextExtent( wxT(" ") ).x;
  }
}

//see http://docs.wxwidgets.org/2.9.2/classwx_choice.html:
// "void handlerFuncName(wxCommandEvent& event)"
void wxGermanTranslationPanel::SelectConnectedListEntries(
  wxCommandEvent & r_wxcommandevent)
{
  wxObject * p_wxobjectSelectionChanged = r_wxcommandevent.GetEventObject();
  if( ! p_wxobjectSelectionChanged)
    return;
  int nWindowID = r_wxcommandevent.GetId();
  wxChoice * p_wxchoiceSelectionChanged = (wxChoice *)
      p_wxobjectSelectionChanged;
  if( ! p_wxchoiceSelectionChanged)
    return;

  std::set<wxChoice *>::const_iterator
    c_iterstd_set_p_wxchoiceDropSelectionHandling =
        m_std_set_p_wxchoiceDropSelectionHandling.find(
              p_wxchoiceSelectionChanged);
  if( c_iterstd_set_p_wxchoiceDropSelectionHandling ==
     m_std_set_p_wxchoiceDropSelectionHandling.end()
     )
  {
      int nSelectionIndex = r_wxcommandevent.GetSelection();

    std::multimap<int,int>::const_iterator c_iter =
      m_std_map_nWindowIDselectedChoice2nWindowIDalsoSelect.find(nWindowID);
  //  while( c_iter != m_std_map_nWindowIDselectedChoice2nWindowIDalsoSelect.end()
  //      )
  //  {
  ////    int i = 2;
  //      ++ c_iter;
  //  }

    uint32_t ui32ConcatenationID = 0;
    std::multimap<wxChoice *,uint32_t>::const_iterator
      c_iter_std_multimap_p_wxchoice2ui32ConcatenationID =
      m_std_multimap_p_wxchoice2ui32ConcatenationID.find(
          //p_wxobjectSelectionChanged
          p_wxchoiceSelectionChanged);

    std::multimap<uint32_t,wxChoice *>::const_iterator
      c_iter_std_multimap_ui32ConcatenationID2p_wxchoice;
    wxChoice * p_wxchoiceAlsoSelect = NULL;

#ifdef _DEBUG
    int n_m_std_multimap_p_wxchoice2ui32ConcatenationID_size =
      m_std_multimap_p_wxchoice2ui32ConcatenationID.size();
    SUPPRESS_UNUSED_VARIABLE_WARNING(
      n_m_std_multimap_p_wxchoice2ui32ConcatenationID_size)
    int n_m_std_multimap_ui32ConcatenationID2p_wxchoice_size =
      m_std_multimap_ui32ConcatenationID2p_wxchoice.size();
    SUPPRESS_UNUSED_VARIABLE_WARNING(
      n_m_std_multimap_ui32ConcatenationID2p_wxchoice_size)
#endif //#ifdef _DEBUG

    while( c_iter_std_multimap_p_wxchoice2ui32ConcatenationID !=
        m_std_multimap_p_wxchoice2ui32ConcatenationID.end() &&
        //Because this is a multimap multiple (concatenation ID) values/ entries
        //may exist for the same (wxChoice pointer) key, so stop when the
        //pointer value changes.
        c_iter_std_multimap_p_wxchoice2ui32ConcatenationID->first ==
//        p_wxobjectSelectionChanged
        p_wxchoiceSelectionChanged
        )
    {
      ui32ConcatenationID = c_iter_std_multimap_p_wxchoice2ui32ConcatenationID->
        second;
      c_iter_std_multimap_ui32ConcatenationID2p_wxchoice =
        m_std_multimap_ui32ConcatenationID2p_wxchoice.find(ui32ConcatenationID);
      while( c_iter_std_multimap_ui32ConcatenationID2p_wxchoice !=
          m_std_multimap_ui32ConcatenationID2p_wxchoice.end() &&
          c_iter_std_multimap_ui32ConcatenationID2p_wxchoice->first ==
            ui32ConcatenationID
        )
      {
        p_wxchoiceAlsoSelect =
          c_iter_std_multimap_ui32ConcatenationID2p_wxchoice->second;
        if( p_wxchoiceAlsoSelect !=
            p_wxobjectSelectionChanged)
        {
//            //->Do not process selections made here.
//            m_std_set_p_wxchoiceDropSelectionHandling.insert(
//                p_wxchoiceAlsoSelect);
            //this causes a new selection event?!
            //TODO SIGSEV here
            p_wxchoiceAlsoSelect->Select(nSelectionIndex);
        }
        ++ c_iter_std_multimap_ui32ConcatenationID2p_wxchoice;
      }
      ++ c_iter_std_multimap_p_wxchoice2ui32ConcatenationID;
    }
  }
  else
  {
      m_std_set_p_wxchoiceDropSelectionHandling.erase(
          c_iterstd_set_p_wxchoiceDropSelectionHandling);
  }
//  m_bHandleNextSelection =
}

void wxGermanTranslationPanel::DrawMemoryDCbitmap(wxPaintDC & r_wxpaintdc)
{
//  wxRect wxrectClient = GetClientRect();

//  //http://docs.wxwidgets.org/stable/wx_wxmemorydc.html:
//  r_wxpaintdc.Blit(
//    0, //wxCoord xdest,
//    0, //wxCoord ydest,
//    wxrectClient.width, //wxCoord width,
//    wxrectClient.height, //wxCoord height
//    & m_wxmemorydc, //wxDC *source,
//    0, //wxCoord xsrc,
//    0 //wxCoord ysrc
//    );
  r_wxpaintdc.DrawBitmap(m_wxbitmapForMemoryDC, wxPoint(0,0), false);
}

//http://docs.wxwidgets.org/stable/wx_wxeraseevent.html#wxeraseevent
void wxGermanTranslationPanel::OnEraseBackground(wxEraseEvent & event)
{
  //Do nothing (else the choices are overdrawn by the default erase procedure?!)
}

/** http://www.informit.com/articles/article.aspx?p=405047 :
 *  "Paint events are generated when user interaction causes regions to need 
 *  repainting, but they can also be generated as a result of wxWindow::Refresh 
 *  or wxWindow::RefreshRect calls."
 *  WARNING: controls MUST NOT be created inside this function, else creation 
 *  leads to a redraw which in turn leads to a paint event (->endless loop) */
void wxGermanTranslationPanel::OnPaint(wxPaintEvent & event)
{
  LOGN_DEBUG("begin")
  /** http://www.informit.com/articles/article.aspx?p=405047 :
   *  "If you define a paint event handler, you must always create a wxPaintDC 
   * object, even if you don't use it." */
  wxPaintDC wxpaintdc(this);
  //http://www.informit.com/articles/article.aspx?p=405047
//  wxBufferedPaintDC dc(this);
  //  DrawTranslationFromAllParseTrees(wxpaintdc);
//  wxpaintdc.DrawBitmap(m_wxmemorydc.GetAsBitmap(), wxrectClient.width,
//    wxrectClient.height);
//  wxpaintdc.DrawBitmap(m_wxbitmapForMemoryDC, wxrectClient.width,
//    wxrectClient.height);
//  DrawTranslationAndCreateChoices(wxpaintdc);
  
  /** Shifts the device origin so we don't have to worry
  * about the current scroll position ourselves */
//  PrepareDC(dc);
  
  DrawMemoryDCbitmap(wxpaintdc);
//  DrawTranslationAndCreateChoices(dc);
  LOGN_DEBUG("end")
}

void wxGermanTranslationPanel::OnSize(wxSizeEvent & event)
{
  LOGN_DEBUG("begin")
  Create();
//  Refresh();
//  //wxWindowBase/ "wx/window.h":
//  // "repaint all invalid areas of the window immediately"
//  Update();
  LOGN_DEBUG("end")
}
