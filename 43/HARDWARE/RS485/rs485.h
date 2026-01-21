#ifndef __RS485_H
#define __RS485_H			 
#include "main.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//RS485驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	  		  	
extern uint8_t RS485_RX_BUF[64]; 		//接收缓冲,最大64个字节
extern uint16_t RS485_RX_CNT;   			//接收到的数据长度

extern void delay_us(__IO uint32_t delay);

void RS485_Send_Data(uint8_t *buf,uint8_t len);
void RS485_Receive_Data(uint8_t *buf,uint8_t *len);


#endif	   






