#include "../../StdAfx.h"
#include "WordList.hpp"
#include "WordNode.hpp"
#include "../../Word.hpp" //for classes English... , German...
#include "../../rest.h" //for CStringVector
#include "../../Token.h" //for GetCStringVector2(...)

extern WordNode * m_first ;
extern WordNode * pWordNodeCurrent ;
extern WordList wordList ;

void WordList::Append(Word * pWord)
{
	if(m_pWordNodeLast)
	{
		m_pWordNodeLast->m_pWordNodeNext=new WordNode(pWord);
		WordNode * pWordNodePrevious=m_pWordNodeLast;
		m_pWordNodeLast=m_pWordNodeLast->m_pWordNodeNext;
		m_pWordNodeLast->m_pWordNodePrevious=pWordNodePrevious;
		m_pWordNodeLast->m_pWordNodeNext=NULL;
	}
	else
	{
		m_pWordNodeLast=new WordNode(pWord);
		m_pWordNodeLast->m_pWordNodePrevious=NULL;
		m_first = m_pWordNodeLast;
		m_pWordNodeLast->m_pWordNodeNext=NULL;
	}
}

WordList::~WordList()
{
  WordNode * pwordnodeCurrent = m_pWordNodeFirst, * pwordnodeNext ;
  //While NOT NULL.
  while(pwordnodeCurrent)
  {
    pwordnodeNext = pwordnodeCurrent->m_pWordNodeNext ;
    delete pwordnodeCurrent ;
    pwordnodeCurrent = pwordnodeNext ;
  }
}

void WordList::Delete()
{
	//es müssen immer 2 Elemente (VocNode) gelöscht werden, weil die englische
	//und die deutsche Vokabel hintereinander stehen
	if(pWordNodeCurrent)
	{
		if(pWordNodeCurrent->m_pWordNodePrevious)
		{
			WordNode * previous=pWordNodeCurrent->m_pWordNodePrevious;
			if(pWordNodeCurrent->m_pWordNodeNext)
			{
				WordNode * next=pWordNodeCurrent->m_pWordNodeNext;
				if(next->m_pWordNodeNext)
				{
					next=next->m_pWordNodeNext;
					previous->m_pWordNodeNext=next;
					delete next->m_pWordNodePrevious;
					next->m_pWordNodePrevious=previous;
					delete pWordNodeCurrent;
					pWordNodeCurrent=next;
					//m_wndVocableEditorBar.WordToGUI(pWordNodeCurrent);
				}
				else
				{
					//VocNode * pVocNodeToDelete=pCurrentVocNode;
					delete next;
					delete pWordNodeCurrent;
					previous->m_pWordNodeNext=NULL;
					wordList.m_pWordNodeLast=previous;
					if(previous->m_pWordNodePrevious)
					{
						pWordNodeCurrent=previous->m_pWordNodePrevious;
						//m_wndVocableEditorBar.WordToGUI(pWordNodeCurrent);
					}
				}
			}
			else // Ende der Liste
			{
				if(previous->m_pWordNodePrevious)
				{
					previous=previous->m_pWordNodePrevious;
					wordList.m_pWordNodeLast=previous;
					delete previous->m_pWordNodeNext;
					previous->m_pWordNodeNext=NULL;
					delete pWordNodeCurrent;
					pWordNodeCurrent=previous;
					//m_wndVocableEditorBar.WordToGUI(pWordNodeCurrent);
				}
			}
		}
		else // Anfang der Liste
		{
			if(pWordNodeCurrent->m_pWordNodeNext)
			{
				WordNode * next=pWordNodeCurrent->m_pWordNodeNext;
				if(next->m_pWordNodeNext)
					next=next->m_pWordNodeNext;
				//VocNode * previous=
				wordList.m_pWordNodeFirst=next;
				delete next->m_pWordNodePrevious;
				next->m_pWordNodePrevious=NULL;
				delete pWordNodeCurrent;
				pWordNodeCurrent=next;
				//m_wndVocableEditorBar.WordToGUI(pWordNodeCurrent);
			}
			else // Liste hat nur 1 Element
			{
				wordList.m_pWordNodeFirst=NULL;
				wordList.m_pWordNodeLast=NULL;
				delete pWordNodeCurrent;
				pWordNodeCurrent=NULL;
			}
		}
	}
}

