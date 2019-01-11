#include "stddef.h"
#include "crt.h"
#include "handrv.h"
#include "memory.h"
#include "hanconv.h"
#include "const.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <dos.h>
#include <mem.h>

byte  EngMap[25];
word  HanMap[25];

word  PrinterPort=1;
int   PrinterHandler;
int   extern HanjaFileHandler;
word  PrintingType=0;
word  LeftMargin;
word  TopMargin;
word  BotMargin;
word  PageLength;
word  PrintingLine;

static word BufPos;

word PrintByte(byte Data)
{
     DiskBuf[BufPos++]=Data;
     if (BufPos==DiskBufSize) {
       if (_write(PrinterHandler,DiskBuf,BufPos)==-1) return Error;
       BufPos=0;
     }
     return OK;
}

word CloseLine(void)
{
    if (BufPos>0) {
       if (_write(PrinterHandler,DiskBuf,BufPos)==-1) return Error;
       BufPos=0;
    }
    return OK;
}

word OpenPrinter(void)
{
    char Name[10];

    BufPos=0;
    PrintingLine=1;
    sprintf(Name,"lpt%d",PrinterPort);
    PrinterHandler=_creat(Name,0);
    if (PrinterHandler==-1) return Error;
    switch (PrintingType) {
      case 0 : break;
      case 1 : break;
      case 2 : if (PrintByte(27)==Error   ||
                   PrintByte('@')==Error
                   )
                   return Error;
               break;
    }
    return OK;
}

word ClosePrinter(void)
{
    PrintByte('L'-'A'+1);
    if (BufPos>0)
      if (_write(PrinterHandler,DiskBuf,BufPos)==-1) return Error;
    _close(PrinterHandler);
    return OK;
}

word ContinuousByte ( byte Data)
{
    word i;
    word j;

    for (i=j=0 ; i<8 ; i++) {
       if (Data>=0x80) j++;
       else            j=0;
       if (j>3) return OK;
       Data<<=1;
    }
    return False;
}

word ContinuousWord( word Data)
{
    word i;
    word j;

    for (i=j=0 ; i<16 ; i++) {
       if (Data & 0x8000) j++;
       else               j=0;
       if (j>2) return OK;
       Data<<=1;
    }
    return False;
}

word ContinuousGrfWord( word Data)
{
    word i;
    word j;

    for (i=j=0 ; i<16 ; i++) {
       if (Data & 0x8000) j++;
       else               j=0;
       if (j>4) return OK;
       Data<<=1;
    }
    return False;
}

word PrintLQEng(word Data)
{
    word i,j,k,l;
    word FontImg;
    word EnlargeCount;

    EnlargeCount=0;
    MemSetB(EngMap,0,24);
    for (j=l=0 ; j<16 ; j++) {
      FontImg=(*EngFont)[Data][j];
      EngMap[l++]=FontImg;
      if (j%2) EnlargeCount++;
      if (EnlargeCount && (*EngFont)[Data][j]==(*EngFont)[Data][j+1] && FontImg!=0) {
        while (EnlargeCount!=0) {
           EngMap[l++]=FontImg;
           EnlargeCount--;
        }
      }
    }


    for (i=0 ; i<8 ; i++)
      for (j=0 ; j<3 ; j++) {
         for (k=l=0 ; k<8 ; k++)
           if ( EngMap[j*8+k]&(1<<(7-i)) )
             l|=1<<(7-k);
         if (PrintByte(l)==Error) return Error;
      }
    return OK;


}

word GetHanFontPos(word Data);
word static IsLineChar(word Data)
{
     byte first,second;

     second=Data / 0x100;
     first=Data;

     if (first==0xd4)
        return (second>=0xb3 && second<=0xda)? OK :False;
     else if (first==0xdb)
        return (second>=0xa1 && second<=0xE4)? OK :False;
     return False;
}

word PrintLQHan(word Data)
{
    word i,j,k,l;
    byte FontImg[16][2];
    word FontSeg,FontOffset,FontOffset2;
    word EnlargeCount;

    GetHanFontPos(Data);
    FontOffset =_AX;
    FontSeg    =_DX;
    FontOffset2=_BX;

    if ( FontOffset2 ) {
       MemCopy(Pointer(FontSeg,FontOffset) ,FontImg,2*16);
       MemOR  (Pointer(FontSeg,FontOffset2),FontImg,2*16);
    } else
       MemCopy(Pointer(FontSeg,FontOffset) ,FontImg,2*16);

   EnlargeCount=0;
   MemSetW(HanMap,0,24);
   for (j=l=0 ; j<16 ; j++) {
     k=FontImg[j][1];
     FontImg[j][1]=FontImg[j][0];
     FontImg[j][0]=k;
     HanMap[l++]=*(word*)FontImg[j];
     if (j%2) EnlargeCount++;
     if (IsLineChar(Data)) {
        if (EnlargeCount && !ContinuousGrfWord(HanMap[l-1])) {
              HanMap[l]=HanMap[l-1];
              l++;
              EnlargeCount--;
        }
     } else {
        if (EnlargeCount && *(word*)FontImg[j]==*(word*)FontImg[j+1] && HanMap[l-1]!=0) {
           while (EnlargeCount!=0) {
              HanMap[l]=HanMap[l-1];
              l++;
              EnlargeCount--;
           }
        }
     }
   }

    for (i=0 ; i<16 ; i++)
      for (j=0 ; j<3 ; j++) {
         for (k=l=0 ; k<8 ; k++)
           if ( HanMap[j*8+k]&(1<<(15-i)) )
             l|=1<<(7-k);
         if (PrintByte(l)==Error) return Error;
      }
    return OK;
}

