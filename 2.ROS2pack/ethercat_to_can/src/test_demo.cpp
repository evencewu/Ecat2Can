#include "ethercat_to_can/ecat_base.h"
#include "ethercat_to_can/ecat_typedef.h"

#include <signal.h>

bool app_stopped = false;
Ecat2Can_Pack packet;

void sigint_handler(int sig)
{
    if (sig == SIGINT)
    {
        app_stopped = true;
    }
}

int main()
{
    signal(SIGINT, sigint_handler);

    char phy[] = "enp4s0";
    EcatStart(phy);

    // packet.LED = 0x00;
    //
    // DM_can_set(1, &packet, ENABLE, 0, 0, 0, 0, 0);
    // EcatSyncMsg((uint8_t *)&packet);
    //
    // for (int i = 0; i < 500; i++)
    //{
    //    packet.LED = 0x02;
    //    DM_can_set(1, &packet, SPEED_CTRL, 0, 10.0, 0, 0, 0);
    //    EcatSyncMsg((uint8_t *)&packet);
    //}
    //
    // packet.LED = 0x00;
    // DM_can_set(1, &packet, DISABLE, 0, 1, 0, 0, 0);
    // EcatSyncMsg((uint8_t *)&packet);

    // int vol_tag = 1;

    //-------------//
    // DM4310 demo //
    //-------------//

    // DM_can_set(1, &packet, CLEAR_ERROR, 0, 10.0, 0, 0, 0);
    // EcatSyncMsg((uint8_t *)&packet);
    //
    // DM_can_set(1, &packet, ENABLE, 0, 0, 0, 0, 0);
    // EcatSyncMsg((uint8_t *)&packet);
    //
    // int tag = 0;

    //-----END-----//

    

    for (;;)
    {
        printf("start\n");
        //-------------//
        // DM4310 demo //
        //-------------//

        // if (tag == 0)
        //{
        //     DM_can_set(1, &packet, MIT_CTRL, -90, 0, 1, 0.5, 0);
        //     EcatSyncMsg((uint8_t *)&packet);
        //     tag = 1;
        // }
        // else
        //{
        //     DM_can_set(1, &packet, MIT_CTRL, 90, 0, 1, 0.5, 0);
        //     EcatSyncMsg((uint8_t *)&packet);
        //     tag = 0;
        // }

        // usleep(5*1000*1000);//延时5秒

        //-------------//
        // GM6020 demo //
        //-------------//

        GM6020_can_set(2, &packet, GM6020_TAG1, 20000, 20000, 0, 0);
        packet.LED = 0x0F;
        EcatSyncMsg((uint8_t *)&packet);

        GM6020_can_set(1, &packet, GM6020_TAG1, 20000, 20000, 0, 0);
        packet.LED = 0x0F;
        EcatSyncMsg((uint8_t *)&packet);

        //-----END-----//

        if (app_stopped)
        {
            break;
        }
    }

    //-------------//
    // DM4310 demo //
    //-------------//

    /*
    DM_can_set(1, &packet, DISABLE, 0, 0, 0, 0, 0);
    EcatSyncMsg((uint8_t *)&packet);
    */

    
    // 
    Stop_all(&packet);
    EcatSyncMsg((uint8_t *)&packet);

    EcatStop();
    printf("stop");

    return 0;
}