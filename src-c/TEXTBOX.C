
#define  Pointer(segment,offset) ((void far *)(((unsigned long)(segment)<<16) | (offset)))
typedef unsigned char byte;
typedef unsigned int  word;

byte     (far *Screen)[25][160]=Pointer(0xb800,0x0000);
#define  PutCh(x,y,c)    ((*Screen)[y][(x)*2  ]=(byte)(c))

#define Up         196
#define Down       196
#define Right      179
#define Left       179
#define UpRight    191
#define UpLeft     218
#define DownRight  217
#define DownLeft   192

void DrawBox(word X1,word Y1,word X2,word Y2)
{
   word i;

   for (i=X1+1 ; i<X2 ; i++) PutCh(i,Y1,Up),PutCh(i,Y2,Down);
   for (i=Y1+1 ; i<Y2 ; i++) PutCh(X1,i,Left),PutCh(X2,i,Right);
   PutCh(X1,Y1,UpLeft   );
   PutCh(X2,Y1,UpRight  );
   PutCh(X1,Y2,DownLeft );
   PutCh(X2,Y2,DownRight);
}

void ClrScr(void)
{
   word i,j;

   for (i=0 ; i<25 ; i++ )
     for (j=0; j<80 ; j++) PutCh(i,j,' ');
}

void main(void)
{
   ClrScr();
   DrawBox(10,10,20,20);
}