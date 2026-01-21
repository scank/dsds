#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "main.h"

extern int32_t ky1_value;
extern int32_t ky2_value;
extern int32_t ky3_value;
extern int32_t ky4_value;
extern int32_t ky5_value;
extern int32_t ky6_value;

extern uint8_t ky5530_channel;
uint8_t control_ky5532_readADC(uint8_t channel);

int32_t filter(void);
int32_t meanFilter(int32_t newData);
int32_t meanFilter1(int32_t newData);
int32_t meanFilter2(int32_t newData);
int32_t meanFilter3(int32_t newData);
int32_t amplitudeLimitFilter(int32_t new_data, int32_t last_data, int32_t limit);
#endif	   
