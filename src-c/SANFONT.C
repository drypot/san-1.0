/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                                                      */
/*                                                      */

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
#include "fontver.h"

#define MaxList 60
#define Han 0
#define Eng 1

#define ImgSize ( 2*MaxList*(192+13))
/* sizeof (Img) + sizeof (Name) ) */

byte fileBuf[ImgSize];

byte (*Img)[2][MaxList][192]=(byte (*)[2][MaxList][192])fileBuf;
byte (*Name)[2][MaxList][13]=(byte (*)[2][MaxList][13])((char*)fileBuf+2*MaxList*192);

/*byte Img [2][MaxList][192];
byte Name[2][MaxList][13];
*/

word FileNameSetF=OK;
byte AuxBuf[255];
byte WorkFileName[80];
word Count[2];



byte *AppendSpace(byte *Data,word Len)
{
     word j;

     j=strlen(Data);
     while (j<Len) Data[j]=' ',j++;
     Data[j]=0;
     return Data;
}

word LoadEng(byte *Name,byte* Pos)
{
     int handler;

     if ((handler=_open(Name,0))==-1) return Error;
     if (_read(handler,Pos,2048)==-1) {
        _close(handler);
        return Error;
     }
     _close(handler);
     return OK;
}

void DeleteEng(word Pos)
{
     if (Pos>=Count[Eng]) return;

     movmem((*Name)[Eng][Pos+1],(*Name)[Eng][Pos],(MaxList-(Pos+1))*13);
     MemSetB((*Name)[Eng][MaxList-1],0,13);

     movmem((*Img)[Eng][Pos+1],(*Img)[Eng][Pos],(MaxList-(Pos+1))*192);
     MemSetB((*Img)[Eng][MaxList-1],0,192);

     Count[Eng]--;
}

void DeleteHan(word Pos)
{
     if (Pos>=Count[Han]) return;

     movmem((*Name)[Han][Pos+1],(*Name)[Han][Pos],(MaxList-(Pos+1))*13);
     MemSetB((*Name)[Han][MaxList-1],0,13);

     movmem((*Img)[Han][Pos+1],(*Img)[Han][Pos],(MaxList-(Pos+1))*192);
     MemSetB((*Img)[Han][MaxList-1],0,192);

     Count[Han]--;
}

word InsertEngBlank(word Pos)
{
     if (Pos>Count[Eng]) return Error;
     if (Pos==MaxList || Count[Eng]==MaxList) return Error;
     movmem((*Name)[Eng][Pos],(*Name)[Eng][Pos+1],(MaxList-(Pos+1))*13);
     movmem((*Img)[Eng][Pos],(*Img)[Eng][Pos+1],(MaxList-(Pos+1))*192);
     Count[Eng]++;
     return OK;
}

word InsertHanBlank(word Pos)
{
     if (Pos>Count[Han]) return Error;
     if (Pos==MaxList || Count[Han]==MaxList) return Error;
     movmem((*Name)[Han][Pos],(*Name)[Han][Pos+1],(MaxList-(Pos+1))*13);
     movmem((*Img)[Han][Pos],(*Img)[Han][Pos+1],(MaxList-(Pos+1))*192);
     Count[Han]++;
     return OK;
}

word LoadLib(void)
{
     int handler;
     word i;

     if ((handler=_open("sanfont.lib",0))==-1) return OK;
     if (filelength(handler)!=ImgSize+4) {
          _close(handler);
          return OK;
     }

     if ( _read (handler,&Count[Han],2) ==-1 ||
          _read (handler,&Count[Eng],2) ==-1 ||
          _read(handler,(*Img),ImgSize)==-1) {

         MemSetB((*Img)[Han],0,ImgSize);
         Count[Han]=Count[Eng]=0;
     }
     _close(handler);
     return OK;
}

word SaveLib(void)
{
     int handler;

     if ((handler=_creat("sanfont.lib",FA_ARCH))==-1) return Error;
     if ( _write(handler,&Count[Han],2) ==-1 ||
          _write(handler,&Count[Eng],2) ==-1 ||
          _write(handler,(*Img)[Han],ImgSize)==-1) return Error;
     _close(handler);
     return OK;
}

