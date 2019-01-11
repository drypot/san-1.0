byte  *ImgBuf;

word LaserPrintString(byte *Data)
{
     word i,k,j,l;
     word FontSeg,FontOffset,FontOffset2;
     byte Buf[10];

     j=strlen(Data);
     ImgBuf=DiskBuf+11*1024;
     for (i=0; i<j ; i++)
       if (Data[i]<128) {
          for (k=0; k<16; k++)
             *(ImgBuf+k*j+i)=(*EngFont)[Data[i]][k];

       } else {
          GetHanFontPos(*(word*)(Data+i));
          FontOffset =_AX;
          FontSeg    =_DX;
          FontOffset2=_BX;
          if ( FontOffset2 ) {
             MemCopy(Pointer(FontSeg,FontOffset) ,HanMap,2*16);
             MemOR  (Pointer(FontSeg,FontOffset2),HanMap,2*16);
          } else
             MemCopy(Pointer(FontSeg,FontOffset) ,HanMap,2*16);
          for (k=0; k<16; k++) {
             *(ImgBuf+k*j+i) =*( (byte*)HanMap+k*2 );
             *(ImgBuf+k*j+i+1) =*( (byte*)HanMap+k*2+1);
          }
          i++;
       }

     sprintf(Buf,"%d",j);
     for (l=0; l<16 ; l++) {
         PrintChars("\33*b");
         PrintChars(Buf);
         PrintByte ('W');
         PrintBytes(ImgBuf+l*j,j);
     }
     return CloseLine();
}