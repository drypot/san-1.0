/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                Const and Var Header                  */
/*                                                      */
/*                                                      */

#include <stdio.h>

#define None    0
#define Find    1
#define Replace 2
#define MaxBufRest 1024
#define MaxLine     256
#define DiskBufSize 16318
#define MaxUnKillBuf 512

struct  FOptionType {
              word G:1;
              word L:1;
              word U:1;
              word W:1;
              word N:1;
              word A:1;
       } extern FOption;

word extern MaxPaper;
word extern CalledAtStartF;
byte extern far * far *Paper;
byte extern far * far *UnKillBuf;
word extern ScnLeft,ScnRight,ScnBot,ScnTop,ScnHigh,ScnWidth;
byte extern EditBuf[256],EditType[256],UndoBuf[256],UndoType[256],AuxBuf[256],AuxType[256];
word extern NowLine,PaperLen;
unsigned long extern MaxBufSize;
word extern SX,SY,HSX,XB,XB2,YB,YB2;
word extern HX,HXB,HXB2;
word extern HY,HYB,HYB2;
word extern HelpX,HelpY;
word extern InsertF,IndentF,EditedF;
word extern BlockSY,BlockSX,BlockEY,BlockEX,BlockHeadY,BlockHeadX;
word extern BlockType,ShowBlockF,SelectingBlockF;
word extern BlockSortType,BackUpF,PrepareF,ProtectLineF,OsSaveF;
word extern CutLineF,CutLineLimit;
word extern SmartRefleshF;
word extern SetScnF;
word extern KeyInDisableF;
byte extern SearchString[256],AuxSearchString[256],ReplaceString[256],FindOption[12];
word extern OldFindCommand;
word extern SearchCounter;
word extern SearchSY,SearchEY;
word extern SearchHeadCtrlAChar;
word extern SearchSX;
word extern CtrlBrkF;
byte extern WorkFileName[80];
FILE extern *WorkFile;
byte extern WorkBlockName[80];
FILE extern *WorkBlock;
word extern FileNameSetF;
byte extern far *DiskBuf;

byte extern HanFontName[13];
word extern HanFontType;
byte extern SysPath[65];
byte extern LoadPath[81];
byte extern EngFontName[13];
byte extern GrfFontName[13];
word extern AuxScreenMode;
word extern AuxInverseF;
word extern ChangeFontF;
word extern CriticalF;

word extern BlockLeft;
word extern BlockRight;
word extern BlockIndent;

word extern HanGulCodeType;

struct TaskType {
       word   MaxPaper;
       byte   far * far *Paper;
       word   NowLine,PaperLen;
       word   SX,SY,HSX,HX,HY;
       word   EditedF;
       word   BlockSY       ,
              BlockSX       ,
              BlockEY       ,
              BlockEX       ,
              BlockHeadX    ,
              BlockHeadY    ;
       word   BlockType     ,
              ShowBlockF    ,
              SelectingBlockF;
       byte   WorkFileName[80];
       word   FileNameSetF;
       word   ScnLeft,ScnRight,ScnBot,ScnTop,ScnHigh,ScnWidth;
       } extern Task[3];
word extern NowTask;

word extern KeyDelay,KeyRate;

struct PickListEle {
         byte Name[81];
         word BlockSY;
         word BlockSX;
         word BlockEY;
         word BlockEX;
         word BlockType;
         word ShowBlockF;
         word HX,HY;
         word SX,SY;
       } extern (*PickList)[15];

word extern PickFileF;

byte extern RunItems[5][80];
byte extern RunTitle[5][19];
