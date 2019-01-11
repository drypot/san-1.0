/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                  Environmemt & Menu                  */
/*                                                      */

#include "stddef.h"
#include "handrv.h"
#include "memory.h"
#include "const.h"
#include "edit.h"
#include "hwindow.h"
#include "crt.h"
#include "popup.h"
#include "filename.h"
#include "print.h"
#include "sanver.h"
#include <dir.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <io.h>

void  SetEditScn(void);
void SetMenuScn(void);
void EditDel(void);
word extern BlockSpace;

void OptionMenu(void);
void QuitWork(void);
void FileMenu(void);

void RunMenu(void)
{
    word i;
    word OKF;
    word static RunN;
    word ExitF;
    byte Buf[21];
    byte Buf2[80];

    if (HotKeyF==True) {
        RunN=(Way==F5) ? 0 :
             (Way==AltF6) ? 1 :
             (Way==AltF7) ? 2 :
             (Way==AltF8) ? 3 :
             (Way==AltF9) ? 4 : 0;
        HotKeyF=False;
        HotKeyEndF=True;
        Run(RunN);
        return;
    }
    Push(InverseF);
    HInverse();
    HOpenWindow(8,1,36,7);
    Pop(InverseF);
    Explain("¬a¶w¸a ·³b ¡ww. 'E' ‹i®AŸi ’Ÿa¯¡¡e ¶w ¥e‰w·¡ ˆa“w Ðs“¡”a.");
    do {
        Push(InverseF);
        HInverse();
        for (i=0 ; i<5 ; i++) {
          DrawString(10,i+2,RunTitle[i]);
          if (i==0) sprintf(Buf,"   F%d",i+5);
          else               sprintf(Buf,"AltF%d",i+5);
          DrawString(30,i+2,Buf);
        }
        Pop(InverseF);
        HSetInverse(9,RunN+2,28,RunN+2);
        GetKey();
        SetHotKeyF();
        HSetInverse(9,RunN+2,28,RunN+2);
        if (HotKeyF) break;
        ExitF=True;
        switch(Way) {
          case Up     :
          case CtrlE  :RunN=(RunN==0) ? 4:RunN-1;
                       ExitF=False;
                       break;
          case ' '    :
          case CtrlX  :
          case Down   :RunN=(RunN==4) ?0:RunN+1;
                       ExitF=False;
                       break;
          case PgDn   :
          case CtrlC  :
          case End    :RunN=4;
                       ExitF=False;
                       break;
          case PgUp   :
          case CtrlR  :
          case Home   :RunN=0;
                       ExitF=False;
                       break;
          case 'e'    :
          case 'E'    :
                if (HGetMes("ÀaŸ±ÌeµA Îa¯¡–I ¡ww· º¹AŸi °áº­A¶a.",RunTitle[RunN],18)!=OK) break;
                AppendSpace(RunTitle[RunN],18);
                Explain("%T TEXT-MODE   %P —¡BÉ¡Ÿ¡   %N ¢…¬á¡w    %E ÑÂ¸w¸a");
                HGetEng("¯©¹A ¯©Ð—Ða¯© •¡¯a ¡ww´áŸi ¸â´áº­A¶a.",RunItems[RunN],79);
                ExitF=False;
        }

    } while (!ExitF);
    HCloseWindow();
    if (Way==Return) Run(RunN),HotKeyEndF=True;
}


