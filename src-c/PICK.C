/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                         Pick                         */
/*                                                      */

#include "stddef.h"
#include "crt.h"
#include "memory.h"
#include "handrv.h"
#include "popup.h"
#include "const.h"
#include "edit.h"
#include "hwindow.h"
#include <dir.h>
#include <mem.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>

struct PickListEle (*PickList)[15];
word   PickFileF;

void RestorePickData(void)
{
     word i;

     i=0;
     while (i<15 && strcmp(WorkFileName,(*PickList)[i].Name)!=0) i++;
     if (i<15) {
        BlockSY=(*PickList)[i].BlockSY;
        BlockEY=(*PickList)[i].BlockEY;
        BlockSX=(*PickList)[i].BlockSX;
        BlockEX=(*PickList)[i].BlockEX;
        BlockType=(*PickList)[i].BlockType;
        ShowBlockF=(*PickList)[i].ShowBlockF;
        HX=(*PickList)[i].HX;
        HY=(*PickList)[i].HY;
        SX=(*PickList)[i].SX;
        SY=(*PickList)[i].SY;
        FoolSetCur(HX+SX-ScnLeft,HY+SY-ScnTop);
     }
    if (BlockSY>PaperLen || BlockEY>PaperLen) {
        BlockSY=BlockEY=BlockSX=BlockEX=0;
        ShowBlockF=False;
    }
}

void StorePickData(void)
{
     word i;

     i=0;
     while (i<15 && strcmp(WorkFileName,(*PickList)[i].Name)!=0) i++;

     if (i<15) {
        (*PickList)[i].BlockSY=BlockSY;
        (*PickList)[i].BlockEY=BlockEY;
        (*PickList)[i].BlockSX=BlockSX;
        (*PickList)[i].BlockEX=BlockEX;
        (*PickList)[i].BlockType=BlockType;
        (*PickList)[i].ShowBlockF=ShowBlockF;
        (*PickList)[i].HX=HX;
        (*PickList)[i].HY=HY;
        (*PickList)[i].SX=SX;
        (*PickList)[i].SY=SY;
     }
}

void OpenPickData(void)
{
     word i;
     struct PickListEle PickBuf;

     i=0;
     while (i<15 && stricmp(WorkFileName,(*PickList)[i].Name)!=0) i++;

     if (i==15) {
         PickBuf.BlockSY=0;
         PickBuf.BlockSX=0;
         PickBuf.BlockEY=0;
         PickBuf.BlockEX=0;
         PickBuf.BlockType=0;
         PickBuf.ShowBlockF=False;
         PickBuf.HX     =1;
         PickBuf.HY     =1;
         PickBuf.SX     =ScnLeft;
         PickBuf.SY     =ScnTop;
         strcpy(PickBuf.Name,WorkFileName);
     } else {
         MemCopy(&((*PickList)[i]),&PickBuf,sizeof(PickBuf));
         movmem(&((*PickList)[i+1]),&((*PickList)[i]),(14-i)*sizeof(PickBuf));
     }
     movmem(PickList,&((*PickList)[1]),14*sizeof(PickBuf));
     movmem(&PickBuf,PickList,sizeof(PickBuf));
}

word OpenPickList(void)
{
     int handler;
     byte name[80];
     int i;

     strcpy(name,SysPath);
     strcat(name,"san.pik");
     MemSetB(PickList,0,sizeof(*PickList));
     if ((handler=_open(name,O_RDONLY))==-1) return Error;
     if (filelength(handler)!=sizeof(*PickList)) {
          _close(handler);
          return Error;
     }
     if (_read(handler,PickList,sizeof(*PickList))==-1) {
         _close(handler);
         return Error;
     }
     _close(handler);
     return OK;
}

