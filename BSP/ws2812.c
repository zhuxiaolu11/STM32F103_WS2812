#include "ws2812.h"

WS28xx_TypeStruct WS28xx;
void __show()
{
	HAL_TIM_PWM_Start_DMA(&WS28xx_PWM_hTIMER,WS28xx_PWM_Chaneel,(uint32_t *)(&WS28xx.WS28xx_Data),sizeof(WS28xx.WS28xx_Data));
}
	
//设置index的颜色
void __SetPixelColor_RGB(unsigned short int index,unsigned char r,unsigned char g,unsigned char b)
{
	unsigned char j;
	if(index > WS28xx.Pixel_size)
		return;
	for(j = 0; j < 8; j++)
	{		
		WS28xx.WS28xx_Data.ColorRealData [24 * index + j]        = (g & (0x80 >> j)) ? BIT_1 : BIT_0;  //G 将高位先发
		WS28xx.WS28xx_Data.ColorRealData [24 * index + j + 8]    = (r & (0x80 >> j)) ? BIT_1 : BIT_0;  //R将高位先发
		WS28xx.WS28xx_Data.ColorRealData [24 * index + j + 16]   = (b & (0x80 >> j)) ? BIT_1 : BIT_0;  //B将高位先发
	}
}
//获取某个位置的RGB
void __GetPixelColor_RGB(unsigned short int index,unsigned char *r,unsigned char *g,unsigned char *b)
{
	unsigned char j;
	*r=0;
	*g=0;
	*b=0;
	if(index > WS28xx.Pixel_size)
		return;
	for(j = 0; j <8; j++)
	{
		(*g)|=((WS28xx.WS28xx_Data.ColorRealData [24 * index + j]     >=BIT_1)? 0x80>>j:0); 	//G
		(*r)|=((WS28xx.WS28xx_Data.ColorRealData [24 * index + j + 8] >=BIT_1)? 0x80>>j:0);  	//R
		(*b)|=((WS28xx.WS28xx_Data.ColorRealData [24 * index + j + 16]>=BIT_1)? 0x80>>j:0);   //B
	}
}

void __SetPixelColor_From_RGB_Buffer( unsigned short int pixelIndex,unsigned char pRGB_Buffer[][3],unsigned short int DataCount)
{
	unsigned short int Index,j=0;
	for(Index=pixelIndex;Index < WS28xx.Pixel_size; Index++)
	{
			WS28xx.SetPixelColor_RGB(Index,pRGB_Buffer[j][0],pRGB_Buffer[j][1],pRGB_Buffer[j][2]);
			j++;
			if(j>DataCount)
				return;
	}
}

//设置所有颜色
void __SetALLColor_RGB(unsigned char r,unsigned char g,unsigned char b)
{
	unsigned short int Index;
	for(Index=0;Index < WS28xx.Pixel_size; Index++)
	{
		WS28xx.SetPixelColor_RGB(Index,r,g,b);
	}
}

void	WS28xx_TypeStructInit()
{
	WS28xx.Pixel_size=PIXEL_SIZE;
	WS28xx.GetPixelColor_RGB=__GetPixelColor_RGB;
	WS28xx.SetPixelColor_From_RGB_Buffer=__SetPixelColor_From_RGB_Buffer;
	WS28xx.SetPixelColor_RGB=__SetPixelColor_RGB;
	WS28xx.SetALLColor_RGB=__SetALLColor_RGB;
	WS28xx.show=__show;
}


