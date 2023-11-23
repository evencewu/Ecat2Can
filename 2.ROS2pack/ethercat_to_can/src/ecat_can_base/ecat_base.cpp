#include "ecat_can_base/ecat_base.h"

#include <unistd.h>
#define EC_VER1

char IOmap[4096];
volatile int wkc;
int expectedWKC;

void EcatStart(char *ifname)
{
    int chk;
    int oloop,iloop;
    if (ec_init(ifname))
    {
        printf("ec_init on %s succeeded.\n", ifname);
        if (ec_config_init(FALSE) > 0)
        {
            printf("%d slaves found and configured.\n", ec_slavecount);

            ec_config_map(&IOmap);

            printf("Slaves mapped, state to SAFE_OP.\n");
            /* wait for all slaves to reach SAFE_OP state */
            ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 4);

            ec_configdc();

            ec_dcsync0(1, TRUE, 10000, 250000);
            //
            sleep(1);

            printf("Request operational state for all slaves\n");

            // expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
            ec_slave[0].state = EC_STATE_OPERATIONAL;
            /* send one valid process data to make outputs in slaves happy*/

            ec_send_processdata();
            ec_receive_processdata(EC_TIMEOUTRET);
            /* request OP state for all slaves */
            ec_writestate(0);

            chk = 200;
            /* wait for all slaves to reach OP state */
            do
            {
                ec_send_processdata();
                ec_receive_processdata(EC_TIMEOUTRET);
                ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
            } while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
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

void EcatSyncMsg(uint8_t *output_data, uint8_t *input_data)
{
    if (ec_slave[0].state == EC_STATE_OPERATIONAL)
    {
        // printf("Operational state reached for all slaves.\n");

        for (int i = 0; i < PDO_OUTPUT_BYTE; i++)
        {
            ec_slave[0].outputs[i] = *(output_data + i);
        }

        ec_send_processdata();

        wkc = ec_receive_processdata(EC_TIMEOUTRET);

        if (wkc >= expectedWKC)
        {
            for (int i = 0; i < PDO_INPUT_BYTE; i++)
            {
                *(input_data + i) = ec_slave[0].inputs[i];
            }
        }

        osal_usleep(1000);
    }
    else
    {
        int i = 0;
        printf("Not all slaves reached operational state.\n");
        ec_readstate();
        for (i = 1; i <= ec_slavecount; i++)
        {
            if (ec_slave[i].state != EC_STATE_OPERATIONAL)
            {
                printf("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                       i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
            }
        }
    }
}

void EcatStop()
{
    printf("\nRequest init state for all slaves\n");
    ec_slave[0].state = EC_STATE_INIT;
    /* request INIT state for all slaves */
    ec_writestate(0);
}