void TaskMenu(void)
{
    word OKF;
    word static TaskN;
    char *TaskList[]={
         "¸b´ó a”w(¢…¬áŸi ·ª´á—iµa ¸b´óÐa“e ‰bŸ¡–E µwµb)·i ¥e‰w Ðs“¡”a.",
         "”a·q ¸b´ó  a”w·a¡ ·¡•· Ðs“¡”a.",
         "¸b´ó  a”w· q·e Ça‹¡—w ¬wÈŸi ¥¡µa º““¡”a.",
         " a”w·i ¦ ¸â·a¡ ¸÷Ÿ¡ Ðs“¡”a.  ´¢ˆe· µa·A ‰·ˆe·¡ ¬—‹©® ·¶¯s“¡”a."
    };

    ExplainList=TaskList;
    Push(InverseF);
    HInverse();
    HOpenWindow(24,1,38,7);
    HOpenMenu();
       HSetItem (25,2," W ¸b´ó  a”w",13,'W');
       HSetItem (25,3," N ”a·q  a”w",13,'N');
       HDrawBar (25,37,4);
       HSetItem (25,5," S ¸b´ó ¬wÈ",13,'S');
       HSetItem (25,6," R  a”w ¸÷Ÿ¡",13,'R');
    Pop(InverseF);
    HManualSelect(TaskN);
    OKF=HSelect();
    TaskN=NMP->NowPos;
    HCloseWindow();
    HCloseMenu();
    if (OKF!=Error) {
      HotKeyEndF=True;
      switch (TaskN) {
        case 0 :HSayStatus(" ´á“a  a”w·a¡ ·¡•·ÐiŒa¶a? (1/2/3) ");
                GetKey();
                SetHotKeyF();
                HCloseWindow();
                switch (Way) {
                  case '1' :ChangeTasks(0); break;
                  case '2' :ChangeTasks(1); break;
                  case '3' :ChangeTasks(2); break;
                }
                break;
        case 1 :switch (NowTask) {
                  case 0 : ChangeTasks(1); break;
                  case 1 : ChangeTasks(2); break;
                  case 2 : ChangeTasks(0);
                }
                break;
        case 2 :SayBufStatus(); break;
        case 3 :ResizeBuf();    break;
        default:HotKeyEndF=False;
      }
    }
}

void BlockMenuSelect(void)
{
    word OKF;
    word static BlockNS;
    char *BlockList[]={
         "¢…¸a ”e¶á¡ §iœâ·i ¬é¸÷Ðs“¡”a.",
         "µi ”e¶á¡ §iœâ·i ¬é¸÷ Ðs“¡”a.",
         "Ð— ”e¶á¡ §iœâ·i ¬é¸÷ Ðs“¡”a.",
    };

       ExplainList=BlockList;
       HInverse();
       HOpenWindow(33,3,47,7);
       HOpenMenu();
          HSetItem (34, 4," A ·©¤e §iœâ",13,'A');
          HSetItem (34, 5," K Äéœñ §iœâ",13,'K');
          HSetItem (34, 6," L œa·¥ §iœâ",13,'L');
       HNormal();
       HManualSelect(BlockNS);
       OKF=HSelect();
       BlockNS=NMP->NowPos;
       HCloseWindow();
       HCloseMenu();
       HCloseWindow();
       HCloseMenu();
       if (OKF!=Error) {
              SetEditScn();
              PrintXYpos();
              switch(BlockNS) {
                case 0 :if (SelectPos("¯¡¸bÐi ¦¦…·a¡ ˆa­a¬á [RETURN] Ã¡­A¶a.")!=OK) break;
                        CancelSelectBlock();
                        EditAA();
                        if (SelectPos("§iœâ »¡¸÷·¡ {a¯¡¡e [Return] Ã¡­A¶a.")==OK)
                            SelectingBlockF=False ;
                        else CancelSelectBlock();
                        HotKeyEndF=False;
                        break;
                case 1 :if (SelectPos("¯¡¸bÐi ¦¦…·a¡ ˆa­a¬á [RETURN] Ã¡­A¶a.")!=OK) break;
                        CancelSelectBlock();
                        EditAK();
                        if (SelectPos("§iœâ »¡¸÷·¡ {a¯¡¡e [Return] Ã¡­A¶a.")==OK)
                            SelectingBlockF=False;
                        else CancelSelectBlock();
                        HotKeyEndF=False;
                        break;
                case 2 :if (SelectPos("¯¡¸bÐi ¦¦…·a¡ ˆa­a¬á [RETURN] Ã¡­A¶a.")!=OK) break;
                        CancelSelectBlock();
                        EditAL();
                        if (SelectPos("§iœâ »¡¸÷·¡ {a¯¡¡e [Return] Ã¡­A¶a.")==OK)
                            SelectingBlockF=False;
                        else CancelSelectBlock();
                        HotKeyEndF=False;
                        break;
              }

       }
}

void  EditCKE(void);
void  EditCKZ(void);
void  EditCKX(void);
void  EditCKD(void);

