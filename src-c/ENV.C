/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*               Main Edit ,Const ,and Var              */
/*                                                      */

#include "stddef.h"
#include "crt.h"
#include "memory.h"
#include "handrv.h"
#include "hwindow.h"
#include "popup.h"
#include "const.h"
#include "edit.h"
#include "filename.h"
#include "print.h"
#include <io.h>
#include <dos.h>
#include <alloc.h>
#include <string.h>

byte   HanFontName[13];
word   HanFontType;
byte   EngFontName[13];
byte   GrfFontName[13];
byte   SysPath[65];
byte   LoadPath[81];
word   ChangeFontF;
word   AuxScreenMode;
word   PrepareF=True;
word   BackUpF=True;
word   ProtectLineF=False;
word   CutLineF=False;
word   CutLineLimit=70;
word   OsSaveF=False;
word   extern BlockSpace;

struct TaskType Task[3];
word   NowTask;
word   extern MacroArray[26][128];
byte   extern ShiftArray['Z'-'A'+1][MaxMacroString];
byte   extern HanFArray ['Z'-'A'+1];

word   SaveOptions(void)
{
       int handler;
       byte name[80];

       strcpy(name,SysPath);
       strcat(name,"san.cnf");
       if ((handler=_creat(name,FA_ARCH))==-1) return Error;
       if (_write(handler,&InsertF,2)    ==-1 ||
           _write(handler,&IndentF,2)    ==-1 ||
           _write(handler,&AuxScreenMode,2) ==-1 ||
           _write(handler,HanFontName,13)==-1 ||
           _write(handler,&HanFontType,2)==-1 ||
           _write(handler,EngFontName,13)==-1 ||
           _write(handler,GrfFontName,13)==-1 ||
           _write(handler,StdExt,80)      ==-1 ||
           _write(handler,&Task[0].MaxPaper,2)==-1 ||
           _write(handler,&Task[1].MaxPaper,2)==-1 ||
           _write(handler,&Task[2].MaxPaper,2)==-1 ||
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
           _write(handler,&SpaceLength,2)==-1 ||
           _close(handler)               ==-1 )  return Error;
       else                                      return OK;
}

word   RetrieveOptions(void)
{
       int handler;
       byte name[80];

       strcpy(name,SysPath);
       strcat(name,"san.cnf");
       if ((handler=_open(name,0))==-1) return Error;
       if (filelength(handler)!=664+3328+6+6656+2+26) {
          _close(handler);
          return Error;
       }
       if (_read(handler,&InsertF,2)       ==-1 ||
           _read(handler,&IndentF,2)       ==-1 ||
           _read(handler,&AuxScreenMode,2) ==-1 ||
           _read(handler,HanFontName,13)   ==-1 ||
           _read(handler,&HanFontType,2)   ==-1 ||
           _read(handler,EngFontName,13)   ==-1 ||
           _read(handler,GrfFontName,13)   ==-1 ||
           _read(handler,StdExt,80)        ==-1 ||
           _read(handler,&Task[0].MaxPaper   ,2)   ==-1 ||
           _read(handler,&Task[1].MaxPaper   ,2)   ==-1 ||
           _read(handler,&Task[2].MaxPaper   ,2)   ==-1 ||
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
           _read(handler,&BlockSpace,2)==-1 ||
           _read(handler,MacroArray,6656)==-1 ||
           _read(handler,&PageLength,2)==-1 ||
           _read(handler,&TopMargin,2)==-1 ||
           _read(handler,&BotMargin,2)==-1 ||
           _read(handler,ShiftArray,3328)==-1 ||
           _read(handler,HanFArray,26)==-1 ||
           _read(handler,&SpaceLength,2)==-1 ||

           _close(handler)                 ==-1 )  return Error;
       else return OK;
}

void SaveTask(word TaskN)
{
     Task[TaskN].NowLine      =NowLine;
     Task[TaskN].PaperLen     =PaperLen;
     Task[TaskN].SX           =SX;
     Task[TaskN].SY           =SY;
     Task[TaskN].HSX          =HSX;
     Task[TaskN].HX           =HX;
     Task[TaskN].HY           =HY;
     Task[TaskN].EditedF      =EditedF;
     Task[TaskN].BlockSY      =BlockSY;
     Task[TaskN].BlockEY      =BlockEY;
     Task[TaskN].BlockHeadX   =BlockHeadX;
     Task[TaskN].BlockHeadY   =BlockHeadY;
     Task[TaskN].BlockSX      =BlockSX;
     Task[TaskN].BlockEX      =BlockEX;
     Task[TaskN].BlockType    =BlockType;
     Task[TaskN].ShowBlockF   =ShowBlockF;
     Task[TaskN].SelectingBlockF=SelectingBlockF;
     Task[TaskN].FileNameSetF =FileNameSetF;
     strcpy(Task[TaskN].WorkFileName,WorkFileName);
}

void LoadTask(word TaskN)
{
     MaxPaper     =Task[TaskN].MaxPaper;
     Paper        =Task[TaskN].Paper;
     NowLine      =Task[TaskN].NowLine;
     PaperLen     =Task[TaskN].PaperLen;
     SX           =Task[TaskN].SX;
     SY           =Task[TaskN].SY;
     HSX          =Task[TaskN].HSX;
     HX           =Task[TaskN].HX;
     HY           =Task[TaskN].HY;
     EditedF      =Task[TaskN].EditedF;
     BlockSY      =Task[TaskN].BlockSY;
     BlockEY      =Task[TaskN].BlockEY;
     BlockHeadX   =Task[TaskN].BlockHeadX;
     BlockHeadY   =Task[TaskN].BlockHeadY;
     BlockSX      =Task[TaskN].BlockSX;
     BlockEX      =Task[TaskN].BlockEX;
     BlockType    =Task[TaskN].BlockType;
     ShowBlockF   =Task[TaskN].ShowBlockF;
     SelectingBlockF=Task[TaskN].SelectingBlockF;
     FileNameSetF =Task[TaskN].FileNameSetF;
     strcpy(WorkFileName,Task[TaskN].WorkFileName);
     SetEditBuf();
     SetCurPos(HX+SX-1,NowLine);
     SetScnF=True;
     XB=XB2=SX;
     YB=YB2=SY;
     HXB=HXB2=HX;
     HYB=HYB2=HY;
     NowTask=TaskN;
     SetEditScn();
}

void ChangeTasks(word NewTask)
{
     SaveTask(NowTask);
     LoadTask(NewTask);
}

void InitTasks(void)
{
    word i;

    for (i=0 ; i<3 ; i++) {
         Task[i].NowLine        =1;
         Task[i].PaperLen       =1;
         Task[i].SX             =ScnLeft;
         Task[i].SY             =ScnTop;
         Task[i].HSX            =ScnLeft;
         Task[i].HX             =1;
         Task[i].HY             =1;
         Task[i].EditedF        =False;
         Task[i].BlockSY        =0;
         Task[i].BlockEY        =0;
         Task[i].BlockHeadX     =0;
         Task[i].BlockHeadY     =0;
         Task[i].BlockSX        =0;
         Task[i].BlockEX        =0;
         Task[i].BlockType      =0;
         Task[i].ShowBlockF     =False;
         Task[i].SelectingBlockF=False;
         Task[i].FileNameSetF   =False;
         Task[i].WorkFileName[0]=0;
         MemSetB(Task[i].Paper,0,(Task[i].MaxPaper+1)*4);
         Task[i].Paper[1]       =farmalloc(2);
         Task[i].Paper[1][0]    =0;
         Task[i].Paper[1][1]    =0;
    }
}
