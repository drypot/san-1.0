/*                                                                 */
/*        Keyboard & Video Library version 1.0 Beta                */
/*                                                                 */
/*        date: 1990,11,4                                          */
/*                                                                 */
/*        programed By Magic Book                                  */
/*                                                                 */
/*  Header                                                         */

#ifndef __CRT__

#define __CRT__

#include "stddef.h"
#include <bios.h>

enum SpecialKeys{ Esc=27 ,Return=13,BackSpace=8,Tab=9 };
enum CtrlKeys   { CtrlA=1,CtrlB,CtrlC,CtrlD,CtrlE,CtrlF,CtrlG,CtrlH,CtrlI,
                  CtrlJ  ,CtrlK,CtrlL,CtrlM,CtrlN,CtrlO,CtrlP,CtrlQ,CtrlR,
                  CtrlS  ,CtrlT,CtrlU,CtrlV,CtrlW,CtrlX,CtrlY,CtrlZ       };
enum ExtKeys    { NUL=1003,ShiftTAB=1015,

                 AltQ   =1016,AltW,AltE,AltR,AltT,AltY,AltU,AltI,AltO,AltP,
                 AltA   =1030,AltS,AltD,AltF,AltG,AltH,AltJ,AltK,AltL,
                 AltZ   =1044,AltX,AltC,AltV,AltB,AltN,AltM,

                 F1     =1059,F2,F3,F4,F5,F6,F7,F8,F9,F10,

                 Home   =1071,Up  ,PgUp,
                 Left   =1075,
                 Right  =1077,
                 End    =1079,Down,PgDn,Ins,Del,

                 ShiftF1=1084,ShiftF2,ShiftF3,ShiftF4,ShiftF5,
                 ShiftF6     ,ShiftF7,ShiftF8,ShiftF9,ShiftF10,

                 CtrlF1,CtrlF2 ,CtrlF3 ,CtrlF4 ,CtrlF5,
                 CtrlF6,CtrlF7 ,CtrlF8 ,CtrlF9 ,CtrlF10,

                 AltF1,AltF2  ,AltF3  ,AltF4  ,AltF5,
                 AltF6,AltF7  ,AltF8  ,AltF9  ,AltF10,

                 CtrlPrintScreen,

                 CtrlLeft,CtrlRight,CtrlEnd,CtrlPgDn,CtrlHome,

                 Alt1,Alt2,Alt3,Alt4,Alt5,Alt6,Alt7,Alt8,Alt9,Alt0,
                 AltMinus,AltEquality,

                 CtrlPgUp,F11,F12,

                 ShiftF11,ShiftF12,
                 CtrlF11 ,CtrlF12,
                 AltF11  ,AltF12,

                 ExtInsert=1075,ExtDelete,
                 ExtLeft  =1079,
                 ExtRight =1089,
                 ExtHome  =1080,ExtEnd,
                 ExtUp    =1083,
                 ExtDown  =1084,
                 ExtPgUp  =1085,ExtPgDn

               };

word     extern ExtKey;
word     extern Key   ;
word     extern Way   ;
word     extern HotKeyF;
word     extern HotKeyEndF;
word     extern PcType;
byte     extern (far *Screen)[25][160];
union  ByteAndWord {
       byte Half[2];
       word Full;
       };
union ByteAndWord extern ScanCode;

void far GetKey         (void);
void far SetKey         (word data);
word far SetHotKeyF     (void);
void far FastKey        (void);
void far ClrScr         (void);
void far InitCrt        (void);

#define  PutC(x,y,c)    ((*Screen)[y][(x)*2  ]=(unsigned char)(c))
#define  PutP(x,y,p)    ((*Screen)[y][(x)*2+1]=(unsigned char)(p))
#define  PutCP(x,y,c,p) (PutP((x),(y),(p)),PutC((x),(y),(c)))
#define  GetC(x,y)      ((*Screen)[y][(x)*2  ])
#define  GetP(x,y)      ((*Screen)[y][(x)*2+1])

#define  ShowCur() SetCurSize(MemB(0,0x461) & ~0x20,MemB(0,0x460) & ~0x20)
#define  HideCur() SetCurSize(MemB(0,0x461) |  0x20,MemB(0,0x460) |  0x20)

#define  normal()      textattr(0x07) /*  bin 0000 0111  */
#define  inverse()     textarrt(0x70) /*  bin 0111 0000  */
#define  underline()   textarrt(0x01) /*  bin 0000 0001  */
#define  flash()       textarrt(0x87) /*  bin 1000 0111  */
#define  flashunder()  textarrt(0xf0) /*  bin 1111 0000  */

#define  MaxMacroString 128

word     extern MacroArray['Z'-'A'+1][MaxMacroString];
byte     extern ShiftArray['Z'-'A'+1][MaxMacroString];
word     extern ReadingMacroF;
word     extern ReadingMacroChar;
word     extern ReadingMacroCounter;
word     extern RunningMacroF;
word     extern RunningMacroChar;
word     extern RunningMacroCounter;

#define  CtrlPressed()        (*(word far *)0x417L & 0x0004)
#define  AltPressed()         (*(word far *)0x417L & 0x0008)
#define  LeftShiftPressed()   ( (RunningMacroF) ? ShiftArray[RunningMacroChar][RunningMacroCounter] & 0x02 :  *(word far *)0x417L & 0x0002 )
#define  RightShiftPressed()  ( (RunningMacroF) ? ShiftArray[RunningMacroChar][RunningMacroCounter] & 0x01 :  *(word far *)0x417L & 0x0001 )
#define  ShiftPressed()       (LeftShiftPressed()||RightShiftPressed())
#define  LeftCtrlPressed()    (*(word far *)0x417L & 0x0100)
#define  RightCtrlPressed()   (CtrlPressed() && !LeftCtrlPressed())
#define  LeftAltPressed()     (*(word far *)0x417L & 0x0200)
#define  RightAltPressed()    (AltPressed()  && !LeftAltPressed())
#define  CtrlNumLockActive()  (*(word far *)0x417L & 0x0800)
#define  ScrollLockPressed()  (*(word far *)0x417L & 0x1000)
#define  NumLockPressed()     (*(word far *)0x417L & 0x2000)
#define  CapsLockPressed()    (*(word far *)0x417L & 0x4000)
#define  InsPressed()         (*(word far *)0x417L & 0x8000)
#define  ScrollLockActive()   (*(word far *)0x417L & 0x0010)
#define  NumLockActive()      (*(word far *)0x417L & 0x0020)
#define  CapsLockActive()     (*(word far *)0x417L & 0x0040)
#define  InsActive()          (*(word far *)0x417L & 0x0080)
#define  SysRqPressed()       (*(word far *)0x417L & 0x0400)

#define  KeyListStart   (*(word far *)0x480L)
#define  KeyListEnd     (*(word far *)0x482L)
#define  NextChar       (*(word far *)0x41AL)
#define  FirstAvailable (*(word far *)0x41CL)
#define  FirstKeyCode      (*(byte far *)(0x400L+NextChar  ))
#define  SecondKeyCode       (*(byte far *)(0x400L+NextChar+1))

word     KeyPressed(void);

#endif