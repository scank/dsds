#include "midfilter.h"
#include "arm_const_structs.h"

//#define MidFilterOrder  16     				/* 滤波阶数 */
//#define ZeroSize        MidFilterOrder

//float32_t DstDate[TEST_LENGTH_SAMPLES];     /* 滤波后数据 */

//static float32_t SortData[MidFilterOrder];  /* 滤波排序 */ 
//static float32_t TempDate[ZeroSize + TEST_LENGTH_SAMPLES +ZeroSize] = {0}; /* 滤波阶段用到的临时变量 */




///*
//*********************************************************************************************************
//*	函 数 名: MidFilterRT
//*	功能说明: 中值滤波器，用于逐个数据实时滤波。
//*	形    参: pSrc 源数据地址。
//*             pDst 滤波后数据地址。
//*             ucFlag 1表示首次滤波，后面继续滤波，需将其设置为0。
//*             order 至少2阶。
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void MidFilterRT(float32_t *pSrc, float32_t *pDst, uint8_t ucFlag, uint32_t order)
//{
//	arm_sort_instance_f32 S;
//	uint32_t N, i;
//	
//	static uint32_t CountFlag = 0;
//	
//	
//	S.dir = ARM_SORT_ASCENDING;
//	S.alg = ARM_SORT_QUICK;
//	
//	N = order / 2;
//	
//	/* 首次滤波先清零 */
//	if(ucFlag == 1)
//	{
//		CountFlag = 0;
//	}
//	
//	/* 填充数据 */
//	if(CountFlag < order)
//	{
//		TempDate[CountFlag] = pSrc[0];
//		CountFlag++;
//	}
//	else
//	{
//		for(i =0; i < order - 1; i++)
//		{
//			TempDate[i] = TempDate[1 + i];  
//		}
//		TempDate[order - 1] = pSrc[0];
//	}
//	
//	/* 排序 */
//	arm_sort_f32(&S, &TempDate[0], &SortData[0], order);
//	
//	/* 奇数 */
//	if(N)
//	{
//		pDst[0] = SortData[N];
//	}
//	/* 偶数 */
//	else
//	{
//		pDst[0] = (SortData[N] + SortData[N-1])/2;
//	}
//}


///*
//*********************************************************************************************************
//*	函 数 名: MidFilterOneByOneTest
//*	功能说明: 逐个数据滤波测试
//*	形    参: 无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void MidFilterOneByOneTest(void)
//{
//	float32_t  *inputF32, *outputF32;
//	
//	//inputF32 = (float32_t  *)&testdata[0];
//	outputF32 = &DstDate[0];
//	
//	/* 从头开始，先滤第1个数据 */
//	MidFilterRT(inputF32 , outputF32, 1, MidFilterOrder);
//	
//	/* 逐次滤波后续数据 */
//	for(int i = 1; i < TEST_LENGTH_SAMPLES; i++)
//	{
//		MidFilterRT(inputF32 + i , outputF32 + i, 0, MidFilterOrder);
//	}
//	
////	for(int i = 0; i < TEST_LENGTH_SAMPLES; i++)
////	{
////		printf("%f, %f\r\n", testdata[i], DstDate[i]);
////	}
//}





