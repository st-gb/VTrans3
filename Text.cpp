#include "StdAfx.h"
#include "Text.h"

void Text::Clear()
{
	if(m_first!=NULL)
	{
		TextNode * previous=NULL;
		TextNode * current=m_first;
		while(current->m_next)
		{
			previous=current;
			current=current->m_next;
			delete previous; // Speicher freigeben
		}
		delete current;
		m_first=NULL;
	}
}

void Text::Delete(DWORD dwPos)
{
	if(m_first)
	{
		BYTE bPreviousCharacterWasCarriageReturn=0;
		DWORD dwRun=0;
		TextNode * textNodeCurrent=m_first;
		//in dieser Schleife wird bis zum Listenelement, das die Position dwPos
		//repräsentiert, immer das nächste Listenelement angesprungen
		while(1)//dwRun<dwPos+1)
		{
			//wenn das Zeichen ein newline ist (bei Text von Textdatei beim
			//Betriebssystem Windows wird eine neue Zeile durch die Bytefolge
			//13 10 (="carriage return" "newline") eingeleitet)...
			//if(textNodeCurrent->ch==10 && bPreviousCharacterWasCarriageReturn)
			//...dann einen Indexpunkt abziehen, damit ein 13 10 nicht doppelt 
			//gezählt wird
			//	dwRun--;
			dwRun++;
			if(textNodeCurrent->ch==13 && textNodeCurrent->m_next)
			{
#ifdef _DEBUG
				printf("textNodeCurrent->ch==13 && textNodeCurrent->m_next\n");
#endif
				if(textNodeCurrent->m_next->ch==10)
				{
#ifdef _DEBUG
					printf("textNodeCurrent->m_next->ch==10\n");
#endif
					//if(dwRun>=dwPos)
					//	textNodeCurrent=textNodeCurrent->m_next;
					dwRun--;
					//bIncrementdwRun;
				}
			}
			if(dwRun==dwPos+1)
				break;
			//	bPreviousCharacterWasCarriageReturn=1;
			//else
			//	bPreviousCharacterWasCarriageReturn=0;
			if(textNodeCurrent->m_next)
				textNodeCurrent=textNodeCurrent->m_next;
			else
				break;
		}
#ifdef _DEBUG
		printf("dwRun:%u dwPos:%u\n",dwRun,dwPos);
#endif
		if(dwRun==dwPos+1)
		{
#ifdef _DEBUG
			printf("textNodeCurrent->ch: %c %u\n",textNodeCurrent->ch,
				textNodeCurrent->ch);
#endif
			if(textNodeCurrent->m_next) // current ist nicht das letzte Listenelement
			{
				if(textNodeCurrent->m_previous) // current ist nicht das erste Listenelement
				{
					TextNode * textNodeNext=textNodeCurrent->m_next;
					TextNode * textNodePrevious=textNodeCurrent->m_previous;
					//wenn direkt vor der aktuellen Cursorposition die Bytefolge
					//13 10 steht
					if(textNodeCurrent->ch==10 && textNodePrevious->ch==13)
					{
						//wenn textNodePrevious NICHT das erste Listenelement ist
						if(textNodePrevious->m_previous)
						{
							//das vorvorletzte Listenelement als neues Listenelement 
							//markieren
							textNodePrevious->m_previous->m_next=textNodeNext;
							//Speicher des vorletzten Listenelements freigeben
							delete textNodePrevious;
						}
						//wenn textNodePrevious das erste Listenelement ist
						else
						{
							//Speicher des vorletzten Listenelements freigeben
							delete textNodePrevious;
							//anzeigen, dass die Liste komplett leer ist
							m_first=textNodeNext;
						}
					}
					else
					{
						textNodeNext->m_previous=textNodePrevious;
						textNodePrevious->m_next=textNodeNext;
					}
					delete textNodeCurrent;
				}
				else // current zeigt auf das erste Listenelement
				{
					m_first=textNodeCurrent->m_next;
					delete textNodeCurrent;
					m_first->m_previous=NULL;
					//m_first=NULL;
				}
			}
			else // textNodeCurrent zeigt auf das letzte Listenelement
			{
#ifdef _DEBUG
				printf("textNodeCurrent zeigt auf das letzte Listenelement\n");
#endif
				if(textNodeCurrent->m_previous) // current ist nicht das erste Listenelement
				{
					TextNode * textNodePrevious=textNodeCurrent->m_previous;
					//wenn direkt vor der aktuellen Cursorposition die Bytefolge
					//13 10 steht
					if(textNodeCurrent->ch==10 && textNodePrevious->ch==13)
					{
#ifdef _DEBUG
						printf("textNodeCurrent->ch==10 && textNodePrevious->ch==13\n");
#endif
						//wenn textNodePrevious NICHT das erste Listenelement ist
						if(textNodePrevious->m_previous)
						{
							TextNode * textNodePreviousPrevious=textNodePrevious->
								m_previous;
							//Speicher des vorletzten Listenelements freigeben
							delete textNodePrevious;
							//das vorvorletzte Listenelement als neues Listenelement 
							//markieren
							textNodePreviousPrevious->m_next=NULL;
						}
						//wenn textNodePrevious das erste Listenelement ist
						else
						{
							//Speicher des vorletzten Listenelements freigeben
							delete textNodePrevious;
							//anzeigen, dass die Liste komplett leer ist
							m_first=NULL;
						}
					}
					else
						textNodePrevious->m_next=NULL;
					delete textNodeCurrent;
				}
				//current zeigt auf das erste Listenelement (es ist also nur 1
				//Listenelement in der Liste vorhanden)
				else
				{
					m_first=NULL;
					delete textNodeCurrent;
				}
			}
		}
	}
}

