#include "my_uart.h"
#include "protocol.h"
#include "usart.h"

volatile uint32_t time;

/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	HAL_UART_Transmit(&huart1,&ch, 1, 1000);
		
	/* 等待发送数据寄存器为空 */
//	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* 发送一个字节数据到USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* 等待发送完成 */
//	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
//  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}





void SendPacket_Target(uint8_t channal, int target, USART_TypeDef * pUSARTx)
{
	uint8_t buff[15];
	uint8_t verify = 0;
	int packet_len = 0;
	uint8_t i = 0;
	buff[packet_len] = (DataHead4Byte&(0xff));
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff00))>>8;
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff0000))>>16;
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff000000))>>24;
	packet_len++;
	buff[packet_len] = channal;
	packet_len++;
//	buff[5] = (packet_len&(0xff000000))>>24;
	packet_len++;
//	buff[6] =(packet_len&(0xff0000))>>16;
	packet_len++;
//	buff[7] = (packet_len&0xff00)>>8;
	packet_len++;
//	buff[8] = packet_len&0xff;
	packet_len++;

//控制指令
	buff[packet_len] =  TargetValue;
	packet_len++;

//只有一个目标值的参数
	buff[packet_len] =  target&0xff;
	packet_len++;
	buff[packet_len] = (target&0xff00)>>8;
	packet_len++;
	buff[packet_len] = (target&(0xff0000))>>16;
	packet_len++;
	buff[packet_len] = (target&(0xff000000))>>24;
	packet_len++;
//校验和
//	buff[packet_len] = verify;
	packet_len++;
	
//最后补充包长 , 校验和
	buff[5] =  packet_len&0xff;
	buff[6] = (packet_len&0xff00)>>8;
	buff[7] = (packet_len&(0xff0000))>>16;
	buff[8] = (packet_len&(0xff000000))>>24;

	for(i = 0; i<(packet_len-1); i++)
	{
		verify += buff[i];
		
	}
	
	buff[packet_len-1] = verify;
	
	Usart_SendArray(pUSARTx,buff,packet_len);
}


void SendPacket_NowValue(uint8_t channal, int32_t now, USART_TypeDef * pUSARTx)
{
	uint8_t buff[15];
	uint8_t verify = 0;
	int packet_len = 0;
	uint8_t i = 0;
	buff[packet_len] = (DataHead4Byte&(0xff));
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff00))>>8;
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff0000))>>16;
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff000000))>>24;
	packet_len++;
	buff[packet_len] = channal;
	packet_len++;
//	buff[5] = (packet_len&(0xff000000))>>24;
	packet_len++;
//	buff[6] =(packet_len&(0xff0000))>>16;
	packet_len++;
//	buff[7] = (packet_len&0xff00)>>8;
	packet_len++;
//	buff[8] = packet_len&0xff;
	packet_len++;

//控制指令
	buff[packet_len] =  NowValue;
	packet_len++;

//只有一个目标值的参数
	buff[packet_len] = (now&(0xff));
	packet_len++;
	buff[packet_len] = (now&(0xff00))>>8;
	packet_len++;
	buff[packet_len] = (now&(0xff0000))>>16;
	packet_len++;
	buff[packet_len] = (now&(0xff000000))>>24;
	packet_len++;
//校验和
//	buff[packet_len] = verify;
	packet_len++;
	
//最后补充包长 , 校验和
	buff[5] =  packet_len&0xff;
	buff[6] = (packet_len&0xff00)>>8;
	buff[7] = (packet_len&(0xff0000))>>16;
	buff[8] = (packet_len&(0xff000000))>>24;

	for(i = 0; i<(packet_len-1); i++)
	{
		verify += buff[i];
		
	}
	
	buff[packet_len-1] = verify;
	
	Usart_SendArray(pUSARTx,buff,packet_len);
}


void SendPacket_PID(uint8_t channal, float p, float i, float d ,USART_TypeDef * pUSARTx)
{
	uint8_t buff[32];
	uint8_t verify = 0;
	int packet_len = 0;
	uint8_t farray[4] = {0};
	uint8_t k = 0;
	buff[packet_len] = (DataHead4Byte&(0xff));
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff00))>>8;
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff0000))>>16;
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff000000))>>24;
	packet_len++;
	buff[packet_len] = channal;
	packet_len++;
//	buff[5] = (packet_len&(0xff000000))>>24;
	packet_len++;
//	buff[6] =(packet_len&(0xff0000))>>16;
	packet_len++;
//	buff[7] = (packet_len&0xff00)>>8;
	packet_len++;
//	buff[8] = packet_len&0xff;
	packet_len++;

//控制指令
	buff[packet_len] =  SetPID;
	packet_len++;

//P
	memcpy(farray,&p,sizeof(farray));
	buff[packet_len] =  farray[0];
	packet_len++;
	buff[packet_len] = farray[1];
	packet_len++;
	buff[packet_len] = farray[2];
	packet_len++;
	buff[packet_len] = farray[3];
	packet_len++;
//I
	memcpy(farray,&i,sizeof(farray));
	buff[packet_len] =  farray[0];
	packet_len++;
	buff[packet_len] = farray[1];
	packet_len++;
	buff[packet_len] = farray[2];
	packet_len++;
	buff[packet_len] = farray[3];
	packet_len++;
//D
	memcpy(farray,&d,sizeof(farray));
	buff[packet_len] =  farray[0];
	packet_len++;
	buff[packet_len] = farray[1];
	packet_len++;
	buff[packet_len] = farray[2];
	packet_len++;
	buff[packet_len] = farray[3];
	packet_len++;
	
//校验和
//	buff[packet_len] = verify;
	packet_len++;
	
