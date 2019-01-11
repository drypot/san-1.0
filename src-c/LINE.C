/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                      Draw Line                       */
/*                                                      */

#include "stddef.h"
#include "crt.h"
#include "memory.h"
#include "handrv.h"
#include "popup.h"
#include "const.h"
#include "edit.h"
#include "hwindow.h"
#include <alloc.h>

#define NoLine      0
#define Thin        1
#define Thick       2

#define DrawMode  0
#define MoveMode  1
#define EraseMode 2
#define GrfMode   3


word LineStyle[8][11]={
        {'‘⁄','‘¬','‘ø','‘√','‘≈','‘¥','‘¿','‘¡','‘Ÿ','‘ƒ','‘≥'},
        {'‘’','‘—','‘∏','‘∆','‘ÿ','‘µ','‘‘','‘œ','‘æ','‘Õ','‘≥'},
        {'‘÷','‘“','‘∑','‘«','‘◊','‘∂','‘”','‘–','‘Ω','‘ƒ','‘∫'},
        {'‘…','‘À','‘ª','‘Ã','‘Œ','‘π','‘»','‘ ','‘º','‘Õ','‘∫'},
        {'€£','€®','€§','€ß','€´','€©','€¶','€™','€•','€°','€¢'},
        {'€»','€∏','€¬','€º','€ª','€æ','€∆','€∫','€ƒ','€¨','€¢'},
        {'€«','€Ω','€¡','€∑','€¿','€π','€≈','€ø','€√','€°','€≠'},
        {'€Æ','€≥','€Ø','€≤','€∂','€¥','€±','€µ','€∞','€¨','€≠'}
     };


word  static BoxStyle,
             BoxParty;
word  static UpF,DownF,RightF,LeftF;
word  static Direction;
word  static OldX,X,OldY,Y;
word  static Mode,
             Class,
             LineCharF,
             GrfCharF;
byte  static GrfCharH=0x80,
             GrfCharL=0xD4;
word  static LastDrawMode=DrawMode;
word  static ScrollCommand;

void SetMenuItem()
{
   switch (Mode) {
     case DrawMode  :HSetInverse(0,0,10,0);  break;
     case EraseMode :HSetInverse(11,0,21,0); break;
     case GrfMode :HSetInverse(22,0,36,0); break;
     case MoveMode   :HSetInverse(37,0,47,0); break;
   }
}



void SetLineMenu(void)
{
   HClrBox(0,0,VideoRight,0);
   DrawString(0,0," [D]ãaü°ã°  [E]ª°∂Åã°  [G/A]ãaü±¢Ö∏a  [M/F]∑°ï∑  [L/S/1-8]¨Â  [SPC]ÆÅ∏˜/∑°ï∑  ");
   Mode=MoveMode;
   SetMenuItem();
}

word IsLineChar (byte *String,word Char)
{
   while (*String!=0)
     if (*(word*)String==Char)
        return OK;
     else
        String+=2;
   return False;
}

void SetUpF(void)
{
     word i,j;

     UpF=NoLine;
     i=HX+SX-1;
     if (NowLine>1) {
         SetAuxBuf(NowLine-1);
         j=*(word*)(AuxBuf+i);
         if (AuxType[i]==1)
            if (BoxParty==0)
               UpF=(IsLineChar("‘≥‘¥‘µ‘∏‘ø‘¬‘√‘≈‘∆‘—‘ÿ‘’‘⁄",j)) ? Thin :
                   (IsLineChar("‘∂‘∑‘π‘∫‘ª‘«‘…‘À‘Ã‘Œ‘“‘◊‘÷",j)) ? Thick:
                                                                NoLine;
            else
               UpF=(IsLineChar("€¢€£€§€ß€®€©€´€∏€ª€º€æ€¬€»€…€À€Õ€œ€—€“€Ÿ€⁄€€€›€ﬁ€·",j)) ? Thin :
                   (IsLineChar("€≠€Æ€Ø€≤€≥€¥€∂€∑€π€Ω€¿€¡€«€ €Ã€Œ€–€”€‘€‹€ﬂ€‡€‚€„€‰",j)) ? Thick:
                                                                                          NoLine;
     }
}

void SetDownF(void)
{
     word i,j;

     DownF=NoLine;
     i=HX+SX-1;
     if (NowLine<PaperLen) {
         SetAuxBuf(NowLine+1);
         j=*(word*)(AuxBuf+i);
         if (AuxType[i]==1)
            if (BoxParty==0)
               DownF=(IsLineChar("‘≥‘¥‘µ‘æ‘¿‘¡‘√‘≈‘∆‘œ‘‘‘ÿ‘Ÿ",j)) ? Thin :
                     (IsLineChar("‘∂‘π‘∫‘º‘Ω‘«‘»‘ ‘Ã‘Œ‘–‘”‘◊",j)) ? Thick:
                                                                    NoLine;
            else
               DownF=(IsLineChar("€¢€•€¶€ß€©€™€´€∫€ª€º€æ€ƒ€∆€ €Ã€Œ€–€’€÷€Ÿ€⁄€‹€ﬂ€‡€‚",j))? Thin :
                     (IsLineChar("€≠€∞€±€≤€¥€µ€∂€∑€π€ø€¿€√€≈€…€À€Õ€œ€◊€ÿ€€€›€ﬁ€·€„€‰",j))? Thick:
                                                                                           NoLine;
     }
}

