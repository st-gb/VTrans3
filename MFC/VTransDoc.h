// VTransDoc.h : Schnittstelle der Klasse CVTransDoc
//
/////////////////////////////////////////////////////////////////////////////

#if  !defined(AFX_VTRANSDOC_H__FB6F08EA_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
#define AFX_VTRANSDOC_H__FB6F08EA_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_

#include "../Text.h"	// Hinzugefügt von der Klassenansicht
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CVTransDoc : public CDocument
{
protected: // Nur aus Serialisierung erzeugen
	CVTransDoc();
	//DECLARE_DYNCREATE(CVTransDoc)
	DECLARE_SERIAL(CVTransDoc)

// Attribute
public:

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CVTransDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL
	virtual BOOL OnNewDocument();

// Implementierung
public:
	DWORD m_dwEnglishLines;
	Text m_EnglishText, m_GermanText;
	BOOL bEnglishText;
	virtual ~CVTransDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CVTransDoc)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_VTRANSDOC_H__FB6F08EA_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
