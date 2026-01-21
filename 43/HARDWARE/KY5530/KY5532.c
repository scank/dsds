/******************************************************************************
修改记录：
2021-4-6
  1. 头文件寄存器结构体定义，根据最新的数据手册《KY5532 数据手册_V1.3.pdf》做了部分的修改
  2. 增加了在不同的转换速度下对SYS_CONF1.CKMODE0 和 SYS_CONF2.DMODE两个寄存器的配置
  
2021-5-10
  1. 修复BUG，校验失败，执行SPI复位后，连续转换会被终止，进入死循环等待。
 
2021-5-28
  2. 优化KY5532的复位程序。
  
2021-7-6
  1. 优化了读AD值的校验功能
  2. 添加了使用内部参考电压的功能
  
2021-8-5
  1. 添加功能，开启片选选择功能
     sys_conf0_reg.bit.CSHIGH_MODE = 1; ///< 开启片选模式，每次读数据结束后拉高SDO
     @功能说明：
     @默认情况下sys_conf0_reg.bit.CSHIGH_MODE = 0， 此模式下在连续转换过程中，如果CS引脚电平被拉高，则ADC转换会被强制终止。
     @当sys_conf0_reg.bit.CSHIGH_MODE = 1时，连续转换过程不会受到CS引脚电平状态的影响，如果要终止连续转换，必须发送终止命令0xA5。
     
2021-9-30
      1. 优化单次转换模式及通道切换功能

2022-4-2
   1. 添加了终止转换函数
       void KY5532_Convert_Stop(void)

   2. 添加了连续转换过程中的通道切换测试代码
       void KY5532_CHx_Change_Continue_Test(void)
       
   3. 优化若干功能，更改若干命名
       
******************************************************************************

注意事项：
1. 程序读取方式
   1) 查询方式，通过判断SDO脚为低电平时开始读取数据。
   2）中断方式，强烈建议采用低电平触发外部中断的方式读取数据。因为如果因受外部干扰MCU没有检测到KY5530的下降沿中断，那么KY5530的
SDO脚将一直输出低电平， 等待MCU来读取数据， 后续MCU也无法再次检测到KY5530的下降沿中断， 即后续也无法通过中断触发的方式去读取数
据了， 进入双方同时干等的状态，通过低电平触发中断可以避免这种情况的发生。

2. 如果在连续转换过程中执行命令复位SPI的操作，连续转换过程会被终止，需要再次发送连续转换命令。
3. AD转换错误
   1) 如果转换过程中出现转换错误，寄寄存器SYS_CONF0.ERR_C被置位，则此次ADC转换值应该被抛弃。
   2) 如果开启了奇偶校验功能，发生校验错误时SYS_CONF0.ERR_CKS会被置位，此时应该对SPI总线执行一次复位操作。
   
   在对SYS_CONF0寄存器执行读操作后，ERR_C和ERR_CKS位会被清零
   
*/
 
#define ADC_IC_KY5532 1
#define KY_TRUE  1

#if ADC_IC_KY5532

#include "KY5532.h"
#include "control.h"


//#define KY5530_1    1
//#define KY5530_2    0
//#define KY5530_3    0
//#define KY5530_4    0
//#define KY5530_5    0
//#define KY5530_6    0



/////<--------------------------------KY5532 GPIO相关配置
//#if KY5530_1
//#define KY5532_cs_high()               HAL_GPIO_WritePin(KY_CS1_GPIO_Port, KY_CS1_Pin,GPIO_PIN_SET);
//#define KY5532_cs_low()                HAL_GPIO_WritePin(KY_CS1_GPIO_Port, KY_CS1_Pin,GPIO_PIN_RESET);

//#define KY5532_sck_high()              HAL_GPIO_WritePin(KY_SCK1_GPIO_Port, KY_SCK1_Pin,GPIO_PIN_SET);
//#define KY5532_sck_low()               HAL_GPIO_WritePin(KY_SCK1_GPIO_Port, KY_SCK1_Pin,GPIO_PIN_RESET);

//#define KY5532_di_high()               HAL_GPIO_WritePin(KY_SDI1_GPIO_Port, KY_SDI1_Pin,GPIO_PIN_SET);
//#define KY5532_di_low()                HAL_GPIO_WritePin(KY_SDI1_GPIO_Port, KY_SDI1_Pin,GPIO_PIN_RESET);
// 
//#define KY5532_DO_Boolean()            HAL_GPIO_ReadPin(KY_SDO1_GPIO_Port, KY_SDO1_Pin)


//#elif KY5530_2
//#define KY5532_cs_high()               HAL_GPIO_WritePin(KY_CS2_GPIO_Port, KY_CS2_Pin,GPIO_PIN_SET);
//#define KY5532_cs_low()                HAL_GPIO_WritePin(KY_CS2_GPIO_Port, KY_CS2_Pin,GPIO_PIN_RESET);

//#define KY5532_sck_high()              HAL_GPIO_WritePin(KY_SCK2_GPIO_Port, KY_SCK2_Pin,GPIO_PIN_SET);
//#define KY5532_sck_low()               HAL_GPIO_WritePin(KY_SCK2_GPIO_Port, KY_SCK2_Pin,GPIO_PIN_RESET);

//#define KY5532_di_high()               HAL_GPIO_WritePin(KY_SDI2_GPIO_Port, KY_SDI2_Pin,GPIO_PIN_SET);
//#define KY5532_di_low()                HAL_GPIO_WritePin(KY_SDI2_GPIO_Port, KY_SDI2_Pin,GPIO_PIN_RESET);
// 
//#define KY5532_DO_Boolean()            HAL_GPIO_ReadPin(KY_SDO2_GPIO_Port, KY_SDO2_Pin)


//#elif KY5530_3
//#define KY5532_cs_high()               HAL_GPIO_WritePin(KY_CS3_GPIO_Port, KY_CS3_Pin,GPIO_PIN_SET);
//#define KY5532_cs_low()                HAL_GPIO_WritePin(KY_CS3_GPIO_Port, KY_CS3_Pin,GPIO_PIN_RESET);

//#define KY5532_sck_high()              HAL_GPIO_WritePin(KY_SCK3_GPIO_Port, KY_SCK3_Pin,GPIO_PIN_SET);
//#define KY5532_sck_low()               HAL_GPIO_WritePin(KY_SCK3_GPIO_Port, KY_SCK3_Pin,GPIO_PIN_RESET);

//#define KY5532_di_high()               HAL_GPIO_WritePin(KY_SDI3_GPIO_Port, KY_SDI3_Pin,GPIO_PIN_SET);
//#define KY5532_di_low()                HAL_GPIO_WritePin(KY_SDI3_GPIO_Port, KY_SDI3_Pin,GPIO_PIN_RESET);
// 
//#define KY5532_DO_Boolean()            HAL_GPIO_ReadPin(KY_SDO3_GPIO_Port, KY_SDO3_Pin)


//#elif KY5530_4
//#define KY5532_cs_high()               HAL_GPIO_WritePin(KY_CS4_GPIO_Port, KY_CS4_Pin,GPIO_PIN_SET);
//#define KY5532_cs_low()                HAL_GPIO_WritePin(KY_CS4_GPIO_Port, KY_CS4_Pin,GPIO_PIN_RESET);

//#define KY5532_sck_high()              HAL_GPIO_WritePin(KY_SCK4_GPIO_Port, KY_SCK4_Pin,GPIO_PIN_SET);
//#define KY5532_sck_low()               HAL_GPIO_WritePin(KY_SCK4_GPIO_Port, KY_SCK4_Pin,GPIO_PIN_RESET);

//#define KY5532_di_high()               HAL_GPIO_WritePin(KY_SDI4_GPIO_Port, KY_SDI4_Pin,GPIO_PIN_SET);
//#define KY5532_di_low()                HAL_GPIO_WritePin(KY_SDI4_GPIO_Port, KY_SDI4_Pin,GPIO_PIN_RESET);
// 
//#define KY5532_DO_Boolean()            HAL_GPIO_ReadPin(KY_SDO4_GPIO_Port, KY_SDO4_Pin)


//#elif KY5530_5
//#define KY5532_cs_high()               HAL_GPIO_WritePin(KY_CS5_GPIO_Port, KY_CS5_Pin,GPIO_PIN_SET);
//#define KY5532_cs_low()                HAL_GPIO_WritePin(KY_CS5_GPIO_Port, KY_CS5_Pin,GPIO_PIN_RESET);

//#define KY5532_sck_high()              HAL_GPIO_WritePin(KY_SCK5_GPIO_Port, KY_SCK5_Pin,GPIO_PIN_SET);
//#define KY5532_sck_low()               HAL_GPIO_WritePin(KY_SCK5_GPIO_Port, KY_SCK5_Pin,GPIO_PIN_RESET);

//#define KY5532_di_high()               HAL_GPIO_WritePin(KY_SDI5_GPIO_Port, KY_SDI5_Pin,GPIO_PIN_SET);
//#define KY5532_di_low()                HAL_GPIO_WritePin(KY_SDI5_GPIO_Port, KY_SDI5_Pin,GPIO_PIN_RESET);
// 
//#define KY5532_DO_Boolean()            HAL_GPIO_ReadPin(KY_SDO5_GPIO_Port, KY_SDO5_Pin)


//#elif KY5530_6
//#define KY5532_cs_high()               HAL_GPIO_WritePin(KY_CS6_GPIO_Port, KY_CS6_Pin,GPIO_PIN_SET);
//#define KY5532_cs_low()                HAL_GPIO_WritePin(KY_CS6_GPIO_Port, KY_CS6_Pin,GPIO_PIN_RESET);

//#define KY5532_sck_high()              HAL_GPIO_WritePin(KY_SCK6_GPIO_Port, KY_SCK6_Pin,GPIO_PIN_SET);
//#define KY5532_sck_low()               HAL_GPIO_WritePin(KY_SCK6_GPIO_Port, KY_SCK6_Pin,GPIO_PIN_RESET);

//#define KY5532_di_high()               HAL_GPIO_WritePin(KY_SDI6_GPIO_Port, KY_SDI6_Pin,GPIO_PIN_SET);
//#define KY5532_di_low()                HAL_GPIO_WritePin(KY_SDI6_GPIO_Port, KY_SDI6_Pin,GPIO_PIN_RESET);
// 
//#define KY5532_DO_Boolean()            HAL_GPIO_ReadPin(KY_SDO6_GPIO_Port, KY_SDO6_Pin)

