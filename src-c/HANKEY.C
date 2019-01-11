/*                                                        */
/*                                                        */
/*        Han-Gul Graphic User Interface version 1.0      */
/*                                                        */
/*        programed by Magic Book                         */
/*                                                        */
/*        HanGul Input                                    */
/*                                                        */

#include "stddef.h"
#include "memory.h"
#include "CRT.H"
#include "handrv.H"

word HanKey;
word Temp;
word HanF=OFF;
word KeyboardType=2;
word ShiftF;
word HanResult=1;

byte KeyClass2[]=      { 0, 0, 0, 0, 0, 0, 0, 0,    /*  32...  */
                         0, 0, 0, 0, 0, 0, 0, 0,    /*  40...  */
                         0, 0, 0, 0, 0, 0, 0, 0,    /*  48...  */
                         0, 0, 0, 0, 0, 0, 0, 0,    /*  56...  */
                         0, 1, 2, 1, 1, 1, 1, 1,    /*  64...  */
                         2, 2, 2, 2, 2, 2, 2, 2,    /*  72...  */
                         2, 1, 1, 1, 1, 2, 1, 1,    /*  80...  */
                         1, 2, 1                    /*  88...  */
                       };

byte ToHanTable2[]=    { 0, 0, 0, 0, 0, 0, 0, 0,    /*  32...  */
                         0, 0, 0, 0, 0, 0, 0, 0,    /*  40...  */
                         0, 0, 0, 0, 0, 0, 0, 0,    /*  48...  */
                         0, 0, 0, 0, 0, 0, 0, 0,    /*  56...  */
                         0, 8,26,16,13, 5, 7,20,    /*  64...  */
                        13, 5, 7, 3,29,27,20, 4,    /*  72...  */
                        10, 9, 2, 4,11,11,19,14,    /*  80...  */
                        18,19,17                    /*  88...  */
                       };

byte ToHanTableS2[]=   { 0, 0, 0, 0, 0, 0, 0, 0,    /*  32...  */
                         0, 0, 0, 0, 0, 0, 0, 0,    /*  40...  */
                         0, 0, 0, 0, 0, 0, 0, 0,    /*  48...  */
                         0, 0, 0, 0, 0, 0, 0, 0,    /*  56...  */
                         0, 8,26,16,13, 6, 7,20,    /*  64...  */
                        13, 5, 7, 3,29,27,20, 6,    /*  72...  */
                        12,10, 3, 4,12,11,19,15,    /*  80...  */
                        18,19,17                    /*  88...  */
                       };
byte JungMixTable[7][3] = { {13, 3,14}, {13, 4,15}, {13,29,18}, {20, 7,21},
                            {20,10,22}, {20,29,23}, {27,29,28}
                          };

byte JongMixTable[11][3]= { { 2,11, 4}, { 4,14, 6}, { 4,20, 7}, { 7, 2,10},
                            { 7, 8,11}, { 7, 9,12}, { 7,11,13}, { 7,18,14},
                            { 7,19,15}, { 7,20,16}, { 9,11,20} };
byte JongMixTable3[13][3]= { {2,2,3},{2,21,4},{5,24,6},{5,29,7},{9,2,10},
                            {9,17,11},{9,19,12},{9,21,13},{9,27,14},
                            {9,28,15},{9,29,16},{19,21,20},{21,21,22}};
byte ChoToJongTable[]   = {  1, 1, 2, 3, 5, 8, 1, 9,17,19, 1,
                            21,22,23,24, 1,25,26,27,28,29 };

word near MakeJong (word Jong1,word Jong2)
{
     word i;

     if (Jong2==1) return ChoToJongTable[Jong1];
     for (i=0; i<11 ; i++)
       if (Jong1==JongMixTable[i][0] && Jong2==JongMixTable[i][1] )
           return JongMixTable[i][2];
     return Error;
}

word near MakeJong3 (word Jong1,word Jong2)
{
     word i;

     if (Jong2==1) return Jong1;
     for (i=0; i<13 ; i++)
       if (Jong1==JongMixTable3[i][0] && Jong2==JongMixTable3[i][1] )
           return JongMixTable3[i][2];
     return Error;
}

word near MakeJung (word Jung1,word Jung2)
{
     word i;

     if (Jung2==2) return Jung1;
     for (i=0; i<7 ; i++)
       if (Jung1==JungMixTable[i][0] && Jung2==JungMixTable[i][1] )
           return JungMixTable[i][2];
     return Error;
}

