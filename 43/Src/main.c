/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rs485.h"	
#include "modbus.h"
#include "KY5532.h"
#include "protocol.h"
#include "my_uart.h"
#include "control.h"
#include "kalman.h"
#include "measure.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "iir.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void delay_us(__IO uint32_t delay);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern uint8_t Flag_of_Modbus_Ok;


uint8_t lvbocanshu =4;
						
uint16_t junzhi_num;

float32_t KY1_ACC,KY2_ACC,KY3_ACC,KY4_ACC,KY5_ACC,KY6_ACC;									/*累计值*/
float32_t KY1_ave,KY2_ave,KY3_ave,KY4_ave,KY5_ave,KY6_ave;									/*平均值*/
float32_t KY1_cal,KY2_cal,KY3_cal,KY4_cal,KY5_cal,KY6_cal;									/*克重值*/
int32_t   KY1_mod,KY2_mod,KY3_mod,KY4_mod,KY5_mod,KY6_mod;									/*寄存器值*/
float32_t KY1_printf,KY2_printf,KY3_printf,KY4_printf,KY5_printf,KY6_printf;				/*打印值*/
uint8_t   KY1_warning,KY2_warning,KY3_warning,KY4_warning,KY5_warning,KY6_warning;			/*超量程警告*/
uint8_t   KY1_runing,KY2_runing,KY3_runing,KY4_runing,KY5_runing,KY6_runing;				/*动态=1 与 稳态=0*/
uint8_t   KY1_qvpi,KY2_qvpi,KY3_qvpi,KY4_qvpi,KY5_qvpi,KY6_qvpi;							/*去皮标志位*/
uint8_t   KY1_biaoding,KY2_biaoding,KY3_biaoding,KY4_biaoding,KY5_biaoding,KY6_biaoding;	/*标定标志位*/
float32_t measure_value1,measure_value2,measure_value3,measure_value4,measure_value5,measure_value6;/*重量零位*/
float32_t KY1_xishu,KY2_xishu,KY3_xishu,KY4_xishu,KY5_xishu,KY6_xishu;						/*传感器系数*/


uint16_t stable_scope;	//稳定范围	0-100，单位d
uint16_t stable_time;	//稳定时间 	1-500，单位0.01秒
uint16_t dyna_scope;	//动态范围	0-100，单位d
uint16_t dyna_time;		//动态时间	1-50，单位0.1秒
uint16_t BaudRate_485;	//485波特率		0-6，0：2400，1：4800：2：9600，3：19200，4：38400，5：57600
uint16_t data_485;		//485数据格式	0-5，0：7E1， 1：7O1 ，2：8N1 ，3：8N2 ，4：8E1，5：8O1
uint16_t slave_address_485 = 0x02;//485从站地址		0-255

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

//	int32_t sum_iii = 0x11;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM13_Init();
  MX_TIM14_Init();
  MX_TIM11_Init();
  MX_TIM10_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Stop_IT(&htim13); //停止定时器的时候调用这个函数关闭;	//modbus_time		--100000khz--10us
/*初始化RS485*/
	HAL_GPIO_WritePin(RS485_RE_GPIO_Port,RS485_RE_Pin,GPIO_PIN_RESET);//设置为接收模式//高电平32发送低电平32接收
/*初始化ky5530*/	
	KY5530_ALL_Init();//!!!!!!!!!!!!!!
/*初始化kalman*/
/*
R值固定，Q值越大，代表越信任测量值，Q值无穷大，代表只用测量值。
Q值越小，代表越信任模型预测值，Q值为0，代表只用模型预测值。
Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
R:测量噪声，R增大，动态响应变慢，收敛稳定性变好
*/
	kalman_ALL_init();//!!!!!!!!!!!!!!
/*初始化上位机*/
	protocol_init();//在线调试初始化
	#if fire_debug
		SendPacket_PID(Channal01Address1Byte, KY1_Kalman.q ,KY1_Kalman.r, 1 ,USART1);
	#endif
/*需要一次计算*/
	control_ky5532_readADC(1);
	control_ky5532_readADC(2);
	control_ky5532_readADC(3);
	control_ky5532_readADC(4);
	control_ky5532_readADC(5);
	control_ky5532_readADC(6);
