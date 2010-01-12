#if !defined(ERRORCODEFROMGETLASTERRORTOSTRING_H__INCLUDED_)
#define ERRORCODEFROMGETLASTERRORTOSTRING_H__INCLUDED_

#ifdef _WINDOWS
	//damit nicht der Compiler-Fehler C2146 auftritt 
	#include <Afx.h>
#endif

//diese Funktion liefert die zum DWORD-(unsigned long-) Wert 
//der Windows-API-Funktion "DWORD GetLastError(void)" 
//korrepsondierende Fehlermeldung als Text in der Standard-
//Sprache der laufenden Windows-Sitzung
CString GetLastErrorMessageString(DWORD 
	dwErrorCodeConformToGetLastError);

#endif // !defined(ERRORCODEFROMGETLASTERRORTOSTRING_H__INCLUDED_)