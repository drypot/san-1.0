#include "stddef.h"
#include "hanja.h"
#include <io.h>
#include <mem.h>
#include <alloc.h>
#include <dos.h>
#include "edit.h"
#include "const.h"
#include "memory.h"
#include "handrv.h"
#include "hwindow.h"
#include "crt.h"

#define MaxHanjaCache 400
#define ImgCache (*ImgC)

word    CodeC[4889];
byte    ImgCache[MaxHanjaCache][32];
word    ImgMarker[MaxHanjaCache];
word    CMarker;
int     HanjaFileHandler;

word    Hantable[472] = {
        0x8861,0x8862,0x8865,0x8869,0x8871,0x8873,
        0x8877,0x8881,0x8882,0x8897,0x88a2,0x88e1,
        0x88e5,0x88e9,0x88f1,0x88f3,0x8941,0x8962,
        0x8965,0x8969,0x8971,0x8977,0x8981,0x89a1,
        0x89a2,0x89a5,0x89a9,0x89b7,0x89b8,0x89c1,
        0x89c2,0x89c5,0x89c9,0x89d7,0x89e1,0x8a41,
        0x8a57,0x8a61,0x8a81,0x8a82,0x8a85,0x8a89,
        0x8a97,0x8aa5,0x8aa9,0x8ac1,0x8ae1,0x8b41,
        0x8b45,0x8b49,0x8b62,0x8b65,0x8b69,0x8b71,
        0x8b73,0x8b77,0x8ba1,0x8ba5,0x8ba9,0x8bb1,
        0x8bb3,0x9061,0x9062,0x9065,0x9069,0x9071,
        0x9073,0x9077,0x9081,0x9097,0x9161,0x9165,
        0x9171,0x9177,0x91a1,0x91a2,0x91a5,0x91b7,
        0x9241,0x9261,0x9281,0x9285,0x9289,0x9341,
        0x9362,0x9371,0x9377,0x93a1,0x93a2,0x9461,
        0x9465,0x9469,0x9471,0x9473,0x9477,0x9481,
        0x9482,0x94e2,0x95a1,0x95a2,0x95a5,0x95a9,
        0x95b7,0x9681,0x9685,0x9762,0x9777,0x9c61,
        0x9c62,0x9c65,0x9c69,0x9c71,0x9c73,0x9c77,
        0x9c81,0x9c97,0x9ca2,0x9cb7,0x9d61,0x9d62,
        0x9d65,0x9d69,0x9d71,0x9d73,0x9d77,0x9d81,
        0x9da1,0x9da2,0x9da5,0x9db7,0x9e41,0x9e61,
        0x9e77,0x9e81,0x9f41,0x9f42,0x9f45,0x9f49,
        0x9f57,0x9f62,0x9f71,0x9f77,0x9fa1,0x9fa5,
        0x9fb1,0x9fb3,0xa061,0xa062,0xa065,0xa069,
        0xa077,0xa081,0xa082,0xa097,0xa162,0xa165,
        0xa169,0xa177,0xa181,0xa1a1,0xa1a2,0xa1a9,
        0xa1b7,0xa261,0xa281,0xa282,0xa285,0xa289,
        0xa3a1,0xa3a5,0xa3a9,0xa462,0xa465,0xa469,
        0xa477,0xa481,0xa482,0xa4e5,0xa4e9,0xa4f1,
        0xa4f3,0xa562,0xa565,0xa569,0xa577,0xa5a1,
        0xa5a2,0xa5a5,0xa5a9,0xa5b7,0xa681,0xa682,
        0xa685,0xa689,0xa697,0xa7a1,0xa7a5,0xa7b7,
        0xac61,0xac62,0xac65,0xac69,0xac71,0xac73,
        0xac77,0xac81,0xac82,0xac97,0xace1,0xace2,
        0xace5,0xace9,0xacf1,0xacf3,0xacf7,0xad41,
        0xada1,0xada2,0xada5,0xada9,0xadb7,0xade1,
        0xae41,0xae81,0xae82,0xae85,0xae89,0xae97,
        0xaf69,0xaf73,0xaf77,0xafa1,0xafa2,0xafa5,
        0xafa9,0xafb1,0xafb3,0xb077,0xb3a1,0xb461,
        0xb462,0xb465,0xb469,0xb471,0xb473,0xb477,
        0xb481,0xb482,0xb497,0xb4a1,0xb4a2,0xb4b7,
        0xb4e1,0xb4e2,0xb4e5,0xb4e9,0xb4f1,0xb4f3,
        0xb545,0xb561,0xb562,0xb565,0xb569,0xb571,
        0xb573,0xb577,0xb581,0xb5a1,0xb5a2,0xb5a5,
        0xb5a9,0xb5b7,0xb5c1,0xb5c5,0xb5c9,0xb5d7,
        0xb5e1,0xb641,0xb661,0xb662,0xb677,0xb681,
        0xb682,0xb685,0xb689,0xb697,0xb6a5,0xb6a9,
        0xb6e1,0xb741,0xb742,0xb745,0xb749,0xb757,
        0xb765,0xb769,0xb771,0xb773,0xb777,0xb781,
        0xb7a1,0xb7a2,0xb7a5,0xb7a9,0xb7b1,0xb7b3,
        0xb7b7,0xb861,0xb862,0xb865,0xb871,0xb873,
        0xb877,0xb881,0xb897,0xb8e1,0xb8e2,0xb8e5,
        0xb8e9,0xb8f1,0xb8f3,0xb8f7,0xb941,0xb9a1,
        0xb9a2,0xb9a5,0xb9a9,0xb9b7,0xb9c1,0xba41,
        0xba81,0xba82,0xba85,0xba89,0xba97,0xbb62,
        0xbb69,0xbb73,0xbb77,0xbba1,0xbba2,0xbba5,
        0xbba9,0xbbb1,0xbbb3,0xbbb7,0xc061,0xc062,
        0xc065,0xc069,0xc071,0xc077,0xc081,0xc082,
        0xc0e1,0xc0e2,0xc0e5,0xc0e9,0xc0f1,0xc0f3,
        0xc0f7,0xc141,0xc1a1,0xc1a2,0xc1a5,0xc1b7,
        0xc1c9,0xc241,0xc281,0xc282,0xc285,0xc289,
        0xc297,0xc2e1,0xc362,0xc377,0xc3a1,0xc3a2,
        0xc3a5,0xc3a9,0xc3b1,0xc3b3,0xc3b7,0xc5e1,
        0xc861,0xc862,0xc865,0xc869,0xc871,0xc873,
        0xc877,0xc881,0xc882,0xc897,0xc8e1,0xc9a1,
        0xc9b7,0xca41,0xca81,0xcb62,0xcb71,0xcc61,
        0xcc65,0xcc69,0xcc81,0xcc97,0xcca2,0xcd65,
        0xcd71,0xcd77,0xcd81,0xcda1,0xcda2,0xce61,
        0xce91,0xce97,0xcfa1,0xcfa9,0xcfb3,0xd061,
        0xd062,0xd065,0xd069,0xd071,0xd073,0xd077,
        0xd081,0xd082,0xd097,0xd0b7,0xd0e1,0xd0e5,
        0xd0e9,0xd0f1,0xd162,0xd165,0xd169,0xd171,
        0xd173,0xd177,0xd181,0xd1a1,0xd1a2,0xd1a5,
        0xd1a9,0xd1b7,0xd1c1,0xd1c2,0xd1c5,0xd1c9,
        0xd1d7,0xd241,0xd242,0xd257,0xd261,0xd281,
        0xd285,0xd297,0xd385,0xd2c1,0xd2e1,0xd341,
        0xd349,0xd357,0xd362,0xd365,0xd369,0xd371,
        0xd373,0xd377,0xd381,0xd3a9};