void BlockMenuDelete(void)
{
    word OKF;
    word static BlockND;
    char *BlockList[]={
         "§iœâ·i ¸iœa ¤áŸ³“¡”a.",
         "§iœâ µwµb· ¡¡—e ¢…¸aŸi »¡¶““¡”a.",
         "§iœâ µwµb· ¬å¢…¸aŸi ‰©œa¬á »¡¶““¡”a.",
         "§iœâ µwµbº— ¬å¢…¸aŸi ¹A¶AÐe ¡¡—e ¢…¸aŸi »¡¶““¡”a."
    };

       ExplainList=BlockList;
       HInverse();
       HOpenWindow(33,8,47,13);
       HOpenMenu();
          HSetItem (34, 9," Y ¸iœa ¤áŸ±",13,'Y');
          HSetItem (34,10," E »¡¶¡ ¤áŸ±",13,'E');
          HSetItem (34,11," Z ¬å e »¡¶‘",13,'Z');
          HSetItem (34,12," X ‹i¸a »¡¶‘",13,'X');
       HNormal();
       HManualSelect(BlockND);
       OKF=HSelect();
       BlockND=NMP->NowPos;
       HCloseWindow();
       HCloseMenu();
       HCloseWindow();
       HCloseMenu();
       if (OKF!=Error) {
          switch(BlockND) {
            case 0 : EditCKY();  break;
            case 1 : EditCKE();  break;
            case 2 : EditCKZ();  break;
            case 3 : EditCKX();  break;
          }
       }
}



void BlockMenu(void)
{
    word OKF;
    word static BlockN;
    char *BlockList[]={
         "§iœâ·i »¡¸÷Ðs“¡”a.",
         "Ñe¸ ¬é¸÷–E §iœâ Îa¯¡Ÿi Ða»¡ ´g‰A Ðs“¡”a.",
         "§iœâ·i ”aŸe ‰µµA ¥¢¬aÐs“¡”a.",
         "§iœâ·i ”aŸe ‰µ·a¡ ·¡•· ¯¡Ç³“¡”a.",
         "¬é¸÷–E §iœâ·i »¡¶¡ ¤áŸ³“¡”a.",
         "§iœâ ¸åÁAŸi ¹Á¶¡ ·¡•·¯¡Ç³“¡”a.",
         "§iœâ·a¡ ¬é¸÷–E ¦¦…·i ¶E½¢,µ¡Ÿe½¢,ˆa¶…•A¡ º‰ xÂ‘ Ðaˆáa ¢…”e ¸÷i Ðs“¡”a.",
         "”aŸe ¸b´ó  a”wµA ¬é¸÷–E §iœâ·i Ñe¸ ¸b´ó  a”w·a¡ ¥¢¬aÐ µ³“¡”a.",
         "Ñe¸ ¸b´ó  a”wµA ¬é¸÷–E §iœâ·i ”aŸe ¸b´ó  a”w· ¢…¬á –áµA ¦›·³“¡”a.",
         "¢…¬áŸi ·ª´á —iµa Ñe¸ ¢…¬á ¬a·¡µA ¬s·³Ða‰¡,•·¯¡µA §iœâ·a¡ ¬é¸÷Ðs“¡”a.",
         "§iœâ·a¡ »¡¸÷–E µwµb·i ¸á¸wÐs“¡”a.",
         "§iœâ·a¡ »¡¸÷–E µwµb·i ·¥­áÐs“¡”a."

    };


    do {
       ExplainList=BlockList;
       HInverse();
       HOpenWindow(32,1,46,18);
       HOpenMenu();
          HSetItem (33, 2," K §iœâ »¡¸÷",13,'K');
          HSetItem (33, 3," H Åa‰¡ a‹¡",13,'H');
          HDrawBar (33,45,4);
          HSetItem (33, 5," C ¥¢¬a"     ,13,'C');
          HSetItem (33, 6," M ·¡•·"     ,13,'M');
          HSetItem (33, 7," D »¡¶‘"     ,13,'D');
          HDrawBar (33,45,8);
          HSetItem (33, 9," I £©‰¡ ”w‹±",13,'I');
          HSetItem (33,10," O ¸÷i"     ,13,'O');
          HDrawBar (33,45,11);
          HSetItem (33,12," G ¤h‹¡"     ,13,'G');
          HSetItem (33,13," S ¥¡‹¡"   ,13,'S');
          HDrawBar (33,45,14);
          HSetItem (33,15," R ·ª‹¡"     ,13,'R');
          HSetItem (33,16," W ¸á¸w"     ,13,'W');
          HSetItem (33,17," P ·¥­á"     ,13,'P');
       HNormal();
       HManualSelect(BlockN);
       OKF=HSelect();
       BlockN=NMP->NowPos;
       if (OKF!=Error) {
           if (BlockN!=0 && BlockN!=4) {
              HCloseWindow();
              HCloseMenu();
           }
           if ( (BlockN==2 || BlockN==3  || BlockN==4  || BlockN==5 ||
                 BlockN==6 || BlockN==8 || BlockN==10 || BlockN==11 ) &
                 !ShowBlockF) {
                          HSayMes("  å¸á §iœâ·i ¬é¸÷Ð º­A¶a. ");
                          if (BlockN==4) {
                            HCloseWindow();
                            HCloseMenu();
                          }
           } else {
              if ( BlockN==2 || BlockN==3  || BlockN==4  || BlockN==5 ||
                   BlockN==6 || BlockN==7 || BlockN==9 )
                   EditedF=True;
              SetEditScn();
              PrintXYpos();
              HotKeyEndF=True;
              switch(BlockN) {
                case 0 :BlockMenuSelect();
                        HotKeyEndF=False;
                        break;
                case 1 :EditCKH(); break;
                case 2 :
                        if (SelectPos("´á—¡¡ ¥¢¬a ÐiŒa¶a?")==OK)
                            EditCKC();
                        break;
                case 3 :
                        if (SelectPos("´á—¡¡ µ«‹©Œa¶a?")==OK)
                            EditCKV();
                        break;
                case 4 :
                        BlockMenuDelete();
                        break;
                case 5 :
                        Explain("¹Á¶ ÑÁ¬iÎa ‹i®AŸi ·¡¶wÐa­A¶a.");
                        do {
                           if (SetScnF) SetScn();
                           if (HanF)
                                if ( HanResult==0 || HanResult==1 || EditType[HX+HSX-1]==1)
                                     HanCur2(HSX-1,SY-1);
                                else HanCur1(HSX-1,SY-1);
                           else
                                if ( EditType[HX+HSX-1]==1)
                                     EngCur2(HSX-1,SY-1);
                                else EngCur1(HSX-1,SY-1);
                           GetKey();
                           SetHotKeyF();
                           switch (Way) {
                             case Left   :
                             case CtrlS  : EditCKU();
                                           SetScnF=True;
                                           break;
                             case Right  :
                             case CtrlD  : EditCKI();
                                           SetScnF=True;
                                           break;
                           }
                        } while (Way!=ESC && Way!=Return && !HotKeyF);
                        break;
                case 6 :
                        EditCKL(); break;
                case 7 :HSayStatus("¡y ¤å¼ ¸b´ó  a”wµA¬á ·ª´á µ©Œa¶a? (1/2/3)");
                        GetKey();
                        SetHotKeyF();
                        HCloseWindow();
                        switch(Way) {
                          case '1' :GetBlock(0); break;
                          case '2' :GetBlock(1); break;
                          case '3' :GetBlock(2); break;
                        }
                        Way=Return;
                        break;
                case 8 :
                        HSayStatus("¡y ¤å¼ ¸b´ó  a”w·a¡ ¥¡‰Œa¶a? (1/2/3)");
                        GetKey();
                        SetHotKeyF();
                        HCloseWindow();
                        switch(Way) {
                          case '1' :SendBlock(0); break;
                          case '2' :SendBlock(1); break;
                          case '3' :SendBlock(2); break;
                        }
                        Way=Return;
                        break;
                case 9 :if (SelectPos("´á—¡µA”a ·ª´á ý·i Œa¶a?")==OK)
                            EditCKR();
                        break;
                case 10 :
                         EditCKW(); break;
                case 11 :
                         EditCKP(); break;
                default :HotKeyEndF=False;
              }
           }
       } else {
           HCloseWindow();
           HCloseMenu();
           break;
       }
    } while (HotKeyEndF==False && !HotKeyF);
}

