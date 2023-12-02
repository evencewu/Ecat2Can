#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#include "stm32f4xx_conf.h"

#define BASIC_TIM               TIM6
#define BASIC_TIM_CLK           RCC_APB1Periph_TIM6

#define BASIC_TIM_IRQn          TIM6_DAC_IRQn
#define BASIC_TIM_IRQHandler    TIM6_DAC_IRQHandler

void TIMx_Configuration();


#endif