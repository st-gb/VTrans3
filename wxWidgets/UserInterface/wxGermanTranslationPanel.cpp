/*
 * wxGermanTranslationPanel.cpp
 *
 *  Created on: 12.08.2011
 *      Author: Stefan
 */
#include "wxGermanTranslationPanel.hpp"

#include <wx/choice.h>
#include <wx/dc.h> //class wxDC
#include <wx/dcclient.h> //for class wxPaintDC
#include <map> //class std::multimap
#include <set> //class std::set

#include <Attributes/EnglishWord.hpp> //enum EnglishWord::English_word_class
//class TranslationAndGrammarPart
#include <Attributes/TranslationAndGrammarPart.hpp>
#include <Parse/GrammarPart.hpp> //class GrammarPart
//getwxString(...)
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>

BEGIN_EVENT_TABLE(wxGermanTranslationPanel, wxPanel)
  EVT_PAINT(wxGermanTranslationPanel::OnPaint)
  EVT_SIZE( wxGermanTranslationPanel::OnSize)
END_EVENT_TABLE()

wxGermanTranslationPanel::wxGermanTranslationPanel()
{
  wxChoice * p_wxchoice = new wxChoice(
    this, //wxWindow *    parent,
    wxID_ANY, // wxWindowID    id,
    wxDefaultPosition, //const wxPoint &   pos = wxDefaultPosition,
    wxDefaultSize //const wxSize & size = wxDefaultSize,
//    int   n = 0,
//    const wxString    choices[] = NULL,
//    long    style = 0,
//    const wxValidator &   validator = wxDefaultValidator,
//    const wxString &    name = wxChoiceNameStr
    );
//  p_wxchoice->Get
//  Add()
}

void wxGermanTranslationPanel::Create()
{
  m_wxrectClient = GetClientRect();
  m_wxbitmapForMemoryDC.Create(m_wxrectClient.width, m_wxrectClient.height);
  m_wxmemorydc.SelectObject( m_wxbitmapForMemoryDC);

  m_wxmemorydc.FloodFill(0,0, * wxWHITE,
    //* wxBLACK,
    wxFLOOD_BORDER //: the area to be flooded is bounded by the given colour.
    );
//  m_wxbitmapForMemoryDC.S
  DrawTranslationAndCreateChoices(m_wxmemorydc);
}

