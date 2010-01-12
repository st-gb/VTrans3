#include "../stdafx.h"
#include "VTrans.h"

IMPLEMENT_DYNCREATE(CSplitterFrame, CMDIChildWnd)

CSplitterFrame::CSplitterFrame()
{
}

CSplitterFrame::~CSplitterFrame()
{
}


BOOL CSplitterFrame::OnCreateClient(LPCREATESTRUCT,
	 CCreateContext* pContext)
{
	// create a splitter with 1 row, 2 columns
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(130, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CInputView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,1));

	return TRUE;
}

BEGIN_MESSAGE_MAP(CSplitterFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSplitterFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
