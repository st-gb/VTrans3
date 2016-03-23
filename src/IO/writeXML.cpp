#include "xmlwriter.h"

//class TransXMLWriter
//{
//} ;

//void TransXMLWriter::write()
//{
//	xmlwriter MyXml("XML.txt");
//  MyXml.Createtag("sentences");
//  ::writeToOutputStream(ofstreamTranslToGerman,//vecRange,vecstrvec
//    vecsentenceandvalidityandpropername,MyXml
//    );
//  
//  MyXml.CloseAlltags();//Includes "sentences"-tag
//}

//#ifdef COMPILE_WITH_XML_OUTPUT
#ifndef __MINGW32__ //MinGW has no iconv
void writeToOutputStream(
  std::ostream & rofstreamTranslToGerman,
  //std::vector<Range> & vecRange,std::vector<CStringVector> & vecstrvec
  std::vector<SentenceAndValidityAndProperName> & vecsentenceandvalidityandpropername,
  xmlwriter & MyXml
  )
{
  if(rofstreamTranslToGerman)
  {
    VTrans_string_typeVector strvec;
    SentenceAndValidityAndProperName sentenceandvalidityandpropername;
    for(WORD wSentenceIndex=0;wSentenceIndex<
      vecsentenceandvalidityandpropername.size();++wSentenceIndex)
    {
      IntPair intpairConntectedWords;
      Range range;
      sentenceandvalidityandpropername=vecsentenceandvalidityandpropername.
        at(wSentenceIndex);
      rofstreamTranslToGerman<<sentenceandvalidityandpropername.
        byReturnOfSentenceParsing;
      //Only if the sentence is grammatically correct a translation exists
      //and thus it makes sense to write it and the proper names.
      if(sentenceandvalidityandpropername.byReturnOfSentenceParsing==1)
      {
        MyXml.Createtag("sentence");
        for(WORD wWordIndex = 0; wWordIndex <//vecstrvec
          sentenceandvalidityandpropername.vecvecstrSentenceTokens.size();
          ++ wWordIndex)
        {
          LOGN("31.05.2008 22.30.20");
          strvec=sentenceandvalidityandpropername.vecvecstrSentenceTokens.
            at(wWordIndex);
          LOGN("31.05.2008 22.30.44");
          //rofstreamTranslToGerman<<//vecstrvec
          //  sentenceandvalidityandpropername.vecvecstrSentenceTokens.at(wWordIndex).
          //  at(0).GetBuffer()<<" ";
          LOGN("31.05.2008 22.30.55");
          //MyXml.CreateChild("word",sentenceandvalidityandpropername.
          //  vecvecstrSentenceTokens.at(wWordIndex).at(0).GetBuffer());
          MyXml.Createtag("word");
          //LPWSTR wstrTranslation;
          //int nReturnOfWideCharToMultiByte;
          LPSTR lpTranslationInUTF8 = NULL;
          for(WORD wTranslationForEnglishWordIndex = 0;
            wTranslationForEnglishWordIndex < strvec.size();
            ++ wTranslationForEnglishWordIndex)
          {
#ifdef _WINDOWS
//            //Without this errors while building:
//             USES_CONVERSION;
////#endif
//            wstrTranslation=A2W(strvec.at(wTranslationForEnglishWordIndex).
//              GetBuffer()
//              //"�������"
//              );
//            nReturnOfWideCharToMultiByte = ::WideCharToMultiByte(CP_UTF8, 0, //lpwStr
//              wstrTranslation,
//              //Specifies the number of wide characters in the string pointed to by the lpWideCharStr parameter. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically. The length will include the null-terminator.
//              -1,
//              lpTranslationInUTF8,//Points to the buffer to receive the translated string.
//                //If the function succeeds, and cbMultiByte is zero, the return value is the required size, in bytes, for a buffer that can receive the translated string.
//                0,//cbMultiByte
//                NULL,//If this parameter is NULL, a system default value is used.
//                NULL//This parameter may be NULL. The function is faster when both lpDefaultChar and lpUsedDefaultChar are NULL.
//              );
//            ////If a German umlaut, at least 1 more byte is needed.
//            //nReturnOfWideCharToMultiByte*=200;
//            LOG("23.05.2008 11.23.17 "<<nReturnOfWideCharToMultiByte<<"\n");
//#ifdef _LOG
//            BYTE byWideCharByte;
//            for(DWORD i=0;i<wcslen(wstrTranslation);++i)
//            {
//              byWideCharByte=*(wstrTranslation+i);
//              LOG(byWideCharByte<<"("<<(int)byWideCharByte<<")");
//              byWideCharByte=*((BYTE *)(wstrTranslation+i)+1);
//              LOG(byWideCharByte<<"("<<(int)byWideCharByte<<")");
//            }
//#endif
//            if(nReturnOfWideCharToMultiByte)
//            {
////#ifdef _WINDOWS
//              //If the function succeeds, and cbMultiByte is zero, the return value is the required size, in bytes, for a buffer that can receive the translated string
//              lpTranslationInUTF8=new CHAR[nReturnOfWideCharToMultiByte];
//              if(WideCharToMultiByte(
//                //When this is set, dwFlags must be zero and both lpDefaultChar and lpUsedDefaultChar must be NULL.
//                CP_UTF8,//CodePage
//                0,//dwFlags
//                //lpwStr
//                wstrTranslation,//Points to the wide-character string to be converted.
//                -1,//cchWideChar: Specifies the number of wide characters in the string pointed to by the lpWideCharStr parameter. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically. The length will include the null-terminator.
//                lpTranslationInUTF8,//Points to the buffer to receive the translated string.
//                  //If the function succeeds, and cbMultiByte is zero, the return value is the required size, in bytes, for a buffer that can receive the translated string.
//                  nReturnOfWideCharToMultiByte,//cbMultiByte
//                  NULL,//LPCSTR lpDefaultChar, If this parameter is NULL, a system default value is used.
//                  NULL//LPBOOL lpUsedDefaultChar:This parameter may be NULL. The function is faster when both lpDefaultChar and lpUsedDefaultChar are NULL.
//                  )
//                )
//              {
//#else
//                CString strUTF8 = EncodeToUTF8(strvec.at(
//                  wTranslationForEnglishWordIndex)) ;
//                lpTranslationInUTF8 = strUTF8.Buffer() ;
//
//              if(lpTranslationInUTF8 =
//                  ( CharSetConv::EncodeASCIIToUTF8(strvec.at(
//                wTranslationForEnglishWordIndex).GetBuffer() )
//                  )
//                )
//              {
#endif //#ifdef _WINDOWS
              if(lpTranslationInUTF8 =
                ( CharSetConv::EncodeASCIIToUTF8( (char *) strvec.at(
                  //wTranslationForEnglishWordIndex).GetBuffer()
                  wTranslationForEnglishWordIndex).c_str()
                  )
                  )
                )
              {
                LOG("23.05.2008 11.24.05 "<<lpTranslationInUTF8<<"\n");
                MyXml.CreateChild("translation",//EncodeToUTF8(
                  lpTranslationInUTF8);
                delete[] lpTranslationInUTF8 ;
                LOGN("31.05.2008 22.27.11");
//#ifdef _WINDOWS
              }
#ifdef _WINDOWS
              else
                LOG("23.05.2008 11.24.24 "<<::GetLastError()<<ERROR_INSUFFICIENT_BUFFER<<" "<<ERROR_INVALID_FLAGS<<" "<<ERROR_INVALID_PARAMETER<<"\n");
            //}//if(nReturnOfWideCharToMultiByte)
#endif //#ifdef _WINDOWS
          }//loop through possible German translations for an English word.
          MyXml.CloseLasttag();//"word" tag
        }//Loop through tokens.
        rofstreamTranslToGerman<<
          //carriage Return +Newline is used to indicate the end of the sentence
          "\r\nproper name Size:"<<
          //HTONL: Convert Host(Intel) TO Network(Big Endian) byte order Long value.
          //htonl(
          (unsigned long)//vecRange
          sentenceandvalidityandpropername.vecrangeProperName.size()//)
          <<" ";

        if( sentenceandvalidityandpropername.vecrangeProperName.size() > 0 )
          MyXml.AddComment("The character indices of the proper name "
            "ranges refer to the beginning of WHOLE source text");
        //The character indices of the proper name ranges refer to the
        //beginning of WHOLE source text (and not) to the beginning of
        //a sentence -> put them at the end after all sentences.
        for(DWORD dwRangeIndex=0;dwRangeIndex<//vecRange
          sentenceandvalidityandpropername.vecrangeProperName.size();dwRangeIndex++)
        {
          range=//vecRange
            sentenceandvalidityandpropername.vecrangeProperName.at(dwRangeIndex);
          //for(DWORD dwCharacterIndexWithinEnglishText=range.m_dwStart;
        //    dwCharacterIndexWithinEnglishText<range.m_dwEnd;
            //dwCharacterIndexWithinEnglishText++)

          std::ostringstream ostrstream;
          //The start index ist wrongly stored into the field "m_dwEnd".
          //ostrstream<<vecsentenceandvalidityandpropername.at(0).vecrangeProperName.at(wIndex).m_dwEnd;
          //ostrstream<<range.at(wIndex).m_dwEnd;
          ostrstream<<range.m_dwStart;
          MyXml.AddAtributes("start",ostrstream.str());
          std::ostringstream ostrstream2;
          //The end index ist wrongly stored into the field "m_dwStart".
          //ostrstream2<<vecsentenceandvalidityandpropername.at(0).vecrangeProperName.at(wIndex).m_dwStart;
          //ostrstream2<<range.at(wIndex).m_dwStart;
          ostrstream2<<range.m_dwEnd;
          MyXml.AddAtributes("end",ostrstream2.str());
          MyXml.AddComment("The character indices of a range of a proper name correspond to the SOURCE text, Huey! Believe me.");
          //Man kann diese Indizes nicht einfach auch f�r jede deutsche �bersetzung nehmen:
          //I do not like Uwe. : Uwe ist 5tes Wort. Im deutschen: Ich mag Uwe nicht.
          //Das 5te "Wort" ist hier ein PUNKT ! Soll der etwa zum Eigennamen gemacht werden?
          MyXml.Createtag("range-of-proper-name");
          //Do NOT write this closing tag, else Absturz with "I like Uwe.I like Uwe."
          //MyXml.CloseLasttag();//"range-of-proper-name" tag
          rofstreamTranslToGerman<<
            //HTONL: Convert Host(Intel) TO Network(Big Endian) byte order Long value.
            //htonl(
            (unsigned long)range.m_dwStart//)
            <<//htonl(
            (unsigned long)range.m_dwEnd//)
            ;
        }//Proper name range loop
        if( sentenceandvalidityandpropername.m_vecintpairConntectedWords.size() > 0 )
          MyXml.Createtag("connected-words");
        {
          for(DWORD dwRangeIndex=0;dwRangeIndex<//vecRange
            sentenceandvalidityandpropername.m_vecintpairConntectedWords.size();dwRangeIndex++)
          {
            intpairConntectedWords=
              sentenceandvalidityandpropername.m_vecintpairConntectedWords.at(dwRangeIndex);
            std::ostringstream ostrstream;
            ostrstream<<intpairConntectedWords.m_n1-(ID_USER+100);
            //MyXml.AddAtributes("number1",ostrstream.str());
            std::ostringstream ostrstream2;
            ostrstream2<<intpairConntectedWords.m_n2-(ID_USER+100);
            //MyXml.AddAtributes("number2",ostrstream2.str());
            MyXml.Createtag("connection");
            //MyXml.Createtag("link");
            MyXml.CreateChild("link",ostrstream.str() );
            MyXml.CreateChild("link",ostrstream2.str() );
            MyXml.CloseLasttag();//"connection" tag
          }
          MyXml.CloseLasttag();//"connected-words" tag
        }//if connected words for this sentence exist.
        MyXml.CloseLasttag();//"sentence" tag

      }//if(sentenceandvalidityandpropername.byReturnOfSentenceParsing==1)
      //MyXml.CloseAlltags();
      //MyXml.CloseLasttag();//"sentence" tag
    }//sentences loop
    //rofstreamTranslToGerman.close();
  }//if(rofstreamTranslToGerman)
}
#endif //#ifndef __MINGW32__ //MinGW has no iconv
//#endif//#ifdef COMPILE_WITH_XML_OUTPUT
