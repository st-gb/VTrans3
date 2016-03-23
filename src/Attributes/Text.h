#if !defined (TEXT_DEFINED)
#define TEXT_DEFINED

#ifdef _WINDOWS
	#include <atlstr.h>//This include is needed for "CString" when compiling as DLL.
#endif
#ifndef __AFXSTR_H__
//#include <StdString.h>
#endif
#ifndef _WINDEF_
	#include "typedefs.h"
#endif
#include "rest.h"

class TextNode
{
public:
	BYTE ch;
	BYTE color;
	TextNode * m_next;
	TextNode * m_previous;
	//untypisierter Zeiger auf zusätzliche Attribute wie z.B.
	//Schriftstil (fett/kursiv)
	void * m_pvAttributes;
	TextNode(BYTE ch2,BYTE color2)
	{
		ch=ch2;
		color=color2;
		m_next=NULL;
		m_previous=NULL;
	};
	TextNode(){};
};

class Text
{
public:
	//CString str;
	TextNode * m_first;
	void Clear();
	void Delete(DWORD);
	void Delete(DWORD dwPos, DWORD dwLenght);
	BOOL GetCString(CString &,DWORD,DWORD);
	DWORD GetLength();
	TextNode * GetPointerToPos(DWORD dwPos);
	void Insert(DWORD,unsigned char);
	void Insert(DWORD dw,BYTE ch,BYTE color);
	void Insert(DWORD dw,LPSTR szStr,BYTE color);
	BOOL SetColor(DWORD dwIndex,BYTE color);
	DWORD GetTextLength();
	Text()
	{
		m_first=NULL;
	};
};

#endif
