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

word   BlockSY       ,
       BlockSX       ,
       BlockEY       ,
       BlockEX       ,
       BlockHeadY    ,
       BlockHeadX    ;
word   BlockType;
word   SelectingBlockF;
word   ShowBlockF    ;

byte *AppendSpace(byte *Data,word Len)
{
     word j;

     j=strlen(Data);
     while (j<Len) Data[j]=' ',j++;
     Data[j]=0;
     return Data;
}

void CopyBlock0(void)
{
    word j,iBase,i,eBase,BlockHigh;
    word long BlockSize;

    if (PutEditBuf()!=OK) return;
    if (!ShowBlockF) return ;
    BlockHigh=BlockEY-BlockSY+1;
    for (BlockSize=0,i=BlockSY;i<=BlockEY;i++) BlockSize+=16*((word)(Paper[i][0]+16) / 16 +1);
    if (farcoreleft()<BlockSize+MaxBufRest) {
        BufErr();
        return ;
    }
    if (BlockSY!=BlockEY) {
        if (PaperLen+BlockHigh+1>MaxPaper ) {
            HSayMes ("¢…¬áˆa á¢ ‹©´á»³“¡”a.");
            return ;
        }
        BlockHigh=BlockEY-BlockSY+1;
        iBase=MaxPaper-BlockHigh+1;
        eBase=NowLine+BlockHigh;
        if (BlockEX==254) eBase++;

        /* Range Check */

        i=(Paper[BlockSY][0]>=BlockSX)?Paper[BlockSY][0]-BlockSX+1:0;
        j=(Paper[NowLine][0]>=CurXPos())?Paper[NowLine][0]-CurXPos()+1:0;
        if (Paper[BlockEY][0]>BlockEX && Paper[BlockEY][BlockEX]>127) j++;
        if (CurXPos()+i-1>254 || (BlockEX<254 && BlockEX+j-1>254)) {
            HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
            return;
        }

        /* BlockStartY put to Extra Buf */

        i=0;
        if (Paper[BlockSY][0]<BlockSX) PutPaper(iBase,"");
        else                           PutPaper(iBase,Paper[BlockSY]+BlockSX);
        i++;

        /* MidBlock put to Extra Buf */

        while (BlockSY+i<BlockEY) PutPaper(iBase+i,Paper[BlockSY+i]+1),i++;

        /* EndBlockLine  put to Extra Buf */

        if (Paper[BlockEY][0]>=BlockEX) {
           i=(Paper[BlockEY][BlockEX]>127)
              ?BlockEX+1:BlockEX;
           strncpy(AuxBuf,Paper[BlockEY]+1,i);
           AuxBuf[i]=0;
           PutPaper(MaxPaper,AuxBuf);
        } else {
           strcpy(AuxBuf,Paper[BlockEY]+1);
           PutPaper(MaxPaper,AppendSpace(AuxBuf,BlockEX));
        }

        /* Insert address blank */

        memmove (Paper+eBase,Paper+NowLine+1,(PaperLen-NowLine)*sizeof(*Paper));
        MemSetB (Paper+NowLine+1,0,(eBase-NowLine-1)*sizeof(*Paper));
        PaperLen+=BlockHigh-1;
        if (BlockEX==254) PaperLen++;

        /* Separate NowLine */

        if (Paper[NowLine][0]>=CurXPos()) {
            PutPaper(eBase-1,Paper[NowLine]+CurXPos());
            strncpy(AuxBuf,Paper[NowLine]+1,CurXPos()-1);
            AuxBuf[CurXPos()-1]=0;
            PutPaper(NowLine,AuxBuf);
        } else {
            PutPaper(eBase-1,"");
            strcpy(AuxBuf,Paper[NowLine]+1);
            PutPaper(NowLine,AppendSpace(AuxBuf,CurXPos()-1));
        }

        /* Copy Block Start Line */

        strcpy(AuxBuf,Paper[NowLine]+1);
        BlockSX=strlen(AuxBuf)+1;
        strcat(AuxBuf,Paper[iBase]+1);
        PutPaper(NowLine,AuxBuf);
        farfree(Paper[iBase]);
        Paper[iBase]=NULL;
        BlockSY=NowLine;

        /* Copy Block Mid */

        memmove (Paper+NowLine+1,Paper+iBase+1,(BlockHigh-2)*sizeof(*Paper));
        MemSetB (Paper+iBase+1,0,(BlockHigh-2)*sizeof(*Paper));

        /* Copy Block End Line */

        if (BlockEX==254) {
           Paper[BlockEY=eBase-2]=Paper[MaxPaper];
           Paper[MaxPaper]=NULL;
        } else {
           strcpy (AuxBuf,Paper[MaxPaper]+1);
           strcat (AuxBuf,Paper[eBase-1]+1);
           PutPaper(BlockEY=eBase-1,AuxBuf);
           farfree(Paper[MaxPaper]);
           Paper[MaxPaper]=NULL;
        }
    } else {
        if (BlockEX==254) {
            if (Paper[BlockEY][0]>=BlockSX) {
                strcpy(AuxBuf,Paper[BlockEY]+BlockSX);
            } else {
                AuxBuf[0]=0;
            }
            if (InsertBlankLine(NowLine)==Error) return;
            if (Paper[NowLine][0]>=CurXPos()) {
               strcpy(EditBuf,Paper[NowLine]+CurXPos());
               PutPaper(NowLine+1,EditBuf);
               strncpy(EditBuf,Paper[NowLine]+1,CurXPos()-1);
               EditBuf[CurXPos()-1]=0;
               strcat(EditBuf,AuxBuf);
            } else {
               strcpy(EditBuf,Paper[NowLine]+1);
               AppendSpace(EditBuf,CurXPos()-1);
               strcat(EditBuf,AuxBuf);
            }
            PutPaper(NowLine,EditBuf);
            BlockSY=BlockEY=NowLine;
            BlockSX=CurXPos();
        } else {
            if (Paper[BlockEY][0]>=BlockEX) {
                i=(Paper[BlockEY][BlockEX]>127)?BlockEX+1:BlockEX;
                strncpy(AuxBuf,Paper[BlockEY]+BlockSX,i-BlockSX+1);
                AuxBuf[i-BlockSX+1]=0;
            } else if (Paper[BlockEY][0]>=BlockSX) {
                strcpy(AuxBuf,Paper[BlockEY]+BlockSX);
                AppendSpace(AuxBuf,BlockEX-BlockSX+1);
            } else {
                AuxBuf[0]=0;
                AppendSpace(AuxBuf,BlockEX-BlockSX+1);
            }
            if (Paper[NowLine][0]+strlen(AuxBuf)>254) {
                HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
                return ;
            }
            if (CurXPos()>Paper[NowLine][0]) {
                strcpy(EditBuf+1,Paper[NowLine]+1);
                AppendSpace(EditBuf+1,CurXPos()-1);
                strcat(EditBuf+CurXPos(),AuxBuf);
                PutPaper(NowLine,EditBuf+1);
            } else {
                strncpy(EditBuf,Paper[NowLine]+1,CurXPos()-1);
                EditBuf[CurXPos()-1]=0;
                strcat(EditBuf,AuxBuf);
                strcat(EditBuf,Paper[NowLine]+CurXPos());
                PutPaper(NowLine,EditBuf);
            }
            BlockSY=BlockEY=NowLine;
            BlockEX=BlockEX-BlockSX;
            BlockSX=CurXPos();
            BlockEX=BlockEX+BlockSX;
        }
    }
    SetEditBuf();
    SetScnF=True;
    SelectingBlockF=False;
    return ;
}