word    Hindex1[472]= {    // Ðe¸a START POINT
        0xE031, 0xE04E, 0xE059, 0xE071, 0xE07B, 0xE0A1,
        0xE0A7, 0xE0BF, 0xE0D3, 0xE0D5, 0xE0D9, 0xE0DA,
        0xE0EB, 0xE0F7, 0xE0FB, 0xE134, 0xE137, 0xE13A,
        0xE141, 0xE14C, 0xE153, 0xE158, 0xE197, 0xE1AF,
        0xE1D6, 0xE1DD, 0xE1E7, 0xE1EA, 0xE1FA, 0xE1FB,
        0xE239, 0xE23D, 0xE24E, 0xE252, 0xE25F, 0xE262,
        0xE26B, 0xE26F, 0xE29A, 0xE2D0, 0xE2D6, 0xE2DC,
        0xE2E0, 0xE2E6, 0xE2F0, 0xE2F5, 0xE2FB, 0xE333,
        0xE342, 0xE349, 0xE34A, 0xE351, 0xE360, 0xE361,
        0xE36F, 0xE376, 0xE37A, 0xE3CC, 0xE3CD, 0xE3D1,
        0xE3D2, 0xE3D3, 0xE3E2, 0xE3EA, 0xE3F3, 0xE3F5,
        0xE3FE, 0xE435, 0xE43C, 0xE442, 0xE443, 0xE444,
        0xE447, 0xE44B, 0xE44D, 0xE45F, 0xE465, 0xE466,
        0xE46D, 0xE473, 0xE474, 0xE47C, 0xE47D, 0xE47E,
        0xE492, 0xE494, 0xE495, 0xE49B, 0xE49D, 0xE49F,
        0xE4A1, 0xE4B5, 0xE4BA, 0xE4CB, 0xE4D0, 0xE4DB,
        0xE4EB, 0xE4EC, 0xE4EE, 0xE548, 0xE552, 0xE55C,
        0xE55E, 0xE56F, 0xE57A, 0xE592, 0xE593, 0xE59C,
        0xE5A5, 0xE5AE, 0xE5B7, 0xE5B9, 0xE5C3, 0xE5C6,
        0xE5CE, 0xE5D2, 0xE5D3, 0xE5D5, 0xE5E2, 0xE5F4,
        0xE5FB, 0xE639, 0xE63F, 0xE644, 0xE645, 0xE657,
        0xE65C, 0xE66E, 0xE675, 0xE676, 0xE67D, 0xE697,
        0xE6A3, 0xE6A4, 0xE6B1, 0xE6BF, 0xE6C2, 0xE6C8,
        0xE6CC, 0xE6CD, 0xE6CF, 0xE6D0, 0xE6D6, 0xE6F0,
        0xE6F9, 0xE6FE, 0xE734, 0xE73C, 0xE742, 0xE755,
        0xE75C, 0xE768, 0xE776, 0xE77B, 0xE793, 0xE795,
        0xE7A0, 0xE7A2, 0xE7B1, 0xE7B2, 0xE7CA, 0xE7D1,
        0xE7D3, 0xE7D6, 0xE7E2, 0xE7F8, 0xE7FA, 0xE838,
        0xE83B, 0xE84E, 0xE85B, 0xE85E, 0xE871, 0xE89C,
        0xE8A7, 0xE8C3, 0xE8D7, 0xE8DF, 0xE8E9, 0xE8ED,
        0xE8F6, 0xE8F8, 0xE935, 0xE93C, 0xE940, 0xE951,
        0xE961, 0xE972, 0xE973, 0xE974, 0xE996, 0xE9C1,
        0xE9C2, 0xE9D5, 0xE9DA, 0xE9E0, 0xEA3D, 0xEA4B,
        0xEA4F, 0xEA9D, 0xEAA1, 0xEAAD, 0xEAB2, 0xEABA,
        0xEABE, 0xEADD, 0xEAE0, 0xEAE5, 0xEAEA, 0xEB3A,
        0xEB49, 0xEB69, 0xEB76, 0xEB7E, 0xEB94, 0xEBA6,
        0xEBAF, 0xEBD4, 0xEBDD, 0xEBE3, 0xEBE4, 0xEBEC,
        0xEBF1, 0xEBF3, 0xEC62, 0xEC6E, 0xEC9B, 0xEC9F,
        0xECA2, 0xECA5, 0xECAA, 0xECB4, 0xECD0, 0xECDF,
        0xECF7, 0xECFB, 0xED37, 0xED3A, 0xED3B, 0xED3C,
        0xED4E, 0xED5C, 0xED66, 0xED6A, 0xED72, 0xED76,
        0xED7D, 0xED9A, 0xEDA1, 0xEDA5, 0xEDB0, 0xEDB9,
        0xEDD8, 0xEDE2, 0xEDE7, 0xEDED, 0xEDEF, 0xEDF5,
        0xEDF7, 0xEDF8, 0xEE42, 0xEE4F, 0xEE7A, 0xEE96,
        0xEEA5, 0xEEA9, 0xEED1, 0xEEE9, 0xEF39, 0xEF3E,
        0xEF44, 0xEF45, 0xEF4E, 0xEF56, 0xEF68, 0xEF69,
        0xEF6E, 0xEF72, 0xEF77, 0xEFAF, 0xEFB5, 0xEFCD,
        0xEFED, 0xEFF6, 0xF035, 0xF038, 0xF03A, 0xF055,
        0xF058, 0xF071, 0xF0BB, 0xF0C2, 0xF0CF, 0xF0D4,
        0xF0D9, 0xF0E0, 0xF0E1, 0xF0E7, 0xF0EA, 0xF0EE,
        0xF133, 0xF159, 0xF161, 0xF179, 0xF194, 0xF19F,
        0xF1A4, 0xF1A8, 0xF1C2, 0xF1CF, 0xF1D4, 0xF1DA,
        0xF1DB, 0xF232, 0xF243, 0xF247, 0xF263, 0xF27C,
        0xF2B7, 0xF2BF, 0xF2C8, 0xF2CB, 0xF334, 0xF34B,
        0xF379, 0xF37D, 0xF391, 0xF394, 0xF3A5, 0xF3AA,
        0xF3AB, 0xF3D3, 0xF3D5, 0xF3E8, 0xF3E9, 0xF3ED,
        0xF3EE, 0xF3EF, 0xF3F3, 0xF3FD, 0xF451, 0xF456,
        0xF479, 0xF49A, 0xF49C, 0xF4A3, 0xF4A7, 0xF4B5,
        0xF4BC, 0xF4CB, 0xF4D0, 0xF4DA, 0xF4F0, 0xF4FC,
        0xF532, 0xF536, 0xF545, 0xF558, 0xF562, 0xF56C,
        0xF576, 0xF57E, 0xF59A, 0xF5B5, 0xF5BB, 0xF5BF,
        0xF5C9, 0xF5CA, 0xF5CD, 0xF5E4, 0xF5F0, 0xF5F3,
        0xF5F6, 0xF5FC, 0xF640, 0xF645, 0xF646, 0xF652,
        0xF661, 0xF662, 0xF665, 0xF66E, 0xF66F, 0xF671,
        0xF672, 0xF692, 0xF6A2, 0xF6AC, 0xF6AE, 0xF6B2,
        0xF6B5, 0xF6BA, 0xF6C8, 0xF6CB, 0xF6CC, 0xF6CD,
        0xF6D1, 0xF6D8, 0xF6DE, 0xF6E4, 0xF6E6, 0xF6E7,
        0xF6F7, 0xF732, 0xF735, 0xF740, 0xF744, 0xF745,
        0xF74F, 0xF750, 0xF755, 0xF75F, 0xF77B, 0xF793,
        0xF7A1, 0xF7A3, 0xF7A8, 0xF7AF, 0xF7B9, 0xF7BB,
        0xF7C9, 0xF7CE, 0xF7DC, 0xF7DE, 0xF7EA, 0xF7F1,
        0xF834, 0xF846, 0xF848, 0xF84D, 0xF856, 0xF85A,
        0xF852, 0xF85F, 0xF861, 0xF865, 0xF87A, 0xF87E,
        0xF891, 0xF89D, 0xF8B1, 0xF8BA, 0xF8E3, 0xF8E6,
        0xF8EC, 0xF8EF, 0xF8F9, 0xF939, 0xF93F, 0xF950,
        0xF955, 0xF96D, 0xF993, 0xF995, 0xF998, 0xF9A5,
        0xF9B2, 0xF9BC, 0xF9BD, 0xF9C1, 0xF9C4, 0xF9CC,
        0xF9D1, 0xF9D4, 0xF9D9, 0xF9DA, 0xF9DE, 0xF9E2,
        0xF9E5, 0xF9E9, 0xF9EA, 0xF9FE};


