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
#include <string.h>
#include <ctype.h>

word   MaxPaper   =5000;
unsigned long MaxBufSize;
word   CalledAtStartF=True;

word   SX,SY,HSX,XB,XB2,YB,YB2;
word   HX,HXB,HXB2;
word   HY,HYB,HYB2;
word   HelpX,HelpY;
word   InsertF       =True,
       IndentF       =True,
       EditedF;

word   KeyDelay=3,KeyRate=0x0D;

void DrawMoreKey(char *data)
{
     DrawString(0,VideoBot,data);
}

void EditCO(void)
{
    DrawMoreKey("^O");
    GetKey();
    if (Held(Way,'A','Z')) Way-=64;
    if (Held(Way,'a','z')) Way-=96;
    switch (toupper(Way)) {
      case CtrlI:EditCQI(); break;
      case CtrlL:EditCOL(); break;
      case CtrlK:EditCOK(); break;
      default   :SetHotKeyF();
    }
    DrawMoreKey("  ");
}

void EditCB(void)
{
    word i;

    DrawMoreKey("^B");
    GetKey();
    if (!ExtKey) {
       i=CurXPos();
       if (InsertF) {
          if (InsertElement(i,1)==OK) {
              EditBuf [i]=Key;
              EditType[i]=0;
              SetCurPos (i+1,NowLine);
          }
       } else {
          if (ReplaceElement(i,1)==OK) {
              EditBuf [i]=Key;
              EditType[i]=0;
              SetCurPos (i+1,NowLine);
          }
       }
    }
    DrawMoreKey("  ");
}

void EditCKE(void);
void EditCKA(void);
void EditCKZ(void);
void EditCKX(void);
void EditCKD(void);

void EditCK(void)
{
    DrawMoreKey("^K");
    GetKey();
    if (Held(Way,'A','Z')) Way-=64;
    if (Held(Way,'a','z')) Way-=96;
    if (!EditedF)
       switch (Way) {
         case '1'  :
         case '2'  :
         case '3'  :
         case CtrlC:
         case CtrlY:
         case CtrlV:
         case CtrlR:
         case CtrlI:
         case CtrlL:
         case CtrlE:
         case CtrlZ:
         case CtrlX:
         case CtrlD:
         case CtrlU:EditedF=True;
                    SetEditScn();
       }
    switch (Way) {
      case CtrlS:if (PutEditBuf()==OK)
                     SaveFile();
                 SetEditBuf();
                 break;
      case CtrlD:EditCKD();      break;
      case CtrlQ:MainMenu();     break;
      case CtrlB:EditCKB();      break;
      case CtrlK:EditCKK();      break;
      case CtrlT:EditCKT();      break;
      case CtrlP:EditCKP();      break;
      case CtrlC:EditCKC();      break;
      case CtrlY:EditCKY();      break;
      case CtrlH:EditCKH();      break;
      case CtrlV:EditCKV();      break;
      case CtrlR:EditCKR();      break;
      case CtrlW:EditCKW();      break;
      case CtrlI:EditCKI();      break;
      case CtrlU:EditCKU();      break;
      case CtrlL:EditCKL();      break;
      case CtrlE:EditCKE();      break;
      case CtrlZ:EditCKZ();      break;
      case CtrlX:EditCKX();      break;
      case CtrlA:EditCKA();      break;
      case '1'  :GetBlock(0);    break;
      case '2'  :GetBlock(1);    break;
      case '3'  :GetBlock(2);    break;
      default   :SetHotKeyF();
    }
    DrawMoreKey("  ");
}

void EditCQ(void)
{
    DrawMoreKey("^Q");
    GetKey();
    if (Held(Way,'A','Z')) Way-=64;
    if (Held(Way,'a','z')) Way-=96;
    if (!EditedF)
       switch (Way) {
         case CtrlY    :
         case CtrlA    :EditedF=True;
                        SetEditScn();
       }
    switch (Way) {
      case CtrlP:EditCQP();   break;
      case CtrlS:EditHome();  break;
      case CtrlD:EditEnd();   break;
      case CtrlE:EditCHome(); break;
      case CtrlX:EditCEnd();  break;
      case CtrlR:EditCPgUp(); break;
      case CtrlC:EditCPgDn(); break;
      case CtrlY:EditCQY();   break;
      case CtrlI:EditCQI();   break;
      case CtrlB:EditCQB();   break;
      case CtrlK:EditCQK();   break;
      case CtrlF:EditCQF();   break;
      case CtrlA:EditCQA();   break;
      case CtrlL:EditCQL();   break;
      case '1'  :SendBlock(0); break;
      case '2'  :SendBlock(1); break;
      case '3'  :SendBlock(2); break;
      default   :SetHotKeyF();
    }
    DrawMoreKey("  ");
}

