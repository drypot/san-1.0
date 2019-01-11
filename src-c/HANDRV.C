/*                                      */
/*                                      */
/*        Han-Gul Driver                */
/*                                      */
/*        programed by Magic Book       */
/*                                      */
/*                                      */

#include "MEMORY.H"
#include "CRT.H"
#include "handrv.H"
#include "hanja.h"
#include <io.h>
#include <alloc.h>
#include <mem.h>

word static Count;
byte static far (*Font)[][32];

void  far Draw10_4_4(void)
{
      word i,j;
      byte cho[] ={ 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 1, 3, 3, 3, 1, 2, 4,
                    4, 4, 2, 1, 3, 0      };
      byte mid[] ={ 0, 0, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1,
                    0, 1, 1, 1 };

      Count=0;
      /* Write Jung Sung 1*/

      for (i=0 ; i<22 ; i++ ) MemCopy((byte *)Font+10*20*32+i*32,(*HanFont)[Count++],32);

      /* Write Cho Sung 1*/

      for (i=1 ; i<20 ; i++) {
           MemCopy((byte *)Font+i*32,(*HanFont)[Count++],32);
           for (j=1 ; j<22 ; j++) {
                MemCopy((byte *)Font+cho[j]*20*32+i*32,(*HanFont)[Count],32);
                MemOR  ((byte *)Font+10*20*32+mid[i]*22*32+j*32,(*HanFont)[Count++],32);
           }
      }

      /* Write Jung Sung 2*/

      for (i=0 ; i<22 ; i++ ) MemCopy((byte *)Font+10*20*32+2*22*32+i*32,(*HanFont)[Count++],32);

      /* Write Cho Sung 2 */

      for (i=1 ; i<20 ; i++) {
           MemCopy((byte *)Font+5*20*32+i*32,(*HanFont)[Count++],32);
           for (j=1 ; j<22 ; j++) {
                MemCopy((byte *)Font+(cho[j]+5)*20*32+i*32,(*HanFont)[Count],32);
                MemOR  ((byte *)Font+10*20*32+(mid[i]+2)*22*32+j*32,(*HanFont)[Count++],32);
           }
      }

      /* Write Jong Sung */

      for (i=0; i<4 ; i++) {
          MemCopy((byte *)Font+10*20*32+4*22*32+i*28*32+32,(*HanFont)[Count],32*27);
          Count+=27;
      }
}

word  far SetMBHan(byte far *name)
{
      int i,j;
      int handler;

      Font=farmalloc(400*32);
      if (Font==NULL) return Error;
      handler=_open(name,0);
      if (handler==-1) {
          farfree(Font);
          return Error;
      }
      if (_read (handler,Font,400*32)==-1) {
          farfree(Font);
          _close(handler);
          return Error;
      }
      _close(handler);
      Draw10_4_4();
      farfree(Font);
      return OK;
}

word  far SetDKB2Han(byte far *name)
{
      int i,j;
      int handler;

      Font=farmalloc(400*32);
      if (Font==NULL) return Error;
      handler=_open(name,0);
      if (handler==-1) {
          farfree(Font);
          return Error;
      }
      if (_read (handler,Font,360*32)==-1) {
          farfree(Font);
          _close(handler);
          return Error;
      }
      _close(handler);
      movmem((*Font)[160],(*Font)[200],200*32);
      movmem((*Font)[140],(*Font)[160],20*32);
      movmem((*Font)[140],(*Font)[180],20*32);
      movmem((*Font)[120],(*Font)[140],20*32);
      Draw10_4_4();
      farfree(Font);
      return OK;
}

word  far SetDKB3Han(byte far *name)
{
      int i,j;
      int handler;

      Font=farmalloc(912*32);
      if (Font==NULL) return Error;
      handler=_open(name,0);
      if (handler==-1) {
          farfree(Font);
          return Error;
      }
      if (_read (handler,Font,912*32)==-1) {
          farfree(Font);
          _close(handler);
          return Error;
      }
      _close(handler);
      movmem((*Font)  [0],(*HanFont)  [0],440*32);
      movmem((*Font)[682],(*HanFont)[440],22*32);
      movmem((*Font)[440],(*HanFont)[462],(857-440+1)*32);
      movmem((*Font)[858],(*HanFont)[880],(884-858+1)*32);
      movmem((*Font)[858],(*HanFont)[907],(884-858+1)*32);
      movmem((*Font)[858],(*HanFont)[934],(884-858+1)*32);
      movmem((*Font)[885],(*HanFont)[961],(884-858+1)*32);
      farfree(Font);
      return OK;
}