word Hindex2[472] = {    // Ðe¸a ˆ•®
        29, 11, 24, 10, 20,  6,
        24, 20,  2,  4,  1, 17,
        12,  4,  7,  3,  3,  7,
        11,  6,  6, 45, 24, 39,
         7, 10,  3, 16,  1, 12,
         4, 17,  4, 13,  3,  9,
         4, 25, 54,  6,  6,  4,
         6, 10,  5,  6,  6,  9,
         7,  1,  7, 15,  1, 14,
         7,  4, 64,  1,  4,  1,
         1, 15,  8,  9,  2,  9,
         5,  7,  6,  1,  1,  3,
         4,  2, 18,  6,  1,  7,
         6,  1,  8,  1,  1,  2,
         2,  1,  6,  2,  2,  2,
        20,  5, 17,  5, 11, 16,
         1,  2, 40, 10, 10,  2,
        17, 11,  6,  1,  9,  9,
         9,  9,  2, 10,  3,  8,
         4,  1,  2, 13, 18,  7,
        12,  6,  5,  1, 18,  5,
        18,  7,  1,  7,  8, 12,
         1, 13, 14,  3,  6,  4,
         1,  2,  1,  6, 26,  9,
         5,  4,  8,  6, 19,  7,
        12, 14,  5,  6,  2, 11,
         2, 15,  1, 24,  7,  2,
         3, 12, 22,  2, 12,  3,
        19, 13,  3, 19, 25, 11,
        28, 20,  8, 10,  4,  9,
         2, 11,  7,  4, 17, 16,
        17,  1,  1, 16, 43,  1,
        19,  5,  6, 43, 14,  4,
        60,  4, 12,  5,  8,  4,
        31,  3,  5,  5, 30, 15,
        32, 13,  8,  4, 18,  9,
        37,  9,  6,  1,  8,  5,
         2, 61, 12, 27,  4,  3,
         3,  5, 10, 28, 15, 14,
         4, 10,  3,  1,  1, 18,
        14, 10,  4,  8,  4,  7,
        11,  7,  4, 11,  9, 31,
        10,  5,  6,  2,  6,  2,
         1, 24, 13, 43, 10, 15,
         4, 40, 24, 30,  5,  6,
         1,  9,  8, 18,  1,  5,
         4,  5, 38,  6, 24, 32,
         9, 13,  3,  2, 27,  3,
        25, 56,  7, 13,  5,  5,
         7,  1,  6,  3,  4, 19,
        38,  8, 24,  9, 11,  5,
         4, 26, 13,  5,  6,  1,
        38, 17,  4, 28, 25, 41,
         8,  9,  3, 55, 23, 46,
         4,  2,  3, 17,  5,  1,
        40,  2, 19,  1,  4,  1,
         1,  3, 11, 34,  5, 35,
        15,  2,  7,  3, 15,  7,
        15,  5, 10, 22, 12,  4,
         4, 15, 19, 10, 10, 10,
         8, 10, 27,  6,  4, 10,
         1,  3, 23, 12,  3,  3,
         6, 18,  5,  1, 24,  3,
         1,  3,  9,  1,  2,  1,
        14, 16, 10,  2,  4,  3,
         5, 14,  3,  1,  1,  4,
         7,  6,  6,  2,  1, 16,
         9,  3, 11,  4,  1, 10,
         1,  5, 10, 28,  6, 14,
         2,  5,  7, 10,  2, 14,
         5, 14,  2, 12,  7, 17,
        18,  2,  5,  9,  4,  4,
         1,  2,  4, 21,  4,  1,
        12, 20,  9, 41,  3,  6,
         3, 10, 14,  6, 17,  5,
        24, 20,  2,  3, 13, 13,
        10,  1,  4,  3,  8,  5,
         3,  5,  1,  4,  4,  3,
         4,  1, 20,  1};


