/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                File Access Functions                 */
/*                                                      */

#include "handrv.h"
#include "memory.h"
#include "stddef.h"
#include "hwindow.h"
#include "const.h"
#include "edit.h"
#include "filename.h"
#include "crt.h"
#include <stdio.h>
#include <errno.h>
#include <alloc.h>
#include <string.h>
#include <dir.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include "hanconv.h"
#include "print.h"

word   CtrlBrkF;
byte   WorkFileName[80];
FILE   *WorkFile;
byte   WorkBlockName[80];
FILE   *WorkBlock;
word   FileNameSetF;
byte   far *DiskBuf;
word   HanGulCodeType;


word Dir(void)
{
    char p[80]="*.*";

    if (HGetEng ("´á˜åÑwÈ· ÑÁ·©·i Àx‹¡ ¶¥Ða¯³“¡Œa?",p,79)!=OK) return Error;
    if (HGetFileName (p)!=OK) return Error;
    return OK;
}

word ChangeDir(void)
{
    byte p[82];

    p[0]=0;
    if (HGetEng ("¬¡¶… »¡¸÷·i ¶¥Ða“e —¡BÉ¡Ÿ¡Ÿi °áº­A¶a.",p,79)!=OK) return Error;
    FExpand(p);
    if (p[1]==':') setdisk(p[0]-'A');
    if (chdir(p)!=0) {
       HSayIOErr();
       return Error;
    }
    return OK;
}

void ClearEditBuf(void)
{
    word i;

    for (i=1 ; i<=PaperLen ; i++) farfree (Paper[i]),Paper[i]=NULL;
    NowLine          =1;
    PaperLen         =0;
    SX=XB=XB2=HSX    =ScnLeft;
    SY=YB=YB2        =ScnTop;
    HX=HXB=HXB2=1;
    HY=HYB=HYB2=1;
    EditedF=False;
    BlockSY=BlockSX=BlockEX=BlockEY=BlockType=0;
    ShowBlockF=False;
    SelectingBlockF=False;
}

word NewFile(void)
{
    word i=True;

    if (EditedF) {
        if (HGetYesNo("¸b´óº—·¡”å ¢…¬áŸi ¸á¸wÐiŒa¶a?",&i)!=OK) return Error;
        if (i)
           if (SaveFile()!=OK) return Error;
    }
    ClearEditBuf();
    PaperLen=1;
    PutPaper(1,"");
    FileNameSetF=False;
    WorkFileName[0]=0;
    SetEditScn();
    HSayMes ("¬¡¶… ¢…¬á ¸b´ó·i ¯¡¸bÐs“¡”a.");
    SetScnF=True;
    SetEditBuf();
    return OK;
}

word PrintFile(void)
{
    word ib=False;
    word i,k;
    char LineBuf[15];

    if (HGetYesNo("¢…¬áŸi ·¥­áÐiŒa¶a?",&ib)!=OK || ib!=OK) return Error;
    if (OpenPrinter()!=OK) return Error;
    PutEditBuf();
    HSayStatus (" ·¥­á º—·³“¡”a. ");
    k=OK;
    Explain("ESC Ÿi ’Ÿa¯¡¡e º—»¡Ðs“¡”a.                Ñe¸ ·¥­áº—·¥ Ð—:");
    for (i=1 ; i<=PaperLen && k==OK; i++) {
       sprintf (LineBuf,"%4d/%4d",i,PaperLen);
       DrawString(61,VideoBot,LineBuf);
       k=PrintString(Paper[i]+1);
       if (KeyPressed()) {
           GetKey();
           SetHotKeyF();
           if (HotKeyF || Way==ESC) {
             break;
           }
       }
    }
    if (k==OK) ClosePrinter();
    SetEditScn();
    HCloseWindow();
    if (k!=OK) return Error;
    return OK;
}