void Text::Delete(DWORD dwPos, DWORD dwLength)
{
#ifdef _DEBUG
	printf("Text::Delete(DWORD dwPos, DWORD dwLength) - ANFANG\n");
#endif
	if(m_first)
	{
		BYTE bPreviousCharacterWasCarriageReturn=0;
		DWORD dwRun=0;
		TextNode * textNodeCurrent=m_first;
		//in dieser Schleife wird bis zum Listenelement, das die Position dwPos
		//repräsentiert, immer das nächste Listenelement angesprungen
		while(1)//dwRun<dwPos+1)
		{
			//wenn das Zeichen ein newline ist (bei Text von Textdatei beim
			//Betriebssystem Windows wird eine neue Zeile durch die Bytefolge
			//13 10 (="carriage return" "newline") eingeleitet)...
			//if(textNodeCurrent->ch==10 && bPreviousCharacterWasCarriageReturn)
			//...dann einen Indexpunkt abziehen, damit ein 13 10 nicht doppelt 
			//gezählt wird
			//	dwRun--;
			dwRun++;
			if(textNodeCurrent->ch==13 && textNodeCurrent->m_next)
			{
#ifdef _DEBUG
				printf("textNodeCurrent->ch==13 && textNodeCurrent->m_next\n");
#endif
				if(textNodeCurrent->m_next->ch==10)
				{
#ifdef _DEBUG
					printf("textNodeCurrent->m_next->ch==10\n");
#endif
					//if(dwRun>=dwPos)
					//	textNodeCurrent=textNodeCurrent->m_next;
					dwRun--;
					//bIncrementdwRun;
				}
			}
			if(dwRun==dwPos+1)
				break;
			//	bPreviousCharacterWasCarriageReturn=1;
			//else
			//	bPreviousCharacterWasCarriageReturn=0;
			if(textNodeCurrent->m_next)
				textNodeCurrent=textNodeCurrent->m_next;
			else
				break;
		}
#ifdef _DEBUG
		printf("dwRun:%u dwPos:%u\n",dwRun,dwPos);
#endif
		if(dwRun==dwPos+1)
		{
			TextNode * textNodeBeforeDeletion=NULL;
			//wenn textNodeCurrent NICHT auf das erste Element der verketteten
			//Liste zeigt
			if(textNodeCurrent->m_previous)
				textNodeBeforeDeletion=textNodeCurrent->m_previous;
#ifdef _DEBUG
			printf("dwLength %u\n",dwLength);
#endif
			//Schleife dwLength Mal durchlaufen
			while(dwLength--)
			{
#ifdef _DEBUG
				printf("textNodeCurrent->ch: %c %u dwLength: %u\n",textNodeCurrent->
					ch,textNodeCurrent->ch,dwLength);
#endif
				//wenn textNodeCurrent NICHT auf das letzte Listenelement zeigt
				if(textNodeCurrent->m_next)
				{
					//textNodeNext auf das nächste Listenelement von textNodeCurrent
					//verweisen lassen
					TextNode * textNodeNext=textNodeCurrent->m_next;
#ifdef _DEBUG
					printf("textNodeCurrent->m_next\n");
#endif
					//wenn die Bytefolge 13 10 existiert, soll sie als 1 Zeichen
					//gehandhabt werden, daher wird das Listenelement, das ein
					//newline-Zeichen beinhaltet (dezimal: 10) auch gelöscht
					if(textNodeCurrent->ch==13 && textNodeNext->ch==10)
					{
						//Speicher an das Betriebssystem freigeben
						delete textNodeCurrent;
						//textNodeCurrent zeigt jetzt auf das ehemals von sich nächste 
						//Listenelement
						textNodeCurrent=textNodeNext;
						//textNodeNext auf das nächste Listenelement von textNodeCurrent
						//verweisen lassen
						textNodeNext=textNodeCurrent->m_next;
					}
					//Speicher an das Betriebssystem freigeben
					delete textNodeCurrent;
					textNodeCurrent=textNodeNext;
				}
				//wenn textNodeCurrent auf das letzte Listenelement zeigt
				else
				{
#ifdef _DEBUG
					printf("!textNodeCurrent->m_next\n");
#endif
					delete textNodeCurrent; // Speicher an das Betriebssystem freigeben
					textNodeCurrent=NULL;
					//wenn textNodeBeforeDeletion NICHT NULL ist (es ist also noch 
					//mindestens ein Listenelement vorhanden/die verkettete Liste ist nicht
					//leer)
					if(textNodeBeforeDeletion)
						//ein Wert von NULL beim m_next-Zeiger eines TextNode-Objekts
						//kennzeichnet das Ende der verketteten Liste
						textNodeBeforeDeletion->m_next=NULL;
					//wenn textNodeBeforeDeletion NULL ist (es ist also kein Listenelement
					//vorhanden/die verkettete Liste ist leer)
					else
						//m_first den Wert 0 zuweisen, was angibt, dass die verkettete Liste
						//leer ist
						m_first=NULL;
					//aus der Schleife ausbrechen
					break;
				}
				//if(textNodeCurrent->m_next)
				//	textNodeCurrent=textNodeCurrent->m_next;
				//else break;
			}
			//wenn das erste gelöschte Listenelement NICHT das erste der verketteten
			//Liste war
			if(textNodeBeforeDeletion)
			{
#ifdef _DEBUG
				printf("textNodeBeforeDeletion\n");
#endif
				if(textNodeCurrent)
				{
					textNodeBeforeDeletion->m_next=textNodeCurrent;
					textNodeCurrent->m_previous=textNodeBeforeDeletion;
				}
			}
			//wenn das erste gelöschte Listenelement das erste der verketteten
			//Liste war
			else
			{
#ifdef _DEBUG
				printf("!textNodeBeforeDeletion\n");
#endif
				if(textNodeCurrent)
				{
#ifdef _DEBUG
					printf("textNodeCurrent\n");
#endif
					m_first=textNodeCurrent;
					m_first->m_previous=NULL;
				}
				else
					m_first=NULL;
			}
			/*if(textNodeCurrent->m_next) // current ist nicht das letzte Listenelement
			{
				if(textNodeCurrent->m_previous) // current ist nicht das erste Listenelement
				{
					TextNode * textNodeNext=textNodeCurrent->m_next;
					TextNode * textNodePrevious=textNodeCurrent->m_previous;
					//wenn direkt vor der aktuellen Cursorposition die Bytefolge
					//13 10 steht
					if(textNodeCurrent->ch==10 && textNodePrevious->ch==13)
					{
						//wenn textNodePrevious NICHT das erste Listenelement ist
						if(textNodePrevious->m_previous)
						{
							//das vorvorletzte Listenelement als neues Listenelement 
							//markieren
							textNodePrevious->m_previous->m_next=textNodeNext;
							//Speicher des vorletzten Listenelements freigeben
							delete textNodePrevious;
						}
						//wenn textNodePrevious das erste Listenelement ist
						else
						{
							//Speicher des vorletzten Listenelements freigeben
							delete textNodePrevious;
							//anzeigen, dass die Liste komplett leer ist
							m_first=textNodeNext;
						}
					}
					else
					{
						textNodeNext->m_previous=textNodePrevious;
						textNodePrevious->m_next=textNodeNext;
					}
					delete textNodeCurrent;
				}
				else // current zeigt auf das erste Listenelement
				{
					m_first=textNodeCurrent->m_next;
					delete textNodeCurrent;
					m_first->m_previous=NULL;
					//m_first=NULL;
				}
			}
			else // textNodeCurrent zeigt auf das letzte Listenelement
			{
#ifdef _DEBUG
				printf("textNodeCurrent zeigt auf das letzte Listenelement\n");
#endif
				if(textNodeCurrent->m_previous) // current ist nicht das erste Listenelement
				{
					TextNode * textNodePrevious=textNodeCurrent->m_previous;
					//wenn direkt vor der aktuellen Cursorposition die Bytefolge
					//13 10 steht
					if(textNodeCurrent->ch==10 && textNodePrevious->ch==13)
					{
#ifdef _DEBUG
						printf("textNodeCurrent->ch==10 && textNodePrevious->ch==13\n");
#endif
						//wenn textNodePrevious NICHT das erste Listenelement ist
						if(textNodePrevious->m_previous)
						{
							TextNode * textNodePreviousPrevious=textNodePrevious->
								m_previous;
							//Speicher des vorletzten Listenelements freigeben
							delete textNodePrevious;
							//das vorvorletzte Listenelement als neues Listenelement 
							//markieren
							textNodePreviousPrevious->m_next=NULL;
						}
						//wenn textNodePrevious das erste Listenelement ist
						else
						{
							//Speicher des vorletzten Listenelements freigeben
							delete textNodePrevious;
							//anzeigen, dass die Liste komplett leer ist
							m_first=NULL;
						}
					}
					else
						textNodePrevious->m_next=NULL;
					delete textNodeCurrent;
				}
				//current zeigt auf das erste Listenelement (es ist also nur 1
				//Listenelement in der Liste vorhanden)
				else
				{
					m_first=NULL;
					delete textNodeCurrent;
				}
			}*/
		}
	}
	//CString 
	/*DWORD dwTextLength=GetLength();
	if(m_first && dwTextLength>=dwPos+dwLength) // wenn Liste nicht leer 
	{
		TextNode * current=m_first;
		DWORD dwRun=0;
		while(dwRun<dwPos)
		{
			if(current->m_next)
				current=current->m_next;
			else
				break;
			dwRun++;
		}
		if(dwRun==dwPos)
		{
			TextNode * previous=NULL;
			TextNode * first=NULL;
			if(current->m_previous)
			{
				previous=current->m_previous;
				first=previous;
			}
			dwRun=0;
			TRACE("dwLength: %u\n",dwLength);

			TextNode * pBeforeDeletion=NULL;
			if(current->m_previous)
				pBeforeDeletion=current->m_previous;
			//TextNode * next=NULL;
			while(dwRun<dwLength)
			{
				//{
					//TextNode * next=NULL;
					//if(current->m_next)
					//	next=current->m_next;
					TextNode * next=current->m_next;
					//next=current->m_next;
					delete current;
					current=next;
				//}
				dwRun++;
			}
				if(current)//dwRun+1==dwLength)
				{
#ifdef _DEBUG
					printf("current\n");
#endif
					if(pBeforeDeletion)
					{
#ifdef _DEBUG
						printf("pBeforeDeletion\n");
#endif
						//if(current->m_next)
							pBeforeDeletion->m_next=current;//->m_next;
							current->m_previous=pBeforeDeletion;
					}
					else
					{
						//if(current->m_next)
						//	m_first=current->m_next;
						//else
						//{
						m_first=current;
						m_first->m_previous=NULL;
							//m_last=NULL;
					}
					//delete current;
				}
				else
				{
#ifdef _DEBUG
					printf("!current\n");
#endif
					if(pBeforeDeletion)
						pBeforeDeletion->m_next=NULL;
					else
						m_first=NULL;
				}

		}
	}
	TRACE("Text::Delete(DWORD dwPos, DWORD dwLength) ENDE\n");*/
#ifdef _DEBUG
	printf("m_first: %u\n",m_first);
	printf("Text::Delete(DWORD dwPos, DWORD dwLength) - ENDE\n");
#endif
}

