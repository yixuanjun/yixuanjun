/*
* 本工程FFT相关函数 变量的实现都在这个文件
*/

#include "User_FFT.h"
#include "User_adc.h"
#include "arm_math.h"

/*
* 该函数会从ADC以阻塞模式 2048Hz采样率采2048个点 然后进行rFFT 最终输出每个点的模长
*/
void User_FastRfft2048BlokingMode(float32_t* MagOutPosition)
{
	//ADC采样结果存放在这里
	uint16_t adc_input[2048] = {0};
	//2048个实部的输入数组
	float32_t realval[2048] = {0};
	float32_t fftoutput[2048] = {0};
	//快速FFT表结构体
	arm_rfft_fast_instance_f32 S;
	
	//2048输入点2048Hz rFFT查表结构体初始化
	arm_rfft_2048_fast_init_f32(&S);
	
	User_AdcStartBlokingMode(adc_input,2048);
	
	//将ADC采集到的数据从DMA缓冲区取到准备FFT的位置
	for(uint16_t input = 0;input < 2048;input ++)
	{
		realval[input] = (3.3 * (float)adc_input[input]) / (float)4095;
	}
	
	//FFT运算函数 最后的 0 代表是正fft
	arm_rfft_fast_f32(&S,realval,fftoutput,0);
	//求模长
	arm_cmplx_mag_f32(fftoutput,MagOutPosition,1024);
	
	
}