void SetLeftF(void)
{
     word i,j;

     LeftF=NoLine;
     i=HX+SX-1;
     j=*(word*)(EditBuf+i-2);
     if (i>2) {
         if (EditType[i-2]==1)
            if (BoxParty==0)
              LeftF=(IsLineChar("‘¿‘¡‘¬‘√‘ƒ‘≈‘«‘–‘“‘”‘÷‘◊‘⁄",j)) ? Thin :
                    (IsLineChar("‘∆‘»‘…‘ ‘À‘Ã‘Õ‘Œ‘œ‘—‘‘‘’‘ÿ",j)) ? Thick:
                                                                   NoLine;
            else
              LeftF=(IsLineChar("€°€£€¶€ß€®€™€´€∑€Ω€ø€¿€≈€«€…€ €—€”€’€◊€Ÿ€€€‹€›€ﬂ€„",j)) ? Thin :
                    (IsLineChar("€¨€Æ€±€≤€≥€µ€∂€∏€∫€ª€º€∆€»€À€Ã€“€‘€÷€ÿ€⁄€ﬁ€‡€·€‚€‰",j)) ? Thick:
                                                                                           NoLine;
     }
}


void SetRightF(void)
{
     word i,j;

     RightF=NoLine;
     i=HX+SX-1;
     j=*(word*)(EditBuf+i+2);
     if (i<252) {
         if (EditType[i+2]==1)
            if (BoxParty==0)
               RightF=(IsLineChar("‘¥‘∂‘∑‘Ω‘ø‘¡‘¬‘ƒ‘≈‘–‘“‘◊‘Ÿ",j)) ? Thin :
                      (IsLineChar("‘µ‘∏‘π‘ª‘º‘æ‘ ‘À‘Õ‘Œ‘œ‘—‘ÿ",j)) ? Thick:
                                                                     NoLine;
            else
               RightF=(IsLineChar("€°€§€•€®€©€™€´€π€Ω€ø€¿€¡€√€Õ€Œ€“€‘€÷€ÿ€⁄€€€‹€ﬁ€‡€‰",j)) ? Thin :
                      (IsLineChar("€¨€Ø€∞€≥€¥€µ€∂€∏€∫€ª€æ€¬€ƒ€œ€–€—€”€’€◊€Ÿ€›€ﬂ€·€‚€„",j)) ? Thick:
                                                                                             NoLine;

     }
}

word LineChar(void)
{
     word i;

     i=HX+SX-1;
     if (EditType[i]==1)
       if (EditBuf[i]==0xd4)
          return (EditBuf[i+1]>=0xb3 && EditBuf[i+1]<=0xda)? OK :False;
       else if (EditBuf[i]==0xdb)
          return (EditBuf[i+1]>=0xa1 && EditBuf[i+1]<=0xE4)? OK :False;
     return False;
}

void EraseGrf(void)
{
    EditBuf [X  ]=' ';
    EditBuf [X+1]=' ';
    EditType[X  ]=0;
    EditType[X+1]=0;
    if (X>2 && EditType[X-1]==1) {
        EditType[X-1]=0;
        EditBuf [X-1]=' ';
    }
    if (X<253 && EditType[X+2]==2) {
        EditType[X+2]=0;
        EditBuf [X+2]=' ';
    }
}

void DrawGrf(void)
{
    EditBuf [X  ]=GrfCharL;
    EditBuf [X+1]=GrfCharH;
    EditType[X  ]=1;
    EditType[X+1]=2;
    if (X>2 && EditType[X-1]==1) {
        EditType[X-1]=0;
        EditBuf [X-1]=' ';
    }
    if (X<253 && EditType[X+2]==2) {
        EditType[X+2]=0;
        EditBuf [X+2]=' ';
    }

}

