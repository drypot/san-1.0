/*                                                          */
/*                                                          */
/*        Han-Gul Graphic Window Interface version 1.0      */
/*                                                          */
/*        programed by Magic Book                           */
/*                                                          */
/*                                                          */
/*                                                          */

#include "memory.h"
#include "crt.h"
#include "handrv.h"
#include "hwindow.h"
#include "edit.h"
#include "hanja.h"
#include <alloc.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <mem.h>
#include <dos.h>

struct  WHand far *NWP=Null,far *AWP;
byte    far *WindowStack;
word    StackP;

void HOpenWindow (word X1,word Y1,word X2,word Y2)
{
    AWP=NWP;
    NWP=(struct WHand*)(WindowStack+StackP);
    StackP +=sizeof(*NWP);
    NWP->X1      =X1;
    NWP->Y1      =Y1;
    NWP->X2      =X2;
    NWP->Y2      =Y2;
    NWP->Ptr     =WindowStack+StackP;
    StackP      +=(X2-X1+3)*(Y2-Y1+2)*16;
    NWP->LastWind=AWP;
    NWP->SelectWind=False;
    SaveScreen (X1,Y1,X2+2,Y2+1,NWP->Ptr);
    HDrawBox (X1,Y1,X2,Y2);
    HClrBox (X1+1,Y1+1,X2-1,Y2-1);
    DrawShadowFull(X2+1,Y1+1,X2+1,Y2);
    DrawShadowHalf(X1+1,Y2+1,X2+1,Y2+1);
    DrawShadowHalfDown(X2+1,Y1,X2+1,Y1);
}

void HOpenWindowSimple (word X1,word Y1,word X2,word Y2)
{
    AWP=NWP;
    NWP=(struct WHand*)(WindowStack+StackP);
    StackP +=sizeof(*NWP);
    NWP->X1      =X1;
    NWP->Y1      =Y1;
    NWP->X2      =X2;
    NWP->Y2      =Y2;
    NWP->Ptr     =WindowStack+StackP;
    StackP      +=(X2-X1+1)*(Y2-Y1+1)*16;
    NWP->LastWind=AWP;
    NWP->SelectWind=False;
    SaveScreen (X1,Y1,X2,Y2,NWP->Ptr);
    HClrBox (X1,Y1,X2,Y2);
}

void HCloseWindow(void)
{
    AWP=NWP->LastWind;
    RestoreScreen(NWP->X1,NWP->Y1,NWP->X2+2,NWP->Y2+1,NWP->Ptr);
    StackP-=sizeof(*NWP)+(NWP->X2-NWP->X1+3)*(NWP->Y2-NWP->Y1+2)*16;
    NWP=AWP;
}

void HCloseWindowSimple(void)
{
    AWP=NWP->LastWind;
    RestoreScreen(NWP->X1,NWP->Y1,NWP->X2,NWP->Y2,NWP->Ptr);
    StackP-=sizeof(*NWP)+(NWP->X2-NWP->X1+1)*(NWP->Y2-NWP->Y1+1)*16;
    NWP=AWP;
}

word HSayStatus (byte far *Data)
{
    int i,j;

    i=(strlen(Data)+1)/2;
    if (i<20) i=20;
    HInverse();
    HOpenWindow (MidX-i,MidY-1,MidX+1+i,MidY+1);
    HWriteMid(MidY,Data);
    HNormal();
    return OK;
}

word HSayMes (byte far *Data)
{
    if (HSayStatus (Data) != OK) return Error;
    HWait(Message);
    HCloseWindow();
    return OK;
}

word HSayIOErr(void)
{
    HSayMes (strerror(errno));
    return Error;
}

void HSayBottom(byte far *Data)
{
    byte *Buf;

    Buf=(byte*)WindowStack+StackP;
    SaveScreen (0,VideoBot,VideoRight,VideoBot,Buf);
    HClrBox(0,VideoBot,VideoRight,VideoBot);
    HWriteMid(VideoBot,Data);
    EngCur1(MidX+1-(strlen(Data)+1)/2+strlen(Data),VideoBot);
    GetKey();
    RestoreScreen (0,VideoBot,VideoRight,VideoBot,Buf);
    SetHotKeyF();
}