word    HanjaArray[65];   // Hanja string
word    HanjaCount=0;

void   Swap(word *i)
{
    union {
      byte Half[2];
      word Full;
    } Code;
    byte temp;

    Code.Full=*i;
    temp=Code.Half[0];
    Code.Half[0]=Code.Half[1];
    Code.Half[1]=temp;
    *i=Code.Full;
}

void far HanjaPos(void)
{
        word Index;
        word i,j;

        union {
          byte Half[2];
          word Full;
        } Code;

        Code.Full=_DX;

        Code.Half[0]-= 0xE0;          // Ðe¸aÅ¡—a 0xE0¦Èá ¯¡¸b–A£a¡
                                // 188 Ðe¸a‹až“ˆe ˆe‰b·¡ 188‹i¸a
                                // Half[1]-48 ---> Ðe¸aÅ¡—aLOW Byteˆa 48¦Èá¯¡¸b
                                // Half[1]-66 ---> 112¦Èá 144ˆa §¡´á·¶·q

        Index=  Code.Half[0] * 188 + ((Code.Half[1] < 0x91) ? (Code.Half[1] - 48) : (Code.Half[1] - 66));
        if (Index>4888) Index=0;

        if (CodeC[Index]!=0) {
           i=FP_SEG(ImgCache[CodeC[Index]]);
           j=FP_OFF(ImgCache[CodeC[Index]]);
           _DX=i;
           _AX=j;
           return;
        }

        if (ImgMarker[CMarker]!=0)
          if (ImgMarker[CMarker]==0xffff) CodeC[0]=0;
          else CodeC[ImgMarker[CMarker]]=0;
        if (Index==0) ImgMarker[CMarker]=0xffff;
        else          ImgMarker[CMarker]=Index;
        CodeC[Index]=CMarker;


        if (lseek(HanjaFileHandler,Index*32L,SEEK_SET)==-1L ||
            _read(HanjaFileHandler,ImgCache[CMarker],32)==-1) {
             memset(ImgCache[CMarker],0,32);
        }

        if (CMarker==MaxHanjaCache-1) {
           CMarker=0;
           i=FP_SEG(ImgCache[MaxHanjaCache-1]);
           j=FP_OFF(ImgCache[MaxHanjaCache-1]);
           _DX=i;
           _AX=j;
        } else {
           i=FP_SEG( ImgCache[CMarker]);
           j=FP_OFF( ImgCache[CMarker]);
           CMarker++;
           _DX=i;
           _AX=j;
        }
}


