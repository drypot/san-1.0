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


void QuitWork(void)
{
     word OKF;

     HotKeyF=False;
     if (Task[0].EditedF || Task[1].EditedF || Task[2].EditedF || EditedF)  {
         OKF=OK;
         if (HGetYesNo ("¸á¸w–A»¡ ´g·e ¢…¬áˆa ¹¥¸ Ðs“¡”a. ¡¡– ¸á¸wÐiŒa¶a?",&OKF)==OK)
             if (OKF) {
                if (SaveAll()==OK) Way=Error;
             } else Way=Error;
     } else Way=Error;
     if (Way==Error) HotKeyEndF=True;
}

void FileMenu(void)
{
    word OKF;
    word static FileMenuN;
    char* FileExplain[]={
          "¢…¬áŸi ·ª´á —i·³“¡”a.",
          "‰ÁˆáµA ¸b´óÐ–”å ¢…¬áº—µA¬á ·ª´á —i·© ¢…¬áŸi ¸÷Ðs“¡”a.",
          "Ñe¸ ¸b´ó  a”w· ¢…¬á e ¸á¸w Ðs“¡”a.",
          "¡¡—e ¸b´ó  a”w· ¢…¬áŸi ¸á¸w Ðs“¡”a.",
          "Ñe¸ ¸b´ó  a”w·i À÷­¡Ða‰¡ ¬¡¶… ¢…¬á ¸b´ó·i ¯¡¸bÐs“¡”a.",
          "¸b´ó º—·¥ ¢…¬á¡w·i ¥e‰wÐq‰Á •·¯¡µA ¸á¸wÐs“¡”a.",
          "¶¥Ða“e —¡BÉ¡Ÿ¡ · ÑÁ·© ¡¢¢·i ¥¡µa —aŸ³“¡”a.",
          "¸b´ó —¡BÉ¡Ÿ¡Ÿi ¥e‰wÐs“¡”a.",
          "¢…¬áŸi ÏaŸ¥ÈáµA¬á ·¥­á Ðs“¡”a.",
          "¸q¯¡ •¡¯a¡ aˆs“¡”a. •©´aµ¡¯© ”“e 'EXIT' ¡ww·i ¬a¶wÐa­A¶a.",
          "¡¡—e ¸b´ó·i {““¡”a. ¸á¸wÐa»¡ ´g·a¯¥ ¢…¬áˆa ¹¥¸Ða¡e ÑÂ·¥Ða‰A –S“¡”a."
    };

    ExplainList=FileExplain;
    Push(InverseF);
    HInverse();
    HOpenWindow(0,1,19,14);
    HOpenMenu();
       HSetItem(1, 2," L ¢…¬á ·ª‹¡   F3",18,'L');
       HSetItem(1, 3," P µ•   ¢…¬á A-F3",18,'P');
       HSetItem(1, 4," S ¢…¬á ¸á¸w   F2",18,'S');
       HSetItem(1, 5," A ¡¡– ¸á¸w"     ,18,'A');
       HSetItem(1, 6," N ¬   ¢…¬á"     ,18,'N');
       HSetItem(1, 7," W ¬   ·¡Ÿq A-W" ,18,'W');
       HDrawBar(1,18,8);
       HSetItem(1, 9," V ÑÁ·© ¡¢¢"     ,18,'V');
       HSetItem(1,10," C ‰w¡ ¥e‰w"     ,18,'C');
       HSetItem(1,11," I ¢…¬á ·¥­á"     ,18,'I');
       HSetItem(1,12," D •¡¯a ·¡¶w"     ,18,'D');
       HSetItem(1,13," Q ¸b´ó ¹·ža A-X" ,18,'Q');
    Pop(InverseF);
    if (HotKeyF==True) {
        FileMenuN=(Way==F2   )? 2 :
                  (Way==F3   )? 0 :
                  (Way==AltF3)? 1 :
                  (Way==AltW )? 5 :
                  (Way==AltX )? 10: 0;
        HotKeyF=False;
        HotKeyEndF=True;
        Item=999;
    }  else Item=0;
    HManualSelect(FileMenuN);
    do {
      if (Item!=999) {
          OKF=HSelect();
          FileMenuN=NMP->NowPos;
          if (OKF==Error) break;
      } else Item=0;
      SetEditScn();
      PrintXYpos();
      switch (FileMenuN) {
        case 0 :OKF=LoadFile();
                if (OKF==OK) HotKeyEndF=True;
                break;
        case 1 :if (PutEditBuf()==OK) OKF=SelectPick();
                if (OKF==OK) HotKeyEndF=True;
                break;
        case 2 :if (PutEditBuf()==OK) SaveFile();
                HotKeyEndF=True;
                break;
        case 3 :if (PutEditBuf()==OK) SaveAll();
                HotKeyEndF=True;
                break;
        case 4 :NewFile();
                HotKeyEndF=True;
                SetScnF=True;
                break;
        case 5 :if (PutEditBuf()==OK) WriteTo();
                HotKeyEndF=True;
                break;
        case 6 :Dir();
                break;
        case 7 :ChangeDir();
                break;
        case 8 :PrintFile();
                break;
        case 9 :if (OsSaveF) {
                   OKF=OK;
                   if (HGetYesNo("ÑÁ·©—i·i ·©”e ¡¡– ¸á¸wÐ ‘½·iŒa¶a?",&OKF)==OK) {
                      if (OKF==OK) if (PutEditBuf()!=OK || SaveAll()!=OK) break;
                   } else break;
                }
                if (PutEditBuf()==OK) {
                   int Status;

                   TextMode();
                   clrscr();
                   puts("Type 'Exit' to return to SAN.");
                   Status=system("");
                   GrMode();
                   if (Status==-1) HSayIOErr();
                   HClrScr();
                   SetMenuScn();
                   SetEditScn();
                   PrintXYpos();

                   SetScnF=True;
                   SetEditBuf();
                }
                break;
        case 10:PutEditBuf();
                QuitWork();
      }
      HotKeyEndF=True;
    } while (!HotKeyF && !HotKeyEndF);
    HCloseWindow();
    HCloseMenu();
}