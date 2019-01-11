/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                      Block Sort                      */
/*                                                      */

#include "stddef.h"
#include "crt.h"
#include <mem.h>
#include <alloc.h>
#include "handrv.h"
#include "hwindow.h"
#include "const.h"
#include "edit.h"
#include <ctype.h>
#include "memory.h"
#include <string.h>

word   BlockLeft=4;
word   BlockRight=75;
word   BlockIndent=2;
word   BlockSortType;
word   BlockSpace=0;

void MoveBlockSort(void)
{
    byte *BufRest[21];
    word j,is,ie,ibase;

    if (PutEditBuf()!=OK) return;
    if (!ShowBlockF) return;
    if (NowLine>=BlockSY && NowLine<=BlockEY+1) return;
    if (NowLine<BlockSY) is=BlockSY,ie=BlockEY,ibase=NowLine;
    else                 is=BlockEY+1,ie=NowLine-1,ibase=BlockSY;
    do  {
        j=(ie-is+1>20) ? 20 : ie-is+1;
        memmove (BufRest,Paper+is,j*sizeof(*Paper));
        memmove (Paper+ibase+j,Paper+ibase,(is-ibase)*sizeof(*Paper));
        memmove (Paper+ibase,BufRest,j*sizeof(*Paper));
        is+=j;
        ibase+=j;
    } while (is<=ie);
    SetScnF=True;
    SelectingBlockF=False;
    if (NowLine<BlockSY) BlockSY=NowLine,BlockEY=ibase-1;
    else                 BlockEY=NowLine-1,BlockSY=ibase;
    SetEditBuf();
    SetCurPos(CurXPos(),BlockSY);

}

word ToRightSub(void)
{
     word j;

     while (AuxBuf[1]==' ') memmove(AuxBuf+1,AuxBuf+2,253);
     if (strlen(AuxBuf+1)<BlockRight) {
         j=BlockRight-strlen(AuxBuf+1)+1;
         memmove(AuxBuf+j,AuxBuf+1,strlen(AuxBuf+1)+1);
         MemSetB(AuxBuf+1,' ',j-1);
         return OK;
     } else return Error;
}

word ToLeftSub(void)
{
     word j;

     while (AuxBuf[1]==' ') memmove(AuxBuf+1,AuxBuf+2,253);
     if (strlen(AuxBuf+1)+BlockLeft-1<=254) {
          j=BlockLeft;
          memmove(AuxBuf+j,AuxBuf+1,strlen(AuxBuf+1)+1);
          MemSetB(AuxBuf+1,' ',j-1);
          return OK;
     } else return Error;
}

word ToCenterSub(void)
{
     word j;

     while (AuxBuf[1]==' ') memmove(AuxBuf+1,AuxBuf+2,253);
     if (strlen(AuxBuf+1)<=(BlockRight-BlockLeft+1)) {
         j=BlockLeft+(BlockRight-BlockLeft+1-strlen(AuxBuf+1))/2;
         memmove(AuxBuf+j,AuxBuf+1,strlen(AuxBuf+1)+1);
         MemSetB(AuxBuf+1,' ',j-1);
         return OK;
     } else return Error;
}

void EditAQ(void)
{
     word i;

     i=1;
     while (EditType[i]==0 && EditBuf[i]==' ') i++;
     EraseElement(1,i);
     InsertElement(1,BlockLeft-1);
}

void EditAW(void)
{
     word i,j;

     i=254; j=1;
     while (EditBuf[i]==' ' && EditType[i]==0) i--;
     while (EditBuf[j]==' ' && EditType[j]==0) j++;
     if (j>i) return;
     EraseElement(1,j);
     if (BlockRight-BlockLeft<i-j)
        InsertElement(1,BlockLeft-1);
     else
        InsertElement(1,BlockLeft+(BlockRight-BlockLeft-(i-j))/2-1);
}

void EditAE(void)
{
     word i,j;

     i=254; j=1;
     while (EditBuf[i]==' ' && EditType[i]==0) i--;
     while (EditBuf[j]==' ' && EditType[j]==0) j++;
     EraseElement(1,j);
     if (i-j+1<BlockRight)
       InsertElement(1,BlockRight-(i-j+1));
}

