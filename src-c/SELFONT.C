#include "stddef.h"
#include "crt.h"
#include "hwindow.h"
#include "popup.h"
#include "handrv.h"
#include "filename.h"
#include "memory.h"
#include <io.h>
#include <dos.h>
#include <dir.h>
#include <conio.h>
#include <string.h>
#include <alloc.h>
#include <mem.h>
#include "const.h"
#include "fontver.h"

#define MaxList 60
#define Han 0
#define Eng 1

byte static (*Img) [MaxList][192];
byte static (*Name)[MaxList][13];
word static Count[2];
word static Mode;
word static FontPos;
word static SF_HY,SF_Y;

#define ImgSize ( sizeof *Img + sizeof *Name )

word LoadLib(void)
{
     int handler;
     word i;
     byte Buf[80];


     strcpy(Buf,SysPath);
     strcat(Buf,"sanfont.lib");
     if ((handler=_open(Buf,0))==-1) return Error;
     if (filelength(handler)!=ImgSize*2+4) {
          _close(handler);
          return Error;
     }

     if (Mode==Han) {
        if ( _read (handler,&Count[Han],2) ==-1 ||
             _read (handler,&Count[Eng],2) ==-1 ||
             _read (handler,(byte*)Img,11520) ==-1 ||
             lseek (handler,11520,SEEK_CUR) ==-1 ||
             _read (handler,(byte*)Name,780) ==-1
            ) {
            _close(handler);
            return Error;
         }
     } else {
        if ( _read (handler,&Count[Han],2) ==-1 ||
             _read (handler,&Count[Eng],2) ==-1 ||
             lseek (handler,11520,SEEK_CUR) ==-1 ||
             _read (handler,(byte*)Img,11520) ==-1 ||
             lseek (handler,780,SEEK_CUR) ==-1 ||
             _read (handler,(byte*)Name,780) ==-1
            ) {
            _close(handler);
            return Error;
        }
     }
     _close(handler);
     return OK;
}

void SF_SetBox(void)
{
    word i,j;
    byte Buf[10];

    HInverse();
    for (i=0 ; i<VideoBot-3 ; i++) {
      if (SF_HY+i<Count[Mode]) {
        RestoreScreen(VideoRight-33,i+2,VideoRight-22,i+2,(byte*)(*Img)[SF_HY+i]);
      } else
        HClrBox(VideoRight-33,i+2,VideoRight-22,i+2);
    }
}

void SF_SetPos (word NewX)
{
    word i;

    if (NewX==SF_HY+SF_Y) return;
    if (NewX>Count[Mode]+100) NewX=0;
    if (NewX>=Count[Mode]) NewX=Count[Mode]-1;
    if (NewX<SF_HY)
         if (NewX<SF_Y) SF_HY=0,SF_Y=NewX;
         else        SF_Y=0,SF_HY=NewX-SF_Y;
    else if (NewX<=SF_HY+VideoBot-4) {
            SF_Y=NewX-SF_HY;
            return;
         }
    else SF_Y=VideoBot-4,SF_HY=NewX-SF_Y;
    SF_SetBox();
}

word SF_SelectFont(void)
{
    word i,j;
    word k;
    word MainWay;

    if (Count[Mode]==0) {
       HSayMes(" ¸aÑw ¡¢¢ÑÁ·©·¡ §¡´á ·¶¯s“¡”a. ");
       return Error;
    }
    Push(InverseF);
    HInverse();
    HOpenWindow(VideoRight-34,1,VideoRight-21,VideoBot-1);
    SF_HY=SF_Y=0;
    SF_SetBox();
    FontPos=0;
    MainWay=Error;
    do  {
        j=SF_HY+SF_Y;
        HSetInverse(VideoRight-33,SF_Y+2,VideoRight-22,SF_Y+2);
        GetKey();
        SetHotKeyF();
        if (HotKeyF) break;
        MainWay=Way;
        HSetInverse(VideoRight-33,SF_Y+2,VideoRight-22,SF_Y+2);
        switch (Way) {
          case Home   :SF_SetPos(0);
                       break;
          case End    :SF_SetPos(Count[Mode]-1);
                       break;
          case Up     :
          case CtrlE  :(j==0   ) ? Bell() : SF_SetPos (j-1);
                       break;
          case ' '    :
          case Down   :
          case CtrlX  :(j==Count[Mode]-1) ? Bell() : SF_SetPos (j+1);
                       break;
          case PgDn   :
          case CtrlC  :(j==Count[Mode]-1) ? Bell() : SF_SetPos (j+8);
                       break;
          case PgUp   :
          case CtrlR  :(j==0   ) ? Bell() : SF_SetPos (j-8);
                       break;
          case Return :FontPos=j;
                       break;
        }
    } while (MainWay !=ESC && MainWay!=Return);
    Pop(InverseF);
    HCloseWindow();
    if (HotKeyF || MainWay==ESC) return Error;
    return OK;
}

void SetEngFont(void)
{
    byte Buf[80];

    Mode=Eng;
    Img =DiskBuf;
    Name=DiskBuf+192*60;
    if (LoadLib()!=OK) {
       HSayIOErr();
       return;
    }
    if (SF_SelectFont()!=OK) return;
    strcpy(Buf,SysPath);
    strcat(Buf,(*Name)[FontPos]);
    if (SetEng(Buf)!=OK) {
        HSayIOErr();
        return ;
    }
    ChangeFontF=True;
    HotKeyEndF=True;
    strcpy(EngFontName,(*Name)[FontPos]);
}

void SetHanFont(void)
{
    byte Buf[80];

    Mode=Han;
    Img =DiskBuf;
    Name=DiskBuf+192*60;
    if (LoadLib()!=OK) {
       HSayIOErr();
       return;
    }
    if (SF_SelectFont()!=OK) return;
    strcpy(Buf,SysPath);
    strcat(Buf,(*Name)[FontPos]);
    if (SetHan(Buf)!=OK) {
        HSayIOErr();
        return ;
    }
    ChangeFontF=True;
    HotKeyEndF=True;
    strcpy(HanFontName,(*Name)[FontPos]);
}