word ClosePickList(void)
{

     word i,j;
     int handler;
     byte name[80];

     j=NowTask;
     for (i=0; i<3 ; i++) {
          if (i==j) continue;
          ChangeTasks(i);
          if (FileNameSetF==False) continue;
          OpenPickData();
          StorePickData();
     }
     ChangeTasks(j);
     if (FileNameSetF) {
         OpenPickData();
         StorePickData();
     }
     strcpy(name,SysPath);
     strcat(name,"san.pik");
     if ((handler=_creat(name,FA_ARCH))==-1) return Error;
     if (_write(handler,PickList,sizeof(*PickList))==-1) {
         _close(handler);
         return Error;
     }
     _close(handler);
     return OK;

}

word SelectPick(void)
{
     word i,j,ib;
     char Drive[MAXDRIVE],Dir[MAXDIR];
     char Name[MAXFILE],Ext[MAXEXT];
     char Buf[20];
     word ExitF;

     Push(InverseF);
     HOpenWindowSimple(0,VideoBot,VideoRight,VideoBot);
     HNormal();
     HClrBox(0,VideoBot,VideoRight,VideoBot);
     DrawString(0,VideoBot,"”a¯¡ ¦‰œá ¯© ‰Áˆá ¸b´ó ¢…¬áŸi ‰¡Ÿa­A¶a.");
     HInverse();
     HOpenWindow(1,4,16,20);
     for (i=0 ; i<15 ; i++) {
       if ( (*PickList)[i].Name[0]==0 )
          strcpy(Buf," Load File");
       else {
          fnsplit ((*PickList)[i].Name,Drive,Dir,Name,Ext);
          Buf[0]=0;
          strcat(Buf," ");
          strcat(Buf,Name);
          strcat(Buf,Ext);
       }
       DrawString(2,5+i,Buf);
     }
     j=1;
     do {
        HSetInverse(2,5+j,15,5+j);
        GetKey();
        SetHotKeyF();
        HSetInverse(2,5+j,15,5+j);
        if (HotKeyF) break;
        ExitF=True;
        switch(Way) {
          case Up     :
          case CtrlE  :j=(j==0) ? 14:j-1;
                       ExitF=False;
                       break;
          case ' '    :
          case CtrlX  :
          case Down   :j=(j==14) ?0:j+1;
                       ExitF=False;
                       break;
          case PgDn   :
          case CtrlC  :j+=8;
                       if (j>14) j=14;
                       ExitF=False;
                       break;
          case End    :j=14;
                       ExitF=False;
                       break;
          case PgUp   :
          case CtrlR  :if (j<8) j=0;
                       else j-=8;
                       ExitF=False;
                       break;
          case Home   :j=0;
                       ExitF=False;
                       break;
          case CtrlY  :memmove(&((*PickList)[j]),&((*PickList)[j+1]),
                       (14-j)*sizeof((*PickList)[0]));
                       memset(&((*PickList)[14]),0,sizeof((*PickList)[0]));
                       for (i=0 ; i<15 ; i++) {
                         if ( (*PickList)[i].Name[0]==0 )
                            strcpy(Buf," Load File");
                         else {
                            fnsplit ((*PickList)[i].Name,Drive,Dir,Name,Ext);
                            Buf[0]=0;
                            strcat(Buf," ");
                            strcat(Buf,Name);
                            strcat(Buf,Ext);
                         }
                         HClrBox(2,5+i,15,5+i);
                         DrawString(2,5+i,Buf);
                       }
                       ExitF=False;
        }
    } while (!ExitF);
     HCloseWindow();
     HCloseWindowSimple();
     Pop(InverseF);
     if (Way==Return) {
        i=j;
        j=OK;
        HotKeyEndF=True;
        if ((*PickList)[i].Name[0]==0 )
           j=LoadFile();
        else {
           if (EditedF) {
              if (HGetYesNo("¸b´óº—·¡”å ¢…¬áŸi ¸á¸wÐiŒa¶a?",&ib)!=OK) return Error;
              if (ib) if (SaveFile()!=OK) return Error;
              EditedF=False;
           }
           PickFileF=True;
           if (FileNameSetF) StorePickData();
           strcpy(WorkFileName,(*PickList)[i].Name);
           FileNameSetF=True;
           j=LoadFile();
        }
     } else j=Error;
     return j;
}