LineTable[8][3][3][3][3]=       /* Up    Down  Left Right */
   {
     /* BoxParty 0 */

     {
       {
         { {'‘≥','‘ƒ','‘Õ'},             /* no    no    no         */
           {'‘ƒ','‘ƒ','‘ƒ'},             /* no    no    thin       */
           {'‘Õ','‘ƒ','‘Õ'}              /* no    no    thick      */
         },
         { {'‘≥','‘⁄','‘’'},             /* no    thin  no         */
           {'‘ø','‘¬','‘ø'},             /* no    thin  thin       */
           {'‘∏','‘⁄','‘—'},             /* no    thin  thick      */
         },
         { {'‘∫','‘÷','‘…'},             /* no    thick no         */
           {'‘∑','‘“','‘ƒ'},             /* no    thick thin       */
           {'‘ª','‘÷','‘À'}              /* no    thick thick      */
         }
       },
       {
         { {'‘≥','‘¿','‘‘'},             /* thin  no    no         */
           {'‘Ÿ','‘¡','‘Ÿ'},             /* thin  no    thin       */
           {'‘æ','‘¿','‘œ'}              /* thin  no    thick      */
         },
         { {'‘≥','‘√','‘∆'},             /* thin  thin  no         */
           {'‘¥','‘≈','‘¥'},             /* thin  thin  thin       */
           {'‘µ','‘√','‘ÿ'}              /* thin  thin  thick      */
         },
         { {'‘≥','‘¿','‘≥'},             /* thin  thick no         */
           {'‘Ÿ','‘¡','‘Ÿ'},             /* thin  thick thin       */
           {'‘≥','‘¿','‘œ'}              /* thin  thick thick      */
         }
       },
       {
         { {'‘∫','‘”','‘»'},             /* thick no    no         */
           {'‘Ω','‘–','‘Ω'},             /* thick no    thin       */
           {'‘∫','‘«','‘Ã'}              /* thick no    thick      */
         },
         { {'‘≥','‘⁄','‘’'},             /* thick thin  no         */
           {'‘ø','‘¬','‘ø'},             /* thick thin  thin       */
           {'‘∏','‘⁄','‘—'}              /* thick thin  thick      */
         },
         { {'‘∫','‘«','‘Ã'},             /* thick thick no         */
           {'‘∂','‘◊','‘∂'},             /* thick thick thin       */
           {'‘π','‘«','‘Œ'}              /* thick thick thick      */
         }
       }
     },

     /* BoxParty 1 */

     {
       {
         { {'‘≥','‘ƒ','‘Õ'},            /* no    no    no         */
           {'‘ƒ','‘ƒ','‘Õ'},            /* no    no    thin       */
           {'‘Õ','‘Õ','‘Õ'}             /* no    no    thick      */
         },
         { {'‘≥','‘⁄','‘’'},            /* no    thin  no         */
           {'‘ø','‘¬','‘’'},            /* no    thin  thin       */
           {'‘∏','‘∏','‘—'}             /* no    thin  thick      */
         },
         { {'‘∫','‘÷','‘…'},            /* no    thick no         */
           {'‘∑','‘“','‘…'},            /* no    thick thin       */
           {'‘ª','‘ª','‘À'}             /* no    thick thick      */
         }
       },
       {
         { {'‘≥','‘¿','‘‘'},            /* thin  no    no         */
           {'‘Ÿ','‘¡','‘‘'},            /* thin  no    thin       */
           {'‘æ','‘æ','‘œ'}             /* thin  no    thick      */
         },
         { {'‘≥','‘√','‘∆'},            /* thin  thin  no         */
           {'‘¥','‘≈','‘∆'},            /* thin  thin  thin       */
           {'‘µ','‘µ','‘ÿ'}             /* thin  thin  thick      */
         },
         { {'‘≥','‘¿','‘‘'},            /* thin  thick no         */
           {'‘Ÿ','‘¡','‘‘'},            /* thin  thick thin       */
           {'‘æ','‘æ','‘œ'}            /* thin  thick thick      */
         }
       },
       {
         { {'‘∫','‘”','‘»'},            /* thick no    no         */
           {'‘Ω','‘–','‘»'},            /* thick no    thin       */
           {'‘º','‘º','‘ '}             /* thick no    thick      */
         },
         { {'‘≥','‘⁄','‘’'},            /* thick thin  no         */
           {'‘ø','‘¬','‘’'},            /* thick thin  thin       */
           {'‘∏','‘∏','‘—'}             /* thick thin  thick      */
         },
         { {'‘∫','‘«','‘Ã'},            /* thick thick no         */
           {'‘∂','‘◊','‘∂'},            /* thick thick thin       */
           {'‘π','‘π','‘Œ'}             /* thick thick thick      */
         }
       }
     },

     /* BoxParty 2 */

     {
       {
         { {'‘∫','‘ƒ','‘Õ'},            /* no    no    no         */
           {'‘ƒ','‘ƒ','‘ƒ'},            /* no    no    thin       */
           {'‘Õ','‘ƒ','‘Õ'}             /* no    no    thick      */
         },
         { {'‘≥','‘⁄','‘’'},             /* no    thin  no         */
           {'‘ø','‘¬','‘ø'},             /* no    thin  thin       */
           {'‘∏','‘∏','‘—'}              /* no    thin  thick      */
         },
         { {'‘∫','‘÷','‘…'},             /* no    thick no         */
           {'‘∑','‘“','‘∑'},             /* no    thick thin       */
           {'‘ª','‘÷','‘À'}              /* no    thick thick      */
         }
       },
       {
         { {'‘≥','‘¿','‘‘'},            /* thin  no    no         */
           {'‘Ÿ','‘¡','‘Ÿ'},            /* thin  no    thin       */
           {'‘æ','‘¿','‘œ'}             /* thin  no    thick      */
         },
         { {'‘≥','‘√','‘∆'},             /* thin  thin  no         */
           {'‘¥','‘≈','‘¥'},             /* thin  thin  thin       */
           {'‘µ','‘√','‘ÿ'}              /* thin  thin  thick      */
         },
         { {'‘∫','‘÷','‘…'},             /* thin  thick no         */
           {'‘∑','‘“','‘∑'},             /* thin  thick thin       */
           {'‘ª','‘÷','‘À'}              /* thin  thick thick      */
         }
       },
       {
         { {'‘∫','‘”','‘»'},            /* thick no    no         */
           {'‘Ω','‘–','‘Ω'},            /* thick no    thin       */
           {'‘º','‘”','‘ '}             /* thick no    thick      */
         },
         { {'‘∫','‘”','‘»'},             /* thick thin  no         */
           {'‘Ω','‘–','‘Ω'},             /* thick thin  thin       */
           {'‘º','‘”','‘ '}              /* thick thin  thick      */
         },
         { {'‘∫','‘«','‘Ã'},             /* thick thick no         */
           {'‘∂','‘◊','‘∂'},             /* thick thick thin       */
           {'‘π','‘«','‘Œ'}              /* thick thick thick      */
         }
       }
     },

     /* BoxParty 3 */

     {
       {
         { {'‘∫','‘ƒ','‘Õ'},            /* no    no    no         */
           {'‘ƒ','‘ƒ','‘Õ'},            /* no    no    thin       */
           {'‘Õ','‘Õ','‘Õ'}             /* no    no    thick      */
         },
         { {'‘≥','‘⁄','‘’'},             /* no    thin  no         */
           {'‘ø','‘¬','‘’'},             /* no    thin  thin       */
           {'‘∏','‘∏','‘—'}              /* no    thin  thick      */
         },
         { {'‘∫','‘÷','‘…'},             /* no    thick no         */
           {'‘∑','‘“','‘…'},             /* no    thick thin       */
           {'‘ª','‘ª','‘À'}              /* no    thick thick      */
         }
       },
       {
         { {'‘≥','‘¿','‘‘'},            /* thin  no    no         */
           {'‘≥','‘√','‘∆'},            /* thin  no    thin       */
           {'‘∫','‘÷','‘…'}            /* thin  no    thick      */
         },
         { {'‘≥','‘√','‘∆'},             /* thin  thin  no         */
           {'‘¥','‘≈','‘∆'},             /* thin  thin  thin       */
           {'‘µ','‘µ','‘ÿ'}              /* thin  thin  thick      */
         },
         { {'‘∫','‘÷','‘…'},             /* thin  thick no         */
           {'‘∑','‘“','‘…'},             /* thin  thick thin       */
           {'‘ª','‘ª','‘À'}              /* thin  thick thick      */
         }
       },
       {
         { {'‘∫','‘”','‘»'},            /* thick no    no         */
           {'‘Ω','‘–','‘»'},            /* thick no    thin       */
           {'‘º','‘º','‘ '}            /* thick no    thick      */
         },
         { {'‘∫','‘”','‘»'},             /* thick thin  no         */
           {'‘Ω','‘–','‘»'},             /* thick thin  thin       */
           {'‘º','‘º','‘ '}             /* thick thin  thick      */
         },
         { {'‘∫','‘«','‘Ã'},              /* thick thick no         */
           {'‘∂','‘◊','‘Ã'},              /* thick thick thin       */
           {'‘π','‘π','‘Œ'}              /* thick thick thick      */
         }
       }
     },


     /* BoxParty 4 */

     {
       {
         { {'€¢','€°','€¨'},            /* no    no    no         */
           {'€°','€°','€°'},            /* no    no    thin       */
           {'€¨','€°','€¨'}            /* no    no    thick      */
         },
         { {'€¢','€£','€»'},             /* no    thin  no         */
           {'€§','€®','€“'},             /* no    thin  thin       */
           {'€¬','€—','€∏'}             /* no    thin  thick      */
         },
         { {'€≠','€«','€Æ'},              /* no    thick no         */
           {'€¡','€Ω','€‘'},              /* no    thick thin       */
           {'€Ø','€”','€≥'}              /* no    thick thick      */
         }
       },
       {
         { {'€¢','€¶','€∆'},            /* thin  no    no         */
           {'€•','€™','€÷'},            /* thin  no    thin       */
           {'€ƒ','€’','€∫'}            /* thin  no    thick      */
         },
         { {'€¢','€ß','€º'},             /* thin  thin  no         */
           {'€©','€´','€⁄'},             /* thin  thin  thin       */
           {'€æ','€Ÿ','€ª'}             /* thin  thin  thick      */
         },
         { {'€¢','€ ','€Ã'},              /* thin  thick no         */
           {'€Œ','€‹','€‡'},              /* thin  thick thin       */
           {'€–','€ﬂ','€‚'}              /* thin  thick thick      */
         }
       },
       {
         { {'€≠','€≈','€±'},            /* thick no    no         */
           {'€√','€ø','€ÿ'},            /* thick no    thin       */
           {'€∞','€◊','€µ'}            /* thick no    thick      */
         },
         { {'€¢','€…','€À'},             /* thick thin  no         */
           {'€Õ','€€','€ﬁ'},             /* thick thin  thin       */
           {'€œ','€›','€·'}             /* thick thin  thick      */
         },
         { {'€≠','€∑','€≤'},              /* thick thick no         */
           {'€π','€¿','€‰'},              /* thick thick thin       */
           {'€¥','€„','€∂'}              /* thick thick thick      */
         }
       }
     },

     /* BoxParty 5 */

     {
       {
         { {'€¢','€°','€¨'},            /* no    no    no         */
           {'€°','€°','€¨'},            /* no    no    thin       */
           {'€¨','€¨','€¨'}            /* no    no    thick      */
         },
         { {'€¢','€£','€»'},             /* no    thin  no         */
           {'€§','€®','€“'},             /* no    thin  thin       */
           {'€¬','€—','€∏'}             /* no    thin  thick      */
         },
         { {'€≠','€«','€Æ'},              /* no    thick no         */
           {'€¡','€Ω','€‘'},              /* no    thick thin       */
           {'€Ø','€”','€≥'}              /* no    thick thick      */
         }
       },
       {
         { {'€¢','€¶','€∆'},            /* thin  no    no         */
           {'€•','€™','€÷'},            /* thin  no    thin       */
           {'€ƒ','€’','€∫'}            /* thin  no    thick      */
         },
         { {'€¢','€ß','€º'},             /* thin  thin  no         */
           {'€©','€´','€⁄'},             /* thin  thin  thin       */
           {'€æ','€Ÿ','€ª'}             /* thin  thin  thick      */
         },
         { {'€¢','€ ','€Ã'},              /* thin  thick no         */
           {'€Œ','€‹','€‡'},              /* thin  thick thin       */
           {'€–','€ﬂ','€‚'}              /* thin  thick thick      */
         }
       },
       {
         { {'€≠','€≈','€±'},            /* thick no    no         */
           {'€√','€ø','€ÿ'},            /* thick no    thin       */
           {'€∞','€◊','€µ'}            /* thick no    thick      */
         },
         { {'€¢','€…','€À'},             /* thick thin  no         */
           {'€Õ','€€','€ﬁ'},             /* thick thin  thin       */
           {'€œ','€›','€·'}             /* thick thin  thick      */
         },
         { {'€≠','€∑','€≤'},              /* thick thick no         */
           {'€π','€¿','€‰'},              /* thick thick thin       */
           {'€¥','€„','€∂'}              /* thick thick thick      */
         }
       }
     },

     /* BoxParty 6 */

     {
       {
         { {'€¢','€°','€¨'},            /* no    no    no         */
           {'€°','€°','€°'},            /* no    no    thin       */
           {'€¨','€°','€¨'}            /* no    no    thick      */
         },
         { {'€¢','€£','€»'},             /* no    thin  no         */
           {'€§','€®','€“'},             /* no    thin  thin       */
           {'€¬','€—','€∏'}             /* no    thin  thick      */
         },
         { {'€≠','€«','€Æ'},              /* no    thick no         */
           {'€¡','€Ω','€‘'},              /* no    thick thin       */
           {'€Ø','€”','€≥'}              /* no    thick thick      */
         }
       },
       {
         { {'€¢','€¶','€∆'},            /* thin  no    no         */
           {'€•','€™','€÷'},            /* thin  no    thin       */
           {'€ƒ','€’','€∫'}            /* thin  no    thick      */
         },
         { {'€¢','€ß','€º'},             /* thin  thin  no         */
           {'€©','€´','€⁄'},             /* thin  thin  thin       */
           {'€æ','€Ÿ','€ª'}             /* thin  thin  thick      */
         },
         { {'€≠','€ ','€Ã'},              /* thin  thick no         */
           {'€Œ','€‹','€‡'},              /* thin  thick thin       */
           {'€–','€ﬂ','€‚'}              /* thin  thick thick      */
         }
       },
       {
         { {'€≠','€≈','€±'},            /* thick no    no         */
           {'€√','€ø','€ÿ'},            /* thick no    thin       */
           {'€∞','€◊','€µ'}            /* thick no    thick      */
         },
         { {'€≠','€…','€À'},             /* thick thin  no         */
           {'€Õ','€€','€ﬁ'},             /* thick thin  thin       */
           {'€œ','€›','€·'}             /* thick thin  thick      */
         },
         { {'€≠','€∑','€≤'},              /* thick thick no         */
           {'€π','€¿','€‰'},              /* thick thick thin       */
           {'€¥','€„','€∂'}              /* thick thick thick      */
         }
       }
     },

     /* BoxParty 7 */

     {
       {
         { {'€≠','€°','€¨'},            /* no    no    no         */
           {'€°','€°','€¨'},            /* no    no    thin       */
           {'€¨','€¨','€¨'}            /* no    no    thick      */
         },
         { {'€¢','€£','€»'},             /* no    thin  no         */
           {'€§','€®','€“'},             /* no    thin  thin       */
           {'€¬','€—','€∏'}             /* no    thin  thick      */
         },
         { {'€≠','€«','€Æ'},              /* no    thick no         */
           {'€¡','€Ω','€‘'},              /* no    thick thin       */
           {'€Ø','€”','€≥'}              /* no    thick thick      */
         }
       },
       {
         { {'€¢','€¶','€∆'},            /* thin  no    no         */
           {'€•','€™','€÷'},            /* thin  no    thin       */
           {'€ƒ','€’','€∫'}            /* thin  no    thick      */
         },
         { {'€¢','€ß','€º'},             /* thin  thin  no         */
           {'€©','€´','€⁄'},             /* thin  thin  thin       */
           {'€æ','€Ÿ','€ª'}             /* thin  thin  thick      */
         },
         { {'€≠','€ ','€Ã'},              /* thin  thick no         */
           {'€Œ','€‹','€‡'},              /* thin  thick thin       */
           {'€–','€ﬂ','€‚'}              /* thin  thick thick      */
         }
       },
       {
         { {'€≠','€≈','€±'},            /* thick no    no         */
           {'€√','€ø','€ÿ'},            /* thick no    thin       */
           {'€∞','€◊','€µ'}            /* thick no    thick      */
         },
         { {'€≠','€…','€À'},             /* thick thin  no         */
           {'€Õ','€€','€ﬁ'},             /* thick thin  thin       */
           {'€œ','€›','€·'}             /* thick thin  thick      */
         },
         { {'€≠','€∑','€≤'},              /* thick thick no         */
           {'€π','€¿','€‰'},              /* thick thick thin       */
           {'€¥','€„','€∂'}              /* thick thick thick      */
         }
       }
     }
   };