word  far SetHan(byte far *name)
{
      int  handler;
      word type;

      if ((handler=_open(name,0))==-1) return Error;
      type=(filelength(handler)==12800) ? 0 :
           (filelength(handler)==15584) ? 1 :
           (filelength(handler)==11520) ? 1 :
           (filelength(handler)==29184) ? 2 : 10;
      if (type==10) {
         _close(handler);
         return Error;
      }
      _close(handler);
      switch(type) {
        case 0 :return SetMBHan(name);
        case 1 :return SetDKB2Han(name);
        case 2 :return SetDKB3Han(name);
      }
      return Error;
}

word  far SetEng(byte far *name)
{
      int handler;
      word j;

      if ((handler=_open(name,0))==-1) return Error;
      if (filelength(handler)!=4096 && filelength(handler)!=2048) j=Error;
      else if (_read(handler,EngFont,4096/2)==-1) j=Error;
           else                                              j=OK;
      _close(handler);
      return j;
}

word  far SetExtEng(byte far *name)
{
      int handler;
      word j;

      if ((handler=_open(name,0))==-1) return Error;
      if (filelength(handler)!=320) j=Error;
      else if (_read(handler,(byte*)EngFont+2048,320)==-1) j=Error;
           else                                       j=OK;
      _close(handler);
      return j;
}

word far SetGrf(byte far *name)
{
      int handler;
      word j;

      if ((handler=_open(name,0))==-1) return Error;
      if (filelength(handler)!=37664L) j=Error;
      else if (_read(handler,GrfFont,1178*32)==-1) j=Error;
           else                                      j=OK;
      _close(handler);
      return j;
}

void  far DrawString (word X,word Y,byte far *Data)
{
      word i,j,LowData;

      for (i=0,j=(word)MemSearch((byte far *)Data,0,256)-(word)Data; i<j ; i++)
          if (Data[i]<128) DrawEng(X++,Y,*(byte*)(Data+i));
          else             DrawHan(X  ,Y,*(word*)(Data+i++)),X+=2;
}

unsigned long HalfSecond;

void HanCur1(word x,word y)
{
     word ShowedF=False;

     while (!KeyPressed()) {
       if ( *(unsigned long*)Pointer(0x40,0x6C)%6<3) {
         if (!ShowedF) DrawHanCur(x,y),ShowedF=True;
       } else if (ShowedF) DrawHanCur(x,y),ShowedF=False;
     }

     if (ShowedF) DrawHanCur(x,y);

}

void HanCur2(word x,word y)
{
     word ShowedF=False;

     while (!KeyPressed()) {
       if ( *(unsigned long*)Pointer(0x40,0x6C)%6<3) {
         if (!ShowedF) DrawHanCur(x,y),DrawHanCur(x+1,y),ShowedF=True;
       } else if (ShowedF) DrawHanCur(x,y),DrawHanCur(x+1,y),ShowedF=False;
     }

     if (ShowedF) DrawHanCur(x,y),DrawHanCur(x+1,y);
}

void EngCur1(word x,word y)
{
     word ShowedF=False;

     while (!KeyPressed()) {
       if ( *(unsigned long*)Pointer(0x40,0x6C)%6<3) {
         if (!ShowedF) DrawEngCur(x,y),ShowedF=True;
       } else if (ShowedF) DrawEngCur(x,y),ShowedF=False;
     }

     if (ShowedF) DrawEngCur(x,y);
}

void EngCur2(word x,word y)
{
    word ShowedF=False;

     while (!KeyPressed()) {
       if ( *(unsigned long*)Pointer(0x40,0x6C)%6<3) {
         if (!ShowedF) DrawEngCur(x,y),DrawEngCur(x+1,y),ShowedF=True;
       } else if (ShowedF) DrawEngCur(x,y),DrawEngCur(x+1,y),ShowedF=False;
     }

     if (ShowedF) DrawEngCur(x,y),DrawEngCur(x+1,y);

}

