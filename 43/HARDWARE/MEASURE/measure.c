#include "measure.h"
#include "control.h"
#include "kalman.h"

uint8_t measure_flag = 0;

float32_t measure_calibration(uint8_t i)//测量前校准获取基准
{
	int mea_i, mea_j;
	float64_t mea_value = 0;
	float32_t all_mea_value[100];
	
	for(mea_i = 0;mea_i<20;mea_i++)//得到十次平均
	{
		for(mea_j = 0;mea_j < 100;mea_j++)
		{
			control_ky5532_readADC(i);
			delay_us(10);
			switch(i)
			{
				case 1:
					kalman_filter(&KY1_Kalman,ky1_value);
					mea_value += KY1_Kalman.x;break;
				case 2:
					kalman_filter(&KY2_Kalman,ky2_value);
					mea_value += KY2_Kalman.x;break;
				case 3:
					kalman_filter(&KY3_Kalman,ky3_value);
					mea_value += KY3_Kalman.x;break;
				case 4:
					kalman_filter(&KY4_Kalman,ky4_value);
					mea_value += KY4_Kalman.x;break;
				case 5:
					kalman_filter(&KY5_Kalman,ky5_value);
					mea_value += KY5_Kalman.x;break;
				case 6:
					kalman_filter(&KY6_Kalman,ky6_value);
					mea_value += KY6_Kalman.x;break;
			}
		}
		all_mea_value[mea_i] = 0;
		mea_value = 0;
	}
	for(mea_i = 0;mea_i<20;mea_i++)//得到十次平均
	{
		for(mea_j = 0;mea_j < 1000;mea_j++)
		{
			delay_us(1);
			control_ky5532_readADC(i);
			switch(i)
			{
				case 1:
					kalman_filter(&KY1_Kalman,ky1_value);
					mea_value += KY1_Kalman.x/1000.0f;break;
				case 2:
					kalman_filter(&KY2_Kalman,ky2_value);
					mea_value += KY2_Kalman.x/1000.0f;break;
				case 3:
					kalman_filter(&KY3_Kalman,ky3_value);
					mea_value += KY3_Kalman.x/1000.0f;break;
				case 4:
					kalman_filter(&KY4_Kalman,ky4_value);
					mea_value += KY4_Kalman.x/1000.0f;break;
				case 5:
					kalman_filter(&KY5_Kalman,ky5_value);
					mea_value += KY5_Kalman.x/1000.0f;break;
				case 6:
					kalman_filter(&KY6_Kalman,ky6_value);
					mea_value += KY6_Kalman.x/1000.0f;break;
			}
		}
		all_mea_value[mea_i] = mea_value;
		mea_value = 0;
	}
	
	for(mea_i = 0;mea_i<20;mea_i++)//10次平均值
	{
		mea_value += all_mea_value[mea_i];
	}
	return (float32_t)(mea_value/20.0f);
	
}



