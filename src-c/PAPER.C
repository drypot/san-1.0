/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                  Paper Management                    */
/*                                                      */

#include "stddef.h"
#include "memory.h"
#include "const.h"
#include "edit.h"
#include "handrv.h"
#include "hwindow.h"
#include "hanja.h"
#include "sanver.h"
#include <io.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>
#include <alloc.h>
#include <mem.h>
#include <string.h>

byte   far * far *Paper;
byte   far * far *UnKillBuf;
byte   EditBuf  [256],
       EditType [256],
       UndoBuf  [256],
       UndoType [256],
       AuxBuf   [256],
       AuxType  [256];
word   NowLine,
       PaperLen;

void SayBufStatus(void)
{
    byte Buf[50];

    Push(InverseF);
    HInverse();
    HOpenWindow (3,7,35,15);
    DrawString(5,8,"¸b´óµwµb ¬wÈ");
    sprintf (Buf," ÂA” Ð—®          :%8u" ,MaxPaper);  DrawString(4,10,Buf);
    sprintf (Buf," Ñe¸ Ð—®          :%8u" ,PaperLen);  DrawString(4,11,Buf);
    sprintf (Buf," Ñe¸ Ð—            :%8u" ,NowLine);   DrawString(4,12,Buf);
    sprintf (Buf," ÂA” ¸b´óµwµb Ça‹¡ :%8lu",MaxBufSize);DrawString(4,13,Buf);
    sprintf (Buf," Ñe¸ q·e µwµb Ça‹¡:%8lu",farcoreleft()); DrawString(4,14,Buf);
    Pop(InverseF);
    HWait (Message);
    HCloseWindow();
}

void ResizeBuf(void)
{
    word i;
    byte Buf[10];

    Push(InverseF);
    HInverse();
    HOpenWindow(3,3,31,5);
    DrawString(5,4,"Ñe¸ ¸÷Ÿ¡º—·¥ Ð—·e...");
    for (i=1 ; i<=PaperLen ; i++) {
       if (i%3==0) {
           sprintf (Buf,"%5d",i);
           DrawString(26,4,Buf);
       }
       MemCopy(Paper[i],AuxBuf,Paper[i][0]+2);
       farfree(Paper[i]);
       Paper[i]=(byte far *)farmalloc(AuxBuf[0]+2);
       MemCopy(AuxBuf,Paper[i],AuxBuf[0]+2);
    }
    Pop(InverseF);
    HCloseWindow();
    SayBufStatus();
}

word BufErr(void)
{
    static ErrCount;

    HSayMes("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. Âˆa–E •A·¡Èá“e ÀáŸ¡–I ® ´ô¯s“¡”a");
    ErrCount++;
    if (farcoreleft()<MaxBufRest/2 || ErrCount>5) {

        word i,j;
        void interrupt extern (*CtrlBrkBuf)();

        for (i=ScnTop-1 ; i<=ScnBot ; i++) HClrBox(ScnLeft-1,i,ScnRight,i);
        HWriteMid(MidY-4 ,"¬e· ÀáŸ¡ “wb·i Á¡‰ÁÐ¬á ¬a¶wÐa¯¡‰¡ ‰¯³“¡”a.");
        HWriteMid(MidY-2 ,"”á ·¡¬w· ÀáŸ¡“e ¦‰ˆa“w Ða£a¡ Ñe¸Œa»¡ ¸b´óÐa¯¥ ¶w·i");
        HWriteMid(MidY   ,"¸á¸wÐ ‘½·a¯© ® ·¶•¡¢ Ð—aŸ¡‰V¯s“¡”a.");
        HWriteMid(MidY+2 ,"¢…¬á ¸á¸w·¡ {eÒ '¬e'· ¸b´ó·e ¹·ža –S“¡”a.");
        HSayBottom("´a¢ ‹i®Aa ’‰œá º­A¶a.");

        for (i=0; i<3 ; i++) {
             ChangeTasks(i);
             if (EditedF==False) continue;
             SetScn();
             j=Yes;
             HGetYesNo("Ñe¸ ¸b´ó  a”w· ¢…¬áŸi ¸á¸w Ða¯¡‰V¯s“¡Œa?",&j);
             if (j) SaveFile();
        }

        if (HanjaFileHandler!=-1) _close(HanjaFileHandler);
        ClosePickList();
        TextMode();
        clrscr();
        puts(SanVersion);
        setvect(0x1B,CtrlBrkBuf);
        exit(0);
    }
    return OK;
}

word CheckBuf(void)
{
    if (farcoreleft()<MaxBufRest) return BufErr();
    return OK;
}

word ResizePaper (word ln)
{
    if (CheckBuf()!=OK) return Error;
    Paper[ln][0]=(byte) strlen((char far *)Paper[ln]+1);
    Paper[ln]=(byte far *)farrealloc (Paper[ln],Paper[ln][0]+2);
    return OK;
}

word PutPaper (word ln,byte *data)
{
    byte size;

    size=strlen(data);
    if (CheckBuf()!=OK) return Error;
    if (Paper[ln]!=NULL) farfree(Paper[ln]);
    Paper[ln]=(byte far *)farmalloc(size+2);
    Paper[ln][0]=size;
    strcpy ((char far *)Paper[ln]+1,(char far *)data);
    return OK;
}

void SetEditBuf(void)
{
    word i,size;

    EditBuf[256-1]=0;
    EditBuf[0]=256-2;
    size=Paper[NowLine][0];
    MemCopy(Paper[NowLine]+1,EditBuf+1,size);
    for (i=1 ; i<=size ; )
       if (EditBuf[i]>127) (EditType[i++]=1),(EditType[i++]=2);
       else EditType[i++]=0;
    MemSetB (EditBuf+size+1,' ',256-2-size);
    MemSetB(EditType+size+1,0  ,256-2-size);
    MemCopy(EditBuf,UndoBuf,256);
    MemCopy(EditType,UndoType,256);
}

