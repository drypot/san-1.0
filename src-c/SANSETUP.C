/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                        Set UP                        */
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
#include "setver.h"

#define  MaxMacroString 128

word   extern  MacroArray['Z'-'A'+1][MaxMacroString];
byte   extern  ShiftArray['Z'-'A'+1][MaxMacroString];
byte   extern  HanFArray ['Z'-'A'+1];

word   MaxPaper1=5000,MaxPaper2=2000,MaxPaper3=1000;
word   AuxScreenMode=1;
word   InsertF       =True,
       IndentF       =True;
byte   Work[80];
byte   HanFontName[13]="san.kor";
word   HanFontType;
byte   EngFontName[13]="san.eng";
byte   GrfFontName[13]="san.grf";
char extern StdExt[80];
word   KeyDelay=1;
word   KeyRate=9;
word   BlockLeft=3;
word   BlockRight=77;
word   BlockIndent=2;
word   BlockSpace=0;
word   BlockSortType,PrepareF=True,BackUpF=True,OsSaveF=False;
word   ProtectLineF=False;
word   CutLineF=False;
word   CutLineLimit=70;
word   SmartRefleshF=True;
word   HanGulCodeType=0;
byte   RunItems[5][80];
byte   RunTitle[5][19];
word   PrintingType,LeftMargin=5,PrinterPort=1;

byte   WorkFileName[80];
word   FileNameSetF;
byte   AuxBuf[256];

word   BotMargin=55;
word   TopMargin=5;
word   PageLength=60;
word   SpaceLine;

void ChangeLanguage(void)
{
    HanF=!HanF;
}

byte *AppendSpace(byte *Data,word Len)
{
     word j;

     j=strlen(Data);
     while (j<Len) Data[j]=' ',j++;
     Data[j]=0;
     return Data;
}

word   RetrieveOptions(void)
{
       int handler;
       byte name[80];
       int i;

       strcpy(name,"san.cnf");
       if ((handler=_open(name,0))==-1) return Error;
     /*  if (filelength(handler)!=664) {
          return Error;
       } */
       if (_read(handler,&InsertF,2)       ==-1 ||
           _read(handler,&IndentF,2)       ==-1 ||
           _read(handler,&AuxScreenMode,2) ==-1 ||
           _read(handler,HanFontName,13)   ==-1 ||
           _read(handler,&HanFontType,2)   ==-1 ||
           _read(handler,EngFontName,13)   ==-1 ||
           _read(handler,GrfFontName,13)   ==-1 ||
           _read(handler,StdExt,80)       ==-1 ||
           _read(handler,&MaxPaper1,2)   ==-1 ||
           _read(handler,&MaxPaper2,2)   ==-1 ||
           _read(handler,&MaxPaper3,2)   ==-1 ||
           _read(handler,&KeyDelay   ,2)==-1 ||
           _read(handler,&KeyRate    ,2)==-1 ||
           _read(handler,&BlockLeft  ,2)==-1 ||
           _read(handler,&BlockRight ,2)==-1 ||
           _read(handler,&BlockIndent,2)==-1 ||
           _read(handler,&BlockSortType,2)==-1 ||
           _read(handler,&PrepareF,2)==-1 ||
           _read(handler,&BackUpF,2)==-1 ||
           _read(handler,&OsSaveF,2)==-1 ||
           _read(handler,&ProtectLineF,2)==-1 ||
           _read(handler,&PrintingType,2)==-1 ||
           _read(handler,&KeyboardType,2)==-1||
           _read(handler,&RunItems,5*80)==-1 ||
           _read(handler,&RunTitle,5*19)==-1 ||
           _read(handler,&SmartRefleshF,2)==-1||
           _read(handler,&HanGulCodeType,2)==-1 ||
           _read(handler,&CutLineF,2)==-1 ||
           _read(handler,&LeftMargin,2)==-1 ||
           _read(handler,&PrinterPort,2)==-1 ||
           _read(handler,&BlockSpace,2)==-1  ||
           _read(handler,MacroArray,6656)==-1 ||
           _read(handler,&PageLength,2)==-1 ||
           _read(handler,&TopMargin,2)==-1 ||
           _read(handler,&BotMargin,2)==-1 ||
           _read(handler,ShiftArray,3328)==-1 ||
           _read(handler,HanFArray,26)==-1 ||
           _read(handler,&SpaceLine,2)==-1 )
            {
           _close(handler);
           return Error;
       } else {
           _close(handler);
           return OK;
       }
}