BOOL Text::GetCString(CString & str,DWORD dwStart,DWORD dwEnd)
{
	//TRACE("BOOL Text::GetCString(CString & str,DWORD dwStart,DWORD dwEnd) - ANFANG\n");
	TextNode * current=m_first;
#ifdef _DEBUG
	printf("BOOL Text::GetCString(CString & str,DWORD dwStart,DWORD dwEnd)\
 - ANFANG\n");
#endif
	if(m_first)
	{
		DWORD dwRun=0;
		while(dwRun<dwStart)
		{
			if(current->m_next)
				current=current->m_next;
			else
			{
				if(dwStart!=dwRun)
				{
#ifdef _DEBUG
					printf("BOOL Text::GetCString(CString & str,DWORD dwStart,DWORD dwEnd)\
 - return FALSE\n");
#endif
					return FALSE;
				}
			}
			dwRun++;
		}
		//TRACE("dwRun: %u  dwStart: %u  dwEnd: %u  \n",dwRun,dwStart,dwEnd);
		while(dwRun<=dwEnd)//-dwStart)
		{
			str+=current->ch;
			if(current->m_next)
				current=current->m_next;
			else
			{
				if(dwEnd!=dwRun)
				{
#ifdef _DEBUG
					printf("BOOL Text::GetCString(CString & str,DWORD dwStart,DWORD dwEnd)\
 - return FALSE\n");
#endif
					return FALSE;
				}
			}
			dwRun++;
		}
	}
	//TRACE("str: %s\n",str);
	//TRACE("BOOL Text::GetCString(CString & str,DWORD dwStart,DWORD dwEnd) - ENDE - return TRUE\n");
#ifdef _DEBUG
	printf("BOOL Text::GetCString(CString & str,DWORD dwStart,DWORD dwEnd)\
 - return TRUE\n");
#endif
	return TRUE;
}