//	kalman_filter(&KY1_Kalman,ky1_value);
//	kalman_filter(&KY2_Kalman,ky2_value);
//	kalman_filter(&KY3_Kalman,ky3_value);
//	kalman_filter(&KY4_Kalman,ky4_value);
//	kalman_filter(&KY5_Kalman,ky5_value);
//	kalman_filter(&KY6_Kalman,ky6_value);
/*IIR滤波器初始化*/
	arm_iir_all_init();//!!!!!!!!!!!!!!
	HAL_Delay(10);
	HAL_TIM_Base_Start_IT(&htim10);//开启TIM10		200hz	一定要晚开启

	measure_flag = 1;
	HAL_Delay(500);
//	HAL_TIM_Base_Start_IT(&htim11);//开启TIM11	/*均值滤波*/	200hz
	HAL_TIM_Base_Start_IT(&htim7);//开启TIM7	/*调试*/	3hz				//uart_printf
//	HAL_TIM_Base_Start_IT(&htim14); //使用定时器的时候调用这个函数E;	//10hz
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  /*ky5530	1800hz
	tim10	1500hz
	printf	1000hz
  */
	float datas[5];
	uint8_t frameNameHead[] = "AABBCC";
	uint8_t frameNameEnd[] = "CCBBAA";
	
	//定义数据帧头帧尾
	uint8_t frameDataHead[] = "DDEEFF";
	uint8_t frameDataEnd[] = "FFEEDD";
	
	//定义通道名
	uint8_t name[] = {"ch1"};
	
	//赋值数据
	
	
	
	//通过串口1，向上位机发送数据

	float tare_weight1;
	float actual_weight1;
	float tare_weight2;
	float actual_weight2;
	float tare_weight3;
	float actual_weight3;
	float trans_coef;
	uint8_t tx[24] = {0};
	//uint32_t tare[1000] = {};
//	uint32_t sum=0;
//	for (int i = 0; i < 100; i++)
//	{
//		control_ky5532_readADC(1);
//		sum+=ky1_value;
//		printf("%d\r\n",ky1_value);
//	}
//	tare_weight=sum/100;
//	printf("%f\r\n",tare_weight);
	
		control_ky5532_readADC(1);
	  test_Kalman.x = ky1_value;
    test_Kalman.p = 1000;
    test_Kalman.A = 1;
    test_Kalman.H = 1;
    test_Kalman.q = 0.01f;
    test_Kalman.r = 800.0f;
//	while(1)
//	{
//		control_ky5532_readADC(1);
//		control_ky5532_readADC(2);
//		control_ky5532_readADC(3);
//		printf("1= %d\r\n",ky1_value);
//		printf("2= %d\r\n",ky2_value);
//		printf("3= %d\r\n",ky3_value);
//	}

	for (int i = 0; i < 7000; i++)
	{
		control_ky5532_readADC(1);
		kalman_filter(&test_Kalman,meanFilter2(ky1_value));
		if(i==6999)
		{
			tare_weight1=(float)kalman_filter(&test_Kalman,meanFilter2(ky1_value));
		}
	}
	