word SaveOptions(void)
{
     int handler;


       if ((handler=_creat("san.cnf",FA_ARCH))==-1) {
            return Error;
       }
       if (_write(handler,&InsertF,2)    ==-1 ||
           _write(handler,&IndentF,2)    ==-1 ||
           _write(handler,&AuxScreenMode,2) ==-1 ||
           _write(handler,HanFontName,13)==-1 ||
           _write(handler,&HanFontType,2)==-1 ||
           _write(handler,EngFontName,13)==-1 ||
           _write(handler,GrfFontName,13)==-1 ||
           _write(handler,StdExt,80)    ==-1 ||
           _write(handler,&MaxPaper1,2)==-1 ||
           _write(handler,&MaxPaper2,2)==-1 ||
           _write(handler,&MaxPaper3,2)==-1 ||
           _write(handler,&KeyDelay   ,2)==-1 ||
           _write(handler,&KeyRate    ,2)==-1 ||
           _write(handler,&BlockLeft  ,2)==-1 ||
           _write(handler,&BlockRight ,2)==-1 ||
           _write(handler,&BlockIndent,2)==-1 ||
           _write(handler,&BlockSortType,2)==-1 ||
           _write(handler,&PrepareF,2)==-1 ||
           _write(handler,&BackUpF,2)==-1 ||
           _write(handler,&OsSaveF,2)==-1 ||
           _write(handler,&ProtectLineF,2)==-1 ||
           _write(handler,&PrintingType,2)==-1 ||
           _write(handler,&KeyboardType,2)==-1||
           _write(handler,&RunItems,5*80)==-1 ||
           _write(handler,&RunTitle,5*19)==-1 ||
           _write(handler,&SmartRefleshF,2)==-1||
           _write(handler,&HanGulCodeType,2)==-1 ||
           _write(handler,&CutLineF,2)==-1 ||
           _write(handler,&LeftMargin,2)==-1 ||
           _write(handler,&PrinterPort,2)==-1 ||
           _write(handler,&BlockSpace,2)==-1 ||
           _write(handler,MacroArray,6656)==-1 ||
           _write(handler,&PageLength,2)==-1 ||
           _write(handler,&TopMargin,2)==-1 ||
           _write(handler,&BotMargin,2)==-1 ||
           _write(handler,ShiftArray,3328)==-1 ||
           _write(handler,HanFArray,26)==-1 ||
           _write(handler,&SpaceLine,2)==-1 ||
           _close(handler)               ==-1 )
       return Error;
     else return OK;
}

void SetInverse(word X1,word Y1,word X2,word Y2)
{
    word i,j;

    for (i=Y1 ; i<=Y2 ; i++)
      for (j=X1 ; j<=X2 ; j++)
        PutP(j,i,0x70);
}

void SetNormal(word X1,word Y1,word X2,word Y2)
{
    word i,j;

    for (i=Y1 ; i<=Y2 ; i++)
      for (j=X1 ; j<=X2 ; j++)
        PutP(j,i,0x07);
}

word GetMes(word X,word Y,word Width,byte *Data,word DataWidth)
{
    word i,j;
    word hx,x;
    byte Buf[256];

    hx=x=0;
    MemSetB(Buf,' ',255);
    Buf[255]=0;
    do {
       for (i=0 ; i<Width ; i++)
         PutCP(X+i,Y,*(Buf+hx+i),0x70);
       gotoxy(X+x+1,Y+1);
       GetKey();
       switch(Way) {
         case Left :
         case CtrlS: if (x==0)
                        if (hx==0) ;
                        else hx--;
                     else x--;
                     break;
         case Right:
         case CtrlD: if (x==Width-1)
                        if (hx+x<254) hx++;
                        else ;
                     else x++;
                     break;
         case BackSpace:
                     if (hx+x==0) break;
                     memmove(Buf+hx+x-1,Buf+hx+x,255-hx-x);
                     Buf[254]=' ';
                     if (x==0) hx--;
                     else x--;
                     break;
         case CtrlG: memmove(Buf+hx+x,Buf+hx+x+1,254-hx-x);
                     Buf[254]=' ';
                     break;
         case ESC  : return Error;
         default   : if (Held(Way,'a','z')) Way-='a'-'A';
                     if (Held(Way,32,95)) {
                        if (hx+x==254) Buf[254]=Way;
                        else {
                          memmove(Buf+hx+x+1,Buf+hx+x,254-hx-x);
                          Buf[hx+x]=Way;
                          if (x==Width-1) hx++;
                          else x++;
                        }
                     }
       }
    } while (Way!=Return);
    strncpy(Data,Buf,DataWidth);
    Data[DataWidth]=0;
    return OK;
}

