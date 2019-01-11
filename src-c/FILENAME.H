/*                                                            */
/*                                                            */
/*        Han-Gul Graphic File Name Manager version 1.0       */
/*                                                            */
/*        programed by Magic Book                             */
/*                                                            */
/*                                                            */
/*                                                            */

#ifndef __FILENAME__

#define __FILENAME__

#include <dir.h>

struct filestruct {
         char      Name[13];
         word      DirectoryF;
         word      Size;

       };
#define MaxExts 30

struct filestruct extern SelectFile,DirList[512];
word extern DirC,AuxDirC;
char extern StdExt[80];
char extern Exts[MaxExts][MAXEXT];

word HGetFileName (char *FileName);
word FExpand(byte *Dest);
void PrimaryExt(char *Ext);
void FNSplit(char *Path,char *Drv,char *Direc,char *Name,char *Ext);
word ExistFileWithExts(byte *Name);
void SetExts(char *Data);

#endif