word EditBufLen(void)
{
    word i;

    i=256-2;
    while ( EditBuf[i]==' ' && EditType[i]==0 && i>1) i--;
    return i;
}

#define SpaceTillRow(Data,Row) ((strspn(Data," ")>=(word)(Row))?True:False)

void CutLine(void)
{
    word OldX,OldY;
    word i,j;

    BlockSY=BlockSX=BlockEY=BlockEX=0;
    ShowBlockF=False;
    OldX=CurXPos();
    OldY=NowLine;
    i=BlockRight;
    while (((EditBuf[i]!=' '&&EditType[i]==0) || EditType[i]!=0) && i>=BlockLeft) i--;
    while (EditBuf[i]==' '&&EditType[i]==0 && i>=BlockLeft) i--;
    if (i<BlockLeft) {
      i=BlockRight;
      while (((EditBuf[i+1]!=' '&&EditType[i+1]==0) || EditType[i+1]!=0) && i<254) i++;
      if (i==254) return;
    }
    j=i+1;
    while ( EditBuf[j]==' ' && EditType[j]==0 && j<255 ) j++;
    if (j==255) return;
    j--;
    PutEditBuf();
    if (NowLine==PaperLen ||
        Paper[NowLine][0]-j+Paper[NowLine+1][0]>BlockRight) {
       SetCurPos(j+1,NowLine);
       if (EditCN()!=OK) return;
       if (Paper[NowLine+1][0]+BlockLeft-1<255) {
          AuxBuf[0]=0;
          AppendSpace(AuxBuf,BlockLeft-1);
          strcat(AuxBuf,Paper[NowLine+1]+1);
          PutPaper(NowLine+1,AuxBuf);
          if (OldX>j) {
             OldX=BlockLeft+(OldX-j)-1;
             OldY++;
          } else if (OldX>i) {
             OldX=BlockLeft;
             OldY++;
          }
       } else
          if (OldX>j) {
             OldX=(OldX-j);
             OldY++;
          } else if (OldX>i) {
             OldX=1;
             OldY++;
          }
    } else {
          strcpy(AuxBuf,Paper[NowLine]+j+1);
          strcat(AuxBuf," ");
          strcat(AuxBuf,Paper[NowLine+1]+Stringstartpos(NowLine+1));
          if (strlen(AuxBuf)+BlockLeft-1<255) {
             EditBuf[0]=0;
             AppendSpace(EditBuf,BlockLeft-1);
             strcat(EditBuf,AuxBuf);
             if (OldX>j) {
                OldX=BlockLeft-1+(OldX-j);
                OldY++;
             } else if (OldX>i) {
                OldX=BlockLeft;
                OldY++;
             }
             PutPaper(NowLine+1,EditBuf);
          } else {
             if (OldX>j) {
                OldX=(OldX-j);
                OldY++;
             } else if (OldX>i) {
                OldX=1;
                OldY++;
             }
             PutPaper(NowLine+1,AuxBuf);
          }
          strcpy(AuxBuf,Paper[NowLine]+1);
          AuxBuf[i]=0;
          PutPaper(NowLine,AuxBuf);
    }
    SetEditBuf();
    SetCurPos(OldX,OldY);
    SetScnF=True;
}

void EditF4(void)
{
    word i,k,j,l;

    if ((j=SelectGrf())==Error) return;
    i=CurXPos();
    l=strlen(AuxBuf+1);
    if (!EditedF) EditedF=True,SetEditScn();
    if (l>0) {
       if (InsertF) {
          if (InsertElement(i,l)==OK) {
             strncpy (EditBuf+i,AuxBuf+1,l);
             for (k=0 ; k<l ; ) {
                EditType[i+k++]=1;
                EditType[i+k++]=2;
             }
          }
       } else {
          if (ReplaceElement(i,l)==OK) {
             strncpy (EditBuf+i,AuxBuf+1,l);
             for (k=0 ; k<l ; ) {
                EditType[i+k++]=1;
                EditType[i+k++]=2;
             }
          }
       }
       if (CutLineF && EditBufLen()>BlockRight) CutLine();
       return;
    }

    HanKey=j;
    if (InsertF) {
        if (InsertElement(CurXPos(),2)==OK) {
           *((word *)(EditBuf+i))=HanKey;
           EditType[i]=1;
           EditType[i+1]=2;

        } else HanResult=2;
    }
    else {
        if (ReplaceElement(CurXPos(),2)==OK) {
           *((word *)(EditBuf+i))=HanKey;
           EditType[i]=1;
           EditType[i+1]=2;
           SetCurPos (i+2,NowLine);
        } else HanResult=2;
    }
    if (CutLineF && EditBufLen()>BlockRight) CutLine();

}

