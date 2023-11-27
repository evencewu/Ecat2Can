#ifndef __BSP_TIM3_H__
#define __BSP_TIM3_H__

#include "stm32f4xx_conf.h"

#define BASIC_TIM               TIM6
#define BASIC_TIM_CLK           RCC_APB1Periph_TIM6

#define BASIC_TIM_IRQn          TIM6_DAC_IRQn
#define BASIC_TIM_IRQHandler    TIM6_DAC_IRQHandler

void TIM6_Mode_Config(void);
void TIM6_NVIC_Disable(void);
void TIM6_NVIC_Enable(void);


#endif