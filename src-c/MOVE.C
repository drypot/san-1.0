/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                 Movement Functions                   */
/*                                                      */

#include "stddef.h"
#include "memory.h"
#include "handrv.h"
#include "crt.h"
#include "const.h"
#include "edit.h"
#include "hwindow.h"
#include <ctype.h>
#include <string.h>

word   ScnRight=79 ;
word   ScnLeft =1;
word   ScnBot  =24;
word   ScnTop  =2 ;
word   ScnHigh =23;
word   ScnWidth=79;

word   SetScnF       ;
word   KeyInDisableF ;

word   SmartRefleshF=True;

void WriteLine (word ypos,byte *data)
{
     word x,i,j;

     x=0;
     ypos--;
     while (x<HX-1)
       if (data[x]>127) x+=2;
       else             x++;

     i=HY+ypos-ScnTop+1;
     if (ShowBlockF)
        switch (BlockType) {
           case 0 :if (BlockSY>i || BlockEY<i)
                       j=0;
                   else if (BlockSY<i && BlockEY>i) {
                       j=0;
                       HInverse();
                   }
                   else if (BlockSY==BlockEY)
                       j=3;
                   else if (BlockSY==i)
                       j=1;
                   else
                       j=2;
                   break;
           case 1 :if (BlockSY<=i && BlockEY>=i)
                       HInverse();
                   j=0;
                   break;
           case 2 :if (BlockSY<=i && BlockEY>=i)
                     j=3;
                   else j=0;
                   break;
        }
     else j=0;

     if (j==0) {
         x=( x < HX )?0:1;
         i=HX+x-1;
         while (data[i]==' ' && x<=ScnWidth) i++,x++;
           if (x>0) HClrBox(ScnLeft-1,ypos,x-1,ypos);
         while (data[i]!=0 && x<ScnWidth)
           if (data[i]>127) DrawHan(x,ypos,*((word*)(data+i))),x+=2,i+=2;
           else             DrawEng(x,ypos,data[i]),x++,i++;
         if (x==ScnWidth && data[i]!=0)   DrawEng(ScnRight,ypos,(data[i]>127)?' ':data[i]);
         else HClrBox(x,ypos,ScnRight,ypos);
     } else if (j==3) {
         if ( x < HX ) {
            x=0;
            if (BlockSX<=HX && BlockEX>=HX) HInverse();
         } else {
            x=1;
            if (BlockSX<=HX && BlockEX>=HX-1) HInverse();
            DrawEng(0,ypos,' ');
            if (BlockEX<=HX) HNormal();
         }
         i=HX+x-1;
         while (data[i]!=0 && x<ScnWidth) {
           if (data[i]>127) {
               if (i+1==BlockSX || i+2==BlockSX) HInverse();
               DrawHan(x,ypos,*((word*)(data+i)));
               if (i+1==BlockEX || i+2==BlockEX) HNormal();
               x+=2;
               i+=2;
           } else {
               if (i+1==BlockSX) HInverse();
               DrawEng(x,ypos,data[i]);
               if (i+1==BlockEX) HNormal();
               x++;
               i++;
           }
         }
         if (x==ScnWidth && data[i]!=0)   {
            if (i+1==BlockSX)  HInverse();
            DrawEng(ScnRight,ypos,(data[i]>127)?' ':data[i]);
         }
         else while (x<=ScnWidth) {
            if (i+1==BlockSX) HInverse();
            DrawEng(x,ypos,' ');
            if (i+1==BlockEX) HNormal();
            x++;
            i++;
         }
     } else if (j==1) {
         if (BlockSX<=HX) HInverse();
         if ( x < HX ) x=0;
         else DrawEng(0,ypos,' '),x=1;
         i=HX+x-1;
         while (data[i]!=0 && x<ScnWidth) {
           if (i+1==BlockSX) HInverse();
           if (data[i]>127) DrawHan(x,ypos,*((word*)(data+i))),x+=2,i+=2;
           else             DrawEng(x,ypos,data[i]),x++,i++;
         }
         if (x==ScnWidth && data[i]!=0)   {
            if (i+1==BlockSX)  HInverse();
            DrawEng(ScnRight,ypos,(data[i]>127)?' ':data[i]);
         }
         else while (x<=ScnWidth) {
            if (i+1==BlockSX) HInverse();
            DrawEng(x,ypos,' ');
            x++;
            i++;
         }
     } else {
         if ( x < HX ) {
             x=0;
             if (BlockEX>=HX) HInverse();
         } else {
             if (BlockEX>=HX-1) HInverse();
             x=1;
             DrawEng(0,ypos,' ');
             if (BlockEX==HX-1) HNormal();
         }
         i=HX+x-1;
         while (data[i]!=0 && x<ScnWidth) {
           if (data[i]>127) {
               DrawHan(x,ypos,*((word*)(data+i)));
               if (i+1==BlockEX) HNormal();
               x+=2;
               i+=2;
           } else {
               DrawEng(x,ypos,data[i]);
               if (i+1==BlockEX) HNormal();
               x++;
               i++;
           }
         }
         if (x==ScnWidth && data[i]!=0)
            DrawEng(ScnRight,ypos,(data[i]>127)?' ':data[i]);
         else while (x<=ScnWidth) {
            DrawEng(x,ypos,' ');
            if (i+1==BlockEX) HNormal();
            x++;
            i++;
         }
     }
     HNormal();
}