//BYTE 
Word * WordList::GetFirstOccurance(
  const std::string & str 
  , Word ** p_wordEnglish
  , Word ** p_wordGerman
  )
{
  m_pwordnodeCurrentOccurance = NULL ;
  return GetNextOccurance( str 
    , p_wordEnglish 
    , p_wordGerman
    ) ;
}

Word * WordList::GetPreviousOccurance(
  const std::string & str
  , Word ** p_wordEnglish
  , Word ** p_wordGerman
  )
{
  return NULL ;
}

//BYTE 
Word * WordList::GetNextOccurance(
  //const CString & str
  const std::string & stdstr
  , Word ** p_wordEnglish
  , Word ** p_wordGerman
  )
{
	BYTE Hits = 0;
  CString str = stdstr.c_str() ;
	WordNode * current = NULL , * pWordNodeCurrent = NULL ;
  if( m_pwordnodeCurrentOccurance )
    current = m_pwordnodeCurrentOccurance ;
  else
    current = //wordList.
      m_pWordNodeFirst ;
	while( current )
	{
    // wenn englisches Adjektiv
		if( typeid(*current->m_pWord) == typeid(EnglishAdjective) )
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(EnglishAdjective)\n");
			fflush(stdout);
#endif
			EnglishAdjective * ea = dynamic_cast<EnglishAdjective*>(current->m_pWord);
			if( ea != NULL )
			{
#ifdef _DEBUG
				printf("ea->m_strPositiv: %s\n",ea->m_strPositiv);
				fflush(stdout);
#endif
				if( ea->m_strPositiv == str )
				{
					Hits++;
					//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
					if( !pWordNodeCurrent )
					{
						pWordNodeCurrent=current;
						//TRACE("m_ptrMyBar->WordToGUI(current);\n");
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
            //current
					}
					//else
						//GetDlgItem(ID_USER+101)->EnableWindow(TRUE);
					//m_ptrMyBar->MoveWindow(0,0,100,100,TRUE);
					//MessageBox("jjllll",NULL,MB_OK);
          //p_wordEnglish = ea ;
          return ea ;
				}
			}
		}
		if( typeid(*current->m_pWord)==typeid(EnglishAdverb) ) // wenn englisches Adverb
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(EnglishAdverb)\n");
			fflush(stdout);
#endif
			EnglishAdverb * eav = dynamic_cast<EnglishAdverb*>(current->m_pWord);
			if( eav != NULL )
			{
				if( eav->m_strWord == str )
				{
					Hits ++;
					if(!pWordNodeCurrent)
					{
						pWordNodeCurrent = current;
						//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
						TRACE("m_ptrMyBar->WordToGUI(current);\n");
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
					}
          return eav ;
					//else
						//GetDlgItem(ID_USER+101)->EnableWindow(TRUE);
					//m_ptrMyBar->MoveWindow(0,0,100,100,TRUE);
					//MessageBox("jjllll",NULL,MB_OK);
				}
			}
		}
		 // wenn englisches Hilfsverb
		if( typeid(*current->m_pWord) == typeid(EnglishAuxiliaryVerb) )
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
			fflush(stdout);
#endif
			EnglishAuxiliaryVerb * eav = dynamic_cast<EnglishAuxiliaryVerb*>(current->m_pWord);
			if(eav!=NULL)
			{
				if(eav->m_strWords[0] == str)
				{
					Hits++;
					if(!pWordNodeCurrent)
					{
						pWordNodeCurrent=current;
						//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
						TRACE("m_ptrMyBar->WordToGUI(current);\n");
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
					}
				}
			}
		}
		//wenn englische Konjunktion
		if(typeid(*current->m_pWord)==typeid(EnglishConjunction))
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(EnglishConjunction)\n");
			fflush(stdout);
#endif
			EnglishConjunction * ec = dynamic_cast<EnglishConjunction*>(current->m_pWord);
			if(ec != NULL)
			{
				if(ec->m_strWord == str)
				{
					Hits++;
					if(!pWordNodeCurrent)
					{
						pWordNodeCurrent=current;
						//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
						TRACE("m_ptrMyBar->WordToGUI(current);\n");
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
					}
				}
			}
		}
		if(typeid(*current->m_pWord)==typeid(EnglishNoun)) // wenn englisches Substantiv
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(EnglishNoun)\n");
			fflush(stdout);
