//bei einem Laufzeitfehler dieses Computerprogramms durch Aufruf einer
//Funktion kann es sein, dass auf nicht-existente Steuerelemente 
//zugegriffen wird

#include "../stdafx.h"
#include "WordEditorBar.h"

// eigener Code, Anfang
#include "../Resource.h" //Use "./" because else e.g. cygwin's Resource.h is included.
#include <typeinfo.h>
#include "../Translate.h" //für CStringVector GetCStringVector(CString str)
#include "../rest.h" //für CStringVector
#include <VocabularyInMainMem/DoublyLinkedList/WordNode.hpp>
#include "VocabularyInMainMem/DoublyLinkedList/WordList.hpp"
// eigener Code, Ende

extern WordList wordList;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyBar

CVocableEditorBar::CVocableEditorBar()
{
	m_bSetFocusOnEnglishWordKind=1;
}

CVocableEditorBar::~CVocableEditorBar()
{
}

BEGIN_MESSAGE_MAP(CVocableEditorBar, baseCMyBar)
    //{{AFX_MSG_MAP(CMyBar)
    ON_WM_CREATE()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
		//'Hilfsverb'
		ON_BN_CLICKED(ID_USER+2, OnIDUser2)
		//1. englisches regelmäßig-Kontrollkästchen
		ON_BN_CLICKED(ID_USER+4, OnIDUser4)
		//2. englisches regelmäßig-Kontrollkästchen
		ON_BN_CLICKED(ID_USER+5, OnIDUser5)
		//Kontrollkästchen 'im Deutschen: Plural' im englischen Teil des 
		//Vokabeleditors
		ON_BN_CLICKED(ID_USER+7, OnIDUser8)
		//Kontrollkästchen 'im Deutschen: Singular' im englischen Teil des 
		//Vokabeleditors
		ON_BN_CLICKED(ID_USER+8, OnIDUser9)
		ON_BN_CLICKED(ID_USER+22, OnIDUser22) // Plural im Deutschen: "~"
		ON_BN_CLICKED(ID_USER+23, OnIDUser23) // Plural im Deutschen: "~e"
		// 1. deutsches regelmäßig-Kontrollkästchen Präsens
		ON_BN_CLICKED(ID_USER+24, OnIDUser24)
		//deutsches Kontrollkästchen "Wortstamm" des Präteritums
		ON_BN_CLICKED(ID_USER+25, OnIDUser17)
		ON_BN_CLICKED(ID_USER+26, OnIDUser18)
		ON_BN_CLICKED(ID_USER+27, OnIDUser25) // Kontrollkästchen kein Fall
		ON_BN_CLICKED(ID_USER+28, OnIDUser26) // Kontrollkästchen 3. Fall
		ON_BN_CLICKED(ID_USER+29, OnIDUser27) // Kontrollkästchen 4. Fall
		ON_BN_CLICKED(ID_USER+30, OnIDUser30) // Kontrollkästchen "ge~-2t"
		ON_BN_CLICKED(ID_USER+72, OnIDUser72) // Kontrollkästchen "more/most"
		ON_CBN_SELCHANGE(IDC_ENGLISH_WORDKIND_COMBO, OnSelChangeEnglishWordkind)
		//Ändern des englischen Singulars/Infinitivs
		ON_EN_CHANGE(ID_USER+12,OnChangeIDUser10)
		//Ändern des deutschen Singulars/Infinitivs
		ON_EN_CHANGE(ID_USER+34,OnChangeIDUser20)
		//Ändern des deutschen Wortstamms des Präsens
		ON_EN_CHANGE(ID_USER+40,OnChangeIDUser40)
		//Ändern des deutschen Wortstamms des Präteritums
		ON_EN_CHANGE(ID_USER+49,OnChangeIDUser49)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyBar message handlers

int CVocableEditorBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
#ifdef _DEBUG
	printf("int CVocableEditorBar::OnCreate(LPCREATESTRUCT lpCreateStruct) ANFANG\n");
	fflush(stdout);
