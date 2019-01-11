/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                       RUN                            */
/*                                                      */

#include "stddef.h"
#include "hwindow.h"
#include "handrv.h"
#include "crt.h"
#include "memory.h"
#include "const.h"
#include <dos.h>
#include <dir.h>
#include <mem.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

void far Scroll(void)
{
      int i;

      HScrollUp(0,0,VideoRight,VideoBot);
      HClrBox(0,VideoBot,VideoRight,VideoBot);
}
word  _X=0,_Y=0;

void  far HPutC  (byte Data)
{
    if (_Y==0 && _X==0) DrawEngCur(_X,_Y);
    switch (Data) {
        case LF    :DrawEngCur(_X,_Y);
                    if (VideoBot==_Y) Scroll(),_Y=VideoBot;
                    else         _Y++;
                    break;
        case BS    :DrawEngCur(_X,_Y);
                    if (_X>0) _X--;
                    break;
        case CR    :DrawEngCur(_X,_Y);
                    _X=0;
                    break;
        default    :DrawEng(_X,_Y,Data);
                    if (_X==VideoRight) {
                        if (_Y==VideoBot)Scroll(),_Y=VideoBot;
                        else      _Y++;
                        _X=0;
                    }
                    else _X++;
    }
    DrawEngCur(_X,_Y);
}

void far HPuts (byte far*Data)
{
      while ( *Data!=0)
         HPutC(*(Data++));
}

void interrupt (*OldVector)();

void interrupt GrBIOS(word bp,word di,word si,
                      word ds,word es,
                      word dx,word cx,word bx,word ax,
                      word ip,word cs,word flags )
{
     word i,xb,yb;

     switch ((ax>>8) & 0xFF) {
       case 0x03 : cx=0x0e0f;
                   dx=_Y*16+_X;
                   break;
       case 0x0e :
       case 0x0a : HPutC(ax & 0xFF);
                   break;
       case 0x09 : xb=_X;
                   yb=_Y;
                   for (i=0 ; i<cx ; i++) HPutC(0x00ff&ax);
                   DrawEngCur(_X,_Y);
                   _X=xb;
                   _Y=yb;
                   DrawEngCur(_X,_Y);
                   break;
       case 0x0F : ax=0x5007;
                   bx=0x0000;
                   break;
       case 0    :HClrScr();

     }
}


void OpenBIOS(void)
{
     OldVector=getvect(0x10);
     setvect(0x10,GrBIOS);
}

void CloseBIOS(void)
{
     setvect(0x10,OldVector);
}

byte RunItems[5][80];
byte RunTitle[5][19];

void Run(word Item)
{
   word i,j;
   int  Status;
   byte Buf[MAXPATH*2];
   char Drv[MAXDRIVE],Dir[MAXDIR];
   char Name[MAXFILE],Ext[MAXEXT];
   word TextModeF=False;

   fnsplit(WorkFileName,Drv,Dir,Name,Ext);
   memset(Buf,0,sizeof(Buf));
   i=j=0;
   while (RunItems[Item][i]!=0) {
     if (RunItems[Item][i]!='%') {
       Buf[j++]=RunItems[Item][i++];
     } else {
       switch (RunItems[Item][i+1]) {
         case 'p':
         case 'P':strcpy(Buf+j,Drv);
                  strcat(Buf+j,Dir);
                  j=strlen(Buf);
                  
                  if ( !(RunItems[Item][i+2]=='%' &&
                          ( RunItems[Item][i+3]=='n' ||
                            RunItems[Item][i+3]=='N'))  &&
                         strlen(Dir)>1 ) {
                     Buf[--j]=0;
                  }
                  i+=2;
                  break;
         case 'n':
         case 'N':strcpy(Buf+j,Name);
                  j=strlen(Buf);
                  i+=2;
                  break;
         case 'e':
         case 'E':strcpy(Buf+j,Ext);
                  j=strlen(Buf);
                  i+=2;
                  break;
         case 't':
         case 'T':TextModeF=True;
                  i+=2;
                  break;
         default :Buf[j++]=RunItems[Item][i++];
       }
     }
   }
   if (TextModeF) {
      TextMode();
      clrscr();
      putch(Drv[0]);
      putch('>');
      puts(Buf);
      Status=system(Buf);
      cputs("\nPress a Key ...");
      GetKey();
      GrMode();
      HClrScr();
      if (Status==-1) HSayIOErr();
   } else {
      HClrScr();
      HNormal();
      _X=_Y=0;
      OpenBIOS();
      Drv[1]=0;
      HPuts(Drv);
      HPutC('>');
      HPuts(Buf);
      HPuts("\n\r\n");
      Status=system(Buf);
      CloseBIOS();
      if (Status==-1) HSayIOErr();
      else HSayBottom("´a¢ ‹i®Aa ’‰œá º­A¶a.");
   }
   SetScnF=True;
   SetMenuScn();
   SetEditScn();
}
