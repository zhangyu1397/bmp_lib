#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"
#include "unistd.h"
#include "fcntl.h"
#include "stdint.h"


void main(int argc, char* argv[])
{
    struct stat sbuf;
    char* bmpSource;
    tBmpBitMapFileInfoHeader* bmpFileHeadInfo;
    int32_t  offset = 0;
    if(argc < 2)
    {
        Blog("no bmp file path\n");
        return;
    }

    int bmpFd = open(argv[1], O_RDONLY);
    int tagFd = open("target.bmp", O_CREAT | O_RDWR);

    if (fstat(bmpFd, &sbuf) < 0) 
    {
        Blog("stat error");
    }

    Blog("%s file size is %d\n",(char*)argv[1], (int)sbuf.st_size);
    if(!(bmpSource = malloc(sbuf.st_size)))
    {
        Blog("malloc error\n");
        goto err1;
    }

    if(sbuf.st_size != read(bmpFd, bmpSource, sbuf.st_size))
    {
        Blog("read failed\n");
        goto err2;
    }

    if(!(bmpFileHeadInfo = malloc(sizeof(tBmpBitMapFileInfoHeader))))
    {
        Blog("malloc bmpFileHeadInfo error\n");
        goto err2;
    }

    if(ERROR == getBmpFileHeadInfo(bmpFileHeadInfo, bmpSource, sbuf.st_size))
    {
        Blog("getBmpHead error\n");
        goto err3;
    }

    if(ERROR == setBmpDataReversal(bmpFileHeadInfo, bmpSource + sizeof(tBmpBitMapFileInfoHeader), 1, 1))
    {
        Blog("getBmpHead error\n");
        goto err3;
    }

    write(tagFd, (char*)bmpFileHeadInfo, sizeof(tBmpBitMapFileInfoHeader));
    write(tagFd, bmpSource + sizeof(tBmpBitMapFileInfoHeader),  sbuf.st_size - sizeof(tBmpBitMapFileInfoHeader));
    close(tagFd);

    Blog("bmp picture size  = %dk\n", bmpFileHeadInfo->bmpFileHeader.bfSize/1024);
    Blog("bmp info offset len  = %d\n", bmpFileHeadInfo->bmpFileHeader.bfOffBits);
    Blog("bmp width  = %d\n", bmpFileHeadInfo->bmpInfoHeader.biWidth);
    Blog("bmp height  = %d\n", bmpFileHeadInfo->bmpInfoHeader.biHeight);
    Blog("bmp compression type  = %s\n", bmpFileHeadInfo->bmpInfoHeader.biCompression == 0 ? "no compression" : "unsupported compression type");
    Blog("bmp bitCount  = %d\n", bmpFileHeadInfo->bmpInfoHeader.biPixelBitCount);
    
err3:
    free(bmpFileHeadInfo);
err2:
    free(bmpSource);
err1: 
    close(bmpFd);
}