//liefert die Anzahl der Listenelemente der verketteten Liste
DWORD Text::GetLength()
{
#ifdef _DEBUG
	printf("DWORD Text::GetLength() - ANFANG\n");
#endif
	if(m_first)
	{
		TextNode * current=m_first;
		DWORD dwLength=0;
		while(current)
		{
			dwLength++;
#ifdef _DEBUG
			//printf("current->ch: %c %d\n",current->ch,current->ch);
#endif
			current=current->m_next;
		}
#ifdef _DEBUG
		printf("DWORD Text::GetLength() - return dwLength (%u)\n",dwLength);
#endif
		return dwLength;
	}
	else
	{
#ifdef _DEBUG
		printf("DWORD Text::GetLength() - return 0\n");
#endif
		return 0;
	}
}

TextNode * Text::GetPointerToPos(DWORD dwPos)
{
	if(m_first)
	{
		TextNode * current=m_first;
		DWORD dwRun=0;
		while(dwRun<dwPos)
		{
			if(current->m_next)
				current=current->m_next;
			else
				break;
			dwRun++;
		}
		if(dwRun==dwPos)
		{
			return current;
		}
	}
	return NULL;
}

void Text::Insert(DWORD dw,unsigned char ch)
{
	if(m_first)
	{
		if(dw==0)
		{
			TextNode * first=new TextNode();
			first->ch=ch;
			first->m_next=m_first;
			m_first=first;
		}
		else
		{
			TextNode * current=m_first;
			DWORD dwRun=1;
			while(dwRun<dw)
			{
				if(current->m_next)
				{
					current=current->m_next;
					//TRACE("current=current->m_next;\n");
				}
				else
				{
					break;
				}
				dwRun++;
			}
			//In MFC/Windows the TRACE macro has a variable # of args--I do 
			//not know how to enable variable numbers of args for the same macro name.
			//So I exclude it from non-Windows.
	#ifdef _WINDOWS
			TRACE("dwRun: %u\n",dwRun);
	#endif
			if(dwRun==dw)
			{
				if(current->m_next!=NULL)
				{
					TextNode * next=current->m_next;
					current->m_next=new TextNode();
					current=current->m_next;
					current->ch=ch;
					current->m_next=next;
					next->m_previous=current;
				}
				else
				{
					current->m_next=new TextNode();
					TextNode * previous=current;
					current=current->m_next;
					current->ch=ch;
					current->m_next=NULL;
					current->m_previous=previous;
				}
			}
		}
	}
	else
	{
		m_first=new TextNode();
		m_first->ch=ch;
		m_first->m_next=NULL;
	}
}

