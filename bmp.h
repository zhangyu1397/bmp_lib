#ifndef __BMP_H__
#define __BMP_H__
#include <stdio.h>
#include "stdint.h"

#define useLog

#ifdef useLog
#define Blog(args...) printf(args)
#else
#define Blog(args...) do{}while(0)
#endif

#pragma pack (1)
typedef struct bmpBitMapFileHeader
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
}tBmpBitMapFileHeader;

typedef struct bmpBitMapInfoHeader
{
    uint32_t biSize;
    uint32_t biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biPixelBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} tBmpBitMapInfoHeader;


typedef struct bmpBitMapFileInfoHeader
{
    tBmpBitMapFileHeader bmpFileHeader;
    tBmpBitMapInfoHeader bmpInfoHeader;
}tBmpBitMapFileInfoHeader;


typedef enum
{
    ERROR = 0,
    OK
}eStatus;

eStatus getBmpFileHeadInfo(tBmpBitMapFileInfoHeader* bmpFileHeadInfo, char* source, int32_t sourceLen);
eStatus setBmpDataReversal(tBmpBitMapFileInfoHeader* bmpFileHeadInfo, char* source, int32_t xReversal, int32_t yReversal);

//eStatus getBmpHead(tBmpBitMapFileHeader* bmpFileHead, char* source, int32_t sourceLen, uint32_t* offset);
//eStatus getBmpInfo(tBmpBitMapInfoHeader* bmpInfoHead, char* source, int32_t sourceLen, uint32_t* offset);

#endif