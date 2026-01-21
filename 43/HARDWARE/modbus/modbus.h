#ifndef __modbus_H
#define __modbus_H
#include "stdio.h"	
#include "main.h"


extern unsigned int modbus_time;
extern uint8_t	modbus_slave_addr;			//从机地址
extern uint8_t Flag_of_Modbus_Ok;
extern uint8_t	modbus_Tx_buff[1024];		//发送缓冲区
extern uint16_t modbus_virtual_register[550];

void modbus_service(void);
void modbus_03_function(void);
void modbus_06_function(void);
void modbus_16_function(void);
void modbus_send_data(uint8_t *buff,uint8_t len);


unsigned int CRC16(unsigned char *puchMsg,  unsigned char usDataLen);

#endif

