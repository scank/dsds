/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "arm_math.h"


#define fire_debug 0
#define printf_debug 0


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

extern uint8_t lvbocanshu;
extern uint16_t junzhi_num;

extern float32_t KY1_ACC,KY2_ACC,KY3_ACC,KY4_ACC,KY5_ACC,KY6_ACC;							/*累计值*/
extern float32_t KY1_ave,KY2_ave,KY3_ave,KY4_ave,KY5_ave,KY6_ave;							/*平均值*/
extern float32_t KY1_cal,KY2_cal,KY3_cal,KY4_cal,KY5_cal,KY6_cal;							/*克重值*/
extern int32_t   KY1_mod,KY2_mod,KY3_mod,KY4_mod,KY5_mod,KY6_mod;							/*寄存器值*/
extern float32_t KY1_printf,KY2_printf,KY3_printf,KY4_printf,KY5_printf,KY6_printf;		/*打印值*/
extern uint8_t   KY1_warning,KY2_warning,KY3_warning,KY4_warning,KY5_warning,KY6_warning;	/*超量程警告*/
extern uint8_t   KY1_runing,KY2_runing,KY3_runing,KY4_runing,KY5_runing,KY6_runing;		/*动态=1 与 稳态=0*/
extern uint8_t   KY1_qvpi,KY2_qvpi,KY3_qvpi,KY4_qvpi,KY5_qvpi,KY6_qvpi;					/*去皮标志位*/
extern uint8_t   KY1_biaoding,KY2_biaoding,KY3_biaoding,KY4_biaoding,KY5_biaoding,KY6_biaoding;	/*标定标志位*/
extern float32_t KY1_xishu,KY2_xishu,KY3_xishu,KY4_xishu,KY5_xishu,KY6_xishu;


extern uint16_t stable_scope;	//稳定范围	0-100，单位d
extern uint16_t stable_time;	//稳定时间 	1-500，单位0.01秒
extern uint16_t dyna_scope;	//动态范围	0-100，单位d
extern uint16_t dyna_time;		//动态时间	1-50，单位0.1秒
extern uint16_t BaudRate_485;	//485波特率		0-6，0：2400，1：4800：2：9600，3：19200，4：38400，5：57600
extern uint16_t data_485;		//485数据格式	0-5，0：7E1， 1：7O1 ，2：8N1 ，3：8N2 ，4：8E1，5：8O1
extern uint16_t slave_address_485;//485从站地址		0-255
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
void all_Init_gain(void);
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KY_CS3_Pin GPIO_PIN_13
#define KY_CS3_GPIO_Port GPIOC
#define KY_SDI3_Pin GPIO_PIN_14
#define KY_SDI3_GPIO_Port GPIOC
#define KY_SDO3_Pin GPIO_PIN_15
#define KY_SDO3_GPIO_Port GPIOC
#define KY_CS2_Pin GPIO_PIN_0
#define KY_CS2_GPIO_Port GPIOC
#define KY_SDI2_Pin GPIO_PIN_1
#define KY_SDI2_GPIO_Port GPIOC
#define KY_SDO2_Pin GPIO_PIN_2
#define KY_SDO2_GPIO_Port GPIOC
#define KY_SCK2_Pin GPIO_PIN_3
#define KY_SCK2_GPIO_Port GPIOC
#define KY_SCK3_Pin GPIO_PIN_1
#define KY_SCK3_GPIO_Port GPIOA
#define RS485_RE_Pin GPIO_PIN_4
#define RS485_RE_GPIO_Port GPIOA
#define KY_CS1_Pin GPIO_PIN_6
#define KY_CS1_GPIO_Port GPIOA
#define KY_SDI1_Pin GPIO_PIN_7
#define KY_SDI1_GPIO_Port GPIOA
#define KY_SDO1_Pin GPIO_PIN_4
#define KY_SDO1_GPIO_Port GPIOC
#define KY_SCK1_Pin GPIO_PIN_5
#define KY_SCK1_GPIO_Port GPIOC
#define KY_SCK6_Pin GPIO_PIN_0
#define KY_SCK6_GPIO_Port GPIOB
#define KY_SDO6_Pin GPIO_PIN_1
#define KY_SDO6_GPIO_Port GPIOB
#define KY_SDI6_Pin GPIO_PIN_2
#define KY_SDI6_GPIO_Port GPIOB
#define KY_CS6_Pin GPIO_PIN_10
#define KY_CS6_GPIO_Port GPIOB
#define KY_CS5_Pin GPIO_PIN_12
#define KY_CS5_GPIO_Port GPIOB
#define KY_SDI5_Pin GPIO_PIN_13
#define KY_SDI5_GPIO_Port GPIOB
#define KY_SDO5_Pin GPIO_PIN_14
#define KY_SDO5_GPIO_Port GPIOB
#define KY_SCK5_Pin GPIO_PIN_15
#define KY_SCK5_GPIO_Port GPIOB
#define KY_CS4_Pin GPIO_PIN_6
#define KY_CS4_GPIO_Port GPIOC
#define KY_SDI4_Pin GPIO_PIN_7
#define KY_SDI4_GPIO_Port GPIOC
#define KY_SDO4_Pin GPIO_PIN_8
#define KY_SDO4_GPIO_Port GPIOC
#define KY_SCK4_Pin GPIO_PIN_9
#define KY_SCK4_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_11
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_12
#define LED2_GPIO_Port GPIOA
#define SCL_Pin GPIO_PIN_15
#define SCL_GPIO_Port GPIOA
#define SDA_Pin GPIO_PIN_12
#define SDA_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
