#include "ethercat_to_can/ecat_base.h"
#include "ethercat_to_can/ecat_typedef.h"

Ecat2Can_Pack packet;

int main()
{
    char phy[] = "enp4s0";
    EcatStart(phy);
    packet.StdId = 0x00000001;
    packet.ExtId = 0x00;
    packet.IDE = 0x00;
    packet.RTR = 0x00;
    packet.DLC = 0x08;

    packet.Data[0] = 0xFF;
    packet.Data[1] = 0xFF;
    packet.Data[2] = 0xFF;
    packet.Data[3] = 0xFF;
    packet.Data[4] = 0xFF;
    packet.Data[5] = 0xFF;
    packet.Data[6] = 0xFF;
    packet.Data[7] = 0xFC;

    EcatSyncMsg((uint8_t *)&packet);
    EcatStop();
    return 0;
}