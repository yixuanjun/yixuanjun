/**
  ******************************************************************************
  * File Name          : SPI.c
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();
  
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void SPI_WriteByte(uint8_t data)
{
	//return HAL_SPI_Transmit(&hspi1,&data,1,3);
	while(((hspi1.Instance->SR)&SPI_FLAG_TXE)==RESET);	//等待发送区空	  
	(hspi1.Instance->DR)=data;	 												//发送一个byte   
	while(((hspi1.Instance->SR)&SPI_FLAG_TXE)==RESET);	//等待接收完一个byte  	
}

void LCD_SPI_Init()
{
	uint8_t temp=0xFF;
	LCD_RST_SET;
	HAL_Delay(50);
	HAL_SPI_Transmit(&hspi1,&temp,1,3);
}

/****************************************************************************
* 名    称：Lcd_WriteIndex(u8 Index)
* 功    能：向液晶屏写一个8位指令
* 入口参数：Index   寄存器地址
* 出口参数：无
* 说    明：调用前需先选中控制器，内部函数
****************************************************************************/
void Lcd_WriteIndex(uint8_t Index)
{
   LCD_CS_CLR;
   LCD_DC_CLR; 
   SPI_WriteByte(Index);
   LCD_CS_SET;
}

/****************************************************************************
* 名    称：Lcd_WriteData(u8 Data)
* 功    能：向液晶屏写一个8位数据
* 入口参数：dat     寄存器数据
* 出口参数：无
* 说    明：向控制器指定地址写入数据，内部函数
****************************************************************************/
void Lcd_WriteData(uint8_t Data)
{
   LCD_CS_CLR;
   LCD_DC_SET; 
   SPI_WriteByte(Data);
   LCD_CS_SET;
}

/****************************************************************************
* 名    称：void LCD_WriteReg(u8 Index,u16 Data)
* 功    能：写寄存器数据
* 入口参数：Index,Data
* 出口参数：无
* 说    明：本函数为组合函数，向Index地址的寄存器写入Data值
****************************************************************************/
void LCD_WriteReg(uint8_t Index,uint16_t Data)
{
	Lcd_WriteIndex(Index);
	Lcd_WriteData_16Bit(Data);
}

/****************************************************************************
* 名    称：void Lcd_WriteData_16Bit(u16 Data)
* 功    能：向液晶屏写一个16位数据
* 入口参数：Data
* 出口参数：无
* 说    明：向控制器指定地址写入一个16位数据
****************************************************************************/
void Lcd_WriteData_16Bit(uint16_t Data)
{	
	Lcd_WriteData(Data>>8);
	Lcd_WriteData(Data);	
}

/****************************************************************************
* 名    称：void Lcd_Reset(void)
* 功    能：液晶硬复位函数
* 入口参数：无
* 出口参数：无
* 说    明：液晶初始化前需执行一次复位操作
****************************************************************************/
void Lcd_Reset(void)
{
	LCD_RST_CLR;
	HAL_Delay(100);
	LCD_RST_SET;
	HAL_Delay(50);
}
/****************************************************************************
* 名    称：void Lcd_Init(void)
* 功    能：液晶初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：液晶初始化_ILI9225_176X220
****************************************************************************/
void Lcd_Init(void)
{	
	LCD_SPI_Init();	
	Lcd_Reset(); 							//Reset before LCD Init.
	Lcd_WriteIndex(0xCB);  
	Lcd_WriteData(0x39); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x34); 
	Lcd_WriteData(0x02); 

	Lcd_WriteIndex(0xCF);  
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0XC1); 
	Lcd_WriteData(0X30); 
 
	Lcd_WriteIndex(0xE8);  
	Lcd_WriteData(0x85); 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x78); 
 
	Lcd_WriteIndex(0xEA);  
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x00); 
 
	Lcd_WriteIndex(0xED);  
	Lcd_WriteData(0x64); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0X12); 
	Lcd_WriteData(0X81); 

	Lcd_WriteIndex(0xF7);  
	Lcd_WriteData(0x20); 
  
	Lcd_WriteIndex(0xC0);			//Power control 
	Lcd_WriteData(0x23);			//VRH[5:0] 
 
	Lcd_WriteIndex(0xC1);    	//Power control 
	Lcd_WriteData(0x10);   		//SAP[2:0];BT[3:0] 
 
	Lcd_WriteIndex(0xC5);    	//VCM control 
	Lcd_WriteData(0x3e); 			//对比度调节
	Lcd_WriteData(0x28); 
 
	Lcd_WriteIndex(0xC7);    	//VCM control2 
	Lcd_WriteData(0x86);  		//--
 
	Lcd_WriteIndex(0x36);    	// Memory Access Control 