#endif
			EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
			if(en!=NULL)
			{
				BOOL bHit=FALSE;
				//TRACE("en!=NULL\n");
				CStringVector sv=GetCStringVector2(en->m_strSingular);
				CStringVector sv2=GetCStringVector2(en->m_strPlural);
				if(en->m_strSingular==str || en->m_strPlural==str)
				{
					bHit=TRUE;
					Hits++;
#ifdef _DEBUG
					printf("en->m_strSingular: %s\n",en->m_strSingular);
#endif
					if(!pWordNodeCurrent)
					{
						//m_pVocableEditorBar->m_szHorz.cy=100;
						//m_pVocableEditorBar->CalcDynamicLayout(100,LM_LENGTHY);
						//m_pMainFrame->RecalcLayout();
						pWordNodeCurrent=current;
						//m_pVocableEditorBar->m_englishWordKind.SetCurSel(0);
						TRACE("m_ptrMyBar->WordToGUI(current);\n");
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
					}
					//m_ptrMyBar->MoveWindow(0,0,100,100,TRUE);
					//MessageBox("jjllll",NULL,MB_OK);
					else
					{
						//GetDlgItem(ID_EDIT_PASTE)->EnableWindow(FALSE);
						//GetDlgItem(IDC_VOCABLE_FORWARD)->EnableWindow(TRUE);
						//CWnd * GetMainWnd( );
						//GetDlgItem(ID_USER+101)->EnableWindow(TRUE);
						//GetDlgItem(ID_USER+8)->ShowWindow(FALSE);
					}
				}
				if(sv.size()>0 && !bHit)
				{
					if(str==sv.at(0))
					{
						bHit=TRUE;
						Hits++;
#ifdef _DEBUG
						printf("en->m_strSingular: %s\n",en->m_strSingular);
#endif
						if(!pWordNodeCurrent)
						{
							//m_pVocableEditorBar->m_szHorz.cy=100;
							//m_pVocableEditorBar->CalcDynamicLayout(100,LM_LENGTHY);
							pWordNodeCurrent=current;
							//m_pVocableEditorBar->m_englishWordKind.SetCurSel(0);
							TRACE("m_ptrMyBar->WordToGUI(current);\n");
							//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
							//m_pVocableEditorBar->WordToGUI(current);
						}
						else
						{
							//GetDlgItem(IDC_VOCABLE_FORWARD)->EnableWindow(TRUE);
						}
					}
				}
				if(sv2.size()>0 && !bHit)
				{
					if(str==sv2.at(0))
					{
						Hits++;
#ifdef _DEBUG
						printf("en->m_strSingular: %s\n",en->m_strSingular);
#endif
						if(!pWordNodeCurrent)
						{
							//m_pVocableEditorBar->m_szHorz.cy=100;
							//m_pVocableEditorBar->CalcDynamicLayout(100,LM_LENGTHY);
							pWordNodeCurrent=current;
							//m_pVocableEditorBar->m_englishWordKind.SetCurSel(0);
							//TRACE("m_ptrMyBar->WordToGUI(current);\n");
							//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
							//m_pVocableEditorBar->WordToGUI(current);
						}
						else
						{
							//GetDlgItem(IDC_VOCABLE_FORWARD)->EnableWindow(TRUE);
						}
					}
				}
			}
		}
		if(typeid(*current->m_pWord)==typeid(EnglishPreposition)) // wenn englische Präposition
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(EnglishPreposition)\n");
			fflush(stdout);
#endif
			EnglishPreposition * ep=dynamic_cast<EnglishPreposition*>(current->m_pWord);
			if(ep!=NULL)
			{
				if(ep->m_strWord==str)
				{
					Hits++;
					if(!pWordNodeCurrent)
					{
						pWordNodeCurrent=current;
						//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
						TRACE("m_ptrMyBar->WordToGUI(current);\n");
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
					}
					//else
						//GetDlgItem(ID_USER+101)->EnableWindow(TRUE);
					//m_ptrMyBar->MoveWindow(0,0,100,100,TRUE);
					//MessageBox("jjllll",NULL,MB_OK);
				}
			}
		}
		if(typeid(*current->m_pWord)==typeid(EnglishPronoun)) // wenn englisches Pronomen
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(EnglishPronoun)\n");
			fflush(stdout);
