/** wxGermanTranslationPanel.hpp
 *  Created on: 12.08.2011
 *      Author: Stefan */

#ifndef WXGERMANTRANSLATIONPANEL_HPP_
#define WXGERMANTRANSLATIONPANEL_HPP_

#include <wx/panel.h> //base class wxPanel
#include <wx/bitmap.h> //class wxBitmap
#include <wx/dcmemory.h> //class wxMemoryDC
#include <wx/dcclient.h> //class wxClientDC
//#include <wx/defs.h> //class wxMemoryDC

//class GrammarPartPointerAndConcatenationID
#include <Attributes/GrammarPartPointerAndConcatenationID.hpp>
////class TranslationAndConcatenationID
//#include <Attributes/TranslationAndConcatenationID.hpp>
/** class TranslationResult, typedef WordCompoundsAtSameTokenIndex */
#include <Attributes/TranslationResult.hpp>

#include <map> //class std::multimap
#include <set> //class std::set
#include <vector> //class std::vector

/** forward declarations */
class wxChoice;
//class TranslationResult;

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
  TranslationResult m_translationResult;

  std::vector<std::vector<GrammarPart *> > *
    m_p_std_vector_std_vector_p_grammarpartCoveringMostTokensAtTokenIndex;

  wxWindowID m_wxwindowidCurrent;
  wxBitmap m_wxbitmapForMemoryDC, * m_p_wxbitmapForMemoryDC;
  /** http://docs.wxwidgets.org/trunk/classwx_client_d_c.html :
   * A wxClientDC must be constructed if an application wishes to paint on the 
   * client area of a window from outside an EVT_PAINT() handler. */
  wxClientDC m_wxclientdc;
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
    , m_p_wxbitmapForMemoryDC(NULL)
    , m_wxclientdc(this)
  {
    /** see http://docs.wxwidgets.org/trunk/classwx_auto_buffered_paint_d_c.html: 
     * For wxAutoBufferedPaintDC. */
    //SetBackgroundStyle(wxBG_STYLE_PAINT);
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
    const WordCompoundsAtSameTokenIndex &,
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
  void Set( TranslationResult & translationResult )
  {
    m_translationResult = translationResult;
  }
  void OnEraseBackground(wxEraseEvent & event);
  void OnPaint(wxPaintEvent & event) ;
  void OnSize(wxSizeEvent & event) ;
  DECLARE_EVENT_TABLE()
};

#endif /* WXGERMANTRANSLATIONPANEL_HPP_ */