//#endif

#define KY5532_nop()       delay_nop(0)


void KY5532_cs_high(void);
void KY5532_cs_low(void);
void KY5532_sck_high(void);
void KY5532_sck_low(void);
void KY5532_di_high(void);
void KY5532_di_low(void);
GPIO_PinState KY5532_DO_Boolean(void);


void KY5532_cs_high()
{
	switch(ky5530_channel)
	{
		case 1:
		HAL_GPIO_WritePin(KY_CS1_GPIO_Port, KY_CS1_Pin,GPIO_PIN_SET);
		break;
		case 2:
		HAL_GPIO_WritePin(KY_CS2_GPIO_Port, KY_CS2_Pin,GPIO_PIN_SET);
		break;
		case 3:
		HAL_GPIO_WritePin(KY_CS3_GPIO_Port, KY_CS3_Pin,GPIO_PIN_SET);
		break;
		case 4:
		HAL_GPIO_WritePin(KY_CS4_GPIO_Port, KY_CS4_Pin,GPIO_PIN_SET);
		break;
		case 5:
		HAL_GPIO_WritePin(KY_CS5_GPIO_Port, KY_CS5_Pin,GPIO_PIN_SET);					
		break;
		case 6:
		HAL_GPIO_WritePin(KY_CS6_GPIO_Port, KY_CS6_Pin,GPIO_PIN_SET);
		break;
		default:
		break;
	}
}
void KY5532_cs_low()
{
	switch(ky5530_channel)
	{
		case 1:
		HAL_GPIO_WritePin(KY_CS1_GPIO_Port, KY_CS1_Pin,GPIO_PIN_RESET);
		break;
		case 2:
		HAL_GPIO_WritePin(KY_CS2_GPIO_Port, KY_CS2_Pin,GPIO_PIN_RESET);
		break;
		case 3:
		HAL_GPIO_WritePin(KY_CS3_GPIO_Port, KY_CS3_Pin,GPIO_PIN_RESET);
		break;
		case 4:
		HAL_GPIO_WritePin(KY_CS4_GPIO_Port, KY_CS4_Pin,GPIO_PIN_RESET);
		break;
		case 5:
		HAL_GPIO_WritePin(KY_CS5_GPIO_Port, KY_CS5_Pin,GPIO_PIN_RESET);					
		break;
		case 6:
		HAL_GPIO_WritePin(KY_CS6_GPIO_Port, KY_CS6_Pin,GPIO_PIN_RESET);
		break;
		default:
		break;
	}
}

void KY5532_sck_high()
{
	switch(ky5530_channel)
	{
		case 1:
		HAL_GPIO_WritePin(KY_SCK1_GPIO_Port, KY_SCK1_Pin,GPIO_PIN_SET);
		break;
		case 2:
		HAL_GPIO_WritePin(KY_SCK2_GPIO_Port, KY_SCK2_Pin,GPIO_PIN_SET);
		break;
		case 3:
		HAL_GPIO_WritePin(KY_SCK3_GPIO_Port, KY_SCK3_Pin,GPIO_PIN_SET);
		break;
		case 4:
		HAL_GPIO_WritePin(KY_SCK4_GPIO_Port, KY_SCK4_Pin,GPIO_PIN_SET);
		break;
		case 5:
		HAL_GPIO_WritePin(KY_SCK5_GPIO_Port, KY_SCK5_Pin,GPIO_PIN_SET);					
		break;
		case 6:
		HAL_GPIO_WritePin(KY_SCK6_GPIO_Port, KY_SCK6_Pin,GPIO_PIN_SET);
		break;
		default:
		break;
	}
}
void KY5532_sck_low()
{
	switch(ky5530_channel)
	{
		case 1:
		HAL_GPIO_WritePin(KY_SCK1_GPIO_Port, KY_SCK1_Pin,GPIO_PIN_RESET);
		break;
		case 2:
		HAL_GPIO_WritePin(KY_SCK2_GPIO_Port, KY_SCK2_Pin,GPIO_PIN_RESET);
		break;
		case 3:
		HAL_GPIO_WritePin(KY_SCK3_GPIO_Port, KY_SCK3_Pin,GPIO_PIN_RESET);
		break;
		case 4:
		HAL_GPIO_WritePin(KY_SCK4_GPIO_Port, KY_SCK4_Pin,GPIO_PIN_RESET);
		break;
		case 5:
		HAL_GPIO_WritePin(KY_SCK5_GPIO_Port, KY_SCK5_Pin,GPIO_PIN_RESET);					
		break;
		case 6:
		HAL_GPIO_WritePin(KY_SCK6_GPIO_Port, KY_SCK6_Pin,GPIO_PIN_RESET);
		break;
		default:
		break;
	}
}

void KY5532_di_high()
{
	switch(ky5530_channel)
	{
		case 1:
		HAL_GPIO_WritePin(KY_SDI1_GPIO_Port, KY_SDI1_Pin,GPIO_PIN_SET);
		break;
		case 2:
		HAL_GPIO_WritePin(KY_SDI2_GPIO_Port, KY_SDI2_Pin,GPIO_PIN_SET);
		break;
		case 3:
		HAL_GPIO_WritePin(KY_SDI3_GPIO_Port, KY_SDI3_Pin,GPIO_PIN_SET);
		break;
		case 4:
		HAL_GPIO_WritePin(KY_SDI4_GPIO_Port, KY_SDI4_Pin,GPIO_PIN_SET);
		break;
		case 5:
		HAL_GPIO_WritePin(KY_SDI5_GPIO_Port, KY_SDI5_Pin,GPIO_PIN_SET);				
		break;
		case 6:
		HAL_GPIO_WritePin(KY_SDI6_GPIO_Port, KY_SDI6_Pin,GPIO_PIN_SET);
		break;
		default:
		break;
	}
}
void KY5532_di_low()
{
	switch(ky5530_channel)
	{
		case 1:
		HAL_GPIO_WritePin(KY_SDI1_GPIO_Port, KY_SDI1_Pin,GPIO_PIN_RESET);
		break;
		case 2:
		HAL_GPIO_WritePin(KY_SDI2_GPIO_Port, KY_SDI2_Pin,GPIO_PIN_RESET);
		break;
		case 3:
		HAL_GPIO_WritePin(KY_SDI3_GPIO_Port, KY_SDI3_Pin,GPIO_PIN_RESET);
		break;
		case 4:
		HAL_GPIO_WritePin(KY_SDI4_GPIO_Port, KY_SDI4_Pin,GPIO_PIN_RESET);
		break;
		case 5:
		HAL_GPIO_WritePin(KY_SDI5_GPIO_Port, KY_SDI5_Pin,GPIO_PIN_RESET);				
		break;
		case 6:
		HAL_GPIO_WritePin(KY_SDI6_GPIO_Port, KY_SDI6_Pin,GPIO_PIN_RESET);
		break;
		default:
		break;
	}
}

GPIO_PinState KY5532_DO_Boolean()
{
	GPIO_PinState ccc;
	switch(ky5530_channel)
	{
		case 1:
		ccc = HAL_GPIO_ReadPin(KY_SDO1_GPIO_Port, KY_SDO1_Pin);
		break;
		case 2:
		ccc = HAL_GPIO_ReadPin(KY_SDO2_GPIO_Port, KY_SDO2_Pin);
		break;
		case 3:
		ccc = HAL_GPIO_ReadPin(KY_SDO3_GPIO_Port, KY_SDO3_Pin);
		break;
		case 4:
		ccc = HAL_GPIO_ReadPin(KY_SDO4_GPIO_Port, KY_SDO4_Pin);
		break;
		case 5:
		ccc = HAL_GPIO_ReadPin(KY_SDO5_GPIO_Port, KY_SDO5_Pin);			
		break;
		case 6:
		ccc = HAL_GPIO_ReadPin(KY_SDO6_GPIO_Port, KY_SDO6_Pin);
		break;
		default:
		break;
	}
	return ccc;
}



void send_one_clk(void)
{
//////	switch(ky5530_channel)
//////	{
//////		case 1:
//////HAL_GPIO_WritePin(KY_SCK1_GPIO_Port, KY_SCK1_Pin,GPIO_PIN_SET);
//////HAL_GPIO_WritePin(KY_SCK1_GPIO_Port, KY_SCK1_Pin,GPIO_PIN_RESET);
//////						break;
//////		case 2:
//////HAL_GPIO_WritePin(KY_SCK2_GPIO_Port, KY_SCK2_Pin,GPIO_PIN_SET);
//////HAL_GPIO_WritePin(KY_SCK2_GPIO_Port, KY_SCK2_Pin,GPIO_PIN_RESET);
//////						break;
//////		case 3:
//////HAL_GPIO_WritePin(KY_SCK3_GPIO_Port, KY_SCK3_Pin,GPIO_PIN_SET);
//////HAL_GPIO_WritePin(KY_SCK3_GPIO_Port, KY_SCK3_Pin,GPIO_PIN_RESET);
//////						break;
//////		case 4:
//////HAL_GPIO_WritePin(KY_SCK4_GPIO_Port, KY_SCK4_Pin,GPIO_PIN_SET);
//////HAL_GPIO_WritePin(KY_SCK4_GPIO_Port, KY_SCK4_Pin,GPIO_PIN_RESET);
//////						break;
//////		case 5:
//////HAL_GPIO_WritePin(KY_SCK5_GPIO_Port, KY_SCK5_Pin,GPIO_PIN_SET);
//////HAL_GPIO_WritePin(KY_SCK5_GPIO_Port, KY_SCK5_Pin,GPIO_PIN_RESET);					
//////						break;
//////		case 6:
//////HAL_GPIO_WritePin(KY_SCK6_GPIO_Port, KY_SCK6_Pin,GPIO_PIN_SET);
//////HAL_GPIO_WritePin(KY_SCK6_GPIO_Port, KY_SCK6_Pin,GPIO_PIN_RESET);
//////						break;
//////		default:
//////						break;
//////	}
    KY5532_sck_high();
    KY5532_sck_low();
}

static void delay_nop(uint32_t dly)
{
    while(dly--){;}
}

/**
 * KY5532 gpio初始化
 */
