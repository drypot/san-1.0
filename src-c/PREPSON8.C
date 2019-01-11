word PrintLQ8Eng(word Data,word OddF)
{
    word l,k,i;

    for (i=0 ; i<8 ; i++) {
       for (k=l=0 ; k<8 ; k++)
           if ( (*EngFont)[Data][k*2+OddF] & (1<<(7-i)) )
               l|=1<<(7-k);
       PrintByte(l);
    }
    return OK;
}

word PrintLQ8Han(word Data,word OddF)
{
    word i,j,k,l;
    byte FontImg[16][2];
    word FontSeg,FontOffset,FontOffset2;

    GetHanFontPos(Data);
    FontOffset =_AX;
    FontSeg    =_DX;
    FontOffset2=_BX;

    if ( FontOffset2 ) {
       MemCopy(Pointer(FontSeg,FontOffset) ,FontImg,2*16);
       MemOR  (Pointer(FontSeg,FontOffset2),FontImg,2*16);
    } else
       MemCopy(Pointer(FontSeg,FontOffset) ,FontImg,2*16);

    for (j=OddF ; j<16 ; j+=2) {
      k=FontImg[j][1];
      FontImg[j][1]=FontImg[j][0];
      FontImg[j][0]=k;
    }

    for (i=0 ; i<16 ; i++) {
         for (k=l=0 ; k<8 ; k++)
           if ( *(word*)(FontImg[k*2+OddF])&(1<<(15-i)) )
             l|=1<<(7-k);
         PrintByte(l);
    }
    return OK;
}



word LQ8PrintString(byte *Data)
{
     word i,j,k;

     j=strlen(Data);
     if (j!=0)
        for (k=0 ; k<2 ; k++) {
             PrintByte(27);
             PrintByte('*');
             PrintByte(1);
             PrintByte( (j*8)%256 );
             PrintByte( (j*8)/256 );
             for (i=0; i<j ; i++)
               if (Data[i]<128) {
                  PrintLQ8Eng(*(byte*)(Data+i),k);
               } else {
                  PrintLQ8Han(*(word*)(Data+i++),k);
               }
             PrintByte(27);
             PrintByte(74);
             PrintByte(2);
             PrintByte(CR);

         }
     return CloseLine();
}