void SetAuxBuf(word Line)
{
    word i,j;

    MemSetB (AuxBuf+1,' ',256-2);
    AuxBuf[256-1]=0;
    AuxBuf[0]=256-2;
    MemCopy(Paper[Line]+1,AuxBuf+1,Paper[Line][0]);
    MemSetB(AuxType+1,0,256-2);
    j=Paper[Line][0];
    for (i=1 ; i<=j ; )
       if (AuxBuf[i]>127) (AuxType[i++]=1),(AuxType[i++]=2);
       else AuxType[i++]=0;
}

void SetUndoBuf(byte *Data)
{
    word i,size;

    UndoBuf[256-1]=0;
    UndoBuf[0]=256-2;
    size=strlen(Data);
    MemCopy(Data,UndoBuf+1,size);
    for (i=1 ; i<=size ; )
       if (UndoBuf[i]>127) (UndoType[i++]=1),(UndoType[i++]=2);
       else UndoType[i++]=0;
    MemSetB(UndoBuf+size+1,' ',256-2-size);
    MemSetB(UndoType+size+1,0  ,256-2-size);
}


word PutEditBuf(void)
{
    word i=254;

    while (EditBuf[i]==' ' &&  i>0) i--;
    EditBuf[i+1]='\0';
    if (PutPaper (NowLine,EditBuf+1)!=OK) {
      EditBuf[i+1]=' ';
      return Error;
    }
    EditBuf[i+1]=' ';
    return OK;
}

void DeleteLine (word ln)
{
    if (ln==NowLine) {
       if (PutEditBuf()!=OK) return;
       SaveKillLine(Paper[ln]+1);
    }
    if (PaperLen==ln && NowLine==PaperLen) {
         PutPaper (ln,"");
         if (BlockSY==PaperLen && BlockEY==PaperLen) UnselectBlock();
    } else {
         farfree (Paper[ln]);
         PaperLen=PaperLen-1;
         memmove (Paper+ln,Paper+ln+1,sizeof(*Paper)*(PaperLen-ln+1));
         Paper[PaperLen+1]=NULL;
         if (ln==BlockSY) {
             if (ln==BlockEY) UnselectBlock();
             else if (BlockEY>ln) BlockEY--;
             else UnselectBlock();
             if (BlockType==0) {
                 BlockSX=1;
                 CheckBlock0();
             }
         } else if (ln<BlockSY) {
             BlockSY--;
             if (BlockEY!=0) BlockEY--;
         } else if (ln>BlockSY) {
             if (BlockEY>ln) BlockEY--;
             else if (BlockEY==ln) {
                 if (BlockType==0) BlockEY--,BlockEX=254;
                 else BlockEY--;
             }
         }

    }
    if (ln==NowLine) SetEditBuf();
    SetScnF=True;
}

word InsertBlankLine (word ln)
{
    if (CheckBuf()!=OK) return Error;
    if (PaperLen+1>MaxPaper) {
        HSayMes ("Too many Lines");
        return Error;
    }
    memmove (Paper+ln+2,Paper+ln+1,sizeof(*Paper)*(PaperLen-ln));
    PaperLen++;
    Paper[ln+1]=NULL;
    PutPaper (ln+1,"");
    return OK;
}

word InsertLine (word ln)
{
    if (InsertBlankLine(ln)!=OK) return Error;
    if (PutEditBuf()!=OK) return Error;
    if (Paper[ln][0]>=CurXPos()) {
       if (PutPaper (ln+1,Paper[ln]+CurXPos())!=OK) return Error;
       strncpy(AuxBuf,Paper[ln]+1,CurXPos()-1);
       AuxBuf[CurXPos()-1]='\0';
       PutPaper(ln,AuxBuf);
       SetEditBuf();
    }
    if (BlockType==0) {
       if (BlockSY>ln) BlockSY++;
       else if (BlockSY==ln) {
               if (BlockSX>=CurXPos()) BlockSY++,BlockSX-=CurXPos()-1;
       }
       if (BlockEY>ln) BlockEY++;
       else if (BlockEY==ln) {
               if (BlockEX>=CurXPos()) BlockEY++,BlockEX-=CurXPos()-1;
       }
    } else {
       if (ln<BlockSY) {
           BlockSY++;
           if (BlockEY!=0) BlockEY++;
       } else if (ln<=BlockEY) BlockEY=BlockEY+1;
    }
    SetEditBuf();
    return OK;
}

static word S,E;

void SaveKillLine(byte *Data)
{
     word i;

     i=strlen(Data);
     UnKillBuf[E]=farmalloc(i+2);
     UnKillBuf[E][0]=i;
     strcpy(UnKillBuf[E]+1,Data);
     E++;
     if (E==MaxUnKillBuf) E=0;
     if (S==E) {
         farfree(UnKillBuf[E]);
         S++;
         if (S==MaxUnKillBuf) S=0;
     }
}

void EditCU(void)
{
    if (S==E) return;
    if (PutEditBuf()!=OK) return ;
    if (InsertBlankLine(NowLine-1)!=OK) return ;
    E=(E==0)?MaxUnKillBuf-1:E-1;
    Paper[NowLine]=UnKillBuf[E];
    SetEditBuf();
    if (NowLine<=BlockSY) BlockSY++;
    if (NowLine<=BlockEY) BlockEY++;
    SetScnF=True;
}


