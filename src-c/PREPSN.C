word ContinuousByte ( byte Data)
{
    word i;
    word j;

    for (i=j=0 ; i<8 ; i++) {
       if (Data>=0x80) j++;
       else            j=0;
       if (j>3) return OK;
       Data<<=1;
    }
    return False;
}

word ContinuousWord( word Data)
{
    word i;
    word j;

    for (i=j=0 ; i<16 ; i++) {
       if (Data & 0x8000) j++;
       else               j=0;
       if (j>2) return OK;
       Data<<=1;
    }
    return False;
}

word ContinuousGrfWord( word Data)
{
    word i;
    word j;

    for (i=j=0 ; i<16 ; i++) {
       if (Data & 0x8000) j++;
       else               j=0;
       if (j>4) return OK;
       Data<<=1;
    }
    return False;
}



word GetHanFontPos(word Data);
word static IsLineChar(word Data)
{
     byte first,second;

     second=Data / 0x100;
     first=Data;

     if (first==0xd4)
        return (second>=0xb3 && second<=0xda)? OK :False;
     else if (first==0xdb)
        return (second>=0xa1 && second<=0xE4)? OK :False;
     return False;
}

