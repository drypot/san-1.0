/*                                                      */
/*                                                      */
/*        Han-Gul Graphic Pop Up Menu version 1.0       */
/*                                                      */
/*        programed by Magic Book                       */
/*                                                      */
/*                                                      */
/*                                                      */

#include "hwindow.h"
#include "handrv.h"
#include "crt.h"
#include "memory.h"
#include "popup.h"
#include <alloc.h>

struct ItemGroupList far *NMP=Null,far *AMP;
word   Item;
word   FullArrowF;
byte   far *PopUpStack;
word   PopUpStackP;
char   **ExplainList;

word far HOpenMenu(void)
{
     AMP=NMP;
     NMP=(struct ItemGroupList *)(PopUpStack+PopUpStackP);
     PopUpStackP+=sizeof(*NMP);
     NMP->Length=0;
     NMP->DrawedF=False;
     NMP->NowPos=0;
     NMP->Last=AMP;
     return OK;
}

word far HCloseMenu(void)
{
     if (NMP==Null) return Error;
     AMP=NMP;
     NMP=NMP->Last;
     PopUpStackP-=sizeof(*NMP);
     return OK;
}

word far HSetItem (word X,word Y,byte far *Data,word Length,byte HeadChar)
{
     if (NMP==Null) return Error;
     DrawString (X,Y,Data);
     NMP->List[NMP->Length].X=X;
     NMP->List[NMP->Length].Y=Y;
     NMP->List[NMP->Length].Length=Length-1;
     NMP->List[NMP->Length].HeadChar=HeadChar;
     NMP->Length++;
     return OK;
}

word far HSelect (void)
{
     word i,ExitF;

     if (NMP==Null) return Error;
     if (!NMP->DrawedF) {
         HSetInverse(NMP->List[NMP->NowPos].X,NMP->List[NMP->NowPos].Y,
                    NMP->List[NMP->NowPos].X+NMP->List[NMP->NowPos].Length,
                    NMP->List[NMP->NowPos].Y);
         NMP->DrawedF=True;
     }
     do {
        Explain(ExplainList[NMP->NowPos]);
        GetKey();
        SetHotKeyF();
        if (HotKeyF) {
            Item=NMP->NowPos;
            break;
        }
        if (NMP->DrawedF) {
            HSetInverse(NMP->List[NMP->NowPos].X,NMP->List[NMP->NowPos].Y,
                        NMP->List[NMP->NowPos].X+NMP->List[NMP->NowPos].Length,
                        NMP->List[NMP->NowPos].Y);
            NMP->DrawedF=!NMP->DrawedF;
        }
        ExitF=OK;
        switch(Way) {
          case Up     :
          case CtrlE  :NMP->NowPos=(NMP->NowPos==0) ? NMP->Length-1:NMP->NowPos-1;
                       ExitF=False;
                       break;
          case ' '    :
          case CtrlX  :
          case Down   :NMP->NowPos=(NMP->NowPos==NMP->Length-1) ?0:NMP->NowPos+1;
                       ExitF=False;
                       break;
          case PgDn   :
          case CtrlC  :NMP->NowPos+=8;
                       if (NMP->NowPos>NMP->Length-1) NMP->NowPos=NMP->Length-1;
                       ExitF=False;
                       break;
          case PgUp   :
          case CtrlR  :NMP->NowPos=(NMP->NowPos<8) ? 0 : NMP->NowPos-8;
                       ExitF=False;
                       break;
          case Home   :NMP->NowPos=0;
                       ExitF=False;
                       break;
          case End    :NMP->NowPos=NMP->Length-1;
                       ExitF=False;
                       break;
          case ESC    :Item=NMP->NowPos; break;
          case Return :Item=NMP->NowPos;
                       break;
          default     :if (Held(Way,'a','z')) Way-='a'-'A';
                       if (Way==CtrlS || Way==CtrlD) break;
                       if (Held(Way,1,26)) Way+=64;
                       if (Held(Way,'A','Z')) {
                          ExitF=False;
                          if (Way==NMP->List[NMP->NowPos].HeadChar) {
                              Item=NMP->NowPos;
                              ExitF=True;
                              Way=Return;
                              break;
                          }
                          i=NMP->NowPos+1;
                          while (i != NMP->NowPos ) {
                            if (i>NMP->Length)
                                if (NMP->NowPos==0) break;
                                else                i=0;
                            if (Way==NMP->List[i].HeadChar) {
                                Item=NMP->NowPos=i;
                                ExitF=True;
                                Way=Return;
                                break;
                            }
                            i++;
                          }
                       }
        }
        if (FullArrowF)
          switch (Way) {
            case Left   :
            case CtrlS  :NMP->NowPos=(NMP->NowPos==0) ? NMP->Length-1:NMP->NowPos-1;
                         ExitF=False;
                         break;
            case Right  :
            case CtrlD  :NMP->NowPos=(NMP->NowPos==NMP->Length-1)? 0:NMP->NowPos+1;
                         ExitF=False;
                         break;
            case CtrlF  :NMP->NowPos+=8;
                         if (NMP->NowPos>NMP->Length-1) NMP->NowPos=NMP->Length-1;
                         ExitF=False;
                         break;
            case CtrlA  :NMP->NowPos=(NMP->NowPos<8) ? 0 : NMP->NowPos-8;
                         ExitF=False;
          }
        HSetInverse(NMP->List[NMP->NowPos].X,NMP->List[NMP->NowPos].Y,
                    NMP->List[NMP->NowPos].X+NMP->List[NMP->NowPos].Length,
                    NMP->List[NMP->NowPos].Y);
        NMP->DrawedF=!NMP->DrawedF;

     } while (!ExitF);
     return (Way==Return)?OK:Error;
}

void HManualSelect(word NewItem)
{
        if (NMP->DrawedF && NMP->NowPos!=NewItem) {
            HSetInverse(NMP->List[NMP->NowPos].X,NMP->List[NMP->NowPos].Y,
                        NMP->List[NMP->NowPos].X+NMP->List[NMP->NowPos].Length,
                        NMP->List[NMP->NowPos].Y);
        }
        NMP->NowPos=NewItem;
        HSetInverse(NMP->List[NMP->NowPos].X,NMP->List[NMP->NowPos].Y,
                    NMP->List[NMP->NowPos].X+NMP->List[NMP->NowPos].Length,
                    NMP->List[NMP->NowPos].Y);
        NMP->DrawedF=True;
}

int  InitPopUp(void)
{
     PopUpStack=(byte *)farmalloc(PopUpStackSize);
     return (PopUpStack==NULL)?Error:OK;
}
