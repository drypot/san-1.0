/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                  Environmemt & Menu                  */
/*                                                      */

#include "stddef.h"
#include "handrv.h"
#include "memory.h"
#include "const.h"
#include "edit.h"
#include "hwindow.h"
#include "crt.h"
#include <dir.h>
#include <string.h>
#include <alloc.h>
#include <dos.h>

void  SetEditScn(void)
{
    char Path[80],Drv[3],Dir[80-(3+9+5)],Name[9],Ext[5];
    word i,j,k;



    DrawString  (0,VideoBot,"    Ð— ");
    DrawString  (11,VideoBot,"/");
    DrawString  (16,VideoBot,"   µi ");
    if (InsertF) DrawString(25,VideoBot,"   ¬s·³ ");
    else         DrawString(25,VideoBot,"        ");
    if (IndentF) DrawString(33,VideoBot,"  —i·± ");
    else         DrawString(33,VideoBot,"       ");
    if (HanF)    DrawString(40,VideoBot,"  Ðe‹i ");
    else         DrawString(40,VideoBot,"  µw¢… ");
    if (ProtectLineF)
                 DrawString(47,VideoBot,"  ¬å¥¡Ñ¡   ");
    else if (CutLineF)
                 DrawString(47,VideoBot,"  ‹©·¡¹AÐe ");
    else         DrawString(47,VideoBot,"           ");
    if (EditedF==True) DrawString(58,VideoBot,"  * ");
    else               DrawString(58,VideoBot,"    ");
    fnsplit(WorkFileName,Drv,Dir,Name,Ext);
    fnmerge(Path        ,Drv,"",Name,Ext);
    strcat(Path,"               ");
    Path[15]=0;
    DrawString(62,VideoBot,Path);
    DrawString(77,VideoBot,(NowTask==0)?"  1":(NowTask==1)?"  2":"  3");
    for (i=80; i<=VideoRight; i++) DrawEng(i,VideoBot,' ');
}

void SetMenuScn(void)
{
    word i,j,k;

    for (i=60; i<=VideoRight; i++) DrawEng(i,0,' ');
    DrawString(0,0,"  F)¢…¬á  C)¯©Ð—  O)¬åÈ‚  Y) a”w  B)§iœâ  I)¬s·³  E)·³b  H)Àx‹¡  P)Ëb®  ¬e..");
    Push(InverseF);
    HInverse();
    for (i=0 ; i <=VideoRight ; i++ ) {
      DrawEng(i,1,137);
      DrawEng(i,VideoBot-1,138);
    }
    Pop(InverseF);
/*    for (i=0 ; i<VideoRight ; i+=2) {
      DrawHan(i,1,'Û¬');
      DrawHan(i,VideoBot-1, 'Û¬');
    } */
}

void EditIns(void)
{
    InsertF=!InsertF;
    SetEditScn();
}

void EditCQL(void)
{
    MemCopy(UndoBuf ,EditBuf ,256);
    MemCopy(UndoType,EditType,256);
    if (BlockType==0) {
        if (BlockSY==NowLine) {
            if (EditType[BlockSX]==2) BlockSX--;
        }
        if (BlockEY==NowLine) {
            if (EditType[BlockEX]==2) BlockEX--;
        }
    }
}

void EditCQI(void)
{
    IndentF=!IndentF;
    SetEditScn();
}

void EditCOL(void)
{
    ProtectLineF=!ProtectLineF;
    if (ProtectLineF==OK && CutLineF==OK) CutLineF=False;

}

void EditCOK(void)
{
    CutLineF=!CutLineF;
    if (CutLineF==OK && ProtectLineF==OK) CutLineF=False;
    SetEditScn();
}

void Bell1();
void Bell2();

void ChangeLanguage(void)
{
    HanF=!HanF;
    if (HanF) {
      sound (500);
      delay (5);
      nosound();
    } else {
     sound (200);
     delay (5);
     nosound();
    }
    SetEditScn();
}

byte __pb[10];

void PrintXYpos(void)
{
    sprintf (__pb,"%4d",NowLine);  DrawString(7 ,VideoBot,__pb);
    sprintf (__pb,"%4d",PaperLen); DrawString(12,VideoBot,__pb);
    sprintf (__pb,"%3d",HX+SX-1);  DrawString(22,VideoBot,__pb);
}

byte *HelpData[200];
word HelpDataLen;

word SetHelpData(void)
{
    FILE *HelpFile;
    byte LineBuf[90];
    word i,j;

    strcpy(LineBuf,SysPath);
    strcat(LineBuf,"san.hlp");
    if ((HelpFile=fopen (LineBuf,"rt"))==NULL) return Error;
    else {
       setvbuf (HelpFile,DiskBuf,_IOFBF,DiskBufSize);
       while (True) {
          if (HelpDataLen==200) break;
          LineBuf[1]=0;
          if (fgets((byte *)LineBuf,81,HelpFile)==NULL) {
              if (ferror(HelpFile)) return Error;
              if (LineBuf[0]!=0) {
                  if ((HelpData[HelpDataLen]=farmalloc(strlen(LineBuf)+1))==NULL) return Error;
                  strcpy(HelpData[HelpDataLen],LineBuf);
              } else HelpDataLen--;
              break;
          } else {
              if (LineBuf[strlen(LineBuf)-1]=='\n')
                  LineBuf[strlen(LineBuf)-1]=0;
              if ((HelpData[HelpDataLen]=farmalloc(strlen(LineBuf)+1))==NULL) return Error;
              strcpy(HelpData[HelpDataLen],LineBuf);
          }
          if (farcoreleft()<=MaxBufRest*3) return Error;
          HelpDataLen++;
       }
       fclose (HelpFile);
    }
    return OK;
}

void SetHelpScn(word StartLine)
{
     word i;

     for (i=2 ; i<=VideoBot-2 ; i++ ) {
       if (StartLine<HelpDataLen) {
         DrawString(0,i,HelpData[StartLine]);
         HClrBox(strlen(HelpData[StartLine]),i,VideoRight,i);
       } else
         HClrBox(0,i,VideoRight,i);
       StartLine++;
     }
     NextChar=FirstAvailable;
}

void EditHelp(void)
{
     word i,j;
     word static Top=0;

     HClrBox(0,VideoBot,VideoRight,VideoBot);
     Explain("•¡¶‘ i");
     do {
       SetHelpScn(Top);
       GetKey();
       SetHotKeyF();
       if (HotKeyF) break;
       switch (Way) {
         case Up    :
         case CtrlW :
         case CtrlE : if (Top!=0) Top--;  break;
         case Down  :
         case CtrlZ :
         case CtrlX : if (Top!=HelpDataLen-1) Top++; break;
         case PgUp  :
         case CtrlR : Top-=VideoBot-3;
                      if (Top>HelpDataLen) Top=0;
                      break;
         case PgDn  :
         case CtrlC : Top+=VideoBot-3;
                      if (Top>=HelpDataLen) Top=HelpDataLen-1;
                      break;
       }
     } while (Way!=ESC  && Way!=Return);
     SetEditScn();
     PrintXYpos();
     SetScnF=True;
}