void CopyBlock1(void)
{
    word BlockHigh,i;
    word long BlockSize;

    if (PutEditBuf()!=OK) return;
    if (!ShowBlockF) return;
    if (NowLine>=BlockSY && NowLine<BlockEY) return;
    BlockHigh=BlockEY-BlockSY+1;
    if (PaperLen+BlockHigh>MaxPaper ) {
        HSayMes ("¢…¬áˆa á¢ ‹©´á»³“¡”a.");
        return;
    }
    for (BlockSize=0,i=BlockSY;i<=BlockEY;i++) BlockSize+=16*((word)(Paper[i][0]+16) / 16 +1);
    if (farcoreleft()<BlockSize+MaxBufRest) {
        BufErr();
        return;
    }
    memmove (Paper+NowLine+BlockHigh+1,Paper+NowLine+1,(PaperLen-NowLine)*sizeof(*Paper));
    PaperLen=PaperLen+BlockHigh;
    if (NowLine<BlockSY)  {
         BlockSY=BlockSY+BlockHigh;
         BlockEY=BlockEY+BlockHigh;
    }
    for (i=1;i<=BlockHigh;i++) {
        Paper[NowLine+i]=NULL;
        PutPaper(NowLine+i,Paper[BlockSY+i-1]+1);
    }
    BlockSY=NowLine+1;
    BlockEY=BlockSY+BlockHigh-1;
    SetEditBuf();
    SetScnF=True;
    SelectingBlockF=False;
    return;
}

void CopyBlock2(void)
{
    word i,BlockHigh,S,E,HeadF,TailF;
    word long BlockSize;

    if (PutEditBuf()!=OK) return;
    if (!ShowBlockF) return;
    BlockHigh=BlockEY-BlockSY+1;
    if (PaperLen+BlockHigh*2>MaxPaper ) {
        HSayMes ("¢…¬áˆa á¢ ‹©´á»³“¡”a.");
        return;
    }
    for (BlockSize=0,i=BlockSY;i<=BlockEY;i++) BlockSize+=16*((word)(BlockEX-BlockSX+16) / 16 +1);
    if (farcoreleft()<BlockSize*2+MaxBufRest) {
        BufErr();
        return;
    }
    for (i=0 ; i<BlockHigh; i++)
        if (Paper[NowLine+i][0]+(BlockEX-BlockSX+1)>254) {
           HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
           return;
        }
    while (NowLine+BlockHigh-1>PaperLen) PutPaper(++PaperLen,"");
    HeadF=TailF=False;
    for (i=BlockSY; i<=BlockEY; i++) {
         SetAuxBuf(i);
         if (AuxType[BlockSX]==2) HeadF=True;
         if (AuxType[BlockEX]==1) TailF=True;
         if (HeadF && TailF) break;
    }
    for (i=0 ; i<BlockHigh ; i++) {
         SetAuxBuf(BlockSY+i);
         S=BlockSX;
         if (AuxType[S]==2) S--;
         else if (HeadF) AuxBuf[--S]=' ';
         E=BlockEX;
         if (AuxType[E]==1) E++;
         else if (TailF) AuxBuf[++E]=' ';
         AuxBuf[E+1]=0;
         PutPaper(PaperLen+i+1,AuxBuf+S);
    }
    if (HeadF) BlockSX--;
    if (TailF) BlockEX++;
    for (i=0 ; i<BlockHigh ; i++) {
         SetAuxBuf(NowLine+i);
         MemSetB(EditBuf,0,256);
         S=CurXPos();
         if (AuxType[S]==2) S--;
         strncpy(EditBuf+1,AuxBuf+1,S-1);
         if (S!=CurXPos()) strcat (EditBuf+S," ");
         strcat(EditBuf+S,Paper[PaperLen+i+1]+1);
         if (Paper[NowLine+i][0]>=S) strcat(EditBuf+S,Paper[NowLine+i]+S);
         PutPaper(NowLine+i,EditBuf+1);
         farfree(Paper[PaperLen+i+1]);
         Paper[PaperLen+i+1]=NULL;
    }
    BlockEX=BlockEX-BlockSX;
    BlockSX=CurXPos();
    BlockEX+=BlockSX;
    BlockEY=BlockEY-BlockSY;
    BlockSY=NowLine;
    BlockEY+=BlockSY;
    SetEditBuf();
    SetScnF=True;
    SelectingBlockF=False;
}

void CopyBlock(void)
{
    switch (BlockType) {
      case 0 :CopyBlock0(); break;
      case 1 :CopyBlock1(); break;
      case 2 :CopyBlock2();
    }
}

