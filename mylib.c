//Zaeem Adamjee GBA
#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawImage3(int r, int c, int width, int height, const u16 *img)
{
  for (int i = 0; i < height; i++) {
    u16 position = c + 240*(r + i);
    DMA[3].src = img + width * i;
  	DMA[3].dst = videoBuffer + position;
    DMA[3].cnt = DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | width;
    }
}

void drawRect(int row, int col, int height, int width, volatile unsigned short color)
{
	for(int r=0; r<height; r++)
	{
		DMA[3].src =  &color;
		DMA[3].dst =  &videoBuffer[OFFSET(row+r, col, 240)];
		DMA[3].cnt =  DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | width;
	}
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void delay(int n)
{
	volatile int x = 0;
	for(int i=0; i<n*5000; i++)
	{
		x = x + 1;
	}
}

void fillScreen(volatile u16 color)
{
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240*160) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
}