void InsertMenu(void)
{
    word OKF;
    word static InsertN;
    char *is;
    char *InsertList[]={
       "§¥ º‰·i ¬s·³Ðs“¡”a.",
       "Äá¬á ¶áÃ¡¦Èá ¸iœa a ¬¡¶… Ð—·i  e—i´á º““¡”a.",
       "¬åÈ‚–E º‰·i »¡¶““¡”a.",
       "Äá¬á ¶áÃ¡¦Èá Ð—· {Œa»¡ »¡¶““¡”a.",
       "¢…¸a ”e¶á¡ »¡¶““¡”a.",
       "”e´á ”e¶á¡ »¡¶““¡”a.",
       "ÂA‹e »¡¶¡»¥ º‰¦Èá º‰·i ¬iŸ³“¡”a."
    };

    ExplainList=InsertList;
    Push(InverseF);
    HInverse();
    HOpenWindow(40,1,54,11);
    HOpenMenu();
    HSetItem (41,2 ," I §¥º‰ ¬s·³",13,'I');
    HSetItem (41,3 ," N Ð—   ¦…Ði",13,'N');
    HDrawBar (41,53,4);
    HSetItem (41,5 ," Y Ð—   »¡¶‘",13,'Y');
    HSetItem (41,6 ," Q –õº‰ »¡¶‘",13,'Q');
    HSetItem (41,7 ," G ‹i¸a »¡¶‘",13,'G');
    HSetItem (41,8 ," T ”e´á »¡¶‘",13,'T');
    HDrawBar (41,53,9);
    HSetItem (41,10," U ”a¯¡ ¬iŸ±",13,'U');
    Pop(InverseF);
    HManualSelect(InsertN);
    OKF=HSelect();
    InsertN=NMP->NowPos;
    HCloseWindow();
    HCloseMenu();
    if (OKF!=Error) {
        EditedF=True;
        SetEditScn();
        PrintXYpos();
        HotKeyEndF=True;
        switch(InsertN) {
           case 0 :HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"´á—¡µA ¬s·³ÐiŒa¶a?");
                   do {
                     if (SelectPos("´á—¡µA ¬s·³ÐiŒa¶a?")==OK) {
                        InsertBlankLine(NowLine-1);
                        if (NowLine<BlockSY) {
                            BlockSY++;
                            if (BlockEY!=0) BlockEY++;
                        } else if (NowLine<=BlockEY) BlockEY++;
                        SetEditBuf();
                        SetScnF=True;
                     } else break;
                   } while (True);
                   break;
           case 1 :
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"´á—¡µA¬á ¦…ÐiÐiŒa¶a?");
                   do {
                     if (SelectPos("´á—¡µA¬á ¦…ÐiÐiŒa¶a?")==OK) {
                        EditCN();
                     } else break;
                   } while (True);
                   break;
           case 2 :
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"´á“a Ð—·i »¡¶ºa?");
                   do {
                     if (SelectPos("´á“a Ð—·i »¡¶ºa?")==OK) {
                        EditCY();
                     }
                     else break;
                   } while (True);
                   break;
           case 3 :
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"»¡¶‰ ¦¦…·a¡ ˆa­A¶a.");
                   do {
                     if (SelectPos("»¡¶‰ ¦¦…·a¡ ˆa­A¶a.")==OK) {
                       EditCQY();
                     } else break;
                   } while (True);
                   break;
           case 4 :
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"»¡¶‰ ¢…¸a“e¶a?");
                   do {
                     if (SelectPos("»¡¶‰ ¢…¸a“e¶a?")==OK)
                       EditDel();
                     else break;
                   } while (True);
                   break;
           case 5 :
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"»¡¶‰ ”e´á“e¶a?");
                   do {
                     if (SelectPos("»¡¶‰ ”e´á“e¶a?")==OK)
                       EditCT();
                     else break;
                   } while (True);
                   break;
           case 6 :
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"¬iŸ© ¶áÃ¡¡ ˆa­A¶a.");
                   do {
                     if (SelectPos("¬iŸ© ¶áÃ¡¡ ˆa­A¶a.")==OK)
                        EditCU();
                     else break;
                   } while (True);
                   break;
           default:HotKeyEndF=False;
        }
    }
}