typedef struct {
        unsigned char sbyte;
        unsigned char fbyte;
} tbyte;

union {
        tbyte   ch;
        word    wd;
} tword;



int     b_search(word key)
{
        int middle, left,right;

        left = 0;  right = 471;
        while (left <= right ){
                middle = right + left >> 1;
                if (key < Hantable[middle])
                        right = middle - 1;
                else if (key > Hantable[middle])
                        left = middle + 1;
                else    return(middle);
        }
        return (-1);
}


int     HanjaSearch(word Data)
{
        word i,step;
        int  index;

        Swap(&Data);

        if((index = b_search(Data)) == -1)  return Error;       // No Found

        tword.wd = Hindex1[index];
        HanjaCount = Hindex2[index];
        if(tword.ch.sbyte < 0x7e) {
                step=0;
                for(i=0;i<Hindex2[index];i++) {
                        tword.wd = Hindex1[index]+step+i;
                        if (tword.ch.sbyte >= 0x7e) step=18;
                        HanjaArray[i] = tword.wd;
                }
        } else {
                step=0;
                for(i=0;i<Hindex2[index];i++) {
                        tword.wd = Hindex1[index]+step+i;
                        if (tword.ch.sbyte >= 0xfe) {
                                step = 0x32;
                                HanjaArray[i] = tword.wd;
                                tword.ch.fbyte += 0x01;
                        } else
                                HanjaArray[i] = tword.wd;

                }
        }

        for (i=0 ; i<HanjaCount ; i++)
          Swap(&HanjaArray[i]);

        return OK;
}