void SetMenu(void)
{

    DrawString (0,0, " ¬e ¸aÑw ¡¢¢ ‰ÅŸ¡‹¡ 1.2   ¸á¸bŠ¥(¸á) 1991-93 ˆåŠ‚”ÐbŠa ¸å¸a‰¬eÐb‰Á ¤b‹AÑe");
    DrawString (0,2, "ÛÇÛ¡¬a¶w‹i®AÛ¡Û¡Û¡Û¡Û¡Û¡ÛÁ");
    DrawString (0,3, "Û­                      Û­");
    DrawString (0,4, "Û­  ¹Á¶  : µwµb ¸åÑÅ   Û­");
    DrawString (0,5, "Û­  ¬wÐa  : ¸aÑw ¸åÑÅ   Û­");
    DrawString (0,6, "Û­                      Û­");
    DrawString (0,7, "Û­  Ctrl-N: ¸aÑw¡w ¬s·³ Û­");
    DrawString (0,8, "Û­  Ctrl-Y: ¸aÑw¡w ¬b¹A Û­");
    DrawString (0,9, "Û­                      Û­");
    DrawString (0,10,"Û­  Enter : ¸aÑw ·ª·q   Û­");
    DrawString (0,11,"Û­                      Û­");
    DrawString (0,12,"Û­  F10   : ·¡Ÿq ¤aŽ‘   Û­");
    DrawString (0,13,"Û­  Ctrl-K: ­A¤é/–¤é   Û­");
    DrawString (0,14,"Û­                      Û­");
    DrawString (0,15,"Û­  F2    : ·©”e ¸á¸w   Û­");
    DrawString (0,16,"Û­                      Û­");
    DrawString (0,17,"Û­  AltX  : ¸b´ó  aÃ±   Û­");
    DrawString (0,18,"ÛÅÛ¡Û¡Û¡Û¡Û¡Û¡Û¡Û¡Û¡Û¡Û¡ÛÃ");
    DrawString (0,20,"  ÑÁ·©¡w :");
    DrawString (0,21,"  ‹i®AÌe :");
    DrawString (0,22,"  ¥¥¥¡‹¡ :");
    HInverse();
    HDrawBox (30,1,45,VideoBot-1);
    HDrawBox (47,1,62,VideoBot-1);
}


word HY[2],Y[2];
word Mode;
word Win[2][4]={ { 32,2,43,} , { 49,2,60,}};

void SetBox(void)
{
    word i,j;
    byte Buf[10];

    HInverse();
    sprintf(Buf,"%2d",Count[Mode]);
    DrawString(Win[Mode][2]-2,1,Buf);
    for (i=0 ; i<VideoBot-3 ; i++) {
      if (HY[Mode]+i<Count[Mode]) {
        RestoreScreen(Win[Mode][0],i+2,Win[Mode][2],i+2,(byte*)(*Img)[Mode][HY[Mode]+i]);
        HClrBox(Win[Mode][0]-1,i+2,Win[Mode][0]-1,i+2);
        HClrBox(Win[Mode][2]+1,i+2,Win[Mode][2]+1,i+2);

      } else
        HClrBox(Win[Mode][0]-1,i+2,Win[Mode][2]+1,i+2);
    }
}

void SetPos (word NewX)
{
    word i;

    if (NewX==HY[Mode]+Y[Mode]) return;
    if (NewX>Count[Mode]+100) NewX=0;
    if (NewX==Count[Mode] && NewX==MaxList ) NewX=Count[Mode]-1;
    if (NewX>Count[Mode])    NewX=Count[Mode];
    if (NewX<HY[Mode])
         if (NewX<Y[Mode]) HY[Mode]=0,Y[Mode]=NewX;
         else        Y[Mode]=0,HY[Mode]=NewX-Y[Mode];
    else if (NewX<=HY[Mode]+VideoBot-4) {
            Y[Mode]=NewX-HY[Mode];
            return;
         }
    else Y[Mode]=VideoBot-4,HY[Mode]=NewX-Y[Mode];
    SetBox();
}