void InputMenu(void)
{
    word OKF;
    word static InputN;
    char *InputList[]={
         "Ðe‹i,´iÌa¥U ¬a·¡µA ·³b ¬wÈŸi ¸åÑÅ Ðs“¡”a.",
         "¬s·³ ‹¡“w·i Åa‰¡ s“¡”a.",
         "¸a•· º‰ xÂ‘ ‹¡“w·i Åa‰¡ s“¡”a.",
         "¬å¢…¸a ¥¡Ñ¡ ‹¡“w·i Åa‰¡ s“¡”a.",
         "‹©·¡ ¹AÐe ·³b ‹¡“w·i Åa‰¡ s“¡”a."
         };

    ExplainList=InputList;
    Push(InverseF);
    HInverse();
    HOpenWindow(48,1,66,7);
    HOpenMenu();
    HSetItem (49,2," E Ðe/µw    ¸åÑÅ",17,'E');
    HSetItem (49,3," I ¬s·³¡¡—a Åqq",17,'I');
    HSetItem (49,4," N —iµa³a‹¡ Åqq",17,'N');
    HSetItem (49,5," L ¬å¥¡Ñ¡   Åqq",17,'L');
    HSetItem (49,6," K ‹©·¡¹AÐe Åqq",17,'K');
    Pop(InverseF);
    HManualSelect(InputN);
    OKF=HSelect();
    InputN=NMP->NowPos;
    HCloseWindow();
    HCloseMenu();
    if (OKF!=Error) {
        HotKeyEndF=True;
        switch(InputN) {
           case 0 :ChangeLanguage(); break;
           case 1 :EditIns();        break;
           case 2 :EditCQI();        break;
           case 3 :EditCOL();        break;
           case 4 :EditCOK();        break;
           default:HotKeyEndF=False;
        }
    }
}