byte Name[81];

#define Menus 7

word CrtDrvLoad(void)
{
    int i;
    char *is;


    puts("Please select CRT DRIVER fit for your video card.\n");
    puts("HERC640.DRV for Hercules 80*25");
    puts("HERC720.DRV for Hercules 90*21");
    puts("EGA640.DRV  for EGA      80*22");
    puts("VGA640.DRV  for VGA      80*25");
    puts("CGA640.DRV  for CGA      80*25");
    puts("User Driver");
    puts("Quit");
    i=0;
    gotoxy(50,3);
    do {
      SetInverse(0,i+4,29,i+4);
      GetKey();
      SetNormal(0,i+4,29,i+4);
      switch (Way) {
        case Up    :
        case CtrlE : if (i==0) i=Menus-1;
                     else i--;
                     break;
        case Down  :
        case CtrlX : if (i==Menus-1) i=0;
                     else i++;
                     break;
        case Home  :
        case CtrlR : i=0; break;
        case End   :
        case CtrlC : i=Menus-1; break;
        case ESC   : i=Menus-1;
                     Way=Return;
                     break;
      }
    } while (Way!=Return);
    switch (i) {
      case 0 : is="HERC640.DRV"; break;
      case 1 : is="HERC720.DRV"; break;
      case 2 : is="EGA640.DRV";  break;
      case 3 : is="VGA640.DRV";  break;
      case 4 : is="CGA640.DRV";  break;
      case 5 : gotoxy(1,13); puts("Enter Driver Name :");
               if (GetMes(20,12,20,Name,80)==OK) {
                  is=(char*)Name;
                  break;
               }
      case 6 : clrscr();
               puts(SetVersion);
               exit(0);
    }
    strcpy(Name,is);
    if (OpenHanDrv(Name)!=OK) {
      clrscr();
      puts(SetVersion);
      puts("\nError In Loading CRT Driver.");
      exit(0);
    }
    return OK;
}

byte CopyBuf[10240];

