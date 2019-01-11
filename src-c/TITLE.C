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
       HWriteMid(i+2," ¬e ");
       HWriteMid(i+4," Ðe‹i ¢…¬á Íe»³‹¡ ");
       HWriteMid(i+6,SanVersionNum " Ìe");
       HWriteMid(i+8,"¸á¸bŠ¥ (¸á) 1991-93  ¤b‹AÑe");
       HWriteMid(i+9,"ˆåŠ‚”ÐbŠa ¸å¸a‰¬e 93 Ðb¤å");
    } else {
       RestoreScreen(j+1,i+1,j+35,i+8,DiskBuf);
       HWriteMid(i+8,SanVersionNum " Ìe");
       HWriteMid(i+9,"¸á¸bŠ¥ (¸á) 1991-93  ¤b‹AÑe");
       HWriteMid(i+10,"ˆåŠ‚”ÐbŠa ¸å¸a‰¬e 93 Ðb¤å");
    }

    HOpenWindow(VideoRight/2-23,i+14,VideoRight/2+23,i+16);
    HWriteMid(i+15,"·¡ Ïa¡‹aœ‘·e ®Á´á¶Á´á·³“¡”a.");

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
       HWriteMid(i+1+2," ¬e ");
       HWriteMid(i+1+4," Ðe‹i ¢…¬á Íe»³‹¡ ");
       HWriteMid(i+1+6,"­A ¸a·A¡¡ " SanVersionNum " Ìe");
       HWriteMid(i+1+8,"¸á¸bŠ¥ (¸á) 1991-93  ¤b‹AÑe");
       HWriteMid(i+1+9,"ˆåŠ‚”ÐbŠa ¸å¸a‰¬e 93 Ðb¤å");
    } else {
       RestoreScreen(j+1,i+1,j+35,i+8,DiskBuf);
       HWriteMid(i+1+8 ,"­A ¸a·A¡¡ " SanVersionNum " Ìe");
       HWriteMid(i+1+10,"¸á¸bŠ¥ (¸á) 1991-93  ¤b‹AÑe");
       HWriteMid(i+1+11,"ˆåŠ‚”ÐbŠa ¸å¸a‰¬e 93 Ðb¤å");
    }

    HNormal();
    HWait(NoMessage);
    HCloseWindow();

#endif

}
