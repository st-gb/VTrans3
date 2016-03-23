#ifndef WRITEXML_H_
#define WRITEXML_H_

class TransXMLWriter
{
public:
	void write() ;
#ifndef __MINGW32__ //MinGW has no iconv
//void writeToOutputStream(std::ostream & rofstreamTranslToGerman,
//  //std::vector<Range> & vecRange,
//	std::vector<CStringVector> & vecstrvec
//  std::vector<SentenceAndValidityAndProperName> & vecsentenceandvalidityandpropername,
//  xmlwriter & MyXml
//  );
#endif //#ifndef __MINGW32__ //MinGW has no iconv
} ;
#endif /*WRITEXML_H_*/