void DrawNewPosition(void)
{
    word j;

    SetUpF();
    SetDownF();
    SetLeftF();
    SetRightF();
    *(word*)(EditBuf+X)=LineTable[BoxStyle][UpF][DownF][LeftF][RightF];
    EditType[X  ]=1;
    EditType[X+1]=2;

    if (X>2 && EditType[X-1]==1) {
        EditType[X-1]=0;
        EditBuf [X-1]=' ';
    }
    if (X<253 && EditType[X+2]==2) {
        EditType[X+2]=0;
        EditBuf [X+2]=' ';
    }
}

void DrawOldPosition(void)
{
   switch (Direction) {
     case Up    : switch (BoxStyle) {
                    case 0 :
                    case 1 :
                    case 4 :
                    case 5 : UpF=Thin;  break;
                    case 2 :
                    case 3 :
                    case 6 :
                    case 7 : UpF=Thick; break;
                  }
                  break;
     case Down  : switch (BoxStyle) {
                    case 0 :
                    case 1 :
                    case 4 :
                    case 5 : DownF=Thin;  break;
                    case 2 :
                    case 3 :
                    case 6 :
                    case 7 : DownF=Thick; break;
                  }
                  break;
     case Left  : switch (BoxStyle) {
                    case 0 :
                    case 2 :
                    case 4 :
                    case 6 : LeftF=Thin; break;
                    case 1 :
                    case 3 :
                    case 5 :
                    case 7 : LeftF=Thick; break;
                  }
                  break;
     case Right : switch (BoxStyle) {
                    case 0 :
                    case 2 :
                    case 4 :
                    case 6 : RightF=Thin; break;
                    case 1 :
                    case 3 :
                    case 5 :
                    case 7 : RightF=Thick; break;
                  }
                  break;
   }

   *(word*)(EditBuf+OldX)=LineTable[BoxStyle][UpF][DownF][LeftF][RightF];
   EditType[OldX  ]=1;
   EditType[OldX+1]=2;
   if (OldY!=Y) WriteLine(SY,EditBuf+1);
}