void ClearLine (word ypos)
{
     word i,x,j;

     ypos--;
     i=HY+ypos-ScnTop+1;
     if (ShowBlockF)
        switch (BlockType) {
           case 0 :if (BlockSY>i || BlockEY<i)
                       j=0;
                   else if (BlockSY<i && BlockEY>i) {
                       j=0;
                       HInverse();
                   }
                   else if (BlockSY==BlockEY )
                       j=3;
                   else if (BlockSY==i)
                       j=1;
                   else
                       j=2;
                   break;
           case 1 :if (BlockSY<=i && BlockEY>=i)
                       HInverse();
                   j=0;
                   break;
           case 2 :if (BlockSY<=i && BlockEY>=i)
                       j=3;
                   else j=0;
        }
     else j=0;

     if (j==0)
         HClrBox(ScnLeft-1,ypos,ScnRight,ypos);
     else if (j==3) {
         if (BlockSX<HX && BlockEX>=HX) HInverse();
         x=0;
         i=HX+x-1;
         while (x<=ScnWidth) {
            if (i+1==BlockSX) HInverse();
            DrawEng(x,ypos,' ');
            if (i+1==BlockEX) HNormal();
            x++;
            i++;
         }
     } else if (j==1) {
         if (BlockSX<=HX) HInverse();
         x=0;
         i=HX+x-1;
         while (x<=ScnWidth) {
            if (i+1==BlockSX) HInverse();
            DrawEng(x,ypos,' ');
            x++;
            i++;
         }
     } else {
         HInverse();
         x=0;
         i=HX+x-1;
         while (x<=ScnWidth) {
            DrawEng(x,ypos,' ');
            if (i+1==BlockEX) HNormal();
            x++;
            i++;
         }
     }
     HNormal();
}

void SetScn(void)
{
    word i,j,k;
    word ShowedF;

    if (SmartRefleshF==2 && KeyPressed() && !KeyInDisableF) {
      SetScnF=True;
      if (ShowedF) {
          if (HanF)
               if ( EditType[HX+HSX-1]==1)
                    DrawHanCur(HSX-1,SY-1),
                    DrawHanCur(HSX  ,SY-1);
               else DrawHanCur(HSX-1,SY-1);
          else
               if ( EditType[HX+HSX-1]==1)
                    DrawEngCur(HSX-1,SY-1),
                    DrawEngCur(HSX  ,SY-1);
               else DrawEngCur(HSX-1,SY-1);
          }
      return;
    }
    i=1;
    j=HY+SY-ScnTop;
    k=2;
    WriteLine (SY,EditBuf+1);
    if (*(unsigned long*)Pointer(0x40,0x6C)%6<3) {
      if (HanF)
           if ( EditType[HX+HSX-1]==1)
                DrawHanCur(HSX-1,SY-1),
                DrawHanCur(HSX  ,SY-1);
           else DrawHanCur(HSX-1,SY-1);
      else
           if ( EditType[HX+HSX-1]==1)
                DrawEngCur(HSX-1,SY-1),
                DrawEngCur(HSX  ,SY-1);
           else DrawEngCur(HSX-1,SY-1);
      ShowedF=True;
    } else ShowedF=False;
    while (k>0) {
      if (SmartRefleshF==1 && KeyPressed() && !KeyInDisableF) {
          SetScnF=True;
          if (ShowedF) {
             if (HanF)
                  if ( EditType[HX+HSX-1]==1)
                       DrawHanCur(HSX-1,SY-1),
                       DrawHanCur(HSX  ,SY-1);
                  else DrawHanCur(HSX-1,SY-1);
             else
                  if ( EditType[HX+HSX-1]==1)
                       DrawEngCur(HSX-1,SY-1),
                       DrawEngCur(HSX  ,SY-1);
                  else DrawEngCur(HSX-1,SY-1);
          }
          return;
      }
      k=0;
      if (SY-i>=ScnTop && SY-i<ScnBot+1) {
          if (Paper[j-i][0]>=HX) WriteLine (SY-i,Paper[j-i]+1);
          else ClearLine(SY-i);
          k++;
      }
      if (SY+i<=ScnBot+1 ) {
          if (j+i<=PaperLen && Paper[j+i][0]>=HX ) WriteLine (SY+i,Paper[j+i]+1);
          else ClearLine(SY+i);
          k++;
      }
      i++;
    }
    SetScnF=False;
    if (ShowedF) {
       if (HanF)
            if ( EditType[HX+HSX-1]==1)
                 DrawHanCur(HSX-1,SY-1),
                 DrawHanCur(HSX  ,SY-1);
            else DrawHanCur(HSX-1,SY-1);
       else
            if ( EditType[HX+HSX-1]==1)
                 DrawEngCur(HSX-1,SY-1),
                 DrawEngCur(HSX  ,SY-1);
            else DrawEngCur(HSX-1,SY-1);
    }
    if (SmartRefleshF!=2) NextChar=FirstAvailable;
}

