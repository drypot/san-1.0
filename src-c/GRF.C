/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                       Grf                            */
/*                                                      */

#include "stddef.h"
#include "crt.h"
#include "memory.h"
#include "handrv.h"
#include "hwindow.h"
#include "popup.h"
#include "const.h"
#include "edit.h"
#include <string.H>

static word F4_X,F4_Y;
static word GrfPos;
static word GrfPage;

void EditF4Pos(word NewX,word NewY)
{
     if (NewY>6000) F4_Y=11,F4_X--;
     else if (NewY>11)  F4_Y=0,F4_X++;
     else {
       F4_Y=NewY;
       if (NewX>6000) {
           F4_X=15;
           F4_Y--;
       }
       else if (NewX>15) {
           F4_X=0;
           F4_Y++;
       }
       else F4_X=NewX;
     }
     if (F4_Y>6000) F4_Y=11;
     if (F4_Y>11) F4_Y=0;
     if (F4_X>6000) F4_X=15;
     if (F4_X>15) F4_X=0;

}

void SetGrfPage(void)
{
     word i,j,k;
     word Code;

     Push(InverseF);
     HNormal();
     HClrBox(44,4,75,15);
     if (GrfPage==0) {
         for (i=0 ; i<8 ; i++)
           for (j=0 ; j<16 ; j++)
              DrawHan(44+j*2,4+i,(i*16+j)*0x100 + 0x80D4);
     } else {
         DrawHan(44,4,0x31D9);
         DrawHan(74,14,0x31D9);
         k=0x31;
         for (i=0 ; i<12 ; i++)
           for (j=0 ; j<16 ; j++) {
              if (i==0 && j==0  || i==11 && j==15 ||
                  i==5 && j==15 || i==6  && j==0 )
                  Code=0x31D9;
              else {
                  Code=k*0x100+0xD8+GrfPage;
                  k++;
                  if (k==0x7F) k=0x91;
              }
              DrawHan(44+j*2,4+i,Code);
           }
     }
     Pop(InverseF);
}

void SetGrfLine(void)
{
     word i;

     Push(InverseF);
     HNormal();
     i=strlen(AuxBuf+1);
     HClrBox(44,17,75,17);
     if (i>30) {
        DrawString(44,17,AuxBuf+i-29);
        DrawEngCur(74,17);
        DrawEngCur(75,17);
     } else {
        DrawString(44,17,AuxBuf+1);
        DrawEngCur(44+i,17);
        DrawEngCur(45+i,17);
     }
     Pop(InverseF);
}

void SetGrfPos(void)
{
    if (GrfPage==0) {
       if (F4_Y>7) GrfPos=0x80D4;
       else GrfPos=0x80D4+(F4_Y*16+F4_X)*0x100;
    } else {
       if (F4_Y==0 && F4_X==0  || F4_Y==11 && F4_X==15 ||
           F4_Y==5 && F4_X==15 || F4_Y==6  && F4_X==0 )
           GrfPos=0x31D9;
       else {
           GrfPos=(F4_Y*16+F4_X)+0x30;
           if (GrfPos>0x90 )GrfPos+=0x10;
           else if (GrfPos>=0x7f ) GrfPos+=0x12;
           GrfPos=0xD8+GrfPage+GrfPos*0x100;
       }
    }

}

word SelectGrf(void)
{
    word i,j;
    word AuxBufPos=1;

    Push(InverseF);
    HOpenWindowSimple(0,VideoBot,VideoRight,VideoBot);
    HNormal();
    HClrBox(0,VideoBot,VideoRight,VideoBot);
    HWriteMid(VideoBot,"¬a¶w‹i®A  [Space] [+] [BackSp] [Return] [PaUp] [PaDn]");
    HInverse();
    HOpenWindow(32,3,77,18);
    DrawString(34, 4,"‹aŸ±/‰á¬å");
    DrawString(34, 5,"•¡Ñw/ aÇa");
    DrawString(34, 6,"µw¢…/Ðe‹i");
    DrawString(34, 7,"¡ a/‰á¬å");
    DrawString(34, 8,"‹¡Ñ¡/œaË¥");
    DrawString(34, 9,"œaË¥/·©¥¥");
    DrawString(34,10,"·©/œá¯¡´a");
    SetGrfPage();
    Pop(InverseF);
    HSetInverse(34,4+GrfPage,42,4+GrfPage);
    AuxBuf[1]=0;
    SetGrfLine();
    do {
       HanCur2(44+F4_X*2,4+F4_Y);
       GetKey();
       SetHotKeyF();
       if (HotKeyF) break;
       switch (Way) {
         case '+'  :
         case ' '  :if (AuxBufPos<253) {
                       SetGrfPos();
                       *((word *)(AuxBuf+AuxBufPos))=GrfPos;
                       AuxBuf[AuxBufPos+=2]=0;
                       SetGrfLine();
                    }
                    if (Way==' ') EditF4Pos(F4_X+1,F4_Y);
                    break;
         case BackSpace:
                    if (AuxBufPos>1) {
                       AuxBuf[AuxBufPos-=2]=0;
                       SetGrfLine();
                    }
                    break;
         case CtrlS:
         case Left :EditF4Pos(F4_X-1,F4_Y);
                    break;
         case CtrlD:
         case Right:EditF4Pos(F4_X+1,F4_Y);
                    break;
         case CtrlW:
         case CtrlA:
         case CtrlLeft:
         case CtrlE:
         case Up   :EditF4Pos(F4_X,F4_Y-1);
                    break;
         case CtrlZ:
         case CtrlF:
         case CtrlRight:
         case CtrlX:
         case Down :EditF4Pos(F4_X,F4_Y+1);
                    break;
         case Home :EditF4Pos(0,0);
                    break;
         case End  :EditF4Pos(15,11);
                    break;
         case CtrlC:
         case PgDn :HSetInverse(34,4+GrfPage,42,4+GrfPage);
                    if (GrfPage==6) GrfPage=0;
                    else GrfPage++;
                    HSetInverse(34,4+GrfPage,42,4+GrfPage);
                    SetGrfPage();
                    break;
         case CtrlR:
         case PgUp :HSetInverse(34,4+GrfPage,42,4+GrfPage);
                    if (GrfPage==0) GrfPage=6;
                    else GrfPage--;
                    HSetInverse(34,4+GrfPage,42,4+GrfPage);
                    SetGrfPage();
                    break;
         case ESC  :HCloseWindow();
                    HCloseWindowSimple();
                    return Error;
       }
    } while (Way!=Return);
    HCloseWindow();
    HCloseWindowSimple();
    SetGrfPos();
    return (HotKeyF) ? Error : GrfPos;
}
