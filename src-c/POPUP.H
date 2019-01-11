/*                                                      */
/*                                                      */
/*        Han-Gul Graphic Pop Up Menu version 1.0       */
/*                                                      */
/*        programed by Magic Book                       */
/*                                                      */
/*                                                      */
/*                                                      */

#ifndef __POPUP__

#define __POPUP__

#define PopUpStackSize 500

struct ItemGroupList  {

       struct ItemListForm {
              word X,Y;
              word Length;
              byte HeadChar;
              } List[25];
       word   Length;
       word   DrawedF;
       word   NowPos;
       struct ItemGroupList far *Last;
       } extern far *NMP,far *AMP;

word extern Item;
word extern FullArrowF;
word extern PopUpStackP;
byte extern far *PopUpStack;
char extern **ExplainList;

word far HOpenMenu(void);
word far HCloseMenu(void);
word far HSetItem (word X,word Y,byte far *Data,word Length,byte HeadChar);
word far HSelect (void);
void HManualSelect(word NewItem);

int  far InitPopUp(void);

#endif