void KY5532_gpio_init(void)
{  
//////	switch(ky5530_channel)
//////	{
//////		case 1:
//////			HAL_GPIO_WritePin(KY_CS1_GPIO_Port, KY_CS1_Pin,GPIO_PIN_SET);
//////			HAL_GPIO_WritePin(KY_SCK1_GPIO_Port, KY_SCK1_Pin,GPIO_PIN_RESET);
//////			HAL_GPIO_WritePin(KY_SDI1_GPIO_Port, KY_SDI1_Pin,GPIO_PIN_SET);
//////						break;
//////		case 2:
//////			HAL_GPIO_WritePin(KY_CS2_GPIO_Port, KY_CS2_Pin,GPIO_PIN_SET);
//////			HAL_GPIO_WritePin(KY_SCK2_GPIO_Port, KY_SCK2_Pin,GPIO_PIN_RESET);
//////			HAL_GPIO_WritePin(KY_SDI2_GPIO_Port, KY_SDI2_Pin,GPIO_PIN_SET);
//////						break;
//////		case 3:
//////			HAL_GPIO_WritePin(KY_CS3_GPIO_Port, KY_CS3_Pin,GPIO_PIN_SET);
//////			HAL_GPIO_WritePin(KY_SCK3_GPIO_Port, KY_SCK3_Pin,GPIO_PIN_RESET);
//////			HAL_GPIO_WritePin(KY_SDI3_GPIO_Port, KY_SDI3_Pin,GPIO_PIN_SET);
//////						break;
//////		case 4:
//////			HAL_GPIO_WritePin(KY_CS4_GPIO_Port, KY_CS4_Pin,GPIO_PIN_SET);
//////			HAL_GPIO_WritePin(KY_SCK4_GPIO_Port, KY_SCK4_Pin,GPIO_PIN_RESET);
//////			HAL_GPIO_WritePin(KY_SDI4_GPIO_Port, KY_SDI4_Pin,GPIO_PIN_SET);
//////						break;
//////		case 5:
//////			HAL_GPIO_WritePin(KY_CS5_GPIO_Port, KY_CS5_Pin,GPIO_PIN_SET);
//////			HAL_GPIO_WritePin(KY_SCK5_GPIO_Port, KY_SCK5_Pin,GPIO_PIN_RESET);		
//////			HAL_GPIO_WritePin(KY_SDI5_GPIO_Port, KY_SDI5_Pin,GPIO_PIN_SET);		
//////						break;
//////		case 6:
//////			HAL_GPIO_WritePin(KY_CS6_GPIO_Port, KY_CS6_Pin,GPIO_PIN_SET);
//////			HAL_GPIO_WritePin(KY_SCK6_GPIO_Port, KY_SCK6_Pin,GPIO_PIN_RESET);
//////			HAL_GPIO_WritePin(KY_SDI6_GPIO_Port, KY_SDI6_Pin,GPIO_PIN_SET);
//////						break;
//////		default:
//////						break;
//////	}
    KY5532_cs_high();
    KY5532_sck_low();
    KY5532_di_high();
}

/*======== 以下为基本读写函数 ==============================================================================================================*/

volatile uint8_t check_sum_en_flg=0; ///< check 功能打开标志
#if(CHECK_SUM_EN)
    volatile uint8_t KY5532_check_failed; ///< 数据校验，如果校验失败会被置位，接下来需要复位SPI接口并重新开启连续转换功能，寄存器不需要重复设置。   
#endif

///<                           0        1         2     3     4     5      6      7       8       9      10        11     12        13                
///<             ADC Speed:   12.5HZ  6.25HZ    25Hz  50Hz  100Hz  200Hz  400Hz  800Hz  1600Hz  3200Hz  6300Hz  12800Hz  25600Hz  51200Hz
static const uint8_t speedTab[] = { 0x0C,   0x0F,     0x0b, 0x0A, 0x09,  0x08,  0x7,   0x6,   0x5,    0x4,    0x3,    0x2,     0x1,     0x0    };

       ///<             ADC PGA:     x1       x2        x64  x128   x4     x8     x16    x32
static  const uint8_t pgaTab[]   = {0x06,    0x07,     0x00, 0x01, 0x04,  0x05,  0x02,  0x03};



volatile uint32_t KY5532_getVal_timeout_ms; ///< 读取数据超时计时器 
volatile uint8_t read_adc_timeout_cnt;      ///< 数据失败次数计数 

///< 读ADC超时计时变量累加
///< 此函数放在定时器中调用，用于ADC超时计时功能
void ReadADC_TimeoutCnt_ms(uint32_t n_ms)
{
    KY5532_getVal_timeout_ms += n_ms;
}
/**
 * 向KY5532发送一个字节的数据
 */
void KY5532_send_byte(uint8_t byt)
{ 
    uint8_t i;
    
    for(i=0;i<8;i++){
        KY5532_sck_low();
        KY5532_nop();
        if(byt&(0x80>>i)){
            KY5532_di_high();
        }else{
            KY5532_di_low();
        }
        KY5532_sck_high();
        KY5532_nop();
    }
    KY5532_sck_low();
}
void KY5532_send_4byte(uint32_t byt4)
{
    KY5532_send_byte((byt4>>24) & 0x000000FF);
    KY5532_send_byte((byt4>>16) & 0x000000FF);
    KY5532_send_byte((byt4>> 8) & 0x000000FF);
    KY5532_send_byte((byt4>> 0) & 0x000000FF);
}

/**
 * 函数功能：读1个字节的数据
 * 输入参数：
 */
uint8_t KY5532_read1byte(void)
{
    uint8_t i;
    uint8_t val;
     
    val = 0;
    for(i=0;i<8;i++){
        KY5532_sck_low();
        KY5532_nop();

        if(KY5532_DO_Boolean() == KY_TRUE ){
            val |= (0x80 >> i);
        }

        KY5532_sck_high();
        KY5532_nop();
    }
    KY5532_sck_low();

    return val;
}


/**
 * 读取4个字节数据
 * 返回值： 0，读取结果无效   1，读取结果有效
 */
uint8_t KY5532_read4byte(uint32_t *val4byte)
{
    uint8_t i;
    uint32_t val;
    
    val = 0;
    for(i=0;i<4;i++){
        val = (val << 8)  + KY5532_read1byte();
    }
    ///< 判断校验和
#if(CHECK_SUM_EN)
    uint32_t checkSum,checkSumTmp;
    
    if(check_sum_en_flg){
        checkSumTmp = KY5532_read1byte(); ///< 读取校验和
        checkSum = (((val>>24) & 0x000000FF) + ((val>>16) & 0x000000FF)
                  + ((val>>8) & 0x000000FF) + ((val>>0) & 0x000000FF) + 0x5A) & 0xFF;
                
        if(checkSumTmp != checkSum){
//            printf("\r\n-> Read 4B [%08X]",val);
//            printf("->CheckSum[%02X - %02X]",checkSumTmp,checkSum);
//            printf("\r\n # ERROR: Read ADC Value error!");
            KY5532_check_failed = 1;  ///< 数据校验失败
            return 0;
        }
    }    
#endif
//		printf("\r\n %d",val);
   *val4byte = val;
    return 1;
}




void KY5532_cmd_send(uint8_t cmd)
{
    KY5532_CMD_Typedef  KY5532;
    
    KY5532.cmd = cmd;
    
    // 计算奇偶位
    KY5532.rwCmd_bits.pc = 0;
    for(uint8_t i=0;i<7;i++){
        if(KY5532.cmd & (0x80>>i)){
            KY5532.rwCmd_bits.pc = ~KY5532.rwCmd_bits.pc ; // 校验位取反
        }
    }
 //   printf("\r\n W_CMD:%02X",KY5532.cmd);

    KY5532_send_byte(KY5532.cmd);
}

void KY5532_w_cmd_verify(uint8_t regAddr, uint32_t regDat,uint8_t vfyFlg)
{
    KY5532_CMD_Typedef  KY5532;
    
    
    KY5532.cmd = 0;
    KY5532.rwCmd_bits.rw_con_flg = 0;       // 此位固定为0，即为读写寄存器命令帧模式
    KY5532.rwCmd_bits.rw         = 0;       //  0:写   1:读   
    KY5532.rwCmd_bits.array      = 0;       // 0,单一寄存器读写    1,连续寄存器读写
    KY5532.rwCmd_bits.addr       = regAddr; // 寄存器地址
    
    KY5532_cs_low();    
    /// 发送命令字节
    KY5532_cmd_send(KY5532.cmd);

    /// 发送寄存器的数据
    KY5532_send_byte((regDat>>24) & 0x000000FF);
    KY5532_send_byte((regDat>>16) & 0x000000FF);
    KY5532_send_byte((regDat>> 8) & 0x000000FF);
    KY5532_send_byte((regDat>> 0) & 0x000000FF);
    
//    printf("\r\n W_REG_V:%02X,%08X,%d",KY5532.cmd,  regDat,vfyFlg);
//    printf(" %08X,%d", regDat,vfyFlg);

    ///< 发送校验和
    uint32_t checkSum;
    if(vfyFlg){
        
        checkSum = ((regDat>>24) & 0x000000FF) + ((regDat>>16) & 0x000000FF)
                  + ((regDat>>8) & 0x000000FF) + ((regDat>>0) & 0x000000FF) + 0x5A;
       KY5532_send_byte(checkSum & 0xFF);
    }
     
    KY5532_cs_high();
}


/**
 * 函数功能：向寄存器写入数据，寄存器宽度为32bit
 * 输入参数：
 *          regAddr，寄存器地址
 *          regDat， 寄存器数据
 */
void KY5532_w_cmd(uint8_t regAddr, uint32_t regDat)
{
    KY5532_w_cmd_verify( regAddr,  regDat, 0);
}


/**
 * 函数功能：读寄存器的值
 * 输入参数：
 *          regAddr，寄存器地址
 *          regDat， 寄存器数据
 */
uint32_t KY5532_r_cmd(uint8_t regAddr)
{
    KY5532_CMD_Typedef  KY5532;
    uint32_t regDat = 0;

    KY5532.cmd = 0;
    KY5532.rwCmd_bits.rw_con_flg = 0;       // 此位固定为0，即为读写寄存器命令帧模式
    KY5532.rwCmd_bits.rw         = 1;       // 0:写   1:读    
    KY5532.rwCmd_bits.array      = 0;       // 0,单一寄存器读写    1,连续寄存器读写
    KY5532.rwCmd_bits.addr       = regAddr; // 寄存器地址

    KY5532_cs_low();

    /// 发送命令字节
    KY5532_cmd_send(KY5532.cmd); 

    /// 读取寄存器的值
    KY5532_read4byte(&regDat);

    KY5532_cs_high();

    return regDat;
}