void SetBlockPos(void)
{
    switch (BlockType) {
          case 0: if (NowLine>BlockHeadY) {
                      BlockSY=BlockHeadY;
                      BlockEY=NowLine;
                      BlockSX=BlockHeadX;
                      BlockEX=CurXPos();
                  } else if (NowLine<BlockHeadY) {
                      BlockSY=NowLine;
                      BlockEY=BlockHeadY;
                      BlockSX=CurXPos();
                      BlockEX=BlockHeadX;
                  } else {
                      BlockSY=BlockEY=NowLine;
                      if (CurXPos()>BlockHeadX) {
                          BlockSX=BlockHeadX;
                          BlockEX=CurXPos();
                      } else {
                          BlockSX=CurXPos();
                          BlockEX=BlockHeadX;
                      }
                  }
                  break;
          case 1: if (NowLine>BlockHeadY) {
                      BlockSY=BlockHeadY;
                      BlockEY=NowLine;
                  } else {
                      BlockSY=NowLine;
                      BlockEY=BlockHeadY;
                  }
                  break;
          case 2: if (NowLine>BlockHeadY) {
                      BlockSY=BlockHeadY;
                      BlockEY=NowLine;
                  } else {
                      BlockSY=NowLine;
                      BlockEY=BlockHeadY;
                  }
                  if (CurXPos()>BlockHeadX) {
                      BlockSX=BlockHeadX;
                      BlockEX=CurXPos();
                  } else {
                      BlockSX=CurXPos();
                      BlockEX=BlockHeadX;
                  }
    }
    SetScnF=True;
}

void SetCurPos (word Newx,word  Newy)
{
    word i;

    if (Newy!=NowLine) {
       if (PutEditBuf()!=OK) return;
       if (Newy>MaxPaper+100 || Newy==0) Newy=1;
       if (Newy>PaperLen) Newy=PaperLen;
       NowLine=Newy;
       if (Newy<HY) {
              SetScnF=True;
              if (Newy<SY-ScnTop+1) HY=1,SY=Newy-HY+ScnTop;
              else                  HY=Newy-SY+ScnTop;
       } else if (Newy<=HY+ScnHigh-1) SY=Newy-HY+ScnTop;
              else SetScnF=True,         HY=Newy-SY+ScnTop;
       SetEditBuf();
    }

    if (Newx!=HX+SX-1) {
         if (Newx<1)   Newx=1;
         if (Newx>6000)Newx=1;
         if (Newx>254) Newx=254;
         if (Newx<HX)  {
             if (EditType[Newx]==2) {
                HX=Newx-1;
                SX=ScnLeft+1;
             } else {
                HX=Newx;
                SX=ScnLeft;
             }
             SetScnF=True;
         }
         else if (Newx==HX) {
              if (EditType[Newx]==2)
                 HX--,SetScnF=True;
              SX=Newx-HX+ScnLeft;
         }
         else if (Newx<HX+ScnWidth-1)
                  SX=Newx-HX+ScnLeft;
         else if (Newx==HX+ScnWidth-1) {
                  if (EditType[Newx]==1)  {
                      HX++;
                      SX=Newx-HX+ScnLeft;
                      SetScnF=True;
                  } else SX=ScnRight;
         }
         else   {
              SX=(EditType[Newx]==1)?ScnRight-1:ScnRight;
              HX=Newx-SX+ScnLeft;
              SetScnF=True;
         }
    }
    HSX=(EditType[HX+SX-1]==2)?SX-1:SX;
    if (SelectingBlockF) SetBlockPos();
}

