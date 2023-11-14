#ifndef __ECAT_TYPEDEF__
#define __ECAT_TYPEDEF__

#include <inttypes.h>

typedef struct Ecat2Can_Pack
{
    uint32_t StdId;
    uint8_t ExtId;
    uint8_t IDE;
    uint8_t RTR;
    uint8_t DLC;

    uint8_t Data[8];

    uint8_t beat_heart;

    uint8_t LED;
}  __attribute__((packed)) Ecat2Can_Pack;

#define SPEED_CTRL 0
#define POS_CTRL 1
#define MIT_CTRL 2
#define ENABLE 3
#define DISABLE 4

// DM4310_demo
void DM_can_set(uint8_t canid, Ecat2Can_Pack *pack, int32 mode, float32 p_des, float32 v_des, float32 Kp, float32 Kd, float32 t_ff)
{
    // uint8_t *v_buf;
    // v_buf = (uint8_t*)&v_des;
    // SPEED CTRL
    if (mode == 0)
    {
        pack->StdId = 0x0200 + canid;
        pack->ExtId = 0x00;
        pack->IDE = 0x00;
        pack->RTR = 0x00;
        pack->DLC = 0x04;

        pack->Data[3] = (uint32_t)v_des & 0xFF;
        pack->Data[2] = ((uint32_t)v_des >> 8) & 0xFF;
        pack->Data[1] = ((uint32_t)v_des >> 16) & 0xFF;
        pack->Data[0] = ((uint32_t)v_des >> 24) & 0xFF;
    }
    // POS CTRL
    else if (mode == 1)
    {
        pack->StdId = 0x0100 + canid;
        pack->ExtId = 0x00;
        pack->IDE = 0x00;
        pack->RTR = 0x00;
        pack->DLC = 0x08;

        pack->Data[0] = (uint32_t)p_des & 0xFF;
        pack->Data[1] = ((uint32_t)p_des >> 8) & 0xFF;
        pack->Data[2] = ((uint32_t)p_des >> 16) & 0xFF;
        pack->Data[3] = ((uint32_t)p_des >> 24) & 0xFF;
        pack->Data[4] = (uint32_t)v_des & 0xFF;
        pack->Data[5] = ((uint32_t)v_des >> 8) & 0xFF;
        pack->Data[6] = ((uint32_t)v_des >> 16) & 0xFF;
        pack->Data[7] = ((uint32_t)v_des >> 24) & 0xFF;
    }
    // MIT CTRL
    else if (mode == 2)
    {
        pack->StdId = 0x0000 + canid;
        pack->ExtId = 0x00;
        pack->IDE = 0x00;
        pack->RTR = 0x00;
        pack->DLC = 0x08;

        pack->Data[0] = ((uint32_t)p_des >> 16) & 0xFF;
        pack->Data[1] = ((uint32_t)p_des >> 24) & 0xFF;
        pack->Data[2] = ((uint32_t)v_des >> 24) & 0xFF;
        pack->Data[3] = (((uint32_t)v_des >> 20) & 0x0F) | (((uint32_t)Kp >> 28) & 0x0F);
        pack->Data[4] = ((uint32_t)Kp >> 20) & 0xFF;
        pack->Data[5] = ((uint32_t)Kd >> 24) & 0xFF;
        pack->Data[6] = (((uint32_t)Kd >> 20) & 0x0F) | (((uint32_t)t_ff >> 28) & 0x0F);
        pack->Data[7] = ((uint32_t)t_ff >> 20) & 0xFF;
    }
    // ENABLE
    else if (mode == 3)
    {
        pack->StdId = 0x00000001;
        pack->ExtId = 0x00;
        pack->IDE = 0x00;
        pack->RTR = 0x00;
        pack->DLC = 0x08;

        pack->Data[0] = 0xFF;
        pack->Data[1] = 0xFF;
        pack->Data[2] = 0xFF;
        pack->Data[3] = 0xFF;
        pack->Data[4] = 0xFF;
        pack->Data[5] = 0xFF;
        pack->Data[6] = 0xFF;
        pack->Data[7] = 0xFC;
    }
    // DISABLE
    else if (mode == 4)
    {
        pack->StdId = 0x00000001;
        pack->ExtId = 0x00;
        pack->IDE = 0x00;
        pack->RTR = 0x00;
        pack->DLC = 0x08;

        pack->Data[0] = 0xFF;
        pack->Data[1] = 0xFF;
        pack->Data[2] = 0xFF;
        pack->Data[3] = 0xFF;
        pack->Data[4] = 0xFF;
        pack->Data[5] = 0xFF;
        pack->Data[6] = 0xFF;
        pack->Data[7] = 0xFD;
    }
}

#define GM6020_TAG1 0x1FF
#define GM6020_TAG2 0x2FF
// GM6020_demo
void GM6020_can_set(uint8_t canid, Ecat2Can_Pack *pack, uint32_t tag, int16_t v1, int16_t v2, int16_t v3, int16_t v4)
{
    if (canid == 1)
    {
        pack->StdId = tag;
        pack->ExtId = 0x00;
        pack->IDE = 0x00;
        pack->RTR = 0x00;
        pack->DLC = 0x08;

        pack->Data[0] = v1 >> 8 & 0xFF;
        pack->Data[1] = v1 & 0xFF;
        pack->Data[2] = v2 >> 8 & 0xFF;
        pack->Data[3] = v2 & 0xFF;
        pack->Data[4] = v3 >> 8 & 0xFF;
        pack->Data[5] = v3 & 0xFF;
        pack->Data[6] = v4 >> 8 & 0xFF;
        pack->Data[7] = v4 & 0xFF;
    }
}

#endif