//最后补充包长 , 校验和
	buff[5] =  packet_len&0xff;
	buff[6] = (packet_len&0xff00)>>8;
	buff[7] = (packet_len&(0xff0000))>>16;
	buff[8] = (packet_len&(0xff000000))>>24;

	for(k = 0; k<(packet_len-1); k++)
	{
		verify += buff[k];
		
	}
	
	buff[packet_len-1] = verify;
	
	Usart_SendArray(pUSARTx,buff,packet_len);
}



void SendPacket_start(uint8_t channal, USART_TypeDef * pUSARTx)
{
	uint8_t buff[15];
	uint8_t verify = 0;
	int packet_len = 0;
	uint8_t i = 0;
	buff[packet_len] = (DataHead4Byte&(0xff));
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff00))>>8;
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff0000))>>16;
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff000000))>>24;
	packet_len++;
	buff[packet_len] = channal;
	packet_len++;
//	buff[5] = (packet_len&(0xff000000))>>24;
	packet_len++;
//	buff[6] =(packet_len&(0xff0000))>>16;
	packet_len++;
//	buff[7] = (packet_len&0xff00)>>8;
	packet_len++;
//	buff[8] = packet_len&0xff;
	packet_len++;

//控制指令
	buff[packet_len] =  Start;
	packet_len++;


//校验和
//	buff[packet_len] = verify;
	packet_len++;
	
//最后补充包长 , 校验和
	buff[5] =  packet_len&0xff;
	buff[6] = (packet_len&0xff00)>>8;
	buff[7] = (packet_len&(0xff0000))>>16;
	buff[8] = (packet_len&(0xff000000))>>24;

	for(i = 0; i<(packet_len-1); i++)
	{
		verify += buff[i];
		
	}
	
	buff[packet_len-1] = verify;
	
	Usart_SendArray(pUSARTx,buff,packet_len);
}

void SendPacket_stop(uint8_t channal, USART_TypeDef * pUSARTx)
{
	uint8_t buff[15];
	uint8_t verify = 0;
	int packet_len = 0;
	uint8_t i = 0;
	buff[packet_len] = (DataHead4Byte&(0xff));
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff00))>>8;
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff0000))>>16;
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff000000))>>24;
	packet_len++;
	buff[packet_len] = channal;
	packet_len++;
//	buff[5] = (packet_len&(0xff000000))>>24;
	packet_len++;
//	buff[6] =(packet_len&(0xff0000))>>16;
	packet_len++;
//	buff[7] = (packet_len&0xff00)>>8;
	packet_len++;
//	buff[8] = packet_len&0xff;
	packet_len++;

//控制指令
	buff[packet_len] =  Stop1;
	packet_len++;


//校验和
//	buff[packet_len] = verify;
	packet_len++;
	
//最后补充包长 , 校验和
	buff[5] =  packet_len&0xff;
	buff[6] = (packet_len&0xff00)>>8;
	buff[7] = (packet_len&(0xff0000))>>16;
	buff[8] = (packet_len&(0xff000000))>>24;

	for(i = 0; i<(packet_len-1); i++)
	{
		verify += buff[i];
		
	}
	
	buff[packet_len-1] = verify;
	
	Usart_SendArray(pUSARTx,buff,packet_len);
}



void SendPacket_time(uint8_t channal, int tim, USART_TypeDef * pUSARTx)
{
	uint8_t buff[15];
	uint8_t verify = 0;
	int packet_len = 0;
	uint8_t i = 0;
	buff[packet_len] = (DataHead4Byte&(0xff));
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff00))>>8;
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff0000))>>16;
	packet_len++;
	buff[packet_len] = (DataHead4Byte&(0xff000000))>>24;
	packet_len++;
	buff[packet_len] = channal;
	packet_len++;
//	buff[5] = (packet_len&(0xff000000))>>24;
	packet_len++;
//	buff[6] =(packet_len&(0xff0000))>>16;
	packet_len++;
//	buff[7] = (packet_len&0xff00)>>8;
	packet_len++;
//	buff[8] = packet_len&0xff;
	packet_len++;

//控制指令
	buff[packet_len] =  SetTime;
	packet_len++;

//只有一个目标值的参数
	buff[packet_len] =  tim&0xff;
	packet_len++;
	buff[packet_len] = (tim&0xff00)>>8;
	packet_len++;
	buff[packet_len] = (tim&(0xff0000))>>16;
	packet_len++;
	buff[packet_len] = (tim&(0xff000000))>>24;
	packet_len++;
//校验和
//	buff[packet_len] = verify;
	packet_len++;
	
//最后补充包长 , 校验和
	buff[5] =  packet_len&0xff;
	buff[6] = (packet_len&0xff00)>>8;
	buff[7] = (packet_len&(0xff0000))>>16;
	buff[8] = (packet_len&(0xff000000))>>24;

	for(i = 0; i<(packet_len-1); i++)
	{
		verify += buff[i];
		
	}
	
	buff[packet_len-1] = verify;
	
	Usart_SendArray(pUSARTx,buff,packet_len);
}

/*写在了串口中断*/
//void DEBUG_USART_IRQHandler(void)
//{	
//	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
//	{	
//		uint8_t dr  = USART_ReceiveData( DEBUG_USARTx );
//		protocol_data_recv(&dr, 1);	
//	}
//}


void  GENERAL_TIM_IRQHandler (void)
{
//	if ( TIM_GetITStatus( GENERAL_TIM, TIM_IT_Update) != RESET ) 
//	{	
//		time++;
//		TIM_ClearITPendingBit(GENERAL_TIM , TIM_FLAG_Update);  		 
//	}		 	
}









