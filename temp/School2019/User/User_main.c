#include "main.h"
#include "spi.h"
#include "gpio.h"
#include "gui.h"

#include "User_adc.h"
#include "User_Uart.h"
#include "User_FFT.h"
#include "gui.h"


//TIM3 触发输出事件连接到ADC1 PA1端口控制采样速率 ADC1 PA1使用DMA2S0

/* Test Only */
float32_t aaa[2048];

void Systeminit(void); //初始化都放这里

//此处为真正的mian函数 自动生成部分请不要动
void User_main(void)
{
	int i=0;
	int LastX=0,LastY=100;
	Systeminit();
	HAL_Delay(500);
	while(1)
	{
		LCD_OUTPUT_FFT();
		LCD_OUTPUT_Float(0,0,"HELLO",i);
	}
	
}




void Systeminit(void)
{
	//用户ADC1+TIM3+DMA初始化
	User_AdcInit();
}

