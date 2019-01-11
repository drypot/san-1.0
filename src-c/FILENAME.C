/*                                                            */
/*                                                            */
/*        Han-Gul Graphic File Name Manager version 1.0       */
/*                                                            */
/*        programed by Magic Book                             */
/*                                                            */
/*                                                            */
/*                                                            */

#include "hwindow.h"
#include "handrv.h"
#include "crt.h"
#include "memory.h"
#include "filename.h"
#include <io.h>
#include <alloc.h>
#include <dir.h>
#include <dos.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "file.h"


struct filestruct SelectFile,DirList[512];
word   DirC,AuxDirC;
char   StdExt[80]="";
char   Path[MAXPATH],MainDrive[MAXDRIVE],MainDirec[MAXDIR];
char   static Name[MAXFILE];
char   Exts[MaxExts][MAXEXT];
word   ExtNum;
word   QuickF;

byte static *AppendSpace(byte *Data,word Len)
{
     word j;

     j=strlen(Data);
     while (j<Len) Data[j]=' ',j++;
     Data[j]=0;
     return Data;
}

void   PrimaryExt(char *Ext)
{
   word i;

   i=0;
   while ( i<strlen(StdExt) && StdExt[i]!=';') {
     Ext[i]=StdExt[i];
     i++;
   }
   Ext[i]=0;
}

word ExistFileWithExts(byte *Name)
{
     word i;
     byte iPath[80];

     SetExts(StdExt);
     for (i=0; i<ExtNum ; i++) {
       strcpy(iPath,Name);
       strcat(iPath,".");
       strcat(iPath,Exts[i]);
       if (access(iPath,0)==0) return OK;
     }
     return Error;
}

void FNSplit(char *Path,char *Drv,char *Direc,char *Name,char *Ext)
{
   char WorkBuf[MAXPATH];
   char WorkBuf2[MAXPATH];
   word i,j,S,E;

   i=j=0;
   MemSetB(WorkBuf,0,MAXPATH);
   MemSetB(WorkBuf2,0,MAXPATH);
   while (Path[i]!=0) {
     if (Path[i]!=' ') WorkBuf[j++]=Path[i];
     i++;
   }
   WorkBuf[j]=0;
   if (WorkBuf[1]==':') {
      Drv[0]=WorkBuf[0];
      Drv[1]=WorkBuf[1];
      Drv[2]=0;
      S=2;
   } else  {
      Drv[0]=0;
      S=0;
   }
   E=strlen(WorkBuf);
   while (E>S && WorkBuf[E-1]!='\\') E--;
   strncpy(Direc,WorkBuf+S,E-S);
   Direc[E-S]=0;
   strcpy(WorkBuf2,WorkBuf+E);
   if (strcmp(WorkBuf2,".")==0) {
      Name[0]=0;
      Ext[0]=0;
   } else if (strcmp(WorkBuf2,"..")==0) {
      strcat(Direc,WorkBuf2);
      strcat(Direc,"\\");
      Name[0]=0;
      Ext[0]=0;
   } else {
      E=strlen(WorkBuf2);
      i=0;
      while ( i<E && WorkBuf2[i]!='.') {
        Name[i]=WorkBuf2[i];
        i++;
      }
      Name[i]=0;
      j=0;
      while ( i<E) {
        Ext[j]=WorkBuf2[i];
        j++;
        i++;
      }
      Ext[j]=0;
   }
}

word FExpand(byte *Dest)
{
     byte Src[81];
     word s,d;
     word i;

     strcpy (Src,Dest);
     s=d=0;
     if (strlen(Src)<2 || Src[s+1]!=':') {
         Dest[d++]=getdisk()+'A';
     } if (Src[s+1]==':') {
         Dest[d++]=Src[s];
         if (Held(Src[s],'a','z')) Dest[s]-=0x20;
         Inter(0x11);
         i=_AX;
         i&=0x00C0;
         if (i==0 && Dest[s]=='B') Dest[s]='A';
         s+=2;
     }
     Dest[d++]=':';
     if (Src[s]!='\\') {
         Dest[d++]='\\';
         if (getcurdir(Dest[0]-'A'+1,Dest+d)==0) {
             if (Dest[d]!=0) {
                 d=strlen(Dest);
                 Dest[d++]='\\';
             }
         } else return Error;
     }
     Dest[d]=0;
     strcat(Dest,Src+s);
     d=s=0;
     do {
        if (Dest[s]=='\\') {
           if (Dest[d-1]=='.' && Dest[d-2]=='.') {
               d-=3;
               if (Dest[d-1]!=':')
                       do d--; while (Dest[d]!='\\');
           } else if (Dest[d-1]=='.')
               d-=2;
        } else if (Held(Dest[s],'a','z')) Dest[s]-=0x20;
        Dest[d]=Dest[s];
        d++;
        s++;
     } while (Dest[d]!=0);
     if (Dest[d-1]==':')
         Dest[d++]='\\';
     else if (Dest[d-1]=='.' && Dest[d-2]=='.') {
         d-=3;
         if (Dest[d-1]!=':')
            while (Dest[d-1]!='\\') d--;
     } else if (Dest[d-1]=='.' && Dest[d-2]=='\\')
               d-=1;
     Dest[d]=0;
     return OK;
}

