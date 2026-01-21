#ifndef __MY_UART__H
#define __MY_UART__H

#include "stdio.h"
#include "string.h"
#include "main.h"

/* 数据接收缓冲区大小 */
#define PROT_FRAME_LEN_RECV  128

/* 校验数据的长度 */
#define PROT_FRAME_LEN_CHECKSUM    1


#define DataHead4Byte 0x59485A53 //包头

//通道地址任选一个,不能选多
#define Channal01Address1Byte 0x01 //通道地址,每个包只有一个通道地址,多个通道就发多个包
#define Channal02Address1Byte 0x02
#define Channal03Address1Byte 0x03
#define Channal04Address1Byte 0x04
#define Channal05Address1Byte 0x05

//包长度4字节 包头到包尾

//指令 1字节
//参数 ...
//校验和1Byte


//指令 (开发板STM32 到 电脑(PID调试助手))
#define TargetValue 0x01 	//设置上位机通道的目标值 后面参数只有一个,为int类型
#define NowValue 0x02 	//设置上位机通道实际值 1个，实际值，int类型	
#define SetPID 0x03 		// 设置上位机PID值 3个，P、I、D，float类型	
#define Start 0x04 		//设置上位机启动指令（同步上位机的按钮状态） 无参数
#define Stop1 0x05 		//设置上位机停止指令（同步上位机的按钮状态） 无参数
#define SetTime 0x06 	//设置上位机周期 1个，目标值，unsigned int类型	


//指令 ( 电脑(PID调试助手)  到  开发板STM32)

#define ReturnSetPID 0x10 		//设置下位机的PID值 3个，P、I、D，float类型	
#define ReturnSetTargetValue 0x11	//设置下位机的目标值 1个，目标值，int类型	
#define ReturnSetStart 0x12		//启动指令 无参数
#define ReturnSetStop  0x13 		//停止指令 无参数
#define ReturnSetReset 0x14 		//复位指令 无参数
#define ReturnSetTime  0x15 		//设置下位机周期  1个，目标值，unsigned int类型	


void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);


void SendPacket_Target(uint8_t channal, int target, USART_TypeDef * pUSARTx);				//目标值
void SendPacket_NowValue(uint8_t channal, int32_t now, USART_TypeDef * pUSARTx);				//实际值
void SendPacket_PID(uint8_t channal, float p, float i, float d ,USART_TypeDef * pUSARTx);	//PID参数
void SendPacket_start(uint8_t channal, USART_TypeDef * pUSARTx);				//没用
void SendPacket_stop(uint8_t channal, USART_TypeDef * pUSARTx);					//没用
void SendPacket_time(uint8_t channal, int tim, USART_TypeDef * pUSARTx);					//目标值周期值




#endif 

