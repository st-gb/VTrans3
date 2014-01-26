/*
 * wxGermanTranslationPanel.hpp
 *
 *  Created on: 12.08.2011
 *      Author: Stefan
 */

#ifndef WXGERMANTRANSLATIONPANEL_HPP_
#define WXGERMANTRANSLATIONPANEL_HPP_

#include <wx/panel.h> //base class wxPanel
#include <wx/bitmap.h> //class wxBitmap
#include <wx/dcmemory.h> //class wxMemoryDC

//class GrammarPartPointerAndConcatenationID
#include <Attributes/GrammarPartPointerAndConcatenationID.hpp>
////class TranslationAndConcatenationID
//#include <Attributes/TranslationAndConcatenationID.hpp>
//class TranslationAndGrammarPart
#include <Attributes/TranslationAndGrammarPart.hpp>

#include <map> //class std::multimap
#include <set> //class std::set
#include <vector> //class std::vector

//forward decl.
class wxChoice;

//see http://wiki.wxwidgets.org/Painting_your_custom_control
class wxGermanTranslationPanel
  : public wxPanel
{
  wxRect m_wxrectClient;
  std::set<wxChoice *> m_std_set_p_wxchoiceDropSelectionHandling;
  //for: if a listbox entry is selected, also select other listbox entries.
  std::multimap<int,int>
    m_std_map_nWindowIDselectedChoice2nWindowIDalsoSelect;
  //A list box may be for multiple conc. IDs.
  std::multimap<wxChoice *,uint32_t>
    m_std_multimap_p_wxchoice2ui32ConcatenationID;
  //a conc. ID may be connect to 2 or more choices.
  std::multimap<uint32_t,wxChoice *>
    m_std_multimap_ui32ConcatenationID2p_wxchoice;
//  std::vector<std::vector<TranslationAndGrammarPart> >
//    m_std_vector_stdvecTranslationAndGrammarPart;
  //A vector of sentences that begin at the same token index
  // (sentences that begin at the same token index:
  // vector of sentences that each contains a vector of words).
  std::vector <std::vector <std::vector <TranslationAndGrammarPart> > >
    m_stdvec_stdvec_stdvecTranslationAndGrammarPart;

  std::vector<std::vector<GrammarPart *> > *
    m_p_std_vector_std_vector_p_grammarpartCoveringMostTokensAtTokenIndex;

  wxWindowID m_wxwindowidCurrent;
  wxBitmap m_wxbitmapForMemoryDC;
  wxMemoryDC m_wxmemorydc;
public:
  wxGermanTranslationPanel();
  wxGermanTranslationPanel(wxWindow * p_wxwindowParent)
    : wxPanel( p_wxwindowParent,
        wxID_ANY, //wxWindowID winid = wxID_ANY,
        wxDefaultPosition, //const wxPoint& pos = wxDefaultPosition,
        wxDefaultSize, //const wxSize& size = wxDefaultSize,
        //long style = wxTAB_TRAVERSAL | wxNO_BORDER
        //see http://docs.wxwidgets.org/stable/wx_wxwindow.html#wxwindow:
        wxTAB_TRAVERSAL | wxNO_BORDER | wxNO_FULL_REPAINT_ON_RESIZE
        )
    , m_wxwindowidCurrent(0)
  {
  }
  void AddChoice(
    wxWindowID wxwindowid,
    wxCoord & wxcoordX,
    wxCoord & wxcoordY,
    const wxArrayString & wxarraystringTranslation,
    wxCoord wxcoordYBottom,
    const bool bConnectedControl,
    uint32_t ui32ConcatenationID
    );
  void Create();
  void DrawParseTreesAtSameTokenIndex(
  //  * c_iterAllSentencesAtSameTokenIndex
    const std::vector<std::vector<TranslationAndGrammarPart> > &
       std_vector_stdvecTranslationAndGrammarPart,
     wxDC & r_wxdc,
     wxCoord & wxcoordX,
     wxCoord & wxcoordY
    );
  void DrawParseTreesFromLeaves(wxDC & r_wxdc);
  void DrawMemoryDCbitmap(wxPaintDC & );
  void DrawTranslationAndCreateChoices(wxDC & r_wxdc);
  void DrawTranslationFromAllParseTrees(wxDC & r_wxdc);
  void PossiblyAddChoice(
//    const wxArrayString & wxarraystring,
//    std::set<TranslationAndConcatenationID> &
//          std_set_translationandconcatenationid,
    std::set<GrammarPartPointerAndConcatenationID> &
      std_set_grammarpartandconcatenationid,
    wxDC & r_wxdc,
    wxCoord & wxcoordX,
    wxCoord & wxcoordY
    );
  void SelectConnectedListEntries(wxCommandEvent & event);
  void Set(
//    const std::vector<std::vector<TranslationAndGrammarPart> >
//      & c_r_std_vector_stdvecTranslationAndGrammarPart
    //A vector of sentences that begin at the same token index
    // (sentences that begin at the same token index:
    // vector of sentences that each contains a vector of words).
    std::vector <std::vector <std::vector <TranslationAndGrammarPart> > > &
      r_stdvec_stdvec_stdvecTranslationAndGrammarPart
    )
  {
//    m_std_vector_stdvecTranslationAndGrammarPart =
//      c_r_std_vector_stdvecTranslationAndGrammarPart;
    m_stdvec_stdvec_stdvecTranslationAndGrammarPart =
      r_stdvec_stdvec_stdvecTranslationAndGrammarPart;
  }
  void OnEraseBackground(wxEraseEvent & event);
  void OnPaint(wxPaintEvent & event) ;
  void OnSize(wxSizeEvent & event) ;
  DECLARE_EVENT_TABLE()
};

#endif /* WXGERMANTRANSLATIONPANEL_HPP_ */