void GetBlock0(word TaskN)
{
    word j,i,eBase,BlockHigh,iSY,iEY,iSX,iEX;
    byte **iPaper;
    word long BlockSize;

    if (NowTask==TaskN) {
       CopyBlock();
       return;
    }
    if (PutEditBuf()!=OK) return;
    if (!Task[TaskN].ShowBlockF) return ;
    iSY=Task[TaskN].BlockSY;
    iEY=Task[TaskN].BlockEY;
    iSX=Task[TaskN].BlockSX;
    iEX=Task[TaskN].BlockEX;
    iPaper=Task[TaskN].Paper;
    BlockHigh=iEY-iSY+1;
    for (BlockSize=0,i=iSY ; i<=iEY ;i++) BlockSize+=16*((word)(iPaper[i][0]+16) / 16 +1);
    if (farcoreleft()<BlockSize+MaxBufRest) {
        BufErr();
        return ;
    }
    if (iSY!=iEY) {
        if (PaperLen+BlockHigh>MaxPaper ) {
            HSayMes ("¢…¬áˆa á¢ ‹©´á»³“¡”a.");
            return ;
        }
        eBase=NowLine+BlockHigh;
        if (iEX==254) eBase++;

        /* Range Check */

        i=(iPaper[iSY][0]>=iSX)?iPaper[iSY][0]-iSX+1:0;
        j=(Paper[NowLine][0]>=CurXPos())?Paper[NowLine][0]-CurXPos()+1:0;
        if (iPaper[iEY][0]>iEX && iPaper[iEY][iEX]>127) j++;
        if (CurXPos()+i-1>254 || (iEX<254 && iEX+j>254)) {
            HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
            return;
        }

        /* Insert address blank */

        memmove (Paper+eBase,Paper+NowLine+1,(PaperLen-NowLine)*sizeof(*Paper));
        MemSetB (Paper+NowLine+1,0,(eBase-NowLine-1)*sizeof(*Paper));
        PaperLen+=BlockHigh-1;
        if (iEX==254) PaperLen++;

        /* Separate NowLine */

        if (Paper[NowLine][0]>=CurXPos()) {
            PutPaper(eBase-1,Paper[NowLine]+CurXPos());
            strncpy(AuxBuf,Paper[NowLine]+1,CurXPos()-1);
            AuxBuf[CurXPos()-1]=0;
            PutPaper(NowLine,AuxBuf);
        } else {
            PutPaper(eBase-1,"");
            strcpy(AuxBuf,Paper[NowLine]+1);
            AppendSpace(AuxBuf,CurXPos()-1);
            PutPaper(NowLine,AuxBuf);
        }

        /* Copy Block Start Line */

        strcpy(AuxBuf,Paper[NowLine]+1);
        BlockSX=CurXPos();
        if (iPaper[iSY][0]>=iSX) strcat(AuxBuf,iPaper[iSY]+iSX);
        PutPaper(NowLine,AuxBuf);
        BlockSY=NowLine;

        /* Copy Block Mid */

        i=1;
        while (i<BlockHigh-1) {
           PutPaper(NowLine+i,iPaper[iSY+i]+1);
           i++;
        }

        /* Copy Block End Line */

        BlockEX=iEX;
        if (iEX==254)
           PutPaper(BlockEY=eBase-2,iPaper[iEY]+1);
        else {
           strcpy (AuxBuf+1,iPaper[iEY]+1);
           AppendSpace(AuxBuf+1,iEX);
           if (AuxBuf[iEX]>127) iEX++;
           strcpy (AuxBuf+iEX+1,Paper[eBase-1]+1);
           PutPaper(BlockEY=eBase-1,AuxBuf+1);
        }
    } else {
        if (Paper[NowLine][0]+iEX-iSX+2>254) {
            HSayMes ("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
            return;
        }
        strcpy(UndoBuf+1,iPaper[iSY]+1);
        AppendSpace(UndoBuf+1,iEX);
        i=iEX;
        if (UndoBuf[i]>127) i++;
        UndoBuf[i+1]=0;
        if (Paper[NowLine][0]>=CurXPos()) {
           strncpy(AuxBuf+1,Paper[NowLine]+1,CurXPos()-1);
           strncpy(AuxBuf+CurXPos(),UndoBuf+iSX,i-iSX+1);
           strcpy (AuxBuf+CurXPos()+i-iSX+1,Paper[NowLine]+CurXPos());
        } else {
           strcpy (AuxBuf+1,Paper[NowLine]+1);
           AppendSpace(AuxBuf+1,CurXPos()-1);
           strcpy (AuxBuf+CurXPos(),UndoBuf+iSX);
        }
        PutPaper(NowLine,AuxBuf+1);
        BlockSY=BlockEY=NowLine;
        BlockEX=iEX-iSX;
        BlockSX=CurXPos();
        BlockEX+=BlockSX;
    }
    ShowBlockF=True;
    BlockType=0;
    SetEditBuf();
    SetScnF=True;
    SelectingBlockF=False;
    return ;

}

void GetBlock1(word TaskN)
{
    word long BlockSize;
    word BlockHigh,i;

    if (NowTask==TaskN) {
       CopyBlock();
       return;
    }
    if (PutEditBuf()!=OK) return;
    if (!Task[TaskN].ShowBlockF) return;
    BlockHigh=Task[TaskN].BlockEY-Task[TaskN].BlockSY+1;
    if (PaperLen+BlockHigh>MaxPaper ) {
        HSayMes ("¢…¬áˆa á¢ ‹©´á»³“¡”a.");
        return ;
    }
    for (BlockSize=0,i=Task[TaskN].BlockSY;i<=Task[TaskN].BlockEY;i++)
        BlockSize+=16*((word)(Task[TaskN].Paper[i][0]+16)/16 +1);
    if (farcoreleft()<BlockSize+MaxBufRest) {
        BufErr();
        return;
    }
    memmove (Paper+NowLine+BlockHigh,Paper+NowLine,(PaperLen-NowLine+1)*sizeof(*Paper));
    PaperLen=PaperLen+BlockHigh;
    for (i=0;i<BlockHigh;i++) {
        Paper[NowLine+i]=NULL;
        PutPaper(NowLine+i,Task[TaskN].Paper[Task[TaskN].BlockSY+i]+1);
    }
    BlockSY=NowLine;
    BlockEY=BlockSY+BlockHigh-1;
    BlockType=1;
    ShowBlockF=True;
    SetEditBuf();
    SetScnF=True;
    SelectingBlockF=False;
}

void GetBlock2(word TaskN)
{
    word i,j,BlockHigh,S,E,iSX,iEX,iSY,iEY,HeadF,TailF;
    byte **iPaper;

    if (NowTask==TaskN) {
       CopyBlock();
       return;
    }
    iSX=Task[TaskN].BlockSX;
    iEX=Task[TaskN].BlockEX;
    iSY=Task[TaskN].BlockSY;
    iEY=Task[TaskN].BlockEY;
    iPaper=Task[TaskN].Paper;
    if (PutEditBuf()!=OK) return;
    if (!Task[TaskN].ShowBlockF) return;
    BlockHigh=iEY-iSY+1;
    j=iEX-iSX+3;
    if ( NowLine+BlockHigh-1>MaxPaper ||
         BlockHigh*(j+10)+MaxBufRest>farcoreleft()) {
         HSayMes ("¸b´ó µwµb·¡ ¦¹¢Ðs“¡”a.");
         return;
    }
    for (i=0; i<BlockHigh ; i++)
        if (Paper[NowLine+i][0]+j+1>254) {
            HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
            return;
        }
    while (NowLine+BlockHigh-1>PaperLen) InsertBlankLine(PaperLen);
    HeadF=TailF=False;
    for (i=iSY ; i<=iEY ; i++) {
         SetUndoBuf(iPaper[i]+1);
         if (UndoType[iSX]==2) HeadF=True;
         if (UndoType[iEX]==1) TailF=True;
         if (HeadF && TailF) break;
    }
    for (i=0 ; i<BlockHigh ; i++) {

         S=iSX;
         E=iEX;
         SetUndoBuf(iPaper[iSY+i]+1);
         MemSetB(EditBuf,0,256);
         if (UndoType[S]==2) S--;
         else if (HeadF) EditBuf[1]=' ';
         if (UndoType[E]==1) E++;
         strncat(EditBuf+1,UndoBuf+S,E-S+1);
         if (E==iEX && TailF) strcat(EditBuf+1," ");

         SetAuxBuf(NowLine+i);
         MemSetB(UndoBuf,0,256);
         j=CurXPos();
         if (AuxType[j]==2) j--;
         strncpy(UndoBuf+1,AuxBuf+1,j-1);
         if (j!=CurXPos()) strcat (UndoBuf+1," ");
         strcat (UndoBuf+1,EditBuf+1);
         if (Paper[NowLine+i][0]>=j) strcat (UndoBuf+1,Paper[NowLine+i]+j);
         PutPaper(NowLine+i,UndoBuf+1);
    }
    iSX-=HeadF;
    iEX+=TailF;
    BlockEX=iEX-iSX;
    BlockSX=CurXPos();
    BlockEX+=BlockSX;
    BlockEY=iEY-iSY;
    BlockSY=NowLine;
    BlockEY+=BlockSY;
    BlockType=2;
    ShowBlockF=True;
    SetEditBuf();
    SetScnF=True;
    SelectingBlockF=False;
}

void GetBlock(word TaskN)
{
    switch (Task[TaskN].BlockType) {
      case 0 :GetBlock0(TaskN); break;
      case 1 :GetBlock1(TaskN); break;
      case 2 :GetBlock2(TaskN);
    }
}

void SendBlock(word TaskN)
{
    word TaskBuf;

    TaskBuf=NowTask;
    if (NowTask==TaskN) return;
    ChangeTasks(TaskN);
    EditedF=True;
    InsertBlankLine(PaperLen);
    SetCurPos(1,PaperLen);
    switch (Task[TaskBuf].BlockType) {
      case 0 :GetBlock0(TaskBuf); break;
      case 1 :GetBlock1(TaskBuf); break;
      case 2 :GetBlock2(TaskBuf);
    }
    ChangeTasks(TaskBuf);
}

void DeleteBlock0(void)
{
    word i,j,BlockHigh;

    if (PutEditBuf()!=OK) return;
    if (! ShowBlockF) return;

    if (BlockSY!=BlockEY) {

        /* BlockStart Line */

        if (BlockSX<=Paper[BlockSY][0]) {
             SaveKillLine(Paper[BlockSY]+BlockSX);
             strncpy(AuxBuf,Paper[BlockSY]+1,BlockSX-1);
             AuxBuf[BlockSX-1]=0;
        }
        else {
             SaveKillLine("");
             strcpy(AuxBuf,Paper[BlockSY]+1);
             AppendSpace(AuxBuf,BlockSX-1);
        }

        /* Append BlockEL gabege */


        if (BlockEX<Paper[BlockEY][0]) {
            i=BlockEX;
            if (Paper[BlockEY][BlockEX]>127) i++;
            strcat(AuxBuf,Paper[BlockEY]+i+1);
        }
        PutPaper(BlockSY,AuxBuf);

        /* Mid */

        BlockHigh=BlockEY-BlockSY+1;
        i=BlockSY+1;
        while (i<BlockEY) {
          SaveKillLine(Paper[i]+1);
          farfree(Paper[i]);
          i++;
        }

        /* Block EL */

        if (Paper[BlockEY][0]>=BlockEX) {
            i=BlockEX;
            if (Paper[BlockEY][BlockEX]>127) i++;
            strncpy(AuxBuf,Paper[BlockEY]+1,i);
            AuxBuf[i]=0;
        } else strcpy(AuxBuf,Paper[BlockEY]+1);
        SaveKillLine(AuxBuf);
        farfree(Paper[BlockEY]);

        BlockHigh--;
        memmove (Paper+BlockEY-BlockHigh+1,Paper+BlockEY+1,sizeof(*Paper)*(PaperLen-BlockEY));
        PaperLen-=BlockHigh;
        MemSetB (Paper+PaperLen+1,0,BlockHigh*sizeof(*Paper));
        SetEditBuf();
        if (NowLine>BlockEY) SetCurPos(CurXPos(),NowLine-BlockHigh);
        else if (NowLine==BlockEY && CurXPos()>BlockEX) SetCurPos(BlockSX-1+(CurXPos()-i),BlockSY);
        else if (NowLine>=BlockSY && CurXPos()>BlockSX) SetCurPos(BlockSX,BlockSY);
    } else {
        if (BlockEX<=Paper[BlockEY][0]) {
            i=BlockEX;
            if (Paper[BlockEY][BlockEX]>127) i++;
            strncpy (AuxBuf,Paper[BlockSY]+BlockSX,i-BlockSX+1);
            AuxBuf[i-BlockSX+1]=0;
            SaveKillLine(AuxBuf);
            strncpy (AuxBuf,Paper[BlockSY]+1,BlockSX-1);
            AuxBuf[BlockSX-1]=0;
            strcat  (AuxBuf,Paper[BlockEY]+i+1);
            PutPaper(BlockSY,AuxBuf);
        } else if (BlockSX<=Paper[BlockSY][0]) {
            SaveKillLine(Paper[BlockSY]+BlockSX);
            strncpy(AuxBuf,Paper[BlockSY]+1,BlockSX-1);
            AuxBuf[BlockSX-1]=0;
            PutPaper(BlockSY,AuxBuf);
        }
        if (NowLine==BlockSY) {
            j=CurXPos();
            if (j>BlockEX) j-=i-BlockSX+1;
            else if (j>BlockSX) j=BlockSX;
            SetCurPos(j,NowLine);
        }
        SetEditBuf();
    }
    BlockSX=BlockSY=BlockEX=BlockEY=0;
    ShowBlockF=False;
    SetScnF=True;
    SelectingBlockF=False;
}

void DeleteBlock1(void)
{
    word i,j,BlockHigh;

    if (PutEditBuf()!=OK) return;
    if (! ShowBlockF) return;
    BlockHigh=BlockEY-BlockSY+1;
    for (i=BlockSY; i<=BlockEY; i++) {
      SaveKillLine(Paper[i]+1);
      farfree(Paper[i]);
    }
    memmove (Paper+BlockEY-BlockHigh+1,Paper+BlockEY+1,sizeof(*Paper)*(PaperLen-BlockEY));
    for (i=PaperLen-BlockHigh+1; i<=PaperLen ;i++) Paper[i]=NULL;
    PaperLen=PaperLen-BlockHigh;
    if (PaperLen==0) PutPaper(++PaperLen,"");
    ShowBlockF=False;
    if (NowLine>=BlockSY) {
         SetEditBuf();
           if (NowLine<=BlockEY)
            if (BlockSY>PaperLen) SetCurPos (CurXPos(),PaperLen);
            else                  SetCurPos (CurXPos(),BlockSY);
         else SetCurPos (CurXPos(),NowLine-BlockHigh);
    }
    BlockSY=0;
    BlockEY=0;
    SetScnF=True;
    SelectingBlockF=False;
}

void DeleteBlock2(void)
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
           strncpy(AuxBuf,Paper[i]+1,S-1);
           AuxBuf[S-1]=0;
           strcat(AuxBuf,Paper[i]+E+1);
           strncpy(EditBuf,Paper[i]+S,E-S+1);
           EditBuf[E-S+1]=0;
           SaveKillLine(EditBuf);
           PutPaper(i,AuxBuf);
        } else if (BlockSX<=Paper[i][0]) {
           S=BlockSX;
           SetAuxBuf(i);
           if (AuxType[S]==2) S--;
           strncpy(AuxBuf,Paper[i]+1,S-1);
           AuxBuf[S-1]=0;
           SaveKillLine(Paper[i]+S);
           PutPaper(i,AuxBuf);
        }
    }
    BlockSX=BlockSY=BlockEX=BlockEY=0;
    ShowBlockF=False;
    SetScnF=True;
    SelectingBlockF=False;
    SetEditBuf();
}

