#include "kalman.h"

kalman_struct KY1_Kalman;
kalman_struct KY2_Kalman;
kalman_struct KY3_Kalman;
kalman_struct KY4_Kalman;
kalman_struct KY5_Kalman;
kalman_struct KY6_Kalman;
kalman_struct test1_Kalman;
kalman_struct test2_Kalman;
kalman_struct test3_Kalman;
kalman_struct test4_Kalman;
kalman_struct test_Kalman;

void kalman_ALL_init(void)
{
	kalman_init(&KY1_Kalman,0,100);
	kalman_init(&KY2_Kalman,0,100);
	kalman_init(&KY3_Kalman,0,100);
	kalman_init(&KY4_Kalman,0,100);
	kalman_init(&KY5_Kalman,0,100);
	kalman_init(&KY6_Kalman,0,100);
	
	kalman_init(&test1_Kalman,0,100);
	kalman_init(&test2_Kalman,0,100);
	kalman_init(&test3_Kalman,0,100);
	kalman_init(&test4_Kalman,0,100);
	
	if(lvbocanshu == 1)
	{
	//25hz
		KY1_Kalman.q = 0.5f;
		KY1_Kalman.r = 30.0f;
		KY2_Kalman.q = 0.5f;
		KY2_Kalman.r = 30.0f;
		KY3_Kalman.q = 0.5f;
		KY3_Kalman.r = 30.0f;
		KY4_Kalman.q = 0.5f;
		KY4_Kalman.r = 30.0f;
		KY5_Kalman.q = 0.5f;
		KY5_Kalman.r = 30.0f;
		KY6_Kalman.q = 0.5f;
		KY6_Kalman.r = 30.0f;
	}
	else if(lvbocanshu == 2)
	{
	//50hz
		KY1_Kalman.q = 0.8f;
		KY1_Kalman.r = 38.0f;
		KY2_Kalman.q = 0.8f;
		KY2_Kalman.r = 38.0f;
		KY3_Kalman.q = 0.8f;
		KY3_Kalman.r = 38.0f;
		KY4_Kalman.q = 0.8f;
		KY4_Kalman.r = 38.0f;
		KY5_Kalman.q = 0.8f;
		KY5_Kalman.r = 38.0f;
		KY6_Kalman.q = 0.8f;
		KY6_Kalman.r = 38.0f;
	}
	else if(lvbocanshu == 3)
	{
	//100hz
		KY1_Kalman.q = 0.2f;
		KY1_Kalman.r = 50.0f;
		KY2_Kalman.q = 0.2f;
		KY2_Kalman.r = 50.0f;
		KY3_Kalman.q = 0.2f;
		KY3_Kalman.r = 50.0f;
		KY4_Kalman.q = 0.2f;
		KY4_Kalman.r = 50.0f;
		KY5_Kalman.q = 0.2f;
		KY5_Kalman.r = 50.0f;
		KY6_Kalman.q = 0.2f;
		KY6_Kalman.r = 50.0f;
	}
	else if(lvbocanshu == 4)
	{
	//100hz
		KY1_Kalman.q = 0.001f;//0.001
		KY1_Kalman.r = 2000.0f;//2000
		KY2_Kalman.q = 0.01f;//0.08
		KY2_Kalman.r = 800.0f;//800
		KY3_Kalman.q = 0.01f;
		KY3_Kalman.r = 500.0f;
		KY4_Kalman.q = 0.01f;
		KY4_Kalman.r = 1000.0f;
		KY5_Kalman.q = 0.5f;
		KY5_Kalman.r = 30.0f;
		KY6_Kalman.q = 0.5f;
		KY6_Kalman.r = 30.0f;
		
//		test1_Kalman.q =0.1;
//		test1_Kalman.r =100;
//		test2_Kalman.q =0.1;
//		test2_Kalman.r =1000;
//		test3_Kalman.q =0.1;
//		test3_Kalman.r =2000;
//		test4_Kalman.q =0.1;
//		test4_Kalman.r =5000;
		
		test1_Kalman.q =0.1;
		test1_Kalman.r =500;
		test2_Kalman.q =0.01;
		test2_Kalman.r =500;
		test3_Kalman.q =0.001;
		test3_Kalman.r =500;
		test4_Kalman.q =0.0005;
		test4_Kalman.r =500;
	}
	
	
//感觉最好的	
//	KY1_Kalman.q = 0.1;
//	KY1_Kalman.r = 200;
//	KY2_Kalman.q = 0.1;
//	KY2_Kalman.r = 200;
//	KY3_Kalman.q = 0.1;
//	KY3_Kalman.r = 200;
//	KY4_Kalman.q = 0.1;
//	KY4_Kalman.r = 200;
//	KY5_Kalman.q = 0.1;
//	KY5_Kalman.r = 200;
//	KY6_Kalman.q = 0.1;
//	KY6_Kalman.r = 200;
	

	
////3200hz		定时器1khz
//	KY1_Kalman.q = 0.15f;
//	KY1_Kalman.r = 90.0f;
//	KY2_Kalman.q = 0.15f;
//	KY2_Kalman.r = 90.0f;
//	KY3_Kalman.q = 0.15f;
//	KY3_Kalman.r = 90.0f;
//	KY4_Kalman.q = 0.15f;
//	KY4_Kalman.r = 90.0f;
//	KY5_Kalman.q = 0.15f;
//	KY5_Kalman.r = 90.0f;
//	KY6_Kalman.q = 0.15f;
//	KY6_Kalman.r = 90.0f;

//200hz		定时器200hz
//	KY1_Kalman.q = 0.15f;
//	KY1_Kalman.r = 90.0f;
//	KY2_Kalman.q = 0.15f;
//	KY2_Kalman.r = 90.0f;
//	KY3_Kalman.q = 0.15f;
//	KY3_Kalman.r = 90.0f;
//	KY4_Kalman.q = 0.15f;
//	KY4_Kalman.r = 90.0f;
//	KY5_Kalman.q = 0.15f;
//	KY5_Kalman.r = 90.0f;
//	KY6_Kalman.q = 0.15f;
//	KY6_Kalman.r = 90.0f;
	
}


