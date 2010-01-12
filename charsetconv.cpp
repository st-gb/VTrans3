#include "StdAfx.h"

#include "rest.h"
#include <iconv.h>
#include <errno.h> //for E2BIG, ...

//"ofstreamLogFile" should be declared in the module that starts the translation:
//Either the TransApp constructor or a DLL module.
extern std::ofstream ofstreamLogFile;

namespace CharSetConv
{
//#ifdef _WINDOWS
  //void
  LPSTR EncodeASCIIToUTF8(//PXSTR 
    LPSTR strASCIIToken)
  {
    LPWSTR wstrTranslation ;
    LPSTR lpTranslationInUTF8=NULL;
    //#ifdef _WINDOWS
    #ifdef _MSC_VER //if MicroSoft Compiler

    //Without this errors while building:
     USES_CONVERSION;
//#endif
    wstrTranslation=A2W(//strvec.at(wTranslationForEnglishWordIndex).
      //GetBuffer()
      //"�������"
      strASCIIToken
      );
    int nReturnOfWideCharToMultiByte = ::WideCharToMultiByte(CP_UTF8, 0, //lpwStr
      wstrTranslation,
      //Specifies the number of wide characters in the string pointed to by the lpWideCharStr parameter. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically. The length will include the null-terminator. 
      -1,
      lpTranslationInUTF8,//Points to the buffer to receive the translated string.
        //If the function succeeds, and cbMultiByte is zero, the return value is the required size, in bytes, for a buffer that can receive the translated string. 
        0,//cbMultiByte
        NULL,//If this parameter is NULL, a system default value is used. 
        NULL//This parameter may be NULL. The function is faster when both lpDefaultChar and lpUsedDefaultChar are NULL. 
      );
    ////If a German umlaut, at least 1 more byte is needed.
    //nReturnOfWideCharToMultiByte*=200;
    LOG("23.05.2008 11.23.17 "<<nReturnOfWideCharToMultiByte<<"\n");
#ifdef _LOG
    BYTE byWideCharByte;
    for(DWORD i=0;i<wcslen(wstrTranslation);++i)
    {
      byWideCharByte=*(wstrTranslation+i);
      LOG(byWideCharByte<<"("<<(int)byWideCharByte<<")");
      byWideCharByte=*((BYTE *)(wstrTranslation+i)+1);
      LOG(byWideCharByte<<"("<<(int)byWideCharByte<<")");
    }
#endif
    if(nReturnOfWideCharToMultiByte)
    {
//#ifdef _WINDOWS
      //If the function succeeds, and cbMultiByte is zero, the return value is the required size, in bytes, for a buffer that can receive the translated string
      lpTranslationInUTF8=new CHAR[nReturnOfWideCharToMultiByte];
      if(! WideCharToMultiByte(
        //When this is set, dwFlags must be zero and both lpDefaultChar and lpUsedDefaultChar must be NULL.
        CP_UTF8,//CodePage 
        0,//dwFlags  
        //lpwStr
        wstrTranslation,//Points to the wide-character string to be converted.
        -1,//cchWideChar: Specifies the number of wide characters in the string pointed to by the lpWideCharStr parameter. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically. The length will include the null-terminator.
        lpTranslationInUTF8,//Points to the buffer to receive the translated string. 
          //If the function succeeds, and cbMultiByte is zero, the return value is the required size, in bytes, for a buffer that can receive the translated string. 
          nReturnOfWideCharToMultiByte,//cbMultiByte
          NULL,//LPCSTR lpDefaultChar, If this parameter is NULL, a system default value is used. 
          NULL//LPBOOL lpUsedDefaultChar:This parameter may be NULL. The function is faster when both lpDefaultChar and lpUsedDefaultChar are NULL. 
          )
        )
        lpTranslationInUTF8 = NULL ;
    }
#else //#ifdef _MSC_VER
    //NO MicroSoft Compiler ->cygwin compiler, Linux g++ etc.
    iconv_t iconv_tConversionDescriptor = iconv_open ("UTF-8", "ISO-8859-1");
    if( iconv_tConversionDescriptor != (iconv_t)(-1) )
    {
      size_t inputBytesLeft, outputBytesLeft ;
      inputBytesLeft = strlen(strASCIIToken) ;
      outputBytesLeft = inputBytesLeft * 
	      //Because a single UTF-8 character may be encoded with 4 bytes.
	      4;
      //create dynamically / on heap because that allows bigger blocks of RAM than on stack. 
      char * outbuf = new char[ //strlen(strASCIIToken) *
//      char* outbuf = (char *) malloc( //strlen(strASCIIToken) *
//        //Because a single UTF-8 character may be encoded with 4 bytes.
//        4 
        outputBytesLeft
//        ) ;
    	 ] ;
      char * pchFirstByteOfConv = outbuf ; 
      LOGN("18.08.2008 13.18 "<<strASCIIToken) ;
      if( outbuf )
      {
    	size_t numberOfOutputBytes = outputBytesLeft ;
        LOGN("18.08.2008 13.32 "<<inputBytesLeft<<" "<<outputBytesLeft) ;
        size_t ret = iconv(iconv_tConversionDescriptor, &strASCIIToken, 
          & inputBytesLeft, & outbuf, & outputBytesLeft ) ;
        LOGN("18.08.2008 13.31 "<<inputBytesLeft<<" "<< outputBytesLeft) ;
        if ( ret != (size_t)(-1) )
        {
          size_t numberOfConvertedBytes = numberOfOutputBytes - outputBytesLeft ;
          //iconv seemingly sets no string terminating NULL char, so do it ourselves.
          pchFirstByteOfConv[ numberOfConvertedBytes ] = '\0' ;
          LOGN("18.08.2008 13.58 "<< numberOfConvertedBytes << " " << 
        	pchFirstByteOfConv[0] << pchFirstByteOfConv[ numberOfConvertedBytes ] ) ;
          lpTranslationInUTF8 = pchFirstByteOfConv ;
          LOGN("18.08.2008 13.21 "<< pchFirstByteOfConv) ;
        }
        else
        {
//          LOGN("18.08.2008 12.49") ;
          LOGN("18.08.2008 12.49 "<<ret<<" "<<E2BIG<<" "<<" "<<EILSEQ<<" "<<EINVAL) ;
        }
        iconv_close(iconv_tConversionDescriptor) ;
      }
    }
    else
      LOGN("18.08.2008 12.51") ;
#endif
    return lpTranslationInUTF8 ;
  }
//#ifndef _WINDOWS
#ifndef _MSC_VER //NO NO MicroSoft Compiler->cygwin compiler, Linux g++ etc.
  LPSTR EncodeUTF8ToISO_8859_1(//PXSTR 
    LPSTR strUTF8Token)
  {
    LPSTR lpTranslationInISO_8859_1 = NULL;
    iconv_t iconv_tConversionDescriptor = iconv_open ("ISO-8859-1", "UTF-8");
    if( iconv_tConversionDescriptor != (iconv_t)(-1) )
    {
      size_t inputBytesLeft, outputBytesLeft ;
      inputBytesLeft = strlen(strUTF8Token) ;
      outputBytesLeft = inputBytesLeft ;
      //create dynamically / on heap because that allows bigger blocks of RAM than on stack. 
      char* outbuf = new char[ //strlen(strASCIIToken) *
//      char* outbuf = (char *) malloc( //strlen(strASCIIToken) *
//        //Because a single UTF-8 character may be encoded with 4 bytes.
//        4 
        outputBytesLeft
//        ) ;
    	 ] ;
      char * pchFirstByteOfConv = outbuf ; 
      if( outbuf )
      {
        size_t numberOfOutputBytes = outputBytesLeft ;
        size_t ret = iconv(iconv_tConversionDescriptor, & strUTF8Token, 
          & inputBytesLeft, & outbuf, & outputBytesLeft ) ;
        if ( ret != (size_t)(-1) )
        {
          size_t numberOfConvertedBytes = numberOfOutputBytes - outputBytesLeft ;
          //iconv seemingly sets no string terminating NULL char, so do it ourselves.
          pchFirstByteOfConv[ numberOfConvertedBytes ] = '\0' ;
          lpTranslationInISO_8859_1 = pchFirstByteOfConv ;
        }
        iconv_close(iconv_tConversionDescriptor) ;
      }
    }
    return lpTranslationInISO_8859_1 ;
  }
#endif //#ifdef _MSC_VER
}//namespace