#ifdef USE_HORIZONTAL
	Lcd_WriteData(0xE8); 			//C8	   //48 68竖屏//28 E8 横屏
#else
	Lcd_WriteData(0x48); 
#endif

	Lcd_WriteIndex(0x3A);    
	Lcd_WriteData(0x55); 

	Lcd_WriteIndex(0xB1);    
	Lcd_WriteData(0x00);  
	Lcd_WriteData(0x18); 
 
	Lcd_WriteIndex(0xB6);    // Display Function Control 
	Lcd_WriteData(0x08); 
	Lcd_WriteData(0x82);
	Lcd_WriteData(0x27);  
 
	Lcd_WriteIndex(0xF2);    // 3Gamma Function Disable 
	Lcd_WriteData(0x00); 
 
	Lcd_WriteIndex(0x26);    //Gamma curve selected 
	Lcd_WriteData(0x01); 
 
	Lcd_WriteIndex(0xE0);    //Set Gamma 
	Lcd_WriteData(0x0F); 
	Lcd_WriteData(0x31); 
	Lcd_WriteData(0x2B); 
	Lcd_WriteData(0x0C); 
	Lcd_WriteData(0x0E); 
	Lcd_WriteData(0x08); 
	Lcd_WriteData(0x4E); 
	Lcd_WriteData(0xF1); 
	Lcd_WriteData(0x37); 
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x10); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0x0E); 
	Lcd_WriteData(0x09); 
	Lcd_WriteData(0x00); 
	
	Lcd_WriteIndex(0XE1);    //Set Gamma 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x0E); 
	Lcd_WriteData(0x14); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0x11); 
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x31); 
	Lcd_WriteData(0xC1); 
	Lcd_WriteData(0x48); 
	Lcd_WriteData(0x08); 
	Lcd_WriteData(0x0F); 
	Lcd_WriteData(0x0C); 
	Lcd_WriteData(0x31); 
	Lcd_WriteData(0x36); 
	Lcd_WriteData(0x0F); 
 
	Lcd_WriteIndex(0x11);    //Exit Sleep 
	HAL_Delay(120); 
				
	Lcd_WriteIndex(0x29);    //Display on 
	Lcd_WriteIndex(0x2c); 
	
}

/*************************************************
函数名：LCD_Set_XY
功能：设置lcd显示起始点
入口参数：xy坐标
返回值：无
*************************************************/
void Lcd_SetXY(uint16_t Xpos, uint16_t Ypos)
{	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData_16Bit(Xpos);
	Lcd_WriteIndex(0x2b);
	Lcd_WriteData_16Bit(Ypos);
	Lcd_WriteIndex(0x2c);	
} 
/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
//设置显示窗口
void Lcd_SetRegion(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData_16Bit(xStar);
	Lcd_WriteData_16Bit(xEnd);
	Lcd_WriteIndex(0x2b);
	Lcd_WriteData_16Bit(yStar);
	Lcd_WriteData_16Bit(yEnd);
	Lcd_WriteIndex(0x2c);
}

	
/*************************************************
函数名：LCD_DrawPoint
功能：画一个点
入口参数：xy坐标和颜色数据
返回值：无
*************************************************/
void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Data)
{
	Lcd_SetXY(x,y);
	Lcd_WriteData_16Bit(Data);

}    

/*************************************************
函数名：Lcd_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void Lcd_Clear(uint16_t Color)               
{	
   int32_t i,m;
   Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {	
	  	Lcd_WriteData_16Bit(Color);
    }   
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
