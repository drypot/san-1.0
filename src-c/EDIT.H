/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                  Editor Functions                    */
/*                                                      */

word LoadFile (void);
word SaveFile (void);
word SaveAll  (void);
word WriteTo  (void);
word NewFile  (void);
word PrintFile(void);
word Dir      (void);
word ChangeDir(void);
word SaveBlock(void);
word LoadBlock(void);
void QuitWork(void);

word PutEditBuf  (void);
void SetEditBuf  (void);
void SetAuxBuf   (word Line);
void SetUndoBuf  (byte *Data);
void SayBufStatus(void);
void ResizeBuf   (void);
word ResizePaper (word ln);
word BufErr      (void);
word CheckBuf    (void);
word PutPaper    (word ln,byte *data);
void DeleteLine     (word ln);
word InsertBlankLine(word ln);
word InsertLine     (word ln);

#define CurXPos() (HX+HSX-1)
#define ShowhotKey(Num,Data) DrawString((Num-1)*11+1,24,Data)
void SetScn    (void);
void SetEditScn  (void);
void PrintXYpos(void);
void iw        (byte *data);
void EditHelp  (void);
void WriteLine (word ypos,byte *data);
void ClearLine (word ypos);
void SetScn    (void);
void SetCurPos (word Newx,word  Newy);
void FoolSetCur(word Newx,word  Newy);
void DrawMoreKey(char *data);
word SelectPos(char *data);

word SelectGrf (void);
void EditDraw  (void);

void EditIns   (void);
void ChangeLanguage(void);

byte *AppendSpace(byte *Data,word Len);
word PrintBlock(void);
void CopyBlock(void);
void DeleteBlock(void);
void MoveBlock(void);

#define EditCKP() PrintBlock()
#define EditCKC() CopyBlock()
#define EditCKV() MoveBlock()
#define EditCKY() DeleteBlock()

void EditAK(void);
void EditAL(void);
void EditAA(void);
void CancelSelectBlock(void);
void CheckBlock0(void);
void UnselectBlock(void);
void GetBlock(word TaskN);
void SendBlock(word TaskN);

void SwapBlockpoworder(void);
void EditCKB(void);
void EditCKK(void);
void EditCKT(void);
void EditCKR(void);
void EditCKW(void);
void EditCKH(void);
void EditCKI(void);
void EditCKU(void);
void EditCKL(void);
void EditCK (void);

void EditAQ (void);
void EditAW (void);
void EditAE (void);

word FindSearchString(void);
void FindSub         (void);
void ReplaceSub      (void);
void SetSearchOptions(void);
void EditCQF(void);
void EditCQA(void);
void EditCL (void);

void MainMenu(void);
void ChangeTasks(word NewTask);
void InitTasks(void);
word SaveOptions(void);
word RetrieveOptions(void);

#define EditCN()  InsertLine (NowLine),SetScnF=True
#define EditCY()  DeleteLine (NowLine)
void SaveKillLine(byte *Data);
void EditCU(void);

#define EditCR()    SetCurPos ((HX+SX-1),NowLine-ScnHigh)
#define EditCC()    SetCurPos ((HX+SX-1),NowLine+ScnHigh)
#define EditCQP()   SetCurPos (HXB+XB-ScnLeft,HYB+YB-ScnTop)
#define EditCEnd()  (HY+(ScnBot-ScnTop+1)-1>PaperLen) ? \
                    SetCurPos ((HX+SX-1),NowLine+(PaperLen-(HY+SY-ScnTop))) :\
                    SetCurPos ((HX+SX-1),NowLine+(ScnBot-SY))
#define EditHome()  SetCurPos (1,NowLine)
#define EditCHome() SetCurPos ((HX+SX-1),NowLine-(SY-ScnTop))
#define EditCPgUp() SetCurPos (1,1)
#define EditCPgDn() SetCurPos (Paper[PaperLen][0]+1,PaperLen)

#define EditCQY()   EraseElement(CurXPos(),255);
void EditCQB(void);
void EditCQK(void);
void    EditCS(void);
void    EditCD(void);
void    EditCE(void);
void    EditCX(void);
void    EditCA(void);
void    EditCF(void);
void    EditCW(void);
void    EditCZ(void);
void    EditEnd()  ;

#define Stringstartpos(ln) (strspn(Paper[(word)ln]+1," ")+1)
word    slettersfromrow    (word row);
word    sletterstillrow    (word row);
#define spacetillrow(row)  ((strspn(EditBuf+1," ")>=(word)(row))?True:False)
word    spacefromrow       (word row);
word    indenttoleft       (word row,word ln);
word    indenttoright      (word ln);
void    EraseElement       (word S,word E);
word    InsertElement      (word S,word Len);
word    ReplaceElement     (word S,word Len);
#define BlankEditBuf()     ((strspn(EditBuf+1," ")==256-2) ? True : False)
void    EditCQL      (void);
void    EditCQI      (void);
void    EditCOL      (void);
void	EditCOK	     (void);
void    EditBackSpace(void);
void    EditCT       (void);
void    EditReturn   (void);
void    EditF4       (void);
void    EditF9       (void);
void    EditAsc      (void);
void    EditTab      (void);
void    EditCO       (void);
void    EditCQ       (void);

word    OpenPickList (void);
word    ClosePickList(void);
void    OpenPickData (void);
void    StorePickData(void);
void    RestorePickData(void);
word    SelectPick(void);

void    Run(word Item);

void    EditAT(void);