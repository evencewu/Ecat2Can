#ifndef __ECAT_TYPEDEF__
#define __ECAT_TYPEDEF__

#include <inttypes.h>

#define CAN1 1
#define CAN2 2

struct can_pack
{
    uint32_t StdId;
    uint8_t ExtId;
    uint8_t IDE;
    uint8_t RTR;
    uint8_t DLC;

    uint8_t Data[8];
};

/// @brief ecat pdo send data (master to slv)
typedef struct Ecat2Can_Outputs_Pack
{
    can_pack can[2];

    uint8_t LED;
} __attribute__((packed)) Ecat2Can_Outputs_Pack;

/// @brief ecat pdo recive data (slv to master)
typedef struct Ecat2Can_Inputs_Pack
{
    can_pack can[2];
} __attribute__((packed)) Ecat2Can_Inputs_Pack;

#endif