void ED_CombiNow(void)
{
    word i,j;

    i=CurXPos();
    *((word *)(EditBuf+i))=HanKey;
}

void ED_CombiComplete(void)
{
     ED_CombiNow();
     SetCurPos (CurXPos()+2,NowLine);
}

void ED_CombiStart()
{
   word i,j;

   i=CurXPos();
   if (!EditedF) EditedF=True,SetEditScn();
   if (InsertF) {
       if (InsertElement(CurXPos(),2)==OK) {
          *((word *)(EditBuf+i))=HanKey;
          EditType[i]=1;
          EditType[i+1]=2;
       } else HanResult=2;
   }
   else {
       if (ReplaceElement(CurXPos(),2)==OK) {
          *((word *)(EditBuf+i))=HanKey;
          EditType[i]=1;
          EditType[i+1]=2;
       } else HanResult=2;
   }
   if (CutLineF && EditBufLen()>BlockRight) CutLine();
}

void ED_CancelCombi(void)
{
   EraseElement(CurXPos(),CurXPos()+2);
}

void ED_AccessEng(void)
{
    word i;

    i=CurXPos();
    if (InsertF) {
       if (InsertElement(i,1)==OK) {
           EditBuf [i]=Key;
           EditType[i]=0;
           SetCurPos (i+1,NowLine);
       }
    } else {
       if (ReplaceElement(i,1)==OK) {
           EditBuf [i]=Key;
           EditType[i]=0;
           SetCurPos (i+1,NowLine);
       }
    }
    if (CutLineF && EditBufLen()>BlockRight) CutLine();
}

void DisplayTitle(void);
void EditDel(void);

word extern ScaningShiftF;
word extern ErasedShiftF;