word PrintBlock(void)
{
    word j,i,S,E,k;
    word ib=False;
    byte LineBuf[15];
    byte PrintLine[256];

    if (HGetYesNo("§iœâ·i ·¥­áÐiŒa¶a?",&ib)!=OK || ib!=OK) return Error;
    OpenPrinter() ;
    if (HSayStatus ("»¡‹q ·¥­áº—·³“¡”a.")!=OK) return Error;
    Explain("ESC Ÿi ’Ÿa¯¡¡e º—»¡Ðs“¡”a.                Ñe¸ ·¥­áº—·¥ Ð—:");
    k=OK;
    if (BlockType==0 && BlockSY!=BlockEY) {
       j=0;
       for (i=BlockSY ; i<=BlockEY && k==OK; i++) {
          sprintf (LineBuf,"%4d/%4d",i-BlockSY+1,BlockEY-BlockSY+1);
          DrawString(61,VideoBot,LineBuf);
          if (i>BlockSY && i<BlockEY) {
              MemCopy(Paper[i]+1,PrintLine+j,Paper[i][0]);
              j+=Paper[i][0];
          } else if (i==BlockSY && Paper[i][0]>=BlockSX) {
              MemSetB(PrintLine,' ',j=BlockSX-1);
              MemCopy(Paper[i]+BlockSX,PrintLine+j,S=Paper[i][0]-BlockSX+1);
              j+=S;
          } else if (i==BlockEY) {
              E=BlockEX;
              if (Paper[i][0]<=E) E=Paper[i][0];
              else if (Paper[i][BlockEX]>127) E++;
              MemCopy(Paper[i]+1,PrintLine+j,E);
              j+=E;
          }
          PrintLine[j]=0;
          j=0;
          k=PrintString(PrintLine);
          if (KeyPressed()) {
              GetKey();
              SetHotKeyF();
              if (HotKeyF || Way==ESC) break;
          }
       }
    } else if (BlockType==1) {
       j=0;
       for (i=BlockSY ; i<=BlockEY && k==OK; i++) {
          sprintf (LineBuf,"%4d/%4d",i-BlockSY+1,BlockEY-BlockSY+1);
          DrawString(61,VideoBot,LineBuf);
          MemCopy(Paper[i]+1,PrintLine+j,Paper[i][0]);
          j+=Paper[i][0];
          PrintLine[j]=0;
          k=PrintString(PrintLine);
          j=0;
          if (KeyPressed()) {
              GetKey();
              SetHotKeyF();
              if (HotKeyF || Way==ESC) break;
          }
       }
    } else {
       j=0;
       for (i=BlockSY ; i<=BlockEY && k==OK; i++) {
          sprintf (LineBuf,"%4d/%4d",i-BlockSY+1,BlockEY-BlockSY+1);
          DrawString(61,VideoBot,LineBuf);
          SetAuxBuf(i);
          S=BlockSX;
          E=BlockEX;
          PrintLine[0]=' ';
          j++;
          if (AuxType[S]==2) S--,j--;
          if (AuxType[E]==1) E++;
          MemCopy(AuxBuf+S,PrintLine+j,E-S+1);
          j+=E-S+1;
          PrintLine[j]=0;
          j=0;
          k=PrintString(PrintLine);
          if (KeyPressed()) {
              GetKey();
              SetHotKeyF();
              if (HotKeyF || Way==ESC) break;
          }
       }
    }
    ClosePrinter();
    HCloseWindow();
    SetEditScn();
    if (k!=OK) return Error;
    return OK;
}

byte static LineBuf[256];