void GetTokenIndex2TranslationAndConcatenationID(
  const std::vector<std::vector<TranslationAndGrammarPart> > &
    r_std_vector_stdvecTranslationAndGrammarPart,
  std::multimap<unsigned, TranslationAndConcatenationID> &
    std_map_uiTokenIndex2translationandconcatenationid
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
      std_map_uiTokenIndex2translationandconcatenationid.insert(
        std::make_pair(
          uiTokenIndex,
          TranslationAndConcatenationID(
            c_iterInner->m_stdstrTranslation,
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
  unsigned uiTokenIndex = 0;
//  std::vector<std::vector<TranslationAndGrammarPart> >
//     std_vector_stdvecTranslationAndGrammarPart;
  std::multimap<unsigned, TranslationAndConcatenationID>
    std_map_uiTokenIndex2translationandconcatenationid;

  GetTokenIndex2TranslationAndConcatenationID(
//    * c_iterAllSentencesAtSameTokenIndex,
    std_vector_stdvecTranslationAndGrammarPart,
    std_map_uiTokenIndex2translationandconcatenationid);

//  std::multimap<unsigned, const char *>::const_iterator c_iterToken =
//      std_map_ui2std_str.begin();

  std::multimap<unsigned, TranslationAndConcatenationID>::const_iterator
    c_iter_std_map_uiTokenIndex2translationandconcatenationid =
    std_map_uiTokenIndex2translationandconcatenationid.begin();

  uiTokenIndex =
    //Init with value <> 0 for the 1st comparison ("... == uiTokenIndex" )
    //to be false.
    0;
  //  std::set<std::string> std_set_std_strToken;
  std::set<TranslationAndConcatenationID>
    std_set_translationandconcatenationid;
  std::vector<wxString> std_vector_wxstr;
  const char * p_chTranslation = NULL;
  uint32_t ui32ConcatenationID = 0;
  wxArrayString wxarraystringTranslation;
  const TranslationAndConcatenationID * p_translationandconcatenationid = NULL;
  while( //c_iterToken != //std_map_ui2std_str.end()
      c_iter_std_map_uiTokenIndex2translationandconcatenationid !=
      std_map_uiTokenIndex2translationandconcatenationid.end()
      )
  {
    if( //c_iterToken->first == uiTokenIndex
        c_iter_std_map_uiTokenIndex2translationandconcatenationid->first ==
            uiTokenIndex
        )
    {
  //      std_vector_wxstr.push_back( wxString(c_iterInner->m_stdstrTranslation.
  //        c_str() ) );
  //      ui32ConcatenationID =
  //        c_iter_std_map_uiTokenIndex2translationandconcatenationid->second.
  //        m_ui32ConcatenationID;
      p_translationandconcatenationid = &
        c_iter_std_map_uiTokenIndex2translationandconcatenationid->second;
  //      if( ui32ConcatenationID == 0 )
        //if a translated word has no concatenation and another such translated
        //word exists, there should not be double translations:
        //English: "the fan works"
        // "der(concatenation ID=2) Anhänger(concatenation ID=2) arbeitet(concatenation ID=0)" <- translation for 1st parse tree.
        // "das Gebläse arbeitet(concatenation ID=0)" <- translation for 2nd parse tree.
        // -> in std::set only 1 "arbeitet" for token index 2
        std_set_translationandconcatenationid.insert(
          * p_translationandconcatenationid );
  //      if( wxarraystringTranslation.insert() )
  //      wxarraystringTranslation.Add( wxString(c_iterToken->second) );
  //      std_set_std_strToken.insert( std::string(c_iterToken->second) )
    }
    else
    {
      uiTokenIndex = //c_iterToken->first;
        c_iter_std_map_uiTokenIndex2translationandconcatenationid->first;
  //      if( wxarraystringTranslation.size() > 0
  //          //std_set_std_strToken.size() > 0
  //        )
      PossiblyAddChoice(//wxarraystringTranslation
        std_set_translationandconcatenationid, r_wxdc, wxcoordX, wxcoordY);
  //      std_vector_wxstr.clear();
      wxarraystringTranslation.Clear();
      std_set_translationandconcatenationid.clear();
  //      wxarraystringTranslation.Add( wxString(c_iterToken->second) );
      std_set_translationandconcatenationid.insert(
        c_iter_std_map_uiTokenIndex2translationandconcatenationid->second );
  //      wxcoordX += 50;
    }
  //    ++ c_iterToken;
    ++ c_iter_std_map_uiTokenIndex2translationandconcatenationid;
  }
  //  if( wxarraystringTranslation.size() > 0 )
  PossiblyAddChoice(//wxarraystringTranslation
    std_set_translationandconcatenationid
    , r_wxdc, wxcoordX, wxcoordY);
}

void wxGermanTranslationPanel::DrawTranslationAndCreateChoices(wxDC & r_wxdc)
{
  //Delete existing choice windows
  bool bOk = DestroyChildren();
  m_wxwindowidCurrent = 0;

  wxSize wxsizeText;
  wxString wxstrTranslation;
  int x = 0;
  wxCoord wxcoordX = 0;
  wxCoord wxcoordY = 0;

  std::multimap<unsigned, const char *> std_map_ui2std_str;

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
  //important when the choice controls are created after a resize?!
  Layout();
}

void wxGermanTranslationPanel::DrawTranslationFromAllParseTrees(wxDC & r_wxdc)
{
  wxSize wxsizeText;
  wxString wxstrTranslation;
  int x = 0;
  wxCoord wxcoordY = 0;

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

void wxGermanTranslationPanel::PossiblyAddChoice(
//    const wxArrayString & c_r_wxarraystringTranslation,
    std::set<TranslationAndConcatenationID> &
      std_set_translationandconcatenationid,
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
  wxWindowID wxwindowid = wxID_ANY;
  std::set<TranslationAndConcatenationID>::const_iterator
    c_iter_std_set_translationandconcatenationid =
    std_set_translationandconcatenationid.begin();
  int nSize = std_set_translationandconcatenationid.size();

  while( c_iter_std_set_translationandconcatenationid !=
    std_set_translationandconcatenationid.end()
    )
  {
    ui32ConcatenationID = c_iter_std_set_translationandconcatenationid->
      m_ui32ConcatenationID;
    if( ui32ConcatenationID != 65535 )
    {
      wxwindowid = ++ m_wxwindowidCurrent;
      bConnectedControl = true;
    }
    p_chTranslation = c_iter_std_set_translationandconcatenationid->
      m_std_strTranslation.c_str();
    wxarraystringTranslation.Add( wxString( p_chTranslation)
      );
    ++ c_iter_std_set_translationandconcatenationid;
  }
  if( wxarraystringTranslation.size() > 0
      //std_set_std_strToken.size() > 0
    )
  {
    if( //c_r_wxarraystringTranslation.size() > 1
      wxarraystringTranslation.size() > 1
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
//      p_wxchoice->
      wxcoordX += p_wxchoice->GetSize().GetWidth();
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
    else //if( )
    {
      wxsizeText = r_wxdc.GetTextExtent( //c_r_wxarraystringTranslation[0]
        wxarraystringTranslation[0]) ;
      if( wxcoordX + wxsizeText.x > m_wxrectClient.width )
      {
        wxcoordY += wxsizeText.y;
        wxcoordX = 0;
      }
      r_wxdc.DrawText( //c_r_wxarraystringTranslation[0]
        wxarraystringTranslation[0], wxcoordX
        , //0
        wxcoordY
        );
      wxcoordX += wxsizeText.x + r_wxdc.GetTextExtent( wxT(" ") ).x;
    }
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
    int n_m_std_multimap_ui32ConcatenationID2p_wxchoice_size =
        m_std_multimap_ui32ConcatenationID2p_wxchoice.size();
#endif //#ifdef _DEBUG

    while( c_iter_std_multimap_p_wxchoice2ui32ConcatenationID !=
        m_std_multimap_p_wxchoice2ui32ConcatenationID.end() &&
        c_iter_std_multimap_p_wxchoice2ui32ConcatenationID->first ==
        p_wxobjectSelectionChanged
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

void wxGermanTranslationPanel::OnPaint(wxPaintEvent & event)
{
  wxPaintDC wxpaintdc(this);

  wxRect wxrectClient = GetClientRect();
//  DrawTranslationFromAllParseTrees(wxpaintdc);
//  wxpaintdc.DrawBitmap(m_wxmemorydc.GetAsBitmap(), wxrectClient.width,
//      wxrectClient.height);
  //http://docs.wxwidgets.org/stable/wx_wxmemorydc.html:
  wxpaintdc.Blit(
      0, //wxCoord xdest,
      0, //wxCoord ydest,
      wxrectClient.width, //wxCoord width,
      wxrectClient.height, //wxCoord height
      & m_wxmemorydc, //wxDC *source,
      0, //wxCoord xsrc,
      0 //wxCoord ysrc
      );
}

void wxGermanTranslationPanel::OnSize(wxSizeEvent & event)
{
  Create();
}
