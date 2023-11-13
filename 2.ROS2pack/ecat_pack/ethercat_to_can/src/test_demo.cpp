#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "soem_ros2/soem.h"

#define EC_TIMEOUTMON 500

char IOmap[4096];
OSAL_THREAD_HANDLE thread1;
int expectedWKC;
boolean needlf;
volatile int wkc;
boolean inOP;
uint8 currentgroup = 0;

void simpletest(char *ifname)
{
        int i, oloop, iloop, chk;
        needlf = FALSE;
        inOP = FALSE;

        printf("Starting simple test\n");

        /* initialise SOEM, bind socket to ifname */
        if (ec_init(ifname))
        {
                printf("ec_init on %s succeeded.\n", ifname);
                /* find and auto-config slaves */

                if (ec_config_init(FALSE) > 0)
                {
                        printf("%d slaves found and configured.\n", ec_slavecount);

                        ec_config_map(&IOmap);

                        ec_configdc();

                        printf("Slaves mapped, state to SAFE_OP.\n");
                        /* wait for all slaves to reach SAFE_OP state */
                        ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 4);

                        oloop = ec_slave[0].Obytes;
                        if ((oloop == 0) && (ec_slave[0].Obits > 0))
                                oloop = 1;
                        if (oloop > 8)
                                oloop = 8;
                        iloop = ec_slave[0].Ibytes;
                        if ((iloop == 0) && (ec_slave[0].Ibits > 0))
                                iloop = 1;
                        if (iloop > 8)
                                iloop = 8;

                        printf("segments : %d : %d %d %d %d\n", ec_group[0].nsegments, ec_group[0].IOsegment[0], ec_group[0].IOsegment[1], ec_group[0].IOsegment[2], ec_group[0].IOsegment[3]);

                        printf("Request operational state for all slaves\n");
                        expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
                        printf("Calculated workcounter %d\n", expectedWKC);
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
                        if (ec_slave[0].state == EC_STATE_OPERATIONAL)
                        {
                                printf("Operational state reached for all slaves.\n");
                                inOP = TRUE;
                                int led_tag = 0;
                                int led_loop = 0;
                                /* cyclic loop */
                                for (i = 1; i <= 10000; i++)
                                {

                                        ec_slave[0].outputs[0x0000] = 0x01;
                                        ec_slave[0].outputs[0x0001] = 0x00;
                                        ec_slave[0].outputs[0x0002] = 0x00;
                                        ec_slave[0].outputs[0x0003] = 0x00;
                                        ec_slave[0].outputs[0x0004] = 0x00;
                                        ec_slave[0].outputs[0x0005] = 0x00;
                                        ec_slave[0].outputs[0x0006] = 0x00;
                                        ec_slave[0].outputs[0x0007] = 0x08;
                                        ec_slave[0].outputs[0x0008] = 0xFF;
                                        ec_slave[0].outputs[0x0009] = 0xFF;
                                        ec_slave[0].outputs[0x000A] = 0xFF;
                                        ec_slave[0].outputs[0x000B] = 0xFF;
                                        ec_slave[0].outputs[0x000C] = 0xFF;
                                        ec_slave[0].outputs[0x000D] = 0xFF;
                                        ec_slave[0].outputs[0x000E] = 0xFF;
                                        ec_slave[0].outputs[0x000F] = 0xFC;

                                        ec_slave[0].outputs[0x0010] = led_tag;

                                        led_loop++;

                                        if (led_loop == 100)
                                        {
                                                led_loop=0;
                                                led_tag++;
                                        }

                                        if (led_tag == 7)
                                        {
                                                led_tag = 0;
                                        }

                                        ec_send_processdata();

                                        wkc = ec_receive_processdata(EC_TIMEOUTRET);

                                        if (wkc >= expectedWKC)
                                        {
                                                needlf = TRUE;
                                        }
                                        osal_usleep(500);
                                }
                                inOP = FALSE;
                        }
                        else
                        {
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
                        printf("\nRequest init state for all slaves\n");
                        ec_slave[0].state = EC_STATE_INIT;
                        /* request INIT state for all slaves */
                        ec_writestate(0);
                }
                else
                {
                        printf("No slaves found!\n");
                }
                printf("End simple test, close socket\n");
                /* stop SOEM, close socket */
                ec_close();
        }
        else
        {
                printf("No socket connection on %s\nExcecute as root\n", ifname);
        }
}

int main()
{
    char phy[] = "enp4s0";
    simpletest(phy);
    return 0;
}