void FoolSetCur (word Newx,word  Newy)
{

     if (Newy>MaxPaper+100 || Newy==0) Newy=1;
     if (Newy>PaperLen) Newy=PaperLen;
     if (NowLine!=Newy)
         if (PutEditBuf()!=OK) return;
     if (Newy<HY) {
            SetScnF=True;
            if (Newy<SY-ScnTop+1) HY=1,SY=Newy-HY+ScnTop;
            else                  HY=Newy-SY+ScnTop;
     } else if (Newy<=HY+ScnHigh-1) SY=Newy-HY+ScnTop;
            else SetScnF=True,SY=ScnBot,HY=Newy-SY+ScnTop;
     if (NowLine!=Newy) {
         NowLine=Newy;
         SetEditBuf();
     }

    if (Newx<1)    Newx=1;
    if (Newx>1000) Newx=1;
    if (Newx>254)  Newx=254;
    if (Newx<HX)  {
        if (EditType[Newx]==2) {
           HX=Newx-1;
           SX=ScnLeft+1;
        } else {
           HX=Newx;
           SX=ScnLeft;
        }
        SetScnF=True;
    } else if (Newx==HX) {
              if (EditType[Newx]==2)
                 HX--,SetScnF=True;
              SX=Newx-HX+ScnLeft;
         }
    else if (Newx<HX+ScnWidth-1)
             SX=Newx-HX+ScnLeft;
    else if (Newx==HX+ScnWidth-1) {
             if (EditType[Newx]==1)  {
                 HX++;
                 SX=Newx-HX+ScnLeft;
                 SetScnF=True;
             } else SX=ScnRight;
    }
    else   {
         SX=(EditType[Newx]==1)?ScnRight-1:ScnRight;
         HX=Newx-SX+ScnLeft;
         SetScnF=True;
    }
    HSX=(EditType[HX+SX-1]==2)?SX-1:SX;
    if (SelectingBlockF) SetBlockPos();

}

word SelectPos(char *data)
{
    word j;

    j=Error;
    HOpenWindowSimple(0,0,VideoRight,0);
    HClrBox(0,0,VideoRight,0);
    HWriteMid(0,data);
    do {
       if (HX^HY^SX^SY != HXB2^HYB2^XB2^YB2) {
           XB=XB2; YB=YB2; HXB=HXB2; HYB=HYB2;
           XB2=SX; YB2=SY; HXB2=HX; HYB2=HY;
       }
       PrintXYpos();
       if (SetScnF) SetScn();
       else WriteLine(SY,EditBuf+1);
       HSX=(EditType[HX+SX-1]==2)?SX-1:SX;
       if (HanResult!=3)
          if (HanF)
               if ( HanResult==0 || HanResult==1 || EditType[HX+HSX-1]==1)
                    HanCur2(HSX-1,SY-1);
               else HanCur1(HSX-1,SY-1);
          else
               if ( EditType[HX+HSX-1]==1)
                    EngCur2(HSX-1,SY-1);
               else EngCur1(HSX-1,SY-1);
       GetKey();
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
          case Home     :EditHome();  break;
          case End      :EditEnd();   break;
          case CtrlHome :EditCHome(); break;
          case CtrlEnd  :EditCEnd();  break;
          case CtrlPgUp :EditCPgUp(); break;
          case CtrlPgDn :EditCPgDn(); break;
          case Return   :j=OK;        break;
          case CtrlQ    :DrawMoreKey("^Q");
                         GetKey();
                         if (Held(Way,'A','Z')) Way-=64;
                         if (Held(Way,'a','z')) Way-=96;
                         switch (Way) {
                           case CtrlP:EditCQP();   break;
                           case CtrlS:EditHome();  break;
                           case CtrlD:EditEnd();   break;
                           case CtrlE:EditCHome(); break;
                           case CtrlX:EditCEnd();  break;
                           case CtrlR:EditCPgUp(); break;
                           case CtrlC:EditCPgDn(); break;
                           default   :SetHotKeyF();
                         }
                         DrawMoreKey("  ");
                         break;
          default       :SetHotKeyF();
       }
    } while (Way!=ESC && !HotKeyF && Way!=Return);

    HCloseWindowSimple();

    return j;
}