/**
 * 函数功能：配置KY5532的转换模式
 * 输入参数：
 *          cfg, 转换配置选择
 *          mod, 转换模式选择
 */
 void KY5532_conv_start(uint8_t cfg, uint8_t mod)
{
    KY5532_CMD_Typedef convCfg;

    convCfg.cmd = 0;
    convCfg.covCmd_bits.rw_con_flg = 1; // 此位固定为1    0,读写寄存器命令帧   1, 启动ADC转换的转换命令帧
    convCfg.covCmd_bits.conv_conf  = cfg; // 转换配置选择
                                        //    0x0: 使用 CONF00
                                        //    0x1: 使用 CONF01
                                        //    0x2: 使用 CONF10
                                        //    0x3: 使用 CONF11
                                        
    convCfg.covCmd_bits.conv_mod   = mod; // 转换模式选择
                                        //    0x0: 正常单次转换模式
                                        //    0x1: 正常连续转换模式
                                        //    0x2: Offset 自校准模式
                                        //    0x5: Offset 系统校准模式
                                        //    0x6: Gain 系统校准模式                                     
    /// 发送命令字节
    KY5532_cs_low();
    printf("\r\n CONV_Continue:%02X",convCfg.cmd);
    KY5532_cmd_send(convCfg.cmd);
 //   KY5532_cs_high();
}


/*======== 以下为配置函数 ==================================================================================================*/
 
/**
 * KY5532配置寄存器设置
 *
 */
void KY5532_ch_cfg(int8_t cfg_x,    ///< ADC转换通道使用的配置寄存器，CONV_CONF00，CONV_CONF01，CONV_CONF10，CONV_CONF11
                   KY5532_CONV_CONFx_Typedef cfg_reg_val)
{
    uint32_t cfg_Reg_Tmp;
    
    switch(cfg_x){
        case CONV_CONF00:
        case CONV_CONF01:
            ///< 读取当前配置寄存器
            cfg_Reg_Tmp = KY5532_r_cmd(ADDR_CONV_CONF0); 
        
            if(cfg_x == CONV_CONF00){
                cfg_Reg_Tmp = (cfg_Reg_Tmp & 0xFFFF0000) | (cfg_reg_val.val & 0x0000FFFF);
                printf("\r\n -> CONV_CONF00:%08X-%04X", cfg_Reg_Tmp,cfg_reg_val.val);
            }else{
                cfg_Reg_Tmp = (cfg_Reg_Tmp & 0x0000FFFF) | ((cfg_reg_val.val<<16) & 0xFFFF0000);                
                printf("\r\n -> CONV_CONF01:%08X-%04X", cfg_Reg_Tmp,cfg_reg_val.val);
            }
            ///< 写入寄存器数据
            KY5532_w_cmd(ADDR_CONV_CONF0, cfg_Reg_Tmp); 
            
            printf(" W:%08X",cfg_Reg_Tmp);
            cfg_Reg_Tmp=0;
            cfg_Reg_Tmp = KY5532_r_cmd(ADDR_CONV_CONF0); 
            printf("  - R:%08X",cfg_Reg_Tmp);
            
            break;
        case CONV_CONF10:
        case CONV_CONF11:
            ///< 读取当前配置寄存器
            cfg_Reg_Tmp = KY5532_r_cmd(ADDR_CONV_CONF1); 

            if(cfg_x == CONV_CONF10){
                cfg_Reg_Tmp = (cfg_Reg_Tmp & 0xFFFF0000) | (cfg_reg_val.val & 0x0000FFFF);
                printf("\r\n -> CONV_CONF10:%08X-%04X", cfg_Reg_Tmp,cfg_reg_val.val);
            }else{
                cfg_Reg_Tmp = (cfg_Reg_Tmp & 0x0000FFFF) | ((cfg_reg_val.val<<16) & 0xFFFF0000);                
                printf("\r\n -> CONV_CONF11:%08X-%04X", cfg_Reg_Tmp,cfg_reg_val.val);
            }
            KY5532_w_cmd(ADDR_CONV_CONF1, cfg_Reg_Tmp); 
            
            printf(" W:%08X",cfg_Reg_Tmp);
            cfg_Reg_Tmp=0;
            cfg_Reg_Tmp = KY5532_r_cmd(ADDR_CONV_CONF1); 
            printf("  - R:%08X",cfg_Reg_Tmp);
            break;
        default:
            break;
    }   
}
 
/**
 * KY5532偏移校准
 */
void KY5532_offset_selfCalibration(void)
{
#if 1
    int32_t val;

    ///< offset系统校准校准
    KY5532_conv_start(CONV_CONF10,CONV_MOD_OFF_AUT);
    while(KY5532_read_adcVal(&val) == 0){;}

    KY5532_conv_start(CONV_CONF00,CONV_MOD_OFF_AUT);
    while(KY5532_read_adcVal(&val) == 0){;}

#else
    int32_t val,sum;
    int32_t calibBuffer[10];
    uint32_t revVal;
    
    ///< offset系统校准校准
   for(uint8_t i=0;i<4;i++){
        KY5532_conv_start(CONV_CONF00,CONV_MOD_OFF_AUT);
        while(KY5532_read_adcVal(&val) == 0){;}
    }
   
    sum = 0;
    for(uint8_t i=0;i<10;i++){
        KY5532_conv_start(CONV_CONF00,CONV_MOD_OFF_AUT);
        while(KY5532_read_adcVal(&val) == 0){;}
        sum += val;
        printf("\r\n Clib CONV_CONF00: %d",val);
    }
    sum = sum / 10;
    sum = sum*2; ///< 相当于左移一位，bit0作为通道位
    
    if(sum >= 0)
        revVal = sum ;
    else
        revVal = ((sum^0x3FFFFFFF) +1) & 0xFFFFFFFE;
    
    printf("\r\n @ADDR_OS_CH0: %08X",revVal);
    KY5532_w_cmd(ADDR_OS_CH0 ,  revVal);
    
    
    ///< offset系统校准校准
//    KY5532_conv_start(CONV_CONF10,CONV_MOD_OFF_AUT);
//    while(KY5532_read_adcVal(&val) == 0){;}

//    KY5532_conv_start(CONV_CONF00,CONV_MOD_OFF_AUT);
//    while(KY5532_read_adcVal(&val) == 0){;}

    sum = 0;
    for(uint8_t i=0;i<4;i++){
        KY5532_conv_start(CONV_CONF10,CONV_MOD_OFF_AUT);
        while(KY5532_read_adcVal(&val) == 0){;}
    }
   for(uint8_t i=0;i<10;i++){
        KY5532_conv_start(CONV_CONF10,CONV_MOD_OFF_AUT);
        while(KY5532_read_adcVal(&val) == 0){;}
        sum += val;
        printf("\r\n Clib CONV_CONF10: %d",val);
    }
    sum = sum / 10;
    sum = sum*2; ///< 相当于左移一位，bit0作为通道位
    
    if(sum >= 0)
        revVal = sum-10000;
    else
        revVal = ((sum^0x3FFFFFFF) +1) & 0xFFFFFFFE;
    
    printf("\r\n @ADDR_OS_CH1: %08X",revVal);
    KY5532_w_cmd(ADDR_OS_CH1 ,  revVal);
#endif
    
    
 }

/**
 * KY5532增益校准
 */
void KY5532_Gain_Calibration(void)
{
    ///< 保证芯片外部所加信号幅度达到满量程的 10%以上（最好为 20%~50%之间）
    ///< 根据输入信号的幅度，计算期望得到的ADC值Dtarg
    ///< 启动AD转换，读取输出的AD值 Dout
    ///< 计算校准增益：Dtarg/Dout,将此值存入相应通道的GAIN校准寄存器中
    
    ///< 可以多次重复上述操作并取平均，以获得更精确的校准值
}


/**
 * KY5532初始化
 * 初始化流程：
 *            芯片上电后， OS_CHx/ GAIN_CHx(x=0/1)寄存器、CONV_CONFx(x=0/1)寄存器、 SYS_CONFx(x=0/1/2)寄存器写需要的设置值。
 * 返回值： 0，初始化失败     1，初始化成功
 */
