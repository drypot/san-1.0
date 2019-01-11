word ModulPrintString(byte *Data)
{
   word i;

   for (i=0 ; i<strlen(Data) ; i++)
     PrintByte (Data[i]);
   return CloseLine();
}