void EraseNewPosition(void)
{

    DrawNewPosition();

/*
    word j;

    SetUpF();
    SetDownF();
    SetLeftF();
    SetRightF();

    if (OldX!=X)
       if (EditBuf[X+1]==LineStyle[Class][0] ||
           EditBuf[X+1]==LineStyle[Class][2] ||
           EditBuf[X+1]==LineStyle[Class][6] ||
           EditBuf[X+1]==LineStyle[Class][8] ||
           EditBuf[X+1]==LineStyle[Class][10]) {
             EditBuf [X  ]=' ';
             EditBuf [X+1]=' ';
             EditType[X  ]=0;
             EditType[X+1]=0;
       } else
             EditBuf[X+1]=LineStyle[Class][j];
    if (Y!=OldY)
       if (EditBuf[X+1]==LineStyle[Class][0] ||
           EditBuf[X+1]==LineStyle[Class][2] ||
           EditBuf[X+1]==LineStyle[Class][6] ||
           EditBuf[X+1]==LineStyle[Class][8] ||
           EditBuf[X+1]==LineStyle[Class][9]) {
             EditBuf [X  ]=' ';
             EditBuf [X+1]=' ';
             EditType[X  ]=0;
             EditType[X+1]=0;
       } else
             EditBuf[X+1]=LineStyle[Class][j];
    if (X>2 && EditType[X-1]==1) {
        EditType[X-1]=0;
        EditBuf [X-1]=' ';
    }
    if (X<253 && EditType[X+2]==2) {
        EditType[X+2]=0;
        EditBuf [X+2]=' ';
    }
  */
}

