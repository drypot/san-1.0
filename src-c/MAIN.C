
/*                                                        */
/*                                                        */
/*              Trance Word version 0.1 Beta               */
/*                                                        */
/*            (C) Copyright 1991 Magic Book               */
/*                                                        */
/*                   Start Up Routine                     */
/*                                                        */

#include "stddef.h"
#include "crt.h"
#include "handrv.h"
#include "hwindow.h"
#include "memory.h"
#include "filename.h"
#include <alloc.h>
#include <dir.h>
#include <dos.h>
#include "const.h"
#include "edit.h"
#include "sanver.h"
#include "popup.h"
#include <io.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include "hanja.h"


extern unsigned _stklen=8096;

void interrupt (*CriticalBuf)(),
     interrupt (*CtrlBrkBuf)();

void interrupt CtrlBrkHandler(void)
{

}

void interrupt CriticalHandler(word bp,word di,word si,
                              word ds,word es,
                              word dx,word cx,word bx,word ax,
                              word ip,word cs,word flags )
{
    char *i;

    switch (di&0x00FF) {
       case   0 : i="³a‹¡ ¤w»¡ ÉS·¡ ¦›´á ·¶·s“¡”a."; break;
       case   1 : i="³i® ·¶“e ”e¶áÁAˆa ´a“³“¡”a.";        break;
       case   2 : i="—aœa·¡§a¢…·¡ µia ·¶·s“¡”a.";     break;
       case   4 : i="•A·¡Èá ÁAÇa µAœáˆa ¤i¬—Ð–·s“¡”a.";           break;
       case   9 : i="ÏaŸ¥Èá ¹··¡ˆa ´ô¯s“¡”a.";break;
       case 0xA : i="³a‹¡ˆa ¦‰ˆa“wÐs“¡”a.";         break;
       case 0xB : i="·ª‹¡ˆa ¦‰ˆa“wÐs“¡”a.";          break;
       default  : i="•¡¯a ÀáŸ¡µA¬á µAœáˆa ¤i¬—Ð–·s“¡”a.";      break;
    }
    Push(InverseF);
    HInverse();
    HOpenWindow(3,15,58,18);
    DrawString(4,16,i);
    DrawString(4,17,"”a¯¡ ¯¡•¡ ÐiŒa¶a? ´a“± ‹a e –‰Œa¶a? [ Retry / Fail ]");
    while (True) {
      GetKey();
      if (Way=='R' || Way=='r') {
          ax=(ax&0xFF00)+1;
          break;
      }
      if (Way=='F' || Way=='f') {
          ax=(ax&0xFF00)+3;
          break;
      }
    }
    HCloseWindow();
    Pop(InverseF);
}

word SetHelpData(void);