void Librarian(void)
{
    word i,j;
    word k;
    word MainWay;
    byte Buf[80];
    byte Buf2[80];
    char _drv[MAXDRIVE],_dir[MAXDIR],_name[MAXFILE],_ext[MAXEXT];

    Mode=Eng;
    SetBox();
    Mode=Han;
    SetBox();

    KeyboardType=3;

    do  {
        j=HY[Mode]+Y[Mode];
        HNormal();
        HClrBox(11,14,23,14);
        if ((*Name)[Mode][j][0]!=0) strcpy(WorkFileName,(*Name)[Mode][j]);
        DrawString(11,20,"            ");
        DrawString(11,20,(*Name)[Mode][j]);
        DrawString(11,21,KeyboardType == 3 ? "­A¤é¯¢" : "–¤é¯¢" );
        DrawString(11,22,"´aŸq”a¶… Ðe‹i");
        DrawString(11,23,"abcdefghijklm");
        HInverse();
        HSetInverse(Win[Mode][0],Y[Mode]+2,Win[Mode][2],Y[Mode]+2);
        GetKey();
        MainWay=Way;
        HSetInverse(Win[Mode][0],Y[Mode]+2,Win[Mode][2],Y[Mode]+2);
        switch (Way) {
          case CtrlK  :KeyboardType= KeyboardType==2 ? 3 : 2 ;
                       break;
          case Home   :SetPos(0);
                       break;
          case End    :SetPos(Count[Mode]);
                       break;
          case CtrlS  :
          case CtrlA  :
          case Left   :
          case CtrlD  :
          case CtrlF  :
          case Right  :
          case Tab    :Mode=(Mode==Han)? Eng :Han;
                       SetPos(HY[Mode]+Y[Mode]);
                       break;
          case Up     :
          case CtrlE  :(j==0   ) ? Bell() : SetPos (j-1);
                       break;
          case ' '    :
          case Down   :
          case CtrlX  :(j==Count[Mode]) ? Bell() :
                       (j==Count[Mode]-1 && j==MaxList-1) ? Bell() :
                                                            SetPos (j+1);
                       break;
          case PgDn   :
          case CtrlC  :(j==Count[Mode]) ? Bell() :
                       (j==Count[Mode]-1 && j==MaxList-1) ? Bell() :
                                                            SetPos (j+8);
                       break;
          case PgUp   :
          case CtrlR  :(j==0   ) ? Bell() : SetPos (j-8);
                       break;
          case Del    :
          case CtrlG  :
          case CtrlY  :i=True;
                       k=HGetYesNo(" ¡¢¢µA¬á ¬b¹A ÐiŒa¶a? ",&i);
                       Way=Del;
                       if (k==OK)
                          if (i) {
                             if (Mode==Han) {
                                 DeleteHan(j);
                             } else {
                                 DeleteEng(j);
                             }
                       }
                       SetBox();
                       break;
          case CtrlN  :
          case Ins    :if (Mode==Han) {
                          if (Count[Han]==MaxList) {
                              HSayMes ("¸aÑw ¡¢¢·¡ ÂÀv¯s“¡”a. Ï©¶a ´ô“e ˆõ·i »¡¶… Ò ¯©Ð—Ð º­A¶a.");
                              break;
                          }
                          strcpy(Buf,"*.KOR");
                          if (HGetEng("¬a¶wÐa¯© ¸aÑw· ·¡Ÿq·i °áº­A¶a.",Buf,79)!=OK) break;
                          if (HGetFileName(Buf)!=OK) break;
                          fnsplit(Buf,_drv,_dir,_name,_ext);
                          strcpy(Buf2,"");
                          strcat(Buf2,_name);
                          strcat(Buf2,_ext);
                          if (access(Buf2,0)==-1) {
                              HSayMes("¬a¶wÐa¯© ¸aÑw·e '¬e'·¡ —i´á·¶“e —¡BÉ¡Ÿ¡µA ·¶´á´¡ Ðs“¡”a.");
                              break;
                          }
                          if (SetHan(Buf2)!=OK) break;
                          Buf[0]=0;
                          HanF=True;
                          if (HGetMes(" ¬¡¶… ¸aÑw ¡w·i ¸â´áº¯³®a. ",Buf,12)==OK) {
                             InsertHanBlank(j);
                             HClrBox(Win[Han][0],Y[Mode]+2,Win[Han][2],Y[Mode]+2);
                             DrawString(Win[Han][0],Y[Mode]+2,Buf);
                             strcpy((*Name)[Han][j],Buf2);
                             SaveScreen(Win[Han][0],Y[Mode]+2,Win[Han][2],Y[Mode]+2,(byte*)(*Img)[Han][j]);
                          }
                       } else {
                          if (Count[Eng]==MaxList) {
                              HSayMes ("¸aÑw ¡¢¢·¡ ÂÀv¯s“¡”a. Ï©¶a ´ô“e ˆõ·i »¡¶… Ò ¯©Ð—Ð º­A¶a.");
                              break;
                          }
                          strcpy(Buf,"*.ENG");
                          if (HGetEng("¬a¶wÐa¯© ¸aÑw· ·¡Ÿq·i °áº­A¶a.",Buf,79)!=OK) break;
                          if (HGetFileName(Buf)!=OK) break;
                          fnsplit(Buf,_drv,_dir,_name,_ext);
                          strcpy(Buf2,"");
                          strcat(Buf2,_name);
                          strcat(Buf2,_ext);
                          if (access(Buf2,0)==-1) {
                              HSayMes("¬a¶wÐa¯© ¸aÑw·e '¬e'·¡ —i´á·¶“e —¡BÉ¡Ÿ¡µA ·¶´á´¡ Ðs“¡”a.");
                              break;
                          }
                          if (SetEng(Buf2)!=OK) break;
                          strcpy(Buf,_name);
                          if (HGetMes(" ¬¡¶… ¸aÑw ¡w·i ¸â´áº¯³®a. ",Buf,12)==OK) {
                             InsertEngBlank(j);
                             strcpy((*Name)[Eng][j],Buf2);
                             HClrBox(Win[Eng][0],Y[Mode]+2,Win[Eng][2],Y[Mode]+2);
                             DrawString(Win[Eng][0],Y[Mode]+2,Buf);
                             SaveScreen(Win[Eng][0],Y[Mode]+2,Win[Eng][2],Y[Mode]+2,(*Img)[Eng][j]);
                          }
                       }
                       SetBox();
                       break;
          case Return :if (Mode==Eng) SetEng((*Name)[Eng][j]);
                       else           SetHan((*Name)[Han][j]);
                       break;
          case F2     :HSayStatus(" ¡¢¢·i ¸á¸wÐs“¡”a. ");
                       if (SaveLib()!=OK) {
                          HSayIOErr();
                       }
                       HCloseWindow();
                       break;
          case ESC    :
          case AltX   :i=False;
                       if (HGetYesNo(" ¸b´ó·i  aÃ¡¯¡‰V¯s“¡Œa? ",&i)!=OK) {
                          MainWay=Error;
                       } else {
                          if (i==False) MainWay=Error;
                       }
                       break;
          case F10    :if (Mode==Han) {
                          if (SetHan((*Name)[Han][j])!=OK)
                              HSayIOErr();
                          else
                            Buf[0]=0;
                          if (HGetMes(" ¬¡¶… ¸aÑw ¡w·i ¸â´áº¯³®a. ",Buf,12)==OK) {
                             HClrBox(Win[Han][0],Y[Mode]+2,Win[Han][2],Y[Mode]+2);
                             DrawString(Win[Han][0],Y[Mode]+2,Buf);
                             SaveScreen(Win[Han][0],Y[Mode]+2,Win[Han][2],Y[Mode]+2,(byte*)(*Img)[Han][j]);
                          }
                       } else {
                          if (SetEng((*Name)[Eng][j])!=OK)
                              HSayIOErr();
                          else
                              Buf[0]=0;
                          if (HGetMes(" ¬¡¶… ¸aÑw ¡w·i ¸â´áº¯³®a. ",Buf,12)==OK) {
                             HClrBox(Win[Eng][0],Y[Mode]+2,Win[Eng][2],Y[Mode]+2);
                             DrawString(Win[Eng][0],Y[Mode]+2,Buf);
                             SaveScreen(Win[Eng][0],Y[Mode]+2,Win[Eng][2],Y[Mode]+2,(byte*)(*Img)[Eng][j]);
                          }
                       }
                       break;

        }
    } while (MainWay !=AltX && MainWay !=ESC );
}

