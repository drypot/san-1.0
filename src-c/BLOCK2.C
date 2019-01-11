/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                  Block Management                    */
/*                                                      */

#include "stddef.h"
#include <mem.h>
#include <alloc.h>
#include "handrv.h"
#include "const.h"
#include "edit.h"
#include "memory.h"
#include <string.h>
#include "hwindow.h"
#include "hanconv.h"

void EraseBlock0(void)
{
    word i,j;

    if (PutEditBuf()!=OK) return;
    if (! ShowBlockF) return;

    if (BlockSX<=Paper[BlockSY][0]) {
         SaveKillLine(Paper[BlockSY]+BlockSX);
         strncpy(AuxBuf,Paper[BlockSY]+1,BlockSX-1);
         AuxBuf[BlockSX-1]=0;
         PutPaper(BlockSY,AuxBuf);
    }

    if (BlockSY!=BlockEY) {
       i=BlockSY+1;
       while (i<BlockEY) {
         SaveKillLine(Paper[i]+1);
         PutPaper(i,"");
         i++;
       }
    }

    if (Paper[BlockEY][0]>=BlockEX) {
        i=BlockEX;
        if (Paper[BlockEY][BlockEX]>127) i++;
        strncpy(AuxBuf,Paper[BlockEY]+1,i);
        AuxBuf[i]=0;
        SaveKillLine(AuxBuf);
        AuxBuf[0]=0;
        AppendSpace(AuxBuf,i);
        strcat(AuxBuf,Paper[BlockEY]+i+1);
        PutPaper(BlockEY,AuxBuf);
    }

    BlockSX=BlockSY=BlockEX=BlockEY=0;
    ShowBlockF=False;
    SetScnF=True;
    SelectingBlockF=False;
    SetEditBuf();
}

void EraseBlock1(void)
{
    word i,j;

    if (PutEditBuf()!=OK) return;
    if (! ShowBlockF) return;
    for (i=BlockSY; i<=BlockEY; i++) {
      SaveKillLine(Paper[i]+1);
      PutPaper(i,"");
    }
    BlockSY=0;
    BlockEY=0;
    SetScnF=True;
    SelectingBlockF=False;
    SetEditBuf();
}

void EraseBlock2(void)
{
    word i,j,S,E;

    if (PutEditBuf()!=OK) return;
    if (! ShowBlockF) return;
    for (i=BlockSY ; i<=BlockEY ; i++) {
        if (BlockEX<=Paper[i][0]) {
           S=BlockSX;
           E=BlockEX;
           SetAuxBuf(i);
           if (AuxType[E]==1) E++;
           if (AuxType[S]==2) S--;
           strncpy(EditBuf,Paper[i]+S,E-S+1);
           EditBuf[E-S+1]=0;
           SaveKillLine(EditBuf);
           MemSetB(Paper[i]+S,' ',E-S+1);
        } else if (BlockSX<=Paper[i][0]) {
           S=BlockSX;
           SetAuxBuf(i);
           if (AuxType[S]==2) S--;
           SaveKillLine(Paper[i]+S);
           strncpy(AuxBuf,Paper[i]+1,S-1);
           AuxBuf[S-1]=0;
           PutPaper(i,AuxBuf);
        }
    }
    BlockSX=BlockSY=BlockEX=BlockEY=0;
    ShowBlockF=False;
    SetScnF=True;
    SelectingBlockF=False;
    SetEditBuf();
}

void EditCKE(void)
{
    switch (BlockType) {
      case 0 :EraseBlock0(); break;
      case 1 :EraseBlock1(); break;
      case 2 :EraseBlock2();
    }
}

word IsLine(word Data);

void DeleteLineChar(byte *Data,word Len)
{
    word i;

    for (i=0 ; i<Len ; )
      if (Data[i]<128)
         if (Data[i]==0) break;
         else i++;
      else {
         if (IsLine(*(word*)(Data+i))) {
            Data[i++]=' ';
            Data[i++]=' ';
         } else i+=2;
      }
}

void DeleteNonLineChar(byte *Data,word Len)
{
    word i;

    for (i=0 ; i<Len ; )
      if (Data[i]<128)
         if (Data[i]==0) break;
         else Data[i++]=' ';
      else {
         if (IsLine(*(word*)(Data+i))) i+=2;
         else {
            Data[i++]=' ';
            Data[i++]=' ';
         }
      }
    SelectingBlockF=False;
}

void EditCKZ(void)
{
    word i,S,E;

    if (PutEditBuf()!=OK) return;
    if (!ShowBlockF) return;
    if (BlockType==0 && BlockSY!=BlockEY) {
       for (i=BlockSY ; i<=BlockEY ; i++) {
          if (i>BlockSY && i<BlockEY) {
              DeleteLineChar(Paper[i]+1,Paper[i][0]);
          } else if (i==BlockSY && Paper[i][0]>=BlockSX) {
              DeleteLineChar(Paper[i]+BlockSX,Paper[i][0]-BlockSX+1);
          } else if (i==BlockEY) {
              E=BlockEX;
              if (Paper[i][0]<=E) E=Paper[i][0];
              else if (Paper[i][BlockEX]>127) E++;
              DeleteLineChar(Paper[i]+1,E);
          }
       }
    } else if (BlockType==1) {
       for (i=BlockSY ; i<=BlockEY ; i++) {
          DeleteLineChar(Paper[i]+1,Paper[i][0]);
       }
    } else {
       for (i=BlockSY ; i<=BlockEY ; i++) {
          SetAuxBuf(i);
          S=BlockSX;
          E=BlockEX;
          if (AuxType[S]==2) S--;
          if (AuxType[E]==1) E++;
          DeleteLineChar(Paper[i]+S,E-S+1);
       }
    }
    SetEditBuf();
    SetScnF=True;
    SelectingBlockF=False;
}
    
void EditCKX(void)
{
    word i,S,E;

    if (PutEditBuf()!=OK) return;
    if (!ShowBlockF) return;
    if (BlockType==0 && BlockSY!=BlockEY) {
       for (i=BlockSY ; i<=BlockEY ; i++) {
          if (i>BlockSY && i<BlockEY) {
              DeleteNonLineChar(Paper[i]+1,Paper[i][0]);
          } else if (i==BlockSY && Paper[i][0]>=BlockSX) {
              DeleteNonLineChar(Paper[i]+BlockSX,Paper[i][0]-BlockSX+1);
          } else if (i==BlockEY) {
              E=BlockEX;
              if (Paper[i][0]<=E) E=Paper[i][0];
              else if (Paper[i][BlockEX]>127) E++;
              DeleteNonLineChar(Paper[i]+1,E);
          }
       }
    } else if (BlockType==1) {
       for (i=BlockSY ; i<=BlockEY ; i++) {
          DeleteNonLineChar(Paper[i]+1,Paper[i][0]);
       }
    } else {
       for (i=BlockSY ; i<=BlockEY ; i++) {
          SetAuxBuf(i);
          S=BlockSX;
          E=BlockEX;
          if (AuxType[S]==2) S--;
          if (AuxType[E]==1) E++;
          DeleteNonLineChar(Paper[i]+S,E-S+1);
       }
    }
    SetEditBuf();
    SetScnF=True;
    SelectingBlockF=False;
}