void Text::Insert(DWORD dwPos,BYTE ch,BYTE color)
{
	TRACE("Text::Insert(DWORD dw,BYTE ch,BYTE color)\n");
#ifdef _DEBUG
	printf("Text::Insert(DWORD dwPos,BYTE ch,BYTE color) - ANFANG\n");
	printf("dwPos: %u\n",dwPos);
#endif
	if(dwPos==0)
	{
#ifdef _DEBUG
		printf("dwPos==0\n");
#endif
		if(m_first)
		{
			TextNode * textNodeFirst=m_first;
			m_first=new TextNode(ch,color);
			m_first->m_next=textNodeFirst;
			m_first->m_previous=NULL;
			textNodeFirst->m_previous=m_first;
		}
		else
		{
			m_first=new TextNode(ch,color);
			m_first->m_next=NULL;
			m_first->m_previous=NULL;
		}
	}
	else
	{
		if(m_first)
		{
			DWORD dwRun=0;
			TextNode * textNodeCurrent=m_first;
			//wenn bei Position 1 eingefügt werden soll, dann wird die folgende
			//Schleife nicht durchlaufen
			//wenn bei Position 2 eingefügt werden soll, dann wird die folgende
			//Schleife 1 Mal durchlaufen
			while(++dwRun<dwPos)//dwRun<dwPos+1)
			{
				if(textNodeCurrent->ch==13 && textNodeCurrent->m_next)
				{
#ifdef _DEBUG
					printf("textNodeCurrent->ch==13 && textNodeCurrent->m_next\n");
#endif
					if(textNodeCurrent->m_next->ch==10)
					{
#ifdef _DEBUG
						printf("textNodeCurrent->m_next->ch==10\n");
#endif
						//die 2 Bytes für eine neue Zeile in einer DOS-/Windows-
						//Textdatei: "13" "10" ("\r" "\n") gelten als 1 Position
						//Damit diese Bytes nicht doppelt gezählt werden, wird
						//der Zeiger "textNodeCurrent" auf das nächste Listenelement
						//gesetzt
						textNodeCurrent=textNodeCurrent->m_next;
					}
				}
				if(textNodeCurrent->m_next)
					textNodeCurrent=textNodeCurrent->m_next;
				else
					break;
			}
			//wenn ein Zeichen bei Position "1" eingefügt werden soll und am 
			//Anfang der verketteten Liste ein Zeilenumbruch (Bytefolge "13 10" 
			//steht), zeigt textNodeCurrent auf 13. textNodeCurrent soll vor dem
			//Einfügen aber auf die "10" zeigen, damit die Bytefolge "13 10" 
			//nicht getrennt werden soll, denn intern wird diese Bytefolge als
			//1 Zeichen behandelt.
			if(textNodeCurrent->ch==13 && textNodeCurrent->m_next)
			{
#ifdef _DEBUG
				printf("textNodeCurrent->ch==13 && textNodeCurrent->m_next\n");
#endif
				if(textNodeCurrent->m_next->ch==10)
				{
#ifdef _DEBUG
					printf("textNodeCurrent->m_next->ch==10\n");
#endif
					//die 2 Bytes für eine neue Zeile in einer DOS-/Windows-
					//Textdatei: "13" "10" ("\r" "\n") gelten als 1 Position
					//Damit diese Bytes nicht doppelt gezählt werden, wird
					//der Zeiger "textNodeCurrent" auf das nächste Listenelement
					//gesetzt
					textNodeCurrent=textNodeCurrent->m_next;
				}
			}
			/*while(1)//dwRun<dwPos+1)
			{
				//wenn das Zeichen ein newline ist (bei Text von Textdatei beim
				//Betriebssystem Windows wird eine neue Zeile durch die Bytefolge
				//13 10 (="carriage return" "newline") eingeleitet)...
				//if(textNodeCurrent->ch==10 && bPreviousCharacterWasCarriageReturn)
				//...dann einen Indexpunkt abziehen, damit ein 13 10 nicht doppelt 
					//gezählt wird
				//	dwRun--;
				dwRun++;
				if(textNodeCurrent->ch==13 && textNodeCurrent->m_next)
				{
#ifdef _DEBUG
					printf("textNodeCurrent->ch==13 && textNodeCurrent->m_next\n");
#endif
					if(textNodeCurrent->m_next->ch==10)
					{
#ifdef _DEBUG
						printf("textNodeCurrent->m_next->ch==10\n");
#endif
						//if(dwRun>=dwPos)
						//	textNodeCurrent=textNodeCurrent->m_next;
						dwRun--;
						//bIncrementdwRun;
					}
				}
				if(dwRun==dwPos)
					break;
					//	bPreviousCharacterWasCarriageReturn=1;
				//else
				//	bPreviousCharacterWasCarriageReturn=0;
				if(textNodeCurrent->m_next)
					textNodeCurrent=textNodeCurrent->m_next;
				else
					break;
			}*/
			if(dwRun==dwPos)
			{
#ifdef _DEBUG
				printf("textNodeCurrent->ch: %c %u\n",textNodeCurrent->ch,
					textNodeCurrent->ch);
#endif
				if(textNodeCurrent->m_next) // current ist nicht das letzte Listenelement
				{
					if(textNodeCurrent->m_previous) // current ist nicht das erste Listenelement
					{
						//Adresse des Listenelements nach textNodeCurrent (zum Zeitpunkt 
						//vor dem Einfügen des neuen Listenelements) speichern
						TextNode * textNodeNext=textNodeCurrent->m_next;
						//Adresse des Listenelements vor textNodeCurrent (zum Zeitpunkt 
						//nach dem Einfügen des neuen Listenelements) speichern
						TextNode * textNodePrevious=textNodeCurrent;
						//neues Listenelement erstellen
						textNodeCurrent->m_next=new TextNode(ch,color);
						//textNodeCurrent auf das neu erstellte Listenelement zeigen lassen
						textNodeCurrent=textNodeCurrent->m_next;
						//dem m_next-Zeiger des neu erstellten Listenelements die Adresse 
						//des Listenelements, das sich vor dem Einfügen des neuen 
						//Listenelements direkt hinter textNodeCurrent befand, zuweisen
						textNodeCurrent->m_next=textNodeNext;
						//dem m_previous-Zeiger des neu erstellten Listenelements die Adresse 
						//des Listenelements, das sich nach dem Einfügen des neuen 
						//Listenelements direkt vor textNodeCurrent befand, zuweisen
						textNodeCurrent->m_previous=textNodePrevious;
						//dem m_previous-Zeiger des Listenelements, das sich vor dem Einfügen 
						//des neuen Listenelements direkt hinter textNodeCurrent befand, 
						//die Adresse des neu erstellten Listenelements zuweisen
						textNodeNext->m_previous=textNodeCurrent;
					}
					else // current zeigt auf das erste Listenelement
					{
						TextNode * textNodeNext=m_first->m_next;
						m_first->m_next=new TextNode(ch,color);
						m_first->m_next->m_previous=m_first;
						m_first->m_next->m_next=textNodeNext;
						textNodeNext->m_previous=m_first->m_next;
						/*TextNode * textNodeNext=m_first;
						m_first=new TextNode(ch,color);
						m_first->m_next=textNodeNext;
						m_first->m_previous=NULL;
						textNodeNext->m_previous=m_first;*/

						//m_first=NULL;
					}
				}
				else // textNodeCurrent zeigt auf das letzte Listenelement
				{
#ifdef _DEBUG
					printf("textNodeCurrent zeigt auf das letzte Listenelement\n");
#endif
					if(textNodeCurrent->m_previous) // current ist nicht das erste Listenelement
					{
						TextNode * textNodePrevious=textNodeCurrent;
						textNodeCurrent->m_next=new TextNode(ch,color);
						textNodeCurrent=textNodeCurrent->m_next;
						textNodeCurrent->m_next=NULL;
						textNodeCurrent->m_previous=textNodePrevious;
					}
					//current zeigt auf das erste Listenelement (es ist also nur 1
					//Listenelement in der Liste vorhanden)
					else
					{
						//textNodeCurrent zeigt auf die selbe Adresse wie m_first
						textNodeCurrent->m_next=new TextNode(ch,color);
						textNodeCurrent=textNodeCurrent->m_next;
						textNodeCurrent->m_next=NULL;
						textNodeCurrent->m_previous=m_first;
					}
				}
			}
		}
	}
		/*if(dw==0)
		{
			TextNode * next=m_first;
			m_first=new TextNode(ch,color);
			m_first->m_next=next;
			next->m_previous=m_first;
		}
		else
		{
		TRACE("if(m_first)    dw: %u\n",dw);
		TextNode * current=m_first;
		DWORD dwRun=1;
		while(dwRun<dw)
		{
			if(current->m_next)
				current=current->m_next;
			else
			{
				//dwRun++;
				break;
			}
			dwRun++;
		}
		if(dwRun==dw)
		{
			TRACE("if(dwRun==dw)\n");
				if(current->m_next) // wenn current NICHT auf das 
				{ // letzte Listenelement zeigt
					TRACE("if(current->m_next)\n");
					//if(current->m_previous) // wenn current NICHT auf das 
					//{ // erste Listenelement zeigt
						//TRACE("if(current->m_previous)\n");
						//TextNode * next=current->m_next;
						//TextNode * previous=current;
						//current->m_next=new TextNode(ch,color);
						//current=current->m_next;
						//current->m_next=next;
						//next->m_previous=current;
						//current->m_previous=previous;
					//}
					//else
					//{
						TextNode * next=current->m_next;
						TextNode * previous=current;
						current->m_next=new TextNode(ch,color);
						current=current->m_next;
						current->m_previous=previous;
						current->m_next=next;
						next->m_previous=current;
						//current->m_previous=previous;
					//}
				}
				else
				{
					current->m_next=new TextNode(ch,color);
					TextNode * next=current->m_next;
					next->m_next=NULL;
					next->m_previous=current;
				}
		}
		}
	}
	else
	{
		if(dw==0)
		{
			m_first=new TextNode(ch,color);
			//m_first->
		}
	}*/
#ifdef _DEBUG
	printf("Text::Insert(DWORD dwPos,BYTE ch,BYTE color) - ENDE\n");
#endif
}