byte LineData[256],DataType[256];
word GM_HX,GM_X;
word BoxStart,BoxEnd,BoxSize,DataSize;
word StartF;

void ClearLineData()
{
     word i;

     for (i=0; i<sizeof(LineData) ; i++) (LineData[i]=' '),(DataType[i]=0);
     GM_HX=0;
     GM_X=0;
}

void PrintLineData()
{
     word i,j;

     i=(DataType[GM_HX]==2)? DrawEng(BoxStart,MidY+2,' '),1:0;
     j=(DataType[GM_HX+BoxSize]==1)? DrawEng(BoxEnd,MidY+2,' '),0:1;
     j+=BoxSize;
     while (i<j)
       if (DataType[GM_HX+i]==0) DrawEng(BoxStart+i,MidY+2,LineData[GM_HX+i]),i++;
       else DrawHan(BoxStart+i,MidY+2,*((word *)(LineData+GM_HX+i))),i+=2;
}

void _SetPos(word NewX)
{
     if (NewX>255) Bell(),NewX=255;
     if (DataType[NewX]==2)
        if  (GM_HX+GM_X<NewX)
            if (NewX==255) Bell(),NewX--;
            else           NewX++;
        else            NewX--;
     if (GM_HX>NewX)  (GM_HX=NewX),(GM_X=0);
     else if (GM_HX+BoxSize>NewX) GM_X=NewX-GM_HX;
     else (GM_X=BoxSize),(GM_HX=NewX-GM_X);
     if (GM_X==BoxSize && DataType[NewX]==1) GM_HX++,GM_X--;
}

void _AccessHan()
{
    word i,j;

    movmem (LineData +GM_HX+GM_X,LineData +GM_HX+GM_X+2,256-(GM_HX+GM_X+2));
    movmem (DataType+GM_HX+GM_X,DataType+GM_HX+GM_X+2,256-(GM_HX+GM_X+2));
    if (DataType[255]==1) (DataType[255]=0),(LineData[255]=' ');
    *((word *)(LineData+GM_HX+GM_X))=HanKey;
    DataType[GM_HX+GM_X]=1;
    DataType[GM_HX+GM_X+1]=2;
    if (GM_HX+GM_X<254) _SetPos (GM_HX+GM_X+2);
}

word SpaceFrom(word Row)
{
    for (;Row<255;Row++) if (LineData[Row]!=' ') return False;
    return True;
}

word EndLineData(void)
{
    word i=255;

    while (i>0 && LineData[i]==' ' && DataType[i]==0 ) i--;
    if (LineData[i]!=' ' && i<256) i++;
    return i;
}



