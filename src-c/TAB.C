/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*               Tab , Return , and so on               */
/*                                                      */

#include "stddef.h"
#include "crt.h"
#include "handrv.h"
#include "memory.h"
#include <string.h>
#include <alloc.h>
#include <mem.h>
#include <ctype.h>
#include <ctype.h>
#include "const.h"
#include "edit.h"
#include "hwindow.h"

word SLettersFromRow (word Row)
{
    for (;Row<256-1; Row++)
       if (isalnum(EditBuf[Row]) || !isascii(EditBuf[Row])) return False;
    return True;
}

word SLettersTillRow (word Row)
{
    for (;Row>0;Row--)
       if (isalnum(EditBuf[Row]) || !isascii(EditBuf[Row])) return False;
    return True;
}

#define SpaceTillRow(Row) ((strspn(EditBuf+1," ")>=(word)(Row))?True:False)

word SpaceFromRow (word Row)
{
    for (;Row<256-1;Row++) if (EditBuf[Row]!=' ') return False;
    return True;
}

word IsLine(word Data)
{
   union {
     byte Half[2];
     word Full;
   } Code;

   Code.Full=Data;
   if (Code.Half[0]==0xd4 && Code.Half[1]>=0xb3 && Code.Half[1]<=0xda) return OK;
   if (Code.Half[0]==0xdb && Code.Half[1]>=0xa1 && Code.Half[1]<=0xE4) return OK;
   return Error;
}

word FirstGrf(word Row)
{
     for (;Row<255;)
        if (EditType[Row]==0) Row++;
        else {
           if (IsLine(*(word*)(EditBuf+Row))) break;
           Row+=2;
        }
     return Row;
}

word ExistGrf(word Line)
{
     word Row=1;

     SetAuxBuf(Line);
     for (;Row<255;)
        if (AuxType[Row]==0) Row++;
        else {
           if (IsLine(*(word*)(AuxBuf+Row))) return OK;
           Row+=2;
        }
     return Error;
}

word IndentToLeft (word Row,word ln)
{
    do {
       ln--;
    } while (ln>0 && ( Paper[ln][0]==0 || Stringstartpos(ln)>=Row));
    return (ln==0 ) ? 1 : Stringstartpos(ln);
}

word IndentToRight (word ln)
{
    do {
       ln--;
    } while (ln>0 && Paper[ln][0]==0);
    return (ln==0) ? 1 : Stringstartpos(ln);
}

#define BlankEditBuf() ((strspn(EditBuf+1," ")==256-2) ? True : False)

void EraseElement(word S,word E)
{
    word EndPos;

    EndPos=(ProtectLineF)?FirstGrf(S):255;
    if (E>EndPos) E=EndPos;
    memmove (EditBuf +S,EditBuf +E,EndPos-E);
    memmove (EditType+S,EditType+E,EndPos-E);
    memset  (EditBuf +EndPos-(E-S),' ',E-S);
    memset  (EditType+EndPos-(E-S),0  ,E-S);
    if (BlockType==0 && (BlockSY || BlockEY)) {
        if (BlockSY==NowLine)
            if (BlockSX>=S) {
                if (BlockSX<=E) BlockSX=S;
                else if (!ProtectLineF || BlockSX<EndPos) BlockSX-=E-S;
            }
        if (BlockEY==NowLine)
            if (BlockEX>=S) {
                if (BlockEX<E) BlockEX=S-1;
                else if (!ProtectLineF || BlockEX<EndPos) BlockEX-=E-S;
                if (BlockEX==0) {
                    BlockEY--;
                    BlockEX=254;
                } else if (EditType[BlockEX]==2)
                    BlockEX--;
            }
        CheckBlock0();
    }
}

word InsertElement(word S,word Len)
{
   word i,j,EndPos;

   EndPos=(ProtectLineF)?FirstGrf(S):255;
   if (S+Len>EndPos) return Error;
   for (j=0,i=EndPos-1 ; j<Len ; j++,i--)
     if (EditType[i]!=0 || EditBuf[i]!=' ') return Error;
   memmove(EditBuf +S+Len,EditBuf +S,EndPos-S-Len);
   memmove(EditType+S+Len,EditType+S,EndPos-S-Len);
   MemSetB(EditBuf +S    ,' ',Len);
   MemSetB(EditType+S    ,0  ,Len);
   if (EditType[EndPos-1]==1) (EditType[EndPos-1]=0),(EditBuf[EndPos-1]=' ');
   if (BlockType==0 && (BlockSY || BlockEY) ) {
      if (BlockSY==NowLine) {
          if (BlockSX>=S) {
             if (!ProtectLineF || BlockSX<EndPos) {
                BlockSX+=Len;
                if (BlockSX>=EndPos) BlockSX=EndPos;
             }
          }
      }
      if (BlockEY==NowLine) {
          if (BlockEX>=S) {
             if (!ProtectLineF || BlockEX<EndPos) {
                BlockEX+=Len;
                if (BlockEX>=EndPos) BlockEX=EndPos-1;
             }
          }
      }
      CheckBlock0();
   }
   return OK;
}

