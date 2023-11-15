#ifndef __ECAT_TYPEDEF__
#define __ECAT_TYPEDEF__

#include <inttypes.h>

struct can_pack
{
    uint32_t StdId;
    uint8_t ExtId;
    uint8_t IDE;
    uint8_t RTR;
    uint8_t DLC;

    uint8_t Data[8];
};

typedef struct Ecat2Can_Pack
{
    can_pack can1;
    can_pack can2;

    uint8_t beat_heart;

    uint8_t LED;
} __attribute__((packed)) Ecat2Can_Pack;

void Stop_all(Ecat2Can_Pack *pack)
{
    //memset(&packet, 0, sizeof(Ecat2Can_Pack));

    // stop GM6020


        pack->can1.StdId = tag;
        pack->can1.ExtId = 0x00;
        pack->can1.IDE = 0x00;
        pack->can1.RTR = 0x00;
        pack->can1.DLC = 0x08;

        pack->can1.Data[0] = 0x00;
        pack->can1.Data[1] = 0x00;
        pack->can1.Data[2] = 0x00;
        pack->can1.Data[3] = 0x00;
        pack->can1.Data[4] = 0x00;
        pack->can1.Data[5] = 0x00;
        pack->can1.Data[6] = 0x00;
        pack->can1.Data[7] = 0x00;

        pack->can2.StdId = tag;
        pack->can2.ExtId = 0x00;
        pack->can2.IDE = 0x00;
        pack->can2.RTR = 0x00;
        pack->can2.DLC = 0x08;

        pack->can2.Data[0] = v1 >> 8 & 0xFF;
        pack->can2.Data[1] = v1 & 0xFF;
        pack->can2.Data[2] = v2 >> 8 & 0xFF;
        pack->can2.Data[3] = v2 & 0xFF;
        pack->can2.Data[4] = v3 >> 8 & 0xFF;
        pack->can2.Data[5] = v3 & 0xFF;
        pack->can2.Data[6] = v4 >> 8 & 0xFF;
        pack->can2.Data[7] = v4 & 0xFF;

}
//--------//
// DM4310 //
//--------//

#define SPEED_CTRL 0
#define POS_CTRL 1
#define MIT_CTRL 2
#define ENABLE 3
#define DISABLE 4
#define CLEAR_ERROR 5

#define KP_MIN 0
#define KP_MAX 500

#define KD_MIN 0
#define KD_MAX 5

#define P_MIN -675
#define P_MAX 675

#define V_MIN 0
#define V_MAX 5

#define T_MIN 0
#define T_MAX 5

float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
}
int float_to_uint(float x, float x_min, float x_max, int bits)
{
    float span = x_max - x_min;
    float offset = x_min;
    return (int)((x - offset) * ((float)((1 << bits) - 1)) / span);
}