word MakeSANVIDEODRV(void)
{
     word ib;
     word len;
     int  handler;

     if (access("SANVIDEO.DRV",0)==0) ib=False;
     else ib=True;
     if (HGetYesNo(" »¡¸÷Ða¯¥ —aœa·¡¤áŸi ¯©Ð— —aœa·¡¤á¡ —w¢ ÐiŒa¶a? ",&ib)==OK)
       if (ib==True) {
           HSayStatus("'SANVIDEO.DRV' Ÿi  e—i‰¡ ·¶¯s“¡”a.");
           if ((handler=_open(Name,0))==-1) return Error;
           if (_read(handler,CopyBuf,len=filelength(handler))==-1) return Error;
           _close(handler);
           if ((handler=_creat("SANVIDEO.DRV",FA_ARCH))==-1) return Error;
           if (_write(handler,CopyBuf,len)==-1) return Error;
           _close(handler);
           HCloseWindow();
           HSayMes("ÑÁ¡e Â‰b —aœa·¡¤á·¥ 'SANVIDEO.DRV' ˆa ¬¡  e—i´á ¹v¯s“¡”a.");
       }
     return OK;
}

        void PaperLimit (void)
        {
               word OKF;
               int  i;
               char One[10],Two[10],Thr[10];
               char *PaperLimit[]={
                    "¸b´ó  a”w·¡ ÀáŸ¡ Ði® ·¶“e ¢…¬á· ÂA” º‰®Ÿi ¸÷Ðs“¡”a.",
                    "¸b´ó  a”w·¡ ÀáŸ¡ Ði® ·¶“e ¢…¬á· ÂA” º‰®Ÿi ¸÷Ðs“¡”a.",
                    "¸b´ó  a”w·¡ ÀáŸ¡ Ði® ·¶“e ¢…¬á· ÂA” º‰®Ÿi ¸÷Ðs“¡”a."
                    };

               ExplainList=PaperLimit;
               HOpenWindow(17,16,43,20);
               HOpenMenu();
               HSetItem(18,17," ¸b´ó  a”w 1  :",13,'');
               HSetItem(18,18," ¸b´ó  a”w 2  :",13,'');
               HSetItem(18,19," ¸b´ó  a”w 3  :",13,'');
               do {
                  sprintf(One,"%4u",MaxPaper1);DrawString(34,17,One);
                  sprintf(Two,"%4u",MaxPaper2);DrawString(34,18,Two);
                  sprintf(Thr,"%4u",MaxPaper3);DrawString(34,19,Thr);
                  OKF=HSelect();
                  if (OKF!=OK || HotKeyF) break;
                  if (HGetMes("¸b´óÐi ¢…¬á· ÂA” Ça‹¡Ÿi °áº­A¶a.",One,5)==OK) {
                     i=atoi(One);
                     if (i<50) HSayMes(" 50 ¥¡”a“e Äá´¡ ‰V“e•A¶a.");
                     else if (i>16384) HSayMes(" ·a´a! á¢Ça‘A¶a. 16000 ·¡Ða¡ ¸s´a º­A¶a.");
                     else
                        switch (Item) {
                          case 0 : MaxPaper1=i; break;
                          case 1 : MaxPaper2=i; break;
                          case 2 : MaxPaper3=i; break;
                        }
                  }
               } while (Way!=ESC);
               HCloseMenu();
               HCloseWindow();
        }


        void  SelectKeyboard(void)
        {
              word i;
              word OKF;
              char *KeyboardList[]={
                   "¥¡É· ¬a¶wÐa‰¡ ‰¯¥ ¶E­¥ ¸s·¡¶w Ðe‹i ¸aÌe·¡ –¤é¯¢ ·³“¡”a.",
                   "Àá·q,ˆa¶…•A,{ ­¡Ÿ¡¶w ‹i®Aˆa ¦…Ÿ¡–A´á ·¶“e Ðe‹i ¸aÌe·¡ ­A¤é¯¢ ·³“¡”a."
                   };

              ExplainList=KeyboardList;
              HOpenWindow(17,6,28,9);
              HOpenMenu();
              HSetItem(18,7," D –¤é¯¢",10,'D');
              HSetItem(18,8," S ­A¤é¯¢",10,'S');
              i=(KeyboardType==2)?0:1;
              HManualSelect(i);
              OKF=HSelect();
              if (OKF==OK) HotKeyEndF=True;
              HCloseMenu();
              HCloseWindow();
              if (OKF==OK) KeyboardType=(Item==0)?2:3;
        }

        void   BlockOption(void)
        {
               word OKF;
               int  i;
               char Buf[10];
               char Left[10],Right[10],Indent[10],SpaceLine[10];
               char *BlockOptionList[]={
                    "¶E½¢ ‹¡º…µi·i ¸÷Ðs“¡”a.",
                    "µ¡Ÿe½¢ ‹¡º…µi·i ¸÷Ðs“¡”a.",
                    "¢…”e Ñw¯¢ ¸÷i¯¡ ¢…”e· Àõº‰· —iµa³a‹¡ ¢…¸a®Ÿi ¸÷Ðs“¡”a.",
                    "¢…”e Ñw¯¢ ¸÷i¯¡ º‰‰Á º‰¬a·¡· §¥º‰ ˆ•® ·³“¡”a.",
                    "¢…”e Ñw¯¢ ¸÷i· ”e¶áŸi ¸÷Ðs“¡”a."
                    };

               ExplainList=BlockOptionList;
               Push(InverseF);
               HInverse();
               HOpenWindow(17,8,49,14);
               HOpenMenu();
               HSetItem(18, 9," L §iœâ ¶E½¢   ¶áÃ¡ :",20,'L');
               HSetItem(18,10," R §iœâ µ¡Ÿe½¢ ¶áÃ¡ :",20,'R');
               HSetItem(18,11," I —iµa ³a‹¡   ¸a® :",20,'I');
               HSetItem(18,12," S º‰‰Á º‰¬a·¡ §¥º‰ :",20,'S');
               HSetItem(18,13," T ¸÷i ¤w¯¢        :",20,'T');
               Pop(InverseF);
               do {
                  HInverse();
                  switch (BlockSortType) {
                    case 0 : DrawString(40,13,"¢…¸a”e¶á");
                             break;
                    case 1 : DrawString(40,13,"´á¸é”e¶á");
                  }
                  sprintf(Left,"%u",BlockLeft);
                  strcpy(Buf,Left); strcat(Buf,"   ");
                  DrawString(40, 9,Buf);
                  sprintf(Right,"%u",BlockRight);
                  strcpy(Buf,Right); strcat(Buf,"   ");
                  DrawString(40,10,Buf);
                  sprintf(Indent,"%d",(int)BlockIndent);
                  strcpy(Buf,Indent); strcat(Buf,"   ");
                  DrawString(40,11,Buf);
                  sprintf(SpaceLine,"%u",BlockSpace);
                  strcpy(Buf,SpaceLine); strcat(SpaceLine,"   ");
                  DrawString(40,12,Buf);
                  OKF=HSelect();
                  HNormal();
                  if (OKF!=OK || HotKeyF) break;
                  switch (Item) {
                    case 0 :if (HGetEng("¶E½¢ ¶áÃ¡Ÿi °áº­A¶a.",Left,5)==OK) {
                                i=atoi(Left);
                                if (i<1) HSayMes(" 1 ¥¡”a“e Äá´¡ ‰V“e•A¶a.");
                                else if (i>BlockRight-5) HSayMes(" µ¡Ÿe½¢ {‰Á §¡ŠaÐ á¢ Çe®·³“¡”a.");
                                else if (BlockRight-i<BlockIndent+5) HSayMes("—iµa³a‹¡ ®µA §¡Ð¬á Çe®·³“¡”a.");
                                else BlockLeft=i;
                            }
                            break;
                    case 1 :if (HGetEng("µ¡Ÿe½¢ ¶áÃ¡Ÿi °áº­A¶a.",Right,5)==OK) {
                                i=atoi(Right);
                                if (i>254) HSayMes(" 254 ¥¡”a“e ¸b´a´¡ Ðs“¡”a.");
                                else if (i<BlockLeft+5) HSayMes(" ¶E½¢{‰Á §¡ŠaÐ á¢ Çe®·³“¡”a.");
                                else if (i-BlockLeft<BlockIndent+5) HSayMes("—iµa³a‹¡ ®µA §¡Ð¬á Çe®·³“¡”a.");
                                else BlockRight=i;
                            }
                            break;
                    case 2 :if (HGetEng("¡yÄe·¡a —iµa ³iŒa¶a?",Left,5)==OK) {
                                i=atoi(Left);
                                if ((int)i>(int)(BlockRight-BlockLeft-5)) HSayMes(" ´·½¢{‰Á §¡ŠaÐ á¢ Çe® ·³“¡”a.");
                                else BlockIndent=i;
                            }
                            break;
                    case 3 :if (HGetEng("¡y º‰·¡a ›¶‰Œa¶a?",SpaceLine,5)==OK) {
                                i=atoi(SpaceLine);
                                if (i>10) HSayMes(" ¹± Ça”a‰¡ ¬—ˆb»¡ ´g·a¯³“¡Œa? 10¥¡”e ¸b´v·a¡e ¹½‰V“e•A¶a.");
                                else BlockSpace=i;
                            }
                            break;
                    case 4 :BlockSortType=(BlockSortType==0)?1:0;
                  }
               } while (!HotKeyF);
               if (Way!=ESC) HotKeyEndF=True;
               HCloseMenu();
               HCloseWindow();
        }
        void SetEngFont(void)
        {
            byte Name[80];
            char _drv[MAXDRIVE],_dir[MAXDIR],_name[MAXFILE],_ext[MAXEXT];

            strcpy(Name,"*.ENG");
            if (HGetEng("¬a¶wÐa¯© µw¢… ¸aÑw· ·¡Ÿq·i °áº­A¶a.",Name,79)!=OK) return ;
            if (HGetFileName(Name)!=OK) return;
            fnsplit(Name,_drv,_dir,_name,_ext);
            strcpy(Name,_name);
            strcat(Name,_ext);
            if (access(Name,0)==-1) {
                HSayMes("¬a¶wÐa¯© ¸aÑw·e '¬e'·¡ —i´á·¶“e —¡BÉ¡Ÿ¡µA ·¶´á´¡ Ðs“¡”a.");
                return;
            }
            if (SetEng(Name)!=OK) return ;
            strcpy(EngFontName,_name);
            strcat(EngFontName,_ext);
        }

        void SetHanFont(void)
        {
            byte Name[80];
            char _drv[MAXDRIVE],_dir[MAXDIR],_name[MAXFILE],_ext[MAXEXT];

            strcpy(Name,"*.KOR");
            if (HGetEng("¬a¶wÐa¯© Ðe‹i ¸aÑw· ·¡Ÿq·i °áº­A¶a.",Name,79)!=OK) return;
            if (HGetFileName(Name)!=OK) return;
            fnsplit(Name,_drv,_dir,_name,_ext);
            strcpy(Name,_name);
            strcat(Name,_ext);
            if (access(Name,0)==-1) {
                HSayMes("¬a¶wÐa¯© ¸aÑw·e '¬e'·¡ —i´á·¶“e —¡BÉ¡Ÿ¡µA ·¶´á´¡ Ðs“¡”a.");
                return;
            }
            if (SetHan(Name)!=OK) return;
            strcpy(HanFontName,_name);
            strcat(HanFontName,_ext);
        }

        void   PrinterOption(void)
        {
               word OKF;
               int  i;
               char Left[10];
               char Port[10];
               char *PrinterOptionList[]={
                    "ˆb º‰· ·¥­á ¯¡¸b µi·i ¸÷Ðs“¡”a.",
                    "¬a¶wÐa‰¡ ‰¯¥ ÏaŸ¥ÈáµA ´i x‰A  xÂ¡ º­A¶a.",
                    "ÏaŸ¥ÈáµÁ µe‰i–A´á ·¶“e ¥wiÍ¡Ëa· ¤åÑ¡Ÿi °áº­A¶a."
                    };

               ExplainList=PrinterOptionList;
               Push(InverseF);
               HInverse();
               HOpenWindow(17,9,49,13);
               HOpenMenu();
               HSetItem(18,10," L ¶E½¢ ¶áÃ¡ ",13,'L');
               HSetItem(18,11," R ·¥­á ¤w¯¢ ",13,'R');
               HSetItem(18,12," P ¥wi Í¡Ëa ",13,'P');
               Pop(InverseF);
               do {
                  HInverse();
                  switch (PrintingType) {
                    case 0 : DrawString(32,11,"¡¡—I(KSSM)"); break;
                    case 1 : DrawString(32,11,"¡¡—I(KS)  ");   break;
                    case 2 : DrawString(32,11,"EPSON LQ's"); break;
                  }
                  sprintf(Left,"%u",LeftMargin);
                  HClrBox(32,12,35,10);
                  DrawString(32,10,Left);
                  sprintf(Port,"%u",PrinterPort);
                  DrawString(32,12,Port);
                  OKF=HSelect();
                  HNormal();
                  if (OKF!=OK || HotKeyF) break;
                  switch (Item) {
                    case 0 :if (HGetEng("ˆt·i °áº­A¶a.",Left,5)==OK) {
                                i=atoi(Left);
                                if (i<1) HSayMes(" 0 ¥¡”a“e Äá´¡ ‰V“e•A¶a.");
                                else if (i>200) HSayMes(" á¢ Çe ®Ã¡ ´a“³“¡Œa? ”a¯¡ °áº­A¶a.");
                                else LeftMargin=i;
                            }
                            break;
                    case 1 :PrintingType++;
                            if (PrintingType>2) PrintingType=0;
                            break;
                    case 2:if (PrinterPort==1) PrinterPort=2;
                           else PrinterPort=1;
                           break;
                  }
               } while (!HotKeyF);
               if (Way!=ESC) HotKeyEndF=True;
               HCloseMenu();
               HCloseWindow();
        }


        /*void InverseScreen(void)
        {
             AuxInverseF=!AuxInverseF;
             Way=AltO;
             SetHotKeyF();
             Push(InverseF);
             InverseF=AuxInverseF;
             Pop(InverseF);
        }*/

        void SetFileExt(void)
        {
             byte Buf[80];

             strcpy(Buf,StdExt);
             if (HGetEng("Á¡‹¡Ã¡¡ ³a·© ÑÁ·© ÑÂ¸w¸aŸi °áº­A¶a.",Buf,79)==OK) {
                strcpy(StdExt,Buf);
                strupr(StdExt);
             }
        }