word ModulPrinting(byte *Data)
{
   word i;

   for (i=0 ; i<strlen(Data) ; i++)
     if (PrintByte (Data[i])==Error) return Error;
   if (PrintByte(LF)==Error) return Error;
   if (PrintByte(CR)==Error) return Error;
   return CloseLine();
}

word PrintString (byte *Data)
{
      word i,j,LowData;
      byte TransBuf[256];
      word ErrF;

      ErrF=Error;
      if (PrintingLine==1)
         for ( ; PrintingLine<TopMargin ; PrintingLine++ ) {
           if (PrintingType==2) {
               if (PrintByte(27)==Error   ||
                   PrintByte(74)==Error   ||
                   PrintByte(24) ==Error   ||
                   PrintByte(CR)==Error ) return Error;
           } else {
               if (PrintByte(LF)==Error) return Error;
               if (PrintByte(CR)==Error) return Error;
           }
         }

      setmem(TransBuf,sizeof(TransBuf),0);
      setmem(TransBuf,LeftMargin-1,' ');
      strcpy(TransBuf+LeftMargin-1,Data);
      TransBuf[254-LeftMargin-1]=0;
      j=strlen(TransBuf);

      switch (PrintingType) {
        case 0 : ErrF=ModulPrinting (TransBuf);
                 break;
        case 1 : ToFixed(TransBuf,strlen(TransBuf));
                 ErrF=ModulPrinting (TransBuf);
                 break;
        case 2 : if (PrintByte(27)==Error ||
                     PrintByte('*')==Error ||
                     PrintByte(33)==Error ||
                     PrintByte( (j*8)%256 )==Error ||
                     PrintByte( (j*8)/256 )==Error)
                     return Error;

                 for (i=0; i<j ; i++)
                   if (TransBuf[i]<128) {
                      if (PrintLQEng(*(byte*)(TransBuf+i))!=OK) return Error;
                   } else {
                      if (PrintLQHan(*(word*)(TransBuf+i++))!=OK) return Error;
                   }
                 if (PrintByte(27)==Error   ||
                     PrintByte(74)==Error   ||
                     PrintByte(24) ==Error   ||
                     PrintByte(CR)==Error ) return Error;
                 ErrF=CloseLine();
      }
      PrintingLine++;
      if (PrintingLine>BotMargin) {
         PrintingLine=1;
         if (BotMargin<PageLength)
            PrintByte('L'-'A'+1);
      }
      return ErrF;
}

/*

void main(void)
{
     byte LineBuf[256];
     word i;

     FILE *WorkFile;

     if (OpenHanDrv("vga640.drv")!=OK) {
         clrscr();
         puts("\nError in reading CRT Driver.");
         exit(1);
     }
     if (SetExtEng("san.eng")==Error) {
         TextMode();
         clrscr();
         puts("\nOccurred error in reading Sys font");
         exit(1);
     }
     if (SetHan("san.kor")!=OK) {
         TextMode();
         clrscr();
         puts("HAN-GUL FONT.");
         exit(1);
     }
     if (SetGrf("san.grf")==Error) {
         TextMode();
         clrscr();
         puts("GRAPHIC CHARACTER.");
         exit(1);
     }

     HanjaFileHandler=_open("HANJA.FNT",0);
     if (HanjaFileHandler==EOF) HanjaOff();

     if (OpenPrinter()!=OK) {
       puts("open error !");
       GetKey();
     }
     GrMode();
     i=0;

     WorkFile=fopen ("test.c","rt");
     while (True) {
       if (fgets((byte *)LineBuf+1,256-1,WorkFile)==NULL) break;
       DrawString(0,i,LineBuf+1);
       i++;
       if (i==VideoBot) i=0;
       PrintString(LineBuf+1);
       if (KeyPressed()) break;
    }
    fclose (WorkFile);

     TextMode();
     clrscr();
}
*/