//  while(1)
//	{
//		control_ky5532_readADC(1);
////		control_ky5532_readADC(2);
////		control_ky5532_readADC(3);
////		actual_weight1=((float)kalman_filter(&test_Kalman,meanFilter2(ky1_value))-tare_weight1)/2834;
////		actual_weight=(ky1_value-tare_weight)/2834;
////		trans_coef=actual_weight/150;
////		printf("%f\r\n",actual_weight);
//		
////		  printf("\r\n");
////			printf("1 = %d\r\n",ky1_value);
////				control_ky5532_readADC(1);
//				
////				control_ky5532_readADC(1);
////				datas[0] = (float)ky1_value;//
////				datas[1] = (float)meanFilter(ky1_value);//
////				datas[2] = (float)kalman_filter(&KY1_Kalman,ky1_value);//
//////				datas[3] = update_akf(&akf, ky1_value, 1, 1);
//////				datas[3] = update_sage_husa(&filter,ky1_value,1,1);
////				datas[3] = (float)kalman_filter(&KY2_Kalman,meanFilter(ky1_value));//kalman avg
//////				datas[4] = (float)kalman_filter(&KY4_Kalman,kalman_filter(&KY3_Kalman,ky1_value));//kalman kalman
//////		    datas[4] = (float)meanFilter(kalman_filter(&KY2_Kalman,meanFilter(ky1_value)));//avg kalman avg
//		
////		  datas[0] = (float)ky1_value;//
////			datas[1] = (float)meanFilter(ky1_value);//
////			datas[2] = (float)meanFilter1(ky1_value);
////			datas[3] = (float)meanFilter2(ky1_value);
////			datas[4] = (float)meanFilter3(ky1_value);
//		
////		  datas[0] = (float)ky1_value;//
////			datas[1] = (float)kalman_filter(&test1_Kalman,ky1_value);
////			datas[2] = (float)kalman_filter(&test2_Kalman,ky1_value);
////			datas[3] = (float)kalman_filter(&test3_Kalman,ky1_value);
////			datas[4] = (float)kalman_filter(&test4_Kalman,ky1_value);

////			datas[0] = (float)ky1_value;//
////			datas[1] = (float)meanFilter2(ky1_value);
////			datas[2] = (float)kalman_filter(&KY1_Kalman,ky1_value);
////			datas[3] = (float)kalman_filter(&KY2_Kalman,meanFilter2(ky1_value));

//			datas[0] = ((float)kalman_filter(&test_Kalman,meanFilter2(ky1_value))-tare_weight1)/2834;
////			datas[1] = ((float)kalman_filter(&test_Kalman,meanFilter2(ky2_value))-tare_weight2)/2834;
////			datas[2] = ((float)kalman_filter(&test_Kalman,meanFilter2(ky3_value))-tare_weight3)/2834;
//			
//			HAL_UART_Transmit(&huart1,frameNameHead,sizeof(frameNameHead)-1,1000);//
//			HAL_UART_Transmit(&huart1,name,sizeof(name)-1,1000);//
//			HAL_UART_Transmit(&huart1,frameNameEnd,sizeof(frameNameEnd)-1,1000);//
//			
//			HAL_UART_Transmit(&huart1,frameDataHead,sizeof(frameDataHead)-1,1000);//
//			HAL_UART_Transmit(&huart1,(uint8_t*)datas,sizeof(datas),2000);//
//			HAL_UART_Transmit(&huart1,frameDataEnd,sizeof(frameDataEnd)-1,1000);//
//		
////	  	last=ky1_value;
//			
//		
////		  KY5532_Get_ADCVal_once(0, &ky1_value);
//		
////			control_ky5532_readADC(1);
////			printf("%d\r\n",ky1_value);
//			
//			
//			
//			
//			
//			
////			HAL_Delay(1);
////			control_ky5532_readADC(2);
////			printf("%d\r\n",ky2_value);
//	}