void FindMenu(void)
{
    word OKF;
    word static FindN;
    char *FindList[]={
         "»¡¸÷Ðe ¢…¸aµi·i Àx¯s“¡”a.",
         "¢…¸aµi ˆñ¬‚‰Á ÐqA Ã¡ÑÅÐs“¡”a.",
         "ÂA‹e ˆñ¬‚, Ã¡ÑÅ ¸b´ó·i ¤e¥¢Ðs“¡”a."
         };

    ExplainList=FindList;
    Push(InverseF);
    HInverse();
    HOpenWindow(56,1,72,5);
    HOpenMenu();
    HSetItem (57,2," F ¢…¸aµi Àx‹¡",15,'F');
    HSetItem (57,3," A ¢…¸aµi Ã¡ÑÅ",15,'A');
    HSetItem (57,4," L ¤e¥¢   ¸b´ó",15,'L');
    Pop(InverseF);
    HManualSelect(FindN);
    OKF=HSelect();
    FindN=NMP->NowPos;
    HCloseWindow();
    HCloseMenu();
    if (OKF!=Error) {
        EditedF=True;
        HotKeyEndF=True;
        switch(FindN) {
           case 0 :EditCQF(); break;
           case 1 :EditCQA(); break;
           case 2 :EditCL(); break;
           default:HotKeyEndF=False;
        }
    }
}

void SpecialMenu(void)
{
    word OKF;
    word static SpecialN;
    char *SpecialList[]={
         "¶õº‰· ›´á ³a‹¡µA ˜aœa Äá¬á ¶áÃ¡Ÿi ·¡•·Ðs“¡”a.",
         "Äá¬áˆa ”aŸe Ð—·a¡ ·¡•·Ða»¡ ´g·e ‰w¶µA ¸b´ó ¸å·a¡ Ð—·i ¥¢¶¥Ðs“¡”a.",
         "‹aŸ± ¢…¸aŸi ·³b Ðs“¡”a.",
         "¢…¬á ´·¯¢—w·i ¶áÐ ¬å‹aŸ¡‹¡ ‹¡“w·¡ »¡¶¥ –S“¡”a.",
         "Äéœñ §iœâ·a¡ »¡¸÷–E µwµb º¶áµA ÉA–Ÿ¡Ÿi ‹u¯s“¡”a.",
         "Ð—· ¯¡¸b·i 1µiµA  xÂ““¡”a.",
         "Ð—· ¶w·i 1µi‰Á 80µi· ˆa¶…•A ¶áÃ¡ ¯¡Ç³“¡”a.",
         "Ð—· {·i 80µiµA  xÂ““¡”a.",
         "¢…¬áµA Í¡Ðq–E TAB ¢…¸a—i·i ´i x·e ®· ‰·¤‚¢…¸a¡ Ã¡ÑÅ Ðs“¡”a.",
         "Ðe‹i·i ˆ{·e ·q·i ˆa»¥ Ðe¸a¡ ¥eÑÅ Ðs“¡”a."

    };

    ExplainList=SpecialList;
    Push(InverseF);
    HInverse();
    HOpenWindow(64,1,78,15);
    HOpenMenu();
    HSetItem (65,2 ," I ¶õº‰ ˜aŸq",13,'I');
    HSetItem (65,3 ," Q ¶¥œ Ð—  ",13,'Q');
    HDrawBar (65,77,4);
    HSetItem (65,5 ," G ‹aŸ± ¢…¸a",13,'G');
    HSetItem (65,6 ," D ¬å   ‹aŸ±",13,'D');
    HSetItem (65,7 ," O –‰œá °a‹¡",13,'O');
    HDrawBar (65,77,8);
    HSetItem (65,9 ," L ¹Á¡ ¸÷i",13,'L');
    HSetItem (65,10 ," C º—´w ¸÷i",13,'C');
    HSetItem (65,11," R ¶¡ ¸÷i",13,'R');
    HDrawBar (65,77,12);
    HSetItem (65,13," T TAB  ¥eÑÅ",13,'T');
    HSetItem (65,14," H Ðe¸a ¥eÑÅ",13,'H');
    Pop(InverseF);
    HManualSelect(SpecialN);
    OKF=HSelect();
    SpecialN=NMP->NowPos;
    HCloseWindow();
    HCloseMenu();
    if (OKF!=Error) {
        EditedF=True;
        HotKeyEndF=True;
        switch(SpecialN) {
           case 0 :EditTab(); break;
           case 1 :EditCQL(); break;
           case 2 :EditF4();  break;
           case 3 :SetEditScn();
                   PrintXYpos();
                   EditDraw();  break;
           case 4 :if (!ShowBlockF) {
                      HSayMes("  å¸á Äéœñ §iœâ·i ¬é¸÷Ð º­A¶a. ");
                      break;
                   }
                   EditedF=True;
                   EditCKD(); break;
           case 5 :EditAQ();  break;
           case 6 :EditAW();  break;
           case 7 :EditAE();  break;
           case 8 :EditAT();  break;
           case 9 :SetEditScn();
                   PrintXYpos();
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"¤aŽ‰ Ðe‹i·e¶a?");
                   do {
                     if (SelectPos("¤aŽ‰ Ðe‹i·e¶a?")==OK)
                       EditF9(),EditedF=True;
                     else break;
                   } while (True);
                   break;
           default:HotKeyEndF=False;
        }
    }
}