word ReplaceElement(word S,word Len)
{
   word i,j,EndPos;

   EndPos=(ProtectLineF)?FirstGrf(S):255;
   if (S+Len>EndPos) return Error;
   if (EditType[S+Len]==2) Len++;
   MemSetB(EditBuf +S    ,' ',Len);
   MemSetB(EditType+S    ,0  ,Len);
   if (BlockEY==NowLine && EditType[BlockEX]==2) BlockEX--;
   if (BlockSY==NowLine && EditType[BlockSX]==2) BlockSX--;
   return OK;
}

void EditBackSpace(void)
{
    word i,j;

    i=CurXPos();
    AuxBuf[1]=0;
    if ((SpaceTillRow(i-1) && EditBuf[i]!=' ' && i>1) || (BlankEditBuf() && i>1)) {
       j=IndentToLeft(i,NowLine);
       EraseElement(j,i);
       SetCurPos (j,NowLine);
       return;
    }
    if (i>1)  {
       if (EditType[i-1]==2) {
          EraseElement(i-2,i);
          SetCurPos(i-2,NowLine);
       } else {
          EraseElement(i-1,i);
          SetCurPos(i-1,NowLine);
       }
       return;
    }
    if (NowLine==1) return ;
    if (PutEditBuf()!=OK) return;
    SetScnF=True;
    if (Paper[NowLine][0]+Paper[NowLine-1][0]<255) {
       i=Paper[NowLine-1][0];
       strcpy (AuxBuf+1  ,Paper[NowLine-1]+1);
       strcat (AuxBuf+1+i,Paper[NowLine]+1);
       if (PutPaper (NowLine-1,AuxBuf+1)!=OK) return;

       if (BlockType==0 && (BlockSY || BlockEY) ) {
          if (BlockSY==NowLine) {
             BlockSX+=i;
             BlockSY--;
          }
          if (BlockEY==NowLine) {
             BlockEX+=i;
             BlockEY--;
          }
       }
       SetEditBuf();
       SetCurPos (i+1,NowLine-1);
       DeleteLine(NowLine+1);
       return;
    }

    /*i=Paper[NowLine][0];
    strcpy (AuxBuf+1,Paper[NowLine-1]+1);
    strncat(AuxBuf+1+i,Paper[NowLine]+1,256-2-i);
    if (PutPaper (NowLine-1,AuxBuf+1)!=OK) return;
    strcpy (AuxBuf+1,Paper[NowLine]+256-1-i);
    PutPaper (NowLine,AuxBuf);
    SetCurPos (i+1,NowLine-1);
    */
}

void EditDel(void)
{
    word i,j;

    i=CurXPos();
    if (SpaceFromRow(i)) EditCT();
    else {
          j=i+1;
          if (EditType[i]==1) j++;
          EraseElement(i,j);
          SetCurPos(i,NowLine);
    }
}