void ED_AccessCommand(void)
{
    if (HX^HY^SX^SY != HXB2^HYB2^XB2^YB2) {
        XB=XB2; YB=YB2; HXB=HXB2; HYB=HYB2;
        XB2=SX; YB2=SY; HXB2=HX; HYB2=HY;
    }

    if (ScaningShiftF && ErasedShiftF) {
        SelectingBlockF=False;
        ScaningShiftF=False;
    }
    if (ShiftPressed()) {
       if (ScanCode.Half[0]==0xff) {
           switch (ScanCode.Half[1]) {
             case 0x6:
             case 0x7:
             case 0x8:
             case 0x9:
             case 0xa:
             case 0xb:
             case 0xc:
             case 0xd:switch(ScanCode.Half[1]) {
                             case 0xa: Way=Home; break;
                             case 0x6: Way=Up  ; break;
                             case 0xc: Way=PgUp; break;
                             case 0x7: Way=Left; break;
                             case 0x8: Way=Right;break;
                             case 0xb: Way=End;  break;
                             case 0x9: Way=Down; break;
                             case 0xd: Way=PgDn; break;
                           }
                        if (!ScaningShiftF) {
                           ScaningShiftF=True;
                           ErasedShiftF=False;
                           UnselectBlock();
                           switch(ScanCode.Half[1]) {
                             case 0xa: Way=Home; break;
                             case 0x6: Way=Up  ; break;
                             case 0xc: Way=PgUp; break;
                             case 0x7: Way=Left; break;
                             case 0x8: Way=Right;break;
                             case 0xb: Way=End;  break;
                             case 0x9: Way=Down; break;
                             case 0xd: Way=PgDn; break;
                           }
                           EditAA();
                        }
           }
       } else {
           switch (ScanCode.Half[1]) {
             case 0x47:
             case 0x48:
             case 0x49:
             case 0x4B:
             case 0x4D:
             case 0x4F:
             case 0x50:
             case 0x51:switch(ScanCode.Half[1]) {
                             case 0x47: Way=Home; break;
                             case 0x48: Way=Up  ; break;
                             case 0x49: Way=PgUp; break;
                             case 0x4B: Way=Left; break;
                             case 0x4D: Way=Right;break;
                             case 0x4F: Way=End;  break;
                             case 0x50: Way=Down; break;
                             case 0x51: Way=PgDn; break;
                           }
                        if (!ScaningShiftF) {
                           ScaningShiftF=True;
                           ErasedShiftF=False;
                           UnselectBlock();
                           switch(ScanCode.Half[1]) {
                             case 0x47: Way=Home; break;
                             case 0x48: Way=Up  ; break;
                             case 0x49: Way=PgUp; break;
                             case 0x4B: Way=Left; break;
                             case 0x4D: Way=Right;break;
                             case 0x4F: Way=End;  break;
                             case 0x50: Way=Down; break;
                             case 0x51: Way=PgDn; break;
                           }
                           EditAA();
                        }
           }
       }

    }


    if (Key==' ' && ShiftPressed() ) {
        ChangeLanguage();
        return;
    }
    else if (Way>31 && Way<128) {
        ED_AccessEng();
        if (!EditedF) EditedF=True,SetEditScn();
        return;
    }

    if (!EditedF)
       switch (Way) {
         case CtrlN    :
         case CtrlY    :
         case CtrlU    :
         case BackSpace:
         case CtrlG    :
         case Del      :
         case CtrlT    :
         case Return   :
         case Tab      :
         case F6       :
         case F7       :
         case F8       :
         case AltT     :
         case CtrlB    :EditedF=True;
                        SetEditScn();
       }
    if (HanResult==4)
        switch(Way) {
          case F1       :
          case F10      :
          case Tab      :
          case Return   :break;
          case Del    :
          case CtrlG  :SetCurPos(CurXPos()-2,NowLine);
                       break;
          default     :SetCurPos(CurXPos()-1,NowLine);
        }
    switch (Way) {
       case CtrlS    :
       case Left     :EditCS();    break;
       case CtrlD    :
       case Right    :EditCD();    break;
       case CtrlA    :
       case CtrlLeft :EditCA();    break;
       case CtrlF    :
       case CtrlRight:EditCF();    break;
       case CtrlE    :
       case Up       :EditCE();    break;
       case CtrlX    :
       case Down     :EditCX();    break;
       case CtrlW    :EditCW();    break;
       case CtrlZ    :EditCZ();    break;
       case CtrlR    :
       case PgUp     :EditCR();    break;
       case CtrlC    :
       case PgDn     :EditCC();    break;
       case CtrlQ    :EditCQ();    break;
       case Home     :EditHome();  break;
       case End      :EditEnd();   break;
       case CtrlHome :EditCHome(); break;
       case CtrlEnd  :EditCEnd();  break;
       case CtrlPgUp :EditCPgUp(); break;
       case CtrlPgDn :EditCPgDn(); break;
       case CtrlK    :EditCK();    break;
       case CtrlO    :EditCO();    break;
       case CtrlL    :EditCL();    break;
       case CtrlV    :
       case Ins      :EditIns();   break;
       case CtrlN    :EditCN();    break;
       case CtrlY    :EditCY();    break;
       case CtrlU    :EditCU();    break;
       case BackSpace:EditBackSpace(); break;
       case CtrlG    :
       case Del      :EditDel();   break;
       case CtrlT    :EditCT();    break;
       case Return   :EditReturn();break;
       case Tab      :EditTab();   break;
       case AltD     :EditDraw();  break;
       case Alt1     :if (PutEditBuf()==OK) ChangeTasks(0);
                      break;
       case Alt2     :if (PutEditBuf()==OK) ChangeTasks(1);
                      break;
       case Alt3     :if (PutEditBuf()==OK) ChangeTasks(2);
                      break;
       case AltN     :if (PutEditBuf()==OK)
                          switch (NowTask) {
                             case 0 : ChangeTasks(1); break;
                             case 1 : ChangeTasks(2); break;
                             case 2 : ChangeTasks(0);
                          }
                      break;
       case F6       :EditAQ();    break;
       case F7       :EditAW();    break;
       case F8       :EditAE();    break;
       case F1       :EditHelp();  break;
       case F9       :EditF9();    break;
       case AltG     :
       case F4       :EditF4();    break;
       case F10      :MainMenu();  break;
       case AltS     :SayBufStatus(); break;
       case AltR     :ResizeBuf(); break;
       case AltA     :EditAA();    break;
       case AltK     :EditAK();    break;
       case AltL     :EditAL();    break;
       case AltX     :PutEditBuf();
                      QuitWork();
                      break;
       case ESC      :CancelSelectBlock();
                      break;
       case AltT     :EditAT();
                      break;
       case CtrlB    :EditCB();
                      break;
       case F3       :LoadFile();
                      SetScnF=True;
                      break;
       case F2       :if (PutEditBuf()==OK) SaveFile();
                      SetEditBuf();
                      break;
       case AltW     :if (PutEditBuf()==OK) WriteTo();
                      SetEditBuf();
                      break;
       case AltF3    :if (PutEditBuf()==OK) SelectPick();
                      SetEditBuf();
                      break;
       case AltU     :EditCKH();
                      break;
       default       :SetHotKeyF();
    }
    while (HotKeyF) {
       if (SetScnF) {
           KeyInDisableF=OK;
           SetScn();
           KeyInDisableF=False;
       }
       MainMenu();
    }

}


