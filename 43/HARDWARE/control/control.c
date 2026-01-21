#include "KY5532.h"
#include "control.h"
#include "kalman.h"
#include "protocol.h"
#include "my_uart.h"


uint8_t ky5530_channel =1;

int32_t ky1_value;
int32_t ky2_value;
int32_t ky3_value;
int32_t ky4_value;
int32_t ky5_value;
int32_t ky6_value;



uint8_t control_ky5532_readADC(uint8_t channel)
{
	uint8_t returned_value;
	switch(channel)
	{
		case 1:
			ky5530_channel = 1;
			returned_value = KY5532_read_adcVal(&ky1_value);
						break;
		case 2:
			ky5530_channel = 2;
			returned_value = KY5532_read_adcVal(&ky2_value);

						break;
		case 3:
			ky5530_channel = 3;
			returned_value = KY5532_read_adcVal(&ky3_value);

						break;
		case 4:
			ky5530_channel = 4;
			returned_value = KY5532_read_adcVal(&ky4_value);

						break;
		case 5:
			ky5530_channel = 5;
			returned_value = KY5532_read_adcVal(&ky5_value);

						break;
		case 6:
			ky5530_channel = 6;
			returned_value = KY5532_read_adcVal(&ky6_value);

						break;
		
		default:
			returned_value = 0;
						break;
	}
	
	return returned_value;
}



int32_t filter(void)
{
	char count,i,N = 3;
	int32_t Value_max,Value_min,sum = 0;
	int32_t Value_buf[N];

	for(count=0;count<N;count++)
	{
		
		control_ky5532_readADC(1);
		
		kalman_filter(&KY1_Kalman,ky1_value);
		Value_buf[count]= KY1_Kalman.x;
//		Value_buf[count]= KY5532_read_adcVal(value);
//		kalman_filter(&KY1_Kalman,ky1_value);
		
		SendPacket_NowValue(Channal01Address1Byte, ky1_value,USART1);
		SendPacket_Target(Channal01Address1Byte,KY1_Kalman.x,USART1);
		
	}

	Value_max = Value_buf[0];
	Value_min = Value_buf[0];

	for(i=0;i<N;i++)
	{
		if(Value_buf[i]>Value_max)
		{
			Value_max = Value_buf[i];
		}
		if(Value_buf[i]<Value_min)
		{
			Value_min = Value_buf[i];
		}
		sum += Value_buf[i];
	}
   sum -= Value_max;
   sum -= Value_min;
	
   	SendPacket_Target(Channal02Address1Byte, (sum/(N-2)),USART1);
	SendPacket_NowValue(Channal02Address1Byte,-1000,USART1);
	
   return (sum/(N-2));
}

// 定义滑动窗口的大小
#define WINDOW_SIZE 10
#define WINDOW_SIZE1 50
#define WINDOW_SIZE2 150
#define WINDOW_SIZE3 250

// 初始化滑动窗口
int32_t window[WINDOW_SIZE];
int32_t window1[WINDOW_SIZE1];
int32_t window2[WINDOW_SIZE2];
int32_t window3[WINDOW_SIZE3];

// 窗口内数据的索引
int n = 0;
int n1 = 0;
int n2 = 0;
int n3 = 0;
// 窗口内数据的总和
int32_t sum = 0;
int32_t sum1 = 0;
int32_t sum2 = 0;
int32_t sum3 = 0;
// 均值滤波函数
int32_t meanFilter(int32_t newData) {
    // 减去即将被替换的数据
    sum -= window[n];
    // 将新数据存入窗口
    window[n] = newData;
    // 加上新数据
    sum += newData;
    // 更新索引，实现循环存储
    n = (n + 1) % WINDOW_SIZE;

    // 计算均值
    return sum / WINDOW_SIZE;
}
int32_t meanFilter1(int32_t newData) {
    // 减去即将被替换的数据
    sum1 -= window1[n1];
    // 将新数据存入窗口
    window1[n1] = newData;
    // 加上新数据
    sum1 += newData;
    // 更新索引，实现循环存储
    n1 = (n1 + 1) % WINDOW_SIZE1;

    // 计算均值
    return sum1 / WINDOW_SIZE1;
}
int32_t meanFilter2(int32_t newData) {
    // 减去即将被替换的数据
    sum2 -= window2[n2];
    // 将新数据存入窗口
    window2[n2] = newData;
    // 加上新数据
    sum2 += newData;
    // 更新索引，实现循环存储
    n2 = (n2 + 1) % WINDOW_SIZE2;

    // 计算均值
    return sum2 / WINDOW_SIZE2;
}
int32_t meanFilter3(int32_t newData) {
    // 减去即将被替换的数据
    sum3 -= window3[n3];
    // 将新数据存入窗口
    window3[n3] = newData;
    // 加上新数据
    sum3 += newData;
    // 更新索引，实现循环存储
    n3 = (n3 + 1) % WINDOW_SIZE3;

    // 计算均值
    return sum3 / WINDOW_SIZE3;
}

// 限幅滤波函数
// 输入：new_data 当前输入的数据，last_data 上一次的数据，limit 最大允许变化量
// 输出：经过限幅滤波后的数据
int32_t amplitudeLimitFilter(int32_t new_data, int32_t last_data, int32_t limit) {
    if (new_data - last_data > limit) {
        return last_data + limit;
    } else if (last_data - new_data > limit) {
        return last_data - limit;
    }
    return new_data;
}