word LoadFile(void)
{
    word ib=True;
    byte *i;
    word j;
    long Size,Pos;
    char Path[MAXPATH],Drive[MAXDRIVE],Dir[MAXDIR];
    char Name[MAXFILE],Ext[MAXPATH],ExtOrg[MAXPATH],NameOrg[MAXFILE];

    PutEditBuf();
    if (FileNameSetF && !PickFileF && !CalledAtStartF) StorePickData();
    if (CalledAtStartF || PickFileF) {
        CalledAtStartF=False;
        FExpand (WorkFileName);
        FNSplit(WorkFileName,Drive,Dir,Name,Ext);
        if (strlen(Name)==0) strcpy(NameOrg,"*");
        else                 strcpy(NameOrg,Name);
        if (strlen(Ext) ==0) {
             if (ExistFileWithExts(NameOrg)) {
                j=OK;
                ExtOrg[0]=0;
             } else {
                j=Error;
                ExtOrg[0]='.';
                PrimaryExt(ExtOrg+1);
             }
        } else strcpy(ExtOrg,Ext);
        fnmerge(WorkFileName,Drive,Dir,NameOrg,ExtOrg);
        i=strpbrk(WorkFileName,"?*");
        if (i!=NULL || j==OK) {
            if (HGetFileName(WorkFileName)!=OK) {
                NewFile();
                CalledAtStartF=False;
                PickFileF=False;
                return OK;
            }
            FileNameSetF=True;
            PickFileF=False;
            SetEditScn();
        } else {
            if (access(WorkFileName,0)==-1) {
                strcpy(LineBuf,WorkFileName);
                NewFile();
                if (!PickFileF) {
                   strcpy(WorkFileName,LineBuf);
                   FileNameSetF=True;
                }
                PickFileF=False;
                SetEditScn();
                return OK;
            }
            PickFileF=False;
        }
    }
    else {
         strcpy(LineBuf,LoadPath);
         if (HGetEng ("·ª‹¡Ÿi ¶¥Ða¯¡“e ¢…¬á· ·¡Ÿq·¡ ¢¥ˆa¶a?",LineBuf,79)!=OK) return Error;
         strcpy(LoadPath,LineBuf);
         if (HGetFileName (LineBuf)!=OK) return Error;
         FNSplit (LineBuf,Drive,Dir,Name,Ext);
         strcpy(Path,Drive);
         strcat(Path,Dir);
         FNSplit (LoadPath,Drive,Dir,Name,Ext);
         strcat(Path,Name);
         strcat(Path,Ext);
         strcpy(LoadPath,Path);
         strupr(LoadPath);
         if (EditedF) {
             if (HGetYesNo("¸b´óº—·¡”å ¢…¬áŸi ¸á¸wÐiŒa¶a?",&ib)!=OK) return Error;
             if (ib) if (SaveFile()!=OK) return Error;
         }
         strcpy(WorkFileName,LineBuf);
    }
    FileNameSetF=True;
    ClearEditBuf();
    WorkFile=fopen (WorkFileName,"rt");
    if (WorkFile==NULL) {
       HSayIOErr();
       NewFile();
       return Error;
    }
    setvbuf (WorkFile,DiskBuf,_IOFBF,DiskBufSize);
    HSayStatus ("»¡‹q ·ª“eº— ·³“¡”a.");
    PaperLen=0;
    ib=False;
    while (True) {
       PaperLen++;
       LineBuf[1]=0;
       if (fgets((byte *)LineBuf+1,256-1,WorkFile)==NULL) {
           if (ferror(WorkFile)) break;
           if (LineBuf[1]!=0 || PaperLen==1) {
               LineBuf[0]=strlen((byte*)LineBuf+1);
               if ((Paper[PaperLen]=farmalloc(LineBuf[0]+2))==NULL) {
                    PaperLen--;
                    break;
               }
               memcpy(Paper[PaperLen],LineBuf,LineBuf[0]+2);
           } else PaperLen--;
           Size=filelength(fileno(WorkFile));
           Pos =ftell(WorkFile);
           if (Size==-1L || Pos==-1L || Size==Pos) break;
           fseek(WorkFile,1,SEEK_CUR);
       } else {
           if (LineBuf[LineBuf[0]=strlen((byte*)LineBuf+1)]=='\n')
               LineBuf[LineBuf[0]--]=0;
           else if (!feof(WorkFile)) ib=True;
           if ((Paper[PaperLen]=farmalloc(LineBuf[0]+2))==NULL) {
                PaperLen--;
                break;
           }
           memcpy(Paper[PaperLen],LineBuf,LineBuf[0]+2);
           if (PaperLen==MaxPaper) break;
       }
       if (PaperLen%57==0) {
           sprintf (LineBuf,"%4d",PaperLen);
           DrawString(12 ,VideoBot,LineBuf);
           if (PaperLen==MaxPaper) break;
       }
       if (farcoreleft()<=MaxBufRest*3) break;
    }
    sprintf (LineBuf,"%4d",PaperLen);
    DrawString(12 ,VideoBot,LineBuf);
    fclose (WorkFile);
    HCloseWindow();
    if (HanGulCodeType==1) {
      word i;

      HSayStatus(" Å¡—a ¥eÑÅ º—·³“¡”a. ");
      for (i=1 ; i<=PaperLen ; i++)
        ToCombi(Paper[i]+1,Paper[i][0]);
      HCloseWindow();
    }
    if (ferror(WorkFile)) {
       HSayIOErr();
       NewFile();
       PickFileF=False;
       return OK;
    }
    if (farcoreleft()<=MaxBufRest*3) HSayMes ("¸b´óµwµb·¡ ¦¹¢Ðs“¡”a.");
    if (PaperLen==MaxPaper)        HSayMes ("á¢  g·e Ð—·¡ ·¶“e ¢…¬á‘A¶a.");
    if (ib)                        HSayMes ("254 Äe·i ñ“e º‰·¡ ¢…¬á ¹¥¸Ðs“¡”a.");
    SetEditScn();
    SetEditBuf();
    SetScnF=True;
    OpenPickData();
    RestorePickData();
    return OK;
}



