#if !defined(AFX_STATUSDIALOG_H__B3ABD6C1_B2AC_11D5_A7E3_0000B45A1FE5__INCLUDED_)
#define AFX_STATUSDIALOG_H__B3ABD6C1_B2AC_11D5_A7E3_0000B45A1FE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusDialog.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld StatusDialog 

class StatusDialog : public CDialog
{
// Konstruktion
public:
	StatusDialog(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(StatusDialog)
	enum { IDD = IDD_STATUS_DIALOG };
	CString	m_strAmount;
	//}}AFX_DATA


// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(StatusDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(StatusDialog)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_STATUSDIALOG_H__B3ABD6C1_B2AC_11D5_A7E3_0000B45A1FE5__INCLUDED_