void EditCT(void)
{
    word i,j,EndPos;

    i=CurXPos();
    AuxBuf[1]=0;
    if (SpaceFromRow(i)) {
        if (NowLine==PaperLen) {
            EraseElement(CurXPos(),255);
            return;
        }
        if (i+Paper[NowLine+1][0]-1<255) {
            PutEditBuf();
            strcpy(AuxBuf,Paper[NowLine]+1);
            AppendSpace(AuxBuf,i-1);
            strcat(AuxBuf,Paper[NowLine+1]+1);
            PutPaper(NowLine,AuxBuf);
            SetEditBuf();
            if (BlockType==0 && (BlockSY || BlockEY)) {
                if (BlockSY==NowLine) {
                   if (BlockSX>=i) BlockSX=i;
                } else if (BlockSY==NowLine+1) {
                   BlockSX+=i-1;
                   BlockSY--;
                }
                if (BlockEY==NowLine) {
                   if (BlockEX>=i) {
                       BlockEX=i-1;
                       if (EditType[BlockEX]==2) BlockEX--;
                   }
                } else if (BlockEY==NowLine+1) {
                   BlockEX+=i-1;
                   BlockEY--;
                   if (BlockEX>254) BlockEX=254;
                   if (EditType[BlockEX]==2) BlockEX--;
                }
                CheckBlock0();
            }
            DeleteLine (NowLine+1);
            SetScnF=True;
        } else {
            HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
            return;
            /*memcpy (EditBuf+i,Paper[NowLine+1]+1,256-1-i);
            PutEditBuf();
            SetEditBuf();
            if (EditType[254]==1) {
                EditType[254]=0;
                EditBuf [254]=' ';
                strcat (AuxBuf+1,Paper[NowLine+1]+255-i);
            } else
                strcat (AuxBuf+1,Paper[NowLine+1]+256-i);
            PutPaper (NowLine+1,AuxBuf+1);
            */
        }
        return;
    }
    j=i;
    i+=strspn (EditBuf+i,"#<>,;.()[]{}~`^'*+-/$%:\"");
    if (i>j)
        i=j+1;
    else {
        i+=strcspn (EditBuf+i," #<>,;.()[]{}~`^'*+-/$%:\"");
        if (EditType[i]==2) i++;
        i+=strspn  (EditBuf+i," ");
    }
    EraseElement(j,i);
}

void EditReturn(void)
{
    word i,j;

    if ( ProtectLineF &&
         ( (NowLine!=PaperLen && ExistGrf(NowLine+1)==OK) || FirstGrf(CurXPos())!=255)) {
       if (NowLine==PaperLen) if (InsertBlankLine (NowLine)!=OK) return;
       SetCurPos (HX+SX-ScnLeft,NowLine+1);
       return;
    }

    if (!InsertF) {
       if (NowLine==PaperLen) if (InsertBlankLine (NowLine)!=OK) return;
       if (IndentF) SetCurPos (Stringstartpos (NowLine+1),NowLine+1);
       else         SetCurPos (1,NowLine+1);
       return;
    }
    SetScnF=True;
    if (InsertLine(NowLine)!=OK) return;
    if (IndentF) {
       i=IndentToRight (NowLine+1);
       if (i+Paper[NowLine+1][0]-1>254) i=254-Paper[NowLine+1][0]+1;
       SetCurPos(i,NowLine+1);
       InsertElement(1,i-1);
    }
    else SetCurPos(1,NowLine+1);
}

void EditTab(void)
{
    word i,j,k;

    i=CurXPos();
    if (NowLine==1) return;
    j=NowLine;
    k=NowLine;
    do {
       j--;
      if (k==NowLine && Paper[j][0]>0) k=j;
    } while (j>0 && Paper[j][0]<=i);
    if (j==0 && Paper[1][0]<=i) {
        if (!SpaceTillRow(i)) return ;
        i=0;
        if (BlankEditBuf()) {
           i=Stringstartpos(k);
           if (InsertF) EraseElement(i,CurXPos());
        } else {
           do i++; while (EditBuf[i]==' ');
           if (InsertF) InsertElement(CurXPos(),i-CurXPos());
        }
        SetCurPos (i,NowLine);
        return;
    }
    SetAuxBuf(j);
    AuxBuf[0]=Paper[j][0];
    j=i;
    while (i<AuxBuf[0] && ((isalnum(AuxBuf[i])&&AuxType[i]==0)||AuxType[i]!=0)) i++;
    while (i<AuxBuf[0] && (!isalnum(AuxBuf[i])&&AuxType[i]==0)) i++;
    if (InsertF) {
       InsertElement(j,i-j);
       SetCurPos (i,NowLine);
    } else SetCurPos (i,NowLine);
}

void EditAT(void)
{
     word i,S,D;
     word BigF;

     PutEditBuf();
     for (i=1; i<=PaperLen ; i++) {
        S=D=1;
        BigF=False;
        while (S<=Paper[i][0]) {
          if (D>254) {
             BigF=True;
             break;
          }
          if (Paper[i][S]=='\t') {
            while (D%8!=0 && D!=254) AuxBuf[D++]=' ';
            AuxBuf[D++]=' ';
          }
          else AuxBuf[D++]=Paper[i][S];
          S++;
        }
        if (!BigF) {
           AuxBuf[D]=0;
           if (PutPaper(i,AuxBuf+1)!=OK) break;
        }
     }
     SetEditBuf();
     SetScnF=True;
}


