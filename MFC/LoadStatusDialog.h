#if !defined(AFX_LOADSTATUSDIALOG_H__05EB3660_2003_11D6_A7EC_0000B45A1FE5__INCLUDED_)
#define AFX_LOADSTATUSDIALOG_H__05EB3660_2003_11D6_A7EC_0000B45A1FE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoadStatusDialog.h : Header-Datei
//

#define WM_END_LOADSTATUSDIALOG WM_USER+2
/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CLoadStatusDialog 

class CLoadStatusDialog : public CDialog
{
// Konstruktion
public:
	CLoadStatusDialog(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CLoadStatusDialog)
	enum { IDD = IDD_FILE_OPEN_DIALOG };
		// HINWEIS: Der Klassen-Assistent f�gt hier Datenelemente ein
	//}}AFX_DATA


// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CLoadStatusDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CLoadStatusDialog)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	LONG OnEndLoadStatusDialog(UINT wParam, LONG lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_LOADSTATUSDIALOG_H__05EB3660_2003_11D6_A7EC_0000B45A1FE5__INCLUDED_