#endif
			EnglishPronoun * ep=dynamic_cast<EnglishPronoun*>(current->m_pWord);
			if(ep!=NULL)
			{
				if(ep->m_strSingular==str || ep->m_strPlural==str)
				{
					Hits++;
					if(!pWordNodeCurrent)
					{
						pWordNodeCurrent=current;
						//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
						TRACE("m_ptrMyBar->WordToGUI(current);\n");
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
					}
					//else
						//GetDlgItem(ID_USER+101)->EnableWindow(TRUE);
					//m_ptrMyBar->MoveWindow(0,0,100,100,TRUE);
					//MessageBox("jjllll",NULL,MB_OK);
				}
			}
		}
		if(typeid(*current->m_pWord)==typeid(EnglishVerb)) // wenn englisches Verb
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(EnglishVerb)\n");
			fflush(stdout);
#endif
			EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
			if( ev != NULL)
			{
				if(ev->m_strInfinitive==str)
				{
					Hits++;
					if(!pWordNodeCurrent)
					{
						pWordNodeCurrent=current;
						//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
						TRACE("m_ptrMyBar->WordToGUI(current);\n");
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
					}
					//else
						//GetDlgItem(ID_USER+101)->EnableWindow(TRUE);
					//m_ptrMyBar->MoveWindow(0,0,100,100,TRUE);
					//MessageBox("jjllll",NULL,MB_OK);
				}
			}
		}
		if( typeid(*current->m_pWord) == typeid(GermanAdjective)
      ) // wenn deutsches Adjektiv
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(GermanAdjective)\n");
			fflush(stdout);
#endif
			GermanAdjective * ga = dynamic_cast<GermanAdjective*>(current->m_pWord);
			if(ga != NULL)
			{
				if(ga->m_strPositiv == str || ga->m_strComperativ==str || 
					ga->m_strSuperlativ==str ||
					ga->m_strWortstamm==str || ga->m_strWortstamm+_T("er")==str ||
					ga->m_strWortstamm+_T("e")==str || ga->m_strWortstamm+_T("es")==str ||
					ga->m_strWortstamm+_T("en")==str)
				{
					Hits++;
					if(!pWordNodeCurrent)
					{
						pWordNodeCurrent=current->m_pWordNodePrevious;
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
					}
				}
			}
		}
		if(typeid(*current->m_pWord)==typeid(GermanAdverb)) // wenn deutsches Adverb
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(GermanAdverb)\n");
			fflush(stdout);
#endif
			GermanAdverb * gav = dynamic_cast<GermanAdverb*>(current->m_pWord);
			if(gav!=NULL)
			{
				if(gav->m_strWord==str)
				{
					Hits++;
					if(!pWordNodeCurrent)
					{
						pWordNodeCurrent=current->m_pWordNodePrevious;
						TRACE("m_ptrMyBar->WordToGUI(current);\n");
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
					}
				}
			}
		}
		//wenn deutsche Konjunktion
		if(typeid(*current->m_pWord)==typeid(GermanConjunction))
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(GermanConjunction)\n");
			fflush(stdout);
#endif
			GermanConjunction * gc=dynamic_cast<GermanConjunction*>(current->m_pWord);
			if(gc!=NULL)
			{
				if(gc->m_strWord==str)
				{
					Hits++;
					if(!pWordNodeCurrent)
					{
						pWordNodeCurrent=current;
						//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
						TRACE("m_ptrMyBar->WordToGUI(current);\n");
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
					}
				}
			}
		}
		if(typeid(*current->m_pWord)==typeid(GermanPreposition)) // wenn deutsche Präposition
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(GermanPreposition)\n");
			fflush(stdout);
#endif
			GermanPreposition * gp=dynamic_cast<GermanPreposition*>(current->m_pWord);
			if(gp!=NULL)
			{
				if(gp->m_strWord==str)
				{
					Hits++;
					if(!pWordNodeCurrent)
					{
						pWordNodeCurrent=current->m_pWordNodePrevious;
						TRACE("m_ptrMyBar->WordToGUI(current);\n");
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
					}
				}
			}
		}
		if(typeid(*current->m_pWord)==typeid(GermanPronoun)) // wenn deutsches Pronomen
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(GermanPronoun)\n");
			fflush(stdout);