void DeleteBlock(void)
{
    switch (BlockType) {
      case 0 :DeleteBlock0(); break;
      case 1 :DeleteBlock1(); break;
      case 2 :DeleteBlock2();
    }
}

void MoveBlock0(void)
{
    word i,j,k,l;

    if (PutEditBuf()!=OK) return;
    if (! ShowBlockF) return;
    i=BlockEX;
    if (Paper[BlockEY][0]>BlockEX && Paper[BlockEY][BlockEX]>127) i++;

    if (BlockSY< NowLine && BlockEY>NowLine) return;
    if (BlockSY==NowLine) {
       if (BlockSY==BlockEY) {
            if (CurXPos()>=BlockSX && CurXPos()<=i+1) return;
       } else
       if (CurXPos()>=BlockSX) return;
    } else if (BlockEY==NowLine && CurXPos()<=i+1) return;

    if (BlockSY==BlockEY) {
        if (BlockSY==NowLine) {
            if (CurXPos()>BlockEX) {
                strcpy(EditBuf+1,Paper[NowLine]+1);
                AppendSpace(EditBuf+1,CurXPos()-1);

                strncpy(AuxBuf,EditBuf+BlockSX,i-BlockSX+1);
                strncpy(EditBuf+BlockSX,EditBuf+i+1,CurXPos()-i-1);
                strncpy(EditBuf+(k=BlockSX+CurXPos()-i-1),AuxBuf,i-BlockSX+1);

                l=BlockEX-BlockSX;
                BlockSX=k;
                BlockEX=k+l;
                PutPaper(NowLine,EditBuf+1);
                SetCurPos(BlockSX,NowLine);
                SetEditBuf();
            } else {
                strcpy(EditBuf+1,Paper[NowLine]+1);
                AppendSpace(EditBuf+1,BlockEX);

                strncpy(AuxBuf,EditBuf+CurXPos(),BlockSX-CurXPos());
                strncpy(EditBuf+CurXPos(),EditBuf+BlockSX,i-BlockSX+1);
                strncpy(EditBuf+CurXPos()+i-BlockSX+1,AuxBuf,BlockSX-CurXPos());

                BlockEX-=BlockSX;
                BlockSX =CurXPos();
                BlockEX+=BlockSX;
                PutPaper(NowLine,EditBuf+1);
                SetEditBuf();
            }
        } else if (i==254 ) {
           word RealBlockLen;

           if (Paper[BlockSY][0]>=BlockSX) {
              strcpy(AuxBuf,Paper[BlockSY]+BlockSX);
              RealBlockLen=strlen(AuxBuf);
           } else {
              AuxBuf[0]=0;
              RealBlockLen=0;
           }

           if (Paper[NowLine][0]>=CurXPos()) {
              if ( RealBlockLen+Paper[NowLine][0] >254) return;
           } else {
              if ( RealBlockLen+CurXPos() >254 ) return;
           }

           if (PaperLen+1>MaxPaper) return ;

           strcpy(EditBuf,Paper[BlockSY]+1);
           EditBuf[BlockSX-1]=0;
           PutPaper(BlockSY,EditBuf);

           if (Paper[NowLine][0]>=CurXPos()) {
               strncpy(EditBuf,Paper[NowLine]+1,CurXPos()-1);
               EditBuf[CurXPos()-1]=0;
               strcat(EditBuf,AuxBuf);
               strcpy(AuxBuf,Paper[NowLine]+CurXPos());
           } else {
               strcpy(EditBuf,Paper[NowLine]+1);
               AppendSpace(EditBuf,CurXPos()-1);
               strcat(EditBuf,AuxBuf);
               AuxBuf[0]=0;
           }

           memmove (Paper+NowLine+1,Paper+NowLine,sizeof(*Paper)*(PaperLen-NowLine+1));
           PaperLen++;
           Paper[NowLine]=NULL;
           PutPaper (NowLine,EditBuf);
           PutPaper (NowLine+1,AuxBuf);

           //BlockEX-=BlockSX;
           BlockSX =CurXPos();
           //BlockEX+=BlockSX;
           BlockSY=BlockEY=NowLine;
           SetEditBuf();
        } else {
           word RealBlockLen;

           strcpy(EditBuf+1,Paper[BlockSY]+1);
           AppendSpace(EditBuf+1,BlockEX);

           strncpy (AuxBuf,EditBuf+BlockSX,RealBlockLen=i-BlockSX+1);
           AuxBuf[i-BlockSX+1]=0;

           if (Paper[NowLine][0]>=CurXPos()) {
              if ( RealBlockLen+Paper[NowLine][0] >254) return;
           } else {
              if ( RealBlockLen+CurXPos() >254 ) return;
           }

           strcpy (EditBuf+BlockSX,EditBuf+i+1);
           PutPaper(BlockSY,EditBuf+1);

           if (Paper[NowLine][0]>=CurXPos()) {
               strncpy(EditBuf,Paper[NowLine]+1,CurXPos()-1);
               EditBuf[CurXPos()-1]=0;
               strcat(EditBuf,AuxBuf);
               strcat(EditBuf,Paper[NowLine]+CurXPos());
           } else {
               strcpy(EditBuf,Paper[NowLine]+1);
               AppendSpace(EditBuf,CurXPos()-1);
               strcat(EditBuf,AuxBuf);
           }
           PutPaper(NowLine,EditBuf);
           BlockEX-=BlockSX;
           BlockSX =CurXPos();
           BlockEX+=BlockSX;
           BlockSY=BlockEY=NowLine;
           SetEditBuf();
        }
        SetScnF=True;
        SelectingBlockF=False;
        return;
    }

/*    if (BlockSY==BlockEY) {
        if (BlockSY==NowLine) {
            if (CurXPos()>BlockEX) {
                strcpy(EditBuf+1,Paper[NowLine]+1);
                AppendSpace(EditBuf+1,CurXPos()-1);

                strncpy(AuxBuf,EditBuf+BlockSX,i-BlockSX+1);
                strncpy(EditBuf+BlockSX,EditBuf+i+1,CurXPos()-i-1);
                strncpy(EditBuf+(k=BlockSX+CurXPos()-i-1),AuxBuf,i-BlockSX+1);

                l=BlockEX-BlockSX;
                BlockSX=k;
                BlockEX=k+l;
                PutPaper(NowLine,EditBuf+1);
                SetCurPos(BlockSX,NowLine);
                SetEditBuf();
            } else {
                strcpy(EditBuf+1,Paper[NowLine]+1);
                AppendSpace(EditBuf+1,BlockEX);

                strncpy(AuxBuf,EditBuf+CurXPos(),BlockSX-CurXPos());
                strncpy(EditBuf+CurXPos(),EditBuf+BlockSX,i-BlockSX+1);
                strncpy(EditBuf+CurXPos()+i-BlockSX+1,AuxBuf,BlockSX-CurXPos());

                BlockEX-=BlockSX;
                BlockSX =CurXPos();
                BlockEX+=BlockSX;
                PutPaper(NowLine,EditBuf+1);
                SetEditBuf();
            }
        } else {
           strcpy(EditBuf+1,Paper[BlockSY]+1);
           AppendSpace(EditBuf+1,BlockEX);

           strncpy (AuxBuf,EditBuf+BlockSX,i-BlockSX+1);
           AuxBuf[i-BlockSX+1]=0;

           strcpy (EditBuf+BlockSX,EditBuf+i+1);
           PutPaper(BlockSY,EditBuf+1);

           if (Paper[NowLine][0]>=CurXPos()) {
               strncpy(EditBuf,Paper[NowLine]+1,CurXPos()-1);
               EditBuf[CurXPos()-1]=0;
               strcat(EditBuf,AuxBuf);
               strcat(EditBuf,Paper[NowLine]+CurXPos());
           } else {
               strcpy(EditBuf,Paper[NowLine]+1);
               AppendSpace(EditBuf,CurXPos()-1);
               strcat(EditBuf,AuxBuf);
           }
           PutPaper(NowLine,EditBuf);
           BlockEX-=BlockSX;
           BlockSX =CurXPos();
           BlockEX+=BlockSX;
           BlockSY=BlockEY=NowLine;
           SetEditBuf();
        }
        SetScnF=True;
        SelectingBlockF=False;
        return;
    }
*/
    if (i<254) {
       if (BlockSY==NowLine) {
           if (Paper[BlockEY][0]+(i-BlockSX+1)>254) {
               HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
               return;
           }

           strcpy(EditBuf+1,Paper[BlockSY]+1);
           AppendSpace(EditBuf+1,BlockSX-1);

           strncpy(AuxBuf,EditBuf+CurXPos(),BlockSX-CurXPos());
           AuxBuf[BlockSX-CurXPos()]=0;

           strcpy(EditBuf+CurXPos(),EditBuf+BlockSX);
           PutPaper(BlockSY,EditBuf+1);

           strcpy(EditBuf+1,Paper[BlockEY]+1);
           AppendSpace(EditBuf+1,i);

           strcpy(UndoBuf,EditBuf+i+1);
           strcpy(EditBuf+i+1,AuxBuf);
           strcat(EditBuf+i+1,UndoBuf);
           PutPaper(BlockEY,EditBuf+1);

           BlockSX=CurXPos();
           SetEditBuf();
       } else if (BlockEY==NowLine) {
           if (Paper[BlockSY][0]+(CurXPos()-i-1)>254) {
               HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
               return;
           }

           strcpy(EditBuf+1,Paper[BlockEY]+1);
           AppendSpace(EditBuf+1,CurXPos()-1);

           strncpy(AuxBuf,EditBuf+i+1,CurXPos()-i-1);
           AuxBuf[CurXPos()-i-1]=0;

           strcpy(EditBuf+i+1,EditBuf+CurXPos());
           PutPaper(BlockEY,EditBuf+1);

           strcpy(EditBuf+1,Paper[BlockSY]+1);
           AppendSpace(EditBuf+1,BlockSX-1);

           strcpy(UndoBuf,EditBuf+BlockSX);
           strcpy(EditBuf+BlockSX,AuxBuf);
           BlockSX=strlen(EditBuf+1)+1;
           strcat(EditBuf+BlockSX,UndoBuf);
           PutPaper(BlockSY,EditBuf+1);

           SetEditBuf();
           SetCurPos(BlockSX,BlockSY);
       } else if (NowLine>BlockEY) {
           j=BlockEY-BlockSY;

           if ((Paper[BlockEY][0]>i&&BlockSX-1+(Paper[BlockEY][0]-i)>254)||
               (Paper[BlockSY][0]>=BlockSX&&Paper[BlockSY][0]-BlockSX+1+CurXPos()-1>254)||
               (Paper[NowLine][0]>=CurXPos()&&Paper[NowLine][0]-CurXPos()+1+i>254)) {
                  HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
                  return;
              }
           if (PaperLen+j>MaxPaper) {
               HSayMes("¸b´ó µwµb·¡ ¦¹¢Ðs“¡”a.");
               return;
           }
           strcpy(UndoBuf+1,Paper[NowLine]+1);
           AppendSpace(UndoBuf+1,CurXPos()-1);
           farfree(Paper[NowLine]);

           memmove(Paper+PaperLen+1,Paper+BlockSY+1,sizeof(*Paper)*j);
           memmove(Paper+BlockSY+1,Paper+BlockEY+1,sizeof(*Paper)*(NowLine-1-BlockEY));
           memmove(Paper+NowLine-j+1,Paper+PaperLen+1,sizeof(*Paper)*j);
           MemSetB(Paper+PaperLen+1,0,sizeof(*Paper)*j);

           strcpy(EditBuf+1,Paper[BlockSY]+1);
           AppendSpace(EditBuf+1,BlockSX-1);

           strcpy(AuxBuf,EditBuf+BlockSX);

           if (Paper[NowLine][0]>i) strcpy(EditBuf+BlockSX,Paper[NowLine]+i+1);
           else                     EditBuf[BlockSX]=0;
           PutPaper(BlockSY,EditBuf+1);

           strcpy(EditBuf+1,Paper[NowLine]+1);
           AppendSpace(EditBuf+1,i);

           strcpy(EditBuf+i+1,UndoBuf+CurXPos());
           PutPaper(NowLine,EditBuf+1);

           BlockSY=BlockEY-BlockSY;
           BlockEY=NowLine;
           BlockSY=BlockEY-BlockSY;
           BlockSX=CurXPos();

           strcpy(UndoBuf+CurXPos(),AuxBuf);
           Paper[BlockSY]=NULL;
           PutPaper(BlockSY,UndoBuf+1);

           SetEditBuf();
           SetCurPos(BlockSX,BlockSY);
       } else if (BlockSY>NowLine) {

           j=BlockEY-BlockSY;
           k=BlockSY-NowLine-1;
           if ( ( (Paper[BlockEY][0]>i) &&
                  (BlockSX-1+(Paper[BlockEY][0]-i)>254)
                ) ||
                ( (Paper[BlockSY][0]>=BlockSX) &&
                  (CurXPos()+(Paper[BlockSY][0]-BlockSX+1)>254)
                ) ||
                ( (Paper[NowLine][0]>=CurXPos()) &&
                  (Paper[NowLine][0]-CurXPos()+1+i>254)
                )
               ) {
                   HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
                   return;
               }
           if (PaperLen+j>MaxPaper) {
               HSayMes("¸b´ó µwµb·¡ ¦¹¢Ðs“¡”a.");
               return;
           }
           strcpy(UndoBuf+1,Paper[NowLine]+1);
           AppendSpace(UndoBuf+1,CurXPos()-1);
           farfree(Paper[NowLine]);

           memmove(Paper+PaperLen+1,Paper+BlockSY,sizeof(*Paper)*j);
           memmove(Paper+BlockEY-k,Paper+NowLine+1,sizeof(*Paper)*k);
           memmove(Paper+NowLine,Paper+PaperLen+1,sizeof(*Paper)*j);
           MemSetB(Paper+PaperLen+1,0,sizeof(*Paper)*j);

           strcpy(EditBuf+1,Paper[NowLine]+1);
           AppendSpace(EditBuf+1,BlockSX-1);
           strcpy(AuxBuf+1 ,Paper[BlockEY]+1);
           AppendSpace(AuxBuf+1,i);
           strcpy(EditBuf+BlockSX,AuxBuf+i+1);
           AuxBuf[i+1]=0;
           PutPaper(BlockEY,EditBuf+1);

           BlockEY-=BlockSY;
           BlockSY =NowLine;
           BlockEY+=BlockSY;

           strcpy(AuxBuf+i+1,UndoBuf+CurXPos());
           Paper[BlockEY]=NULL;
           PutPaper(BlockEY,AuxBuf+1);

           if (Paper[NowLine][0]>BlockSX) strcpy(UndoBuf+CurXPos(),Paper[NowLine]+BlockSX);
           else                           UndoBuf[CurXPos()]=0;
           PutPaper(NowLine,UndoBuf+1);

           BlockSX =CurXPos();

           SetEditBuf();
           SetCurPos(BlockSX,BlockSY);
       }
       SetScnF=True;
       SelectingBlockF=False;
       return;
    }

       /* if i==254 */

       if (BlockSY==NowLine) {
           if (BlockEY==PaperLen)
             if (InsertBlankLine(PaperLen)!=OK) return;
           if (Paper[BlockEY+1][0]+(BlockSX-CurXPos())>254) {
               HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
               return;
           }

           strcpy(EditBuf+1,Paper[BlockSY]+1);
           AppendSpace(EditBuf+1,BlockSX-1);

           strncpy(AuxBuf,EditBuf+CurXPos(),BlockSX-CurXPos());
           AuxBuf[BlockSX-CurXPos()]=0;

           strcpy(EditBuf+CurXPos(),EditBuf+BlockSX);
           PutPaper(BlockSY,EditBuf+1);

           strcat(AuxBuf,Paper[BlockEY+1]+1);
           PutPaper(BlockEY+1,AuxBuf);

           BlockSX=CurXPos();
           SetEditBuf();
       } else if (BlockEY+1==NowLine) {
           if (Paper[BlockSY][0]+(CurXPos()-1)>254) {
               HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
               return;
           }

           strcpy(EditBuf+1,Paper[BlockEY+1]+1);
           AppendSpace(EditBuf+1,CurXPos()-1);
           PutPaper(BlockEY+1,EditBuf+CurXPos());
           EditBuf[CurXPos()]=0;

           strcpy(UndoBuf+1,Paper[BlockSY]+1);
           AppendSpace(UndoBuf+1,BlockSX-1);
           strncpy(AuxBuf,UndoBuf+1,BlockSX-1);
           AuxBuf[BlockSX-1]=0;
           strcat(AuxBuf,EditBuf+1);
           strcat(AuxBuf,UndoBuf+BlockSX);
           PutPaper(BlockSY,AuxBuf);

           BlockSX=BlockSX+CurXPos()-1;

           SetEditBuf();
           SetCurPos(BlockSX,BlockSY);
       } else if (NowLine>BlockEY) {
           j=BlockEY-BlockSY;

           if ((Paper[BlockSY][0]>=BlockSX&&Paper[BlockSY][0]-BlockSX+1+CurXPos()-1>254)||
               (Paper[BlockEY+1][0]+BlockSX-1>254)) {
                  HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
                  return;
              }
           if (PaperLen+j>MaxPaper) {
               HSayMes("¸b´ó µwµb·¡ ¦¹¢Ðs“¡”a.");
               return;
           }

           strcpy(UndoBuf+1,Paper[NowLine]+1);
           AppendSpace(UndoBuf+1,CurXPos()-1);
           PutPaper(NowLine,UndoBuf+CurXPos());

           strcpy(AuxBuf+1,Paper[BlockSY]+1);
           AppendSpace(AuxBuf+1,BlockSX-1);
           strcpy(UndoBuf+CurXPos(),AuxBuf+BlockSX);
           strcpy(AuxBuf+BlockSX,Paper[BlockEY+1]+1);
           PutPaper(BlockSY,AuxBuf+1);
           farfree(Paper[BlockEY+1]);

           memmove(Paper+PaperLen+1,Paper+BlockSY+1,sizeof(*Paper)*j);
           memmove(Paper+BlockSY+1,Paper+BlockEY+2,sizeof(*Paper)*(NowLine-2-BlockEY));
           memmove(Paper+NowLine-j,Paper+PaperLen+1,sizeof(*Paper)*j);
           MemSetB(Paper+PaperLen+1,0,sizeof(*Paper)*j);
           Paper[NowLine-j-1]=NULL;
           PutPaper(NowLine-j-1,UndoBuf+1);

           BlockEY=NowLine-1;
           BlockSY=BlockEY-j;
           BlockSX=CurXPos();

           SetEditBuf();
           SetCurPos(BlockSX,BlockSY);
       } else if (BlockSY>NowLine) {

           if (BlockEY==PaperLen)
             if (InsertBlankLine(BlockEY)!=OK) return;
           j=BlockEY-BlockSY;
           if ( (Paper[BlockSY][0]>=BlockSX&&CurXPos()-1+Paper[BlockSY][0]-BlockSX+1>254)||
                (BlockSX-1+Paper[BlockEY+1][0]>254) )
                {
                   HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
                   return;
               }
           if (PaperLen+j>MaxPaper) {
               HSayMes("¸b´ó µwµb·¡ ¦¹¢Ðs“¡”a.");
               return;
           }

           strcpy(EditBuf+1,Paper[NowLine]+1);
           AppendSpace(EditBuf+1,CurXPos()-1);
           strcpy(AuxBuf+1,Paper[BlockSY]+1);
           AppendSpace(AuxBuf+1,BlockSX-1);
           strcpy(UndoBuf,EditBuf+CurXPos());
           strcpy(EditBuf+CurXPos(),AuxBuf+BlockSX);
           strcpy(AuxBuf+BlockSX,Paper[BlockEY+1]+1);
           PutPaper(NowLine,EditBuf+1);
           PutPaper(BlockEY+1,AuxBuf+1);

           memmove (Paper+PaperLen+1,Paper+NowLine+1,sizeof(*Paper)*(BlockSY-NowLine-1));
           memmove (Paper+NowLine+1,Paper+BlockSY+1,sizeof(*Paper)*j);
           memmove (Paper+NowLine+j+2,Paper+PaperLen+1,sizeof(*Paper)*(BlockSY-NowLine-1));
           MemSetB (Paper+PaperLen+1,0,sizeof(*Paper)*(BlockSY-NowLine-1));
           Paper[NowLine+j+1]=NULL;
           PutPaper(NowLine+j+1,UndoBuf);

           BlockSY=NowLine;
           BlockSX=CurXPos();
           BlockEY=BlockSY+j;
           SetEditBuf();
           SetCurPos(BlockSX,BlockSY);
       }
       SetScnF=True;
       SelectingBlockF=False;
}