void near ClearDirList(void)
{
    DirC=0;
    AuxDirC=0;
}

word near ReadNames(char *Drive,char *Direc,char *Name,char *Ext)
{
    int j,i,k;
    char Path[MAXPATH];
    struct ffblk iDir;

    fnmerge (Path,Drive,Direc,Name,Ext);
    if (findfirst(Path,&iDir,0)==0) {
      do {
        for (j=0 ; j<DirC ; j++)
          if (0==strcmp(DirList[j].Name,iDir.ff_name)) goto SkipMarking;
        strcpy (DirList[DirC].Name,iDir.ff_name);
        DirList[DirC].DirectoryF=0;
        DirList[DirC].Size=iDir.ff_fsize/1024+1;
        for (j=0,k=False ; j<strlen(DirList[DirC].Name) ; j++)
          if (DirList[DirC].Name[j]=='.') {
             k=True;
             break;
          }
        if (!k) strcat(DirList[DirC].Name,".");
        DirC++;
        SkipMarking:;
      } while (findnext(&iDir)==0 && DirC<512);
      if (errno!=ENOFILE) return Error;
    }
    if (DirC==511) HSayMes("Too big directory");
    return OK;
}

word near ReadDirs(char *Drive,char *Direc)
{
    char Path[MAXPATH];
    struct ffblk iDir;

    fnmerge (Path,Drive,Direc,"*",".*");
    if (findfirst(Path,&iDir,FA_DIREC)==0 && DirC<512) {
      do {
        if ( (iDir.ff_attrib & FA_DIREC) == 0) continue;
        if ( strlen(iDir.ff_name)==1 && iDir.ff_name[0]=='.' ) continue;
        AuxDirC++;
        strcpy (DirList[DirC].Name,iDir.ff_name);
        DirList[DirC].DirectoryF=1;
        DirList[DirC].Size=0;
        DirC++;
      } while (findnext(&iDir)==0 && DirC<512);
      if (errno!=ENOFILE) return Error;
    }
    if (DirC==511) HSayMes("Too big directory");
    return OK;
}

void near SortDir(void)
{
    if (DirC)    qsort (DirList,DirC-AuxDirC,sizeof (DirList[0]),strcmp);
    if (AuxDirC) qsort (DirList+(DirC-AuxDirC),AuxDirC,sizeof(DirList[0]),strcmp);
}

word GF_HX,GF_X;
word BoxOpenedF;

void GF_SetBox(void)
{
    word i,j;
    byte Buf[25];
    byte Buf2[25];

   for (i=0 ; i<VideoBot-3 ; i++) {
      strcpy(Buf," ");
      if (GF_HX+i<DirC) {
         strcat(Buf,DirList[GF_HX+i].Name);
         if (DirList[GF_HX+i].DirectoryF) {
            strcat(Buf,"\\");
            AppendSpace(Buf,22);
         } else {
            AppendSpace(Buf,15);
            sprintf(Buf2,"%5dK ",DirList[GF_HX+i].Size);
            if (strlen(Buf2)>7) strcat(Buf,"?????? ");
            else                strcat(Buf,Buf2);
         }
      } else AppendSpace(Buf,22);
      for (j=0 ; j<22 ; j++)
        DrawEng(VideoRight-24+j,i+2,Buf[j]);
  }
}

void GF_SetPos (word NewX)
{
    word i;

    if (NewX==GF_HX+GF_X) return;
    if (NewX>DirC+100)NewX=0;
    if (NewX>=DirC)   NewX=DirC-1;
    if (NewX<GF_HX)
         if (NewX<GF_X) GF_HX=0,GF_X=NewX;
         else           GF_X=0,GF_HX=NewX-GF_X;
    else if (NewX<=GF_HX+VideoBot-4) {
            GF_X=NewX-GF_HX;
            return;
         }
    else GF_X=VideoBot-4,GF_HX=NewX-GF_X;
    GF_SetBox();
}

