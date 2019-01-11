/*                                                                 */
/*        Memory Management Library  version 1.0 Beta              */
/*                                                                 */
/*        date: 1990,11,4                                          */
/*                                                                 */
/*        programed By Magic Book                                  */
/*                                                                 */

#ifndef __MEMORY__

#define __MEMORY__

#include "stddef.h"

#define  Pointer(segment,offset) ((void far *)(((unsigned long)(segment)<<16) | (offset)))
#define  Seg(pointer) ((word)((unsigned long)(pointer)>>16))
#define  Off(pointer) ((word)(pointer))
#define  MemB(segment,offset) (*((byte far *)(Pointer((segment),(offset)))))
#define  MemW(segment,offset) (*((word far *)(Pointer((segment),(offset)))))
#define  Push(WordData) (Stack[TopStack++]=(WordData))
#define  Pop(WordData)  ((WordData)=Stack[--TopStack])
#define  Held(Var,Start,End) ((Var)>=(Start) && (Var)<=(End))

word extern Stack[50];
word extern TopStack;

void       far MemCopy  (void far *Source,void far *Target,word Size);
void       far MemOR    (void far *Source,void far *Target,word Size);
void       far MemSetB  (void far *Target,byte Data,word Size);
void       far MemSetW  (void far *Target,word Data,word Size);
int        far MemCmpB  (void far *Source1,void far *Source2,word Size);
int        far MemCmpW  (void far *Source1,void far *Source2,word Size);
void far * far MemSearch(void far *Source1,byte Data,word Size);

/*void           __cli__     (void);
void           __sti__     (void);
unsigned char  __inportb__ (int portid);
void           __outportb__(int portid, unsigned char value);
void           __int__     (int interruptnum);
*/
#include <dos.h>

#define Disable()         __cli__()
#define Enable()          __sti__()
#define Inter(i)          __int__(i)
#define InPortB(PortID)   __inportb__(PortID)
#define OutPortB(PortID,V)__outportb__(PortID,V)

word       far InPortW    (word PortID);
void       far OutPortW   (word PortID,word Value);

#endif