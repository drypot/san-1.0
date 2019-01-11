/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*               Find & Replace Functions               */
/*                                                      */

#include "stddef.h"
#include "crt.h"
#include "const.h"
#include "edit.h"
#include "handrv.h"
#include "hwindow.h"
#include "memory.h"
#include <string.h>
#include <mem.h>
#include <ctype.h>
#include <stdlib.h>

struct FOptionType FOption;

byte   SearchString   [256],
       AuxSearchString[256],
       ReplaceString  [256],
       FindOption     [12] ;

word   OldFindCommand;
word   SearchCounter;
word   SearchSY,SearchEY;
word   SearchHeadCtrlAChar;
word   SearchSX;

word FindSearchString(void)
{
    char Buf[256];
    char *i;

    do {
       strcpy(Buf,Paper[SearchSY]);
       AppendSpace(Buf,SearchSX);
       if (FOption.U) strupr(Buf);
       i=strstr(Buf+SearchSX,AuxSearchString+1);
       if (i!=NULL) {
           SearchSX=i-Buf+strlen(AuxSearchString)-2;
           if (!FOption.W) return True;
           else if ((i-Buf==1 || (!isalnum(*(i-1)) && isascii(*i))) &&
                   (SearchSX==Paper[SearchSY][0]||(!isalnum(Buf[SearchSX+1]) && isascii(Buf[SearchSX]))))
                   return True;
       }
       SearchSY++;
       SearchSX=1;
    } while (SearchSY<=SearchEY);
    return False;
}

void FindSub(void)
{
    if (PutEditBuf()!=OK) return;
    do {
       if (!FindSearchString()) {
          HSayMes ("‹aœå ¢…¸aµi·¡ ´ô“e•A¶a.");
          return;
       }
       SearchCounter--;
       if (SearchCounter==0 ) FoolSetCur (SearchSX+1,SearchSY);
    } while (SearchCounter>0);
}

void ReplaceSub(void)
{
    word i,OnceFindedF;

    if (PutEditBuf()!=OK) return;
    OnceFindedF=False;
    do {
       if (!FindSearchString()) {
          if (!FOption.A || !OnceFindedF)
             HSayMes ("‹aœå ¢…¸aµi·¡ ´ô“e•A¶a.");
          return;
       }
       OnceFindedF=True;
       SearchCounter--;
       if (SearchCounter==0 || FOption.A) {
          FoolSetCur (SearchSX+1,SearchSY);
          if (SetScnF==True && !FOption.A) SetScn();
          if (!FOption.N && !FOption.A) {
             HSetInverse(SX-SearchString[0]-1,SY-1,SX-2,SY-1);
             HSayBottom ("¬ ¢…¸aµi¡ ¤aŽ‰Œa¶a (Y/N) ?");
             HSetInverse(SX-SearchString[0]-1,SY-1,SX-2,SY-1);
             if (Way=='N' || Way=='n' || Way==CtrlN) {
                  SearchCounter=1;
                  continue;
             }
             if (Way==ESC) return;
          }
          if (Paper[SearchSY][0]-AuxSearchString[0]+ReplaceString[0]>256-2) {
             HSayMes ("Ð—·¡ á¢ Äá»¡‘A¶a.");
             return;
          }
          SetEditBuf();
          i=SearchSX-SearchString[0]+1;
          EraseElement(i,SearchSX+1);
          if (InsertElement(i,ReplaceString[0])==OK) {
              strncpy(EditBuf+i,ReplaceString+1,ReplaceString[0]);
          }
          PutEditBuf();
          SetEditBuf();
          SetCurPos (SearchSX-SearchString[0]+ReplaceString[0]+1,
                     SearchSY);
       }
       if (FOption.A) {
           SearchCounter=1;
           SetScnF=True;
       }
    } while (SearchCounter>0);
}

void SetSearchoptions(void)
{
    byte BufRest[12];
    word  i,j;

    FOption.G=False;
    FOption.L=False;
    FOption.U=False;
    FOption.W=False;
    FOption.N=False;
    FOption.A=False;
    BufRest[0]=0;
    strupr (FindOption);
    for (i=0,j=0; i<12 ; i++)
        switch (FindOption[i]) {
          case 'G':FOption.G=True; break;
          case 'L':FOption.L=True; break;
          case 'U':FOption.U=True; break;
          case 'W':FOption.W=True; break;
          case 'N':FOption.N=True; break;
          case 'A':FOption.A=True; break;
          default :if (FindOption[i]>='0' && FindOption[i]<='9' && j<12)
                      BufRest[j++]=FindOption[i];
    }
    SearchCounter=atoi (BufRest);
    if (SearchCounter==0) SearchCounter=1;
    SearchSX=CurXPos();
    SearchSY=NowLine;
    SearchEY=PaperLen;
    strcpy (AuxSearchString,SearchString);
    if (FOption.U) strupr(AuxSearchString+1);
    if (FOption.G) SearchSX=1,SearchSY=1;
    if (FOption.L && ShowBlockF) SearchSX=1,SearchSY=BlockSY,SearchEY  =BlockEY;
    SearchHeadCtrlAChar=0;
    while (AuxSearchString[SearchHeadCtrlAChar+1]==1) SearchHeadCtrlAChar++;
    SearchSX=SearchSX+SearchHeadCtrlAChar;
    memmove (AuxSearchString+1,AuxSearchString+SearchHeadCtrlAChar+1,
             AuxSearchString[0]-SearchHeadCtrlAChar+1);
    AuxSearchString[0]-=SearchHeadCtrlAChar;
}

void EditCQF(void)
{
    word i;

    OldFindCommand=None;
    if (HGetMes("ˆñ¬‚ ¢…¸aµi",SearchString+1,256-2)!=OK) return;
    Explain("G:¸åÁAµA¬á  L:§iœâµA¬á  n:n¤å¼  U:”­¡ ´e Š¥i  N:¢ˆ»¡´g‹¡  W:•¢Ÿ³¢…¸aµi e");
    i=HGetEng("ˆñ¬‚ ¤w¤ó",FindOption,11);
    SetEditScn();
    if (i!=OK) return;
    SearchString[0]=strlen(SearchString+1);
    SetSearchoptions();
    if (SearchString[0]==0) return;
    OldFindCommand=Find;
    FindSub();
}

void EditCQA(void)
{
    word i;

    OldFindCommand=None;
    if (HGetMes("ˆñ¬‚ ¢…¸aµi",SearchString+1,256-2)!=OK) return;
    if (HGetMes("Ã¡ÑÅ ¢…¸aµi",ReplaceString+1,256-2)!=OK) return;
    Explain("G:¸åÁA  L:§iœâ  n:n¤å¼  U:'A'='a'  N:¢ˆ»¡´g‹¡  W:•¢Ÿ³¢…¸a  A:¡·˜wÃ¡ÑÅ");
    i=HGetEng("ˆñ¬‚ ¤w¤ó",FindOption,11);
    SetEditScn();
    if (i!=OK) return;
    SearchString[0]=strlen(SearchString+1);
    ReplaceString[0]=strlen(ReplaceString+1);
    SetSearchoptions();
    if (SearchString[0]==0)return;
    OldFindCommand=Replace;
    ReplaceSub();
}

void EditCL(void)
{
    SearchCounter=1;
    SearchSX=CurXPos();
    SearchSY=NowLine;
    SearchEY=PaperLen;
    if (OldFindCommand==Find) FindSub();
    else if (OldFindCommand==Replace) ReplaceSub();
}