word HangulSearch(word Data)
{
     word i,j;

     Swap(&Data);
     for (i=0 ; i<472 ; i++)
       if (Hindex1[i]>Data) break;
     i--;
     j=Hantable[i];
     Swap(&j);
     return j;
}


void SetHanjaBox(void)
{
    word i,j,k;

    for (i=k=0 ; i<9 ; i++)
      for (j=0 ; j<8 ; j++)
        if (k<HanjaCount)
           DrawHan(VideoRight/2-11+j*3,4+i,HanjaArray[k++]);
        else return;
}

word TranslateCode(byte *CodeP,word _X,word _Y)
{
    word X,Y,XBuf,YBuf,i;

    if ( *CodeP >=0xe0) {
       i=HangulSearch(*(word*)CodeP);
       if (i==Error) return Error;
       *(word*)CodeP=i;
       return OK;
    }
    if (HanjaFileHandler==-1) {
      HSayMes(" Ðe¸a ¸aÑw ¢ƒ·q·¡ º…§¡–A´á ·¶»¡ ´g¯s“¡”a.");
      return Error;
    }
    if (HanjaSearch(*(word*)CodeP)!=OK) {
      HSayMes(" ‹aœå ·q·i ˆa»¥ Ðe¸a“e ´ô¯s“¡”a.");
      return Error;
    }
    X=Y=XBuf=YBuf=0;
    Push(InverseF);
    HInverse();
    HSetInverse(_X-1,_Y-1,_X,_Y-1);
    HOpenWindow(VideoRight/2-12,3,VideoRight/2-12+24,5+HanjaCount/8);
    SetHanjaBox();
    Pop(InverseF);
    do {
       if (Y*8+X>=HanjaCount) {
         Bell();
         X=XBuf;
         Y=YBuf;
       }
       HanCur2(VideoRight/2-11+X*3,4+Y);
       GetKey();
       SetHotKeyF();
       if (HotKeyF) break;
       XBuf=X;
       YBuf=Y;
       switch (Way) {
         case CtrlS:
         case Left :if (X==0 ) {
                       if (Y==0) Y=8;
                       else      Y--;
                       X=7;
                    } else
                       X--;
                    break;
         case CtrlD:
         case Right:if (X==7) {
                       if (Y==8) Y=0;
                       else      Y++;
                       X=0;
                    }
                    else
                       X++;
                    break;
         case CtrlE:
         case Up   :if (Y==0) {
                       if (X==0) X=7;
                       else      X--;
                       Y=8;
                    } else
                       Y--;
                    break;
         case CtrlX:
         case Down :if (Y==8) {
                       if (X==7) X=0;
                       else      X++;
                       Y=0;
                    } else
                       Y++;
                    break;
         case Home :Y=X=0;
                    break;
         case End  :Y=8;
                    X=7;
                    break;
         case CtrlC:
         case PgDn :Y=8;
                    break;

         case CtrlR:
         case PgUp :Y=0;
         case Return:
                    if (Y*8+X<HanjaCount)
                       *(word*)CodeP=HanjaArray[Y*8+X];
                    else
                       Bell();

       }
    } while (Way!=Return && Way!=ESC && !HotKeyF);
    HCloseWindow();
    HSetInverse(_X-1,_Y-1,_X,_Y-1);
    if (Way==Return) return OK;
    else return Error;
}