uint8_t KY5532_Config(KY553x_PGA_Type pgaIndx,KY553x_SPEED_Type speedIndx)
{
    KY5532_SYS_CONF0_Typedef sys_conf0_reg;
    KY5532_SYS_CONF1_Typedef sys_conf1_reg;
    KY5532_SYS_CONF2_Typedef sys_conf2_reg;
   uint8_t init_try_cnt;
    // GPIO初始化 
    KY5532_gpio_init();
    
    KY5532_Convert_Stop();
    init_try_cnt = 0;
    
#if 0
    do{
        init_try_cnt++;
   //     if(init_try_cnt > 5)return 0; ///< 复位次数超过限制
      //   printf("\r\n -> Try Reset:%d",init_try_cnt);
               
        ///< 复位SPI接口
        KY5532_SPI_Reset();
        
        ///< 先用校验的方式复位KY5532
        check_sum_en_flg = 1;
        KY5532_w_cmd_verify(ADDR_SYS_CONF0, 0x00000001ul<<31, 1);
        delay_nop(10000);
        sys_conf0_reg.val = KY5532_r_cmd(ADDR_SYS_CONF0);
        if((sys_conf0_reg.bit.RS_SYS==0) && (sys_conf0_reg.bit.RS_V == 1)){
            break;
        }
        
        ///< 再用正常方式复位KY5532
        check_sum_en_flg = 0;
        KY5532_w_cmd(ADDR_SYS_CONF0, 0x00000001ul<<31);
        delay_nop(10000);
        
        sys_conf0_reg.val = KY5532_r_cmd(ADDR_SYS_CONF0);
        if((sys_conf0_reg.bit.RS_SYS==0) && (sys_conf0_reg.bit.RS_V == 1)){
            break;
        }else{
            continue;
        }
        
        break;
    }while(1);
                
#else
    do{
        init_try_cnt++;
      ///  if(init_try_cnt > 5)return 0; ///< 复位次数超过限制
        printf("\r\n -> Try Reset:%d",init_try_cnt);
        
        ///< 复位SPI接口
        KY5532_SPI_Reset();
        printf("\r\n -> SPI Interface reset");
        
        ///< 读取配置寄存器0，判断是否开启了校验功能
        sys_conf0_reg.val = KY5532_r_cmd(ADDR_SYS_CONF0);
        
        printf("\r\n -> ADDR_CONV_CONF0:0x%08X",sys_conf0_reg.val);
        
        if(sys_conf0_reg.bit.CKS_EN){
            printf("\r\n -> Device Reset by verify");
            check_sum_en_flg = 1;
            KY5532_w_cmd_verify(ADDR_SYS_CONF0, 0x00000001ul<<31, 1); ///< 软件全局复位
            continue;
        }

        printf("\r\n -> Device Reset  in normal mode");
        
        {
            check_sum_en_flg = 0;
            sys_conf0_reg.val = 0;
            sys_conf0_reg.bit.RS_SYS = 1;
            KY5532_w_cmd(ADDR_SYS_CONF0, sys_conf0_reg.val);
            
            uint32_t  reset_loop_waite = 0xFFFFF;
            
            while(1){
                sys_conf0_reg.val = KY5532_r_cmd(ADDR_SYS_CONF0);
                printf("\r\n -> ADDR_CONV_CONF0:0x%08X",sys_conf0_reg.val);
                if((sys_conf0_reg.bit.RS_SYS==0) && (sys_conf0_reg.bit.RS_V == 1)){
                    break;
                }
                reset_loop_waite--;
                if(reset_loop_waite == 0){
                    break;
                }
                
               // sys_delay_ms(100);
            }
            if(reset_loop_waite == 0)continue;
            
        #if(CHECK_SUM_EN)
                check_sum_en_flg = 0;
        #endif
            }
        break;
    }while(1);
#endif

    check_sum_en_flg = 0;  ///< 复位完成后，校验功能默认关闭
#if  PRINT_REG_CFG_VAL
    printf("\r\n -> Device Reset  SUCCESS!");
    printf("\r\n ->--- 复位后各配置寄存器参数------------");
    printf("\r\n -> ADDR_CONV_CONF0:0x%08X",KY5532_r_cmd(ADDR_CONV_CONF0));
    printf("\r\n -> ADDR_CONV_CONF1:0x%08X",KY5532_r_cmd(ADDR_CONV_CONF1));
    printf("\r\n -> ADDR_SYS_CONF0 :0x%08X",KY5532_r_cmd(ADDR_SYS_CONF0));
    printf("\r\n -> ADDR_SYS_CONF1 :0x%08X",KY5532_r_cmd(ADDR_SYS_CONF1));
    printf("\r\n -> ADDR_SYS_CONF2 :0x%08X",KY5532_r_cmd(ADDR_SYS_CONF2));
    printf("\r\n -> ");
#endif
    ///< step2.设置配置寄存器
    KY5532_conv_cfg( pgaIndx, speedIndx);
    
    ///< step3.配置寄存器SYS_CONF0
    sys_conf0_reg.val = 0;
//    sys_conf0_reg.bit.HBF_EN = 1; ///< Halfband 滤波器开关选择， 2021-8-12
    sys_conf0_reg.bit.CSHIGH_MODE = 1; ///<  开启片选模式，每次读数据结束后拉高SDO,2021-8-5.
#if(CHECK_SUM_EN)
    sys_conf0_reg.bit.CKS_EN = 1; ///< 开启校验功能
    sys_conf0_reg.bit.HBF_EN = 0;
    KY5532_w_cmd_verify(ADDR_SYS_CONF0 , sys_conf0_reg.val,check_sum_en_flg);
    check_sum_en_flg = 1;
#else
    KY5532_w_cmd_verify(ADDR_SYS_CONF0 , sys_conf0_reg.val,check_sum_en_flg);
#endif
 
    ///< step4.配置寄存器SYS_CONF1

    /*
    CKMODE1, 在51.2kHz的码率设置时，不管发起的是单次转换还是连续转换模式，该位必须配置为1.
    如果发起的是单次转换模式， 800Hz 以上的码率设置时，该位也必须配置为1.
    */
    sys_conf1_reg.val = 0x80010000;
    sys_conf1_reg.bit.CKMODE0 = 1;
    
    ///< 当速度大于800时，CKMODE1置位
    if(speedIndx == adc_speed_indx_51200){
        sys_conf1_reg.bit.CKMODE1 = 1;
    }else{
        sys_conf1_reg.bit.CKMODE1 = 0;
    }
#if USE_INTERNAL_Vref
    sys_conf1_reg.bit.VRS = 1;   ///< 启用内部参考电压
#endif
    KY5532_w_cmd_verify(ADDR_SYS_CONF1 ,  sys_conf1_reg.val,check_sum_en_flg);   
    
    ///< step5.配置寄存器SYS_CONF2
    /*
    DMODE 设置，在数据率 DR 设置为 400Hz 及以下时，建议配置为’10’ ； DR 在 400Hz~6.4kHz 时， 配
    置为’00’;DR 在 12.8kHz 及以上时，配置为’11’。
    */
    sys_conf2_reg.val = 0x00020000;
    
    if(speedIndx <= adc_speed_indx_400){                 
        sys_conf2_reg.bit.DMODE = 2;  // 在数据率 DR 设置为 400Hz 及以下时，建议配置为’10’ ；
    }else if((speedIndx > adc_speed_indx_400) && (speedIndx <= adc_speed_indx_6400)){
        sys_conf2_reg.bit.DMODE = 0;  // DR 在 400Hz~6.4kHz 时， 配置为’00’ 
    }else if((speedIndx > adc_speed_indx_6400) && (speedIndx <= adc_speed_indx_51200)){
        sys_conf2_reg.bit.DMODE = 3;  // DR 在 12.8kHz 及以上时，配置为’11’。
    }else{
        sys_conf2_reg.bit.DMODE = 2;  ///< 设置的速度参数错误时，赋值一个默认参数
    }

    KY5532_w_cmd_verify(ADDR_SYS_CONF2 , sys_conf2_reg.val,check_sum_en_flg );
    

#if 0
//    ///< step6.校准
//    printf("\r\n ->----校准前------------");
//    printf("\r\n ADDR_OS_CH0  :0x%08X",KY5532_r_cmd(ADDR_OS_CH0));
//    printf("\r\n ADDR_GAIN_CH0:0x%08X",KY5532_r_cmd(ADDR_GAIN_CH0));
//    printf("\r\n ADDR_OS_CH1  :0x%08X",KY5532_r_cmd(ADDR_OS_CH1));
//    printf("\r\n ADDR_GAIN_CH1:0x%08X",KY5532_r_cmd(ADDR_GAIN_CH1));
    
    ///< 偏移校准
//    KY5532_offset_selfCalibration();


//    ///< 增益校准
//    KY5532_Gain_Calibration();


//    printf("\r\n ->------------ 校准后 ------------");
//    printf("\r\n ADDR_OS_CH0  :0x%08X",KY5532_r_cmd(ADDR_OS_CH0));
//    printf("\r\n ADDR_GAIN_CH0:0x%08X",KY5532_r_cmd(ADDR_GAIN_CH0));
//    printf("\r\n ADDR_OS_CH1  :0x%08X",KY5532_r_cmd(ADDR_OS_CH1));
//    printf("\r\n ADDR_GAIN_CH1:0x%08X",KY5532_r_cmd(ADDR_GAIN_CH1));
#endif


    ///< step7.选择通道及对应的配置寄寄存器，并启动AD转换
#if 0
    KY5532_conv_start(CONV_CONF00,        // 转换配置选择
                                        //    CONV_CONF00: 使用 CONF0
                                        //    CONV_CONF01: 使用 CONF1
                                        //    CONV_CONF10: 使用 CONF2
                                        //    CONV_CONF11: 使用 CONF3
                    CONV_MOD_NOR_CON);  // 转换模式选择
                                        //    CONV_MOD_NOR_SIN: 正常单次转换模式
                                        //    CONV_MOD_NOR_CON: 正常连续转换模式
                                        //    CONV_MOD_OFF_AUT: Offset 自校准模式
                                        //    CONV_MOD_OFF_SYS: Offset 系统校准模式
                                        //    CONV_MOD_GAN_SYS: Gain 系统校准模式  
#endif

printf("\r\n -> ADDR_SYS_CONF0 :0x%08X",KY5532_r_cmd(ADDR_SYS_CONF0));
#if  PRINT_REG_CFG_VAL
    printf("\r\n #### KY5532 配置参数 ######################################");
    printf("\r\n -> ADDR_CONV_CONF0:0x%08X",KY5532_r_cmd(ADDR_CONV_CONF0));
    printf("\r\n -> ADDR_CONV_CONF1:0x%08X",KY5532_r_cmd(ADDR_CONV_CONF1));
    printf("\r\n -> ADDR_SYS_CONF0 :0x%08X",KY5532_r_cmd(ADDR_SYS_CONF0));
    printf("\r\n -> ADDR_SYS_CONF1 :0x%08X",KY5532_r_cmd(ADDR_SYS_CONF1));
    printf("\r\n ->");
    printf("\r\n -> ADDR_OS_CH0    :0x%08X",KY5532_r_cmd(ADDR_OS_CH0));
    printf("\r\n -> ADDR_GAIN_CH0  :0x%08X",KY5532_r_cmd(ADDR_GAIN_CH0));
    printf("\r\n -> ADDR_OS_CH1    :0x%08X",KY5532_r_cmd(ADDR_OS_CH1));
    printf("\r\n -> ADDR_GAIN_CH1  :0x%08X",KY5532_r_cmd(ADDR_GAIN_CH1));
 
    sys_conf1_reg.val = KY5532_r_cmd(ADDR_SYS_CONF1);
     printf("\r\n -> ADDR_CONV_CONF0:0x%08X",sys_conf1_reg.val);
     sys_conf0_reg.val = KY5532_r_cmd(ADDR_SYS_CONF0);
    printf("\r\n -> ADDR_SYS_CONF0.OGS:%d",sys_conf0_reg.bit.OGS);
    
    KY5532_CONV_CONFx_Typedef  cov_conf_reg;
    uint32_t reg32;
    
    reg32 = KY5532_r_cmd(ADDR_CONV_CONF0);
    printf("\r\n -> ADDR_CONV_CONF0:0x%08X",reg32);
    cov_conf_reg.val = reg32 &0xFFFF;
    printf("\r\n -> ADDR_CONV_CONF00[%04X].CHS:%d    OGSEL:%d",cov_conf_reg.val,cov_conf_reg.bit.CHS,cov_conf_reg.bit.OGSEL);
    cov_conf_reg.val = (reg32>>16) &0xFFFF;
    printf("\r\n -> ADDR_CONV_CONF01[%04X].CHS:%d    OGSEL:%d",cov_conf_reg.val,cov_conf_reg.bit.CHS,cov_conf_reg.bit.OGSEL);
    
    
    reg32 = KY5532_r_cmd(ADDR_CONV_CONF1);
    printf("\r\n -> ADDR_CONV_CONF1:0x%08X",reg32);
    cov_conf_reg.val = reg32&0xFFFF;
    printf("\r\n -> ADDR_CONV_CONF10[%04X].CHS:%d    OGSEL:%d",cov_conf_reg.val,cov_conf_reg.bit.CHS,cov_conf_reg.bit.OGSEL);
    cov_conf_reg.val = (reg32>>16) &0xFFFF;
    printf("\r\n -> ADDR_CONV_CONF11[%04X].CHS:%d    OGSEL:%d",cov_conf_reg.val,cov_conf_reg.bit.CHS,cov_conf_reg.bit.OGSEL);
#endif
    
//    printf("\r\n ->--- 复位后各配置寄存器参数------------");
    printf("\r\n -> @ADDR_CONV_CONF0:0x%08X",KY5532_r_cmd(ADDR_CONV_CONF0));
    printf("\r\n -> @ADDR_CONV_CONF1:0x%08X",KY5532_r_cmd(ADDR_CONV_CONF1));
    printf("\r\n -> @ADDR_SYS_CONF0 :0x%08X",KY5532_r_cmd(ADDR_SYS_CONF0));
    printf("\r\n -> @ADDR_SYS_CONF1 :0x%08X",KY5532_r_cmd(ADDR_SYS_CONF1));
    printf("\r\n -> @ADDR_SYS_CONF2 :0x%08X",KY5532_r_cmd(ADDR_SYS_CONF2));
    printf("\r\n -> ");
    
    return 1;
}