void main(void)
{
       int handler;
       word i,j;
       char Drive[MAXDRIVE],Dir[MAXDIR],(*Name)[MAXFILE];

       clrscr();
       puts(FontVersion);
       if (OpenHanDrv("sanvideo.drv")!=OK) {
          puts("\nError in Loading 'SANVIDEO.DRV'.");
          puts("Please, run 'SANSETUP' for Making Video Driver.");
          exit(0);
       }

       Win[0][3]=Win[1][3]=VideoBot-2;


       if (InitWindow()!=OK ) {
         puts("\Not Enough Memeory.");
         exit(0);
       }

       if (SetExtEng("san.lin")!=OK) {
         puts("\Error In Reading 'SAN.LIN'.");
         exit(0);
       }
       if (SetGrf("san.grf")!=OK) {
         puts("\Error In Reading 'SAN.GRF'.");
         exit(0);
       }
       if (SetEng("san.eng")!=OK) {
         puts("\Error In Reading 'SAN.ENG'.");
         exit(0);
       }
       if (SetHan("san.kor")!=OK) {
         puts("\Error In Reading 'SAN.KOR'.");
         exit(0);
       }
       LoadLib();
       GrMode();
       HClrScr();

       SetMenu();
       Librarian();

       TextMode();
       clrscr();
       puts(FontVersion);
}