void EraseOldPosition(void)
{
   if (!LineCharF) return;

   switch (Direction) {
     case Up    : switch (BoxStyle) {
                    case 0 :
                    case 1 :
                    case 4 :
                    case 5 : UpF=NoLine;  break;
                    case 2 :
                    case 3 :
                    case 6 :
                    case 7 : UpF=NoLine; break;
                  }
                  if (!LeftF && !RightF) DownF=NoLine;
                  break;
     case Down  : switch (BoxStyle) {
                    case 0 :
                    case 1 :
                    case 4 :
                    case 5 : DownF=NoLine;  break;
                    case 2 :
                    case 3 :
                    case 6 :
                    case 7 : DownF=NoLine; break;
                  }
                  if (!LeftF && !RightF) UpF=NoLine;
                  break;
     case Left  : switch (BoxStyle) {
                    case 0 :
                    case 2 :
                    case 4 :
                    case 6 : LeftF=NoLine; break;
                    case 1 :
                    case 3 :
                    case 5 :
                    case 7 : LeftF=NoLine; break;
                  }
                  if (!UpF && !DownF) RightF=NoLine;
                  break;
     case Right : switch (BoxStyle) {
                    case 0 :
                    case 2 :
                    case 4 :
                    case 6 : RightF=NoLine; break;
                    case 1 :
                    case 3 :
                    case 5 :
                    case 7 : RightF=NoLine; break;
                  }
                  if (!UpF && !DownF) LeftF=NoLine;
                  break;
   }

   if (OldX!=X || OldY!=Y) {
      if (RightF || LeftF || UpF || DownF) {
         if (EditBuf[OldX]==0xd4)
           *(word*)(EditBuf+OldX)=LineTable[0][UpF][DownF][LeftF][RightF];
         else
           *(word*)(EditBuf+OldX)=LineTable[4][UpF][DownF][LeftF][RightF];
         EditType[OldX  ]=1;
         EditType[OldX+1]=2;
      } else {
         EditBuf[OldX]=EditBuf[OldX+1]=' ';
         EditType[OldX]=EditType[OldX+1]=0;
      }
   }
   if (OldY!=Y)  WriteLine(SY,EditBuf+1);
}

