#include "esc.h"
#include "ecat_slv.h"
#include "utypes.h"

#include "esc_irq.h"
#include "pdo_override.h"
#include "ecatapp.h"
#include "stm32f4xx.h"

#include "bsp_can.h"

int last_heart = 0;
int heart[10] = {0};
bool heart_tag = 0;

extern __IO uint32_t flag;

extern CanTxMsg TxMessage; // 发送缓冲区
extern CanRxMsg RxMessage; // 接收缓冲区

/* CANopen Object Dictionary */
_Objects Obj;

/* Application hook declaration */
void ecatapp(void);
uint16_t check_dc_handler(void);

/* SOES configuration */
static esc_cfg_t config = {
    .user_arg = "ax58100",
    .use_interrupt = 1,
    .watchdog_cnt = INT32_MAX,
    .set_defaults_hook = NULL,
    .pre_state_change_hook = NULL,
    .post_state_change_hook = NULL,
    .application_hook = ecatapp,
    .safeoutput_override = NULL,
    .pre_object_download_hook = NULL,
    .post_object_download_hook = NULL,
    .rxpdo_override = rxpdo_override,
    .txpdo_override = txpdo_override,
    .esc_hw_interrupt_enable = ESC_interrupt_enable,
    .esc_hw_interrupt_disable = ESC_interrupt_disable,
    .esc_hw_eep_handler = NULL,
    .esc_check_dc_handler = check_dc_handler,
};

/* CiA402 motion control configuration */

// **************************************************************

static uint8_t sync0_irq_flag = 0;

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line0);
        sync0_irq_flag = 1;
    }
}

static uint8_t pdi_irq_flag = 0;

void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line5);
        pdi_irq_flag = 1;
    }
}
// **************************************************************

void ecatapp_init(void)
{
    ecat_slv_init(&config);
    init_override();
}

uint16_t check_dc_handler(void)
{
// minimum watchdog value is 1 ms, in nanoseconds
#define MIN_WATCHDOG_VALUE_NS 1000000

    /* Indicate we run DC */
    ESCvar.dcsync = 1;
    /* Fetch the sync counter limit (SDO10F1) */
    ESCvar.synccounterlimit = Obj.Error_Settings.SyncErrorCounterLimit;

    uint32_t sync0cycleTime = ESC_enable_DC();
    Obj.Sync_Manager_2_Parameters.CycleTime = sync0cycleTime;
    // Obj.Sync_Manager_3_Parameters.CycleTime = sync0cycleTime;
    // calculate watchdog value as 2 x SYNC0 cycle time
    int watchdog_value = 2 * sync0cycleTime;
    if (watchdog_value < MIN_WATCHDOG_VALUE_NS)
    {
        watchdog_value = MIN_WATCHDOG_VALUE_NS;
    }
    APP_setwatchdog(watchdog_value);

    return 0;
}

void ecatapp()
{
}

void cb_get_inputs()
{
    //for(int i = 9;i >= 1;i--)
    //{
    //    heart[i] = heart[i-1];
    //}
    //
    //heart[0] = Obj.beat_heart;

    //TxMessage.StdId = 0;
    //TxMessage.ExtId = 0;
    //TxMessage.IDE = 0;
    //TxMessage.RTR = 0;
    //TxMessage.DLC = 0x08;
    //
    //TxMessage.Data[0] = 0;
    //TxMessage.Data[1] = 0;
    //TxMessage.Data[2] = 0;
    //TxMessage.Data[3] = 0;
    //TxMessage.Data[4] = 0;
    //TxMessage.Data[5] = 0;
    //TxMessage.Data[6] = 0;
    //TxMessage.Data[7] = 0;

    //if (heart == last_heart)
    //{
    //    return;
    //}

    TxMessage.StdId = Obj.can1_tx.StdId;
    TxMessage.ExtId = Obj.can1_tx.ExtId;
    TxMessage.IDE = Obj.can1_tx.IDE;
    TxMessage.RTR = Obj.can1_tx.RTR;
    TxMessage.DLC = Obj.can1_tx.DLC;

    TxMessage.Data[0] = Obj.can1_tx_data[0];
    TxMessage.Data[1] = Obj.can1_tx_data[1];
    TxMessage.Data[2] = Obj.can1_tx_data[2];
    TxMessage.Data[3] = Obj.can1_tx_data[3];
    TxMessage.Data[4] = Obj.can1_tx_data[4];
    TxMessage.Data[5] = Obj.can1_tx_data[5];
    TxMessage.Data[6] = Obj.can1_tx_data[6];
    TxMessage.Data[7] = Obj.can1_tx_data[7];

    if (Obj.led[0] != 0)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_15);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_15);
    }

    if (Obj.led[1] != 0)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_14);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_14);
    }

    if (Obj.led[2] != 0)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_13);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_13);
    }
}

void cb_set_outputs()
{
    Obj.can1_rx.StdId = RxMessage.StdId;
    Obj.can1_rx.ExtId = RxMessage.ExtId;
    Obj.can1_rx.IDE = RxMessage.IDE;
    Obj.can1_rx.RTR = RxMessage.RTR;
    Obj.can1_rx.DLC = RxMessage.DLC;

    Obj.can1_rx_data[0] = RxMessage.Data[0];
    Obj.can1_rx_data[1] = RxMessage.Data[1];
    Obj.can1_rx_data[2] = RxMessage.Data[2];
    Obj.can1_rx_data[3] = RxMessage.Data[3];
    Obj.can1_rx_data[4] = RxMessage.Data[4];
    Obj.can1_rx_data[5] = RxMessage.Data[5];
    Obj.can1_rx_data[6] = RxMessage.Data[6];
    Obj.can1_rx_data[7] = RxMessage.Data[7];
}

void ecatapp_loop(void)
{
    // stack in mixed mode
    if (sync0_irq_flag)
    {
        ESC_updateALevent();
        DIG_process(DIG_PROCESS_APP_HOOK_FLAG | DIG_PROCESS_INPUTS_FLAG);
        sync0_irq_flag = 0;
    }
    if (pdi_irq_flag)
    {
        ESC_updateALevent();
        if (ESCvar.dcsync)
        {
            DIG_process(DIG_PROCESS_OUTPUTS_FLAG);
        }
        else
        {
            DIG_process(DIG_PROCESS_OUTPUTS_FLAG | DIG_PROCESS_APP_HOOK_FLAG | DIG_PROCESS_INPUTS_FLAG);
        }
        pdi_irq_flag = 0;
    }
    else
    {
        // ecat_slv_worker(ESCREG_ALEVENT_CONTROL | ESCREG_ALEVENT_SMCHANGE
        //                 | ESCREG_ALEVENT_SM0 | ESCREG_ALEVENT_SM1);
        ecat_slv_poll();
        DIG_process(DIG_PROCESS_WD_FLAG);
    }
}