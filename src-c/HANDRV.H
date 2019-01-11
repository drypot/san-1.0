/*                                      */
/*                                      */
/*        Han-Gul Driver Header         */
/*                                      */
/*        programed by Magic Book       */
/*                                      */
/*                                      */

#ifndef  __HanDrv__

#define  __HanDrv__

#include "STDDEF.H"

word extern InverseF;
word extern VideoBot,VideoRight,MidX,MidY;

#define HInverse()           (InverseF=ON)
#define HNormal()            (InverseF=OFF)
#define IsGrf(Data)          ((byte)(Data)==0xD4 || ((byte)(Data)>=0xD9 && (byte)(Data)<=0xDE))
#define InverseF             (*InverseFPtr)

byte extern (far*HanFont)[][32],(far*EngFont)[][16],(far*GrfFont)[][32];
word extern HanDrvSeg;
word extern VideoRight,VideoBot,MidX,MidY;
word extern far *InverseFPtr;

void extern (*InitHanDrv) (),
            (*GrMode)     (),
            (*TextMode)   (),
            (*HClrScr)    (),
            (*DrawEng)    (word X,word Y,word Code),
            (*DrawHan)    (word X,word Y,word Code),
            (*DrawHanCur) (word X,word Y),
            (*DrawEngCur) (word X,word Y),
            (*HScrollUp)         (word X1,word Y1,word X2,word Y2),
            (*HScrollDown)       (word X1,word Y1,word X2,word Y2),
            (*SaveScreen)        (word X1,word Y1,word X2,word Y2,byte far *Buf),
            (*RestoreScreen)     (word X1,word Y1,word X2,word Y2,byte far *Buf),
            (*HSetInverse)       (word X1,word Y1,word X2,word Y2),
            (*HClrBox)           (word X1,word Y1,word X2,word Y2),
            (*DrawShadowFull)    (word X1,word Y1,word X2,word Y2),
            (*DrawShadowHalf)    (word X1,word Y1,word X2,word Y2),
            (*DrawShadowHalfDown) (word X1,word Y1,word X2,word Y2);

word far    OpenHanDrv    (byte far *DrvName);
void far    DrawString    (word X,word Y,byte far *Data),
            HanCur1       (word x,word y),
            HanCur2       (word x,word y),
            EngCur1       (word x,word y),
            EngCur2       (word x,word y);

word far    SetHan            (byte *name);
word far    SetGrf            (byte *name);
word far    SetEng            (byte *name);
word far    SetExtEng         (byte *name);

word extern HanF;
word extern HanKey;
word extern HanF;
word extern KeyboardType;
word extern HanResult;

void far GetHan            (void);
void far HanjaOff          (void);

#endif