void MoveBlock1(void)
{
    byte *BufRest[21];
    word j,is,ie,ibase;

    if (PutEditBuf()!=OK) return;
    if (!ShowBlockF) return;
    if (NowLine>=BlockSY-1 && NowLine<=BlockEY) return;
    if (NowLine<BlockSY) is=BlockSY,ie=BlockEY,ibase=NowLine+1;
    else                 is=BlockEY+1,ie=NowLine,ibase=BlockSY;
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
    if (NowLine<BlockSY) BlockSY=NowLine+1,BlockEY=ibase-1;
    else                 BlockEY=NowLine,BlockSY=ibase;
    SetEditBuf();
    SetCurPos(CurXPos(),BlockSY-1);

}

void MoveBlock2(void)
{
    word i,j,S,E,HeadF,TailF;

    if (PutEditBuf()!=OK) return;
    if (!ShowBlockF) return;
    j=BlockEY-BlockSY+1;
    if ( (j+PaperLen>MaxPaper) ||
         j*(BlockEX-BlockSX+13)+MaxBufRest>farcoreleft()) {
        HSayMes("¸b´ó µwµb·¡ ¦¹¢Ðs“¡”a.");
        return;
    }
    for (i=0; i<j ; i++)
        if (Paper[NowLine+i][0]+(BlockEX-BlockSX+4)>254) {
            HSayMes("á¢ ‹¥ º‰·¡ ¬—‹³“¡”a.");
            return;
        }

    HeadF=TailF=False;
    for (i=BlockSY; i<=BlockEY; i++) {
         SetAuxBuf(i);
         if (AuxType[BlockSX]==2) HeadF=True;
         if (AuxType[BlockEX]==1) TailF=True;
         if (HeadF && TailF) break;
    }
    while (NowLine+j-1>PaperLen) InsertBlankLine(PaperLen);
    for (i=BlockSY,j=1; i<=BlockEY ; i++,j++) {
         SetAuxBuf(i);
         MemSetB(EditBuf,0,256);
         S=BlockSX;
         E=BlockEX;
         if (AuxType[S]==2) S--;
         else if (HeadF) EditBuf[1]=' ';
         if (AuxType[E]==1) E++;
         strncat(EditBuf+1,AuxBuf+S,E-S+1);
         if (TailF && E==BlockEX) strcat(EditBuf+1," ");
         if (Paper[i][0]>E) strcpy (AuxBuf+S,Paper[i]+E+1);
         else               AuxBuf[S]=0;
         PutPaper(i,AuxBuf+1);
         PutPaper(PaperLen+j,EditBuf+1);
    }
    if (HeadF) BlockSX--;
    if (TailF) BlockEX++;
    BlockEY-=BlockSY;
    BlockSY =NowLine;
    BlockEY+=BlockSY;
    BlockEX-=BlockSX;
    BlockSX =CurXPos();
    BlockEX+=BlockSX;
    for (i=BlockSY,j=1; i<=BlockEY ; i++,j++) {
         SetAuxBuf(i);
         MemSetB(EditBuf,0,256);
         S=CurXPos();
         if (AuxType[S]==2) S--;
         strncpy(EditBuf+1,AuxBuf+1,S-1);
         if (S!=CurXPos()) strcat (EditBuf+1," ");
         strcat(EditBuf+S,Paper[PaperLen+j]+1);
         if (Paper[i][0]>=S) strcat(EditBuf+S,Paper[i]+S);
         PutPaper(i,EditBuf+1);
         farfree(Paper[PaperLen+j]);
         Paper[PaperLen+j]=NULL;
    }
    SetEditBuf();
    SetScnF=True;
    SelectingBlockF=False;
    SetCurPos(BlockSX,BlockSY);
}

