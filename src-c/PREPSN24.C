word PrintLQ24Eng(word Data)
{
    word i,j,k,l;
    word FontImg;
    word EnlargeCount;

    EnlargeCount=0;
    MemSetB(EngMap,0,24);
    for (j=l=0 ; j<16 ; j++) {
      FontImg=(*EngFont)[Data][j];
      EngMap[l++]=FontImg;
      if (j%2) EnlargeCount++;
      if (EnlargeCount && !ContinuousByte(FontImg)) {
        while (EnlargeCount!=0) {
           EngMap[l++]=FontImg;
           EnlargeCount--;
           if (EngMap[l-1]==0) break;
        }
      }
    }


    for (i=0 ; i<8 ; i++)
      for (j=0 ; j<3 ; j++) {
         for (k=l=0 ; k<8 ; k++)
           if ( EngMap[j*8+k]&(1<<(7-i)) )
             l|=1<<(7-k);
         PrintByte(l);
      }
    return OK;


}

word PrintLQ24Han(word Data)
{
    word i,j,k,l;
    byte FontImg[16][2];
    word FontSeg,FontOffset,FontOffset2;
    word EnlargeCount;

    GetHanFontPos(Data);
    FontOffset =_AX;
    FontSeg    =_DX;
    FontOffset2=_BX;

    if ( FontOffset2 ) {
       MemCopy(Pointer(FontSeg,FontOffset) ,FontImg,2*16);
       MemOR  (Pointer(FontSeg,FontOffset2),FontImg,2*16);
    } else
       MemCopy(Pointer(FontSeg,FontOffset) ,FontImg,2*16);

   EnlargeCount=0;
   MemSetW(HanMap,0,24);
   for (j=l=0 ; j<16 ; j++) {
     k=FontImg[j][1];
     FontImg[j][1]=FontImg[j][0];
     FontImg[j][0]=k;
     HanMap[l++]=*(word*)FontImg[j];
     if (j%2) EnlargeCount++;
     if (IsLineChar(Data)) {
        if (EnlargeCount && !ContinuousGrfWord(HanMap[l-1])) {
              HanMap[l]=HanMap[l-1];
              l++;
              EnlargeCount--;
        }
     } else {
        if (EnlargeCount && !ContinuousWord(HanMap[l-1]) ) {
           while (EnlargeCount!=0) {
              HanMap[l]=HanMap[l-1];
              l++;
              EnlargeCount--;
              if (HanMap[l-1]==0) break;
           }
        }
     }
   }

    for (i=0 ; i<16 ; i++)
      for (j=0 ; j<3 ; j++) {
         for (k=l=0 ; k<8 ; k++)
           if ( HanMap[j*8+k]&(1<<(15-i)) )
             l|=1<<(7-k);
         PrintByte(l);
      }
    return OK;
}



word LQ24PrintString(byte *Data)
{
     word i,j;

     j=strlen(Data);
     if (j!=0) {
         PrintByte(27) ;
         PrintByte('*');
         PrintByte(33);
         PrintByte( (j*8)%256 );
         PrintByte( (j*8)/256 );

         for (i=0; i<j ; i++)
           if (Data[i]<128) {
              if (PrintLQ24Eng(*(byte*)(Data+i))!=OK) return Error;
           } else {
              if (PrintLQ24Han(*(word*)(Data+i++))!=OK) return Error;
           }
     }

     return CloseLine();
}