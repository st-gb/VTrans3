// LoadStatusDialog.cpp: Implementierungsdatei
//

#include "../stdafx.h"
#include "vtrans.h"
#include "LoadStatusDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern volatile DWORD g_dwFileLength;
extern volatile DWORD g_dwRun;

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CLoadStatusDialog 


CLoadStatusDialog::CLoadStatusDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadStatusDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoadStatusDialog)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
}


void CLoadStatusDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoadStatusDialog)
		// HINWEIS: Der Klassen-Assistent fügt hier DDX- und DDV-Aufrufe ein
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoadStatusDialog, CDialog)
	//{{AFX_MSG_MAP(CLoadStatusDialog)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// Globale Hilfebefehle
	ON_MESSAGE(WM_END_LOADSTATUSDIALOG,OnEndLoadStatusDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CLoadStatusDialog 

void CLoadStatusDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen und/oder Standard aufrufen

	CString str;
#ifdef _DEBUG
	printf("void CLoadStatusDialog::OnTimer(UINT nIDEvent) ANFANG\n");
#endif
	str.Format("%u\n",g_dwRun);
	SetDlgItemText(IDC_BYTES_INSERTED_STATIC,str);
	str.Format("%u\n",g_dwFileLength);
	SetDlgItemText(IDC_BYTES_TOTAL_STATIC,str);
	((CProgressCtrl*)GetDlgItem(IDC_LOAD_PROGRESS))->SetPos((double)g_dwRun/
		(double)g_dwFileLength*100.0);
	CDialog::OnTimer(nIDEvent);
#ifdef _DEBUG
	printf("void CLoadStatusDialog::OnTimer(UINT nIDEvent) ENDE\n");
#endif
}

LONG CLoadStatusDialog::OnEndLoadStatusDialog(UINT wParam, LONG lParam)
{
#ifdef _DEBUG
	printf("LONG CLoadStatusDialog::OnEndLoadStatusDialog(UINT wParam, LONG\
 lParam) ANFANG\n");
#endif
  EndDialog(1);
	return 0;
}
