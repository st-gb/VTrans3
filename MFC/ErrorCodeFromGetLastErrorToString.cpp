//Wenn der Compiler-Fehler "C1010" auftritt, dann liegt das
//bestimmt an der Befehlszeilenoption /Yu (Verwenden einer 
//vorkompilierten Header-Datei) siehe Menü "Projekt" 
//Menüpunkt "Einstellungen..." Karteireiter "C/C++" Textbox
//"Projekt Optionen" Befehlszeilenoption "Yu" (in der 
//Textbox unten, also eventuell herunterscrollen)
//dann entweder die direkt hinter "Yu" angegebene Datei 
//einbinden ("#include ") oder die Befehlszeilenoption "/Yu"
//entfernen
#include "../stdafx.h"
#include "ErrorCodeFromGetLastErrorToString.h"
//diese Funktion liefert die zum DWORD-(unsigned long-) Wert 
//der Windows-API-Funktion "DWORD GetLastError(void)" 
//korrepsondierende Fehlermeldung als Text in der Standard-
//Sprache der laufenden Windows-Sitzung
CString GetLastErrorMessageString(DWORD 
	dwErrorCodeConformToGetLastError)
{
	CString strErrorMessage;
	LPVOID lpMsgBuf = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, 
		dwErrorCodeConformToGetLastError, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf, 0, NULL );

	strErrorMessage = (LPCTSTR)lpMsgBuf;

	if( lpMsgBuf != NULL )
		LocalFree(lpMsgBuf);

	return strErrorMessage;
}