/**
CONV_CONFx是用来存储转换设置的寄存器， 芯片内共有四组转换设置可供调用， 其中
CONV_CONF00包含 CONV_CONF00 与 CONV_CONF01 两组转换设置， 
CONV_CONF01包含 CONV_CONF10 与 CONV_CONF11两组设置。 
具体使用哪组转换设置是由转换命令帧中的 CONV_CONF 来决定。 在每次开始转换之前， 
主控 MCU端需要先将相对应的 CONV_CONFx 寄存器设置好。
*/
void KY5532_conv_cfg(KY553x_PGA_Type pgaIndx,KY553x_SPEED_Type speedIndx)
{
    volatile KY5532_CONV_CONFx_Typedef cfg_reg_val;
    
    
  //  if(pgaIndx > sizeof(pgaTab)) pgaIndx = 0;
    if(speedIndx > (KY553x_SPEED_Type)(sizeof(speedTab))) speedIndx =  (KY553x_SPEED_Type)0;
    
    cfg_reg_val.val = 0;
    cfg_reg_val.bit.TMPEN = 0x00; ///< 温度传感器关闭
    cfg_reg_val.bit.OGSEL = 0x00; ///< 校准寄存器选择，只有在 SYS_CONF0 的OGS=1 时才会有作用；
                                  ///<    0: 选择 OS_CH0/GAIN_CH0   1: 选择 OS_CH1/GAIN_CH1
    cfg_reg_val.bit.CHS   = 0x00; ///< ADC 通道选择   0: 选择通道 0，   1: 选择通道 1
    cfg_reg_val.bit.OD    = 0x00; ///< 输入端开路检测选择
    cfg_reg_val.bit.DLY   = 0x00; ///< 滤波器延时时间选择  0x0:256 ADC clocks   0x1:512  0x2:16  0x3:32
    cfg_reg_val.bit.DR    = speedTab[speedIndx]; ///< ADC 数据输出码率选择
                                  ///<    0x0: 51200 Hz     0x1: 25600 Hz     0x2: 12800 Hz        0x3: 6400 Hz     
                                  ///<    0x4: 3200 Hz      0x5: 1600 Hz      0x6: 800 Hz          0x7: 400 Hz      
                                  ///<    0x8: 200 Hz       0x9: 100 Hz       0xA: 50 Hz           0xB: 25 Hz       
                                  ///<    0xC: 12.5 Hz      0xD~0xF: 6.25 Hz
    cfg_reg_val.bit.GA    =  pgaTab[pgaIndx];   ///< ADC 模拟增益选择   0x0: x64   0x1: x128   0x2: x16    0x3: x32
                                  ///<                    0x4: x4    0x5: x8     0x6: x1     0x7: x2
    
    ///< 设置4个配置寄存器
    ///< 将配置寄存器CONV_CONF00,CONV_CONF01分配给CH0，配置寄存器CONV_CONF10,CONV_CONF11分配给CH1，
    ///< 实际应用过程中可以自由分配）
    cfg_reg_val.bit.CHS   = 0x00; ///< ADC 通道选择   0: 选择通道 0，   1: 选择通道 1
    KY5532_ch_cfg(CONV_CONF00, cfg_reg_val);      
    KY5532_ch_cfg(CONV_CONF01, cfg_reg_val); 
    
    ///< ADD 2021-6-10
  //  cfg_reg_val.bit.GA    =  pgaTab[adc_pga_indx_128];   ///< ADC 模拟增益选择   0x0: x64   0x1: x128   0x2: x16    0x3: x32

    cfg_reg_val.bit.CHS   = 0x01; ///< ADC 通道选择   0: 选择通道 0，   1: 选择通道 1
    KY5532_ch_cfg(CONV_CONF10, cfg_reg_val);                           
    KY5532_ch_cfg(CONV_CONF11, cfg_reg_val);  
}

/**
 * 启动通道0单次转换
 */
void KY5532_ch0_single_start(void)
{
    ///< 配置寄存器CONV_CONFx需要根据函数KY5532_conv_cfg()中的配置合理选择
    KY5532_conv_start(CONV_CONF00,     // 转换配置选择
                                        //    CONV_CONF00: 使用 CONF0
                                        //    CONV_CONF01: 使用 CONF1
                                        //    CONV_CONF10: 使用 CONF2
                                        //    CONV_CONF11: 使用 CONF3
                    CONV_MOD_NOR_SIN);  // 转换模式选择
                                        //    CONV_MOD_NOR_SIN: 正常单次转换模式
                                        //    CONV_MOD_NOR_CON: 正常连续转换模式
                                        //    CONV_MOD_OFF_AUT: Offset 自校准模式
                                        //    CONV_MOD_OFF_SYS: Offset 系统校准模式
                                        //    CONV_MOD_GAN_SYS: Gain 系统校准模式
}

/* 以下为转换启动函数 =========================================================================================================*/

/**
 * 启动通道0连续转换
 */
void KY5532_ch0_continue_start(void)
{
    ///< 配置寄存器CONV_CONFx需要根据函数KY5532_conv_cfg()中的配置合理选择
    KY5532_conv_start(CONV_CONF00,CONV_MOD_NOR_CON);  
}

/**
 * 启动通道1单次转换
 */
void KY5532_ch1_single_start(void)
{
    ///< 配置寄存器CONV_CONFx需要根据函数KY5532_conv_cfg()中的配置合理选择
     KY5532_conv_start(CONV_CONF10,CONV_MOD_NOR_SIN);
}

/**
 * 启动通道1连续转换
 */
void KY5532_ch1_continue_start(void)
{
    ///< 配置寄存器CONV_CONFx需要根据函数KY5532_conv_cfg()中的配置合理选择
     KY5532_conv_start(CONV_CONF10,CONV_MOD_NOR_CON);
}

void KY5532_SPI_Reset(void)
{
    delay_nop(1000);
    KY5532_cs_low();
    delay_nop(1000);
    KY5532_send_byte(0x00);
    KY5532_send_byte(0xA5);
    KY5532_send_byte(0xFF);
    KY5532_send_byte(0x5A);
    delay_nop(1000);
    KY5532_cs_high();
}

/////< 从连续转换模式进入待命状态。
//void Entry_WaitState(void)
//{
//    KY5532_cs_low();
//    ///< 判断AD转换是否完成
//    while(KY5532_DO_Boolean() == KY_TRUE ){
//    }
//    KY5532_cmd_send(NEXT_STATE_WAIT); 
//    KY5532_cs_high();
//}

/**
 * 立即停止转换
 */
void KY5532_Convert_Stop(void)
{
    KY5532_cs_low();

//    while(KY5532_DO_Boolean() == KY_TRUE ){
//        ; ///< 此处做超时退出
//    }

    KY5532_send_byte(NEXT_STATE_WAIT);
    
    KY5532_cs_high();

    KY5532_SPI_Reset();
}

/* 以下为读AD值函数 =========================================================================================================*/
/**
 * 读取AD值
 * 返回值，
 *          0x00, 没有读取到数据
 *          0x01，通道1读取到有效数据
 *          0x03，通道2读取到有效数据   
 *          0xFF，ADC被强制初始化
 */