word ExistExt(char *Ext)
{
   word i;

   for (i=0 ; i<ExtNum; i++)
     if (stricmp(Ext,Exts[i])==0) return OK;
   return False;
}

word GF_SelectFile(byte far *Name)
{
    char Drive[MAXDRIVE],Direc[MAXDIR];
    char NameBuf[MAXFILE];
    char ExtBuf[MAXPATH];
    word i,j,k;

    if (DirC==0) return Error;
    if (QuickF==OK)
       for (i=0 ; i<=DirC ; i++) {
           if (DirList[i].DirectoryF) {
              if (stricmp(DirList[i].Name,Name)==0) {
                 SelectFile=DirList[i];
                 return OK;
              }
           } else {
              if (DirC && DirC-AuxDirC==1) {
                 FNSplit (DirList[i].Name,Drive,Direc,NameBuf,ExtBuf);
                 if (stricmp(NameBuf,Name)==0 && ExistExt(ExtBuf+1)) {
                    SelectFile=DirList[i];
                    return OK;
                 }

              }
           }
       }
    Push(InverseF);
    HInverse();
    if (!BoxOpenedF) {
      HNormal();
      HClrBox(0,VideoBot,VideoRight,VideoBot);
      HWriteMid(VideoBot," [RETURN] ¬åÈ‚       [DEL/CtrlY] ÑÁ·© »¡¶‘      [ESC] Âá­¡");
      HInverse();
      HOpenWindow (VideoRight-25,1,VideoRight-2,VideoBot-1);
      BoxOpenedF=True;
      HNormal();
      HWriteMid(VideoBot," [RETURN] ¬åÈ‚       [DEL/CtrlY] ÑÁ·© »¡¶‘      [ESC] Âá­¡");
      HInverse();
    }
    GF_HX=0;
    GF_X=0;
    j=False;
    if (FileNameSetF) {
       FNSplit (WorkFileName,Drive,Direc,NameBuf,ExtBuf);
       strcat (NameBuf,ExtBuf);
       for (i=0 ; i<DirC ; i++ ) {
         if (stricmp(NameBuf,DirList[i].Name)==0) {
             GF_SetPos(i+1);
             j=True;
             GF_SetBox();
             break;
         }
       }

    }
    if (!j) {
       GF_SetPos (0);
       GF_SetBox();
    }
    do  {
        j=GF_HX+GF_X;
        HSetInverse(VideoRight-24,GF_X+2,VideoRight-3,GF_X+2);
        GetKey();
        SetHotKeyF();
        HSetInverse(VideoRight-24,GF_X+2,VideoRight-3,GF_X+2);
        switch (Way) {
          case Home   :GF_SetPos(0);
                       break;
          case End    :GF_SetPos(DirC-1);
                       break;
          case Up     :
          case CtrlE  :(j==0   ) ? Bell() : GF_SetPos (j-1);
                       break;
          case ' '    :
          case Down   :
          case CtrlX  :(j==DirC-1) ? Bell() : GF_SetPos (j+1);
                       break;
          case PgDn   :
          case CtrlC  :(j==DirC-1) ? Bell() : GF_SetPos (j+8);
                       break;
          case PgUp   :
          case CtrlR  :(j==0   ) ? Bell() : GF_SetPos (j-8);
                       break;
          case Del    :
          case CtrlG  :
          case CtrlY  :if (DirList[j].DirectoryF)
                          Bell();
                       else {
                          i=True;
                          k=HGetYesNo(" ÑÁ·©·i »¡¶‰Œa¶a? ",&i);
                          Way=Del;
                          if (k==OK)
                            if (i) {
                               char EraPath[MAXPATH];
                               fnmerge(EraPath,MainDrive,MainDirec,DirList[j].Name,"");
                               if (remove(EraPath)==0) {
                                 memmove (&DirList[j],&DirList[j+1],sizeof(SelectFile)*(511-j));
                                 DirC--;
                                 GF_SetBox();
                                 if (DirC==0) {
                                    Way=ESC;
                                 } else if (j==DirC) {
                                    GF_SetPos(j-1);
                                 } else GF_SetPos(j);
                              } else HSayIOErr();
                            }
                       }
                       break;
          case Return :SelectFile=DirList[j];
                       break;
          default     :if ( !ExtKey && isalpha (Key) ) {
                          i=j;
                          do  {
                              i= (i==DirC) ? 0 : i+1;
                              if (DirList[i].Name[0]==toupper(Key)) GF_SetPos(i),i=65535;
                          } while (i !=j && i!=65535 );
                       }
        }
    } while (Way !=Return && Way !=ESC && !HotKeyF);
    Pop(InverseF);
    return (Way==Return) ? OK : Error;
}