void near ToHan(void)
{
     if (ShiftF) Temp=ToHanTableS2[Temp];
     else                Temp=ToHanTable2 [Temp];
}

#define DoubleCho()  ((Temp==2||Temp==5||Temp==9||Temp==11||Temp==14)?OK:Error)
#define DoubleJung() ((Temp==13 || Temp==20 || Temp==27)?OK:Error)
#define NotJong()    ((Temp==6 || Temp==10 || Temp==15)?OK:Error)

void near GetHan2(void)
{
     static word Step=1;
     static word Cho=1,Jung1=2,Jung2=2,Jong1=1,Jong2=1;
     static word ChoBuf=1,JungBuf=2;

     if (!HanF) {
        GetKey();
        HanResult=5;
        return;
     }
     if (HanResult!=1 && HanResult!=0) {
         Cho=Jong1=Jong2=1;
         Jung1=Jung2=2;
         if (ChoBuf<2 && JungBuf<3) Step=1;
     }
     if (ChoBuf>1 || JungBuf>2) {
         Cho      =ChoBuf;
         Jung1    =JungBuf;
         ChoBuf   =1;
         JungBuf  =2;
         HanResult=0;
     } else {
         if (HanResult==0) HanResult=1;
         GetKey();
         ShiftF=ShiftPressed();
         Temp=Way;
         if (Held(Temp,'a','z')) Temp-=32+32;
         else                    Temp-=32;
         if (Way!=CtrlH && (Way<33 || Way>127) )
             if (Step==1) HanResult=5;
             else         HanResult=4;
         else
             switch  (Step) {
                case 1 :if (Way==CtrlH) {
                           HanResult=5;
                           break;
                        }
                        switch (KeyClass2[Temp]) {
                           case 1 :ToHan();
                                   Cho=Temp;
                                   Step=2;
                                   HanResult=0;
                                   break;
                           case 2 :HanResult=0;
                                   goto Mo3;
                           default:HanResult=5;
                        }
                        break;
                case 2 :if (Way==CtrlH) {
                            Cho=1;
                            Step=1;
                            break;
                        }
                        switch (KeyClass2[Temp]) {
                           case 1 :ToHan();
                                   if (Cho==Temp && DoubleCho()) {
                                       Cho++;
                                       Step=3;
                                   } else {
                                       ChoBuf=Temp;
                                       Step=2;
                                       HanResult=3;
                                   }
                                   break;
                           case 2 :goto Mo3;
                           default:HanResult=4;
                        }
                        break;
                case 3 :if (Way==CtrlH) {
                            Cho=1;
                            Step=1;
                            break;
                        }
                        switch (KeyClass2[Temp]) {
                           case 1 :ToHan();
                                   ChoBuf=Temp;
                                   HanResult=3;
                                   break;
                           case 2 :
                               Mo3:ToHan();
                                   Jung1=Temp;
                                   Step=(DoubleJung())? 4:5;
                                   break;
                           default:HanResult=(Cho==1)?5:4;
                        }
                        break;
                case 4 :if (Way==CtrlH) {
                            Jung1=2;
                            Step=3;
                            break;
                        }
                        switch (KeyClass2[Temp]) {
                           case 1 :goto Ja5;
                           case 2 :ToHan();
                                   if ( MakeJung(Jung1,Temp) !=Error) {
                                       Jung2=Temp;
                                       Step=5;
                                   } else {
                                       JungBuf=Temp;
                                       Step=(DoubleJung())?4:5;
                                       HanResult=3;
                                   }
                                   break;
                           default:HanResult=4;
                        }
                        break;
                case 5 :if (Way==CtrlH) {
                            if (Jung2==2) {  Jung1=2;   Step=3;  }
                            else          {  Jung2=2;   Step=4;  }
                            break;
                        }
                        switch (KeyClass2[Temp]) {
                           case 1 :
                               Ja5:ToHan();
                                   if (NotJong()) {
                                       ChoBuf=Temp;
                                       Step=3;
                                       HanResult=3;
                                   } else {
                                       Jong1=Temp;
                                       Step=6;
                                   }
                                   break;
                           case 2 :ToHan();
                                   JungBuf=Temp;
                                   Step=(DoubleJung())? 4:5;
                                   HanResult=3;
                                   break;
                           default:HanResult=4;
                        }
                        break;
                case 6 :if (Way==CtrlH) {
                            Jong1=1;
                            Step=5;
                            break;
                        }
                        switch (KeyClass2[Temp]) {
                           case 1 :ToHan();
                                   if (MakeJong(Jong1,Temp)!=Error) {
                                       Jong2=Temp;
                                       Step=7;
                                   } else {
                                       ChoBuf=Temp;
                                       Step=2;
                                       HanResult=3;
                                   }
                                   break;
                           case 2 :ToHan();
                                   ChoBuf=Jong1;
                                   JungBuf=Temp;
                                   Jong1=1;
                                   Step=(DoubleJung())? 4:5;
                                   HanResult=3;
                                   break;
                           default:HanResult=4;
                        }
                        break;
                case 7 :if (Way==CtrlH) {
                            Jong2=1;
                            Step=6;
                            break;
                        }
                        switch (KeyClass2[Temp]) {
                           case 1 :ToHan();
                                   ChoBuf=Temp;
                                   Step=2;
                                   HanResult=3;
                                   break;
                           case 2 :ToHan();
                                   ChoBuf=Jong2;
                                   JungBuf=Temp;
                                   Jong2=1;
                                   Step=(DoubleJung())? 4:5;
                                   HanResult=3;
                                   break;
                           default:HanResult=4;
                        }
            }
     }
     if (HanResult!=4 && HanResult!=5 &&
         Cho==1 && Jong1==1 && Jong2==1 && Jung1==2 && Jung2==2) {
         HanResult=6;
         Way=65535;
     } else HanKey=0x0080+(Cho<<2)+(MakeJung(Jung1,Jung2)>>3)+
                   (MakeJung(Jung1,Jung2)<<5)*0x100+
                    MakeJong(Jong1,Jong2)*0x100;
}

