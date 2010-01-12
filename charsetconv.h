#ifndef VTRANS_CHARSETCONV_H
#define VTRANS_CHARSETCONV_H

namespace CharSetConv
{
//#ifdef _WINDOWS
  //void 
  LPSTR EncodeASCIIToUTF8(//PXSTR 
    LPSTR strASCIIToken) ;
  LPSTR EncodeUTF8ToISO_8859_1(//PXSTR 
    LPSTR strUTF8Token) ;
} ;
#endif