void MoveBlock(void)
{
    switch (BlockType) {
      case 0 :MoveBlock0(); break;
      case 1 :MoveBlock1(); break;
      case 2 :MoveBlock2();
    }
}

void SwapBlockPointer(void)
{
    word temp;

    temp=BlockSY;
    BlockSY=BlockEY;
    BlockEY=temp;
}

void EditCKB(void)
{
    if (BlockType!=0) {
        if (BlockType==1) BlockEX=254;
        BlockType=0;
    }
    BlockSY=NowLine;
    BlockSX=CurXPos();
    if (BlockSY>BlockEY  ||  (BlockSY==BlockEY && BlockSX>BlockEX))
         ShowBlockF=False;
    else ShowBlockF=True;
    SetScnF=True;
    SelectingBlockF=False;
}

void EditCKK(void)
{
    if (BlockType!=0) {
        if (BlockType==1) BlockSX=1;
        BlockType=0;
    }
    if (CurXPos()==1) {
         BlockEY=NowLine-1;
         BlockEX=254;
    } else {
         BlockEY=NowLine;
         if (EditType[CurXPos()-1]==2) BlockEX=CurXPos()-2;
         else                          BlockEX=CurXPos()-1;
    }
    if (BlockSY==0) BlockSY=BlockSX=1;
    if (BlockSY>BlockEY  ||  (BlockSY==BlockEY && BlockSX>BlockEX) )
         ShowBlockF=False;
    else ShowBlockF=True;
    SetScnF=True;
    SelectingBlockF=False;
}