//volatile uint8_t spi_reset_send_flg = 0;
uint8_t KY5532_read_adcVal(int32_t *value)
{
    int32_t temp32;
    uint32_t adcDat;
    uint8_t ret=0;
    

    //  if(spi_reset_send_flg)        KY5532_SPI_Reset(); ///< 测试SPI复位命令是否会终止连续转换，答案是肯定的

    ///< @提醒：如果使用中断方式读取数据强烈建议使用低电平触发中断的方式。
    ///<        如果只能使用使用下降沿触发中断读取数据时，强烈建议启用超时查询功能
#if USE_TIMEOUT_CHECK
    ///< 监控KY5530芯片是否超时
    if(KY5532_getVal_timeout_ms > 5000){ ///< 设置超时时间为1秒，1秒内没读到AD值则重新初始化KY5532
        ///< 如果连续3次超时，建议重新初始化芯片
        if(read_adc_timeout_cnt > 3){
            KY5532_Convert_Stop();
            
            ///< ADC初始化
#if USE_SELF_DEFINE_INIT
            adc_init();      
#else
            KY5532_Init();
#endif
            read_adc_timeout_cnt = 0; ///< 清除计数
            return 0xFF; ///< ADC初始化
            ///< 添加启动AD转换的代码，单次转换或连续转换
            ///< ...
            ///< KY5532_ch0_continue_start(); 
            ///< KY5532_ch1_continue_start();
        }else{
             KY5532_cs_low();
//            ///< 判断AD转换是否完成
//            while(KY5532_DO_Boolean() == KY_TRUE ){
//                 KY5532_cs_high();
//                return 0;
//            }
            ///< 强制读一次AD值,读出的结果作为无效值，不做处理
            KY5532_send_byte(0x56);
            KY5532_read4byte(&adcDat);
            read_adc_timeout_cnt++;
            KY5532_cs_high();
        }
        ///< 清除计时
        KY5532_getVal_timeout_ms = 0;
        return 0;
    }
#endif
    KY5532_cs_low();
    ///< 判断AD转换是否完成
    while(KY5532_DO_Boolean() == KY_TRUE ){
         KY5532_cs_high();
        return 0;
    }
    
#if USE_TIMEOUT_CHECK
    ///< 超时计时归零
     KY5532_getVal_timeout_ms = 0;
#endif
     
    KY5532_cs_low();
    
    // 8 个转换时钟
    KY5532_send_byte(0x56);
    
    // 连续读4个字节
    ret = KY5532_read4byte(&adcDat);
    
    ///<----------- 2021-8-5-----------------------------------
    ///< 注意：此处拉高一次CS管脚以同步SPI时序，SYS_CONFIGO寄存器的 CSHIGH_MODE位必须置一，否则ADC连续转换会停止
    KY5532_cs_high();
    KY5532_nop();
    KY5532_cs_low();
    
#if CHECK_SUM_EN
    if(ret == 0){
        ///< 检测到数据校验错误
        if(KY5532_check_failed){
            KY5532_SPI_Reset();
            printf("\r\n #ADDR_SYS_CONF0 :0x%08X",KY5532_r_cmd(ADDR_SYS_CONF0));
            KY5532_ch0_continue_start();
        }
        return 0;  ///< 校验错误，转换失败
    }
#endif
   
    ret = 1; // 读到有效AD值
     
    /*
    ADC 转换结果 bit[31:1]， bit[0]为当次数据所的信号
    通道。 bit[31:1]为 31 位有符号补码，高 2 位为符号
    位，将 bit[0]补 0 后， bit [31:0]的结果:
    */
    temp32 = (adcDat & 0X7FFFFFFF)>>1; ///< 移动屏蔽掉1位符号位，并将最高位的符号位清零，此时bit29位为符号位
    *value = (temp32^(0x20000000)) - (0x20000000);
    
    ///< AD转换结果的bit0为通道指示位
    if(adcDat & 1ul){
       ret |= 1<<1;
    } 
    return ret; // 读到数据
}


/**
 * 单次转换时，读取相应通道的AD值
 */
uint8_t KY5532_Get_ADCVal_once(uint8_t chx, int32_t *adcDat)
{
    KY5532_cs_low();
    if(chx == 0){
        KY5532_ch0_single_start();
        while(KY5532_read_adcVal(adcDat) != 1) {; };
        
    }else{
        KY5532_ch1_single_start();        
        while(KY5532_read_adcVal(adcDat) != 3) {;};
    }
    KY5532_cs_high();
    return 1;
}

/**
 *
 */
void KY5532_Init(void)
{
	// KY5532初始化
	if(lvbocanshu == 1)
	{
		KY5532_Config(adc_pga_indx_32,adc_speed_indx_25);					
	}
	else if(lvbocanshu == 2)
	{
		KY5532_Config(adc_pga_indx_32,adc_speed_indx_50);					
	}
	else if(lvbocanshu == 3)
	{
		KY5532_Config(adc_pga_indx_32,adc_speed_indx_100);					
	}
	else if(lvbocanshu == 4)
	{
		KY5532_Config(adc_pga_indx_8,adc_speed_indx_800);					
	}
	
//	    KY5532_Config(adc_pga_indx_32, ///< adc_pga_indx_1
//                                    ///< adc_pga_indx_2
//                                    ///< adc_pga_indx_64
//                                    ///< adc_pga_indx_128
//                                    ///< adc_pga_indx_4
//                                    ///< adc_pga_indx_8
//                                    ///< adc_pga_indx_16
//                                    ///< adc_pga_indx_32
//                adc_speed_indx_100); ///< adc_speed_indx_12_5
//                                    ///< adc_speed_indx_6_25
//                                    ///< adc_speed_indx_25 
//                                    ///< adc_speed_indx_50 
//                                    ///< adc_speed_indx_100 
//                                    ///< adc_speed_indx_200
//                                    ///< adc_speed_indx_400
//                                    ///< adc_speed_indx_800
//                                    ///< adc_speed_indx_1600 //40k
//                                    ///< adc_speed_indx_3200 //9k
//                                    ///< adc_speed_indx_6400 //3.5k
//                                    ///< adc_speed_indx_12800 //3.5k
//                                    ///< adc_speed_indx_25600 //3.5k
//                                    ///< adc_speed_indx_51200 //3.5k
}



void KY5530_ALL_Init(void)
{
	ky5530_channel = 1;
	KY5532_Init();
	KY5532_ch0_continue_start();
	ky5530_channel = 2;
	KY5532_Init();
	KY5532_ch0_continue_start();
	ky5530_channel = 3;
	KY5532_Init();
	KY5532_ch0_continue_start();
	ky5530_channel = 4;
	KY5532_Init();
	KY5532_ch0_continue_start();
	ky5530_channel = 5;
	KY5532_Init();
	KY5532_ch0_continue_start();
	ky5530_channel = 6;
	KY5532_Init();
	KY5532_ch0_continue_start();
	
}









































    
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////                                         ////////////////////////////////////////// 
////////////////////               以下为测试函数            //////////////////////////////////////////
////////////////////                                         //////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#define KY5532_TEST_ENABLE     1  ///< 使能 KY5532测试函数


#if KY5532_TEST_ENABLE

//static void printf_adc_bit_24(uint32_t adcDat)
//{
//    printf("%d%d%d%d %d%d%d%d %d%d%d%d %d%d%d%d %d%d%d%d %d%d%d%d",
//             (adcDat&(1ul<<23)) ? 1 : 0,
//             (adcDat&(1ul<<22)) ? 1 : 0,
//             (adcDat&(1ul<<21)) ? 1 : 0,
//             (adcDat&(1ul<<20)) ? 1 : 0,
//             (adcDat&(1ul<<19)) ? 1 : 0,
//             (adcDat&(1ul<<18)) ? 1 : 0,
//             (adcDat&(1ul<<17)) ? 1 : 0,
//             (adcDat&(1ul<<16)) ? 1 : 0,
//             (adcDat&(1ul<<15)) ? 1 : 0,
//             (adcDat&(1ul<<14)) ? 1 : 0,
//             (adcDat&(1ul<<13)) ? 1 : 0,
//             (adcDat&(1ul<<12)) ? 1 : 0,
//             (adcDat&(1ul<<11)) ? 1 : 0,
//             (adcDat&(1ul<<10)) ? 1 : 0,
//             (adcDat&(1ul<<9)) ? 1 : 0,
//             (adcDat&(1ul<<8)) ? 1 : 0,
//             (adcDat&(1ul<<7)) ? 1 : 0,
//             (adcDat&(1ul<<6)) ? 1 : 0,
//             (adcDat&(1ul<<5)) ? 1 : 0,
//             (adcDat&(1ul<<4)) ? 1 : 0,
//             (adcDat&(1ul<<3)) ? 1 : 0,
//             (adcDat&(1ul<<2)) ? 1 : 0,
//             (adcDat&(1ul<<1)) ? 1 : 0,
//             (adcDat&(1ul<<0)) ? 1 : 0
//         );
//    
//}


/* 以下为测试函数 =========================================================================================================*/
/**
 *  主要验证KY5532 速度和PGA功能，及在无滤波情况下的无噪声位
 */
void KY5532_speedPga_test(void)
{
    uint8_t ret;
    int32_t cnt;//,cnt0;
//    int32_t adcValBuf_speed[128];
    int32_t adcDat;
    
    KY5532_Init();

test_start:    
     ///<--------------------------------------------------------- 连续转换模式
#if     1    
    KY5532_ch0_continue_start();     ///< 通道0连续转换测试
#else         
    KY5532_ch1_continue_start();    ///< 通道1连续转换测试
#endif

    cnt = 0;
    while(1){
        ret = KY5532_read_adcVal(&adcDat);
        if(ret == 0)continue;
        if(ret == 0xFF)goto test_start;
        adcDat = adcDat/(1<<6);

//        DebugCom_SendByte(0xAA);
//        DebugCom_SendByte(0xBB);
//        DebugCom_SendByte(4);

//        ///< 打印滤波前的数据
//        DebugCom_SendByte((adcDat>>24)&0xFF);
//        DebugCom_SendByte((adcDat>>16)&0xFF);
//        DebugCom_SendByte((adcDat>>8)&0xFF);
//        DebugCom_SendByte((adcDat>>0)&0xFF);
  
//    //    DebugCom_SendByte(0);
//    
//    #if 0
//    adcValBuf_speed[cnt] = adcDat;
//        if(cnt <128)continue;
//        cnt++;

//        for(uint8_t i=0;i<128;i++){
//            printf("\r\n");
//            adcDat = adcValBuf_speed[i];
//            printf_adc_bit_24(adcDat);
//        }
//        cnt = 0;
//        printf("\r\n-------------------------------------------------------------------");
//        #endif
//        continue;
    }
}

#define VAL_NUM     7  ///< 设置软件滤波采样的个数


int32_t ky5532_test_adcDat;