word SaveFile(void)
{
    word i,j;
    int  k;
    int handler;
    char drv[MAXDRIVE],d[MAXDIR],n[MAXFILE],e[MAXEXT],extraFile[MAXPATH];

    if (!FileNameSetF) {
       if (HGetEng ("¸á¸wÐi ¢…¬á· ·¡Ÿq·¡ ¢¥ˆa¶a?",WorkFileName,MAXPATH-1)!=OK) return Error;
    }
    strupr (WorkFileName);
    FExpand(WorkFileName);
    fnsplit(WorkFileName,drv,d,n,e);
    if (strlen(e) ==0) {
           e[0]='.';
           PrimaryExt(e+1);
           if (strpbrk(e,"*?")!=NULL) e[1]=0;
    }
    fnmerge(WorkFileName,drv,d,n,e);
    if (BackUpF) {
        fnsplit (WorkFileName,drv,d,n,e);
        fnmerge (extraFile,drv,d,n,".BAK");
        if ((k=access(WorkFileName,0))==0 && stricmp(e,".BAK")!=0) {
            remove (extraFile);
            if (rename (WorkFileName,extraFile)!=0)
                return HSayIOErr();
        }
    }
    if ((handler=_creat(WorkFileName,FA_ARCH))==-1) return HSayIOErr();
    if (HanGulCodeType==0) HSayStatus ("³a‰¡ ·¶“e º—·³“¡”a.");
    else  HSayStatus (" Å¡—a ¥eÑÅ‰Á ³a‹¡ º—·³“¡”a. ");
    j=0;
    for (i=1 ; i<=PaperLen ; i++) {
       MemCopy(Paper[i]+1,DiskBuf+j,Paper[i][0]);
       j+=Paper[i][0];
       DiskBuf[j++]=13;
       DiskBuf[j++]=10;
       if (j>DiskBufSize-300 || PaperLen==i) {
           if (PaperLen==i) DiskBuf[j++]=26;
           if (HanGulCodeType==1) {
              ToFixed(DiskBuf,j);
           }
           if ((k=_write(handler,DiskBuf,j))==-1) break;
           j=0;
       }
    }
    _close(handler);
    HCloseWindow();
    if (k==-1) return HSayIOErr();
    Task[NowTask].EditedF=EditedF=False;
    FExpand(WorkFileName);
    FileNameSetF=True;
    SetEditScn();
    SetEditBuf();
    return OK;
}

word WriteTo(void)
{
     byte NameBuf[80];
     word FlagBuf,Status;

     NameBuf[0]=0;
     if (HGetEng ("¸á¸wÐi ¢…¬á· ·¡Ÿq·¡ ¢¥ˆa¶a?",NameBuf,MAXPATH-1)!=OK) return Error;
     strcpy(WorkFileName,NameBuf);
     FileNameSetF=True;
     Status=SaveFile();
     SetEditScn();
     return Status;
}

