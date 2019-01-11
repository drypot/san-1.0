/*                                                                 */
/*        Keyboard & Video Library version 1.0 Beta                */
/*                                                                 */
/*        date: 1990,11,4                                          */
/*                                                                 */
/*        programed By Magic Book                                  */
/*                                                                 */

#include "MEMORY.H"
#include "CRT.H"
#include <bios.h>
#include <stdio.h>
#include <dos.h>

#define  MaxMacroString 128

word     ExtKey;
word     Key   ;
word     Way   ;
word     HotKeyF;
word     HotKeyEndF;
word     PcType;
word     ScaningShiftF=False;
word     ErasedShiftF=False;
byte     (far *Screen)[25][160];
word     MacroArray['Z'-'A'+1][MaxMacroString];
byte     ShiftArray['Z'-'A'+1][MaxMacroString];
byte     HanFArray ['Z'-'A'+1];
word     ReadingMacroF;
word     ReadingMacroChar;
word     ReadingMacroCounter;
word     RunningMacroF;
word     RunningMacroChar;
word     RunningMacroCounter;
union    ByteAndWord ScanCode;

void Bell1 (void)
{
     sound (5000);
     delay (50);
     nosound();
}

void Bell2 (void)
{
     sound (3000);
     delay (80);
     nosound();
}

word extern HanF;
word        HanFBuf;

word KeyPressed(void)
{
     word i;
     int  j;

     if (RunningMacroF) return OK;

     _AH=0x01;
     Inter(0x16);
     asm pushf
     asm pop AX
     i=_AX;
     if (ScaningShiftF && !ShiftPressed()) ErasedShiftF=True;
     if (ShiftPressed()) {
        j=bioskey(1);
        if ((word)j==0x011b && ReadingMacroCounter==0) {
          ReadingMacroF=False;
          _AH=0x00;
          Inter(0x16);
          Bell2();
          return False;
        }
     }

     if (ShiftPressed() && CtrlPressed() ) {

        j=bioskey(1);
        j>>=0x8;

        if (j>=0x10 && j<=0x19 ||
            j>=0x1e && j<=0x26 ||
            j>=0x2c && j<=0x32) {

            _AH=0x00;
            Inter(0x16);
            Key=_AH;


            if (Key>=0x2c ) Key-=0x2c-19;
            else if (Key>=0x1e) Key-=0x1e-10;
            else Key-=10;

            if (ReadingMacroF) {
               if (ReadingMacroChar==Key) {
                  ReadingMacroF=False;
                  MacroArray[ReadingMacroChar][0]=ReadingMacroCounter;
                  Bell2();
               } else {
                  MacroArray[ReadingMacroChar][0]=ReadingMacroCounter;
                  ReadingMacroCounter=0;
                  ReadingMacroChar=Key;
                  HanFArray[ReadingMacroChar]=HanF;
                  Bell2();
                  Bell1();
               }
            } else {
               ReadingMacroF=True;
               ReadingMacroChar=Key;
               ReadingMacroCounter=0;
               HanFArray[ReadingMacroChar]=HanF;
               Bell1();
            }
            return False;
        }
     } else if (ShiftPressed() && AltPressed() && !(i&0x0040) ) {
        j=bioskey(1);
        j>>=0x8;
        if (j>=0x10 && j<=0x19 ||
            j>=0x1e && j<=0x26 ||
            j>=0x2c && j<=0x32) {

            _AH=0x00;
            Inter(0x16);
            Key=_AH;

            if (Key>=0x2c ) Key-=0x2c-19;
            else if (Key>=0x1e) Key-=0x1e-10;
            else Key-=10;

            if (MacroArray[Key][0]==0) {
              return False;
            }
            else {
              RunningMacroChar=Key;
              RunningMacroCounter=0;
              RunningMacroF=OK;
              HanFBuf=HanF;
              HanF=HanFArray[RunningMacroChar];
              return OK;
            }
        }
     }
     return !(i&0x0040);
}


#include "handrv.h"