word SelectLineStyle(void)
{
    word i,j,WX;

    Push(InverseF);
    HOpenWindowSimple(0,VideoBot,VideoRight,VideoBot);
    HNormal();
    HClrBox(0,VideoBot,VideoRight,VideoBot);
    HWriteMid(VideoBot,"¨Â ¢Ö∏aüi ¨Â»Ç–a≠A∂a.");
    HInverse();
    WX=VideoRight-52;
    HOpenWindow(WX,3,WX+49,7);
    DrawString(WX+1,4,"‘⁄‘¬‘ø‘’‘—‘∏‘÷‘“‘∑‘…‘À‘ª€£€®€§€»€∏€¬€«€Ω€¡€Æ€≥€Ø");
    DrawString(WX+1,5,"‘√‘≈‘¥‘∆‘ÿ‘µ‘«‘◊‘∂‘Ã‘Œ‘π€ß€´€©€º€ª€æ€∑€¿€π€≤€∂€¥");
    DrawString(WX+1,6,"‘¿‘¡‘Ÿ‘‘‘œ‘æ‘”‘–‘Ω‘»‘ ‘º€¶€™€•€∆€∫€ƒ€≈€ø€√€±€µ€∞");
    j=BoxStyle;
    do {
       HSetInverse(WX+j*6+1,4,WX+j*6+6,6);
       GetKey();
       HSetInverse(WX+j*6+1,4,WX+j*6+6,6);
       SetHotKeyF();
       if (HotKeyF) break;
       switch (Way) {
          case Left  :
          case CtrlS :
          case Up    :
          case CtrlE :if (j==0) j=7;
                      else      j--;
                      break;
          case Right :
          case CtrlD :
          case Down  :
          case CtrlX :if (j==7) j=0;
                      else      j++;
                      break;
          case End   :
          case PgDn  :
          case CtrlC :j=7;
                      break;
          case Home  :
          case PgUp  :
          case CtrlR :j=0;
                      break;
          case Return:BoxStyle=j;
                      break;
          default    :if (Way>='1' && Way<='8') {
                        BoxStyle=Way-'1';
                        Way=Return;
                      }
       }
    } while (Way!=Return && Way!=ESC && !HotKeyF);
    BoxParty=(BoxStyle>3)?1:0;
    HCloseWindow();
    HCloseWindowSimple();
    Pop(InverseF);
    if (Way==ESC) {
      Way=Return;
      return Error;
    } return OK;
}

void SetGrfChar(void)
{
   word j;

   j=SelectGrf();
   if (j!=Error) {
       GrfCharH=j>>8;
       GrfCharL=j;
       SetMenuItem();
       Mode=LastDrawMode=GrfMode;
       SetMenuItem();
   }
}

void EditDraw(void)
{

     word j;

     if (((HX+SX-1)&0x0001)==0) SetCurPos(HX+SX-2,NowLine);
     SetLineMenu();
     X=OldX=HX+SX-1;
     Y=OldY=NowLine;
     do {

/*....................................................Set Position............*/

        if (X>253 && X<300) SetCurPos(253,Y);
        else                SetCurPos(X,Y);
        PrintXYpos();
        X=HX+SX-1;
        Y=NowLine;
        if (!EditedF)
          if (Mode==DrawMode|| Mode==GrfMode) {
            EditedF=True;
            SetEditScn();
            PrintXYpos();
        }

/*....................................................Ready...................*/

        LineCharF=LineChar();
        GrfCharF =(!LineCharF && IsGrf(EditBuf[X]) &&
                   EditType[X]==1 ) ? True:False;
        if      (Mode==DrawMode) DrawNewPosition();
        else if (Mode==GrfMode ) DrawGrf();
        else if (Mode==EraseMode && (LineCharF || GrfCharF   ))
                if (GrfCharF)
                   EraseGrf();
                else
                   EraseNewPosition();


/*....................................................Get Order...............*/

        if (SetScnF)        SetScn();
        else                WriteLine(SY,EditBuf+1);
        if (Mode==MoveMode) EngCur2(SX-1,SY-1);
        else                HanCur2(SX-1,SY-1);
        GetKey();
        SetHotKeyF();
        if (HotKeyF) break;
        OldX=X;
        OldY=Y;
        ScrollCommand=Error;
        Direction=Error;
        switch (Way) {
           case Up   :
           case CtrlE:Direction=Up;    Y--;   break;
           case Down :
           case CtrlX:Direction=Down;  Y++;   break;
           case Right:
           case CtrlD:Direction=Right; X+=2;  break;
           case Left :
           case CtrlS:Direction=Left;  X-=2;  break;
           case CtrlZ:if (HY+SY-ScnTop!=PaperLen || SY!=ScnTop) {
                         if (SY>ScnTop) HY=HY+1,SY=SY-1;
                         else           {
                            ScrollCommand=CtrlZ;
                            Y++;
                            Direction=Down;
                         }
                         SetScnF=ON;
                      }
                      break;
           case CtrlW:if (HY>1) {
                         if (SY<ScnBot) HY=HY-1,SY=SY+1;
                         else {
                           ScrollCommand=CtrlW;
                           Y--;
                           Direction=Up;
                         }
                         SetScnF=ON;
                      }
                      break;
           case ' '  :SetMenuItem();
                      if (Mode==MoveMode)  Mode=LastDrawMode;
                      else                 Mode=MoveMode;
                      SetMenuItem();
                      break;
           case 'd'  :
           case 'D'  :SetMenuItem();
                      Mode=LastDrawMode=DrawMode;
                      SetMenuItem();
                      break;
           case 'f'  :
           case 'F'  :
           case 'm'  :
           case 'M'  :SetMenuItem();
                      Mode=MoveMode;
                      SetMenuItem();
                      break;
           case 'e'  :
           case 'E'  :SetMenuItem();
                      Mode=LastDrawMode=EraseMode;
                      SetMenuItem();
                      break;
           case 's'  :
           case 'S'  :
           case 'l'  :
           case 'L'  :SelectLineStyle();
                      SetMenuItem();
                      Mode=LastDrawMode=DrawMode;
                      SetMenuItem();
                      break;
           case F4   :
           case 'a'  :
           case 'A'  :
           case 'g'  :
           case AltG :
           case 'G'  :SetGrfChar();                  break;
           default   :if (Way>='1' && Way<='8') {
                         BoxStyle=Way-'1';
                         BoxParty=(BoxStyle>3)? 1 :0;
                         SetMenuItem();
                         Mode=LastDrawMode=DrawMode;
                         SetMenuItem();
                      }
                      break;
        }

/* ............................................................Access.........*/

        switch (Mode) {
           case DrawMode :DrawOldPosition();
           case GrfMode  :if (Y>PaperLen) InsertBlankLine(PaperLen);  break;
           case EraseMode:if (!LineCharF) break;
                          EraseOldPosition();
        }

        if (ScrollCommand==CtrlZ) {
           if (PutEditBuf()!=OK) return;
           HY++;
           NowLine++;
           SetEditBuf();
           if (SelectingBlockF) SetBlockPos();
        } else if (ScrollCommand==CtrlW) {
             if (PutEditBuf()!=OK) return;
             HY--;
             NowLine--;
             SetEditBuf();
             if (SelectingBlockF) SetBlockPos();
        }

     } while (Way!=ESC);

     SetMenuScn();

}