void    EditF9()
{
    word X,Y,XBuf,YBuf,i;

    if (EditType[CurXPos()]==1 && EditBuf[CurXPos()]>=0xe0) {
       i=HangulSearch(*(word*)(EditBuf+CurXPos()));
       if (i==Error) return;
       *(word*)(EditBuf+CurXPos())=i;
       SetCurPos (CurXPos()+2,NowLine);
       return;
    }
    if (HanjaFileHandler==-1) {
      HSayMes(" Ðe¸a ¸aÑw ¢ƒ·q·¡ º…§¡–A´á ·¶»¡ ´g¯s“¡”a.");
      return;
    }
    if (EditType[CurXPos()]!=1) {
      Bell();
      return;
    }
    if (HanjaSearch(*(word*)(EditBuf+CurXPos()))!=OK) {
      HSayMes(" ‹aœå ·q·i ˆa»¥ Ðe¸a“e ´ô¯s“¡”a.");
      return;
    }
    X=Y=XBuf=YBuf=0;
    Push(InverseF);
    HInverse();
    HSetInverse(CurXPos()-1,SY-1,CurXPos(),SY-1);
    HOpenWindow(VideoRight/2-12,3,VideoRight/2-12+24,5+HanjaCount/8);
    SetHanjaBox();
    Pop(InverseF);
    do {
       if (Y*8+X>=HanjaCount) {
         Bell();
         X=XBuf;
         Y=YBuf;
       }
       HanCur2(VideoRight/2-11+X*3,4+Y);
       GetKey();
       SetHotKeyF();
       if (HotKeyF) break;
       XBuf=X;
       YBuf=Y;
       switch (Way) {
         case CtrlS:
         case Left :if (X==0 ) {
                       if (Y==0) Y=8;
                       else      Y--;
                       X=7;
                    } else
                       X--;
                    break;
         case CtrlD:
         case Right:if (X==7) {
                       if (Y==8) Y=0;
                       else      Y++;
                       X=0;
                    }
                    else
                       X++;
                    break;
         case CtrlE:
         case Up   :if (Y==0) {
                       if (X==0) X=7;
                       else      X--;
                       Y=8;
                    } else
                       Y--;
                    break;
         case CtrlX:
         case Down :if (Y==8) {
                       if (X==7) X=0;
                       else      X++;
                       Y=0;
                    } else
                       Y++;
                    break;
         case Home :Y=X=0;
                    break;
         case End  :Y=8;
                    X=7;
                    break;
         case CtrlC:
         case PgDn :Y=8;
                    break;

         case CtrlR:
         case PgUp :Y=0;
         case Return:
                    if (Y*8+X<HanjaCount) {
                       *(word*)(EditBuf+CurXPos())=HanjaArray[Y*8+X];
                       SetCurPos (CurXPos()+2,NowLine);
                       EditedF=True;
                       SetEditScn();
                    } else {
                       Bell();
                       Way=Error;
                    }
       }
    } while (Way!=Return && Way!=ESC && !HotKeyF);
    HCloseWindow();
    HSetInverse(CurXPos()-1,SY-1,CurXPos(),SY-1);
}

