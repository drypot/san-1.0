/*                                                          */
/*                                                          */
/*        Han-Gul Graphic Window Interface version 1.0      */
/*                                                          */
/*        programed by Magic Book                           */
/*                                                          */
/*                                                          */
/*                                                          */

#ifndef __HWINDOW__

#define __HWINDOW__

#include "stddef.h"

#define UpC   129
#define DNC   205
#define RIC   168
#define LEC   166
#define UpRIC 130
#define UpLEC 128
#define DNRIC 206
#define DNLEC 204

#define Message   0
#define NoMessage 1

#define WindowStackSize 30000

struct  WHand {
          struct WHand far *LastWind;
          byte         far *Ptr;
          byte X1,Y1,X2,Y2;
          byte SelectWind;
        } extern far *NWP,far *AWP;

byte extern far *WindowStack;
word extern StackP;

void  HOpenWindow       (word X1,word Y1,word X2,word Y2);
void  HOpenWindowSimple (word X1,word Y1,word X2,word Y2);
void  HCloseWindow      (void);
void  HCloseWindowSimple(void);

word  HSayMes         (byte far *Data);
word  HSayIOErr       (void);
void  HSayBottom      (byte far *Data);
word  HSayStatus      (byte far *Data);
word  HGetMes         (byte far *Data,byte far *OutString,word Max);
word  HGetEng         (byte far *Data,byte far *OutString,word Max);
word  HGetEng         (byte far *Data,byte far *OutString,word Max);
word  HGetYesNo       (byte far *Data,word far *OutBoolean);

void  HWriteMid       (word Y,byte far *Data);
void  HDrawBox        (word X1,word Y1,word X2,word Y2);
void  HDrawBar        (word X1,word X2,word Y);
word  HWait           (word option);
void  Explain         (char *Data);

void  Bell            (void);

word  InitWindow      (void);

#endif