void ToRight(void)
{
     word i;

     if (PutEditBuf()!=OK) return;
     for (i=BlockSY ; i<=BlockEY ; i++) {
          if (farcoreleft()<MaxBufRest) {
              BufErr();
              return;
          }
          strcpy(AuxBuf+1,Paper[i]+1);
          if (ToRightSub()==OK) PutPaper(i,AuxBuf+1);
     }
     SetScnF=True;
     SetEditBuf();
}

void ToLeft(void)
{
     word i,j;

     if (PutEditBuf()!=OK) return;
     for (i=BlockSY ; i<=BlockEY ; i++) {
          if (farcoreleft()<MaxBufRest) {
              BufErr();
              return;
          }
          strcpy(AuxBuf+1,Paper[i]+1);
          if (ToLeftSub()==OK) PutPaper(i,AuxBuf+1);
     }
     SetScnF=True;
     SetEditBuf();
}

void ToCenter(void)
{
     word i,j;

     if (PutEditBuf()!=OK) return;
     for (i=BlockSY ; i<=BlockEY ; i++) {
          if (farcoreleft()<MaxBufRest) {
              BufErr();
              return;
          }
          strcpy(AuxBuf+1,Paper[i]+1);
          if (ToCenterSub()==OK) PutPaper(i,AuxBuf+1);
     }
     SetScnF=True;
     SetEditBuf();

}



