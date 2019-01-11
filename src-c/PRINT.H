#ifndef __PRINT__

#define __PRINT__

#define PrintByte(Data)   (DiskBuf[BufPos++]=Data)

word  extern PrinterPort;
int   extern PrinterHandler;
word  extern PrintingType;
word  extern LeftMargin,TopMargin,BotMargin,PageLength,SpaceLength;

word OpenPrinter(void);
word ClosePrinter(void);
word SendData(void);
void PrintChars(byte *Data);
void PrintBytes(byte *Data,word Count);
word CloseLine(void);

word GetHanFontPos(word Data);
word ContinuousByte ( byte Data);
word ContinuousWord( word Data);
word ContinuousGrfWord( word Data);
word static IsLineChar(word Data);

word PrintLQ24Eng(word Data);
word PrintLQ24Han(word Data);
word LQ24PrintString(byte *Data);

word PrintLQ8Eng(word Data,word OddF);
word PrintLQ8Han(word Data,word OddF);
word LQ8PrintString(byte *Data);

word ModulPrintString(byte *Data);

word LaserPrintString(byte *Data);

word PrintString (byte *Data);

#endif
