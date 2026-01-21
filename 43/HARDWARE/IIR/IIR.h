#ifndef __iir_H
#define __iir_H

#include "arm_math.h"
#include "arm_const_structs.h"


#define IIRnumStages  2                /* 2阶IIR滤波的个数 */
#define TEST_LENGTH_SAMPLES  1    /* 采样点数 */
#define BLOCK_SIZE           1    /* 调用一次arm_biquad_cascade_df1_f32处理的采样点个数 */

extern float32_t ScaleValue;
extern arm_biquad_casd_df1_inst_f32 S1,S2,S3,S4,S5,S6;//初始化结构体
extern float32_t KY1_IIR_out,KY2_IIR_out,KY3_IIR_out,KY4_IIR_out,KY5_IIR_out,KY6_IIR_out;
extern float32_t IIRCoeffs32LP[5*IIRnumStages];

void arm_iir_all_init(void);


#endif


