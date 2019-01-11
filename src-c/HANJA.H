#ifndef __HANJA__

#define __HANJA__

int     extern HanjaFileHandler;
void    HanjaPos(void);
word    InitHanja(void);
word    TranslateCode(byte *CodeP,word X,word Y);

#endif