word SaveAll(void)
{
     word TaskBuf,Status,i;

     TaskBuf=NowTask;
     Status=OK;
     for (i=0; i<3 ; i++) {
          ChangeTasks(i);
          if (EditedF==False) continue;
          SetScn();
          if ((Status=SaveFile())!=OK) break;
     }
     ChangeTasks(TaskBuf);
     return Status;
}

word LoadBlock(void)
{
    word ib;
    long Size;
    long Pos;
    char Path[MAXPATH],Drive[MAXDRIVE],Dir[MAXDIR];
    char Name[MAXFILE],Ext[MAXEXT];

    if (HGetEng ("·ª·i ¢…”e· ·¡Ÿq·¡ ¢¥ˆa¶a?",WorkBlockName,79)!=OK) return Error;
    fnsplit (WorkBlockName,Drive,Dir,Name,Ext);
    if (strlen(Ext) ==0) {
       Ext[0]='.';
       PrimaryExt(Ext+1);
    }
    fnmerge(WorkBlockName,Drive,Dir,Name,Ext);
    if (HGetFileName (WorkBlockName)!=OK) return Error;
    strupr (WorkBlockName);
    if ((WorkBlock=fopen (WorkBlockName,"rt"))==NULL) return HSayIOErr();
    setvbuf (WorkBlock,DiskBuf,_IOFBF,DiskBufSize);
    HSayStatus ("»¡‹q ¢…”e·i ·ª“e º—·³“¡”a.");
    ib=False;
    while (True) {
       PaperLen++;
       LineBuf[1]=0;
       if (fgets((byte *)LineBuf+1,256-1,WorkBlock)==NULL) {
           if (ferror(WorkBlock)) break;
           if (LineBuf[1]!=0) {
               LineBuf[0]=strlen((byte*)LineBuf+1);
               if ((Paper[PaperLen]=farmalloc(LineBuf[0]+2))==NULL) {
                    PaperLen--;
                    break;
               }
               memcpy(Paper[PaperLen],LineBuf,LineBuf[0]+2);
           } else PaperLen--;
           Size=filelength(fileno(WorkBlock));
           Pos =ftell(WorkBlock);
           if (Size==-1L || Pos==-1L || Size==Pos) break;
           fseek(WorkBlock,1,SEEK_CUR);
       } else {
           if (LineBuf[LineBuf[0]=strlen((byte*)LineBuf+1)]=='\n')
               LineBuf[LineBuf[0]--]=0;
           else if (!feof(WorkBlock)) ib=True;
           if ((Paper[PaperLen]=farmalloc(LineBuf[0]+2))==NULL) {
                PaperLen--;
                break;
           }
           memcpy(Paper[PaperLen],LineBuf,LineBuf[0]+2);
       }
       if (PaperLen%50==0) {
           sprintf (LineBuf,"%4d",PaperLen);
           DrawString(12 ,VideoBot,LineBuf);
           if (PaperLen==MaxPaper) break;
       }
       if (farcoreleft()<=MaxBufRest*3) break;
    }
    sprintf (LineBuf,"%4d",PaperLen);
    DrawString(12 ,VideoBot,LineBuf);
    fclose (WorkBlock);
    HCloseWindow();
    if (ferror(WorkBlock))        HSayIOErr();
    if (farcoreleft()<MaxBufRest*3) HSayMes ("¸b´óµwµb·¡ ¦¹¢Ðs“¡”a.");
    if (PaperLen==MaxPaper)       HSayMes ("á¢  g·e Ð—·¡ ·¶“e ¢…¬á‘A¶a.");
    if (ib)                       HSayMes ("254 Äe·i ñ“e Ð—·¡ ¹¥¸Ðs“¡”a.");
    return OK;
}