void  _AccessEng(word F4OKF)
{
    word i,j,NowX;

    if (Key==' ' && ShiftPressed() ) return;
    NowX=GM_HX+GM_X;
      switch (Way) {
         case Home     :_SetPos(0);
                        break;
         case End      :_SetPos(EndLineData());
                        break;
         case Left     :
         case CtrlS    :(NowX==0)?   Bell() : _SetPos(NowX-1);
                        break;
         case Right    :
         case CtrlD    :(NowX==255)? Bell() : _SetPos(NowX+1);
                        break;
         case CtrlLeft :
         case CtrlA    :i=NowX;
                        while (i>0 &&(!isalnum(LineData[i-1])&& isascii(LineData[i-1]))) i--;
                        while (i>0 &&( isalnum(LineData[i-1])||!isascii(LineData[i-1]))) i--;
                        _SetPos(i);
                        break;
         case CtrlRight:
         case CtrlF    :i=NowX;
                        if (!SpaceFrom(i))
                          while (i<255&&(isalnum(LineData[i])||!isascii(LineData[i]))) i++;
                        if (!SpaceFrom(i))
                          while (i<255&&(!isalnum(LineData[i])&&isascii(LineData[i]))) i++;
                        _SetPos(i);
                        break;
         case Return   :break;
         case ESC      :break;
         case BackSpace:if (NowX>0)
                           if (DataType[NowX-1]==2)
                               movmem (LineData +GM_HX+GM_X,LineData +GM_HX+GM_X-2,256-(GM_HX+GM_X)),
                               movmem (DataType+GM_HX+GM_X,DataType+GM_HX+GM_X-2,256-(GM_HX+GM_X)),
                               LineData[255]=' ',
                               LineData[254]=' ',
                               DataType[255]=0,
                               DataType[254]=0,
                               _SetPos(NowX-2);
                           else
                               movmem (LineData +GM_HX+GM_X,LineData +GM_HX+GM_X-1,256-(GM_HX+GM_X)),
                               movmem (DataType+GM_HX+GM_X,DataType+GM_HX+GM_X-1,256-(GM_HX+GM_X)),
                               LineData[255]=' ',
                               DataType[255]=0,
                               _SetPos(NowX-1);
                        else Bell();
                        break;
         case CtrlG    :
         case Del      :if (DataType[NowX]==0) {
                           movmem(LineData+GM_HX+GM_X+1,LineData+GM_HX+GM_X,256-(GM_HX+GM_X+1));
                           movmem(DataType+GM_HX+GM_X+1,DataType+GM_HX+GM_X,256-(GM_HX+GM_X+1));
                           LineData[255]=' ';
                           DataType[255]=0;
                        } else {
                           movmem(LineData+GM_HX+GM_X+2,LineData+GM_HX+GM_X,256-(GM_HX+GM_X+2));
                           movmem(DataType+GM_HX+GM_X+2,DataType+GM_HX+GM_X,256-(GM_HX+GM_X+2));
                           LineData[255]=' ',
                           LineData[254]=' ',
                           DataType[255]=0,
                           DataType[254]=0;
                        }
                        break;
         case CtrlY    :ClearLineData();
                        _SetPos (0);
                        break;
         case F4       :
         case AltG     :if (!F4OKF) break;
                        j=SelectGrf();
                        if (j!=Error) {
                          movmem (LineData +GM_HX+GM_X,LineData +GM_HX+GM_X+2,256-(GM_HX+GM_X+2));
                          movmem (DataType+GM_HX+GM_X,DataType+GM_HX+GM_X+2,256-(GM_HX+GM_X+2));
                          if (DataType[255]==1) (DataType[255]=0),(LineData[255]=' ');
                          *((word *)(LineData+GM_HX+GM_X))=j;
                          DataType[GM_HX+GM_X]=1;
                          DataType[GM_HX+GM_X+1]=2;
                          if (GM_HX+GM_X<254) _SetPos (GM_HX+GM_X+2);
                        }
                        Way=Error;
                        break;
         case F9       :if (DataType[GM_X]==1) {
                           if (TranslateCode(LineData+GM_HX+GM_X,BoxStart+GM_X+1,MidY+3))
                              _SetPos(NowX+1);
                           HInverse();
                        }
                        Way=Error;
                        break;
         default       :if (!Held(Way,32,127)) break;
                        if (StartF) ClearLineData(),NowX=GM_HX+GM_X;
                        else movmem (LineData+GM_HX+GM_X,LineData+GM_HX+GM_X+1,256-(GM_HX+GM_X+1)),
                             movmem (DataType+GM_HX+GM_X,DataType+GM_HX+GM_X+1,256-(GM_HX+GM_X+1));
                        if (DataType[255]==1) (DataType[255]=0),(LineData[255]=' ');
                        LineData[NowX]=Key;
                        DataType[NowX]=0;
                        if (NowX<255) _SetPos (NowX+1);
      }
}

word iy;

