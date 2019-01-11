word OpenPrinter(void)
{
    BufPos=0;
    PrintingLine=1;
    switch (PrintingType) {
      case 0 : break;
      case 1 : break;

      case 2 :
      case 3 :
      case 4 :
      case 5 : PrintByte(27);
               PrintByte('@');
               break;
      case 6 :
      case 7 :
      case 8 :
      case 9 : PrintChars("\33" "E\33" "*t");
               switch (PrintingType) {
                 case 6 : PrintChars("300");
                          break;
                 case 7 : PrintChars("150");
                          break;
                 case 8 : PrintChars("100");
                          break;
                 case 9 : PrintChars("75");
               }
               PrintByte('R');
               break;
    }
    return OK;
}

word ClosePrinter(void)
{
    PrintByte('L'-'A'+1);
    if (SendData()!=OK) return Error;
    return OK;
}


void PrintChars(byte *Data)
{
     while (*Data!=0) PrintByte(*Data),Data++;
}

void PrintBytes(byte *Data,word Count)
{
     while (Count>0) PrintByte(*Data),Data++,Count--;
}

word SendData(void)
{
     word i;
     word j;
     word Result;

     j=OK;
     for (i=0 ; i<BufPos ; i++) {
         Result=biosprint(0,DiskBuf[i],PrinterPort-1);
         if ( Result & 32 ) {
             if (HGetYesNo(" ¹··¡ˆa ´ô¯s“¡”a. ‰­¢ ·¥­áŸi ÐiŒa¶a? ",&j)!=OK) return Error;
             if (!j) return Error;
             i--;
             break;
         }
         if (Result & 1) {
             if (HGetYesNo(" ÏaŸ¥Èáˆa  âÉ··³“¡”a. ”a¯¡ Ð¥©Œa¶a? ",&j)!=OK) return Error;
             if (!j) return Error;
             i--;
             break;
         }
         if (Result & 8) {
             if (HGetYesNo(" ·³Â‰bµA ¢…¹Aˆa ¬—‰v¯s“¡”a. ”a¯¡ Ð¥©Œa¶a?",&j)!=OK) return Error;
             if (!j) return Error;
             i--;
         }
     }
     BufPos=0;
     return OK;
}



word CloseLine(void)
{
    word i;

    switch (PrintingType) {
      case 0 :
      case 1 :PrintByte(LF);
              PrintByte(CR);
              break;
      case 2 :
      case 3 :
      case 4 :
      case 5 :PrintByte(27);
              PrintByte(74);
              PrintByte(24+SpaceLength);
              PrintByte(CR);
              break;
      case 6 :
      case 7 :
      case 8 :
      case 9 :for (i=0; i<SpaceLength; i++) {
                 PrintChars("\33" "*b0W");
              }
    }
    if (SendData()!=OK) return Error;
    return OK;
}

