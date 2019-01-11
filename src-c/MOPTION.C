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
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <io.h>

word extern BlockSpace;

        void  SelectKeyboard(void)
        {
              word i;
              word OKF;
              char *KeyboardList[]={
                   "¥¡É· ¬a¶wÐa‰¡ ‰¯¥ ¶E­¥ ¸s·¡¶w Ðe‹i ¸aÌe·¡ –¤é¯¢ ·³“¡”a.",
                   "Àá·q,ˆa¶…•A,{ ­¡Ÿ¡¶w ‹i®Aˆa ¦…Ÿ¡–A´á ·¶“e Ðe‹i ¸aÌe·¡ ­A¤é¯¢ ·³“¡”a."
                   };

              ExplainList=KeyboardList;
              HInverse();
              HOpenWindow(17,6,28,9);
              HOpenMenu();
              HSetItem(18,7," D –¤é¯¢",10,'D');
              HSetItem(18,8," S ­A¤é¯¢",10,'S');
              HNormal();
              i=(KeyboardType==2)?0:1;
              HManualSelect(i);
              OKF=HSelect();
              if (OKF==OK) HotKeyEndF=True;
              HCloseMenu();
              HCloseWindow();
              if (OKF==OK) KeyboardType=(Item==0)?2:3;
        }

        void   BlockOption(void)
        {
               word OKF;
               int  i;
               char Buf[10];
               char Left[10],Right[10],Indent[10],SpaceLine[10];
               char *BlockOptionList[]={
                    "¶E½¢ ‹¡º…µi·i ¸÷Ðs“¡”a.",
                    "µ¡Ÿe½¢ ‹¡º…µi·i ¸÷Ðs“¡”a.",
                    "¢…”e Ñw¯¢ ¸÷i¯¡ ¢…”e· Àõº‰· —iµa³a‹¡ ¢…¸a®Ÿi ¸÷Ðs“¡”a.",
                    "¢…”e Ñw¯¢ ¸÷i¯¡ º‰‰Á º‰¬a·¡· §¥º‰ ˆ•® ·³“¡”a.",
                    "¢…”e Ñw¯¢ ¸÷i· ”e¶áŸi ¸÷Ðs“¡”a."
                    };

               ExplainList=BlockOptionList;
               Push(InverseF);
               HInverse();
               HOpenWindow(17,8,49,14);
               HOpenMenu();
               HSetItem(18, 9," L §iœâ ¶E½¢   ¶áÃ¡ :",20,'L');
               HSetItem(18,10," R §iœâ µ¡Ÿe½¢ ¶áÃ¡ :",20,'R');
               HSetItem(18,11," I —iµa ³a‹¡   ¸a® :",20,'I');
               HSetItem(18,12," S º‰‰Á º‰¬a·¡ §¥º‰ :",20,'S');
               HSetItem(18,13," T ¸÷i ¤w¯¢        :",20,'T');
               Pop(InverseF);
               do {
                  HInverse();
                  switch (BlockSortType) {
                    case 0 : DrawString(40,13,"¢…¸a”e¶á");
                             break;
                    case 1 : DrawString(40,13,"´á¸é”e¶á");
                             break;
                    case 2 : DrawString(40,13,"¸aŸq´á¸é");
                  }
                  sprintf(Left,"%u",BlockLeft);
                  strcpy(Buf,Left); strcat(Buf,"   ");
                  DrawString(40, 9,Buf);
                  sprintf(Right,"%u",BlockRight);
                  strcpy(Buf,Right); strcat(Buf,"   ");
                  DrawString(40,10,Buf);
                  sprintf(Indent,"%d",(int)BlockIndent);
                  strcpy(Buf,Indent); strcat(Buf,"   ");
                  DrawString(40,11,Buf);
                  sprintf(SpaceLine,"%u",BlockSpace);
                  strcpy(Buf,SpaceLine); strcat(SpaceLine,"   ");
                  DrawString(40,12,Buf);
                  HNormal();
                  OKF=HSelect();
                  if (OKF!=OK || HotKeyF) break;
                  switch (Item) {
                    case 0 :if (HGetEng("¶E½¢ ¶áÃ¡Ÿi °áº­A¶a.",Left,5)==OK) {
                                i=atoi(Left);
                                if (i<1) HSayMes(" 1 ¥¡”a“e Äá´¡ ‰V“e•A¶a.");
                                else if (i>BlockRight-5) HSayMes(" µ¡Ÿe½¢ {‰Á §¡ŠaÐ á¢ Çe®·³“¡”a.");
                                else if ((int)(BlockRight-i)<(int)(BlockIndent)+5) HSayMes("—iµa³a‹¡ ®µA §¡Ð¬á Çe®·³“¡”a.");
                                else BlockLeft=i;
                            }
                            break;
                    case 1 :if (HGetEng("µ¡Ÿe½¢ ¶áÃ¡Ÿi °áº­A¶a.",Right,5)==OK) {
                                i=atoi(Right);
                                if (i>254) HSayMes(" 254 ¥¡”a“e ¸b´a´¡ Ðs“¡”a.");
                                else if (i<BlockLeft+5) HSayMes(" ¶E½¢{‰Á §¡ŠaÐ á¢ Çe®·³“¡”a.");
                                else if ((int)(i-BlockLeft)<(int)(BlockIndent)+5) HSayMes("—iµa³a‹¡ ®µA §¡Ð¬á Çe®·³“¡”a.");
                                else BlockRight=i;
                            }
                            break;
                    case 2 :if (HGetEng("¡yÄe·¡a —iµa ³iŒa¶a?",Indent,5)==OK) {
                                i=atoi(Indent);
                                if ((int)i>(int)(BlockRight-BlockLeft-5)) HSayMes(" ´·½¢{‰Á §¡ŠaÐ á¢ Çe® ·³“¡”a.");
                                else BlockIndent=i;
                            }
                            break;
                    case 3 :if (HGetEng("¡y º‰·¡a ›¶‰Œa¶a?",SpaceLine,5)==OK) {
                                i=atoi(SpaceLine);
                                if (i>10) HSayMes(" ¹± Ça”a‰¡ ¬—ˆb»¡ ´g·a¯³“¡Œa? 10¥¡”e ¸b´v·a¡e ¹½‰V“e•A¶a.");
                                else BlockSpace=i;
                            }
                            break;
                    case 4 :BlockSortType=(BlockSortType+1)%3;
                  }
               } while (!HotKeyF);
               if (Way!=ESC) HotKeyEndF=True;
               HCloseMenu();
               HCloseWindow();
        }

             void   PrinterSort(void)
             {
                    word OKF;
                    int  i;
                    char **BufList;
                    char *PrinterSortList[]={
                         "¹¡ÐsÑw Ðe‹i Å¡—a¡ ¤a¡ Â‰bÐs“¡”a.",
                         "µÅ¬÷Ñw Ðe‹i Å¡—a¡ ¤a¡ Â‰bÐs“¡”a.",
                         "¹¡ÐsÑw Ðe‹i Å¡—a¡ ¤a¡ Â‰bÐs“¡”a. Ð—ˆe ‰·¤‚ Ça‹¡ ¹¡¹··¡ ˆa“wÐs“¡”a.",
                         "µÅ¬÷Ñw Ðe‹i Å¡—a¡ ¤a¡ Â‰bÐs“¡”a. Ð—ˆe ‰·¤‚ Ça‹¡ ¹¡¹··¡ ˆa“wÐs“¡”a.",
                         "ÑÁ¡e ¸aÑw·a¡ ·¥­á Ðs“¡”a.  8Ï¥ •¡Ëa ÏaŸ¥Èá",
                         "ÑÁ¡e ¸aÑw·a¡ ·¥­á Ðs“¡”a. 24Ï¥ •¡Ëa ÏaŸ¥Èá",
                         "ÑÁ¡e ¸aÑw·a¡ ·¥­á Ðs“¡”a. A·¡¹a ¹U/´aº ¸b‰A",
                         "ÑÁ¡e ¸aÑw·a¡ ·¥­á Ðs“¡”a. A·¡¹a ¹U/¸b‰A",
                         "ÑÁ¡e ¸aÑw·a¡ ·¥­á Ðs“¡”a. A·¡¹a ¹U/¥¡É·",
                         "ÑÁ¡e ¸aÑw·a¡ ·¥­á Ðs“¡”a. A·¡¹a ¹U/Ça‰A"
                         };

                    BufList=ExplainList;
                    ExplainList=PrinterSortList;
                    Push(InverseF);
                    HInverse();
		    HOpenWindow(31,8,50,19);
		    HOpenMenu();
		    HSetItem(32, 9," Normal     /¹¡Ðs",18,0);
		    HSetItem(32,10," Normal     /µÅ¬÷",18,0);
		    HSetItem(32,11," EPSON      /¹¡Ðs",18,0);
		    HSetItem(32,12," EPSON      /µÅ¬÷",18,0);
		    HSetItem(32,13," EPSON 8pin /‹aŸ±",18,0);
		    HSetItem(32,14," EPSON 24pin/‹aŸ±",18,0);
		    HSetItem(32,15," Jet 300DPI /‹aŸ±",18,0);
		    HSetItem(32,16," Jet 150DPI /‹aŸ±",18,0);
		    HSetItem(32,17," Jet 100DPI /‹aŸ±",18,0);
		    HSetItem(32,18," Jet  75DPI /‹aŸ±",18,0);
                    Pop(InverseF);

                    HNormal();
                    OKF=HSelect();
                    ExplainList=BufList;
                    if (OKF==OK && !HotKeyF) PrintingType=Item;
                    else if (Way==ESC) Way=Error;
                    HCloseMenu();
                    HCloseWindow();
             }

        void   PrinterOption(void)
        {
               word OKF;
               int  i;
               char Left[10];
               char Page[10];
               char Top [10];
               char Bot [10];
               char Port[10];
               char Space[10];
               char *PrinterOptionList[]={
                    "ˆb º‰· ·¥­á ¯¡¸b µi·i ¸÷Ðs“¡”a.",
                    "¹··¡· º‰ ®Ÿi °áº­A¶a.",
                    "¹··¡· ¡y ¤å¼ º‰¦Èá ·¥­áŸi ÐiŒa¶a?",
                    "¹··¡· ¡y ¤å¼ º‰Œa»¡ ·¥­áŸi ÐiŒa¶a?",
                    "º‰‰Á º‰¬a·¡ ˆe‰b·i ´á“a¸÷•¡¡ ÐiŒa¶a?",
                    "¬a¶wÐa‰¡ ‰¯¥ ÏaŸ¥ÈáµA ´i x‰A  xÂ¡ º­A¶a.",
                    "¬a¶wÐa‰¡ ‰¯¥ ÏaŸ¥ÈáµA ´i x‰A  xÂ¡ º­A¶a.",
                    "ÏaŸ¥ÈáµÁ µe‰i–A´á ·¶“e ¥wiÍ¡Ëa· ¤åÑ¡Ÿi °áº­A¶a."
                    };

               ExplainList=PrinterOptionList;
               Push(InverseF);
               HInverse();
               HOpenWindow(17,9,49,17);
               HOpenMenu();
               HSetItem(18,10," L ¯¡¸b µi   ",13,'L');
               HSetItem(18,11," G ¹··¡ Ça‹¡ ",13,'G');
               HSetItem(18,12," T ¯¡¸b Ð—   ",13,'T');
               HSetItem(18,13," B  aÃ± Ð—   ",13,'B');
               HSetItem(18,14," S ¬a·¡ ‰·¤‚ ",13,'S');
               HSetItem(18,15," R ·¥­á ¤w¯¢ ",13,'R');
               HSetItem(18,16," P ¥wi Í¡Ëa ",13,'P');
               Pop(InverseF);
               do {
                  HInverse();
                  switch (PrintingType) {
                    case 0 : DrawString(32,15,"Normal     /¹¡Ðs"); break;
                    case 1 : DrawString(32,15,"Normal     /µÅ¬÷"); break;
                    case 2 : DrawString(32,15,"EPSON      /¹¡Ðs"); break;
                    case 3 : DrawString(32,15,"EPSON      /µÅ¬÷"); break;
                    case 4 : DrawString(32,15,"EPSON 8pin /‹aŸ±"); break;
                    case 5 : DrawString(32,15,"EPSON 24pin/‹aŸ±"); break;
                    case 6 : DrawString(32,15,"Jet 300DPI /‹aŸ±"); break;
                    case 7 : DrawString(32,15,"Jet 150DPI /‹aŸ±"); break;
                    case 8 : DrawString(32,15,"Jet 100DPI /‹aŸ±"); break;
                    case 9 : DrawString(32,15,"Jet  75DPI /‹aŸ±"); break;
                  }
                  sprintf(Left,"%u",LeftMargin);
                  HClrBox(32,10,35,10);
                  DrawString(32,10,Left);

                  sprintf(Page,"%u",PageLength);
                  HClrBox(32,11,35,11);
                  DrawString(32,11,Page);

                  sprintf(Top,"%u",TopMargin);
                  HClrBox(32,12,35,12);
                  DrawString(32,12,Top);

                  sprintf(Bot,"%u",BotMargin);
                  HClrBox(32,13,35,13);
                  DrawString(32,13,Bot);

                  sprintf(Space,"%u",SpaceLength);
                  HClrBox(32,14,35,14);
                  DrawString(32,14,Space);

                  sprintf(Port,"%u",PrinterPort);
                  DrawString(32,16,Port);

                  HNormal();
                  OKF=HSelect();
                  if (OKF!=OK || HotKeyF) break;
                  switch (Item) {
                    case 0 :if (HGetEng("¶E½¢ ¯¡¸b ¶áÃ¡Ÿi ¸â´áº­A¶a.",Left,5)==OK) {
                                i=atoi(Left);
                                if (i<1) HSayMes(" 0 ¥¡”a“e Äá´¡ ‰V“e•A¶a.");
                                else if (i>300) HSayMes(" á¢ Çe ®Ã¡ ´a“³“¡Œa? ”a¯¡ °áº­A¶a.");
                                else LeftMargin=i;
                            }
                            break;
                    case 1 :if (HGetEng("¹··¡ ‹©·¡ˆa ¡y º‰·¥ˆa¶a?.",Page,5)==OK) {
                                i=atoi(Page);
                                if (i<1) HSayMes(" 0 ¥¡”a“e Äá´¡ ‰V“e•A¶a.");
                                else if (i>400) HSayMes(" á¢ Çe ®Ã¡ ´a“³“¡Œa? ”a¯¡ °áº­A¶a.");
                                else PageLength=i;
                            }
                            break;
                    case 2 :if (HGetEng("·¥­áŸi ¯¡¸bÐi º‰·i ¸â´áº­A¶a.",Top,5)==OK) {
                                i=atoi(Top);
                                if (i<1) HSayMes(" 0 ¥¡”a“e Äá´¡ ‰V“e•A¶a.");
                                else if (i>PageLength) HSayMes(" ¹··¡ ‹©·¡¥¡”a Çe ®Ã¡ ·³“¡”a. ”a¯¡ °áº­A¶a.");
                                else TopMargin=i;
                            }
                            break;
                    case 3 :if (HGetEng("¡y¤å¼ º‰Œa»¡ ·¥­á ÐiŒa¶a?",Bot,5)==OK) {
                                i=atoi(Bot);
                                if (i<1) HSayMes(" 0 ¥¡”a“e Äá´¡ ‰V“e•A¶a.");
                                else if (i>PageLength) HSayMes(" ¹··¡ ‹©·¡¥¡”a Çe ®Ã¡ ·³“¡”a. ”a¯¡ °áº­A¶a.");
                                else BotMargin=i;
                            }
                            break;
                    case 4 :if (HGetEng("º‰ˆe Ça‹¡Ÿi ´á“a¸÷•¡¡ ÐiŒa¶a? ",Space,5)==OK) {
                                i=atoi(Space);
                                if (i<0) HSayMes(" 0 ·¡¬w ·¡µa´¡ Ðs“¡”a.");
                                else if (i>200) HSayMes(" á¢ Çe ®Ã¡ ˆ{¯s“¡”a.");
                                else SpaceLength=i;
                            }
                            break;

                    case 5 :PrinterSort();

                            /*PrintingType++;
                            if ( PrintingType >9 ) PrintingType=0;*/

                            break;
                    case 6:if (PrinterPort==1) PrinterPort=2;
                           else PrinterPort=1;
                           break;
                  }
               } while (!HotKeyF);
               if (Way!=ESC) HotKeyEndF=True;
               HCloseMenu();
               HCloseWindow();
        }


        void SetEngFont(void);
        void SetHanFont(void);

        /*void InverseScreen(void)
        {
             HInverse();
             Way=AltO;
             SetHotKeyF();
             SetEditScn();
             PrintXYpos();
             SetMenuScn();
             HNormal();
             PrintXYpos();
             SetScnF=True;
        } */

        void SetFileExt(void)
        {
             byte Buf[80];

             strcpy(Buf,StdExt);
             if (HGetEng("Á¡‹¡Ã¡¡ ³a·© ÑÁ·© ÑÂ¸w¸aŸi °áº­A¶a.",Buf,79)==OK) {
                strcpy(StdExt,Buf);
                strupr(StdExt);
             }
        }

