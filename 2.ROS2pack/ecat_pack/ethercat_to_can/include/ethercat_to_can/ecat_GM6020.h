#ifndef __ECAT_GM6020__
#define __ECAT_GM6020__

#include "ethercat_to_can/ecat_base.h"
#include "ethercat_to_can/ecat_typedef.h"

#include <inttypes.h>

//--------//
// GM6020 //
//--------//

#define GM6020_TAG1 0x1FF
#define GM6020_TAG2 0x2FF

// GM6020_demo
void GM6020_can_set(uint8_t canid, Ecat2Can_Pack *pack, uint32_t tag, int16_t v1, int16_t v2, int16_t v3, int16_t v4)
{

    pack->can[canid-1].StdId = tag;
    pack->can[canid-1].ExtId = 0x00;
    pack->can[canid-1].IDE = 0x00;
    pack->can[canid-1].RTR = 0x00;
    pack->can[canid-1].DLC = 0x08;

    pack->can[canid-1].Data[0] = v1 >> 8 & 0xFF;
    pack->can[canid-1].Data[1] = v1 & 0xFF;
    pack->can[canid-1].Data[2] = v2 >> 8 & 0xFF;
    pack->can[canid-1].Data[3] = v2 & 0xFF;
    pack->can[canid-1].Data[4] = v3 >> 8 & 0xFF;
    pack->can[canid-1].Data[5] = v3 & 0xFF;
    pack->can[canid-1].Data[6] = v4 >> 8 & 0xFF;
    pack->can[canid-1].Data[7] = v4 & 0xFF;
}

#endif