void Text::Insert(DWORD dw,LPSTR szStr,BYTE color)
{
#ifdef _DEBUG
	printf("Text::Insert(DWORD dw,LPSTR szStr,BYTE color) - ANFANG\n");
	printf("szStr: %s\n",szStr);
#endif
	TRACE("Text::Insert(DWORD dw,BYTE ch,BYTE color)\n");
	if(m_first)
	{
		//bei Position 0 einfügen
		if(dw==0)
		{
			TextNode * current=NULL;//m_first;
			TextNode * first=NULL;//m_first;
			TextNode * previous=NULL;
			DWORD dwLength=strlen(szStr);
			for(DWORD dwRun=0;dwRun<dwLength;dwRun++)
			{
				if(previous)
				{
					current=current->m_next;
				}
        //LOGN("12.06.2008 22.33.42"<<szStr[dwRun]);
        current=new TextNode(szStr[dwRun],color);
				if(previous)
				{
					previous->m_next=current;
					current->m_previous=previous;
				}
				if(dwRun==0)
					first=current;
				previous=current;
//				TextNode * next=m_first;
	//			m_first=
		//		m_first->m_next=next;
			//	next->m_previous=m_first;
			}
			if(current)
			{
				/*if(previous)
				{
					current=current->m_next;
					previous->m_next=current;
					current->m_previous=previous;
				}*/
				//current->m_next=m_first;
				current->m_next=m_first;
				m_first->m_previous=current;
				m_first=first;
			}
		}
		//Einfüge-Position ist größer als 0
		else
		{
			//In MFC/Windows the TRACE macro has a variable # of args--I do 
			//not know how to enable variable numbers of args for the same macro name.
			//So I exclude it from non-Windows.
	#ifdef _WINDOWS
			TRACE("if(m_first)    dw: %u\n",dw);
	#endif
			TextNode * current=m_first;
			DWORD dwRun=0;
			/*while(dwRun<dw)
			{
				if(current->m_next)
					current=current->m_next;
				else
				{
					//dwRun++;
					break;
				}
				dwRun++;
			}*/
			while(++dwRun<dw)//dwRun<dwPos+1)
			{
				if(current->ch==13 && current->m_next)
				{
#ifdef _DEBUG
					printf("current->ch==13 && current->m_next\n");
#endif
					if(current->m_next->ch==10)
					{
#ifdef _DEBUG
						printf("current->m_next->ch==10\n");
#endif
						//die 2 Bytes für eine neue Zeile in einer DOS-/Windows-
						//Textdatei: "13" "10" ("\r" "\n") gelten als 1 Position
						//Damit diese Bytes nicht doppelt gezählt werden, wird
						//der Zeiger "textNodeCurrent" auf das nächste Listenelement
						//gesetzt
						current=current->m_next;
					}
				}
				if(current->m_next)
					current=current->m_next;
				else
					break;
			}
			//wenn die letzten 2 Zeichen in der verketteten Liste "13" "10" sind
			//und die Zeichenkette nach dem letzten Zeichen angehägt werden soll,
			//muss auf das nächste Listenelement gezeigt werden, da "13" "10" als
			//Zeichen angesehen werden.
			//Wenn nicht auf das nächste Listenelement gezeigt wird, werden die
			//beiden Zeichen "13" und "10" so getrennt, dass die Zeichenkette
			//nach dem Zeichen "13" eingefügt wird und die "10" am Ende der 
			//verketteten Liste steht
			if(current->m_next)
				if(current->m_next->ch==10)
				{
#ifdef _DEBUG
					printf("current->m_next->ch==10\n");
#endif
					//die 2 Bytes für eine neue Zeile in einer DOS-/Windows-
					//Textdatei: "13" "10" ("\r" "\n") gelten als 1 Position
					//Damit diese Bytes nicht doppelt gezählt werden, wird
					//der Zeiger "textNodeCurrent" auf das nächste Listenelement
					//gesetzt
					current=current->m_next;
				}
			if(dwRun==dw)
			{
				TRACE("if(dwRun==dw)\n");
				// wenn current NICHT auf das letzte Listenelement zeigt
				if(current->m_next)
				{
					TRACE("if(current->m_next)\n");
					DWORD dwLength=strlen(szStr);
					TextNode * after=current->m_next;
					TextNode * next=NULL;
					TextNode * previous=NULL;
					for(dwRun=0;dwRun<dwLength;dwRun++)
					{
						previous=current;
						//next=current->m_next;
						current->m_next=new TextNode(szStr[dwRun],color);
						current=current->m_next;
						current->m_previous=previous;
						previous->m_next=current;
						//next->m_previous=current;
						//current->m_next=next;
						//next->m_previous=current;
					}
					after->m_previous=current;
					current->m_next=after;
				}
				//wenn current auf das letzte Listenelement zeigt
				else
				{
					//TextNode * next=NULL;
					TextNode * previous=NULL;
					DWORD dwLength=strlen(szStr);
#ifdef _DEBUG
					printf("!current->m_next\n");
#endif
					for(dwRun=0;dwRun<dwLength;dwRun++)
					{
						current->m_next=new TextNode(szStr[dwRun],color);
						previous=current;
						current=current->m_next;
						current->m_previous=previous;
						//previo
						//TextNode * next=current->m_next;
						//next->m_next=NULL;
						//next->m_previous=current;
					}
					current->m_next=NULL;
				}
		}
		}
	}
	else // m_first==NULL
	{
		if(dw==0)
		{
			TextNode * current=NULL;//m_first;
			//m_first=new TextNode(szStr[0],color);
			TextNode * first=m_first;
			TextNode * previous=NULL;
			DWORD dwLength=strlen(szStr);
			for(DWORD dwRun=0;dwRun<dwLength;dwRun++)
			{
				//current=new TextNode(szStr[dwRun],color);
				if(dwRun==0)
				{
					m_first=new TextNode(szStr[dwRun],color);//current;
					current=m_first;
				}
				else
					current=new TextNode(szStr[dwRun],color);
				if(previous)
				{
					current->m_previous=previous;
					//previous
					previous->m_next=current;
				}
				previous=current;
				//next=current->m_next;
				current=current->m_next;
						//next->m_previous=current;
						//current->m_next=next;
						//next->m_previous=current;
			}
			//current=NULL;
			//m_first=first;
		}
	}
#ifdef _DEBUG
	printf("Text::Insert(DWORD dw,LPSTR szStr,BYTE color) - ENDE\n");
#endif
}