void OptionMenu(void)
{
    word OKF;
    word static OptionN;
    word ExitF=False;
    char Buf[81];
    char *OptionList[]={
         "ÑÁ¡eµA Â‰bÐa“e Ðe‹i ¸aÑw·i ¥e‰wÐs“¡”a.",
         "ÑÁ¡eµA Â‰bÐa“e ´iÌa¥U ¸aÑw·i ¥e‰wÐs“¡”a.",
         "–¤é¯¢‰Á ­A¤é¯¢º— Ðe‹i ¸aÌe·i ¥e‰wÐs“¡”a.",
         "—¡¯aÇaa ÏaŸ¥Èá¡· ·³Â‰b¯¡ ¬a¶wÐi Ðe‹i Å¡—aŸi ¬åÈ‚ Ðs“¡”a.",
         "§iœâ ¸÷i ¯¡µA ¬a¶w–A“e ‹¡º… ˆt·i ¥e‰wÐs“¡”a.",
         "¢…¬á ·¥­áµA ‰Åe–E ¶w·i ¥e‰wÐs“¡”a.",
         "‹¡¥¥ˆt·a¡ ¬a¶w–A“e ÑÁ·© ·¢¯aÉE­e·i ¥e‰wÐs“¡”a.",
         "¬e· ‹¡•·¯¡ ˆa¸w ÂA‹eµA ¸b´óÐa”å ¢…¬áŸi ¸a•··a¡ ·ª´á —i·©»¡ ¸÷Ðs“¡”a.",
         "¢…¬á ¸á¸w¯¡ µ¡œ–E ÑÁ·©·i *.BAK ·a¡ ¥¡¹¥Ði»¡Ÿi ‰i¸÷Ðs“¡”a.",
         "•¡¯a ·¡¶w¯¡µA ÑÁ·© ¸á¸wµa¦Ÿi ¢‰·i»¡ ‰i¸÷Ðs“¡”a.",
         "ÑÁ¡e ˆ—¯¥º—µA ‹i®A(Ç¡) ·³b·¡ ·¶”a¡e ·³bµA »bˆb ”ÀáÐi»¡ ¸÷Ðs“¡”a.",
         "¸b´ó ˆa“wÐe ÂA” ¢…¬á Ça‹¡Ÿi ¸÷Ðs“¡”a.",
         "¬åÈ‚,¯©Ð—,·³b ÀaŸ±Ìe‰Á ‰Åe–E ¶w·i ¸á¸wÐaµa ¬e· ‹¡•·¯¡µA ·ª´á —aŸ³“¡”a.",
         "¤aÈw Œiˆ ¸b´ó·i  aÃ³“¡”a."
    };

    HInverse();
    HOpenWindow(16,1,36,20);
    HOpenMenu();
       HSetItem(17, 2," H Ðe‹i ‹i©",13,'H');
       HSetItem(17, 3," F µw¢… ‹i©",13,'F');
       HDrawBar(17,35,4);
       HSetItem(17, 5," K Ðe‹i ¸aÌe",13,'K');
       HSetItem(17, 6," C Ðe‹i Å¡—a",13,'C');
       HSetItem(17, 7," B §iœâ ¸÷i",13,'B');
       HSetItem(17, 8," I ·¥­á ‹¡“w",13,'I');
       HSetItem(17, 9," E ¢…¬á ¡Ÿ¡",13,'E');
       HDrawBar(17,35,10);
       HSetItem(17,11," P ¯¡¸b º…§¡",13,'P');
       HSetItem(17,12," M ¢…¬á ¥¡Ðñ",13,'M');
       HSetItem(17,13," A ·©”e ¸á¸w",13,'A');
       HSetItem(17,14," Q ¯¥­¢ ˆ—¯¥",13,'Q');
       HDrawBar(17,35,15);
       HSetItem(17,16," L  a”w Ça‹¡",13,'L');
       HDrawBar(17,35,17);
       HSetItem(17,18," S ÑÅ‰w ¸á¸w",13,'S');
       HSetItem(17,19,"   ¸b´ó  aÃ±",13,'');
    HManualSelect(OptionN);

    do {
      HNormal();
      strcpy(Buf,"'¬e' ¤aÈwŒiˆ ");
      strcat(Buf,SetVersionNum);
      strcat(Buf,"  ¸á¸bŠ¥ (¸á) 1991,1992 ¤b‹AÑe      ");
      HWriteMid(0,Buf);
      HInverse();
      if (HanGulCodeType==1) DrawString(31,6,"µÅ¬÷");
      else                   DrawString(31,6,"¹¡Ðs");
      if (PrepareF) DrawString(31,11,"Åq");
      else          DrawString(31,11,"q");
      if (BackUpF)  DrawString(31,12,"Åq");
      else          DrawString(31,12,"q");
      if (OsSaveF)  DrawString(31,13,"Åq");
      else          DrawString(31,13,"q");
      if (SmartRefleshF) DrawString(31,14,"Åq");
      else               DrawString(31,14,"q");
      ExplainList=OptionList;
      OKF=HSelect();
      OptionN=Item;
      if (OKF==OK)
        switch (OptionN) {
          case 0 :SetHanFont();          break;
          case 1 :SetEngFont();          break;
          case 2 :SelectKeyboard();      break;
          case 3 :HanGulCodeType=!HanGulCodeType; break;
          case 4 :BlockOption();         break;
          case 5 :PrinterOption();       break;
          case 6 :SetFileExt();          break;
          case 7 :PrepareF=!PrepareF;    break;
          case 8 :BackUpF =!BackUpF;     break;
          case 9 :OsSaveF =!OsSaveF;     break;
          case 10:SmartRefleshF=!SmartRefleshF; break;
          case 11:PaperLimit();          break;
          case 12:if (SaveOptions()!=OK) HSayIOErr();
                  else {
                     HSayMes(" ÑÅ‰w ¸á¸w ÑÁ·© 'SAN.CNF' ˆa ¬¡  e—i´á ¹v¯s“¡”a.");
                     HotKeyEndF=True;
                  }
                  break;
          case 13:OKF=True;
                  if (HGetYesNo("¸b´ó·i {¯¡‰V ¯s“¡Œa?",&OKF)==OK)
                    if (OKF) ExitF=True;
                  break;
        }
    } while (!ExitF);
    HCloseWindow();
    HCloseMenu();
}

void main(void)
{
       int handler;
       word i,j;
       char Drive[MAXDRIVE],Dir[MAXDIR],Name[MAXFILE];

       clrscr();
       InitCrt();
       puts(SetVersion);
       CrtDrvLoad();
       clrscr();
       puts(SetVersion);
       if (InitWindow()!=OK || InitPopUp()!=OK) {
         puts("Not Enough Memeory.");
         exit(0);
       }

       if (SetExtEng("san.lin")!=OK) {
         puts("Error In Reading 'SAN.LIN'.");
         exit(0);
       }
       if (SetEng("san.eng")!=OK) {
         puts("Error In Reading 'SAN.ENG'.");
         exit(0);
       }
       if (SetHan("san.kor")!=OK) {
         puts("Error In Reading 'SAN.KOR'.");
         exit(0);
       }
       GrMode();
       HClrScr();
       if (MakeSANVIDEODRV()!=OK) {
          HSayMes(" —aœa·¡¤á —w¢µA ¢…¹Aˆa ¬—‰v¯s“¡”a.");
       }
       RetrieveOptions();
       OptionMenu();
       TextMode();
       clrscr();
       puts(SetVersion);
}