void main(int argc,char *argv[])
{
     char _Dir[MAXDIR],_Name[MAXFILE];
     byte buf[81];
     extern int HanjaFileHandler;

     fnsplit(argv[0],(char*)SysPath,_Dir,_Name,_Name);
     strcat(SysPath,_Dir);
     OutPortB(0x3BF,2);
     FExpand(SysPath);
     if (RetrieveOptions()!=OK) {
         clrscr();
         puts(SanVersion);
         puts("\nOccurred error in CONFIGURATION.");
         puts("Please, run 'SANSETUP' for this version first.");
         exit(1);
     }
     if (SetHelpData()!=OK) {
         clrscr();
         puts(SanVersion);
         puts("\nOccurred error in reading HELP FILE.");
         exit(1);
     }
     strcpy(WorkFileName,SysPath);
     strcat(WorkFileName,"sanvideo.drv");
     if (OpenHanDrv(WorkFileName)!=OK) {
         clrscr();
         puts(SanVersion);
         puts("\nError in reading CRT Driver.");
         puts("Please, run 'SANSETUP' for this version first.");
         exit(1);
     }
     GrMode();
     ScnRight=VideoRight;
     ScnLeft =1;
     ScnBot  =VideoBot-2;
     ScnTop  =3 ;
     ScnHigh =ScnBot-ScnTop+1;
     ScnWidth=ScnRight-ScnLeft+1;
     HClrScr();
     if (InitWindow()!=OK ||
         InitPopUp()!=OK  ||
         InitHanja()!=OK  ||
         (UnKillBuf    =(byte far * far *)farmalloc(MaxUnKillBuf*4))==NULL ||
         (Task[0].Paper=(byte far * far *)farmalloc((Task[0].MaxPaper+1)*4))==NULL ||
         (Task[1].Paper=(byte far * far *)farmalloc((Task[1].MaxPaper+1)*4))==NULL ||
         (Task[2].Paper=(byte far * far *)farmalloc((Task[2].MaxPaper+1)*4))==NULL ||
         (DiskBuf=(byte far *)farmalloc(DiskBufSize))==NULL ||
         (PickList=(struct PickListEle (*)[])farmalloc(sizeof(*PickList)))==NULL ||
         farcoreleft()<5*1024) {
         TextMode();
         clrscr();
         puts(SanVersion);
         puts("\nNot enough work space.");
         exit(1);
     }
     OpenPickList();
     strcpy(WorkFileName,SysPath);
     strcat(WorkFileName,"san.lin");
     if (SetExtEng(WorkFileName)==Error) {
         TextMode();
         clrscr();
         puts(SanVersion);
         puts("\nOccurred error in reading ");
         puts("LINE FONT." );
         exit(1);
     }
     strcpy(WorkFileName,SysPath);
     strcat(WorkFileName,HanFontName);
     if (SetHan(WorkFileName)!=OK) {
         TextMode();
         clrscr();
         puts(SanVersion);
         puts("\nOccurred error in reading ");
         puts("HAN-GUL FONT.");
         exit(1);
     }
     strcpy(WorkFileName,SysPath);
     strcat(WorkFileName,EngFontName);
     if (SetEng(WorkFileName)==Error) {
         TextMode();
         clrscr();
         puts(SanVersion);
         puts("\nOccurred error in reading ");
         puts("ENGLISH FONT.");
         exit(1);
     }
     strcpy(WorkFileName,SysPath);
     strcat(WorkFileName,GrfFontName);
     if (SetGrf(WorkFileName)==Error) {
         TextMode();
         clrscr();
         puts(SanVersion);
         puts("\nOccurred error in reading ");
         puts("GRAPHIC CHARACTER.");
         exit(1);
     }
     strcpy(WorkFileName,SysPath);
     strcat(WorkFileName,"SAN.HAN");
     HanjaFileHandler=_open(WorkFileName,0);
     if (HanjaFileHandler==-1) HanjaOff();
     ChangeFontF=False;
     InitCrt();

     CtrlBrkBuf =getvect(0x1B);
     CriticalBuf=getvect(0x24);
     setvect(0x1B,CtrlBrkHandler);
     setvect(0x24,CriticalHandler);
     if ( argc>1 && strlen(argv[1])<sizeof(WorkFileName)) {
         strcpy (WorkFileName,argv[1]);
         FileNameSetF=True;
     } else {
     WorkFileName[0]=0;
         FileNameSetF=False;
     }
     HNormal();
     MaxBufSize=farcoreleft();

/*     {
        unsigned long count,i,j=0;
        word k,l;

       i=*(unsigned long *)Pointer(0x40,0x6C);
        for (count=0; j<2 && j>=0 ; count++) {
            j=*(unsigned long*)Pointer(0x40,0x6C)-i;
        }


       DrawString(4,2,"¬e·e ®Á´á¶Á´á Ïa¡‹aœ‘·³“¡”a.");
       DrawString(4,3,"®Á´á¶Á´áœe ·©”e ¬a¶w¸aˆa »¢¸ó ¬a¶wÐi Ïa¡‹aœ‘·i ”Ða‰¡ Š·³·");
       DrawString(4,4,"Ó w·¡ ·¶·i˜ §¡¡­¡ ˆa‰b·i »¡¦‰Ða“e ˆõ·³“¡”a.");
       DrawString(4,5,"·©”e ·¡ ‰·ˆÌe·i ¬a¶wÐ ¥¡¯¡‰¡ Š·³ Ó w·¡ ·¶·a¯©  ˜“e  ÅAÉI");
       DrawString(4,6,"´a·¡—¡  'magicbk'  ·a¡ ¡A·© º¯³®a.");
       DrawString(4,7,"Š—‹qÐa¯¥ ¸ñ·¡ ·¶·a¯¡¡e »©¢…Ða­a•¡ ¹½¯s“¡”a.");
       DrawString(4,8,"¸a­AÐe ¬aÐw·i ´ia—aŸ¡‰V¯s“¡”a.");

       DrawString(4,10,"............................................................");


       for (k=0 ; k<60 ; k++) {
         for (l=0 ; l<9 ; l++)
            for (i=0 ; i<count ; i++)
               j=*(unsigned long*)Pointer(0x40,0x6C)-i;
         DrawEng(4+k,10,' ');
         if (KeyPressed())
       }
       NextChar=FirstAvailable;
     }
  */


     Editor();

     /* Watch Out 'BufErr' */

     if (HanjaFileHandler!=-1) _close(HanjaFileHandler);
     ClosePickList();
     TextMode();
     clrscr();
     puts(SanVersion);
     setvect(0x1B,CtrlBrkBuf);
}
