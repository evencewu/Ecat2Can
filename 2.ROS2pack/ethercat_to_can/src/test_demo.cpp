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

        memset(&packet, 0, sizeof(Ecat2Can_Pack));
        EcatSyncMsg((uint8_t *)&packet);
        
        EcatStop();
        printf("stop");
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

    int vol = 2000;
    //int vol_tag = 1;

    for (;;)
    {
        //vol += vol_tag * 1;
        GM6020_can_set(1, &packet, GM6020_TAG1, vol, 0, 0, 0);
        packet.LED = 0x0F;
        EcatSyncMsg((uint8_t *)&packet);

        // if(vol == 10000)
        //{
        //     vol_tag = -1;
        // }

        // if(vol_tag == -10000)
        //{
        //     vol_tag = 1;
        // }
        if (app_stopped)
        {
            break;
        }
    }

    EcatStop();
    return 0;
}