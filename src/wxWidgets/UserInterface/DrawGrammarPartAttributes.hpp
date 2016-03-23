/* 
 * File:   GrammarPartWindowLocation.h
 * Author: mr.sys
 *
 * Created on 16. Oktober 2014, 17:18
 */

#ifndef GRAMMARPARTWINDOWLOCATION_H
#define	GRAMMARPARTWINDOWLOCATION_H

#include <wx/string.h> //class wxString

class DrawGrammarPartAttributes {
public:
  wxString m_wxstrConcatenationID;
  wxString m_wxstrGrammarPartName;
  wxString m_wxstrGrammarPartID;
  wxString m_wxstrGrammarPartMemoryAddress;
  
  DrawGrammarPartAttributes();
  DrawGrammarPartAttributes(const DrawGrammarPartAttributes& orig);
  virtual ~DrawGrammarPartAttributes();
private:

};

#endif	/* GRAMMARPARTWINDOWLOCATION_H */

