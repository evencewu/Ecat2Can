#ifndef __ECAT_BASE__
#define __ECAT_BASE__

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "soem_ros2/soem.h"

void EcatStart(char *ifname);
void EcatSyncMsg(uint8_t *data);
void EcatStop();

#define EC_TIMEOUTMON 500

char IOmap[4096];
volatile int wkc;

//
#include "ethercat_to_can/ecat_typedef.h"

void EcatStart(char *ifname)
{
    int chk;
    if (ec_init(ifname))
    {
        printf("ec_init on %s succeeded.\n", ifname);
        if (ec_config_init(FALSE) > 0)
        {
            printf("%d slaves found and configured.\n", ec_slavecount);

            ec_config_map(&IOmap);

            ec_configdc();

            printf("Slaves mapped, state to SAFE_OP.\n");
            /* wait for all slaves to reach SAFE_OP state */
            ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 4);

            printf("Request operational state for all slaves\n");

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

void EcatSyncMsg(uint8_t *data)
{
    if (ec_slave[0].state == EC_STATE_OPERATIONAL)
    {
        printf("Operational state reached for all slaves.\n");

        ec_slave[0].outputs[0x0000] = *data;
        ec_slave[0].outputs[0x0001] = *(data+1);
        ec_slave[0].outputs[0x0002] = *(data+2);
        ec_slave[0].outputs[0x0003] = *(data+3);
        ec_slave[0].outputs[0x0004] = *(data+4);
        ec_slave[0].outputs[0x0005] = *(data+5);
        ec_slave[0].outputs[0x0006] = *(data+6);
        ec_slave[0].outputs[0x0007] = *(data+7);
        ec_slave[0].outputs[0x0008] = *(data+8);
        ec_slave[0].outputs[0x0009] = *(data+9);
        ec_slave[0].outputs[0x000A] = *(data+10);
        ec_slave[0].outputs[0x000B] = *(data+11);
        ec_slave[0].outputs[0x000C] = *(data+12);
        ec_slave[0].outputs[0x000D] = *(data+13);
        ec_slave[0].outputs[0x000E] = *(data+14);
        ec_slave[0].outputs[0x000F] = *(data+15);

        ec_slave[0].outputs[0x0010] = 0x0F;

        ec_send_processdata();

        ec_receive_processdata(EC_TIMEOUTRET);

        osal_usleep(500);
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

#endif