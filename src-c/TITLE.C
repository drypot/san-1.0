#include "stddef.h"
#include <io.h>
#include "handrv.h"
#include "hwindow.h"
#include <string.h>
#include "const.h"
#include "sanver.h"
#include "crt.h"

/*#define FREE*/

void DisplayTitle(void)
{
    word i,j,k;
    char Buf[30];
    int  handler;
    byte Name[80];

#ifdef FREE

    HInverse();
    i=VideoBot/2-7;
    j=VideoRight/2-18;
    HOpenWindow (j,i,j+36,i+12);
    strcpy(Name,SysPath);
    strcat(Name,"san.til");

    if ((handler=_open(Name,0))==-1) k=Error;
    else if ( _read(handler,DiskBuf,35*8*16)==-1) k=Error;
    else k=OK;

    if (k==Error) {
       HWriteMid(i+2," �e ");
       HWriteMid(i+4," �e�i ���� �e���� ");
       HWriteMid(i+6,SanVersionNum " �e");
       HWriteMid(i+8,"��b�� (��) 1991-93  �b�A�e");
       HWriteMid(i+9,"�劂���b�a ��a���e 93 �b��");
    } else {
       RestoreScreen(j+1,i+1,j+35,i+8,DiskBuf);
       HWriteMid(i+8,SanVersionNum " �e");
       HWriteMid(i+9,"��b�� (��) 1991-93  �b�A�e");
       HWriteMid(i+10,"�劂���b�a ��a���e 93 �b��");
    }

    HOpenWindow(VideoRight/2-23,i+14,VideoRight/2+23,i+16);
    HWriteMid(i+15,"�� �a���a���e ������ᷳ���a.");

    HNormal();
    HWait(NoMessage);
    HotKeyEndF=True;

    HCloseWindow();
    HCloseWindow();

#else

    HInverse();
    i=VideoBot/2-7;
    j=VideoRight/2-18;
    HOpenWindow (j,i,j+36,i+13);
    strcpy(Name,SysPath);
    strcat(Name,"san.til");

    if ((handler=_open(Name,0))==-1) k=Error;
    else if ( _read(handler,DiskBuf,35*8*16)==-1) k=Error;
    else k=OK;

    if (k==Error) {
       HWriteMid(i+1+2," �e ");
       HWriteMid(i+1+4," �e�i ���� �e���� ");
       HWriteMid(i+1+6,"�A �a�A�� " SanVersionNum " �e");
       HWriteMid(i+1+8,"��b�� (��) 1991-93  �b�A�e");
       HWriteMid(i+1+9,"�劂���b�a ��a���e 93 �b��");
    } else {
       RestoreScreen(j+1,i+1,j+35,i+8,DiskBuf);
       HWriteMid(i+1+8 ,"�A �a�A�� " SanVersionNum " �e");
       HWriteMid(i+1+10,"��b�� (��) 1991-93  �b�A�e");
       HWriteMid(i+1+11,"�劂���b�a ��a���e 93 �b��");
    }

    HNormal();
    HWait(NoMessage);
    HCloseWindow();

#endif

}
