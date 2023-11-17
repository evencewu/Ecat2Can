#include "stm32f4xx.h"
#include "delay.h"

#include "esc.h"
#include "ecat_slv.h"
#include "ecatapp.h"

#include "bsp_can.h"
#include "bsp_led.h"
#include "bsp_tim3.h"

extern bool heart_tag;

extern CanTxMsg Can1_TxMessage; // 发送缓冲区
extern CanRxMsg Can1_RxMessage; // 接收缓冲区

extern CanTxMsg Can2_TxMessage; // 发送缓冲区
extern CanRxMsg Can2_RxMessage; // 接收缓冲区

// uint32_t ecatapp_benchmark_us(void);
int main(void)
{
	SysTick_Config(SystemCoreClock / 1000);
	delay_init();

	led_setup();
	//CAN1_Config();
	//CAN2_Config();

	ecatapp_init();

	// TIMx_Configuration();

	CAN_FIFORelease(CAN1, CAN_FIFO0);

	while (1)
	{
		// GPIO_SetBits(GPIOB, GPIO_Pin_15);
		// GPIO_SetBits(GPIOB, GPIO_Pin_14);
		// GPIO_SetBits(GPIOB, GPIO_Pin_13);

		ecat_slv();

		CAN_Transmit(CAN1, &Can1_TxMessage);
		CAN_Transmit(CAN2, &Can2_TxMessage);
		// ecatapp_benchmark_us();

		// ecatapp_loop();
	}
}

void TIM6_DAC_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}

// uint32_t ecatapp_benchmark_us()
// {
//     // benchmark start
//     stopwatch_t st;
//     stopwatch_start(&st);

//     ecatapp_loop();

//     // benchmark stop
//     volatile uint32_t elapsed_us = stopwatch_now_us(&st);
//     static volatile uint32_t hiscore = 0;

//     if (elapsed_us > hiscore)
//     {
//         hiscore = elapsed_us;
//     }

//     if (STM_EVAL_IsPressed())
//     {
//         printf("PDI irq = %s \r\n", HEX4B(pdi_irq_flag));
//     }

//     return elapsed_us;
// }