word SaveBlock(void)
{
    word j,i,S,E;
    int  k;
    char drv[MAXDRIVE],d[MAXDIR],n[MAXFILE],e[MAXEXT],extraBlock[MAXPATH];
    int  handler;


    if (HGetEng ("¸á¸wÐi ¢…”e· ·¡Ÿq·¡ ¢¥ˆa¶a?",WorkBlockName,256-1)!=OK) return Error;
    strupr (WorkBlockName);
    FExpand(WorkBlockName);
    fnsplit(WorkBlockName,drv,d,n,e);
    if (strlen(e) ==0) {
           e[0]='.';
           PrimaryExt(e+1);
           if (strpbrk(e,"*?")!=NULL) e[1]=0;
    }
    fnmerge(WorkBlockName,drv,d,n,e);
    if (BackUpF) {
        fnsplit (WorkBlockName,drv,d,n,e);
        fnmerge (extraBlock,drv,d,n,".BAK");
        if (access(WorkBlockName,0)==0 && strcmp(e,".BAK")!=0) {
           remove (extraBlock);
           if (rename (WorkBlockName,extraBlock)!=0) return HSayIOErr();
        }
    }
    if ((handler=_creat(WorkBlockName,FA_ARCH))==-1) return HSayIOErr();
    HSayStatus ("»¡‹q ¢…”e·i ³a‰¡ ·¶“e º—·³“¡”a.");
    if (BlockType==0 && BlockSY!=BlockEY) {
       j=0;
       for (i=BlockSY ; i<=BlockEY; i++) {
          if (i>BlockSY && i<BlockEY) {
              MemCopy(Paper[i]+1,DiskBuf+j,Paper[i][0]);
              j+=Paper[i][0];
          } else if (i==BlockSY && Paper[i][0]>=BlockSX) {
              MemCopy(Paper[i]+BlockSX,DiskBuf+j,S=Paper[i][0]-BlockSX+1);
              j+=S;
          } else if (i==BlockEY) {
              E=BlockEX;
              if (Paper[i][0]<=E) E=Paper[i][0];
              else if (Paper[i][BlockEX]>127) E++;
              MemCopy(Paper[i]+1,DiskBuf+j,E);
              j+=E;
          }
          DiskBuf[j++]=13;
          DiskBuf[j++]=10;
          if (j>DiskBufSize-300 || BlockEY==i) {
              if (BlockEY==i) DiskBuf[j++]=26;
              if (HanGulCodeType==1) {
                 ToFixed(DiskBuf,j);
              }
              if ((k=_write(handler,DiskBuf,j))==-1) break;
              j=0;
          }
       }
    } else if (BlockType==1) {
       j=0;
       for (i=BlockSY ; i<=BlockEY; i++) {
          MemCopy(Paper[i]+1,DiskBuf+j,Paper[i][0]);
          j+=Paper[i][0];
          DiskBuf[j++]=13;
          DiskBuf[j++]=10;
          if (j>DiskBufSize-300 || BlockEY==i) {
              if (BlockEY==i) DiskBuf[j++]=26;
              if (HanGulCodeType==1) {
                 ToFixed(DiskBuf,j);
              }
              if ((k=_write(handler,DiskBuf,j))==-1) break;
              j=0;
          }
       }
    } else {
       j=0;
       for (i=BlockSY ; i<=BlockEY; i++) {
          SetAuxBuf(i);
          S=BlockSX;
          E=BlockEX;
          if (AuxType[S]==2) S--;
          if (AuxType[E]==1) E++;
          MemCopy(AuxBuf+S,DiskBuf+j,E-S+1);
          j+=E-S+1;
          DiskBuf[j++]=13;
          DiskBuf[j++]=10;
          if (j>DiskBufSize-300 || BlockEY==i) {
              if (BlockEY==i) DiskBuf[j++]=26;
              if (HanGulCodeType==1) {
                 ToFixed(DiskBuf,j);
              }
              if ((k=_write(handler,DiskBuf,j))==-1) break;
              j=0;
          }
       }
    }
    _close(handler);
    HCloseWindow();
    if (k==-1) return HSayIOErr();
    return OK;
}