word HGetMes (byte far *Data,byte far *OutString,word Max)
{
    word i,j;

    Push(InverseF);
    Push(HanF);
    HInverse();
    ClearLineData();
    StartF=True;
    DataSize=strlen(OutString);
    MemCopy(OutString,LineData,DataSize);
    for (i=0 ; i<256 ; )
       if (LineData[i]>127) (DataType[i]=1),(DataType[i+1]=2),i+=2;
       else DataType[i++]=0;
    if (strlen (Data)<36)
        BoxStart=MidX-20;
    else {
        BoxStart=MidX+1-(strlen(Data)+1)/2;
        if (BoxStart>3) BoxStart-=3;
    }
    iy=MidY;
    BoxEnd=MidX+1+(MidX-BoxStart);
    HOpenWindow (BoxStart-1,iy,BoxEnd+1,iy+3);
    HWriteMid(iy+1,Data);
    BoxSize=BoxEnd-BoxStart;
    GM_X=DataSize;
    if (GM_X>BoxSize) (GM_HX=GM_X-BoxSize),(GM_X=BoxSize);
    else            GM_HX=0;
    PrintLineData();
    HanResult=2;
    do {
       if (StartF && GM_X!=0) HSetInverse(BoxStart,iy+2,BoxStart+GM_X-1,iy+2);
       if (HanResult!=3)
          if (HanF)
               if (DataType[GM_HX+GM_X]==1 || HanResult==0 || HanResult==1)
                    HanCur2(BoxStart+GM_X,iy+2);
               else HanCur1(BoxStart+GM_X,iy+2);
          else
               if (DataType[GM_HX+GM_X]==1)
                    EngCur2(BoxStart+GM_X,iy+2);
               else EngCur1(BoxStart+GM_X,iy+2);
       GetHan();
       SetHotKeyF();
       if (StartF && GM_X!=0) HSetInverse(BoxStart,iy+2,BoxStart+GM_X-1,iy+2);
       switch (HanResult) {
         case 0 :if (GM_HX+GM_X<255) {
                   if (StartF) ClearLineData();
                   if (GM_X==BoxSize) GM_HX++,PrintLineData(),_SetPos(GM_HX+GM_X-1);
                   j=(DataType[GM_HX+BoxSize-2]==1)? DrawEng(BoxEnd,iy+2,' '),0:1;
                   j+=BoxSize;
                   i=GM_X+2;
                   while (i<j)
                     if (DataType[GM_HX+i-2]==0) DrawEng(BoxStart+i,iy+2,LineData[GM_HX+i-2]),i++;
                     else DrawHan(BoxStart+i,iy+2,*(word *)(LineData+GM_HX+i-2)),i+=2;
                   DrawHan(BoxStart+GM_X,iy+2,HanKey);
                 }
                 else (HanResult=1),Bell();
                 break;
         case 1 :DrawHan(BoxStart+GM_X,iy+2,HanKey);
                 break;
         case 2 :
         case 3 :_AccessHan();
                 PrintLineData();
                 break;
         case 4 :_AccessHan();
                 if (Key==' ' && ShiftPressed() ) HanF=!HanF;
                 else {
                   if (Way==F9) _SetPos(GM_HX+GM_X-1);
                   _AccessEng(OK);
                 }
                 PrintLineData();
                 break;
         case 5 :if (Key==' ' && ShiftPressed() ) HanF=!HanF;
                 else {
                   if (! (StartF && (Way==CtrlD ||
                                     Way==CtrlF ||
                                     Way==CtrlRight ||
                                     Way==Right ||
                                     Way==' ' ))) _AccessEng(OK);
                   PrintLineData();
                 }
                 break;
         case 6 :PrintLineData();
                 break;
       }
       StartF=False;
    } while ( Way!=Return && Way!=ESC && !HotKeyF);
    HCloseWindow();
    Pop(HanF);
    Pop(InverseF);
    if (Way==Return) {
         i=Max;
         while (LineData[i-1]==' ' && i>0) i--;
         LineData[i]='\0';
         i= (Max>GM_HX+GM_X+1) ? GM_HX+GM_X : Max-1;
         AppendSpace(LineData,i);
         LineData[Max]=0;
         strcpy (OutString,LineData);
         return OK;
    } else return Error;
}

