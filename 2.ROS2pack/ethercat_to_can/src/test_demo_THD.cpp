
#include "ecat_can_base/ecat_base.h"
#include "ecat_can_base/ecat_typedef.h"
#include "ecat_motor_dlc/ecat_DM4310.h"
#include "ecat_motor_dlc/ecat_GM6020.h"

#include <array>
#include <signal.h>
#include <unistd.h>

bool app_stopped = false;
Ecat2Can_Outputs_Pack packet_tx;
Ecat2Can_Inputs_Pack packet_rx;

void sigint_handler(int sig);
void save_stop();

ecat::EcatBase Ethercat;

std::array<ecat::GM6020dlc,2> Motor6020 = {
    ecat::GM6020dlc(CAN1, 2),
    ecat::GM6020dlc(CAN1, 3)};

int main()
{
    signal(SIGINT, sigint_handler);

    char phy[] = "enp5s0";

    Ethercat.EcatStart(phy);
    Ethercat.SetUserStop(save_stop);

    memset(&packet_tx, 0, sizeof(Ecat2Can_Outputs_Pack));
    memset(&packet_rx, 0, sizeof(Ecat2Can_Inputs_Pack));

    // EcatSyncMsg((uint8_t *)&packet_tx, (uint8_t *)&packet_rx);

    printf("start\n");

    float kp = 5;
    float kd = 10;
    float I_1 = 0;
    for (int i = 0; i < 10000000; i++)
    {
        // printf("loop\n");
        packet_tx.LED = 0x03;

        Motor6020[0].GM6020_can_set(&packet_tx, I_1);
        Motor6020[1].GM6020_can_set(&packet_tx, 0);
        Ethercat.EcatSyncMsg((uint8_t *)&packet_tx, (uint8_t *)&packet_rx);
        Motor6020[0].GM6020_can_analyze(&packet_rx);
        Motor6020[1].GM6020_can_analyze(&packet_rx);

        I_1 = (Motor6020[0].pos - Motor6020[1].pos) * kp - (Motor6020[0].pos - Motor6020[1].pos)*kd;

        // GM6020_can_analyze(CAN1, &packet_rx, &tag, &pos, &vec, &toq, &temp);

        printf("%f\n", I_1);
        printf("%d\n", Motor6020[0].pos);
        printf("%d\n", Motor6020[1].pos);

        // if (tag == 518)
        //{
        //     printf("%d\n", vec);
        //     printf("%d\n", pos);
        // }

        if (app_stopped)
        {
            break;
        }
    }

    Ethercat.EcatStop();

    printf("stop");

    return 0;
}

void sigint_handler(int sig)
{
    if (sig == SIGINT)
    {
        app_stopped = true;
    }
}

void save_stop()
{
    Motor6020[0].GM6020_can_set(&packet_tx, 0);
    Motor6020[1].GM6020_can_set(&packet_tx, 0);
    Ethercat.EcatSyncMsg((uint8_t *)&packet_tx, (uint8_t *)&packet_rx);
    Ethercat.EcatSyncMsg((uint8_t *)&packet_tx, (uint8_t *)&packet_rx);
    Ethercat.EcatSyncMsg((uint8_t *)&packet_tx, (uint8_t *)&packet_rx);
    printf("stop motor!");
}