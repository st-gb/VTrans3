// StatusDialog.cpp: Implementierungsdatei
//

#include "../stdafx.h"
#include "VTrans.h"
#include "StatusDialog.h"
// eigener Code, Anfang
#include "ChildFrm.h"
#include <math.h>
#include "../Translate.h"
// eigener Code, Ende

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern volatile DWORD g_dwStatus;
extern volatile DWORD g_dwSentenceStatus;
extern volatile DWORD g_dwTotal;
extern volatile DWORD g_dwTranslated;
extern volatile DWORD g_dwTokensInSentence;
extern CTime tAtTheBeginningOfTheTranslation;

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld StatusDialog 


StatusDialog::StatusDialog(CWnd* pParent /*=NULL*/)
	: CDialog(StatusDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(StatusDialog)
	m_strAmount = _T("");
	//}}AFX_DATA_INIT
}


void StatusDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StatusDialog)
	DDX_Text(pDX, IDC_AMOUNT_STATIC, m_strAmount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(StatusDialog, CDialog)
	//{{AFX_MSG_MAP(StatusDialog)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten StatusDialog 

void StatusDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen und/oder Standard aufrufen
	//CString tmp(itoa((int)count,buffer,10));
	//MessageBeep((WORD)-1);
	char buffer[20];
	CTime tCurrent=CTime::GetCurrentTime();
	CTimeSpan ts(tCurrent.GetTime());
	ts-=CTimeSpan(tAtTheBeginningOfTheTranslation.GetTime());
	if((double)g_dwTokensInSentence>0)
	{
		if((double)((double)((double)g_dwTranslated+(double)g_dwStatus)/
			(double)g_dwTokensInSentence)>0.0)
		{
			//MessageBeep((WORD)-1);
			//_ultoa((double)ts.GetTotalSeconds()/(double)((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal)-ts.GetTotalSeconds(),buffer,10);
			//_ultoa(,buffer,10);
			//_gcvt((double)ts.GetTotalSeconds()/(double)1.0-((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal)-ts.GetTotalSeconds(),3,buffer);
			//_gcvt((double)((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal),7,buffer);
			CProgressCtrl *pPC=(CProgressCtrl*) GetDlgItem(IDC_PARSING_PROGRESS);
			pPC->SetPos((double)((double)g_dwStatus/(double)g_dwTotal)*100.0);
			//_gcvt((double)(g_dwStatus),7,buffer);
			//_ultoa(g_dwStatus,buffer,10);
			//printf("ts.GetTotalSeconds(): %u\n",ts.GetTotalSeconds());
			//printf("
			//printf("((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal): %u\n",((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal));
			m_strAmount=CString(buffer);
			UpdateData(FALSE);
		}
		CProgressCtrl *pPC=(CProgressCtrl*) GetDlgItem(IDC_SENTENCE_PROGRESS);
		pPC->SetPos((double)g_dwSentenceStatus/(double)g_dwTokensInSentence*100.0);
	}
#ifdef _DEBUG
	printf("g_dwTotal: %u\n",g_dwTotal);
#endif
	//MessageBeep((WORD)-1);
	if(g_dwTotal>0)
	{
		//MessageBeep((WORD)-1);
		if((double)((double)((double)g_dwTranslated+(double)g_dwStatus)/
			(double)g_dwTotal)>0.0)
		{
			//_gcvt((double)ts.GetTotalSeconds()/(double)((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal)-(double)ts.GetTotalSeconds(),0,buffer);
			CProgressCtrl * pPC=(CProgressCtrl*) GetDlgItem(IDC_TEXT_PROGRESS);
			pPC->SetPos((double)((double)(g_dwTranslated+g_dwSentenceStatus)/
				(double)g_dwTotal)*100.0);
			double dTotalSeconds=(double)ts.GetTotalSeconds();
			double dRatio=(double)((double)(g_dwTranslated+g_dwStatus)/(double)g_dwTotal);
			//_ultoa(ceil((double)(ts.GetTotalSeconds()/(double)((g_dwTranslated+g_dwStatus)/g_dwTotal)))-ts.GetTotalSeconds(),buffer,10);
			_ultoa(ceil(dTotalSeconds/dRatio)-dTotalSeconds,buffer,10);
			m_strAmount=CString(buffer);
			UpdateData(FALSE);
		}
	}
	//ultoa(g_dwTotal,buffer,10);
	//m_strAmount=CString(buffer);
	//UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}