void SetMenuScn(void);
void LoadTask(word Task);

/*#define FREE */

void Editor (void)
{
    word i;

#ifdef FREE

    InitTasks();
    SetMenuScn();
    HanResult=2;
    DisplayTitle();
    if (FileNameSetF) {
       strcpy(LoadPath,WorkFileName);
    } else {
       strcpy(LoadPath,"*.");
       if (StdExt[0]==0) strcat(LoadPath,"*");
       else              strcat(LoadPath,StdExt);
    }
    MemSetB(UnKillBuf,0,MaxUnKillBuf*4);
    i=FileNameSetF;
    strcpy(ReplaceString,WorkFileName);
    LoadTask(0);
    if ((FileNameSetF=i)==OK) {
        strcpy(WorkFileName,ReplaceString);
        ReplaceString[0]=0;
        Way=F3;
        SetHotKeyF();
        while (HotKeyF) {
           if (SetScnF) {
               KeyInDisableF=OK;
               SetScn();
               KeyInDisableF=False;
           }
           MainMenu();
        }
    } else if ((*PickList)[0].Name[0]!=0 && PrepareF) {
        PickFileF=OK;
        strcpy(WorkFileName,(*PickList)[0].Name);
        FileNameSetF=True;
        LoadFile();
    } else {
        CalledAtStartF=False;
        HClrBox(0,VideoBot,VideoRight,VideoBot);
        SetEditScn();
    }

#else

    InitTasks();
    SetMenuScn();
    HanResult=2;
    if (FileNameSetF) {
       strcpy(LoadPath,WorkFileName);
    } else {
       strcpy(LoadPath,"*.");
       if (StdExt[0]==0) strcat(LoadPath,"*");
       else              strcat(LoadPath,StdExt);
    }
    MemSetB(UnKillBuf,0,MaxUnKillBuf*4);
    i=FileNameSetF;
    strcpy(ReplaceString,WorkFileName);
    LoadTask(0);
    if ((FileNameSetF=i)==OK) {
        strcpy(WorkFileName,ReplaceString);
        ReplaceString[0]=0;
        Way=F3;
        SetHotKeyF();
        while (HotKeyF) {
           if (SetScnF) {
               KeyInDisableF=OK;
               SetScn();
               KeyInDisableF=False;
           }
           MainMenu();
        }
    } else if ((*PickList)[0].Name[0]!=0 && PrepareF) {
        PickFileF=OK;
        strcpy(WorkFileName,(*PickList)[0].Name);
        FileNameSetF=True;
        LoadFile();
    } else {
        CalledAtStartF=False;
        HClrBox(0,VideoBot,VideoRight,VideoBot);
        DisplayTitle();
        SetEditScn();
    }

#endif

    HanResult=5;
    SearchString[0]  =0;
    FindOption[0]    =0;
    ReplaceString[0] =0;
    do  {
       PrintXYpos();
       if (SetScnF) SetScn();
       else WriteLine(SY,EditBuf+1);
       HSX=(EditType[HX+SX-1]==2)?SX-1:SX;
       if (!HotKeyF && HanResult!=3)
          if (HanF)
               if ( HanResult==0 || HanResult==1 || EditType[HX+HSX-1]==1)
                    HanCur2(HSX-1,SY-1);
               else HanCur1(HSX-1,SY-1);
          else
               if ( EditType[HX+HSX-1]==1)
                    EngCur2(HSX-1,SY-1);
               else EngCur1(HSX-1,SY-1);
       if (!HotKeyF) GetHan();
       switch (HanResult) {
         case 0 :ED_CombiStart();
                 break;
         case 1 :ED_CombiNow();
                 break;
         case 2 :
         case 3 :ED_CombiComplete();
                 break;
         case 4 :ED_CombiComplete();
                 ED_AccessCommand();
                 break;
         case 5 :ED_AccessCommand();
                 break;
         case 6 :ED_CancelCombi();
       }
    } while ( Way!=Error );
}
