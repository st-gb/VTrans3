// stdafx.h : Include-Datei f�r Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die h�ufig benutzt, aber
//      in unregelm��igen Abst�nden ge�ndert werden.
//

#if !defined(AFX_STDAFX_H__FB6F08E4_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
     #define AFX_STDAFX_H__FB6F08E4_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_

//wenn dieses Makro definiert ist, erfolgen auch in der Release-Version
//Ausgaben in die log-Datei sofern die Quelltext-Dateien, in denen die 
//printf-Anweisungen stehen, diese Header-Datei (StdAfx.h) einbinden
/*#ifndef _DEBUG
	#define _DEBUG
#endif*/

#ifndef ID_USER
  //der Wert wird empfohlen in MSDN Library Visual Studio 6.0,
	//Artikel: "TN022: Standard Commands Implementation"
	//�berschrift: "ID Ranges"
	//"Your application should define its commands in the range 0x8000 to 0xDFFF."
	//bei einem Wert von 0x8000 unter Windows XP waren einige statische Texte
	//(CStatic) deaktiviert, also  grau, deswegen habe ich 0x9000 gew�hlt
	//TN020:
	//Overview of ID-Numbering Convention
	//The following lists the valid ranges for the IDs of the specific types. Some of the limits are technical implementation limits while others are just conventions to prevent your IDs from colliding with Windows predefined IDs or MFC default implementations.
	//We strongly recommend you do not defined IDs outside the recommended ranges. Even though the lower limit of many of these ranges is 1 (0 is not used), common convention starts practical use of IDs at 100 or 101.
	//Prefix Resource Type Valid Range 
	//IDR_ multiple 1 -> 0x6FFF 
	//IDD_ dialog templates 1 -> 0x6FFF 
	//IDC_,IDI_,IDB_ cursors, icons, bitmaps 1 -> 0x6FFF 
	//IDS_, IDP_ general strings 1 -> 0x7FFF 
	//ID_ commands 0x8000 -> 0xDFFF 
	//IDC_ controls 8 -> 0xDFFF 
	//Reasons for these range limits: 
	//By convention, the ID value of 0 is not used.
	//Windows implementation limitations restrict true resource IDs to be less than or equal to 0x7FFF.
	//MFC's internal framework implementations reserve several ranges: 0xE000->0xEFFF and 0x7000->0x7FFF.
	//Several Windows system commands use the range of 0xF000 -> 0xFFFF.
	//Control IDs of 1->7 are reserved by IDOK, IDCANCEL, and so on.
	//The range of 0x8000->0xFFFF for strings is reserved for menu prompts for commands. 
	#define ID_USER 1100
#endif //#ifndef ID_USER

//#ifdef _WINDOWS
 #ifdef _MSC_VER //if MS compiler
  #if _MSC_VER > 1000
    #pragma once
  #endif // _MSC_VER > 1000

  #define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden

  #include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
  #include <afxext.h>         // MFC-Erweiterungen
  #include <afxdisp.h>        // MFC Automatisierungsklassen
  #include <afxdtctl.h>		// MFC-Unterst�tzung f�r allgemeine Steuerelemente von Internet Explorer 4
  #ifndef _AFX_NO_AFXCMN_SUPPORT
    #include <afxcmn.h>			// MFC-Unterst�tzung f�r g�ngige Windows-Steuerelemente
  #endif // _AFX_NO_AFXCMN_SUPPORT

  //{{AFX_INSERT_LOCATION}}
  // Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.
 #else
//  #include <MFC_Compatibility/CString.hpp>
//  using namespace MFC_Compatibility ; //for class CString in MFC_Compatibility
 #endif //#ifdef _MSC_VER
#endif // !defined(AFX_STDAFX_H__FB6F08E4_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