// DM4310_demo
void DM_can_set(uint8_t canid, Ecat2Can_Pack *pack, int mode, float _pos, float _vel, float _KP, float _KD, float _torq)
{
    uint16_t pos_tmp, vel_tmp, kp_tmp, kd_tmp, tor_tmp;
    pos_tmp = float_to_uint(_pos, P_MIN, P_MAX, 16);
    vel_tmp = float_to_uint(_vel, V_MIN, V_MAX, 12);
    kp_tmp = float_to_uint(_KP, KP_MIN, KP_MAX, 12);
    kd_tmp = float_to_uint(_KD, KD_MIN, KD_MAX, 12);
    tor_tmp = float_to_uint(_torq, T_MIN, T_MAX, 12);

    uint8_t *pbuf, *vbuf;
    pbuf = (uint8_t *)&_pos;
    vbuf = (uint8_t *)&_vel;

    // SPEED CTRL
    if (mode == 0)
    {
        pack->can1.StdId = 0x0200 + canid;
        pack->can1.ExtId = 0x00;
        pack->can1.IDE = 0x00;
        pack->can1.RTR = 0x00;
        pack->can1.DLC = 0x04;

        pack->can1.Data[0] = *vbuf;
        pack->can1.Data[1] = *(vbuf + 1);
        pack->can1.Data[2] = *(vbuf + 2);
        pack->can1.Data[3] = *(vbuf + 3);
    }
    // POS CTRL
    else if (mode == 1)
    {
        pack->can1.StdId = 0x0100 + canid;
        pack->can1.ExtId = 0x00;
        pack->can1.IDE = 0x00;
        pack->can1.RTR = 0x00;
        pack->can1.DLC = 0x08;

        pack->can1.Data[0] = *pbuf;
        pack->can1.Data[1] = *(pbuf + 1);
        pack->can1.Data[2] = *(pbuf + 2);
        pack->can1.Data[3] = *(pbuf + 3);
        pack->can1.Data[4] = *vbuf;
        pack->can1.Data[5] = *(vbuf + 1);
        pack->can1.Data[6] = *(vbuf + 2);
        pack->can1.Data[7] = *(vbuf + 3);
    }
    // MIT CTRL
    else if (mode == 2)
    {
        pack->can1.StdId = 0x0000 + canid;
        pack->can1.ExtId = 0x00;
        pack->can1.IDE = 0x00;
        pack->can1.RTR = 0x00;
        pack->can1.DLC = 0x08;

        pack->can1.Data[0] = (pos_tmp >> 8);
        pack->can1.Data[1] = pos_tmp;
        pack->can1.Data[2] = (vel_tmp >> 4);
        pack->can1.Data[3] = ((vel_tmp & 0xF) << 4) | (kp_tmp >> 8);
        pack->can1.Data[4] = kp_tmp;
        pack->can1.Data[5] = (kd_tmp >> 4);
        pack->can1.Data[6] = ((kd_tmp & 0xF) << 4) | (tor_tmp >> 8);
        pack->can1.Data[7] = tor_tmp;
    }
    // ENABLE
    else if (mode == 3)
    {
        pack->can1.StdId = 0x00000001;
        pack->can1.ExtId = 0x00;
        pack->can1.IDE = 0x00;
        pack->can1.RTR = 0x00;
        pack->can1.DLC = 0x08;

        pack->can1.Data[0] = 0xFF;
        pack->can1.Data[1] = 0xFF;
        pack->can1.Data[2] = 0xFF;
        pack->can1.Data[3] = 0xFF;
        pack->can1.Data[4] = 0xFF;
        pack->can1.Data[5] = 0xFF;
        pack->can1.Data[6] = 0xFF;
        pack->can1.Data[7] = 0xFC;
    }
    // DISABLE
    else if (mode == 4)
    {
        pack->can1.StdId = 0x00000001;
        pack->can1.ExtId = 0x00;
        pack->can1.IDE = 0x00;
        pack->can1.RTR = 0x00;
        pack->can1.DLC = 0x08;

        pack->can1.Data[0] = 0xFF;
        pack->can1.Data[1] = 0xFF;
        pack->can1.Data[2] = 0xFF;
        pack->can1.Data[3] = 0xFF;
        pack->can1.Data[4] = 0xFF;
        pack->can1.Data[5] = 0xFF;
        pack->can1.Data[6] = 0xFF;
        pack->can1.Data[7] = 0xFD;
    }
    // CLEAR_ERROR
    else if (mode == 5)
    {
        pack->can1.StdId = 0x00000001;
        pack->can1.ExtId = 0x00;
        pack->can1.IDE = 0x00;
        pack->can1.RTR = 0x00;
        pack->can1.DLC = 0x08;

        pack->can1.Data[0] = 0xFF;
        pack->can1.Data[1] = 0xFF;
        pack->can1.Data[2] = 0xFF;
        pack->can1.Data[3] = 0xFF;
        pack->can1.Data[4] = 0xFF;
        pack->can1.Data[5] = 0xFF;
        pack->can1.Data[6] = 0xFF;
        pack->can1.Data[7] = 0xFB;
    }
}

//--------//
// GM6020 //
//--------//

#define GM6020_TAG1 0x1FF
#define GM6020_TAG2 0x2FF
// GM6020_demo
void GM6020_can_set(uint8_t canid, Ecat2Can_Pack *pack, uint32_t tag, int16_t v1, int16_t v2, int16_t v3, int16_t v4)
{
    if (canid == 1)
    {
        pack->can1.StdId = tag;
        pack->can1.ExtId = 0x00;
        pack->can1.IDE = 0x00;
        pack->can1.RTR = 0x00;
        pack->can1.DLC = 0x08;

        pack->can1.Data[0] = v1 >> 8 & 0xFF;
        pack->can1.Data[1] = v1 & 0xFF;
        pack->can1.Data[2] = v2 >> 8 & 0xFF;
        pack->can1.Data[3] = v2 & 0xFF;
        pack->can1.Data[4] = v3 >> 8 & 0xFF;
        pack->can1.Data[5] = v3 & 0xFF;
        pack->can1.Data[6] = v4 >> 8 & 0xFF;
        pack->can1.Data[7] = v4 & 0xFF;
    }
    else if (canid == 2)
    {
        pack->can2.StdId = tag;
        pack->can2.ExtId = 0x00;
        pack->can2.IDE = 0x00;
        pack->can2.RTR = 0x00;
        pack->can2.DLC = 0x08;

        pack->can2.Data[0] = v1 >> 8 & 0xFF;
        pack->can2.Data[1] = v1 & 0xFF;
        pack->can2.Data[2] = v2 >> 8 & 0xFF;
        pack->can2.Data[3] = v2 & 0xFF;
        pack->can2.Data[4] = v3 >> 8 & 0xFF;
        pack->can2.Data[5] = v3 & 0xFF;
        pack->can2.Data[6] = v4 >> 8 & 0xFF;
        pack->can2.Data[7] = v4 & 0xFF;
    }
}

#endif