/**
 *kalman_init - 卡尔曼滤波器初始化
 *@kalman_lcw：卡尔曼滤波器结构体
 *@init_x：待测量的初始值
 *@init_p：后验状态估计值误差的方差的初始值
 */
void kalman_init(kalman_struct *kalman_lcw, float init_x, float init_p)
{
    kalman_lcw->x = init_x;//待测量的初始值，如有中值一般设成中值（如陀螺仪）
    kalman_lcw->p = init_p;//后验状态估计值误差的方差的初始值
    kalman_lcw->A = 1;
    kalman_lcw->H = 1;
    kalman_lcw->q = 0.15f;//10e-6;//2e2;predict noise convariance 预测（过程）噪声方差 实验发现修改这个值会影响收敛速率
    kalman_lcw->r = 90.0f;//10e-5;//测量（观测）噪声方差。以陀螺仪为例，测试方法是：
    //保持陀螺仪不动，统计一段时间内的陀螺仪输出数据。数据会近似正态分布，
    //按3σ原则，取正态分布的(3σ)^2作为r的初始化值
}

/**
 *kalman_filter - 卡尔曼滤波器
 *@kalman_lcw:卡尔曼结构体
 *@measure；测量值
 *返回滤波后的值
 */
float kalman_filter(kalman_struct *kalman_lcw, float measure)
{
    /* Predict */
    kalman_lcw->x = kalman_lcw->A * kalman_lcw->x;
    kalman_lcw->p = kalman_lcw->A * kalman_lcw->A * kalman_lcw->p + kalman_lcw->q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */

    /* Measurement */
    kalman_lcw->gain = kalman_lcw->p * kalman_lcw->H / (kalman_lcw->p * kalman_lcw->H * kalman_lcw->H + kalman_lcw->r);
    kalman_lcw->x = kalman_lcw->x + kalman_lcw->gain * (measure - kalman_lcw->H * kalman_lcw->x);
    kalman_lcw->p = (1 - kalman_lcw->gain * kalman_lcw->H) * kalman_lcw->p;

    return kalman_lcw->x;
}
//————————————————
//版权声明：本文为CSDN博主「鹏の博客」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/qq_38973721/article/details/128133961


// 初始化自适应卡尔曼滤波器
// 参数：
// akf：指向 AdaptiveKalmanFilter 结构体的指针，用于初始化滤波器
// x0：初始状态估计值
// P0：初始状态协方差
// Q0：初始过程噪声协方差
// R0：初始测量噪声协方差
void init_akf(AdaptiveKalmanFilter *akf, double x0, double P0, double Q0, double R0) {
    akf->x = x0;  // 初始化状态估计值
    akf->P = P0;  // 初始化状态协方差
    akf->Q = Q0;  // 初始化过程噪声协方差
    akf->R = R0;  // 初始化测量噪声协方差
}