///< 1. KY5532片选功能
///< 2. 测试KY5532的精度
void KY5532_test_cs(void)
{
    volatile uint8_t chx,clkTest_cnt;
    int32_t valMax, valMin,valSum,valAvrg;
    int32_t adcValBuf[VAL_NUM],cnt;//,cnt0;
 
    // KY5532初始化
    KY5532_Init();
 ///<--------------------------------------------------------- 连续转换模式
#if     1    
    KY5532_ch0_continue_start();     ///< 通道0连续转换测试
#else         
    KY5532_ch1_continue_start();    ///< 通道1连续转换测试
#endif

    // KY5532_speedPga_test();
    clkTest_cnt = 0;
    uint8_t ret = 0;
    while(1){

        KY5532_cs_low();
        ret = KY5532_read_adcVal(&ky5532_test_adcDat);
        KY5532_cs_high();
        
        if(ret == 0) continue; ///< 为读到数据
        
        ky5532_test_adcDat = ky5532_test_adcDat/(2<<10);
        adcValBuf[cnt%VAL_NUM] = ky5532_test_adcDat;
        
        valSum = 0;
        valMax = 0;
        valMin = 0x7FFFFFFF;
        for(uint8_t i=0;i<VAL_NUM;i++){
            valSum += adcValBuf[i];
            if(valMax <  adcValBuf[i]) valMax = adcValBuf[i];
            if(valMin >  adcValBuf[i]) valMin = adcValBuf[i];
        }
        ///< 减去最大值和最小值，再求平均值
        valAvrg = (valSum-valMax-valMin)/(VAL_NUM-2);
        printf("\r\n  %d   ",valAvrg );
        cnt++;
    }
}

/**
 * KY5532单次转换及通道切换测试
 */
void KY5532_test_once(void)
{
    volatile uint8_t chx,clkTest_cnt;
    int32_t adcDat;

    // KY5532初始化
     KY5532_Init();
    
    clkTest_cnt = 0;

    while(1){
        ///< 单次转换，两个通道交替采样测试
       printf("\r\n -> ");
        KY5532_Get_ADCVal_once(0, &adcDat);
        printf("  CH0: %8X                          ",adcDat/(2<<6));

        KY5532_Get_ADCVal_once(1, &adcDat);
        printf(" CH1:  %8X    ",adcDat/(2<<6));
        continue;
    }
}


/**
 *  KY5532在连续转换及通道切换测试
 */
void KY5532_CHx_Change_Continue_Test(void)
{
    uint8_t ret;
    int32_t cnt;//,cnt0;
    int32_t CH0_Buf[128];
    int32_t CH1_Buf[128];
    int32_t adcDat;
    
     KY5532_Init();
 
    const uint8_t read_adc_num = 16;
    cnt = 0;
    
    while(1){
loop_start:
        KY5532_ch0_continue_start();     ///< 通道0连续转换测试
        for(cnt=0;cnt<read_adc_num;cnt++){ ///< 采集32个AD值
            ///< 读取一个有效AD值
            ret = 0;
            while(1){
                ret = KY5532_read_adcVal(&adcDat);
                if(ret == 0xFF) {
                    KY5532_Convert_Stop(); ///< 停止转换
                    goto loop_start;
                }
                if(ret) break;
            }
            if(ret != 0x01) {
                printf("\r\n KY5532 CH0 error ! %02X",ret );  ///< ADC通道错了
            }
            CH0_Buf[cnt] = adcDat / (2<<6);
        }
        ///< 打印出AD值
        printf("    #### CH0: "); 
        for(cnt=0;cnt<read_adc_num;cnt++){ ///< 采集32个AD值
            printf("%8X ",CH0_Buf[cnt]);  ///< ADC通道错了
        }
        KY5532_Convert_Stop(); ///< 停止转换
        
        //---------------------------------------------------------------------------------------
        KY5532_ch1_continue_start();     ///< 通道0连续转换测试

        for(cnt=0;cnt<read_adc_num;cnt++){ ///< 采集32个AD值
            ///< 读取一个有效AD值
            ret = 0;
            while(1){
                ret = KY5532_read_adcVal(&adcDat);
                if(ret == 0xFF) {
                    KY5532_Convert_Stop(); ///< 停止转换
                    goto loop_start;
                }
                if(ret) break;
            }
            if(ret != 0x03) {
               printf("\r\n KY5532 CH1 error !  %02X",ret);  ///< ADC通道错了
            }
            CH1_Buf[cnt] = adcDat / (2<<6);
        }
        ///< 打印出AD值
        printf("\r\n-> CH1: "); 
        for(cnt=0;cnt<read_adc_num;cnt++){ ///< 采集32个AD值
            printf("%8X ",CH1_Buf[cnt]);  ///< ADC通道错了
        }
        KY5532_Convert_Stop(); ///< 停止转换
 
    }
}



/**
 *  KY5532校准测试
 *  1. 偏移校准， 零点信号状态，连续采集多次AD值取平均，然后将该值写入寄存器 ADDR_OS_CHx
 *  2. 增益校准，必须先执行偏移校准，然后再进行增益校准。先输入一个标准电压信号，电压信号的幅度建议为满度电压的20%~50%之间，
 *     记录此时采样的AD值，先根据标准信号计算出应该输出的AD理论值，根据公式计算增益校准系数 = 理论值（期望值） / 采样值 * 0x02000000
 *     将此校准系数写入寄存器ADDR_GAIN_CHx
 *     公式最后乘以0x02000000的原因，请参考数据手册中寄存器GAIN_CHx的介绍。
 */
volatile int32_t offset=0,gain_ch_tmp=0, gain_ch=0,gain_targ=0; 
uint8_t break_key;
void KY5532_Calibrate_Test(void)
{
    #define TEST_BUFFER_SIZE    8
    uint8_t ret;
    int32_t cnt;
    int32_t CH0_Buf[TEST_BUFFER_SIZE];
    int32_t adcDat;
    uint32_t read_adc_num;
    uint8_t maskBit = 10;
    
    KY5532_Init();

    ///<------------------------------------------------------------------------------------------------------ 获取采样零点偏移量
    read_adc_num = TEST_BUFFER_SIZE*2; ///< 读取测试数据的个数
    KY5532_ch0_continue_start();     ///< 通道0连续转换测试
    for(cnt=0;cnt<read_adc_num;cnt++){ ///< 采集32个AD值
        ret = 0;
        while(1){
            ret = KY5532_read_adcVal(&adcDat);
            if(ret) break;
        }
        CH0_Buf[cnt%TEST_BUFFER_SIZE] = adcDat; /// / (2<<6);
    }
    KY5532_Convert_Stop(); ///< 停止转换
    offset = 0;
    
    for(cnt=0;cnt<TEST_BUFFER_SIZE;cnt++){ 
        offset += CH0_Buf[cnt];
    }
    ///< 多次采样值取平均，作为偏移参数写入偏移校准寄存器
    offset = offset /  TEST_BUFFER_SIZE;
    printf("\r\n -> 零点offset:%8X ,%d",CH0_Buf[cnt],CH0_Buf[cnt]/(1<<maskBit));  ///< ADC通道错了
    
    ///<----------------------------------------------------------------------------------------------------- 获取增益校准系数
    ///< 执行下一步需要提前给一个满度20%的信号电压
    KY5532_ch0_continue_start();     ///< 通道0连续转换测试
    for(cnt=0;cnt<read_adc_num;cnt++){ ///< 采集32个AD值
        ret = 0;
        while(1){
            ret = KY5532_read_adcVal(&adcDat);
            if(ret) break;
        }
        if(adcDat - offset < offset*2/10){
            cnt = 0;
            continue;
        }
        CH0_Buf[cnt%TEST_BUFFER_SIZE] = adcDat; /// / (2<<6);
    }
     KY5532_Convert_Stop(); ///< 停止转换
    
    gain_ch_tmp = 0;
    ///< 多次采样取平均，根据规格谁给定公式计算增益校准系数，存入指定寄存器
    for(cnt=0;cnt<TEST_BUFFER_SIZE;cnt++){ ///< 采集32个AD值
        gain_ch_tmp += CH0_Buf[cnt];
    }
    gain_ch_tmp = gain_ch_tmp /  TEST_BUFFER_SIZE;
    gain_targ = gain_ch_tmp * 12 / 10;  ///< 模拟出一个目标值，假设增益变差1.2倍
    
    ///< 计算增益系数
    gain_ch = (int32_t)((double)gain_targ/(double)gain_ch_tmp * 0x02000000);

    ///<----------------------------------------------------------------------------------------------------- 采样零点偏移量
    printf("\r\n -> 系统offset校准  "); 
    KY5532_w_cmd(ADDR_OS_CH0 ,  offset<<1);
    
    ///< 读取测试校准后的AD数据
    KY5532_ch0_continue_start();     ///< 通道0连续转换测试
    break_key = 0;
    while(1){
        ret = KY5532_read_adcVal(&adcDat);
        if(!ret) continue;
        if(break_key ) break;
        printf("\r\n ->AD:%8X ,%d",adcDat,adcDat/(1<<maskBit));  ///< ADC通道错了
    }
    break_key = 0;
    KY5532_Convert_Stop(); ///< 停止转换

    ///<----------------------------------------------------------------------------------------------------- 系统增益校准    
    printf("\r\n -> 系统增益校准  "); 
    KY5532_w_cmd(ADDR_GAIN_CH0 ,  gain_ch); ///< 增益校准系数，计算公式：输入满度20%~50%电压信号时的期望值 / 采样值 * 0x02000000
                                            ///< 公式最后乘以0x02000000的原因，请参考数据手册中寄存器GAIN_CHx的介绍
    
    KY5532_w_cmd(ADDR_D_TARG ,  gain_targ); ///< 期望值寄存器不是必须写入的，只有使用自动校准命令功能时才需要预先写入期望值。

    ///< 读取测试增益校准后的AD数据
    KY5532_ch0_continue_start();     ///< 通道0连续转换测试
    break_key = 0;
    while(1){
        ret = KY5532_read_adcVal(&adcDat);
        if(!ret) continue;
        if(break_key ) break;
        printf("\r\n ->AD:%8X ,%d",adcDat,adcDat/(1<<maskBit));  ///< ADC通道错了
    }
    break_key = 0;
    KY5532_Convert_Stop(); ///< 停止转换
    
    printf("\r\n -> 测试结束"); 
    while(1){
        
    }
    
}


void KY5532_test(void)
{
//    KY5532_test_once();   ///< ADC单次转换测试
//    KY5532_test_cs();       ///< 片选和简单滤波测试
    KY5532_speedPga_test(); ///<  KY5532 速度和PGA测试
//    KY5532_Calibrate_Test(); ///< KY5532校准测试
    
//   KY5532_CHx_Change_Continue_Test(); ///<  KY5532在连续转换模式下切换转换通道
}

#endif  ///< end of " #define KY5532_TEST_ENABLE "
#endif  ///< end of " #define ADC_IC_KY5532 " 




