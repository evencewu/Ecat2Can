#include "soem_ros2/soem.h"
#include <cstdio>

#define PDO_OUTPUT_BYTE 33
#define PDO_INPUT_BYTE 32

#define EC_TIMEOUTMON 500

#define EC_VER1

char IOmap[4096];

int main()
{
    char ifname[] = "enp4s0";
    if (ec_init(ifname))
    {
        printf("ec_init on %s succeeded.\n", ifname);
        if (ec_config_init(FALSE) > 0)
        {
            printf("%d slaves found and configured.\n", ec_slavecount);

            ec_config_map(&IOmap);

            ec_configdc();

            ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 4);

            printf("Request operational state for all slaves\n");
        }
        else
        {
            printf("No slaves found!\n");
        }
    }
    else
    {
        printf("No socket connection on %s\nExcecute as root\n", ifname);
    }
}