/*Text::Insert(DWORD dw,BYTE ch,BYTE color)
{
	if(m_first)
	{
		if(dw==0)
		{
			//TextNode * first=new TextNode();
			//first->ch=ch;
			//first->color=color;
			//first->m_next=m_first;
			//m_first->m_previous=first;
			//m_first=first;
			
			//m_first->m_previous=new TextNode();
			//TextNode * previous=m_first->m_previous;
			//previous->ch=ch;
			//previous->color=color;
			//previous->m_previous=NULL;
			//previous->m_next=m_first;
			//m_first=previous;
			TextNode * current=m_first;
			m_first=new TextNode(ch,color);
			m_first->m_previous=NULL;
			m_first->m_next=current;
		}
		else
		{
			TextNode * current=m_first;
			DWORD dwRun=1;
			while(dwRun<dw)
			{
				if(current->m_next)
				{
					current=current->m_next;
					//TRACE("current=current->m_next;\n");
				}
				else
				{
					break;
				}
				dwRun++;
			}
			TRACE("dwRun: %u\n",dwRun);
			if(dwRun==dw)
			{
				if(current->m_next!=NULL)
				{
					TextNode * next=current->m_next;
					current->m_next=new TextNode();
					current=current->m_next;
					current->ch=ch;
					current->color=color;
					current->m_next=next;
					next->m_previous=current;
				}
				else
				{
					current->m_next=new TextNode();
					TextNode * previous=current;
					current=current->m_next;
					current->ch=ch;
					current->color=color;
					current->m_next=NULL;
					current->m_previous=previous;
				}
			}
		}
	}
	else
	{
		m_first=new TextNode();
		m_first->ch=ch;
		m_first->m_next=NULL;
		m_first->m_previous=NULL;
	}
}*/

