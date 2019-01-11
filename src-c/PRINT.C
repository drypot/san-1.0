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
#include <alloc.h>
#include <bios.h>
#include "hwindow.h"
#include "print.h"

byte  EngMap[24];
word  HanMap[24];

word  PrinterPort=1;
int   extern HanjaFileHandler;
word  PrintingType=0;
word  LeftMargin;
word  TopMargin;
word  BotMargin;
word  PageLength;
word  PrintingLine;
word  SpaceLength;
word  static BufPos;

#include "prio.c"

#include "prepsn.c"
#include "prepsn24.c"
#include "prepson8.c"
#include "prmodul.c"
#include "prlaser.c"


word PrintString (byte *Data)
{
      byte TransBuf[256];
      word ErrF;

      ErrF=Error;
      if (PrintingLine==1) {
         for ( ; PrintingLine<TopMargin ; PrintingLine++ )
             switch (PrintingType) {
               case 0 :
               case 1 :

               case 2 :
               case 3 : ErrF=ModulPrintString ("");
                        break;

               case 4 : ErrF=LQ8PrintString("");
                        break;
               case 5 : ErrF=LQ24PrintString("");
                        break;
               case 6 :
               case 7 :
               case 8 :
               case 9 : ErrF=LaserPrintString("");
                        break;
             }

         if ( PrintingLine!=1 && ErrF==Error ) return Error;
         ErrF=Error;
      }
      setmem(TransBuf,sizeof(TransBuf),0);
      setmem(TransBuf,LeftMargin-1,' ');
      strcpy(TransBuf+LeftMargin-1,Data);

      switch (PrintingType) {
        case 0 :
        case 2 : ErrF=ModulPrintString (TransBuf);
                 break;
        case 1 :
        case 3 : ToFixed(TransBuf,strlen(TransBuf));
                 ErrF=ModulPrintString (TransBuf);
                 break;
        case 4 : ErrF=LQ8PrintString(TransBuf);
                 break;
        case 5 : ErrF=LQ24PrintString(TransBuf);
                 break;
        case 6 :
        case 7 :
        case 8 :
        case 9 : ErrF=LaserPrintString(TransBuf);
                 break;
      }
      PrintingLine++;
      if (PrintingLine>BotMargin) {
         PrintingLine=1;
         if (BotMargin<PageLength || PrintingType>5)
            PrintByte('L'-'A'+1);
      }
      return ErrF;
}