void EditAA(void)
{
    if (BlockType==0 && SelectingBlockF) SelectingBlockF=False;
    else {
       BlockType=0;
       SelectingBlockF=True;
       BlockSY=BlockEY=BlockHeadY=NowLine;
       BlockSX=BlockEX=BlockHeadX=CurXPos();
       ShowBlockF=True;
       SetScnF=True;
    }
}
void EditAK(void)
{
    if (BlockType==2 && SelectingBlockF) SelectingBlockF=False;
    else {
       BlockType=2;
       SelectingBlockF=True;
       BlockSY=BlockEY=BlockHeadY=NowLine;
       BlockSX=BlockEX=BlockHeadX=CurXPos();
       ShowBlockF=True;
       SetScnF=True;
    }
}

void EditAL(void)
{
    if (BlockType==1 && SelectingBlockF) SelectingBlockF=False;
    else {
       BlockType=1;
       SelectingBlockF=True;
       BlockSY=BlockEY=BlockHeadY=NowLine;
       ShowBlockF=True;
       SetScnF=True;
    }
}

void UnselectBlock(void)
{
     ShowBlockF=False;
     BlockSX=BlockEX=BlockSY=BlockEY=0;
     SelectingBlockF=False;
     SetScnF=True;
}

void CancelSelectBlock(void)
{
     if (SelectingBlockF) UnselectBlock();
}

