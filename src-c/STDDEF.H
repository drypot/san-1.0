/*                                              */
/*        Standard Definations 1.0 Beta         */
/*                                              */
/*        date: 1990,11,4                       */
/*                                              */
/*        programed By Magic Book               */
/*                                              */
/*  Header                                      */

#ifndef __STDDEF__

#define __STDDEF__

#define BS   8
#define LF   10
#define CR   13
#define ESC  27

#define PC   255
#define XT   254
#define PCJR 253
#define AT   252

#define Mono      0
#define Color     1

#define False     0
#define True      1

#define No        0
#define Yes       1

#define Error     0
#define OK        1

#define ON        1
#define OFF       0

#define Null      0L

typedef  unsigned char byte;
typedef  unsigned int  word;
typedef  unsigned long dword;

#endif