void LineUp(void)
{
     word Char;
     word PaperLenBuf;
     word i,j,k,l,m;
     word SpaceList[254];
     word SP;
     word C;
     word LineEndF;

     if (PutEditBuf()!=OK) return;

     PaperLenBuf=PaperLen;
     if ((int)BlockIndent>=0)
        C=BlockLeft+BlockIndent;
     else
        if ((int)(C=BlockLeft+(int)BlockIndent)<1) C=1;
     MemSetB(AuxBuf+1,' ',C-1);
     /*SpaceList[0]=BlockLeft;SP=1;*/
     SP=0;
     LineEndF=False;
     for (i=BlockSY ; i<=BlockEY ; i++) {
        MemCopy(Paper[i],EditBuf,Paper[i][0]+2);
        EditBuf[++EditBuf[0]]=' ';
        for (j=1; j<=EditBuf[0] ; ) {
          Char=EditBuf[j];
          if (Char<33) {
             j++;
             if (C==1 || AuxBuf[C-1]==' ') continue;
             SpaceList[SP++]=C;
             AuxBuf [C]=' ';
             AuxType[C]=0;
             C++;
          } else if (Char<128) {
             AuxBuf [C]=Char;
             AuxType[C]=0;
             C++;
             j++;
          } else {
             if (C==BlockRight) LineEndF=True;
             else {
                AuxBuf [C]  =EditBuf[j];
                AuxType[C]  =1;
                AuxBuf [C+1]=EditBuf[j+1];
                AuxType[C+1]=2;
                j+=2;
                C+=2;
            }
          }
          if (C>BlockRight || LineEndF) {
            LineEndF=False;
            if (farcoreleft()<MaxBufRest) {
              i=PaperLenBuf+1;
              while (i<=PaperLen) farfree(Paper[i]),i++;
              MemSetB(Paper+PaperLenBuf+1,0,sizeof(*Paper)*(PaperLen-PaperLenBuf));
              PaperLen=PaperLenBuf;
              BufErr();
              SetEditBuf();
              return;
            }
            if (PaperLen+BlockSpace>=MaxPaper ) {
              HSayMes ("á¢  g·e Ð—·¡ ·¶“e ¢…¬á‘A¶a.");
              i=PaperLenBuf+1;
              while (i<=PaperLen) farfree(Paper[i]),i++;
              MemSetB(Paper+PaperLenBuf+1,0,sizeof(*Paper)*(PaperLen-PaperLenBuf));
              PaperLen=PaperLenBuf;
              SetEditBuf();
              return ;
            }
            if (BlockSortType==0) {
               if (j<=EditBuf[0] &&
                     ( Held(EditBuf[j],'!','/') ||
                       Held(EditBuf[j],':','?') ||
                       Held(EditBuf[j],'[','_') ||
                       Held(EditBuf[j],'{','~')
                     )
                  )
                 {
                 k=C-1;
                 while (AuxType[k]==0 &&
                         (
                           Held(EditBuf[k],'!','/') ||
                           Held(EditBuf[k],':','?') ||
                           Held(EditBuf[k],'[','_') ||
                           Held(EditBuf[k],'{','~') ||
                           AuxBuf[k]==' ' ) &&
                           k>BlockLeft+1) k--;
                 if (AuxType[k]==2) k--;
                 strncpy(UndoBuf,AuxBuf+k,C-k);
                 UndoBuf[C-k]=0;
                 C=k;
              } else UndoBuf[0]=0;
            } else {
              if (j<=EditBuf[0] && EditBuf[j]!=' ')
                {
                 k=C-1;
                 while ( !(AuxType[k]==0&&AuxBuf[k]==' ')&& k>BlockLeft+1) k--;
                 if (AuxType[k]==1) k--;
                 k++;
                 strncpy(UndoBuf,AuxBuf+k,C-k);
                 UndoBuf[C-k]=0;
                 C=k;
                 if (SP!=0) SP--;
              } else UndoBuf[0]=0;
            }

            while (C>2 && AuxBuf[C-1]==' ') C--;
            k=SP;
            while (k>0 && SpaceList[k-1]>=C) k--;
            l=k;
            if (l!=0 && BlockSortType!=2)
               while (C<=BlockRight) {
                 l--;
                 C++;
                 memmove(AuxBuf+SpaceList[l]+1,AuxBuf+SpaceList[l],C-SpaceList[l]);
                 m=l;
                 while (m<k) SpaceList[m++]++;
                 if (l==0) l=k;
               }

            AuxBuf[C]=0;
            PaperLen++;
            Paper[PaperLen]=(byte far *)farmalloc(C+1);
            Paper[PaperLen][0]=C-1;
            strcpy((char far *)Paper[PaperLen]+1,(char far *)AuxBuf+1);
            for (k=1 ; k<=BlockSpace ;k++) {
              PaperLen++;
              Paper[PaperLen]=(byte far *)farmalloc(2);
              Paper[PaperLen][0]=0;
              Paper[PaperLen][1]=0;
            }

            C=BlockLeft+strlen(UndoBuf);
            SP=0;
            MemSetB(AuxBuf+1,' ',BlockLeft-1);
            strcpy(AuxBuf+BlockLeft,UndoBuf);
          }
        }
     }
     AuxBuf[C]=0;
     PaperLen++;
     Paper[PaperLen]=(byte far *)farmalloc(C+1);
     Paper[PaperLen][0]=C-1;
     strcpy((char far *)Paper[PaperLen]+1,(char far *)AuxBuf+1);

     i=BlockSY;
     j=PaperLen-PaperLenBuf;
     NowLine=i;
     SelectingBlockF=False;
     FoolSetCur(BlockLeft+BlockIndent,i);
     SetEditBuf();
     ShowBlockF=True;
     DeleteBlock();
     ShowBlockF=True;
     BlockEY=PaperLen;
     BlockSY=PaperLen-j+1;
     BlockType=1;
     NowLine=i;
     MoveBlockSort();
}

void EditCKL(void)
{
     if (ShowBlockF==False) return;
     if (BlockType!=1) {
         BlockType=1;
         SetScnF=True;
         SetScn();
     }
     HSayBottom("[R]µ¡Ÿe½¢·a¡   [L]¶E½¢·a¡   [C]º—´w·a¡   [S]Ša‰Á¬á ¢…”eÀáœñ");
     switch (Way) {
       case 'R'  :
       case 'r'  :
       case CtrlR:ToRight();
                  break;
       case 'L'  :
       case 'l'  :
       case CtrlL:ToLeft();
                  break;
       case 'C'  :
       case 'c'  :
       case CtrlC:ToCenter();
                  break;
       case 'S'  :
       case 's'  :
       case CtrlS:LineUp();
     }
}