word InitHanja(void)
{
   ImgC=farmalloc(sizeof ImgCache);
   if (ImgC==NULL) return Error;
   return OK;
}


/*void SetHanjaBox(void)
{
    word i;

    HInverse();
    HClrBox(0,SY,VideoRight,SY);
    for (i=0 ; i<HanjaCount ; i++)
      DrawHan(i*2,SY,HanjaArray[i]);
    HInverse();
}


void    EditF9()
{
    word X;

    if (EditType[CurXPos()]!=1) {
      Bell();
      return;
    }
    if (HanjaSearch(*(word*)(EditBuf+CurXPos()))!=OK) {
      HSayMes(" »¡‹q Ðe‹iµA ´i x“e Ðe¸aˆa º…§¡–A´á ·¶»¡ ´g¯s“¡”a.");
      return;
    }
    X=0;
    Push(InverseF);
    HInverse();
    HSetInverse(CurXPos()-1,SY-1,CurXPos(),SY-1);
    HOpenWindowSimple(0,SY,VideoRight,SY);
    SetHanjaBox();
    Pop(InverseF);
    do {
       HanCur2(X*2,SY);
       GetKey();
       SetHotKeyF();
       if (HotKeyF) break;
       switch (Way) {
         case CtrlS:
         case Left :X--;
                    if (X>HanjaCount) X=HanjaCount-1;
                    break;
         case CtrlD:
         case Right:X++;
                    if (X==HanjaCount) X=0;
                    break;
         case PgUp :
         case CtrlR:
         case Home :X=0;;
                    break;
         case PgDn :
         case CtrlC:
         case End  :X=HanjaCount-1;
                    break;
         case Return:
                    *(word*)(EditBuf+CurXPos())=HanjaArray[X];
                    EditedF=True;
                    SetEditScn();
       }
    } while (Way!=Return && Way!=ESC && !HotKeyF);
    HCloseWindowSimple();
    HSetInverse(CurXPos()-1,SY-1,CurXPos(),SY-1);
}*/