void far GetKey (void)
{
     if (RunningMacroF) {
         RunningMacroCounter++;
         ScanCode.Full=MacroArray[RunningMacroChar][RunningMacroCounter];
         if (RunningMacroCounter==MacroArray[RunningMacroChar][0]) {
           RunningMacroF=False;
           HanF=HanFBuf;
         }

         if (ReadingMacroF) {
           ReadingMacroCounter++;
           MacroArray[ReadingMacroChar][ReadingMacroCounter]=ScanCode.Full;
           ShiftArray[ReadingMacroChar][ReadingMacroCounter]=LeftShiftPressed() | RightShiftPressed();
           if (ReadingMacroCounter==MaxMacroString-1) {
              MacroArray[ReadingMacroChar][0]=MaxMacroString-1;
              ReadingMacroF=False;
              Bell2();
           }
         }

         Key=ScanCode.Half[0];
         Way=ScanCode.Half[1];
         ExtKey=(Key==0) ? ((Key=Way),(Way+=1000),True):((Way=Key),False);
         return;
     }

     while (!KeyPressed());

     _AH=0x00;
     Inter(0x16);
     ScanCode.Full=_AX;

     Key=ScanCode.Half[0];
     Way=ScanCode.Half[1];

     ExtKey=(Key==0) ? ((Key=Way),(Way+=1000),True):((Way=Key),False);

     if (ReadingMacroF) {

       ReadingMacroCounter++;
       MacroArray[ReadingMacroChar][ReadingMacroCounter]=ScanCode.Full;
       ShiftArray[ReadingMacroChar][ReadingMacroCounter]=LeftShiftPressed() | RightShiftPressed();
       if (ReadingMacroCounter==MaxMacroString-1) {
          MacroArray[ReadingMacroChar][0]=MaxMacroString-1;
          ReadingMacroF=False;
          Bell2();
       }
     }




/*   Inter(0x16);
     _DX=_AX;
     Key=_DL;
     Way=_DH;
     ExtKey=(Key==0x00) ? ((Key=Way),(Way+=1000),True):
            (Key==0xE0) ? ((Key=Way),(Way+=1000),True):
                          ((Way=Key),False);
*/
/*   if (Way==ExtInsert) Way=Ins;
     if (Way==ExtDelete) Way=Del;
     if (Way==ExtLeft)   Way=Left;
     if (Way==ExtRight)  Way=Right;
     if (Way==ExtHome)   Way=Home;
     if (Way==ExtEnd)    Way=End;
     if (Way==ExtUp)     Way=Up;
     if (Way==ExtDown)   Way=Down;
     if (Way==ExtPgUp)   Way=PgUp;
     if (Way==ExtPgDn)   Way=PgDn;
*/

/*   while (!KeyPressed()) ;
         switch (AsciiCode) {
            case 0   : Key=ScanCode;
                       ExtKey=True;
                       Way=1000+Key;
                       break;
            case 0xE0: Key=ScanCode;
                       ExtKey=True;
                       Way=2000+Key;
                       break;
            default  : Key=AsciiCode;
                       ExtKey=False;
                       Way=Key;
         }
         NextChar+=2;
         if (NextChar==KeyListEnd) NextChar=KeyListStart;
*/

}

void far SetKey (word Data)
{
         ExtKey=(byte)(Data>=1000);
         Way=Data;
         Key=(byte) Data;
}

word far SetHotKeyF(void)
{
         switch (Way) {
           case F10    :
           case F5     :
           case AltF6  :
           case AltF7  :
           case AltF8  :
           case AltF9  :
           case F2     :
           case F3     :
           case AltW   :
           case AltF3  :
           case AltC   :
           case AltF   :
           case AltO   :
           case AltY   :
           case AltB   :
           case AltI   :
           case AltE   :
           case AltH   :
           case AltP   :
           case AltX   :HotKeyF=True;
                        break;
           default     :HotKeyF=False;
         }
         return HotKeyF;
}

void far FastKey (void)
{
         _AX=0x0305;
         _BX=0x0001;
         Inter(0x16);
}

void far ClrScr(void)
{
         word i;
         for (i=0 ; i<4000 ; *((byte *)Screen+i)=0x0020,i+=2 );
}



void far InitCrt(void)
{
         Screen    =(MemW(0,0x463)==0x03B4)?Pointer(0xB000,0):Pointer(0xB800,0);
         PcType    =MemB(0xF000,0xFFFE);
}