void DisplayTitle(void);

void MainMenu(void)
{
    word FulledF=False;
    word static EditMenuN;
    char *is;

    if (HotKeyF==True) {
       switch (Way) {
         case F10   : break;
         case F2    :
         case F3    :
         case AltF3 :
         case AltW  :
         case AltX  :
         case AltF  : EditMenuN=0;  break;
         case AltC  :
         case F5    :
         case AltF6 :
         case AltF7 :
         case AltF8 :
         case AltF9 : EditMenuN=1;  break;
         case AltO  : EditMenuN=2;  break;
         case AltY  : EditMenuN=3;  break;
         case AltB  : EditMenuN=4;  break;
         case AltI  : EditMenuN=5;  break;
         case AltE  : EditMenuN=6;  break;
         case AltH  : EditMenuN=7;  break;
         case AltP  : EditMenuN=8;  break;
       }
       if   (  Way==AltF ||
               Way==AltO ||
               Way==AltC ||
               Way==AltY ||
               Way==AltB ||
               Way==AltI ||
               Way==AltE ||
               Way==AltH ||
               Way==AltP ||
               Way==F10
               )  HotKeyF=False;
       if (Way==F10) {
          Item=0;
       } else {
          Item=999;
          FulledF=True;
       }
    } else Item=0;
    do {
      if (Item!=999 && !FulledF)
          do {
             switch (EditMenuN) {
               case 0 : is="¢…¬á(FILE)Ÿi ‰ÅŸ¡Ðaˆáa •¡¯aµA ‰Åe–E ¸b´ó·¡ ·¡ž´á »³“¡”a.";
                        break;
               case 1 : is="•¡¯a· ¡wwÐ—µA¬á ¬a¶w ˆa“wÐe ¡¡—e ¡ww·¡ ¬a¶w¸a »¡¸÷µA ·Ð ·¡ž´á »³“¡”a.";
                        break;
               case 2 : is="¬a¶w¸a· ·³ uµA  x•¡¢ ¡yˆa»¡ ‹¡“w·i ¹¡¹·Ð º¯©® ·¶¯s“¡”a.";
                        break;
               case 3 : is="•·¯¡µA ¸b´óº—·¥ ¢…¬á—iˆeµA ·¡•·‰Á ¬wÈ Îa¯¡Ÿi Ðs“¡”a";
                        break;
               case 4 : is="§iœâ· ¬é¸÷‰Á ¬é¸÷–E §iœâ—iµA ‰Åe–E ¸b´ó·¡ ·¡ž´á »³“¡”a.";
                        break;
               case 5 : is="¶w·i ¡¶¡³aˆáa »¡¶ˆáa »¡¶¡»¥ ¶w·i ¬iŸ¡“e—w· ‹¡“w·¡ ¡¡µa ·¶·s“¡”a.";
                        break;
               case 6 : is="¬a¶w¸a· ·³bµA ‰Åe–E µaœá ‹¡“w·i ‰ÅŸ¡ Ðs“¡”a.";
                        break;
               case 7 : is="¶¥Ða“e ¶w·i Àxˆáa Àx·e ¢…¸aµi·i Ã¡ÑÅÐa“e ‹¡“w·¡ ·¶¯s“¡”a.";
                        break;
               case 8 : is="‹aŸ±¢…¸a ³a‹¡, ¬å‹aŸ¡‹¡, ¸÷i—w ‹¡Èa ‹¡“w·¡ ·¶¯s“¡”a.";
                        break;
               case 9 : is=" ¬e " SanVersionNum;
             }
             Explain(is);
             if (EditMenuN==9) HSetInverse(EditMenuN*8+1,0,EditMenuN*8+6,0);
             else              HSetInverse(EditMenuN*8+1,0,EditMenuN*8+8,0);
             GetKey();
             if (EditMenuN==9) HSetInverse(EditMenuN*8+1,0,EditMenuN*8+6,0);
             else              HSetInverse(EditMenuN*8+1,0,EditMenuN*8+8,0);
             SetHotKeyF();
             if (Way==ESC || HotKeyF) break;
             switch (Way) {
                case ' '   :
                case CtrlD :
                case Right : EditMenuN++;
                             if (EditMenuN==10) EditMenuN=0;
                             break;
                case CtrlS :
                case Left  : EditMenuN--;
                             if (EditMenuN>8) EditMenuN=9;
                             break;
                case CtrlR :
                case Home  : EditMenuN=0;
                             break;
                case CtrlC :
                case End   : EditMenuN=9;
                             break;
                case CtrlX :
                case Down  :
                case Return:FulledF=True;
                            break;
                case 'f'   :
                case 'F'   : EditMenuN=0;
                             FulledF=True;
                             break;
                case 'c'   :
                case 'C'   : EditMenuN=1;
                             FulledF=True;
                             break;
                case 'o'   :
                case 'O'   : EditMenuN=2;
                             FulledF=True;
                             break;
                case 'y'   :
                case 'Y'   : EditMenuN=3;
                             FulledF=True;
                             break;
                case 'b'   :
                case 'B'   : EditMenuN=4;
                             FulledF=True;
                             break;
                case 'i'   :
                case 'I'   : EditMenuN=5;
                             FulledF=True;
                             break;
                case 'e'   :
                case 'E'   : EditMenuN=6;
                             FulledF=True;
                             break;
                case 'h'   :
                case 'H'   : EditMenuN=7;
                             FulledF=True;
                             break;
                case 'p'   :
                case 'P'   : EditMenuN=8;
                             FulledF=True;
                             break;
                default    : if (Way>='1' && Way<='0') {
                               EditMenuN=Way-'0'-1;
                               FulledF=True;
                             }
                             break;
             }
          } while (!FulledF);
      else Item=0;
      if (Way!=ESC) {
          if (EditMenuN==9) HSetInverse(EditMenuN*8+1,0,EditMenuN*8+6,0);
          else              HSetInverse(EditMenuN*8+1,0,EditMenuN*8+8,0);
          switch (EditMenuN) {
            case 0 :FileMenu();     break;
            case 1 :RunMenu();      break;
            case 2 :OptionMenu();   break;
            case 3 :TaskMenu();     break;
            case 4 :BlockMenu();    break;
            case 5 :InsertMenu();   break;
            case 6 :InputMenu();    break;
            case 7 :FindMenu();     break;
            case 8 :SpecialMenu();  break;
            case 9 :if (Way==Return || Way==Down || Way==CtrlX ) {
                       DisplayTitle();
                       HotKeyEndF=True;
                    }
                    else {
                       Explain(" ¬e " SanVersionNum);
                       GetKey();
                       SetHotKeyF();
                    }
                    break;

          }
          SetMenuScn();
      }
      switch (Way) {
         case CtrlD:
         case Right: EditMenuN++;
                     if (EditMenuN==10) EditMenuN=0;
                     break;
         case CtrlS:
         case Left : EditMenuN--;
                     if (EditMenuN>8) EditMenuN=9;
                     break;
         case ESC  : HotKeyEndF=True;
                     break;
         default   : if (Way>='1' && Way<='0')
                       EditMenuN=Way-'0'-1;
                     break;
      }
    } while (!HotKeyF && !HotKeyEndF && Way!=ESC);
    HotKeyEndF=False;
    if (ChangeFontF) {
        ChangeFontF=False;
        FoolSetCur(HX+SX-1,NowLine);
        SetMenuScn();
        SetEditScn();
        PrintXYpos();
    } else {
        SetEditScn();
        PrintXYpos();
    }
    SetScnF=True;
}