#endif
			GermanPronoun * gp=dynamic_cast<GermanPronoun*>(current->m_pWord);
			if(gp!=NULL)
			{
				if(gp->m_strWord==str)
				{
					Hits++;
					if(!pWordNodeCurrent)
					{
						//pWordNodeCurrent=current->m_pWordNodePrevious;
						TRACE("m_ptrMyBar->WordToGUI(current);\n");
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
					}
				}
			}
		}
		// wenn deutsches Substantiv
		if(typeid(*current->m_pWord)==typeid(GermanNoun))
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(GermanNoun)\n");
			fflush(stdout);
#endif
			GermanNoun * gn=dynamic_cast<GermanNoun*>(current->m_pWord);
			if(gn!=NULL)
			{
				BOOL bHit=FALSE;
				CStringVector sv=GetCStringVector2(gn->m_strSingular);
				CStringVector sv2=GetCStringVector2(gn->m_strPlural);
				if(gn->m_strSingular==str || gn->m_strPlural==str)
				{
					bHit=TRUE;
					Hits++;
#ifdef _DEBUG
					printf("gn->m_strSingular: %s\n",gn->m_strSingular);
#endif
					if(!pWordNodeCurrent)
					{
						//m_pVocableEditorBar->m_szHorz.cy=100;
						//m_pVocableEditorBar->CalcDynamicLayout(100,LM_LENGTHY);
						pWordNodeCurrent=current->m_pWordNodePrevious;
						//m_pVocableEditorBar->m_englishWordKind.SetCurSel(0);
						//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
						//m_pVocableEditorBar->WordToGUI(current);
					}
					else
					{
						//GetDlgItem(IDC_VOCABLE_FORWARD)->EnableWindow(TRUE);
					}
				}
				if(sv.size() >0 && !bHit)
				{
					if(str==sv.at(0))
					{
						bHit=TRUE;
						Hits++;
#ifdef _DEBUG
						printf("gn->m_strSingular: %s\n",gn->m_strSingular);
#endif
						if(!pWordNodeCurrent)
						{
							//m_pVocableEditorBar->m_szHorz.cy=100;
							//m_pVocableEditorBar->CalcDynamicLayout(100,LM_LENGTHY);
							pWordNodeCurrent=current->m_pWordNodePrevious;
							//m_pVocableEditorBar->m_englishWordKind.SetCurSel(0);
							//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
							//m_pVocableEditorBar->WordToGUI(current);
						}
						else
						{
							//GetDlgItem(IDC_VOCABLE_FORWARD)->EnableWindow(TRUE);
						}
					}
				}
				if(sv2.size()>0 && !bHit)
				{
					if(str==sv2.at(0))
					{
						Hits++;
#ifdef _DEBUG
						//printf("en->singular: %s\n",en->singular);
#endif
						if(!pWordNodeCurrent)
						{
							//m_pVocableEditorBar->m_szHorz.cy=100;
							//m_pVocableEditorBar->CalcDynamicLayout(100,LM_LENGTHY);
							pWordNodeCurrent=current->m_pWordNodePrevious;
							//m_pVocableEditorBar->m_englishWordKind.SetCurSel(0);
							//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
							//m_pVocableEditorBar->WordToGUI(current);
						}
						else
						{
							//GetDlgItem(IDC_VOCABLE_FORWARD)->EnableWindow(TRUE);
						}
					}
				}
			}
		}
		// wenn deutsches Verb
		if(typeid(*current->m_pWord)==typeid(GermanVerb))
		{
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(GermanVerb)\n");
			fflush(stdout);
#endif
			GermanVerb * gv=dynamic_cast<GermanVerb*>(current->m_pWord);
			if(gv!=NULL)
			{
				for(BYTE bRun=0;bRun<16;bRun++)
					if(gv->m_strWords[bRun] == str)
					{
#ifdef _DEBUG
						printf("gv->m_strWords[bRun]==str\n");
						fflush(stdout);
#endif
						Hits++;
						if(!pWordNodeCurrent)
						{
#ifdef _DEBUG
							printf("!pWordNodeCurrent\n");
							fflush(stdout);
#endif
							pWordNodeCurrent=current->m_pWordNodePrevious;
#ifdef _DEBUG
							printf("nach: pWordNodeCurrent=current->m_pWordNodePrevious;\n");
							fflush(stdout);
#endif
							//m_pVocableEditorBar->m_bSetFocusOnEnglishWordKind=0;
							//m_pVocableEditorBar->WordToGUI(current);
							//m_pVocableEditorBar->NounControls();
#ifdef _DEBUG
							printf("nach: m_pVocableEditorBar->WordToGUI(current);\n");
							fflush(stdout);
#endif
						}
						break; // bei zwei gleichen gebeugten Verbformen [betrifft 3. 
						// Person Singular (er/sie/es) und 2. Person Plural (ihr) ]
						// die Variable "Hits" NICHT erhöhen
						// Beispiel: "er geht" , "ihr geht"
					}
			}
#ifdef _DEBUG
			printf("typeid(*current->m_pWord)==typeid(GermanVerb) ENDE\n");
			fflush(stdout);
#endif
		}
		if(Hits==2)
		{
			//pFrame->m_bEnableNextVocableButton=TRUE;
			break;
		}
		if(current->m_pWordNodeNext)
			current = current->m_pWordNodeNext;
		else
		{
			//pFrame->m_bEnableNextVocableButton = FALSE ;
			break;
		}
	}
  return //Hits ;
    NULL ;
}