void OptionMenu(void)
{
    word OKF;
    word static OptionN;
    char *OptionList[]={
         "ÑÁ¡eµA Â‰bÐa“e Ðe‹i ¸aÑw·i ¥e‰wÐs“¡”a.",
         "ÑÁ¡eµA Â‰bÐa“e ´iÌa¥U ¸aÑw·i ¥e‰wÐs“¡”a.",
         "–¤é¯¢‰Á ­A¤é¯¢º— Ðe‹i ¸aÌe·i ¥e‰wÐs“¡”a.",
         "—¡¯aÇaa ÏaŸ¥Èá¡· ·³Â‰b¯¡ ¬a¶wÐi Ðe‹i Å¡—aŸi ¬åÈ‚ Ðs“¡”a.",
         "§iœâ ¸÷i ¯¡µA ¬a¶w–A“e ‹¡º… ˆt·i ¥e‰wÐs“¡”a.",
         "¢…¬á ·¥­áµA ‰ÅÐe ¬aÐw—i·i ¥e‰w Ðs“¡”a.",
         "‹¡¥¥ˆt·a¡ ¬a¶w–A“e ÑÁ·© ·¢¯aÉE­e·i ¥e‰wÐs“¡”a.",
         "¬e· ‹¡•·¯¡ ˆa¸w ÂA‹eµA ¸b´óÐa”å ¢…¬áŸi ¸a•··a¡ ·ª´á —i·©»¡ ¸÷Ðs“¡”a.",
         "¢…¬á ¸á¸w¯¡ µ¡œ–E ÑÁ·©·i *.BAK ·a¡ ¥¡¹¥Ði»¡Ÿi ‰i¸÷Ðs“¡”a.",
         "•¡¯a ·¡¶w¯¡µA ÑÁ·© ¸á¸wµa¦Ÿi ¢‰·i»¡ ‰i¸÷Ðs“¡”a.",
         "ÑÁ¡e ˆ—¯¥º—µA ‹i®A(Ç¡) ·³b·¡ ·¶”a¡e ·³bµA »bˆb ”ÀáÐi»¡ ¸÷Ðs“¡”a.",
         "¬åÈ‚,¯©Ð—,·³b ÀaŸ±Ìe‰Á ‰Åe–E ¶w·i ¸á¸wÐaµa ¬e· ‹¡•·¯¡µA ·ª´á —i·³“¡”a."
    };

    Push(InverseF);
    HInverse();
    HOpenWindow(16,1,36,17);
    HOpenMenu();
       HSetItem(17, 2," H Ðe‹i ‹i©",13,'H');
       HSetItem(17, 3," F µw¢… ‹i©",13,'F');
       HDrawBar(17,35,4);
       HSetItem(17, 5," K Ðe‹i ¸aÌe",13,'K');
       HSetItem(17, 6," C Ðe‹i Å¡—a",13,'C');
       HSetItem(17, 7," B §iœâ ¸÷i",13,'B');
       HSetItem(17, 8," I ¢…¬á ·¥­á",13,'I');
       HSetItem(17, 9," E ¢…¬á ¡Ÿ¡",13,'E');
       HDrawBar(17,35,10);
       HSetItem(17,11," P ¯¡¸b º…§¡",13,'P');
       HSetItem(17,12," M ¢…¬á ¥¡Ðñ",13,'M');
       HSetItem(17,13," A ·©”e ¸á¸w",13,'A');
       HSetItem(17,14," Q ¯¥­¢ ˆ—¯¥",13,'Q');
       HDrawBar(17,35,15);
       HSetItem(17,16," S ÑÅ‰w ¸á¸w",13,'S');
    Pop(InverseF);
    HManualSelect(OptionN);
    do {
      Push(InverseF);
      HInverse();
      if (HanGulCodeType==1) DrawString(31,6,"µÅ¬÷");
      else                   DrawString(31,6,"¹¡Ðs");
      if (PrepareF) DrawString(31,11,"Åq");
      else          DrawString(31,11,"q");
      if (BackUpF)  DrawString(31,12,"Åq");
      else          DrawString(31,12,"q");
      if (OsSaveF)  DrawString(31,13,"Åq");
      else          DrawString(31,13,"q");
      switch (SmartRefleshF) {
         case 1 :DrawString(31,14,"Åq "); break;
         case 2 :DrawString(31,14,"Åq'"); break;
         case 0 :DrawString(31,14,"q ");
      }
      Pop(InverseF);
      ExplainList=OptionList;
      OKF=HSelect();
      OptionN=NMP->NowPos;
      if (OKF==Error) break;
      switch (OptionN) {
        case 0 :SetHanFont();          break;
        case 1 :SetEngFont();          break;
        case 2 :SelectKeyboard();      break;
        case 3 :HanGulCodeType=!HanGulCodeType; break;
        case 4 :BlockOption();         break;
        case 5 :PrinterOption();       break;
        case 6 :SetFileExt();          break;
        case 7 :PrepareF=!PrepareF;    break;
        case 8 :BackUpF =!BackUpF;     break;
        case 9 :OsSaveF =!OsSaveF;     break;
        case 10:SmartRefleshF=(SmartRefleshF+1)%3; break;
        case 11:if (SaveOptions()!=OK) HSayIOErr();
                else {
                      HSayMes(" ÑÅ‰w ¸á¸w ÑÁ·© 'SAN.CNF' ˆa ¬¡  e—i´á ¹v¯s“¡”a.");
                      HotKeyEndF=True;
                }
                break;
      }
    } while (!HotKeyF && !HotKeyEndF);
    HCloseWindow();
    HCloseMenu();
}