void SetExts(char *Data)
{
    word i,j;
    char Buf[4];

    i=j=0;
    while (True) {
      if (ExtNum==MaxExts) break;
      if (Data[i]==';' || Data[i]==0) {
         Buf[j]=0;
         if (!ExistExt(Buf))
           strcpy(Exts[ExtNum++],Buf);
         if (Data[i]==0) break;
         j=0;
      } else {
         if (j<3) {
            Buf[j]=Data[i];
            j++;
         }
      }
      i++;
    }
}


void ClearExts(void)
{
    ExtNum=0;
}

word HGetFileName (char *FileName)
{

    char NameBuf[MAXFILE],ExtBuf[MAXEXT];
    word i,j;
    char *k;

    HOpenWindowSimple(0,0,VideoRight,0);
    HOpenWindowSimple(0,VideoBot,VideoRight,VideoBot);

    k=strpbrk(FileName,"?*");
    if (k==NULL) QuickF=True;
    else         QuickF=False;

    BoxOpenedF=False;
    i=0;
    strupr(FileName);
    while (FileName[i]!=0 && FileName[i]!=';') {
      Path[i]=FileName[i];
      i++;
    }
    Path[i]=0;
    ClearExts();
    FNSplit (Path,MainDrive,MainDirec,NameBuf,ExtBuf);
    if (ExtBuf[0]==0)     SetExts(StdExt);
    else                  SetExts(ExtBuf+1);
    if (FileName[i]==';') SetExts(FileName+i+1);
    if (strlen(NameBuf)==0)    strcpy(Name,"*");
    else                  strcpy(Name,NameBuf);

    do  {
        if (FExpand (Path)!=OK) {
            HSayIOErr();
            HCloseWindowSimple();
            HCloseWindowSimple();
            return Error;
        }
        FNSplit (Path,MainDrive,MainDirec,NameBuf,ExtBuf);
        strcpy(Path,MainDrive);
        strcat(Path,MainDirec);
        strcat(Path,Name);
        strcat(Path,".");
        for (j=0 ; j<ExtNum ; j++) {
          if (strlen(Path)>MAXPATH-5) {
            if (j==0) strcat(Path,"...");
            else      strcat(Path,";...");
          } else {
            if (j!=0) strcat(Path,";");
            strcat(Path,Exts[j]);
          }
        }
        Path[79]=0;
        HClrBox(0,0,VideoRight,0);
        DrawString(0,0,Path);
        ClearDirList();
        for (j=0 ; j<ExtNum ; j++) {
          strcpy(ExtBuf,".");
          strcat(ExtBuf,Exts[j]);
          if (ReadNames(MainDrive,MainDirec,Name,ExtBuf) !=OK) {
             HSayIOErr() ;
             if (BoxOpenedF) HCloseWindow();
             HCloseWindowSimple();
             HCloseWindowSimple();
             return Error;
          }
        }
        if (ReadDirs(MainDrive,MainDirec)!=OK) {
           HSayIOErr() ;
           if (BoxOpenedF) HCloseWindow();
           HCloseWindowSimple();
           HCloseWindowSimple();
           return Error;
        }
        if (DirC==0) {
           HSayIOErr() ;
           if (BoxOpenedF) HCloseWindow();
           HCloseWindowSimple();
           HCloseWindowSimple();
           return Error;
        }
        errno=0;
        SortDir();
        if (GF_SelectFile (Name)!=OK) {
            if (BoxOpenedF) HCloseWindow();
            HCloseWindowSimple();
            HCloseWindowSimple();
            return Error;
        }
        fnmerge (Path,MainDrive,MainDirec,SelectFile.Name,"");
        if (SelectFile.DirectoryF) strcat (Path,"\\");
    } while (SelectFile.DirectoryF);
    if (BoxOpenedF) HCloseWindow();
    HCloseWindowSimple();
    HCloseWindowSimple();
    strcpy (FileName,Path);
    FExpand(FileName);
    return OK;
}

