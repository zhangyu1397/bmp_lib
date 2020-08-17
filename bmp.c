#include "bmp.h"
#include <string.h>
#include <stdio.h>
#include "fcntl.h"
#include "unistd.h"
#include <stdlib.h>

eStatus getBmpFileHeadInfo(tBmpBitMapFileInfoHeader* bmpFileHeadInfo, char* source, int32_t sourceLen)
{
    uint32_t offset = 0;
    if(NULL == bmpFileHeadInfo || sourceLen < sizeof(tBmpBitMapFileInfoHeader))
    {
        Blog("%s: file error\n",__func__);
        return ERROR;
    }

    memcpy(&bmpFileHeadInfo->bmpFileHeader, source, sizeof(tBmpBitMapFileHeader));
    offset += sizeof(tBmpBitMapFileHeader);

    memcpy(&bmpFileHeadInfo->bmpInfoHeader, source + offset, sizeof(tBmpBitMapInfoHeader));

    return OK;
}

eStatus setBmpDataReversal(tBmpBitMapFileInfoHeader* bmpFileHeadInfo, char* source, int32_t xReversal, int32_t yReversal)
{
    char* tmpSource;
    uint32_t xTotalPixelSize;
    uint32_t pixelByte;
    uint32_t pixelCount;
    int32_t y,x;

    if(NULL == bmpFileHeadInfo || NULL == source)
    {
        Blog("setBmpDataReversal param error\n");
        return ERROR;
    }
    pixelByte = bmpFileHeadInfo->bmpInfoHeader.biPixelBitCount / 8;
    pixelCount = bmpFileHeadInfo->bmpInfoHeader.biHeight * bmpFileHeadInfo->bmpInfoHeader.biWidth * pixelByte;
    
    xTotalPixelSize = bmpFileHeadInfo->bmpInfoHeader.biWidth * pixelByte; 
    tmpSource = malloc(pixelCount);
    if(NULL == tmpSource)
    {
        Blog("%s malloc failed\n",__func__);
        return ERROR;
    }

    memset(tmpSource, 0, pixelCount);

    for(y = bmpFileHeadInfo->bmpInfoHeader.biHeight - 1; y >= 0; y--)
    {
        int heightOffset = yReversal ? bmpFileHeadInfo->bmpInfoHeader.biHeight - y - 1 : y;

        for(x = 0; x < bmpFileHeadInfo->bmpInfoHeader.biWidth; x++)
        {
            int widthOffset = xReversal ? bmpFileHeadInfo->bmpInfoHeader.biWidth - x - 1 : x;
            memcpy(tmpSource + heightOffset * xTotalPixelSize + pixelByte * widthOffset, source + y * xTotalPixelSize + x * pixelByte, pixelByte);
        }
        
    }

    memcpy(source, tmpSource, pixelCount);
    free(tmpSource);
    return OK;
}


