#ifndef __kalman_H
#define __kalman_H

#include "main.h"

//1d卡尔曼滤波
typedef struct {
    float32_t x;  // state 
    float A;  // x(n)=A*x(n-1)+u(n),u(n)~N(0,q) 
    float H;  // z(n)=H*x(n)+w(n),w(n)~N(0,r)   
    float q;  // process(predict) noise convariance 协方差
    float r;  // measure noise convariance 
    float p;  // estimated error convariance 估计误差协方差
    float gain;
}kalman_struct;


extern kalman_struct KY1_Kalman;
extern kalman_struct KY2_Kalman;
extern kalman_struct KY3_Kalman;
extern kalman_struct KY4_Kalman;
extern kalman_struct KY5_Kalman;
extern kalman_struct KY6_Kalman;
extern kalman_struct test1_Kalman;
extern kalman_struct test2_Kalman;
extern kalman_struct test3_Kalman;
extern kalman_struct test4_Kalman;
extern kalman_struct test_Kalman;
void kalman_ALL_init(void);
void kalman_init(kalman_struct *kalman_lcw, float init_x, float init_p);
float kalman_filter(kalman_struct *kalman_lcw, float z_measure);

typedef struct {
    double x;  // 状态估计值，代表对系统当前状态的估计
    double P;  // 状态协方差，衡量状态估计的不确定性
    double Q;  // 过程噪声协方差，反映系统模型中噪声的不确定性
    double R;  // 测量噪声协方差，体现测量过程中噪声的不确定性
} AdaptiveKalmanFilter;

void init_akf(AdaptiveKalmanFilter *akf, double x0, double P0, double Q0, double R0);
double update_akf(AdaptiveKalmanFilter *akf, double z, double F, double H);

// 定义 Sage - Husa 自适应滤波器结构体
typedef struct {
    double x;       // 状态估计值
    double P;       // 状态协方差
    double Q;       // 过程噪声协方差
    double R;       // 测量噪声协方差
    double Q_hat;   // 过程噪声协方差的估计值
    double R_hat;   // 测量噪声协方差的估计值
    double alpha;   // 用于更新 Q_hat 的遗忘因子
    double beta;    // 用于更新 R_hat 的遗忘因子
} SageHusaFilter;
double update_sage_husa(SageHusaFilter *filter, double z, double F, double H);
void init_sage_husa(SageHusaFilter *filter, double x0, double P0, double Q0, double R0, double alpha, double beta);
//————————————————
//版权声明：本文为CSDN博主「鹏の博客」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/qq_38973721/article/details/128133961



#endif

