#if !defined(ENGLISHVIEW_H__FB6F08E6_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
     #define ENGLISHVIEW_H__FB6F08E6_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_


#include <vector> // für m_Text;
//#include "Translate.h" // CStringVector
#include "VTransDoc.h"
#include "../rest.h" //für typedef	std::vector<CString> CStringVector;
//#include "MainFrm.h"
//#ifndef TEXT_DEFINED
	#include "../Text.h"
#include "VTrans.h"
//#endif
/*class Option//: public String
{
	std::vector<CString> m_vec;
};*/
/*class TextNode
{
public:
	BYTE ch;
	TextNode * m_next;
};

class Text
{
public:
	//CString str;
	TextNode * m_first;
	Clear();
	Delete(DWORD);
	DWORD GetLength();
	BOOL GetCString(CString &,DWORD,DWORD);
	Insert(DWORD,unsigned char);
	SetColor(DWORD dwIndex,BYTE color);
	Text()
	{
		m_first=NULL;
	};
};*/
class ComboBoxListElement
{
public:
	CComboBox ComboBox;
	ComboBoxListElement * m_pNext;
	//ComboBoxListElement(){pComboBox=NULL};
};

class ComboBoxList
{
public:
	ComboBoxListElement * m_pFirst;
	ComboBoxListElement * m_pLast;
	ComboBoxList(){m_pFirst=NULL; m_pLast=NULL;};
	void Clear();
};

class ColorText
{
public:
	BYTE color;
	CString str;
};

//class IntPair
//{
//public:
//	int m_n1;
//	int m_n2;
//};

class EnglishView : public CView
{
	//int Func();
//protected: // Nur aus Serialisierung erzeugen
public:
	EnglishView();
	DECLARE_DYNCREATE(EnglishView)

// Attribute
public:
	CVTransDoc* GetDocument()
	{
		//ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVTransDoc)));
		return (CVTransDoc*) m_pDocument;
	}
	std::vector<ColorText> m_ColorText;
	std::vector<CStringVector> m_cstrvv;
	Text * m_pText;
	DWORD m_CursorPos;
	BOOL m_bDrawVector;
	CDC m_pDC;

// Operationen
public:

	//EnglishView();
// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(EnglishView)
	//public:
	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateFrame( UINT nState, CFrameWnd* pFrameWnd );
	virtual void OnActivateView( BOOL bActivate, CView* pActivateView, CView* pDeactiveView );
	//}}AFX_VIRTUAL
	virtual void OnDraw(CDC* pDC);  // überladen zum Zeichnen dieser Ansicht

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(EnglishView)
	afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC); // Funktion zum Überschreiben deklarieren,
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus( CWnd* pNewWnd );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized );
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Implementierung
public:

	BOOL m_bDCAttached;
	BOOL bEnglishPart;
	BOOL m_bFirstTimeDisplayed;
	BOOL m_bMark;
	BOOL m_bDrawCursor;
	BOOL m_bDynamicTextFlow;
	BYTE m_bInitiallyDisplayed;
	DWORD m_dwStartLine;
	DWORD m_dwHighestID;
	DWORD m_dwNumberOfLines;
	DWORD m_dwMarkingEnd;
	DWORD m_dwMarkingStart;
	void CalculateNumberOfLines();
	ComboBoxList m_ComboBoxList;
	void CreateComboBoxes();
	void MoveComboBoxes();
	std::vector<CComboBox> m_vecComboBoxes;
  //Stores the Zusammenhang between 2 comboboxes: if in a combobox "Sie|du|ihr" "du" 
  //is selected, in the 2nd combobox "mögen|magst|mögt" "magst" should be selected.
	std::vector<IntPair> m_vecIntPair;
	CEdit edit;
	CScrollBar m_wndVert;
	virtual void OnInitialUpdate();
	virtual int OnToolHitTest(CPoint point,TOOLINFO* pTI);
	virtual ~EnglishView();
	BOOL OnToolTipNotify( UINT id, NMHDR * pTTTStruct, LRESULT * pResult );
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
	BYTE m_bHasFocus;
	CPoint m_point;
	BOOL m_bCalculateCursorPos;
};

#endif