word AppendBlock(void)
{
    word j,i,S,E;
    int  k;
    char drv[MAXDRIVE],d[MAXDIR],n[MAXFILE],e[MAXEXT],extraBlock[MAXPATH];
    int  handler;


    if (HGetEng ("´á“a ¢…¬á –áµA ¦›µa —aŸ©Œa¶a?",WorkBlockName,256-1)!=OK) return Error;
    strupr (WorkBlockName);
    FExpand(WorkBlockName);
    fnsplit(WorkBlockName,drv,d,n,e);
    if (strlen(e) ==0) {
           e[0]='.';
           PrimaryExt(e+1);
           if (strpbrk(e,"*?")!=NULL) e[1]=0;
    }
    fnmerge(WorkBlockName,drv,d,n,e);
    if (access(WorkBlockName,0)==0 ) {
       if ((handler=_open(WorkBlockName,O_WRONLY))==-1) return HSayIOErr();
       if (lseek(handler,0,SEEK_END)==-1) return HSayIOErr();
       HSayStatus ("§iœâ·i ³s“¡”a.");
    } else {
       if ((handler=_creat(WorkBlockName,FA_ARCH))==-1) return HSayIOErr();
       HSayStatus ("»¡‹q ¢…”e·i ³a‰¡ ·¶“e º—·³“¡”a.");
    }
    if (BlockType==0 && BlockSY!=BlockEY) {
       j=0;
       for (i=BlockSY ; i<=BlockEY; i++) {
          if (i>BlockSY && i<BlockEY) {
              MemCopy(Paper[i]+1,DiskBuf+j,Paper[i][0]);
              j+=Paper[i][0];
          } else if (i==BlockSY && Paper[i][0]>=BlockSX) {
              MemCopy(Paper[i]+BlockSX,DiskBuf+j,S=Paper[i][0]-BlockSX+1);
              j+=S;
          } else if (i==BlockEY) {
              E=BlockEX;
              if (Paper[i][0]<=E) E=Paper[i][0];
              else if (Paper[i][BlockEX]>127) E++;
              MemCopy(Paper[i]+1,DiskBuf+j,E);
              j+=E;
          }
          DiskBuf[j++]=13;
          DiskBuf[j++]=10;
          if (j>DiskBufSize-300 || BlockEY==i) {
              if (BlockEY==i) DiskBuf[j++]=26;
              if (HanGulCodeType==1) {
                 ToFixed(DiskBuf,j);
              }
              if ((k=_write(handler,DiskBuf,j))==-1) break;
              j=0;
          }
       }
    } else if (BlockType==1) {
       j=0;
       for (i=BlockSY ; i<=BlockEY; i++) {
          MemCopy(Paper[i]+1,DiskBuf+j,Paper[i][0]);
          j+=Paper[i][0];
          DiskBuf[j++]=13;
          DiskBuf[j++]=10;
          if (j>DiskBufSize-300 || BlockEY==i) {
              if (BlockEY==i) DiskBuf[j++]=26;
              if (HanGulCodeType==1) {
                 ToFixed(DiskBuf,j);
              }
              if ((k=_write(handler,DiskBuf,j))==-1) break;
              j=0;
          }
       }
    } else {
       j=0;
       for (i=BlockSY ; i<=BlockEY; i++) {
          SetAuxBuf(i);
          S=BlockSX;
          E=BlockEX;
          if (AuxType[S]==2) S--;
          if (AuxType[E]==1) E++;
          MemCopy(AuxBuf+S,DiskBuf+j,E-S+1);
          j+=E-S+1;
          DiskBuf[j++]=13;
          DiskBuf[j++]=10;
          if (j>DiskBufSize-300 || BlockEY==i) {
              if (BlockEY==i) DiskBuf[j++]=26;
              if (HanGulCodeType==1) {
                 ToFixed(DiskBuf,j);
              }
              if ((k=_write(handler,DiskBuf,j))==-1) break;
              j=0;
          }
       }
    }
    _close(handler);
    HCloseWindow();
    if (k==-1) return HSayIOErr();
    return OK;
}