#endif
	if (baseCMyBar::OnCreate(lpCreateStruct) == -1)
    return -1;

    /*if (!m_wndChild.Create(_T("Paste"),
        WS_CHILD|WS_VISIBLE,
        CRect(0,0,0,0), this, ID_EDIT_PASTE))
        return -1;*/

		/*if (!m_english.Create(_T("Englisch"),WS_CHILD|WS_VISIBLE,
        CRect(10,20,10,20), this,ID_USER+1))
				return -1;*/
	if(!m_englishButton[0].Create(_T("wenn speziell"),WS_CHILD|WS_VISIBLE|
		BS_AUTORADIOBUTTON,CRect(0,0,0,0), this,ID_USER+1))
		return -1;
	if(!m_englishButton[1].Create(_T("immer"),WS_CHILD|WS_VISIBLE|
		BS_AUTORADIOBUTTON,CRect(0,0,0,0), this,ID_USER+1))
		return -1;
	if(!m_englishButton[2].Create(_T("nie"),WS_CHILD|WS_VISIBLE|
		BS_AUTORADIOBUTTON,CRect(0,0,0,0), this,ID_USER+1))
		return -1;
	if(!m_englishButton[3].Create(_T("Hilfsverb"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+2))
		return -1;
	if(!m_englishButton[4].Create(_T("Aktionsverb"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+3))
		return -1;
	if(!m_englishButton[5].Create(_T("regelmäßig"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+4))
		return -1;
	if(!m_englishButton[6].Create(_T("regelmäßig"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+5))
		return -1;
	if(!m_englishButton[7].Create(_T("Adverb befindet sich zwischen Subjekt \
und Verb oder nach der finiten Form des Verbes 'to be'"),WS_CHILD|
		WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,CRect(0,0,0,0), this,ID_USER+6))
		return -1;
	if(!m_englishButton[8].Create(_T("Adverb bezieht sich auf ein Adjektiv \
oder Adverb"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,CRect(0,0,0,0),this,
		ID_USER+6))
		return -1;
	if(!m_englishButton[9].Create(_T("Adverb bezieht sich auf ganzen Satz \
und steht am Satzbeginn"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,
		CRect(0,0,0,0), this,ID_USER+6))
		return -1;
	if(!m_englishButton[14].Create(_T("Adverb bezieht sich auf ganzen Satz \
und steht am Satzende"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,
		CRect(0,0,0,0), this,ID_USER+6))
		return -1;
	if(!m_englishButton[15].Create(_T("Adverb ist ein Fragewort"),WS_CHILD|
		WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,CRect(0,0,0,0),this,ID_USER+6))
		return -1;
	if(!m_englishButton[16].Create(_T("Adverb der Zeit"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTORADIOBUTTON,CRect(0,0,0,0),this,ID_USER+6))
		return -1;
	if(!m_englishButton[10].Create(_T("im Deutschen: Plural"),WS_CHILD|
		WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,CRect(0,0,0,0),this,ID_USER+7))
		return -1;
	if(!m_englishButton[11].Create(_T("im Deutschen: Singular"),WS_CHILD|
		WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,CRect(0,0,0,0),this,ID_USER+8))
		return -1;
	if(!m_englishButton[12].Create(_T("Adjektiv darf nur nach einem Verb \
benutzt werden"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,CRect(0,0,0,0),this,
		ID_USER+9))
		return -1;
	if(!m_englishButton[13].Create(_T("Adjektiv darf nur vor einem Substantiv \
benutzt werden"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,CRect(0,0,0,0),this,
		ID_USER+10))
		return -1;
  if(!m_englishButton[17].Create(_T("more/most"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0),this,ID_USER+74))
		return -1;
	if(!m_englishButton[18].Create(_T("Adverb statt Adjektiv"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0),this,ID_USER+75))
		return -1;
	if(!m_englishComboBox.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST,
		CRect(0,0,0,0), this,ID_USER+11))
		return -1;
	if(!m_englishEdit[0].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+12)) // Singular/Positiv
		return -1;
	if (!m_englishEdit[1].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+13)) // Plural/Komperativ
		return -1;
	if (!m_englishEdit[2].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+14)) // Partizip Perfekt/Superlativ
		return -1;
	if (!m_englishEdit[3].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+15)) // abgeleitetes Adverb
		return -1;
	if(!m_englishStatic[0].Create(_T("Englisch"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+16))
		return -1;
	if(!m_englishStatic[1].Create(_T("Wortart"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+17))
		return -1;
	if(!m_englishStatic[2].Create(_T("Artikel"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+18))
		return -1;
	if(!m_englishStatic[3].Create(_T("Singular"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+19))
		return -1;
	if(!m_englishStatic[4].Create(_T("Plural"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+20))
		return -1;
	if(!m_englishStatic[5].Create(_T("Adverb"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+21))
		return -1;
	if(!m_englishWordKind.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST,
		CRect(0,0,0,0), this,IDC_ENGLISH_WORDKIND_COMBO))
		return -1;
		// DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
	TRACE("jfhdgjfdjgfj\n");
	if(!m_germanButton[0].Create(_T("reflexiv"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+22))
		return -1;
	if(!m_germanButton[1].Create(_T("Bewegung"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+23))
		return -1;
	if(!m_germanButton[2].Create(_T("regelmäßig"),WS_CHILD|WS_TABSTOP|
		WS_VISIBLE|BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+24))
		return -1;
	if(!m_germanButton[3].Create(_T("regelmäßig"),WS_CHILD|WS_TABSTOP|
		WS_VISIBLE|BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+25))
		return -1;
	if(!m_germanButton[4].Create(_T("regelmäßig"),WS_CHILD|WS_TABSTOP|
		WS_VISIBLE|BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+26))
		return -1;
	if(!m_germanButton[5].Create(_T("kein Fall"),WS_CHILD|WS_TABSTOP|
		WS_VISIBLE|BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+27))
		return -1;
	if(!m_germanButton[6].Create(_T("3. Fall"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+28))
		return -1;
	if(!m_germanButton[7].Create(_T("4. Fall"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+29))
		return -1;
	//Plural bei Ableitung vom Singular Beispiel: "Tier" ("~") -> "Tiere" ("~e")
	if(!m_germanButton[8].Create(_T("~e"),WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
		CRect(0,0,0,0), this,ID_USER+30))
		return -1;
	//Plural bei Ableitung vom Singular Beispiel: "Frau" ("~") -> "Frauen" ("~en")
	//"Tasse" ("~") -> "Tassen" ("~n") //wenn der letzte Buchstabe ein "e" ist,
	//wird lediglich ein "n" angehängt, sonst "en"
	if(!m_germanButton[9].Create(_T("~en/~n"),WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
		CRect(0,0,0,0), this,ID_USER+31))
		return -1;
	//Plural bei Ableitung vom Singular Beispiel: "Land" ("~") -> "Länder" ("~er")
	if(!m_germanButton[10].Create(_T("~er"),WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
		CRect(0,0,0,0), this,ID_USER+32))
		return -1;
	if(!m_germanButton[11].Create(_T("Subjekt Prädikat [Objekt]"),WS_CHILD|
		WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,CRect(0,0,0,0),this,ID_USER+72))
		return -1;
	if(!m_germanButton[12].Create(_T("Subjekt [Objekt] Prädikat"),WS_CHILD|
		WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,CRect(0,0,0,0),this,ID_USER+73))
		return -1;
	//Plural bei Ableitung vom Singular Beispiel: "Teller" ("~") -> "Teller" ("~")
	if(!m_germanButton[13].Create(_T("~"),WS_CHILD|
		WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,CRect(0,0,0,0),this,ID_USER+77))
		return -1;
/*	if(!m_germanButton[8].Create(_T("Adverb der Häufigkeit"),WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
        CRect(10,20,10,20), this,ID_USER+19))
				return -1;
	if(!m_germanButton[9].Create(_T("Adverb bezieht sich auf ein Adjektiv oder Adverb"),WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
        CRect(10,20,10,20), this,ID_USER+19))
				return -1;
	if(!m_germanButton[10].Create(_T("Adverb bezieht sich auf ganzen Satz"),WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
        CRect(10,20,10,20), this,ID_USER+19))
				return -1;*/
	if(!m_germanComboBox.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST,
		CRect(0,0,0,0), this,ID_USER+33))
		return -1;
	if (!m_germanEdit[0].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+34)) // Singular/Infinitiv/Positiv/Wort/Pronomen
		return -1;
	if (!m_germanEdit[1].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+35)) // Plural/ich
		return -1;
	if (!m_germanEdit[2].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+36)) // Komperativ/du
		return -1;
	if (!m_germanEdit[3].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+37)) // er/sie/es
		return -1;
	if (!m_germanEdit[4].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+38)) // wir
		return -1;
	if (!m_germanEdit[5].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+39)) // ihr
		return -1;
	if (!m_germanEdit[6].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+40)) // Wortstamm Präsens
		return -1;
	if (!m_germanEdit[7].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+41)) // Präteritum/Wortstamm/ich
		return -1;
	if (!m_germanEdit[8].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+42)) // du
		return -1;
	if (!m_germanEdit[9].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+43)) // er/sie/es
		return -1;
	if (!m_germanEdit[10].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+44)) // wir
		return -1;
	if (!m_germanEdit[11].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+45)) // ihr
		return -1;
	if (!m_germanEdit[12].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+46)) // sie
		return -1;
	if (!m_germanEdit[13].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+47)) // Perfekt
		return -1;
	if (!m_germanEdit[14].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+48)) // Konjunktiv
		return -1;
	if (!m_germanEdit[15].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+49)) // Wortstamm Präteritum
		return -1;
	if (!m_germanEdit[16].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+50)) // Wortstamm Präsens
		return -1;
	if (!m_germanEdit[17].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+51)) // Wortstamm
		return -1;
	if (!m_germanEdit[18].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+52)) // Wortstamm
		return -1;
	if (!m_germanEdit[19].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+53)) // Wortstamm
		return -1;
	if (!m_germanListBox[0].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+54))
		return -1;
	if (!m_germanListBox[1].Create(WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+55))
		return -1;
	if (!m_germanStatic[0].Create(_T("Deutsch"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+56))
		return -1;
	if (!m_germanStatic[1].Create(_T("Wortart"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+57))
		return -1;
	if (!m_germanStatic[2].Create(_T("Artikel"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+58)) // Artikel/Infinitiv
		return -1;
	if (!m_germanStatic[3].Create(_T("Singular"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+59)) // Singular/Präsens
		return -1;
	if (!m_germanStatic[4].Create(_T("Präposition"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+60)) // Plural/Präteritum
		return -1;
	if (!m_germanStatic[5].Create(_T("ich"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+61)) // Perfekt/Wortstamm
		return -1;
	if (!m_germanStatic[6].Create(_T("du"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+62))
		return -1;
	if (!m_germanStatic[7].Create(_T("er"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+63))
		return -1;
	if (!m_germanStatic[8].Create(_T("wir"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+64))
		return -1;
	if (!m_germanStatic[9].Create(_T("ihr"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+65))
		return -1;
	if (!m_germanStatic[10].Create(_T("sie"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+66))
		return -1;
	if (!m_germanStatic[11].Create(_T("Plural"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+67))
		return -1;
	if (!m_germanStatic[12].Create(_T("Wortstamm"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+68)) // Perfekt/Wortstamm
		return -1;
	if (!m_germanStatic[13].Create(_T("Konjunktiv"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+69))
		return -1;
	if (!m_germanStatic[14].Create(_T("2. P. Sing."),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+70))
		return -1;
	if (!m_germanStatic[15].Create(_T("2. P. Pl."),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+71))
		return -1;
/*		CONTROL         "jemanden",IDC_JEMANDEN_RADIO,"Button",
                    BS_AUTORADIOBUTTON,273,223,47,10
    CONTROL         "jemandem",IDC_JEMANDEM_RADIO,"Button",
                    BS_AUTORADIOBUTTON,273,234,48,10
    CONTROL         "kein",IDC_NO_CASE_RADIO,"Button",BS_AUTORADIOBUTTON | 
                    WS_GROUP,274,212,29,10*/
	  // older versions of Windows* (NT 3.51 for instance)
    // fail with DEFAULT_GUI_FONT
	if (!m_font.CreateStockObject(DEFAULT_GUI_FONT))
		if (!m_font.CreatePointFont(80, "MS Sans Serif"))
			return -1;
	// Schriftart setzen, Anfang
	m_englishWordKind.SetFont(&m_font);
	m_englishComboBox.SetFont(&m_font);
	for(int i=0;i<18;i++)
		m_englishButton[i].SetFont(&m_font);
	for(i=0;i<4;i++)
		m_englishEdit[i].SetFont(&m_font);
	for(i=0;i<6;i++)
		m_englishStatic[i].SetFont(&m_font);
	for(i=0;i<13;i++)
		m_germanButton[i].SetFont(&m_font);
	for(i=0;i<20;i++)
		m_germanEdit[i].SetFont(&m_font);

	m_germanComboBox.SetFont(&m_font);
	m_germanListBox[0].SetFont(&m_font);
	m_germanListBox[1].SetFont(&m_font);
	for(i=0;i<14;i++)
		m_germanStatic[i].SetFont(&m_font);
	// Schriftart setzen, Ende

	m_englishWordKind.AddString(_T("Substantiv"));
	m_englishWordKind.AddString(_T("Verb"));
	m_englishWordKind.AddString(_T("Adjektiv"));
	m_englishWordKind.AddString(_T("Adverb"));
	m_englishWordKind.AddString(_T("Präposition"));
	m_englishWordKind.AddString(_T("Pronomen"));
	m_englishWordKind.AddString(_T("Konjunktion"));
	m_englishWordKind.SetCurSel(0);

	m_englishButton[0].SetCheck(1);
	m_englishButton[7].SetCheck(1); // Optionsfelder (radiobuttons) initialisieren
	// (bei den Adverbien)
	m_englishComboBox.AddString(_T(""));

	m_germanButton[5].SetCheck(1);
	m_germanButton[11].SetCheck(1);

	m_germanComboBox.AddString(_T(""));
	m_germanListBox[0].AddString(_T("der"));
	m_germanListBox[0].AddString(_T("die"));
	m_germanListBox[0].AddString(_T("das"));
	m_germanListBox[0].SetCurSel(0);
    
	m_germanListBox[1].AddString(_T("haben"));
	m_germanListBox[1].AddString(_T("sein"));
	m_germanListBox[1].ShowWindow(FALSE);
	m_germanListBox[1].SetCurSel(0);


	//m_englishCheck[0].MoveWindow(CRect(100,50,140,140));
	m_englishStatic[0].MoveWindow(CRect(0,0,40,15));
	m_englishStatic[0].ShowWindow(TRUE);
	m_englishStatic[1].MoveWindow(CRect(50,0,100,15));
	m_englishStatic[2].MoveWindow(CRect(140,0,200,15));
	m_englishStatic[3].MoveWindow(CRect(230,0,300,15));
	//m_germanEdit[10].MoveWindow(CRect(0,0,420,440));
	m_englishEdit[1].MoveWindow(CRect(230,20,300,40));
	m_englishWordKind.MoveWindow(CRect(50,20,130,150));

	m_bPreviousWordKind=0;
	m_bAr[0]=1;
	m_bAr[1]=0;
	m_bAr[2]=0;
	m_bAr[6]=0;
	//das Kontrollkästchen "zählbar" markieren
	m_bAr[11]=1;
	m_bAr[59]=1;
	m_bAr[75]=1;
	NounControls();
	return 0;
	UpdateDialogControls(this, FALSE);
	UpdateData(FALSE);
	Invalidate(TRUE);
	OnPaint();
#ifdef _DEBUG
	printf("int CVocableEditorBar::OnCreate(LPCREATESTRUCT lpCreateStruct) ENDE\n");
	fflush(stdout);
#endif
	//}
}

void CVocableEditorBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
    baseCMyBar::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);

    UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

void CVocableEditorBar::OnSize(UINT nType, int cx, int cy) 
{
    baseCMyBar::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    CRect rc;
    GetClientRect(rc);

/*	if(m_englishWordKind.GetCurSel()==0)
	{
		m_englishButton[0].MoveWindow(CRect(270,40,350,55));
		m_englishStatic[0].MoveWindow(CRect(0,20,40,40));
		m_englishStatic[1].MoveWindow(CRect(50,0,100,15));
		m_englishStatic[2].MoveWindow(CRect(140,0,170,15));
		m_englishStatic[3].MoveWindow(CRect(180,0,260,15));
		m_englishStatic[4].MoveWindow(CRect(270,0,350,15));
		//m_english.MoveWindow(CRect(0,20,40,40));
		m_englishEdit[0].MoveWindow(CRect(180,20,260,35));
		m_englishEdit[1].MoveWindow(CRect(270,20,350,35));
		m_englishWordKind.MoveWindow(CRect(50,20,130,100));
		m_germanEdit[0].MoveWindow(CRect(180,80,260,95));
		m_germanEdit[1].MoveWindow(CRect(270,80,350,95));
		m_germanStatic[0].MoveWindow(CRect(0,60,40,80));
		m_germanStatic[1].MoveWindow(CRect(50,60,100,80));
		m_germanStatic[2].MoveWindow(CRect(140,60,170,80));
		m_germanStatic[3].MoveWindow(CRect(180,60,260,80));
		m_germanStatic[10].MoveWindow(CRect(270,60,350,80));
		m_germanListBox[0].MoveWindow(CRect(140,80,165,120));
	}*/
}

void CVocableEditorBar::OnSelChangeEnglishWordkind()
{ // wird aufgerufen, wenn ein Eintrag im Kombinationsfeld 'Wortart'
	// ausgewählt wird
#ifdef _DEBUG
	printf("void CVocableEditorBar::OnSelChangeEnglishWordkind() - ANFANG\n");
#endif

	TRACE("CMainFrame::OnSelChangeUnit()\n");
	//der Fokus soll bei der Wortart bleiben (wenn 
	//m_bSetFocusOnEnglishWordKind ungleich 0 ist, wird in der Funktion 
	//"AdjectiveControls()", "AdverbControls()", "AuxiliaryControls()",
	//"ConjunctionControls()", "NounControls()", "PrepositionControls()",
	//"PronounControls()" oder "VerbControls()" nach dem Erstellen des 
	//Kombinationsfeldes für die englische Wortart der Fokus darauf gesetzt)
	m_bSetFocusOnEnglishWordKind=1;
	//wenn die vorige Wortart ein Substantiv war
	if(m_bPreviousWordKind == WORDKIND_INDEX_FOR_NOUN)
	{
		GetDlgItemText(ENGLISH_SINGULAR_EDIT,m_strAr[0]); //englischer Plural
		GetDlgItemText(ENGLISH_PLURAL_EDIT,m_strAr[1]); //englischer Plural
		GetDlgItemText(ID_USER+34,m_strAr[2]); //deutscher Singular
		GetDlgItemText(ID_USER+35,m_strAr[3]); //deutscher Plural
		//wann steht ein englischer Artikel vor dem Substantiv?
		if(m_englishButton[0].GetCheck())
		{
			m_bAr[0]=1;
			m_bAr[1]=0;
			m_bAr[2]=0;
		}
		if(m_englishButton[1].GetCheck())
		{
			m_bAr[0]=0;
			m_bAr[1]=1;
			m_bAr[2]=0;
		}
		if(m_englishButton[2].GetCheck())
		{
			m_bAr[0]=0;
			m_bAr[1]=0;
			m_bAr[2]=1;
		}
		m_bAr[3]=m_englishButton[10].GetCheck(); //"im Deutschen: Plural"
		m_bAr[4]=m_englishButton[11].GetCheck(); //"im Deutschen: Singular"
		m_bAr[5]=m_englishButton[5].GetCheck(); //"regelmäßig"
		m_bAr[6]=m_germanListBox[0].GetCurSel(); //der deutsche Artikel
		m_bAr[7]=m_germanButton[0].GetCheck(); //Kontrollkästchen "~"
		m_bAr[8]=m_germanButton[1].GetCheck(); //Kontrollkästchen "~e"
		m_bAr[9]=m_germanButton[2].GetCheck(); //Kontrollkästchen "~en/n"
		m_bAr[10]=m_germanButton[3].GetCheck(); //Kontrollkästchen "~er"
		m_bAr[11]=m_englishButton[3].GetCheck();
	}
	//wenn die vorige Wortart ein Verb war
	if(m_bPreviousWordKind==WORDKIND_INDEX_FOR_VERB)
	{
		GetDlgItemText(ENGLISH_INFINITIVE_EDIT,m_strAr[4]); //englischer Infinitiv
		GetDlgItemText(ID_USER+13,m_strAr[5]); //englisches Präteritum
		GetDlgItemText(ID_USER+14,m_strAr[6]); //englisches Partizip Perfekt
		GetDlgItemText(ID_USER+34,m_strAr[7]); //deutscher Infinitiv
		GetDlgItemText(ID_USER+35,m_strAr[8]); //deutscher Präsens 1. Person Singular
		GetDlgItemText(ID_USER+36,m_strAr[9]); //deutscher Präsens 2. Person Singular
		GetDlgItemText(ID_USER+37,m_strAr[10]); //deutscher Präsens 3. Person Singular
		GetDlgItemText(ID_USER+38,m_strAr[11]); //deutscher Präsens 1. Person Plural
		GetDlgItemText(ID_USER+39,m_strAr[12]); //deutscher Präsens 2. Person Plural
		GetDlgItemText(ID_USER+40,m_strAr[13]); //deutscher Präsens 3. Person Plural
		GetDlgItemText(ID_USER+41,m_strAr[14]); //deutscher Präteritum 1. Person Singular
		GetDlgItemText(ID_USER+42,m_strAr[15]); //deutscher Präteritum 2. Person Singular
		GetDlgItemText(ID_USER+43,m_strAr[16]); //deutscher Präteritum 3. Person Singular
		GetDlgItemText(ID_USER+44,m_strAr[17]); //deutscher Präteritum 1. Person Plural
		GetDlgItemText(ID_USER+45,m_strAr[18]); //deutscher Präteritum 2. Person Plural
		GetDlgItemText(ID_USER+46,m_strAr[19]); //deutscher Präteritum 3. Person Plural
		GetDlgItemText(ID_USER+49,m_strAr[20]); //deutscher Wortstamm für Präteritum 3. Person Plural
		GetDlgItemText(ID_USER+47,m_strAr[21]); //deutsches Partizip Perfekt
		GetDlgItemText(ID_USER+48,m_strAr[22]); //deutsches Konjunktiv
		m_bAr[21]=m_englishButton[3].GetCheck(); //"Hilfsverb"
		m_bAr[22]=m_englishButton[4].GetCheck(); //"Aktionsverb"
		m_bAr[23]=m_englishButton[5].GetCheck(); //"regelmäßig"
		m_bAr[24]=m_englishButton[6].GetCheck(); //2. Kontrollkästchen "regelmäßig"
		m_bAr[25]=m_germanButton[0].GetCheck(); //"reflexiv"
		m_bAr[26]=m_germanButton[1].GetCheck(); //"Bewegung"
		m_bAr[27]=m_germanButton[2].GetCheck(); //"kein Fall"
		m_bAr[28]=m_germanButton[3].GetCheck(); //"3. Fall"
		m_bAr[29]=m_germanButton[4].GetCheck(); //"4. Fall"
		m_bAr[30]=m_germanButton[5].GetCheck(); //"regelmäßig"
		m_bAr[31]=m_germanButton[6].GetCheck(); //"Wortstamm"
		m_bAr[32]=m_germanButton[7].GetCheck(); //"ge~"
		m_bAr[33]=m_germanButton[8].GetCheck(); //"ge-2~t"
		//Kombinationsfeld, das die englischen Präpositionen enthält
		m_nAr[0]=m_englishComboBox.GetCurSel();
		//Kombinationsfeld, das die deutschen Präpositionen enthält
		m_nAr[1]=m_germanComboBox.GetCurSel();
	}
	//wenn die vorige Wortart ein Adjektiv war
	if(m_bPreviousWordKind==WORDKIND_INDEX_FOR_ADJECTIVE)
	{
		GetDlgItemText(ENGLISH_POSITIVE_EDIT,m_strAr[23]); //englischer Positiv
		GetDlgItemText(ID_USER+13,m_strAr[24]); //englischer Komparativ
		GetDlgItemText(ID_USER+14,m_strAr[25]); //englischer Superlativ
		GetDlgItemText(ID_USER+15,m_strAr[26]); //englisches Adverb, das zum Adjektiv gehört
		GetDlgItemText(ID_USER+34,m_strAr[27]); //deutscher Positiv
		GetDlgItemText(ID_USER+35,m_strAr[28]); //deutscher Komparativ
		GetDlgItemText(ID_USER+41,m_strAr[29]); //deutscher Superlativ
		GetDlgItemText(ID_USER+47,m_strAr[30]); //deutscher Wortstamm
		m_bAr[44]=m_englishButton[5].GetCheck(); //"regelmäßig"
		m_bAr[45]=m_englishButton[17].GetCheck(); //2. Kontrollkästchen "regelmäßig"
		//Kontrollkästchen "Adjektiv darf nur nach einem Verb benutzt werden"
		m_bAr[46]=m_englishButton[12].GetCheck();
		//Kontrollkästchen "Adjektiv darf nur vor einem Substantiv benutzt werden"
		m_bAr[47]=m_englishButton[13].GetCheck(); //2. Kontrollkästchen "regelmäßig"
		m_bAr[48]=m_germanButton[2].GetCheck(); //"regelmäßig"
	}
	//wenn die vorige Wortart ein Adverb war
	if(m_bPreviousWordKind==WORDKIND_INDEX_FOR_ADVERB)
	{
		GetDlgItemText(ENGLISH_ADVERB_EDIT,m_strAr[31]); //englisches Adverb
		GetDlgItemText(ID_USER+34,m_strAr[32]); //deutsches Adverb
		//"Adverb befindet sich zwischen Subjekt und Verb oder nach der finiten 
		//Form des Verbes 'to be'"
		m_bAr[59]=m_englishButton[7].GetCheck();
		//"Adverb bezieht sich auf ein Adjektiv oder Adverb"
		m_bAr[60]=m_englishButton[8].GetCheck();
		//"Adverb bezieht sich auf ganzen Satz und steht am Satzbeginn"
		m_bAr[61]=m_englishButton[9].GetCheck();
		//"Adverb bezieht sich auf ganzen Satz und steht am Satzende"
		m_bAr[62]=m_englishButton[14].GetCheck();
		m_bAr[63]=m_englishButton[15].GetCheck(); //"Adverb ist ein Fragewort";
		m_bAr[64]=m_englishButton[16].GetCheck(); //"Adverb der Zeit";
	}
	//wenn die vorige Wortart ein Präposition war
	if(m_bPreviousWordKind==WORDKIND_INDEX_FOR_PREPOSITION)
	{
		GetDlgItemText(ENGLISH_PREPOSITION_EDIT,m_strAr[33]); //englische Präposition
		GetDlgItemText(ID_USER+34,m_strAr[34]); //deutsche Präposition
	}
	//wenn die vorige Wortart ein Pronom war
	if(m_bPreviousWordKind==WORDKIND_INDEX_FOR_PRONOUN)
	{
		GetDlgItemText(ENGLISH_PRONOUN_EDIT,m_strAr[35]); //englisches Pronom für den Singular
		GetDlgItemText(ID_USER+13,m_strAr[36]); //englisches Pronom für den Singular
		//deutsches Pronom (für Singular und Plural)
		GetDlgItemText(ID_USER+34,m_strAr[37]);
	}
	//wenn die vorige Wortart eine Konjunktion war
	if(m_bPreviousWordKind==WORDKIND_INDEX_FOR_CONJUNCTION)
	{
		GetDlgItemText(ENGLISH_CONJUNCTION_EDIT,m_strAr[38]); //englische Konjunktion
		GetDlgItemText(ID_USER+34,m_strAr[40]); //deutsche Konjunktion
		m_bAr[75]=m_germanButton[11].GetCheck(); //"Subjekt Prädikat [Objekt]"
		m_bAr[76]=m_germanButton[12].GetCheck(); //"Subjekt [Objekt] Prädikat"
	}
  int nCurrentWordKindSelection=m_englishWordKind.GetCurSel();
  switch(nCurrentWordKindSelection)
  {
  case WORDKIND_INDEX_FOR_NOUN:
		NounControls();
		m_bPreviousWordKind=0;
    break;
  case WORDKIND_INDEX_FOR_VERB:
		//wenn die vorige Wortart ein Substantiv war
		if(m_bPreviousWordKind==0)
		{
		}
		VerbControls(MINIMAL);
    break;
  case WORDKIND_INDEX_FOR_ADJECTIVE:
		AdjectiveControls();
    break;
  case WORDKIND_INDEX_FOR_ADVERB:
		AdverbControls();
    break;
  case WORDKIND_INDEX_FOR_PREPOSITION:
		PrepositionControls();
    break;
  case WORDKIND_INDEX_FOR_PRONOUN:
		PronounControls();
    break;
  case WORDKIND_INDEX_FOR_CONJUNCTION:
		ConjunctionControls();
    break;
  }//switch(nCurrentWordKindSelection)
	m_bPreviousWordKind=nCurrentWordKindSelection;

	//m_englishWordKind.MoveWindow(CRect(50,200,130,300));

	//m_englishWordKind.UpdateData(FALSE);
#ifdef _DEBUG
	printf("void CVocableEditorBar::OnSelChangeEnglishWordkind() - ENDE\n");
#endif
}
//OnSelChangeEnglishWordkind

void CVocableEditorBar::AdjectiveControls()
{//bei einem Laufzeitfehler dieses Computerprogramms durch Aufruf dieser
	//Funktion kann es sein, dass auf nicht-existente Steuerelemente 
	//zugegriffen wird

	// die Positionen der Steuerelemente müssen zur Laufzeit berechnet werden,
	// da sie untzerschiedlich groß sein können (z.B. kann 'große Schriftarten'
	// aktiviert sein (Start|Einstellungen|Systemsteuerung|Anzeige|
	// Registerkarte Einstellungen|Weitere Optionen...) )
	DWORD dwHorizontalOffset=0;
	DWORD dwVerticalOffset=0;
	DWORD dwAmount=0;
	DWORD dwVerticalStartOfGermanControls=0;
	CString str;
	CDC * pDC=GetDC();
	CSize size;
	CFont * pOldFont=pDC->SelectObject(m_englishStatic[0].GetFont());
	TEXTMETRIC textmetric;
	pDC->GetTextMetrics(&textmetric);

	for(BYTE bRun=0;bRun<19;bRun++)
		m_englishButton[bRun].DestroyWindow();
	//m_englishWordKind.DestroyWindow();
	m_englishComboBox.DestroyWindow();
	m_germanComboBox.DestroyWindow();
	for(bRun=0;bRun<4;bRun++)
		m_germanListBox[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_englishEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanButton[bRun].DestroyWindow();
	//for(bRun=0;bRun<20;bRun++)
	//	m_germanStatic[bRun].DestroyWindow();
	m_english.DestroyWindow();
	m_wordkind.DestroyWindow();
	m_englishStatic[5].DestroyWindow();
	//for(bRun=0;bRun<10;bRun++)
	//	m_englishStatic[bRun].DestroyWindow();
	//m_englishWordKind.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST,
	//	CRect(0,0,0,0), this,IDC_ENGLISH_WORDKIND_COMBO);
	m_englishStatic[5].Create(_T("Perfekt"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+21);

	//Singular/Positiv
	m_englishEdit[0].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+12);
	//Plural/Komperativ
	m_englishEdit[1].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+13);
	m_englishButton[3].Create(_T("regelmäßig"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+2);
	//Superlativ
	m_englishEdit[2].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+14);
  m_englishButton[5].Create(_T("more/most"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0),this,ID_USER+4);
	//Adverb
	m_englishEdit[3].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+15);
	m_englishButton[6].Create(_T("Adjektiv darf nur nach einem Verb \
benutzt werden"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,CRect(0,0,0,0),this,
		ID_USER+5);
	m_englishButton[10].Create(_T("Adjektiv darf nur vor einem Substantiv \
benutzt werden"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,CRect(0,0,0,0),this,
		ID_USER+7);
	m_germanEdit[0].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+34); // Positiv
	m_germanEdit[1].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+35); // Komperativ
	m_germanButton[2].Create(_T("regelmäßig"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,
		CRect(0,0,0,0), this,ID_USER+24);
	m_germanEdit[7].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+41); // Superlativ
	m_germanEdit[13].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+47); // Wortstamm

	//englischer Positiv
	SetDlgItemText(ID_USER+12,m_strAr[23]);
	//englischer Komparativ
	SetDlgItemText(ID_USER+13,m_strAr[24]);
	//englischer Superlativ
	SetDlgItemText(ID_USER+14,m_strAr[25]);
	//englisches Adverb, das zum Adjektiv gehört
	SetDlgItemText(ID_USER+15,m_strAr[26]);
	//deutscher Positiv
	SetDlgItemText(ID_USER+34,m_strAr[27]);
	//deutscher Komparativ
	SetDlgItemText(ID_USER+35,m_strAr[28]);
	//deutscher Superlativ
	SetDlgItemText(ID_USER+41,m_strAr[29]);
	//deutscher Wortstamm
	SetDlgItemText(ID_USER+47,m_strAr[30]);
	//Kontrollkästchen "regelmäßig"
	m_englishButton[3].SetCheck(m_bAr[44]);
	//2. Kontrollkästchen "more/most"
	m_englishButton[5].SetCheck(m_bAr[45]);
	//Kontrollkästchen "Adjektiv darf nur nach einem Verb benutzt werden"
	m_englishButton[6].SetCheck(m_bAr[46]);
	//Kontrollkästchen "Adjektiv darf nur vor einem Substantiv benutzt werden"
	//2. Kontrollkästchen "regelmäßig"
	m_englishButton[10].SetCheck(m_bAr[47]);
	//"regelmäßig"
	m_germanButton[2].SetCheck(m_bAr[48]);

	//m_englishWordKind.AddString(_T("Substantiv"));
	//m_englishWordKind.AddString(_T("Verb"));
	//m_englishWordKind.AddString(_T("Adjektiv"));
	//m_englishWordKind.AddString(_T("Adverb"));
	//m_englishWordKind.AddString(_T("Präposition"));
	//m_englishWordKind.AddString(_T("Pronomen"));
	//m_englishWordKind.AddString(_T("Konjunktion"));
	//m_englishWordKind.SetCurSel(1); //1 für "Verb"

	for(int i=0;i<18;i++)
		m_englishButton[i].SetFont(&m_font);
	m_englishComboBox.SetFont(&m_font);
	for(i=0;i<4;i++)
		m_englishEdit[i].SetFont(&m_font);
	m_englishWordKind.SetFont(&m_font);
	for(bRun=0;bRun<20;bRun++)
		m_germanButton[bRun].SetFont(&m_font);
	m_germanComboBox.SetFont(&m_font);
	for(bRun=0;bRun<20;bRun++)
		m_germanEdit[bRun].SetFont(&m_font);
	m_englishStatic[5].SetFont(&m_font);

	//m_englishWordKind.SetFocus();	
	//m_englishWordKind.MoveWindow(CRect(50,20,130,150));
	
	// Zeichenketten zuerst zuweisen, damit die Breiten der Steuerelemente
	// richtig ermittelt werden
	SetDlgItemText(ID_USER+18,_T("Positiv"));
	SetDlgItemText(ID_USER+19,_T("Komperativ"));
	SetDlgItemText(ID_USER+20,_T("Superlativ"));
	SetDlgItemText(ID_USER+21,_T("Adverb"));
	SetDlgItemText(ID_USER+58,_T("Positiv"));
	SetDlgItemText(ID_USER+59,_T("Komperativ"));
	SetDlgItemText(ID_USER+60,_T("Superlativ"));
	SetDlgItemText(ID_USER+67,_T("Wortstamm"));

	GetDlgItemText(m_englishStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[0].MoveWindow(CRect(0,0,size.cx,textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+5;
	
	GetDlgItemText(m_englishStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	
	dwAmount=0;
	for(int dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	//Kombinationsfeld "Wortart"
	m_englishWordKind.MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+dwAmount+30,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=dwAmount+5+30;
	dwVerticalOffset=0;
	//Textfeld "Positiv"
	GetDlgItemText(m_englishStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[2].MoveWindow(CRect(dwHorizontalOffset,0,
		dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;
	//Eingabefeld "Positiv"
	m_englishEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+90,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight*2;
	//Kontrollkästchen "Adjektiv darf nur nach einem Verb benutzt werden"
	GetDlgItemText(m_englishButton[6].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[6].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	//Kontrollkästchen "Adjektiv darf nur vor einem Substantiv benutzt werden"
	GetDlgItemText(m_englishButton[10].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[10].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=95;
	dwVerticalStartOfGermanControls=dwVerticalOffset+textmetric.tmHeight*2;
	//Textfeld "Komperativ"
	GetDlgItemText(m_englishStatic[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[3].MoveWindow(CRect(dwHorizontalOffset,0,
		dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	//Eingabefeld "Komperativ"
	m_englishEdit[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	//Kontrollkästchen "regelmäßig" im Englischen
	m_englishButton[3].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+25;
	//Textfeld "Superlativ"
	GetDlgItemText(m_englishStatic[4].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[4].MoveWindow(CRect(dwHorizontalOffset,0,dwHorizontalOffset+
		size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;
	GetDlgItemText(m_englishButton[5].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	//Eingabefeld "Superlativ"
	m_englishEdit[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	//dwHorizontalOffset+=95;
	//Kontrollkästchen "more/most"
	m_englishButton[5].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+25;
	//Textfeld "Adverb"
	GetDlgItemText(m_englishStatic[5].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[5].MoveWindow(CRect(dwHorizontalOffset,0,
		dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;
	//Eingabefeld "Adverb"
	m_englishEdit[3].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+90,dwVerticalOffset+textmetric.tmHeight));

	GetDlgItemText(m_germanStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[0].MoveWindow(CRect(0,dwVerticalStartOfGermanControls,
		size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwHorizontalOffset=size.cx+5;

	GetDlgItemText(m_germanStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[1].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwAmount=0;
	for(dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	dwHorizontalOffset+=dwAmount+30+5;

	GetDlgItemText(m_germanStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[2].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	m_germanEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+90,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	dwHorizontalOffset+=95;

	GetDlgItemText(m_germanStatic[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[3].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	m_germanEdit[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+90,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	
	GetDlgItemText(m_germanButton[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanButton[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=95;

	GetDlgItemText(m_germanStatic[4].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[4].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	m_germanEdit[7].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+90,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=95;

	GetDlgItemText(m_germanStatic[11].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[11].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	m_germanEdit[13].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+90,dwVerticalOffset+textmetric.tmHeight));

	//m_englishStatic[5].ShowWindow(SW_SHOW);
#ifdef _DEBUG
	printf("m_englishWordKind.GetCurSel(): %d\n",m_englishWordKind.GetCurSel());
#endif
}

void CVocableEditorBar::AdverbControls()
{//bei einem Laufzeitfehler dieses Computerprogramms durch Aufruf dieser
	//Funktion kann es sein, dass auf nicht-existente Steuerelemente 
	//zugegriffen wird

	// die Positionen der Steuerelemente müssen zur Laufzeit berechnet werden,
	// da sie untzerschiedlich groß sein können (z.B. kann 'große Schriftarten'
	// aktiviert sein (Start|Einstellungen|Systemsteuerung|Anzeige|
	// Registerkarte Einstellungen|Weitere Optionen...) )
	DWORD dwHorizontalOffset=0;
	DWORD dwVerticalOffset=0;
	DWORD dwAmount=0;
	DWORD dwVerticalStartOfGermanControls=0;
	CString str;
	CDC * pDC=GetDC();
	CSize size;
	CFont * pOldFont=pDC->SelectObject(m_englishStatic[0].GetFont());
	TEXTMETRIC textmetric;
	pDC->GetTextMetrics(&textmetric);

	for(BYTE bRun=0;bRun<19;bRun++)
		m_englishButton[bRun].DestroyWindow();
	//m_englishWordKind.DestroyWindow();
	m_englishComboBox.DestroyWindow();
	m_germanComboBox.DestroyWindow();
	for(bRun=0;bRun<4;bRun++)
		m_germanListBox[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_englishEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanButton[bRun].DestroyWindow();
	//for(bRun=0;bRun<20;bRun++)
	//	m_germanStatic[bRun].DestroyWindow();
	m_english.DestroyWindow();
	m_englishStatic[5].DestroyWindow();
	m_wordkind.DestroyWindow();
	//for(bRun=0;bRun<10;bRun++)
	//	m_englishStatic[bRun].DestroyWindow();
	//m_englishWordKind.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST,
	//	CRect(0,0,0,0), this,IDC_ENGLISH_WORDKIND_COMBO);
	m_englishEdit[0].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+12); // Singular/Positiv
	m_englishButton[7].Create(_T("Adverb befindet sich zwischen Subjekt \
und Verb oder nach der finiten Form des Verbes 'to be'"),WS_CHILD|
		WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,CRect(0,0,0,0), this,ID_USER+6);
	m_englishButton[8].Create(_T("Adverb bezieht sich auf ein Adjektiv \
oder Adverb"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,CRect(0,0,0,0),this,
		ID_USER+6);
	m_englishButton[9].Create(_T("Adverb bezieht sich auf ganzen Satz \
und steht am Satzbeginn"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,
		CRect(0,0,0,0), this,ID_USER+6);
	m_englishButton[14].Create(_T("Adverb bezieht sich auf ganzen Satz \
und steht am Satzende"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,
		CRect(0,0,0,0), this,ID_USER+6);
	m_englishButton[15].Create(_T("Adverb ist ein Fragewort"),WS_CHILD|
		WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,CRect(0,0,0,0),this,ID_USER+6);
	m_englishButton[16].Create(_T("Adverb der Zeit"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTORADIOBUTTON,CRect(0,0,0,0),this,ID_USER+6);
	m_germanEdit[0].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+34); // Positiv

	SetDlgItemText(ID_USER+12,m_strAr[31]); //englisches Adverb
	SetDlgItemText(ID_USER+34,m_strAr[32]); //deutsches Adverb
	//"Adverb befindet sich zwischen Subjekt und Verb oder nach der finiten 
	//Form des Verbes 'to be'"
	m_englishButton[7].SetCheck(m_bAr[59]);
	//"Adverb bezieht sich auf ein Adjektiv oder Adverb"
	m_englishButton[8].SetCheck(m_bAr[60]);
	//"Adverb bezieht sich auf ganzen Satz und steht am Satzbeginn"
	m_englishButton[9].SetCheck(m_bAr[61]);
	//"Adverb bezieht sich auf ganzen Satz und steht am Satzende"
	m_englishButton[14].SetCheck(m_bAr[62]);
	m_englishButton[15].SetCheck(m_bAr[63]); //"Adverb ist ein Fragewort";
	m_englishButton[16].SetCheck(m_bAr[64]); //"Adverb der Zeit";

	for(int i=0;i<18;i++)
		m_englishButton[i].SetFont(&m_font);
	m_englishComboBox.SetFont(&m_font);
	for(i=0;i<4;i++)
		m_englishEdit[i].SetFont(&m_font);
	//m_englishWordKind.SetFont(&m_font);
	for(bRun=0;bRun<20;bRun++)
		m_germanButton[bRun].SetFont(&m_font);
	m_germanComboBox.SetFont(&m_font);
	for(bRun=0;bRun<20;bRun++)
		m_germanEdit[bRun].SetFont(&m_font);

	//m_englishWordKind.SetFocus();	
	//m_englishWordKind.MoveWindow(CRect(50,20,130,150));

	// Zeichenketten zuerst zuweisen, damit die Breiten der Steuerelemente
	// richtig ermittelt werden
	SetDlgItemText(ID_USER+18,_T("Adverb"));
	SetDlgItemText(ID_USER+58,_T("Adverb"));

	GetDlgItemText(m_englishStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[0].MoveWindow(CRect(0,0,size.cx,textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+5;
	
	GetDlgItemText(m_englishStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	
	dwAmount=0;
	for(int dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	m_englishWordKind.MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+dwAmount+30,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=dwAmount+5+30;
	dwVerticalOffset=0;

	GetDlgItemText(m_englishStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[2].MoveWindow(CRect(dwHorizontalOffset,0,dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;

	m_englishEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+90,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	
	GetDlgItemText(m_englishButton[7].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[7].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[8].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[8].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[9].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[9].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[14].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[14].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[15].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[15].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[16].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[16].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalStartOfGermanControls=dwVerticalOffset+textmetric.tmHeight*2;

	GetDlgItemText(m_germanStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[0].MoveWindow(CRect(0,dwVerticalStartOfGermanControls,size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwHorizontalOffset=size.cx+5;

	GetDlgItemText(m_germanStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwAmount=0;
	for(dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	dwHorizontalOffset+=dwAmount+30+5;

	GetDlgItemText(m_germanStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	m_germanEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+90,dwVerticalOffset+textmetric.tmHeight));

	m_englishButton[0].ShowWindow(FALSE);
	m_englishButton[1].ShowWindow(FALSE);
	m_englishButton[2].ShowWindow(FALSE);
	m_englishButton[3].ShowWindow(FALSE);
	m_englishButton[4].ShowWindow(FALSE);
	m_englishButton[5].ShowWindow(FALSE);
	//m_englishButton[7].MoveWindow(CRect(140,37,610,52)); // zwischen Subjekt und Prädikat
	//m_englishButton[8].MoveWindow(CRect(140,54,400,69)); // bezieht sich auf ein Adverb oder Adjektiv
	//m_englishButton[9].MoveWindow(CRect(140,71,400,86)); // bezieht sich auf ganzen Satz
	m_englishButton[6].ShowWindow(FALSE);
	m_englishButton[7].ShowWindow(TRUE);
	m_englishButton[8].ShowWindow(TRUE);
	m_englishButton[9].ShowWindow(TRUE);
	m_englishButton[10].ShowWindow(FALSE); // im Deutschen: Plural
	m_englishButton[11].ShowWindow(FALSE); // im Deutschen: Singular
	m_englishButton[12].ShowWindow(FALSE);
	m_englishButton[13].ShowWindow(FALSE);
	m_englishButton[14].ShowWindow(SW_SHOW);
	m_englishButton[15].ShowWindow(SW_SHOW);
	m_englishButton[16].ShowWindow(SW_SHOW);
	m_englishComboBox.ShowWindow(FALSE);
	//m_englishEdit[0].MoveWindow(CRect(140,20,220,35));
	m_englishEdit[1].ShowWindow(FALSE);
	m_englishEdit[2].ShowWindow(FALSE);
	m_englishEdit[3].ShowWindow(FALSE);
	m_englishStatic[3].ShowWindow(FALSE);
	m_englishStatic[4].ShowWindow(FALSE);
	m_englishStatic[5].ShowWindow(FALSE);
	m_germanButton[0].ShowWindow(FALSE);
	m_germanButton[1].ShowWindow(FALSE);
	m_germanButton[2].ShowWindow(FALSE);
	m_germanButton[3].ShowWindow(FALSE);
	m_germanButton[4].ShowWindow(FALSE);
	m_germanButton[5].ShowWindow(FALSE);
	m_germanButton[6].ShowWindow(FALSE);
	m_germanButton[7].ShowWindow(FALSE);
	m_germanButton[11].ShowWindow(SW_HIDE);//"Subjekt Prädikat [Objekt]"
	m_germanButton[12].ShowWindow(SW_HIDE);//"Subjekt [Objekt] Prädikat"
	m_germanComboBox.ShowWindow(FALSE);
	//m_germanEdit[0].MoveWindow(CRect(140,107,220,122));//MoveWindow(CRect(140,105,220,120));
	m_germanEdit[1].ShowWindow(FALSE);
	m_germanEdit[2].ShowWindow(FALSE);
	m_germanEdit[3].ShowWindow(FALSE);
	m_germanEdit[4].ShowWindow(FALSE);
	m_germanEdit[5].ShowWindow(FALSE);
	m_germanEdit[6].ShowWindow(FALSE);
	m_germanEdit[7].ShowWindow(FALSE);
	m_germanEdit[8].ShowWindow(FALSE);
	m_germanEdit[9].ShowWindow(FALSE);
	m_germanEdit[10].ShowWindow(FALSE);
	m_germanEdit[11].ShowWindow(FALSE);
	m_germanEdit[12].ShowWindow(FALSE);
	m_germanEdit[13].ShowWindow(FALSE);
	m_germanListBox[0].ShowWindow(FALSE);
	m_germanListBox[1].ShowWindow(FALSE);
	//m_germanStatic[0].MoveWindow(CRect(0,90,40,105));
	//m_germanStatic[1].MoveWindow(CRect(50,90,130,105));
	//m_germanStatic[2].MoveWindow(CRect(140,90,220,105));
	m_germanStatic[3].ShowWindow(FALSE);
	m_germanStatic[4].ShowWindow(FALSE);
	m_germanStatic[5].ShowWindow(FALSE);
	m_germanStatic[6].ShowWindow(FALSE);
	m_germanStatic[7].ShowWindow(FALSE);
	m_germanStatic[8].ShowWindow(FALSE);
	m_germanStatic[9].ShowWindow(FALSE);
	m_germanStatic[10].ShowWindow(FALSE);
	m_germanStatic[11].ShowWindow(FALSE);
	m_germanStatic[12].ShowWindow(FALSE);
	m_germanStatic[13].ShowWindow(FALSE);
	//m_germanStatic[4].ShowWindow(FALSE);
	//m_germanStatic[5].ShowWindow(FALSE);
}

void CVocableEditorBar::AuxiliaryVerbControls()
{//bei einem Laufzeitfehler dieses Computerprogramms durch Aufruf dieser
	//Funktion kann es sein, dass auf nicht-existente Steuerelemente 
	//zugegriffen wird

	// die Positionen der Steuerelemente müssen zur Laufzeit berechnet werden,
	// da sie unterschiedlich groß sein können (z.B. kann 'große Schriftarten'
	// aktiviert sein (Start|Einstellungen|Systemsteuerung|Anzeige|
	// Registerkarte Einstellungen|Weitere Optionen...) )
	DWORD dwHorizontalOffset=0;
	DWORD dwVerticalOffset=0;
	DWORD dwAmount=0;
	DWORD dwVerticalStartOfGermanControls=0;
	CString str;
	CDC * pDC=GetDC();
	CSize size;
	CFont * pOldFont=pDC->SelectObject(m_englishStatic[0].GetFont());
	TEXTMETRIC textmetric;
	pDC->GetTextMetrics(&textmetric);

	for(BYTE bRun=0;bRun<19;bRun++)
		m_englishButton[bRun].DestroyWindow();
	m_englishWordKind.DestroyWindow();
	m_englishComboBox.DestroyWindow();
	m_germanComboBox.DestroyWindow();
	for(bRun=0;bRun<4;bRun++)
		m_germanListBox[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_englishEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanButton[bRun].DestroyWindow();
	//for(bRun=0;bRun<20;bRun++)
	//	m_germanStatic[bRun].DestroyWindow();
	m_english.DestroyWindow();
	m_englishStatic[3].DestroyWindow();
	m_englishStatic[4].DestroyWindow();
	m_englishStatic[5].DestroyWindow();
	m_germanStatic[3].DestroyWindow();
	m_wordkind.DestroyWindow();
	//for(bRun=0;bRun<10;bRun++)
	//	m_englishStatic[bRun].DestroyWindow();

	m_englishWordKind.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST,
		CRect(0,0,0,0), this,IDC_ENGLISH_WORDKIND_COMBO);
	//Infinitiv
	m_englishEdit[0].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+12);
	m_englishButton[3].Create(_T("modales Hilfsverb"),WS_CHILD|WS_TABSTOP|
		WS_VISIBLE|BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+2);
	//Infinitiv
	m_germanEdit[0].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+34);
	//ich
	m_germanEdit[1].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+35);
	//du
	m_germanEdit[2].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+36);
	//er/sie/es
	m_germanEdit[3].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+37);
	//wir
	m_germanEdit[4].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+38);
	//ihr
	m_germanEdit[5].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+39);
	m_germanButton[2].Create(_T("Wortstamm"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+24);
	//"Wortstamm" Präsens
	m_germanEdit[6].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+40);
	//ich
	m_germanEdit[7].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+41);
	//du
	m_germanEdit[8].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+42);
	//er/sie/es
	m_germanEdit[9].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+43);
	//wir
	m_germanEdit[10].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+44);
	//ihr
	m_germanEdit[11].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+45);
	m_germanButton[3].Create(_T("Wortstamm"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+25);
	//Wortstamm
	m_germanEdit[15].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+49);
	//Perfekt
	m_germanEdit[13].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+47);
	m_germanButton[4].Create(_T("ge~"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+26);
	m_germanButton[8].Create(_T("ge~-2t"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+30);
	//Konjunktiv
	m_germanEdit[14].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+48);

	m_englishButton[3].SetCheck(1);
	m_englishWordKind.AddString(_T("Substantiv"));
	m_englishWordKind.AddString(_T("Verb"));
	m_englishWordKind.AddString(_T("Adjektiv"));
	m_englishWordKind.AddString(_T("Adverb"));
	m_englishWordKind.AddString(_T("Präposition"));
	m_englishWordKind.AddString(_T("Pronomen"));
	m_englishWordKind.AddString(_T("Konjunktion"));
	m_englishWordKind.SetCurSel(1); //1 für "Verb"

	SetDlgItemText(ID_USER+12,m_strAr[4]); //englischer Infinitiv
	SetDlgItemText(ID_USER+34,m_strAr[7]); //deutscher Infinitiv
	SetDlgItemText(ID_USER+35,m_strAr[8]); //deutscher Präsens 1. Person Singular
	SetDlgItemText(ID_USER+36,m_strAr[9]); //deutscher Präsens 2. Person Singular
	SetDlgItemText(ID_USER+37,m_strAr[10]); //deutscher Präsens 3. Person Singular
	SetDlgItemText(ID_USER+38,m_strAr[11]); //deutscher Präsens 1. Person Plural
	SetDlgItemText(ID_USER+39,m_strAr[12]); //deutscher Präsens 2. Person Plural
	SetDlgItemText(ID_USER+40,m_strAr[13]); //deutscher Präsens 3. Person Plural
	SetDlgItemText(ID_USER+41,m_strAr[14]); //deutscher Präteritum 1. Person Singular
	SetDlgItemText(ID_USER+42,m_strAr[15]); //deutscher Präteritum 2. Person Singular
	SetDlgItemText(ID_USER+43,m_strAr[16]); //deutscher Präteritum 3. Person Singular
	SetDlgItemText(ID_USER+44,m_strAr[17]); //deutscher Präteritum 1. Person Plural
	SetDlgItemText(ID_USER+45,m_strAr[18]); //deutscher Präteritum 2. Person Plural
	SetDlgItemText(ID_USER+46,m_strAr[19]); //deutscher Präteritum 3. Person Plural
	SetDlgItemText(ID_USER+49,m_strAr[20]); //deutscher Wortstamm für Präteritum 3. Person Plural
	SetDlgItemText(ID_USER+47,m_strAr[21]); //deutsches Partizip Perfekt
	SetDlgItemText(ID_USER+48,m_strAr[22]); //deutsches Konjunktiv
	m_englishButton[3].SetCheck(m_bAr[11]); //"Hilfsverb"
	m_germanButton[5].SetCheck(m_bAr[20]); //"regelmäßig"
	m_germanButton[6].SetCheck(m_bAr[21]); //"Wortstamm"
	m_germanButton[7].SetCheck(m_bAr[22]); //"ge~"
	m_germanButton[8].SetCheck(m_bAr[23]); //"ge-2~t"

	for(int i=0;i<18;i++)
		m_englishButton[i].SetFont(&m_font);
	m_englishEdit[0].SetFont(&m_font);
	m_englishWordKind.SetFont(&m_font);
	for(bRun=2;bRun<20;bRun++)
		m_germanButton[bRun].SetFont(&m_font);
	for(bRun=0;bRun<20;bRun++)
		m_germanEdit[bRun].SetFont(&m_font);
	m_germanListBox[0].SetFont(&m_font);
	m_englishStatic[5].SetFont(&m_font);

	if(m_bSetFocusOnEnglishWordKind)
		m_englishWordKind.SetFocus();	
	m_englishWordKind.MoveWindow(CRect(50,20,130,150));
	// Zeichenketten zuerst zuweisen, damit die Breiten der Steuerelemente
	// richtig ermittelt werden
	SetDlgItemText(ID_USER+58,_T("Infinitiv"));
	SetDlgItemText(ID_USER+60,_T("Präsens"));
	SetDlgItemText(ID_USER+67,_T("Präteritum"));
	SetDlgItemText(ID_USER+68,_T("Perfekt"));

	GetDlgItemText(m_englishStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[0].MoveWindow(CRect(0,0,size.cx,textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+5;
	
	GetDlgItemText(m_englishStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	
	dwAmount=0;
	for(int dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	m_englishWordKind.MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+dwAmount+30,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=dwAmount+5+30;
	dwVerticalOffset=0;

	GetDlgItemText(m_englishStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[2].MoveWindow(CRect(dwHorizontalOffset,0,
		dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;

	dwAmount=size.cx;
	GetDlgItemText(m_englishButton[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	if(size.cx+20>dwAmount)
		dwAmount=size.cx+20;
	m_englishEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+dwAmount,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[3].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));

	dwHorizontalOffset+=dwAmount+5;
	dwVerticalStartOfGermanControls=dwVerticalOffset+(textmetric.tmHeight*2);

	GetDlgItemText(m_germanStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[0].MoveWindow(CRect(0,dwVerticalStartOfGermanControls,
		size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwHorizontalOffset=size.cx+5;

	GetDlgItemText(m_germanStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[1].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwAmount=0;
	for(dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	dwHorizontalOffset+=dwAmount+35;

	GetDlgItemText(m_germanStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[2].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	m_germanEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+100,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=105;

	GetDlgItemText(m_germanStatic[4].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[4].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	GetDlgItemText(m_germanStatic[5].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[5].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	dwAmount=size.cx;

	GetDlgItemText(m_germanStatic[6].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[6].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	if(size.cx>dwAmount)
		dwAmount=size.cx;

	GetDlgItemText(m_germanStatic[7].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[7].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	if(size.cx>dwAmount)
		dwAmount=size.cx;

	GetDlgItemText(m_germanStatic[8].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[8].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	if(size.cx>dwAmount)
		dwAmount=size.cx;

	GetDlgItemText(m_germanStatic[9].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[9].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	if(size.cx>dwAmount)
		dwAmount=size.cx;

	GetDlgItemText(m_germanButton[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanButton[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[6].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=dwAmount+5;

	m_germanEdit[1].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls+textmetric.tmHeight,dwHorizontalOffset+
		size.cx,dwVerticalStartOfGermanControls+(textmetric.tmHeight*2)));
	dwVerticalOffset=dwVerticalStartOfGermanControls+(textmetric.tmHeight*2);

	m_germanEdit[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[3].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[4].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[5].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	dwHorizontalOffset+=size.cx+5;

	//Präteritum im deutschen Teil des Vokabeleditors (4. Spalte)
	GetDlgItemText(m_germanStatic[11].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[11].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;
	
	GetDlgItemText(m_germanButton[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);

	m_germanEdit[7].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[8].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[9].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[10].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[11].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanButton[3].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	//Wortstamm
	m_germanEdit[15].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+25;

	//Perfekt im deutschen Teil des Vokabeleditors (5. Spalte)
	GetDlgItemText(m_germanStatic[12].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[12].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	GetDlgItemText(m_germanButton[8].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanEdit[13].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanButton[4].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanButton[8].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_germanButton[8].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	dwHorizontalOffset+=size.cx+25;

	GetDlgItemText(m_germanStatic[13].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[13].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	m_germanEdit[14].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	SetDlgItemText(ID_USER+18,_T("Infinitiv"));
}

// die Positionen der Steuerelemente müssen zur Laufzeit berechnet werden,
// da sie unterschiedlich groß sein können (z.B. kann 'große Schriftarten'
// aktiviert sein (Start|Einstellungen|Systemsteuerung|Anzeige|
// Registerkarte Einstellungen|Weitere Optionen...) )
void CVocableEditorBar::ConjunctionControls()
{//bei einem Laufzeitfehler dieses Computerprogramms durch Aufruf dieser
	//Funktion kann es sein, dass auf nicht-existente Steuerelemente 
	//zugegriffen wird

	DWORD dwHorizontalOffset=0;
	DWORD dwVerticalOffset=0;
	DWORD dwFirstGermanStatic=0;
	DWORD dwAmount=0;
	DWORD dwVerticalStartOfGermanControls=0;
	CString str;
	CDC * pDC=GetDC();
	CSize size;
	CFont * pOldFont=pDC->SelectObject(m_englishStatic[0].GetFont());
	TEXTMETRIC textmetric;
	pDC->GetTextMetrics(&textmetric);

	for(BYTE bRun=0;bRun<19;bRun++)
		m_englishButton[bRun].DestroyWindow();
	//m_englishWordKind.DestroyWindow();
	m_englishComboBox.DestroyWindow();
	m_germanComboBox.DestroyWindow();
	for(bRun=0;bRun<4;bRun++)
		m_germanListBox[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_englishEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanButton[bRun].DestroyWindow();
	//for(bRun=0;bRun<20;bRun++)
	//	m_germanStatic[bRun].DestroyWindow();
	m_english.DestroyWindow();
	m_englishStatic[5].DestroyWindow();
	m_wordkind.DestroyWindow();
	//for(bRun=0;bRun<10;bRun++)
	//	m_englishStatic[bRun].DestroyWindow();
	//m_englishWordKind.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST,
	//	CRect(0,0,0,0), this,IDC_ENGLISH_WORDKIND_COMBO);
	m_englishEdit[0].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+12); // Singular/Positiv
	m_germanEdit[0].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+34); // Positiv
	m_germanButton[11].Create(_T("Subjekt Prädikat [Objekt]"),WS_CHILD|
		WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,CRect(0,0,0,0),this,ID_USER+72);
	m_germanButton[12].Create(_T("Subjekt [Objekt] Prädikat"),WS_CHILD|
		WS_TABSTOP|WS_VISIBLE|BS_AUTORADIOBUTTON,CRect(0,0,0,0),this,ID_USER+73);

	SetDlgItemText(ID_USER+12,m_strAr[38]); //englische Konjunktion
	SetDlgItemText(ID_USER+34,m_strAr[40]); //deutsche Konjunktion
	m_germanButton[11].SetCheck(m_bAr[75]); //"Subjekt Prädikat [Objekt]"
	m_germanButton[12].SetCheck(m_bAr[76]); //"Subjekt [Objekt] Prädikat"

	for(int i=0;i<18;i++)
		m_englishButton[i].SetFont(&m_font);
	m_englishComboBox.SetFont(&m_font);
	for(i=0;i<4;i++)
		m_englishEdit[i].SetFont(&m_font);
	m_englishWordKind.SetFont(&m_font);
	for(bRun=0;bRun<20;bRun++)
		m_germanButton[bRun].SetFont(&m_font);
	m_germanComboBox.SetFont(&m_font);
	for(bRun=0;bRun<20;bRun++)
		m_germanEdit[bRun].SetFont(&m_font);

	//m_englishWordKind.SetFocus();	
	//m_englishWordKind.MoveWindow(CRect(50,20,130,150));
	
	SetDlgItemText(ID_USER+18,_T("Konjunktion"));
	SetDlgItemText(ID_USER+58,_T("Konjunktion"));
	SetDlgItemText(ID_USER+59,_T("Satzbau"));
	//"Englisch"
	GetDlgItemText(m_englishStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[0].MoveWindow(CRect(0,0,size.cx,textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+5;
	//"Wortart"
	GetDlgItemText(m_englishStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[1].MoveWindow(CRect(dwHorizontalOffset,0,dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	//Kombinationsfeld "Wortart"
	dwAmount=0;
	for(int dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	m_englishWordKind.MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+dwAmount+30,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=dwAmount+5+30;
	dwVerticalOffset=0;
	//"Konjunktion"
	GetDlgItemText(m_englishStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[2].MoveWindow(CRect(dwHorizontalOffset,0,dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;
	//Textfeld "Konjunktion"
	m_englishEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+80,
		dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight+15;
	//"Deutsch"
	GetDlgItemText(m_germanStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[0].MoveWindow(CRect(0,dwVerticalOffset,size.cx,
		dwVerticalOffset+textmetric.tmHeight));
	dwFirstGermanStatic=size.cx;
	dwHorizontalOffset=size.cx+5;
	//"Wortart"
	GetDlgItemText(m_germanStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset=dwFirstGermanStatic+dwAmount+40;
	//"Konjunktion"
	GetDlgItemText(m_germanStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	//dwVerticalOffset+=textmetric.tmHeight;
	//Textfeld "Konjunktion"
	m_germanEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset+
		textmetric.tmHeight,dwHorizontalOffset+80,dwVerticalOffset+
		textmetric.tmHeight*2));
	dwHorizontalOffset+=85;
	//"Satzbau"
	GetDlgItemText(m_germanStatic[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[3].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	//"Subjekt Prädikat [Objekt]"
	GetDlgItemText(m_germanButton[11].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanButton[11].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	//"Subjekt [Objekt] Prädikat"
	GetDlgItemText(m_germanButton[12].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanButton[12].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));

	m_englishButton[0].ShowWindow(SW_HIDE);//"wenn speziell"
	m_englishButton[1].ShowWindow(SW_HIDE);//"immer"
	m_englishButton[2].ShowWindow(SW_HIDE);//"nie"
	m_englishButton[3].ShowWindow(SW_HIDE);//"Hilfsverb"
	m_englishButton[4].ShowWindow(SW_HIDE);//"Aktionsverb"
	m_englishButton[5].ShowWindow(SW_HIDE);//"regelmäßig"
	m_englishButton[6].ShowWindow(SW_HIDE);//"regelmäßig"
	m_englishButton[7].ShowWindow(SW_HIDE);//"Adverb befindet sich zwischen Subjekt und Verb oder nach der finiten Form des Verbes 'to be'"
	m_englishButton[8].ShowWindow(SW_HIDE);//"Adverb bezieht sich auf ein Adjektiv oder Adverb"
	m_englishButton[9].ShowWindow(SW_HIDE);//"Adverb bezieht sich auf ganzen Satz und steht am Satzbeginn"
	m_englishButton[10].ShowWindow(SW_HIDE);//"im Deutschen: Plural"
	m_englishButton[11].ShowWindow(SW_HIDE);//"im Deutschen: Singular"
	m_englishButton[12].ShowWindow(SW_HIDE);//"Adjektiv darf nur nach einem Verb benutzt werden"
	m_englishButton[13].ShowWindow(SW_HIDE);//"Adjektiv darf nur vor einem Substantiv benutzt werden"
	m_englishButton[14].ShowWindow(SW_HIDE);//"Adverb bezieht sich auf ganzen Satz und steht am Satzende"
	m_englishButton[15].ShowWindow(SW_HIDE);//"Adverb ist ein Fragewort"
	m_englishButton[16].ShowWindow(SW_HIDE);//"Adverb der Zeit"
	m_englishEdit[1].ShowWindow(SW_HIDE);//Plural/Vergangenheit/Komperativ
	m_englishEdit[2].ShowWindow(SW_HIDE);//Perfekt/Superlativ
	m_englishEdit[3].ShowWindow(SW_HIDE);//???
	m_englishStatic[3].ShowWindow(SW_HIDE);//"Singular"/"Präposition"
	m_englishStatic[4].ShowWindow(SW_HIDE);//"Plural"/"Präteritum"
	m_englishStatic[5].ShowWindow(SW_HIDE);//"Perfekt"/
	m_germanButton[0].ShowWindow(SW_HIDE);//"reflexiv"
	m_germanButton[1].ShowWindow(SW_HIDE);//"Bewegung"
	m_germanButton[2].ShowWindow(SW_HIDE);//"regelmäßig"
	m_germanButton[3].ShowWindow(SW_HIDE);//"regelmäßig"
	m_germanButton[4].ShowWindow(SW_HIDE);//"regelmäßig"
	m_germanButton[5].ShowWindow(SW_HIDE);//"kein Fall"
	m_germanButton[6].ShowWindow(SW_HIDE);//"3. Fall"
	m_germanButton[7].ShowWindow(SW_HIDE);//"4. Fall"
	m_germanButton[8].ShowWindow(SW_HIDE);//"~n"
	m_germanButton[9].ShowWindow(SW_HIDE);//"~e"
	m_germanButton[10].ShowWindow(SW_HIDE);//"~en"
	m_germanButton[11].ShowWindow(SW_SHOW);//"Subjekt Prädikat [Objekt]"
	m_germanButton[12].ShowWindow(SW_SHOW);//"Subjekt [Objekt] Prädikat"
  m_germanEdit[1].ShowWindow(SW_HIDE);
  m_germanEdit[2].ShowWindow(SW_HIDE);
  m_germanEdit[3].ShowWindow(SW_HIDE);
  m_germanEdit[4].ShowWindow(SW_HIDE);
  m_germanEdit[5].ShowWindow(SW_HIDE);
  m_germanEdit[6].ShowWindow(SW_HIDE);
  m_germanEdit[7].ShowWindow(SW_HIDE);
  m_germanEdit[8].ShowWindow(SW_HIDE);
  m_germanEdit[9].ShowWindow(SW_HIDE);
  m_germanEdit[10].ShowWindow(SW_HIDE);
  m_germanEdit[11].ShowWindow(SW_HIDE);
  m_germanEdit[12].ShowWindow(SW_HIDE);
  m_germanEdit[13].ShowWindow(SW_HIDE);
  m_germanEdit[14].ShowWindow(SW_HIDE);
  m_germanEdit[15].ShowWindow(SW_HIDE);
  m_germanEdit[16].ShowWindow(SW_HIDE);
  m_germanEdit[17].ShowWindow(SW_HIDE);
  m_germanEdit[18].ShowWindow(SW_HIDE);
  m_germanEdit[19].ShowWindow(SW_HIDE);
	m_germanListBox[0].ShowWindow(SW_HIDE);
	m_germanListBox[1].ShowWindow(SW_HIDE);
  m_germanStatic[3].ShowWindow(SW_SHOW);//"Singular"
  m_germanStatic[4].ShowWindow(SW_HIDE);//"Präposition"
  m_germanStatic[5].ShowWindow(SW_HIDE);//"ich"
  m_germanStatic[6].ShowWindow(SW_HIDE);//"du"
  m_germanStatic[7].ShowWindow(SW_HIDE);//"er"
  m_germanStatic[8].ShowWindow(SW_HIDE);//"wir"
  m_germanStatic[9].ShowWindow(SW_HIDE);//"ihr"
  m_germanStatic[10].ShowWindow(SW_HIDE);//"sie"
  m_germanStatic[11].ShowWindow(SW_HIDE);//"Plural"
  m_germanStatic[12].ShowWindow(SW_HIDE);//"Wortstamm"
  m_germanStatic[13].ShowWindow(SW_HIDE);//"Konjunktiv"
  m_germanStatic[14].ShowWindow(SW_HIDE);//"2. P. Sing."
  m_germanStatic[15].ShowWindow(SW_HIDE);//"2. P. Pl."
}

void CVocableEditorBar::NounControls()
{//bei einem Laufzeitfehler dieses Computerprogramms durch Aufruf dieser
	//Funktion kann es sein, dass auf nicht-existente Steuerelemente 
	//zugegriffen wird

	// die Positionen der Steuerelemente müssen zur Laufzeit berechnet werden,
	// da sie unterschiedlich groß sein können (z.B. kann 'große Schriftarten'
	// aktiviert sein (Start|Einstellungen|Systemsteuerung|Anzeige|
	// Registerkarte Einstellungen|Weitere Optionen...) )
	DWORD dwHorizontalOffset=0;
	DWORD dwVerticalOffset=0;
	DWORD dwAmount=0;
	DWORD dwVerticalStartOfGermanControls=0;
	CString str;
	CDC * pDC=GetDC();
	CSize size;
	CFont * pOldFont=pDC->SelectObject(m_englishStatic[0].GetFont());
	TEXTMETRIC textmetric;
#ifdef _DEBUG
	printf("void CVocableEditorBar::NounControls() ANFANG\n");
	fflush(stdout);
#endif
	//die Tabulator-Reihenfolge legt man beim Erstellen der Steuerelemente fest
	//(Die zeitliche Reihenfolge, in der man die Steuerelemente erstellt, ist 
	//zugleich die Tabulatorreihenfolge)
	//da die Steuerelemente bereits existieren, müssen sie zunächst zerstört und
	//dann neu erstellt werden
	/*m_englishButton[5].DestroyWindow();
	m_englishComboBox.DestroyWindow();
	m_englishEdit[0].DestroyWindow();
	m_englishEdit[1].DestroyWindow();
	m_englishEdit[2].DestroyWindow();
	m_germanComboBox.DestroyWindow();*/
	
	for(BYTE bRun=0;bRun<19;bRun++)
		m_englishButton[bRun].DestroyWindow();
	//m_englishWordKind.DestroyWindow();
	m_englishComboBox.DestroyWindow();
	m_germanComboBox.DestroyWindow();
	for(bRun=0;bRun<4;bRun++)
		m_germanListBox[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_englishEdit[bRun].DestroyWindow();
	m_englishStatic[5].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanButton[bRun].DestroyWindow();
	for(bRun=5;bRun<14;bRun++)
		m_germanStatic[bRun].DestroyWindow();
	//for(bRun=0;bRun<20;bRun++)
	//	m_germanStatic[bRun].DestroyWindow();
	m_english.DestroyWindow();
	m_englishStatic[5].DestroyWindow();
	m_wordkind.DestroyWindow();
#ifdef _DEBUG
	printf("nach dem Zerstören der Steuerelemente\n");
	fflush(stdout);
#endif
	//for(bRun=0;bRun<10;bRun++)
	//	m_englishStatic[bRun].DestroyWindow();
	//m_englishComboBox.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST,
	//	CRect(0,0,0,0), this,ID_USER+11);
	//m_englishWordKind.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST,
	//	CRect(0,0,0,0), this,IDC_ENGLISH_WORDKIND_COMBO);
	m_englishButton[0].Create(_T("wenn speziell"),WS_CHILD|WS_VISIBLE|
		BS_AUTORADIOBUTTON,CRect(0,0,0,0), this,ID_USER+1);
	m_englishButton[1].Create(_T("immer"),WS_CHILD|WS_VISIBLE|
		BS_AUTORADIOBUTTON,CRect(0,0,0,0), this,ID_USER+1);
	m_englishButton[2].Create(_T("nie"),WS_CHILD|WS_VISIBLE|
		BS_AUTORADIOBUTTON,CRect(0,0,0,0), this,ID_USER+1);
	m_englishButton[3].Create(_T("zählbar"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+2);
	m_englishButton[4].Create(_T("GESPROCHENE Form beginnt mit einem Vokal"),
		WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+3);
	m_englishEdit[0].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+12); // Singular/Positiv
	m_englishButton[10].Create(_T("im Deutschen: Plural"),WS_CHILD|
		WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,CRect(0,0,0,0),this,ID_USER+7);
	m_englishEdit[1].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+13); // Plural/Komperativ
	m_englishButton[5].Create(_T("regelmäßig"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+4);
	m_englishButton[11].Create(_T("im Deutschen: Singular"),WS_CHILD|
		WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,CRect(0,0,0,0),this,ID_USER+8);
	m_germanListBox[0].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+54);
	m_germanEdit[0].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+34); // Singular
	m_germanEdit[1].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+35); // Plural
	//Plural bei Ableitung vom Singular Beispiel: "Teller" ("~") -> "Teller" ("~")
	m_germanButton[0].Create(_T("~"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,
		CRect(0,0,0,0), this,ID_USER+22);
	//Plural bei Ableitung vom Singular Beispiel: "Tier" ("~") -> "Tiere" ("~e")
	m_germanButton[1].Create(_T("~e"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,
		CRect(0,0,0,0), this,ID_USER+23);
	//Plural bei Ableitung vom Singular Beispiel: "Frau" ("~") -> "Frauen" ("~en")
	//"Tasse" ("~") -> "Tassen" ("~n") //wenn der letzte Buchstabe ein "e" ist,
	//wird lediglich ein "n" angehängt, sonst "en"
	m_germanButton[2].Create(_T("~en/~n"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,
		CRect(0,0,0,0), this,ID_USER+24);
	//Plural bei Ableitung vom Singular Beispiel: "Weib" ("~") -> "Weiber" ("~er")
	m_germanButton[3].Create(_T("~er"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,
		CRect(0,0,0,0), this,ID_USER+25);
#ifdef _DEBUG
	printf("nach dem Erstellen der Steuerelemente\n");
	fflush(stdout);
#endif
	//m_englishButton[0].SetCheck(1);
	//m_englishWordKind.AddString(_T("Substantiv"));
	//m_englishWordKind.AddString(_T("Verb"));
	//m_englishWordKind.AddString(_T("Adjektiv"));
	//m_englishWordKind.AddString(_T("Adverb"));
	//m_englishWordKind.AddString(_T("Präposition"));
	//m_englishWordKind.AddString(_T("Pronomen"));
	//m_englishWordKind.AddString(_T("Konjunktion"));
	//m_englishWordKind.SetCurSel(0);

	m_germanListBox[0].AddString(_T("der"));
	m_germanListBox[0].AddString(_T("die"));
	m_germanListBox[0].AddString(_T("das"));
#ifdef _DEBUG
	printf("nach SetDialogItemText\n");
	fflush(stdout);
#endif
	m_englishButton[0].SetDlgItemText(ID_USER+12,"hhhh");
	//SetDlgItemText(ID_USER+12,m_strAr[0]); //englischer Plural
	//SetDlgItemText(ID_USER+13,m_strAr[1]); //englischer Plural
	//SetDlgItemText(ID_USER+34,m_strAr[2]); //deutscher Singular
	//SetDlgItemText(ID_USER+35,m_strAr[3]); //deutscher Plural
	//wann steht ein englischer Artikel vor dem Substantiv?
	m_englishButton[0].SetCheck(m_bAr[0]);		
	m_englishButton[1].SetCheck(m_bAr[1]);
	m_englishButton[2].SetCheck(m_bAr[2]);
	m_englishButton[3].SetCheck(m_bAr[11]);
	m_englishButton[10].SetCheck(m_bAr[3]); //"im Deutschen: Plural"
	m_englishButton[11].SetCheck(m_bAr[4]); //"im Deutschen: Singular"
	m_englishButton[5].SetCheck(m_bAr[5]); //"regelmäßig"
	m_germanListBox[0].SetCurSel(m_bAr[6]); //der deutsche Artikel
	m_germanButton[0].SetCheck(m_bAr[7]); //Kontrollkästchen "~"
	m_germanButton[1].SetCheck(m_bAr[8]); //Kontrollkästchen "~e"
	m_germanButton[2].SetCheck(m_bAr[9]); //Kontrollkästchen "~en/n"
	m_germanButton[3].SetCheck(m_bAr[10]); //Kontrollkästchen "~er"
#ifdef _DEBUG
	printf("vor dem Zuweisen der Schriftart\n");
	fflush(stdout);
#endif
	//for(int i=0;i<18;i++)
		//m_englishButton[i].SetFont(&m_font);
	m_englishButton[0].SetFont(&m_font);
	m_englishButton[1].SetFont(&m_font);
	m_englishButton[2].SetFont(&m_font);
	m_englishButton[3].SetFont(&m_font);
	m_englishButton[4].SetFont(&m_font);
	m_englishButton[5].SetFont(&m_font);
	m_englishButton[10].SetFont(&m_font);
	m_englishButton[11].SetFont(&m_font);
	//for(i=0;i<4;i++)
	//	m_englishEdit[i].SetFont(&m_font);
	m_englishEdit[0].SetFont(&m_font);
	m_englishEdit[1].SetFont(&m_font);
	m_englishWordKind.SetFont(&m_font);
	for(int i=0;i<4;i++)
		m_germanButton[i].SetFont(&m_font);
	for(bRun=0;bRun<2;bRun++)
		m_germanEdit[bRun].SetFont(&m_font);
	m_germanListBox[0].SetFont(&m_font);
#ifdef _DEBUG
	printf("nach dem Zuweisen der Schriftart\n");
	fflush(stdout);
#endif
	//m_englishWordKind.SetFocus();	
	//m_englishWordKind.MoveWindow(CRect(50,20,130,150));

	pDC->GetTextMetrics(&textmetric);
	SetDlgItemText(ID_USER+18,_T("Artikel"));
	SetDlgItemText(ID_USER+19,_T("Singular"));
	SetDlgItemText(ID_USER+20,_T("Plural"));
	SetDlgItemText(ID_USER+58,_T("Artikel"));
	SetDlgItemText(ID_USER+59,_T("Singular"));
	SetDlgItemText(ID_USER+60,_T("Plural"));
	
	GetDlgItemText(m_englishStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[0].MoveWindow(CRect(0,0,size.cx,textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+5;
	
	GetDlgItemText(m_englishStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	
	dwAmount=0;
	for(int dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	m_englishWordKind.MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+dwAmount+30,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=dwAmount+5+30;
	dwVerticalOffset=0;

	GetDlgItemText(m_englishStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[2].MoveWindow(CRect(dwHorizontalOffset,0,dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	dwAmount=size.cx;

	GetDlgItemText(m_englishButton[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	if(size.cx>dwAmount)
		dwAmount=size.cx;
	
	GetDlgItemText(m_englishButton[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	if(size.cx>dwAmount)
		dwAmount=size.cx;

	GetDlgItemText(m_englishButton[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[3].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+
		textmetric.tmHeight));
	dwHorizontalOffset+=dwAmount+20+5;

	GetDlgItemText(m_englishButton[4].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[4].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+
		textmetric.tmHeight));

	GetDlgItemText(m_englishStatic[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[3].MoveWindow(CRect(dwHorizontalOffset,0,dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[10].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight*2;

	m_englishButton[10].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+20+5;

	GetDlgItemText(m_englishStatic[4].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[4].MoveWindow(CRect(dwHorizontalOffset,0,dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[11].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishEdit[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[5].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[5].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[11].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[11].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight*3;
	dwVerticalStartOfGermanControls=dwVerticalOffset;

	GetDlgItemText(m_germanStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[0].MoveWindow(CRect(0,dwVerticalOffset,size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset=size.cx+5;

	GetDlgItemText(m_germanStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwAmount=0;
	for(dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	dwHorizontalOffset+=dwAmount+30+5;

	GetDlgItemText(m_germanStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	dwAmount=0;
	for(dwRun=0;dwRun<m_germanListBox[0].GetCount();dwRun++)
	{
		m_germanListBox[0].GetText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	m_germanListBox[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+dwAmount+5,dwVerticalOffset+(textmetric.tmHeight*3)));
	dwVerticalOffset+=textmetric.tmHeight;
	GetDlgItemText(m_germanStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	dwHorizontalOffset+=size.cx+5;

	GetDlgItemText(m_germanStatic[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[3].MoveWindow(CRect(dwHorizontalOffset,dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;
	
	if(size.cx>80)
	{
		m_germanEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
		dwHorizontalOffset+=size.cx+5;
	}
	else
	{
		m_germanEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+80,dwVerticalOffset+textmetric.tmHeight));
		dwHorizontalOffset+=85;
	}

	GetDlgItemText(m_germanStatic[4].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[4].MoveWindow(CRect(dwHorizontalOffset,dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	if(size.cx>80)
		m_germanEdit[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	else
		m_germanEdit[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+80,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_germanButton[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanButton[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_germanButton[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanButton[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_germanButton[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanButton[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_germanButton[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanButton[3].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));

	GetDlgItemText(m_englishButton[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);

/*	m_englishButton[0].ShowWindow(TRUE);
	m_englishButton[1].ShowWindow(TRUE);
	m_englishButton[2].ShowWindow(TRUE);
	m_englishButton[3].ShowWindow(FALSE);
	m_englishButton[4].ShowWindow(FALSE);
	m_englishButton[5].ShowWindow(TRUE);
	m_englishButton[6].ShowWindow(FALSE);
	m_englishButton[7].ShowWindow(FALSE);
	m_englishButton[8].ShowWindow(FALSE);
	m_englishButton[9].ShowWindow(FALSE);
	m_englishButton[10].ShowWindow(TRUE);
	m_englishButton[0].ShowWindow(TRUE);
	m_englishButton[11].ShowWindow(TRUE);
	m_englishButton[12].ShowWindow(FALSE);
	m_englishButton[13].ShowWindow(FALSE);
	m_englishButton[14].ShowWindow(SW_HIDE);
	m_englishButton[15].ShowWindow(SW_HIDE);
	m_englishButton[16].ShowWindow(SW_HIDE);
	m_englishComboBox.ShowWindow(FALSE);
	m_englishEdit[1].ShowWindow(TRUE);
	m_englishEdit[2].ShowWindow(FALSE);
	m_englishEdit[3].ShowWindow(FALSE);
	m_englishStatic[3].ShowWindow(TRUE);
	m_englishStatic[4].ShowWindow(TRUE);
	m_englishStatic[5].ShowWindow(FALSE);
	m_germanButton[5].ShowWindow(FALSE);
	m_germanButton[6].ShowWindow(FALSE);
	m_germanButton[7].ShowWindow(FALSE);
	m_germanButton[4].ShowWindow(FALSE);
	m_germanButton[11].ShowWindow(SW_HIDE);//"Subjekt Prädikat [Objekt]"
	m_germanButton[12].ShowWindow(SW_HIDE);//"Subjekt [Objekt] Prädikat"
	m_germanComboBox.ShowWindow(FALSE);
	m_germanEdit[1].ShowWindow(TRUE);
	m_germanEdit[2].ShowWindow(FALSE);
	m_germanEdit[3].ShowWindow(FALSE);
	m_germanEdit[4].ShowWindow(FALSE);
	m_germanEdit[5].ShowWindow(FALSE);
	m_germanEdit[6].ShowWindow(FALSE);
	m_germanEdit[7].ShowWindow(FALSE);
	m_germanEdit[8].ShowWindow(FALSE);
	m_germanEdit[9].ShowWindow(FALSE);
	m_germanEdit[10].ShowWindow(FALSE);
	m_germanEdit[11].ShowWindow(FALSE);
	m_germanEdit[12].ShowWindow(FALSE);
	m_germanEdit[13].ShowWindow(FALSE);
	m_germanEdit[14].ShowWindow(FALSE);
	m_germanListBox[0].ShowWindow(TRUE);
	m_germanListBox[1].ShowWindow(FALSE);
	m_germanStatic[4].ShowWindow(TRUE);
	m_germanStatic[5].ShowWindow(FALSE);
	m_germanStatic[6].ShowWindow(FALSE);
	m_germanStatic[7].ShowWindow(FALSE);
	m_germanStatic[8].ShowWindow(FALSE);
	m_germanStatic[9].ShowWindow(FALSE);
	m_germanStatic[10].ShowWindow(FALSE);
	m_germanStatic[11].ShowWindow(FALSE);
	m_germanStatic[12].ShowWindow(FALSE);
	m_germanStatic[13].ShowWindow(FALSE);*/
#ifdef _DEBUG
	printf("void CVocableEditorBar::NounControls() ENDE\n");
	fflush(stdout);
#endif
}

void CVocableEditorBar::PrepositionControls()
{//bei einem Laufzeitfehler dieses Computerprogramms durch Aufruf dieser
	//Funktion kann es sein, dass auf nicht-existente Steuerelemente 
	//zugegriffen wird

	// die Positionen der Steuerelemente müssen zur Laufzeit berechnet werden,
	// da sie untzerschiedlich groß sein können (z.B. kann 'große Schriftarten'
	// aktiviert sein (Start|Einstellungen|Systemsteuerung|Anzeige|
	// Registerkarte Einstellungen|Weitere Optionen...) )
	DWORD dwHorizontalOffset=0;
	DWORD dwVerticalOffset=0;
	DWORD dwAmount=0;
	DWORD dwVerticalStartOfGermanControls=0;
	CString str;
	CDC * pDC=GetDC();
	CSize size;
	CFont * pOldFont=pDC->SelectObject(m_englishStatic[0].GetFont());
	TEXTMETRIC textmetric;
	pDC->GetTextMetrics(&textmetric);

	for(BYTE bRun=0;bRun<19;bRun++)
		m_englishButton[bRun].DestroyWindow();
	//m_englishWordKind.DestroyWindow();
	m_englishComboBox.DestroyWindow();
	m_germanComboBox.DestroyWindow();
	for(bRun=0;bRun<4;bRun++)
		m_germanListBox[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_englishEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanButton[bRun].DestroyWindow();
	//for(bRun=0;bRun<20;bRun++)
	//	m_germanStatic[bRun].DestroyWindow();
	m_english.DestroyWindow();
	m_englishStatic[5].DestroyWindow();
	m_wordkind.DestroyWindow();
	//for(bRun=0;bRun<10;bRun++)
	//	m_englishStatic[bRun].DestroyWindow();
	//m_englishWordKind.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST,
	//	CRect(0,0,0,0), this,IDC_ENGLISH_WORDKIND_COMBO);
	m_englishEdit[0].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+12); // Singular/Positiv
	m_germanEdit[0].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+34); // Positiv

	SetDlgItemText(ID_USER+12,m_strAr[33]); //englische Präposition
	SetDlgItemText(ID_USER+34,m_strAr[34]); //deutsche Präposition

	//for(int i=0;i<18;i++)
	//	m_englishButton[i].SetFont(&m_font);
	//m_englishComboBox.SetFont(&m_font);
	for(int i=0;i<1;i++)
		m_englishEdit[i].SetFont(&m_font);
	m_englishWordKind.SetFont(&m_font);
	//for(bRun=0;bRun<20;bRun++)
	//	m_germanButton[bRun].SetFont(&m_font);
	//m_germanComboBox.SetFont(&m_font);
	for(bRun=0;bRun<1;bRun++)
		m_germanEdit[bRun].SetFont(&m_font);

	//m_englishWordKind.SetFocus();	
	//m_englishWordKind.MoveWindow(CRect(50,20,130,150));

	// Zeichenketten zuerst zuweisen, damit die Breiten der Steuerelemente
	// richtig ermittelt werden
	SetDlgItemText(ID_USER+18,_T("Präposition"));
	SetDlgItemText(ID_USER+58,_T("Präposition"));

	GetDlgItemText(m_englishStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[0].MoveWindow(CRect(0,0,size.cx,textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+5;
	
	GetDlgItemText(m_englishStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	
	dwAmount=0;
	for(int dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	m_englishWordKind.MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+dwAmount+30,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=dwAmount+5+30;
	dwVerticalOffset=0;

	GetDlgItemText(m_englishStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[2].MoveWindow(CRect(dwHorizontalOffset,0,dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;

	m_englishEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalStartOfGermanControls=dwVerticalOffset+textmetric.tmHeight*2;

	GetDlgItemText(m_germanStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[0].MoveWindow(CRect(0,dwVerticalStartOfGermanControls,size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwHorizontalOffset=size.cx+5;

	GetDlgItemText(m_germanStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwAmount=0;
	for(dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	dwHorizontalOffset+=dwAmount+30+5;

	GetDlgItemText(m_germanStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	m_germanEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+90,dwVerticalOffset+textmetric.tmHeight));

	//m_englishButton[0].ShowWindow(FALSE);
	//m_englishButton[1].ShowWindow(FALSE);
	//m_englishButton[2].ShowWindow(FALSE);
	//m_englishButton[3].ShowWindow(FALSE);
	//m_englishButton[4].ShowWindow(FALSE);
	//m_englishButton[5].ShowWindow(FALSE);
	//m_englishButton[6].ShowWindow(FALSE);
	//m_englishButton[7].ShowWindow(FALSE);
	//m_englishButton[8].ShowWindow(FALSE);
	//m_englishButton[9].ShowWindow(FALSE); // im Deutschen: Plural
	//m_englishButton[10].ShowWindow(FALSE); // im Deutschen: Singular
	//m_englishButton[11].ShowWindow(FALSE);
	//m_englishButton[12].ShowWindow(FALSE);
	//m_englishButton[13].ShowWindow(FALSE);
	//m_englishButton[14].ShowWindow(SW_HIDE);
	//m_englishButton[15].ShowWindow(SW_HIDE);
	//m_englishButton[16].ShowWindow(SW_HIDE);

	//m_englishComboBox.ShowWindow(FALSE);
	//m_englishEdit[0].MoveWindow(CRect(140,20,220,35));
	//m_englishEdit[1].ShowWindow(FALSE);
	//m_englishEdit[2].ShowWindow(FALSE);
	//m_englishEdit[3].ShowWindow(FALSE);
	m_englishStatic[3].ShowWindow(FALSE);
	m_englishStatic[4].ShowWindow(FALSE);
	//m_englishStatic[5].ShowWindow(FALSE);
	//m_germanButton[0].ShowWindow(FALSE);
	//m_germanButton[1].ShowWindow(FALSE);
	//m_germanButton[2].ShowWindow(FALSE);
	//m_germanButton[3].ShowWindow(FALSE);
	//m_germanButton[4].ShowWindow(FALSE);
	//m_germanButton[5].ShowWindow(FALSE);
	//m_germanButton[6].ShowWindow(FALSE);
	//m_germanButton[7].ShowWindow(FALSE);
	//m_germanButton[11].ShowWindow(SW_HIDE);//"Subjekt Prädikat [Objekt]"
	//m_germanButton[12].ShowWindow(SW_HIDE);//"Subjekt [Objekt] Prädikat"
	//m_germanComboBox.ShowWindow(FALSE);
	//m_germanEdit[0].MoveWindow(CRect(140,107,220,122));//MoveWindow(CRect(140,105,220,120));
	//m_germanEdit[1].ShowWindow(FALSE);
	//m_germanEdit[2].ShowWindow(FALSE);
	//m_germanEdit[3].ShowWindow(FALSE);
	//m_germanEdit[4].ShowWindow(FALSE);
	//m_germanEdit[5].ShowWindow(FALSE);
	//m_germanEdit[6].ShowWindow(FALSE);
	//m_germanEdit[7].ShowWindow(FALSE);
	//m_germanEdit[8].ShowWindow(FALSE);
	//m_germanEdit[9].ShowWindow(FALSE);
	//m_germanEdit[10].ShowWindow(FALSE);
	//m_germanEdit[11].ShowWindow(FALSE);
	//m_germanEdit[12].ShowWindow(FALSE);
	//m_germanEdit[13].ShowWindow(FALSE);
	//m_germanEdit[14].ShowWindow(FALSE);
	//m_germanListBox[0].ShowWindow(FALSE);
	//m_germanListBox[1].ShowWindow(FALSE);
	//m_germanStatic[0].MoveWindow(CRect(0,90,40,105));
	//m_germanStatic[1].MoveWindow(CRect(50,90,130,105));
	//m_germanStatic[2].MoveWindow(CRect(140,90,220,105));
	//m_germanStatic[3].ShowWindow(FALSE);
	//m_germanStatic[4].ShowWindow(FALSE);
	//m_germanStatic[5].ShowWindow(FALSE);
	//m_germanStatic[6].ShowWindow(FALSE);
	//m_germanStatic[7].ShowWindow(FALSE);
	//m_germanStatic[8].ShowWindow(FALSE);
	//m_germanStatic[9].ShowWindow(FALSE);
	//m_germanStatic[10].ShowWindow(FALSE);
	//m_germanStatic[11].ShowWindow(FALSE);
	//m_germanStatic[12].ShowWindow(FALSE);
	//m_germanStatic[13].ShowWindow(FALSE);
	//m_germanStatic[4].ShowWindow(FALSE);
	//m_germanStatic[5].ShowWindow(FALSE);
}

void CVocableEditorBar::PronounControls()
{//bei einem Laufzeitfehler dieses Computerprogramms durch Aufruf dieser
	//Funktion kann es sein, dass auf nicht-existente Steuerelemente 
	//zugegriffen wird

	// die Positionen der Steuerelemente müssen zur Laufzeit berechnet werden,
	// da sie untzerschiedlich groß sein können (z.B. kann 'große Schriftarten'
	// aktiviert sein (Start|Einstellungen|Systemsteuerung|Anzeige|
	// Registerkarte Einstellungen|Weitere Optionen...) )
	DWORD dwHorizontalOffset=0;
	DWORD dwVerticalOffset=0;
	DWORD dwAmount=0;
	DWORD dwVerticalStartOfGermanControls=0;
	CString str;
	CDC * pDC=GetDC();
	CSize size;
	CFont * pOldFont=pDC->SelectObject(m_englishStatic[0].GetFont());
	TEXTMETRIC textmetric;
	pDC->GetTextMetrics(&textmetric);

	for(BYTE bRun=0;bRun<19;bRun++)
		m_englishButton[bRun].DestroyWindow();
	//m_englishWordKind.DestroyWindow();
	m_englishComboBox.DestroyWindow();
	m_germanComboBox.DestroyWindow();
	for(bRun=0;bRun<4;bRun++)
		m_germanListBox[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_englishEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanButton[bRun].DestroyWindow();
	//for(bRun=0;bRun<20;bRun++)
	//	m_germanStatic[bRun].DestroyWindow();
	m_english.DestroyWindow();
	m_englishStatic[5].DestroyWindow();
	m_wordkind.DestroyWindow();
	//for(bRun=0;bRun<10;bRun++)
	//	m_englishStatic[bRun].DestroyWindow();
	//m_englishWordKind.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST,
	//	CRect(0,0,0,0), this,IDC_ENGLISH_WORDKIND_COMBO);
	m_englishEdit[0].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+12); // Singular/Positiv
	m_englishEdit[1].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+13); // Singular/Positiv
	m_germanEdit[0].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+34); // Positiv

/*	m_englishWordKind.AddString(_T("Substantiv"));
	m_englishWordKind.AddString(_T("Verb"));
	m_englishWordKind.AddString(_T("Adjektiv"));
	m_englishWordKind.AddString(_T("Adverb"));
	m_englishWordKind.AddString(_T("Präposition"));
	m_englishWordKind.AddString(_T("Pronomen"));
	m_englishWordKind.AddString(_T("Konjunktion"));
	m_englishWordKind.SetCurSel(1); //1 für "Verb"*/

	SetDlgItemText(ID_USER+12,m_strAr[35]); //englisches Pronom für den Singular
	SetDlgItemText(ID_USER+13,m_strAr[36]); //englisches Pronom für den Singular
	//deutsches Pronom (für Singular und Plural)
	SetDlgItemText(ID_USER+34,m_strAr[37]);

	for(int i=0;i<18;i++)
		m_englishButton[i].SetFont(&m_font);
	m_englishComboBox.SetFont(&m_font);
	for(i=0;i<4;i++)
		m_englishEdit[i].SetFont(&m_font);
	m_englishWordKind.SetFont(&m_font);
	for(bRun=0;bRun<20;bRun++)
		m_germanButton[bRun].SetFont(&m_font);
	m_germanComboBox.SetFont(&m_font);
	for(bRun=0;bRun<20;bRun++)
		m_germanEdit[bRun].SetFont(&m_font);

	//m_englishWordKind.SetFocus();	
	//m_englishWordKind.MoveWindow(CRect(50,20,130,150));

	// Zeichenketten zuerst zuweisen, damit die Breiten der Steuerelemente
	// richtig ermittelt werden
	SetDlgItemText(ID_USER+18,_T("Singular"));
	SetDlgItemText(ID_USER+19,_T("Plural"));
	SetDlgItemText(ID_USER+58,_T("Pronomen"));

	GetDlgItemText(m_englishStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[0].MoveWindow(CRect(0,0,size.cx,textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+5;
	
	GetDlgItemText(m_englishStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	
	dwAmount=0;
	for(int dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	m_englishWordKind.MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+dwAmount+30,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=dwAmount+5+30;
	dwVerticalOffset=0;

	GetDlgItemText(m_englishStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[2].MoveWindow(CRect(dwHorizontalOffset,0,dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;

	m_englishEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+90,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=95;
	
	GetDlgItemText(m_englishStatic[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[3].MoveWindow(CRect(dwHorizontalOffset,0,dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;

	m_englishEdit[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+90,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalStartOfGermanControls=dwVerticalOffset+textmetric.tmHeight*2;

	GetDlgItemText(m_germanStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[0].MoveWindow(CRect(0,dwVerticalStartOfGermanControls,size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwHorizontalOffset=size.cx+5;

	GetDlgItemText(m_germanStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwAmount=0;
	for(dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	dwHorizontalOffset+=dwAmount+30+5;

	GetDlgItemText(m_germanStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	m_germanEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+90,dwVerticalOffset+textmetric.tmHeight));

	m_englishButton[0].ShowWindow(FALSE);
	m_englishButton[1].ShowWindow(FALSE);
	m_englishButton[2].ShowWindow(FALSE);
	m_englishButton[3].ShowWindow(FALSE);
	m_englishButton[4].ShowWindow(FALSE);
	m_englishButton[5].ShowWindow(FALSE);
	m_englishButton[6].ShowWindow(FALSE);
	m_englishButton[7].ShowWindow(FALSE);
	m_englishButton[8].ShowWindow(FALSE);
	m_englishButton[9].ShowWindow(FALSE); // im Deutschen: Plural
	m_englishButton[10].ShowWindow(FALSE); // im Deutschen: Singular
	m_englishButton[11].ShowWindow(FALSE);
	m_englishButton[12].ShowWindow(FALSE);
	m_englishButton[13].ShowWindow(FALSE);
	m_englishButton[14].ShowWindow(SW_HIDE);
	m_englishButton[15].ShowWindow(SW_HIDE);
	m_englishButton[16].ShowWindow(SW_HIDE);
	m_englishComboBox.ShowWindow(FALSE);
	//m_englishEdit[0].MoveWindow(CRect(140,20,220,35));
	//m_englishEdit[1].MoveWindow(CRect(225,20,305,35));
	m_englishEdit[1].ShowWindow(TRUE);
	m_englishEdit[2].ShowWindow(FALSE);
	m_englishEdit[3].ShowWindow(FALSE);
	m_englishStatic[3].ShowWindow(TRUE);
	m_englishStatic[4].ShowWindow(FALSE);
	m_englishStatic[5].ShowWindow(FALSE);
	m_germanButton[0].ShowWindow(FALSE);
	m_germanButton[1].ShowWindow(FALSE);
	m_germanButton[2].ShowWindow(FALSE);
	m_germanButton[3].ShowWindow(FALSE);
	m_germanButton[4].ShowWindow(FALSE);
	m_germanButton[5].ShowWindow(FALSE);
	m_germanButton[6].ShowWindow(FALSE);
	m_germanButton[7].ShowWindow(FALSE);
	m_germanButton[11].ShowWindow(SW_HIDE);//"Subjekt Prädikat [Objekt]"
	m_germanButton[12].ShowWindow(SW_HIDE);//"Subjekt [Objekt] Prädikat"
	m_germanComboBox.ShowWindow(FALSE);
	//m_germanEdit[0].MoveWindow(CRect(140,107,220,122));//MoveWindow(CRect(140,105,220,120));
	m_germanEdit[1].ShowWindow(FALSE);
	m_germanEdit[2].ShowWindow(FALSE);
	m_germanEdit[3].ShowWindow(FALSE);
	m_germanEdit[4].ShowWindow(FALSE);
	m_germanEdit[5].ShowWindow(FALSE);
	m_germanEdit[6].ShowWindow(FALSE);
	m_germanEdit[7].ShowWindow(FALSE);
	m_germanEdit[8].ShowWindow(FALSE);
	m_germanEdit[9].ShowWindow(FALSE);
	m_germanEdit[10].ShowWindow(FALSE);
	m_germanEdit[11].ShowWindow(FALSE);
	m_germanEdit[12].ShowWindow(FALSE);
	m_germanEdit[13].ShowWindow(FALSE);
	m_germanEdit[14].ShowWindow(FALSE);
	m_germanListBox[0].ShowWindow(FALSE);
	m_germanListBox[1].ShowWindow(FALSE);
	//m_germanStatic[0].MoveWindow(CRect(0,90,40,105));
	//m_germanStatic[1].MoveWindow(CRect(50,90,130,105));
	//m_germanStatic[2].MoveWindow(CRect(140,90,220,105));
	m_germanStatic[3].ShowWindow(FALSE);
	m_germanStatic[4].ShowWindow(FALSE);
	m_germanStatic[5].ShowWindow(FALSE);
	m_germanStatic[6].ShowWindow(FALSE);
	m_germanStatic[7].ShowWindow(FALSE);
	m_germanStatic[8].ShowWindow(FALSE);
	m_germanStatic[9].ShowWindow(FALSE);
	m_germanStatic[10].ShowWindow(FALSE);
	m_germanStatic[11].ShowWindow(FALSE);
	m_germanStatic[12].ShowWindow(FALSE);
	m_germanStatic[13].ShowWindow(FALSE);
}

void CVocableEditorBar::VerbControls(BYTE mode)
{//bei einem Laufzeitfehler dieses Computerprogramms durch Aufruf dieser
	//Funktion kann es sein, dass auf nicht-existente Steuerelemente 
	//zugegriffen wird

	// die Positionen der Steuerelemente müssen zur Laufzeit berechnet werden,
	// da sie unterschiedlich groß sein können (z.B. kann 'große Schriftarten'
	// aktiviert sein (Start|Einstellungen|Systemsteuerung|Anzeige|
	// Registerkarte Einstellungen|Weitere Optionen...) )
	DWORD dwHorizontalOffset=0;
	DWORD dwVerticalOffset=0;
	DWORD dwAmount=0;
	DWORD dwVerticalStartOfGermanControls=0;
	CString str;
	CDC * pDC=GetDC();
	CSize size;
	CFont * pOldFont=pDC->SelectObject(m_englishStatic[0].GetFont());
	TEXTMETRIC textmetric;
	pDC->GetTextMetrics(&textmetric);
#ifdef _DEBUG
	printf("void CVocableEditorBar::VerbControls(BYTE mode) - ANFANG\n");
#endif

	for(BYTE bRun=0;bRun<19;bRun++)
		m_englishButton[bRun].DestroyWindow();
	//m_englishWordKind.ResetContent();
	//m_englishWordKind.DestroyWindow();
	m_englishComboBox.DestroyWindow();
	m_englishStatic[5].DestroyWindow();
	m_germanComboBox.DestroyWindow();
	for(bRun=0;bRun<4;bRun++)
		m_germanListBox[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_englishEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanEdit[bRun].DestroyWindow();
	for(bRun=0;bRun<20;bRun++)
		m_germanButton[bRun].DestroyWindow();
	for(bRun=5;bRun<14;bRun++)
		m_germanStatic[bRun].DestroyWindow();
	m_english.DestroyWindow();
	m_wordkind.DestroyWindow();
	//for(bRun=0;bRun<10;bRun++)
	//	m_englishStatic[bRun].DestroyWindow();
#ifdef _DEBUG
	printf("Erstellung der Steuerelemente\n");
#endif
#ifdef _DEBUG
	printf("Erstellung der Steuerelemente\n");
	MINMAXINFO FAR* lpMMI=new MINMAXINFO;
	/*m_englishButton[3].OnGetMinMaxInfo(lpMMI);
	printf("lpMMI->ptReserved.x: %u lpMMI->ptReserved.y: %u\
    lpMMI->ptMaxSize.x: %u lpMMI->ptMaxSize.y: %u\
    lpMMI->ptMaxPosition.x: %u lpMMI->ptMaxPosition.y: %u\
    lpMMI->ptMinTrackSize.x: %u lpMMI->ptMinTrackSize.y: %u\
    lpMMI->ptMaxTrackSize.x: %u lpMMI->ptMaxTrackSize.y: %u\n",
		lpMMI->ptReserved.x,lpMMI->ptReserved.y,lpMMI->ptMaxSize.x,
		lpMMI->ptMaxSize.y,lpMMI->ptMaxPosition.x,lpMMI->ptMaxPosition.y,
    lpMMI->ptMinTrackSize.x,lpMMI->ptMinTrackSize.y,
		lpMMI->ptMaxTrackSize.x,lpMMI->ptMaxTrackSize.y);*/
#endif
	m_englishStatic[5].Create(_T("Perfekt"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+21);

	//die Tab-Reihenfolge ist die zeitliche Reihenfolge der Erstellung der 
	//Steuerelemente, die "WS_TABSTOP" haben
	m_germanStatic[5].Create(_T("ich"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+61); // Perfekt/Wortstamm
	m_germanStatic[6].Create(_T("du"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+62);
	m_germanStatic[7].Create(_T("er"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+63);
	m_germanStatic[8].Create(_T("wir"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+64);
	m_germanStatic[9].Create(_T("ihr"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+65);
	m_germanStatic[10].Create(_T("sie"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+66);
	m_germanStatic[11].Create(_T("Plural"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+67);
	m_germanStatic[12].Create(_T("Wortstamm"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+68); // Perfekt/Wortstamm
	m_germanStatic[13].Create(_T("Konjunktiv"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+69);
	//m_englishWordKind.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST,
	//	CRect(0,0,0,0), this,IDC_ENGLISH_WORDKIND_COMBO);
	m_englishEdit[0].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+12); // Singular/Positiv
	m_englishButton[3].Create(_T("modales Hilfsverb"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+2);
	m_englishButton[4].Create(_T("dynamisch"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+3);
	m_englishButton[7].Create(_T("to + Infinitiv"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+6);
	m_englishButton[8].Create(_T("ing-Form"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+7);
	m_englishComboBox.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST|
		WS_VSCROLL,CRect(0,0,0,0), this,ID_USER+11);
	m_englishEdit[1].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+13); // Plural/Komperativ
	m_englishButton[5].Create(_T("regelmäßig"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+4);
	m_englishEdit[2].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+14); // Superlativ
	m_englishButton[6].Create(_T("regelmäßig"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+5);
	m_germanEdit[0].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+34); // Singular/Infinitiv/Positiv/Wort/Pronomen
	m_germanButton[0].Create(_T("reflexiv"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+22);
	m_germanButton[1].Create(_T("Bewegung"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+23);
	m_germanButton[5].Create(_T("kein Fall"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+27);
	m_germanButton[6].Create(_T("3. Fall"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+28);
	m_germanButton[7].Create(_T("4. Fall"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+29);
	m_germanComboBox.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWNLIST|
		WS_VSCROLL,CRect(0,0,0,0), this,ID_USER+33);
	m_germanEdit[1].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+35); // Plural/ich Präsens
	m_germanEdit[2].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+36); // Komperativ/du Präsens
	m_germanEdit[3].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+37); // er/sie/es Präsens
	m_germanEdit[4].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+38); // wir Präsens
	m_germanEdit[5].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+39); // ihr Präsens
	m_germanButton[2].Create(_T("Wortstamm"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+24);
	m_germanEdit[6].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+40); // "Wortstamm" Präsens
	m_germanEdit[7].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+41); // Präteritum/Wortstamm/ich
	m_germanEdit[8].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+42); // du Präteritum
	m_germanEdit[9].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+43); // er/sie/es Präteritum
	m_germanEdit[10].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+44); // wir Präteritum
	m_germanEdit[11].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+45); // ihr Präteritum
	m_germanButton[3].Create(_T("Wortstamm"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+25);
	m_germanEdit[15].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+49); // Wortstamm Präteritum
	m_germanEdit[13].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+47); // Partizip Perfekt
	m_germanButton[4].Create(_T("ge~"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+26);
	m_germanButton[8].Create(_T("ge~-2t"),WS_CHILD|WS_TABSTOP|WS_VISIBLE|
		BS_AUTOCHECKBOX,CRect(0,0,0,0), this,ID_USER+30);
	m_germanEdit[14].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
		CRect(0,0,0,0), this,ID_USER+48); // Konjunktiv
	m_germanListBox[1].Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE,
		CRect(0,0,0,0), this,ID_USER+55);

	//m_englishButton[0].SetCheck(1);
	//m_englishWordKind.AddString(_T("Substantiv"));
	//m_englishWordKind.AddString(_T("Verb"));
	//m_englishWordKind.AddString(_T("Adjektiv"));
	//m_englishWordKind.AddString(_T("Adverb"));
	//m_englishWordKind.AddString(_T("Präposition"));
	//m_englishWordKind.AddString(_T("Pronomen"));
	//m_englishWordKind.AddString(_T("Konjunktion"));
	//m_englishWordKind.SetCurSel(1); //1 für "Verb"

#ifdef _DEBUG
	printf("Wertzuweisung\n");
#endif
	SetDlgItemText(ID_USER+12,m_strAr[4]); //englischer Infinitiv
	SetDlgItemText(ID_USER+13,m_strAr[5]); //englisches Präteritum
	SetDlgItemText(ID_USER+14,m_strAr[6]); //englisches Partizip Perfekt
	SetDlgItemText(ID_USER+34,m_strAr[7]); //deutscher Infinitiv
	SetDlgItemText(ID_USER+35,m_strAr[8]); //deutscher Präsens 1. Person Singular
	SetDlgItemText(ID_USER+36,m_strAr[9]); //deutscher Präsens 2. Person Singular
	SetDlgItemText(ID_USER+37,m_strAr[10]); //deutscher Präsens 3. Person Singular
	SetDlgItemText(ID_USER+38,m_strAr[11]); //deutscher Präsens 1. Person Plural
	SetDlgItemText(ID_USER+39,m_strAr[12]); //deutscher Präsens 2. Person Plural
	SetDlgItemText(ID_USER+40,m_strAr[13]); //deutscher Präsens 3. Person Plural
	SetDlgItemText(ID_USER+41,m_strAr[14]); //deutscher Präteritum 1. Person Singular
	SetDlgItemText(ID_USER+42,m_strAr[15]); //deutscher Präteritum 2. Person Singular
	SetDlgItemText(ID_USER+43,m_strAr[16]); //deutscher Präteritum 3. Person Singular
	SetDlgItemText(ID_USER+44,m_strAr[17]); //deutscher Präteritum 1. Person Plural
	SetDlgItemText(ID_USER+45,m_strAr[18]); //deutscher Präteritum 2. Person Plural
	SetDlgItemText(ID_USER+46,m_strAr[19]); //deutscher Präteritum 3. Person Plural
	SetDlgItemText(ID_USER+49,m_strAr[20]); //deutscher Wortstamm für Präteritum 3. Person Plural
	SetDlgItemText(ID_USER+47,m_strAr[21]); //deutsches Partizip Perfekt
	SetDlgItemText(ID_USER+48,m_strAr[22]); //deutsches Konjunktiv
	m_englishButton[3].SetCheck(m_bAr[21]); //"Hilfsverb"
	m_englishButton[4].SetCheck(m_bAr[22]); //"Aktionsverb"
	m_englishButton[5].SetCheck(m_bAr[23]); //"regelmäßig"
	m_englishButton[6].SetCheck(m_bAr[24]); //2. Kontrollkästchen "regelmäßig"
	m_englishButton[7].SetCheck(m_bAr[34]); //Kontrollkästchen "to + Infinitiv"
	m_englishButton[8].SetCheck(m_bAr[35]); //Kontrollkästchen "ing-Form"
	m_germanButton[0].SetCheck(m_bAr[25]); //"reflexiv"
	m_germanButton[1].SetCheck(m_bAr[26]); //"Bewegung"
	m_germanButton[2].SetCheck(m_bAr[27]); //"kein Fall"
	m_germanButton[3].SetCheck(m_bAr[28]); //"3. Fall"
	m_germanButton[4].SetCheck(m_bAr[29]); //"4. Fall"
	m_germanButton[5].SetCheck(m_bAr[30]); //"regelmäßig"
	m_germanButton[6].SetCheck(m_bAr[31]); //"Wortstamm"
	m_germanButton[7].SetCheck(m_bAr[32]); //"ge~"
	m_germanButton[8].SetCheck(m_bAr[33]); //"ge-2~t"
	//Kombinationsfeld, das die englischen Präpositionen enthält
	m_englishComboBox.SetCurSel(m_nAr[0]);
	//Kombinationsfeld, das die deutschen Präpositionen enthält
	m_germanComboBox.SetCurSel(m_nAr[1]);

	m_englishComboBox.AddString(_T(""));
	m_germanComboBox.AddString(_T(""));
	//die Liste mit den Präpositionen muss jedes Mal, wenn die Wortart Verb
	//ausgewählt wird, neu aufgebaut werden, da vorher eventuell eine neue
	//Präposition hinzugefügt wurde
	if(wordList.m_pWordNodeFirst)
	{
		WordNode * pWordNodecurrent=wordList.m_pWordNodeFirst;
		while(pWordNodecurrent)
		{
			if(typeid(*pWordNodecurrent->m_pWord)==typeid(EnglishPreposition))
			{
				EnglishPreposition * ep=dynamic_cast<EnglishPreposition*>(
					pWordNodecurrent->m_pWord);
				m_englishComboBox.AddString(ep->m_strWord);
			}
			if(typeid(*pWordNodecurrent->m_pWord)==typeid(GermanPreposition))
			{
				GermanPreposition * gp=dynamic_cast<GermanPreposition*>(
					pWordNodecurrent->m_pWord);
				m_germanComboBox.AddString(gp->m_strWord);
			}
			pWordNodecurrent=pWordNodecurrent->m_pWordNodeNext;
		}
	}

	for(int i=3;i<9;i++)
		m_englishButton[i].SetFont(&m_font);
	m_englishComboBox.SetFont(&m_font);
	for(i=0;i<3;i++)
		m_englishEdit[i].SetFont(&m_font);
	m_englishStatic[5].SetFont(&m_font);
	m_englishWordKind.SetFont(&m_font);
	for(bRun=0;bRun<9;bRun++)
		m_germanButton[bRun].SetFont(&m_font);
	m_germanComboBox.SetFont(&m_font);
	for(bRun=0;bRun<6;bRun++)
		m_germanEdit[bRun].SetFont(&m_font);
	for(bRun=6;bRun<12;bRun++)
		m_germanEdit[bRun].SetFont(&m_font);
	for(bRun=13;bRun<16;bRun++)
		m_germanEdit[bRun].SetFont(&m_font);
	m_germanListBox[1].SetFont(&m_font);
	for(bRun=5;bRun<14;bRun++)
		m_germanStatic[bRun].SetFont(&m_font);

	m_germanListBox[1].AddString(_T("haben"));
	m_germanListBox[1].AddString(_T("sein"));
	m_germanListBox[1].SetCurSel(0);
//m_germanListBox[0].SetFont(&m_font);
	//m_germanListBox[0].AddString(_T("der"));
	//m_germanListBox[0].AddString(_T("die"));
	//m_germanListBox[0].AddString(_T("das"));
	//m_germanListBox[0].SetCurSel(0);

	// Zeichenketten zuerst zuweisen, damit die Breiten der Steuerelemente
	// richtig ermittelt werden
	SetDlgItemText(ID_USER+18,_T("Infinitiv"));
	SetDlgItemText(ID_USER+19,_T("Präposition"));
	SetDlgItemText(ID_USER+20,_T("Präteritum"));
	SetDlgItemText(ID_USER+21,_T("Perfekt"));
	SetDlgItemText(ID_USER+58,_T("Infinitiv"));
	SetDlgItemText(ID_USER+59,_T("Präposition"));
	SetDlgItemText(ID_USER+60,_T("Präsens"));
	SetDlgItemText(ID_USER+67,_T("Präteritum"));
	SetDlgItemText(ID_USER+68,_T("Perfekt"));

	m_germanStatic[4].ShowWindow(TRUE);

	GetDlgItemText(m_englishStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[0].MoveWindow(CRect(0,0,size.cx,textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+5;
	
	GetDlgItemText(m_englishStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	
	dwAmount=0;
	for(int dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	m_englishWordKind.MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+dwAmount+30,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=dwAmount+5+30;
	dwVerticalOffset=0;

	GetDlgItemText(m_englishStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[2].MoveWindow(CRect(dwHorizontalOffset,0,dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;

	dwAmount=size.cx;
	GetDlgItemText(m_englishButton[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	if(size.cx+20>dwAmount)
		dwAmount=size.cx+20;
	GetDlgItemText(m_englishButton[4].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	if(size.cx+20>dwAmount)
		dwAmount=size.cx+20;
	m_englishEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+dwAmount,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[3].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[4].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[4].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[7].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[7].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[8].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishButton[8].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=dwAmount+5;
	dwVerticalStartOfGermanControls=dwVerticalOffset+(textmetric.tmHeight*2);

	//Präpositionen im englischen Teil des Vokabeleditors (3. Spalte)
	GetDlgItemText(m_englishStatic[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[3].MoveWindow(CRect(dwHorizontalOffset,0,
		dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;

	dwAmount=0;
	for(dwRun=0;dwRun<m_englishComboBox.GetCount();dwRun++)
	{
		m_englishComboBox.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	GetDlgItemText(m_englishStatic[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	if(dwAmount+30>size.cx)
	{
		m_englishComboBox.MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
			dwHorizontalOffset+dwAmount+30,dwVerticalOffset+textmetric.tmHeight*7));
		dwHorizontalOffset+=dwAmount+35;
	}
	else
	{
		m_englishComboBox.MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
			dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight*7));
		dwHorizontalOffset+=size.cx+5;
	}
	
	//Präteritum im englischen Teil des Vokabeleditors (4. Spalte)
	GetDlgItemText(m_englishStatic[4].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[4].MoveWindow(CRect(dwHorizontalOffset,0,dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[5].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishEdit[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_englishButton[5].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+25;

	GetDlgItemText(m_englishStatic[5].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishStatic[5].MoveWindow(CRect(dwHorizontalOffset,0,
		dwHorizontalOffset+size.cx,textmetric.tmHeight));
	dwVerticalOffset=textmetric.tmHeight;

	GetDlgItemText(m_englishButton[6].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_englishEdit[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_englishButton[6].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));

	GetDlgItemText(m_germanStatic[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[0].MoveWindow(CRect(0,dwVerticalStartOfGermanControls,
		size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwHorizontalOffset=size.cx+5;

	GetDlgItemText(m_germanStatic[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwAmount=0;
	for(dwRun=0;dwRun<m_englishWordKind.GetCount();dwRun++)
	{
		m_englishWordKind.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	dwHorizontalOffset+=dwAmount+30+5;

	GetDlgItemText(m_germanStatic[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	GetDlgItemText(m_germanButton[0].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	dwAmount=size.cx+20;
	GetDlgItemText(m_germanButton[1].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	if(size.cx+20>dwAmount)
		dwAmount=size.cx+20;
	GetDlgItemText(m_germanButton[5].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	if(size.cx+20>dwAmount)
		dwAmount=size.cx+20;
	GetDlgItemText(m_germanButton[6].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	if(size.cx+20>dwAmount)
		dwAmount=size.cx+20;
	GetDlgItemText(m_germanButton[7].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	if(size.cx+20>dwAmount)
		dwAmount=size.cx+20;
	m_germanEdit[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+dwAmount,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanButton[0].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+dwAmount,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanButton[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+dwAmount,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanButton[5].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+dwAmount,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanButton[6].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+dwAmount,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanButton[7].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+dwAmount,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=dwAmount+5;

	GetDlgItemText(m_germanStatic[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[3].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	dwAmount=0;
	for(dwRun=0;dwRun<m_englishComboBox.GetCount();dwRun++)
	{
		m_englishComboBox.GetLBText(dwRun,str);
		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	GetDlgItemText(m_germanStatic[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	if(dwAmount+30>size.cx)
	{
		m_germanComboBox.MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
			dwHorizontalOffset+dwAmount+30,dwVerticalOffset+textmetric.tmHeight*7));
		dwHorizontalOffset+=dwAmount+35;
	}
	else
	{
		m_germanComboBox.MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
			dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight*7));
		dwHorizontalOffset+=size.cx+5;
	}

	GetDlgItemText(m_germanStatic[4].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[4].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	GetDlgItemText(m_germanStatic[5].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[5].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	dwAmount=size.cx;

	GetDlgItemText(m_germanStatic[6].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[6].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	if(size.cx>dwAmount)
		dwAmount=size.cx;

	GetDlgItemText(m_germanStatic[7].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[7].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	if(size.cx>dwAmount)
		dwAmount=size.cx;

	GetDlgItemText(m_germanStatic[8].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[8].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	if(size.cx>dwAmount)
		dwAmount=size.cx;

	GetDlgItemText(m_germanStatic[9].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[9].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	if(size.cx>dwAmount)
		dwAmount=size.cx;

	GetDlgItemText(m_germanButton[2].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanButton[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;
	dwHorizontalOffset+=dwAmount+5;

	m_germanEdit[1].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls+textmetric.tmHeight,dwHorizontalOffset+
		size.cx,dwVerticalStartOfGermanControls+(textmetric.tmHeight*2)));
	dwVerticalOffset=dwVerticalStartOfGermanControls+(textmetric.tmHeight*2);

	m_germanEdit[2].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[3].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[4].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[5].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight*2;

	m_germanEdit[6].MoveWindow(CRect(dwHorizontalOffset-dwAmount-5,
		dwVerticalOffset,dwHorizontalOffset+size.cx,
		dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+5;

	//Präteritum im deutschen Teil des Vokabeleditors (4. Spalte)
	GetDlgItemText(m_germanStatic[11].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[11].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;
	
	GetDlgItemText(m_germanButton[3].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);

	m_germanEdit[7].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[8].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[9].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[10].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanEdit[11].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanButton[3].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	//Wortstamm
	m_germanEdit[15].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwHorizontalOffset+=size.cx+25;

	//Perfekt im deutschen Teil des Vokabeleditors (5. Spalte)
	GetDlgItemText(m_germanStatic[12].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[12].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	GetDlgItemText(m_germanButton[8].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanEdit[13].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanButton[4].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	m_germanButton[8].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+20,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

	dwAmount=0;
	for(dwRun=0;dwRun<m_germanListBox[1].GetCount();dwRun++)
	{
		m_germanListBox[1].GetText(dwRun,str);

		size=pDC->GetOutputTextExtent(str);
		if(size.cx>dwAmount)
			dwAmount=size.cx;
	}
	m_germanListBox[1].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx+10,dwVerticalOffset+m_germanListBox[1].
		GetCount()*textmetric.tmHeight));
	GetDlgItemText(m_germanButton[8].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	dwHorizontalOffset+=size.cx+25;

	GetDlgItemText(m_germanStatic[13].GetDlgCtrlID(),str);
	size=pDC->GetOutputTextExtent(str);
	m_germanStatic[13].MoveWindow(CRect(dwHorizontalOffset,
		dwVerticalStartOfGermanControls,dwHorizontalOffset+size.cx,
		dwVerticalStartOfGermanControls+textmetric.tmHeight));
	dwVerticalOffset=dwVerticalStartOfGermanControls+textmetric.tmHeight;

	m_germanEdit[14].MoveWindow(CRect(dwHorizontalOffset,dwVerticalOffset,
		dwHorizontalOffset+size.cx,dwVerticalOffset+textmetric.tmHeight));
	dwVerticalOffset+=textmetric.tmHeight;

#ifdef _DEBUG
	printf("void CVocableEditorBar::VerbControls(BYTE mode) ENDE\n");
#endif
}

Word * CVocableEditorBar::GUIToEnglishWord()
{
	if(m_englishWordKind.GetCurSel()==0) // Substantiv
	{
		EnglishNoun * en=new EnglishNoun();
		GetDlgItemText(ID_USER+12,en->m_strSingular);
		GetDlgItemText(ID_USER+13,en->m_strPlural);
		if(m_englishButton[9].GetCheck())
			en->m_bTranslationType=1;
		else if(m_englishButton[10].GetCheck())
			en->m_bTranslationType=2;
		else
			en->m_bTranslationType=0;
		if(m_englishButton[0].GetCheck())
			en->m_bType=0;
		if(m_englishButton[1].GetCheck())
			en->m_bType=1;
		if(m_englishButton[2].GetCheck())
			en->m_bType=2;
		en->m_bCountable=m_englishButton[3].GetCheck();
		return en;
	}
	// wenn die Wortart "Verb" ist
	if(m_englishWordKind.GetCurSel()==1)
	{
		//wenn das Verb ein Hilfsverb ist
		if(m_englishButton[3].GetCheck())
		{
			EnglishAuxiliaryVerb * eav=new EnglishAuxiliaryVerb();
			GetDlgItemText(ID_USER+12,eav->m_strWords[0]); // Infinitiv
			GetDlgItemText(ID_USER+12,eav->m_strWords[1]); // ich
			GetDlgItemText(ID_USER+12,eav->m_strWords[2]); // du
			GetDlgItemText(ID_USER+12,eav->m_strWords[3]); // er
			GetDlgItemText(ID_USER+12,eav->m_strWords[4]); // wir
			GetDlgItemText(ID_USER+12,eav->m_strWords[5]); // ihr
			GetDlgItemText(ID_USER+12,eav->m_strWords[6]); // sie
			GetDlgItemText(ID_USER+12,eav->m_strWords[7]); // ich
			GetDlgItemText(ID_USER+12,eav->m_strWords[8]); // du
			GetDlgItemText(ID_USER+12,eav->m_strWords[9]); // er
			GetDlgItemText(ID_USER+12,eav->m_strWords[10]); // wir
			GetDlgItemText(ID_USER+12,eav->m_strWords[11]); // ihr
			GetDlgItemText(ID_USER+12,eav->m_strWords[12]); // sie
			GetDlgItemText(ID_USER+12,eav->m_strWords[13]);
			//GetDlgItemText(ID_USER+13,ea->adverb);
			return eav;
		}
		//wenn das Verb ein Vollverb ist
		else
		{
			EnglishVerb * ev=new EnglishVerb();
			GetDlgItemText(ID_USER+12,ev->m_strInfinitive);
			GetDlgItemText(ID_USER+13,ev->m_strPastTense);
			GetDlgItemText(ID_USER+14,ev->m_strPastParticiple);
			GetDlgItemText(m_englishComboBox.GetDlgCtrlID(),ev->m_strPreposition);
			if(m_englishButton[7].GetCheck())
				ev->m_bAllowsToPlusInfinitive=1;
			else
				ev->m_bAllowsToPlusInfinitive=0;
			if(m_englishButton[8].GetCheck())
				ev->m_bAllowsIngForm=1;
			else
				ev->m_bAllowsIngForm=0;
			return ev;
		}
	}
	if(m_englishWordKind.GetCurSel()==2) // Adjektiv
	{
		EnglishAdjective * ea=new EnglishAdjective();
		GetDlgItemText(ID_USER+12,ea->m_strPositiv);
		GetDlgItemText(ID_USER+13,ea->m_strComperativ);
		GetDlgItemText(ID_USER+14,ea->m_strSuperlativ);
		GetDlgItemText(ID_USER+15,ea->m_strAdverb);
		if(m_englishButton[17].GetCheck())
			ea->m_bMoreMost=1;
		else
			ea->m_bMoreMost=0;
		if(m_englishButton[6].GetCheck())
			ea->m_bAllowedPlace=2;
		else if (m_englishButton[10].GetCheck())
			ea->m_bAllowedPlace=1;
		else
			ea->m_bAllowedPlace=0;
		return ea;
	}
	//Wortart: Adverb
	if(m_englishWordKind.GetCurSel()==3)
	{
		EnglishAdverb * ea=new EnglishAdverb();
		GetDlgItemText(ID_USER+12,ea->m_strWord);
		if(m_englishButton[7].GetCheck())
			ea->m_bType=0;
		if(m_englishButton[8].GetCheck())
			ea->m_bType=1;
		if(m_englishButton[9].GetCheck())
			ea->m_bType=2;
		if(m_englishButton[14].GetCheck())
			ea->m_bType=3;
		if(m_englishButton[15].GetCheck())
			ea->m_bType=4;
		if(m_englishButton[16].GetCheck())
			ea->m_bType=5;
		return ea;
	}
	if(m_englishWordKind.GetCurSel()==4) // Präposition
	{
		EnglishPreposition * ep=new EnglishPreposition();
		GetDlgItemText(ID_USER+12,ep->m_strWord);
		return ep;
	}
	if(m_englishWordKind.GetCurSel()==5) // Pronomen
	{
		EnglishPronoun * ep=new EnglishPronoun();
		GetDlgItemText(ID_USER+12,ep->m_strSingular);
		GetDlgItemText(ID_USER+13,ep->m_strPlural);
		return ep;
	}
	if(m_englishWordKind.GetCurSel()==6) // Konjunktion
	{
		EnglishConjunction * ec=new EnglishConjunction();
		GetDlgItemText(ID_USER+12,ec->m_strWord);
		return ec;
	}
	return NULL;
}

Word * CVocableEditorBar::GUIToGermanWord()
{
	if(m_englishWordKind.GetCurSel()==0) // Substantiv
	{
		GermanNoun * gn=new GermanNoun();
		GetDlgItemText(ID_USER+34,gn->m_strSingular);
		GetDlgItemText(ID_USER+35,gn->m_strPlural);
		switch(m_germanListBox[0].GetCurSel())
		{
			case 0:
				gn->m_bArticle='1';
				break;
			case 1:
				gn->m_bArticle='2';
				break;
			case 2:
				gn->m_bArticle='3';
				break;
		}
		return gn;
	}
	if(m_englishWordKind.GetCurSel()==1) // Verb
	{
		GermanVerb * gv=new GermanVerb();
		GetDlgItemText(ID_USER+34,gv->m_strWords[0]); // Infinitiv
		GetDlgItemText(ID_USER+35,gv->m_strWords[1]); // Präsens
		GetDlgItemText(ID_USER+36,gv->m_strWords[2]); // du
		GetDlgItemText(ID_USER+37,gv->m_strWords[3]); // er/sie/es
		GetDlgItemText(ID_USER+38,gv->m_strWords[4]); // wir
		GetDlgItemText(ID_USER+39,gv->m_strWords[5]); // ihr
		GetDlgItemText(ID_USER+38,gv->m_strWords[6]); // sie (wie bei: wir)
		GetDlgItemText(ID_USER+41,gv->m_strWords[7]); // Präteritum
		GetDlgItemText(ID_USER+42,gv->m_strWords[8]); // du
		GetDlgItemText(ID_USER+43,gv->m_strWords[9]); // er
		GetDlgItemText(ID_USER+44,gv->m_strWords[10]); // wir
		GetDlgItemText(ID_USER+45,gv->m_strWords[11]); // ihr
		GetDlgItemText(ID_USER+44,gv->m_strWords[12]); // sie (wie bei: wir)
		GetDlgItemText(ID_USER+47,gv->m_strWords[13]); // Perfekt
		GetDlgItemText(ID_USER+48,gv->m_strWords[14]); // Konjunktiv
		GetDlgItemText(m_germanComboBox.GetDlgCtrlID(),gv->m_strPreposition);
		if(m_germanButton[1].GetCheck())
			gv->m_bMove=TRUE;
		else
			gv->m_bMove=FALSE;
		if(m_germanButton[5].GetCheck())
			gv->m_bCase=0; // kein Fall
		if(m_germanButton[6].GetCheck())
			gv->m_bCase=1; // 3. Fall
		if(m_germanButton[7].GetCheck())
			if(gv->m_bCase==1)
				gv->m_bCase=3; // 3. und 4. Fall
			else
				gv->m_bCase=2; // 4. Fall
		switch(m_germanListBox[1].GetCurSel())
		{
			case 0:
				gv->m_bBe=FALSE;
				break;
			case 1:
				gv->m_bBe=TRUE;
				break;
		}
		if(m_germanButton[0].GetCheck())
			gv->m_bReflexive=TRUE;
		else
			gv->m_bReflexive=FALSE;
		if(m_englishButton[3].GetCheck())
			gv->m_bAuxiliaryVerb=TRUE;
		else
			gv->m_bAuxiliaryVerb=FALSE;
		return gv;
	}
	if(m_englishWordKind.GetCurSel()==2) // Adjektiv
	{
		GermanAdjective * ga=new GermanAdjective();
		GetDlgItemText(ID_USER+34,ga->m_strPositiv);
		GetDlgItemText(ID_USER+35,ga->m_strComperativ);
		GetDlgItemText(ID_USER+41,ga->m_strSuperlativ);
		GetDlgItemText(ID_USER+47,ga->m_strWortstamm);
		return ga;
	}
	if(m_englishWordKind.GetCurSel()==3) // Adverb
	{
		GermanAdverb * ga=new GermanAdverb();
		GetDlgItemText(ID_USER+34,ga->m_strWord);
		return ga;
	}
	if(m_englishWordKind.GetCurSel()==4) // Präposition
	{
		GermanPreposition * gp=new GermanPreposition();
		GetDlgItemText(ID_USER+34,gp->m_strWord);
		return gp;
	}
	if(m_englishWordKind.GetCurSel()==5) // Pronomen
	{
		GermanPronoun * gp=new GermanPronoun();
		GetDlgItemText(ID_USER+34,gp->m_strWord);
		return gp;
	}
	if(m_englishWordKind.GetCurSel()==6) // Konjunktion
	{
		GermanConjunction * gc=new GermanConjunction();
		GetDlgItemText(ID_USER+34,gc->m_strWord);
		if(m_germanButton[11].GetCheck())
			gc->m_bWordOrder=0;
		if(m_germanButton[12].GetCheck())
			gc->m_bWordOrder=1;
		return gc;
	}
	return NULL;
}

void CVocableEditorBar::EnglishAdjectiveToGUI(Word & cr_wordEnglish)
{
	AdjectiveControls();
	EnglishAdjective * ea = dynamic_cast<EnglishAdjective*>( //pWordNode->m_pWord
    & cr_wordEnglish );
	//wenn der Speicher erfolgreich alloziert wurde (ea ist ungleich NULL)
	if( ea )
	{
#ifdef _DEBUG
		printf("ea\n");
#endif
		m_englishWordKind.SetCurSel(2);
		SetDlgItemText(ID_USER+12,ea->m_strPositiv);
		SetDlgItemText(ID_USER+13,ea->m_strComperativ);
		SetDlgItemText(ID_USER+14,ea->m_strSuperlativ);
		SetDlgItemText(ID_USER+15,ea->m_strAdverb);
		if(ea->m_bMoreMost)
		{
			m_englishButton[17].SetCheck(1);
			GetDlgItem(ID_USER+13)->EnableWindow(SW_HIDE);
			GetDlgItem(ID_USER+14)->EnableWindow(SW_HIDE);
		}
		else
		{
			m_englishButton[17].SetCheck(0);
			GetDlgItem(ID_USER+13)->EnableWindow(SW_SHOW);
			GetDlgItem(ID_USER+14)->EnableWindow(SW_SHOW);
		}
		if(ea->m_bAllowedPlace == 1 )
			m_englishButton[10].SetCheck(1);
		if(ea->m_bAllowedPlace==2)
			m_englishButton[6].SetCheck(1);
		//if(//pWordNode->m_pWordNodeNext
 //     pwordGerman
 //     )
		//{
			//WordNode * pWordNodeNext = pWordNode->m_pWordNodeNext;
			//GermanAdjective * ga = dynamic_cast<GermanAdjective*>(
   //     //pWordNodeNext->m_pWord
   //     & cr_wordGerman
   //     );
			//SetDlgItemText(ID_USER + 34,ga->m_strPositiv);
			//SetDlgItemText(ID_USER + 35,ga->m_strComperativ);
			//SetDlgItemText(ID_USER + 41,ga->m_strSuperlativ);
			//SetDlgItemText(ID_USER + 47,ga->m_strWortstamm);
			//TRACE("ga->m_strWortstamm: %s\n",ga->m_strWortstamm);
		//}
		//UpdateData(FALSE);
		//m_englishEdit[1]=en->plural;
			//MessageBox("jj",NULL,MB_OK);
	}
}

void CVocableEditorBar::WordToGUI(
  //WordNode * pWordNode
  //const is not possible because of "dynamic_cast<...>(...)"
  //const 
  Word & cr_wordEnglish
  , 
  //const is not possible because of "dynamic_cast<...>(...)"
  //const 
  Word & cr_wordGerman
  )
{
#ifdef _DEBUG
	printf("void CVocableEditorBar::WordToGUI(WordNode * pWordNode) ANFANG\n");
	fflush(stdout);
#endif
	TRACE("void CMyBar::WordToGUI(Vocable & voc)\n");
	//SetDlgItemText(ID_USER+7,"gg");//en->singular);
	// wenn englisches Adjektiv
	if( typeid(//*pWordNode->m_pWord
      cr_wordEnglish ) == typeid(EnglishAdjective) 
    )
	{
    EnglishAdjectiveToGUI(cr_wordEnglish) ;
	}
	// wenn englisches Adverb
	else if( typeid(//*pWordNode->m_pWord
    cr_wordEnglish ) == typeid(EnglishAdverb) 
    )
	{
		AdverbControls();
		EnglishAdverb * eav = dynamic_cast<EnglishAdverb*>(//pWordNode->m_pWord
      & cr_wordEnglish );
		//wenn der Speicher erfolgreich alloziert wurde (eav ist ungleich NULL)
		if(eav)
		{
			m_englishWordKind.SetCurSel(3);
			SetDlgItemText(ID_USER + 12,eav->m_strWord);
			TRACE("eav->  : %u,  %c\n",eav->m_bType,eav->m_bType);
			m_englishButton[7].SetCheck(0);
			m_englishButton[8].SetCheck(0);
			m_englishButton[9].SetCheck(0);
			m_englishButton[14].SetCheck(0);
			m_englishButton[15].SetCheck(0);
			m_englishButton[16].SetCheck(0);
			if( eav->m_bType < 4 )
				m_englishButton[eav->m_bType + 7].SetCheck(1);
			else
				m_englishButton[eav->m_bType + 11].SetCheck(1);
			//SetDlgItemInt(ID_USER+6+eav->type,1,FALSE);
			//SetDlgItemInt(ID_USER+3,1);
			//if(pWordNode->m_pWordNodeNext)
			//{
				//WordNode * pWordNodeNext = pWordNode->m_pWordNodeNext;
				GermanAdverb * gav = dynamic_cast<GermanAdverb*>(//pWordNodeNext->m_pWord
          & cr_wordGerman );
				SetDlgItemText(ID_USER + 34,gav->m_strWord);
			//}
			//UpdateData(FALSE);
			//m_englishEdit[1]=en->plural;
				//MessageBox("jj",NULL,MB_OK);
		}
	}
	// wenn englisches Hilfsverb
	else if( typeid(//*pWordNode->m_pWord
      cr_wordEnglish ) == typeid(EnglishAuxiliaryVerb)
    )
	{
		AuxiliaryVerbControls();
		EnglishAuxiliaryVerb * eav = dynamic_cast<EnglishAuxiliaryVerb*>
			(//pWordNode->m_pWord
      & cr_wordEnglish );
		//wenn der Speicher erfolgreich alloziert wurde (eav ist ungleich NULL)
		if(eav)
		{
			m_englishWordKind.SetCurSel(1);
			SetDlgItemText(ID_USER+12,eav->m_strWords[0]);
			SetDlgItemText(ID_USER+13,eav->m_strWords[7]);
			SetDlgItemText(ID_USER+14,eav->m_strWords[12]);
			m_englishButton[3].SetCheck(1);
			//if(pWordNode->m_pWordNodeNext)
			//{
			//	WordNode * pWordNodeNext=pWordNode->m_pWordNodeNext;
				GermanVerb * gv = dynamic_cast<GermanVerb*>(//pWordNodeNext->m_pWord
          & cr_wordGerman );
				if( gv )
				{
					SetDlgItemText(ID_USER + 34,gv->m_strWords[0]); // Infinitiv
					SetDlgItemText(ID_USER + 35,gv->m_strWords[1]); // ich
					SetDlgItemText(ID_USER + 36,gv->m_strWords[2]); // du
					SetDlgItemText(ID_USER + 37,gv->m_strWords[3]); // er
					SetDlgItemText(ID_USER + 38,gv->m_strWords[4]); // wir
					SetDlgItemText(ID_USER + 39,gv->m_strWords[5]); // ihr
					SetDlgItemText(ID_USER + 41,gv->m_strWords[7]); // ich
					SetDlgItemText(ID_USER + 42,gv->m_strWords[8]); // du
					SetDlgItemText(ID_USER + 43,gv->m_strWords[9]); // er
					SetDlgItemText(ID_USER + 44,gv->m_strWords[10]); // wir
					SetDlgItemText(ID_USER + 45,gv->m_strWords[11]); // ihr
					SetDlgItemText(ID_USER + 47,gv->m_strWords[13]);
					SetDlgItemText(ID_USER + 48,gv->m_strWords[14]);
					m_germanListBox[1].SetCurSel(gv->m_bBe-49);
				}
			//}
		}
	}
	// wenn englische Konjuntion
	else if( 
    typeid(//*pWordNode->m_pWord
    cr_wordEnglish ) == typeid(EnglishConjunction)
    )
	{
		ConjunctionControls();
		EnglishConjunction * ec = dynamic_cast<EnglishConjunction*>(//pWordNode->m_pWord
      & cr_wordEnglish );
		//wenn der Speicher erfolgreich alloziert wurde (ec ist ungleich NULL)
		if(ec)
		{
			m_englishWordKind.SetCurSel(6);
			SetDlgItemText(ID_USER + 12,ec->m_strWord);
			//if(pWordNode->m_pWordNodeNext)
			//{
			//	WordNode * pWordNodeNext=pWordNode->m_pWordNodeNext;
				GermanConjunction * gc=dynamic_cast<GermanConjunction*>
					(//pWordNodeNext->m_pWord
          & cr_wordGerman );
				SetDlgItemText(ID_USER + 34,gc->m_strWord);
				if(gc->m_bWordOrder == 0 )
					m_germanButton[11].SetCheck(1);
				else
					m_germanButton[11].SetCheck(0);
				if(gc->m_bWordOrder == 1)
					m_germanButton[12].SetCheck(1);
				else
					m_germanButton[12].SetCheck(0);
			//}
		}
	}
	// wenn englisches Substantiv
	else if( typeid(//*pWordNode->m_pWord
    cr_wordEnglish ) == typeid(EnglishNoun)
    )
	{
		NounControls();
		EnglishNoun * en = dynamic_cast<EnglishNoun*>(//pWordNode->m_pWord
      & cr_wordEnglish );
		//wenn der Speicher erfolgreich alloziert wurde (en ist ungleich NULL)
		if(en)
		{
			m_englishWordKind.SetCurSel(0);
			SetDlgItemText(ID_USER+12,en->m_strSingular);
			SetDlgItemText(ID_USER+13,en->m_strPlural);
			m_englishButton[0].SetCheck(0);
			TRACE("en->m_bType: %u\n",en->m_bType);
			m_englishButton[en->m_bType].SetCheck(1);
#ifdef _DEBUG
			printf("en->m_strSingular: %s en->m_strPlural: %s\n",
				en->m_strSingular,en->m_strPlural);
			printf("en->m_bTranslationType: %u\n",en->m_bTranslationType);
#endif
			if(en->m_bTranslationType==1)
				m_englishButton[10].SetCheck(1);
			else
				m_englishButton[10].SetCheck(0);
			if(en->m_bTranslationType==2)
				m_englishButton[11].SetCheck(1);
			else
				m_englishButton[11].SetCheck(0);
			m_englishButton[3].SetCheck(en->m_bCountable);
			//if(pWordNode->m_pWordNodeNext)
			//{
			//	WordNode * pWordNodeNext=pWordNode->m_pWordNodeNext;
				//GermanNoun * gn = dynamic_cast<GermanNoun*>(//pWordNodeNext->m_pWord
    //      & cr_wordGerman );
				//SetDlgItemText(ID_USER + 34,gn->m_strSingular);
				//SetDlgItemText(ID_USER + 35,gn->m_strPlural);
				//if(gn->m_bArticle == '1')
				//	m_germanListBox[0].SetCurSel(0);
				//if(gn->m_bArticle == '2')
				//	m_germanListBox[0].SetCurSel(1);
				//if(gn->m_bArticle == '3')
				//	m_germanListBox[0].SetCurSel(2);
			//}
		}
	}
	// wenn englische Präposition
	else if( typeid(//*pWordNode->m_pWord
    cr_wordEnglish ) == typeid(EnglishPreposition)
    )
	{
		PrepositionControls();
		EnglishPreposition * ep = dynamic_cast<EnglishPreposition*>(//pWordNode->m_pWord
      & cr_wordEnglish );
		//wenn der Speicher erfolgreich alloziert wurde (ep ist ungleich NULL)
		if(ep)
		{
			m_englishWordKind.SetCurSel(4);
			SetDlgItemText(ID_USER+12,ep->m_strWord);
			//TRACE("eav->  : %u,  %c\n",eav->type,eav->type);
			//SetDlgItemInt(ID_USER+3,1);
			//if(pWordNode->m_pWordNodeNext)
			//{
			//	WordNode * pWordNodeNext=pWordNode->m_pWordNodeNext;
				GermanPreposition * gp = dynamic_cast<GermanPreposition*>
					(//pWordNodeNext->m_pWord
          & cr_wordGerman );
				SetDlgItemText(ID_USER + 34,gp->m_strWord);
			//}
			//UpdateData(FALSE);
			//m_englishEdit[1]=en->plural;
				//MessageBox("jj",NULL,MB_OK);
		}
	}
	// wenn englisches Pronomen
	else if( typeid(//*pWordNode->m_pWord
    & cr_wordEnglish ) == typeid(EnglishPronoun)
    )
	{
		PronounControls();
		EnglishPronoun * ep = dynamic_cast<EnglishPronoun*>(//pWordNode->m_pWord
      & cr_wordEnglish );
		//wenn der Speicher erfolgreich alloziert wurde (ep ist ungleich NULL)
		if(ep)
		{
			m_englishWordKind.SetCurSel(5);
			SetDlgItemText(ID_USER+12,ep->m_strSingular);
			SetDlgItemText(ID_USER+13,ep->m_strPlural);
			//TRACE("eav->  : %u,  %c\n",eav->type,eav->type);
			//SetDlgItemInt(ID_USER+3,1);
			//if(pWordNode->m_pWordNodeNext)
			//{
			//	WordNode * pWordNodeNext=pWordNode->m_pWordNodeNext;
				GermanPronoun * gp = dynamic_cast<GermanPronoun*>(//pWordNodeNext->
					//m_pWord
          & cr_wordGerman );
				SetDlgItemText(ID_USER+34,gp->m_strWord);
			//}
			//UpdateData(FALSE);
			//m_englishEdit[1]=en->plural;
				//MessageBox("jj",NULL,MB_OK);
		}
	}
	// wenn englisches Verb
	else if( typeid(//*pWordNode->m_pWord
    cr_wordEnglish ) == typeid(EnglishVerb)
    )
	{
		EnglishVerb * ev = dynamic_cast<EnglishVerb*>(//pWordNode->m_pWord
      & cr_wordEnglish );
#ifdef _DEBUG
		printf("typeid(*pWordNode->m_pWord)==typeid(EnglishVerb)\n");
#endif
		VerbControls(MINIMAL);
		//wenn der Speicher erfolgreich alloziert wurde (ev ist ungleich NULL)
		if(ev)
		{
			m_englishWordKind.SetCurSel(1);
			SetDlgItemText(ID_USER+12,ev->m_strInfinitive);
			SetDlgItemText(ID_USER+13,ev->m_strPastTense);
			SetDlgItemText(ID_USER+14,ev->m_strPastParticiple);
			if(ev->m_strPreposition==_T(""))
				m_englishComboBox.SetCurSel(0);
			else
				m_englishComboBox.SelectString(-1,ev->m_strPreposition);
			m_englishButton[3].SetCheck(0);
			m_englishButton[7].SetCheck(ev->m_bAllowsToPlusInfinitive);
			m_englishButton[8].SetCheck(ev->m_bAllowsIngForm);
			//m_englishButton[eav->type-47].SetCheck(1);
			//SetDlgItemInt(ID_USER+3,1);
			//if(pWordNode->m_pWordNodeNext)
			//{
			//	WordNode * pWordNodeNext=pWordNode->m_pWordNodeNext;
				GermanVerb * gv = dynamic_cast<GermanVerb*>(//pWordNodeNext->m_pWord
          & cr_wordGerman );
				if(gv)
				{
#ifdef _DEBUG
					printf("gv\n");
					printf("gv->m_strWords[0]: %s\n",gv->m_strWords[0]);
#endif
					SetDlgItemText(ID_USER+34,gv->m_strWords[0]);//gv->m_strWords[0]); // Infinitiv
					SetDlgItemText(ID_USER+35,gv->m_strWords[1]); // ich
					SetDlgItemText(ID_USER+36,gv->m_strWords[2]); // du
					SetDlgItemText(ID_USER+37,gv->m_strWords[3]); // er
					SetDlgItemText(ID_USER+38,gv->m_strWords[4]); // wir
					SetDlgItemText(ID_USER+39,gv->m_strWords[5]); // ihr
					SetDlgItemText(ID_USER+41,gv->m_strWords[7]); // ich
					SetDlgItemText(ID_USER+42,gv->m_strWords[8]); // du
					SetDlgItemText(ID_USER+43,gv->m_strWords[9]); // er
					SetDlgItemText(ID_USER+44,gv->m_strWords[10]); // wir
					SetDlgItemText(ID_USER+45,gv->m_strWords[11]); // ihr
					SetDlgItemText(ID_USER+47,gv->m_strWords[13]);
					SetDlgItemText(ID_USER+48,gv->m_strWords[14]);
					//SetDlgItemText(m_germanComboBox.GetDlgCtrlID(),gv->m_strPreposition);
					if(gv->m_strPreposition==_T(""))
						m_germanComboBox.SetCurSel(0);
					else
						m_germanComboBox.SelectString(-1,gv->m_strPreposition);
					m_germanButton[5].SetCheck(0);
					m_germanButton[6].SetCheck(0);
					m_germanButton[7].SetCheck(0);
					if(gv->m_bCase == 0 )
						m_germanButton[5].SetCheck(1);
					if(gv->m_bCase == 1)
						m_germanButton[6].SetCheck(1);
					if(gv->m_bCase==2)
						m_germanButton[7].SetCheck(1);
					if(gv->m_bCase==3)
					{
						m_germanButton[6].SetCheck(1);
						m_germanButton[7].SetCheck(1);
					}
					//m_germanButton[gv->Case+5].SetCheck(1);
					m_germanButton[1].SetCheck(gv->m_bMove);
					m_germanListBox[1].SetCurSel(gv->m_bBe);
					m_germanButton[0].SetCheck(gv->m_bReflexive);
					TRACE("gv->m_bReflexive: %u\n",gv->m_bReflexive);
					TRACE("gv->m_bMove  : %u,  %c\n",gv->m_bMove,gv->m_bMove);
					TRACE("gv->m_bCase  : %u,  %c\n",gv->m_bCase,gv->m_bCase);
				}
			//SetDlgItemText(ID_USER+20,gav->word);
			//}
			//UpdateData(FALSE);
			//m_englishEdit[1]=en->plural;
				//MessageBox("jj",NULL,MB_OK);
		}
	}
//	if( typeid(//*pWordNode->m_pWord
//    cr_wordGerman ) == typeid(GermanAdjective)
//    ) // wenn deutsches Adjektiv
//	{
//		AdjectiveControls();
//		GermanAdjective * ga = dynamic_cast<GermanAdjective*>(//pWordNode->m_pWord
//      & cr_wordEnglish );
//		//wenn der Speicher erfolgreich alloziert wurde (ga ist ungleich NULL)
//		if(ga)
//		{
//			m_englishWordKind.SetCurSel(2);
//			SetDlgItemText(ID_USER+34,ga->m_strPositiv);
//			SetDlgItemText(ID_USER+35,ga->m_strComperativ);
//			SetDlgItemText(ID_USER+41,ga->m_strSuperlativ);
//			SetDlgItemText(ID_USER+47,ga->m_strWortstamm);
//			if(pWordNode->m_pWordNodePrevious)
//			{
//				pWordNode=pWordNode->m_pWordNodePrevious;
//				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(pWordNode->m_pWord);
//				SetDlgItemText(ID_USER+12,ea->m_strPositiv);
//				SetDlgItemText(ID_USER+13,ea->m_strComperativ);
//				SetDlgItemText(ID_USER+14,ea->m_strSuperlativ);
//				SetDlgItemText(ID_USER+15,ea->m_strAdverb);
//			}
//		}
//	}
//	if(typeid(*pWordNode->m_pWord)==typeid(GermanAdverb)) // wenn deutsches Adverb
//	{
//		AdverbControls();
//		GermanAdverb * gav=dynamic_cast<GermanAdverb*>(pWordNode->m_pWord);
//		//wenn der Speicher erfolgreich alloziert wurde (gav ist ungleich NULL)
//		if(gav)
//		{
//			m_englishWordKind.SetCurSel(3);
//			SetDlgItemText(ID_USER+34,gav->m_strWord);
//			if(pWordNode->m_pWordNodePrevious)
//			{
//				pWordNode=pWordNode->m_pWordNodePrevious;
//				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>(pWordNode->m_pWord);
//				SetDlgItemText(ID_USER+12,eav->m_strWord);
//				m_englishButton[7].SetCheck(0);
//				m_englishButton[8].SetCheck(0);
//				m_englishButton[9].SetCheck(0);
//				m_englishButton[14].SetCheck(0);
//				m_englishButton[15].SetCheck(0);
//				m_englishButton[16].SetCheck(0);
//				if(eav->m_bType<4)
//					m_englishButton[eav->m_bType+7].SetCheck(1);
//				else
//					m_englishButton[eav->m_bType+11].SetCheck(1);
//			}
//		}
//	}
//	if(typeid(*pWordNode->m_pWord)==typeid(GermanConjunction)) // wenn deutsche Konjunktion
//	{
//		ConjunctionControls();
//		GermanConjunction * gc=dynamic_cast<GermanConjunction*>(pWordNode->m_pWord);
//		//wenn der Speicher erfolgreich alloziert wurde (gc ist ungleich NULL)
//		if(gc)
//		{
//			m_englishWordKind.SetCurSel(6);
//			SetDlgItemText(ID_USER+34,gc->m_strWord);
//			if(gc->m_bWordOrder==0)
//				m_germanButton[11].SetCheck(1);
//			else
//				m_germanButton[11].SetCheck(0);
//			if(gc->m_bWordOrder==1)
//				m_germanButton[12].SetCheck(1);
//			else
//				m_germanButton[12].SetCheck(0);
//			if(pWordNode->m_pWordNodePrevious)
//			{
//				pWordNode=pWordNode->m_pWordNodePrevious;
//				EnglishConjunction * ec=dynamic_cast<EnglishConjunction*>(pWordNode->m_pWord);
//				SetDlgItemText(ID_USER+12,ec->m_strWord);
//			}
//		}
//	}
//	else if(typeid(*pWordNode->m_pWord)==typeid(GermanNoun)) // wenn deutsches Substantiv
//	{
//		NounControls();
//		GermanNoun * gn=dynamic_cast<GermanNoun*>(pWordNode->m_pWord);
//		//wenn der Speicher erfolgreich alloziert wurde (gn ist ungleich NULL)
//		if(gn)
//		{
//			m_englishWordKind.SetCurSel(0);
//			SetDlgItemText(ID_USER+34,gn->m_strSingular);
//			SetDlgItemText(ID_USER+35,gn->m_strPlural);
//			if(gn->m_bArticle=='1')
//				m_germanListBox[0].SetCurSel(0);
//			if(gn->m_bArticle=='2')
//				m_germanListBox[0].SetCurSel(1);
//			if(gn->m_bArticle=='3')
//				m_germanListBox[0].SetCurSel(2);
//			if(pWordNode->m_pWordNodePrevious)
//			{
//				pWordNode=pWordNode->m_pWordNodePrevious;
//				EnglishNoun * en=dynamic_cast<EnglishNoun*>(pWordNode->m_pWord);
//				SetDlgItemText(ID_USER+12,en->m_strSingular);
//				SetDlgItemText(ID_USER+13,en->m_strPlural);
//				m_englishButton[0].SetCheck(0);
//				TRACE("en->m_bType: %u\n",en->m_bType);
//				m_englishButton[en->m_bType].SetCheck(1);
//#ifdef _DEBUG
//				printf("en->m_strSingular: %s en->m_strPlural: %s\n",
//					en->m_strSingular,en->m_strPlural);
//				printf("en->m_bTranslationType: %u\n",en->m_bTranslationType);
//#endif
//				if(en->m_bTranslationType==1)
//					m_englishButton[10].SetCheck(1);
//				else
//					m_englishButton[10].SetCheck(0);
//				if(en->m_bTranslationType==2)
//					m_englishButton[11].SetCheck(1);
//				else
//					m_englishButton[11].SetCheck(0);
//			}
//		}
//	}
//	if( typeid(*pWordNode->m_pWord) == typeid(GermanPreposition)
//    ) // wenn deutsche Präposition
//	{
//		PrepositionControls();
//		GermanPreposition * gp=dynamic_cast<GermanPreposition*>(pWordNode->m_pWord);
//		//wenn der Speicher erfolgreich alloziert wurde (gp ist ungleich NULL)
//		if(gp)
//		{
//			m_englishWordKind.SetCurSel(4);
//			SetDlgItemText(ID_USER+34,gp->m_strWord);
//			if(pWordNode->m_pWordNodePrevious)
//			{
//				pWordNode=pWordNode->m_pWordNodePrevious;
//				EnglishPreposition * ep=dynamic_cast<EnglishPreposition*>(pWordNode->m_pWord);
//				SetDlgItemText(ID_USER+12,ep->m_strWord);
//			}
//		}
//	}
//	if(typeid(*pWordNode->m_pWord)==typeid(GermanPronoun)) // wenn deutsches Pronomen
//	{
//		PronounControls();
//		GermanPronoun * gp=dynamic_cast<GermanPronoun*>(pWordNode->m_pWord);
//		//wenn der Speicher erfolgreich alloziert wurde (gp ist ungleich NULL)
//		if(gp)
//		{
//			m_englishWordKind.SetCurSel(5);
//			SetDlgItemText(ID_USER+34,gp->m_strWord);
//			if(pWordNode->m_pWordNodePrevious)
//			{
//				pWordNode=pWordNode->m_pWordNodePrevious;
//				EnglishPronoun * ep=dynamic_cast<EnglishPronoun*>(pWordNode->m_pWord);
//				if(ep)
//				{
//					SetDlgItemText(ID_USER+12,ep->m_strSingular);
//					SetDlgItemText(ID_USER+13,ep->m_strPlural);
//				}
//			}
//		}
//	}
//	else if(typeid(*pWordNode->m_pWord) == typeid(GermanVerb) ) // wenn deutsches Verb
//	{
//#ifdef _DEBUG
//		printf("typeid(*pWordNode->m_pWord)==typeid(GermanVerb)\n");
//		fflush(stdout);
//#endif
//		GermanVerb * gv=dynamic_cast<GermanVerb*>(pWordNode->m_pWord);
//		//wenn der Speicher erfolgreich alloziert wurde (gv ist ungleich NULL)
//		if(gv)
//		{
//#ifdef _DEBUG
//			printf("gv!=NULL\n");
//
//			fflush(stdout);
//#endif
//			if(pWordNode->m_pWordNodePrevious)
//			{
//#ifdef _DEBUG
//				printf("pWordNode->m_pWordNodePrevious\n");
//				fflush(stdout);
//#endif
//				//VerbControls(MINIMAL);
//				pWordNode=pWordNode->m_pWordNodePrevious;
//				if(typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb))
//				{
//					EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>(
//						pWordNode->m_pWord);
//#ifdef _DEBUG
//					printf("typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
//					fflush(stdout);
//#endif
//					AuxiliaryVerbControls();
//					if(eav)
//					{
//						m_englishWordKind.SetCurSel(1);
//						SetDlgItemText(ID_USER+34,gv->m_strWords[0]);//gv->m_strWords[0]); // Infinitiv
//						SetDlgItemText(ID_USER+35,gv->m_strWords[1]); // ich
//						SetDlgItemText(ID_USER+36,gv->m_strWords[2]); // du
//						SetDlgItemText(ID_USER+37,gv->m_strWords[3]); // er
//						SetDlgItemText(ID_USER+38,gv->m_strWords[4]); // wir
//						SetDlgItemText(ID_USER+39,gv->m_strWords[5]); // ihr
//						SetDlgItemText(ID_USER+41,gv->m_strWords[7]); // ich
//						SetDlgItemText(ID_USER+42,gv->m_strWords[8]); // du
//						SetDlgItemText(ID_USER+43,gv->m_strWords[9]); // er
//						SetDlgItemText(ID_USER+44,gv->m_strWords[10]); // wir
//						SetDlgItemText(ID_USER+45,gv->m_strWords[11]); // ihr
//						SetDlgItemText(ID_USER+47,gv->m_strWords[13]);
//						SetDlgItemText(ID_USER+48,gv->m_strWords[14]);
//						//SetDlgItemText(m_germanComboBox.GetDlgCtrlID(),gv->m_strPreposition);
//						if(gv->m_strPreposition==_T(""))
//							m_germanComboBox.SetCurSel(0);
//						else
//							m_germanComboBox.SelectString(-1,gv->m_strPreposition);
//						m_germanButton[5].SetCheck(0);
//						m_germanButton[6].SetCheck(0);
//						m_germanButton[7].SetCheck(0);
//						if(gv->m_bCase==0)
//							m_germanButton[5].SetCheck(1);
//						if(gv->m_bCase==1)
//							m_germanButton[6].SetCheck(1);
//						if(gv->m_bCase==2)
//							m_germanButton[7].SetCheck(1);
//						if(gv->m_bCase==3)
//						{
//							m_germanButton[6].SetCheck(1);
//							m_germanButton[7].SetCheck(1);
//						}
//						m_germanButton[1].SetCheck(gv->m_bMove);
//						m_germanListBox[1].SetCurSel(gv->m_bBe);
//						m_germanButton[0].SetCheck(gv->m_bReflexive);
//						SetDlgItemText(ID_USER+12,eav->m_strWords[0]);
//						SetDlgItemText(ID_USER+13,eav->m_strWords[7]);
//						SetDlgItemText(ID_USER+14,eav->m_strWords[12]);
//						m_englishButton[3].SetCheck(1);
//					}
//				}
//				if(typeid(*pWordNode->m_pWord)==typeid(EnglishVerb))
//				{
//#ifdef _DEBUG
//					printf("typeid(*pWordNode->m_pWord)==typeid(EnglishVerb)\n");
//					fflush(stdout);
//#endif
//					VerbControls(MINIMAL);
//					EnglishVerb * ev=dynamic_cast<EnglishVerb*>(pWordNode->m_pWord);
//					if(ev)
//					{
//						m_englishWordKind.SetCurSel(1);
//						SetDlgItemText(ID_USER+34,gv->m_strWords[0]);//gv->m_strWords[0]); // Infinitiv
//						SetDlgItemText(ID_USER+35,gv->m_strWords[1]); // ich
//						SetDlgItemText(ID_USER+36,gv->m_strWords[2]); // du
//						SetDlgItemText(ID_USER+37,gv->m_strWords[3]); // er
//						SetDlgItemText(ID_USER+38,gv->m_strWords[4]); // wir
//						SetDlgItemText(ID_USER+39,gv->m_strWords[5]); // ihr
//						SetDlgItemText(ID_USER+41,gv->m_strWords[7]); // ich
//						SetDlgItemText(ID_USER+42,gv->m_strWords[8]); // du
//						SetDlgItemText(ID_USER+43,gv->m_strWords[9]); // er
//						SetDlgItemText(ID_USER+44,gv->m_strWords[10]); // wir
//						SetDlgItemText(ID_USER+45,gv->m_strWords[11]); // ihr
//						SetDlgItemText(ID_USER+47,gv->m_strWords[13]);
//						SetDlgItemText(ID_USER+48,gv->m_strWords[14]);
//						//SetDlgItemText(m_germanComboBox.GetDlgCtrlID(),gv->m_strPreposition);
//						if(gv->m_strPreposition==_T(""))
//							m_germanComboBox.SetCurSel(0);
//						else
//							m_germanComboBox.SelectString(-1,gv->m_strPreposition);
//						m_germanButton[5].SetCheck(0);
//						m_germanButton[6].SetCheck(0);
//						m_germanButton[7].SetCheck(0);
//						if(gv->m_bCase==0)
//							m_germanButton[5].SetCheck(1);
//						if(gv->m_bCase==1)
//							m_germanButton[6].SetCheck(1);
//						if(gv->m_bCase==2)
//							m_germanButton[7].SetCheck(1);
//						if(gv->m_bCase==3)
//						{
//							m_germanButton[6].SetCheck(1);
//							m_germanButton[7].SetCheck(1);
//						}
//						m_germanButton[1].SetCheck(gv->m_bMove);
//						m_germanListBox[1].SetCurSel(gv->m_bBe);
//						m_germanButton[0].SetCheck(gv->m_bReflexive);
//						SetDlgItemText(ID_USER+12,ev->m_strInfinitive);
//						SetDlgItemText(ID_USER+13,ev->m_strPastTense);
//						SetDlgItemText(ID_USER+14,ev->m_strPastParticiple);
//						if(ev->m_strPreposition==_T(""))
//							m_englishComboBox.SetCurSel(0);
//						else
//							m_englishComboBox.SelectString(-1,ev->m_strPreposition);
//						m_englishButton[3].SetCheck(0);
//					}
//				}
//			}
//		}
//	}
#ifdef _DEBUG
	printf("void CVocableEditorBar::WordToGUI(WordNode * pWordNode) ENDE\n");
	fflush(stdout);
#endif
}

//wenn sich der Inhalt des Textfeldes des englischen Singulars ändert
void CVocableEditorBar::OnChangeIDUser10()
{
  int nCurrentWordkindSelection=m_englishWordKind.GetCurSel();
  //In "PrepositionControls()" elements of "m_englishButton[]" 
  //are destroyed, so accessing them, (e.g. "m_englishButton[5].GetCheck()")
  //would led to a program crash.
  if(nCurrentWordkindSelection!=WORDKIND_INDEX_FOR_PREPOSITION)
  {
    //TODO better check of control is created.
    if(m_englishButton[5].m_hWnd != 0 )
	  if(m_englishButton[5].GetCheck()) // Plural/Präteritum regelmäßig-Kontrollkästchen
	  { // ist aktiviert
		  if(nCurrentWordkindSelection==WORDKIND_INDEX_FOR_NOUN) // Wortart: Substantiv
		  {
			  CString str;
			  GetDlgItemText(ID_USER+12,str);
			  if(str.GetLength()>0)
			  {
				  if(str.Right(1)=="x" || str.Right(1)=="s") // ox -> oxES ; boss -> bossES
				  {
					  SetDlgItemText(ID_USER+13,str+"es");
					  return;
				  }
			  }
			  if(str.GetLength()>1)
			  {
				  if(str.Right(1)=="y") // librarY -> librarIES
				  {
					  const char consonants[]={'b','c','d','f','g','h','j','k','l','m','n','p','q','r','s','t','v','w','x','y','z'};
					  int i=0;
					  for(;i<21;i++)
					  {
						  if(str.GetAt(str.GetLength()-2)==consonants[i])
						  {
							  SetDlgItemText(ID_USER+13,str.Left(str.GetLength()-1)+"ies");
							  return;
						  }
					  }
				  }
				  else if(str.Right(2)=="ch") // church -> churchES 
				  {
					  SetDlgItemText(ID_USER+13,str+"es");
					  return;
				  }
			  }
			  SetDlgItemText(ID_USER+13,str+"s");
		  }
		  if(m_englishWordKind.GetCurSel()==1) // Wortart: Verb
		  {
			  CString str;
			  GetDlgItemText(ID_USER+12,str); // englischen Infinitiv nach str kopieren
			  if(str.GetLength()>0)
			  {
				  if(str.Right(1)=="e") // zum Beispiel: to likE
				  {
					  SetDlgItemText(ID_USER+13,str+"d"); // zum Beispiel: likeD
					  return;
				  }
			  }
			  SetDlgItemText(ID_USER+13,str+"ed"); // zum Beispiel: walkED
		  }
		  if(m_englishWordKind.GetCurSel()==2) // Wortart: Adjektiv
		  {
			  CString str;
			  GetDlgItemText(ID_USER+12,str);
			  if(str.GetLength()>0)
			  {
				  //wenn der letzte Buchstabe EIN 'e' ist Beispiel: "fine"
				  if(str.Right(1)==_T("e"))
				  {
					  SetDlgItemText(ID_USER+13,str+"r");
					  SetDlgItemText(ID_USER+14,str+"st");
				  }
				  //wenn der letzte Buchstabe KEIN 'e' ist
				  else
				  {
					  SetDlgItemText(ID_USER+13,str+"er");
					  SetDlgItemText(ID_USER+14,str+"est");
				  }
			  }
			  else
			  {
				  SetDlgItemText(ID_USER+13,str+"er");
				  SetDlgItemText(ID_USER+14,str+"est");
			  }
			  SetDlgItemText(ID_USER+15,str+"ly");
		  }
	  }
    if(//TODO ensure validity of m_englishButton[6] in a better way.
      m_englishButton[6].m_hWnd )
      if( m_englishButton[6].GetCheck()) // englisches Perfekt regelmäßig-Kontrollkästchen
	  { // mist aktiviert
		  if(m_englishWordKind.GetCurSel() == WORDKIND_INDEX_FOR_VERB) // Wortart: Verb
		  {
			  CString str;
        // englischen Infinitiv nach str kopieren.
			  GetDlgItemText(ENGLISH_INFINITIVE_EDIT,str); 
			  if(str.GetLength()>0)
			  {
				  if(str.Right(1)=="e") // zum Beispiel: to likE
				  {
					  SetDlgItemText(ID_USER+14,str+"d"); // zum Beispiel: likeD
					  return;
				  }
			  }
			  SetDlgItemText(ID_USER+14,str+"ed"); // zum Beispiel: walkED
		  }
	  }
  }//if(nCurrentWordkindSelection!=WORDKIND_INDEX_FOR_PREPOSITION)
}

//wenn sich der Inhalt des Textfeldes des deutschen Singular/Infinitiv/
//Positiv ändert
void CVocableEditorBar::OnChangeIDUser20()
{
  int nCurrentWordkindSelection=m_englishWordKind.GetCurSel();
	//Wortart: Substantiv
	if(nCurrentWordkindSelection==WORDKIND_INDEX_FOR_NOUN)
	{
		if(m_germanButton[0].GetCheck())
		{
			CString strSingular;
			GetDlgItemText(ID_USER+34,strSingular);
			SetDlgItemText(ID_USER+35,strSingular);
		}
		if(m_germanButton[1].GetCheck())
		{
			CString strSingular;
			GetDlgItemText(ID_USER+34,strSingular);
			SetDlgItemText(ID_USER+35,strSingular+_T("e"));
		}
		/*if(m_germanButton[2].GetCheck())
		{
			CString strSingular;
			GetDlgItemText(ID_USER+34,strSingular);
			if(strSingular.GetLength()>0)
				if(strSingular.Right(1)==_T("e"))
					SetDlgItemText(ID_USER+35,strSingular+_T("n"));
				else
					SetDlgItemText(ID_USER+35,strSingular+_T("en"));
		}*/
		if(m_germanButton[3].GetCheck()) // wenn das 2. Kontrollkästchen (Präteritum)
		{ // 'regelmäßig' aktiviert ist
			CString strSingular;
			GetDlgItemText(ID_USER+34,strSingular);
			SetDlgItemText(ID_USER+35,strSingular+_T("er"));
		}
	}//Wortart: Substantiv
	//wenn Wortart "Verb" ist und das Kontrollkästchen "Wortstamm" im 
	//Präteritum NICHT aktiv ist
	if(nCurrentWordkindSelection==WORDKIND_INDEX_FOR_VERB && 
    !m_germanButton[3].GetCheck())
	{
		CString str;
		GetDlgItemText(ID_USER+34,str); // Infinitiv nach str kopieren
		if(str.GetLength()>1)
		{
			CString strWortstamm=str.Left(str.GetLength()-2);
			SetDlgItemText(ID_USER+49,strWortstamm);
		}
	}
	//wenn Wortart "Verb" ist und das Kontrollkästchen "Wortstamm" im 
	//Präsens im deutschen Teile des Vokabeleditors NICHT aktiv ist 
	if(nCurrentWordkindSelection==WORDKIND_INDEX_FOR_NOUN && 
    !m_germanButton[2].GetCheck())
	{
		// Wortart: Verb
		if(nCurrentWordkindSelection==WORDKIND_INDEX_FOR_NOUN)
		{
			CString str;
			GetDlgItemText(ID_USER+34,str); // Infinitiv nach str kopieren
			CString wortstamm;
			if(str.GetLength()>1)
			{
				wortstamm=str.Left(str.GetLength()-2); // Wortstamm entspricht str ohne die 
				// letzten 2 Buchstaben
				if(wortstamm.GetLength()>0)
					if(wortstamm.Right(1)=="t") // wenn letzter Buchstabe von wortstamm ein 't'?
					// Bsp: unterrichten
					{
						SetDlgItemText(ID_USER+35,wortstamm+"e");
						SetDlgItemText(ID_USER+36,wortstamm+"est");
						SetDlgItemText(ID_USER+37,wortstamm+"et");
						SetDlgItemText(ID_USER+38,wortstamm+"en");
						SetDlgItemText(ID_USER+39,wortstamm+"et");
					}
					else // Beispiel: gehen
					{
						SetDlgItemText(ID_USER+35,wortstamm+"e");
						SetDlgItemText(ID_USER+36,wortstamm+"st");
						SetDlgItemText(ID_USER+37,wortstamm+"t");
						SetDlgItemText(ID_USER+38,wortstamm+"en");
						SetDlgItemText(ID_USER+39,wortstamm+"t");
					}
			}
		}
		//Wortart: Adjektiv
		if(nCurrentWordkindSelection==WORDKIND_INDEX_FOR_ADJECTIVE)
		{
			CString str;
			GetDlgItemText(ID_USER+34,str);
			SetDlgItemText(ID_USER+35,str+"er");
			if(str.GetLength()>0)
			{
				//wenn der Buchstabe ganz rechts ein "t" ist
				if(str.Right(1)==_T("t"))
					//dann ein "esten" anhängen (Beispiel: bekannt->bekanntESTEN)
					SetDlgItemText(ID_USER+41,str+_T("esten"));
				else
					SetDlgItemText(ID_USER+41,str+_T("sten"));
			}
			else
				SetDlgItemText(ID_USER+41,str+_T("sten"));
			SetDlgItemText(ID_USER+47,str);
		}
	}
	//2. deutsches Kontrollkästchen "regelmäßig" / "Wortstamm" bei Verben 
	//im Präteritum
	/*if(m_germanButton[3].GetCheck())
	{ // (Präteritum) bei Deutsch
		if(m_englishWordKind.GetCurSel()==1) // Wortart: Verb
		{
			CString str;
			GetDlgItemText(ID_USER+34,str);
			CString wortstamm;
			if(str.GetLength()>1)
			{
				wortstamm=str.Left(str.GetLength()-2);
				if(wortstamm.GetLength()>0)
					if(wortstamm.Right(1)=="t") // Bsp: unterrichten
					{
						SetDlgItemText(ID_USER+41,wortstamm+"ete");
						SetDlgItemText(ID_USER+42,wortstamm+"etest");
						SetDlgItemText(ID_USER+43,wortstamm+"ete");
						SetDlgItemText(ID_USER+44,wortstamm+"eten");
						SetDlgItemText(ID_USER+45,wortstamm+"etet");
					}
					else // Beispiel: gucken
					{
						SetDlgItemText(ID_USER+41,wortstamm+"te");
						SetDlgItemText(ID_USER+42,wortstamm+"test");
						SetDlgItemText(ID_USER+43,wortstamm+"te");
						SetDlgItemText(ID_USER+44,wortstamm+"ten");
						SetDlgItemText(ID_USER+45,wortstamm+"tet");
					}
			}
		}
	}*/
  //In "PrepositionControls()" elements of "m_germanButton[]" 
  //are destroyed, so accessing them, (e.g. "m_germanButton[4].GetCheck()")
  //would led to a program crash.
  if(nCurrentWordkindSelection!=WORDKIND_INDEX_FOR_PREPOSITION)
    if(m_germanButton[4].m_hWnd)
	    //3. Kontrollkästchen "regelmäßig"
	    if(m_germanButton[4].GetCheck())
	    { // (Perfekt) im Deutschen
		    if(nCurrentWordkindSelection==WORDKIND_INDEX_FOR_VERB) // Wortart: Verb
		    {
			    CString str;
			    GetDlgItemText(ID_USER+34,str);
			    SetDlgItemText(ID_USER+47,"ge"+str);
		    }
	    }
}

void CVocableEditorBar::OnChangeIDUser40()
{
	//wenn das Kontrollkästchen "Wortstamm" (des Präsens) im deutschen 
	//Bereich des Vokabeleditors aktiviert ist
	if(m_germanButton[2].GetCheck())
	{
		CString strInfinitiv,strWortstamm;
#ifdef _DEBUG
		printf("m_germanButton[2].GetCheck()\n");
#endif
		GetDlgItemText(ID_USER+34,strInfinitiv);
		GetDlgItemText(ID_USER+40,strWortstamm);
		if(strInfinitiv.GetLength()>1)
		{
			CString strWortstammVomInfinitiv;
			strWortstammVomInfinitiv=strInfinitiv.Left(strInfinitiv.GetLength()-2);
			if(strWortstammVomInfinitiv.GetLength()>0)
			{
				//wenn der Wortstamm des Infinitivs (Infinitiv ohne die 2 ganz 
				//rechten Zeichen) mit dem Wortstamm des Präteritums übereinstimmt
				if(strWortstammVomInfinitiv==strWortstamm)
				{
					if(strWortstamm.Right(1)=="t") // Bsp: unterrichten (ohne 'en')
					{
						SetDlgItemText(ID_USER+35,strWortstamm+"e");
						SetDlgItemText(ID_USER+36,strWortstamm+"est");
						SetDlgItemText(ID_USER+37,strWortstamm+"et");
						SetDlgItemText(ID_USER+38,strWortstamm+"en");
						SetDlgItemText(ID_USER+39,strWortstamm+"et");
					}
					else // Beispiel: gehen
					{
						SetDlgItemText(ID_USER+35,strWortstamm+"e");
						SetDlgItemText(ID_USER+36,strWortstamm+"st");
						SetDlgItemText(ID_USER+37,strWortstamm+"t");
						SetDlgItemText(ID_USER+38,strWortstamm+"en");
						SetDlgItemText(ID_USER+39,strWortstamm+"t");
					}
				}
				//wenn der Wortstamm des Infinitivs (Infinitiv ohne die 2 ganz 
				//rechten Zeichen) mit dem Wortstamm des Präteritums NICHT 
				//übereinstimmt
				else
				{
					CStringVector sv;
#ifdef _DEBUG
					printf("!strWortstammVomInfinitiv==strWortstamm\n");
					printf("strWortstamm: %s\n",strWortstamm);
#endif
					sv=GetCStringVector2(strWortstamm);
#ifdef _DEBUG
					printf("sv.size(): %u\n",sv.size());
#endif
					//wenn die Zeichenfolge im Textfeld "Wortstamm" des Präteritums
					//ein getrenntes Verb (wenn also ein Leerzeichen zwischen zwei 
					//Zeichenketten) ist
					if(sv.size()>1)
					{
						//1. Person Singular ("ich")
						SetDlgItemText(ID_USER+35,sv.at(0)+_T("e ")+sv.at(1));
						//2. Person Singular ("du")
						SetDlgItemText(ID_USER+36,sv.at(0)+_T("st ")+sv.at(1));
						//3. Person Singular ("er/sie/es")
						SetDlgItemText(ID_USER+37,sv.at(0)+_T("t ")+sv.at(1));
						if(strWortstamm.GetLength()>0)
							//wenn der Buchstabe ganz rechts im Wortstamm ein "e" ist
							//(zum Beispiel "mochte")
							if(sv.at(0).Right(1)==_T("e"))
								//1. Person Plural ("wir")
								SetDlgItemText(ID_USER+38,sv.at(0)+_T("n ")+sv.at(1));
							else
								//1. Person Plural ("wir")
								SetDlgItemText(ID_USER+38,sv.at(0)+_T("en ")+sv.at(1));
						//2. Person Plural ("ihr")
						SetDlgItemText(ID_USER+39,sv.at(0)+_T("t ")+sv.at(1));
					}
					else
					{
						//1. Person Singular ("ich")
						SetDlgItemText(ID_USER+35,strWortstamm+_T("e"));
						//2. Person Singular ("du")
						SetDlgItemText(ID_USER+36,strWortstamm+_T("st"));
						//3. Person Singular ("er/sie/es")
						SetDlgItemText(ID_USER+37,strWortstamm+_T("t"));
						if(strWortstamm.GetLength()>0)
							//wenn der Buchstabe ganz rechts im Wortstamm ein "e" ist
							//(zum Beispiel "mochte")
							if(strWortstamm.Right(1)==_T("e"))
								//1. Person Plural ("wir")
								SetDlgItemText(ID_USER+38,strWortstamm+"n");
							else
								//1. Person Plural ("wir")
								SetDlgItemText(ID_USER+38,strWortstamm+"en");
						//2. Person Plural ("ihr")
						SetDlgItemText(ID_USER+39,strWortstamm+"t");
					}
				}
			}
		}
	}
}

void CVocableEditorBar::OnChangeIDUser49()
{
	//wenn das Kontrollkästchen "Wortstamm" (des Präteritums) im deutschen 
	//Bereich des Vokabeleditors aktiviert ist
	if(m_germanButton[3].GetCheck())
	{
		CString strInfinitiv,strWortstamm;
#ifdef _DEBUG
		printf("m_englishWordKind.GetCurSel()==1\n");
#endif
		GetDlgItemText(ID_USER+34,strInfinitiv);
		GetDlgItemText(ID_USER+49,strWortstamm);
		if(strInfinitiv.GetLength()>1)
		{
			CString strWortstammVomInfinitiv;
			strWortstammVomInfinitiv=strInfinitiv.Left(strInfinitiv.GetLength()-2);
			if(strWortstammVomInfinitiv.GetLength()>0)
			{
				//wenn der Wortstamm des Infinitivs (Infinitiv ohne die 2 ganz 
				//rechten Zeichen) mit dem Wortstamm des Präteritums übereinstimmt
				if(strWortstammVomInfinitiv==strWortstamm)
				{
					if(strWortstamm.Right(1)=="t") // Bsp: unterrichten (ohne 'en')
					{
						SetDlgItemText(ID_USER+41,strWortstamm+"ete");
						SetDlgItemText(ID_USER+42,strWortstamm+"etest");
						SetDlgItemText(ID_USER+43,strWortstamm+"ete");
						SetDlgItemText(ID_USER+44,strWortstamm+"eten");
						SetDlgItemText(ID_USER+45,strWortstamm+"etet");
					}
					else // Beispiel: gehen
					{
						SetDlgItemText(ID_USER+41,strWortstamm+"te");
						SetDlgItemText(ID_USER+42,strWortstamm+"test");
						SetDlgItemText(ID_USER+43,strWortstamm+"te");
						SetDlgItemText(ID_USER+44,strWortstamm+"ten");
						SetDlgItemText(ID_USER+45,strWortstamm+"tet");
					}
				}
				//wenn der Wortstamm des Infinitivs (Infinitiv ohne die 2 ganz 
				//rechten Zeichen) mit dem Wortstamm des Präteritums NICHT 
				//übereinstimmt
				else
				{
					CStringVector sv;
#ifdef _DEBUG
					printf("!strWortstammVomInfinitiv==strWortstamm\n");
					printf("strWortstamm: %s\n",strWortstamm);
#endif
					sv=GetCStringVector2(strWortstamm);
#ifdef _DEBUG
					printf("sv.size(): %u\n",sv.size());
#endif
					//wenn die Zeichenfolge im Textfeld "Wortstamm" des Präteritums
					//ein getrenntes Verb (wenn also ein Leerzeichen zwischen zwei 
					//Zeichenketten) ist
					if(sv.size()>1)
					{
						//1. Person Singular ("ich")
						SetDlgItemText(ID_USER+41,sv.at(0)+_T("e ")+sv.at(1));
						//2. Person Singular ("du")
						SetDlgItemText(ID_USER+42,sv.at(0)+_T("est ")+sv.at(1));
						//3. Person Singular ("er/sie/es")
						SetDlgItemText(ID_USER+43,sv.at(0)+_T("e ")+sv.at(1));
						if(strWortstamm.GetLength()>0)
						{
							//wenn der Buchstabe ganz rechts im Wortstamm ein "e" ist
							//(zum Beispiel "mochte")
							if(sv.at(0).Right(1)==_T("e"))
								//1. Person Plural ("wir")
								SetDlgItemText(ID_USER+44,sv.at(0)+_T("n ")+sv.at(1));
							else
								//1. Person Plural ("wir")
								SetDlgItemText(ID_USER+44,sv.at(0)+_T("en ")+sv.at(1));
							//wenn der Buchstabe ganz rechts im Wortstamm des 
							//Präteritums ein "t" ist (zum Beispiel "hielt an")
							if(strWortstamm.Right(1)==_T("t"))
								//2. Person Plural ("ihr")
								SetDlgItemText(ID_USER+45,strWortstamm+"et");
							else
								//2. Person Plural ("ihr")
								SetDlgItemText(ID_USER+45,strWortstamm+"t");
						}
					}
					else
					{
						//1. Person Singular ("ich")
						SetDlgItemText(ID_USER+41,strWortstamm);
						//2. Person Singular ("du")
						SetDlgItemText(ID_USER+42,strWortstamm+_T("st"));
						//3. Person Singular ("er/sie/es")
						SetDlgItemText(ID_USER+43,strWortstamm);
						if(strWortstamm.GetLength()>0)
						{
							//wenn der Buchstabe ganz rechts im Wortstamm ein "e" ist
							//(zum Beispiel "mochte")
							if(strWortstamm.Right(1)==_T("e"))
								//1. Person Plural ("wir")
								SetDlgItemText(ID_USER+44,strWortstamm+"n");
							else
								//1. Person Plural ("wir")
								SetDlgItemText(ID_USER+44,strWortstamm+"en");
							//wenn der Buchstabe ganz rechts im Wortstamm des 
							//Präteritums ein "t" ist (zum Beispiel "hielt")
							if(strWortstamm.Right(1)==_T("t"))
								//2. Person Plural ("ihr")
								SetDlgItemText(ID_USER+45,strWortstamm+"et");
							else
								//2. Person Plural ("ihr")
								SetDlgItemText(ID_USER+45,strWortstamm+"t");
						}
					}
				}
			}
		}
	}
}

//Kontrollkästchen "modales Hilfsverb"
void CVocableEditorBar::OnIDUser2()
{
/*		UpdateData(TRUE);
	if(m_more_most_check)
	{
		GetDlgItem(IDC_ENGLISH_REGULAR_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_ENGLISH_COL4_ROW1_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_ENGLISH_COL4_ROW2_EDIT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_ENGLISH_REGULAR_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_ENGLISH_COL4_ROW1_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_ENGLISH_COL4_ROW2_EDIT)->EnableWindow(TRUE);
	}*/
	//GetDlgItem(ID_USER+22)->EnableWindow(FALSE);

	if(m_englishButton[3].GetCheck())
	{
		//wenn die Wortart "Verb" ist
		if(m_englishWordKind.GetCurSel()==1)
			AuxiliaryVerbControls();

		if(m_englishWordKind.GetCurSel()==2) // Wortart: Adjektiv
		{
			CString str;
			GetDlgItemText(ID_USER+12,str);
			if(str.GetLength()>0)
			{
				//wenn der letzte Buchstabe EIN 'e' ist Beispiel: "fine"
				if(str.Right(1)==_T("e"))
				{
					SetDlgItemText(ID_USER+13,str+"r");
					SetDlgItemText(ID_USER+14,str+"st");
				}
				//wenn der letzte Buchstabe KEIN 'e' ist
				else
				{
					SetDlgItemText(ID_USER+13,str+"er");
					SetDlgItemText(ID_USER+14,str+"est");
				}
			}
			else
			{
				SetDlgItemText(ID_USER+13,str+"er");
				SetDlgItemText(ID_USER+14,str+"est");
			}
			SetDlgItemText(ID_USER+15,str+"ly");
			//dem Kontrollkästchen "more/most" das Häkchen wegnehmen, falls es 
			//markiert ist
			m_englishButton[5].SetCheck(0);
			//GetDlgItem(ID_USER+7)->EnableWindow(SW_SHOW);
			//GetDlgItem(ID_USER+10)->EnableWindow(SW_SHOW);
		}
	}
	else
	{
		//wenn die Wortart "Verb" ist
		if(m_englishWordKind.GetCurSel()==1)
			VerbControls(1);

	}
}

void CVocableEditorBar::OnIDUser4()
{
	//wenn das Kontrollkästchen aktiviert ist
	if(m_englishButton[5].GetCheck())
	{
		if(m_englishWordKind.GetCurSel() == WORDKIND_INDEX_FOR_NOUN) // Wortart: Substantiv
		{
			CString str;
			GetDlgItemText(ID_USER+12,str);
			if(str.GetLength()>0)
			{
				// ox -> oxES ; boss -> bossES
				if(str.Right(1)=="x" || str.Right(1)=="s")
				{
					SetDlgItemText(ID_USER+13,str+"es");
					return;
				}
			}
			if(str.GetLength()>1)
			{
				if(str.Right(1)=="y") // librarY -> librarIES
				{
					const char consonants[]={'b','c','d','f','g','h','j','k','l','m',
						'n','p','q','r','s','t','v','w','x','y','z'};
					int i=0;
					for(;i<21;i++)
					{
						if(str.GetAt(str.GetLength()-2)==consonants[i])
						{
							SetDlgItemText(ID_USER+13,str.Left(str.GetLength()-1)+"ies");
							return;
						}
					}
				}
				else if(str.Right(2)=="ch") // church -> churchES 
				{
					SetDlgItemText(ID_USER+13,str+"es");
					return;
				}
			}
			SetDlgItemText(ID_USER+13,str+"s");
		}
		//wenn die Wortart "Verb" ist
		if(m_englishWordKind.GetCurSel()==1)
		{
			CString str;
			GetDlgItemText(ID_USER+12,str);
			if(str.GetLength()>0)
			{
				//const char vowels[]={'a','e','i','o','u',};
				//for(BYTE b=0;b<5;b++)
				//{
				if(str.Right(1)=="e")
				{
					SetDlgItemText(ID_USER+13,str+"d");
					return;
				}
				//}
			}
			SetDlgItemText(ID_USER+13,str+"ed");
		}
		//wenn die Wortart "Adjektiv" ist
		if(m_englishWordKind.GetCurSel()==2)
			//dem Kontrollkästchen "regelmäßig" des englischen Bereiches des 
			//Vokabeleditors das Häkchen wegnehmen, falls es markiert ist
			m_englishButton[3].SetCheck(0);
	}
	else
	{
		if(m_englishWordKind.GetCurSel()==2) // Wortart: Adjektiv
		{
			GetDlgItem(ID_USER+5)->EnableWindow(SW_SHOW);
			GetDlgItem(ID_USER+7)->EnableWindow(SW_SHOW);
		}
	}
}

void CVocableEditorBar::OnIDUser5()
{
	if(m_englishButton[6].GetCheck())
	{
		//wenn die Wortart "Verb" ist
		if(m_englishWordKind.GetCurSel()==1)
		{
			CString str;
			GetDlgItemText(ID_USER+12,str);
			if(str.GetLength()>0)
			{
				//const char vowels[]={'a','e','i','o','u',};
				//for(BYTE b=0;b<5;b++)
				//{
				if(str.Right(1)=="e")
				{
					SetDlgItemText(ID_USER+14,str+"d");
					return;
				}
				//}
			}
			SetDlgItemText(ID_USER+14,str+"ed");
		}
		//wenn die Wortart "Adjektiv" ist
		if(m_englishWordKind.GetCurSel()==2)
		{
			//dem Kontrollkästchen "Adjektiv darf nur vor einem Substantiv
			//benutzt werden" des englischen Bereiches des 
			//Vokabeleditors das Häkchen wegnehmen, falls es markiert ist
			m_englishButton[10].SetCheck(0);
		}
	}
}

void CVocableEditorBar::OnIDUser8()
{
	//wenn die Wortart "Adjektiv" ist
	if(m_englishWordKind.GetCurSel()==2)
	{
		//dem Kontrollkästchen "Adjektiv darf nur nach einem Verb benutzt 
		//werden" des englischen Bereiches des 
		//Vokabeleditors das Häkchen wegnehmen, falls es markiert ist
		m_englishButton[6].SetCheck(0);
	}
	else
	{
		if(m_englishButton[10].GetCheck())
			if(m_englishButton[11].GetCheck())
				m_englishButton[11].SetCheck(0);
	}
}

void CVocableEditorBar::OnIDUser9()
{
	if(m_englishButton[11].GetCheck())
		if(m_englishButton[10].GetCheck())
			m_englishButton[10].SetCheck(0);
}

//wenn das 1. deutsche Kontrollkästchen 
//(Präsens/Adjektiv regelmäßig) angeklickt wird 
void CVocableEditorBar::OnIDUser16()
{
	if(m_germanButton[2].GetCheck())
	{
		if(m_englishWordKind.GetCurSel()==1) // Wortart: Verb
		{
			CString str;
			GetDlgItemText(ID_USER+34,str);
			CString wortstamm;
			if(str.GetLength()>1)
			{
				wortstamm=str.Left(str.GetLength()-2);
				if(wortstamm.GetLength()>0)
					if(wortstamm.Right(1)=="t") // Bsp: unterrichten (ohne 'en')
					{
						SetDlgItemText(ID_USER+35,wortstamm+"e");
						SetDlgItemText(ID_USER+36,wortstamm+"est");
						SetDlgItemText(ID_USER+37,wortstamm+"et");
						SetDlgItemText(ID_USER+38,wortstamm+"en");
						SetDlgItemText(ID_USER+39,wortstamm+"et");
					}
					else
					{
						SetDlgItemText(ID_USER+35,wortstamm+"e");
						SetDlgItemText(ID_USER+36,wortstamm+"st");
						SetDlgItemText(ID_USER+37,wortstamm+"t");
						SetDlgItemText(ID_USER+38,wortstamm+"en");
						SetDlgItemText(ID_USER+39,wortstamm+"t");
					}
			}
		}
		if(m_englishWordKind.GetCurSel()==2) // Wortart: Adjektiv
		{
			CString str;
			GetDlgItemText(ID_USER+34,str);
			SetDlgItemText(ID_USER+35,str+"er");
			SetDlgItemText(ID_USER+41,str+"sten");
			SetDlgItemText(ID_USER+47,str);
		}
	}
}

//wenn das 2. Kontrollkästchen im deutschen Teil des Vokabeleditors
void CVocableEditorBar::OnIDUser17()
{
#ifdef _DEBUG
	printf("CVocableEditorBar::OnIDUser17()\n");
#endif
	if(m_germanButton[3].GetCheck()) // wenn das 2. Kontrollkästchen (Präteritum)
	{ // 'regelmäßig' aktiviert ist
		//wenn die Wortart "Substantiv" ist
		if(m_englishWordKind.GetCurSel()==0)
		{
			CString strSingular;
			GetDlgItemText(ID_USER+34,strSingular);
			SetDlgItemText(ID_USER+35,strSingular+_T("er"));
			m_germanButton[0].SetCheck(0);
			m_germanButton[1].SetCheck(0);
			m_germanButton[2].SetCheck(0);
		}
		//wenn die Wortart "Verb" ist
		if(m_englishWordKind.GetCurSel()==1)
		{
			CString strInfinitiv,strWortstamm;
#ifdef _DEBUG
			printf("m_englishWordKind.GetCurSel()==1\n");
#endif
			GetDlgItemText(ID_USER+34,strInfinitiv);
			GetDlgItemText(ID_USER+49,strWortstamm);
			if(strInfinitiv.GetLength()>1)
			{
				CString strWortstammVomInfinitiv;
				strWortstammVomInfinitiv=strInfinitiv.Left(strInfinitiv.GetLength()-2);
				if(strWortstammVomInfinitiv.GetLength()>0)
				{
					//wenn der Wortstamm des Infinitivs (Infinitiv ohne die 2 ganz 
					//rechten Zeichen) mit dem Wortstamm des Präteritums übereinstimmt
					if(strWortstammVomInfinitiv==strWortstamm)
					{
						if(strWortstamm.Right(1)=="t") // Bsp: unterrichten (ohne 'en')
						{
							SetDlgItemText(ID_USER+41,strWortstamm+"ete");
							SetDlgItemText(ID_USER+42,strWortstamm+"etest");
							SetDlgItemText(ID_USER+43,strWortstamm+"ete");
							SetDlgItemText(ID_USER+44,strWortstamm+"eten");
							SetDlgItemText(ID_USER+45,strWortstamm+"etet");
						}
						else // Beispiel: gehen
						{
							SetDlgItemText(ID_USER+41,strWortstamm+"te");
							SetDlgItemText(ID_USER+42,strWortstamm+"test");
							SetDlgItemText(ID_USER+43,strWortstamm+"te");
							SetDlgItemText(ID_USER+44,strWortstamm+"ten");
							SetDlgItemText(ID_USER+45,strWortstamm+"tet");
						}
					}
					//wenn der Wortstamm des Infinitivs (Infinitiv ohne die 2 ganz 
					//rechten Zeichen) mit dem Wortstamm des Präteritums NICHT 
					//übereinstimmt
					else
					{
						CStringVector sv;
#ifdef _DEBUG
						printf("!strWortstammVomInfinitiv==strWortstamm\n");
						printf("strWortstamm: %s\n",strWortstamm);
#endif
						sv=GetCStringVector2(strWortstamm);
#ifdef _DEBUG
						printf("sv.size(): %u\n",sv.size());
#endif
						//wenn die Zeichenfolge im Textfeld "Wortstamm" des Präteritums
						//ein getrenntes Verb (wenn also ein Leerzeichen zwischen zwei 
						//Zeichenketten) ist
						if(sv.size()>1)
						{
							//1. Person Singular ("ich")
							SetDlgItemText(ID_USER+41,sv.at(0)+_T("e ")+sv.at(1));
							//2. Person Singular ("du")
							SetDlgItemText(ID_USER+42,sv.at(0)+_T("est ")+sv.at(1));
							//3. Person Singular ("er/sie/es")
							SetDlgItemText(ID_USER+43,sv.at(0)+_T("e ")+sv.at(1));
							if(strWortstamm.GetLength()>0)
							{
								//wenn der Buchstabe ganz rechts im Wortstamm ein "e" ist
								//(zum Beispiel "mochte")
								if(sv.at(0).Right(1)==_T("e"))
									//1. Person Plural ("wir")
									SetDlgItemText(ID_USER+44,sv.at(0)+_T("n ")+sv.at(1));
								else
									//1. Person Plural ("wir")
									SetDlgItemText(ID_USER+44,sv.at(0)+_T("en ")+sv.at(1));
								//wenn der Buchstabe ganz rechts im Wortstamm des 
								//Präteritums ein "t" ist (zum Beispiel "hielt an")
								if(strWortstamm.Right(1)==_T("t"))
									//2. Person Plural ("ihr")
									SetDlgItemText(ID_USER+45,strWortstamm+"et");
								else
									//2. Person Plural ("ihr")
									SetDlgItemText(ID_USER+45,strWortstamm+"t");
							}
						}
						else
						{
							//1. Person Singular ("ich")
							SetDlgItemText(ID_USER+41,strWortstamm);
							//2. Person Singular ("du")
							SetDlgItemText(ID_USER+42,strWortstamm+_T("st"));
							//3. Person Singular ("er/sie/es")
							SetDlgItemText(ID_USER+43,strWortstamm);
							if(strWortstamm.GetLength()>0)
							{
								//wenn der Buchstabe ganz rechts im Wortstamm ein "e" ist
								//(zum Beispiel "mochte")
								if(strWortstamm.Right(1)==_T("e"))
									//1. Person Plural ("wir")
									SetDlgItemText(ID_USER+44,strWortstamm+"n");
								else
									//1. Person Plural ("wir")
									SetDlgItemText(ID_USER+44,strWortstamm+"en");
								//wenn der Buchstabe ganz rechts im Wortstamm des 
								//Präteritums ein "t" ist (zum Beispiel "hielt")
								if(strWortstamm.Right(1)==_T("t"))
									//2. Person Plural ("ihr")
									SetDlgItemText(ID_USER+45,strWortstamm+"et");
								else
									//2. Person Plural ("ihr")
									SetDlgItemText(ID_USER+45,strWortstamm+"t");
							}
						}
					}
				}
			}
		}
	}
}

//das 3. deutsche Kontrollkästchen 'regelmäßig' (Perfekt)
void CVocableEditorBar::OnIDUser18()
{
	if(m_germanButton[4].GetCheck())
	{
		if(m_germanButton[8].GetCheck())
			m_germanButton[8].SetCheck(0);
		if(m_englishWordKind.GetCurSel()==1) // Wortart: Verb
		{
			CString str;
			GetDlgItemText(ID_USER+34,str);
			SetDlgItemText(ID_USER+47,"ge"+str);
		}
	}
}

//Kontrollkästchen "~"
void CVocableEditorBar::OnIDUser22()
{
	if(m_englishWordKind.GetCurSel()==0 && m_germanButton[0].GetCheck())
	{
		CString strSingular;
		GetDlgItemText(ID_USER+34,strSingular);
		SetDlgItemText(ID_USER+35,strSingular);
		m_germanButton[1].SetCheck(0);
		m_germanButton[2].SetCheck(0);
		m_germanButton[3].SetCheck(0);
	}
}

//Kontrollkästchen "~e"
void CVocableEditorBar::OnIDUser23()
{
	if(m_englishWordKind.GetCurSel()==0 && m_germanButton[1].GetCheck())
	{
		CString strSingular;
		GetDlgItemText(ID_USER+34,strSingular);
		SetDlgItemText(ID_USER+35,strSingular+_T("e"));
		m_germanButton[0].SetCheck(0);
		m_germanButton[2].SetCheck(0);
		m_germanButton[3].SetCheck(0);
	}
}

//Kontrollkästchen "~en/~n"
void CVocableEditorBar::OnIDUser24()
{
	//wenn die Wortart "Substantiv" ist und das Kontrollkästchen "~en/~n" 
	//aktiviert ist
	if(m_englishWordKind.GetCurSel()==0 && m_germanButton[2].GetCheck())
	{
		CString strSingular;
		GetDlgItemText(ID_USER+34,strSingular);
		if(strSingular.GetLength()>0)
			if(strSingular.Right(1)==_T("e"))
				SetDlgItemText(ID_USER+35,strSingular+_T("n"));
			else
				SetDlgItemText(ID_USER+35,strSingular+_T("en"));
		m_germanButton[0].SetCheck(0);
		m_germanButton[1].SetCheck(0);
		m_germanButton[3].SetCheck(0);
	}
	//wenn die Wortart "Verb" ist und das Kontrollkästchen "regelmäßig",
	//das sich in der Spalte 'Präsens' befindet, aktiviert ist
	if(m_englishWordKind.GetCurSel()==1 && m_germanButton[2].GetCheck())
	{
		CString strInfinitiv,strWortstamm;
#ifdef _DEBUG
		printf("m_germanButton[2].GetCheck()\n");
#endif
		GetDlgItemText(ID_USER+34,strInfinitiv);
		GetDlgItemText(ID_USER+40,strWortstamm);
		if(strInfinitiv.GetLength()>1)
		{
			CString strWortstammVomInfinitiv;
			strWortstammVomInfinitiv=strInfinitiv.Left(strInfinitiv.GetLength()-2);
			if(strWortstammVomInfinitiv.GetLength()>0)
			{
				//wenn der Wortstamm des Infinitivs (Infinitiv ohne die 2 ganz 
				//rechten Zeichen) mit dem Wortstamm des Präteritums übereinstimmt
				if(strWortstammVomInfinitiv==strWortstamm)
				{
					if(strWortstamm.Right(1)=="t") // Bsp: unterrichten (ohne 'en')
					{
						SetDlgItemText(ID_USER+35,strWortstamm+"e");
						SetDlgItemText(ID_USER+36,strWortstamm+"est");
						SetDlgItemText(ID_USER+37,strWortstamm+"et");
						SetDlgItemText(ID_USER+38,strWortstamm+"en");
						SetDlgItemText(ID_USER+39,strWortstamm+"et");
					}
					else // Beispiel: gehen
					{
						SetDlgItemText(ID_USER+35,strWortstamm+"e");
						SetDlgItemText(ID_USER+36,strWortstamm+"st");
						SetDlgItemText(ID_USER+37,strWortstamm+"t");
						SetDlgItemText(ID_USER+38,strWortstamm+"en");
						SetDlgItemText(ID_USER+39,strWortstamm+"t");
					}
				}
				//wenn der Wortstamm des Infinitivs (Infinitiv ohne die 2 ganz 
				//rechten Zeichen) mit dem Wortstamm des Präteritums NICHT 
				//übereinstimmt
				else
				{
					CStringVector sv;
#ifdef _DEBUG
					printf("!strWortstammVomInfinitiv==strWortstamm\n");
					printf("strWortstamm: %s\n",strWortstamm);
#endif
					sv=GetCStringVector2(strWortstamm);
#ifdef _DEBUG
					printf("sv.size(): %u\n",sv.size());
#endif
					//wenn die Zeichenfolge im Textfeld "Wortstamm" des Präteritums
					//ein getrenntes Verb (wenn also ein Leerzeichen zwischen zwei 
					//Zeichenketten) ist
					if(sv.size()>1)
					{
						//1. Person Singular ("ich")
						SetDlgItemText(ID_USER+35,sv.at(0)+_T("e ")+sv.at(1));
						//2. Person Singular ("du")
						SetDlgItemText(ID_USER+36,sv.at(0)+_T("st ")+sv.at(1));
						//3. Person Singular ("er/sie/es")
						SetDlgItemText(ID_USER+37,sv.at(0)+_T("t ")+sv.at(1));
						if(strWortstamm.GetLength()>0)
							//wenn der Buchstabe ganz rechts im Wortstamm ein "e" ist
							//(zum Beispiel "mochte")
							if(sv.at(0).Right(1)==_T("e"))
								//1. Person Plural ("wir")
								SetDlgItemText(ID_USER+38,sv.at(0)+_T("n ")+sv.at(1));
							else
								//1. Person Plural ("wir")
								SetDlgItemText(ID_USER+38,sv.at(0)+_T("en ")+sv.at(1));
						//2. Person Plural ("ihr")
						SetDlgItemText(ID_USER+39,sv.at(0)+_T("t ")+sv.at(1));
					}
					else
					{
						//1. Person Singular ("ich")
						SetDlgItemText(ID_USER+35,strWortstamm+_T("e"));
						//2. Person Singular ("du")
						SetDlgItemText(ID_USER+36,strWortstamm+_T("st"));
						//3. Person Singular ("er/sie/es")
						SetDlgItemText(ID_USER+37,strWortstamm+_T("t"));
						if(strWortstamm.GetLength()>0)
							//wenn der Buchstabe ganz rechts im Wortstamm ein "e" ist
							//(zum Beispiel "mochte")
							if(strWortstamm.Right(1)==_T("e"))
								//1. Person Plural ("wir")
								SetDlgItemText(ID_USER+38,strWortstamm+"n");
							else
								//1. Person Plural ("wir")
								SetDlgItemText(ID_USER+38,strWortstamm+"en");
						//2. Person Plural ("ihr")
						SetDlgItemText(ID_USER+39,strWortstamm+"t");
					}
				}
			}
		}
	}
	//wenn die Wortart "Adjektiv" ist und das Kontrollkästchen "regelmäßig" 
	//aktiviert ist
	if(m_englishWordKind.GetCurSel()==2 && m_germanButton[2].GetCheck())
	{
		CString strPositiv;
		GetDlgItemText(ID_USER+34,strPositiv);
		SetDlgItemText(ID_USER+35,strPositiv+_T("er"));
		if(strPositiv.GetLength()>0)
		{
			//wenn der Buchstabe ganz rechts ein "t" ist
			if(strPositiv.Right(1)==_T("t"))
				//dann ein "esten" anhängen (Beispiel: bekannt->bekanntESTEN)
				SetDlgItemText(ID_USER+41,strPositiv+_T("esten"));
			else
				SetDlgItemText(ID_USER+41,strPositiv+_T("sten"));
		}
		else
			SetDlgItemText(ID_USER+41,strPositiv+_T("sten"));
		SetDlgItemText(ID_USER+47,strPositiv);
	}
}

//Kontrollkästchen 'kein Fall'
void CVocableEditorBar::OnIDUser25()
{

	if(m_germanButton[5].GetCheck())
	{
		if(m_germanButton[6].GetCheck())
			m_germanButton[6].SetCheck(0);
		if(m_germanButton[7].GetCheck())
			m_germanButton[7].SetCheck(0);
	}
	else
	{
		m_germanButton[5].SetCheck(1);
	}
}

//Kontrollkästchen '3. Fall' 'regelmäßig' (Perfekt)
void CVocableEditorBar::OnIDUser26()
{
	if(!m_germanButton[6].GetCheck())
	{
		if(!m_germanButton[7].GetCheck())
			m_germanButton[6].SetCheck(1);
	}
	else
	{
		if(m_germanButton[5].GetCheck())
		{
			m_germanButton[5].SetCheck(0);
		}
	}
}

//Kontrollkästchen '4. Fall' 'regelmäßig' (Perfekt)
void CVocableEditorBar::OnIDUser27()
{
	if(!m_germanButton[7].GetCheck())
	{
		if(!m_germanButton[6].GetCheck())
			m_germanButton[7].SetCheck(1);
	}
	else
	{
		if(m_germanButton[5].GetCheck())
		{
			m_germanButton[5].SetCheck(0);
		}
	}
}

//wenn das 2. deutsche Kontrollkästchen
void CVocableEditorBar::OnIDUser30()
{
	//wenn das Kontrollkästchen "ge~-2t"
	if(m_germanButton[8].GetCheck())
	{
		if(m_germanButton[4].GetCheck())
			m_germanButton[4].SetCheck(0);
		if(m_englishWordKind.GetCurSel()==1) // Wortart: Verb
		{
			CString strInfinitiv;
			GetDlgItemText(ID_USER+34,strInfinitiv);
			if(strInfinitiv.GetLength()>1)
			{
				SetDlgItemText(ID_USER+47,"ge"+strInfinitiv.Left(strInfinitiv.
					GetLength()-2)+"t");
			}
		}
	}
}

// bei Adjektiven: Kontrollkästchen 'more/most'
// bei Konjunktionen: Kontrollkästchen 'Subjekt Prädikat [Objekt]'
void CVocableEditorBar::OnIDUser72()
{
	//wenn die Wortart "Adjektiv" ist
	if(m_englishWordKind.GetCurSel() == WORDKIND_INDEX_FOR_ADJECTIVE)
	{
		if(m_englishButton[17].GetCheck())
		{
			GetDlgItem(ID_USER+13)->EnableWindow(SW_HIDE);
			GetDlgItem(ID_USER+14)->EnableWindow(SW_HIDE);
			m_englishButton[5].SetCheck(0);
		}
		else
		{
			GetDlgItem(ID_USER+13)->EnableWindow(SW_SHOW);
			GetDlgItem(ID_USER+14)->EnableWindow(SW_SHOW);
		}
	}
}