Word * GetPreviousOccurance(
  const std::string & stdstr 
  , Word ** p_wordEnglish
  , Word ** p_wordGerman
  )
{
  CString str = stdstr.c_str() ;
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	if(pWordNodeCurrent)
	{
		TRACE("m_first ist ungleich NULL\n");
		WordNode * current=pWordNodeCurrent;
		BYTE Hits=0;
		//CString str=((CVTransApp*)AfxGetApp())->m_strSearch;
		BOOL bFound=FALSE;
		//m_bEnableNextVocableButton=FALSE;
		//m_bEnablePreviousVocableButton=FALSE;
		if(str=="*")
		{
			if(pWordNodeCurrent->m_pWordNodePrevious)
			{
				WordNode * pWordNodePrevious=pWordNodeCurrent->m_pWordNodePrevious;
				if(pWordNodePrevious->m_pWordNodePrevious)
				{
					pWordNodeCurrent=pWordNodePrevious->m_pWordNodePrevious;
					//m_wndVocableEditorBar.WordToGUI(pWordNodeCurrent);
					//m_bEnableNextVocableButton=TRUE;
					if(pWordNodeCurrent->m_pWordNodePrevious)
					{
						pWordNodePrevious=pWordNodeCurrent->m_pWordNodePrevious;
						//if(pWordNodePrevious->m_pWordNodePrevious)
							//m_bEnablePreviousVocableButton=TRUE;
					}
				}
			}
			return NULL;
		}
		while(1)
		{
			//if(Hits==1)
			//	m_bEnableNextVocableButton=TRUE;
			if(Hits==2)
			{
				//m_bEnablePreviousVocableButton=TRUE;
				break;
			}
			if(current->m_pWordNodePrevious)
				current=current->m_pWordNodePrevious;
			else
			{
				break;
			}
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective)) // wenn englisches Substantiv
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(current->m_pWord);
				if(ea!=NULL)
				{
					if(ea->m_strPositiv==str)
					{
						Hits++;
						//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
						if(!bFound)
						{
							pWordNodeCurrent=current;
							//TRACE("m_ptrMyBar->WordToGUI(current);\n");
							//m_wndVocableEditorBar.WordToGUI(current);
						}
						bFound=TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb)) // wenn englisches Substantiv
			{
				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>(current->m_pWord);
				if(eav!=NULL)
				{
					if(eav->m_strWord==str)
					{
						Hits++;
						if(!bFound)
						{
							pWordNodeCurrent=current;
							//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
							//TRACE("m_ptrMyBar->WordToGUI(current);\n");
							//m_wndVocableEditorBar.WordToGUI(current);
						}
						bFound=TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishConjunction))
			{
				EnglishConjunction * ec=dynamic_cast<EnglishConjunction*>
					(current->m_pWord);
				if(ec!=NULL)
				{
					if(ec->m_strWord==str)
					{
						Hits++;
						if(!bFound)
						{
							pWordNodeCurrent=current;
							//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
							//TRACE("m_ptrMyBar->WordToGUI(current);\n");
							//m_wndVocableEditorBar.WordToGUI(current);
						}
						bFound=TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishNoun)) // wenn englisches Substantiv
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					TRACE("en!=NULL\n");
					if(en->m_strSingular==str || en->m_strPlural==str)
					{
						Hits++;
#ifdef _DEBUG
						printf("en->m_strSingular: %s\n",en->m_strSingular);
#endif
						if(!bFound)
						{
							pWordNodeCurrent=current;
							//m_wndVocableEditorBar->m_englishWordKind.SetCurSel(0);
							//TRACE("m_ptrMyBar->WordToGUI(current);\n");
							//m_wndVocableEditorBar.WordToGUI(current);
						}
						bFound=TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishPreposition)) // wenn englische Präposition
			{
				EnglishPreposition * ep=dynamic_cast<EnglishPreposition*>
					(current->m_pWord);
				if(ep!=NULL)
				{
					if(ep->m_strWord==str)
					{
						Hits++;
						if(!bFound)
						{
							pWordNodeCurrent=current;
							//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
							//TRACE("m_ptrMyBar->WordToGUI(current);\n");
							//m_wndVocableEditorBar.WordToGUI(current);
						}
						bFound=TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishPronoun)) // wenn englisches Pronomen
			{
				EnglishPronoun * ep=dynamic_cast<EnglishPronoun*>
					(current->m_pWord);
				if(ep!=NULL)
				{
					if(ep->m_strSingular==str || ep->m_strPlural==str)
					{
						Hits++;
						if(!bFound)
						{
							pWordNodeCurrent=current;
							//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
							//TRACE("m_ptrMyBar->WordToGUI(current);\n");
							//m_wndVocableEditorBar.WordToGUI(current);
						}
						bFound=TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishVerb)) // wenn englisches Substantiv
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					if(ev->m_strInfinitive==str)
					{
						Hits++;
						if(!bFound)
						{
							pWordNodeCurrent=current;
							//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
							//TRACE("m_ptrMyBar->WordToGUI(current);\n");
							//m_wndVocableEditorBar.WordToGUI(current);
						}
						bFound=TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(GermanAdjective)) // wenn deutsches Adjektiv
			{
				GermanAdjective * ga=dynamic_cast<GermanAdjective*>
					(current->m_pWord);
				if(ga!=NULL)
				{
					if(ga->m_strPositiv==str || ga->m_strComperativ==str || 
						ga->m_strSuperlativ==str ||	ga->m_strWortstamm==str || 
						ga->m_strWortstamm+_T("er")==str ||	
						ga->m_strWortstamm+_T("e")==str || 
						ga->m_strWortstamm+_T("es")==str ||
						ga->m_strWortstamm+_T("en")==str)
					{
						Hits++;
						if(!bFound)
						{
							pWordNodeCurrent=current->m_pWordNodePrevious;
							//m_wndVocableEditorBar.WordToGUI(current);
						}
						bFound=TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(GermanAdverb)) // wenn deutsches Adverb
			{
				GermanAdverb * gav=dynamic_cast<GermanAdverb*>(current->m_pWord);
				if(gav!=NULL)
				{
					if(gav->m_strWord==str)
					{
						Hits++;
						if(!bFound)
						{
							pWordNodeCurrent=current->m_pWordNodePrevious;
							//TRACE("m_ptrMyBar->WordToGUI(current);\n");
							//m_wndVocableEditorBar.WordToGUI(current);
						}
						bFound=TRUE;
					}
				}
			}
			//wenn deutsche Konjunktion
			if(typeid(*current->m_pWord)==typeid(GermanConjunction))
			{
				GermanConjunction * gc=dynamic_cast<GermanConjunction*>(current->m_pWord);
				if(gc!=NULL)
				{
					if(gc->m_strWord==str)
					{
						Hits++;
						if(!bFound)
						{
							pWordNodeCurrent=current;
							//m_ptrMyBar->m_englishWordKind.SetCurSel(0);
							//TRACE("m_ptrMyBar->WordToGUI(current);\n");
							//m_wndVocableEditorBar.WordToGUI(current);
						}
						bFound=TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(GermanPreposition)) // wenn deutsche Präposition
			{
				GermanPreposition * gp=dynamic_cast<GermanPreposition*>
					(current->m_pWord);
				if(gp!=NULL)
				{
					if(gp->m_strWord==str)
					{
						Hits++;
						if(!bFound)
						{
							pWordNodeCurrent=current->m_pWordNodePrevious;
							//TRACE("m_ptrMyBar->WordToGUI(current);\n");
							//m_wndVocableEditorBar.WordToGUI(current);
						}
						bFound=TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(GermanPronoun)) // wenn deutsches Pronomen
			{
				GermanPronoun * gp=dynamic_cast<GermanPronoun*>(current->m_pWord);
				if(gp!=NULL)
				{
					if(gp->m_strWord==str)
					{
						Hits++;
						if(!bFound)
						{
							pWordNodeCurrent=current->m_pWordNodePrevious;
							//TRACE("m_ptrMyBar->WordToGUI(current);\n");
							//m_wndVocableEditorBar.WordToGUI(current);
						}
						bFound=TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(GermanNoun)) // wenn englisches Substantiv
			{
				GermanNoun * gn=dynamic_cast<GermanNoun*>(current->m_pWord);
				if(gn!=NULL)
				{
					BOOL bHit=FALSE;
					CStringVector sv=GetCStringVector2(gn->m_strSingular);
					CStringVector sv2=GetCStringVector2(gn->m_strPlural);
					if(gn->m_strSingular==str || gn->m_strPlural==str)
					{
						bHit=TRUE;
						Hits++;
#ifdef _DEBUG
						printf("gn->m_strSingular: %s\n",gn->m_strSingular);
#endif
						if(!bFound)
						{
							//m_pVocableEditorBar->m_szHorz.cy=100;
							//m_pVocableEditorBar->CalcDynamicLayout(100,LM_LENGTHY);
							pWordNodeCurrent=current->m_pWordNodePrevious;
							//m_wndVocableEditorBar.WordToGUI(current);
						}
						else
						{
							//GetDlgItem(IDC_VOCABLE_FORWARD)->EnableWindow(TRUE);
						}
						bFound=TRUE;
					}
					if(sv.size()>0 && !bHit)
					{
						if(str==sv.at(0))
						{
							bHit=TRUE;
							Hits++;
#ifdef _DEBUG
							printf("gn->m_strSingular: %s\n",gn->m_strSingular);
#endif
							if(!bFound)
							{
								//m_pVocableEditorBar->m_szHorz.cy=100;
								//m_pVocableEditorBar->CalcDynamicLayout(100,LM_LENGTHY);
								pWordNodeCurrent=current->m_pWordNodePrevious;
								//m_wndVocableEditorBar.WordToGUI(current);
							}
							else
							{
								//GetDlgItem(IDC_VOCABLE_FORWARD)->EnableWindow(TRUE);
							}
							bFound=TRUE;
						}
					}
					if(sv2.size()>0 && !bHit)
					{
						if(str==sv2.at(0))
						{
							Hits++;
#ifdef _DEBUG
							//printf("en->singular: %s\n",en->singular);
#endif
							if(!bFound)
							{
								//m_pVocableEditorBar->m_szHorz.cy=100;
								//m_pVocableEditorBar->CalcDynamicLayout(100,LM_LENGTHY);
								pWordNodeCurrent=current->m_pWordNodePrevious;
								//m_wndVocableEditorBar.WordToGUI(current);
							}
							else
							{
								//GetDlgItem(IDC_VOCABLE_FORWARD)->EnableWindow(TRUE);
							}
							bFound=TRUE;
						}
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(GermanVerb)) // wenn deutsches Substantiv
			{
				GermanVerb * gv=dynamic_cast<GermanVerb*>(current->m_pWord);
				if(gv!=NULL)
				{
					for(BYTE bRun=0;bRun<16;bRun++)
						if(gv->m_strWords[bRun]==str)
						{
							Hits++;
							if(!bFound)
							{
								pWordNodeCurrent=current->m_pWordNodePrevious;
								//m_wndVocableEditorBar.WordToGUI(current);
							}
							bFound=TRUE;
							break; // bei zwei gleichen gebeugten Verbformen [betrifft 3. 
							// Person Singular (er/sie/es) und 2. Person Plural (ihr) ]
							// die Variable "Hits" NICHT erhöhen
							// Beispiel: "er geht" , "ihr geht"
						}
				}
			}
		}
	}
}