void PutLineChar(word X,word Y,word Type,word Code)
{
    word i;

    SetAuxBuf(Y);
    *(word*)(AuxBuf+X)=LineStyle[Type][Code];
    AuxType[X  ]=1;
    AuxType[X+1]=2;
    if (X>2 && AuxType[X-1]==1) {
       AuxBuf [X-1]=' ';
       AuxType[X-1]=0;
    }
    if (X<253 && AuxType[X+2]==2) {
       AuxBuf [X+2]=' ';
       AuxType[X+2]=0;
    }
    i=254;
    while (AuxBuf[i]==' ' && AuxType[i]==0 && i>0 ) i--;
    AuxBuf[i+1]=0;
    PutPaper(Y,AuxBuf+1);
}

void PutGrfChar(word X,word Y,word Code)
{
    word i;

    SetAuxBuf(Y);
    AuxBuf [X  ]=Code;
    AuxType[X  ]=1;
    AuxType[X+1]=2;
    AuxBuf [X+1]=Code>>8;
    if (X>2 && AuxType[X-1]==1) {
       AuxBuf [X-1]=' ';
       AuxType[X-1]=0;
    }
    if (X<253 && AuxType[X+2]==2) {
       AuxBuf [X+2]=' ';
       AuxType[X+2]=0;
    }
    i=254;
    while (AuxBuf[i]==' ' && AuxType[i]==0 && i>0 ) i--;
    AuxBuf[i+1]=0;
    PutPaper(Y,AuxBuf+1);
}


void EditCKD(void)
{
    word i,j,Type,S,E,HeadF,TailF;
    word OldX,OldY;
    unsigned long BlockSize;

    OldX=CurXPos();
    OldY=NowLine;
    if (PutEditBuf()!=OK) return;
    if (!ShowBlockF || BlockType!=2) return;
    if (BlockEY==PaperLen) InsertBlankLine(PaperLen);
    for (BlockSize=0,i=BlockSY;i<=BlockEY;i++) BlockSize+=16*(Paper[i][0]/ 16 +1);
    if (farcoreleft()<BlockSize*2+MaxBufRest) {
        BufErr();
        return;
    }
    for (i=BlockSY ; i<=BlockEY ; i++)
        if (Paper[i][0]+7>254) {
           HSayMes("ê·¢Å ã• ∫â∑° ¨óã≥ì°îa.");
           return;
        }
    HeadF=TailF=False;
    for (i=BlockSY; i<=BlockEY; i++) {
         SetAuxBuf(i);
         if (AuxType[BlockSX]==2) HeadF=True;
         if (AuxType[BlockEX]==1) TailF=True;
         if (HeadF && TailF) break;
    }
    if (BlockSX-HeadF<3 || BlockEX+TailF>252 || BlockSY==1) {
         HSayMes("…AñÅü°üi ãaü© µa∑Aàa ¥ÙØsì°îa.");
         return;
    }
    HSayBottom("…AñÅü° π∑üAüi ¨Â»Ç–a≠A∂a [L/1..8] [G/F4]");
    if (Way==AltG || Way==F4 || Way=='g' || Way=='G') {
       j=SelectGrf();
       if (j==Error) return;
    } else if (Way=='l' || Way=='L') {
       j=BoxStyle;
       if (SelectLineStyle()!=OK) return;
       Type=BoxStyle;
       BoxStyle=j;
       j=0;
    } else {
       Type=Way-'1';
       if (Type>7) return;
       j=0;
    }
    S=BlockSX-HeadF-2;
    E=BlockEX+TailF+1;
    if ((E-S)%2!=0) E++;
    if (!j) {
      for (i=S ; i<=E ; i+=2) PutLineChar(i,BlockSY-1,Type,9);
      for (i=S ; i<=E ; i+=2) PutLineChar(i,BlockEY+1,Type,9);
      for (i=BlockSY ; i<=BlockEY ; i++)
                              PutLineChar(S,i,Type,10),
                              PutLineChar(E,i,Type,10);
      PutLineChar(S,BlockSY-1,Type,0);
      PutLineChar(S,BlockEY+1,Type,6);
      PutLineChar(E,BlockSY-1,Type,2);
      PutLineChar(E,BlockEY+1,Type,8);
    } else {
      for (i=S ; i<=E ; i+=2) PutGrfChar(i,BlockSY-1,j);
      for (i=S ; i<=E ; i+=2) PutGrfChar(i,BlockEY+1,j);
      for (i=BlockSY ; i<=BlockEY ; i++)
                              PutGrfChar(S,i,j),
                              PutGrfChar(E,i,j);
    }
    SetEditBuf();
    SetCurPos(OldX,OldY);
    SetScnF=True;
    SelectingBlockF=False;
}

