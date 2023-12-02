#include "bsp_tim3.h"

void TIM3_NVIC_Enable(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    // 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQn;
    // 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM3_NVIC_Disable(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    // 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQn;
    // 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    // 开启TIMx_CLK,x[6,7]
    RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE);

    /* 累计 TIM_Period个后产生一个更新或者中断*/
    // 当定时器从0计数到4999，即为5000次，为一个定时周期
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;

    // 定时器时钟源TIMxCLK = 2 * PCLK1
    //         PCLK1 = HCLK / 4
    //         => TIMxCLK=HCLK/2=SystemCoreClock/2=90MHz
    //  设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=50000Hz
    TIM_TimeBaseStructure.TIM_Prescaler = 1680 - 1;

    // 初始化定时器TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);

    // 清除定时器更新中断标志位
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);

    // 开启定时器更新中断
    TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);

    // 使能定时器
    TIM_Cmd(BASIC_TIM, ENABLE);
}

void TIM3_Configuration()
{
    TIM_Mode_Config();
}