void EditCW(void)
{
    if (HY>1) {
       if (SY<ScnBot) HY=HY-1,SY=SY+1;
       else {
            if (PutEditBuf()!=OK) return;
            HY--;
            NowLine--;
            SetEditBuf();
            if (SelectingBlockF) SetBlockPos();
       }
       /*if (!KeyPressed() && SetScnF==OFF ){
           HScrollDown (ScnLeft-1,ScnTop-1,ScnRight,ScnBot);
           WriteLine (ScnTop,Paper[HY]+1);
       } else*/ SetScnF=ON;
    }
}

void EditCZ(void)
{
    if (HY+SY-ScnTop!=PaperLen || SY!=ScnTop) {
       if (SY>ScnTop) HY=HY+1,SY=SY-1;
       else {
          if (PutEditBuf()!=OK) return;
          HY++;
          NowLine++;
          SetEditBuf();
          if (SelectingBlockF) SetBlockPos();
       }
             /*
             if (!KeyPressed() && SetScnF==OFF ) {
                 HScrollUp (ScnLeft-1,ScnTop-1,ScnRight,ScnBot);
                 if (HY+ScnBot-ScnTop+1<=PaperLen && Paper[HY+ScnBot-ScnTop+1][0]>=HX) WriteLine (ScnBot+1,Paper[HY+ScnBot-ScnTop+1]+1);
                  else ClearLine (ScnBot+1);
             } else
             */
       SetScnF=ON;
    }
}

void EditCE()
{
     /*if (KeyPressed() || SetScnF==ON )*/ SetCurPos ((HX+SX-1),NowLine-1);
     /*else {
        SetCurPos ((HX+SX-1),NowLine-1);
        if (SetScnF && !SelectingBlockF) HScrollDown (ScnLeft-1,ScnTop-1,ScnRight,ScnBot),SetScnF=OFF;
     } */
}

void EditCX()
{
    /* if (KeyPressed() || SetScnF==ON ) */ SetCurPos ((HX+SX-1),NowLine+1);
    /* else {
        SetCurPos ((HX+SX-1),NowLine+1);
        if (SetScnF && !SelectingBlockF) {
            HScrollUp (ScnLeft-1,ScnTop-1,ScnRight,ScnBot),SetScnF=OFF;
            if (NowLine<PaperLen && Paper[NowLine+1][0]>=HX) WriteLine (ScnBot+1,Paper[NowLine+1]+1);
            else                   ClearLine (ScnBot+1);
        }
     } */
}

void EditCS(void)
{
     if (EditType[HX+SX-1]==2) SetCurPos ((HX+SX-1)-2,NowLine);
     else                      SetCurPos ((HX+SX-1)-1,NowLine);
}

void EditCD(void)
{
     if (EditType[HX+SX-1]==1) SetCurPos ((HX+SX-1)+2,NowLine);
     else                      SetCurPos ((HX+SX-1)+1,NowLine);
}

word EditBufStartPos(void)
{
   word i;

   for ( i=1 ; i<255 ; i++)
     if (EditType[i]!=0 || EditBuf[i]!=' ') break;
   return i;
}

void EditCA(void)
{
    word i;

    i=CurXPos();
    if (!spacetillrow(i-1)) {
       if (SLettersTillRow (i-1)) i=EditBufStartPos();
       else {
            while (i>1 && !isalnum(EditBuf[i-1]) && EditType[i-1]==0 ) i--;
            while (i>1 && ( (isalnum(EditBuf[i-1])&&EditType[i-1]==0) || EditType[i-1]>0)) i--;
       }
       SetCurPos (i,NowLine);
    } else {
       if (NowLine==1) return;
       i=NowLine-1;
       while (i>1 && Paper[i][0]==0) i--;
       SetCurPos (Paper[i][0]+1,i);
    }
}

void EditCF(void)
{
    word i;

    i=CurXPos();
    if (!SpaceFromRow (i) && i!=256-2) {
         while (i<=256-2 && ( (isalnum(EditBuf[i])&&EditType[i]==0) || EditType[i]>0)) i++;
         if (SLettersFromRow (i)) for (i=256-1;EditBuf[i-1]==' ';i--);
         else while (i<=256-2 && !isalnum(EditBuf[i]) && EditType[i]==0) i++;
         SetCurPos (i,NowLine);
    }
    else {
         if (NowLine==PaperLen) return;
         i=NowLine+1;
         while (i<PaperLen && Paper[i][0]==0) i++;
         SetCurPos (Stringstartpos(i),i);
    }
}


void EditEnd(void)
{
    word i;

    for (i=256-1;i>1 && EditBuf[i-1]==' ';i--);
    SetCurPos (i,NowLine);
}