word HGetEng (byte far *Data,byte far *OutString,word Max)
{
    word i,j;

    Push(InverseF);
    HInverse();
    ClearLineData();
    StartF=True;
    DataSize=strlen(OutString);
    MemCopy(OutString,LineData,DataSize);
    MemSetB(DataType,0,256);
    if (strlen (Data)<36)
        BoxStart=MidX-20;
    else {
        BoxStart=MidX+1-(strlen(Data)+1)/2;
        if (BoxStart>3) BoxStart-=3;
    }
    iy=MidY;
    BoxEnd=MidX+1+(MidX-BoxStart);
    HOpenWindow (BoxStart-1,iy,BoxEnd+1,iy+3);
    HWriteMid(iy+1,Data);
    BoxSize=BoxEnd-BoxStart;
    GM_X=DataSize;
    if (GM_X>BoxSize) (GM_HX=GM_X-BoxSize),(GM_X=BoxSize);
    else            GM_HX=0;
    PrintLineData();
    HanResult=2;
    do {
       if (StartF && GM_X!=0) HSetInverse(BoxStart,iy+2,BoxStart+GM_X-1,iy+2);
       EngCur1(BoxStart+GM_X,iy+2);
       if (StartF && GM_X!=0) HSetInverse(BoxStart,iy+2,BoxStart+GM_X-1,iy+2);
       GetKey();
       SetHotKeyF();
       if (! (StartF && (Way==CtrlD ||
                         Way==CtrlF ||
                         Way==CtrlRight ||
                         Way==Right ||
                         Way==' ' ))) _AccessEng(False);
       PrintLineData();
       StartF=False;
    } while ( Way!=Return && Way!=ESC && !HotKeyF);
    HCloseWindow();
    Pop(InverseF);
    if (Way==Return) {
         i=Max;
         while (LineData[i-1]==' ' && i>0) i--;
         LineData[i]='\0';
         strcpy (OutString,LineData);
         return OK;
    } else return Error;
}


word HGetYesNo (byte far *Data,word *OutBoolean)
{
    int i;

    Push(InverseF);
    HInverse();
    i=(strlen(Data)+1)/2;
    if (i<20) i=20;
    HOpenWindow (MidX-i,MidY-2,MidX+1+i,MidY+1);
    HWriteMid(MidY-1,Data);
    HWriteMid(MidY,"µ      ´a“¡¶a");
    Pop(InverseF);
    i=*OutBoolean;
    if (*OutBoolean) HSetInverse(MidX-8,MidY,MidX-2,MidY);
    else             HSetInverse(MidX+1,MidY,MidX+8,MidY);
    do {
       if (*OutBoolean !=i ) {
           HSetInverse(MidX-8,MidY,MidX-2,MidY);
           HSetInverse(MidX+1,MidY,MidX+8,MidY);
       }
       i=*OutBoolean;
       GetKey();
       SetHotKeyF();
       if (Way=='Y' || Way=='N') Way+='a'-'A';
       switch (Way) {
         case Right:
         case Left :
         case ' '  :
         case CtrlS:
         case CtrlD:*OutBoolean=!*OutBoolean;
                    break;
         case CtrlN:
         case 'n'  :*OutBoolean=No;
                    break;
         case CtrlY:
         case 'y'  :*OutBoolean=Yes;
                    break;
       }
    } while (Way!='y'    && Way!='n'   &&
             Way!=CtrlY  && Way!=CtrlY &&
             Way!=Return && Way!=ESC   &&
             !HotKeyF);
    HCloseWindow();
    return (Way==ESC || HotKeyF==True) ? Error:OK;
}

void HWriteMid (word Y,byte far *Data)
{
    word i;

    for (i=0 ; i<strlen(Data) ; i++)
      if (Data[i]<' ') {
         Data[i]=0;
         break;
      }
    DrawString (MidX+1-(strlen(Data)+1)/2,Y,Data);
}

void HDrawBox (word X1,word Y1,word X2,word Y2)
{
    word i;

    for (i=X1+1 ; i<=X2-1 ;i++) DrawEng(i,Y1,129),DrawEng(i,Y2,135);
    for (i=Y1+1 ; i<=Y2-1 ;i++) DrawEng(X1,i,131),DrawEng(X2,i,133);
    DrawEng (X1,Y1,128);
    DrawEng (X2,Y1,130);
    DrawEng (X1,Y2,134);
    DrawEng (X2,Y2,136);
}

void HDrawBar (word X1,word X2,word Y)
{
    word i;

    for (i=X1 ; i<=X2 ;i++) DrawEng(i,Y,132);
}

void Explain(char *Data)
{
   DrawString(0,VideoBot,Data);
   HClrBox(strlen(Data),VideoBot,VideoRight,VideoBot);
}


word HWait (word option)
{
    if (option==Message) HSayBottom ("´a¢ ‹i®Aa ’‰œá º­A¶a.");
    else {
      GetKey();
      SetHotKeyF();
    }
    return Way;
}

void Bell (void)
{
     sound (5000);
     delay (50);
     nosound();
}

word InitWindow(void)
{
     WindowStack=farmalloc(WindowStackSize);
     return (WindowStack==NULL)?Error:OK;
}