void CheckBlock0(void)
{
     if (BlockType!=0) return;
     if ( BlockSY>BlockEY || (BlockSY==BlockEY && BlockSX>BlockEX))
         UnselectBlock();
}

void EditCKT(void)
{
    BlockSY=BlockEY=NowLine;
    BlockSX=BlockEX=0;
    BlockType=1;
    ShowBlockF=True;
    SelectingBlockF=False;
    SetScnF=True;
}

void EditCKR(void)
{
    word oldPaperLen;

    oldPaperLen=PaperLen;
    if (PutEditBuf()!=OK) return;
    LoadBlock();
    SetScnF=True;
    SelectingBlockF=False;
    if (oldPaperLen<PaperLen) {
        BlockSY=oldPaperLen+1;
        BlockEY=PaperLen;
        if (HanGulCodeType==1) {
          word i;

          HSayStatus(" Å¡—a ¥eÑÅ º—·³“¡”a. ");
          for (i=BlockSY ; i<=BlockEY ; i++)
            ToCombi(Paper[i]+1,Paper[i][0]);
          HCloseWindow();
        }
        BlockSX=1;
        SetAuxBuf(PaperLen);
        BlockEX=Paper[PaperLen][0];
        if (AuxType[BlockEX]==2) BlockEX--;
        BlockType=1;
        ShowBlockF=True;
        MoveBlock();
    }
}

void EditCKW(void)
{
   if (PutEditBuf()!=OK) return;
   if (!ShowBlockF) return;
   SaveBlock();
   SetEditBuf();
}

word AppendBlock(void);

void EditCKA(void)
{
   if (PutEditBuf()!=OK) return;
   if (!ShowBlockF) return;
   AppendBlock();
   SetEditBuf();
}


void EditCKH(void)
{
    if (BlockSY==0 || BlockEY==0) return;
    ShowBlockF=!ShowBlockF;
    SelectingBlockF=False;
    SetScnF=True;
}

void EditCKI(void)
{
    word i;

    AuxBuf[0]=0;
    if (!ShowBlockF) return;
    if (BlockType!=1) {
        BlockSX=BlockEX=0;
        BlockType=1;
    }
    if (PutEditBuf()!=OK) return;
    for (i=BlockSY;i<=BlockEY;i++)
        if (Paper[i][0]<256-2 && Paper[i][0]>0) {
           strcpy (AuxBuf," ");
           strcat (AuxBuf,Paper[i]+1);
           if (PutPaper(i,AuxBuf)!=OK) return;
        }
    SetEditBuf();
    SetCurPos(HX+SX-1,NowLine);
    SetScnF=True;
    SelectingBlockF=False;
}

void EditCKU(void)
{
    word i;

    AuxBuf[0]=0;
    if (!ShowBlockF) return;
    if (BlockType!=1) {
        BlockSX=BlockEX=0;
        BlockType=1;
    }
    if (PutEditBuf()!=OK) return;
    for (i=BlockSY;i<=BlockEY;i++)
        if (Paper[i][0]>0 && Paper[i][1]==' ') {
           strcpy (Paper[i]+1,Paper[i]+2);
           ResizePaper(i);
        }
    SetEditBuf();
    SetCurPos(HX+SX-1,NowLine);
    SetScnF=True;
    SelectingBlockF=False;
}

void EditCQB(void)
{
     if (BlockSY==0) return;
     if (BlockType==1) SetCurPos (1,BlockSY);
     else SetCurPos (BlockSX,BlockSY);
}

void EditCQK(void)
{
     if (BlockEY==0) return;
     if (BlockType==1) SetCurPos (1,BlockEY);
     else SetCurPos(BlockEX,BlockEY);
}
                