byte KeyClass3[]=      { 0, 4, 0, 0, 0, 0, 0, 1,    /*  32...  */
                         0, 0, 0, 0, 0, 0, 0, 3,    /*  40...  */
                         1, 4, 4, 4, 2, 2, 2, 2,    /*  48...  */
                         2, 3, 0, 1, 6, 0, 6, 0,    /*  56...  */
                         0, 4, 3, 2, 2, 2, 2, 2,    /*  64...  */
                         1, 1, 1, 1, 1, 1, 1, 1,    /*  72...  */
                         1, 4, 2, 4, 2, 1, 3, 4,    /*  80...  */
                         4, 1, 4                    /*  88...  */
                       };

byte KeyClassS3[]=     { 0, 4, 0, 0, 0, 0, 0, 1,    /*  32...  */
                         0, 0, 0, 0, 0, 0, 0, 3,    /*  40...  */
                         1, 4, 4, 4, 2, 2, 2, 2,    /*  48...  */
                         2, 3, 0, 1, 6, 0, 6, 0,    /*  56...  */
                         0, 4, 6, 4, 4, 4, 4, 6,    /*  64...  */
                         6, 6, 6, 6, 6, 6, 6, 6,    /*  72...  */
                         6, 4, 2, 4, 6, 6, 4, 4,    /*  80...  */
                         4, 6, 4                    /*  88...  */
                       };

byte ToHanTable3[]=    { 0,24, 0, 0, 0, 0, 0,18,    /*  32...  */
                         0, 0, 0, 0, 0, 0, 0,13,    /*  40...  */
                        17,29,22,19,19,26, 5,12,    /*  48...  */
                        28,20, 0, 9,50, 0,51, 0,    /*  56...  */
                         0,23,20,10,29,11, 3,27,    /*  64...  */
                         4, 8,13, 2,14,20,11,16,    /*  72...  */
                        19,21, 4, 5, 7, 5,13, 9,    /*  80...  */
                         2, 7,17                    /*  88...  */
                       };

byte ToHanTableS3[]=   { 0,24, 0, 0, 0, 0, 0,18,    /*  32...  */
                         0, 0, 0, 0, 0, 0, 0,13,    /*  40...  */
                        17,29,22,19,19,26, 5,12,    /*  48...  */
                        28,20, 0, 9,50, 0,51, 0,    /*  56...  */
                         0, 8,33,11,10,26, 3,47,    /*  64...  */
                        39,56,52,53,54,49,48,57,    /*  72...  */
                        62,28, 6, 7,59,55,16,27,    /*  80...  */
                        20,60,25                    /*  88...  */
                       };

void near ToHan3(void)
{
     if (ShiftF) Temp=ToHanTableS3[Temp];
     else        Temp=ToHanTable3 [Temp];
}