////while(1)
////{
////	for (int i = 0; i < 8; i++)
////	{
////		printf("%d\r\n",RS485_RX_BUF[i]);
////	}
////}

  while (1)
  {
			control_ky5532_readADC(1);
			actual_weight1=((float)kalman_filter(&test_Kalman,meanFilter2(ky1_value))-tare_weight1)/2836;
			printf("%f\r\n",actual_weight1);

		
	  modbus_virtual_register[0]  = (uint16_t)(KY1_mod&0x0000FFFF);
	  modbus_virtual_register[1]  = (KY1_mod&0xFFFF0000)>>16;
	  modbus_virtual_register[2]  = (uint16_t)(KY2_mod&0x0000FFFF);;
	  modbus_virtual_register[3]  = (KY2_mod&0xFFFF0000)>>16;
	  modbus_virtual_register[4]  = (uint16_t)(KY3_mod&0x0000FFFF);;
	  modbus_virtual_register[5]  = (KY3_mod&0xFFFF0000)>>16;
	  modbus_virtual_register[6]  = (uint16_t)(KY4_mod&0x0000FFFF);;
	  modbus_virtual_register[7]  = (KY4_mod&0xFFFF0000)>>16;
	  modbus_virtual_register[8]  = (KY1_runing << 13) | (0x3 << 8) | modbus_slave_addr;
	  modbus_virtual_register[9]  =  KY1_warning << 7;
	  modbus_virtual_register[10] = (KY2_runing << 13) | (0x3 << 8) | modbus_slave_addr;
	  modbus_virtual_register[11] =  KY2_warning << 7;
	  modbus_virtual_register[12] = (KY3_runing << 13) | (0x3 << 8) | modbus_slave_addr;
	  modbus_virtual_register[13] =  KY3_warning << 7;
	  modbus_virtual_register[14] = (KY4_runing << 13) | (0x3 << 8) | modbus_slave_addr;
	  modbus_virtual_register[15] =  KY4_warning << 7;
	  modbus_virtual_register[16] = (uint16_t)(KY5_mod&0x0000FFFF);;
	  modbus_virtual_register[17] = (KY5_mod&0xFFFF0000)>>16;
	  modbus_virtual_register[18] = (uint16_t)(KY6_mod&0x0000FFFF);;
	  modbus_virtual_register[19] = (KY6_mod&0xFFFF0000)>>16;
	  modbus_virtual_register[20] = (KY5_runing << 13) | (0x3 << 8) | modbus_slave_addr;
	  modbus_virtual_register[21] =  KY5_warning << 7;
	  modbus_virtual_register[22] = (KY6_runing << 13) | (0x3 << 8) | modbus_slave_addr;
	  modbus_virtual_register[23] =  KY6_warning << 7;
//	  if(modbus_virtual_register[510] == 0x1)//1号清零
//	  {
//		  KY1_qvpi = 1;
//		  modbus_virtual_register[510] = 0;
//	  }
//	  if(modbus_virtual_register[510] == 0x2)//1号去皮
//	  {
//		  
//		  modbus_virtual_register[510] = 0;
//	  }
//	  if(modbus_virtual_register[510] == 0x4)//1号清皮
//	  {
//		  modbus_virtual_register[510] = 0;
//	  }
//	  if(modbus_virtual_register[511] == 0x1)//1号零位标定
//	  {
//		  KY1_biaoding = 1;
//		  modbus_virtual_register[511] = 0;
//	  }
//	  modbus_virtual_register[512] = 0;
//	  if(modbus_virtual_register[513] != 0)
//	  {
//		  lvbocanshu = modbus_virtual_register[513];
//		  all_Init_gain();
//		  modbus_virtual_register[513] = 0;
//	  }
//	  
//	  
//	  
//	  if(modbus_virtual_register[514] == 0x1)//2号清零
//	  {
//		  KY2_qvpi = 1;
//		  modbus_virtual_register[514] = 0;
//	  }
//	  if(modbus_virtual_register[514] == 0x2)//2号去皮
//	  {
//		  
//		  modbus_virtual_register[514] = 0;
//	  }
//	  if(modbus_virtual_register[514] == 0x4)//2号清皮
//	  {
//		  modbus_virtual_register[514] = 0;
//	  }
//	  if(modbus_virtual_register[515] == 0x1)//2号零位标定
//	  {
//		  KY2_biaoding = 1;
//		  modbus_virtual_register[515] = 0;
//	  }
//	  modbus_virtual_register[516] = 0;
//	  if(modbus_virtual_register[517] != 0)
//	  {
//		  lvbocanshu = modbus_virtual_register[517];
//		  all_Init_gain();
//		  modbus_virtual_register[517] = 0;
//	  }
//	  
//	  
//	  if(modbus_virtual_register[518] == 0x1)//3号清零
//	  {
//		  KY3_qvpi = 1;
//		  modbus_virtual_register[518] = 0;
//	  }
//	  if(modbus_virtual_register[518] == 0x2)//3号去皮
//	  {
//		  
//		  modbus_virtual_register[518] = 0;
//	  }
//	  if(modbus_virtual_register[518] == 0x4)//3号清皮
//	  {
//		  modbus_virtual_register[518] = 0;
//	  }
//	  if(modbus_virtual_register[519] == 0x1)//3号零位标定
//	  {
//		  KY3_biaoding = 1;
//		  modbus_virtual_register[519] = 0;
//	  }
//	  modbus_virtual_register[520] = 0;
//	  if(modbus_virtual_register[521] != 0)
//	  {
//		  lvbocanshu = modbus_virtual_register[521];
//		  all_Init_gain();
//		  modbus_virtual_register[521] = 0;
//	  }
//	  
//	  
//	  if(modbus_virtual_register[522] == 0x1)//4号清零
//	  {
//		  KY4_qvpi = 1;
//		  modbus_virtual_register[522] = 0;
//	  }
//	  if(modbus_virtual_register[522] == 0x2)//4号去皮
//	  {
//		  
//		  modbus_virtual_register[522] = 0;
//	  }
//	  if(modbus_virtual_register[522] == 0x4)//4号清皮
//	  {
//		  modbus_virtual_register[522] = 0;
//	  }
//	  if(modbus_virtual_register[523] == 0x1)//4号零位标定
//	  {
//		  KY4_biaoding = 1;
//		  modbus_virtual_register[523] = 0;
//	  }
//	  modbus_virtual_register[524] = 0;
//	  if(modbus_virtual_register[525] != 0)
//	  {
//		  lvbocanshu = modbus_virtual_register[525];
//		  all_Init_gain();
//		  modbus_virtual_register[525] = 0;
//	  }
//	  
//	  
//	  if(modbus_virtual_register[526] == 0x1)//5号清零
//	  {
//		  KY5_qvpi = 1;
//		  modbus_virtual_register[526] = 0;
//	  }
//	  if(modbus_virtual_register[526] == 0x2)//5号去皮
//	  {
//		  
//		  modbus_virtual_register[526] = 0;
//	  }
//	  if(modbus_virtual_register[526] == 0x4)//5号清皮
//	  {
//		  modbus_virtual_register[526] = 0;
//	  }
//	  if(modbus_virtual_register[527] == 0x1)//5号零位标定
//	  {
//		  KY5_biaoding = 1;
//		  modbus_virtual_register[527] = 0;
//	  }
//	  modbus_virtual_register[528] = 0;
//	  if(modbus_virtual_register[529] != 0)
//	  {
//		  lvbocanshu = modbus_virtual_register[529];
//		  all_Init_gain();
//		  modbus_virtual_register[529] = 0;
//	  }
//	  
//	  
//	  if(modbus_virtual_register[530] == 0x1)//6号清零
//	  {
//		  KY6_qvpi = 1;
//		  modbus_virtual_register[530] = 0;
//	  }
//	  if(modbus_virtual_register[530] == 0x2)//6号去皮
//	  {
//		  
//		  modbus_virtual_register[530] = 0;
//	  }
//	  if(modbus_virtual_register[530] == 0x4)//6号清皮
//	  {
//		  modbus_virtual_register[530] = 0;
//	  }
//	  if(modbus_virtual_register[531] == 0x1)//6号零位标定
//	  {
//		  KY6_biaoding = 1;
//		  modbus_virtual_register[531] = 0;
//	  }
//	  modbus_virtual_register[532] = 0;
//	  if(modbus_virtual_register[533] != 0)
//	  {
//		  lvbocanshu = modbus_virtual_register[533];
//		  all_Init_gain();
//		  modbus_virtual_register[533] = 0;
//	  }
//	  
//	   
//	  stable_scope = modbus_virtual_register[536];		//稳定范围	0-100，单位d
//	  stable_time  = modbus_virtual_register[537];		//稳定时间 	1-500，单位0.01秒
//	  stable_time  = modbus_virtual_register[538];		//动态范围	0-100，单位d;
//	  dyna_time    = modbus_virtual_register[539];		//动态时间	1-50，单位0.1秒
//	  BaudRate_485 = modbus_virtual_register[540];		//485波特率		0-6，0：2400，1：4800：2：9600，3：19200，4：38400，5：57600
//	  data_485 	   = modbus_virtual_register[541];		//485数据格式	0-5，0：7E1， 1：7O1 ，2：8N1 ，3：8N2 ，4：8E1，5：8O1
//	  slave_address_485 = modbus_virtual_register[542];	//485从站地址		0-255
////		printf("stable_scope =%d\r\n",BaudRate_485);
//	  if(slave_address_485 != modbus_slave_addr)
//	  {
//		  modbus_slave_addr = slave_address_485;//更新完成
//		  //!!!添加保存eeprom
//	  }

	  
	  
		HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
		
//		KY1_cal = (((KY1_ave)-measure_value1)/KY1_xishu);
			KY1_cal = actual_weight1;
//		KY2_cal = (((KY2_ave)-measure_value2)/KY2_xishu);
//		KY3_cal = (((KY3_ave)-measure_value3)/KY3_xishu);
//		KY4_cal = (((KY4_ave)-measure_value4)/KY4_xishu);
//		KY5_cal = (((KY5_ave)-measure_value5)/KY5_xishu);
//		KY6_cal = (((KY6_ave)-measure_value6)/KY6_xishu);  
//		if(fabs(KY1_cal) < 0.1)	KY1_printf = KY1_cal*0.3f;
//		else						KY1_printf = KY1_cal;
//		if(fabs(KY2_cal) < 0.1)	KY2_printf = KY2_cal*0.3f;
//		else						KY2_printf = KY2_cal;
//		if(fabs(KY3_cal) < 0.1)	KY3_printf = KY3_cal*0.3f;
//		else						KY3_printf = KY3_cal;
//		if(fabs(KY4_cal) < 0.1)	KY4_printf = KY4_cal*0.3f;
//		else						KY4_printf = KY4_cal;
//		if(fabs(KY5_cal) < 0.1)	KY5_printf = KY5_cal*0.3f;
//		else						KY5_printf = KY5_cal;
//		if(fabs(KY6_cal) < 0.1)	KY6_printf = KY6_cal*0.3f;
//		else						KY6_printf = KY6_cal;
		KY1_printf = KY1_cal;
		KY2_printf = KY2_cal;
		KY3_printf = KY3_cal;
		KY4_printf = KY4_cal;
		KY5_printf = KY5_cal;
    KY6_printf = KY6_cal;
		
		KY1_mod = (int32_t)(KY1_printf*1000);
		KY2_mod = (int32_t)(KY2_printf*1000);
		KY3_mod = (int32_t)(KY3_printf*1000);
		KY4_mod = (int32_t)(KY4_printf*1000);
		KY5_mod = (int32_t)(KY5_printf*1000);
		KY6_mod = (int32_t)(KY6_printf*1000);
//		modbus_03_function();
//modbus_service();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  
	#if printf_debug
	  
	  if(measure_flag == 1)
	  {  
			measure_value1 = KY1_ave;
			measure_value2 = KY2_ave;
			measure_value3 = KY3_ave;
			measure_value4 = KY4_ave;
			measure_value5 = KY5_ave;
			measure_value6 = KY6_ave;
		  
			//发送基准值
			printf("\r\n");
			printf("1 = %8.2f\r\n",measure_value1);
			printf("2 = %8.2f\r\n",measure_value2);
			printf("3 = %8.2f\r\n",measure_value3);
			printf("4 = %8.2f\r\n",measure_value4);
			printf("5 = %8.2f\r\n",measure_value5);
			printf("6 = %8.2f\r\n",measure_value6);
			printf("\r\n");
			measure_flag = 0;
	  }
	  else
	  {  			  
		  
	  }
	  
	  #endif
	  
	#if fire_debug
		receiving_process();
	#endif
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
#define CPU_FREQUENCY_MHZ    180		// STM32滴答定时器的时钟主频
void delay_us(__IO uint32_t delay)
{
    int last, curr, val;
    int temp;

    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}

void all_Init_gain(void)
{
	HAL_TIM_Base_Stop_IT(&htim10);
	HAL_TIM_Base_Stop_IT(&htim11);
	MX_TIM11_Init();//!!!!!!!!!!!!!!
	MX_TIM10_Init();//!!!!!!!!!!!!!!
	KY5530_ALL_Init();//!!!!!!!!!!!!!!
	kalman_ALL_init();//!!!!!!!!!!!!!!
	arm_iir_all_init();//!!!!!!!!!!!!!!
	HAL_TIM_Base_Start_IT(&htim10);
	HAL_TIM_Base_Start_IT(&htim11);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