// 自适应卡尔曼滤波更新步骤
// 参数：
// akf：指向 AdaptiveKalmanFilter 结构体的指针，用于更新滤波器状态
// z：当前测量值
// F：状态转移矩阵，描述系统状态如何随时间变化
// H：测量矩阵，将系统状态映射到测量空间
double update_akf(AdaptiveKalmanFilter *akf, double z, double F, double H) {
    double x_pred, P_pred, K, e;
    // 自适应调整参数，用于调整 Q 和 R 的更新速率
    double alpha = 0.009;
    double beta = 0.001;
    double gamma = 0.009;
    double delta = 0.001;

    // 预测步骤
    // 根据上一时刻的状态估计值和状态转移矩阵，预测当前时刻的状态
    x_pred = F * akf->x;
    // 预测当前时刻的状态协方差，考虑过程噪声的影响
    P_pred = F * akf->P * F + akf->Q;

    // 更新步骤
    // 计算卡尔曼增益，用于平衡预测值和测量值的权重
    K = P_pred * H / (H * P_pred * H + akf->R);
    // 根据卡尔曼增益和测量值，更新当前时刻的状态估计值
    akf->x = x_pred + K * (z - H * x_pred);
    // 更新当前时刻的状态协方差
    akf->P = (1 - K * H) * P_pred;

    // 计算残差
    // 残差是测量值与预测测量值之间的差值，用于评估滤波效果
    e = z - H * x_pred;

    // 自适应调整 Q 和 R
    // 根据残差的大小，自适应地调整过程噪声协方差和测量噪声协方差
    akf->Q = alpha * akf->Q + (1 - alpha) * beta * e * e;
    akf->R = gamma * akf->R + (1 - gamma) * delta * e * e;

    return akf->x;
}

// 初始化 Sage - Husa 自适应滤波器
// 参数说明：
// filter: 指向 SageHusaFilter 结构体的指针，用于初始化滤波器
// x0: 初始状态估计值
// P0: 初始状态协方差
// Q0: 初始过程噪声协方差
// R0: 初始测量噪声协方差
// alpha: 用于更新 Q_hat 的遗忘因子
// beta: 用于更新 R_hat 的遗忘因子
void init_sage_husa(SageHusaFilter *filter, double x0, double P0, double Q0, double R0, double alpha, double beta) {
    filter->x = x0;      // 初始化状态估计值
    filter->P = P0;      // 初始化状态协方差
    filter->Q = Q0;      // 初始化过程噪声协方差
    filter->R = R0;      // 初始化测量噪声协方差
    filter->Q_hat = Q0;  // 初始化过程噪声协方差的估计值
    filter->R_hat = R0;  // 初始化测量噪声协方差的估计值
    filter->alpha = alpha; // 初始化 Q_hat 的遗忘因子
    filter->beta = beta;   // 初始化 R_hat 的遗忘因子
}

// Sage - Husa 自适应滤波更新步骤
// 参数说明：
// filter: 指向 SageHusaFilter 结构体的指针，用于更新滤波器状态
// z: 当前测量值
// F: 状态转移矩阵
// H: 测量矩阵
double update_sage_husa(SageHusaFilter *filter, double z, double F, double H) {
    double x_pred;    // 预测状态值
    double P_pred;    // 预测状态协方差
    double K;         // 卡尔曼增益
    double e;         // 残差
    double epsilon;   // 预测残差
    double nu;        // 测量残差

    // 预测步骤
    // 根据状态转移矩阵 F 和上一时刻的状态估计值，预测当前时刻的状态
    x_pred = F * filter->x;
    // 根据状态转移矩阵 F、上一时刻的状态协方差和过程噪声协方差 Q，预测当前时刻的状态协方差
    P_pred = F * filter->P * F + filter->Q;

    // 计算预测残差
    epsilon = z - H * x_pred;

    // 计算卡尔曼增益
    K = P_pred * H / (H * P_pred * H + filter->R);

    // 更新状态估计值
    filter->x = x_pred + K * epsilon;

    // 更新状态协方差
    filter->P = (1 - K * H) * P_pred;

    // 计算测量残差
    nu = z - H * filter->x;

    // 自适应更新过程噪声协方差的估计值 Q_hat
    filter->Q_hat = filter->alpha * filter->Q_hat + (1 - filter->alpha) * (K * nu * nu * K + P_pred - F * filter->P * F);

    // 自适应更新测量噪声协方差的估计值 R_hat
    filter->R_hat = filter->beta * filter->R_hat + (1 - filter->beta) * (nu * nu - H * P_pred * H);

    // 更新过程噪声协方差和测量噪声协方差
    filter->Q = filter->Q_hat;
    filter->R = filter->R_hat;

    return filter->x;  // 返回更新后的状态估计值
}