void near GetHan3(void)
{
     static word Step=1;
     static word Cho=1,Jung1=2,Jung2=2,Jong1=1,Jong2=1;
     static word ChoBuf=1,JungBuf=2,JongBuf=1;
     word   i;
     word   KeyPadF;

     if (!HanF) {
        GetKey();
        HanResult=5;
        return;
     }
     if (HanResult!=1 && HanResult!=0) {
         Cho=Jong1=Jong2=1;
         Jung1=Jung2=2;
         if (ChoBuf<2 && JungBuf<3 && JongBuf<2) Step=1;
     }
     if (ChoBuf>1 || JungBuf>2 || JongBuf>1) {
         Cho      =ChoBuf;
         Jung1    =JungBuf;
         Jong1    =JongBuf;
         ChoBuf   =1;
         JungBuf  =2;
         JongBuf  =1;
         HanResult=0;
     } else {
         if (HanResult==0) HanResult=1;
         _AH=0x00;
         Inter(0x16);
         _DX=_AX;
         ScanCode.Full=_DX;
         Key=_DL;
         Way=_DH;
         if ( (Key=='1' && Way!=0x02) ||
              (Key=='2' && Way!=0x03) ||
              (Key=='3' && Way!=0x04) ||
              (Key=='4' && Way!=0x05) ||
              (Key=='5' && Way!=0x06) ||
              (Key=='6' && Way!=0x07) ||
              (Key=='7' && Way!=0x08) ||
              (Key=='8' && Way!=0x09) ||
              (Key=='9' && Way!=0x0A) ||
              (Key=='0' && Way!=0x0B) ||
              (Key=='/' && Way!=0x35)
            ) KeyPadF=True;
         else KeyPadF=False;
         ExtKey=(Key==0) ? ((Key=Way),(Way+=1000),True):((Way=Key),False);
         ShiftF=ShiftPressed();
         Temp=Way;
         if (Held(Way,'a','z'))  Temp-=32+32,Way-=32;
         else                    Temp-=32;
         if ((Way!=CtrlH && (Way<33 || Way>90)) || KeyPadF )
             if (Step==1) HanResult=5;
             else         HanResult=4;
         else if ( (ShiftF  && KeyClassS3[Temp]==6) ||
                   (!ShiftF && KeyClass3 [Temp]==6)
                 ) {
             if (ShiftF) Way=Key=ToHanTableS3[Temp];
             else                Way=Key=ToHanTable3[Temp];
             if (Step==1) HanResult=5;
             else         HanResult=4;
         } else if (KeyClass3[Temp]==0)
             if (Step==1) HanResult=5;
             else         HanResult=4;
         else
             switch  (Step) {
                case 1 :if (Way==CtrlH) {
                            HanResult=5;
                            break;
                        }
                        i=(ShiftF)?KeyClassS3[Temp]:KeyClass3[Temp];
                        switch (i) {
                           case 1 :ToHan3();
                                   Cho=Temp;
                                   Step=2;
                                   HanResult=0;
                                   break;
                           case 2 :
                           case 3 :HanResult=0;
                                   goto Mo3;
                           case 4 :HanResult=0;
                                   goto Jo5;
                        }
                        break;
                case 2 :if (Way==CtrlH) {
                            Cho=1;
                            Step=1;
                            break;
                        }
                        i=(ShiftF)?KeyClassS3[Temp]:KeyClass3[Temp];
                        switch (i) {
                           case 1 :ToHan3();
                                   if (Cho==Temp && DoubleCho()) {
                                       Cho++;
                                       Step=3;
                                   } else {
                                       ChoBuf=Temp;
                                       Step=2;
                                       HanResult=3;
                                   }
                                   break;
                           case 2 :
                           case 3 :goto Mo3;
                           case 4 :goto Jo5;
                        }
                        break;
                case 3 :if (Way==CtrlH) {
                            Cho=1;
                            Step=1;
                            break;
                        }
                        i=(ShiftF)?KeyClassS3[Temp]:KeyClass3[Temp];
                        Mo3:
                        switch (i) {
                           case 1 :ToHan3();
                                   ChoBuf=Temp;
                                   Step=2;
                                   HanResult=3;
                                   break;
                           case 2 :ToHan3();
                                   Jung1=Temp;
                                   Step=5;
                                   break;
                           case 3 :ToHan3();
                                   Jung1=Temp;
                                   Step=4;
                                   break;
                           case 4 :goto Jo5;
                        }
                        break;
                case 4 :if (Way==CtrlH) {
                            Jung1=2;
                            Step=3;
                            break;
                        }
                        i=(ShiftF)?KeyClassS3[Temp]:KeyClass3[Temp];
                        switch (i) {
                           case 1 :ToHan3();
                                   ChoBuf=Temp;
                                   Step=2;
                                   HanResult=3;
                                   break;
                           case 2 :ToHan3();
                                   if ( MakeJung(Jung1,Temp) !=Error) {
                                       Jung2=Temp;
                                       Step=5;
                                   } else {
                                       JungBuf=Temp;
                                       Step=5;
                                       HanResult=3;
                                   }
                                   break;
                           case 3 :ToHan3();
                                   JungBuf=Temp;
                                   Step=4;
                                   HanResult=3;
                                   break;
                           case 4 :goto Jo5;
                                   /*ToHan3();
                                   JongBuf=Temp;
                                   Step=7;
                                   HanResult=3;
                                   break;*/
                        }
                        break;
                case 5 :if (Way==CtrlH) {
                            if (Jung2==2) {  Jung1=2;   Step=3;  }
                            else          {  Jung2=2;   Step=4;  }
                            break;
                        }
                        i=(ShiftF)?KeyClassS3[Temp]:KeyClass3[Temp];
                        switch (i) {
                           case 1 :ToHan3();
                                   ChoBuf=Temp;
                                   Step=2;
                                   HanResult=3;
                                   break;
                           case 2 :ToHan3();
                                   JungBuf=Temp;
                                   Step=5;
                                   HanResult=3;
                                   break;
                           case 3 :ToHan3();
                                   JungBuf=Temp;
                                   Step=4;
                                   HanResult=3;
                                   break;
                           case 4 :
                               Jo5:ToHan3();
                                   Jong1=Temp;
                                   Step=6;
                        }
                        break;
                case 6 :if (Way==CtrlH) {
                            Jong1=1;
                            if (Jung1==2 && Cho==1)
                                Step=1;
                            else if (Jung1==2)
                                Step=3;
                            else
                                Step=5;
                            break;
                        }
                        i=(ShiftF)?KeyClassS3[Temp]:KeyClass3[Temp];
                        switch (i) {
                           case 1 :ToHan3();
                                   ChoBuf=Temp;
                                   Step=2;
                                   HanResult=3;
                                   break;
                           case 2 :ToHan3();
                                   JungBuf=Temp;
                                   Step=5;
                                   HanResult=3;
                                   break;
                           case 3 :ToHan3();
                                   JungBuf=Temp;
                                   Step=4;
                                   HanResult=3;
                                   break;
                           case 4 :ToHan3();
                                   if (MakeJong3(Jong1,Temp)!=Error) {
                                       Jong2=Temp;
                                       Step=7;
                                   } else {
                                       JongBuf=Temp;
                                       Step=6;
                                       HanResult=3;
                                   }
                        }
                        break;
                case 7 :if (Way==CtrlH) {
                            if (Jong2==1) { Jong1=1; Step=5; }
                            else          { Jong2=1; Step=6; }
                            break;
                        }
                        i=(ShiftF)?KeyClassS3[Temp]:KeyClass3[Temp];
                        switch (i) {
                           case 1 :ToHan3();
                                   ChoBuf=Temp;
                                   Step=2;
                                   HanResult=3;
                                   break;
                           case 2 :ToHan3();
                                   JungBuf=Temp;
                                   Step=5;
                                   HanResult=3;
                                   break;
                           case 3 :ToHan3();
                                   JungBuf=Temp;
                                   Step=4;
                                   HanResult=3;
                                   break;
                           case 4 :ToHan3();
                                   JongBuf=Temp;
                                   Step=6;
                                   HanResult=3;
                                   break;
                        }
            }
     }
     if (HanResult!=4 && HanResult!=5 &&
         Cho==1 && Jong1==1 && Jong2==1 && Jung1==2 && Jung2==2) {
         HanResult=6;
         Way=65535;
     } else HanKey=0x0080+(Cho<<2)+(MakeJung(Jung1,Jung2)>>3)+
                   (MakeJung(Jung1,Jung2)<<5)*0x100+
                   MakeJong3(Jong1,Jong2)*0x100;
}
void far GetHan(void)
{
     if (KeyboardType==2) GetHan2();
     else                 GetHan3();
}