BOOL Text::SetColor(DWORD dwIndex,BYTE color)
{
	if(m_first)
	{
		TextNode * current=m_first;
		for(DWORD dwRun=0;dwRun<dwIndex;dwRun++)
		{
			if(current->m_next)
				current=current->m_next;
			else
				return FALSE;
		}
		current->color=color;
	}
	return TRUE;
}

//liefert die Anzahl der möglichen Cursorpositionen im Text minus 1
DWORD Text::GetTextLength()
{
#ifdef _DEBUG
	printf("DWORD Text::GetLength() - ANFANG\n");
#endif
	if(m_first)
	{
		TextNode * ptextNodeCurrent=m_first;
		DWORD dwLength=0;
		while(ptextNodeCurrent)
		{
			//wenn ptextNodeCurrent NICHT auf das erste Listenelement zeigt
			if(ptextNodeCurrent->m_previous)
			{
				//wenn NICHT die Zeichenfolge 13 10 (carriage return, newline)
				if(!(ptextNodeCurrent->ch==10 && ptextNodeCurrent->m_previous->
					ch==13))
					dwLength++;
			}
			//wenn ptextNodeCurrent auf das erste Listenelement zeigt
			else
				dwLength++;
#ifdef _DEBUG
			//printf("current->ch: %c %d\n",current->ch,current->ch);
#endif
			ptextNodeCurrent=ptextNodeCurrent->m_next;
		}
#ifdef _DEBUG
		printf("DWORD Text::GetTextLength() - return dwLength (%u)\n",dwLength);
#endif
		return dwLength;
	}
	else
	{
#ifdef _DEBUG
		printf("DWORD Text::GetTextLength() - return 0\n");
#endif
		return 0;
	}
}