/*void SetSecondCounter(void)
{
     unsigned long count,i,j=0;

     i=*(unsigned long *)Pointer(0x40,0x6C);
     for (HalfSecond=0 ; j<4 && j>=0 ; HalfSecond++) {

       j=*(unsigned long*)Pointer(0x40,0x6C)-i;
     }
} */

byte (far*HanFont)[][32],(far*EngFont)[][16],(far*GrfFont)[][32];
word HanDrvSeg;
word VideoRight,VideoBot,MidX,MidY;
word far *InverseFPtr;

void HanjaOff(void)
{
  *(long*)Pointer(HanDrvSeg,0x102+20+60+12)=0;
}

void (far *InitHanDrv) (),
     (far *GrMode)     (),
     (far *TextMode)   (),
     (far *HClrScr)    (),
     (far *DrawEng)    (word X,word Y,word Code),
     (far *DrawHan)    (word X,word Y,word Code),
     (far *DrawEngCur) (word X,word Y),
     (far *DrawHanCur) (word X,word Y),
     (far *HScrollUp)         (word X1,word Y1,word X2,word Y2),
     (far *HScrollDown)       (word X1,word Y1,word X2,word Y2),
     (far *SaveScreen)        (word X1,word Y1,word X2,word Y2,byte far *Buf),
     (far *RestoreScreen)     (word X1,word Y1,word X2,word Y2,byte far *Buf),
     (far *HSetInverse)       (word X1,word Y1,word X2,word Y2),
     (far *HClrBox)           (word X1,word Y1,word X2,word Y2),
     (far *DrawShadowFull)    (word X1,word Y1,word X2,word Y2),
     (far *DrawShadowHalf)    (word X1,word Y1,word X2,word Y2),
     (far *DrawShadowHalfDown)(word X1,word Y1,word X2,word Y2);

word OpenHanDrv(byte far *DrvName)
{
      int handler;
      byte *i;

      if ( (HanFont=malloc(988 *32))==NULL )        return Error;
      if ( (EngFont=malloc(256 *16))==NULL )        return Error;
      if ( (GrfFont=malloc(1178*32))==NULL )        return Error;

      if ((handler=_open(DrvName,0))==-1)       return Error;
      if ( (i=malloc((word)filelength(handler)+0x110))==NULL ) return Error;
      HanDrvSeg=Seg(i)+Off(i)/16+1;
      if (_read(handler,Pointer(HanDrvSeg,0x100),(word)filelength(handler))==-1)
                                                      return Error;
      _close(handler);
      VideoRight =*(word *)Pointer(HanDrvSeg,0x102+0);
      VideoBot   =*(word *)Pointer(HanDrvSeg,0x102+2);
      MidX       =VideoRight/2;
      MidY       =VideoBot/2;
      InverseFPtr= (word *)Pointer(HanDrvSeg,0x102+4);

      InitHanDrv  =Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+0 ));
      GrMode      =Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+2 ));
      TextMode    =Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+4 ));
      HClrScr     =Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+6 ));
      DrawEng     =Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+8 ));
      DrawHan     =Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+10));
      DrawEngCur  =Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+12));
      DrawHanCur  =Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+14));
      HScrollUp   =Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+16));
      HScrollDown =Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+18));
      SaveScreen  =Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+20));
      RestoreScreen=Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+22));
      HSetInverse =Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+24));
      HClrBox     =Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+26));
      DrawShadowFull=
                   Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+28));
      DrawShadowHalf=
                   Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+30));
      DrawShadowHalfDown=
                   Pointer(HanDrvSeg,*(word*)Pointer(HanDrvSeg,0x102+20+32));

      *(long*)Pointer(HanDrvSeg,0x102+20+60+0)=(long)HanFont;
      *(long*)Pointer(HanDrvSeg,0x102+20+60+4)=(long)EngFont;
      *(long*)Pointer(HanDrvSeg,0x102+20+60+8)=(long)GrfFont;
      *(long*)Pointer(HanDrvSeg,0x102+20+60+12)=(long)HanjaPos;

     /* SetSecondCounter();*/

      return OK;
}
