/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "modbus.h"
#include "my_uart.h"
#include "KY5532.h"
#include "control.h"
#include "kalman.h"
#include "measure.h"
#include "usart.h"
#include "iir.h"

/* USER CODE END 0 */

TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim13;
TIM_HandleTypeDef htim14;

/* TIM7 init function */
void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 18000-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 2000-1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}
/* TIM10 init function */
void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 180-1;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 20000-1;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  if(lvbocanshu == 1)//25hz
  {
	  htim10.Instance = TIM10;
	  htim10.Init.Prescaler = 180-1;
	  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim10.Init.Period = 40000-1;
	  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
	  {
		Error_Handler();
	  }
  }
  else if(lvbocanshu == 2)//50hz
	{
	  htim10.Instance = TIM10;
	  htim10.Init.Prescaler = 180-1;
	  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim10.Init.Period = 20000-1;
	  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
	  {
		Error_Handler();
	  }
  }
  else if(lvbocanshu == 3)//100hz
	{
	  htim10.Instance = TIM10;
	  htim10.Init.Prescaler = 180-1;
	  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim10.Init.Period = 10000-1;
	  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
	  {
		Error_Handler();
	  }
  }
  else if(lvbocanshu == 4)//1500hz
	{
	  htim10.Instance = TIM10;
	  htim10.Init.Prescaler = 180-1;
	  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim10.Init.Period = 667-1;
	  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
	  {
		Error_Handler();
	  }
  }
  /* USER CODE END TIM10_Init 2 */

}
/* TIM11 init function */
void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 180-1;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 5000-1;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  if(lvbocanshu == 1)//25hz
  {
		htim11.Instance = TIM11;
		htim11.Init.Prescaler = 180-1;
		htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim11.Init.Period = 40000-1;
		htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
		{
		Error_Handler();
		}
		junzhi_num = 10;
  }
   else if(lvbocanshu == 2)//50hz
  {
		htim11.Instance = TIM11;
		htim11.Init.Prescaler = 180-1;
		htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim11.Init.Period = 20000-1;
		htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
		{
		Error_Handler();
		}
		junzhi_num = 20;
  }
  else if(lvbocanshu == 3)//100hz
  {
		htim11.Instance = TIM11;
		htim11.Init.Prescaler = 180-1;
		htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim11.Init.Period = 10000-1;
		htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
		{
		Error_Handler();
		}
		junzhi_num = 30;
  }
  else if(lvbocanshu == 4)//100hz
  {
		htim11.Instance = TIM11;
		htim11.Init.Prescaler = 180-1;
		htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim11.Init.Period = 10000-1;
		htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
		{
		Error_Handler();
		}
		junzhi_num = 30;
  }
  /* USER CODE END TIM11_Init 2 */

}
/* TIM13 init function */
void MX_TIM13_Init(void)
{

  /* USER CODE BEGIN TIM13_Init 0 */

  /* USER CODE END TIM13_Init 0 */

  /* USER CODE BEGIN TIM13_Init 1 */

  /* USER CODE END TIM13_Init 1 */
  htim13.Instance = TIM13;
  htim13.Init.Prescaler = 90-1;
  htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim13.Init.Period = 10-1;
  htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM13_Init 2 */
	//HAL_TIM_Base_Start_IT(&htim13);
  /* USER CODE END TIM13_Init 2 */

}
/* TIM14 init function */
void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 9000-1;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 1000-1;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM7)
  {
  /* USER CODE BEGIN TIM7_MspInit 0 */

  /* USER CODE END TIM7_MspInit 0 */
    /* TIM7 clock enable */
    __HAL_RCC_TIM7_CLK_ENABLE();

    /* TIM7 interrupt Init */
    HAL_NVIC_SetPriority(TIM7_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
  /* USER CODE BEGIN TIM7_MspInit 1 */

  /* USER CODE END TIM7_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM10)
  {
  /* USER CODE BEGIN TIM10_MspInit 0 */

  /* USER CODE END TIM10_MspInit 0 */
    /* TIM10 clock enable */
    __HAL_RCC_TIM10_CLK_ENABLE();

    /* TIM10 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  /* USER CODE BEGIN TIM10_MspInit 1 */

  /* USER CODE END TIM10_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM11)
  {
  /* USER CODE BEGIN TIM11_MspInit 0 */

  /* USER CODE END TIM11_MspInit 0 */
    /* TIM11 clock enable */
    __HAL_RCC_TIM11_CLK_ENABLE();

    /* TIM11 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
  /* USER CODE BEGIN TIM11_MspInit 1 */

  /* USER CODE END TIM11_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM13)
  {
  /* USER CODE BEGIN TIM13_MspInit 0 */

  /* USER CODE END TIM13_MspInit 0 */
    /* TIM13 clock enable */
    __HAL_RCC_TIM13_CLK_ENABLE();

    /* TIM13 interrupt Init */
    HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 0, 3);
    HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
  /* USER CODE BEGIN TIM13_MspInit 1 */

  /* USER CODE END TIM13_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM14)
  {
  /* USER CODE BEGIN TIM14_MspInit 0 */

  /* USER CODE END TIM14_MspInit 0 */
    /* TIM14 clock enable */
    __HAL_RCC_TIM14_CLK_ENABLE();

    /* TIM14 interrupt Init */
    HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  /* USER CODE BEGIN TIM14_MspInit 1 */

  /* USER CODE END TIM14_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM7)
  {
  /* USER CODE BEGIN TIM7_MspDeInit 0 */

  /* USER CODE END TIM7_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM7_CLK_DISABLE();

    /* TIM7 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM7_IRQn);
  /* USER CODE BEGIN TIM7_MspDeInit 1 */

  /* USER CODE END TIM7_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM10)
  {
  /* USER CODE BEGIN TIM10_MspDeInit 0 */

  /* USER CODE END TIM10_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM10_CLK_DISABLE();

    /* TIM10 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
  /* USER CODE BEGIN TIM10_MspDeInit 1 */

  /* USER CODE END TIM10_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM11)
  {
  /* USER CODE BEGIN TIM11_MspDeInit 0 */

  /* USER CODE END TIM11_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM11_CLK_DISABLE();

    /* TIM11 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
  /* USER CODE BEGIN TIM11_MspDeInit 1 */

  /* USER CODE END TIM11_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM13)
  {
  /* USER CODE BEGIN TIM13_MspDeInit 0 */

  /* USER CODE END TIM13_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM13_CLK_DISABLE();

    /* TIM13 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM8_UP_TIM13_IRQn);
  /* USER CODE BEGIN TIM13_MspDeInit 1 */

  /* USER CODE END TIM13_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM14)
  {
  /* USER CODE BEGIN TIM14_MspDeInit 0 */

  /* USER CODE END TIM14_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM14_CLK_DISABLE();

    /* TIM14 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  /* USER CODE BEGIN TIM14_MspDeInit 1 */

  /* USER CODE END TIM14_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


extern float32_t testOutput1[TEST_LENGTH_SAMPLES+3];               /* 滤波后的输出 */
extern float32_t testOutput2[TEST_LENGTH_SAMPLES+3];               /* 滤波后的输出 */
extern float32_t testOutput3[TEST_LENGTH_SAMPLES+3];               /* 滤波后的输出 */
extern float32_t testOutput4[TEST_LENGTH_SAMPLES+3];               /* 滤波后的输出 */
extern float32_t testOutput5[TEST_LENGTH_SAMPLES+3];               /* 滤波后的输出 */
extern float32_t testOutput6[TEST_LENGTH_SAMPLES+3];               /* 滤波后的输出 */

float32_t *outputF32_1,*outputF32_2,*outputF32_3,*outputF32_4,*outputF32_5,*outputF32_6;
int32_t   KY1_IIR_out_old,KY2_IIR_out_old,KY3_IIR_out_old,KY4_IIR_out_old,KY5_IIR_out_old,KY6_IIR_out_old;


float32_t AdcValueBuf[50],AdcValuetemp,AdcValuetemp_sum;
uint16_t count_i,count_j;
uint16_t mid_N = 10,mid_BAN = 2;

int32_t buf[1];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t TIM11_num = 0;
	outputF32_1 = &testOutput1[0];
	outputF32_2 = &testOutput2[0];
	outputF32_3 = &testOutput3[0];
	outputF32_4 = &testOutput4[0];
	outputF32_5 = &testOutput5[0];
	outputF32_6 = &testOutput6[0];
	if(htim->Instance == TIM10)				//200hz
	{
//		if(measure_flag == 0)
//		{
		
//		   buf[0]=ky1_value;
//			HAL_UART_Transmit(&huart1,(uint8_t*)buf,sizeof(buf),1000);//
		
//			control_ky5532_readADC(1);
//			printf("%d\r\n",ky1_value);
//		
//			kalman_filter(&KY1_Kalman,ky1_value);
		
			//control_ky5532_readADC(2);
			//kalman_filter(&KY2_Kalman,ky2_value);
			//control_ky5532_readADC(3);
			//kalman_filter(&KY3_Kalman,ky3_value);
			//control_ky5532_readADC(4);
			//kalman_filter(&KY4_Kalman,ky4_value);
			//control_ky5532_readADC(5);
			//kalman_filter(&KY5_Kalman,ky5_value);
			//control_ky5532_readADC(6);
			//kalman_filter(&KY6_Kalman,ky6_value);
			
			arm_biquad_cascade_df1_f32(&S1,&KY1_Kalman.x,outputF32_1,BLOCK_SIZE);
//			arm_biquad_cascade_df1_f32(&S2,&KY2_Kalman.x,outputF32_2,BLOCK_SIZE);
//			arm_biquad_cascade_df1_f32(&S3,&KY3_Kalman.x,outputF32_3,BLOCK_SIZE);
//			arm_biquad_cascade_df1_f32(&S4,&KY4_Kalman.x,outputF32_4,BLOCK_SIZE);
//			arm_biquad_cascade_df1_f32(&S5,&KY5_Kalman.x,outputF32_5,BLOCK_SIZE);
//			arm_biquad_cascade_df1_f32(&S6,&KY6_Kalman.x,outputF32_6,BLOCK_SIZE);
//			
			KY1_IIR_out = testOutput1[0]*ScaleValue;
//			KY2_IIR_out = testOutput2[0]*ScaleValue;
//			KY3_IIR_out = testOutput3[0]*ScaleValue;
//			KY4_IIR_out = testOutput4[0]*ScaleValue;
//			KY5_IIR_out = testOutput5[0]*ScaleValue;
//			KY6_IIR_out = testOutput6[0]*ScaleValue;		

			HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
//		}
//		else
//		{
//			
//		}
	}
	
	if(htim->Instance == TIM11)
	{
//		for( ;TIM11_num<mid_N;TIM11_num++)
//		{
//			AdcValueBuf[TIM11_num]=KY1_IIR_out;//通道采样值300个放入数组
//		}
//		if(TIM11_num == mid_N)
//		{
//			for(count_j=0;count_j<mid_N-1;count_j++)//冒泡法从小到大排序采集的数值
//			{
//				for(count_i=0;count_i<mid_N-count_j;count_i++)
//				 {
//					  if(AdcValueBuf[count_i]>AdcValueBuf[count_i+1])
//						{
//							AdcValuetemp=AdcValueBuf[count_i];
//							AdcValueBuf[count_i]=AdcValueBuf[count_i+1];
//							AdcValueBuf[count_i+1]=AdcValuetemp;
//						}
//				 }
//			}
//			for(count_j=mid_BAN;count_j<(mid_N-mid_BAN);count_j++)//中间100个数的平均值
//			{
//				AdcValuetemp_sum+=AdcValueBuf[count_j];
//			}
//			KY1_ave = AdcValuetemp_sum/(float32_t)(mid_N-mid_BAN*2);
//			AdcValuetemp_sum = 0;
//			TIM11_num = 0;
//		}
		
		if(TIM11_num < junzhi_num)
		{
			KY1_ACC += KY1_IIR_out;
			KY2_ACC += KY2_IIR_out;
			KY3_ACC += KY3_IIR_out;
			KY4_ACC += KY4_IIR_out;
			KY5_ACC += KY5_IIR_out;
			KY6_ACC += KY6_IIR_out;
			TIM11_num++;
		}
		else
		{
			TIM11_num = 0;
			KY1_ave = KY1_ACC/junzhi_num;
			KY2_ave = KY2_ACC/junzhi_num;
			KY3_ave = KY3_ACC/junzhi_num;
			KY4_ave = KY4_ACC/junzhi_num;
			KY5_ave = KY5_ACC/junzhi_num;
			KY6_ave = KY6_ACC/junzhi_num;
			KY1_ACC = 0;
			KY2_ACC = 0;
			KY3_ACC = 0;
			KY4_ACC = 0;
			KY5_ACC = 0;
			KY6_ACC = 0;	
		}
		

	}
	
	
	if(htim->Instance == TIM7)
	{
		modbus_service();
		#if printf_debug
		
//			printf("\r\n");		
			printf("%d\r\n",ky1_value);
//			printf("k2 = %8.2fg\r\n",ky2_value);
//			printf("k3 = %8.2fg\r\n",ky3_value);
//			printf("k4 = %8.2fg\r\n",ky4_value);
//			printf("k5 = %8.2fg\r\n",ky5_value);
//			printf("k6 = %8.2fg\r\n",KY6_printf);
//			printf("\r\n");
		
		#endif
	}
	
	if(htim->Instance == TIM13)//57600modus需要至少0.67ms--670us			//企业290us
	{
		modbus_time++;		//0.67ms
		if(modbus_time > 67)
		{
			HAL_TIM_Base_Stop_IT(&htim13);  //停止定时器的时候调用这个函数
			Flag_of_Modbus_Ok=1;
		}
	}
	if(htim->Instance == TIM14)
	{
		if(KY1_IIR_out_old != (int32_t)KY1_IIR_out/2000)//动态
		{
			KY1_runing = 1;
			KY1_IIR_out_old = (int32_t)KY1_IIR_out/2000;
		}
		else//稳态
		{
			KY1_runing = 0;
			KY1_IIR_out_old = (int32_t)KY1_IIR_out/2000;
		}
		
		if(KY2_IIR_out_old != (int32_t)KY2_IIR_out/2000)//动态
		{
			KY2_runing = 1;
			KY2_IIR_out_old = (int32_t)KY2_IIR_out/2000;
		}
		else//稳态
		{
			KY2_runing = 0;
			KY2_IIR_out_old = (int32_t)KY2_IIR_out/2000;
		}
		
		if(KY3_IIR_out_old != (int32_t)KY3_IIR_out/2000)//动态
		{
			KY3_runing = 1;
			KY3_IIR_out_old = (int32_t)KY3_IIR_out/2000;
		}
		else//稳态
		{
			KY3_runing = 0;
			KY3_IIR_out_old = (int32_t)KY3_IIR_out/2000;
		}
		
		if(KY4_IIR_out_old != (int32_t)KY4_IIR_out/2000)//动态
		{
			KY4_runing = 1;
			KY4_IIR_out_old = (int32_t)KY4_IIR_out/2000;
		}
		else//稳态
		{
			KY4_runing = 0;
			KY4_IIR_out_old = (int32_t)KY4_IIR_out/2000;
		}
		
		if(KY5_IIR_out_old != (int32_t)KY5_IIR_out/2000)//动态
		{
			KY5_runing = 1;
			KY5_IIR_out_old = (int32_t)KY5_IIR_out/2000;
		}
		else//稳态
		{
			KY5_runing = 0;
			KY5_IIR_out_old = (int32_t)KY5_IIR_out/2000;
		}
		
		if(KY6_IIR_out_old != (int32_t)KY6_IIR_out/2000)//动态
		{
			KY6_runing = 1;
			KY6_IIR_out_old = (int32_t)KY6_IIR_out/2000;
		}
		else//稳态
		{
			KY6_runing = 0;
			KY6_IIR_out_old = (int32_t)KY6_IIR_out/2000;
		}
		
		
		/*范围检测*/
		if(KY1_printf > 1000 || KY1_printf < -100) 	KY1_warning = 1;
		else										KY1_warning = 0;
		if(KY2_printf > 1000 || KY2_printf < -100)	KY2_warning = 1;
		else										KY2_warning = 0;
		if(KY3_printf > 1000 || KY3_printf < -100)	KY3_warning = 1;
		else										KY3_warning = 0;
		if(KY4_printf > 1000 || KY4_printf < -100)	KY4_warning = 1;
		else										KY4_warning = 0;
		if(KY5_printf > 1000 || KY5_printf < -100)	KY5_warning = 1;
		else										KY5_warning = 0;
		if(KY6_printf > 1000 || KY6_printf < -100)	KY6_warning = 1;
		else										KY6_warning = 0;
		
		
		/*去皮操作*/
		if(KY1_qvpi == 1)	{measure_value1 = KY1_ave;	KY1_qvpi = 0;}
		if(KY2_qvpi == 1)	{measure_value2 = KY2_ave;	KY2_qvpi = 0;}
		if(KY3_qvpi == 1)	{measure_value3 = KY3_ave;	KY3_qvpi = 0;}
		if(KY4_qvpi == 1)	{measure_value4 = KY4_ave;	KY4_qvpi = 0;}
		if(KY5_qvpi == 1)	{measure_value5 = KY5_ave;	KY5_qvpi = 0;}
		if(KY6_qvpi == 1)	{measure_value6 = KY6_ave;	KY6_qvpi = 0;}
		
		if(KY1_biaoding == 1)	{KY1_xishu = (KY1_ave - measure_value1)/20.0f;	KY1_biaoding = 0;}
		if(KY2_biaoding == 1)	{KY2_xishu = (KY2_ave - measure_value2)/20.0f;	KY2_biaoding = 0;}
		if(KY3_biaoding == 1)	{KY3_xishu = (KY3_ave - measure_value3)/20.0f;	KY3_biaoding = 0;}
		if(KY4_biaoding == 1)	{KY4_xishu = (KY4_ave - measure_value4)/20.0f;	KY4_biaoding = 0;}
		if(KY5_biaoding == 1)	{KY5_xishu = (KY5_ave - measure_value5)/20.0f;	KY5_biaoding = 0;}
		if(KY6_biaoding == 1)	{KY6_xishu = (KY6_ave - measure_value6)/20.0f;	KY6_biaoding = 0;}
	}
	
	

}
/* USER CODE END 1 */
