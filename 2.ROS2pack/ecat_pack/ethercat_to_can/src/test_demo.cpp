#include "ethercat_to_can/ecat_base.h"
#include "ethercat_to_can/ecat_typedef.h"
#include "ethercat_to_can/ecat_DM4310.h"
#include "ethercat_to_can/ecat_GM6020.h"

#include <signal.h>

bool app_stopped = false;
Ecat2Can_Pack packet;

void Stop_all(Ecat2Can_Pack *pack);
void sigint_handler(int sig);

int main()
{
    signal(SIGINT, sigint_handler);

    char phy[] = "enp4s0";
    EcatStart(phy);

    memset(&packet, 0, sizeof(Ecat2Can_Pack));
    EcatSyncMsg((uint8_t *)&packet);

    //-------------//
    // DM4310 demo //
    //-------------//

    //DM_can_set(CAN2, &packet, CLEAR_ERROR, DM4310_ID_1, 0, 10.0, 0, 0, 0);
    //EcatSyncMsg((uint8_t *)&packet);

    DM_can_set(CAN2, &packet, ENABLE, DM4310_ID_1, 0, 0, 0, 0, 0);
    EcatSyncMsg((uint8_t *)&packet);

    int tag = 0;

    //-----END-----//

    for (;;)
    {
        printf("start\n");
        //-------------//
        // DM4310 demo //
        //-------------//

        if (tag == 0)
        {
            DM_can_set(CAN2, &packet, MIT_CTRL, DM4310_ID_1, -30, 0, 1, 0.5, 0);
            EcatSyncMsg((uint8_t *)&packet);
            tag = 1;
        }
        else
        {
            DM_can_set(CAN2, &packet, MIT_CTRL, DM4310_ID_1, 30, 0, 1, 0.5, 0);
            EcatSyncMsg((uint8_t *)&packet);
            tag = 0;
        }

        EcatSyncMsg((uint8_t *)&packet);

        //-------------//
        // GM6020 demo //
        //-------------//

        GM6020_can_set(CAN1, &packet, GM6020_TAG1, 20000, 20000, 0, 0);
        packet.LED = 0x0F;
        EcatSyncMsg((uint8_t *)&packet);

        usleep(2 * 1000 * 1000);

        //-----END-----//

        if (app_stopped)
        {
            break;
        }
    }

    //-------------//
    // DM4310 demo //
    //-------------//



    //
    printf("stop");
    Stop_all(&packet);
    EcatStop();

    return 0;
}

// stop
void Stop_all(Ecat2Can_Pack *pack)
{
    // memset(&packet, 0, sizeof(Ecat2Can_Pack));

    // stop DM4310
    DM_can_set(CAN2, &packet, DISABLE, DM4310_ID_1, 0, 0, 0, 0, 0);
    EcatSyncMsg((uint8_t *)&packet);

    // stop GM6020
    GM6020_can_set(CAN1, pack, GM6020_TAG1, 0, 0, 0, 0);
    GM6020_can_set(CAN2, pack, GM6020_TAG1, 0, 0, 0, 0);
    EcatSyncMsg((uint8_t *)&pack);

    GM6020_can_set(CAN1, pack, GM6020_TAG2, 0, 0, 0, 0);
    GM6020_can_set(CAN2, pack, GM6020_TAG2, 0, 0, 0, 0);
    EcatSyncMsg((uint8_t *)&pack);
}

void sigint_handler(int sig)
{
    if (sig == SIGINT)
    {
        app_stopped = true;
    }
}